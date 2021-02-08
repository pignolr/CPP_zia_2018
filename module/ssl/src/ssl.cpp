/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ssl.cpp
*/

#include <iostream>
#include "SSLConnection.hpp"

namespace zia::ssl_module {

	dems::CodeStatus ModuleReadSSL(dems::Context &context)
	{
		SSLConnection sslConnection(context);
		if (!sslConnection.isSSL())
			return dems::CodeStatus::HTTP_ERROR;
		sslConnection.read();
		return dems::CodeStatus::OK;
	}

	dems::CodeStatus ModuleWriteSSL(dems::Context &context)
	{
		SSLConnection sslConnection(context);
		if (!sslConnection.isSSL())
			return dems::CodeStatus::HTTP_ERROR;
		sslConnection.write();
		return dems::CodeStatus::OK;
	}

	dems::CodeStatus ModuleDisconnectSSL(dems::Context &context)
	{
		SSLConnection sslConnection(context);
		if (!sslConnection.isSSL())
			return dems::CodeStatus::HTTP_ERROR;
		sslConnection.disconnect();
		return dems::CodeStatus::OK;
	}
}

extern "C" {
	std::string registerHooks(dems::StageManager &manager)
	{
		SSL_library_init();
		SSL_load_error_strings();
		OpenSSL_add_all_algorithms();
		manager.connection().hookToEnd(0, "SSL Read", zia::ssl_module::ModuleReadSSL);
		manager.chunks().hookToFirst(0, "SSL Read", zia::ssl_module::ModuleReadSSL);
		manager.disconnect().hookToFirst(90000, "SSL Write", zia::ssl_module::ModuleWriteSSL);
		manager.disconnect().hookToMiddle(0, "SSL Disconnect", zia::ssl_module::ModuleDisconnectSSL);
		return "SSL";
	}
}
