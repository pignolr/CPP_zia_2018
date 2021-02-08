/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModulesManager.hpp
*/

#pragma once

#include <unordered_map>
#include "api/AModulesManager.hpp"
#include "dlloader/IDynamicLibrary.hpp"
#include "server/ConfigManager.hpp"

typedef std::string (*RegisterFunction)(dems::StageManager &&);

namespace zia::dlloader {

	class ModulesManager: public dems::AModulesManager {
		using IDynamicLibraryPtr = std::unique_ptr<IDynamicLibrary>;
		std::unordered_map<std::string, IDynamicLibraryPtr> _modules;

		void loadBasicModules();
		void loadModulesFromConfig(const dems::config::ConfigObject &modulesConfig);
		void loadOneModuleFromSettings(const dems::config::ConfigObject &moduleSettings);

		void loadConfig(const dems::config::Config &config);
	public:
		/* Suppression des constructeur par copie */
		ModulesManager& operator=(const ModulesManager &) = delete;
		ModulesManager(const ModulesManager &) = delete;

		/* Destructor */
		~ModulesManager() override = default;

		/* Constructor */
		ModulesManager(const dems::config::Config &);
		ModulesManager();

		void loadModules(const std::string &directoryPath) override;
		void loadOneModule(const std::string &filePath) override;

		void unloadModule(const std::string &moduleName) override;

		void reloadConfig(const dems::config::Config &config);
	};
}
