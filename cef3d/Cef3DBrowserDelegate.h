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
#ifndef __Cef3DBrowserDelegate_h__
#define __Cef3DBrowserDelegate_h__
//----------------------------------------------------------------------------
#include "Cef3DRequired.h"

#include "Cef3DClientHandler.h"
//----------------------------------------------------------------------------
namespace CefUI
{
	class Cef3DBrowserDelegate : Cef3DClientHandler::ProcessMessageDelegate
	{
	public:
		Cef3DBrowserDelegate();

		virtual bool OnProcessMessageReceived(
			CefRefPtr<Cef3DClientHandler> handler,
			CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message) OVERRIDE;

		// Create message delegates that run in the browser process
		static void CreateProcessMessageDelegates(Cef3DClientHandler::ProcessMessageDelegateSet *delegates);

		IMPLEMENT_REFCOUNTING(Cef3DBrowserDelegate);
	};
}


//----------------------------------------------------------------------------
#endif
