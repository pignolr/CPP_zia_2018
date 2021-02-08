/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ConfigManager.hpp
*/

#pragma once

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "api/Config.hpp"

namespace zia::server {
	class ConfigManager {

		dems::config::Config _config;

		dems::config::ConfigValue deduce_value(const std::string &);

		void provideConfigObject(const boost::property_tree::ptree &node,
			dems::config::ConfigObject &config);
		void provideConfigArray(const boost::property_tree::ptree &node,
			dems::config::ConfigArray &config);

	public:
		ConfigManager(const std::string &configFile);
		ConfigManager();

		dems::config::Config &&getConfig();
		void reloadConfig(const std::string &configFile);
	};
}
