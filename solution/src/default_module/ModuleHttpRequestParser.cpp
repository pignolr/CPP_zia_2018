/*
** EPITECH PROJECT, 2021
** zia
** File description:
** HttpRequestParser.cpp
*/

#include <cstdlib>
#include <cstring>
#include <variant>
#include "default_module/ModuleHttpRequestParser.hpp"

namespace zia::default_module {

	dems::CodeStatus HttpRequest(dems::Context &cont)
	{
		HttpRequestParser request(cont);

		auto ret = request.setRequest();
		return ret;
	}

	dems::CodeStatus HttpRequestChunked(dems::Context &cont)
	{
		HttpRequestParser request(cont);

		auto ret = request.setRequest();
		if (ret != dems::CodeStatus::OK)
			return (ret);
		while ((ret = request.getChunk(cont.rawData)) != dems::CodeStatus::DECLINED)
			if (ret == dems::CodeStatus::HTTP_ERROR)
				return (dems::CodeStatus::DECLINED);
		return (dems::CodeStatus::OK);
	}

	HttpRequestParser::HttpRequestParser(dems::Context &cont) :
	_length(0), _left(0), _chunked(false), _cont(cont)
	{
		std::string data = "";

		if (!_cont.rawData.size())
			return;
		for (auto &x : _cont.rawData)
			data += x;
		_rest = data;
		zia::default_module::HttpRequestParser::mySplit(_heads, data, "\r\n");
	}

	HttpRequestParser::~HttpRequestParser()
	{ }

	dems::CodeStatus HttpRequestParser::setRequest()
	{
		size_t i;
		std::vector<std::string> line;

		if (!_heads.size())
			return dems::CodeStatus::DECLINED;
		if (zia::default_module::HttpRequestParser::checkFirstline(_heads[0], _cont) == 1)
			return (dems::CodeStatus::HTTP_ERROR);
		_heads.erase(_heads.begin());
		for (i = 0; i < _heads.size(); ++i)
		{
			if (_heads[i] == "")
				break;
			zia::default_module::HttpRequestParser::mySplit(line, _heads[i], ": ");
			if (line[0].compare("Content-Length"))
			{
				_length = std::atoi(line[1].c_str());
				_left = _length;
			}
			if (line[0].compare("Transfer-Encoding") && line[1].compare("chunked"))
				_chunked = true;
			_cont.request.headers->setHeader(line[0], line[1]);
			line.clear();
		}
		cleanRawData(++i);
		if (_chunked == false)
			getStandardBody(_cont.rawData);
		return (dems::CodeStatus::OK);
	}

	void HttpRequestParser::cleanRawData(size_t i)
	{
		_rest.clear();
		if (i <= _heads.size())
			_heads.erase(_heads.begin(), _heads.begin() + i);
		for (auto & line : _heads)
		{
			_rest += line;
			_rest += "\r\n";
		}
		_cont.rawData.clear();
		if (_length && _rest.size() > _length)
			std::copy(_rest.begin(), _rest.begin() + _length, std::back_inserter(_cont.rawData));
		else
			std::copy(_rest.begin(), _rest.end(), std::back_inserter(_cont.rawData));
	}

	dems::CodeStatus HttpRequestParser::getChunk(std::vector<uint8_t> &data)
	{
		std::string body;
		size_t chunkSize;
		size_t total;

		for (auto & c : data)
			body += c;
		total = body.substr(0, body.find_first_of("\r\n")).length();
		if ((chunkSize = getChunkSize(_cont.rawData)) == 0)
		{
			data.clear();
			return (dems::CodeStatus::DECLINED);
		}
		try
		{
			if (!body.substr(chunkSize - 2).compare("\r\n"))
				return (dems::CodeStatus::HTTP_ERROR);
		}
		catch (const std::exception& e)
		{
			return (dems::CodeStatus::HTTP_ERROR);
		}
		_rest += body.substr(body.find_first_of("\r\n") + 2, chunkSize);
		_cont.request.body = _rest;
		data.erase(data.begin(), data.begin() + total + 2 + chunkSize);
		return (dems::CodeStatus::OK);
	}

	ssize_t HttpRequestParser::getChunkSize(std::vector<uint8_t> &data)
	{
		std::string body;
		size_t chunkSize;

		for (auto & c : data)
			body += c;
		try
		{
			chunkSize = std::stoul(body.substr(0, body.find_first_of("\r\n")), nullptr, 16);
		}
		catch (const std::exception& e)
		{
			return (0);
		}
		if (chunkSize == 0)
			return (0);
		return (chunkSize);
	}

	dems::CodeStatus HttpRequestParser::getStandardBody(std::vector<uint8_t> &data)
	{
		std::string body;

		for (auto & c : data)
			body += c;
		if (_length != 0 && _length != body.length())
			return (dems::CodeStatus::DECLINED);
		_cont.request.body = body;
		data.clear();
		return (dems::CodeStatus::OK);
	}

	void HttpRequestParser::mySplit(std::vector<std::string> &dest, std::string &line, std::string const &delim)
	{
		std::string token = "";
		bool reg;

		for (size_t current = 0; current < line.length(); ++current)
		{
			reg = false;
			token += line[current];
			if (line[current] == delim[0])
			{
				reg = true;
				for (size_t j = 0; j < delim.length(); ++j)
					if ((current + j) < line.length() && line[current + j] != delim[j])
						reg = false;
				if (reg == true)
				{
					dest.push_back(token.substr(0, token.length() - 1));
					current += delim.length() - 1;
					token.clear();
				}
			}
		}
		if (token.length() > delim.length())
			dest.push_back(token);
	}

	int HttpRequestParser::checkFirstline(std::string &line, dems::Context &cont)
	{
		std::vector<std::string> Firstline;
		std::vector<std::string> methods = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"};
		std::vector<std::string> versions = {"HTTP/0.9", "HTTP/1.0", "HTTP/1.1"};
		std::string splitChar = " ";

		zia::default_module::HttpRequestParser::mySplit(Firstline, line, splitChar);
		std::get<dems::header::Request>(cont.request.firstLine).method = "";
		for (auto &method : methods)
			if (Firstline[0].compare(method))
			{
				std::get<dems::header::Request>(cont.request.firstLine).method = Firstline[0];
				break ;
			}
		if (std::get<dems::header::Request>(cont.request.firstLine).method == "")
			return (1);
		std::get<dems::header::Request>(cont.request.firstLine).path = Firstline[1];
		std::get<dems::header::Request>(cont.request.firstLine).httpVersion = "";
		for (auto &v : versions)
			if (Firstline[2].compare(v))
			{
				std::get<dems::header::Request>(cont.request.firstLine).httpVersion = Firstline[2];
				break ;
			}
		if (std::get<dems::header::Request>(cont.request.firstLine).httpVersion == "")
			return (1);
		return (0);
	}
}
