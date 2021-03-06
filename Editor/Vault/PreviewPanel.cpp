#include "Precompile.h"
#include "PreviewPanel.h"

#include "VaultFrame.h"
#include "VaultSettings.h"

#include "Core/Asset/AssetClass.h"
#include "Core/Asset/Classes/Entity.h"
#include "Foundation/Component/ComponentHandle.h"
#include "Editor/ArtProvider.h"
#include "Editor/Controls/MenuButton.h"

using namespace Helium;
using namespace Helium::Editor;

PreviewPanel::PreviewPanel( VaultFrame* browserFrame )
: PreviewPanelGenerated( browserFrame, wxID_ANY )
, m_VaultFrame( browserFrame )
{
    m_PreviewWindow->SetVaultFrame( m_VaultFrame );

    m_Label->SetValue( TXT( "" ) );
}

PreviewPanel::~PreviewPanel()
{
}

void PreviewPanel::Preview( Asset::AssetClass* asset )
{
    if ( m_PreviewAsset.Ptr() != asset )
    {
        wxBusyCursor busyCursor;
        m_ContentFile.clear();
        m_PreviewAsset = asset;
        m_PreviewWindow->ClearScene();

        m_Label->SetValue( TXT( "" ) );

        if ( m_PreviewAsset.ReferencesObject() )
        {
            m_Label->SetValue( m_PreviewAsset->GetPath().Get() );

            Asset::EntityPtr entity = Reflect::ObjectCast< Asset::Entity >( asset );
            if ( entity.ReferencesObject() )
            {
                m_ContentFile = entity->GetPath().Get();
                }
            }

        if ( !m_ContentFile.empty() )
        {
            if ( !m_PreviewWindow->LoadScene( m_ContentFile ) )
            {
                // Do something?
            }
        }
    }
}

void PreviewPanel::DisplayReferenceAxis( bool display )
{
    m_PreviewWindow->DisplayReferenceAxis( display );
}

