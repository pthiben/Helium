#pragma once

#include "Core/Scene/Object.h"

#include "Foundation/Container/OrderedSet.h"

namespace Helium
{
    namespace Core
    {
        class ManiuplatorAdapterCollection;
        class PropertiesGenerator;
        class EnumerateElementArgs;

        class CORE_API Selectable : public Object
        {
        protected:
            bool m_Selected;


            //
            // Runtime Type Info
            //

        public:
            SCENE_DECLARE_TYPE( Selectable, Object );
            static void InitializeType();
            static void CleanupType();


            //
            // Implementation
            //

        public:
            Selectable();
            virtual ~Selectable();

            // Is this object currently selectable?
            //  Sometimes objects can on a per-instance or per-type basis decided to NOT be selectable
            //  This prototype exposes the capability to HELIUM_OVERRIDE the selection of an object
            virtual bool IsSelectable() const;

            // Get/Set selected state
            virtual bool IsSelected() const;
            virtual void SetSelected(bool);

            // do enumeration of applicable attributes on this object
            virtual void ConnectProperties(EnumerateElementArgs& args);

            // validate a named panel as usable
            virtual bool ValidatePanel(const tstring& name);
        };

        // Smart pointer to an Selectable
        typedef Helium::SmartPtr< Selectable > LSelectablePtr;

        // vector of selectable objects
        typedef Helium::OrderedSet<Selectable*> OS_SelectableDumbPtr;
    }
}