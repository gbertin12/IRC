/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelModes.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:53 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/20 10:54:22 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELMODES_H
# define CHANNELMODES_H

#include "ChannelModes.hpp"
#include "../Client.hpp"
#include <string>
#include <vector>

class Client;

class ChannelModes {

	public:

	ChannelModes(void);
	~ChannelModes(void);
	// ChannelModes(const ChannelModes& obj);
	// ChannelModes& operator=(const ChannelModes& rhs);

	// methods
	
	void								updateModes(std::vector<std::string> modes, Client &client);
	int									setModeByName(char c, bool mode);
	int									setModeByNameWithKey(char c, bool mode, std::string key);
								

	// settable attributes
	void								setChannelKey(const std::string& key);
	void								removeChannelKey(void);
	void								setChannelLimit(const int& limit);
	void								removeChannelLimit(void);
	void								addBannedUser(const std::string& user);
	void								removeBannedUser(const std::string& user);

	// get settable attributes
	bool								haveChannelKey(void) const;
	bool								haveChannelLimit(void) const;
	bool								haveBannedUsers(void) const;
	std::string							getChannelkey(void) const;
	int									getChannelLimit(void) const;
	std::vector<std::string>			getBannedUsers(void) const;
	
	// check if user is banned
	bool								isBanned(const std::string& user);
	
	// set toggle attributes
	void								setInviteOnly(bool mode);
	void								setModerated(bool mode);
	void								setSecret(bool mode);
	void								setProtectedTopic(bool mode);
	void								setNoExternalMessage(bool mode);
	void								setChannelKeyBool(bool mode);
	void								setChannelLimitBool(bool mode);
	void								setBannedBool(bool mode);

	// get toggle attributes
	bool								isInviteOnly(void) const;
	bool								isModerated(void) const;
	bool								isSecret(void) const;
	bool								isProtectedTopic(void) const;
	bool								isNoExternalMessage(void) const;

	private:
	// option not supported : e I
	
	std::string									_AddOptions; //stock all options will be added
	std::string									_RemoveOptions; //stock all options will be removed
	std::string									_keyOptions; //stock all options with key will be added or removed
	// settable attributes
	std::pair<bool, std::string>				_channelKey; // k
	std::pair<bool, int>						_channelLimit; // l
	std::pair<bool, std::vector<std::string> >	_banned; // option : b / if true, a list of user banned is available
	
	// toggle attributes
	bool										_inviteOnly; // i
	bool										_moderated; // m, if true, only user have voice can talk
	bool										_secret; // s, if true, this channel not appear in /list or /names
	bool										_protectedTopic; // t, if true only operator could change Topic
	bool										_noExternalMessage; // n, if true, only user in channel could send message to it
	
};

#endif