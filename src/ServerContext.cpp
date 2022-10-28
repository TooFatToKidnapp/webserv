/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:46:12 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/28 18:24:05 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../headers/ServerContext.hpp"

ServerContext::ServerContext(size_t *start, std::string file, size_t ServerId) : LocationPos(0), Listen(false), ServerName(false)
	, ServerID(ServerId) {
		

}

ServerContext::~ServerContext() {}

ServerContext::ServerContext(const ServerContext &obj) {
	*this = obj;
}

ServerContext &ServerContext::operator=(const ServerContext& obj) {

}
