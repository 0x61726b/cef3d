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

// See Cef3DEventSystem.h
#define CEF3D_BIND(Fnc,Instance) Cef3DBind(Fnc,Instance)

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

	enum Cef3DMouseEventType
	{
		CEF3D_LBUTTON_DOWN,
		CEF3D_RBUTTON_DOWN,
		CEF3D_MBUTTON_DOWN,
		CEF3D_LBUTTON_UP,
		CEF3D_RBUTTON_UP,
		CEF3D_MBUTTON_UP,
		CEF3D_MOUSE_MOVE,
		CEF3D_MOUSE_LEAVE
	};

	enum Cef3DKeyEventType
	{
		CEF3D_KEY_RAWKEYDOWN,
		CEF3D_KEY_UP,
		CEF3D_KEY_CHAR
	};

	enum CefEventFlags 
	{
		EVENTFLAG_NONE = 0,
		EVENTFLAG_CAPS_LOCK_ON = 1 << 0,
		EVENTFLAG_SHIFT_DOWN = 1 << 1,
		EVENTFLAG_CONTROL_DOWN = 1 << 2,
		EVENTFLAG_ALT_DOWN = 1 << 3,
		EVENTFLAG_LEFT_MOUSE_BUTTON = 1 << 4,
		EVENTFLAG_MIDDLE_MOUSE_BUTTON = 1 << 5,
		EVENTFLAG_RIGHT_MOUSE_BUTTON = 1 << 6,
		// Mac OS-X command key.
		EVENTFLAG_COMMAND_DOWN = 1 << 7,
		EVENTFLAG_NUM_LOCK_ON = 1 << 8,
		EVENTFLAG_IS_KEY_PAD = 1 << 9,
		EVENTFLAG_IS_LEFT = 1 << 10,
		EVENTFLAG_IS_RIGHT = 1 << 11,
	};

	enum Cef3DIPCMessageType
	{
		CREATE_OBJECT_REQUEST,
		CREATE_OBJECT_RESPONSE,

		SET_VALUE_REQUEST,
		SET_VALUE_RESPONSE
	};

	struct Cef3DIPCMessageResult
	{
		bool Result;
		int AssociatedObjectId;
	};

	struct Cef3DRect
	{
		int Width;
		int Height;
		int X;
		int Y;

		Cef3DRect()
		{
			Width = -1;
			Height = -1;
			X = -1;
			Y = -1;
		}

		Cef3DRect(int w, int h)
			: Width(w),
			Height(h)
		{
			X = 0;
			Y = 0;
		}

		Cef3DRect(int x, int y, int w, int h)
			: Width(w), Height(h), X(x), Y(y)
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

		Cef3DRect Rect;
		std::string DefaultUrl;
		Cef3DBrowserType Type;

		WindowHandle ParentHandle;

		bool LoadImmediately;
	};

	struct Cef3DOSRSettings
	{
		bool Transparent;
		bool ShowUpdateRects;
		bool IsPopup;


		Cef3DRect Rect;
	};
}