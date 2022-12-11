/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:32:03 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/11 13:52:59 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "codes.hpp"

class Request {
private:
	int									_Status;
	std::string 						_Buffer;
	std::string							_Method;
	std::string							_Path;
	std::string							_Query;
	std::string							_HttpVersion;
	std::map<std::string, std::string>	_Headers;
	std::map<std::string, std::string>	_QueryMap;
	std::vector<std::string>			_Body;
	std::string							_Host;
	const ServerContext*				_Server;

public:
	Request(std::string & buffer, ConfigFileParser const &config);
	~Request();

	void RequestParsing();
	void ParseStartLine(std::string & str);
	void ParseHeaders(std::vector<std::string> & headers);
	void ParseChunckedBody(std::string &body);
	void ParseBody(std::string &body);
	void ParseQuery(std::string & query);

	const std::string &getHttpVersion() const;
	const std::map<std::string, std::string> &getHeaders() const;
	const int &getStatus() const;
	const std::string &getPath() const;
	const std::string &getHost() const;

	void updatePath(const std::string & path);
	void checkMethod(const std::string & path);
	bool findServer(std::vector<ServerContext> const & servers, std::string &buffer);
};
