#pragma once

#include "Platform/Exception.h"

namespace Helium
{
    namespace Content
    {
        class MissingJointException : public Helium::Exception
        {
        public:
            MissingJointException( const tstring& jointId, const tstring& dataLocation ) : Exception( TXT( "Could not locate joint [%s] in %s." ), jointId.c_str(), dataLocation.c_str() ) {}
        };
    }
}