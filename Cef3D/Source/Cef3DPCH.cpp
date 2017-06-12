//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.cpp
// Date: 13.04.2017
//---------------------------------------------------------------------------

#include "Cef3DPCH.h"

CefRefPtr<Cef3D::Cef3DBrowserApp> GCef3DBrowserApp = 0;
Cef3D::MainContext* GMainContext = 0;
Cef3D::Cef3DCommandLine* GCmdLine = 0;


Cef3D::Cef3DDefinition::Cef3DDefinition()
{
	std::string rootPath = Cef3DPaths::Root();
	std::string logPath = Cef3DPaths::Log();
	std::string subProcessPath;
	logPath = Cef3DPaths::Combine(logPath, "Cef3D.log");

#if PLATFORM_WINDOWS
	subProcessPath = Cef3DPaths::Combine(rootPath, "CefSubProcess.exe");
#else
	subProcessPath = Cef3DPaths::Combine(rootPath, "CefSubProcess");
#endif

	ChildProcessPath = subProcessPath;
	LogPath = logPath;
	LogLevel = Cef3DLogLevel::Default;
	UserAgent = "Cef3D-Default-UA";
	OffscreenRendering = false;
	UseCefLoop = false;
}

Cef3D::Cef3DBrowserDefinition::Cef3DBrowserDefinition()
{
	Type = Cef3D::Cef3DBrowserType::Offscreen;
}

namespace Cef3D
{
	// Transfer a V8 array to a List.
	inline void SetList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target) {

		int arg_length = source->GetArrayLength();
		if (arg_length == 0)
			return;

		// Start with null types in all spaces.
		target->SetSize(arg_length);

		for (int i = 0; i < arg_length; ++i)
			SetListValue(target, i, source->GetValue(i));
	}

	inline CefRefPtr<CefV8Value> ListValueToV8Value(CefRefPtr<CefListValue> value, int index) {
		CefRefPtr<CefV8Value> new_value;

		CefValueType type = value->GetType(index);

		switch (type) {
		case VTYPE_LIST: {
			CefRefPtr<CefListValue> list = value->GetList(index);
			new_value = CefV8Value::CreateArray((int)list->GetSize());
			SetList(list, new_value);
		} break;
		case VTYPE_BOOL:
			new_value = CefV8Value::CreateBool(value->GetBool(index));
			break;
		case VTYPE_DOUBLE:
			new_value = CefV8Value::CreateDouble(value->GetDouble(index));
			break;
		case VTYPE_INT:
			new_value = CefV8Value::CreateInt(value->GetInt(index));
			break;
		case VTYPE_STRING:
			new_value = CefV8Value::CreateString(value->GetString(index));
			break;
		default:
			new_value = CefV8Value::CreateNull();
			break;
		}

		return new_value;
	}

	// Transfer a List value to a V8 array index.
	inline void SetListValue(
		CefRefPtr<CefV8Value> list,
		int index,
		CefRefPtr<CefListValue> value) {

		CefRefPtr<CefV8Value> new_value;

		CefValueType type = value->GetType(index);
		switch (type) {
		case VTYPE_LIST: {
			CefRefPtr<CefListValue> new_list = value->GetList(index);
			new_value = CefV8Value::CreateArray((int)new_list->GetSize());
			SetList(new_list, new_value);
		} break;
		case VTYPE_BOOL:
			new_value = CefV8Value::CreateBool(value->GetBool(index));
			break;
		case VTYPE_DOUBLE:
			new_value = CefV8Value::CreateDouble(value->GetDouble(index));
			break;
		case VTYPE_INT:
			new_value = CefV8Value::CreateInt(value->GetInt(index));
			break;
		case VTYPE_STRING:
			new_value = CefV8Value::CreateString(value->GetString(index));
			break;
		case VTYPE_NULL:
			new_value = CefV8Value::CreateNull();
			break;
		default:
			break;
		}

		if (new_value.get()) {
			list->SetValue(index, new_value);
		}
		else {
			list->SetValue(index, CefV8Value::CreateNull());
		}
	}

	// Transfer a List to a V8 array.
	inline void SetList(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target) {

		int arg_length = (int)source->GetSize();
		if (arg_length == 0)
			return;

		for (int i = 0; i < arg_length; ++i)
			SetListValue(target, i, source);
	}
}