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

#include "headers/Server.hpp"
#include <csignal>

bool sigint = false;

void handle_sigint(int)
{
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
		Server *server = new Server(argv[1], argv[2]);
		signal(SIGINT, handle_sigint);
		server->run();
		stop(server);
	}
	return 0;
}
