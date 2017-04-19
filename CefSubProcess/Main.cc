//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Main.cc
// Date: 18.04.2017
//---------------------------------------------------------------------------

#include "include/cef_render_handler.h"
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"

#include <Cef3D.h>

int main(int argc,char* argv[]) 
{
#ifdef _WIN32
	CefMainArgs main_args(GetModuleHandle(NULL));
#else
	CefMainArgs main_args(argc,argv);
#endif
	CefRefPtr<Cef3D::Cef3DRenderer> app(new Cef3D::Cef3DRenderer());
	return CefExecuteProcess(main_args,app.get(),nullptr);
}