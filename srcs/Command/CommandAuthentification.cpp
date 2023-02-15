#include "../../headers/Command.hpp"

void    Command::capls(void)
{
    this->getClient()->sendResponse("CAP * LS :multi-prefix sasl\r\n");
}

void    Command::capreq(void)
{
    this->getClient()->sendResponse("CAP * ACK multi-prefix\r\n");
}

void    Command::capend(void)
{
    this->getClient()->sendResponse("001 ");
    this->getClient()->sendResponse(this->getClient()->getNickname());
    this->getClient()->sendResponse(" :Welcome ");
    this->getClient()->sendResponse(this->getClient()->getNickname());
    this->getClient()->sendResponse(" :) :) :)\r\n");
}

void	Command::cap(void)
{
    if (_args[0] == "LS")
        capls();
    else if (_args[0] == "REQ")
        capreq();
    else if (_args[0] == "END")
        capend();
    else
		throw ClientUnknownCommand();
}

void    Command::pass(void)
{
    if (_args[0] == getClient()->getServer().getPassword())
    {
        getClient()->setIsConnected(true);
        return ;
    }
    else
    {
        this->getClient()->sendResponse("464");
        this->getClient()->sendResponse(" * ");
        this->getClient()->sendResponse(":Bad password\r\n");
    }
}

void Command::nopass(void)
{
    this->getClient()->sendResponse("464");
    this->getClient()->sendResponse(" * ");
    this->getClient()->sendResponse(":A password is required\r\n");
}