//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DJsValue.h
// Date: 11.06.2017
//---------------------------------------------------------------------------
#include "Cef3DPCH.h"

namespace Cef3D
{
	Cef3DJsValue::Cef3DJsValue(const std::string& name, Cef3DJsValueTypes type)
		: Type(type),
		Name(name)
	{
	}

	Cef3DJsValue Cef3DJsValue::CreateInt(const std::string& Name, int value)
	{
		return Cef3DJsInt::Create(Name, value);
	}

	Cef3DJsValue Cef3DJsValue::CreateString(const std::string& Name, const std::string & value)
	{
		return Cef3DJsString(Name, value);
	}

	void Cef3DJsValue::OnValueChanged()
	{
	}

	/* Int Js type on browser process */
	Cef3DJsInt::Cef3DJsInt(const std::string& Name,int value)
		: Cef3DJsValue(Name, CEF3D_JSVALUE_INT),
		Value(value)
	{
		DCHECK(CURRENTLY_ON_MAIN_THREAD());
	}
	Cef3DJsValue Cef3DJsInt::Create(const std::string& Name, int value)
	{
		DCHECK(CURRENTLY_ON_MAIN_THREAD());
		return Cef3DJsInt(Name, value);
	}
	void Cef3DJsInt::SetValue(int value)
	{
		DCHECK(CURRENTLY_ON_MAIN_THREAD());

		Value = value;
	}

	/* String Js type on browser process */
	Cef3DJsString::Cef3DJsString(const std::string& Name, const std::string & value)
		: Cef3DJsValue(Name, CEF3D_JSVALUE_STRING),
		Value(value)
	{
		DCHECK(CURRENTLY_ON_MAIN_THREAD());
	}

	Cef3DJsValue Cef3DJsString::Create(const std::string& Name, const std::string & value)
	{
		DCHECK(CURRENTLY_ON_MAIN_THREAD());

		return Cef3DJsString(Name, value);
	}

	void Cef3DJsString::SetValue(const std::string & value)
	{
		DCHECK(CURRENTLY_ON_MAIN_THREAD());

		Value = value;
	}
}
