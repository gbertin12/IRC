/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 10:14:28 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/13 15:21:14 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Command.hpp"

Command::Command(void) : 
	_prefix(std::string()), 
	_cmd(std::string()), 
	_args(),
	_client(NULL) { initialize_cmd(); }
	
Command::Command(Client* client) : 
	_prefix(std::string()), 
	_cmd(std::string()), 
	_args(), 
	_client(client) { 
	initialize_cmd(); 
	std::cout << "USER FD : " << this->getClient()->getClientFd() << std::endl;
}
	
Command::~Command(void) {}

/*
Command::Command(const Command& obj) {
	return ;
}

Command&	Command::operator=(const Command& obj) {
	return *this;
}
*/

//----------------------------------------------------------------------//
//							METHODS										//
//----------------------------------------------------------------------//

void Command::initialize_cmd(void)
{
	_mapCmd["JOIN"] = &Command::join;

	//user functions
	_mapCmd["NICK"] = &Command::nick;
	_mapCmd["USER"] = &Command::user;
	//_mapCmd["OPER"] = &Command::oper;
	//_mapCmd["AWAY"] = &Command::away;

	//authentication functions
	_mapCmd["CAP"] = &Command::cap;
	
	#pragma region 
	//channel and user to user functions
	// _vectorCmd.push_back("JOIN");
	// _vectorCmd.push_back("MODE");
	// _vectorCmd.push_back("PART");
	// _vectorCmd.push_back("WHOIS");
	// _vectorCmd.push_back("WHO");
	// _vectorCmd.push_back("KICK");
	// _vectorCmd.push_back("INVITE");
	// _vectorCmd.push_back("TOPIC");
	// _vectorCmd.push_back("NAMES");
	// _vectorCmd.push_back("PRIVMSG");
	// _vectorCmd.push_back("NOTICE");
	// _vectorCmd.push_back("KILL");
	// _vectorCmd.push_back("USERHOST");

	// //user functions
	// _vectorCmd.push_back("NICK");
	// _vectorCmd.push_back("USER");
	// _vectorCmd.push_back("OPER");
	// _vectorCmd.push_back("AWAY");

	// //authentication functions
	// _vectorCmd.push_back("CAP");
	// _vectorCmd.push_back("AUTHENTICATE");
	// _vectorCmd.push_back("PASS");

	// //server function
	// _vectorCmd.push_back("QUIT");
	// _vectorCmd.push_back("LIST");
	// _vectorCmd.push_back("RESTART");
	// _vectorCmd.push_back("SQUIT");

	// //other
	// _vectorCmd.push_back("PING");
	// _vectorCmd.push_back("PONG");
	#pragma endregion

}

void Command::parsing(std::string cmd)
{
	std::vector<std::string> tab = ft_split_string(cmd);
	std::vector<std::string>::iterator it = tab.begin();
	//PREFIX
	if ((*it)[0] == ':')
	{
		_prefix = (*it).substr(1, (*it).size());
		it++;
	}
	//COMMAND
	this->_cmd = _mapCmd.find(*it)->first;
	if (_cmd.empty() == true)
		throw EmptyCommand();
	else
		it++;
	//PARAMS
	while (it != tab.end())
	{
		if ((*it)[0] != ':')
		{
			_args.push_back((*it));
			it++;
		}
		else
		{
			std::string concat = concat_vect_string(tab, it, tab.end());
			concat = concat.substr(1, concat.size());
			_args.push_back(concat);
			break;
		}
	}
}

void	Command::execute() 
{
	std::cout << "IN EXECUTE" << std::endl;
	std::map<std::string, void (Command::*)(void)>::iterator it = this->_mapCmd.find(_cmd);
	if (it != _mapCmd.end())
		(this->*it->second)();
	else
		throw ClientUnknownCommand();
}

//----------------------------------------------------------------------//
//							COMMANDS									//
//----------------------------------------------------------------------//

void	Command::join(void)
{
	std::cout << "JOIN function" << std::endl;
}

void	Command::nick(void)
{
	std::cout << "NICK function" << std::endl;
}

void	Command::user(void)
{
	std::cout << "USER function" << std::endl;
}

void	Command::cap(void)
{
	std::cout << "CAP function" << std::endl;
	std::cout << this->getClient()->getClientFd() << std::endl;
	this->getClient()->sendResponse("CAP * LS :multi-prefix sasl\r\n");
}

//----------------------------------------------------------------------//
//							SETTERS										//
//----------------------------------------------------------------------//

void	Command::setClient(Client* client)
{
	this->_client = client;
}

//----------------------------------------------------------------------//
//							GETTERS										//
//----------------------------------------------------------------------//

Client*	Command::getClient(void) const
{
	return this->_client;
}

std::string	Command::getPrefix(void) const
{
	return this->_prefix;
}

void Command::print_parsing(void)
{
	std::cout << "---------------------" << std::endl;
	std::cout << "prefix = " << _prefix << std::endl;
	std::cout << "commande = " << _cmd << std::endl;
	std::cout << "NICKNAME = " << this->_client->getNickname() << std::endl;
	for(int i = 0; i < (int)_args.size(); i++)
	{
		std::cout << "argument " << i << " = " << _args[i] << std::endl;
	}
}
