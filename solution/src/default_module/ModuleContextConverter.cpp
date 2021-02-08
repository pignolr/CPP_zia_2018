/*
** EPITECH PROJECT, 2019
** zia
** File description:
** code status
*/
#include <iostream>
#include "default_module/ModuleContextConverter.hpp"

namespace zia::default_module {
        dems::CodeStatus ModuleContextConverter(dems::Context &context)
        {
		std::string tmp;
		auto &response = context.response;
		try {
			auto &firstLine = std::get<dems::header::Response>(response.firstLine);
			tmp = firstLine.httpVersion + ' ' + firstLine.statusCode
				+ ' ' + firstLine.message + "\r\n";
		} catch (std::exception &) {
			return dems::CodeStatus::HTTP_ERROR;
		}
		if (response.headers != nullptr) {
			tmp += response.headers->getWholeHeaders();
		}
		tmp += "\r\n" + response.body;
		context.rawData.clear();
		for (auto &it : tmp)
			context.rawData.push_back(it);
		return dems::CodeStatus::OK;
        }
}
