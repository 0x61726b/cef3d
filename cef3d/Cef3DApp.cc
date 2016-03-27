// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "Cef3DApp.h"

#include <string>


#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"

#include "Cef3DV8Handler.h"
#include "Cef3D.h"

Cef3DApp::Cef3DApp(CefOsrDelegate* osr): osr_delegate_(osr)
{
	client_handler_ = (new Cef3DClientHandler(osr_delegate_));
}

void Cef3DApp::OnBeforeCommandLineProcessing(
	const CefString& process_type,
	CefRefPtr<CefCommandLine> command_line)
{
	command_line->AppendSwitch("disable-surfaces");
	command_line->AppendSwitch("disable-extensions");
	command_line->AppendSwitch("disable-gpu");
	command_line->AppendSwitch("disable-gpu-compositing");
	command_line->AppendSwitch("enable-begin-frame-scheduling");
}

int Cef3DApp::CreateBrowser(const CefRect& rect,const std::string& url)
{
	CefWindowInfo window_info;
	window_info.SetAsWindowless(NULL,true);

	client_handler_->AddBrowserRect(rect);
	CefBrowserSettings browser_settings;
	CefBrowserHost::CreateBrowserSync(window_info,client_handler_.get(),url,
		browser_settings,NULL);

	return client_handler_->GetBrowserListSize() - 1;
}


void Cef3DApp::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();

	osr_delegate_->OnContextReady();

	LOG(INFO) << "Context is initialized";
}

void Cef3DApp::SetExtensionSource(const char* extension)
{
	extensionSourceCode_ = extension;
}
