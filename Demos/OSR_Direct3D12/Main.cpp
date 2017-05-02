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
#include <Cef3DPCH.h>
#include "Direct3D12Renderer.h"

LRESULT CALLBACK RootWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char* lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	{
		int WinWidth = 1400;
		int WinHeight = 900;

		class RootWindowWndProc : public WndProcListener
		{
		public:
			LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override
			{
				return RootWindowProc(hWnd, message, wParam, lParam);
			}
		};

		InitWindowDefinition windowDef;
		windowDef.Width = WinWidth;
		windowDef.Height = WinHeight;
		windowDef.Instance = hInInstance;
		windowDef.Delegate = new RootWindowWndProc();

		Win32Window window;
		window.CreateNativeWindow(windowDef);

		Cef3DDirect3D12Renderer renderer;
		renderer.Init(&window);

		MSG msg;
		while (TRUE)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (msg.message == WM_QUIT)
					break;
			}
			renderer.Render();
			renderer.Present();
		}

    }
}