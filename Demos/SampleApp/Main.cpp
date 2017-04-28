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

#include <stdlib.h>  
#include <crtdbg.h> 
//#include <vld.h>
#include <Cef3D.h>
#include <Cef3DPCH.h>
#include <Windows.h>
#include <Cef3DEventSystem.h>

#include <functional>

struct TestEvents
{
	void OnBrowserCreated(Cef3D::Cef3DBrowser* browser)
	{

	}
};

//
int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char* lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	{
		bool isSubProcessed = true;
		Cef3D::Cef3DDefinition definition;
		definition.UseChildProcess = isSubProcessed;
		definition.UseCefLoop = true;


		int returnCode = Cef3D_SubprocessLogic();
		if (returnCode >= 0)
			return returnCode;


		bool init = Cef3D_Init(definition);

		if (!init)
			return -1;

		TestEvents t;
		Cef3D::Cef3DDelegates::OnBrowserCreated.Add(std::bind(&TestEvents::OnBrowserCreated, t,std::placeholders::_1));

		scoped_ptr<Cef3D::Cef3DBrowser> browser2;
		browser2.reset(Cef3D_CreateBrowser(800,600));

		Cef3D_PumpMessageLoop(false);

		Cef3D_Shutdown();
	}

	return 0;
}