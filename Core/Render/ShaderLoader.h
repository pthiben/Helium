#pragma once

#include "Core/API.h"
#include "Platform/Types.h"
#include "Foundation/Memory/SmartPtr.h"

namespace Helium
{
    namespace Core
    {
        namespace Render
        {
            class Shader;
            class ShaderManager;

            class CORE_API ShaderLoader: public Helium::RefCountBase<ShaderLoader>
            {
            public:
                ShaderLoader();
                virtual ~ShaderLoader();

                virtual Shader* ParseFile( const tchar* filename, ShaderManager* db ) = 0;
            };

            typedef Helium::SmartPtr<ShaderLoader> ShaderLoaderPtr;
        }
    }
}