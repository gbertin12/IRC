/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandChannel.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/15 11:52:18 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"
#include <iostream>

void	Command::join(void)
{
	std::vector<Channel*> channels = this->getClient()->getServer().getVectorChannels();
	std::vector<Channel*>::iterator it;
	Client*	client = this->getClient();

	for (it = channels.begin(); it != channels.end(); it++)
	{
		if ((*it)->getName() == this->getArgs()[0])
		{
			(*it)->addUser(*client);
			client->sendResponseToChannel(client->getNickname() + " [" + client->getHostname() \
				+ "] has joined #" + this->getArgs()[0] + "\r\n", this->getArgs()[0]);
		}
	}
	// create channel
	if (it == channels.end())
	{
		Channel* channel = new Channel(this->getArgs()[0]);
		channel->addUser(*client);
		client->getServer().addChannel(channel);
	}
	client->sendResponse(client->getNickname() + " [" + client->getHostname() + "] has joined #" + this->getArgs()[0] + "\r\n");
	//LIST USERS IN CHANNEL
}

void	Command::list(void)
{
	this->getClient()->sendResponse("321 " + this->getClient()->getNickname() + " :LIST of channel(s):\r\n");
	std::string ret = "";
	for (std::vector<Channel*>::iterator it = this->getClient()->getServer().getVectorChannels().begin(); it != this->getClient()->getServer().getVectorChannels().end(); it++)
	{
		//if ((*it)->getModes().isSecret() == false)
		//{
			if (ret != "")
				ret += "\n";
			ret += (*it)->getName();
		//}
	}
	this->getClient()->sendResponse("322 " + this->getClient()->getNickname() + " :" + ret + "\r\n");
	this->getClient()->sendResponse("323 " + this->getClient()->getNickname() + " :End of LIST\r\n");
}

void	Command::names(void)
{
/*
	std::vector<std::string> tab = ft_split_string(_args[0], ",");

	for (std::vector<std::string>::iterator it = tab.begin(); it != tab.end(); it++)
	{
		//on parcourt chaque channel dont on doit lister les noms
	}
*/
	this->getClient()->sendResponse("366 " + this->getClient()->getNickname() + " :End of NAMES list.\r\n");
}