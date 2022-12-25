/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:16:38 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/25 16:29:33 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/http/Response.hpp"

Response::Response(Request req, fd_set writefds) {
	_Writefds = writefds;
	_Headers = req.getHeaders();
	_Headers["Date"] = getDate();
	_Status = req.getStatus();
	_ErrorPage = req.getErrorPage();
	_Index = req.getIndex();
	_AutoIndex = req.getAutoIndex();
	_Body = req.getBody();
}

Response::~Response() {}

std::string Response::deleteFile(std::string const & path) {
	std::ostringstream	headers;
	std::fstream file(path);
	
	if (file.is_open())
		file.close();
	int status = std::remove(path.c_str());
	if (status != 0)
		_Status = Forbidden;
	else
		_Status	= NoContent;

	headers << "HTTP/1.1 " << _Status << " " << ReasonPhrase(_Status) << "\r\n"
	<< "Date: " << _Headers["Date"] << "\r\n"
	<< "Content-Type: text/plain\r\nContent-Length: 0\r\n"
	<< "Connection: " << _Headers["Connection"] << "\r\n"
	<< "\r\n";

	return headers.str();
}

std::string Response::uploadFile() {
	std::string 	body;
	std::string		filename;
	std::string		boundary;
	std::string		content;
	size_t			pos;
	size_t			delpos;
	size_t			endpos;

	pos = _Headers["Content-Type"].find("boundary");
	if (pos == std::string::npos)
		return sendErrorPage(BadRequest);
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
			return sendErrorPage(BadRequest);
		filename = body.substr(pos + 10, endpos - pos - 10);
		if (pathIsFile("./uploads/" + filename) == 1)
			return sendErrorPage(BadRequest);

		pos = body.find("\r\n\r\n", delpos);
		endpos = body.find(boundary, pos);
		if (pos == std::string::npos || endpos == std::string::npos)
			return sendErrorPage(BadRequest);
		content = body.substr(pos + 4, endpos - pos - 6);

		file.open("./uploads/" + filename);
		if (!file.is_open())
			return sendErrorPage(BadRequest);
		file << content;
		delpos = body.find(boundary, delpos + 1);
		if (body[delpos + boundary.size()] == '-' && body[delpos + boundary.size() + 1] == '-')
			break;
	}

	std::ostringstream	headers;
	std::string success = "<html>\r\n<head><title>Success</title></head>\r\n<body>\r\n<center><h1>";
	success.append("Files uploaded successfully");
	success.append("</h1></center>\r\n</body>\r\n</html>");

	headers << "HTTP/1.1 200 OK\r\n"
	<< "Date: " << _Headers["Date"] << "\r\n"
	<< "Content-Type: " << "text/html\r\n"
	<< "Content-Length: " << success.size() << "\r\n"
	<< "Connection: close\r\n\r\n";

	return (headers.str() + success);
}

std::string Response::cgi(Request const &obj){
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
	return headers.str();// + _cgi.getOutput();
}

std::string Response::sendHeaders(const std::string &filename) {
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

	return headers.str();
}

std::string Response::sendErrorPage(int status) {
	std::stringstream ss;
	std::string strStatus;

	ss << status;
	ss >> strStatus;

	if (_ErrorPage[_Status] != "" && pathIsFile(_ErrorPage[_Status]) == 1)
		return sendFile(_ErrorPage[_Status]);
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

		return (headers.str() + error);
	}
}

std::string Response::sendFile(const std::string &filename) {
	char buffer[51] = {0};
	int filelen = getFileLength(filename);
	int fd = open(filename.c_str(), O_RDONLY);
	int bytes_read;
	std::string response;

	response = sendHeaders(filename);

	while (filelen > 0) {
		if ((bytes_read = read(fd, buffer, 50)) <= 0)
			break ;
		buffer[bytes_read] = '\0';
		response += buffer;
		filelen -= bytes_read;
	}
	close(fd);
	return response;
}


std::string Response::sendDir(const char *path, const std::string &host) {
	if (_Index != "") {
		int fd = open(_Index.c_str(), O_RDWR);
		if (fd == -1) {
			return sendErrorPage(Forbidden);
		} else {
			close (fd);
			return sendFile(_Index);
		}
	}
	if (_AutoIndex == false)
		return sendErrorPage(Forbidden);
	std::ostringstream headers;
	std::string dirName(path);
	struct dirent *dirEntry;
    DIR *dir = opendir(path);
	headers << "HTTP/1.1 " << _Status << " " << ReasonPhrase(_Status) << "\r\n"
		<< "Date: " << _Headers["Date"] << "\r\n"
		<< "Connection: " << _Headers["Connection"] << "\r\n"
		<< "\r\n";
    std::string page = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>" + dirName + "</title>\r\n\
	</head>\r\n<body>\r\n<h1>Webserv</h1>\r\n<p>\r\n";
    while ((dirEntry = readdir(dir)) != NULL)
		page += getLink(std::string(dirEntry->d_name), dirName, host);
    page += "</p>\r\n</body>\r\n</html>\r\n";
	closedir(dir);
	return (headers.str() + page);
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
