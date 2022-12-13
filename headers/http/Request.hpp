/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:32:03 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/13 20:54:14 by aabdou           ###   ########.fr       */
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
	std::map<int, std::string>			_ErrorPage;
	std::string							_Index;
	bool								_AutoIndex;

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
	const std::string &getIndex() const;
	const std::map<int, std::string> &getErrorPage() const;
	bool getAutoIndex() const;

	void updatePath(const std::string & path);
	void checkMethod(const std::string & path);
	bool findServer(std::vector<ServerContext> const & servers, std::string &buffer);
	const ServerContext& GetServerBlock() const;
	const std::vector<std::string>& GetBody() const;
};
