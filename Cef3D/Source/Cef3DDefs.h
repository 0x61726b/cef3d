//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 13.04.2017
//---------------------------------------------------------------------------

#pragma once

#define DECLARE_MULTICAST_DELEGATE_NOPARAM(Name) TMulticastDelegate<void()> Name
#define DECLARE_MULTICAST_DELEGATE_ONEPARAM(Name,Param0) TMulticastDelegate<void(Param0)> Name
#define DECLARE_MULTICAST_DELEGATE_TWOPARAM(Name,Param0,Param1) TMulticastDelegate<void(Param0,Param1)> Name
#define DECLARE_MULTICAST_DELEGATE_THREEPARAM(Name,Param0,Param1,Param2) TMulticastDelegate<void(Param0,Param1,Param2)> Name
#define DECLARE_MULTICAST_DELEGATE_FOURPARAM(Name,Param0,Param1,Param2,Param3) TMulticastDelegate<void(Param0,Param1,Param2,Param3)> Name
#define DECLARE_MULTICAST_DELEGATE_FIVEPARAM(Name,Param0,Param1,Param2,Param3,Param4) TMulticastDelegate<void(Param0,Param1,Param2,Param3,Param4)> Name
#define DECLARE_MULTICAST_DELEGATE_SIXPARAM(Name,Param0,Param1,Param2,Param3,Param4,Param5) TMulticastDelegate<void(Param0,Param1,Param2,Param3,Param4,Param5)> Name

namespace Cef3D
{
	class Cef3DBrowser;

	enum CefProcessType
	{
		ProcessBrowser,
		ProcessRenderer,
		ProcessOther
	};

	enum Cef3DLogLevel
	{
		Default,
		Verbose,
		Info,
		Warning,
		Error,
		None
	};

	enum Cef3DBrowserType
	{
		Offscreen,
		Normal
	};

	enum Cef3DOsrRenderType
	{
		View,
		Popup
	};

	struct Cef3DRect
	{
		int Width;
		int Height;

		Cef3DRect()
		{
			Width = -1;
			Height = -1;
		}

		Cef3DRect(int w, int h)
			: Width(w),
			Height(h)
		{

		}
	};

	struct Cef3DDefinition
	{
		Cef3DDefinition();

		Cef3DLogLevel LogLevel;
		std::string LogPath;
		std::string ChildProcessPath;
		std::string UserAgent;
		std::list<std::string> CmdSwitches;

		bool UseChildProcess;
		bool OffscreenRendering;
		bool UseCefLoop;
	};

	struct Cef3DBrowserDefinition
	{
		Cef3DBrowserDefinition();

		int Width;
		int Height;
		std::string DefaultUrl;
		Cef3DBrowserType Type;
	};

	struct Cef3DOSRSettings
	{
		bool Transparent;
		bool ShowUpdateRects;

		Cef3DRect Rect;
	};
}