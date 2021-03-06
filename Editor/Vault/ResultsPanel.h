#pragma once

#include "VaultMenuIDs.h"
#include "VaultGenerated.h"
#include "VaultFrame.h"
#include "VaultSearchPanel.h"
#include "Foundation/Memory/SmartPtr.h"

namespace Helium
{
    namespace Asset
    {
        class AssetFile;
        class AssetFile;
        typedef Helium::SmartPtr< AssetFile > AssetFilePtr;
        typedef std::vector< AssetFilePtr > V_AssetFiles;

        class AssetFolder;
        typedef Helium::SmartPtr< AssetFolder > AssetFolderPtr;
        typedef std::vector< AssetFolderPtr > V_AssetFolders;
    }

    namespace Editor
    {
        //
        // Forwards
        //
        class ThumbnailView;
        struct ThumbnailSelectionArgs;
        struct ThumbnailHighlightArgs;

        class SearchResults;
        typedef Helium::SmartPtr< SearchResults > SearchResultsPtr;

        namespace ViewModes
        {
            enum ViewMode
            {
                Invalid,   // Do not use, just for initialization

                Thumbnail,
                AdvancedSearch,
            };
        }
        typedef ViewModes::ViewMode ViewMode;

        // Selection change event signature and arguments
        struct ResultChangeArgs
        {
            size_t m_NumSelected;
            tstring m_HighlightPath;

            ResultChangeArgs( size_t numSelected, const tstring& highlight )
                : m_NumSelected( numSelected )
                , m_HighlightPath( highlight )
            {
            }

            ResultChangeArgs()
                : m_NumSelected( 0 )
                , m_HighlightPath( TXT( "" ) )
            {
            }
        };
        typedef Helium::Signature< void, const ResultChangeArgs& > ResultSignature;

        // Highlight change event signature and arguments
        struct ResultHighlightChangeArgs
        {
            tstring m_HighlightPath;

            ResultHighlightChangeArgs( const tstring& path )
                : m_HighlightPath( path )
            {
            }
        };
        typedef Helium::Signature< void, const ResultHighlightChangeArgs& > ResultHighlightSignature;

        ///////////////////////////////////////////////////////////////////////////////
        /// Class ResultsPanel
        ///////////////////////////////////////////////////////////////////////////////
        class ResultsPanel : public ResultsPanelGenerated 
        {		
        public:
            ResultsPanel( const tstring& rootDirectory, VaultFrame* browserFrame );
            virtual ~ResultsPanel();

            void SetViewMode( ViewMode view );
            ViewMode GetViewMode() const;

            void SetThumbnailSize( u16 zoom );
            void SetThumbnailSize( ThumbnailSize zoom );

            void SetResults( SearchResults* results );
            void ClearResults();

            void SelectPath( const tstring& path );
            u32 GetSelectedPaths( std::set< Helium::Path >& paths );

            u32 GetNumFiles() const;
            u32 GetNumFolders() const;

        private:
            void OnThumbnailSelectionChanged( const ThumbnailSelectionArgs& args );
            void OnThumbnailHighlightChanged( const ThumbnailHighlightArgs& args );

        private:
            tstring m_RootDirectory;
            ViewMode m_CurrentMode;
            wxWindow* m_CurrentView;
            ThumbnailView* m_ThumbnailView;
            VaultSearchPanel* m_VaultSearchPanel;
            VaultFrame* m_VaultFrame;

        private:
            ResultSignature::Event m_ResultsChanged;
        public:
            void AddResultsChangedListener( const ResultSignature::Delegate& listener )
            {
                m_ResultsChanged.Add( listener );
            }
            void RemoveResultsChangedListener( const ResultSignature::Delegate& listener )
            {
                m_ResultsChanged.Remove( listener );
            }

        private:
            DECLARE_EVENT_TABLE();
        };
    }
}