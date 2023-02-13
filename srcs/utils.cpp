#include <string>
#include <iostream>
#include <vector>


std::vector<std::string> ft_split_string(std::string str)
{
	std::vector<std::string> tab_string;
	unsigned int index = 0;
	std::string delimiter = " ";
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

std::vector<std::string> separateCmd(std::string cmd)
{
	std::vector<std::string> tab; 
	size_t index = 0;

	while ((index = cmd.find("\r\n", 0)) != std::string::npos)
	{
		std::string newStr = cmd.substr(0, index);
		cmd.erase(0, index + 2);
		tab.push_back(newStr);
	}
	return (tab);
}