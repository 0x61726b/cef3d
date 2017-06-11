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

#pragma once

#include "include/cef_v8.h"
#include "include/cef_client.h"

namespace Cef3D
{
	class Cef3DV8Handler : public CefV8Handler
	{
	public:
		explicit Cef3DV8Handler(CefRefPtr<Cef3DRenderer> clientApp)
			: client_app_(clientApp)
		{
		}

		virtual bool Execute(const CefString& name,
			CefRefPtr<CefV8Value> object,
			const CefV8ValueList& arguments,
			CefRefPtr<CefV8Value>& retval,
			CefString& exception) OVERRIDE;

	private:
		CefRefPtr<Cef3DRenderer> client_app_;

		IMPLEMENT_REFCOUNTING(Cef3DV8Handler);
	};
}