#pragma once

#include <hash_map>
#include <set>

#include "Core/API.h"

#include "Core/Asset/AssetFactory.h"

#include "Foundation/Container/OrderedSet.h"
#include "Foundation/File/Path.h"
#include "Foundation/Component/Component.h"
#include "Foundation/Component/ComponentCollection.h" 

namespace Helium
{
    namespace Asset
    {
        namespace AssetProperties
        {
            static const tchar* ShortDescription  = TXT( "ShortDescription" );
            static const tchar* LongDescription   = TXT( "ShortDescription" );
            static const tchar* SmallIcon         = TXT( "SmallIcon" );
            static const tchar* RootFolderSpec    = TXT( "RootFolderSpec" );
            static const tchar* FileFilter        = TXT( "FileFilter" );
            static const tchar* AssetTemplates    = TXT( "AssetTemplates" );
        }

        class AssetClass;
        typedef Helium::SmartPtr< AssetClass > AssetClassPtr;
        typedef std::vector< AssetClassPtr > V_AssetClass;

        typedef std::set<AssetClassPtr> S_AssetClass;
        typedef Helium::OrderedSet< Asset::AssetClassPtr > OS_AssetClass;

        class CORE_API AssetClass HELIUM_ABSTRACT : public Component::ComponentCollection
        {
        private:

            Helium::Path m_SerializationPath; // optional path where this asset was deserialized from
            Helium::Path m_Path; // path to the asset's backing/art file

            tstring m_Description;
            std::set< tstring > m_Tags;

        private:
            static tstring s_BaseBuiltDirectory;

            static std::map< tstring, class AssetFactory* > s_AssetFactories;

        public:
            //
            // RTTI
            //
            REFLECT_DECLARE_ABSTRACT( AssetClass, ComponentCollection );

            static void EnumerateClass( Reflect::Compositor< AssetClass >& comp );


        public:
            AssetClass();

        public:
            static void SetBaseBuiltDirectory( const tstring& path )
            {
                s_BaseBuiltDirectory = path;
            }

            static AssetClassPtr LoadAssetClass( const tchar* path );
            static AssetClassPtr LoadAssetClass( const tstring& path )
            {
                return LoadAssetClass( path.c_str() );
            }
            static AssetClassPtr LoadAssetClass( const Helium::Path& path )
            {
                return LoadAssetClass( path.Get().c_str() );
            }

            template <class T>
            static Helium::SmartPtr<T> LoadAssetClass( const tchar* path )
            {
                return Reflect::TryCast<T>( LoadAssetClass( path ) );
            }

            template <class T>
            static Helium::SmartPtr<T> LoadAssetClass( const tstring& path )
            {
                return Reflect::TryCast<T>( LoadAssetClass( path.c_str() ) );
            }

            template <class T>
            static Helium::SmartPtr<T> LoadAssetClass( const Helium::Path& path )
            {
                return Reflect::TryCast<T>( LoadAssetClass( path.Get().c_str() ) );
            }

        public:
            static void RegisterFactory( const tstring& extension, AssetFactory* factory )
            {
                s_AssetFactories[ extension ] = factory;
            }

            static void UnregisterFactory( const tstring& extension )
            {
                s_AssetFactories.erase( extension );
            }

            static void UnregisterFactory( const AssetFactory* factory )
            {
                for ( std::map< tstring, AssetFactory* >::iterator itr = s_AssetFactories.begin(), end = s_AssetFactories.end(); itr != end; )
                {
                    if ( (*itr).second == factory )
                    {
                        s_AssetFactories.erase( itr++ );

                        if ( itr == end )
                        {
                            break;
                        }
                    }

                    ++itr;
                }
            }

            static AssetClassPtr Create( const Helium::Path& path )
            {
                std::map< tstring, AssetFactory* >::iterator itr = s_AssetFactories.find( path.Extension() );
                
                if ( itr != s_AssetFactories.end() )
                {
                    return (*itr).second->Create( path );
                }

                return NULL;
            }

            static void GetExtensions( std::set< tstring >& extensions )
            {
                for ( std::map< tstring, AssetFactory* >::const_iterator itr = s_AssetFactories.begin(), end = s_AssetFactories.end(); itr != end; ++itr )
                {
                    (*itr).second->GetExtensions( extensions );
                }
            }

        public:

            void SetSerializationPath( const Helium::Path& path )
            {
                m_SerializationPath = path;
            }

            const Helium::Path& GetSerializationPath()
            {
                return m_SerializationPath;
            }

            void SetPath( const Helium::Path& path )
            {
                m_Path = path;
            }
            const Helium::Path& GetPath()
            {
                return m_Path;
            }

            Helium::Path GetBuiltDirectory();

            // stuff/fruitBasketFromHell/appleSuccubus.entity.nrb
            tstring GetFullName() const;

            // appleSuccubus
            tstring GetShortName() const;

            const tstring& GetDescription() const
            {
                return m_Description;
            }
            void SetDescription( const tstring& description )
            {
                m_Description = description;
            }

            const std::set< tstring >& GetTags() const
            {
                return m_Tags;
            }
            void SetTags( const std::set< tstring >& tags )
            {
                m_Tags = tags;
            }
            void AddTag( const tstring& tag )
            {
                m_Tags.insert( tag );
            }
            void RemoveTag( const tstring& tag )
            {
                m_Tags.erase( tag );
            }

        public:
            virtual void GatherSearchableProperties( Helium::SearchableProperties* properties ) const HELIUM_OVERRIDE;
            virtual void GetFileReferences( std::set< Helium::Path >& fileReferences );

        public:
            // we were changed by somebody, reclassify
            virtual void ComponentChanged( const Component::ComponentBase* component = NULL ) HELIUM_OVERRIDE;

            // add to or set an attribute in the collection
            virtual bool SetComponent( const Component::ComponentPtr& component, bool validate = true, tstring* error = NULL ) HELIUM_OVERRIDE;

            // remove attribute from a slot
            virtual bool RemoveComponent( i32 typeID ) HELIUM_OVERRIDE;

        public:

            // Returns true by default. Override to specify more stringent requirements on the asset.
            virtual bool ValidateClass( tstring& error ) const;

            // validate the incoming attribute as ok to consume
            virtual bool ValidateCompatible( const Component::ComponentPtr &component, tstring& error ) const HELIUM_OVERRIDE;


        public:
            // write to the location on disk backed by the file manager id
            virtual void Serialize();

            // callback when this AssetClass has finished loading off disk
            virtual void LoadFinished();

            // copy this asset and its attributes into the destination
            virtual void CopyTo(const Reflect::ElementPtr& destination) HELIUM_OVERRIDE;
        };
    }
}