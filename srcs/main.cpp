/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:22:17 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/13 13:55:35 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"
#include <csignal>

bool sigint = false;

void handle_sigint(int)
{
	std::cout << "SIGINT detected" << std::endl;
	sigint = true;
}

void stop(Server *server)
{
	delete server;
}

int main(int argc, char const *argv[])
{
	if (argc == 3)
	{
		try
		{
			Server *server = new Server(argv[1], argv[2]);
			signal(SIGINT, handle_sigint);
			server->run();
			stop(server);
			return 0;
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			return 1;
		}
	}
	else
	{
		std::cerr << "Usage: ./server <port> <password>" << std::endl;
		return 1;
	}
}
