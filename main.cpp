/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:37:41 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/02 11:44:24 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "codes.hpp"

int main(void)
{
	Socket Sct(80);
	for(;;) {
		int new_fd = Socket::acceptConnection(Sct, Sct.getAddress(), Sct.getAddrlen());
		Socket::testConnection(new_fd, "could not accept the connection");

		try {
			char buffer[30001] = {0};
			ssize_t valread = recv( new_fd , buffer, 30000, 0);
			if (valread == -1)
				return 1;
			buffer[valread] = '\0';
			std::string tmp(buffer);
			Request req(tmp);
			Response res(new_fd, req);

			if (res.getStatus() != OK)
				res.sendErrorPage(res.getStatus());
			else if (!pathIsFile(req.getPath())) {
				res.sendDir(req.getPath().c_str(), "localhost", 80);
			}
			else
				res.sendFile(req.getPath());
			close (new_fd);
		}
		catch(const std::exception& e) {
			std::cerr << e.what() << '\n';
			close (new_fd);
		}
	}
	return 0;
}
