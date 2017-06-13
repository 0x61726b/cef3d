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

#pragma once

namespace Cef3D
{
	enum Cef3DJsValueTypes
	{
		CEF3D_JSVALUE_INT,
		CEF3D_JSVALUE_STRING,
		CEF3D_JSVALUE_DOUBLE,
		CEF3D_JSVALUE_FLOAT
	};
	class CEF3D_API Cef3DJsValue
	{
	public:
		Cef3DJsValue(const std::string& Name, Cef3DJsValueTypes type);
		Cef3DJsValue(const std::string& name, const std::string& value);
		Cef3DJsValue(const std::string& name, const char* value);
		Cef3DJsValue(const std::string& name, int value);
		Cef3DJsValue(const std::string& name, double value);
		Cef3DJsValue(const std::string& name, float value);

		Cef3DJsValueTypes GetType() const { return Type; }
		std::string GetName() const { return Name;  }

		void SetString(const char* value);
		void SetInt(int val);
		void SetDouble(double val);
		void SetFloat(float f);

		const std::string& GetString();
		int GetInt();
		double GetDouble();
		float GetFloat();

		bool operator==(const Cef3DJsValue& rhs) const
		{
			return GetName() == rhs.GetName();
		}
		
	protected:
		Cef3DJsValueTypes Type;
		std::string Name;
		Cef3DVariant Value;
	};
}