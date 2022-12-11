/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:35:54 by aabdou            #+#    #+#             */
/*   Updated: 2022/12/11 16:29:36 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./headers/parsing/ConfigFileParser.hpp"
#include "./headers/parsing/utils.hpp"

#include "http/Request.hpp"
#include "http/Response.hpp"
#include "http/Socket.hpp"

#include <vector>
#include <map>

int main(int ac, char *av[]) {
	try {
		ConfigFileParser conf;
		conf.ParseFile(ac, av);
		
		Socket Sct(80);
		for (;;) {
			int new_fd = Socket::acceptConnection(Sct, Sct.getAddress(), Sct.getAddrlen());
			Socket::testConnection(new_fd, "could not accept the connection");
			char buffer[30001] = {0};

			ssize_t valread = recv( new_fd , buffer, 30000, 0);
			if (valread == -1) {
				close (new_fd);
				return 1;
			}
			else if (valread == 0)
				continue ;
			buffer[valread] = '\0';
			std::string tmp(buffer);
			Request req(tmp, conf);
			Response res(new_fd, req);
			if (res.getStatus() != OK)
				res.sendErrorPage(res.getStatus());
			else if (!pathIsFile(req.getPath())) {
				res.sendDir(req.getPath().c_str(), req.getHost());
			}
			else
				res.sendFile(req.getPath());
			close (new_fd);
		}
	}
	catch(const std::exception &e) {
		cerr << e.what() << std::endl;
	}
	return 0;
}
