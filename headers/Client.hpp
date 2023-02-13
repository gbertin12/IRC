/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:53 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/12 22:56:17 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

#include "Channel.hpp"
#include "Command.hpp"
#include "modes/UserModes.hpp"
#include "modes/PrivilegesModes.hpp"

#include <iostream>
#include <vector>

class Server;
class Command;

class Client {

	public:
	//Client(void);
	Client(const int& client_fd, Server& server);
	~Client(void);
	// Client(const Client& obj);
	// Client& operator=(const Client& rhs);

	// methods
	std::string			recvRequest(void);
	void				sendResponse(const std::string& message) const;
	
	void				addChannel(Channel& channel);
	void				removeChannel(Channel& channel);
	
	Command& 			getCommand(void);

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
	Command												*_command;

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