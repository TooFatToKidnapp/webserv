/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:15:10 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/09 18:00:55 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include "codes.hpp"
#include "Utils.hpp"

class Response {
private:
	std::map<std::string, std::string>	_Headers;
	std::string							_HttpVersion;
	int									_Status;
	int									_Clientfd;

public:
	Response(int clientfd, Request req);
	~Response();

	void sendHeaders(const std::string &filename);
	void sendFile(const std::string &filename);
	void sendDir(const char *path, std::string const &host);
	void sendErrorPage(int status);
	std::string getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host);

	const int &getStatus() const;
};
