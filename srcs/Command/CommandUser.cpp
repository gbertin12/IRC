/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandUser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:14:30 by gbertin           #+#    #+#             */
/*   Updated: 2023/03/11 10:42:11 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../headers/Command.hpp"

void	Command::nick(void)
{
	std::string nickname = this->_args[0];

	// if nickname is already used
	if (this->getClient()->getIsAuthenticated() == false)
	{
		while (this->isClientExistByNickname(nickname))
			nickname = nickname + "_";
	}
	else if (this->isClientExistByNickname(nickname))
	{
		std::cout << "NICKNAME ALREADY USED" << std::endl;
		this->getClient()->sendResponse("433 " + this->getClient()->getNickname() + " " + nickname + " :Nickname is already in use\r\n");
		return ;
	}
	// send response
	if (this->getClient()->getIsAuthenticated() == true)
	{
		std::cout << "SEND RESPONSE TO SERVER client is authenticated" << std::endl;
		this->getClient()->sendResponseToServer("NICK :" + nickname + "\r\n");
		this->getClient()->sendResponse("NICK :" + nickname + "\r\n");
	}
	// set nickname
	std::cout << "NICKNAME : " << nickname << std::endl;
	this->getClient()->setNickname(nickname);
}

void Command::user(void)
{
	if (this->_args.size() == 4)
	{
		if (this->getClient()->getNickname().size() > USERLEN)
			this->getClient()->setNickname(this->getClient()->getNickname().substr(0, USERLEN));
		this->getClient()->setHostname(this->_args[1]);
		this->getClient()->setServername(this->_args[2]);
		if (this->_args[3][0] == ':')
			this->_args[3] = this->_args[3].substr(1, this->_args[3].size() - 1);
		this->getClient()->setRealname(this->_args[3]);
	}
}

void Command::quit(void)
{
	if (_args.empty() == true)
	{
		this->getClient()->sendResponse("QUIT :Quit\r\n");
	}
	else
	{
		this->getClient()->sendResponse("QUIT :Quit" + _args[0] + "\r\n");
	}
	throw Server::ClientDisconnectedException();
}

Client *Command::returnClient(std::string nickname, Server *server)
{
	std::map<int, Client*>::iterator it;
	for (it = server->getMapClients().begin(); it != server->getMapClients().end(); it++)
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
		this->getClient()->sendResponse("311 " + this->getClient()->getNickname() + " " + _args[0] + " " + returnClient(_args[0],this->getClient()->getServer())->getHostname() + " " + returnClient(_args[0],this->getClient()->getServer())->getRealname() + " * :" + returnClient(_args[0],this->getClient()->getServer())->getRealname() + "\r\n");
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
	if (returnChannel(_args[0],*this->getClient()->getServer()) != NULL)
	{
		std::map<int, Client*>::iterator it;
		for (it = returnChannel(_args[0],*this->getClient()->getServer())->getMapUsers().begin(); it != returnChannel(_args[0],*this->getClient()->getServer())->getMapUsers().end(); it++)
		{
			this->getClient()->sendResponse("352 " + this->getClient()->getNickname() + " " + _args[0] + " " + it->second->getNickname() + " " + it->second->getHostname() + " " + it->second->getServer()->getName() + " " + it->second->getNickname() + " H :0 " + it->second->getRealname() + "\r\n");
		}
	}
	else if (returnClient(_args[0],this->getClient()->getServer()) != NULL)
	{
		this->getClient()->sendResponse("352 " + this->getClient()->getNickname() + " * " + returnClient(_args[0],this->getClient()->getServer())->getNickname() + " " + returnClient(_args[0],this->getClient()->getServer())->getHostname() + " " + returnClient(_args[0],this->getClient()->getServer())->getServer()->getName() + " " + returnClient(_args[0],this->getClient()->getServer())->getNickname() + " H :0 " + returnClient(_args[0],this->getClient()->getServer())->getRealname() + "\r\n");
	}
	this->getClient()->sendResponse("315 " + this->getClient()->getNickname() + " " + _args[0] + " :End of /WHO list\r\n");
}

void Command::privmsg(void)
{
	if (_args[0][0] == '#')
    {
        std::vector<Channel*> channels = this->getClient()->getServer()->getVectorChannels();
        std::vector<Channel*>::iterator it;
        //Client* client = this->getClient();
        for (it = channels.begin(); it != channels.end(); it++)
        {
			
            if ((*it)->getName() == _args[0])
            {
				//check external msg
				if ((*it)->getMapUsers().find(this->getClient()->getClientFd()) == (*it)->getMapUsers().end())
				{
					this->getClient()->sendResponse("404 " + this->getClient()->getNickname() + " " + _args[0] + " :Cannot send to channel because you are not in channel\r\n");
					return ;
				}
				//check if user is banned
				if ((*it)->getModes()->isBanned(this->getClient()->getNickname()))
				{
					this->getClient()->sendResponse("404 " + this->getClient()->getNickname() + " " + _args[0] + " :Cannot send to channel because you are banned\r\n");
					return ;
				}
				//check moderated channel
				if ((*it)->getModes()->isModerated() && (*it)->getMapUsers().find(this->getClient()->getClientFd())->second->getPrivilege(*(*it)).isVoice() == false)
				{
					this->getClient()->sendResponse("404 " + this->getClient()->getNickname() + " " + _args[0] + " :Cannot send to channel because channel is moderated and you don't have voice\r\n");
					return ;
				}
                this->getClient()->sendResponseToChannel(":" + this->getClient()->getPrefixe() + " PRIVMSG " + (*it)->getName() + " " + this->getArgs()[1] + "\r\n", _args[0]);        
                return ;
            }
        }
        this->getClient()->sendResponse("403 " + this->getClient()->getNickname() + " " + _args[0] + " :No such channel\r\n");
        return ;
    }
	else
	{
		std::map<int, Client*> mapClients = this->getClient()->getServer()->getMapClients();
		std::map<int, Client*>::iterator it;
		for (it = mapClients.begin(); it != mapClients.end(); it++)
		{
			if (it->second->getNickname() == _args[0] && _args[0] != this->getClient()->getNickname())
			{
				it->second->sendResponseWithoutPrefixe(":" + this->getClient()->getPrefixe() + " PRIVMSG " + _args[0] + " " + this->getArgs()[1] + "\r\n");
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

