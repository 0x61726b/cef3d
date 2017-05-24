//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 21.04.2017
//---------------------------------------------------------------------------

#include "Cef3DPCH.h"

namespace Cef3D
{
	namespace {

	}

	namespace {

		// The default URL to load in a browser window.
		const char kDefaultUrl[] = "http://www.google.com";

		// Returns the ARGB value for |color|.
		cef_color_t ParseColor(const std::string& color) {
			std::string colorToLower;
			colorToLower.resize(color.size());
			std::transform(color.begin(), color.end(), colorToLower.begin(), ::tolower);

			if (colorToLower == "black")
				return CefColorSetARGB(255, 0, 0, 0);
			else if (colorToLower == "blue")
				return CefColorSetARGB(255, 0, 0, 255);
			else if (colorToLower == "green")
				return CefColorSetARGB(255, 0, 255, 0);
			else if (colorToLower == "red")
				return CefColorSetARGB(255, 255, 0, 0);
			else if (colorToLower == "white")
				return CefColorSetARGB(255, 255, 255, 255);

			// Use the default color.
			return 0U;
		}

	}  // namespace

	MainContext::MainContext()
	{
		DCHECK(!GMainContext);
		GMainContext = this;
	}


	MainContext::MainContext(CefRefPtr<CefCommandLine> command_line)
		: CmdLine(command_line),
		IsInitialized(false),
		IsShuttingDown(false),
		UseViews(false),
		UsingCefLoop(false)
	{
		DCHECK(CmdLine.get());

		// Whether windowless (off-screen) rendering will be used.
		WindowLessRendering = CmdLine->HasSwitch("off-screen-rendering-enabled");

#if PLATFORM_WINDOWS || PLATFORM_LINUX
		// Whether the Views framework will be used.
		UseViews = true;

		if (WindowLessRendering && UseViews) {
			LOG(ERROR) <<
				"Windowless rendering is not supported by the Views framework.";
			UseViews = false;
		}
#endif 
	}

	MainContext::~MainContext()
	{
		// The context must either not have been initialized, or it must have also
		// been shut down.
		DCHECK(!IsInitialized || IsShuttingDown);

		Cef3DBrowserList.clear();
	}
                                                

	std::string MainContext::GetDefaultURL()
	{
		return std::string("http://www.google.com");
	}

	bool MainContext::UseWindowlessRendering()
	{
		return WindowLessRendering;
	}

	void MainContext::PopulateSettings(CefSettings* settings)
	{
#if PLATFORM_WINDOWS
		settings->multi_threaded_message_loop = CmdLine->HasSwitch("multi-threaded-message-loop");
#endif

		CefString(&settings->cache_path) = CmdLine->GetSwitchValue("cache-path");

		if (WindowLessRendering)
			settings->windowless_rendering_enabled = true;
	}

	void MainContext::PopulateBrowserSettings(CefBrowserSettings* settings)
	{
		if (CmdLine->HasSwitch("off-screen-frame-rate")) {
			settings->windowless_frame_rate = atoi(CmdLine->GetSwitchValue("off-screen-frame-rate").ToString().c_str());
		}
	}

	void MainContext::PopulateOsrSettings(Cef3DOSRSettings* settings) {
		settings->Transparent = CmdLine->HasSwitch("transparent-painting-enabled");
		settings->ShowUpdateRects = CmdLine->HasSwitch("show-update-rect");
	}

	RootWindowManager* MainContext::GetRootWindowManager() 
	{
		DCHECK(InValidState());
		return WndManager.get();
	}

	bool MainContext::CreateCef3DBrowser(Cef3DBrowser* browser, const Cef3DBrowserDefinition& Def)
	{
		CEF_REQUIRE_UI_THREAD();

		CefBrowserSettings settings = Cef3DPrivate::Cef3DBrowserDefinitionToCef(Def);

		RootWindow* Window = GetRootWindowManager()->CreateRootWindow(Def);
		browser->SetRootWindow(Window);
		Cef3DBrowserList.push_back(browser);
		
		return true;
	}

	Cef3DBrowser* MainContext::GetCef3DBrowser(scoped_refptr<CefBrowser> browser)
	{
		return GetCef3DBrowser(GetRootWindowManager()->GetWindowForBrowser(browser->GetIdentifier()));
	}

	Cef3DBrowser* Cef3D::MainContext::GetCef3DBrowser(scoped_refptr<RootWindow> window)
	{
		std::list<Cef3DBrowser*>::iterator bit = Cef3DBrowserList.begin();
		for (;bit != Cef3DBrowserList.end(); ++bit)
		{
			if ((*bit)->AssociatedWindow->IsSame(window))
				return (*bit);
		}
		return 0;
	}

	CefRefPtr<CefBrowser> MainContext::GetCefBrowser(int id,bool isOsr)
	{
		DCHECK(true);
		return nullptr;
	}

	//Cef3DBrowser * MainContext::CreateCef3DBrowser(const Cef3DBrowserDefinition & Def, Cef3DHandlerDelegate * HandlerDelegate)
	//{
	//	return nullptr;
	//}

	//Cef3DBrowser* MainContext::CreateCef3DBrowser(const Cef3DBrowserDefinition & Def, Cef3DHandlerDelegate * HandlerDelegate, Cef3DOsrHandler::OsrDelegate * OsrDelegate)
	//{
	//	CEF_REQUIRE_UI_THREAD();

	//	CefBrowserSettings settings = Cef3DPrivate::Cef3DBrowserDefinitionToCef(Def);
	//	Cef3DBrowser* cef3DBrowser(new Cef3DBrowser);

	//	std::string testLoadUrl = "http://www.google.com";

	//	// Create browser
	//	CefWindowInfo windowInfo;
	//	if (Def.Type == Cef3DBrowserType::Offscreen)
	//	{
	//		Cef3DOsrHandler* handler(new Cef3DOsrHandler(HandlerDelegate, OsrDelegate, testLoadUrl));
	//		CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(windowInfo, handler, testLoadUrl, settings, NULL);
	//		cef3DBrowser->SetBrowserID(browser->GetIdentifier());
	//	}

	//	Cef3DBrowserList.push_back(cef3DBrowser);
	//	return cef3DBrowser;
	//}

	bool MainContext::Initialize(const CefMainArgs& args, const CefSettings& settings, CefRefPtr<CefApp> application, void* windows_sandbox_info, const Cef3DDefinition& Def)
	{
		DCHECK(ThreadChecker.CalledOnValidThread());
		DCHECK(!IsInitialized);
		DCHECK(!IsShuttingDown);

		if (!CefInitialize(args, settings, application, windows_sandbox_info))
			return false;

		// Need to create the RootWindowManager after calling CefInitialize because
		// TempWindowX11 uses cef_get_xdisplay().
		WndManager.reset(new RootWindowManager());

		IsInitialized = true;
		UsingCefLoop = Def.UseCefLoop;

		return true;
	}

	void MainContext::Shutdown()
	{
		DCHECK(ThreadChecker.CalledOnValidThread());
		DCHECK(IsInitialized);
		DCHECK(!IsShuttingDown);

		WndManager.reset();

		CefShutdown();

		IsShuttingDown = true;
	}
}