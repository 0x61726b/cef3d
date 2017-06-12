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
#include "include/base/cef_bind.h"
#include "include/base/cef_scoped_ptr.h"
#include "include/cef_task.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/cef_v8.h"

#define CURRENTLY_ON_MAIN_THREAD() CefCurrentlyOn(TID_UI)
#define REQUIRE_MAIN_THREAD() DCHECK(CURRENTLY_ON_MAIN_THREAD())
#define MAIN_POST_TASK(task) CefPostTask(TID_UI, task);
#define MAIN_POST_CLOSURE(closure) CefPostTask(TID_UI, (CefCreateClosureTask(closure)));

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

			if (Definition.Type == Cef3DBrowserType::Offscreen)
				settings.windowless_frame_rate = 60;
			return settings;
		}
	}

	FORCEINLINE Cef3DRect CefRectToCef3D(const CefRect& rect)
	{
		return Cef3DRect(rect.width, rect.height);
	}

	FORCEINLINE CefRect Cef3DRectToCefRect(const Cef3DRect& rect)
	{
		return CefRect(rect.X, rect.Y, rect.Width, rect.Height);
	}

	FORCEINLINE int LogicalToDevice(int value, float device_scale_factor) {
		float scaled_val = static_cast<float>(value) * device_scale_factor;
		return static_cast<int>(std::floor(scaled_val));
	}

	FORCEINLINE CefRect LogicalToDevice(const CefRect& value, float device_scale_factor) {
		return CefRect(LogicalToDevice(value.x, device_scale_factor),
			LogicalToDevice(value.y, device_scale_factor),
			LogicalToDevice(value.width, device_scale_factor),
			LogicalToDevice(value.height, device_scale_factor));
	}

	FORCEINLINE int DeviceToLogical(int value, float device_scale_factor) {
		float scaled_val = static_cast<float>(value) / device_scale_factor;
		return static_cast<int>(std::floor(scaled_val));
	}

	FORCEINLINE void DeviceToLogical(CefMouseEvent& value, float device_scale_factor) {
		value.x = DeviceToLogical(value.x, device_scale_factor);
		value.y = DeviceToLogical(value.y, device_scale_factor);
	}

	FORCEINLINE CefRect GetPopupRectInWebView(const CefRect& original_rect,int view_width_,int view_height_) {
		CefRect rc(original_rect);
		// if x or y are negative, move them to 0.
		if (rc.x < 0)
			rc.x = 0;
		if (rc.y < 0)
			rc.y = 0;
		// if popup goes outside the view, try to reposition origin
		if (rc.x + rc.width > view_width_)
			rc.x = view_width_ - rc.width;
		if (rc.y + rc.height > view_height_)
			rc.y = view_height_ - rc.height;
		// if x or y became negative, move them to 0 again.
		if (rc.x < 0)
			rc.x = 0;
		if (rc.y < 0)
			rc.y = 0;
		return rc;
	}

	struct DeleteOnMainThread {
		template<typename T>
		static void Destruct(const T* x) {
			if (CURRENTLY_ON_MAIN_THREAD()) {
				delete x;
			}
			else {
				MAIN_POST_CLOSURE(
					base::Bind(&DeleteOnMainThread::Destruct<T>, x));
			}
		}
	};

	inline void SetList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target);
	inline void SetList(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target);
	inline void SetListValue(CefRefPtr<CefListValue> list, int index, CefRefPtr<CefV8Value> value);
	inline void SetListValue(CefRefPtr<CefV8Value> list, int index, CefRefPtr<CefListValue> value);
}