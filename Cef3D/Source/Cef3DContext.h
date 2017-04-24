//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 21.04.2017
//---------------------------------------------------------------------------

#pragma once

namespace Cef3D
{
	class RootWindowManager;


	// Used to store global context in the browser process.
	class CEF3D_API MainContext
	{
	public:
		MainContext();
		MainContext(CefRefPtr<CefCommandLine> command_line);

		// MainContext members.
		
		std::string GetDefaultURL();
		bool UseWindowlessRendering();
		void PopulateSettings(CefSettings* settings);
		void PopulateBrowserSettings(CefBrowserSettings* settings);
		void PopulateOsrSettings(Cef3DOSRSettings* settings);
		RootWindowManager* GetRootWindowManager();

		// Cef3D methods 
		Cef3DBrowser* CreateCef3DBrowser(const Cef3DBrowserDefinition& Def);
		Cef3DBrowser* GetCef3DBrowser(CefRefPtr<CefBrowser> browser);
		
		bool Initialize(const CefMainArgs& args, const CefSettings& settings, CefRefPtr<CefApp> application, void* windows_sandbox_info);

		bool IsUsingViews() const
		{
			return UseViews;
		}

		void Shutdown();
		~MainContext();

	private:

		// Returns true if the context is in a valid state (initialized and not yet
		// shut down).
		bool InValidState() const 
		{
			return IsInitialized && !IsShuttingDown;
		}

		CefRefPtr<CefCommandLine> CmdLine;

		// Track context state. Accessing these variables from multiple threads is
		// safe because only a single thread will exist at the time that they're set
		// (during context initialization and shutdown).
		bool IsInitialized;
		bool IsShuttingDown;
		bool WindowLessRendering;
		bool UseViews;

		scoped_ptr<RootWindowManager> WndManager;
		std::list<Cef3DBrowser*> Cef3DBrowserList;

		// Used to verify that methods are called on the correct thread.
		base::ThreadChecker ThreadChecker;

		DISALLOW_COPY_AND_ASSIGN(MainContext);
	};
}