#pragma once

#include "Editor/API.h"

#include "Foundation/Automation/Event.h"

#include "Foundation/Undo/Command.h"

namespace Helium
{
    namespace Editor
    {
        //
        // Designed to be aggregated into an editor, this class
        //  allows deferred command execution to allow performing
        //  commands to coexist easier with issuing commands in callbacks
        //

        struct PushCommandArgs
        {
            const Undo::V_CommandSmartPtr m_Commands;

            PushCommandArgs( const Undo::V_CommandSmartPtr& commands )
                : m_Commands (commands)
            {

            }
        };

        typedef Helium::Signature<void, const PushCommandArgs&> PushCommandSignature;

        class CommandQueue : public wxEvtHandler
        {
        private:
            // the editor to use
            wxWindow* m_Window;

            // the commands to redo when flushing
            Undo::V_CommandSmartPtr m_Commands;

        public:
            CommandQueue( wxWindow* window );
            virtual ~CommandQueue();

        protected:
            void HandleEvent( wxCommandEvent& event );

        public:
            void Push( const Undo::CommandPtr& command );
            void Flush();

            //
            // Events
            //
        protected:
            PushCommandSignature::Event m_PushCommand;
        public:
            void AddPushCommandListener( const PushCommandSignature::Delegate& listener )
            {
                m_PushCommand.Add( listener );
            }
            void RemovePushCommandListener( const PushCommandSignature::Delegate& listener )
            {
                m_PushCommand.Remove( listener );
            }
        };
    }
}