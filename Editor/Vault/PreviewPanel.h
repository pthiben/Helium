#pragma once

#include "VaultGenerated.h"

namespace Helium
{
    namespace Asset
    {
        class AssetClass;
        typedef Helium::SmartPtr< AssetClass > AssetClassPtr;
    }

    namespace Reflect
    {
        struct ElementChangeArgs;
    }

    namespace Editor
    {
        class VaultFrame;

        class PreviewPanel : public PreviewPanelGenerated
        {
        public:
            PreviewPanel( VaultFrame* browserFrame );
            virtual ~PreviewPanel();

            void Preview( Asset::AssetClass* asset );

            void DisplayReferenceAxis( bool display = true );

        private:
            VaultFrame* m_VaultFrame;
            Asset::AssetClassPtr m_PreviewAsset;
            tstring m_ContentFile;
        };
    }
}