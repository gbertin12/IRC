/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/15 11:39:34 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include <map>
#include <iostream>

Channel::Channel(std::string name) : _name(name) { }

Channel::~Channel(void) { }

void	Channel::addUser(Client& user) {
	// check if user is already in channel
	if (this->_mapUsers.find(user.getClientFd()) != this->_mapUsers.end())
		return ;
	this->_mapUsers.insert(std::pair<int, Client*>(user.getClientFd(), &user));
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

//----------------------------------------------------------------------//
//							SETTERS										//
//----------------------------------------------------------------------//

void	Channel::setTopic(const std::string& topic) {
	this->_topic = topic;
}

void Channel::setModes(ChannelModes& modes) {
	this->_modes = &modes;
}
void Channel::setName(const std::string& name) {
	this->_name = name;
}

//----------------------------------------------------------------------//
//							GETTERS										//
//----------------------------------------------------------------------//

std::map<int, Client*>& Channel::getMapUsers(void) const {
	return (std::map<int, Client*>&)this->_mapUsers;
}

std::string				Channel::getTopic(void) const {
	return this->_topic;
}

ChannelModes& 			Channel::getModes(void) const {
	return *this->_modes;
}

std::string				Channel::getName(void) const {
	return this->_name;
}