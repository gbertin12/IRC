/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:53 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/11 12:21:53 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

#include "Channel.hpp"
#include "modes/UserModes.hpp"
#include "modes/PrivilegesModes.hpp"
#include <iostream>
#include <vector>

class Server;

class Client {

	public:
	//Client(void);
	Client(const int& client_fd, Server& server);
	~Client(void);
	// Client(const Client& obj);
	// Client& operator=(const Client& rhs);

	// methods
	std::string	read(void);
	void		write(const std::string& message);
	
	void		addChannel(Channel& channel);
	void		removeChannel(Channel& channel);

	// setters
	void				setNickname(const std::string& nickname);

	// getters
	int					getClientFd(void) const;
	std::string			getNickname(void) const;
	PrivilegesModes&	getPrivilege(Channel& channel);
	

	
	
	private:
	int													_client_fd;
	std::string											_nickname;
	
	Server&												_server;
	UserModes											_userModes;
	std::vector<std::pair<Channel&, PrivilegesModes> >	_vectorChannels; 
	//channels[0][0] get Channel 
	//channels[0][1] get privilege

	class ReadingDataException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: During reading data of client");
			}
	};
	
	class ClientDisconnectedException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Client disconnected");
			}
	};
};

#endif