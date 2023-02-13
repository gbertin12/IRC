#include "../headers/Command.hpp"

Command::Command(void) : _prefix(std::string()), _cmd(std::string()), _args()
{
	initialize_cmd();
	return ;
}

Command::~Command(void)
{
	return ;
}

/*
Command::Command(const Command& obj) {
	return ;
}

Command&	Command::operator=(const Command& obj) {
	return *this;
}
*/

void Command::initialize_cmd(void)
{
	//channel and user to user functions
	_vectorCmd.push_back("JOIN");
	_vectorCmd.push_back("MODE");
	_vectorCmd.push_back("PART");
	_vectorCmd.push_back("WHOIS");
	_vectorCmd.push_back("WHO");
	_vectorCmd.push_back("KICK");
	_vectorCmd.push_back("INVITE");
	_vectorCmd.push_back("TOPIC");
	_vectorCmd.push_back("NAMES");
	_vectorCmd.push_back("PRIVMSG");
	_vectorCmd.push_back("NOTICE");
	_vectorCmd.push_back("KILL");
	_vectorCmd.push_back("USERHOST");

	//user functions
	_vectorCmd.push_back("NICK");
	_vectorCmd.push_back("USER");
	_vectorCmd.push_back("OPER");
	_vectorCmd.push_back("AWAY");

	//authentication functions
	_vectorCmd.push_back("CAP");
	_vectorCmd.push_back("AUTHENTICATE");
	_vectorCmd.push_back("PASS");

	//server function
	_vectorCmd.push_back("QUIT");
	_vectorCmd.push_back("LIST");
	_vectorCmd.push_back("RESTART");
	_vectorCmd.push_back("SQUIT");

	//other
	_vectorCmd.push_back("PING");
	_vectorCmd.push_back("PONG");

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
	for (int i = 0; i < (int)_vectorCmd.size(); i++)
	{
		if (*it == _vectorCmd[i])
		{
			_cmd = _vectorCmd[i];
			it++;
			break;
		}
	}
	if (_cmd.empty() == true)
		throw ClientUnknownCommand();

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

void Command::print_parsing(void)
{
	std::cout << "prefix = " << _prefix << std::endl;
	std::cout << "commande = " << _cmd << std::endl;
	for(int i = 0; i < (int)_args.size(); i++)
	{
		std::cout << "argument " << i << " = " << _args[i] << std::endl;
	}
}