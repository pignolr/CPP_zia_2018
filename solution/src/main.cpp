/*
** EPITECH PROJECT, 2021
** Zia
** File description:
** main.cpp
*/

#include <iostream>
#include "server/ShellServer.hpp"

zia::server::ShellServer init_shell(int ac, char **av)
{
	if (ac > 1)
		return zia::server::ShellServer(av[1]);
	else
		return zia::server::ShellServer();
}

int main(int ac, char **av)
{
	signal(SIGPIPE, SIG_IGN);
	if (ac > 2){
		std::cerr << "Error: Invalid number of argument" << std::endl;
		return EXIT_FAILURE;
	}
	try {
		auto shell = init_shell(ac, av);
		shell.run();
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Bye." << std::endl;
	return EXIT_SUCCESS;
}
