#ifndef COMMAND_H
# define COMMAND_H

#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include "Client.hpp"
#include "Server.hpp"
#include "Utils.h"

class Client;
class Server;

class Command {

	public:

	Command(void);
	Command(Client* client);
	~Command(void);
	Command(const Command& obj);
	Command& operator=(const Command& rhs);
	

	// setters
	void 		setClient(Client *client);

	// getters
	Client*						getClient(void) const;
	std::string 				getPrefix(void) const;
	std::string 				getCmd(void) const;
	std::vector<std::string>	getArgs(void) const;

	// methods
	void 		initialize_cmd();
	void 		parsing(std::string cmd);
	void 		print_parsing();
	void		execute();
	bool		ClientIsInChannel(Client *client, std::string channel_name);
	Channel 	*returnChannel(std::string channel, Server& serv);

	// channel commands
	void 		join(void);
	void 		mode(void);
	void		list(void);
	void		names(void);
	void		printNamesInChannel(Channel *channel, Client *client);
	void		topic(void);
	void		printTopicInChannel(Channel *channel, Client *client);
	std::string findChannelMembershipPrefix(Channel *channel, Client *client);
	
	// authentification commands
	void		cap(void);
	void		capls(void);
	void		capreq(void);
	void		capend(void);
	void		pass(void);
	void		nopass(void);

	// user commands
	void		nick(void);
	void		user(void);
	void		quit(void);
	void		privmsg(void);
	void		whois(void);

	// server commands
	void		ping(void);

	class ClientUnknownCommand : public std::exception
	{
		public : 
		const char *what() const throw()
		{
			return ("421 * :Unknown command\r\n");
		}
	};
	/*class EmptyCommand : public std::exception
	{
		public : 
		const char *what() const throw()
		{
			return ("* :The command sent by a client is empty.");
		}
	};*/
	class NoPassword : public std::exception
	{
		const char *what() const throw()
		{
			return ("464 * :A password is required\r\n");
		}
	};
	class BadPassword : public std::exception
	{
		const char *what() const throw()
		{
			return ("464 * :Bad password\r\n");
		}
	};
	class NoNicknameGiven : public std::exception
	{
		const char *what() const throw()
		{
			return ("* :No nickname given.\r\n");
		}
	};
	class NotAuthenticated : public std::exception
	{
		const char *what() const throw()
		{
			return ("* :You have not registered\r\n");
		}
	};

	private : 

	std::vector<std::string> 						_vectorCmd;
	std::map<std::string, void (Command::*)(void)> 	_mapCmd;
	std::string 									_prefix;
	std::string 									_cmd;
	std::vector<std::string>						_args;
	Client*											_client;
};

#endif