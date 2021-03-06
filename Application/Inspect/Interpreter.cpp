#include "Interpreter.h"

#include "Application/Inspect/Controls/Canvas.h"
#include "Application/Inspect/Controls/InspectPanel.h"
#include "Application/Inspect/Controls/Label.h"
#include "Application/Inspect/Controls/Action.h"

using namespace Helium::Inspect;

ST_Container& Interpreter::GetCurrentContainerStack()
{
  Helium::TakeMutex mutex( m_ContainerStackMutex );

  u32 threadId = GetCurrentThreadId();
  
  M_U32ContainerStack::iterator itr = m_ContainerStack.find( threadId );
  if ( itr == m_ContainerStack.end() )
  {
    ST_Container containerStack;
    itr = m_ContainerStack.insert( std::make_pair( threadId, containerStack ) ).first;
  }
  
  return itr->second;
}

void Interpreter::Add(Control* control)
{
  ST_Container& containerStack = GetCurrentContainerStack();
  containerStack.top()->AddControl(control);
}

void Interpreter::Push(Container* container)
{
  ST_Container& containerStack = GetCurrentContainerStack();
  containerStack.push(container);
}

Panel* Interpreter::PushPanel(const tstring& name, bool expanded)
{
  PanelPtr panel = m_Container->GetCanvas()->Create<Panel>( this );

  panel->SetText( name );
  panel->SetInterpreter( this );
  panel->SetExpanded( expanded );

  ST_Container& containerStack = GetCurrentContainerStack();
  containerStack.push(panel);

  return panel;
}

Container* Interpreter::PushContainer()
{
  ContainerPtr container = m_Container->GetCanvas()->Create<Container>( this );
  container->SetInterpreter( this );

  ST_Container& containerStack = GetCurrentContainerStack();
  containerStack.push(container);

  return container;
}

Container* Interpreter::Pop( bool setParent )
{
  ST_Container& containerStack = GetCurrentContainerStack();

  ContainerPtr child = containerStack.top();

  containerStack.pop();

  if ( setParent )
  {
    if (containerStack.empty())
    {
      m_Container->AddControl(child);
    }
    else
    {
      Container* parent = containerStack.top();

      parent->AddControl(child);
    }
  }

  return child;
}

Container* Interpreter::Top()
{
  ST_Container& containerStack = GetCurrentContainerStack();

  if ( !containerStack.empty() )
  {
    return containerStack.top();
  }
  
  return NULL;
}

Label* Interpreter::AddLabel(const tstring& name)
{
  LabelPtr control = m_Container->GetCanvas()->Create<Label>( this );
  control->SetInterpreter( this );
  control->SetText( name );

  ST_Container& containerStack = GetCurrentContainerStack();
  containerStack.top()->AddControl(control);

  return control;
}

Action* Interpreter::AddAction( const ActionSignature::Delegate& listener )
{
  ActionPtr control = m_Container->GetCanvas()->Create<Action>( this );
  control->SetInterpreter( this );
  control->AddListener( listener );

  ST_Container& containerStack = GetCurrentContainerStack();
  containerStack.top()->AddControl(control);

  return control;
}