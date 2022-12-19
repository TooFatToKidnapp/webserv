/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouadel <obouadel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:35:54 by aabdou            #+#    #+#             */
/*   Updated: 2022/12/19 14:33:50 by obouadel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/codes.hpp"
#include "http/Server.hpp"

int main(int ac, char *av[]) {
	try {
		ConfigFileParser conf;
		conf.ParseFile(ac, av);
		std::multimap<std::string, std::string> nmap;
        std::pair<std::string, std::string> isip("80", "127.0.0.1");
        std::pair<std::string, std::string> isip1("6969", "127.0.0.1");
        std::pair<std::string, std::string> isip2("8080", "127.0.0.1");
        nmap.insert(isip2);
        nmap.insert(isip);
        nmap.insert(isip1);
		Server server(nmap);
		server.Run(conf);
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
