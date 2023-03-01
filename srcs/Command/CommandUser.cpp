/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandUser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:14:30 by gbertin           #+#    #+#             */
/*   Updated: 2023/03/01 10:15:25 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../headers/Command.hpp"

void	Command::nick(void)
{
	int err = 0;
	//check is already used
	std::string	nickname = this->getClient()->getNickname();
	std::map<int, Client*> mapClients = this->getClient()->getServer().getMapClients();
	std::map<int, Client*>::iterator it;
	for (it = mapClients.begin(); it != mapClients.end(); it++)
	{
		if (it->second->getNickname() == this->_args[0])
		{
			err = 1;
			break ;
		}
	}
	// if nickname is already used
	if (this->getClient()->getNickname() == this->_args[0] || err)
	{
		this->getClient()->sendResponse("433 * " + this->_args[0] + " :Nickname is already in use\r\n");
		return ;
	}
	// send response
	if (this->getClient()->getIsAuthenticated() == true)
	{
		this->getClient()->sendResponseToServer(":" + this->getClient()->getPrefixe() + " NICK :" + this->_args[0] + "\r\n");
		this->getClient()->sendResponse(":" + this->getClient()->getPrefixe() + " NICK :" + this->_args[0] + "\r\n");
	}
	// set nickname
	this->getClient()->setNickname(this->_args[0]);
}

void Command::user(void)
{
	if (this->_args.size() == 4)
	{
		//this->getClient()->setNickname(this->_args[0]);
		if (this->getClient()->getNickname().size() > USERLEN)
			this->getClient()->setNickname(this->getClient()->getNickname().substr(0, USERLEN));
		this->getClient()->setHostname(this->_args[1]);
		this->getClient()->setServername(this->_args[2]);
		this->getClient()->setRealname(this->_args[3]);
		this->getClient()->sendResponse("001 " + this->getClient()->getNickname() + " :Welcome to the Internet Relay Network " + this->getClient()->getNickname() + "!\r\n");
	}
}

void Command::quit(void)
{
	std::cout << "QUIT function" << std::endl;
}

Client *Command::returnClient(std::string nickname, Server server)
{
	std::map<int, Client*> mapClients = server.getMapClients();
	std::map<int, Client*>::iterator it;
	for (it = mapClients.begin(); it != mapClients.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return (it->second);
	}
	return (NULL);
}

void Command::whois(void)
{
	if (_args.empty() == true)
	{
		this->getClient()->sendResponse("431 " + this->getClient()->getNickname() + " :No nickname given\r\n");
		return ;
	}
	if (returnClient(_args[0],this->getClient()->getServer()) == NULL)
	{
		this->getClient()->sendResponse("401 " + this->getClient()->getNickname() + " " + _args[0] + " :No such nick\r\n");
		return ;
	}
	else
	{
		this->getClient()->sendResponse("311 " + this->getClient()->getNickname() + " " + _args[0] + " " + returnClient(_args[0],this->getClient()->getServer())->getHostname() + " " + returnClient(_args[0],this->getClient()->getServer())->getServer().getName() + " * :" + returnClient(_args[0],this->getClient()->getServer())->getRealname() + "\r\n");
		this->getClient()->sendResponse("318 " + this->getClient()->getNickname() + " " + _args[0] + " :End of /WHOIS list\r\n");
	}
}

void Command::who(void)
{
	if (_args.empty() == true)
	{
		this->getClient()->sendResponse("461 " + this->getClient()->getNickname() + " :Not enough parameters\r\n");
		return ;
	}
	if (returnChannel(_args[0],this->getClient()->getServer()) != NULL)
	{
		std::map<int, Client*>::iterator it;
		for (it = returnChannel(_args[0],this->getClient()->getServer())->getMapUsers().begin(); it != returnChannel(_args[0],this->getClient()->getServer())->getMapUsers().end(); it++)
		{
			this->getClient()->sendResponse("352 " + this->getClient()->getNickname() + _args[0] + " " + it->second->getNickname() + " " + it->second->getHostname() + " " + it->second->getServer().getName() + " " + it->second->getNickname() + " H :0 " + it->second->getHostname() + "\r\n");
		}
	}
	else if (returnClient(_args[0],this->getClient()->getServer()) != NULL)
	{
		this->getClient()->sendResponse("352 " + this->getClient()->getNickname() + " * " + returnClient(_args[0],this->getClient()->getServer())->getNickname() + " " + returnClient(_args[0],this->getClient()->getServer())->getHostname() + " " + returnClient(_args[0],this->getClient()->getServer())->getServer().getName() + " " + returnClient(_args[0],this->getClient()->getServer())->getNickname() + " H :0 " + returnClient(_args[0],this->getClient()->getServer())->getRealname() + "\r\n");
	}
	this->getClient()->sendResponse("315 " + this->getClient()->getNickname() + " " + _args[0] + " :End of /WHO list\r\n");
}

void Command::privmsg(void)
{
	if (_args[0][0] == '#')
    {
        std::vector<Channel*> channels = this->getClient()->getServer().getVectorChannels();
        std::vector<Channel*>::iterator it;
        //Client* client = this->getClient();
        for (it = channels.begin(); it != channels.end(); it++)
        {
            if ((*it)->getName() == _args[0])
            {
                this->getClient()->sendResponseToChannel(":" + this->getClient()->getNickname() + " PRIVMSG " + (*it)->getName() + " " + this->getArgs()[1] + "\r\n", _args[0]);        
                return ;
            }
        }
        this->getClient()->sendResponse("403 " + this->getClient()->getNickname() + " " + _args[0] + " :No such channel\r\n");
        return ;
    }
	else
	{
		std::map<int, Client*> mapClients = this->getClient()->getServer().getMapClients();
		std::map<int, Client*>::iterator it;
		for (it = mapClients.begin(); it != mapClients.end(); it++)
		{
			if (it->second->getNickname() == _args[0])
			{
				it->second->sendResponse(":" + this->getClient()->getNickname() + " PRIVMSG " + _args[0] + " " + this->getArgs()[1] + "\r\n");
				return ;
			}
		}
		this->getClient()->sendResponse("401 " + this->getClient()->getNickname() + " " + _args[0] + " :No such nick\r\n");
	}
}

void Command::whowas(void)
{
	std::cout << "WHOWAS function" << std::endl;
}