/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:22:17 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/08 12:30:26 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Client.hpp"
#include "headers/Server.hpp"

int main(int argc, char const *argv[])
{
	if (argc == 3)
	{
		Server server(argv[1], argv[2]);
		server.run();
	}
	else
	{
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	return 0;
}
