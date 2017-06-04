//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 2.6.2017
//---------------------------------------------------------------------------

#pragma once

#include "include/cef_command_line.h"

namespace Cef3D
{
	class CEF3D_API Cef3DCommandLine
	{
	public:
		Cef3DCommandLine(CefRefPtr<CefCommandLine> cmdLine);

		void InitFromString(const std::string& cmd);

		void AppendSwitch(const std::string& sw);
		void AppendArgument(const std::string& value);
		void AppendSwitchWithValue(const std::string& sw, const std::string& value);

		bool HasSwitch(const std::string& sw);
		std::string GetSwitchValue(const std::string& sw);

		friend class MainContext;

	private:
		CefRefPtr<CefCommandLine> GetCefCmdLine() { return CmdLine;  }

	private:
		void CheckIfValid();
		CefRefPtr<CefCommandLine> CmdLine;
	};
}

