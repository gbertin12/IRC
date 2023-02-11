/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/11 12:26:59 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Client.hpp"
#include "../headers/Utils.h"
#include "sys/socket.h"
#include "../headers/Server.hpp"
#include <iostream>

Client::Client(const int& client_fd, Server& server) : 
	_client_fd(client_fd),
	_nickname(""),
	_server(server), 
	_userModes(UserModes()) {}

Client::~Client(void) { }

//----------------------------------------------------------------------//
//							METHODS										//
//----------------------------------------------------------------------//

std::string	Client::read(void)
{
	char buffer[BUFFER_SIZE];
	int ret;

	ret = recv(this->_client_fd, buffer, BUFFER_SIZE, 0);
	if (ret == -1)
		throw Client::ReadingDataException();
	else if (ret == 0)
		throw Client::ClientDisconnectedException();
	buffer[ret] = '\0';
	std::string str(buffer);
	return str;
}

//----------------------------------------------------------------------//
//							SETTERS										//
//----------------------------------------------------------------------//

void	Client::setNickname(const std::string& nickname) {
	this->_nickname = nickname;
}


//----------------------------------------------------------------------//
//							GETTERS										//
//----------------------------------------------------------------------//

std::string Client::getNickname(void) const {
	return this->_nickname;
}

int			Client::getClientFd(void) const {
	return this->_client_fd;
}

