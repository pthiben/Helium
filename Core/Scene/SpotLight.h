#pragma once

#include "Light.h"
#include "Core/Content/Nodes/ContentSpotLight.h"

namespace Helium
{
    namespace Core
    {
        class PrimitiveCone;

        class CORE_API SpotLight : public Core::Light
        {
            //
            // Members
            //
        protected:
            Core::PrimitiveCone* m_InnerCone;
            Core::PrimitiveCone* m_OuterCone;
            bool            m_FirstEntry;

            //
            // Runtime Type Info
            //

        public:
            SCENE_DECLARE_TYPE( SpotLight, Core::Light );
            static void InitializeType();
            static void CleanupType();


            //
            // Member functions
            //

            SpotLight(Core::Scene* scene);
            SpotLight(Core::Scene* scene, Content::SpotLight* light);
            virtual ~SpotLight();

            virtual i32 GetImageIndex() const HELIUM_OVERRIDE;
            virtual tstring GetApplicationTypeName() const HELIUM_OVERRIDE;

            virtual void Evaluate( GraphDirection direction ) HELIUM_OVERRIDE;

            virtual void Initialize() HELIUM_OVERRIDE;
            virtual void Create() HELIUM_OVERRIDE;
            virtual void Delete() HELIUM_OVERRIDE;
            virtual void SetScale( const Math::Scale& value );

            Core::PrimitiveCone* GetInnerCone() const
            {
                return m_InnerCone;
            }

            Core::PrimitiveCone* GetOuterCone() const
            {
                return m_OuterCone;
            }

            virtual void Render( RenderVisitor* render ) HELIUM_OVERRIDE;
            static void DrawPointer( IDirect3DDevice9* device, DrawArgs* args, const SceneNode* object );
            static void DrawInner( IDirect3DDevice9* device, DrawArgs* args, const SceneNode* object );
            static void DrawOuter( IDirect3DDevice9* device, DrawArgs* args, const SceneNode* object );
            static void DrawGodRayClipPlane( IDirect3DDevice9* device, DrawArgs* args, const SceneNode* object );
            static bool SetupGodRayClipPlane( IDirect3DDevice9* device, const SceneNode* object );

            virtual bool ValidatePanel(const tstring& name) HELIUM_OVERRIDE;
            static void CreatePanel( CreatePanelArgs& args );

            float GetInnerRadius() const;
            void SetInnerRadius( float innerAngle );

            float GetOuterRadius() const;
            void SetOuterRadius( float outerAngle );

            float GetInnerConeAngle() const;
            void SetInnerConeAngle( float innerAngle );

            float GetInnerConeAngleDegrees() const;
            void SetInnerConeAngleDegrees( float innerAngle );

            float GetOuterConeAngle() const;
            void SetOuterConeAngle( float outerAngle );

            float GetOuterConeAngleDegrees() const;
            void SetOuterConeAngleDegrees( float innerAngle );

            bool GetShadowMapHiRes() const;
            void SetShadowMapHiRes( bool b );

            bool GetGodRayEnabled() const;
            void SetGodRayEnabled( bool b );

            float GetGodRayOpacity() const;
            void SetGodRayOpacity( float opacity );

            float GetGodRayDensity() const;
            void SetGodRayDensity(float val);

            float GetGodRayFadeNear() const;
            void SetGodRayFadeNear(float val);

            float GetGodRayFadeFar() const;
            void SetGodRayFadeFar(float val);

            float GetGodRayQuality() const;
            void SetGodRayQuality(float val);

            float GetGodRayClipPlanePhiOffset() const;
            void SetGodRayClipPlanePhiOffset(float val);

            float GetGodRayClipPlaneOffset() const;
            void SetGodRayClipPlaneOffset(float val);

            u8 GetOffsetFactor() const;
            void SetOffsetFactor( u8 factor );

            u8 GetOffsetUnits() const;
            void SetOffsetUnits( u8 units );


        };
    }
}