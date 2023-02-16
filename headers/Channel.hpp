/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:53 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/15 19:17:51 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_H
# define CHANNEL_H

#include "Channel.hpp"
#include "modes/ChannelModes.hpp"
#include <vector>
#include <string>
#include <map>

class Client; 

class Channel {

	public:

	Channel(std::string name);
	~Channel(void);
	// Channel(const Channel& obj);
	// Channel& operator=(const Channel& rhs);

	// methods
	void								addUser(Client& user);
	void								removeUser(Client& user);

	void								giveChannelCreator(Client& user); // O
	void								giveChannelOperator(Client& user); // o
	void								giveVoice(Client& user); // v

	//getters
	std::map<int, Client*>&				getMapUsers(void) const;
	std::string							getTopic(void) const;
	ChannelModes*						getModes(void) const;
	std::string							getName(void) const;
	
	//setters
	void								setTopic(const std::string& topic);
	void								setModes(ChannelModes& modes);
	void								setName(const std::string& name);
	
	private:
	std::string								_name;
	std::string								_topic;
	ChannelModes*							_modes;
	std::map<int, Client*>					_mapUsers;
};

#endif