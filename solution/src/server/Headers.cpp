/*
** EPITECH PROJECT, 2021
** zia
** File description:
** Headers.cpp
*/
#include <iostream>
#include "server/Headers.hpp"

namespace zia::server {
	Headers::Headers(): _content(), _dummy()
	{}

	std::string			&Headers::operator[](const std::string &headerName)
	{
		return (_content[headerName]);
	}

	std::string const	&Headers::getHeader(const std::string &headerName) const
	{
		if (_content.count(headerName))
			return _content.at(headerName);
		else
			return _dummy;
		
	}

	std::string			Headers::getWholeHeaders() const
	{
		std::string tmp;
		for (auto & x : _content)
		{
			tmp += x.first;
			tmp += ": ";
			tmp += x.second;
			tmp += "\r\n";
		}
		return (tmp);
	}

	void				Headers::setHeader(const std::string &headerName,
								const std::string &value)
	{
		//check if headerName == trucs en particuler
		_content[headerName] = value;
	}
}