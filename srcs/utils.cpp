#include <string>
#include <iostream>
#include <vector>
#include "../headers/Client.hpp"

std::vector<std::string> ft_split_string(std::string str, std::string delimiter)
{
	std::vector<std::string> tab_string;
	unsigned int index = 0;
	size_t position = 0;

	while ((position = str.find(delimiter)) != std::string::npos)
	{
		if (str.substr(0, position).empty() == false)
			tab_string.push_back(str.substr(0, position));
		//std::cout << tab_string[index] << std::endl;
		str.erase(0, position + delimiter.size());
		index++;
	}
	if (str.empty() == false)
		tab_string.push_back(str);
	
	return (tab_string);
}

std::string concat_vect_string(std::vector<std::string> tab, std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end)
{
	(void)tab; //can be useful for debugging
	std::string concat = *begin;
	begin++;
	while (begin != end)
	{
		concat = concat + " " + *begin;
		begin++;
	}
	return (concat);
}

std::vector<std::string> separateCmd(std::string cmd, Client *client)
{
	std::vector<std::string> tab; 
	size_t index = 0;
	std::string buffer = client->getBuffer();
	std::string newStr;

	while ((index = cmd.find("\n", 0)) != std::string::npos)
	{	
		if (index != 0 && cmd[index - 1] == '\r')
		{
			newStr = cmd.substr(0, index - 1);
			cmd.erase(0, index + 1);
			buffer.erase(0, index + 1);
		}
		else
		{
			newStr = cmd.substr(0, index);
			cmd.erase(0, index + 1);
			buffer.erase(0, index + 1);
		}
		tab.push_back(newStr);
	}
	client->setBuffer(buffer);
	std::cout << "buffer: " << buffer << std::endl;
	return (tab);
}

