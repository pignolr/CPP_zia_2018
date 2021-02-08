/*
** EPITECH PROJECT, 2021
** zia
** File description:
** Server.cpp
*/

#include <boost/bind.hpp>
#include <iostream>
#include "server/Server.hpp"

namespace zia::server {
	Server::Server(dems::config::Config &config):
		_io_service(),
		_signals(_io_service),
		_ip(),
		_port(),
		_endpoint(),
		_acceptor(_io_service),
		_threadPool(THREAD_NB),
		_isRunning(false)
	{
		_signals.add(SIGINT);
		_signals.add(SIGTERM);
#if defined(SIGQUIT)
		_signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
		_signals.async_wait(
			[this](boost::system::error_code, int) {
				if (_isRunning)
					stop();
			});

		reloadConfig(config);
	}

	Server::~Server()
	{
		std::cerr << "destroy server" << std::endl;
		if (_isRunning)
			stop();
	}

	void Server::loadDefaultConfig(dems::config::Config &config)
	{
		_ip = boost::asio::ip::address_v4();
		_port = 8080;

		config["server"].v = dems::config::ConfigObject();
		auto &configServer = std::get<dems::config::ConfigObject>(config["server"].v);
		configServer["ip"].v = _ip.to_string();
		configServer["port"].v = static_cast<long long>(_port);
	}

	void Server::reloadConfig(dems::config::Config &config)
	{
		std::cout << "Setting up server..." << std::endl;
		if (!config.count("server"))
			loadDefaultConfig(config);
		else
			try {
				auto &configServer = std::get<dems::config::ConfigObject>(config["server"].v);
				if (!configServer.count("ip")) {
					_ip = boost::asio::ip::address_v4();
					configServer["ip"].v = _ip.to_string();
				} else
					try {
						auto &ip = std::get<std::string>(configServer["ip"].v);
						_ip = boost::asio::ip::address_v4::from_string(ip);
					} catch (const std::exception &) {
						_ip = boost::asio::ip::address_v4::from_string("");
					}
				if (!configServer.count("port")) {
					_port = 8080;
					configServer["port"].v = static_cast<long long>(_port);
				} else
					try {
						_port = std::get<long long>(configServer["port"].v);
					} catch (const std::exception &) {
						_port = 8080;
					}
			} catch (const std::exception &) {
				loadDefaultConfig(config);
			}

		boost::asio::ip::tcp::resolver resolver(_io_service);
		_endpoint = *resolver.resolve({_ip, _port});
		std::cout << "Server address: " << _ip << ":" << _port << std::endl;

		_acceptor.close();
		_acceptor.open(_endpoint.protocol());
		_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		_acceptor.bind(_endpoint);
		std::cout << "Server is set up" << std::endl;
	}


	void Server::waitForConnection(const Callback &cb)
	{
		auto socket = SocketPtr(new boost::asio::ip::tcp::socket(_io_service));
		_acceptor.async_accept(*socket,
			boost::bind<void>([this, socket](const Callback &callback, const boost::system::error_code &err) {
				if (!_acceptor.is_open() || err)
					return;
				boost::asio::post(_threadPool, boost::bind(callback, socket));
				waitForConnection(callback);
				}, cb, boost::asio::placeholders::error));
	}

	void Server::run(const Callback &cb)
	{
		std::cout << "Running server..." << std::endl;
		_isRunning = true;
		_acceptor.listen();

		waitForConnection(cb);
		_io_thread = boost::thread([this](){
			try {
				_io_service.run();
			} catch (std::exception &){}
		});
		std::cout << "Server is running" << std::endl;
	}

	void Server::stop()
	{
		std::cout << "Closing server..." << std::endl;
		_isRunning = false;
		_acceptor.close();
		_io_service.stop();
		if (_io_thread.joinable())
			_io_thread.join();
		std::cout << "Server is closed" << std::endl;
	}

	bool Server::isRunning() const
	{
		return _isRunning;
	}
}
