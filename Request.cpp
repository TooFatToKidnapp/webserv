/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:32:18 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/11/22 18:32:46 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Utils.hpp"

Request::Request(std::string &buffer) :_Buffer(buffer), _Status(OK) { RequestParsing(); }

Request::~Request() {}

//TODO: the body is parsed as a header

void Request::RequestParsing() {
	std::vector<std::string> req = ft_split(_Buffer, "\r\n");
	if (req.size()) {
		ParseStartLine(req[0]);
		ParseHeaders(req);
		if (_Headers["Transfer-Encoding"] == "chunked")
			ParseChunckedBody(_Buffer);
		else
			ParseBody(_Buffer);
	}
}

void Request::ParseStartLine(std::string & str) {
	std::vector<std::string> StartLine = ft_split(str, " ");

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

void Request::ParseChunckedBody(std::string &buffer) {
	std::string body = buffer.substr(buffer.find("\r\n\r\n") + 4);
	std::vector<std::string> tmpBody = ft_split(body, "\r\n");

	for (std::size_t i = 1; i < tmpBody.size(); i += 2)
		_Body.push_back(tmpBody[i]);
}

void Request::ParseBody(std::string &buffer) {
	std::string body = buffer.substr(buffer.find("\r\n\r\n") + 4);
	std::size_t bodySize = body.size();
	std::stringstream ss;
	if (_Headers["Content-Length"] != "") {
		ss << _Headers["Content-Length"];
		ss >> bodySize;
	}
	_Body.push_back(body.substr(0, bodySize));
}