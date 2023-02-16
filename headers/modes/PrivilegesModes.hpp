/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivilegesModes.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertin <gbertin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:38:53 by gbertin           #+#    #+#             */
/*   Updated: 2023/02/11 09:00:46 by gbertin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVILEGESMODES_H
# define PRIVILEGESMODES_H

#include "PrivilegesModes.hpp"

class PrivilegesModes {

	public:

	PrivilegesModes(void);
	~PrivilegesModes(void);
	// PrivilegesModes(const PrivilegesModes& obj);
	// PrivilegesModes& operator=(const PrivilegesModes& rhs);

	// methods
	//void	updatePrivileges(Client& user);

	void	setOp(bool mode);
	void	setVoice(bool mode);

	bool	isOp(void) const;
	bool	isVoice(void) const;
	
	private:
	
	bool		_isOp;
	bool		_voice;
	

};

#endif