/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandChannel.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/15 11:52:18 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"
#include <iostream>

void	Command::join(void)
{
	std::vector<Channel*> channels = this->getClient()->getServer().getVectorChannels();
	std::vector<Channel*>::iterator it;
	Client*	client = this->getClient();

	for (it = channels.begin(); it != channels.end(); it++)
	{
		if ((*it)->getName() == this->getArgs()[0])
		{
			(*it)->addUser(*client);
			client->sendResponseToChannel(client->getNickname() + " [" + client->getHostname() \
				+ "] has joined #" + this->getArgs()[0] + "\r\n", this->getArgs()[0]);
		}
	}
	// create channel
	if (it == channels.end())
	{
		Channel* channel = new Channel(this->getArgs()[0]);
		channel->addUser(*client);
		client->getServer().addChannel(channel);
	}
	client->sendResponse(client->getNickname() + " [" + client->getHostname() + "] has joined #" + this->getArgs()[0] + "\r\n");
	//LIST USERS IN CHANNEL
}
