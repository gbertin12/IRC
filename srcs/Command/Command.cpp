/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 10:14:28 by gbertin           #+#    #+#             */
/*   Updated: 2023/03/03 08:45:13 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

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

Command::Command(Command const & src) { *this = src; }

Command & Command::operator=(Command const & rhs)
{
	this->_prefix = rhs._prefix;
	this->_cmd = rhs._cmd;
	this->_args = rhs._args;
	this->_client = rhs._client;
	this->_vectorCmd = rhs._vectorCmd;
	initialize_cmd();
	return (*this);
}

//----------------------------------------------------------------------//
//							METHODS										//
//----------------------------------------------------------------------//

void Command::initialize_cmd(void)
{
	//channel functions
	_mapCmd["JOIN"] = &Command::join;
	_mapCmd["LIST"] = &Command::list;
	_mapCmd["NAMES"] = &Command::names;
	_mapCmd["TOPIC"] = &Command::topic;
	_mapCmd["PART"] = &Command::part;
	_mapCmd["KICK"] = &Command::kick;

	//user functions
	_mapCmd["NICK"] = &Command::nick;
	_mapCmd["USER"] = &Command::user;
	_mapCmd["QUIT"] = &Command::quit;
	_mapCmd["PRIVMSG"] = &Command::privmsg;
	_mapCmd["WHOIS"] = &Command::whois;
	_mapCmd["WHO"] = &Command::who;
	_mapCmd["WHOWAS"] = &Command::whowas;
	//_mapCmd["AWAY"] = &Command::away;

	//authentication functions
	_mapCmd["PASS"] = &Command::pass;
	_mapCmd["CAP"] = &Command::cap;
	
	//server functions
	_mapCmd["OPER"] = &Command::oper;
	_mapCmd["PING"] = &Command::ping;
	_mapCmd["MODE"] = &Command::mode;
	_mapCmd["WALLOPS"] = &Command::wallops;

}

bool	Command::clientIsInChannelByNickname(std::string nickname, Channel *channel)
{
	std::map<int, Client*>::iterator it;

	for (it = channel->getMapUsers().begin(); it != channel->getMapUsers().end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return true;
	}
	return false;
}

bool	Command::ClientIsInChannel(Client *client, std::string channel_name)
{
	std::vector<Channel*>::iterator it = client->getServer()->getVectorChannels().begin();
	
	if (channel_name.empty() == true)
		return (false);

	//this channel exists ?
	while (it != client->getServer()->getVectorChannels().end())
	{
		if ((*it)->getName() == channel_name)
			break;
	}
	if (it == client->getServer()->getVectorChannels().end())
		return (false); //the channel doesn't exist

	//The client is in the channel ?
	for (std::map<int, Client*>::iterator ite = (*it)->getMapUsers().begin(); ite != (*it)->getMapUsers().end(); ite++)
	{
		if ((*ite).first == client->getClientFd())
			return (true);
	}
	return (false);
}

Channel *Command::returnChannel(std::string channel, Server& serv)
{
	if (channel[0] != '#')
		channel = "#" + channel;
	std::vector<Channel*>::iterator it;
	for (it = serv.getVectorChannels().begin(); it != serv.getVectorChannels().end(); it++)
	{
		std::cout << "channel name : " << (*it)->getName() << " channel arg = " << channel  << std::endl;
		if (channel == (*it)->getName())
			return (*it);
	}
	std::cout << "channel not found" << std::endl;
	return (NULL);
}

void Command::parsing(std::string cmd)
{
	_prefix = "";
	_cmd = "";
	_args.clear();
	std::vector<std::string> tab = ft_split_string(cmd, " ");
	std::vector<std::string>::iterator it = tab.begin();
	//PREFIX
	if ((*it)[0] == ':')
	{
		_prefix = (*it).substr(1, (*it).size());
		it++;
	}
	//COMMAND
	std::map<std::string, void (Command::*)(void)>::iterator it2;
	it2 = this->_mapCmd.find((*it));
	if (it2 != this->_mapCmd.end())
		this->_cmd = it2->first;
	else
		return ;
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
			//concat = concat.substr(1, concat.size());
			_args.push_back(concat);
			break;
		}
	}
}

void	Command::execute()
{
	std::map<std::string, void (Command::*)(void)>::iterator it = this->_mapCmd.find(_cmd);
	//CAP LS
	if (_cmd == "CAP" && _args.empty() == false && _args[0] == "LS")
	{
		capls();
		return ;
	}
	//PASS
	if (_cmd == "PASS")
	{
		pass();
		if (this->getClient()->getGaveCorrectPassword() == false)
			throw BadPassword();
		return ;
	}
	//IF CLIENT NOT AUTHENTICATED AND NOT GAVE PASSWORD
	if (this->getClient()->getGaveCorrectPassword() == false)
		throw NoPassword();
	//NICK
	if (_cmd == "NICK")
	{
		nick();
		return ;
	}
	//USER
	if (this->getClient()->getNickname().empty() == true)
		throw NoNicknameGiven();
	else if (_cmd == "USER")
	{
		user();
		this->getClient()->setIsAuthenticated(true);
		capend();
		return ;
	}
	//IF CLIENT NOT AUTHENTICATED
	if (this->getClient()->getIsAuthenticated() == false)
		throw NotAuthenticated();
	//OTEHR COMMANDS	
	if (it != _mapCmd.end())
		(this->*it->second)();
	else //UNKNOWN COMMAND
		throw ClientUnknownCommand();
}

//----------------------------------------------------------------------//
//							COMMANDS									//
//----------------------------------------------------------------------//


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

Client*						Command::getClient(void) const { return this->_client; }
std::string					Command::getPrefix(void) const { return this->_prefix; }
std::string 				Command::getCmd(void) const { return this->_cmd; }
std::vector<std::string> 	Command::getArgs(void) const { return this->_args; }

void Command::print_parsing(void)
{
	std::cout << "---------------------" << std::endl;
	std::cout << "prefix = " << _prefix << std::endl;
	std::cout << "commande = " << _cmd << std::endl;
	for(int i = 0; i < (int)_args.size(); i++)
	{
		std::cout << "argument " << i << " = " << _args[i] << std::endl;
	}
	std::cout << "---------------------" << std::endl;
}
