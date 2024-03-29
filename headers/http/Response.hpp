/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:15:10 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/27 17:47:56 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include "codes.hpp"
#include "Utils.hpp"
#include "../CGI/CGI.hpp"

class Response {
private:
	int									_Status;
	std::map<int, std::string>			_ErrorPage;
	std::string							_Index;
	bool								_AutoIndex;
	std::vector<std::string>			_Body;
	std::string							_Cookie;
	std::map<std::string, std::string>	_Headers;
	std::string							_Method;

public:
	Response(Request req);
	~Response();
	std::string cgi(Request const &obj);
	std::string sendHeaders(const std::string &filename);
	std::string sendFile(const std::string &filename);
	std::string sendDir(const char *path, std::string const &host);
	std::string sendErrorPage(int status);
	std::string getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host);
	std::string uploadFile();
	void parseCgiOutput(std::string &input, std::ostringstream &header, const std::string &ex);
	const int &getStatus() const;
	std::string deleteFile(std::string const & path);
	std::string const & GetCookie();
};
