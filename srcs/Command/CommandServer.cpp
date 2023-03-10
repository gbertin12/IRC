#include "../../headers/Command.hpp"
#include <iostream>

void    Command::ping(void)
{
    this->getClient()->sendResponse("PONG " + _args[0] + "\r\n");
}

void    Command::mode(void)
{
    // mode for channel
    if (_args[0][0] == '#')
    {
        std::vector<Channel*> channels = this->getClient()->getServer()->getVectorChannels();
        std::vector<Channel*>::iterator it;
        //Client* client = this->getClient();
        for (it = channels.begin(); it != channels.end(); it++)
        {
            if ((*it)->getName() == _args[0])
            {
                if ((*it)->getModes()->isBanned(this->getClient()->getNickname()))
				{
					this->getClient()->sendResponse("404 " + this->getClient()->getNickname() + " " + _args[0] + " :Cannot send to channel because you are banned\r\n");
					return ;
				}
                if ((*it)->getMapUsers().find(this->getClient()->getClientFd()) == (*it)->getMapUsers().end())
                {
                    this->getClient()->sendResponse("442 " + this->getClient()->getNickname() + " " + _args[0] + " :You're not on that channel\r\n");
                    return ;
                }
                ChannelModes* channelModes = (*it)->getModes();
                channelModes->updateModes(this->getArgs(), *this->getClient());          
                return ;
            }
        }
        this->getClient()->sendResponse("403 " + this->getClient()->getNickname() + " " + _args[0] + " :No such channel\r\n");
    }
    else
    {
        // check if user exist
        if (returnClient(_args[0], this->getClient()->getServer()) == NULL)
        {
            this->getClient()->sendResponse("401 " + this->getClient()->getNickname() + " " + _args[0] + " :No such nick\r\n");
            return ;
        }
        // check if user have access to change mode
        if (this->getClient()->getNickname() != _args[0] && this->getClient()->getUserModes()->getOperatorMode() == false)
        {
            this->getClient()->sendResponse("502 " + this->getClient()->getNickname() + " :Cant change mode for other users\r\n");
            return ;
        }
       // mode for user
        this->getClient()->getUserModes()->updateModes(this->getArgs(), *this->getClient());
    }
}

void Command::wallops(void)
{
    std::map<int, Client*> mapClients = this->getClient()->getServer()->getMapClients();
    std::map<int, Client*>::iterator it;
    
    //std::cout << " names = " << this->getClient()->getNickname() + " " << this->getClient()->getUserModes()->getOperatorMode() << std::endl;
    if (this->getClient()->getUserModes()->getOperatorMode() == false)
    {
        this->getClient()->sendResponse("481 " + this->getClient()->getNickname() + " :Permission Denied- You're not an IRC operator\r\n");
        return ;
    }
    if (this->getArgs().size() == 0)
    {
        this->getClient()->sendResponse("461 " + this->getClient()->getNickname() + " WALLOPS :Not enough parameters\r\n");
        return ;
    }
    for (it = mapClients.begin(); it != mapClients.end(); it++)
    {
        if (it->second->getUserModes()->getWallopsMode() == true)
        {
            it->second->sendResponse("WALLOPS " + this->getArgs()[0] + "\r\n");
        }
    }
}

void    Command::oper(void)
{
    if (this->_args.size() != 2)
    {
        this->getClient()->sendResponse("461 " + this->getClient()->getNickname() + " OPER :Not enough parameters\r\n");
        return ;
    }
    if (this->_args[0] != this->getClient()->getServer()->getNameAdmin() || this->_args[1] != this->getClient()->getServer()->getPwdAdmin())
    {
        this->getClient()->sendResponse("464 " + this->getClient()->getNickname() + " :Password incorrect\r\n");
        return ;
    }
    this->getClient()->getUserModes()->setOperatorMode(true);
    //std::cout << " names = " << this->getClient()->getNickname() + " " << this->getClient()->getUserModes()->getOperatorMode() << std::endl;
    this->getClient()->sendResponse("381 " + this->getClient()->getNickname() + " :You are now an IRC operator\r\n");

}