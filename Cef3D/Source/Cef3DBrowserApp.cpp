//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DGlobals.h
// Date: 13.04.2017
//---------------------------------------------------------------------------

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"

#include "Cef3DPCH.h"

namespace Cef3D
{
	Cef3DBrowserApp::Cef3DBrowserApp(Cef3DAppDelegate* appDelegate)
	{
		AppDelegate = appDelegate;
	}

	void Cef3DBrowserApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
	{
		command_line->AppendSwitch("enable-experimental-web-platform-features");
		
		if (process_type.empty())
		{
			if (!command_line->HasSwitch("disable-extensions") && !command_line->HasSwitch("disable-pdf-extension"))
			{
				command_line->AppendSwitch("disable-surfaces");
			}

			// Use software rendering and compositing (disable GPU) for increased FPS
			// and decreased CPU usage. This will also disable WebGL so remove these
			// switches if you need that capability.
			// See https://bitbucket.org/chromiumembedded/cef/issues/1257 for details.
			if (!command_line->HasSwitch("enable-gpu"))
			{
				command_line->AppendSwitch("disable-gpu");
				command_line->AppendSwitch("disable-gpu-compositing");
				command_line->AppendSwitch("off-screen-rendering-enabled");
				command_line->AppendSwitch("off-screen-frame-rate=60"); 

			}
		}

		if (command_line->HasSwitch("use-views") && !command_line->HasSwitch("top-chrome-md"))
		{
			// Use non-material mode on all platforms by default. Among other things
			// this causes menu buttons to show hover state. See usage of
			// MaterialDesignController::IsModeMaterial() in Chromium code.
			command_line->AppendSwitchWithValue("top-chrome-md", "non-material");
		}
		AppDelegate->OnBeforeCommandLineProcessing(GetProcessType(command_line), Cef3DCommandLine(command_line));
	}

	void Cef3DBrowserApp::OnContextInitialized()
	{
		AppDelegate->OnContextInitialized();
	}

	void Cef3DBrowserApp::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
	{
		Cef3DCommandLine specialCmdLine(command_line);
		AppDelegate->OnBeforeChildProcessLaunch(specialCmdLine);
	}

	void Cef3DBrowserApp::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
	{
		AppDelegate->OnRenderProcessThreadCreated();
	}

	void Cef3DBrowserApp::OnScheduleMessagePumpWork(int64 delay)
	{
		AppDelegate->OnScheduleMessagePumpWork();
	}

	CefProcessType Cef3DBrowserApp::GetProcessType(CefRefPtr<CefCommandLine> command_line)
	{
		if (!command_line->HasSwitch("type"))
			return CefProcessType::ProcessBrowser;

		const std::string& process_type = command_line->GetSwitchValue("type");
		if (process_type == "renderer")
			return CefProcessType::ProcessRenderer;
#if PLATFORM_LINUX
		else if (process_type == "zygote")
			return CefProcessType::ProcessOther;
#endif

		return CefProcessType::ProcessOther;
	}
}