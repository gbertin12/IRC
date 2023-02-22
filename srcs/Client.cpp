/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/22 09:36:16 by gbertin          ###   ########.fr       */
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
	_gaveCorrectPassword(false),
	_isAuthenticated(false),
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

Client::Client(const Client& obj) { *this = obj; }

Client& Client::operator=(const Client& rhs)
{
	this->_buffer = rhs._buffer;
	this->_client_fd = rhs._client_fd;
	this->_hostname = rhs._hostname;
	this->_nickname = rhs._nickname;
	this->_vectorChannels = rhs._vectorChannels;
	this->_gaveCorrectPassword = rhs._gaveCorrectPassword;
	this->_isAuthenticated = rhs._isAuthenticated;
	this->_server = rhs._server;
	this->_userModes = rhs._userModes;
	this->_command = rhs._command;
	return *this;
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
	this->_buffer += buffer;
	std::string str = this->_buffer;
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

void	Client::addChannel(Channel& channel)
{
	PrivilegesModes *privilegesModes = new PrivilegesModes;
	std::pair<Channel&, PrivilegesModes*> pair(channel, privilegesModes);
	this->_vectorChannels.push_back(pair);
}

void	Client::removeChannel(Channel& channel)
{
	std::vector<std::pair<Channel&, PrivilegesModes*> >::iterator it;

	for (it = this->_vectorChannels.begin(); it != this->_vectorChannels.end(); it++)
	{
		if ((*it).first.getName() == channel.getName())
		{
			this->_vectorChannels.erase(it);
			break ;
		}
	}
}

//----------------------------------------------------------------------//
//							SETTERS										//
//----------------------------------------------------------------------//

void	Client::setBuffer(const std::string& buffer) { this->_buffer = buffer; }
void	Client::setNickname(const std::string& nickname) { this->_nickname = nickname; }
void	Client::setClientFd(const int& client_fd) { this->_client_fd = client_fd; }
void	Client::setUserModes(UserModes& userModes) { this->_userModes = &userModes; }
//void	Client::setServer(Server& server) { this->_server = server; }
void	Client::setCommand(Command& command) { this->_command = &command; }
void	Client::setIsAuthenticated(bool isAuthenticated) { this->_isAuthenticated = isAuthenticated; }
void	Client::setGaveCorrectPassword(bool gaveCorrectPassword) { this->_gaveCorrectPassword = gaveCorrectPassword; }
void	Client::setHostname(const std::string& hostname) { this->_hostname = hostname; }
void	Client::setServername(const std::string& servername) { this->_servername = servername; }
void	Client::setRealname(const std::string& realname) { this->_realname = realname; }

//----------------------------------------------------------------------//
//							GETTERS										//
//----------------------------------------------------------------------//

std::string 		Client::getBuffer(void) const { return this->_buffer; }
std::string 		Client::getNickname(void) const { return this->_nickname; }
int					Client::getClientFd(void) const { return this->_client_fd; }
Command&			Client::getCommand(void) { return *this->_command; }
UserModes*			Client::getUserModes(void) { return this->_userModes; }
Server&				Client::getServer(void) { return *this->_server; }
bool&				Client::getIsAuthenticated(void) { return this->_isAuthenticated; }
bool&				Client::getGaveCorrectPassword(void) { return this->_gaveCorrectPassword; }
std::string			Client::getHostname(void) const  { return this->_hostname; }

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