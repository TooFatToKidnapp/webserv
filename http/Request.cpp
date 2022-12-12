/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:32:18 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/12 14:51:04 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Utils.hpp"

Request::Request(std::string &buffer, ConfigFileParser const &config) : _Status(OK), _Buffer(buffer) {
	std::vector<ServerContext> servers = config.GetServers();
	if (findServer(servers, _Buffer) == false) {
		_Status = BadRequest;
		_Headers["http-version"] = "HTTP/1.1";
		return ;
	}
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
			if (req[1].size() > _Server->GetCmbs() * 1000000){
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

bool	Request::findServer(std::vector<ServerContext> const & servers, std::string &buffer) {
	std::size_t begin = buffer.find("Host");
	if (begin == std::string::npos)
		return false;
	std::size_t end = buffer.find("\r\n", begin);
	if (end == std::string::npos)
        return false;
	_Host = ft_trim(buffer.substr(begin + 5 , end - begin - 5));
	if (_Host.empty())
		return false;
	
	for (std::size_t i = 0; i < servers.size(); ++i) {
		std::vector<std::string> serverNames = servers[i].GetServerNames();
		for (std::size_t j = 0; j < serverNames.size(); ++j) {
			std::vector<std::string> serverPorts = servers[i].GetPortNumbers();
			for (std::size_t k = 0; k < serverPorts.size(); ++k) {
				if ((serverNames[j] + ":" + serverPorts[k]) == _Host) {
					_Server = &servers[i];
					return true;
				}
				else if (serverNames[j] == _Host && serverPorts[k] == "80") {
					_Server = &servers[i];
					return true;
				}
			}
		}
	}
	if (servers.size() != 0) {
		_Server = &servers[0];
        return true;
	}
	return false;
}

void Request::updatePath(const std::string & path) {
	std::vector<LocationContext> locations = _Server->GetLocationContexts();

	_Path = path;
	for (std::size_t i = 0; i < locations.size(); ++i){
		if (locations[i].GetLocationUri().GetUri() == path || (locations[i].GetLocationUri().GetUri() + '/') == path) {
			if (!locations[i].GetReturn().GetUrl().empty()) {
				_Status = locations[i].GetReturn().GetCode();
				_Headers["Location"] = locations[i].GetReturn().GetUrl();
				_HttpVersion = "HTTP/1.1";
			}
			else if (!locations[i].GetRoot().empty())
				_Path = locations[i].GetRoot() + path;
			else if (!locations[i].GetAlias().empty())
				_Path = locations[i].GetAlias();
			if (!locations[i].GetIndex().empty()) {
				for (std::size_t j = 0; j < locations[i].GetIndex().size(); ++j) {
					std::string fullPath;
					if (!locations[i].GetReturn().GetUrl().empty())
						fullPath = locations[i].GetReturn().GetUrl() + path + '/' + locations[i].GetIndex()[j];
					else if (!locations[i].GetRoot().empty())
						fullPath = locations[i].GetRoot() + path + '/' + locations[i].GetIndex()[j];
					else
						fullPath = locations[i].GetAlias() + path + '/' + locations[i].GetIndex()[j];
					if (pathIsFile(fullPath) == 1) {
						_Index = fullPath;
						break ;
					}
				}
			}
			if (locations[i].HasErrorPage())
				_ErrorPage = locations[i].GetErrorPage();
			if (locations[i].GetAutoIndexDir() && _Index == "")
				_AutoIndex = true;
			else
				_AutoIndex = false;
		}
	}
}

void Request::checkMethod(const std::string &path) {
	if (_Method != "GET" && _Method != "POST" && _Method != "DELETE")
		_Status = NotImplemented;

	bool methodIsAllowed = false;
	std::vector<LocationContext> locations = _Server->GetLocationContexts();
	std::vector<std::string> methods;
	
	for (std::size_t i = 0; i < locations.size(); ++i) {
		if (path == locations[i].GetLocationUri().GetUri()) {
			methods = locations[i].GetMethods().GetMethods();
			break ;
		}
	}
	if (methods.size() == 0) {
		methodIsAllowed = true;
	} else {
		for (std::size_t i = 0; i < methods.size(); ++i) {
			if (_Method == methods[i]) {
				methodIsAllowed = true;
				break ;			
			}
		}
	}
	if (methodIsAllowed == false)
	    _Status = MethodNotAllowed;
}

void Request::ParseStartLine(std::string & str) {
	std::vector<std::string> StartLine = ft_split(str, " ");

	if (StartLine.size() == 3) {
		_Method = StartLine[0];
		checkMethod(StartLine[1]);
		if (StartLine[1].size() > 2000)
			_Status = URITooLong;
		std::vector<std::string> RequestTarget = ft_split(StartLine[1], "?");
		updatePath(RequestTarget[0]);
		if (_Status >= 300 && _Status < 400) return;
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

const std::string &Request::getHost() const {
	return _Host;
}

const std::map<int, std::string> &Request::getErrorPage() const {
	return _ErrorPage;
}

const std::string &Request::getIndex() const {
	return _Index;
}

bool Request::getAutoIndex() const {
	return _AutoIndex;
}