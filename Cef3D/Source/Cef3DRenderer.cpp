//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DRenderer.h
// Date: 18.04.2017
//---------------------------------------------------------------------------

#include "Cef3DPCH.h"

#include <iostream>

namespace Cef3D
{
	Cef3DRenderer::Cef3DRenderer()
	{

	}

	void Cef3DRenderer::RegisterDelegate(CefRefPtr<Cef3DRendererDelegate> Del)
	{
		Delegates.insert(Del);
	}


	void Cef3DRenderer::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnRenderThreadCreated(this, extra_info);
	}

	void Cef3DRenderer::OnWebKitInitialized() 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnWebKitInitialized(this);
	}

	void Cef3DRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnBrowserCreated(this, browser);
	}

	void Cef3DRenderer::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnBrowserDestroyed(this, browser);
	}

	CefRefPtr<CefLoadHandler> Cef3DRenderer::GetLoadHandler() 
	{
		CefRefPtr<CefLoadHandler> load_handler;
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end() && !load_handler.get(); ++it)
			load_handler = (*it)->GetLoadHandler(this);

		return load_handler;
	}

	bool Cef3DRenderer::OnBeforeNavigation(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame,CefRefPtr<CefRequest> request,NavigationType navigation_type,bool is_redirect) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it) {
			if ((*it)->OnBeforeNavigation(this, browser, frame, request,
				navigation_type, is_redirect)) {
				return true;
			}
		}

		return false;
	}

	void Cef3DRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnContextCreated(this, browser, frame, context);

		LOG(INFO) << "Context created";
		LOG(INFO) << context->GetBrowser()->GetIdentifier() << " id";

		LOG(INFO) << "Global object count:" << (int)GlobalJsObjects.size();

		if (GlobalJsObjects.size())
		{
			LOG(INFO) << "Applying global objects:" << (int)GlobalJsObjects.size();
			ApplyGlobalJsObjects();
		}
	}

	void Cef3DRenderer::OnContextReleased(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnContextReleased(this, browser, frame, context);
	}

	void Cef3DRenderer::OnUncaughtException(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame,CefRefPtr<CefV8Context> context,CefRefPtr<CefV8Exception> exception,CefRefPtr<CefV8StackTrace> stackTrace) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it) {
			(*it)->OnUncaughtException(this, browser, frame, context, exception,
				stackTrace);
		}
	}

	void Cef3DRenderer::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) 
	{
		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end(); ++it)
			(*it)->OnFocusedNodeChanged(this, browser, frame, node);
	}

	bool Cef3DRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) 
	{
		DCHECK_EQ(source_process, PID_BROWSER);

		bool handled = false;

		//LOG(INFO) << "Breaking";
		//__debugbreak();

		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end() && !handled; ++it) {
			handled = (*it)->OnProcessMessageReceived(this, browser, source_process,
				message);
		}

		CefString message_name = message->GetName();

		if (message_name == "create_object_request")
		{
			CefRefPtr<CefListValue> args = message->GetArgumentList();
			int obj_type = args->GetInt(0);
			CefString obj_name = args->GetString(1);

			DLOG(INFO) << "create_object_request";
			DLOG(INFO) << "Type: " << obj_type << " Name: " << obj_name.ToString().c_str();

			switch (obj_type)
			{
			case Cef3DJsValueTypes::CEF3D_JSVALUE_INT:
			{

			}break;
			case Cef3DJsValueTypes::CEF3D_JSVALUE_STRING:
			{
				CefString obj_val = args->GetString(2);
				Cef3DJsValue cef3d_js_value(obj_name, obj_val);

				AddGlobalJsObject(browser, cef3d_js_value);
				ApplyGlobalJsObjects();
			}break;
			default:
				break;
			}
		}


		return handled;
	}
	bool Cef3DRenderer::GlobalJsObjectExists(CefRefPtr<CefBrowser> browser, const Cef3DJsValue& value)
	{
		GlobalJsObjectMap::iterator it = GlobalJsObjects.find(browser);
		if (it == GlobalJsObjects.end())
			return false;

		std::vector<Cef3DJsValue> objects = it->second;
		std::vector<Cef3DJsValue>::iterator val = std::find(objects.begin(), objects.end(), value);
		if (val == objects.end())
			return false;
		return true;
	}

	void Cef3DRenderer::AddGlobalJsObject(CefRefPtr<CefBrowser> browser, const Cef3DJsValue& value)
	{
		if (!GlobalJsObjectExists(browser, value))
		{
			GlobalJsObjectMap::iterator it = GlobalJsObjects.find(browser);
			std::vector<Cef3DJsValue> objects;
			if (it != GlobalJsObjects.end())
			{
				objects = it->second;
			}
			
			std::vector<Cef3DJsValue>::iterator val = std::find(objects.begin(), objects.end(), value);
			// return if it already exists
			if (val != objects.end())
				return;

			objects.push_back(value);

			if (it == GlobalJsObjects.end())
			{
				GlobalJsObjects.insert(std::make_pair(browser, objects));
			}
		}
	}

	void Cef3DRenderer::RemoveGlobalJsObject(CefRefPtr<CefBrowser> browser, const Cef3DJsValue& value)
	{
		if (GlobalJsObjectExists(browser, value))
		{
			GlobalJsObjectMap::iterator it = GlobalJsObjects.find(browser);
			if (it == GlobalJsObjects.end())
				return;

			std::vector<Cef3DJsValue> objects = it->second;
			std::vector<Cef3DJsValue>::iterator val = std::find(objects.begin(), objects.end(), value);
			// return if it doesnt exists
			if (val == objects.end())
				return;

			objects.erase(val);
		}
	}

	void Cef3DRenderer::UpdateGlobalJsObject(CefRefPtr<CefBrowser> browser, const Cef3DJsValue& value)
	{
		if (GlobalJsObjectExists(browser, value))
		{
			GlobalJsObjectMap::iterator it = GlobalJsObjects.find(browser);
			if (it != GlobalJsObjects.end())
				return;

			std::vector<Cef3DJsValue> objects = it->second;
			std::vector<Cef3DJsValue>::iterator val = std::find(objects.begin(), objects.end(), value);
			// return if it doesnt exists
			if (val == objects.end())
				return;

			*val = value;
		}
	}
	void Cef3DRenderer::ApplyGlobalJsObjects()
	{
		GlobalJsObjectMap::iterator it = GlobalJsObjects.begin();
		for (it; it != GlobalJsObjects.end(); ++it)
		{
			CefRefPtr<CefBrowser> browser = it->first;
			std::vector<Cef3DJsValue> values = it->second;
			for (int i = 0; i < values.size(); ++i)
			{
				Cef3DJsValue cef3d_value = values[i];
				CefString cef3d_obj_name = cef3d_value.GetName();
				int cef3d_value_type = cef3d_value.GetType();

				switch (cef3d_value_type)
				{
				case CEF3D_JSVALUE_STRING:
				{
					CefRefPtr<CefV8Context> v8_context = browser->GetMainFrame()->GetV8Context();
					CefString obj_val = cef3d_value.GetString();

					if (v8_context->IsValid() && v8_context->GetBrowser())
					{
						v8_context->Enter();
						CefRefPtr<CefV8Value> v8_obj_value = CefV8Value::CreateString(obj_val);
						CefRefPtr<CefV8Value> global_context = v8_context->GetGlobal();
						if (global_context->IsValid() && global_context->IsObject())
							global_context->SetValue(cef3d_obj_name, v8_obj_value, V8_PROPERTY_ATTRIBUTE_NONE);

						DLOG(INFO) << "Created new string object";

						v8_context->Exit();
					}
				}break;
				default:
					break;
				}
			}
		}
	}
}