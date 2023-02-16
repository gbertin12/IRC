/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/15 11:49:53 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Client.hpp"
#include "../headers/Server.hpp"
#include "../headers/Utils.h"
#include <algorithm>
#include <iostream>

Server::Server(const std::string& port, const std::string& password) : _password(password) {
	
	char *end;
	long numberlong = std::strtol(port.c_str(), &end, 10);
	
	//check if port is valid
	if (numberlong > 65535 || numberlong < 0 || *end != '\0')
		throw Server::SyntaxPortException();
	this->_port = static_cast<unsigned int>(numberlong);
	// create socket
	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sockfd < 0)
		throw Server::CreateSocketServerException();
		
	// set socket options
	memset(&this->_servaddr, 0, sizeof(this->_servaddr));
	this->_servaddr.sin_family = AF_INET;
	this->_servaddr.sin_port = htons(this->_port);
	this->_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// bind socket and add fd to pollfds
	if (bind(this->_sockfd, (struct sockaddr *)&this->_servaddr, sizeof(this->_servaddr)) < 0)
		throw Server::BindAddressException();
	// add socket to pollfds
	pollfd pollfd = {this->_sockfd, POLLIN, 0};
	this->_vectorPollfds.push_back(pollfd);
	// listen socket
	if (listen(this->_sockfd, 1) < 0)
		throw Server::ListenSocketException();
	return ;
}

Server::~Server(void) { }

void	Server::debug(void) const
{
	std::cout << "map client size = " << _mapClients.size() << std::endl;
	std::cout << "vector pollfds size = " << _vectorPollfds.size() << std::endl;
	std::cout << "vector channel size = " << _vectorChannels.size() << std::endl;
}

//----------------------------------------------------------------------//
//							METHODS										//
//----------------------------------------------------------------------//

bool	Server::clientAuthentification(Client *client)
{
	std::string command = client->recvRequest();
	std::vector<std::string> commands = separateCmd(command);
	int index = 0;

	if (commands.size() < 3)
		return (false);

	//CAP LS
	client->getCommand().parsing(commands[index]);
	if (client->getCommand().getCmd() == "CAP" && client->getCommand().getArgs()[0] == "LS")
	{
		std::cout << "\033[1;46mCLIENT : " << commands[index] << "\033[m" << std::endl;
		//client->getCommand().execute();
		index++;
	}

	//PASS
	client->getCommand().parsing(commands[index]);
	std::cout << "\033[1;46mCLIENT : " << commands[index] << "\033[m" << std::endl;
	if (client->getCommand().getCmd() == "PASS")
		client->getCommand().execute();
	else
		client->getCommand().nopass();
	if (client->getIsConnected() == false)
		return (false);
	index++;

	//NICK
	client->getCommand().parsing(commands[index]);
	std::cout << "\033[1;46mCLIENT : " << commands[index] << "\033[m" << std::endl;
	if (client->getCommand().getCmd() == "NICK")
		client->getCommand().execute();
	else
		return (false);
	index++;

	//USER
	client->getCommand().parsing(commands[index]);
	std::cout << "\033[1;46mCLIENT : " << commands[index] << "\033[m" << std::endl;
	if (client->getCommand().getCmd() == "USER")
		client->getCommand().execute();
	else
		return (false);

	client->getCommand().capend();
/*
	//RECV ANSWER FROM CLIENT
	command.clear(); commands.clear();
	command = client->recvRequest();
	commands = separateCmd(command);
	index = 0;

	//CAP REQ
	client->getCommand().parsing(commands[index]);
	std::cout << "\033[1;46mCLIENT : " << commands[index] << "\033[m" << std::endl;
	if (client->getCommand().getCmd() == "CAP" && client->getCommand().getArgs()[0] == "REQ")
		client->getCommand().execute();
	else
		return (false);
	index++;

	//RECV ANSWER FROM CLIENT
	command.clear(); commands.clear();
	command = client->recvRequest();
	commands = separateCmd(command);
	index = 0;

	//CAP END
	client->getCommand().parsing(commands[index]);
	std::cout << "\033[1;46mCLIENT : " << commands[index] << "\033[m" << std::endl;
	if (client->getCommand().getCmd() == "CAP" && client->getCommand().getArgs()[0] == "END")
		client->getCommand().execute();
	else
		return (false);
*/
	return (true);
}

void	Server::freeClient(Client *client)
{
	std::cout << "Descriptor " << client->getClientFd() << " has disconnected\n" << std::endl; 
	for (std::map<int, Client*>::iterator it = _mapClients.begin(); it != _mapClients.end(); it++)
	{
		if (it->first == client->getClientFd())
		{
			_mapClients.erase(it);
			break;
		}
	}
	for (std::vector<pollfd>::iterator ite = _vectorPollfds.begin(); ite != _vectorPollfds.end(); ite++)
	{
		if (ite->fd == client->getClientFd())
		{
			_vectorPollfds.erase(ite);
			break;
		}
	}
	delete client;
}

void	Server::acceptClient()
{
	// accept client
	socklen_t serv_addr_len = sizeof(this->_servaddr);
	int client_fd = accept(this->_sockfd, (struct sockaddr *)&this->_servaddr, &serv_addr_len);
	if (client_fd < 0)
		throw Server::AcceptClientException();
	// create client
	Client *client = new Client(client_fd, this);
	client->getCommand().setClient(client);
	client->setNickname("anonymous");
	std::cout << "CLIENT FD " << client->getClientFd() << " CONNECTED" << std::endl;
	if (clientAuthentification(client) == true)
	{
		// add client to pollfds
		pollfd client_pollfd = {client_fd, POLLIN, 0};
		this->_vectorPollfds.push_back(client_pollfd);
		// add client to map
		this->_mapClients.insert(std::pair<int, Client*>(client_fd, client));
	}
	else
	{
		this->freeClient(client);
	}
	//this->debug();
}

void Server::run()
{
	int timeout = TIMEOUT;
	while (true)
	{
		try
		{
			// poll
			int poll_ret = poll(this->_vectorPollfds.data(), this->_vectorPollfds.size(), timeout);
			if (poll_ret < 0)
				throw Server::PollException();
			// check if new client || vectorPollfds[0] is the socket of server
			if (this->_vectorPollfds[0].revents & POLLIN)
			{
				this->acceptClient();
				continue;
			}
			// check if client has sent data
			for (unsigned long i = 1; i < this->_vectorPollfds.size(); i++)
			{
				if (this->_vectorPollfds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) 
				{
					std::cout << "Descriptor " << this->_vectorPollfds[i].fd << " has disconnected\n" << std::endl; 
					this->_vectorPollfds.erase(this->_vectorPollfds.begin() + i);
				}
				if (this->_vectorPollfds[i].revents & POLLIN)
				{
					Client 		*client = this->_mapClients.find(this->_vectorPollfds[i].fd)->second;
					std::string command = client->recvRequest();
					std::vector<std::string> commands = separateCmd(command);
					for (size_t i = 0; i < commands.size(); i++)
					{
						std::cout << "\033[1;46mCLIENT : " << commands[i] << "\033[m" << std::endl;
						client->getCommand().parsing(commands[i]);
						client->getCommand().execute();
					}
				}
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

void	Server::addChannel(Channel *channel) { this->_vectorChannels.push_back(channel); }
//----------------------------------------------------------------------//
//							SETTERS										//
//----------------------------------------------------------------------//

void	Server::setSockFd(const int& sockFd) { _sockfd = sockFd; }
void	Server::setPort(const int& port) { _port = port; }
void	Server::setVectorPollfds(const std::vector<pollfd>& pollfd) { _vectorPollfds = pollfd; }
void	Server::setServaddr(const sockaddr_in& servaddr) { _servaddr = servaddr; }
void	Server::setMapClients(const std::map<int, Client*>& map) { _mapClients = map; }
void	Server::setVectorChannels(std::vector<Channel*>& vectChannel) { _vectorChannels = vectChannel; }

//----------------------------------------------------------------------//
//							GETTERS										//
//----------------------------------------------------------------------//

int&						Server::getSockFd(void) { return (_sockfd); }
int&						Server::getPort(void) { return (_port); }
const std::string&			Server::getPassword(void) { return (_password); }
sockaddr_in&				Server::getServaddr(void) { return (_servaddr); }
std::map<int, Client*>&		Server::getMapClients(void) { return (_mapClients); }
std::vector<pollfd>&		Server::getVectorPollfds(void) { return (_vectorPollfds); }
std::vector<Channel*>&		Server::getVectorChannels(void) { return (_vectorChannels); }
