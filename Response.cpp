/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:16:38 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/11/25 17:09:13 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

#define BUFF_SIZE 10000

Response::Response(int clientfd, Request req) {
	_Headers = req.getHeaders();
	_Headers["Date"] = getDate();
	_Headers["http-version"] = req.getHttpVersion();
	_Status = req.getStatus();
	_Clientfd = clientfd;
}

Response::~Response() {}

void Response::sendFile(const std::string &filename) {
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
	int fileLength = getFileLength(filename);
	std::ostringstream headers;

	headers << _Headers["http-version"] << " " << _Status << " " << ReasonPhrase(_Status) << "\r\n"
		<< "Date: " << _Headers["Date"] << "\r\n"
		<< "Content-Type: " << _Headers["Content-Type"] << "\r\n"
		<< "Content-Length: " << fileLength << "\r\n"
		<< "Connection: " << _Headers["Connection"] << "\r\n"
		<< "\r\n\r\n";

	if (send(_Clientfd, headers.str().c_str(), headers.str().size(), 0) == -1)
		exit (1);

	std::string buffer;
	while (_Status != NotFound && !file.eof()) {
		try {
			getline(file, buffer);
		}
		catch(...) {
			break ;
		}
		if (send(_Clientfd, buffer.c_str(), buffer.size(), 0) == -1)
			break;
	}
}