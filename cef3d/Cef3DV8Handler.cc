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
#include "Cef3DV8Handler.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"

#include "Cef3DRendererApp.h"
#include "Cef3DHelper.h"


bool Cef3DV8Handler::Execute(
	const CefString &name,CefRefPtr<CefV8Value> object,
	const CefV8ValueList &arguments,
	CefRefPtr<CefV8Value> &retval,
	CefString &exception)
{
	CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
	if(!browser)
		return false;

	LOG(INFO) << "Received Js execute";

	CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(name);
	CefRefPtr<CefListValue> message_args = message->GetArgumentList();

	if(arguments.size() > 0
		&& !arguments[0]->IsFunction()
		&& !arguments[0]->IsNull()
		)
	{
		std::string function_name = name;
		LOG(ERROR) << "Function called without callback param: "
			<< function_name;

		return false;
	}

	if(arguments.size() > 0 && arguments[0]->IsFunction()) {
		// The first argument is the message id
		client_app_->AddCallback(messageId,CefV8Context::GetCurrentContext(),arguments[0]);
		Cef3DHelpers::SetListValue(message_args,0,CefV8Value::CreateInt(messageId));
	}
	else {
		Cef3DHelpers::SetListValue(message_args,0,CefV8Value::CreateNull());
	}

	// Pass the rest of the arguments
	for(unsigned int i = 1; i < arguments.size(); i++)
		Cef3DHelpers::SetListValue(message_args,i,arguments[i]);
	browser->SendProcessMessage(PID_BROWSER,message);

	retval = CefV8Value::CreateInt(50);
	messageId++;
	return true;
}
