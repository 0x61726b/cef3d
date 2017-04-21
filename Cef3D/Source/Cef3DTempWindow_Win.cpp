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

namespace {

	const wchar_t kWndClass[] = L"Client_TempWindow";

	// Create the temp window.
	HWND CreateTempWindow() {
		HINSTANCE hInstance = ::GetModuleHandle(NULL);

		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.lpfnWndProc = DefWindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = kWndClass;
		RegisterClassEx(&wc);

		// Create a 1x1 pixel hidden window.
		return CreateWindow(kWndClass, 0,
			WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
			0, 0, 1, 1,
			NULL, NULL, hInstance, NULL);
	}

	Cef3D::Cef3DTempWindowWin* g_temp_window = NULL;

}  // namespace

namespace Cef3D
{

	Cef3DTempWindowWin::Cef3DTempWindowWin()
		: hwnd_(NULL) {
		DCHECK(!g_temp_window);
		g_temp_window = this;

		hwnd_ = CreateTempWindow();
		CHECK(hwnd_);
	}

	Cef3DTempWindowWin::~Cef3DTempWindowWin() {
		g_temp_window = NULL;
		DCHECK(hwnd_);
		DestroyWindow(hwnd_);
	}

	// static
	CefWindowHandle Cef3DTempWindowWin::GetWindowHandle() {
		DCHECK(g_temp_window);
		return g_temp_window->hwnd_;
	}
}