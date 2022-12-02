/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:32:18 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/02 12:58:48 by ylabtaim         ###   ########.fr       */
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

	if (req.size() == 1 || req.size() == 2) {
		std::vector<std::string> headers = ft_split(req[0], "\r\n");
		for (std::size_t i = 0; i < headers.size(); ++i)
			std::replace(headers[i].begin(), headers[i].end(), '\r', ' ');
		ParseStartLine(headers[0]);
		if (_Status != OK) return ;
		ParseHeaders(headers);
		if (_Status != OK) return ;
		if (req.size() == 2) {
			if (req[1].size() > 2000000) {
				_Status = PayloadTooLarge;
				return ;
			}
			if (_Headers["Transfer-Encoding"] == "chunked") {
				if (_Headers.find("Content-Length") != _Headers.end()) {
					_Status = BadRequest;
					return ;
				}
				ParseChunckedBody(req[1]);
			}
			else
				ParseBody(req[1]);
		}
		else if (_Headers.find("Content-Length") != _Headers.end() && _Headers["Content-Length"] != "0")
			_Status = BadRequest;
	}
}

void Request::ParseStartLine(std::string & str) {
	std::vector<std::string> StartLine = ft_split(str, " ");

	if (StartLine.size() == 3) {
		_Method = StartLine[0];
		if (_Method != "GET" && _Method != "POST" && _Method != "DELETE")
			_Status = NotImplemented;
		if (StartLine[1].size() > 2000)
			_Status = URITooLong;
		std::vector<std::string> RequestTarget = ft_split(StartLine[1], "?");
		_Path = RequestTarget[0];
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
		if (std::isspace(headers[i][0]) || std::isspace(headers[i][headers.size() - 1]))
			continue ;
		std::size_t pos = headers[i].find(":");
		if (pos == std::string::npos || (headers[i][pos - 1] && std::isspace(headers[i][pos - 1]))) { 
			_Status = BadRequest;
			return ;
		}
		_Headers[headers[i].substr(0, pos)] = ft_trim(headers[i].substr(pos + 1, headers[i].length()));
	}
	if (_Headers.find("Host") == _Headers.end()) {
		_Status = BadRequest;
		return ;
	}
	if (_Headers["Transfer-Encoding"] == "chunked" && _Headers.find("Content-Length") != _Headers.end())
		_Status = BadRequest;
}

void Request::ParseChunckedBody(std::string &body) {
	std::vector<std::string> tmpBody = ft_split(body, "\r\n");
	std::size_t size;
	std::stringstream ss;

	for (std::size_t i = 0; i < tmpBody.size(); ++i) {
		if ((i % 2) == 0) {
			ss.clear();
			ss << std::hex << tmpBody[i];
			ss >> size;
		}		
		else {
			if (size != tmpBody[i].size()) {
				_Status = BadRequest;
				return ;
			}
			else
				_Body.push_back(tmpBody[i]);
		}
	}
}

void Request::ParseBody(std::string &body) {
	std::size_t bodySize;
	std::stringstream ss;
	if (_Headers.find("Content-Length") != _Headers.end()) {
		ss << _Headers["Content-Length"];
		ss >> bodySize;
		if (bodySize != body.size()) {
			_Status = BadRequest;
			return ;
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

const std::string &Request::getPath() const {
	return _Path;
}