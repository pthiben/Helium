#pragma once

#include "Core/API.h"
#include "Application/Inspect/Data/Data.h"
#include "Application/Inspect/Controls/InspectPanel.h"
#include "Core/Scene/SceneNode.h"

namespace Helium
{
    namespace Core
    {
        class SceneNodePanel : public Inspect::Panel
        {
        public:
            SceneNodePanel(PropertiesGenerator* generator, const OS_SelectableDumbPtr& selection);
            virtual ~SceneNodePanel();
            virtual void Create() HELIUM_OVERRIDE;

        protected:
            PropertiesGenerator*          m_Generator;
            OS_SelectableDumbPtr  m_Selection;
        };
    }
}