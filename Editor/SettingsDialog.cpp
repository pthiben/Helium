#include "Precompile.h"
#include "SettingsDialog.h"

#include <wx/wx.h>
#include <wx/listbook.h>

#include "Core/Scene/PropertiesGenerator.h"
#include "Application/Inspect/Controls/Canvas.h"
#include "Application/Inspect/Interpreters/Reflect/ReflectInterpreter.h"

using namespace Helium;
using namespace Helium::Editor;

SettingInfo::SettingInfo( Reflect::ElementPtr& source, Reflect::ElementPtr& clone, Inspect::CanvasPtr& canvas )
: m_Source( source )
, m_Clone( clone )
, m_Canvas( canvas )
{
}

SettingsDialog::SettingsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style )
: wxDialog( parent, id, title, pos, size, style )
, m_SettingSizer( NULL )
, m_CurrentSetting( NULL )
{
}

SettingsDialog::~SettingsDialog()
{
}

int SettingsDialog::ShowModal( Editor::Settings* prefs )
{
  m_SettingSizer = new wxBoxSizer( wxVERTICAL );
  m_CurrentSetting = NULL;
  m_SettingInfo.clear();

#pragma TODO("Automate this")
  Reflect::V_Element preferences;
  preferences.push_back( prefs->GetSceneSettings() );
  preferences.push_back( prefs->GetViewportSettings() );
  preferences.push_back( prefs->GetGridSettings() );
  preferences.push_back( prefs->GetVaultSettings() );

  wxListBox* propertiesListBox = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( 130 /* 207 */, -1 ) );
  propertiesListBox->Connect( propertiesListBox->GetId(), wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SettingsDialog::OnSettingsChanged ), NULL, this );
  
  wxSizer* propertiesSizer = new wxBoxSizer( wxHORIZONTAL );
  propertiesSizer->Add( propertiesListBox, 0, wxEXPAND | wxALL, 6 );
  propertiesSizer->Add( 6, 0, 0 );
  propertiesSizer->Add( m_SettingSizer, 1, wxEXPAND | wxALL, 6 );
  propertiesSizer->Add( 6, 0, 0 );
  
  Inspect::V_Control canvasControls;
  for ( Reflect::V_Element::iterator itr = preferences.begin(), end = preferences.end(); itr != end; ++itr )
  {
    Reflect::ElementPtr clone = (*itr)->Clone();
    clone->AddChangedListener( Reflect::ElementChangeSignature::Delegate( this, &SettingsDialog::OnRefreshElements ) );

    Inspect::CanvasPtr canvas = new Inspect::Canvas();
    canvasControls.push_back( canvas );
    canvas->SetPanelsExpanded( true );

    Inspect::TreeCanvasCtrl* canvasWindow = new Inspect::TreeCanvasCtrl( this );
    canvas->SetControl( canvasWindow );

    m_SettingSizer->Add( canvasWindow, 1, wxEXPAND, 0 );
    m_SettingSizer->Show( canvasWindow, false );

    Inspect::ReflectInterpreterPtr interpreter = new Inspect::ReflectInterpreter( canvas );
    std::vector< Reflect::Element* > elems;
    elems.push_back( clone );
    interpreter->Interpret( elems );

    int index = propertiesListBox->Append( (*itr)->GetTitle() );
    m_SettingInfo.insert( std::make_pair( index, new SettingInfo( (*itr), clone, canvas ) ) );
  }
  
  wxButton* restoreDefaults = new wxButton( this, wxID_ANY, wxT( "Restore Defaults" ), wxDefaultPosition, wxDefaultSize, 0 );
  restoreDefaults->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsDialog::OnRestoreDefaults ), NULL, this );

  wxButton* apply = new wxButton( this, wxID_ANY, wxT( "Apply" ), wxDefaultPosition, wxDefaultSize, 0 );
  apply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsDialog::OnApply ), NULL, this );

  wxBoxSizer* propertiesButtonSizer = new wxBoxSizer( wxHORIZONTAL );
  propertiesButtonSizer->Add( restoreDefaults, 0, 0, 0 );
  propertiesButtonSizer->Add( apply, 0, wxLEFT, 5 );

  m_SettingSizer->Add( 0, 6, 0 );
  m_SettingSizer->Add( propertiesButtonSizer, 0, wxALIGN_RIGHT, 0 );
  m_SettingSizer->Add( new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL ), 0, wxEXPAND | wxTOP, 5 );
  
  wxButton* okButton = new wxButton( this, wxID_ANY, wxT( "OK" ), wxDefaultPosition, wxDefaultSize, 0 );
  okButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsDialog::OnOk ), NULL, this );

  wxButton* cancelButton = new wxButton( this, wxID_ANY, wxT( "Cancel" ), wxDefaultPosition, wxDefaultSize, 0 );
  cancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsDialog::OnCancel ), NULL, this );

  wxBoxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );
  buttonSizer->Add( okButton, 0, wxALL, 5 );
  buttonSizer->Add( cancelButton, 0, wxUP | wxDOWN | wxRIGHT, 5 );
  buttonSizer->Add( 7, 0, 0 );

  wxBoxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );
  mainSizer->Add( 0, 1, 0 );
  mainSizer->Add( propertiesSizer, 1, wxEXPAND, 0 );
  mainSizer->Add( buttonSizer, 0, wxALIGN_RIGHT, 0 );
  mainSizer->Add( 0, 8, 0 );
  
  if ( propertiesListBox->GetCount() && ( m_SettingInfo.find( 0 ) != m_SettingInfo.end() ) )
  {
    propertiesListBox->SetSelection( 0 );
    SelectCanvas( m_SettingInfo[ 0 ] );
  }

  SetSizer( mainSizer );
  Layout();
  Centre();
  
  int result = __super::ShowModal();

  if ( result == wxID_OK )
  {
    for ( M_SettingInfo::iterator itr = m_SettingInfo.begin(), end = m_SettingInfo.end(); itr != end; ++itr )
    {
      SettingInfo* info = itr->second;
      if ( !info->m_Clone->Equals( info->m_Source ) )
      {
        info->m_Clone->CopyTo( info->m_Source );
        info->m_Source->RaiseChanged();
      }
    }
  }
  
  for ( M_SettingInfo::iterator itr = m_SettingInfo.begin(), end = m_SettingInfo.end(); itr != end; ++itr )
  {
    itr->second->m_Clone->RemoveChangedListener( Reflect::ElementChangeSignature::Delegate( this, &SettingsDialog::OnRefreshElements ) );
  }
  
  return result;
}

void SettingsDialog::OnRestoreDefaults( wxCommandEvent& args )
{
  if ( !m_CurrentSetting )
  {
    return;
  }
  
  Reflect::ElementPtr defaultElement = Reflect::ObjectCast<Reflect::Element>( Reflect::Registry::GetInstance()->CreateInstance( m_CurrentSetting->m_Clone->GetType() ) );
  if ( !defaultElement )
  {
    return;
  }

  int tries = 10;
  bool changed = false;
  while ( ( tries-- > 0 ) && ( !defaultElement->Equals( m_CurrentSetting->m_Clone ) ) )
  {
    changed = true;
    defaultElement->CopyTo( m_CurrentSetting->m_Clone );
    m_CurrentSetting->m_Clone->RaiseChanged();
  }

  if ( changed )
  {
    m_CurrentSetting->m_Canvas->Freeze();
    m_CurrentSetting->m_Canvas->Read();
    m_CurrentSetting->m_Canvas->Thaw();
  }
}

void SettingsDialog::OnApply( wxCommandEvent& args )
{
  if ( !m_CurrentSetting )
  {
    return;
  }

  if ( m_CurrentSetting->m_Clone->Equals( m_CurrentSetting->m_Source ) )
  {
    return;
  }

  m_CurrentSetting->m_Clone->CopyTo( m_CurrentSetting->m_Source );
  m_CurrentSetting->m_Source->RaiseChanged();
}

void SettingsDialog::OnOk( wxCommandEvent& args )
{
  EndModal( wxID_OK );
}

void SettingsDialog::OnCancel( wxCommandEvent& args )
{
  EndModal( wxID_CANCEL );
}

void SettingsDialog::OnSettingsChanged( wxCommandEvent& args )
{
  SettingInfo* newSettingInfo = NULL;
  
  M_SettingInfo::iterator itr = m_SettingInfo.find( args.GetInt() );
  if ( itr != m_SettingInfo.end() )
  {
    newSettingInfo = itr->second;
  }
  
  SelectCanvas( newSettingInfo );
}

void SettingsDialog::OnRefreshElements( const Reflect::ElementChangeArgs& args )
{
  if ( m_CurrentSetting )
  {
    m_CurrentSetting->m_Canvas->Freeze();
    m_CurrentSetting->m_Canvas->Read();
    m_CurrentSetting->m_Canvas->Thaw();
  }
}

void SettingsDialog::SelectCanvas( SettingInfo* settingInfo )
{
  if ( !m_SettingSizer )
  {
    return;
  }

  if ( m_CurrentSetting )
  {
    m_SettingSizer->Show( m_CurrentSetting->m_Canvas->GetControl(), false );
  }
    
  m_CurrentSetting = settingInfo;
  
  if ( m_CurrentSetting )
  {
    m_SettingSizer->Show( m_CurrentSetting->m_Canvas->GetControl(), true );

    m_CurrentSetting->m_Canvas->Freeze();
    m_CurrentSetting->m_Canvas->Layout();
    m_CurrentSetting->m_Canvas->Read();
    m_CurrentSetting->m_Canvas->Thaw();
  }
    
  m_SettingSizer->Layout();
}
