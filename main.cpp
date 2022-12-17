/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:35:54 by aabdou            #+#    #+#             */
/*   Updated: 2022/12/17 03:36:16 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./headers/http/codes.hpp"
#include "./headers/http/Server.hpp"

int main(int ac, char *av[]) {
	try {
		ConfigFileParser conf;
		conf.ParseFile(ac, av);
		std::multimap<std::string, std::string> nmap;
		// std::pair<std::string, std::string> firstip("8080", "127.0.0.1");
		// std::pair<std::string, std::string> isip("9090", "127.0.0.1");
		// std::pair<std::string, std::string> secondip("1234", "127.0.0.1");
		// nmap.insert(firstip);
		// nmap.insert(secondip);
		// nmap.insert(isip);
		std::cout << conf.GetNumberOfServers() << "\n";
		for (size_t i = 0; i < conf.GetNumberOfServers(); i++) {
			std::multimap<std::string, std::string> tmp = conf.GetServers()[i].GetListen();
			for(std::multimap<std::string, std::string>::const_iterator it = tmp.begin(); it != tmp.end(); ++it) {
				nmap.insert(std::make_pair(it->first, it->second));
			}
		}
		for(std::multimap<std::string, std::string>::iterator it = nmap.begin() ; it != nmap.end(); ++it) {
				std::cout << "[" << it->first << " " << it->second << "]\n";
			}
		Server server(nmap);
		server.Run(conf);
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
