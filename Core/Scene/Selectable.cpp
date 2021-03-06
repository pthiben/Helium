/*#include "Precompile.h"*/
#include "Core/Scene/Selectable.h"

using namespace Helium;
using namespace Helium::Core;

SCENE_DEFINE_TYPE( Selectable );

void Selectable::InitializeType()
{
  Reflect::RegisterClassType<Selectable>( TXT( "Selectable" ) );
}

void Selectable::CleanupType()
{
  Reflect::UnregisterClassType<Selectable>();
}

Selectable::Selectable()
: m_Selected( false )
{

}

Selectable::~Selectable()
{

}

bool Selectable::IsSelectable() const
{
  return true;
}

bool Selectable::IsSelected() const
{
  return m_Selected;
}

void Selectable::SetSelected(bool selected)
{
  m_Selected = selected;
}

void Selectable::ConnectProperties(EnumerateElementArgs& args)
{

}

bool Selectable::ValidatePanel(const tstring& name)
{
  return false;
}