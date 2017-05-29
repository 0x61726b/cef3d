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
	Cef3DApplication::Cef3DApplication()
	{

	}

	void Cef3DApplication::RegisterDelegate(CefRefPtr<Delegate> Del)
	{
		DelegateList.insert(Del);
	}

	void Cef3DApplication::OnBeforeCommandLineProcessing(const CefString& process_type,CefRefPtr<CefCommandLine> command_line)
	{
		command_line->AppendSwitch("enable-experimental-web-platform-features");
		// Pass additional command-line flags to the browser process.
		if (process_type.empty())
		{
			// Pass additional command-line flags when off-screen rendering is enabled.
			if (true)
			{
				// If the PDF extension is enabled then cc Surfaces must be disabled for
				// PDFs to render correctly.
				// See https://bitbucket.org/chromiumembedded/cef/issues/1689 for details.
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
				}
			}

			if (command_line->HasSwitch("use-views") && !command_line->HasSwitch("top-chrome-md"))
			{
				// Use non-material mode on all platforms by default. Among other things
				// this causes menu buttons to show hover state. See usage of
				// MaterialDesignController::IsModeMaterial() in Chromium code.
				command_line->AppendSwitchWithValue("top-chrome-md", "non-material");
			}

			DelegateSet::iterator it = DelegateList.begin();
			for (; it != DelegateList.end(); ++it)
				(*it)->OnBeforeCommandLineProcessing(this, command_line);
		}
	}

	void Cef3DApplication::OnContextInitialized()
	{
		// Register cookieable schemes with the global cookie manager.
		CefRefPtr<CefCookieManager> manager =
			CefCookieManager::GetGlobalManager(NULL);
		DCHECK(manager.get());
		manager->SetSupportedSchemes(CookieableSchemes, NULL);

		DelegateSet::iterator it = DelegateList.begin();
		for (; it != DelegateList.end(); ++it)
			(*it)->OnContextInitialized(this);
	}

	void Cef3DApplication::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
	{
		DelegateSet::iterator it = DelegateList.begin();
		for (; it != DelegateList.end(); ++it)
			(*it)->OnBeforeChildProcessLaunch(this, command_line);
	}

	void Cef3DApplication::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
	{
		DelegateSet::iterator it = DelegateList.begin();
		for (; it != DelegateList.end(); ++it)
			(*it)->OnRenderProcessThreadCreated(this, extra_info);
	}

	void Cef3DApplication::OnScheduleMessagePumpWork(int64 delay)
	{
		
	}
	CefProcessType Cef3DApplication::GetProcessType(CefRefPtr<CefCommandLine> command_line)
	{
		if(!command_line->HasSwitch("type"))
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