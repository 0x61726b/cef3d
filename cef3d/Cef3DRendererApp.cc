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
#include "Cef3DRendererApp.h"
#include "Cef3DV8Handler.h"
#include "Cef3D.h"


#include "Cef3DHelper.h"

void Cef3DRendererApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context)
{
	CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("rendererContextReady");
	browser->SendProcessMessage(PID_BROWSER,message);
}

void Cef3DRendererApp::OnContextReleased(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context)
{
	
}

void Cef3DRendererApp::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
{
	LOG(INFO) << "Render thread is created";
}
void Cef3DRendererApp::OnWebKitInitialized()
{
	std::string extension(extensionCode_);

	CefRefPtr<Cef3DV8Handler> v8(new Cef3DV8Handler(this));

	CefRegisterExtension("yume",extension,v8);

#ifndef NDEBUG
	LOG(INFO) << "Loading extension source code " << extension.c_str();
#endif
}

bool Cef3DRendererApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message)
{
	bool handled = false;

	if(!handled) {
		if(message->GetName() == "invokeCallback") {
			// This is called by the desktop extension handler to invoke the asynchronous
			// callback function

			CefRefPtr<CefListValue> messageArgs = message->GetArgumentList();
			int32 callbackId = messageArgs->GetInt(0);

			CefRefPtr<CefV8Context> context = callback_map_[callbackId].first;
			CefRefPtr<CefV8Value> callbackFunction = callback_map_[callbackId].second;
			CefV8ValueList arguments;
			context->Enter();

			// Sanity check to make sure the context is still attched to a browser.
			// Async callbacks could be initiated after a browser instance has been deleted,
			// which can lead to bad things. If the browser instance has been deleted, don't
			// invoke this callback.
			if(context->GetBrowser()) {
				for(unsigned int i = 1; i < messageArgs->GetSize(); i++) {
					arguments.push_back(Cef3DHelpers::ListValueToV8Value(messageArgs,i));
				}

				callbackFunction->ExecuteFunction(NULL,arguments);
			}

			context->Exit();

			callback_map_.erase(callbackId);

			handled = true;

		}
		else if(message->GetName() == "dispatchEvent") {
			// This is called by the browser process to dispatch a Event to JavaScript
			//
			// The first argument is the event name. This is required.
			// The second argument is a custom event data. This is optional. Currently only string data supported :-(

			CefRefPtr<CefListValue> messageArgs = message->GetArgumentList();
			std::string eventName = messageArgs->GetString(0);
			std::string eventData = messageArgs->GetSize() > 1 ? messageArgs->GetString(1) : "[]";

			CefRefPtr<CefV8Context> context = browser->GetMainFrame()->GetV8Context();
			context->Enter();
			CefRefPtr<CefV8Value> global = context->GetGlobal();
			if(global->HasValue("testo")) {
				CefRefPtr<CefV8Value> js_window = global->GetValue("testo");
				if(js_window->IsFunction()) {
					CefV8ValueList args;
					args.push_back(CefV8Value::CreateString(eventName));
					args.push_back(CefV8Value::CreateString(eventData));

					js_window->ExecuteFunction(global,args);
				}
			}

			context->Exit();
		}
	}
	return handled;
}

