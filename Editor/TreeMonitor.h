#pragma once

#include <set>

#include "Platform/Types.h"

#include "Foundation/TimerThread.h"

#include "Core/Scene/SceneManager.h"

#include "Editor/Controls/Tree/SortTreeCtrl.h"

namespace Helium
{
    namespace Editor
    {

        /////////////////////////////////////////////////////////////////////////////
        // This class allows you to add a number of tree controls to it, and it will
        // manage whether or not to enable sorting in those tree controls.  This 
        // is so that during operations that would normally cause multiple sorting
        // calls on the tree (such as renaming multiple nodes at a time) you can
        // freeze tree sorting until all the nodes are renamed, then just do one sort
        // at the end.
        // 
        class TreeMonitor
        {
        private:
            typedef std::set< SortTreeCtrl* > S_Trees;

            Core::SceneManager* m_SceneManager;
            S_Trees m_Trees;

            u32 m_FreezeTreeSorting;
            bool m_NeedsSorting;
            TimerThread m_ThawTimer;
            Timer m_SceneChangedTimer;
            bool m_SelfFrozen;

        public:
            TreeMonitor( Core::SceneManager* sceneManager );
            virtual ~TreeMonitor();

            void AddTree( SortTreeCtrl* tree );
            void RemoveTree( SortTreeCtrl* tree );
            void ClearTrees();

            void FreezeSorting();
            void ThawSorting();
            bool IsFrozen() const;

        private:
            void OnSceneAdded( const Core::SceneChangeArgs& args );
            void OnSceneRemoving( const Core::SceneChangeArgs& args );
            void OnNodeAdded( const Core::NodeChangeArgs& args );
            void OnNodeRemoved( const Core::NodeChangeArgs& args );
            void OnNodeRenamed( const Core::SceneNodeChangeArgs& args );
            void OnThawTimer( const TimerTickArgs& args );
        };
    }
}