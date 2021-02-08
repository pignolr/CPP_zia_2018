/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModuleWriter.cpp
*/

#ifdef __linux__
	#include <unistd.h>
#endif // __linux__
#include "default_module/ModuleWriter.hpp"

namespace zia::default_module {
	dems::CodeStatus ModuleWriter(dems::Context &context)
	{
#ifdef __linux__
		write(context.socketFd, context.rawData.data(), context.rawData.size());
#else
		send(context.socketFd, context.rawData.data(), context.rawData.size(), 0);
#endif //__linux
		return dems::CodeStatus::OK;
	}
}
