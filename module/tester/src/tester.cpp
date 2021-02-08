/*
** EPITECH PROJECT, 2021
** zia
** File description:
** tester.cpp
*/

#include <iostream>
#include "tester.hpp"

extern "C" {
	void registerHooks(dems::StageManager &manager)
	{
		(void)manager;
		std::cout << "module tester is loaded" << std::endl;
	}
}
