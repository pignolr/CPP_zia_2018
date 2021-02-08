/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModuleWriter.hpp
*/

#pragma once

#include "api/Stage.hpp"

namespace zia::default_module {
	dems::CodeStatus ModuleWriter(dems::Context &context);

	inline std::string registerWriterHooks(dems::StageManager &manager) {
		manager.disconnect().hookToFirst(100000, "DefaultWriter", ModuleWriter);
		return "DefaultWriter";
	}
}
