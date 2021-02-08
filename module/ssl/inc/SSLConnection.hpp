/*
** EPITECH PROJECT, 2021
** zia
** File description:
** SSLConnection.hpp
*/

#pragma once

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "api/Stage.hpp"

namespace zia::ssl_module {
	class SSLConnection {
		constexpr static char CONF_SSL_PTR[] = "SSL_ptr";
		constexpr static char CONF_SSL_CTX[] = "SSL_ctx";
		constexpr static char CONF_IS_SSL[] = "is_SSL";

		dems::Context &_context;
		std::string _certificate;
		std::string _certificate_key;

		SSL *_ssl;
		SSL_CTX *_ssl_ctx;

		bool _is_ssl;

		bool initCertificate();
		bool checkIsSSL();
		bool initSSL_CTX();
		bool initSSL();
	public:
		explicit SSLConnection(dems::Context &);
		~SSLConnection();

		bool read();
		bool write();

		bool isSSL() const;

		void disconnect();
	};
}
