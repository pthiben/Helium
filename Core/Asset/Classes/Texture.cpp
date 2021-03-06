#include "Texture.h"

using namespace Helium;
using namespace Helium::Asset;

REFLECT_DEFINE_CLASS( Texture );

void Texture::EnumerateClass( Reflect::Compositor< Texture >& comp )
{
    comp.GetComposite().m_UIName = TXT( "Texture" );
    comp.GetComposite().SetProperty( AssetProperties::FileFilter, "*.tga;*.png;*.jpg;*.bmp" );

    comp.AddEnumerationField( &Texture::m_WrapModeU, "m_WrapModeU" );
    comp.AddEnumerationField( &Texture::m_WrapModeV, "m_WrapModeV" );
}
