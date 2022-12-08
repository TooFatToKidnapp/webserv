/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:35:54 by aabdou            #+#    #+#             */
/*   Updated: 2022/12/08 17:26:09 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./headers/ConfigFileParser.hpp"
#include "./headers/utils.hpp"
#include <vector>
#include <map>

using namespace std;

int main(int ac, char *av[]) {
	try {
		ConfigFileParser obj;
		obj.ParseFile(ac, av);
		// std::multimap<std::string, std::string> var = obj.GetServers()[1].GetListen();
		// std::multimap<std::string, std::string>::iterator it = var.begin();
		// std::multimap<std::string, std::string>::iterator it2 = var.end();
		// for (; it != it2; ++it) {
		// 	std::cout << it->first << " " << it->second << "\n";
		// }
		vector<ServerContext> var = obj.GetServers();
		cout << var[0].GetRoot() << "\n";
		vector<LocationContext> var2 = var[0].GetLocationContexts();
		cout << var2[0].GetRoot() << "\n";

	}
	catch(const std::exception &e) {
		cerr << e.what() << std::endl;
	}
	return 0;
}
