#ifndef UTILS_H
# define UTILS_H

#include "Client.hpp"

class Client; 

# define USER 0
# define OPERATOR 1
# define TIMEOUT 5000
# define MAX_CLIENTS 100
# define BUFFER_SIZE 1024


std::vector<std::string> ft_split_string(std::string str, std::string delimiter);
std::string concat_vect_string(std::vector<std::string> tab, std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);
std::vector<std::string> separateCmd(std::string cmd, Client *client);

#endif