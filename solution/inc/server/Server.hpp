/*
** EPITECH PROJECT, 2021
** zia
** File description:
** Server.hpp
*/

#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "api/Config.hpp"

namespace zia::server {
	using SocketPtr = boost::shared_ptr<boost::asio::ip::tcp::socket>;
	using Callback = std::function<void(SocketPtr)>;

	class Server {
		static constexpr std::size_t THREAD_NB = 256;

		/* paramètres boost pour le server Tcp */
		boost::asio::io_service _io_service;
		boost::thread _io_thread;
		boost::asio::signal_set _signals;

		boost::asio::ip::address_v4 _ip;
		unsigned short _port;

		boost::asio::ip::tcp::endpoint _endpoint;
		boost::asio::ip::tcp::acceptor _acceptor;

		boost::asio::thread_pool _threadPool;

		bool _isRunning;

		void waitForConnection(const Callback &cb);

		void reloadConfig(dems::config::Config &);
		void loadDefaultConfig(dems::config::Config &);

	public:
		/* Suppression des constructeur par copie */
		Server& operator=(const Server &) = delete;
		Server(const Server &) = delete;

		/* ctor et dtor */
		explicit Server(dems::config::Config &);
		~Server();

		void run(const Callback &cb);
		void stop();

		bool isRunning() const;
	};
}
