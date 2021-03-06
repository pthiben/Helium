#pragma once

#include "Core/API.h"
#include "Foundation/Component/Component.h"

namespace Helium
{
    namespace Asset
    {
        class CORE_API TransformComponent : public Component::ComponentBase
        {
        private:
            Math::Vector3 m_Translation;
            Math::Vector3 m_Rotation;
            Math::Vector3 m_Scale;

        public:

            REFLECT_DECLARE_CLASS( TransformComponent, Component::ComponentBase );

            static void EnumerateClass( Reflect::Compositor< TransformComponent >& comp );

        public:
            TransformComponent()
            {
            }

            virtual ~TransformComponent()
            {
            }

        public:
            virtual Component::ComponentUsage GetComponentUsage() const HELIUM_OVERRIDE;

        public:

            const Math::Vector3& GetTranslation() const;
            const Math::Vector3& GetRotation() const;
            const Math::Vector3& GetScale() const;

            void SetTranslation( const Math::Vector3& translation );
            void SetRotation( const Math::Vector3& rotation );
            void SetScale( const Math::Vector3& scale );

        };
    }
}