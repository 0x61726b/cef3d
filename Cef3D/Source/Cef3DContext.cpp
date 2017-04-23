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

		class ClientRequestContextHandler : public CefRequestContextHandler {
		public:
			ClientRequestContextHandler() {}

			bool OnBeforePluginLoad(const CefString& mime_type,
				const CefString& plugin_url,
				bool is_main_frame,
				const CefString& top_origin_url,
				CefRefPtr<CefWebPluginInfo> plugin_info,
				PluginPolicy* plugin_policy) OVERRIDE {
				// Always allow the PDF plugin to load.
				if (*plugin_policy != PLUGIN_POLICY_ALLOW &&
					mime_type == "application/pdf") {
					*plugin_policy = PLUGIN_POLICY_ALLOW;
					return true;
				}

				return false;
			}

		private:
			IMPLEMENT_REFCOUNTING(ClientRequestContextHandler);
		};
	}

	class TestDel :
		public Cef3DGenericBrowserWindow::Delegate
	{
	public:
		TestDel() { }

		virtual void OnBrowserCreated(Cef3DBrowser* browser)
		{

		}
	};
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
		UseViews(false)
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

	Cef3DBrowser* MainContext::CreateCef3DBrowser(const Cef3DBrowserDefinition& Def)
	{
		CEF_REQUIRE_UI_THREAD();

		CefBrowserSettings settings = Cef3DPrivate::Cef3DBrowserDefinitionToCef(Def);
		Cef3DBrowser* cef3DBrowser(new Cef3DBrowser);

		std::string testLoadUrl = "http://www.google.com";
		if (Def.Type == Cef3DBrowserType::Normal)
		{
			RootWindow* Wnd = GMainContext->GetRootWindowManager()->CreateRootWindow(false, CefRect(), testLoadUrl);
			cef3DBrowser->SetRootWindow(Wnd);
		}
		else
		{
			// Create browser
			CefWindowInfo windowInfo;
			windowInfo.SetAsWindowless(NULL, true);
			Cef3DGenericBrowserWindow* browserWindow(new Cef3DGenericBrowserWindow(new TestDel()));
			Cef3DOsrBrowser* osrWindow(new Cef3DOsrBrowser);

			CefRefPtr<Cef3DOsrHandler> handler(new Cef3DOsrHandler(browserWindow, osrWindow, testLoadUrl));

			browserWindow->client_handler_ = handler;
			
			/*CefRefPtr<CefBrowser> browser = */CefBrowserHost::CreateBrowser(windowInfo, handler, testLoadUrl, settings,
				/*CefRequestContext::CreateContext(CefRequestContext::GetGlobalContext(), new ClientRequestContextHandler)*/0);
			/*(browser);*/
		}
		Cef3DBrowserList.push_back(cef3DBrowser);
		
		return cef3DBrowser;
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

	bool MainContext::Initialize(const CefMainArgs& args, const CefSettings& settings, CefRefPtr<CefApp> application, void* windows_sandbox_info)
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