/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:35:54 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/23 20:09:53 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./headers/ConfigFileParser.hpp"
#include "./headers/utils.hpp"
#include <vector>
#include <map>

int main(int ac, char *av[]) {
	try {
		ConfigFileParser obj;
		obj.ParseFile(ac, av);
		std::multimap<std::string, std::string> var = obj.GetServers()[1].GetListen();
		std::multimap<std::string, std::string>::iterator it = var.begin();
		std::multimap<std::string, std::string>::iterator it2 = var.end();
		for (; it != it2; ++it) {
			std::cout << it->first << " " << it->second << "\n";
		}
	}
	catch(const std::exception &e) {
		cerr << e.what() << std::endl;
	}
	return 0;
}
