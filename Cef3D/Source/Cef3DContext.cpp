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

#include "Cef3DPCH.h"

namespace Cef3D
{

	namespace {

		// The default URL to load in a browser window.
		const char kDefaultUrl[] = "http://www.google.com";

		// Returns the ARGB value for |color|.
		cef_color_t ParseColor(const std::string& color) {
			std::string colorToLower;
			colorToLower.resize(color.size());
			std::transform(color.begin(), color.end(), colorToLower.begin(), ::tolower);

			if (colorToLower == "black")
				return CefColorSetARGB(255, 0, 0, 0);
			else if (colorToLower == "blue")
				return CefColorSetARGB(255, 0, 0, 255);
			else if (colorToLower == "green")
				return CefColorSetARGB(255, 0, 255, 0);
			else if (colorToLower == "red")
				return CefColorSetARGB(255, 255, 0, 0);
			else if (colorToLower == "white")
				return CefColorSetARGB(255, 255, 255, 255);

			// Use the default color.
			return 0U;
		}

	}  // namespace

	MainContext::MainContext() : terminate_when_all_windows_closed_(true) {
		DCHECK(!GMainContext);
		GMainContext = this;

	}


	MainContext::MainContext(CefRefPtr<CefCommandLine> command_line,
		bool terminate_when_all_windows_closed)
		: command_line_(command_line),
		terminate_when_all_windows_closed_(terminate_when_all_windows_closed),
		initialized_(false),
		shutdown_(false),
		background_color_(CefColorSetARGB(255, 255, 255, 255)),
		use_views_(false) {
		DCHECK(command_line_.get());

		// Set the main URL.
		if (command_line_->HasSwitch("url"))
			main_url_ = command_line_->GetSwitchValue("url");
		if (main_url_.empty())
			main_url_ = kDefaultUrl;

		if (command_line_->HasSwitch("background-color")) {
			// Parse the background color value.
			background_color_ =
				ParseColor(command_line_->GetSwitchValue("background-color"));
		}

		// Whether windowless (off-screen) rendering will be used.
		use_windowless_rendering_ =
			command_line_->HasSwitch("off-screen-rendering-enabled");

#if defined(OS_WIN) || defined(OS_LINUX)
		// Whether the Views framework will be used.
		use_views_ = true;

		if (use_windowless_rendering_ && use_views_) {
			LOG(ERROR) <<
				"Windowless rendering is not supported by the Views framework.";
			use_views_ = false;
		}

		if (use_views_ && command_line->HasSwitch("hide-frame") &&
			!command_line_->HasSwitch("url")) {
			// Use the draggable regions test as the default URL for frameless windows.
			main_url_ = "http://tests/draggable";
		}
#endif  // defined(OS_WIN) || defined(OS_LINUX)
	}

	MainContext::~MainContext() {
		// The context must either not have been initialized, or it must have also
		// been shut down.
		DCHECK(!initialized_ || shutdown_);
		/*GMainContext = NULL;*/
	}

	std::string MainContext::GetConsoleLogPath() {
		return GetAppWorkingDirectory() + "console.log";
	}

	std::string MainContext::GetMainURL() {
		return main_url_;
	}

	cef_color_t MainContext::GetBackgroundColor() {
		return background_color_;
	}

	std::string MainContext::GetDownloadPath(const std::string& file_name)
	{
		return "";
	}

	std::string MainContext::GetAppWorkingDirectory()
	{
		return "";
	}

	bool MainContext::UseViews() {
		return use_views_;
	}

	bool MainContext::UseWindowlessRendering() {
		return use_windowless_rendering_;
	}

	void MainContext::PopulateSettings(CefSettings* settings) {
#if defined(OS_WIN)
		settings->multi_threaded_message_loop =
			command_line_->HasSwitch("multi-threaded-message-loop");
#endif

		CefString(&settings->cache_path) =
			command_line_->GetSwitchValue("cache-path");

		if (use_windowless_rendering_)
			settings->windowless_rendering_enabled = true;

		settings->background_color = background_color_;
	}

	void MainContext::PopulateBrowserSettings(CefBrowserSettings* settings) {
		if (command_line_->HasSwitch("off-screen-frame-rate")) {
			settings->windowless_frame_rate = atoi(command_line_->
				GetSwitchValue("off-screen-frame-rate").ToString().c_str());
		}
	}

	void MainContext::PopulateOsrSettings(Cef3DOSRSettings* settings) {
		settings->Transparent =
			command_line_->HasSwitch("transparent-painting-enabled");
		settings->ShowUpdateRects=
			command_line_->HasSwitch("show-update-rect");
		//settings->background_color = background_color_;
	}

	RootWindowManager* MainContext::GetRootWindowManager() {
		DCHECK(InValidState());
		return root_window_manager_.get();
	}

	bool MainContext::Initialize(const CefMainArgs& args,
		const CefSettings& settings,
		CefRefPtr<CefApp> application,
		void* windows_sandbox_info) {
		DCHECK(thread_checker_.CalledOnValidThread());
		DCHECK(!initialized_);
		DCHECK(!shutdown_);

		if (!CefInitialize(args, settings, application, windows_sandbox_info))
			return false;

		// Need to create the RootWindowManager after calling CefInitialize because
		// TempWindowX11 uses cef_get_xdisplay().
		root_window_manager_.reset(
			new RootWindowManager(terminate_when_all_windows_closed_));

		initialized_ = true;

		return true;
	}

	void MainContext::Shutdown() {
		DCHECK(thread_checker_.CalledOnValidThread());
		DCHECK(initialized_);
		DCHECK(!shutdown_);

		root_window_manager_.reset();

		CefShutdown();

		shutdown_ = true;
	}
}