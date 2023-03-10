/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourrel <abourrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:53 by gbertin           #+#    #+#             */
/*   Updated: 2023/03/12 10:52:09 by abourrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

#include "Client.hpp"
#include "Channel.hpp"

#include <iostream>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

class Client;
class Channel;

class Server {

	public:

	Server(const std::string &port, const std::string& password);
	~Server(void);
	// Server(const Server& obj);
	// Server& operator=(const Server& rhs);

	void	acceptClient(void);
	void	run(void);
	bool	clientAuthentification(Client *client);
	void	freeClient(Client *client);
	bool	isClientExistByNickname(std::string &nickname);
	Client *getClientWithFd(int fd);
	Client *getClientByName(std::string &nickname);
	void	debug(void) const;
	void	addChannel(Channel *channel);
	void	removeChannel(Channel *channel);
	bool 	isChannelExist(const std::string& channelName);
	void	deleteAllClients(void);
	void	deleteAllChannels(void);
	bool 	commonChannel(Client *a, Client *b);

	// setter et getter
	
	std::string					getNameAdmin(void);
	std::string					getPwdAdmin(void);
	int							getSockFd(void);
	void						setSockFd(const int& sockFd);
	int&						getPort(void);
	void						setPort(const int& port);
	const std::string&			getPassword(void);
	sockaddr_in&				getServaddr(void);
	void						setServaddr(const sockaddr_in& servaddr);
	std::map<int, Client*>&		getMapClients(void);
	void						setMapClients(const std::map<int, Client*>& map);
	std::vector<pollfd>&		getVectorPollfds(void);
	void						setVectorPollfds(const std::vector<pollfd>& pollfd);
	std::vector<Channel*>&		getVectorChannels(void);
	void						setVectorChannels(std::vector<Channel*>& vectChannel);
	const std::string&			getName(void);

	class ClientDisconnectedException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Client disconnected");
			}
	};

	private:
	const std::string			_nameAdmin;
	const std::string			_pwdAdmin;
	const std::string			_name;
	int							_sockfd;
	int							_port;
	const std::string&			_password;
	struct sockaddr_in			_servaddr;
		
	std::map<int, Client*>		_mapClients;
	std::vector<pollfd>			_vectorPollfds;
	std::vector<Channel*>		_vectorChannels;
	
	//Server(void);

	class PortAlreadyUseException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Port already used");
			}
	};

	class SyntaxPortException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Bad Syntax Port : range port is 0 to 65535");
			}
	};

	class CreateSocketServerException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Unable to create socket server");
			}
	};

	class BindAddressException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Unable to bind address");
			}
	};

	class ListenSocketException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Unable to listen socket");
			}
	};

	class AcceptClientException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Unable to accept new client");
			}
	};

	class PollException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Poll error");
			}
	};

	class SetSocketOptionsException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Unable to set socket options");
			}
	};

};

#endif