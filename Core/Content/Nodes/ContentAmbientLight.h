#pragma once

#include "Core/API.h"
#include "Core/Content/Nodes/ContentLight.h"

namespace Helium
{
    namespace Content
    {
        class CORE_API AmbientLight : public Light
        {
        public:

            bool m_SampleEnvironment;
            bool m_DoAmbientOcclusion;
            u32 m_Samples;
            f32 m_SampleDistance;
            f32 m_Spread;
            Math::HDRColor3 m_OcclusionColor;



            virtual void Host(ContentVisitor* visitor); 

            AmbientLight ()
                : Light()
                , m_Samples( 16 )
                , m_SampleDistance( 500.0f )
                , m_OcclusionColor( 0.0f, 0.0f, 0.0f )
                , m_DoAmbientOcclusion( true )
                , m_Spread( 1.0f )
                , m_SampleEnvironment( false )
            {
                Light::m_CastShadows = false;
            }

            AmbientLight (Helium::TUID &id)
                : Light (id)
                , m_Samples( 16 )
                , m_SampleDistance( 500.0f )
                , m_OcclusionColor( 0.0f, 0.0f, 0.0f )
                , m_Spread( 1.0f )
                , m_SampleEnvironment( false )
            {
                Light::m_CastShadows = false;
            }

            REFLECT_DECLARE_CLASS(AmbientLight, Light);

            static void AmbientLight::EnumerateClass( Reflect::Compositor<AmbientLight>& comp );

        };

        typedef Helium::SmartPtr<AmbientLight> AmbientLightPtr;
        typedef std::vector<AmbientLightPtr> V_AmbientLight;
    }
}