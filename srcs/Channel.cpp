/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:49 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/08 22:45:25 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <iostream>

Channel::Channel(void){
std::cout << "Channel Constructor called" << std::endl;
	return ;
}

Channel::~Channel(void) {
	std::cout << "Channel Destructor called" << std::endl;
	return ;
}

Channel::Channel(const Channel& obj) {
	return ;
}

Channel&	Channel::operator=(const Channel& obj) {
	return *this;
}
