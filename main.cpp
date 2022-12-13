/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:35:54 by aabdou            #+#    #+#             */
/*   Updated: 2022/12/13 21:00:32 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./headers/http/codes.hpp"
#include "./headers/http/Server.hpp"

int main(int ac, char *av[]) {
	try {
		ConfigFileParser conf;
		conf.ParseFile(ac, av);
		std::multimap<std::string, std::string> nmap;
		std::pair<std::string, std::string> firstip("8080", "127.0.0.1");
		std::pair<std::string, std::string> isip("9090", "127.0.0.1");
		std::pair<std::string, std::string> secondip("1234", "127.0.0.1");
		nmap.insert(firstip);
		nmap.insert(secondip);
		nmap.insert(isip);

		Server server(nmap);
		server.Run(conf);
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
