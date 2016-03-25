// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "include/cef_app.h"
#include "simple_handler.h"

class CefOsrDelegate;

// Implement application-level callbacks for the browser process.
class SimpleApp : public CefApp,
	public CefBrowserProcessHandler
{
public:
	SimpleApp(CefOsrDelegate* osr);

	void OnBeforeCommandLineProcessing(
		const CefString& process_type,
		CefRefPtr<CefCommandLine> command_line) OVERRIDE;
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
		OVERRIDE{return this;}


	int CreateBrowser(const CefRect& rect,const std::string& url);
	virtual void OnContextInitialized() OVERRIDE;

	CefRefPtr<SimpleHandler> GetClientHandler() const { return client_handler_; }

	CefOsrDelegate* osr_delegate_;
	CefRefPtr<SimpleHandler> client_handler_;
private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(SimpleApp);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
