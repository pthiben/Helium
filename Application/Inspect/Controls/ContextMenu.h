#pragma once

#include <map>

#include "Platform/Types.h"
#include "Foundation/Automation/Event.h"
#include "Application/API.h"

#include <wx/event.h>

namespace Helium
{
    namespace Inspect
    {
        class APPLICATION_API Control;


        //
        // Static context menu support
        //

        struct ContextMenuEventArgs
        {
            Control* m_Control;

            const tstring& m_Item;

            ContextMenuEventArgs(Control* control, const tstring& item)
                : m_Control (control)
                , m_Item (item)
            {

            }
        };

        // the delegate for a per-item callback to be called upon activation
        typedef Helium::Signature<void, const ContextMenuEventArgs&> ContextMenuSignature;

        // container for each delegate of each context menu item
        typedef std::map<tstring, ContextMenuSignature::Delegate> M_ContextMenuDelegate;


        //
        // Dynamic context menu support
        //

        class APPLICATION_API ContextMenu;
        typedef Helium::SmartPtr<ContextMenu> ContextMenuPtr;

        // popup-time context menu setup delegate
        typedef Helium::Signature<void, ContextMenuPtr> SetupContextMenuSignature;


        //
        // The menu class
        //

        class APPLICATION_API ContextMenu : public wxEvtHandler, public Helium::RefCountBase<ContextMenu>
        {
        protected:
            Control* m_Control;

            std::vector< tstring > m_Items;
            M_ContextMenuDelegate m_Delegates;

        public:
            ContextMenu(Control* control);
            ~ContextMenu();

        protected:
            void ControlRealized( Control* control );
            void OnShow( wxContextMenuEvent& event );
            void OnItem( wxCommandEvent& event );

        public:
            virtual const std::vector< tstring >& GetItems()
            {
                return m_Items;
            }

            virtual void Resize(size_t size)
            {
                m_Items.resize(size);
            }

            virtual void Clear()
            {
                m_Items.clear();
            }

            // event to add dynamic context menus items to the menu instance
            SetupContextMenuSignature::Event m_SetupContextMenuEvent;

            virtual void AddItem(const tstring& item, ContextMenuSignature::Delegate delegate);
            virtual void AddSeperator();
        };

        typedef Helium::SmartPtr<ContextMenu> ContextMenuPtr;
    }
}