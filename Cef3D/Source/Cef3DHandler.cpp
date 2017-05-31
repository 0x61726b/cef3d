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

#include "Cef3DPCH.h"

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace Cef3D
{
	Cef3DHandler::Cef3DHandler(Cef3DHandlerDelegate* HandlerDelegate, bool Osr, const std::string& url)
		: IsOsr(Osr),
		StartupUrl(url),
		BrowserCount(0),
		Delegate(HandlerDelegate),
		console_log_file_("")
	{
	}

	CefRefPtr<CefBrowser> Cef3DHandler::GetCefBrowser(Cef3DBrowser* Browser)
	{
		DCHECK(Browser);

		BrowserList::iterator bit = browser_list_.begin();
		for (; bit != browser_list_.end(); ++bit)
		{
			if ((*bit)->GetIdentifier() == Browser->GetBrowserID())
				return (*bit);
		}
		return 0;
	}

	bool Cef3DHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
	{
		CEF_REQUIRE_UI_THREAD();

		if (message_router_->OnProcessMessageReceived(browser, source_process,
			message)) {
			return true;
		}

		// Check for messages from the client renderer.
		std::string message_name = message->GetName();
		if (message_name == "ClientRenderer.FocusedNodeChanged") {
			// A message is sent from ClientRenderDelegate to tell us whether the
			// currently focused DOM node is editable. Use of |focus_on_editable_field_|
			// is redundant with CefKeyEvent.focus_on_editable_field in OnPreKeyEvent
			// but is useful for demonstration purposes.
			focus_on_editable_field_ = message->GetArgumentList()->GetBool(0);
			return true;
		}

		return false;
	}

	void Cef3DHandler::OnBeforeContextMenu(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		CefRefPtr<CefMenuModel> model)
	{
		CEF_REQUIRE_UI_THREAD();

		if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0)
		{
			// Add a separator if the menu already has items.
			if (model->GetCount() > 0)
				model->AddSeparator();

			// Add DevTools items to all context menus.
			model->AddItem(0, "&Show DevTools");
			model->AddItem(1, "Close DevTools");
			model->AddSeparator();
			model->AddItem(2, "Inspect Element");
		}

		if (Delegate)
			Delegate->OnBeforeContextMenu(model);
	}


	bool Cef3DHandler::OnContextMenuCommand(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		int command_id,
		EventFlags event_flags) {
		CEF_REQUIRE_UI_THREAD();

		switch (command_id) {
		case 0:
			ShowDevTools(browser, CefPoint());
			return true;
		case 1:
			CloseDevTools(browser);
			return true;
		case 2:
			ShowDevTools(browser, CefPoint(params->GetXCoord(), params->GetYCoord()));
			return true;
		default:
			return true;
		}
	}

	/* CefDisplayHandler */
	void Cef3DHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& url) {
		CEF_REQUIRE_UI_THREAD();

		// Only update the address for the main (top-level) frame.
		if (frame->IsMain())
			NotifyAddress(url);
	}

	void Cef3DHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
	{
		CEF_REQUIRE_UI_THREAD();

		NotifyTitle(title);
	}


	void Cef3DHandler::OnFaviconURLChange(CefRefPtr<CefBrowser> browser, const std::vector<CefString>& icon_urls)
	{
		CEF_REQUIRE_UI_THREAD();

	}

	void Cef3DHandler::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen)
	{
		CEF_REQUIRE_UI_THREAD();

		NotifyFullscreen(fullscreen);
	}

	bool Cef3DHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString& message, const CefString& source, int line)
	{
		CEF_REQUIRE_UI_THREAD();

		return false;
	}
	/* END CefDisplayHandler */

	/* CefKeyboardhandler */
	bool Cef3DHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event, bool* is_keyboard_shortcut)
	{
		CEF_REQUIRE_UI_THREAD();

		if (!event.focus_on_editable_field && event.windows_key_code == 0x20) {
			// Special handling for the space character when an input element does not
			// have focus. Handling the event in OnPreKeyEvent() keeps the event from
			// being processed in the renderer. If we instead handled the event in the
			// OnKeyEvent() method the space key would cause the window to scroll in
			// addition to showing the alert box.
			/*if (event.type == KEYEVENT_RAWKEYDOWN)
				test_runner::Alert(browser, "You pressed the space bar!");*/
			return true;
		}

		return false;
	}
	/* END CefKeyboardhandler */

	/* CefLifeSpanHandler */
	void Cef3DHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
	{
		CEF_REQUIRE_UI_THREAD();

		BrowserCount++;

		if (!message_router_) {
			// Create the browser-side router for query handling.
			CefMessageRouterConfig config;
			message_router_ = CefMessageRouterBrowserSide::Create(config);

			MessageHandlerSet::const_iterator it = message_handler_set_.begin();
			for (; it != message_handler_set_.end(); ++it)
				message_router_->AddHandler(*(it), false);
		}

		// Disable mouse cursor change if requested via the command-line flag.
		if (mouse_cursor_change_disabled_)
			browser->GetHost()->SetMouseCursorChangeDisabled(true);

		NotifyBrowserCreated(browser);

		GMainContext->GetCef3DBrowser(browser)->OnAfterCreated();

		browser->GetMainFrame()->LoadURL("http://google.com");
	}

	bool Cef3DHandler::DoClose(CefRefPtr<CefBrowser> browser)
	{
		CEF_REQUIRE_UI_THREAD();

		NotifyBrowserClosing(browser);

		// Allow the close. For windowed browsers this will result in the OS close
		// event being sent.
		return false;
	}

	void Cef3DHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
	{
		CEF_REQUIRE_UI_THREAD();


		if (--BrowserCount == 0) {
			// Remove and delete message router handlers.
			MessageHandlerSet::const_iterator it =
				message_handler_set_.begin();
			for (; it != message_handler_set_.end(); ++it) {
				message_router_->RemoveHandler(*(it));
				delete *(it);
			}
			message_handler_set_.clear();
			message_router_ = NULL;
		}

		NotifyBrowserClosed(browser);
	}

	bool Cef3DHandler::OnBeforePopup(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		CefLifeSpanHandler::WindowOpenDisposition target_disposition,
		bool user_gesture,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access) {
		CEF_REQUIRE_IO_THREAD();

		// Return true to cancel the popup window.
		return true;
	}

	/* END CefLifeSpanHandler */

	/* CefLoadHandler */
	void Cef3DHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
	{
		CEF_REQUIRE_UI_THREAD();

		if (errorCode == ERR_ABORTED)
			return;

		std::stringstream ss;
		ss << "<html><body bgcolor=\"white\">"
			"<h2>Failed to load URL " << std::string(failedUrl) <<
			" with error " << std::string(errorText) << " (" << errorCode <<
			").</h2></body></html>";
		frame->LoadString(ss.str(), failedUrl);
	}

	void Cef3DHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward) {
		CEF_REQUIRE_UI_THREAD();

		NotifyLoadingState(isLoading, canGoBack, canGoForward);
	}
	/* END CefLoadHandler */

	void Cef3DHandler::ShowDevTools(CefRefPtr<CefBrowser> browser, const CefPoint& inspect_element_at)
	{
		CefWindowInfo windowInfo;
		CefRefPtr<CefClient> client;
		CefBrowserSettings settings;

		if (CreatePopupWindow(browser, CefPopupFeatures(), windowInfo, client,
			settings))
		{
			browser->GetHost()->ShowDevTools(windowInfo, client, settings,
				inspect_element_at);
		}
	}

	void Cef3DHandler::CloseDevTools(CefRefPtr<CefBrowser> browser)
	{
		browser->GetHost()->CloseDevTools();
	}

	bool Cef3DHandler::CreatePopupWindow(
		CefRefPtr<CefBrowser> browser,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings) {
		// Note: This method will be called on multiple threads.

		GMainContext->GetRootWindowManager()->CreateRootWindowAsPopup(true, popupFeatures, windowInfo, client, settings, GMainContext->GetCef3DBrowser(browser)->GetBrowserForPopup());

		return true;
	}

	void Cef3DHandler::CloseAllBrowsers(bool force_close)
	{
		if (!CefCurrentlyOn(TID_UI))
		{
			// Execute on the UI thread.
			CefPostTask(TID_UI,
				base::Bind(&Cef3DHandler::CloseAllBrowsers, this, force_close));
			return;
		}

		if (browser_list_.empty())
			return;

		BrowserList::const_iterator it = browser_list_.begin();
		for (; it != browser_list_.end(); ++it)
			(*it)->GetHost()->CloseBrowser(force_close);
	}

	void Cef3DHandler::NotifyBrowserCreated(CefRefPtr<CefBrowser> browser)
	{
		if (!CURRENTLY_ON_MAIN_THREAD()) {
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(
				base::Bind(&Cef3DHandler::NotifyBrowserCreated, this, browser));
			return;
		}

		if (Delegate)
			Delegate->OnBrowserCreated(browser);
	}

	void Cef3DHandler::NotifyBrowserClosing(CefRefPtr<CefBrowser> browser)
	{
		if (!CURRENTLY_ON_MAIN_THREAD()) {
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(
				base::Bind(&Cef3DHandler::NotifyBrowserClosing, this, browser));
			return;
		}

		if (Delegate)
			Delegate->OnBrowserClosing(browser);
	}

	void Cef3DHandler::NotifyBrowserClosed(CefRefPtr<CefBrowser> browser)
	{
		if (!CURRENTLY_ON_MAIN_THREAD()) {
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(
				base::Bind(&Cef3DHandler::NotifyBrowserClosed, this, browser));
			return;
		}

		if (Delegate)
			Delegate->OnBrowserClosed(browser);
	}

	void Cef3DHandler::NotifyAddress(const CefString& url)
	{
		if (!CURRENTLY_ON_MAIN_THREAD()) {
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(
				base::Bind(&Cef3DHandler::NotifyAddress, this, url));
			return;
		}

		if (Delegate)
			Delegate->OnSetAddress(url);
	}

	void Cef3DHandler::NotifyTitle(const CefString& title)
	{
		if (!CURRENTLY_ON_MAIN_THREAD()) {
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(
				base::Bind(&Cef3DHandler::NotifyTitle, this, title));
			return;
		}

		if (Delegate)
			Delegate->OnSetTitle(title);
	}

	void Cef3DHandler::NotifyFavicon(CefRefPtr<CefImage> image)
	{
		if (!CURRENTLY_ON_MAIN_THREAD()) {
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(
				base::Bind(&Cef3DHandler::NotifyFavicon, this, image));
			return;
		}

		if (Delegate)
			Delegate->OnSetFavicon(image);
	}

	void Cef3DHandler::NotifyFullscreen(bool fullscreen)
	{
		if (!CURRENTLY_ON_MAIN_THREAD()) {
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(
				base::Bind(&Cef3DHandler::NotifyFullscreen, this, fullscreen));
			return;
		}

		if (Delegate)
			Delegate->OnSetFullscreen(fullscreen);
	}

	void Cef3DHandler::NotifyLoadingState(bool isLoading, bool canGoBack, bool canGoForward)
	{
		if (!CURRENTLY_ON_MAIN_THREAD()) {
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(
				base::Bind(&Cef3DHandler::NotifyLoadingState, this,
					isLoading, canGoBack, canGoForward));
			return;
		}

		if (Delegate)
			Delegate->OnSetLoadingState(isLoading, canGoBack, canGoForward);
	}

	void Cef3DHandler::NotifyDraggableRegions(const std::vector<CefDraggableRegion>& regions)
	{
		if (!CURRENTLY_ON_MAIN_THREAD()) {
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(
				base::Bind(&Cef3DHandler::NotifyDraggableRegions, this, regions));
			return;
		}

		if (Delegate)
			Delegate->OnSetDraggableRegions(regions);
	}

	void Cef3DHandler::DetachDelegate()
	{
		if (!CURRENTLY_ON_MAIN_THREAD()) {
			// Execute this method on the main thread.
			MAIN_POST_CLOSURE(base::Bind(&Cef3DHandler::DetachDelegate, this));
			return;
		}

		DCHECK(Delegate);
		Delegate = NULL;
	}

}

