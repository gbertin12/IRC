/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/11 12:20:42 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include <map>
#include <iostream>

Channel::Channel(void) { }

Channel::~Channel(void) { }

void	Channel::addUser(Client& user) {
	// check if user is already in channel
	if (this->_mapUsers.find(user.getClientFd()) != this->_mapUsers.end())
		return ;
	this->_mapUsers.insert(std::pair<int, Client&>(user.getClientFd(), user));
}

void	Channel::removeUser(Client& user) {
	// check if user is in channel
	if (this->_mapUsers.find(user.getClientFd()) == this->_mapUsers.end())
		return ;
	this->_mapUsers.erase(user.getClientFd());
}

void	Channel::giveChannelCreator(Client &user) {
	(void)user;
	std::cout << "giveChannelCreator" << std::endl;
}

void	Channel::giveChannelOperator(Client &user) {
	(void)user;

	std::cout << "giveChannelOperator" << std::endl;
}

void	Channel::giveVoice(Client &user) {
	(void)user;
	std::cout << "giveVoice" << std::endl;
}