/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:37:41 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/11/22 18:28:39 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Response.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "codes.hpp"

int main() {

	std::string buffer = "GET /objectserver/restapi/alerts/status HTTP/1.1\r\nAccept: application/json\r\nAuthorization: Basic dGVzdHVzZXIwMTpuZXRjb29s\r\nHost: localhost\r\nConnection: keep-alive\r\n\r\nbodylines\r\n";
	Request req(buffer);
	std::cout << req._Status << '\n';
	return 0;
}

// int main(int ac , char *av[])
// {
// 	try {
// 		char* webserv = "webserv";
// 		Socket Sct(80);
// 		for(;;) {
// 			int new_fd = Socket::acceptConnection(Sct, Sct.getAddress(), Sct.getAddrlen());
// 			Socket::testConnection(new_fd, "could not accept the connection");

// 			char buffer[30000] = {0};
//         	long valread = read( new_fd , buffer, 30000);
//         	if (valread == -1)
// 				return 1;
// 			std::string tmp(buffer);
// 			printf("%s\n",buffer );
// 			Request req(tmp);
// 			write(new_fd , webserv , strlen(webserv));
// 			close (new_fd);
// 		}
// 	}
// 	catch(const std::exception& e) {
// 		std::cerr << e.what() << '\n';
// 		return 1;
// 	}
// 	return 0;
// }
