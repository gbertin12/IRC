/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/11 12:27:16 by gbertin          ###   ########.fr       */
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

void	Server::acceptClient()
{
	// accept client
	socklen_t serv_addr_len = sizeof(this->_servaddr);
	int client_fd = accept(this->_sockfd, (struct sockaddr *)&this->_servaddr, &serv_addr_len);
	if (client_fd < 0)
		throw Server::AcceptClientException();
	// create client
	Client client(client_fd, *this);
	client.setNickname("Anonymous_" + std::to_string(client_fd));
	// add client to pollfds
	pollfd client_pollfd  = {client_fd, POLLIN, 0};
	this->_vectorPollfds.push_back(client_pollfd);
	// add client to map
	this->_mapClients.insert(std::pair<int, Client>(client_fd, client));
}

void Server::run()
{
	int timeout = TIMEOUT;
	while (true)
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
			if (this->_vectorPollfds[i].revents & POLLIN)
			{
				Client 		&client = this->_mapClients.find(this->_vectorPollfds[i].fd)->second;
				std::string command = client.read();
				std::cout << client.getNickname() << " : " << command << std::endl;
			}
		}
	}
}