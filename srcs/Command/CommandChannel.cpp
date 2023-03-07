/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandChannel.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/03/06 12:10:39 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"
#include <iostream>

void	Command::join(void)
{
	std::vector<Channel*> channels = this->getClient()->getServer()->getVectorChannels();
	std::vector<Channel*>::iterator it;
	Client*	client = this->getClient();
	std::vector<std::string> args = ft_split_string(this->getArgs()[0], ",");
	
	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i][0] != '#')
			args[i] = "#" + args[i];
		for (it = channels.begin(); it != channels.end(); it++)
		{
			if ((*it)->getName() == args[i])
			{
				// if ((*it)->getModes()->isInviteOnly() == true && (*it)->getModes()->isInvited(client->getNickname()) == false)
				// {
				// 	client->sendResponse("473 " + client->getNickname() + " " + args[0] + " :Cannot join channel (+i)\r\n");
				// 	return ;	
				// }
				// if channel limit is set
				if ((*it)->getModes()->haveChannelLimit() && (*it)->getModes()->getChannelLimit() <= (int)(*it)->getMapUsers().size())
				{
					client->sendResponse("471 " + client->getNickname() + " " + args[i] + " :Cannot join channel (+l)\r\n");
					return ;
				}
				// if channel key is set
				if ((*it)->getModes()->haveChannelKey() && (*it)->getModes()->getChannelkey() != args[args.size() - 1])
				{
					client->sendResponse("475 " + client->getNickname() + " " + args[i] + " :Cannot join channel (+k)\r\n");
					return ;
				}
				
				(*it)->addUser(*client);
				client->addChannel(*(*it));
				break ;
			}
		}
		client->sendResponse(":" + client->getPrefixe() + " JOIN " + args[i] + "\r\n");
		// create channel
		if (it == channels.end())
		{
			Channel* channel = new Channel(args[i]);
			client->addChannel(*channel);
			channel->addUser(*client);
			client->getServer()->addChannel(channel);
			client->getPrivilege(*channel).setOp(true);
			client->getPrivilege(*channel).setVoice(true);
			client->getPrivilege(*channel).setOwner(true);
			//MODE CHANNEL
			client->sendResponse(":localhost MODE " + args[i] + " +nt\r\n");
			channel->getModes()->setProtectedTopic(true);
			channel->getModes()->setNoExternalMessage(true);
			
			channel->getModes()->setChannel(channel);
		}
		else
			client->sendResponse(":localhost MODE " + args[i] + " " + (*it)->getModes()->getModesString() + "\r\n");
		//TOPIC CHANNEL
		printTopicInChannel(returnChannel(args[i], *client->getServer()), client);
		//LIST USERS IN CHANNEL
		this->printNamesInChannel(returnChannel(args[i], *client->getServer()), client);
		this->getClient()->sendResponse(":localhost 366 " + this->getClient()->getNickname() + " " + args[i] + " :End of NAMES list\r\n");
		client->sendResponseToChannel(":" + client->getNickname() + " JOIN " + args[i] + "\r\n", args[i]);
	}
}

void	Command::list(void)
{
	this->getClient()->sendResponse("321 " + this->getClient()->getNickname() + " :LIST of channel(s):\r\n");
	std::string ret = "";
	for (std::vector<Channel*>::iterator it = this->getClient()->getServer()->getVectorChannels().begin(); it != this->getClient()->getServer()->getVectorChannels().end(); it++)
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

std::string Command::findChannelMembershipPrefix(Channel *channel, Client *client)
{
	std::string pre = "";
	if (client->getPrivilege(*channel).isOp() == true)
	{
		pre += "@"; //op
	}
	else if (client->getPrivilege(*channel).isVoice() == true)
	{
		pre += "+"; //voice
	}
	std::cout << "PRINT PREFIX MEMBERSHIP " << pre << std::endl;
	return (pre);
}

void	Command::printNamesInChannel(Channel *channel, Client *client)
{
	std::string pre;
	client->sendResponse("353 " +  client->getNickname() + " = " + channel->getName() + " :");
	for (std::map<int, Client*>::iterator it = channel->getMapUsers().begin(); it != channel->getMapUsers().end(); it++)
	{
		if (it != channel->getMapUsers().begin())
			client->sendResponse(" ");
		pre = findChannelMembershipPrefix(channel, (*it).second);
		client->sendResponse(pre + (*it).second->getNickname());
	}
	client->sendResponse("\r\n");
}

void	Command::printTopicInChannel(Channel *channel, Client *client) //special for join cmd
{
	if (channel->getTopic() != "")
		client->sendResponse(":localhost 332 " + client->getNickname() + " " + channel->getName() + " " + channel->getTopic() + "\r\n");
	else
		client->sendResponse(":localhost 331 " + client->getNickname() + " " + channel->getName() + " :No topic is set\r\n");
}

void	Command::names(void)
{
	if (_args.empty() == true) //print all clients
	{
		std::string ret = "Users on the server: ";
		std::map<int, Client*>::iterator it;
		for (it = getClient()->getServer()->getMapClients().begin(); it != getClient()->getServer()->getMapClients().end(); it++)
		{
			if (it != getClient()->getServer()->getMapClients().begin())
				ret += " ";
			ret += (*it).second->getNickname();
		}
		this->getClient()->sendResponse("353 " + this->getClient()->getNickname() + " : " + ret + "\r\n");
		return ;
	}
	//print all clients in the channel
	std::string str;
	size_t position;
	for (position = _args[0].find(",", 0); _args[0].empty() == false; position = _args[0].find(",", 0))
	{
		str = _args[0].substr(0, position);
		//std::cout << "args[0] = " << _args[0] << "position = " << position << ", str = " << str << std::endl;
		if (returnChannel(str, *this->getClient()->getServer()) == NULL)
		{
			this->getClient()->sendResponse("366 " + this->getClient()->getNickname() + " :NAMES : No channel called '" + str + "'\r\n");
			return ;
		}
		else
			printNamesInChannel(returnChannel(str, *this->getClient()->getServer()), this->getClient());
		if (position == std::string::npos)
			break ;
		_args[0].erase(0, position+1);
	}
	this->getClient()->sendResponse("366 " + this->getClient()->getNickname() + " :End of NAMES list\r\n");
}

void	Command::topic(void) //segfault avec getPrivileges
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
	if (_args.size() == 1 && returnChannel(_args[0], *this->getClient()->getServer())->getTopic() != "")
	{
		this->getClient()->sendResponse("332 " + this->getClient()->getNickname() + " " + _args[0] + " :" + returnChannel(_args[0], *this->getClient()->getServer())->getTopic() + ".\r\n");
		return ;
	}

	//RPL_NOTOPIC : the topic is not set
	else if (_args.size() == 1 && returnChannel(_args[0], *this->getClient()->getServer())->getTopic() == "")
	{
		this->getClient()->sendResponse("331 " + this->getClient()->getNickname() + " " + _args[0] + " :No topic is set\r\n");
		return ;
	}

	//IF THE CLIENT WANT TO MODIFY THE TOPIC
	//the client modify the topic and the new topic is sent to users in the concerned channel
	if ((returnChannel(_args[0], *this->getClient()->getServer())->getModes()->isProtectedTopic() == false) || (this->getClient()->getPrivilege(*returnChannel(_args[0], *this->getClient()->getServer())).isOp() == true))
	{
		//std::cout << returnChannel(_args[0], this->getClient()->getServer())->getName() << " has topic protected : " << returnChannel(_args[0], this->getClient()->getServer())->getModes()->isProtectedTopic() << std::endl;
		//std::cout << this->getClient()->getNickname() << " is op : " << this->getClient()->getPrivilege(*returnChannel(_args[0], this->getClient()->getServer())).isOp() << std::endl;
		returnChannel(_args[0], *this->getClient()->getServer())->setTopic(_args[1]);
		this->getClient()->sendResponseToChannel(":" + this->getClient()->getPrefixe() + " TOPIC " + _args[0] + " " + _args[1] + "\r\n", _args[0]); 
		this->getClient()->sendResponse("TOPIC " + _args[0] + " " + _args[1] + "\r\n");
	}
	//if the client doesn't have the right to modify the topic
	else
	{
		this->getClient()->sendResponse("482 " + this->getClient()->getNickname() + " " + _args[0] + " :You're not channel operator\r\n");
	}
}

void	Command::part(void)
{
	if (this->getArgs().empty() == true)
	{
		this->getClient()->sendResponse("461 " + this->getClient()->getNickname() + " :Specify a channel to leave.\r\n");
		return ;
	}
	std::vector<std::string> args = ft_split_string(this->getArgs()[0], ",");
	// parcourir les arguments
	for (size_t i = 0; i < args.size(); i++)
	{
		//rajoute #
		if (args[i][0] != '#')
			args[i] = "#" + args[i];
		// check le channel exist
		if (this->getClient()->getServer()->isChannelExist(this->_args[i]) == false)
		{
			this->getClient()->sendResponse(":localhost 403 " + this->getClient()->getNickname() + " " + this->_args[i] + " :No such channel\r\n");
			continue ;
		}
		// check le client est dans le channel
		if (ClientIsInChannel(this->getClient(), this->_args[i]) == false)
		{
			this->getClient()->sendResponse("442 " + this->getClient()->getNickname() + " " + this->_args[i] + " :You're not in the channel\r\n");
			continue ;
		}
		// send message to channel
		if (args[args.size() - 1][0] == ':')
			this->getClient()->sendResponseToChannel(":" + this->getClient()->getPrefixe() + " PART " + this->_args[i] + " " + args[args.size() - 1] + "\r\n", this->_args[i]);
		else
			this->getClient()->sendResponseToChannel(":" + this->getClient()->getPrefixe() + " PART " + this->_args[i] + "\r\n", this->_args[i]);
		this->getClient()->sendResponse(":" + this->getClient()->getPrefixe() + " PART " + this->_args[i] + "\r\n");
		this->getClient()->removeChannel(this->_args[i]);
		// si le channel est vide on le supprime
		if (returnChannel(this->_args[0], *this->_client->getServer())->getMapUsers().size() == 0)
			this->_client->getServer()->removeChannel(returnChannel(this->_args[0], *this->_client->getServer()));
	}
}

void	Command::kick(void)
{
	// check have enought parameter
	if (this->getArgs().size() == 1)
	{
		this->getClient()->sendResponse("461 " + this->getClient()->getNickname() + " :Not enought parameters\r\n");
		return ;
	}
	// check have permission 
	if (this->getClient()->getPrivilege(*returnChannel(this->getArgs()[0], *this->getClient()->getServer())).isOp() == false)
	{
		this->getClient()->sendResponse("482 " + this->getClient()->getNickname() + " " + this->_args[0] + " :You're not channel operator\r\n");
		return ;
	}
	// check channel exist
	if (this->getClient()->getServer()->isChannelExist(this->_args[0]) == false)
	{
		this->getClient()->sendResponse("403 " + this->getClient()->getNickname() + " " + this->_args[0] + " :No such channel\r\n");
		return ;
	}
	// for (size_t i = 1; i < this->getArgs().size(); i++)
	// {
		// check user
	if (clientIsInChannelByNickname(this->getArgs()[1], this->returnChannel(this->getArgs()[0], *this->getClient()->getServer())) == false)
	{
		this->getClient()->sendResponse("442 " + this->getClient()->getNickname() + " " + this->_args[1] + " :You're not in the channel\r\n");
		return ;
	}
	this->getClient()->sendResponse(":" + this->getClient()->getPrefixe() + " KICK " + this->_args[1] + "\r\n");
	// send message to channel
	if (this->getArgs()[this->getArgs().size() - 1][0] == ':')
		this->getClient()->sendResponseToChannel(":" + this->getClient()->getPrefixe() + " KICK " + this->_args[1] + " " + this->getArgs()[this->getArgs().size() - 1] + "\r\n", this->_args[1]);
	else
		this->getClient()->sendResponseToChannel(":" + this->getClient()->getPrefixe() + " KICK " + this->_args[1] + "\r\n", this->_args[1]);
}
