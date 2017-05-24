//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DGlobals.h
// Date: 13.04.2017
//---------------------------------------------------------------------------

#pragma once

extern CEF3D_API CefRefPtr<Cef3D::Cef3DApplication> Cef3DBrowserApp;
extern CEF3D_API Cef3D::MainContext* GMainContext;
//extern CEF3D_API CefRefPtr<Cef3D::Cef3DHandler> Cef3DBrowserHandler;

namespace Cef3D
{
	/*class Cef3DBrowser;*/

	class Cef3DDelegates
	{
	public:
		// Browser Process Delegates
		//static DECLARE_MULTICAST_DELEGATE_ONEPARAM(OnBrowserCreated, Cef3DBrowser*);
		//static DECLARE_MULTICAST_DELEGATE_ONEPARAM(OnBrowserClosing, Cef3DBrowser*);
		//static DECLARE_MULTICAST_DELEGATE_ONEPARAM(OnBrowserClosed, Cef3DBrowser*);
		//static DECLARE_MULTICAST_DELEGATE_ONEPARAM(OnSetAddress, const std::string&);
		//static DECLARE_MULTICAST_DELEGATE_ONEPARAM(OnSetTitle, const std::string&);
		//static DECLARE_MULTICAST_DELEGATE_ONEPARAM(OnSetFavicon, CefRefPtr<CefImage>);
		//static DECLARE_MULTICAST_DELEGATE_ONEPARAM(OnSetFullscreen, bool);
		//static DECLARE_MULTICAST_DELEGATE_THREEPARAM(OnSetLoadingState, bool,bool,bool);
		//static DECLARE_MULTICAST_DELEGATE_ONEPARAM(OnSetDraggableRegions, const std::vector<CefDraggableRegion>&);
		//static DECLARE_MULTICAST_DELEGATE_ONEPARAM(OnTakeFocus, bool);
		//static DECLARE_MULTICAST_DELEGATE_ONEPARAM(OnBeforeContextMenu, Cef3DBrowser*);

		// OSR Delegate
		


	};
}