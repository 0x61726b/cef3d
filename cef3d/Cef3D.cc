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
#include "Cef3D.h"
#include "simple_app.h"


namespace CefUI
{
	Cef3D::Cef3D(CefOsrDelegate* del,void* wnd)
		:
		windowHandle_(wnd),
		osr_delegate_(del),
		isInitialized_(false)
	{

	}

	Cef3D::~Cef3D()
	{

	}

	bool Cef3D::Initialize(const char* binaryDir)
	{

		if(isInitialized_)
			return true;

		cefSettings_.no_sandbox = 1;

		std::string pathToSubProces(binaryDir);
#ifdef _WIN32
		pathToSubProces.append("/YumeCefSubProcess.exe");
#else
		pathToSubProces.append("/YumeCefSubProcess");
#endif

		CefString(&cefSettings_.browser_subprocess_path).FromASCII(pathToSubProces.c_str());

		client_app_ = (new SimpleApp(osr_delegate_));

		return true;
	}

	bool Cef3D::DoInit()
	{
#ifdef _WIN32
		CefMainArgs args(GetModuleHandle(NULL));
#else
		CefMainArgs args();
#endif

		isInitialized_ = CefInitialize(args,cefSettings_,client_app_.get(),nullptr);

		return isInitialized_;
	}

	int Cef3D::CreateBrowser(const CefRect& rect,const std::string& url)
	{
		if(!isInitialized_)
			DoInit();

		return client_app_->CreateBrowser(::CefRect(rect.x,rect.y,rect.width,rect.height),url);
	}

	void Cef3D::HandleKeyDown(unsigned key,unsigned mouseButton,int repeat)
	{
		
	}


	void Cef3D::HandleKeyUp(unsigned key,unsigned mouseButton,int repeat)
	{
		
	}

	void Cef3D::HandleMouseButtonDown(int mouseX,int mouseY,int modifiers,int button,unsigned buttons)
	{
		client_app_->GetClientHandler()->HandleMouseButtonDown(mouseX,mouseY,modifiers,button,buttons);
	}

	void Cef3D::HandleMouseButtonUp(int mouseX,int mouseY,int modifiers,int button,unsigned buttons)
	{
		client_app_->GetClientHandler()->HandleMouseButtonUp(mouseX,mouseY,modifiers,button,buttons);
	}

	void Cef3D::HandleMouseMove(int mouseX,int mouseY,int modifiers,unsigned buttons)
	{
		client_app_->GetClientHandler()->HandleMouseMove(mouseX,mouseY,modifiers,buttons);
	}

	void Cef3D::HandleKeyEvent(int type,int modifiers,unsigned key)
	{
		client_app_->GetClientHandler()->HandleKeyEvent(type,modifiers,key);
	}


	void Cef3D::Shutdown()
	{
		if(!isInitialized_)
			return;
		CefShutdown();
	}

	void Cef3D::RunCefLoop()
	{
		if(!isInitialized_)
			return;
		CefDoMessageLoopWork();
	}

}
