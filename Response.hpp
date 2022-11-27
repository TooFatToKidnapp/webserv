/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:15:10 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/11/27 15:27:34 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include "codes.hpp"
#include "Utils.hpp"

class Response {
private:
public:
	std::map<std::string, std::string>	_Headers;
	std::string							_HttpVersion;
	int									_Status;
	int									_Clientfd;

	Response(int clientfd, Request req);
	~Response();

	void sendFile(const std::string &filename);
	void sendDir(const char *path, std::string const &host, int port);
	std::string getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host, int port);

	void sendHeaders(const std::string &filename);
};
