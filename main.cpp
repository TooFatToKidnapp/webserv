/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:37:41 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/11/29 16:38:43 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "codes.hpp"

int main(void)
{
	try {
		Socket Sct(80);
		for(;;) {
			int new_fd = Socket::acceptConnection(Sct, Sct.getAddress(), Sct.getAddrlen());
			Socket::testConnection(new_fd, "could not accept the connection");

			char buffer[30000] = {0};
        	long valread = recv( new_fd , buffer, 30000, 0);
        	if (valread == -1)
				return 1;

			std::string tmp(buffer);
			Request req(tmp);
			Response res(new_fd, req);

			if (req._Path == "/")
				req._Path = "/Users/ylabtaim/Desktop";

			if (!pathIsFile(req._Path)) {
				res.sendDir(req._Path.c_str(), "localhost", 80);
			}
			else
				res.sendFile(req._Path);
			close (new_fd);
		}
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
