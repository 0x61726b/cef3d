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



class OsrPaintDelegate
	: public Cef3D::Cef3DOsrDel
{
public:
	virtual void OnAfterCreated(Cef3D::Cef3DBrowser* browser)
	{
	}

	virtual void OnBeforeClose(Cef3D::Cef3DBrowser* browser)
	{

	}

	virtual bool GetViewRect(Cef3D::Cef3DBrowser* browser,
		Cef3D::Cef3DRect& rect)
	{
		return true;
	}

	virtual void OnPaint(Cef3D::Cef3DBrowser* browser,
		Cef3D::Cef3DOsrRenderType type,
		const std::vector<Cef3D::Cef3DRect>& dirtyRects,
		const void* buffer,
		int width,
		int height)
	{

	}
};

int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char* lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	{
		bool isSubProcessed = true;
		Cef3D::Cef3DDefinition definition;
		definition.UseChildProcess = isSubProcessed;



		int returnCode = Cef3D_SubprocessLogic();
		if (returnCode >= 0)
			return returnCode;


		bool init = Cef3D_Init(definition);

		if (!init)
			return -1;

		/*scoped_ptr<Cef3D::Cef3DBrowser> browser1;
		browser1.reset((Cef3D_CreateBrowser(1400, 900)));

		scoped_ptr<Cef3D::Cef3DBrowser> browser2;
		browser2.reset((Cef3D_CreateBrowser(800, 600)));*/

		scoped_ptr<OsrPaintDelegate> PaintListener(new OsrPaintDelegate);

		Cef3D::Cef3DBrowserDefinition def;
		def.Width = 800;
		def.Height = 600;
		def.Type = Cef3D::Cef3DBrowserType::Offscreen;
		def.PaintDelegate = PaintListener.get();
		scoped_ptr<Cef3D::Cef3DBrowser> browser2;
		browser2.reset(Cef3D_CreateBrowser(def));

		Cef3D_PumpMessageLoop(false);

		Cef3D_Shutdown();
	}
	
	_CrtDumpMemoryLeaks();
	//VLDReportLeaks();

	return 0;
}