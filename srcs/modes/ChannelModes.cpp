
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
#include <sstream>
#include <string>
#include <algorithm>

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

void	ChannelModes::updateModes(std::vector<std::string> modes, Client &client)
{
	std::vector<std::string>::iterator	it;
	bool								validOption = false;
	Channel *channel = this->_channel;
	this->_AddOptions = "+";
	this->_RemoveOptions = "-";
	this->_keyOptions = "";
	std::string next = "";
	// +1 car le premier element est le nom du channel
	for (it = modes.begin() + 1; it != modes.end(); it++)
	{
		while ((*it).find("+") != std::string::npos || (*it).find("-") != std::string::npos)
		{

			if (it + 1 != modes.end())
				next = *(it + 1);
			else 
				next = "";
			int minus = 0;
			int plus = 0;
			if ((*it).find("-") != std::string::npos)
				 minus = (*it).find("-");
			if ((*it).find("+") != std::string::npos)
				 plus = (*it).find("+");
			if (minus < plus || (*it).find("+") == std::string::npos)
			{
				// remove mode
				if ((minus + 1) >= (int)(*it).size() || (*it)[minus + 1] == '-' || (*it)[minus + 1] == '+')
				{
					(*it).erase(minus, 1);
					continue ;
				}
				if ((*it)[minus + 1] == 'o' || (*it)[minus + 1] == 'v')
				{
					if (client.getPrivilege(*channel).isOp())
					{
						if (setModeByNameWithKeyAndClient((*it)[minus + 1], false, next, client))
						{
							validOption = true;
							if (this->_AddOptions.find((*it)[minus + 1]) != std::string::npos)
								this->_AddOptions.erase(this->_AddOptions.find((*it)[minus + 1]), 1);
							if (this->_RemoveOptions.find((*it)[minus + 1]) == std::string::npos)
								this->_RemoveOptions += (*it)[minus + 1];
							if ((*it)[minus + 1] == 'o' || (*it)[minus + 1] == 'v')
							{
								this->_keyOptions += " " + next;
								modes.erase(it + 1);
							}
						}
					}
					else
						client.sendResponse("482 " + client.getNickname() + " :You're not channel operator\r\n");
				}
				else
				{
					if (client.getPrivilege(*channel).isOp())
					{
						if (this->setModeByName((*it)[minus + 1], false))
						{
							validOption = true;
							if (this->_AddOptions.find((*it)[minus + 1]) != std::string::npos)
								this->_AddOptions.erase(this->_AddOptions.find((*it)[minus + 1]), 1);
							if (this->_RemoveOptions.find((*it)[minus + 1]) == std::string::npos)
								this->_RemoveOptions += (*it)[minus + 1];
						}
					}
					else
						client.sendResponse("482 " + client.getNickname() + " :You're not channel operator\r\n");
				}
				(*it).erase(minus + 1, 1);
			}
			else
			{
				// add mode
				if ((plus + 1) >= (int)(*it).size() || (*it)[plus + 1] == '-' ||(*it)[plus + 1] == '+')
				{
					(*it).erase(plus, 1);
					continue ;
				}
				if ((*it)[minus + 1] == 'o' || (*it)[minus + 1] == 'v')
				{
					if (client.getPrivilege(*channel).isOp())
					{
						if (setModeByNameWithKeyAndClient((*it)[minus + 1], true, next, client))
						{
							validOption = true;
							if (this->_RemoveOptions.find((*it)[plus + 1]) != std::string::npos)
								this->_RemoveOptions.erase(this->_RemoveOptions.find((*it)[plus + 1]), 1);
							if (this->_AddOptions.find((*it)[plus + 1]) == std::string::npos)
								this->_AddOptions += (*it)[plus + 1];
							if ((*it)[plus + 1] == 'o' || (*it)[plus + 1] == 'v')
							{
								this->_keyOptions += " " + next;
								modes.erase(it + 1);
							}
						}
					}
					else
						client.sendResponse("482 " + client.getNickname() + " :You're not channel operator\r\n");
				}
				else
				{
					if (client.getPrivilege(*channel).isOp())
					{
						if (this->setModeByNameWithKey((*it)[plus + 1], true, next))
						{
							validOption = true;
							if (this->_RemoveOptions.find((*it)[plus + 1]) != std::string::npos)
								this->_RemoveOptions.erase(this->_RemoveOptions.find((*it)[plus + 1]), 1);
							if (this->_AddOptions.find((*it)[plus + 1]) == std::string::npos)
								this->_AddOptions += (*it)[plus + 1];
						}
					}
					else 
						client.sendResponse("482 " + client.getNickname() + " :You're not channel operator\r\n");
					// remove next argument if k b l o v
					if ((*it)[plus + 1] == 'k' || (*it)[plus + 1] == 'b' || (*it)[plus + 1] == 'l' || (*it)[plus + 1] == 'o' || (*it)[plus + 1] == 'v')
					{
						this->_keyOptions += " " + next;
						modes.erase(it + 1);
					}
				}
				(*it).erase(plus + 1, 1);
			}
		}
	}
	if (validOption)
	{
		if (this->_AddOptions.size() == 1)
			this->_AddOptions = "";
		if (this->_RemoveOptions.size() == 1)
			this->_RemoveOptions = "";
		std::cout << "send response : MODE " + modes[0] + " " + this->_AddOptions + this->_RemoveOptions + this->_keyOptions << std::endl;
		client.sendResponse("MODE " + modes[0] + " " + this->_AddOptions + this->_RemoveOptions + this->_keyOptions + "\r\n");
		client.sendResponseToChannel(":" + client.getPrefixe() + " MODE " + modes[0] + " " + this->_AddOptions + this->_RemoveOptions + this->_keyOptions + "\r\n", this->_channel->getName());
	}
}

int 	ChannelModes::setModeByName(char mode, bool value)
{
	switch (mode)
	{
			return 1;
		case 'i':
			this->setInviteOnly(value);
			return 1;
		case 'm':
			this->setModerated(value);
			return 1;
		case 'n':
			this->setNoExternalMessage(value);
			return 1;
		case 't':
			this->setProtectedTopic(value);
			return 1;
		case 's':
			this->setSecret(value);
			return 1;
		default:
			break;
	}
	return 0;
}

int		ChannelModes::setModeByNameWithKey(char mode, bool value, std::string argument)
{

	switch (mode)
	{
		case 'i':
			this->setInviteOnly(value);
			return 1;
		case 'm':
			this->setModerated(value);
			return 1;
		case 'n':
			this->setNoExternalMessage(value);
			return 1;
		case 't':
			this->setProtectedTopic(value);
			return 1;
		case 's':
			this->setSecret(value);
			return 1;
		case 'k':
			this->setChannelKey(argument);
			return 1;
		case 'l':
			{
				std::stringstream ss;
				int num;
				ss << argument;
				ss >> num;
				this->setChannelLimit(num);
				return 1;
			}
		case 'b':
			this->addBannedUser(argument);
			return 1;
		default:
			break;
	}
	return 0;
}

int		ChannelModes::setModeByNameWithKeyAndClient(char mode, bool value, std::string key, Client &client)
{
	// check if client is op
	if (client.getPrivilege(*this->_channel).isOp() == false)
	{
		client.sendResponse("482 " + client.getNickname() + " " + this->_channel->getName() + " :You're not channel operator5\r\n");
		return 0;
	}
	// check if client exist
	if (client.getServer()->isClientExistByNickname(key) == false)
		return 0;
	switch (mode)
	{
		case 'o':
		// operator mode
			if (value == false)
			{
				if (client.getPrivilege(*this->_channel).isOwner() == false)
				{
					client.sendResponse("482 " + client.getNickname() + " " + this->_channel->getName() + " :You're not channel owner1\r\n");
					return 0;
				}
			}
			// si le client est operateur mais pas owner et qu'il souhaite changer les modes d'un owner alors on refuse
			else if ((client.getPrivilege(*this->_channel).isOp() == true && client.getPrivilege(*this->_channel).isOwner() == false) 
				&& client.getServer()->getClientByName(key)->getPrivilege(*this->_channel).isOwner() == true)
			{
				client.sendResponse("482 " + client.getNickname() + " " + this->_channel->getName() + " :You can't change privileges mode of owner channel\r\n");
				return 0;
			}
			client.getServer()->getClientByName(key)->getPrivilege(*this->_channel).setOp(value);
			return 1;
		case 'v':
		// voice mode
		if (value == false)
			{
				if (client.getPrivilege(*this->_channel).isOwner() == false)
				{
					client.sendResponse("482 " + client.getNickname() + " " + this->_channel->getName() + " :You're not channel owner2\r\n");
					return 0;
				}
			}
			// si le client est operateur mais pas owner et qu'il souhaite changer les modes d'un owner alors on refuse
			else if ((client.getPrivilege(*this->_channel).isOp() == true && client.getPrivilege(*this->_channel).isOwner() == false) 
				&& client.getServer()->getClientByName(key)->getPrivilege(*this->_channel).isOwner() == true)
			{
				client.sendResponse("482 " + client.getNickname() + " " + this->_channel->getName() + " :You can't change privileges mode of owner channel\r\n");
				return 0;
			}
			client.getServer()->getClientByName(key)->getPrivilege(*this->_channel).setVoice(value);
			return 1;
	}
	return 0;
}

std::string		ChannelModes::getModesString()
{
	std::string modes = "";
	std::string key = "";
	if (this->_inviteOnly)
		modes += "i";
	if (this->_moderated)
		modes += "m";
	if (this->_noExternalMessage)
		modes += "n";
	if (this->_protectedTopic)
		modes += "t";
	if (this->_secret)
		modes += "s";
	if (this->_channelKey.first)
	{
		modes += "k";
		key += " " + this->_channelKey.second;
	}
	if (this->_channelLimit.first)
	{
		modes += "l";
		std::stringstream ss;
		std::string key;
		ss << this->_channelLimit.second;
		ss >> key;
		key += " " + key;
	}
	if (this->_banned.first)
		modes += "b";
	return modes + key;
}

void		ChannelModes::addInvite(const std::string& nickname)
{
	if (std::find(this->_invited.begin(), this->_invited.end(), nickname) == this->_invited.end())
		this->_invited.push_back(nickname);
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

void	ChannelModes::setInviteOnly(bool mode)  { this->_inviteOnly = mode; }
void	ChannelModes::setModerated(bool mode) { this->_moderated = mode; }
void	ChannelModes::setSecret(bool mode) { this->_secret = mode; }
void	ChannelModes::setProtectedTopic(bool mode) { this->_protectedTopic = mode; }
void	ChannelModes::setNoExternalMessage(bool mode) { this->_noExternalMessage = mode; }
void	ChannelModes::setChannel(Channel *channel) { this->_channel = channel; }
//----------------------------------------------------------------------//
//						GET TOGGLE ATTRIBUTES							//
//----------------------------------------------------------------------//

bool	ChannelModes::isInvited(const std::string &nickname) {
	std::vector<std::string>::iterator it = this->_invited.begin();
	while (it != this->_invited.end()) {
		if (*it == nickname) {
			return true;
		}
		it++;
	}
	return false;
}
bool	ChannelModes::isInviteOnly(void) const { return this->_inviteOnly; }
bool	ChannelModes::isModerated(void) const { return this->_moderated; }
bool	ChannelModes::isSecret(void) const { return this->_secret; }
bool	ChannelModes::isProtectedTopic(void) const { return this->_protectedTopic; }
bool	ChannelModes::isNoExternalMessage(void) const { return this->_noExternalMessage; }