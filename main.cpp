/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:35:54 by aabdou            #+#    #+#             */
/*   Updated: 2022/12/20 14:03:43 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./headers/http/codes.hpp"
#include "./headers/http/Server.hpp"

void s() {
	std::cout << "errno == " << strerror(errno) << "\n";
}

int main(int ac, char *av[]) {
	atexit(s);
	try {
		ConfigFileParser conf;
		conf.ParseFile(ac, av);
		std::multimap<std::string, std::string> nmap;
		for (size_t i = 0; i < conf.GetNumberOfServers(); i++) {
			std::multimap<std::string, std::string> tmp = conf.GetServers()[i].GetListen();
			for(std::multimap<std::string, std::string>::const_iterator it = tmp.begin(); it != tmp.end(); ++it) {
				nmap.insert(std::make_pair(it->first, it->second));
			}
		}
		Server server(nmap);
		server.Run(conf);
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
