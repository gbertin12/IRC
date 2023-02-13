#include "../../headers/Command.hpp"

void	Command::nick(void)
{
	std::cout << "NICK function" << std::endl;
	// check if nick is already used
	if (this->getClient()->getNickname() == this->_args[0] || )
	{

}

void Command::User(void)
{
	std::cout << "USER function" << std::endl;
}