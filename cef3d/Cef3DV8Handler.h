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
#ifndef __CefV8Handler_h__
#define __CefV8Handler_h__
//----------------------------------------------------------------------------
#include "Cef3DRequired.h"
#include "include/cef_v8.h"
#include "include/cef_client.h"
#include "Cef3DRendererApp.h"
//----------------------------------------------------------------------------

class Cef3DV8Handler : public CefV8Handler
{
public:
	explicit Cef3DV8Handler(CefRefPtr<Cef3DRendererApp> clientApp)
		: client_app_(clientApp),
		messageId(0)
	{
	}

	virtual bool Execute(const CefString& name,
		CefRefPtr<CefV8Value> object,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception) OVERRIDE;

private:
	CefRefPtr<Cef3DRendererApp> client_app_;
	int32 messageId;

	IMPLEMENT_REFCOUNTING(Cef3DV8Handler);
};



//----------------------------------------------------------------------------
#endif
