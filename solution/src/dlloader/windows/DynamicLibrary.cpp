/*
** EPITECH PROJECT, 2021
** zia
** File description:
** DynamicLibrary.cpp
*/

/* Code Windows */
#ifdef _WIN32

#include <iostream>
#include "dlloader/linux/DynamicLibrary.hpp"

namespace zia::dlloader::unix_compatibility {
	DynamicLibrary::getLastErrorAsString()
	{
		DWORD errorMessageID = ::GetLastError();
		if(errorMessageID == 0)
			return std::string(); //No error message has been recorded

		LPSTR messageBuffer = nullptr;size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		std::string message(messageBuffer, size);

		//Free the buffer.
		LocalFree(messageBuffer);

		return message;
	}

	DynamicLibrary::DynamicLibrary(const std::string &&library_name)
	: _handle(nullptr)
	{
		_handle = LoadLibrary(library_name.c_str());
		if (_handle == nullptr) {
			throw DynamicLibraryError(getLastErrorAsString());
		}
	}

	DynamicLibrary::~DynamicLibrary()
	{
		if (FreeLibrary(_handle)) {
			std::cerr << getLastErrorAsString() << std::endl;
		}
	}

	void *DynamicLibrary::loadSymbol(const std::string &&symbolName)
	{
		void *symbol;

		symbol = GetProcAddress(_handle, symbolName.c_str());
		if (symbol == nullptr) {
			throw getLastErrorAsString(GetLastError());
		}
		return symbol;
	}
}

#endif // _WIN32
