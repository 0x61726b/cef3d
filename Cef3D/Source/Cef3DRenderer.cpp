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

		CefRefPtr<CefCommandLine> cmd = CefCommandLine::GetGlobalCommandLine();

		CefRefPtr<CefV8Value> object = context->GetGlobal();
		
		CefRefPtr<CefV8Value> song_name = CefV8Value::CreateString(cmd->GetSwitchValue("song_name"));
		CefRefPtr<CefV8Value> artist_name = CefV8Value::CreateString(cmd->GetSwitchValue("artist_name"));
		CefRefPtr<CefV8Value> artist_scrobbles = CefV8Value::CreateString(cmd->GetSwitchValue("artist_scrobbles"));
		CefRefPtr<CefV8Value> album_scrobbles = CefV8Value::CreateString(cmd->GetSwitchValue("album_scrobbles"));
		CefRefPtr<CefV8Value> total_scrobbles = CefV8Value::CreateString(cmd->GetSwitchValue("total_scrobbles"));
		CefRefPtr<CefV8Value> album_cover = CefV8Value::CreateString(cmd->GetSwitchValue("album_cover"));
		CefRefPtr<CefV8Value> artist_cover = CefV8Value::CreateString(cmd->GetSwitchValue("artist_cover"));
		CefRefPtr<CefV8Value> genre = CefV8Value::CreateString(cmd->GetSwitchValue("genre"));
		CefRefPtr<CefV8Value> user_name = CefV8Value::CreateString(cmd->GetSwitchValue("user_name"));
		CefRefPtr<CefV8Value> user_avatar = CefV8Value::CreateString(cmd->GetSwitchValue("user_avatar"));
		CefRefPtr<CefV8Value> user_artist_count = CefV8Value::CreateString(cmd->GetSwitchValue("user_artist_count"));
		CefRefPtr<CefV8Value> user_scrobbles = CefV8Value::CreateString(cmd->GetSwitchValue("user_scrobbles"));
		CefRefPtr<CefV8Value> user_favourites = CefV8Value::CreateString(cmd->GetSwitchValue("user_favourites"));

		for (int i = 0; i<6; i++)
		{
			std::string cover_switch = "prev_track_";
			cover_switch.append(std::to_string(i));
			cover_switch.append("_cover");

			std::string text_switch = "prev_track_";
			text_switch.append(std::to_string(i));
			text_switch.append("_text");

			CefRefPtr<CefV8Value> cover = CefV8Value::CreateString(cmd->GetSwitchValue(cover_switch));
			CefRefPtr<CefV8Value> text = CefV8Value::CreateString(cmd->GetSwitchValue(text_switch));

			object->SetValue(cover_switch, cover, V8_PROPERTY_ATTRIBUTE_NONE);
			object->SetValue(text_switch, text, V8_PROPERTY_ATTRIBUTE_NONE);
		}



		object->SetValue("cppSongName", song_name, V8_PROPERTY_ATTRIBUTE_NONE);
		object->SetValue("cppArtistName", artist_name, V8_PROPERTY_ATTRIBUTE_NONE);
		object->SetValue("cppAlbumCover", album_cover, V8_PROPERTY_ATTRIBUTE_NONE);
		object->SetValue("cppArtistCover", artist_cover, V8_PROPERTY_ATTRIBUTE_NONE);
		object->SetValue("cppArtistScrobbles", artist_scrobbles, V8_PROPERTY_ATTRIBUTE_NONE);
		object->SetValue("cppAlbumScrobbles", album_scrobbles, V8_PROPERTY_ATTRIBUTE_NONE);
		object->SetValue("cppTotalScrobbles", total_scrobbles, V8_PROPERTY_ATTRIBUTE_NONE);
		object->SetValue("cppGenre", genre, V8_PROPERTY_ATTRIBUTE_NONE);
		object->SetValue("cppUserName", user_name, V8_PROPERTY_ATTRIBUTE_NONE);
		object->SetValue("cppUserAvatar", user_avatar, V8_PROPERTY_ATTRIBUTE_NONE);
		object->SetValue("cppUserArtistCount", user_artist_count, V8_PROPERTY_ATTRIBUTE_NONE);
		object->SetValue("cppUserScrobbles", user_scrobbles, V8_PROPERTY_ATTRIBUTE_NONE);
		object->SetValue("cppUserFavourites", user_favourites, V8_PROPERTY_ATTRIBUTE_NONE);

		CefRefPtr<Cef3DV8Handler> handler(new Cef3DV8Handler(this));
		CefRefPtr<CefV8Value> app = CefV8Value::CreateObject(NULL, NULL);

		CefRefPtr<CefV8Value> fnc = app->CreateFunction("exitApp", handler);
		app->SetValue("exitApp", fnc, V8_PROPERTY_ATTRIBUTE_NONE);

		object->SetValue("app", app, V8_PROPERTY_ATTRIBUTE_NONE);

		
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

		DelegateSet::iterator it = Delegates.begin();
		for (; it != Delegates.end() && !handled; ++it) {
			handled = (*it)->OnProcessMessageReceived(this, browser, source_process,
				message);
		}

		return handled;
	}
}