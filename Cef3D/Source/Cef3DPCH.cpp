//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.cpp
// Date: 13.04.2017
//---------------------------------------------------------------------------

#include "Cef3DPCH.h"

CefRefPtr<Cef3D::Cef3DApplication> Cef3DBrowserApp = 0;
Cef3D::MainContext* GMainContext = 0;

using namespace Cef3D;
TMulticastDelegate<void(Cef3DBrowser*)> Cef3DDelegates::OnBrowserCreated;
TMulticastDelegate<void(Cef3DBrowser*)> Cef3DDelegates::OnBrowserClosing;
TMulticastDelegate<void(Cef3DBrowser*)> Cef3DDelegates::OnBrowserClosed;

TMulticastDelegate<void(const std::string&)> Cef3DDelegates::OnSetAddress;
TMulticastDelegate<void(const std::string&)> Cef3DDelegates::OnSetTitle;

TMulticastDelegate<void(CefRefPtr<CefImage>)> Cef3DDelegates::OnSetFavicon;

TMulticastDelegate<void(bool)> Cef3DDelegates::OnSetFullscreen;
TMulticastDelegate<void(bool,bool,bool)> Cef3DDelegates::OnSetLoadingState;
TMulticastDelegate<void(const std::vector<CefDraggableRegion>&)> Cef3DDelegates::OnSetDraggableRegions;
TMulticastDelegate<void(bool)> Cef3DDelegates::OnTakeFocus;
TMulticastDelegate<void(Cef3DBrowser*)> Cef3DDelegates::OnBeforeContextMenu;

TMulticastDelegate<void(Cef3DBrowser*)> Cef3DDelegates::OnAfterCreated;
TMulticastDelegate<void(Cef3DBrowser*)> Cef3DDelegates::OnBeforeClosed;
TMulticastDelegate<void(Cef3DBrowser*,bool)> Cef3DDelegates::OnPopupShow;
TMulticastDelegate<void(Cef3DBrowser*,Cef3DRect)> Cef3DDelegates::OnPopupSize;
TMulticastDelegate<void(Cef3DBrowser*, Cef3DOsrRenderType, std::vector<Cef3DRect>, const void*, int, int)> Cef3DDelegates::OnPaint;

Cef3D::Cef3DDefinition::Cef3DDefinition()
{
	std::string rootPath = Cef3DPaths::Root();
	std::string logPath = Cef3DPaths::Log();
	std::string subProcessPath;
	logPath = Cef3DPaths::Combine(logPath, "Cef3D.log");

#if PLATFORM_WINDOWS
	subProcessPath = Cef3DPaths::Combine(rootPath, "CefSubProcess.exe");
#else
	subProcessPath = Cef3DPaths::Combine(rootPath, "CefSubProcess");
#endif

	ChildProcessPath = subProcessPath;
	LogPath = logPath;
	LogLevel = Cef3DLogLevel::Default;
	UserAgent = "Cef3D-Default-UA";
	OffscreenRendering = false;
	UseCefLoop = false;
}

Cef3D::Cef3DBrowserDefinition::Cef3DBrowserDefinition()
{
	Type = Cef3D::Cef3DBrowserType::Offscreen;
}
