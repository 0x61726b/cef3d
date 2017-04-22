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
		MainContext(CefRefPtr<CefCommandLine> command_line,
			bool terminate_when_all_windows_closed);

		// MainContext members.
		std::string GetConsoleLogPath();
		std::string GetDownloadPath(const std::string& file_name);
		std::string GetAppWorkingDirectory();
		std::string GetMainURL();
		cef_color_t GetBackgroundColor();
		bool UseViews();
		bool UseWindowlessRendering();
		void PopulateSettings(CefSettings* settings);
		void PopulateBrowserSettings(CefBrowserSettings* settings);
		void PopulateOsrSettings(Cef3DOSRSettings* settings);
		RootWindowManager* GetRootWindowManager();

		// Initialize CEF and associated main context state. This method must be
		// called on the same thread that created this object.
		bool Initialize(const CefMainArgs& args,
			const CefSettings& settings,
			CefRefPtr<CefApp> application,
			void* windows_sandbox_info);

		// Shut down CEF and associated context state. This method must be called on
		// the same thread that created this object.
		void Shutdown();
		~MainContext();

	private:
		// Allow deletion via scoped_ptr only.
		//friend struct base::DefaultDeleter<MainContextImpl>;

		// Returns true if the context is in a valid state (initialized and not yet
		// shut down).
		bool InValidState() const {
			return initialized_ && !shutdown_;
		}

		CefRefPtr<CefCommandLine> command_line_;
		const bool terminate_when_all_windows_closed_;

		// Track context state. Accessing these variables from multiple threads is
		// safe because only a single thread will exist at the time that they're set
		// (during context initialization and shutdown).
		bool initialized_;
		bool shutdown_;

		std::string main_url_;
		cef_color_t background_color_;
		bool use_windowless_rendering_;
		bool use_views_;

		scoped_ptr<RootWindowManager> root_window_manager_;

		// Used to verify that methods are called on the correct thread.
		base::ThreadChecker thread_checker_;

		DISALLOW_COPY_AND_ASSIGN(MainContext);
	};
}