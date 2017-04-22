//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.cpp
// Date: 19.04.2017
//---------------------------------------------------------------------------

#include "Cef3D.h"
#include "Cef3DPCH.h"

using namespace Cef3D;

bool Cef3D_Init(const Cef3D::Cef3DDefinition& Definition)
{
	// Enable High-DPI support on Windows 7 or newer.
	CefEnableHighDPISupport();
	CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();

#if PLATFORM_WINDOWS
	CefMainArgs main_args(GetModuleHandle(NULL));
	command_line->InitFromString(::GetCommandLineW());
#else
	CefMainArgs main_args();
	command_line->InitFromString("");
#endif

	GMainContext = (new MainContext(command_line, true));

	CefSettings settings;
	settings.no_sandbox = true;

	if (Definition.UseChildProcess)
	{
		CefString(&settings.browser_subprocess_path).FromASCII(Definition.ChildProcessPath.c_str());
	}
	
	CefString(&settings.log_file).FromASCII(Definition.LogPath.c_str());
	settings.log_severity = Cef3D::Cef3DPrivate::Cef3DLogLevelToCef(Definition.LogLevel);

	Cef3DBrowserApp = (new Cef3D::Cef3DApplication);

	GMainContext->PopulateSettings(&settings);
	
	return GMainContext->Initialize(main_args, settings, Cef3DBrowserApp, NULL);
}

int Cef3D_SubprocessLogic()
{
#if PLATFORM_WINDOWS
	CefMainArgs main_args(GetModuleHandle(NULL));
#else
	CefMainArgs main_args();
#endif

	return CefExecuteProcess(main_args, NULL, NULL);
}

void Cef3D_PumpMessageLoop()
{
	CefRunMessageLoop();
}

Cef3D::Cef3DBrowser* Cef3D_CreateBrowser(int Width, int Height, Cef3D::Cef3DBrowserType Type)
{
	Cef3D::Cef3DBrowserDefinition settings;
	settings.Width = Width;
	settings.Height = Height;

	return Cef3D_CreateBrowser(settings);
}

Cef3D::Cef3DBrowser* Cef3D_CreateBrowser(const Cef3D::Cef3DBrowserDefinition& Definition)
{
	//Cef3D::Cef3DBrowser* browser = new Cef3D::Cef3DBrowser;
	//browser->SetWidth(Definition.Width);
	//browser->SetHeight(Definition.Height);

	Cef3D::Cef3DBrowserDefinition settings;
	settings.Width = Definition.Width;
	settings.Height = Definition.Height;
	/*RootWindow* Window = */Cef3DBrowserApp->CreateBrowser(settings);
	/*browser->SetRootWindow(Window);*/
	
	return 0;
}

bool Cef3D_Shutdown()
{
	/*Cef3DBrowserApp->BrowserList.clear();*/
	/*GMainContext->Shutdown();*/
	/*delete GMainContext;*/

	GMainContext->Shutdown();

	delete GMainContext;
	GMainContext = 0;

	return true;
}