#pragma once

#include "VaultGenerated.h"
#include "VaultFrame.h"
#include "SearchHistory.h"

namespace Helium
{
    namespace Editor
    {

        ///////////////////////////////////////////////////////////////////////////////
        /// Class NavigationPanel
        ///////////////////////////////////////////////////////////////////////////////
        class NavigationPanel : public NavigationPanelGenerated 
        {
        public:
            NavigationPanel( VaultFrame* browserFrame, SearchHistory* history );
            ~NavigationPanel();

            wxString GetNavBarValue() const;
            void SetNavBarValue( const tstring& navBarText, bool isFolder = false );

        protected:
            // Virtual event handlers, overide them in your derived class
            virtual void OnBackButtonClick( wxCommandEvent& event ) HELIUM_OVERRIDE;
            virtual void OnForwardButtonClick( wxCommandEvent& event ) HELIUM_OVERRIDE;
            virtual void OnUpFolderButtonClick( wxCommandEvent& event ) HELIUM_OVERRIDE;
            virtual void OnNavBarComboBox( wxCommandEvent& event ) HELIUM_OVERRIDE;
            virtual void OnNavBarComboBoxMouseWheel( wxMouseEvent& event ) HELIUM_OVERRIDE;
            virtual void OnNavBarText( wxCommandEvent& event ) HELIUM_OVERRIDE;
            virtual void OnNavBarTextEnter( wxCommandEvent& event ) HELIUM_OVERRIDE;
            virtual void OnGoButtonClick( wxCommandEvent& event ) HELIUM_OVERRIDE;
            virtual void OnOptionsButtonClick( wxCommandEvent& event ) HELIUM_OVERRIDE;

            virtual void OnBackMenuSelect( wxCommandEvent& event );
            virtual void OnForwardMenuSelect( wxCommandEvent& event );

            // Event Listener Callbacks
            void OnFwdBackButtonMenuOpen( wxMenuEvent& event );
            void OnSearchHistoryChanged( const Editor::SearchHistoryChangeArgs& args );
            void OnMRUQueriesChanged( const Editor::MRUQueriesChangedArgs& args );

        private:
            typedef std::map< int, int > M_HistoryIndexMap;
            M_HistoryIndexMap m_HistoryIndexMap;

            VaultFrame*     m_VaultFrame;
            SearchHistory*    m_SearchHistory;

            bool m_NavBarIsFolder;
            bool m_IgnoreNavBarTextChanged;

            void UpdateHistoryMenus();
            void UpdateNavBarMRU( const OS_SearchQuery& mruQueries );
        };
    }
}