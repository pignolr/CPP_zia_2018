/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModulesManager.cpp
*/

#include <functional>
#include <experimental/filesystem>
#include "dlloader/ModulesManager.hpp"
#include "dlloader/linux/DynamicLibrary.hpp"
#include "dlloader/windows/DynamicLibrary.hpp"
#include "default_module/ModuleContextConverter.hpp"
#include "default_module/ModuleReader.hpp"
#include "default_module/ModuleWriter.hpp"
#include "default_module/ModuleDefaultPage.hpp"
#include "default_module/ModuleHttpService.hpp"
#include "default_module/ModuleHttpRequestParser.hpp"
#include "default_module/ModuleHttpResponseParser.hpp"

namespace zia::dlloader {
	void ModulesManager::loadModules(const std::string &directoryPath)
	{
		namespace fs = std::experimental::filesystem;

		for (const auto & entry: fs::directory_iterator(directoryPath))
			loadOneModule(entry.path());
	}

	void ModulesManager::loadOneModule(const std::string &filePath)
	{
		if (_modules.count(filePath))
			return;
#ifdef _WIN32
		_modules[filePath] = IDynamicLibraryPtr(new windows_compatibility::DynamicLibrary
			(std::forward<const std::string &&>(filePath)));
#else
		_modules[filePath] = IDynamicLibraryPtr(new unix_compatibility::DynamicLibrary
			(std::forward<const std::string &&>(filePath)));
#endif // _WIN32
		auto registerFunc = reinterpret_cast<RegisterFunction>
			(_modules[filePath]->loadSymbol("registerHooks"));
		(*registerFunc)(std::move(getStageManager()));
	}

	void ModulesManager::unloadModule(const std::string &)
	{}

	void ModulesManager::loadBasicModules()
	{
		std::cout << "Loading of Basic Modules..." << std::endl;
		zia::default_module::registerReaderHooks(getStageManager());
		zia::default_module::registerWriterHooks(getStageManager());
		zia::default_module::registerContextConverterHooks(getStageManager());
		zia::default_module::registerDefaultPageHooks(getStageManager());
		zia::default_module::registerHttpServiceHooks(getStageManager());
		zia::default_module::registerHttpResponseHooks(getStageManager());
		zia::default_module::registerHttpRequestHooks(getStageManager());

		std::cout << "End of loading basic modules..." << std::endl;
	}

	void ModulesManager::loadOneModuleFromSettings(const dems::config::ConfigObject &moduleSettings)
	{
		if (moduleSettings.count("path")) {
			try {
				loadOneModule(std::get<std::string>(moduleSettings.at("path").v));
			}
			catch (const std::bad_variant_access&) {
				std::cerr << "Error: can not loading this module: invalid path" << std::endl;
			}
		} else
			std::cerr << "Error: can not loading this module: no path given" << std::endl;

	}

	void ModulesManager::loadModulesFromConfig(const dems::config::ConfigObject &modulesConfig)
	{
		std::cout << "Loading "<< modulesConfig.size() <<" module(s)..." << std::endl;
		for (auto &it: modulesConfig) {
			std::cout << "Loading " << it.first << "..." << std::endl;
			try {
				auto &moduleSettings = std::get<dems::config::ConfigObject>(it.second.v);
				loadOneModuleFromSettings(moduleSettings);
			}
			catch (const std::bad_variant_access&) {
				std::cerr << "Error: can not loading this module: no settings given" << std::endl;
			}
			catch (const std::exception &e){
				std::cerr << "Error: can not loading this module: " << e.what() << std::endl;
			}
		}
		std::cout << "Modules is loaded." << std::endl;
	}

	void ModulesManager::loadConfig(const dems::config::Config &config)
	{
		try {
			// get modules config
			auto modulesConfig = std::get<dems::config::ConfigObject>
				(config.at("modules").v);
			loadModulesFromConfig(modulesConfig);
		}
		catch (const std::bad_variant_access&) {
		}
	}

	void ModulesManager::reloadConfig(const dems::config::Config &config)
	{
		std::cout << "Reloading modules..." << std::endl;
		/* clear all Modules*/
		getStageManager().connection().clearHooks();
		getStageManager().request().clearHooks();
		getStageManager().chunks().clearHooks();
		getStageManager().disconnect().clearHooks();
		_modules.clear();

		/* load new config */
		loadBasicModules();
		loadConfig(config);
		std::cout << "Modules is reloaded." << std::endl;
	}

	ModulesManager::ModulesManager(const dems::config::Config &config)
	: ModulesManager()
	{
		loadConfig(config);
	}

	ModulesManager::ModulesManager()
	{
		loadBasicModules();
	}
}
