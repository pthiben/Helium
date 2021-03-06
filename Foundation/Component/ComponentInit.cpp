#include "ComponentInit.h" 

#include "Component.h"
#include "ComponentCollection.h"

#include "Foundation/InitializerStack.h"
#include "Foundation/Reflect/Registry.h"

using namespace Helium;

i32 g_ComponentInitCount = 0; 
Helium::InitializerStack g_ComponentRegisteredTypes;

Helium::InitializerStack g_ComponentInitializerStack;

void Component::Initialize()
{
  if (++g_ComponentInitCount == 1)
  {
    g_ComponentInitializerStack.Push( &Reflect::Initialize, &Reflect::Cleanup );

    g_ComponentRegisteredTypes.Push( Reflect::RegisterClassType<ComponentBase>( TXT("ComponentBase") ) );
    g_ComponentRegisteredTypes.Push( Reflect::RegisterClassType<ComponentCollection>( TXT("ComponentCollection") ) );

  }
}

void Component::Cleanup()
{
  if (--g_ComponentInitCount == 0)
  {
    g_ComponentRegisteredTypes.Cleanup();
    g_ComponentInitializerStack.Cleanup();
  }
}
