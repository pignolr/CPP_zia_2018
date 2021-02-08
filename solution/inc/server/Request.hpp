/*
** EPITECH PROJECT, 2021
** zia
** File description:
** Request.hpp
*/

#pragma once

#include "api/Stage.hpp"
#include "server/Server.hpp"

namespace zia::server {
	class Request {
		zia::server::SocketPtr _socket;
		dems::StageManager &_stageManager;
		dems::Context _context;

		void handleChunks();
	public:
		/* Suppression des constructeur par copie */
		Request& operator=(const Request &) = delete;
		Request(const Request &) = delete;

		/* ctor and dtor */
		explicit Request(dems::config::Config &config, dems::StageManager &stageManager, zia::server::SocketPtr &&);
		~Request() = default;

		/* method */
		void handleRequest();
		void handleDisconnect();
	};
}
