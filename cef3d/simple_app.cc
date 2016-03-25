// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_app.h"

#include <string>


#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"

#include "Cef3D.h"

SimpleApp::SimpleApp(CefOsrDelegate* osr): osr_delegate_(osr)
{
	client_handler_ = (new SimpleHandler(osr_delegate_));
}

void SimpleApp::OnBeforeCommandLineProcessing(
	const CefString& process_type,
	CefRefPtr<CefCommandLine> command_line)
{
	command_line->AppendSwitch("disable-surfaces");
	command_line->AppendSwitch("disable-extensions");
	command_line->AppendSwitch("disable-gpu");
	command_line->AppendSwitch("disable-gpu-compositing");
	command_line->AppendSwitch("enable-begin-frame-scheduling");
}

int SimpleApp::CreateBrowser(const CefRect& rect,const std::string& url)
{
	CefWindowInfo window_info;
	window_info.SetAsWindowless(NULL,false);

	client_handler_->AddBrowserRect(rect);
	CefBrowserSettings browser_settings;
	CefBrowserHost::CreateBrowserSync(window_info,client_handler_.get(),url,
		browser_settings,NULL);

	return client_handler_->GetBrowserListSize() - 1;
}

void SimpleApp::OnContextInitialized() {
	CEF_REQUIRE_UI_THREAD();

	osr_delegate_->OnContextReady();
}
