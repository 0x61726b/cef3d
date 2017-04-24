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

#include <Cef3D.h>
#include <Windows.h>

//class TestDel :
//	public Cef3D::Cef3DGenericBrowserWindow::Delegate
//{
//public:
//	TestDel() { }
//
//	virtual void OnBrowserCreated(Cef3D::Cef3DBrowser* browser)
//	{
//
//	}
//};

//int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char* lpCmdLine, _In_ int nCmdShow)
//{
//	UNREFERENCED_PARAMETER(hPrevInstance);
//	UNREFERENCED_PARAMETER(lpCmdLine);
//
//	CefMainArgs main_args(hInInstance);
//
//	CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
//	command_line->InitFromString(::GetCommandLineW());
//
//	using namespace Cef3D;
//
//	CefRefPtr<CefApp> app;
//	if(!command_line->HasSwitch("type"))
//		app = (new Cef3D::Cef3DApplication);
//	else
//	{
//		const std::string& process_type = command_line->GetSwitchValue("type");
//		if (process_type == "renderer")
//			app = new Cef3D::Cef3DRenderer();
//	}
//	int exit_code = CefExecuteProcess(main_args, app, 0);
//	if (exit_code >= 0)
//		return exit_code;
//
//	GMainContext = (new MainContext(command_line));
//
//	CefSettings settings;
//	GMainContext->PopulateSettings(&settings);
//
//	//CefString(&settings.browser_subprocess_path).FromASCII("D:\\Arken\\C++\\cef3d\\Cef3D\\Binaries\\Win64\\CefSubProcess.exe");
//
//
//	GMainContext->Initialize(main_args, settings, app, 0);
//	Cef3DBrowserDefinition def;
//	def.Type = Cef3D::Cef3DBrowserType::Offscreen;
//	def.Width = 800;
//	def.Height = 600;
//	GMainContext->CreateCef3DBrowser(def);
//	/*GMainContext->GetRootWindowManager()->CreateRootWindow(false, CefRect(), "http://google.com");*/
//	// Create browser
//	/*CefBrowserSettings browserSettings;
//	CefWindowInfo windowInfo;
//	windowInfo.SetAsWindowless(NULL, true);
//	Cef3DGenericBrowserWindow* browserWindow(new Cef3DGenericBrowserWindow(new TestDel()));
//	Cef3DOsrBrowser* osrWindow(new Cef3DOsrBrowser);
//
//	CefRefPtr<Cef3DOsrHandler> handler(new Cef3DOsrHandler(browserWindow, osrWindow, "http://google.com"));
//
//	browserWindow->client_handler_ = handler;
//
//	CefBrowserHost::CreateBrowser(windowInfo, handler, "http://google.com", browserSettings, 0);*/
//
//	CefRunMessageLoop();
//
//	GMainContext->Shutdown();
//
//	delete GMainContext;
//
//	return 0;
//}
//
int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char* lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	bool isSubProcessed = true;

	Cef3D::Cef3DDefinition definition;
	definition.UseChildProcess = isSubProcessed;



	int returnCode = Cef3D_SubprocessLogic();
	if (returnCode >= 0)
		return returnCode;


	bool init = Cef3D_Init(definition);

	if (!init)
		return -1;

	Cef3D::Cef3DBrowser* browser1 = Cef3D_CreateBrowser(800, 600,Cef3D::Cef3DBrowserType::Offscreen);
	UNREFERENCED_PARAMETER(browser1);

	Cef3D_PumpMessageLoop();

	Cef3D_Shutdown();

	

	return 0;
}