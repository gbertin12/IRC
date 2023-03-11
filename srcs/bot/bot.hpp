#ifndef BOT_H
# define BOT_H

#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

class Bot {

    public:
        Bot(const std::string& port, const std::string& password);
        ~Bot(void);
        Bot(const Bot& bot);
        Bot& operator=(const Bot& rhs);

        void		sendResponse(const std::string& message) const;
		void		pong(std::string buffer);
		void		handleResponse(std::string buffer);
		void		MoreOrLess(std::string buffer);
		std::string parseClient(std::string buffer);
		std::string parseMessage(std::string buffer);
		bool 		findClient(std::string client);
		void		addClient(std::string client);
		void		removeClient(std::string client);
		int			getRandom(std::string client);

        // methods
        void run(void);

    class SyntaxPortException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Syntax port");
			}
	};

    class SocketException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Socket");
			}
	};

    class ConnectException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Connect");
			}
	};

	class ReceiveException : public std::exception {
		public:
			virtual const char *what() const throw()
			{
				return ("Error: Receive");
			}
	};

    private:
        // attributes
        int _port;
        std::string _password;
        int _sockfd;
        struct sockaddr_in	_addr;
		std::map<std::string, int> _mapClientsMOL;
        
};

#endif