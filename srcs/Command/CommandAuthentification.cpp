#include "../../headers/Command.hpp"

void    Command::capls(void)
{
    //this->getClient()->sendResponse("CAP * LS :multi-prefix sasl\r\n");
}

void    Command::capreq(void)
{
    //this->getClient()->sendResponse("CAP * ACK multi-prefix\r\n");
}

void    Command::capend(void)
{
    this->getClient()->sendResponse("001 " + this->getClient()->getNickname() + " :Welcome to the Internet Relay Network " + this->getClient()->getNickname() + "!\r\n");
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
    if (_args[0] == getClient()->getServer()->getPassword())
        getClient()->setGaveCorrectPassword(true);
    else
        getClient()->setGaveCorrectPassword(false);
}

/*void Command::nopass(void)
{
    getClient()->setIsConnected(false);
    this->getClient()->sendResponse("464 * :A password is required\r\n");
}*/