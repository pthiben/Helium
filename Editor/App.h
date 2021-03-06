#pragma once

#include "Platform/Compiler.h"
#include "Platform/Thread.h"

#include "Application/DocumentManager.h"

#include "Core/SettingsManager.h"

#include "Foundation/InitializerStack.h"

#include "Editor/MainFrame.h"
#include "Editor/Tracker/Tracker.h"
#include "Editor/Vault/Vault.h"
#include "Editor/Settings.h"

#include <wx/app.h>
#include <wx/xrc/xmlres.h>

namespace Helium
{
    namespace Editor
    {
        class App : public wxApp
        {
        public:
            App();
            ~App();

            virtual bool    OnInit() HELIUM_OVERRIDE;
            virtual int     OnExit() HELIUM_OVERRIDE;
            virtual void    OnAssertFailure(const wxChar *file, int line, const wxChar *func, const wxChar *cond, const wxChar *msg) HELIUM_OVERRIDE;

            void SaveSettings();
            void LoadSettings();

            Core::SettingsManagerPtr& GetSettingsManager()
            {
                return m_SettingsManager;
            }

            Settings* GetSettings()
            {
                return m_Settings;
            }

            MainFrame* GetFrame()
            {
                if ( !m_Frame )
                {
                    m_Frame = new MainFrame();
                }
                return m_Frame;
            }

            Vault& GetVault()
            {
                if ( !m_Vault )
                {
                    m_Vault = new Vault();
                }
                return *m_Vault;
            }

        protected:
            Helium::InitializerStack m_InitializerStack;
            Tracker m_Tracker;
            Helium::Thread m_TrackerThread;

            Core::SettingsManagerPtr m_SettingsManager;
            SettingsPtr m_Settings;
            Vault* m_Vault;
            MainFrame* m_Frame;
        };

        DECLARE_APP( App );
    }
}