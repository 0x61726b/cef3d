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

Cef3D::Cef3DDefinition::Cef3DDefinition()
{
	std::string rootPath = Cef3D::Cef3DPaths::Root();
	std::string logPath = Cef3D::Cef3DPaths::Log();
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
}

Cef3D::Cef3DBrowserDefinition::Cef3DBrowserDefinition()
{
	Width = 800;
	Height = 600;
	Type = Cef3D::Cef3DBrowserType::Normal;
}
