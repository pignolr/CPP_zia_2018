/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModuleDefaultPage.cpp
*/

#include "default_module/ModuleDefaultPage.hpp"

namespace zia::default_module {
	dems::CodeStatus ModuleDefaultPage(dems::Context &context)
	{
		if (!context.rawData.empty()
			|| !std::get<dems::header::Response>(context.response.firstLine).statusCode.empty())
			return dems::CodeStatus::OK;
		context.response.firstLine = dems::header::Response{"HTTP/1.1", "404", "Not Found"};
		if (std::get<dems::header::Request>(context.request.firstLine).method != "HEAD")
			context.response.body = "<head></head><body><h1>Page Not Found</h1></body>";
		return dems::CodeStatus::OK;
	}
}
