#pragma once

#include "Core/API.h"

#include "Core/Scene/Object.h"

#include "Core/Scene/SceneNode.h"

namespace Helium
{
    namespace Core
    {
        class SceneNode;

        // nodes appear in the objects window in the UI by type
        struct NodeTypeChangeArgs
        {
            Core::SceneNode* m_Node;

            NodeTypeChangeArgs( Core::SceneNode* node )
                : m_Node (node)
            {

            }
        };
        typedef Helium::Signature< void, const NodeTypeChangeArgs& > NodeTypeChangeSignature;

        //
        // Node type that stores all the instances of any type.  All the
        // instances refer back to this node type for basic, shared information
        // among all the instances.
        // 

        class CORE_API SceneNodeType : public Object
        {
            //
            // Members
            //

        protected:
            // the name of this type
            tstring m_Name;

            // The scene that owns us
            Core::Scene* m_Scene;

            // Index of image to use in the UI (from the 16x16 image list)
            i32 m_ImageIndex;

            // the instances that have this type
            HM_SceneNodeSmartPtr m_Instances;

            // compile time type id of the instances
            i32 m_InstanceType;


            //
            // Runtime Type Info
            //

        public:
            SCENE_DECLARE_TYPE( Core::SceneNodeType, Object );
            static void InitializeType();
            static void CleanupType();


            //
            // Implementation
            //

        public:
            SceneNodeType(Core::Scene* scene, i32 instanceType);
            virtual ~SceneNodeType();

            Core::Scene* GetScene();

            const tstring& GetName() const;
            void SetName( const tstring& name );

            i32 GetImageIndex() const;
            void SetImageIndex( i32 index );

            virtual void Reset();

            virtual void AddInstance(SceneNodePtr n);
            virtual void RemoveInstance(SceneNodePtr n);
            const HM_SceneNodeSmartPtr& GetInstances() const;

            i32 GetInstanceType() const;

            virtual void PopulateManifest( Asset::SceneManifest* manifest ) const;

        private:
            NodeTypeChangeSignature::Event m_NodeAdded;
        public:
            void AddNodeAddedListener( const NodeTypeChangeSignature::Delegate& listener )
            {
                m_NodeAdded.Add( listener );
            }
            void RemoveNodeAddedListener( const NodeTypeChangeSignature::Delegate& listener )
            {
                m_NodeAdded.Remove( listener );
            }

        private:
            NodeTypeChangeSignature::Event m_NodeRemoved;
        public:
            void AddNodeRemovedListener( const NodeTypeChangeSignature::Delegate& listener )
            {
                m_NodeRemoved.Add( listener );
            }
            void RemoveNodeRemovedListener( const NodeTypeChangeSignature::Delegate& listener )
            {
                m_NodeRemoved.Remove( listener );
            }
        };

        typedef Helium::SmartPtr< Core::SceneNodeType > SceneNodeTypePtr;

        typedef std::vector< Core::SceneNodeType* > V_SceneNodeTypeDumbPtr;

        typedef std::set< Core::SceneNodeType* > S_SceneNodeTypeDumbPtr;

        typedef stdext::hash_map< i32, S_SceneNodeTypeDumbPtr > HMS_TypeToSceneNodeTypeDumbPtr;

        typedef stdext::hash_map< tstring, SceneNodeTypePtr > HM_StrToSceneNodeTypeSmartPtr;
    }
}