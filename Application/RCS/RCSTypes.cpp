#include "RCSTypes.h"

#include <sstream>

using namespace Helium;

tstring RCS::GetChangesetIdAsString( const u64 changesetId )
{
  tstringstream str;
  if ( changesetId == DefaultChangesetId )
  {
    str << TXT( "default" );
  }
  else
  {
    str << changesetId;
  }

  return str.str();
}
