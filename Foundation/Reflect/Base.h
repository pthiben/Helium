#pragma once

#include <map>

#include "API.h"

#include "Platform/Types.h"
#include "Foundation/Container/Insert.h"
#include "Foundation/Atomic.h"

//
// Obviously the reflection system itself can't use the same type checking as the code
//  its reflecting upon, so define a simple type checking system just for the reflection classes
//

#define REFLECTION_BASE(__Type) \
    const static int ReflectionTypeID = __Type; \
    virtual int GetReflectionType () const { return __Type; } \
    virtual bool HasReflectionType (int id) const { return __Type == id; }

#define REFLECTION_TYPE(__Type) \
    const static int ReflectionTypeID = __Type; \
    virtual int GetReflectionType () const HELIUM_OVERRIDE { return __Type; } \
    virtual bool HasReflectionType (int id) const HELIUM_OVERRIDE { return __Type == id || __super::HasReflectionType(id); }

namespace Helium
{
    namespace Reflect
    {
        //
        // All types have to belong to this enum
        //

        namespace ReflectionTypes
        {
            enum ReflectionType
            {
                Invalid,
                Type,
                Enumeration,
                Composite,
                Structure,
                Class,
                Field,
                ElementField,
                EnumerationField,
            };
        }
        typedef ReflectionTypes::ReflectionType ReflectionType;


        //
        // This lets us safely cast between reflection class pointers
        //

        class FOUNDATION_API Base : public Helium::AtomicRefCountBase
        {
        public:
            REFLECTION_BASE(ReflectionTypes::Invalid);

            mutable std::map< tstring, tstring > m_Properties;

            template<class T>
            inline void SetProperty( const tstring& key, const T& value )
            {
                tostringstream str;
                str << value;

                if ( !str.fail() )
                {
                    SetProperty<tstring>( key, str.str() );
                }
            }

            template<>
            inline void SetProperty( const tstring& key, const tstring& value )
            {
                m_Properties[key] = value;
            }

            template<class T>
            inline bool GetProperty( const tstring& key, T& value ) const
            {
                tstring strValue;
                bool result = GetProperty<tstring>( key, strValue );

                if ( result )
                {
                    tistringstream str( strValue );
                    str >> value;
                    return !str.fail();
                }

                return false;
            }

            template<>
            inline bool GetProperty( const tstring& key, tstring& value ) const
            {
                std::map< tstring, tstring >::const_iterator found = m_Properties.find( key ); 
                if ( found != m_Properties.end() )
                {
                    value = found->second;
                    return true;
                }

                return false;
            }

            inline const tstring& GetProperty( const tstring& key ) const
            {
                std::map< tstring, tstring >::const_iterator found = m_Properties.find( key );
                if ( found != m_Properties.end() )
                {
                    return found->second;
                }

                static tstring empty;
                return empty;
            }
        }; 

        template<typename T>
        T* ReflectionCast(Base* type)
        {
            return (type && type->HasReflectionType( T::ReflectionTypeID )) ? static_cast<T*>(type) : NULL;
        }

        template<typename T>
        const T* ReflectionCast(const Base* type)
        {
            return (type && type->HasReflectionType( T::ReflectionTypeID )) ? static_cast<const T*>(type) : NULL;
        }
    }
}