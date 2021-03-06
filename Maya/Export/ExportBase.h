#pragma once
#include "Foundation/Profile.h"
#include "API.h"

#include "Core/Content/ContentScene.h"

#include "Platform/Types.h"
#include "Foundation/Memory/SmartPtr.h"

namespace Helium
{
    namespace MayaContent
    {
        class ExportBase;

        typedef Helium::SmartPtr<ExportBase> ExportBasePtr;
        typedef std::vector<ExportBasePtr> V_ExportBase;

        class MAYA_API ExportBase : public Helium::RefCountBase<ExportBase>
        {
        protected:
            // Maya representation of what we are exporting
            MObject m_MayaObject;

            // Content object that the export data gets processed into
            Content::SceneNodePtr m_ContentObject;

        public:
            ExportBase()
            {

            }

            ExportBase( const MObject& mayaObject )
                : m_MayaObject( mayaObject )
            {

            }

            ExportBase( const Content::SceneNodePtr& contentObject )
                : m_ContentObject( contentObject )
            {

            }

            ExportBase( const MObject& mayaObject, const Content::SceneNodePtr& contentObject ) 
                : m_MayaObject( mayaObject )
                , m_ContentObject( contentObject )
            {

            }

            virtual ~ExportBase()
            {

            }

            // Gather the necessary maya data
            virtual void GatherMayaData( V_ExportBase &newExportObjects );

            virtual void ProcessMayaData()
            {

            }

            inline const Content::SceneNodePtr GetContentObject()
            {
                return m_ContentObject;
            }
        };
    }
}