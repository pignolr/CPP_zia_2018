/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModuleReader.hpp
*/

#pragma once

#include "api/Stage.hpp"

namespace zia::default_module {
	dems::CodeStatus ModuleReader(dems::Context &context);

	inline std::string registerReaderHooks(dems::StageManager &manager) {
		manager.connection().hookToEnd(100000, "DefaultReader", ModuleReader);
		manager.chunks().hookToFirst(100000, "DefaultReader", ModuleReader);
		return "DefaultReader";
	}
}
