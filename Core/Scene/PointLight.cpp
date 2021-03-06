/*#include "Precompile.h"*/
#include "PointLight.h"

#include "Core/Scene/Scene.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Scene/InstanceType.h"
#include "Core/Scene/PropertiesGenerator.h"
#include "Core/Scene/PrimitiveSphere.h"
#include "Core/Scene/PrimitivePointer.h"

using namespace Helium;
using namespace Helium::Math;
using namespace Helium::Core;

// RTTI
SCENE_DEFINE_TYPE(Core::PointLight);

void PointLight::InitializeType()
{
  Reflect::RegisterClassType< Core::PointLight >( TXT( "Core::PointLight" ) );

  PropertiesGenerator::InitializePanel( TXT( "PointLight" ), CreatePanelSignature::Delegate( &PointLight::CreatePanel ) );
}

void PointLight::CleanupType()
{
  Reflect::UnregisterClassType< Core::PointLight >();
}

PointLight::PointLight(Core::Scene* scene)
: Core::Light (scene, new Content::PointLight() )
, m_InnerSphere (NULL)
, m_OuterSphere (NULL)
, m_FirstEntry( true )
{

}

PointLight::PointLight(Core::Scene* scene, Content::PointLight* light)
: Core::Light ( scene, light )
, m_InnerSphere (NULL)
, m_OuterSphere (NULL)
, m_FirstEntry( true )
{

}

PointLight::~PointLight()
{
  delete m_InnerSphere;
  delete m_OuterSphere;
}

i32 PointLight::GetImageIndex() const
{
  return -1; // Helium::GlobalFileIconsTable().GetIconID( TXT( "light" ) );
}

tstring PointLight::GetApplicationTypeName() const
{
  return TXT( "PointLight" );
}

void PointLight::Initialize()
{
  __super::Initialize();

  Content::PointLight* light = GetPackage<Content::PointLight>();

  if (!m_InnerSphere)
  {
    m_InnerSphere = new Core::PrimitiveSphere ( m_Owner->GetViewport()->GetResources() );
    m_InnerSphere->m_Radius = light->GetInnerRadius();
    m_InnerSphere->Update();
  }

  if (!m_OuterSphere)
  {
    m_OuterSphere = new Core::PrimitiveSphere ( m_Owner->GetViewport()->GetResources() );
    m_OuterSphere->m_Radius = light->GetOuterRadius();
    m_OuterSphere->Update();
  }
}

void PointLight::Create()
{
  __super::Create();

  if (m_InnerSphere)
  {
    m_InnerSphere->Create();
  }

  if (m_OuterSphere)
  {
    m_OuterSphere->Create();
  }
}

void PointLight::Delete()
{
  __super::Delete();

  if (m_InnerSphere)
  {
    m_InnerSphere->Delete();
  }

  if (m_OuterSphere)
  {
    m_OuterSphere->Delete();
  }
}

void PointLight::SetScale( const Math::Scale& value )
{
  // protect against infinite calls between SetOuterRadius and SetScale
  if( m_FirstEntry )
  {
    m_FirstEntry = false;

     // don't allow negative scaling - take abs of values
    float maximum = std::max( std::max( abs(value.x), abs(value.y) ), abs(value.z) );
    Math::Scale newScale( maximum, maximum, maximum );
    __super::SetScale( newScale );

    SetOuterRadius( maximum );
  }
  m_FirstEntry = true;
}

void PointLight::Render( RenderVisitor* render )
{
  Math::Matrix4 normalizedRenderMatrix = render->State().m_Matrix.Normalized();
  // pointer is drawn normalized
  {
    RenderEntry* entry = render->Allocate(this);
    entry->m_Location = normalizedRenderMatrix;
    entry->m_Center = m_ObjectBounds.Center();
    entry->m_Draw = &PointLight::DrawPointer;
  }

  // shape is drawn normalized
  {
    RenderEntry* entry = render->Allocate(this);
    entry->m_Location = normalizedRenderMatrix;
    entry->m_Center = m_ObjectBounds.Center();
    entry->m_Draw = &PointLight::DrawShape;

    if ( Reflect::AssertCast< Core::InstanceType >( m_NodeType )->IsTransparent() )
    {
      entry->m_Flags |= RenderFlags::DistanceSort;
    }
  }

  // don't call __super here, it will draw big ass axes
  Core::HierarchyNode::Render( render );
}

void PointLight::DrawPointer( IDirect3DDevice9* device, DrawArgs* args, const SceneNode* object )
{
  const Core::PointLight* light = Reflect::ConstAssertCast<Core::PointLight>( object );

  const Core::InstanceType* type = Reflect::ConstAssertCast<Core::InstanceType>( light->GetNodeType() );

  light->SetMaterial( type->GetMaterial() );

  // draw type pointer
  type->GetPointer()->Draw( args );
}

void PointLight::DrawShape( IDirect3DDevice9* device, DrawArgs* args, const SceneNode* object )
{
  const Core::PointLight* light = Reflect::ConstAssertCast<Core::PointLight>( object );

  const Core::InstanceType* type = Reflect::ConstAssertCast<Core::InstanceType>( light->GetNodeType() );

  const Content::Light* contentLight = light->GetPackage< Content::Light >();
  
  if( contentLight )
  {
    Math::HDRColor3 color = contentLight->m_Color;
    color.s = 1.0f;
    color.ToFloat( s_Material.Ambient.r, s_Material.Ambient.g, s_Material.Ambient.b );
    s_Material.Ambient.a = 0.25f;
  }
  
  light->SetMaterial( s_Material );

  if (light->GetInnerSphere())
  {
    light->GetInnerSphere()->Draw( args );
  }

  if (light->GetOuterSphere())
  {
    light->GetOuterSphere()->Draw( args );
  }

  
}

///////////////////////////////////////////////////////////////////////////////
// Returns true if the specified panel is supported by Core::PointLight.
//
bool PointLight::ValidatePanel(const tstring& name)
{
  if (name == TXT( "PointLight" ) )
  {
    return true;
  }

  return __super::ValidatePanel( name );
}

void PointLight::CreatePanel( CreatePanelArgs& args )
{
  args.m_Generator->PushPanel( TXT( "Point Light" ), true);
  {
    args.m_Generator->PushContainer();
    {
      args.m_Generator->AddLabel( TXT( "Inner Radius" ) );
      args.m_Generator->AddValue<Core::PointLight, float>( args.m_Selection, &PointLight::GetInnerRadius, &PointLight::SetInnerRadius );
    }
    args.m_Generator->Pop();

    args.m_Generator->PushContainer();
    {
      args.m_Generator->AddLabel( TXT( "Outer Radius" ) );
      args.m_Generator->AddValue<Core::PointLight, float>( args.m_Selection, &PointLight::GetOuterRadius, &PointLight::SetOuterRadius );
    }
    args.m_Generator->Pop();
  }

  args.m_Generator->Pop();
}

float PointLight::GetInnerRadius() const
{
  return GetPackage< Content::PointLight >()->GetInnerRadius();
}
void PointLight::SetInnerRadius( float radius )
{
  Content::PointLight* light = GetPackage< Content::PointLight >();
  light->SetInnerRadius( radius );
  radius = light->GetInnerRadius();

  if (m_InnerSphere)
  {
    m_InnerSphere->m_Radius = radius;
    m_InnerSphere->Update();
  }

  if( GetOuterRadius() < radius )
  {
    SetOuterRadius( radius );
  }
  else
  {
    m_Changed.Raise( LightChangeArgs( this ) );
  }
}

float PointLight::GetOuterRadius() const
{
  return GetPackage< Content::PointLight >()->GetOuterRadius();
}
void PointLight::SetOuterRadius( float radius )
{
  Content::PointLight* light = GetPackage< Content::PointLight >();
  light->SetOuterRadius( radius );
  radius = light->GetOuterRadius();

  if (m_OuterSphere)
  {
    m_OuterSphere->m_Radius = radius;
    m_OuterSphere->Update();
  }

  if( GetInnerRadius() > radius )
  {
    SetInnerRadius( radius );
  }
  else
  {
    m_Changed.Raise( LightChangeArgs( this ) );
  }

  SetScale( Math::Scale( radius, radius, radius ) );
}

void PointLight::Evaluate( GraphDirection direction )
{
  Core::Transform::Evaluate(direction);

  switch (direction)
  {
    case GraphDirections::Downstream:
    {
      // start the box from scratch
      m_ObjectBounds.Reset();

      if ( m_NodeType )
      {
        m_ObjectBounds.minimum.x  = -1.0f;
        m_ObjectBounds.maximum.x  =  1.0f;
        m_ObjectBounds.minimum.y  = -1.0f;
        m_ObjectBounds.maximum.y  =  1.0f;
        m_ObjectBounds.minimum.z  = -1.0f;
        m_ObjectBounds.maximum.z  =  1.0f;
      }
      break;
    }
  }
}