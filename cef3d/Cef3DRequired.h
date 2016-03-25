//----------------------------------------------------------------------------
//Yume Engine
//Copyright (C) 2015  arkenthera
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/
//----------------------------------------------------------------------------
//
// File : <Filename>
// Date : <Date>
// Comments :
//
//----------------------------------------------------------------------------
#ifndef __Cef3DRequired_h__
#define __Cef3DRequired_h__
//----------------------------------------------------------------------------

#	if YUME_PLATFORM == YUME_PLATFORM_WIN32
#		if defined(BUILDING_CEF3D)
#			define Cef3DExport 
#		else
#			define Cef3DExport
#		endif
#	else
#	define Cef3DExport	
#	endif

namespace CefUI
{
	struct CefRect
	{
		int x;
		int y;
		int width;
		int height;

		CefRect()
		{
			x = 0;
			y = 0;
			width = 0;
			height = 0;
		}

		CefRect(int x_,int y_,int w,int h)
			: x(x_),y(y_),width(w),height(h)
		{
		}
	};
}

//----------------------------------------------------------------------------
#endif
