#include "FileWatcher.h"

#include "Foundation/Log.h"

#include "Platform/Windows/Windows.h"
#include "Path.h"

using namespace Helium;

void EmitLastError()
{
    DWORD error = GetLastError();
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    Log::Error( TXT( "%s\n" ), lpMsgBuf );

    LocalFree(lpMsgBuf);
}

FileWatcher::FileWatcher()
{
}

FileWatcher::~FileWatcher()
{
}

bool FileWatcher::Add( const tstring& path, FileChangedSignature::Delegate& listener, bool watchSubtree )
{
    M_PathToFileWatch::const_iterator itr = m_Watches.find( path );

    if ( itr == m_Watches.end() )
    {

        m_Watches[ path ].m_Path.Set( path );
        m_Watches[ path ].m_ChangeHandle = FindFirstChangeNotification( 
            m_Watches[ path ].m_Path.c_str(),
            watchSubtree,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE ); // watch for writes


        if ( m_Watches[ path ].m_ChangeHandle == NULL || m_Watches[ path ].m_ChangeHandle == INVALID_HANDLE_VALUE )
        {
            EmitLastError();

            m_Watches.erase( path );
            return false;
        }
    }

    m_Watches[ path ].m_Event.Add( listener );

    return true;
}

bool FileWatcher::Remove( const tstring& path, FileChangedSignature::Delegate& listener )
{
    m_Watches[ path ].m_Event.Remove( listener );

    if ( m_Watches[ path ].m_Event.Count() == 0 )
    {
        FindCloseChangeNotification( m_Watches[ path ].m_ChangeHandle );
        m_Watches.erase( path );
    }

    return true;
}

bool FileWatcher::Watch( int timeout )
{
    HANDLE changeHandles[ MAXIMUM_WAIT_OBJECTS ];
    //    FILE_NOTIFY_INFORMATION notificationInformation[1024];
    FileWatch* watches[ MAXIMUM_WAIT_OBJECTS ];

    for ( u32 i = 0; i < MAXIMUM_WAIT_OBJECTS; ++i )
    {
        changeHandles[ i ] = NULL;
        watches[ i ] = NULL;
    }

    u32 handleIndex = 0;
    for ( M_PathToFileWatch::iterator itr = m_Watches.begin(), end = m_Watches.end(); itr != end; ++itr )
    {
        changeHandles[ handleIndex ] = (*itr).second.m_ChangeHandle;
        watches[ handleIndex ] = &( (*itr).second );
        ++handleIndex;
    }

    if ( handleIndex == 0 )
    {
        // nothing to watch
        return true;
    }

    DWORD changedObject = WaitForMultipleObjects( handleIndex, changeHandles, FALSE, timeout );
    if ( changedObject == WAIT_TIMEOUT )
    {
        return true;
    }

    if ( changedObject >= MAXIMUM_WAIT_OBJECTS )
    {
        EmitLastError();
        return false;
    }

    FileWatch* watch = watches[ changedObject ];

    //memset( notificationInformation, 0, sizeof( notificationInformation ) );
    //LPDWORD bytesReturned = 0;
    //ReadDirectoryChangesW( handleIndex, &notificationInformation, sizeof(notificationInformation), watch->m_WatchSubtree, FILE_NOTIFY_ALL, &bytesReturned, NULL, NULL );  

    FileChangedArgs changeEvent( watch->m_Path.c_str(), FileOperations::Unknown );
    watch->m_Event.Raise( changeEvent );

    if ( FindNextChangeNotification( changeHandles[ changedObject ] ) == FALSE )
    {
        EmitLastError();
        return false;
    }

    return true;
}
