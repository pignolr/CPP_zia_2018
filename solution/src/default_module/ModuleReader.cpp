/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModuleReader.cpp
*/

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#ifdef __linux__
	#include <unistd.h>
#endif
#include "default_module/ModuleReader.hpp"

namespace zia::default_module {
	bool is_readable(dems::Context &context)
	{
		char c;

		// check if the first character is uppercase letter
		recv(context.socketFd, &c, 1, MSG_PEEK);
		return std::isupper(c);
	}

	dems::CodeStatus ModuleReader(dems::Context &context)
	{
		constexpr size_t BUFFER_SIZE = 256;
		unsigned char buffer[BUFFER_SIZE];
		ssize_t readByte;
		int flags;

		if (!context.rawData.empty() || !is_readable(context))
			return dems::CodeStatus::OK;
#ifdef __linux__
		// récupère les flags du fd
		flags = fcntl(context.socketFd, F_GETFL, 0);
		// rend le read non-bloquant
		fcntl(context.socketFd, F_SETFL, flags | O_NONBLOCK);
		// lis la socket
		while ((readByte = read(context.socketFd, buffer, BUFFER_SIZE))) {
			if (readByte == -1 && errno == EAGAIN) {
				break;
			} else if (readByte == -1) {
				fcntl(context.socketFd, F_SETFL, flags);
				return dems::CodeStatus::HTTP_ERROR;
			}
			for (int i = 0; i < readByte; ++i) {
				context.rawData.push_back(buffer[i]);
			}
		}
		// remet les anciens flags
		fcntl(context.socketFd, F_SETFL, flags);
#else
		// active le socket en non-bloquant
		flags = 1;
		ioctlsocket(socket, FIONBIO, &flags);
		readByte = recv(context.socketFd, buffer, BUFFER_SIZE, );
		while ((readByte = read(context.socketFd, buffer, BUFFER_SIZE))) {
			if (readByte < 0 && WSAGetLastError() != WSAEWOULDBLOCK)
				break;
			else if (readByte < 0) {
				flags = 0;
				ioctlsocket(socket, FIONBIO, &flags);
				return dems::CodeStatus::HTTP_ERROR;
			}
			for (int i = 0; i < readByte; ++i)
				context.rawData.push_back(buffer[i]);
		}
		// remet en bloquant
		flags = 0;
		ioctlsocket(socket, FIONBIO, &flags);
#endif
		return dems::CodeStatus::OK;
	}
}
