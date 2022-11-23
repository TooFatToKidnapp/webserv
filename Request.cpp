/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:32:18 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/11/23 16:43:16 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Utils.hpp"

Request::Request(std::string &buffer) : _Status(OK), _Buffer(buffer) { RequestParsing(); }

Request::~Request() {}

void Request::RequestParsing() {
	std::vector<std::string> req = ft_split(_Buffer, "\r\n\r\n");

	if (req.size()) {
		std::vector<std::string> headers = ft_split(req[0], "\r\n");
		ParseStartLine(headers[0]);
		ParseHeaders(headers);
		if (_Headers["Transfer-Encoding"] == "chunked")
			ParseChunckedBody(req[1]);
		else
			ParseBody(req[1]);
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
	std::size_t pos;
	for (std::size_t i = 0; i < QueryPairs.size(); ++i) {
		pos = QueryPairs[i].find("=");
		if (pos != std::string::npos)
			_QueryMap[QueryPairs[i].substr(0, pos)] = QueryPairs[i].substr(pos + 1, QueryPairs[i].length());
		else
			_QueryMap[QueryPairs[i]] = "";
	}
}

void Request::ParseHeaders(std::vector<std::string> & headers) {
	for (std::size_t i = 1; i < headers.size() && !headers[i].empty(); ++i) {
		std::size_t pos = headers[i].find(":");
		if (pos == std::string::npos || (headers[i][pos - 1] && std::isspace(headers[i][pos - 1]))) {
			_Status = BadRequest;
			return ;
		}
		_Headers[ft_trim(headers[i].substr(0, pos))] = ft_trim(headers[i].substr(pos + 1, headers[i].length()));
	}
	if (_Headers.find("Host") == _Headers.end())
		_Status = BadRequest;
}

void Request::ParseChunckedBody(std::string &body) {
	std::vector<std::string> tmpBody = ft_split(body, "\r\n");

	for (std::size_t i = 1; i < tmpBody.size(); i += 2)
		_Body.push_back(tmpBody[i]);
}

void Request::ParseBody(std::string &body) {
	std::size_t bodySize = body.size();
	std::stringstream ss;
	if (_Headers["Content-Length"] != "") {
		ss << _Headers["Content-Length"];
		ss >> bodySize;
	}
	_Body.push_back(body.substr(0, bodySize));
}

const std::string &Request::getHttpVersion() const {
	return _HttpVersion;
}

const std::map<std::string, std::string> &Request::getHeaders() const {
	return _Headers;
}

const int &Request::getStatus() const {
	return _Status;
}