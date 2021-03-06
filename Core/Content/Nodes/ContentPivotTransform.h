#pragma once

#include "Core/Content/Nodes/ContentTransform.h"

namespace Helium
{
    namespace Content
    {
        //
        // Pivot transform node adds:
        //  - compound pivot transforms
        //  - shear
        //
        // [T] = [Sp]-1x[S]x[Sh]x[Sp]x[Spt]x[Rp]-1x[R]x[Rp]x[Rpt]x[T]
        //

        class CORE_API PivotTransform : public Transform
        {
        public:
            // The shear values
            Math::Shear m_Shear;

            // The scale pivot translation vector
            Math::Vector3 m_ScalePivot;

            // The compensation vector for preserving the transform when the scale pivot it moved
            Math::Vector3 m_ScalePivotTranslate;

            // The rotation pivot translation vector
            Math::Vector3 m_RotatePivot;

            // The compensation vector for preserving the transform when the rotation pivot it moved
            Math::Vector3 m_RotatePivotTranslate;

            // The translate pivot translation vector
            Math::Vector3 m_TranslatePivot;

            // If this is true, use m_RotatePivot
            bool m_SnapPivots;

            PivotTransform ()
                : m_SnapPivots (true)
            {

            }

            PivotTransform (const Helium::TUID& id)
                : Transform (id)
                , m_SnapPivots (true)
            {

            }

            REFLECT_DECLARE_CLASS(PivotTransform, Transform);

            static void EnumerateClass( Reflect::Compositor<PivotTransform>& comp );

            virtual bool ProcessComponent(Reflect::ElementPtr element, const tstring& memberName) HELIUM_OVERRIDE;
            virtual void ResetTransform() HELIUM_OVERRIDE;
        };

        typedef Helium::SmartPtr<PivotTransform> PivotTransformPtr;
        typedef std::vector<PivotTransformPtr> V_PivotTransform;
    }
}