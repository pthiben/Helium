#pragma once

#include "ReflectFieldInterpreter.h"
#include "ReflectBitfieldCheckBox.h"

namespace Helium
{
    namespace Inspect
    {
        class APPLICATION_API ReflectBitfieldInterpreter : public ReflectFieldInterpreter
        {
        public:
            ReflectBitfieldInterpreter( Container* container );

            virtual void InterpretField( const Reflect::Field* field, const std::vector<Reflect::Element*>& instances, Container* parent ) HELIUM_OVERRIDE;
        };

        typedef Helium::SmartPtr<ReflectBitfieldInterpreter> ReflectBitfieldInterpreterPtr;
        typedef std::vector< ReflectBitfieldInterpreterPtr > V_ReflectBitfieldInterpreterSmartPtr;
    }
}