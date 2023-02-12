/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:53 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/12 10:23:20 by gbertin          ###   ########.fr       */
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

	Channel(void);
	~Channel(void);
	// Channel(const Channel& obj);
	// Channel& operator=(const Channel& rhs);

	// methods
	void								addUser(Client& user);
	void								removeUser(Client& user);

	void								giveChannelCreator(Client& user); // O
	void								giveChannelOperator(Client& user); // o
	void								giveVoice(Client& user); // v
	#pragma region 
	// void 							setBanMask(const std::string& mask); // b
	// void								removeBanMask(const std::string& mask); // b
	// void								setExceptionMask(const std::string& mask); // e
	// void								removeExceptionMask(const std::string& mask); // e

	// void								setInviteMask(const std::string& mask); // I
	// void								removeInviteMask(const std::string& mask); // I
#pragma endregion						
	private:
	std::string							_topic;
	
	ChannelModes						_modes;
	
	std::map<std::string, bool> 			_mapToggleModes; // a i m n q r s t
	std::map<int, Client&>					_mapUsers;
	// std::vector<std::string>				_banMask; // b
	// std::vector<std::string>				_exceptionMask; // e
	// std::vector<std::string>				_inviteMask; // I

};

#endif