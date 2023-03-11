/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserModes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/03/11 11:29:18 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/modes/UserModes.hpp"


#include <iostream>

UserModes::UserModes(void) : _invisible(false), _operator(false), _wallops(false) { }

UserModes::~UserModes(void) { }

//methods 

void	UserModes::updateModes(std::vector<std::string> modes, Client& client)
{
	std::vector<std::string>::iterator	it;
	bool								validOption = false;

	this->_addOptions = "+";
	this->_removeOptions = "-";
	for (it = modes.begin() + 1; it != modes.end(); it++)
	{
		while ((*it).find("+") != std::string::npos || (*it).find("-") != std::string::npos)
		{
			int minus = 0;
			int plus = 0;
			if ((*it).find("-") != std::string::npos)
				 minus = (*it).find("-");
			if ((*it).find("+") != std::string::npos)
				 plus = (*it).find("+");
			if (minus < plus || (*it).find("+") == std::string::npos)
			{
				// remove mode
				if ((minus + 1) >= (int)(*it).size() || (*it)[minus + 1] == '-' || (*it)[minus + 1] == '+')
				{
					(*it).erase(minus, 1);
					continue ;
				}
				if (this->setModeByName((*it)[minus + 1], false))
				{
					validOption = true;
					if (this->_addOptions.find((*it)[minus + 1]) != std::string::npos)
						this->_addOptions.erase(this->_addOptions.find((*it)[minus + 1]), 1);
					if (this->_removeOptions.find((*it)[minus + 1]) == std::string::npos)
						this->_removeOptions += (*it)[minus + 1];
				}
				(*it).erase(minus + 1, 1);
			}
			else
			{
				// add mode
				if ((plus + 1) >= (int)(*it).size() || (*it)[plus + 1] == '-' ||(*it)[plus + 1] == '+')
				{
					(*it).erase(plus, 1);
					continue ;
				}
				if (this->setModeByName((*it)[plus + 1], true))
				{
					validOption = true;
					if (this->_removeOptions.find((*it)[plus + 1]) != std::string::npos)
						this->_removeOptions.erase(this->_removeOptions.find((*it)[plus + 1]), 1);
					if (this->_addOptions.find((*it)[plus + 1]) == std::string::npos)
						this->_addOptions += (*it)[plus + 1];	
				}
				(*it).erase(plus + 1, 1);
			}
		}
	}
	if (validOption)
	{
		if (this->_addOptions.size() == 1)
			this->_addOptions = "";
		if (this->_removeOptions.size() == 1)
			this->_removeOptions = "";
		client.sendResponse("MODE " + modes[0] + " " + this->_addOptions + this->_removeOptions + "\r\n");
	}
}

int		UserModes::setModeByName(char mode, bool value)
{
	switch (mode)
	{
		case 'i':
			this->setInvisibleMode(value);
			return 1;
		case 'r':
			this->setOperatorMode(value);
			return 1;
		case 'w':
			this->setWallopsMode(value);
			return 1;
		default:
			return 0;
	}
	return 0;
}

// getter
bool	UserModes::getInvisibleMode() const { return this->_invisible; }
bool	UserModes::getOperatorMode() const { return this->_operator; }
bool	UserModes::getWallopsMode() const { return this->_wallops; }

// setter
void	UserModes::setInvisibleMode(bool mode) { this->_invisible = mode; }
void	UserModes::setOperatorMode(bool mode) { this->_operator = mode; }
void	UserModes::setWallopsMode(bool mode) { this->_wallops = mode; }
