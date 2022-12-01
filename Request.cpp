/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:32:18 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/01 18:36:08 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Utils.hpp"

Request::Request(std::string &buffer) : _Status(OK), _Buffer(buffer) {
	RequestParsing();
}

Request::~Request() {}

void Request::RequestParsing() {
	std::vector<std::string> req = ft_split(_Buffer, "\r\n\r\n");

	if (req.size()) {
		std::vector<std::string> headers = ft_split(req[0], "\r\n");
		for (std::size_t i = 0; i < headers.size(); ++i)
			std::replace(headers[i].begin(), headers[i].end(), '\r', ' ');
		ParseStartLine(headers[0]);
		ParseHeaders(headers);
		if (req.size() == 2) {
			if (_Headers.find("Transfer-Encoding") != _Headers.end() && _Headers["Transfer-Encoding"] == "chunked")
				ParseChunckedBody(req[1]);
			else
				ParseBody(req[1]);
			if (req[1].size() > 2000000) {
				_Status = PayloadTooLarge;
				throw std::runtime_error("Payload is too large");
			}
		}
	}
}

void Request::ParseStartLine(std::string & str) {
	std::vector<std::string> StartLine = ft_split(str, " ");

	if (StartLine.size() == 3) {
		_Method = StartLine[0];
		if (_Method != "GET" && _Method != "POST" && _Method != "DELETE") {
			_Status = NotImplemented;
			throw std::runtime_error("Method not implemented");
		}
		std::vector<std::string> RequestTarget = ft_split(StartLine[1], "?");
		_Path = RequestTarget[0];
		if (_Path.size() > 2000) {
			_Status = URITooLong;
			throw std::runtime_error("URI is too long");
		}
		if (pathIsFile(_Path) == 1) {
			std::fstream check(_Path);
			if (!check.good())
				_Status = Forbidden;
		}
		else if (pathIsFile(_Path) == 0) {
			DIR *dir = opendir(_Path.c_str());
			if (dir == NULL)
                _Status = Forbidden;
		}
		else
            _Status = NotFound;
		_Query = RequestTarget.size() == 2 ? RequestTarget[1] : "";
		if (_Query != "")
			ParseQuery(_Query);
		_HttpVersion = StartLine[2];
		if (_HttpVersion != "HTTP/1.1") {
			_Status = HTTPVersionNotSupported;
			throw std::runtime_error("HTTP version not supported");
		}
	} else {
		_Status = BadRequest;
		throw std::runtime_error("Problem in the start line");
	}
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
		if (std::isspace(headers[i][0]) || std::isspace(headers[i][headers.size() - 1]))
			continue ;
		std::size_t pos = headers[i].find(":");
		if (pos == std::string::npos || (headers[i][pos - 1] && std::isspace(headers[i][pos - 1]))) { 
			_Status = BadRequest;
			throw std::runtime_error("Problem with the headers");
		}
		_Headers[headers[i].substr(0, pos)] = ft_trim(headers[i].substr(pos + 1, headers[i].length()));
	}
	if (_Headers.find("Host") == _Headers.end()) {
		_Status = BadRequest;
		throw std::runtime_error("host header is missing");
	}
	if (_Headers.find("Transfer-Encoding") != _Headers.end() && _Headers.find("Content-Length") != _Headers.end())
		throw std::runtime_error("Can't have both Transfer-Encoding and Content-Length");
}

void Request::ParseChunckedBody(std::string &body) {
	std::vector<std::string> tmpBody = ft_split(body, "\r\n");

	for (std::size_t i = 1; i < tmpBody.size(); i += 2)
		_Body.push_back(tmpBody[i]);
}

void Request::ParseBody(std::string &body) {
	std::size_t bodySize;
	std::stringstream ss;
	if (_Headers.find("Content-Length") != _Headers.end()) {
		ss << _Headers["Content-Length"];
		ss >> bodySize;
		if (bodySize != body.size()) {
			_Status = BadRequest;
			throw std::runtime_error("Content-Length is unconsistent with the body size");
		}
	}
	if (body.size())
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