/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:16:38 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/22 18:14:29 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/http/Response.hpp"

Response::Response(int clientfd, Request req) {
	_Headers = req.getHeaders();
	_Headers["Date"] = getDate();
	_Status = req.getStatus();
	_Clientfd = clientfd;
	_ErrorPage = req.getErrorPage();
	_Index = req.getIndex();
	_AutoIndex = req.getAutoIndex();
	_Body = req.getBody();
}

Response::~Response() {}


void Response::uploadFile() {
	std::string 	body;
	std::string		filename;
	std::string		boundary;
	std::string		content;
	size_t			pos;
	size_t			delpos;
	size_t			endpos;

	pos = _Headers["Content-Type"].find("boundary");
	if (pos == std::string::npos)
		throw std::runtime_error("missing boundary in headers");
	boundary = _Headers["Content-Type"].substr(pos + 9, _Headers["Content-Type"].size() - pos - 9);
	for (size_t i = 0; i < _Body.size(); ++i) {
		body.append(_Body[i]);
	}

	delpos = body.find(boundary);
	while (delpos != std::string::npos) {
		std::ofstream	file;
		pos = body.find("filename", delpos);
		endpos = body.find("\"", pos + 10);
		if (pos == std::string::npos || endpos == std::string::npos)
			throw std::runtime_error("corrupt body");
		filename = body.substr(pos + 10, endpos - pos - 10);
		if (pathIsFile("./uploads/" + filename) == 1)
			throw std::runtime_error("file already exists");

		pos = body.find("\r\n\r\n", delpos);
		endpos = body.find(boundary, pos);
		if (pos == std::string::npos || endpos == std::string::npos)
			throw std::runtime_error("corrupt body");
		content = body.substr(pos + 4, endpos - pos - 6);

		file.open("./uploads/" + filename);
		if (!file.is_open())
			throw std::runtime_error("cannot upload the files");
		file << content;
		delpos = body.find(boundary, delpos + 1);
		if (body[delpos + boundary.size()] == '-' && body[delpos + boundary.size() + 1] == '-')
			break;
	}
}

void Response::cgi(Request const &obj){
	std::string tmp = obj.getHost();
	size_t pos = tmp.find_first_of(":");
	short port;
	std::string para;
	if (pos == std::string::npos)
		port = 80;
	else{
		pos++;
		port = stoi(tmp.substr(pos));
	}
	CGI _cgi(obj, port);
	std::ostringstream headers;
	// headers << "HTTP/1.1" << " " << _Status << " " << ReasonPhrase(_Status) << "\r\n"
	// << "Date: " << _Headers["Date"] << "\r\n"
	// << "Content-Type: " << "text/html" << "\r\n"
	// << "Content-Length: " << _cgi.GetOutput().length() << "\r\n"
	// << "Connection: " << _Headers["Connection"] << "\r\n"
	// << "\r\n";
	parseCgiOutput(_cgi.GetOutput(), headers, _cgi.GetExtention());
	if (send(_Clientfd, headers.str().c_str(), headers.str().size(), 0) == -1)
		throw std::runtime_error("Could not send the headers");
	// if (send(_Clientfd, _cgi.GetOutput().c_str(), _cgi.GetOutput().size(), 0) == -1)
	// 	throw std::runtime_error("Could not send the headers");
}

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
	char buffer[51] = {0};
	int filelen = getFileLength(filename);
	int fd = open(filename.c_str(), O_RDONLY);
	int bytes_read;

	sendHeaders(filename);

	while (filelen > 0) {
		if ((bytes_read = read(fd, buffer, 50)) <= 0)
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

void Response::parseCgiOutput(std::string &input, std::ostringstream &header, std::string const &ex) {
	std::istringstream s(input);
	std::string buff;
	time_t raw;
	std::string tm;
	time(&raw);
	tm = ctime(&raw);
	tm.pop_back();

	header << "HTTP/1.1" << " " << _Status << " " << ReasonPhrase(_Status) << "\r\n" << "Server: WebServ\r\n" << "Date: " << tm << " GMT\r\n" << "Connection: " << _Headers["Connection"] << "\r\n";
	if (ex.compare(".php") == 0) {
		while (std::getline(s, buff)) {
			if (buff.find("X-Powered-By:") != std::string::npos)
				header << "X-Powered-By: "  << buff.substr(buff.find(": ") + 2);
			else if (buff.find("Set-Cookie:") != std::string::npos)
				header << "Set-Cookie: " <<  buff.substr(buff.find(": ") + 2);
			else if (buff.find("Expires:") != std::string::npos)
				header << "Expires: " << buff.substr(buff.find(": ") + 2);
			else if (buff.find("Cache-Control:") != std::string::npos)
				header << "Cache-Control: " << buff.substr(buff.find(": ") + 2);
			else if (buff.find("Pragma:") != std::string::npos)
				header << "Pragma: " << buff.substr(buff.find(": ") + 2);
			else if (buff.find("Content-type:") != std::string::npos)
				header << "Content-type: " <<  buff.substr(buff.find(": ") + 2);
			else if (buff.compare("\r\n\r\n") == 0)
				break;
		}
		input = input.substr(input.find("\r\n\r\n") + 4);
	}
	else if (ex.compare(".py") == 0) {
		while (std::getline(s, buff))
		{
			if (buff.find("Content-type:") != std::string::npos)
				header << "Content-type: " << buff.substr(buff.find(": ") + 2);
		}
		// std::cout << input << " " << input.length() << "\n";
		input = input.substr(input.find("\n\n") + 1);
		// std::cout << input << " " << input.length() << "\n";
	}
	header << "\r\n";
	header << "Content-Length: " + std::to_string(input.size());
	header << "\r\n\r\n";
	header << input;
}
