/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:32:18 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/11/21 18:35:54 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Utils.hpp"

Request::Request(std::string &buffer) :_Buffer(buffer), _Status(OK) { RequestParsing(); }

Request::~Request() {}

void Request::RequestParsing() {
	std::vector<std::string> req = ft_split(_Buffer, "\n");
	if (req.size()) {
		ParseStartLine(req[0]);
		ParseHeaders(req);
		ParseBody(); // TODO
	}
}

void Request::ParseStartLine(std::string & str) {
	std::vector<std::string> StartLine = ft_split(str, WHITESPACE);

	if (StartLine.size() == 3) {
		_Method = StartLine[0];
		if (_Method != "GET" && _Method != "POST" && _Method != "DELETE")
			_Status = NotImplemented;
		std::vector<std::string> RequestTarget = ft_split(StartLine[1], "?");
		//TODO : set _Status to 414 if URI is Too Long
		_Path = RequestTarget[0];
		_Query = RequestTarget.size() == 2 ? RequestTarget[1] : "";
		if (_Query != "")
			ParseQuery(_Query);
		_HttpVersion = StartLine[2];
		if (_HttpVersion != "HTTP/1.1")
			_Status = HTTPVersionNotSupported;
	}
	else
		_Status = BadRequest;
}

void Request::ParseQuery(std::string & query) {
	std::vector<std::string> QueryPairs = ft_split(query, "&");
	int pos;
	for (int i = 0; i < QueryPairs.size(); ++i) {
		pos = QueryPairs[i].find("=");
		if (pos != std::string::npos)
			_QueryMap[QueryPairs[i].substr(0, pos)] = QueryPairs[i].substr(pos + 1, QueryPairs[i].length());
		else
			_QueryMap[QueryPairs[i]] = "";
	}
}

void Request::ParseHeaders(std::vector<std::string> & headers) {
	for (int i = 1; i < headers.size() && !headers[i].empty(); ++i) {
		int pos = headers[i].find(":");
		if (pos == std::string::npos || (headers[i][pos - 1] && std::isspace(headers[i][pos - 1]))) {
			_Status = BadRequest;
			return ;
		}
		_Headers[ft_trim(headers[i].substr(0, pos))] = ft_trim(headers[i].substr(pos + 1, headers[i].length()));
	}
	if (_Headers.find("Host") == _Headers.end())
		_Status = BadRequest;
}

void Request::ParseBody() {
	
}