#ifndef COMMAND_H
# define COMMAND_H

#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include "Client.hpp"


std::vector<std::string> ft_split_string(std::string str);
std::string concat_vect_string(std::vector<std::string> tab, std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

class Client;

class Command {

	public:

	Command(void);
	Command(Client* client);
	~Command(void);
	//Command(const Command& obj);
	//Command& operator=(const Command& rhs);
	

	void 		setClient(Client *client);
	Client		getClient(void) const;

	std::string getPrefix(void) const;

	void 		initialize_cmd();
	void 		parsing(std::string cmd);
	void 		print_parsing();

	void 		join(void);

	private:

	class ClientUnknownCommand : public std::exception
	{
		const char *what() const throw()
		{
			return ("The command sent by a client is not known to the server.");
		}
	};

	private:
	std::vector<std::string> 						_vectorCmd;
	std::map<std::string, void (Command::*)(void)> 	_mapCmd;
	std::string 									_prefix;
	std::string 									_cmd;
	std::vector<std::string>						_args;
	Client*											_client;

};

#endif