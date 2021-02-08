/*
** EPITECH PROJECT, 2021
** zia
** File description:
** SSLConnection.cpp
*/

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#ifdef __linux__
	#include <unistd.h>
#endif
#include "SSLConnection.hpp"

namespace zia::ssl_module {
	int my_ssl_verify_peer(int, X509_STORE_CTX*) {
		return 1;
	}

	bool SSLConnection::initCertificate()
	{
		// check certificate
		try {
			if (!_context.config.count("modules"))
				return true;
			auto &confModule = std::get<dems::config::ConfigObject>(_context.config["modules"].v);
			if (!confModule.count("SSL"))
				return true;
			auto &confSSL = std::get<dems::config::ConfigObject>(confModule["SSL"].v);
			if (!confSSL.count("certificate") || !confSSL.count("certificate-key"))
				return true;
			_certificate = std::get<std::string>(confSSL["certificate"].v);
			_certificate_key = std::get<std::string>(confSSL["certificate-key"].v);
		} catch (std::exception &) {
			return true;
		}
		return false;
	}

	bool SSLConnection::checkIsSSL()
	{
		// check if the first character is uppercase letter
		char c;
		recv(_context.socketFd, &c, 1, MSG_PEEK);
		bool is_ssl = !std::isupper(c);
		return is_ssl;
	}

	bool SSLConnection::initSSL_CTX()
	{
		_ssl_ctx = SSL_CTX_new(TLSv1_2_server_method());
		if (!_ssl_ctx) {
			std::cerr << "Error SSL: SSL_CTX_new" << std::endl;
			return true;
		}

		int err = SSL_CTX_set_cipher_list(_ssl_ctx, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
		if(err != 1) {
			std::cerr << "Error SSL CTX: cannot set the cipher list" << std::endl;
			return true;
		}

		// set un callback bidon
		SSL_CTX_set_verify(_ssl_ctx, SSL_VERIFY_PEER, my_ssl_verify_peer);

		/* enable srtp */
		err = SSL_CTX_set_tlsext_use_srtp(_ssl_ctx, "SRTP_AES128_CM_SHA1_80");
		if(err != 0) {
			std::cerr << "Error SSL CTX: cannot setup srtp" << std::endl;
			return true;
		}
		err = SSL_CTX_use_certificate_file(_ssl_ctx, _certificate.c_str(), SSL_FILETYPE_PEM);
		if(err != 1) {
			std::cerr << "Error SSL CTX: cannot load certificate file: " << _certificate.c_str() << std::endl;
			return true;
		}
		err = SSL_CTX_use_PrivateKey_file(_ssl_ctx, _certificate_key.c_str(), SSL_FILETYPE_PEM);
		if(err != 1) {
			std::cerr << "Error SSL CTX: cannot load certificate-key file: " << _certificate_key.c_str() << std::endl;
			return true;
		}
		err = SSL_CTX_check_private_key(_ssl_ctx);
		if(err != 1) {
			std::cerr << "Error SSL CTX: checking the private key failed" << std::endl;
			return true;
		}
		return false;
	}

	bool SSLConnection::initSSL()
	{
		_ssl = SSL_new(_ssl_ctx);
		if (!_ssl) {
			std::cerr << "Error: SSL new" << std::endl;
			return true;
		}

		SSL_set_accept_state(_ssl);
		if (!SSL_is_server(_ssl)) {
			std::cerr << "Error: SSL is not server" << std::endl;
			return true;
		}

		if (SSL_set_fd(_ssl, _context.socketFd) <= 0) {
			std::cerr << "Error: SSL set fd" << std::endl;
			return true;
		}
		int ret = SSL_accept(_ssl);
		if (!ret) {
			std::cerr << "Error: SSL accept" << std::endl;
			return true;
		} else if (ret < 0) {
			char msg[1024];
			switch (SSL_get_error(_ssl, ret)) {
			case SSL_ERROR_WANT_WRITE:
				std::cerr << "Error in init_SSL: SSL_ERROR_WANT_WRITE" << std::endl;
				return true;
			case SSL_ERROR_WANT_READ:
				std::cerr << "Error in init_SSL: SSL_ERROR_WANT_READ" << std::endl;
				return true;
			case SSL_ERROR_ZERO_RETURN:
				std::cerr << "Error in init_SSL: SSL_ERROR_ZERO_RETURN" << std::endl;
				return true;
			case SSL_ERROR_SYSCALL:
				std::cerr << "Error in init_SSL: SSL_ERROR_SYSCALL: " << ret << "-" << errno<< std::endl;
				return true;
			case SSL_ERROR_SSL:
				ERR_error_string_n(ERR_get_error(), msg, sizeof(msg));
				std::cerr << "Error in init_SSL: SSL_ERROR_SSL: " << msg << std::endl;
				return true;
			default:
				std::cerr << "Error in init_SSL: SSL Connect fatal error: " << std::endl;
				return true;
			}
		}
		// Bloque jusqu'a ce que des données entrent
		char c;
		recv(_context.socketFd, &c, 1, MSG_PEEK);
		// Rend la socket non-bloquante
#ifdef WIN32
		unsigned long mode = 1;
			if (static_cast<boost>(ioctlsocket(_context.socketFd, FIONBIO, &mode)))
			return true;
#else
		int flags = fcntl(_context.socketFd, F_GETFL, 0);
		if (flags < 0)
			return true;
		if (static_cast<bool>(fcntl(_context.socketFd, F_SETFL, flags | O_NONBLOCK)))
			return true;
#endif
		return false;
	}

	SSLConnection::SSLConnection(dems::Context &context):
	_context(context),
	_certificate(),
	_certificate_key(),
	_ssl(nullptr),
	_ssl_ctx(nullptr)
	{
		if (_context.config.count(CONF_IS_SSL)
			&& _context.config.count(CONF_SSL_CTX)
			&& _context.config.count(CONF_SSL_PTR)) {
			_is_ssl = std::get<bool>(_context.config[CONF_IS_SSL].v);
			_ssl_ctx = reinterpret_cast<SSL_CTX*>(std::get<long long>(_context.config[CONF_SSL_CTX].v));
			_ssl = reinterpret_cast<SSL*>(std::get<long long>(_context.config[CONF_SSL_PTR].v));
		} else {
			_is_ssl = checkIsSSL();
			if (!_is_ssl || initCertificate() || initSSL_CTX() || initSSL()) {
				_is_ssl = false;
				_context.config[CONF_IS_SSL].v = false;
				_context.config[CONF_SSL_CTX].v = 0ll;
				_context.config[CONF_SSL_PTR].v = 0ll;
			}
			_context.config[CONF_IS_SSL].v = _is_ssl;
			_context.config[CONF_SSL_CTX].v = reinterpret_cast<long long>(_ssl_ctx);
			_context.config[CONF_SSL_PTR].v = reinterpret_cast<long long>(_ssl);
		}
	}

	SSLConnection::~SSLConnection()
	{}

	bool SSLConnection::isSSL() const
	{
		return _is_ssl;
	}

	bool SSLConnection::read()
	{
		int readByte;
		constexpr size_t BUFFER_SIZE = 256;
		uint8_t buffer[BUFFER_SIZE];

		// read
		while ((readByte = SSL_read(_ssl, buffer, BUFFER_SIZE)) > 0) {
			for (int i = 0; i < readByte; ++i) {
				_context.rawData.push_back(buffer[i]);
			}
		}
		if (readByte < 0) {
			int err = SSL_get_error(_ssl, readByte);
			switch (err) {
			case SSL_ERROR_WANT_WRITE:
				return false;
			case SSL_ERROR_WANT_READ:
				return false;
			case SSL_ERROR_ZERO_RETURN:
			case SSL_ERROR_SYSCALL:
			case SSL_ERROR_SSL:
			default:
				return true;
			}
		}
		return false;
	}

	bool SSLConnection::write()
	{
		auto size = static_cast<int>(_context.rawData.size() * sizeof(uint8_t));
		int err = SSL_write(_ssl, _context.rawData.data(), size);
		_context.rawData.clear();
		if (err < 0) {
			err = SSL_get_error(_ssl, err);
			switch (err) {
			case SSL_ERROR_WANT_WRITE:
				return false;
			case SSL_ERROR_WANT_READ:
				return false;
			case SSL_ERROR_ZERO_RETURN:
			case SSL_ERROR_SYSCALL:
			case SSL_ERROR_SSL:
			default:
				return true;
			}
		}
		return false;
	}

	void SSLConnection::disconnect()
	{
		if (_ssl) {
			SSL_shutdown(_ssl);
			SSL_free(_ssl);
			_ssl = nullptr;
			_context.config[CONF_SSL_PTR].v = 0ll;
		}
		if (_ssl_ctx) {
			SSL_CTX_free(_ssl_ctx);
			_ssl_ctx = nullptr;
			_context.config[CONF_SSL_CTX].v = 0ll;
		}
		if (_is_ssl) {
			_is_ssl = false;
			_context.config[CONF_IS_SSL].v = false;
		}
	}

}