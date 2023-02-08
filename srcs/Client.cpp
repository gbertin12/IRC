/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/08 12:38:11 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Client.hpp"
#include "../headers/Utils.h"
#include "sys/socket.h"
#include <iostream>

Client::Client(const int& client_fd) : _client_fd(client_fd) {
	return ;
}

Client::Client(void) : _client_fd(0) {
	return ;
}

Client::~Client(void) {
	return ;
}

// Client::Client(const Client& obj) {
// 	return ;
// }

// Client&	Client::operator=(const Client& obj) {
// 	return *this;
// }

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

void	Client::setStatus(int status) {
	this->_status = status;
}

//----------------------------------------------------------------------//
//							GETTERS										//
//----------------------------------------------------------------------//

std::string Client::getNickname(void) const {
	return this->_nickname;
}

int		Client::getStatus(void) const {
	return this->_status;
}

