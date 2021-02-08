/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ShellServer.hpp
*/

#pragma once

#include "dlloader/ModulesManager.hpp"
#include "server/Server.hpp"
#include "server/ConfigManager.hpp"

namespace zia::server {
	class ShellServer {
		using ShellCommand = std::vector<std::string>;

		using ShellCommandFunc = std::function<void(ShellCommand &&)>;
		using ShellCommandFuncMap = std::unordered_map<std::string, ShellCommandFunc>;

		const ShellCommandFuncMap initShellCommandFuncMap();

		void runServer(ShellCommand &&);
		void stopServer(ShellCommand &&);
		void reloadConfig(ShellCommand &&);
		void affHelp(ShellCommand &&);

		std::string _configFile;
		zia::server::ConfigManager _configManager;
		dems::config::Config _config;

		dlloader::ModulesManager _modulesManager;
		std::unique_ptr<Server> _server;

		const ShellCommandFuncMap _shellCommandFuncMap;
	public:
		/* Suppression des constructeur par copie */
		ShellServer& operator=(const ShellServer &) = delete;
		ShellServer(const ShellServer &) = delete;

		/* ctor */
		ShellServer(std::string &&);
		ShellServer();

		virtual ~ShellServer() = default;

		void run();
	};
}
