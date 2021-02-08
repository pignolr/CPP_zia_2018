/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ShellServer.cpp
*/

#include <boost/algorithm/string.hpp>
#include "server/ShellServer.hpp"
#include "server/Request.hpp"


namespace zia::server {
	const ShellServer::ShellCommandFuncMap ShellServer::initShellCommandFuncMap()
	{
		ShellServer::ShellCommandFuncMap map;

		map.emplace("run", [this](ShellCommand &&command)
			{ runServer(std::move(command)); });
		map.emplace("stop", [this](ShellCommand &&command)
			{ stopServer(std::move(command)); });
		map.emplace("reload", [this](ShellCommand &&command)
			{ reloadConfig(std::move(command)); });
		map.emplace("help", [this](ShellCommand &&command)
			{ affHelp(std::move(command)); });
		return map;
	}

	ShellServer::ShellServer(std::string &&configFile):
		_configFile(configFile),
		_configManager(_configFile),
		_config(std::move(_configManager.getConfig())),
		_modulesManager(_config),
		_server(nullptr),
		_shellCommandFuncMap(initShellCommandFuncMap())
	{}


	ShellServer::ShellServer():
		_configFile(),
		_configManager(),
		_config(std::move(_configManager.getConfig())),
		_modulesManager(_config),
		_server(nullptr),
		_shellCommandFuncMap(initShellCommandFuncMap())
	{}

	void ShellServer::affHelp(ShellCommand &&)
	{
		std::cout << "Command:" << std::endl;
		std::cout << "run" << "\t\t\t" << "run the server" << std::endl;
		std::cout << "stop" << "\t\t\t" << "stop the server" << std::endl;
		std::cout << "reload config_file" << "\t" << "reload the config file of the server (server must be stopped)" << std::endl;
		std::cout << "help" << "\t\t\t" << "show the list of command" << std::endl;
	}

	void ShellServer::runServer(ShellCommand &&)
	{
		if (_server && _server->isRunning()) {
			std::cerr << "The server is already running." << std::endl;
		} else {
			_server = std::make_unique<Server>(_config);
			auto handleRequest = zia::server::Callback(std::bind(
				[](dems::config::Config &config, dems::StageManager &stageManager, zia::server::SocketPtr socket)-> void{
					// lambda who handle a request
					try {
						Request request(config, stageManager, std::move(socket));
						request.handleRequest();
						request.handleDisconnect();
					}
					catch (const std::exception &e){
						std::cerr << "Error in Request socket: " << e.what() << std::endl;
					}
				}, _config, _modulesManager.getStageManager(), std::placeholders::_1));

			_server->run(handleRequest);
		}
	}

	void ShellServer::stopServer(ShellCommand &&)
	{
		if (_server && !_server->isRunning()) {
			std::cerr << "The server is already stopped." << std::endl;
		} else {
			_server->stop();
			_server.reset(nullptr);
		}
	}

	void ShellServer::reloadConfig(ShellCommand &&command)
	{
		if (command.size() != 2) {
			std::cerr << "Invalid number of argument: reload config_file" << std::endl;
		} else {
			_configFile = std::move(command[1]);
			_configManager.reloadConfig(_configFile);
			_config = std::move(_configManager.getConfig());
			_modulesManager.reloadConfig(_config);
		}
	}

	void ShellServer::run()
	{
		std::string input;
		std::string command;
		ShellCommand args;

		while (!std::cin.eof()) {
			/* print shell */
			std::cout << "> ";
			/* get input from user */
			std::getline(std::cin, input);
			/* separate input */
			boost::algorithm::trim(input);
			boost::algorithm::split(args, input, boost::is_any_of("\t "),boost::token_compress_on);
			command = args[0];

			/* execute command */
			if (command.empty())
				continue;
			else if(_shellCommandFuncMap.count(command))
				_shellCommandFuncMap.at(command)(std::move(args));
			else if (command == "exit" || command =="quit")
				break;
			else
				std::cerr << "Invalid Command: " << command << std::endl;
		}
	}
}
