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

	// Used to store global context in the browser process. The methods of this
	// class are thread-safe unless otherwise indicated.
	class CEF3D_API MainContext {
	public:
		// Returns the singleton instance of this object.
		static MainContext* Get();

		// Returns the full path to the console log file.
		virtual std::string GetConsoleLogPath() = 0;

		// Returns the full path to |file_name|.
		virtual std::string GetDownloadPath(const std::string& file_name) = 0;

		// Returns the app working directory including trailing path separator.
		virtual std::string GetAppWorkingDirectory() = 0;

		// Returns the main application URL.
		virtual std::string GetMainURL() = 0;

		// Returns the background color.
		virtual cef_color_t GetBackgroundColor() = 0;

		// Returns true if the Views framework will be used.
		virtual bool UseViews() = 0;

		// Returns true if windowless (off-screen) rendering will be used.
		virtual bool UseWindowlessRendering() = 0;

		// Populate |settings| based on command-line arguments.
		virtual void PopulateSettings(CefSettings* settings) = 0;
		virtual void PopulateBrowserSettings(CefBrowserSettings* settings) = 0;
		virtual void PopulateOsrSettings(Cef3DOSRSettings* settings) = 0;

		// Returns the object used to create/manage RootWindow instances.
		virtual RootWindowManager* GetRootWindowManager() = 0;

	protected:
		MainContext();
		virtual ~MainContext();

	private:
		DISALLOW_COPY_AND_ASSIGN(MainContext);
	};


	// Used to store global context in the browser process.
	class CEF3D_API MainContextImpl : public MainContext {
	public:
		MainContextImpl(CefRefPtr<CefCommandLine> command_line,
			bool terminate_when_all_windows_closed);

		// MainContext members.
		std::string GetConsoleLogPath() OVERRIDE;
		std::string GetDownloadPath(const std::string& file_name) OVERRIDE;
		std::string GetAppWorkingDirectory() OVERRIDE;
		std::string GetMainURL() OVERRIDE;
		cef_color_t GetBackgroundColor() OVERRIDE;
		bool UseViews() OVERRIDE;
		bool UseWindowlessRendering() OVERRIDE;
		void PopulateSettings(CefSettings* settings) OVERRIDE;
		void PopulateBrowserSettings(CefBrowserSettings* settings) OVERRIDE;
		void PopulateOsrSettings(Cef3DOSRSettings* settings) OVERRIDE;
		RootWindowManager* GetRootWindowManager() OVERRIDE;

		// Initialize CEF and associated main context state. This method must be
		// called on the same thread that created this object.
		bool Initialize(const CefMainArgs& args,
			const CefSettings& settings,
			CefRefPtr<CefApp> application,
			void* windows_sandbox_info);

		// Shut down CEF and associated context state. This method must be called on
		// the same thread that created this object.
		void Shutdown();

	private:
		// Allow deletion via scoped_ptr only.
		friend struct base::DefaultDeleter<MainContextImpl>;

		~MainContextImpl();

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

		DISALLOW_COPY_AND_ASSIGN(MainContextImpl);
	};
}