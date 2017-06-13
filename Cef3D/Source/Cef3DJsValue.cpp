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

	Cef3DJsValue::Cef3DJsValue(const std::string & name, const std::string & value)
		: Type(CEF3D_JSVALUE_STRING),
		Name(name)
	{
		SetString(value.c_str());
	}

	Cef3DJsValue::Cef3DJsValue(const std::string & name, const char* value)
		: Type(CEF3D_JSVALUE_STRING),
		Name(name)
	{
		SetString(value);
	}

	Cef3DJsValue::Cef3DJsValue(const std::string & name, int value)
		: Type(CEF3D_JSVALUE_INT),
		Name(name)
	{
		SetInt(value);
	}

	Cef3DJsValue::Cef3DJsValue(const std::string & name, double value)
		: Type(CEF3D_JSVALUE_DOUBLE),
		Name(name)
	{
		SetDouble(value);
	}

	Cef3DJsValue::Cef3DJsValue(const std::string & name, float value)
		: Type(CEF3D_JSVALUE_FLOAT),
		Name(name)
	{
		SetFloat(value);
	}

	void Cef3DJsValue::SetString(const char* value)
	{
		Value = value;
	}

	void Cef3DJsValue::SetInt(int val)
	{
		Value = val;
	}

	void Cef3DJsValue::SetDouble(double val)
	{
		Value = val;
	}

	void Cef3DJsValue::SetFloat(float val)
	{
		Value = val;
	}

	const std::string& Cef3DJsValue::GetString()
	{
		return Value.GetString();
	}

	int Cef3DJsValue::GetInt()
	{
		return Value.GetInt();
	}

	double Cef3DJsValue::GetDouble()
	{
		return Value.GetDouble();
	}

	float Cef3DJsValue::GetFloat()
	{
		return Value.GetFloat();
	}
	
}
