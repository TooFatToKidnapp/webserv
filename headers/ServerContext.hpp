/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:49:22 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/28 18:23:17 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONTEXT_HPP
#define SERVERCONTEXT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstddef> // size_t
#include <stdlib.h> // exit()


class ServerContext {
	private:
		size_t LocationPos;
		bool Listen;
		bool ServerName;
		size_t ServerID;

	public:
		ServerContext(size_t *start, std::string file, size_t ServerId);
		~ServerContext();
		ServerContext(const ServerContext &obj);
		ServerContext &operator=(const ServerContext& obj);





};




#endif
