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

#include "Cef3DPCH.h"
#include "Cef3D.h"

using namespace Cef3D;

bool Cef3D_Init(const Cef3D::Cef3DDefinition& Definition, Cef3DAppDelegate* AppDelegate)
{
	// Enable High-DPI support on Windows 7 or newer.
	CefEnableHighDPISupport();
	GCmdLine = new Cef3DCommandLine(CefCommandLine::CreateCommandLine());

#if PLATFORM_WINDOWS
	CefMainArgs main_args(GetModuleHandle(NULL));
	GCmdLine->InitFromString(CefString(::GetCommandLineW()));
#else
	CefMainArgs main_args();
	command_line->InitFromString("");
#endif

	if (Definition.OffscreenRendering)
	{
		GCmdLine->AppendSwitch("off-screen-frame-rate=60");
		GCmdLine->AppendSwitch("off-screen-rendering-enabled");
	}
	GMainContext = new MainContext();

	CefSettings settings;
	if (Definition.OffscreenRendering)
		settings.windowless_rendering_enabled = true;
	settings.no_sandbox = true;

	if (Definition.UseChildProcess)
	{
		CefString(&settings.browser_subprocess_path).FromASCII(Definition.ChildProcessPath.c_str());
	}

	CefString(&settings.log_file).FromASCII(Definition.LogPath.c_str());
	settings.log_severity = Cef3D::Cef3DPrivate::Cef3DLogLevelToCef(Definition.LogLevel);

	GCef3DBrowserApp = new Cef3D::Cef3DBrowserApp(AppDelegate);

	GMainContext->PopulateSettings(&settings);

	return GMainContext->Initialize(main_args, settings, GCef3DBrowserApp, NULL,Definition);
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
	if(GMainContext->IsUsingCefLoop())
		CefRunMessageLoop();
	else
		CefDoMessageLoopWork();
}

bool Cef3D_CreateBrowser(Cef3D::Cef3DBrowser* browser, int Width, int Height, Cef3D::Cef3DBrowserType Type)
{
	Cef3D::Cef3DBrowserDefinition settings;
	settings.Rect = Cef3DRect(Width, Height);
	settings.Type = Type;

	return Cef3D_CreateBrowser(browser, settings);
}

bool Cef3D_CreateBrowser(Cef3D::Cef3DBrowser* browser, int X, int Y, int Width, int Height, const std::string & Url, Cef3D::Cef3DBrowserType Type)
{
	Cef3D::Cef3DBrowserDefinition settings;
	settings.DefaultUrl = Url;
	settings.Rect = Cef3DRect(X, Y, Width, Height);
	settings.Type = Type;
	return Cef3D_CreateBrowser(browser, settings);
}

bool Cef3D_CreateBrowser(Cef3D::Cef3DBrowser* browser, const Cef3D::Cef3DBrowserDefinition& Definition)
{
	return GMainContext->CreateCef3DBrowser(browser, Definition);
}

bool Cef3D_Shutdown()
{
	GMainContext->Shutdown();

	GCef3DBrowserApp = 0;

	delete GMainContext;
	GMainContext = 0;

	return true;
}