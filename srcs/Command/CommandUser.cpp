/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandUser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:14:30 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/14 12:10:40 by gbertin          ###   ########.fr       */
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
	this->getClient()->sendResponseToServer(this->getClient()->getNickname() + "is known as " + this->_args[0] + "\r\n");
	this->getClient()->sendResponse("You're now known as " + this->_args[0] + "\r\n");
	// set nickname
	this->getClient()->setNickname(this->_args[0]);
}

void Command::user(void)
{
	std::cout << "USER function" << std::endl;
}

void Command::quit(void)
{
	std::cout << "QUIT function" << std::endl;
}

void Command::privmsg(void)
{
	std::cout << "PRIVMSG function" << std::endl;
}

void Command::whois(void)
{
	std::cout << "WHOIS function" << std::endl;
}