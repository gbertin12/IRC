/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:53 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/14 11:59:22 by gbertin          ###   ########.fr       */
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
	Client(const int& client_fd, Server* server);
	~Client(void);
	// Client(const Client& obj);
	// Client& operator=(const Client& rhs);

	// methods
	std::string			recvRequest(void);
	void				sendResponse(const std::string& message) const;
	void				sendResponseToChannel(const std::string& message, const std::string& channelName) const;
	void				sendResponseToServer(const std::string& message) const;
	void				sendResponseToUser(const std::string& message, const std::string& nickname) const;
	
	void				addChannel(Channel& channel);
	void				removeChannel(Channel& channel);

	// setters
	void				setNickname(const std::string& nickname);
	void				setClientFd(const int& client_fd);
	void				setUserModes(UserModes& userModes);
	//void				setServer(Server* server);
	void				setCommand(Command& command);
	void				setIsConnected(bool booleen);

	// getters
	int					getClientFd(void) const;
	std::string			getNickname(void) const;
	PrivilegesModes&	getPrivilege(Channel& channel);
	UserModes&			getUserModes(void);
	Server&				getServer(void);
	Command&			getCommand(void);
	bool&				getIsConnected(void);
	

	
	
	private:
	int													_client_fd;
	std::string											_nickname;
	std::vector<std::pair<Channel&, PrivilegesModes*> >	_vectorChannels; 
	bool												_isConnected;

	Server*												_server;
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