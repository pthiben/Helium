#pragma once

#include "Core/API.h"
#include "Core/Content/Nodes/ContentHierarchyNode.h"

namespace Helium
{
    namespace Content
    {
        //
        // Camera
        //

        class CORE_API Camera : public HierarchyNode
        {
        public:
            virtual void Host(ContentVisitor* visitor); 

            Math::Matrix4 m_View;
            float m_FOV;
            u32 m_Width;
            u32 m_Height;

            Camera ()
                : m_FOV ( 0.f )
                , m_Width( 0 )
                , m_Height( 0 )
            {

            }

            REFLECT_DECLARE_CLASS(Camera, HierarchyNode);
            static void EnumerateClass( Reflect::Compositor<Camera>& comp );
        };

        typedef Helium::SmartPtr<Camera> CameraPtr;
    }
}