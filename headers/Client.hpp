/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:53 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/09 00:50:34 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

#include "Client.hpp"
//#include "Channel.hpp"
#include <iostream>

class Client {

	public:
	Client(void);
	Client(const int& client_fd);
	~Client(void);
	// Client(const Client& obj);
	// Client& operator=(const Client& rhs);

	// methods
	std::string	read(void);

	// setters and getters
	void		setNickname(const std::string& nickname);
	void		setStatus(int status);
	std::string	getNickname(void) const;
	int			getStatus(void) const;
	

	private:
	int													_client_fd;
	std::string											_nickname;
	int													_status;
	//std::vector<std::pair<Channel&, struct privilege>>	_channels; 
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