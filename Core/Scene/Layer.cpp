/*#include "Precompile.h"*/
#include "Layer.h"

#include "Foundation/Container/Insert.h" 
#include "Foundation/String/Natural.h"
#include "Foundation/Log.h"

#include "Application/Inspect/Controls/Choice.h"

#include "Core/Scene/Scene.h"

using namespace Helium;
using namespace Helium::Core;

SCENE_DEFINE_TYPE( Core::Layer );

///////////////////////////////////////////////////////////////////////////////
// Called once to prepare this class for use in the RTTI system.  Also sets
// up the static panel for displaying layer information in the GUI.
// 
void Layer::InitializeType()
{
  Reflect::RegisterClassType< Core::Layer >( TXT( "Core::Layer" ) );
  PropertiesGenerator::InitializePanel( TXT( "Layer" ), CreatePanelSignature::Delegate( &Layer::CreatePanel ) );
}

///////////////////////////////////////////////////////////////////////////////
// Called when this type is being unloaded.
// 
void Layer::CleanupType()
{
  Reflect::UnregisterClassType< Core::Layer >();
}

///////////////////////////////////////////////////////////////////////////////
// Constructor
// 
Layer::Layer( Core::Scene* scene, Content::Layer* layer )
: Core::SceneNode( scene, layer )
{
  m_VisibilityData = scene->GetVisibility(layer->m_ID); 
}

///////////////////////////////////////////////////////////////////////////////
// Destructor
// 
Layer::~Layer()
{
}

///////////////////////////////////////////////////////////////////////////////
// Returns the image index to use in the GUI for instances of this class.
// 
i32 Layer::GetImageIndex() const
{
  return -1; // Helium::GlobalFileIconsTable().GetIconID( TXT( "layer" ) );
}

///////////////////////////////////////////////////////////////////////////////
// Returns the UI friendly name for the node type.
// 
tstring Layer::GetApplicationTypeName() const
{
  return TXT( "Layer" );
}

///////////////////////////////////////////////////////////////////////////////
// Called after deserialization of the entire scene.  Builds the layer's 
// dependency graph (representing what objects are members of the layer) from
// the list of ids stored on the persistent data.
// 
void Layer::Initialize()
{
  __super::Initialize();

  Content::Layer* layer = GetPackage< Content::Layer >();
  HELIUM_ASSERT( layer );

  m_Descendants.clear();
  V_TUID memberIDs;
  V_TUID::const_iterator itr = layer->m_Members.begin();
  V_TUID::const_iterator end = layer->m_Members.end();
  for ( ; itr != end; ++itr )
  {
    Core::SceneNode* node = m_Owner->FindNode( *itr );
    if ( node )
    {
      node->CreateDependency( this );

      // The ID might not be the same as what we requested (like
      // in the case of copy/paste), so save off all the IDs
      // and update the persistent data when we are done.
      memberIDs.push_back( node->GetID() );
    }
    else
    {
      tstring idStr;
      (*itr).ToString( idStr );
      Log::Debug( TXT( "Layer %s: Unable to reconnect layer member with ID %s (it is no longer in the scene).\n" ), GetName().c_str(), idStr.c_str() );
    }
  }

  layer->m_Members = memberIDs;
}

///////////////////////////////////////////////////////////////////////////////
// Called just before the scene is serialized.  Traverses the layer's dependency
// graph and stores the id of each descendant in the persistent data for this
// class.
// 
void Layer::Pack()
{
  __super::Pack();

  Content::Layer* layer = GetPackage< Content::Layer >();
  HELIUM_ASSERT( layer );

  layer->m_Members.clear();
  layer->m_Members.resize( m_Descendants.size() );
  S_SceneNodeSmartPtr::const_iterator itr = m_Descendants.begin();
  S_SceneNodeSmartPtr::const_iterator end = m_Descendants.end();
  for ( size_t index = 0; itr != end; ++itr, ++index )
  {
    layer->m_Members[index] = (*itr)->GetID();
  }
}

///////////////////////////////////////////////////////////////////////////////
// Returns true if this layer is marked as being visible.
// 
bool Layer::IsVisible() const
{
  HELIUM_ASSERT( m_VisibilityData ); 
  return m_VisibilityData->GetVisibleLayer(); 

}

///////////////////////////////////////////////////////////////////////////////
// Set whether the layer is visible or not.  Members of this layer will not
// be visible if the layer is not visible.
// 
void Layer::SetVisible( bool visible )
{
  HELIUM_ASSERT( m_VisibilityData ); 
  if ( m_VisibilityData->GetVisibleLayer() != visible )
  {
    m_VisibilityData->SetVisibleLayer( visible ); 

    Dirty();

    m_VisibilityChanged.Raise( SceneNodeChangeArgs( this ) );
  }
}

///////////////////////////////////////////////////////////////////////////////
// Returns true if this layer is marked as selectable.  If a layer is selectable,
// it's members can be picked in the 3D view.
// 
bool Layer::IsSelectable() const
{
  const Content::Layer* layer = GetPackage< Content::Layer >();
  HELIUM_ASSERT( layer );
  return layer->m_Selectable;
}

///////////////////////////////////////////////////////////////////////////////
// Set whether this member should be selectable or not.  See IsSelectable().
// 
void Layer::SetSelectable( bool selectable )
{
  Content::Layer* layer = GetPackage< Content::Layer >();
  HELIUM_ASSERT( layer );
  layer->m_Selectable = selectable;
  Dirty();
}

const Math::Color3& Layer::GetColor() const
{
  return GetPackage< Content::Layer >()->m_Color;
}

void Layer::SetColor( const Math::Color3& color )
{
  Content::Layer* layer = GetPackage< Content::Layer >();
  if ( color != layer->m_Color )
  {
    layer->m_Color = color;
    Dirty();
  }
}

OS_SelectableDumbPtr Layer::GetMembers()
{
  OS_SelectableDumbPtr members;

  for each (Core::SceneNode* n in m_Descendants)
  {
    members.Append(n);
  }

  return members;
}

bool Layer::ContainsMember( Core::SceneNode* node ) const
{
  return m_Descendants.find( node ) != m_Descendants.end();
}

///////////////////////////////////////////////////////////////////////////////
// Pruning a layer does not prune the layer's members.
// 
void Layer::Prune( V_SceneNodeDumbPtr& prunedNodes )
{
  Content::Layer* layer = GetPackage< Content::Layer >();
  HELIUM_ASSERT( layer );

  // Iterate over the layer members, update the persistent data as we do so,
  // and build a local list of the members.  We have to build a list locally
  // because disconnecting at this point would change the list that we are
  // trying to iterate over.
  V_SceneNodeDumbPtr members;
  members.resize( m_Descendants.size() );
  layer->m_Members.clear();
  layer->m_Members.resize( m_Descendants.size() );
  S_SceneNodeSmartPtr::const_iterator itr = m_Descendants.begin();
  S_SceneNodeSmartPtr::const_iterator end = m_Descendants.end();
  for ( size_t index = 0; itr != end; ++itr, ++index )
  {
    const SceneNodePtr& member = (*itr);

    layer->m_Members[index] = member->GetID();
    members[index] = member.Ptr();
  }

  // Iterate over the local member list and disconnect each one
  V_SceneNodeDumbPtr::const_iterator memberItr = members.begin();
  V_SceneNodeDumbPtr::const_iterator memberEnd = members.end();
  for ( ; memberItr != memberEnd; ++memberItr )
  {
    DisconnectDescendant( *memberItr );
  }

  // Let the base class take care of the rest
  __super::Prune( prunedNodes );
}

///////////////////////////////////////////////////////////////////////////////
// Inserting a layer needs to restore its members that were previously pruned.
// 
void Layer::Insert(SceneGraph* g, V_SceneNodeDumbPtr& insertedNodes )
{
  // Let the base class put the layer back into the graph
  __super::Insert( g, insertedNodes );

  // Fetch the persistent data, dereference each object and add it back
  // as a member of the layer.
  if ( IsInitialized() )
  {
    Content::Layer* layer = GetPackage< Content::Layer >();
    HELIUM_ASSERT( layer );
    HELIUM_ASSERT( m_Descendants.empty() );
    V_TUID::const_iterator itr = layer->m_Members.begin();
    V_TUID::const_iterator end = layer->m_Members.end();
    for ( ; itr != end; ++itr )
    {
      const TUID& id = *itr;
      Core::SceneNode* node = m_Owner->FindNode( id );
      if ( node )
      {
        ConnectDescendant( node );
      }
      else
      {
        tstring idStr;
        id.ToString( idStr );
        Log::Debug( TXT( "Layer %s: Unable to reconnect layer member with ID %s (it is no longer in the scene).\n" ), GetName().c_str(), idStr.c_str() );
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Callback from the UI to determine if this class can display the specified
// panel.
// 
bool Layer::ValidatePanel(const tstring& name)
{
  if (name == TXT( "Layer" ) )
  {
    return true;
  }
  else if ( name == TXT( "Membership" ) )
  {
    // We don't have layers of layers, so this panel is not needed
    return false;
  }

  return __super::ValidatePanel(name);
}

///////////////////////////////////////////////////////////////////////////////
// Static callback from the attribute generator indicating that the UI elements
// for the layer panel need to be built.
// 
void Layer::CreatePanel( CreatePanelArgs& args )
{
  tstring unionStr;
  tstring intersectionStr;
  BuildUnionAndIntersection( args.m_Generator, args.m_Selection, unionStr, intersectionStr );

  args.m_Generator->PushPanel( TXT( "Layer" ), true);
  {
    args.m_Generator->PushContainer();
    {
      args.m_Generator->AddLabel( TXT( "Color" ) );
      args.m_Generator->AddColorPicker< Core::Layer, Math::Color3 >( args.m_Selection, &Layer::GetColor, &Layer::SetColor );
    }
    args.m_Generator->Pop();

    if ( args.m_Selection.Size() == 1 )
    {
      // Only one layer selected, just show its members

      args.m_Generator->PushContainer();
      {
        args.m_Generator->AddLabel( TXT( "Members" ) );
      }
      args.m_Generator->Pop();

      Inspect::Container* container = args.m_Generator->PushContainer();
      {
        Inspect::ListPtr control = args.m_Generator->GetContainer()->GetCanvas()->Create<Inspect::List>( args.m_Generator );
        control->Bind( new Inspect::StringFormatter<tstring>( new tstring( unionStr ), true ) );
        container->AddControl(control);
      }
      args.m_Generator->Pop();
    }
    else if ( args.m_Selection.Size() > 1 )
    {
      // More than one layer selected, show union and intersection of members
      
      args.m_Generator->PushContainer();
      {
        args.m_Generator->AddLabel( TXT( "Union" ) );
      }
      args.m_Generator->Pop();

      Inspect::Container* unionContainer = args.m_Generator->PushContainer();
      {
        Inspect::ListPtr control = args.m_Generator->GetContainer()->GetCanvas()->Create<Inspect::List>( args.m_Generator );
        control->Bind( new Inspect::StringFormatter<tstring>( new tstring( unionStr ), true ) );
        unionContainer->AddControl(control);
      }
      args.m_Generator->Pop();

      args.m_Generator->PushContainer();
      {
        args.m_Generator->AddLabel( TXT( "Intersection" ) );
      }
      args.m_Generator->Pop();

      Inspect::Container* intersectionContainer = args.m_Generator->PushContainer();
      {
        Inspect::ListPtr control = args.m_Generator->GetContainer()->GetCanvas()->Create<Inspect::List>( args.m_Generator );
        control->Bind( new Inspect::StringFormatter<tstring>( new tstring( intersectionStr ), true ) );
        intersectionContainer->AddControl(control);
      }
      args.m_Generator->Pop();
    }
  }
  args.m_Generator->Pop();
}

///////////////////////////////////////////////////////////////////////////////
// Static helper function to build the list of all members and common members
// across all selected objects. These lists will be shown in the UI.
// 
void Layer::BuildUnionAndIntersection( PropertiesGenerator* generator, const OS_SelectableDumbPtr& selection, tstring& unionStr, tstring& intersectionStr )
{
  typedef std::set< tstring, CaseInsensitiveNatStrCmp > S_Ordered;
  S_Ordered unionSet;
  S_Ordered intersectionSet;

  if ( generator )
  {
    // Keep a running tally of the union and intersection of members over each layer.
    HM_SceneNodeDumbPtr mapUnion;
    HM_SceneNodeDumbPtr mapIntersection;

    // For each item in the selection
    OS_SelectableDumbPtr::Iterator selItr = selection.Begin();
    OS_SelectableDumbPtr::Iterator selEnd = selection.End();
    for ( bool isFirstLayer = true; selItr != selEnd; ++selItr, isFirstLayer = false )
    {
      HM_SceneNodeDumbPtr layerMembers;

      // If it's a layer
      Core::Layer* layer = Reflect::ObjectCast< Core::Layer >( *selItr );
      if ( layer )
      {
        // Iterate over the layer's members (descendants)
        S_SceneNodeSmartPtr::const_iterator memberItr = layer->GetDescendants().begin();
        S_SceneNodeSmartPtr::const_iterator memberEnd = layer->GetDescendants().end();
        for ( ; memberItr != memberEnd; ++memberItr )
        {
          // Create a list box item
          const SceneNodePtr& dependNode = *memberItr;

          // Try to insert the item into our running list of all the items (the union)
          Helium::Insert<HM_SceneNodeDumbPtr>::Result inserted =
            mapUnion.insert( HM_SceneNodeDumbPtr::value_type( dependNode->GetID(), dependNode ) );

          if ( inserted.second )
          {
            // Maintain an ordered list of the node names (the union)
            unionSet.insert( dependNode->GetName() );
          }

          // Keep a map of all items that belong to this layer (we'll use it to update the intersection)
          layerMembers.insert( HM_SceneNodeDumbPtr::value_type( dependNode->GetID(), dependNode ) );
        }

        // Iterate over all intersecting items and make sure that each one was also in the layer
        // that we just iterated over.  If this is the first time through the loop, there won't
        // be anything in the intersection.
        HM_SceneNodeDumbPtr::iterator intersectionItr = mapIntersection.begin();
        HM_SceneNodeDumbPtr::iterator intersectionEnd = mapIntersection.end();
        while ( intersectionItr != intersectionEnd )
        {
          const TUID& key = intersectionItr->first;
          HM_SceneNodeDumbPtr::iterator found = layerMembers.find( key );
          if ( found == layerMembers.end() )
          {
            // The item in our intersection map was not in the current layer, so erase it.
            intersectionItr = mapIntersection.erase( intersectionItr );
          }
          else
          {
            // This item was in the layer, so leave it in the intersection map.
            intersectionItr++;
          }
        }
      }

      // If this is the first layer that we have looked at, the intersection is simply
      // all members of the layer.  We will remove items from the intersection as we 
      // iterate over the other layers, if an item does not exist in both lists.
      if ( isFirstLayer )
      {
        mapIntersection = layerMembers;
      }
    }

    // Now that we have built the intersection map, add the name of each item to
    // the list that will be used by the UI.
    HM_SceneNodeDumbPtr::const_iterator intersectionItr = mapIntersection.begin();
    HM_SceneNodeDumbPtr::const_iterator intersectionEnd = mapIntersection.end();
    for ( ; intersectionItr != intersectionEnd; ++intersectionItr )
    {
      Core::SceneNode* node = intersectionItr->second;
      intersectionSet.insert( node->GetName() );
    }

    // Build the intersection string
    for each ( const tstring& str in intersectionSet )
    {
      if ( !intersectionStr.empty() )
      {
        intersectionStr += Reflect::s_ContainerItemDelimiter;
      }
      intersectionStr += str;
    }

    // Build the union string
    for each ( const tstring& str in unionSet )
    {
      if ( !unionStr.empty() )
      {
        unionStr += Reflect::s_ContainerItemDelimiter;
      }
      unionStr += str;
    }
  }
}
