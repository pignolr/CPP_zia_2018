/*
** EPITECH PROJECT, 2021
** zia
** File description:
** HttpResponseParser.hpp
*/

#pragma once

#include "api/Stage.hpp"
#include "server/Headers.hpp"

namespace zia::default_module {

	dems::CodeStatus HttpResponse(dems::Context &cont);
	dems::CodeStatus HttpResponseChunked(dems::Context &cont);
	
	class HttpResponseParser
	{
		std::string _rest;
		size_t _length;
		size_t _left;
		bool _chunked;
		dems::Context &_cont;
		std::vector<std::string> _heads;
		dems::header::HTTPMessage _mess;
		dems::header::Response _res;
		//enlever le header du rawdata
		void cleanRawData(size_t);

	public:
		HttpResponseParser(dems::Context &cont);
		~HttpResponseParser();

		dems::CodeStatus setResponse();
		//remettre tout en string et en fonction de chunked ou pas choisir la bonne fonction
		dems::CodeStatus getChunk(std::vector<uint8_t> &data);
		ssize_t getChunkSize(std::string &body);
		dems::CodeStatus getStandardBody(std::vector<uint8_t> &data);
		static void mySplit(std::vector<std::string> &dest, std::string &line, std::string const &delim);
		static int checkFirstline(std::string &, dems::Context &);
	};

	inline std::string registerHttpResponseHooks(dems::StageManager &manager) {
		manager.request().hookToEnd(99999, "HttpResponse", HttpResponse);
		return "HttpResponse";
	}
}
