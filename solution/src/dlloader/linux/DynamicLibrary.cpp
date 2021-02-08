/*
** EPITECH PROJECT, 2021
** zia
** File description:
** DynamicLibrary.cpp
*/

/* Code Linux */
#ifdef __linux__

#include <iostream>
#include "dlloader/linux/DynamicLibrary.hpp"

namespace zia::dlloader::unix_compatibility {
	DynamicLibrary::DynamicLibrary(const std::string &&library_name)
	:_handle(nullptr)
	{
		_handle = dlopen(library_name.c_str(), RTLD_LAZY);
		if (_handle == nullptr) {
			throw DynamicLibraryError(dlerror());
		}
	}

	DynamicLibrary::~DynamicLibrary()
	{
		if (dlclose(_handle)) {
			std::cerr << dlerror() << std::endl;
		}
	}

	void *DynamicLibrary::loadSymbol(const std::string &&symbolName)
	{
		void *symbol;

		symbol = dlsym(_handle, symbolName.c_str());
		if (symbol == nullptr) {
			throw DynamicLibraryError(dlerror());
		}
		return symbol;
	}
}

#endif // __linux__
