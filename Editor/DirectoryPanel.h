#pragma once

#include "Editor/EditorGenerated.h"

#include "Core/Scene/SceneManager.h"
#include "Editor/EntityAssetOutliner.h"
#include "Editor/HierarchyOutliner.h"
#include "Editor/NodeTypeOutliner.h"
#include "Editor/TreeMonitor.h"

namespace Helium
{
    namespace Editor
    {
        class DirectoryPanel : public DirectoryPanelGenerated
        {
        public:
            DirectoryPanel( Core::SceneManager* manager, TreeMonitor* treeMonitor, wxWindow* parent = NULL, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
            virtual ~DirectoryPanel();

            void SaveState( SceneOutlinerState& hierarchyState, SceneOutlinerState& entityState, SceneOutlinerState& typesState );
            void RestoreState( SceneOutlinerState& hierarchyState, SceneOutlinerState& entityState, SceneOutlinerState& typesState );

        private:
            Core::SceneManager*  m_SceneManager;
            TreeMonitor*         m_TreeMonitor;
            EntityAssetOutliner* m_EntityOutline;
            HierarchyOutliner*   m_HierarchyOutline;
            NodeTypeOutliner*    m_TypeOutline;
        };
    }
}