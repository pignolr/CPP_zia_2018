/*
** EPITECH PROJECT, 2021
** zia
** File description:
** IDynamicLibrary.hpp
*/

#pragma once
#include <string>

namespace zia::dlloader {
	class IDynamicLibrary {
	public:
		/* Méthodes de la classe */
		virtual ~IDynamicLibrary() {};
		virtual void *loadSymbol(const std::string &&symbolName) = 0;
	};
}
