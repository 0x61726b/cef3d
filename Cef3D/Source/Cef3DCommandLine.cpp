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

#include "Cef3DPCH.h"

#include "include/cef_command_line.h"

namespace Cef3D
{
	Cef3DCommandLine::Cef3DCommandLine(CefRefPtr<CefCommandLine> cmdLine)
	{
		CmdLine = cmdLine;
	}

	void Cef3DCommandLine::InitFromString(const std::string & cmd)
	{
		CmdLine->InitFromString(cmd);
	}

	void Cef3DCommandLine::AppendSwitch(const std::string & sw)
	{
		CheckIfValid();

		CmdLine->AppendSwitch(sw);
	}
	void Cef3DCommandLine::AppendArgument(const std::string & value)
	{
		CheckIfValid();

		CmdLine->AppendArgument(value);
	}
	void Cef3DCommandLine::AppendSwitchWithValue(const std::string & sw, const std::string & value)
	{
		CheckIfValid();

		CmdLine->AppendSwitchWithValue(sw, value);
	}
	bool Cef3DCommandLine::HasSwitch(const std::string & sw)
	{
		CheckIfValid();

		return CmdLine->HasSwitch(sw);
	}
	std::string Cef3DCommandLine::GetSwitchValue(const std::string & sw)
	{
		CheckIfValid();

		return CmdLine->GetSwitchValue(sw);
	}
	void Cef3DCommandLine::CheckIfValid()
	{
		DCHECK(CmdLine->IsValid());
		DCHECK(!CmdLine->IsReadOnly());
	}
}