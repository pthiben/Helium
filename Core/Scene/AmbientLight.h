#pragma once

#include "Light.h"
#include "Core/Content/Nodes/ContentAmbientLight.h"

namespace Helium
{
    namespace Core
    {
        class AmbientLight : public Core::Light
        {
            //
            // Runtime Type Info
            //

        public:
            SCENE_DECLARE_TYPE( Core::AmbientLight, Core::Light );
            static void InitializeType();
            static void CleanupType();


            //
            // Member functions
            //

            AmbientLight(Core::Scene* scene);
            AmbientLight(Core::Scene* scene, Content::AmbientLight* light);

            virtual i32 GetImageIndex() const HELIUM_OVERRIDE;
            virtual tstring GetApplicationTypeName() const HELIUM_OVERRIDE;

            virtual bool ValidatePanel(const tstring& name) HELIUM_OVERRIDE;
            static void CreatePanel( CreatePanelArgs& args );

            // UI Getter/Setters
            Math::Color3 GetOcclusionColor() const;
            void   SetOcclusionColor( Math::Color3 color );
            float  GetOcclusionIntensity() const;
            void   SetOcclusionIntensity( float intensity );
            f32    GetSampleDistance() const;
            void   SetSampleDistance( f32 distance );
            u32    GetSamples() const;
            void   SetSamples( u32 samples );
            bool   GetDoOcclusion() const;
            void   SetDoOcclusion( bool occlusion );
            f32    GetSpread() const;
            void   SetSpread( float spread );
            bool   GetSampleEnvironment() const;
            void   SetSampleEnvironment( bool sample );


        };
    }
}