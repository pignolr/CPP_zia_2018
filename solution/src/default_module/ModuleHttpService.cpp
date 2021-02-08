/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModuleHttpService.cpp
*/

#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <streambuf>
#include "default_module/ModuleHttpService.hpp"

namespace zia::default_module {
	namespace fs = std::experimental::filesystem;

	dems::CodeStatus getRequestedFile(dems::Context &context, fs::path &&path)
	{
		std::ifstream file;
		try {
			file.open(path);
		} catch (std::exception &) {
			return  dems::CodeStatus::HTTP_ERROR;
		}

		std::string content;

		file.seekg(0, std::ios::end);
		content.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		content.assign((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());

		context.response.firstLine = dems::header::Response{"Http/1.1", "200", "OK"};
		if (std::get<dems::header::Request>(context.request.firstLine).method != "HEAD")
			context.response.body = content;
		return dems::CodeStatus::OK;
	}

	dems::CodeStatus ModuleHttpService(dems::Context &context)
	{
		if (!context.rawData.empty()
			|| !std::get<dems::header::Response>(context.response.firstLine).statusCode.empty())
			return dems::CodeStatus::OK;
		std::string requestFile;
		fs::path path;
		try {
			auto &requestFirstLine = std::get<dems::header::Request>(context.request.firstLine);
			requestFile = requestFirstLine.path;
			auto &serverConfig = std::get<dems::config::ConfigObject>(context.config["server"].v);
			fs::path root = std::get<std::string>(serverConfig["root"].v);
			if (root.is_absolute()) {
				path = root;
			} else {
				path = fs::current_path();
				path /= root;
			}
			if (!fs::exists(path))
				path = fs::current_path();
		}
		catch (std::exception &) {
			path = fs::current_path();
		}
		path += requestFile;
		if (!fs::is_regular_file(path)) {
			path /= "index.html";
			if (!fs::exists(path) || !fs::is_regular_file(path))
				return dems::CodeStatus::HTTP_ERROR;
		}
		return getRequestedFile(context, std::move(path));
	}
}
