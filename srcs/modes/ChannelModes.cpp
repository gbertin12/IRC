
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2022/10/29 09:15:59 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/modes/ChannelModes.hpp"
#include <utility>
#include <iostream>
#include <string>

ChannelModes::ChannelModes(void) :	
	_channelKey(std::pair<bool, std::string>(false, "")), 
	_channelLimit(std::pair<bool, int>(false, 0)), 
	_banned(std::pair<bool, std::vector<std::string> >(false, std::vector<std::string>())), 
	_inviteOnly(false), 
	_moderated(false), 
	_secret(false), 
	_protectedTopic(false), 
	_noExternalMessage(false)  {}
								

ChannelModes::~ChannelModes(void) {
	return ;
}

//----------------------------------------------------------------------//
//								METHODS									//
//----------------------------------------------------------------------//

void	ChannelModes::updateModes(std::vector<std::string> modes)
{
	std::vector<std::string>::iterator it;

	// +1 car le premier element est le nom du channel
	for (it = modes.begin() + 1; it != modes.end(); it++)
	{
		while ((*it).find("+") != std::string::npos || (*it).find("-") != std::string::npos)
		{
			// if mode ==k or mode ==b or mode ==l remove next element
			std::string next = *(it + 1);
			int minus = (*it).find("-");
			int plus = (*it).find("+");

			if (minus < plus || plus == (int)std::string::npos)
			{
				// remove mode
				this->setModeByName((*it)[minus + 1], false);
				(*it).erase(minus, 2);
			}
			else
			{
				// add mode
				this->setModeByNameWithKey((*it)[plus + 1], true, next);
				// remove next argument if k b l
				if ((*it)[plus + 1] == 'k' || (*it)[plus + 1] == 'b' || (*it)[plus + 1] == 'l')
					modes.erase(it + 1);
				(*it).erase(plus, 2);
			}
		}
	}
}

void	ChannelModes::setModeByName(char mode, bool value)
{
	switch (mode)
	{
		case 'i':
			this->setInviteOnly(value);
			break;
		case 'm':
			this->setModerated(value);
			break;
		case 'n':
			this->setNoExternalMessage(value);
			break;
		case 't':
			this->setProtectedTopic(value);
			break;
		case 's':
			this->setSecret(value);
			break;
		default:
			break;
	}
}

void	ChannelModes::setModeByNameWithKey(char mode, bool value, std::string argument)
{
	switch (mode)
	{
		case 'i':
			this->setInviteOnly(value);
			break;
		case 'm':
			this->setModerated(value);
			break;
		case 'n':
			this->setNoExternalMessage(value);
			break;
		case 't':
			this->setProtectedTopic(value);
			break;
		case 's':
			this->setSecret(value);
			break;
		case 'k':
			this->setChannelKey(argument);
			break;
		case 'l':
			this->setChannelLimit(std::stoi(argument));
			break;
		case 'b':
			this->addBannedUser(argument);
			break;
		default:
			break;
	}
}

//----------------------------------------------------------------------//
//					SET  SETTABLE ATTRIBUTES							//
//----------------------------------------------------------------------//

void	ChannelModes::setChannelKey(const std::string& key) {
	this->_channelKey.first = true;
	this->_channelKey.second = key;
}

void	ChannelModes::setChannelLimit(const int& limit) {
	this->_channelLimit.first = true;
	this->_channelLimit.second = limit;
}

void	ChannelModes::addBannedUser(const std::string& user) {
	this->_banned.first = true;
	this->_banned.second.push_back(user);
}

//----------------------------------------------------------------------//
//					REMOVE SETTABLE ATTRIBUTES							//
//----------------------------------------------------------------------//

void	ChannelModes::removeChannelKey(void) {
	this->_channelKey.first = false;
	this->_channelKey.second = "";
}

void	ChannelModes::removeChannelLimit(void) {
	this->_channelLimit.first = false;
	this->_channelLimit.second = 0;
}

void	ChannelModes::removeBannedUser(const std::string& user) {
	this->_banned.first = false;
	std::vector<std::string>::iterator it = this->_banned.second.begin();
	while (it != this->_banned.second.end()) {
		if (*it == user) {
			this->_banned.second.erase(it);
			break ;
		}
		it++;
	}
}

//----------------------------------------------------------------------//
//						GET	SETTABLE ATTRIBUTES							//
//----------------------------------------------------------------------//

bool						ChannelModes::haveChannelKey(void) const { return this->_channelKey.first; }
bool 						ChannelModes::haveChannelLimit(void) const { return this->_channelLimit.first; }
bool						ChannelModes::haveBannedUsers(void) const {	return this->_banned.first; }

std::string					ChannelModes::getChannelkey(void) const { return this->_channelKey.second; }
int							ChannelModes::getChannelLimit(void) const { return this->_channelLimit.second; }
std::vector<std::string>	ChannelModes::getBannedUsers(void) const { return this->_banned.second; }

//----------------------------------------------------------------------//
//						CHECK IF USER BANNED							//
//----------------------------------------------------------------------//

bool	ChannelModes::isBanned(const std::string& user) {
	std::vector<std::string>::iterator it = this->_banned.second.begin();
	while (it != this->_banned.second.end()) {
		if (*it == user) {
			return true;
		}
		it++;
	}
	return false;
}

//----------------------------------------------------------------------//
//						SET TOGGLE ATTRIBUTES							//
//----------------------------------------------------------------------//

void	ChannelModes::setInviteOnly(bool mode) { this->_inviteOnly = mode; }
void	ChannelModes::setModerated(bool mode) { this->_moderated = mode; }
void	ChannelModes::setSecret(bool mode) { this->_secret = mode; }
void	ChannelModes::setProtectedTopic(bool mode) { this->_protectedTopic = mode; }
void	ChannelModes::setNoExternalMessage(bool mode) { this->_noExternalMessage = mode; }

//----------------------------------------------------------------------//
//						GET TOGGLE ATTRIBUTES							//
//----------------------------------------------------------------------//

bool	ChannelModes::isInviteOnly(void) const { return this->_inviteOnly; }
bool	ChannelModes::isModerated(void) const { return this->_moderated; }
bool	ChannelModes::isSecret(void) const { return this->_secret; }
bool	ChannelModes::isProtectedTopic(void) const { return this->_protectedTopic; }
bool	ChannelModes::isNoExternalMessage(void) const { return this->_noExternalMessage; }