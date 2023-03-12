#include "bot.hpp"

extern bool sigint_bot;

Bot::Bot(const std::string& port, const std::string& password) : _password(password)
{
    char *end;
    long numberlong = std::strtol(port.c_str(), &end, 10);
	
	//check if port is valid
	if (numberlong > 65535 || numberlong < 0 || *end != '\0')
		throw Bot::SyntaxPortException();

    this->_port = static_cast<unsigned int>(numberlong);

    this->_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->_sockfd == -1)
        throw Bot::SocketException();

    memset(&this->_addr, 0, sizeof(this->_addr));
    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = htons(this->_port);
    this->_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (connect(this->_sockfd, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) == -1)
    {
        close(this->_sockfd);
        throw Bot::ConnectException();
    }

    std::cout << "Bot connected to server on port " << this->_port << std::endl;

    sendResponse("PASS " + this->_password + "\r\n");
    sendResponse("NICK BOT\r\n");
    sendResponse("USER BOT 0 * :BOT\r\n");
}

Bot::~Bot(void)
{
    close(this->_sockfd);
}

Bot::Bot(const Bot& bot) : _port(bot._port), _password(bot._password)
{
    *this = bot;
}

Bot& Bot::operator=(const Bot& bot)
{
    this->_sockfd = bot._sockfd;
    this->_addr = bot._addr;
    return *this;
}

void	Bot::sendResponse(const std::string& message) const
{
	send(this->_sockfd, message.c_str(), message.length(), 0);
	std::cout << "\033[1;35m BOT --> " << message << "\033[m";
}

void Bot::run(void)
{
    char buffer[1024];
    struct pollfd pfd;
    pfd.fd = this->_sockfd;
    pfd.events = POLLIN;
    int bytes;

    while (sigint_bot == false)
    {
        if (poll(&pfd, 1, 5000) == -1)
        {
            if (errno == EINTR)
				break;
            close(this->_sockfd);
            throw Bot::ReceiveException();
        }

        if (pfd.revents & POLLIN)
        {
            memset(buffer, 0, 1024);
            bytes = recv(this->_sockfd, buffer, 1024, 0);
            if (bytes == -1)
            {
                close(this->_sockfd);
                throw Bot::ReceiveException();
            }
            else if (bytes == 0)
                break;
            
            std::cout << "\033[1;36m SERVEUR --> " << buffer << "\033[m";
            handleResponse(buffer);  
        }
    }
}

void Bot::handleResponse(std::string buffer)
{
    /*if (std::string(buffer).find("PING") != std::string::npos)
        pong(buffer);*/
    if (std::string(buffer).find("PRIVMSG") != std::string::npos)
        MoreOrLess(buffer);
}

/*void Bot::pong(std::string buffer)
{
   std::string pong = "PONG " + std::string(buffer).substr(5);
    sendResponse(pong);
}*/

std::string Bot::parseClient(std::string buffer)
{
    std::string client = std::string(buffer).substr(1);
    client = client.substr(0, client.find("!"));
    return client;
}

std::string Bot::parseMessage(std::string buffer)
{
    std::string message = std::string(buffer).substr(std::string(buffer).find("PRIVMSG"));
    message = message.substr(message.find(":") + 1);
    message = message.substr(0, message.find("\r\n"));
    return message;
}

bool Bot::findClient(std::string client)
{
    for (std::map<std::string, int>::iterator it = this->_mapClientsMOL.begin(); it != this->_mapClientsMOL.end(); it++)
    {
        if (it->first == client)
            return true;
    }
    return false;
}

void Bot::addClient(std::string client)
{
    int random = rand() % 100;
    this->_mapClientsMOL.insert(std::pair<std::string, int>(client, random));
    sendResponse("PRIVMSG " + client + " :I have a number between 0 and 100. Try to guess it\r\n");
}

int Bot::getRandom(std::string client)
{
    return this->_mapClientsMOL[client];
}

void Bot::removeClient(std::string client)
{
    this->_mapClientsMOL.erase(client);
}

bool is_digits(const std::string &str)
{
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        if (!std::isdigit(*it))
            return false;
    }
    return true;
}

void Bot::MoreOrLess(std::string buffer)
{
    std::string client = parseClient(buffer);
    std::string message = parseMessage(buffer);

    //std::cout << "Client: " << client << std::endl;
    //std::cout << "Message: " << message << std::endl;

    if (findClient(client) == false)
    {
        if (message == "!game")
        {
            addClient(client);
            return ;
        }
        else
        {
            sendResponse("PRIVMSG " + client + " :You need to start a game with !game\r\n");
            return ;
        }
    }
    else
    {
        if (message == "!game")
        {
            sendResponse("PRIVMSG " + client + " :You already have a game started\r\n");
            return ;
        }
        else
        {
            int number = std::atoi(message.c_str());
            if (number > 100 || number < 0 || is_digits(message) == false)
            {
                sendResponse("PRIVMSG " + client + " :Number must be between 0 and 100\r\n");
                return ;
            }
            else
            {
                int random = getRandom(client);
                if (number == random)
                {
                    sendResponse("PRIVMSG " + client + " :You win\r\n");
                    removeClient(client);
                    return ;
                }
                else if (number > random)
                {
                    sendResponse("PRIVMSG " + client + " :- Less -\r\n");
                    return ;
                }
                else
                {
                    sendResponse("PRIVMSG " + client + " :+ More +\r\n");
                    return ;
                }
            }
        }
    }
}