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
#include "Cef3DBrowserDelegate.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"

#include "Cef3DHelper.h"
#include "Cef3D.h"
namespace CefUI
{
	namespace {
		const char kNameSpace[] = "Yume";

		const char kGetElapsedTime[] = "GetElapsedTime";
	}
	Cef3DBrowserDelegate::Cef3DBrowserDelegate()
		: ProcessMessageDelegate(kNameSpace)
	{

	}

	bool Cef3DBrowserDelegate::OnProcessMessageReceived(
		CefRefPtr<Cef3DClientHandler> handler,
		CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message)
	{
		std::string message_name = message->GetName();
		CefRefPtr<CefListValue> request_args = message->GetArgumentList();
		int32 callbackId = -1;
		int32 error = NO_ERROR;
		bool invoke_callback = true;
		CefRefPtr<CefProcessMessage> response =
			CefProcessMessage::Create("invokeCallback");
		CefRefPtr<CefListValue> response_args = response->GetArgumentList();

		// V8 extension messages are handled here. These messages come from the
		// render process thread (in cef_app.cpp), and have the following format:
		//   name - the name of the native function to call
		//   argument0 - the id of this message. This id is passed back to the
		//               render process in order to execute callbacks
		//   argument1 - argumentN - the arguments for the function
		//

		if(request_args->GetSize() > 0 && request_args->GetType(0) != VTYPE_NULL) {
			callbackId = request_args->GetInt(0);

			if(callbackId != -1)
				response_args->SetInt(0,callbackId);
		}


#ifndef NDEBUG
		LOG(INFO) << "App message: " << message_name << Cef3DHelpers::DumpList(request_args);
#endif

		if(message_name == "rendererContextReady")
		{
			
		}
		if(message_name == "SendDomEvent")
		{
			CefString arg1 = request_args->GetString(1); //DOM element
			CefString arg2 = request_args->GetString(2); //event type
			CefString arg3 = request_args->GetString(3); //bool
			Cef3D::GetInstance()->GetDelegate()->OnDomEvent(arg1,arg2,arg3);
		}
		if(message_name == "OnDomReady")
		{
			Cef3D::GetInstance()->GetDelegate()->OnRendererContextReady();
		}

		if(message_name == kGetElapsedTime)
		{

		}

		if(invoke_callback && callbackId != -1) {
			response_args->SetInt(1,error);

			// Send response
			browser->SendProcessMessage(PID_RENDERER,response);
		}

		return true;
	}
	void
		Cef3DBrowserDelegate::CreateProcessMessageDelegates(Cef3DClientHandler::ProcessMessageDelegateSet *delegates) {
		delegates->insert(new Cef3DBrowserDelegate);
	}


}
