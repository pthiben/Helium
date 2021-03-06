#pragma once

#include <string>
#include <map>

#include "Application/Inspect/Data/Data.h"

#include "Core/API.h"
#include "Core/Scene/Selection.h"

namespace Helium
{
    namespace Core
    {
        class HierarchyNode;

        namespace ManipulatorModes
        {
            enum ManipulatorMode
            {
                Scale,
                ScalePivot,

                Rotate,
                RotatePivot,

                Translate,
                TranslatePivot,
            };
        }

        typedef ManipulatorModes::ManipulatorMode ManipulatorMode;

        namespace ManipulatorSpaces
        {
            enum ManipulatorSpace
            {
                Object,
                Local,
                World,
            };
            static void ManipulatorSpaceEnumerateEnum( Reflect::Enumeration* info )
            {
                info->AddElement(Object, TXT( "Object" ) );
                info->AddElement(Local, TXT( "Local" ) );
                info->AddElement(World, TXT( "World" ) );
            }
        }

        typedef ManipulatorSpaces::ManipulatorSpace ManipulatorSpace;

        namespace ManipulatorAdapterTypes
        {
            enum ManipulatorAdapterType
            {
                ManiuplatorAdapterCollection,
                ScaleManipulatorAdapter,
                RotateManipulatorAdapter,
                TranslateManipulatorAdapter,
            };
        }

        typedef ManipulatorAdapterTypes::ManipulatorAdapterType ManipulatorAdapterType;

        class CORE_API ManipulatorAdapter : public Helium::RefCountBase<ManipulatorAdapter>
        {
        public:
            const static ManipulatorAdapterType Type = ManipulatorAdapterTypes::ManiuplatorAdapterCollection;

            ManipulatorAdapter()
            {

            }

            virtual ManipulatorAdapterType GetType() = 0;
            virtual Core::HierarchyNode* GetNode() = 0;
            virtual bool AllowSelfSnap()
            {
                return false;
            }

            virtual Math::Matrix4 GetFrame(ManipulatorSpace space) = 0;
            virtual Math::Matrix4 GetObjectMatrix() = 0;
            virtual Math::Matrix4 GetParentMatrix() = 0;
        };

        typedef Helium::SmartPtr<ManipulatorAdapter> ManipulatorAdapterPtr;
        typedef std::vector<ManipulatorAdapterPtr> V_ManipulatorAdapterSmartPtr;

        class CORE_API ScaleManipulatorAdapter : public ManipulatorAdapter
        {
        public:
            const static ManipulatorAdapterType Type = ManipulatorAdapterTypes::ScaleManipulatorAdapter;

            virtual ManipulatorAdapterType GetType() HELIUM_OVERRIDE
            {
                return ManipulatorAdapterTypes::ScaleManipulatorAdapter;
            }

            virtual Math::Vector3 GetPivot() = 0;

            virtual Math::Scale GetValue() = 0;

            virtual Undo::CommandPtr SetValue(const Math::Scale& v) = 0;
        };

        class CORE_API RotateManipulatorAdapter : public ManipulatorAdapter
        {
        public:
            const static ManipulatorAdapterType Type = ManipulatorAdapterTypes::RotateManipulatorAdapter;

            virtual ManipulatorAdapterType GetType() HELIUM_OVERRIDE
            {
                return ManipulatorAdapterTypes::RotateManipulatorAdapter;
            }

            virtual Math::Vector3 GetPivot() = 0;

            virtual Math::EulerAngles GetValue() = 0;

            virtual Undo::CommandPtr SetValue(const Math::EulerAngles& v) = 0;
        };

        class CORE_API TranslateManipulatorAdapter : public ManipulatorAdapter
        {
        public:
            const static ManipulatorAdapterType Type = ManipulatorAdapterTypes::TranslateManipulatorAdapter;

            virtual ManipulatorAdapterType GetType() HELIUM_OVERRIDE
            {
                return ManipulatorAdapterTypes::TranslateManipulatorAdapter;
            }

            virtual Math::Vector3 GetPivot() = 0;

            virtual Math::Vector3 GetValue() = 0;

            virtual Undo::CommandPtr SetValue(const Math::Vector3& v) = 0;
        };

        class CORE_API ManiuplatorAdapterCollection HELIUM_ABSTRACT
        {
        protected:
            V_ManipulatorAdapterSmartPtr m_ManipulatorAdapters;

        public:
            virtual ManipulatorMode GetMode() = 0;

            virtual void AddManipulatorAdapter(const ManipulatorAdapterPtr& accessor)
            {
                m_ManipulatorAdapters.push_back(accessor);
            }
        };
    }
}