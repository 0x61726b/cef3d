// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "include/cef_app.h"
#include "Cef3DClientHandler.h"

class CefOsrDelegate;

// Implement application-level callbacks for the browser process.
class Cef3DApp :
	public CefApp,
	public CefBrowserProcessHandler
{
public:
	Cef3DApp(CefOsrDelegate* osr);

	void OnBeforeCommandLineProcessing(
		const CefString& process_type,
		CefRefPtr<CefCommandLine> command_line) OVERRIDE;

	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
		OVERRIDE{return this;}


	int CreateBrowser(const CefRect& rect,const std::string& url);

	//CefRenderProcess
	virtual void OnContextInitialized() OVERRIDE;


	

	void SetExtensionSource(const char*);

	CefRefPtr<Cef3DClientHandler> GetClientHandler() const { return client_handler_; }



private:
	const char* extensionSourceCode_;
	CefOsrDelegate* osr_delegate_;
	CefRefPtr<Cef3DClientHandler> client_handler_;
private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(Cef3DApp);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
