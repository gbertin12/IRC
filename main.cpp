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

int main(int argc, char const *argv[])
{
	if (argc == 3)
	{
		Server server(argv[1], argv[2]);
		server.run();

	}
	return 0;
}
