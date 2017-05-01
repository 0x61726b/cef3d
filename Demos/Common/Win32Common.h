//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 1.5.2017
//---------------------------------------------------------------------------

#pragma once

#include "Common.h"

#define D3D_SAFE_RELEASE(p) if (p) { ((IUnknown*)p)->Release();  p = 0; }

class WndProcListener
{
public:
	virtual LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
};

class Win32Window : public Cef3DSampleWindow
{
public:
	HWND CreateNativeWindow(const InitWindowDefinition&) override;
	void Resize(int newWidth, int newHeight) override;

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	WndProcListener* Delegate;
};