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

int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char* lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Enable High-DPI support on Windows 7 or newer.
	CefEnableHighDPISupport();

	CefMainArgs main_args(hInInstance);

	int exit_code = CefExecuteProcess(main_args, 0, 0);
	if (exit_code >= 0) {
		return exit_code;
	}

	CefSettings settings;

	CefRefPtr<Cef3D::Cef3DApplication> app(new Cef3D::Cef3DApplication);

	// Initialize CEF.
	CefInitialize(main_args, settings, app.get(), NULL);

	CefRunMessageLoop();

	CefShutdown();
	return 0;
}