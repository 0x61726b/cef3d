//----------------------------------------------------------------------------
//Yume Engine
//Copyright (C) 2015  arkenthera
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/
//----------------------------------------------------------------------------
//
// File : <Filename>
// Date : <Date>
// Comments :
//
//----------------------------------------------------------------------------
#ifndef __Cef3DRendererApp_h__
#define __Cef3DRendererApp_h__
//----------------------------------------------------------------------------
#include "Cef3DRequired.h"
#include "Cef3DClientHandler.h"

#include "include/cef_app.h"
//----------------------------------------------------------------------------

class Cef3DRendererApp :
	public CefApp,
	public CefRenderProcessHandler
{
public:
	Cef3DRendererApp(const char* extension)
		:extensionCode_(extension)
	{
	}

	void AddCallback(int32 id,CefRefPtr<CefV8Context> context,CefRefPtr<CefV8Value> callbackFunction) {
		callback_map_[id] = std::make_pair(context,callbackFunction);
	}

	virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;
	void OnContextCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) OVERRIDE;
	void OnContextReleased(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) OVERRIDE;

	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
		OVERRIDE
	{
		return this;
	}

	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message);

	virtual void OnWebKitInitialized() OVERRIDE;

	typedef std::map<int32,std::pair< CefRefPtr<CefV8Context>,CefRefPtr<CefV8Value> > > CallbackMap;
	CallbackMap callback_map_;

private:
	const char* extensionCode_;
	IMPLEMENT_REFCOUNTING(Cef3DRendererApp);
};


//----------------------------------------------------------------------------
#endif
