/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/14 12:03:00 by gbertin          ###   ########.fr       */
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

void	Client::sendResponseToChannel(const std::string& message, const std::string& channelName) const
{
	std::vector<Channel*> vectorChannels = this->_server->getVectorChannels();
	std::vector<Channel*>::iterator it;
	// search channel
	for (it = vectorChannels.begin(); it != vectorChannels.end(); it++)
		if ((*it)->getName() == channelName)
			break;
	if (it == vectorChannels.end())
		this->sendResponse(" 403 " + this->_nickname + " " + channelName + " :No such channel\r\n") ;
	std::map<int, Client*> mapClients = (*it)->getMapUsers();
	std::map<int, Client*>::iterator it2;
	// send message to all users in channel
	for (it2 = mapClients.begin(); it2 != mapClients.end(); it2++)
		if ((*it2).second->getNickname() != this->getNickname())
			(*it2).second->sendResponse(message);
}

void Client::sendResponseToServer(const std::string& message) const
{
	std::map<int, Client*> mapClients = this->_server->getMapClients();
	std::map<int, Client*>::iterator it;
	
	for (it = mapClients.begin(); it != mapClients.end(); it++)
		if ((*it).second->getNickname() != this->getNickname())
			(*it).second->sendResponse(message);
}

void	Client::sendResponseToUser(const std::string& message, const std::string& nickname) const
{
	std::map<int, Client*> mapClients = this->_server->getMapClients();
	std::map<int, Client*>::iterator it;
	
	for (it = mapClients.begin(); it != mapClients.end(); it++)
	{
		if ((*it).second->getNickname() == nickname)
		{
			(*it).second->sendResponse(message);
			break ;
		}
	}	
}

//----------------------------------------------------------------------//
//							SETTERS										//
//----------------------------------------------------------------------//

void	Client::setNickname(const std::string& nickname) { this->_nickname = nickname; }
void	Client::setClientFd(const int& client_fd) { this->_client_fd = client_fd; }
void	Client::setUserModes(UserModes& userModes) { this->_userModes = &userModes; }
//void	Client::setServer(Server& server) { this->_server = server; }
void	Client::setCommand(Command& command) { this->_command = &command; }
void	Client::setIsConnected(bool booleen) { this->_isConnected = booleen; }

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