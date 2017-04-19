//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DUtils.h
// Date: 19.04.2017
//---------------------------------------------------------------------------

#pragma once

#include "include/cef_base.h"

namespace Cef3D
{
	namespace Cef3DPrivate
	{
		inline cef_log_severity_t Cef3DLogLevelToCef(Cef3DLogLevel Level)
		{
			switch (Level)
			{
			case Cef3DLogLevel::Default:
				return LOGSEVERITY_DEFAULT;
			case Cef3DLogLevel::Error:
				return LOGSEVERITY_ERROR;
			case Cef3DLogLevel::Info:
				return LOGSEVERITY_INFO;
			case Cef3DLogLevel::None:
				return LOGSEVERITY_DISABLE;
			case Cef3DLogLevel::Verbose:
				return LOGSEVERITY_VERBOSE;
			case Cef3DLogLevel::Warning:
				return LOGSEVERITY_WARNING;
			default:
				return LOGSEVERITY_DEFAULT;
			}
		}

		inline CefBrowserSettings Cef3DBrowserDefinitionToCef(const Cef3DBrowserDefinition& Definition)
		{
			CefBrowserSettings settings;
			return settings;
		}
	}
}