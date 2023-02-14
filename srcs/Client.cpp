/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/14 10:39:19 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Client.hpp"
#include "../headers/Utils.h"
#include "sys/socket.h"
#include "../headers/Server.hpp"
#include "../headers/Command.hpp"
#include <iostream>

Client::Client(const int& client_fd, Server* server) : 
	_client_fd(client_fd),
	_nickname(""),
	_isConnected(0),
	_server(server)
{
	UserModes *userModes = new UserModes;
	Command *command = new Command;

	this->_command = command;
	this->_userModes = userModes;
}

Client::~Client(void)
{
	delete _command;
	delete _userModes;
}

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

void	Client::setNickname(const std::string& nickname) { this->_nickname = nickname; }
void	Client::setClientFd(const int& client_fd) { this->_client_fd = client_fd; }
void	Client::setUserModes(UserModes& userModes) { this->_userModes = &userModes; }
//void	Client::setServer(Server& server) { this->_server = server; }
void	Client::setCommand(Command& command) { this->_command = &command; }
void	Client::setIsConnected(bool& booleen) { this->_isConnected = booleen; }

//----------------------------------------------------------------------//
//							GETTERS										//
//----------------------------------------------------------------------//

std::string 		Client::getNickname(void) const { return this->_nickname; }
int					Client::getClientFd(void) const { return this->_client_fd; }
Command&			Client::getCommand(void) { return *this->_command; }
UserModes&			Client::getUserModes(void) { return *this->_userModes; }
Server&				Client::getServer(void) { return *this->_server; }
bool&				Client::getIsConnected(void) { return this->_isConnected; }

PrivilegesModes&	Client::getPrivilege(Channel& channel)
{
	std::vector<std::pair<Channel&, PrivilegesModes*> >::iterator it;
	
	for (it = this->_vectorChannels.begin(); it != this->_vectorChannels.end(); it++)
	{
		if (it->first.getName() == channel.getName())
			break;
	}
	return *it->second;
}