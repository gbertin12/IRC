#ifndef COMMAND_H
# define COMMAND_H

#include <string>
#include <vector>
#include <exception>
#include <iostream>

std::vector<std::string> ft_split_string(std::string str);
std::string concat_vect_string(std::vector<std::string> tab, std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

class Command {

	public:

	Command(void);
	~Command(void);
	//Command(const Command& obj);
	//Command& operator=(const Command& rhs);
	
	std::vector<std::string> _vectorCmd;
	void initialize_cmd();
	void parsing(std::string cmd);
	void print_parsing();

	class ClientUnknownCommand : public std::exception
	{
		const char *what() const throw()
		{
			return ("The command sent by a client is not known to the server.");
		}
	};

	private:
	std::string _prefix;
	std::string _cmd;
	std::vector<std::string> _args; 

};

#endif