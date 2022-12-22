/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:15:10 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/22 12:11:23 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include "codes.hpp"
#include "Utils.hpp"
#include "../CGI/CGI.hpp"

class Response {
private:
	std::map<std::string, std::string>	_Headers;
	int									_Status;
	int									_Clientfd;
	std::map<int, std::string>			_ErrorPage;
	std::string							_Index;
	bool								_AutoIndex;
	std::vector<std::string>			_Body;

public:
	Response(int clientfd, Request req);
	~Response();
	void cgi(Request const &obj);
	void sendHeaders(const std::string &filename);
	void sendFile(const std::string &filename);
	void sendDir(const char *path, std::string const &host);
	void sendErrorPage(int status);
	std::string getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host);
	void uploadFile();

	const int &getStatus() const;
};
