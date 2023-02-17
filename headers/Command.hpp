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

class Command {

	public:

	Command(void);
	Command(Client* client);
	~Command(void);
	//Command(const Command& obj);
	//Command& operator=(const Command& rhs);
	

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

	// channel commands
	void 		join(void);
	void 		mode(void);
	void		list(void);
	void		names(void);
	void		topic(void);
	
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

	// server commands
	void		ping(void);

	private:

	class ClientUnknownCommand : public std::exception
	{
		const char *what() const throw()
		{
			return ("The command sent by a client is not known by the server.");
		}
	};

	private:

	std::vector<std::string> 						_vectorCmd;
	std::map<std::string, void (Command::*)(void)> 	_mapCmd;
	std::string 									_prefix;
	std::string 									_cmd;
	std::vector<std::string>						_args;
	Client*											_client;

	class EmptyCommand : public std::exception
	{
		const char *what() const throw()
		{
			return ("The command sent by a client is empty.");
		}
	};
};

#endif