/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:16:38 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/19 14:57:44 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(int clientfd, Request req) {
	_Headers = req.getHeaders();
	_Headers["Date"] = getDate();
	_Status = req.getStatus();
	_Clientfd = clientfd;
	_ErrorPage = req.getErrorPage();
	_Index = req.getIndex();
	_AutoIndex = req.getAutoIndex();
}

Response::~Response() {}

void Response::sendHeaders(const std::string &filename) {
	std::ifstream file(filename);
	std::ostringstream headers;

	std::size_t i = filename.find_last_of(".");
	std::string ext = (i != std::string::npos) ? filename.substr(i + 1) : "";

	if (ext != "") {
		std::string type = getMediaType(ext);
		_Headers["Content-Type"] = type.empty() ? "text/plain" : type;
	} else {
		_Headers["Content-Type"] = "text/plain";
	}

	headers << "HTTP/1.1 " << _Status << " " << ReasonPhrase(_Status) << "\r\n"
		<< "Date: " << _Headers["Date"] << "\r\n"
		<< "Content-Type: " << _Headers["Content-Type"] << "\r\n"
		<< "Content-Length: " << getFileLength(filename) << "\r\n"
		<< "Connection: " << _Headers["Connection"] << "\r\n"
		<< "\r\n";

	if (send(_Clientfd, headers.str().c_str(), headers.str().size(), 0) == -1)
		throw std::runtime_error("Could not send the headers");
}

void Response::sendErrorPage(int status) {
	std::stringstream ss;
	std::string strStatus;

	ss << status;
	ss >> strStatus;

	if (_ErrorPage[_Status] != "" && pathIsFile(_ErrorPage[_Status]) == 1)
		sendFile(_ErrorPage[_Status]);
	else {
		std::string error = "<html>\r\n<head><title>Error</title></head>\r\n<body>\r\n<center><h1>";
		error.append(strStatus + "\t" + ReasonPhrase(status));
		error.append("</h1></center>\r\n</body>\r\n</html>");

		std::ostringstream headers;

		headers << "HTTP/1.1 " << _Status << " " << ReasonPhrase(_Status) << "\r\n"
		<< "Date: " << _Headers["Date"] << "\r\n"
		<< "Content-Type: " << "text/html\r\n"
		<< "Content-Length: " << error.size() << "\r\n"
		<< "Connection: " << "close\r\n";
		if (_Headers.find("Location") != _Headers.end())
			headers << "Location: " << _Headers["Location"] << "\r\n";
		headers << "\r\n";

		if (send(_Clientfd, headers.str().c_str(), headers.str().size(), 0) == -1)
			throw std::runtime_error("Could not send the headers");

		if (send(_Clientfd, error.c_str(), error.size(), 0) == -1)
			throw std::runtime_error("Could not send the body");
	}
}

void Response::sendFile(const std::string &filename) {	
	char buffer[100001] = {0};
	int filelen = getFileLength(filename);
	int fd = open(filename.c_str(), O_RDONLY);
	int bytes_read;

	sendHeaders(filename);

	while (filelen > 0) {
		if ((bytes_read = read(fd, buffer, 100000)) <= 0)
			break ;
		buffer[bytes_read] = '\0';
		if (send(_Clientfd, buffer, bytes_read, 0) == -1)
			throw std::runtime_error("Could not send the body");
		filelen -= bytes_read;
	}
	close(fd);
}

void Response::sendDir(const char *path, const std::string &host) {
	if (_Index != "") {
		int fd = open(_Index.c_str(), O_RDWR);
		if (fd == -1) {
			sendErrorPage(Forbidden);
		} else {
			sendFile(_Index);
			close (fd);
		}
		return ;
	}
	if (_AutoIndex == false) {
		sendErrorPage(Forbidden);
        return ;
	}
	std::ostringstream headers;
	std::string dirName(path);
	struct dirent *dirEntry;
    DIR *dir = opendir(path);
	headers << "HTTP/1.1 " << _Status << " " << ReasonPhrase(_Status) << "\r\n"
		<< "Date: " << _Headers["Date"] << "\r\n"
		<< "Connection: " << _Headers["Connection"] << "\r\n"
		<< "\r\n";
	if (send(_Clientfd, headers.str().c_str(), headers.str().size(), 0) == -1)
		throw std::runtime_error("Could not send the body");
    std::string page = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>" + dirName + "</title>\r\n\
	</head>\r\n<body>\r\n<h1>Webserv</h1>\r\n<p>\r\n";
    while ((dirEntry = readdir(dir)) != NULL)
		page += getLink(std::string(dirEntry->d_name), dirName, host);
    page += "</p>\r\n</body>\r\n</html>\r\n";
	closedir(dir);
	if (send(_Clientfd, page.c_str(), page.size(), 0) < 0)
		throw std::runtime_error("Could not send the body");
}

std::string Response::getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host) {
    std::stringstream	ss;
	
	std::string dir;
	if (dirName[dirName.length() - 1] != '/')
		dir = dirName + "/" + dirEntry;
	else
        dir = dirName + dirEntry;
	if (pathIsFile(dir) == 1)
		ss << "<p> <a style=\"color:black;text-decoration:none\"";
	else if (!pathIsFile(dir))
		ss << "<p> <a style=\"color:red;\"";
	ss <<" href=\"http://" + host << dir + "\">" + dirEntry + "</a></p>\r\n";
	return ss.str();
}

const int &Response::getStatus() const {
	return _Status;
}