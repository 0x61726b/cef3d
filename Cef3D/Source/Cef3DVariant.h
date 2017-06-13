//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DUtils.h
// Date: 13.06.2017
//---------------------------------------------------------------------------

#pragma once

#include "Variant.h"

namespace Cef3D
{
	extern std::string EmptyString;

	enum Cef3DVariantType
	{
		VAR_EMPTY,
		VAR_INT,
		VAR_DOUBLE,
		VAR_FLOAT,
		VAR_STRING,
		MAX_VAR_TYPES
	};

	class CEF3D_API Cef3DVariant
	{
	public:
		Cef3DVariant() :
			Type(VAR_EMPTY)
		{
		}

		Cef3DVariant(const std::string& str) :
			Type(VAR_EMPTY)
		{
			*this = str;
		}

		Cef3DVariant(int value) :
			Type(VAR_EMPTY)
		{
			*this = value;
		}

		Cef3DVariant(double value) :
			Type(VAR_EMPTY)
		{
			*this = value;
		}

		Cef3DVariant(float value) :
			Type(VAR_EMPTY)
		{
			*this = value;
		}

		//Cef3DVariant& operator =(const std::string& rhs)
		//{
		//	SetType(VAR_STRING);
		//	*(reinterpret_cast<std::string*>(&Value)) = rhs;
		//	return *this;
		//}

		Cef3DVariant& operator =(const char* rhs)
		{
			Value = std::string(rhs);
			return *this;
		}

		Cef3DVariant& operator =(int rhs)
		{
			Value = rhs;
			return *this;
		}


		Cef3DVariant& operator =(unsigned rhs)
		{
			Value = (int)rhs;
			return *this;
		}

		Cef3DVariant& operator =(float rhs)
		{
			Value = rhs;
			return *this;
		}


		Cef3DVariant& operator = (double rhs)
		{
			Value = rhs;
			return *this;
		}

		~Cef3DVariant()
		{
			
		}


		int GetInt() const
		{
			return nonstd::get<int>(Value);;
		}

		float GetFloat() const
		{
			return nonstd::get<float>(Value);;
		}

		double GetDouble() const
		{
			return nonstd::get<double>(Value);;
		}

		const std::string& GetString() const
		{
			return nonstd::get<std::string>(Value);
		}

		Cef3DVariantType GetType() const { return Type;  }
	private:
		Cef3DVariantType Type;
		nonstd::variant<int,double,float, std::string> Value;
	};
}