#include "../../headers/Command.hpp"
#include <iostream>

void    Command::ping(void)
{
    this->getClient()->sendResponse("PONG ");
    this->getClient()->sendResponse(_args[0]);
    this->getClient()->sendResponse("\r\n");
}

void    Command::mode(void)
{
    // mode for channel
    if (_args[0][0] == '#')
    {
        std::vector<Channel*> channels = this->getClient()->getServer().getVectorChannels();
        std::vector<Channel*>::iterator it;
        //Client* client = this->getClient();
        for (it = channels.begin(); it != channels.end(); it++)
        {
            if ((*it)->getName() == _args[0])
            {
                std::cout << "channel found : " << (*it)->getName() << std::endl;
                ChannelModes* channelModes = (*it)->getModes();
                channelModes->updateModes(this->getArgs(), *this->getClient());          
                return ;
            }
        }
        this->getClient()->sendResponse("403 " + this->getClient()->getNickname() + " " + _args[0] + " :No such channel\r\n");
    }
    // else
    // {
    //    // mode for user
    //     UserModes* userModes = this->getClient()->getUserModes();
    //     userModes->updateModes(this->getArgs());
    // }
}