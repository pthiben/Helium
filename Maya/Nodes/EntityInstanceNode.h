#pragma once

#include "API.h"
#include "Foundation/TUID.h"
#include "Core/Content/Nodes/ContentEntityInstance.h"

#include <maya/MDagMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MPxTransform.h>

namespace Helium
{
    class EntityNode;

    class EntityInstanceNode;
    typedef std::vector< EntityInstanceNode* > V_EntityNodePtr;


    //
    // An instance of an art class
    //

    class MAYA_API EntityInstanceNode : public MPxTransform
    {  
    public: 
        // type information
        static const char* s_TypeName; 
        static const MTypeId s_TypeID; 

        // callbacks
        static MIntArray s_CallbackIDs; 
        static MStatus AddCallbacks();
        static MStatus RemoveCallbacks();

        // attributes
        static MObject        s_ArtFilePath;

        // public statics
        static bool           s_ReplaceSelection;
        static MObject        s_EntityNodeGroup;
        static Math::Matrix4  s_RelativeTransform;

    private:
        // private static internals
        static MColor         s_DrawColor;
        static MPointArray    s_PointerPoints;
        static bool           s_PointerYUp;

        static V_EntityNodePtr s_ShowNodes;
        static V_EntityNodePtr s_DupeNodes;

    private:
        // private non-static members
        MPlug                 m_Plug;
        Asset::EntityInstancePtr      m_Entity;
        Helium::TUID        m_UID; // cache the TUID here so we don't have to look it up on the backing entityPtr each time we do a comparison
        MCallbackId           m_AttributeChangedCB;
        MCallbackId           m_ChildAddedCB;

    public:
        EntityInstanceNode();
        virtual ~EntityInstanceNode(); 

        static void*            Creator();
        static MStatus			    Initialize();
        virtual void			      postConstructor();
        virtual void            copyInternalData( MPxNode* node );

        virtual bool            isBounded() const;
        virtual bool			      isAbstractClass() const;
        virtual bool			      excludeAsLocator() const; 

        virtual MStatus   		  compute( const MPlug& plug, MDataBlock& data );
        virtual void            draw( M3dView& view, const MDagPath& path, M3dView::DisplayStyle style, M3dView::DisplayStatus status );

        const Helium::TUID& GetUID() const
        {
            return m_UID;
        }

        const Asset::EntityInstancePtr& GetEntity() const
        {
            return m_Entity;
        }

        bool operator < ( const EntityInstanceNode& rhs )
        {
            return ( m_UID < rhs.GetUID() );
        }

        void GetArtFilePath( MString& artFilePath );

        const Asset::EntityInstancePtr& GetBackingEntity()
        {
            return m_Entity;
        }

        void SetBackingEntity( const Asset::EntityInstancePtr& entity );
        void UpdateBackingEntity();

        void Show( const EntityNode& instanceClassNode );
        void Show();
        void Hide();

        void Flatten();
        void Unselect( MSelectionList& list );

        static void NodeAddedCallback( MObject& node, void* clientData );
        static void NodeRemovedCallback( MObject& node, void* clientData );

        void AddAttributeChangedCallback();
        void RemoveAttributeChangedCallback();
        static void AttributeChangedCallback( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug, void* clientData );

        static void AfterDuplicateCallback( void* clientData );
        static void SelectionChangedCallback( void* clientData );
        static void AllDagChangesCallback( MDagMessage::DagMessage dagMsg, MDagPath& child, MDagPath &parent, void* clientData );
    };

    typedef std::map< Helium::TUID, EntityInstanceNode* > M_EntityNode;

}