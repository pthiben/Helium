#pragma once

#include "Primitive.h"

namespace Helium
{
    namespace Core
    {
        class PrimitiveFrame : public Core::PrimitiveTemplate<TransformedColored>
        {
        public:
            Math::Point m_Start;
            Math::Point m_End;

            u32 m_InnerColor;
            u32 m_BorderColor;

        public:
            PrimitiveFrame(ResourceTracker* tracker);

            virtual void Update() HELIUM_OVERRIDE;
            virtual void Draw( DrawArgs* args, const bool* solid = NULL, const bool* transparent = NULL ) const HELIUM_OVERRIDE;
            virtual bool Pick( PickVisitor* pick, const bool* solid = NULL ) const HELIUM_OVERRIDE;
        };
    }
}