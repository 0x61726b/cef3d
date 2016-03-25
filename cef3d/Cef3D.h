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
#ifndef __Cef3D_h__
#define __Cef3D_h__
//----------------------------------------------------------------------------
#include "Cef3DRequired.h"
#include "simple_handler.h"
#include "simple_app.h"
//----------------------------------------------------------------------------


class Cef3DExport CefOsrDelegate
{
public:
	virtual void OnPaint(int browserIndex,std::vector<CefUI::CefRect>& dirtyRects,const void* buffer,
		int width,
		int height) = 0;

	virtual void OnContextReady() = 0; //Context is ready,we can create browsers here
	virtual void OnBrowserReady(unsigned index) = 0; //Browser is ready,the texture should be up now

};


namespace CefUI
{
	class Cef3DExport Cef3D
	{
	public:
		Cef3D(CefOsrDelegate* del,void* wndHandle);

		virtual ~Cef3D();

		bool Initialize(const char* binaryDir);
		bool DoInit();

		void Shutdown();

		int CreateBrowser(const CefRect& rect,const std::string&);
		void RunCefLoop();

		void HandleKeyDown(unsigned key,unsigned mouseButton,int repeat);
		void HandleKeyUp(unsigned key,unsigned mouseButton,int repeat);
		void HandleMouseButtonDown(int mouseX,int mouseY,int modifiers,int button,unsigned buttons);
		void HandleMouseButtonUp(int mouseX,int mouseY,int modifiers,int button,unsigned buttons);
		void HandleMouseMove(int mouseX,int mouseY,int modifiers,unsigned buttons);

		void HandleKeyEvent(int type,int modifiers,unsigned key);
	private:
		void* windowHandle_;
		CefOsrDelegate* osr_delegate_;
		CefRefPtr<SimpleApp> client_app_;
		CefSettings cefSettings_;

		bool isInitialized_;
	};
}


//----------------------------------------------------------------------------
#endif
