/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:15:10 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/19 22:09:08 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include "codes.hpp"
#include "Utils.hpp"
#include "../cgi/CGI.hpp"

class Response {
private:
	std::map<std::string, std::string>	_Headers;
	std::string							_HttpVersion;
	int									_Status;
	int									_Clientfd;
	std::map<int, std::string>			_ErrorPage;
	std::string							_Index;
	bool								_AutoIndex;

public:
	Response(int clientfd, Request req);
	~Response();
	void cgi(Request const &obj);
	void sendHeaders(const std::string &filename);
	void sendFile(const std::string &filename);
	void sendDir(const char *path, std::string const &host);
	void sendErrorPage(int status);
	std::string getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host);

	const int &getStatus() const;
};
