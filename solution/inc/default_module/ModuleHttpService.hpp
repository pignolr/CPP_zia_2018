/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModuleHttpService.hpp
*/

#pragma once

#include "api/Stage.hpp"

namespace zia::default_module {
	dems::CodeStatus ModuleHttpService(dems::Context &context);

	inline std::string registerHttpServiceHooks(dems::StageManager &manager) {
		manager.request().hookToMiddle(99999, "HttpService", ModuleHttpService);
		return "HttpService";
	}
}
