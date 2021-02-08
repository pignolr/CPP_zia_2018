/*
** EPITECH PROJECT, 2021
** zia
** File description:
** HttpRequestParser.hpp
*/

#pragma once

#include "api/Stage.hpp"
#include "server/Headers.hpp"

namespace zia::default_module {

	dems::CodeStatus HttpRequest(dems::Context &cont);
	dems::CodeStatus HttpRequestChunked(dems::Context &cont);

	class HttpRequestParser
	{
		std::string _rest;
		size_t _length;
		size_t _left;
		bool _chunked;
		dems::Context &_cont;
		std::vector<std::string> _heads;
		dems::header::HTTPMessage _mess;
		dems::header::Request _req;
		//enlever le header du rawdata
		void cleanRawData(size_t);

	public:
		HttpRequestParser(dems::Context &cont);
		~HttpRequestParser();

		dems::CodeStatus setRequest();
		//remettre tout en string et en fonction de chunked ou pas choisir la bonne fonction
		dems::CodeStatus getChunk(std::vector<uint8_t> &data);
		static ssize_t getChunkSize(std::vector<uint8_t> &data);
		dems::CodeStatus getStandardBody(std::vector<uint8_t> &data);
		static void mySplit(std::vector<std::string> &dest, std::string &line, std::string const &delim);
		static int checkFirstline(std::string &, dems::Context &);
	};

	inline std::string registerHttpRequestHooks(dems::StageManager &manager) {
		manager.connection().hookToEnd(200000, "HttpRequest", HttpRequest);
		manager.chunks().hookToMiddle(200000, "HttpRequestChunked", HttpRequestChunked);
		return "HttpRequest";
	}
}
