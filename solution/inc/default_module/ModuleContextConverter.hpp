/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModuleContextConverter.hpp
*/

#pragma once
#include "api/Stage.hpp"

namespace zia::default_module {
	dems::CodeStatus ModuleContextConverter(dems::Context &context);

	inline std::string registerContextConverterHooks(dems::StageManager &manager) {
		manager.request().hookToEnd(100000, "ContextConverter", ModuleContextConverter);
		return "ContextConverter";
	}
}
