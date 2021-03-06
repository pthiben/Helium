#pragma once

#include "CollectionManager.h"
#include "VaultSettings.h"
#include "VaultSearch.h"
#include "VaultEvents.h"
#include "SearchQuery.h"
#include "SearchResults.h"
#include "SearchHistory.h"
#include "Foundation/Memory/SmartPtr.h"
#include "Platform/Types.h"
#include "Application/DocumentManager.h"

namespace Helium
{
    //
    // Forwards
    //
    namespace Asset
    {
        class SearchResults;
        typedef Helium::SmartPtr< SearchResults > SearchResultsPtr;
    }

    namespace Editor
    {
        //
        // Forwards
        //
        class VaultFrame;

        class Vault
        {
        public:
            Vault();
            Vault( const Vault& rhs ) {}
            Vault& operator=( const Vault& rhs ) {}

        public:
            ~Vault();

            void SetRootDirectory( const Helium::Path& path )
            {
                m_RootDirectory = path;
                if ( m_VaultSearch )
                {
                    m_VaultSearch->SetRootDirectory( path );
                }
            }

            void ShowVault( const tstring& queryString = TXT("") );
            bool HasFrame();

            friend class VaultFrame;

        protected:
            void OnCloseVault();

        private:
            Helium::Path         m_RootDirectory;
            VaultSearchPtr          m_VaultSearch;
            VaultFrame*             m_VaultFrame;
            bool                    m_HasFrame;
            SearchHistory*          m_SearchHistory;
            CollectionManager*      m_CollectionManager;
        };
    } // namespace Editor
}