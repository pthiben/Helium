#pragma once

#include "Foundation/TUID.h"

#include "Application/Inspect/DragDrop/FilteredDropTarget.h"

#include "Core/API.h"
#include "Core/Scene/CreateTool.h"
#include "Core/Scene/EntityInstance.h"

namespace Helium
{
    namespace Core
    {
        class EntityRowInfo
        {
        public:
            tstring m_OriginalValue;
            tstring m_Name;
            Helium::Path m_ClassPath;
            float m_Probability;

            tstring GetListName() const;
        };

        typedef std::vector< EntityRowInfo > V_EntityRowInfo;

        class CORE_API EntityInstanceCreateTool : public Core::CreateTool
        {
            //
            // Members
            //

        public:
            Helium::Path m_ClassPath;
            static bool s_PointerVisible;
            static bool s_BoundsVisible;
            static bool s_GeometryVisible;

            //
            // RTTI
            //

            SCENE_DECLARE_TYPE(Core::EntityInstanceCreateTool, Core::CreateTool);
            static void InitializeType();
            static void CleanupType();

        protected:
            Inspect::List* m_RandomEntityList;
            V_EntityRowInfo m_RandomEntityInfo;
            static std::vector< tstring > s_RandomEntities;

            Inspect::FileDialogButton* m_FileButton;
            Inspect::FileBrowserButton* m_BrowserButton;

            Inspect::FileDialogButton* m_FileButtonAdd;
            Inspect::FileBrowserButton* m_BrowserButtonAdd;

        public:
            EntityInstanceCreateTool(Core::Scene* scene, PropertiesGenerator* generator);
            virtual ~EntityInstanceCreateTool();

            virtual Core::TransformPtr CreateNode() HELIUM_OVERRIDE;
            virtual void CreateProperties() HELIUM_OVERRIDE;

            tstring GetEntityAsset() const;
            void SetEntityAsset(const tstring& value);
            void AddEntityAsset(const tstring& value);

            bool GetPointerVisible() const;
            void SetPointerVisible(bool show);
            bool GetBoundsVisible() const;
            void SetBoundsVisible(bool show);
            bool GetGeometryVisible() const;
            void SetGeometryVisible(bool show);
            tstring GetRandomEntity() const;
            void SetRandomEntity( const tstring& entityName );

            void OnDeleteClass( Inspect::Button* button );
            void OnClear( Inspect::Button* button );
            void OnNormalize( Inspect::Button* button );
            void OnModify( Inspect::Button* button );

            void OnEntityDropped( const Inspect::FilteredDropTargetArgs& args );
            void DropEntities( const std::vector< tstring >& entities, bool appendToList );
        };
    }
}