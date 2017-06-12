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
		CEF3D_JSVALUE_STRING
	};
	class CEF3D_API Cef3DJsValue
	{
	public:
		Cef3DJsValue(const std::string& Name, Cef3DJsValueTypes type);

		Cef3DJsValueTypes GetType() const { return Type; }
		std::string GetName() const { return Name;  }

		static Cef3DJsValue CreateInt(const std::string& Name, int value);
		static Cef3DJsValue CreateString(const std::string& Name, const std::string& value);

	private:
		void OnValueChanged();
		
	protected:
		Cef3DJsValueTypes Type;
		std::string Name;
	};

	class CEF3D_API Cef3DJsInt : public Cef3DJsValue
	{
	public:
		Cef3DJsInt(const std::string& Name, int value);

		static Cef3DJsValue Create(const std::string& Name, int value);

	
		void SetValue(int value);
		int GetValue() const { return Value;  }
	protected:
		int Value;
	};

	class CEF3D_API Cef3DJsString : public Cef3DJsValue
	{
	public:
		Cef3DJsString(const std::string& Name, const std::string& value);

		static Cef3DJsValue Create(const std::string& Name, const std::string& value);

		void SetValue(const std::string& value);
		std::string GetValue() const { return Value;}
	protected:
		std::string Value;
	};
}