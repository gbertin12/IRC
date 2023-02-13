/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:53 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/13 14:42:40 by gbertin          ###   ########.fr       */
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



class Server {

	public:

	Server(const std::string &port, const std::string& password);
	~Server(void);
	// Server(const Server& obj);
	// Server& operator=(const Server& rhs);

	void	acceptClient(void);
	void	run(void);

	private:
	int							_sockfd;
	int							_port;
	const std::string&			_password;
	struct sockaddr_in			_servaddr;
		
	std::map<int, Client*>		_mapClients;
	std::vector<pollfd>			_vectorPollfds;
	std::vector<Channel>		_vectorChannels;
	
	
	Server(void);

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
};

#endif