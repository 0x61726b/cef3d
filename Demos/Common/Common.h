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

#include "Cef3DPlatform.h"

class WndProcListener;

enum RendererType
{
#if PLATFORM_WINDOWS
	Direct3D11,
	Direct3D12,
#endif
	OpenGL,
	Vulkan
};

enum ShaderType
{
	Vertex,
	Pixel
};

struct InitWindowDefinition
{
	InitWindowDefinition()
	{
		Width = 800;
		Height = 600;
		X = 0;
		Y = 0;
	}
	int Width;
	int Height;
	int X;
	int Y;
#if PLATFORM_WINDOWS
	HINSTANCE Instance;
	WndProcListener* Delegate;
#endif
};

/* Native window interface */
class Cef3DSampleWindow
{
public:
	virtual WindowHandle CreateNativeWindow(const InitWindowDefinition& def) = 0;
	virtual void Resize(int newWidth,int newHeight) = 0;

	int GetWidth() const { return Width; }
	int GetHeight() const { return Height; }

	WindowHandle GetNativeHandle() const { return Handle; }

#if PLATFORM_WINDOWS
	HINSTANCE GetInstance() const { return Instance;  }
#endif

protected:
	WindowHandle Handle;
#if PLATFORM_WINDOWS
	HINSTANCE Instance;
#endif
	int Width;
	int Height;
};

/* Renderer API interface
* All the renderer API needs to do is
* - Initialize like usual
* - Create 2D dynamic texture
* - Compile the full screen triangle shader
* - Render full screen triangle with the off-screen texture bound
* - Present
*/
class Cef3DSampleRenderer
{
public:
	Cef3DSampleRenderer(RendererType type) : Type(type) { }

	virtual bool Init(Cef3DSampleWindow* window) = 0;
	virtual bool InitResources() = 0;
	virtual void Render() = 0;
	virtual void Present() = 0;
	virtual void Shutdown() = 0;

	virtual void UpdateOffscreenTexture(const void* buffer,
		int width,
		int height) = 0;
	virtual bool Resize(int width, int height) = 0;

public:
	RendererType GetRendererType() { return Type; }
protected:
	RendererType Type;
};