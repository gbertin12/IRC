/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserModes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/11 12:26:43 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/modes/UserModes.hpp"
#include <iostream>

UserModes::UserModes(void) : _invisible(false), _operator(false), _wallops(false) { }

UserModes::~UserModes(void) { }

// getter
bool	UserModes::getInvisibleMode() const { return this->_invisible; }
bool	UserModes::getOperatorMode() const { return this->_operator; }
bool	UserModes::getWallopsMode() const { return this->_wallops; }

// setter
void	UserModes::setInvisibleMode(bool mode) { this->_invisible = mode; }
void	UserModes::setOperatorMode(bool mode) { this->_operator = mode; }
void	UserModes::setWallopsMode(bool mode) { this->_wallops = mode; }
