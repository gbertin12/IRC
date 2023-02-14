/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/13 15:01:46 by gbertin          ###   ########.fr       */
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
	Command *command = new Command;
	this->_command = command;
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
	std::cout << "\033[1;31m" << message << "\033[m";
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

