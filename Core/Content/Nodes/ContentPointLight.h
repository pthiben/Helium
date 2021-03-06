#pragma once

#include "Core/API.h"
#include "Core/Content/Nodes/ContentLight.h"

namespace Helium
{
    namespace Content
    {
        class CORE_API PointLight : public Light
        {
        public:
            virtual void Host(ContentVisitor* visitor); 



            PointLight ()
                : m_InnerRadius( 0.0f )
                , m_OuterRadius( 1.0f )
            {
            }

            PointLight (Helium::TUID &id)
                : m_InnerRadius( 0.0f )
                , m_OuterRadius( 1.0f )
            {
            }

            float GetPhysicalBounds( f32 threshold = 0.1f ) const;

            float GetOuterRadius() const;
            float GetInnerRadius() const;
            void SetOuterRadius( float radius );
            void SetInnerRadius( float radius );

            REFLECT_DECLARE_CLASS(PointLight, Light);

            static void EnumerateClass( Reflect::Compositor<PointLight>& comp );

        private:

            float m_InnerRadius;
            float m_OuterRadius;
        };

        typedef Helium::SmartPtr<PointLight> PointLightPtr;
        typedef std::vector<PointLightPtr> V_PointLight;
    }
}