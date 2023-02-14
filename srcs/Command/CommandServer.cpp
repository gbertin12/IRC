#include "../../headers/Command.hpp"
#include <iostream>

void    Command::ping(void)
{
    this->getClient()->sendResponse("PONG ");
    this->getClient()->sendResponse(_args[0]);
    this->getClient()->sendResponse("\r\n");
}