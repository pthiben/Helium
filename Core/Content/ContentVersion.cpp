#include "ContentVersion.h"

using namespace Helium;
using namespace Helium::Content;

const tchar* Content::ContentVersion::CONTENT_VERSION = TXT( "5" );

REFLECT_DEFINE_CLASS(ContentVersion)

void ContentVersion::EnumerateClass( Reflect::Compositor<ContentVersion>& comp )
{
  Reflect::Field* fieldContentVersion = comp.AddField( &ContentVersion::m_ContentVersion, "m_ContentVersion", Reflect::FieldFlags::Force );
}


Content::ContentVersion::ContentVersion()
{
  m_ContentVersion = CONTENT_VERSION;
}

Content::ContentVersion::ContentVersion(const tchar* source, const tchar* sourceVersion)
: Version (source, sourceVersion)
{
  m_ContentVersion = CONTENT_VERSION;
}

bool Content::ContentVersion::IsCurrent()
{
  return __super::IsCurrent() && m_ContentVersion == CONTENT_VERSION;
}