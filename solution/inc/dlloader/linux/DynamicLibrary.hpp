/*
** EPITECH PROJECT, 2021
** zia
** File description:
** DynamicLibrary.hpp
*/

/* Code Linux */
#ifdef __linux__

#pragma once
#include <dlfcn.h>
#include <stdexcept>
#include "dlloader/IDynamicLibrary.hpp"

namespace zia::dlloader::unix_compatibility {
	class DynamicLibrary: public IDynamicLibrary {
		/* Exception lié à Dynamic Library */
		class DynamicLibraryError: public std::runtime_error {
		public:
			explicit DynamicLibraryError(const std::string &&message)
				: std::runtime_error("Error in DynamicLibrary: " + message)
			{}

			~DynamicLibraryError() override = default;
		};

		/* Attricbuts de la classe */
		void *_handle;

	public:
		/* Suppression des constructeur par copie */
		DynamicLibrary& operator=(const DynamicLibrary &) = delete;
		DynamicLibrary(const DynamicLibrary &) = delete;

		/* Méthodes de la classe */
		explicit DynamicLibrary(const std::string &&);
		~DynamicLibrary() override;

		void *loadSymbol(const std::string &&symbolName);
	};
}

#endif // __linux__
