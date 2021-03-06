#include "ContextMenu.h"
#include "Container.h"

#include <memory>

#include <wx/menu.h>

using namespace Helium::Inspect;

ContextMenu::ContextMenu(Control* control)
: m_Control (control)
{
  m_Control->AddRealizedListener( ControlSignature::Delegate ( this, &ContextMenu::ControlRealized ) );
}

ContextMenu::~ContextMenu()
{
  m_Control->RemoveRealizedListener( ControlSignature::Delegate ( this, &ContextMenu::ControlRealized ) );

  if (m_Control->IsRealized())
  {
    m_Control->GetContextWindow()->Disconnect( m_Control->GetContextWindow()->GetId(), wxEVT_CONTEXT_MENU, wxContextMenuEventHandler( ContextMenu::OnShow ), NULL, this );
  }
}

void ContextMenu::ControlRealized( Control* control )
{
  HELIUM_ASSERT( control == m_Control );
  HELIUM_ASSERT( control->IsRealized() );
  control->GetContextWindow()->Connect( control->GetContextWindow()->GetId(), wxEVT_CONTEXT_MENU, wxContextMenuEventHandler( ContextMenu::OnShow ), NULL, this );
}

void ContextMenu::OnShow( wxContextMenuEvent& event )
{
  wxMenu menu;

  std::vector< tstring >::const_iterator itr = m_Items.begin();
  std::vector< tstring >::const_iterator end = m_Items.end();
  for ( i32 count = 0; itr != end; ++itr, ++count )
  {
    if ( *itr == TXT( "-" ) )
    {
      menu.AppendSeparator();
    }
    else
    {
      menu.Append( wxID_HIGHEST + count, itr->c_str() );
    }
  }

  menu.Connect( wxID_HIGHEST, wxID_HIGHEST + (int)m_Items.size() - 1, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ContextMenu::OnItem ), NULL, this );

  m_Control->GetContextWindow()->PopupMenu( &menu );
}

void ContextMenu::OnItem( wxCommandEvent& event )
{
  const tstring& item ( m_Items[ event.GetId() - wxID_HIGHEST ] );

  M_ContextMenuDelegate::iterator found = m_Delegates.find(item);

  if (found != m_Delegates.end())
  {
    found->second.Invoke( ContextMenuEventArgs (m_Control, item) );
  }
}

void ContextMenu::AddItem(const tstring& item, ContextMenuSignature::Delegate delegate)
{
  M_ContextMenuDelegate::iterator found = m_Delegates.find(item);

  if (found == m_Delegates.end())
  {
    m_Items.push_back(item);
    m_Delegates.insert(M_ContextMenuDelegate::value_type(item, delegate));
  }
  else
  {
    found->second = delegate;
  }
}

void ContextMenu::AddSeperator()
{
  m_Items.push_back( TXT( "-" ) );
}