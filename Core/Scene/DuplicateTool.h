#pragma once

#include "Foundation/TUID.h"

#include "Core/API.h"
#include "Core/Scene/Volume.h"
#include "Core/Scene/CreateTool.h"

namespace Helium
{
    namespace Core
    {
        class CORE_API DuplicateTool : public Core::CreateTool
        {
            //
            // Members
            //

        private:
            Core::Transform* m_Source;

            //
            // RTTI
            //

        public:
            SCENE_DECLARE_TYPE(Core::DuplicateTool, Core::CreateTool);
            static void InitializeType();
            static void CleanupType();

        public:
            DuplicateTool(Core::Scene* scene, PropertiesGenerator* generator);

            virtual ~DuplicateTool();

            virtual Core::TransformPtr CreateNode() HELIUM_OVERRIDE;

            virtual void CreateProperties() HELIUM_OVERRIDE;

            int GetVolumeShape() const;

            void SetVolumeShape(int value);
        };
    }
}