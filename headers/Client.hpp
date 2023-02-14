/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:53 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/14 10:39:15 by gbertin          ###   ########.fr       */
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
	void				setClientFd(const int& client_fd);
	void				setUserModes(UserModes& userModes);
	//void				setServer(Server& server);
	void				setCommand(Command& command);

	// getters
	int					getClientFd(void) const;
	std::string			getNickname(void) const;
	PrivilegesModes&	getPrivilege(Channel& channel);
	UserModes&			getUserModes(void);
	Server&				getServer(void);
	Command&			getCommand(void);
	

	
	
	private:
	int													_client_fd;
	std::string											_nickname;
	std::vector<std::pair<Channel&, PrivilegesModes*> >	_vectorChannels; 
	
	Server&												_server;
	UserModes*											_userModes;
	Command*											_command;

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