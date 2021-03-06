#pragma once

#include "VaultFrame.h"
#include "VaultMenuIDs.h"
#include "Thumbnail.h"
#include "ThumbnailManager.h"
#include "ThumbnailTile.h"
#include "ThumbnailSorter.h"
#include "ThumbnailTileCreator.h"

#include "Platform/Compiler.h"
#include "Foundation/Memory/SmartPtr.h"
#include "Foundation/Math/Matrix4.h"
#include "Foundation/Math/Frustum.h"

#include "Core/Render/DeviceManager.h"

namespace Helium
{
    namespace Asset
    {
        class AssetFile;
        typedef Helium::SmartPtr< AssetFile > AssetFilePtr;
        typedef std::vector< AssetFilePtr > V_AssetFiles;
        typedef Helium::OrderedSet< AssetFilePtr > OS_AssetFiles;

        class AssetFolder;
        typedef Helium::SmartPtr< AssetFolder > AssetFolderPtr;
        typedef std::vector< AssetFolderPtr > V_AssetFolders;
    }

    namespace Editor
    {
        //
        // Forwards
        //
        class ThumbnailLoadedEvent;
        struct D3DEventArgs;

        class SearchResults;
        typedef Helium::SmartPtr< SearchResults > SearchResultsPtr;

        // Selection change event and arguments
        struct ThumbnailSelectionArgs
        {
            size_t m_NumSelected;

            ThumbnailSelectionArgs( size_t numSelected )
                : m_NumSelected( numSelected )
            {
            }
        };
        typedef Helium::Signature< void, const ThumbnailSelectionArgs& > ThumbnailSelectionSignature;

        // Highlight change event and arguments
        struct ThumbnailHighlightArgs
        {
            tstring m_HighlightPath;

            ThumbnailHighlightArgs( const tstring& highlight )
                : m_HighlightPath( highlight )
            {
            }
        };
        typedef Helium::Signature< void, const ThumbnailHighlightArgs& > ThumbnailHighlightSignature;


        /////////////////////////////////////////////////////////////////////////////
        typedef std::map< tstring, DWORD > M_FileTypeColors;
        typedef std::map< tstring, ThumbnailPtr > M_FileTypeIcons;

        typedef std::vector< Math::Vector3 > V_TileCorners;
        typedef std::map< DWORD, V_TileCorners > M_RibbonColorTileCorners;
        typedef std::map< Thumbnail*, V_TileCorners > M_FileTypeTileCorners;

        /////////////////////////////////////////////////////////////////////////////
        // Uses D3D to draw a thumbnail view of search results.  This consists of 
        // tiles for each item, with a screen shot and the name of the item.
        // 
        class ThumbnailView : public wxScrolledWindow
        {
        private:
            enum ContextMenuIDs
            {
                ID_ViewSmall = VaultMenu::ViewSmall,
                ID_ViewMedium = VaultMenu::ViewMedium,
                ID_ViewLarge = VaultMenu::ViewLarge,

                ID_CheckOut = VaultMenu::CheckOut,
                ID_History = VaultMenu::History,

                ID_CopyPathNative = VaultMenu::CopyPathNative,
                ID_CopyPath = VaultMenu::CopyPath,

                ID_Open = VaultMenu::Open,
                ID_Preview = VaultMenu::Preview,

                ID_Refresh = VaultMenu::Refresh,
                ID_Settings = VaultMenu::Settings,

                ID_New = VaultMenu::New,
                ID_NewFolder = VaultMenu::NewFolder,
                ID_Cut = VaultMenu::Cut,
                ID_Copy = VaultMenu::Copy,
                ID_Paste = VaultMenu::Paste,
                ID_Rename = VaultMenu::Rename,
                ID_Delete = VaultMenu::Delete,
                ID_Properties = VaultMenu::Properties,

                ID_SelectAll = VaultMenu::SelectAll,

                ID_Sort = VaultMenu::Sort,
                ID_SortByName = VaultMenu::SortByName,
                ID_SortByType = VaultMenu::SortByType,

                ID_ShowInFolders = VaultMenu::ShowInFolders,
                ID_ShowInPerforce = VaultMenu::ShowInPerforce,
                ID_ShowInWindows = VaultMenu::ShowInWindowsExplorer,

                ID_NewCollectionFromSel = VaultMenu::NewCollectionFromSelection,
                ID_NewDependencyCollectionFromSel = VaultMenu::NewDepedencyCollectionFromSelection,
                ID_NewUsageCollectionFromSel = VaultMenu::NewUsageCollectionFromSelection,
            };

        public:
            ThumbnailView( const tstring& thumbnailDirectory, VaultFrame *browserFrame, wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxNO_BORDER | wxFULL_REPAINT_ON_RESIZE | wxVSCROLL, const wxString& name = wxT( "Editor::ThumbnailView" ) );
            virtual ~ThumbnailView();

            void SetResults( SearchResults* results );
            void ClearResults();
            const SearchResults* GetResults() const;

            void SelectPath( const tstring& path );
            void GetSelectedPaths( std::set< Helium::Path >& paths );

            tstring GetHighlightedPath() const;

            void SetZoom( u16 zoom );

            ThumbnailSortMethod GetSortMethod() const;
            struct SortOptions
            {
                enum SortOption
                {
                    Refresh = 1 << 1,
                    Force   = 1 << 2,
                };
            };
            typedef SortOptions::SortOption SortOption;

            void Sort( ThumbnailSortMethod method = ThumbnailSortMethods::AlphabeticalByName, u32 sortOptions = SortOptions::Refresh | SortOptions::Force );

            virtual void Scroll( int x, int y ) HELIUM_OVERRIDE;

        private:
            void CreateTiles( SearchResults* results );

            friend ThumbnailTileCreator;
            void OnTilesCreated( const M_PathToTilePtr& tiles, const ThumbnailSorter& sorter, const std::set< Helium::Path >& textures );

            bool Select( ThumbnailTile* tile );
            bool Deselect( ThumbnailTile* tile );
            bool SetSelection( ThumbnailTile* tile, bool selected );
            bool ClearSelection();
            bool SelectRange( ThumbnailTile* first, ThumbnailTile* last );

            void Highlight( ThumbnailTile* tile );
            void ClearHighlight();

            bool IsVisible( ThumbnailTile* tile );
            void EnsureVisible( ThumbnailTile* tile );

            void UpdateProjectionMatrix();

            void CreateResources();
            void DeleteResources();

            void WorldToScreen( const Math::Vector3& p, float& x, float& y );
            void ViewportToScreen( const Math::Vector3& v, float& x, float& y );
            void ScreenToViewport( float x, float y, Math::Vector3& v ) const;
            void ViewportToWorldVertex( float x, float y, Math::Vector3& v ) const;

            void CalculateTotalItemSize();
            void CalculateTotalVisibleItems();

            void AdjustScrollBar( bool maintainScrollPos );

            void ShowContextMenu( const wxPoint& pos );

            ThumbnailTile* FindTile( const Helium::Path& path ) const;

            enum Corner
            {
                ThumbnailTopLeft,
                ThumbnailTopRight,
                ThumbnailBottomLeft,
                ThumbnailBottomRight,

                LabelTopLeft,
                LabelTopRight,
                LabelBottomLeft,
                LabelBottomRight,

                CORNER_COUNT,

                TopLeft = ThumbnailTopLeft,
                TopRight = ThumbnailTopRight,
                BottomLeft = LabelBottomLeft,
                BottomRight = LabelBottomRight,
            };
            void GetTileCorners( ThumbnailTile* tile, Math::Vector3 corners[] ) const;

            void EditTileLabel( ThumbnailTile* tile );

            void Pick( wxPoint mousePos1, wxPoint mousePos2, OS_ThumbnailTiles& hits );

            void InsertFileTypeIcon( IDirect3DDevice9* device, M_FileTypeIcons& fileTypeIcons, const tstring& type, const tchar* fileName );

            bool Draw();
            void DrawTile( IDirect3DDevice9* device, ThumbnailTile* tile, bool overlayOnly = false );
            void DrawTileOverlays( IDirect3DDevice9* device, V_TileCorners& tileCorners, Thumbnail* thumbnail );
            void DrawTileRibbons( IDirect3DDevice9* device, V_TileCorners& tileCorners, DWORD color );
            void DrawTileFileType( IDirect3DDevice9* device, V_TileCorners& tileCorners, Thumbnail* thumbnail );

            void OnPaint( wxPaintEvent& args );

            void OnSize( wxSizeEvent& args );

            void OnKeyDown( wxKeyEvent& args );

            void OnMouseWheel( wxMouseEvent& args );
            void MouseSelectionHelper();
            void OnMouseMove( wxMouseEvent& args );
            void OnMouseLeftDown( wxMouseEvent& args );
            void OnMouseLeftUp( wxMouseEvent& args );
            void OnMouseLeftDoubleClick( wxMouseEvent& args );
            void OnMouseRightDown( wxMouseEvent& args );
            void OnMouseLeave( wxMouseEvent& args );

            void OnScrollEvent( wxScrollWinEvent& args );

            void OnSelectAll( wxCommandEvent& args );

            void OnSortAlphabetical( wxCommandEvent& args );
            void OnSortByType( wxCommandEvent& args );
            void OnSort( wxCommandEvent& args );

            void OnFileProperties( wxCommandEvent& args );

            void OnRename( wxCommandEvent& args );

            void OnEditBoxLostFocus( wxFocusEvent& args );
            void OnEditBoxPressEnter( wxCommandEvent& args );

            void OnThumbnailLoaded( Editor::ThumbnailLoadedEvent& args );

            void OnVaultFrameClosing( wxCloseEvent& args );

            void OnReleaseResources( const Core::Render::DeviceStateArgs& args );
            void OnAllocateResources( const Core::Render::DeviceStateArgs& args );

        private:
            static const float s_NearClipDistance;
            static const float s_FarClipDistance;
            static const DWORD s_TextColorDefault;
            static const DWORD s_TextColorBGSelected;

            static const DWORD s_TextColorDark;
            static const float s_SpaceBetweenTileAndLabel;
            static const Math::Vector2 s_GapBetweenTiles;
            static const float s_ThumbnailSize;
            static const float s_MinThumbnailSize;
            static const float s_MaxThumbnailSize;

            Core::Render::DeviceManager m_DeviceManager;
            float m_LabelFontHeight;
            ID3DXFont* m_LabelFont;
            ID3DXFont* m_TypeFont;

            wxTextCtrl* m_EditCtrl;

            std::map<tstring, ThumbnailPtr> m_AssociatedIcons; // this guy is just for use in the bg thread
            ThumbnailPtr m_TextureMissing;
            ThumbnailPtr m_TextureError;
            ThumbnailPtr m_TextureLoading;
            ThumbnailPtr m_TextureFolder;
            ThumbnailPtr m_TextureOverlay;
            ThumbnailPtr m_TextureSelected;
            ThumbnailPtr m_TextureHighlighted;
            ThumbnailPtr m_TextureBlankFile;

            SearchResultsPtr m_Results;

            ThumbnailTileCreator m_TileCreator;

            tstring         m_ThumbnailDirectory;
            ThumbnailManager*   m_ThumbnailManager;
            std::set< Helium::Path > m_CurrentTextureRequests;

            M_PathToTilePtr m_Tiles;
            OS_ThumbnailTiles m_VisibleTiles;
            OS_ThumbnailTiles m_MouseOverTiles; // there will only be one
            OS_ThumbnailTiles m_SelectedTiles;

            M_FileTypeColors m_FileTypeColors;
            M_FileTypeIcons m_FileTypeIcons;

            V_TileCorners m_VisibleTileCorners;
            V_TileCorners m_HighlighedTileCorners;
            V_TileCorners m_SelectedTileCorners;
            M_RibbonColorTileCorners m_RibbonColorTileCorners;
            M_FileTypeTileCorners m_FileTypeTileCorners;


            ThumbnailSorter m_Sorter;

            bool m_MouseDown;
            wxPoint m_MouseDownLoc;
            ThumbnailTile* m_MouseDownTile;
            ThumbnailTile* m_RangeSelectTile;
            bool m_CtrlOnMouseDown;

            Math::Vector2 m_TotalItemSize;

            wxSize m_TotalVisibleItems; // Total number of items that can be displayed in x and y directions

            Math::Matrix4 m_World;
            Math::Matrix4 m_Orientation;
            Math::Matrix4 m_Projection;
            Math::Matrix4 m_ViewMatrix;
            Math::Frustum m_ViewFrustum;
            float m_Scale;

            VaultFrame* m_VaultFrame;

        private:
            ThumbnailSelectionSignature::Event m_SelectionChanged;
        public:
            void AddSelectionChangedListener( const ThumbnailSelectionSignature::Delegate& listener )
            {
                m_SelectionChanged.Add( listener );
            }
            void RemoveSelectionChangedListener( const ThumbnailSelectionSignature::Delegate& listener )
            {
                m_SelectionChanged.Remove( listener );
            }

        private:
            ThumbnailHighlightSignature::Event m_HighlightChanged;
        public:
            void AddHighlightChangedListener( const ThumbnailHighlightSignature::Delegate& listener )
            {
                m_HighlightChanged.Add( listener );
            }
            void RemoveHighlightChangedListener( const ThumbnailHighlightSignature::Delegate& listener )
            {
                m_HighlightChanged.Remove( listener );
            }

        private:
            DECLARE_EVENT_TABLE();
        };
    }
}