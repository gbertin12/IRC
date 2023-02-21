/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserModes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:53 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/20 16:23:43 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERMODES_H
# define USERMODES_H

#include "UserModes.hpp"
#include "../Client.hpp"
#include <vector>

class Client;

class UserModes {

	public:

	UserModes(void);
	~UserModes(void);;

	// methods
	void	updateModes(std::vector<std::string> modes, Client& client);
	int		setModeByName(char mode, bool value);
	
	bool	getInvisibleMode() const;
	bool	getOperatorMode() const;
	bool	getWallopsMode() const;

	void	setInvisibleMode(bool mode);
	void	setOperatorMode(bool mode);
	void	setWallopsMode(bool mode);
	
	private:
	// option not supported : O, r
	
	std::string _addOptions;
	std::string _removeOptions;
	
	bool	_invisible; // option : o / if true, this user not visible for WHO or NAMES command 
	bool	_operator; // option : r / if true, this user is a server operator
	bool	_wallops; // option : w / if true, this user receive wallops messages
};

#endif