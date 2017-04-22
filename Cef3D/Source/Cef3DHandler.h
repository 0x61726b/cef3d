//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DHandler.h
// Date: 14.04.2017
//---------------------------------------------------------------------------

#pragma once

#include "include/cef_client.h"
#include "include/wrapper/cef_message_router.h"
#include "include/wrapper/cef_helpers.h"
#include "include/base/cef_bind.h"
#include "include/base/cef_scoped_ptr.h"
#include "include/cef_task.h"



namespace Cef3D
{
	class Cef3DBrowser;

	class Cef3DHandlerDelegate
	{
	public:
		// Called when the browser is created.
		virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) = 0;

		// Called when the browser is closing.
		virtual void OnBrowserClosing(CefRefPtr<CefBrowser> browser) = 0;

		// Called when the browser has been closed.
		virtual void OnBrowserClosed(CefRefPtr<CefBrowser> browser) = 0;

		// Set the window URL address.
		virtual void OnSetAddress(const std::string& url) = 0;

		// Set the window title.
		virtual void OnSetTitle(const std::string& title) = 0;

		// Set the Favicon image.
		virtual void OnSetFavicon(CefRefPtr<CefImage> image) {};

		// Set fullscreen mode.
		virtual void OnSetFullscreen(bool fullscreen) = 0;

		// Set the loading state.
		virtual void OnSetLoadingState(bool isLoading,
			bool canGoBack,
			bool canGoForward) = 0;

		// Set the draggable regions.
		virtual void OnSetDraggableRegions(
			const std::vector<CefDraggableRegion>& regions) = 0;

		// Set focus to the next/previous control.
		virtual void OnTakeFocus(bool next) {}

		// Called on the UI thread before a context menu is displayed.
		virtual void OnBeforeContextMenu(CefRefPtr<CefMenuModel> model) {}

	protected:
		virtual ~Cef3DHandlerDelegate() {}
	};

	class CEF3D_API Cef3DHandler :
		public CefClient,
		public CefDisplayHandler,
		public CefLifeSpanHandler,
		public CefLoadHandler,
		public CefContextMenuHandler,
		public CefKeyboardHandler
	{
	public:
		explicit Cef3DHandler(Cef3DHandlerDelegate* Delegate,bool IsOsr,const std::string& Url);
		~Cef3DHandler();

		void CloseAllBrowsers(bool force_close);
		void ShowDevTools(CefRefPtr<CefBrowser> browser, const CefPoint& inspect_element_at);
		void CloseDevTools(CefRefPtr<CefBrowser> browser);

		// CefClient methods:
		CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE { return this; }
		CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE { return this; }
		CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this;}
		CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }

		bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE;

		//CefContextMenuHandler
		void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) OVERRIDE;
		bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags) OVERRIDE;

		// CefDisplayHandler methods:
		void OnTitleChange(CefRefPtr<CefBrowser> browser,const CefString& title) OVERRIDE;
		void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) OVERRIDE;
		void OnFaviconURLChange(CefRefPtr<CefBrowser> browser, const std::vector<CefString>& icon_urls) OVERRIDE;
		void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen) OVERRIDE;
		bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString& message, const CefString& source, int line) OVERRIDE;

		// CefKeyboardHandler
		bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event, bool* is_keyboard_shortcut) OVERRIDE;

		// CefLifeSpanHandler methods:
		bool OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url,const CefString& target_frame_name,
			CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client, CefBrowserSettings& settings, bool* no_javascript_access) OVERRIDE;
		void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
		void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

		// CefLoadHandler methods:
		void OnLoadError(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame,ErrorCode errorCode,const CefString& errorText,const CefString& failedUrl) OVERRIDE;
		void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) OVERRIDE;


	public:
		CefRefPtr<CefBrowser> GetCefBrowser(Cef3DBrowser* Browser);

	public:
		void NotifyBrowserCreated(CefRefPtr<CefBrowser> browser);
		void NotifyBrowserClosing(CefRefPtr<CefBrowser> browser);
		void NotifyBrowserClosed(CefRefPtr<CefBrowser> browser);
		void NotifyAddress(const CefString& url);
		void NotifyTitle(const CefString& title);
		void NotifyFavicon(CefRefPtr<CefImage> image);
		void NotifyFullscreen(bool fullscreen);
		void NotifyLoadingState(bool isLoading,
			bool canGoBack,
			bool canGoForward);
		void NotifyDraggableRegions(
			const std::vector<CefDraggableRegion>& regions);

		Cef3DHandlerDelegate* delegate() const { return Delegate; }
		void DetachDelegate();

		bool CreatePopupWindow(
			CefRefPtr<CefBrowser> browser,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings);

	private:
		Cef3DHandlerDelegate* Delegate;
		bool IsOsr;
		std::string StartupUrl;
		int BrowserCount;



		IMPLEMENT_REFCOUNTING(Cef3DHandler);
	private:
		typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
		BrowserList browser_list_;

		// True if mouse cursor change is disabled.
		bool mouse_cursor_change_disabled_;

		CefRefPtr<CefMessageRouterBrowserSide> message_router_;


		// Console logging state.
		const std::string console_log_file_;
		bool first_console_message_;

		// True if an editable field currently has focus.
		bool focus_on_editable_field_;

		typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;
		MessageHandlerSet message_handler_set_;
	};
}