/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 15:31:14 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/05 23:24:36 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>


int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock_fd < 0)
		return (1);
	struct sockaddr_in sockaddr;
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(8080);
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(sock_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
		return (1);
	if (listen(sock_fd, 1) < 0)
		return (1);
	return (0);
}