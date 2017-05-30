//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DUtils.h
// Date: 30.05.2017
//---------------------------------------------------------------------------

#include "Cef3DPCH.h"
#include "Cef3DV8Handler.h"

namespace Cef3D
{
	bool Cef3DV8Handler::Execute(
		const CefString &name, CefRefPtr<CefV8Value> object,
		const CefV8ValueList &arguments,
		CefRefPtr<CefV8Value> &retval,
		CefString &exception)
	{
		CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
		if (!browser)
			return false;

		LOG(INFO) << "Received Js execute. Function name: " << name;

		/*CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(name);
		CefRefPtr<CefListValue> message_args = message->GetArgumentList();*/

		return true;
	}
}