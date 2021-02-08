/*
** EPITECH PROJECT, 2021
** zia
** File description:
** Html.hpp
*/

#pragma once

#include "api/Stage.hpp"

namespace zia::server {
	class Headers : public dems::header::IHeaders
	{
		std::map<std::string, std::string> _content;
		std::string _dummy;
	public:
		Headers();
		~Headers() = default;

		std::string &operator[](const std::string &headerName);
		std::string const &getHeader(const std::string &headerName) const;
		std::string getWholeHeaders() const;
		void setHeader(const std::string &headerName, const std::string &value);
	};
}