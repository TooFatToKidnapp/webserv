/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:16:38 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/11/29 12:33:39 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(int clientfd, Request req) {
	_Headers = req.getHeaders();
	_Headers["Date"] = getDate();
	_Headers["http-version"] = req.getHttpVersion();
	_Status = req.getStatus();
	_Clientfd = clientfd;
}

Response::~Response() {}

void Response::sendHeaders(const std::string &filename) {
	std::ifstream file(filename);
	if (!file.good())
		_Status = NotFound;

	std::size_t i = filename.find_last_of(".");
	std::string ext = (i != std::string::npos) ? filename.substr(i + 1) : "";

	if (ext != "") {
		std::string type = getMediaType(ext);
		_Headers["Content-Type"] = type.empty() ? "text/plain" : type;
	} else {
		_Headers["Content-Type"] = "text/plain";
	}
	
	if (file.good() && !getFileLength(filename))
		_Status = NoContent;

	std::ostringstream headers;

	headers << _Headers["http-version"] << " " << _Status << " " << ReasonPhrase(_Status) << "\r\n"
		<< "Date: " << _Headers["Date"] << "\r\n"
		<< "Content-Type: " << _Headers["Content-Type"] << "\r\n"
		<< "Content-Length: " << getFileLength(filename) << "\r\n"
		<< "Connection: " << _Headers["Connection"] << "\r\n"
		<< "\r\n";

	if (send(_Clientfd, headers.str().c_str(), headers.str().size(), 0) == -1)
		exit (1);
}

void Response::sendFile(const std::string &filename) {	
	char buffer[100001] = {0};
	int filelen = getFileLength(filename);
	int fd = open(filename.c_str(), O_RDONLY);
	int bytes_read;

	sendHeaders(filename);
	if (_Status != OK || fd < 0)
		return ;

	while (filelen > 0) {
		if ((bytes_read = read(fd, buffer, 100000)) <= 0)
			break ;
		buffer[bytes_read] = '\0';
		if (send(_Clientfd, buffer, bytes_read, 0) == -1)
			exit(1);
		filelen -= bytes_read;
	}
	close(fd);
}

void Response::sendDir(const char *path, const std::string &host, int port) {
	std::ostringstream headers;
	std::string dirName(path);
	struct dirent *dirEntry;
    DIR *dir = opendir(path);

    if (dir == NULL)
        _Status = Forbidden;

	headers << _Headers["http-version"] << " " << _Status << " " << ReasonPhrase(_Status) << "\r\n"
		<< "Date: " << _Headers["Date"] << "\r\n"
		<< "Connection: " << _Headers["Connection"] << "\r\n"
		<< "\r\n";
	if (send(_Clientfd, headers.str().c_str(), headers.str().size(), 0) == -1)
		exit (1);

    std::string page = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>" + dirName + "</title>\r\n\
	</head>\r\n<body>\r\n<h1>Webserv</h1>\r\n<p>\r\n";

	if (_Status == Forbidden) {
		//TODO : send a proper error page
		page = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>" + dirName + "</title>\r\n\
		</head>\r\n<body>\r\n<h1>403 Forbidden</h1>\r\n<p>\r\n";
		if (send(_Clientfd, page.c_str(), page.size(), 0) == -1)
			exit (1);
		return ;
	}

    while ((dirEntry = readdir(dir)) != NULL)
		page += getLink(std::string(dirEntry->d_name), dirName, host, port);
    page += "</p>\r\n</body>\r\n</html>\r\n";

	closedir(dir);
	if (send(_Clientfd, page.c_str(), page.size(), 0) == -1)
		exit (1);
}

std::string Response::getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host, int port) {
    std::stringstream	ss;

	if (pathIsFile(dirName + "/" + dirEntry) == 1)
		ss << "<p> <a style=\"color:black;text-decoration:none\"";
	else if (!pathIsFile(dirName + "/" + dirEntry))
		ss << "<p> <a style=\"color:red;\"";
	ss <<" href=\"http://" + host + ":" << port << dirName + "/" + dirEntry + "\">" + dirEntry + "</a></p>\r\n";
	return ss.str();
}