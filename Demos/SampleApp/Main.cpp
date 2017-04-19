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

	Cef3D::Cef3DDefinition definition;

	bool init = Cef3D_Init(definition);

	if (!init)
		return -1;

	Cef3D::Cef3DBrowser* browser = Cef3D_CreateBrowser(800, 600);
	UNREFERENCED_PARAMETER(browser);


	Cef3D_PumpMessageLoop();

	Cef3D_Shutdown();

	

	return 0;
}