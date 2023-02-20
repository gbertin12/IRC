/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandChannel.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/20 15:18:54 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"
#include <iostream>

static bool	ClientIsInChannel(Client *client, std::string channel_name)
{
	std::vector<Channel*>::iterator it = client->getServer().getVectorChannels().begin();
	
	if (channel_name.empty() == true)
		return (false);

	//this channel exists ?
	while (it != client->getServer().getVectorChannels().end())
	{
		if ((*it)->getName() == channel_name)
			break;
	}
	if (it == client->getServer().getVectorChannels().end())
		return (false); //the channel doesn't exist

	//The client is in the channel ?
	for (std::map<int, Client*>::iterator ite = (*it)->getMapUsers().begin(); ite != (*it)->getMapUsers().end(); ite++)
	{
		if ((*ite).first == client->getClientFd())
			return (true);
	}
	return (false);
}

static Channel *returnChannel(std::string channel, Server& serv)
{
	std::vector<Channel*>::iterator it;
	for (it = serv.getVectorChannels().begin(); it != serv.getVectorChannels().end(); it++)
	{
		if (channel == (*it)->getName())
			return (*it);
	}
	return (NULL);
}

void	Command::join(void)
{
	std::vector<Channel*> channels = this->getClient()->getServer().getVectorChannels();
	std::vector<Channel*>::iterator it;
	Client*	client = this->getClient();

	for (it = channels.begin(); it != channels.end(); it++)
	{
		if ((*it)->getName() == this->getArgs()[0])
		{
			// if ((*it)->getModes()->isInviteOnly() == true && (*it)->getModes()->isInvited(client->getNickname()) == false)
			// {
			// 	client->sendResponse("473 " + client->getNickname() + " " + this->getArgs()[0] + " :Cannot join channel (+i)\r\n");
			// 	return ;	
			// }
			// if channel limit is set
			if ((*it)->getModes()->getChannelLimit() <= (int)(*it)->getMapUsers().size())
			{
				client->sendResponse("471 " + client->getNickname() + " " + this->getArgs()[0] + " :Cannot join channel (+l)\r\n");
				return ;
			}
			// if channel key is set
			if ((*it)->getModes()->haveChannelKey() && (*it)->getModes()->getChannelkey() != this->getArgs()[1])
			{
				client->sendResponse("475 " + client->getNickname() + " " + this->getArgs()[0] + " :Cannot join channel (+k)\r\n");
				return ;
			}
			(*it)->addUser(*client);
			client->sendResponseToChannel(":" + client->getNickname() + " JOIN " + this->getArgs()[0] + "\r\n", this->getArgs()[0]);
		}
	}
	// create channel
	if (it == channels.end())
	{
		Channel* channel = new Channel(this->getArgs()[0]);
		channel->addUser(*client);
		client->getServer().addChannel(channel);
	}
	client->sendResponse(":" + client->getNickname() + " JOIN " + this->getArgs()[0] + "\r\n");
	//LIST USERS IN CHANNEL
}

void	Command::list(void)
{
	this->getClient()->sendResponse("321 " + this->getClient()->getNickname() + " :LIST of channel(s):\r\n");
	std::string ret = "";
	for (std::vector<Channel*>::iterator it = this->getClient()->getServer().getVectorChannels().begin(); it != this->getClient()->getServer().getVectorChannels().end(); it++)
	{
		if ((*it)->getModes()->isSecret() == false)
		{
			if (ret != "")
				ret += "\n";
			ret += (*it)->getName();
		}
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

void	Command::topic(void)
{
	//check if the client specify a channel
	if (_args.empty() == true)
	{
		this->getClient()->sendResponse("461 " + this->getClient()->getNickname() + " :Specify a channel to see the topic.\r\n");
		return ;
	}
	//is the client in the concerned channel ?
	if (ClientIsInChannel(this->getClient(), _args[0]) == false)
	{
		this->getClient()->sendResponse("442 " + this->getClient()->getNickname() + " :You're not in the channel " + _args[0] + ".\r\n");
		return ;
	}

	//IF THE CLIENT JUST WANT TO SEE THE TOPIC
	//RPL_TOPIC : send topic
	if (_args.size() == 1 && returnChannel(_args[0], this->getClient()->getServer())->getTopic() != "")
	{
		this->getClient()->sendResponse("332 " + this->getClient()->getNickname() + " " + _args[0] + " :" + returnChannel(_args[0], this->getClient()->getServer())->getTopic() + ".\r\n");
		return ;
	}

	//RPL_NOTOPIC : the topic is not set
	else if (_args.size() == 1 && returnChannel(_args[0], this->getClient()->getServer())->getTopic() == "")
	{
		this->getClient()->sendResponse("331 " + this->getClient()->getNickname() + " " + _args[0] + " :No topic is set\r\n");
		return ;
	}

	//IF THE CLIENT WANT TO MODIFY THE TOPIC
	//the client modify the topic and the new topic is sent to users in the concerned channel
	if ((returnChannel(_args[0], this->getClient()->getServer())->getModes()->isProtectedTopic() == false) || (this->getClient()->getPrivilege(*returnChannel(_args[0], this->getClient()->getServer())).isOp() == true))
	{
		returnChannel(_args[0], this->getClient()->getServer())->setTopic(_args[1]);
		//this->getClient()->sendResponseToChannel("TOPIC " + _args[0] + " " + _args[1] + "\r\n", _args[0]); 
		this->getClient()->sendResponse("TOPIC " + _args[0] + " " + _args[1] + "\r\n");
	}
	//if the client doesn't have the right to modify the topic
	else
	{
		this->getClient()->sendResponse("482 " + this->getClient()->getNickname() + " " + _args[0] + " :You're not channel operator\r\n");
	}

	

}