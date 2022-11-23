/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:37:41 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/11/23 18:37:17 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "codes.hpp"

int main(int ac , char *av[])
{
	(void)ac;
	(void)av;
	try {
		const char* webserv = "<!DOCTYPE html>\r\n<html>\r\n<body>\r\n<h1>Webserv</h1>\r\n</body>\r\n</html>";
		Socket Sct(80);
		for(;;) {
			int new_fd = Socket::acceptConnection(Sct, Sct.getAddress(), Sct.getAddrlen());
			Socket::testConnection(new_fd, "could not accept the connection");

			char buffer[30000] = {0};
        	long valread = recv( new_fd , buffer, 30000, 0);
        	if (valread == -1)
				return 1;
			std::string tmp(buffer);
			printf("%s\n",buffer );
			Request req(tmp);
			Response res(new_fd, req);
			
			send(new_fd, webserv, strlen(webserv), 0);
			close (new_fd);
		}
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
