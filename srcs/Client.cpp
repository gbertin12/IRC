/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/12 22:56:30 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Client.hpp"
#include "../headers/Utils.h"
#include "sys/socket.h"
#include "../headers/Server.hpp"
#include "../headers/Command.hpp"
#include <iostream>

Client::Client(const int& client_fd, Server& server) : 
	_client_fd(client_fd),
	_nickname(""),
	_server(server), 
	_userModes(UserModes())
{
	Command command;

	this->_command = &command;
}

Client::~Client(void) { }

//----------------------------------------------------------------------//
//							METHODS										//
//----------------------------------------------------------------------//

std::string	Client::recvRequest(void)
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

void	Client::sendResponse(const std::string& message) const
{
	send(this->_client_fd, message.c_str(), message.length(), 0);
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

Command&		Client::getCommand(void) {
	return *this->_command;
}

