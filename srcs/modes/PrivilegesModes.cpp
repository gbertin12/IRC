/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivilegesModes.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/11 09:03:35 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/modes/PrivilegesModes.hpp"
#include <iostream>

PrivilegesModes::PrivilegesModes(void) : _isOp(false), _voice(false) { }

PrivilegesModes::~PrivilegesModes(void) {}

void	PrivilegesModes::setOp(bool mode) { this->_isOp = mode; }
void	PrivilegesModes::setVoice(bool mode) { this->_voice = mode; }
bool	PrivilegesModes::isOp(void) const { return (this->_isOp); }
bool	PrivilegesModes::isVoice(void) const { return (this->_voice); }