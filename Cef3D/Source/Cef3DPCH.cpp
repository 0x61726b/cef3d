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
CefRefPtr<Cef3D::Cef3DHandler> Cef3DBrowserHandler = 0;

Cef3D::Cef3DDefinition::Cef3DDefinition()
{
	std::string subProcessPath = Cef3D::Cef3DPaths::Root();
	std::string logPath = Cef3D::Cef3DPaths::Root();
	logPath.append("/Cef3D.log");


#if PLATFORM_WINDOWS
	subProcessPath.append("/CefSubProcess.exe");
#else
	subProcessPath.append("/CefSubProcess");
#endif

	ChildProcessPath = subProcessPath;
	LogPath = logPath;
	LogLevel = Cef3DLogLevel::Default;
	UserAgent = "Cef3D-Default-UA";
}

Cef3D::Cef3DBrowserDefinition::Cef3DBrowserDefinition()
{
	Width = 800;
	Height = 600;
	Type = Cef3D::Cef3DBrowserType::Normal;
}
