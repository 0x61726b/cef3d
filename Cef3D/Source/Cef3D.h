//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 19.04.2017
//---------------------------------------------------------------------------

#pragma once

#include "Cef3DPlatform.h"
#include "Cef3DBuild.h"
#include "Cef3DDefs.h"
#include "Cef3DFileSystem.h"
#include "Cef3DPaths.h"

namespace Cef3D
{
	class Cef3DBrowser;
}

CEF3D_API bool Cef3D_Init(const Cef3D::Cef3DDefinition& Definition);
CEF3D_API int Cef3D_SubprocessLogic();
CEF3D_API bool Cef3D_Shutdown();
CEF3D_API void Cef3D_PumpMessageLoop(bool isWindowless);


CEF3D_API Cef3D::Cef3DBrowser* Cef3D_CreateBrowser(int Width,int Height, Cef3D::Cef3DBrowserType Type = Cef3D::Cef3DBrowserType::Normal);
CEF3D_API Cef3D::Cef3DBrowser* Cef3D_CreateBrowser(const Cef3D::Cef3DBrowserDefinition& Definition);