#include "InspectColorPicker.h"
#include "Application/Inspect/Controls/Container.h"

#include "Application/UI/ColorPicker.h"
#include "Application/UI/SimpleConfig.h"

#include <sstream>

#include <wx/panel.h>

// Using
using namespace Helium::Inspect;

///////////////////////////////////////////////////////////////////////////////
// Class wrapping up the wxColourPickerCtrl.
// 
namespace Helium
{
    class StdColorPicker : public Helium::ColorPicker
    {
    public:
        Inspect::ColorPicker* m_ColorPicker;

        // Constructor
        StdColorPicker( wxWindow* parent, Inspect::ColorPicker* colorPicker )
            : Helium::ColorPicker( parent, wxID_ANY )
            , m_ColorPicker( colorPicker )
        {
            EnableAutoSaveCustomColors();
        };

        // Callback when the color is changed
        void OnChanged( wxColourPickerEvent& )
        {
            m_ColorPicker->Write();
        }

    private:
        DECLARE_EVENT_TABLE();
    };

    // Event table for ColorPicker
    BEGIN_EVENT_TABLE( StdColorPicker, Helium::ColorPicker )
        EVT_COLOURPICKER_CHANGED( wxID_ANY, StdColorPicker::OnChanged )
        END_EVENT_TABLE()

    class StdColorPickerWindow : public wxPanel
    {
    public:
        StdColorPicker* m_ColorPicker;

        StdColorPickerWindow( wxWindow* parent, Inspect::ColorPicker* colorPicker )
            : wxPanel( parent, wxID_ANY )
        {
            m_ColorPicker = new StdColorPicker( this, colorPicker );

            wxSize size( 24, 24 );
            m_ColorPicker->SetSize( size );
            m_ColorPicker->SetMinSize( size );
            m_ColorPicker->SetMaxSize( size );

            SetSizer( new wxBoxSizer( wxHORIZONTAL ) );
            wxSizer* sizer = GetSizer();
            sizer->Add( m_ColorPicker );
            sizer->Add( 1, 0, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL );
            Layout();
        }
    };
}
///////////////////////////////////////////////////////////////////////////////
// Constructor
// 
ColorPicker::ColorPicker()
: m_Alpha (false)
{
}

///////////////////////////////////////////////////////////////////////////////
// Called when the inner wxWidget is created.
// 
void ColorPicker::Realize(Container* parent)
{
    PROFILE_SCOPE_ACCUM( g_RealizeAccumulator );

    if ( !IsRealized() )
    {
        StdColorPickerWindow* picker = new StdColorPickerWindow( parent->GetWindow(), this );
        m_Window = picker;

        __super::Realize( parent );

        // Update the UI
        picker->m_ColorPicker->SetColour( m_Alpha ? wxColour( m_Color4.r, m_Color4.g, m_Color4.b, m_Color4.a ) : wxColour( m_Color3.r, m_Color3.g, m_Color3.b ) );
    }
}

///////////////////////////////////////////////////////////////////////////////
// Read from m_BoundData and write to the UI
// 
void ColorPicker::Read()
{
    if ( IsBound() && IsRealized() )
    {
        tstring str;
        ReadData( str );
        tstringstream stream( str );

        if (m_Alpha)
        {
            stream >> m_Color4;
        }
        else
        {
            stream >> m_Color3;
        }

        // Update the UI
        StdColorPickerWindow* picker = (StdColorPickerWindow*) m_Window;
        wxColourPickerCtrl* colorPicker = static_cast<wxColourPickerCtrl*>( picker->m_ColorPicker );
        colorPicker->SetColour( m_Alpha ? wxColour( m_Color4.r, m_Color4.g, m_Color4.b, m_Color4.a ) : wxColour( m_Color3.r, m_Color3.g, m_Color3.b ) );
    }
}

///////////////////////////////////////////////////////////////////////////////
// Write - get the values from the UI and push into m_BoundData.
// 
bool ColorPicker::Write()
{
    bool result = false;

    wxColour color = static_cast<StdColorPickerWindow*>(GetWindow())->m_ColorPicker->GetColour();

    if ( m_Alpha )
    {
        m_Color4 = Math::Color4( color.Red(), color.Green(), color.Blue(), color.Alpha() );
    }
    else
    {
        m_Color3 = Math::Color3( color.Red(), color.Green(), color.Blue() );
    }

    if ( IsBound() )
    {
        tstringstream stream;

        if ( m_Alpha )
        {
            stream << m_Color4;
        }
        else
        {
            stream << m_Color3;
        }

        WriteData( stream.str() );

        result = true;
    }

    return result;
}

void ColorPicker::UseAlpha( bool alpha )
{
    if (!IsRealized())
    {
        m_Alpha = alpha;
    }
    else
    {
        HELIUM_BREAK();
    }
}

void ColorPicker::SetColor3( const Helium::Math::Color3& color )
{
    if (!m_Alpha)
    {
        m_Color3 = color;

        if ( IsBound() )
        {
            tstringstream stream;
            stream << m_Color3;
            WriteData( stream.str() );
        }

        static_cast<StdColorPickerWindow*>(GetWindow())->m_ColorPicker->SetColour( wxColour ( m_Color3.r, m_Color3.g, m_Color3.b ) );
    }
    else
    {
        HELIUM_BREAK();
    }
}

const Helium::Math::Color3& ColorPicker::GetColor3() const
{
    if (!m_Alpha)
    {
        if ( IsBound() )
        {
            tstring str;
            ReadData( str );
            tstringstream stream( str );
            stream >> (Math::Color3)m_Color3;
        }
    }
    else
    {
        HELIUM_BREAK();
    }

    return m_Color3;
}

void ColorPicker::SetColor4( const Helium::Math::Color4& color )
{
    if (m_Alpha)
    {
        m_Color4 = color;

        if ( IsBound() )
        {
            tstringstream stream;
            stream << m_Color4;
            WriteData( stream.str() );
        }

        static_cast<StdColorPickerWindow*>(GetWindow())->m_ColorPicker->SetColour( wxColour ( m_Color4.r, m_Color4.g, m_Color4.b, m_Color4.a ) );
    }
    else
    {
        HELIUM_BREAK();
    }
}

const Helium::Math::Color4& ColorPicker::GetColor4() const
{
    if (m_Alpha)
    {
        if ( IsBound() )
        {
            tstring str;
            ReadData( str );
            tstringstream stream( str );
            stream >> (Math::Color4)m_Color4;
        }
    }
    else
    {
        HELIUM_BREAK();
    }

    return m_Color4;
}
