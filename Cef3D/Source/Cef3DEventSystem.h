//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DGlobals.h
// Date: 28.04.2017
//---------------------------------------------------------------------------

#pragma once

#include <functional>

namespace Cef3D
{
	template < typename > class TMulticastDelegate;

	template <typename RType, typename... Arguments>
	class TMulticastDelegate< RType(Arguments...) >
	{
	public:
		void Add(const std::function< RType(Arguments...) > fnc)
		{
			List.push_back(fnc);
		}

		void Broadcast(Arguments... args)
		{
			for (const auto& v : List)
			{
				v(args...);
			}
		}
	private:
		std::vector<std::function<RType(Arguments...)> > List;
	};

	/*
	Used to convert functions with instances to std::function without specifying parameters
	So instead of
	auto fnc = std::bind(&Class::Function, instance, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	you can do
	auto fnc = Cef3DBind(&Class::Function, instance)
	*/
	template<typename R, typename C, typename... Args>
	std::function<R(Args...)> Cef3DBind(R(C::* func)(Args...), C* instance) {
		return [=](Args... args) { return (instance->*func)(args...); };
	}
}