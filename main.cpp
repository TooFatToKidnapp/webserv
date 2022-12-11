/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:35:54 by aabdou            #+#    #+#             */
/*   Updated: 2022/12/11 23:31:40 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./headers/parsing/ConfigFileParser.hpp"
#include "./headers/parsing/utils.hpp"
#include <vector>
#include <map>

using namespace std;

void func(const  ConfigFileParser &obj){
	std::string str;
	for (size_t i = 0 ; i < obj.GetNumberOfServers() ; i++) {
		for(size_t j = 0; j < obj.GetServers()[i].GetLocationContexts().size(); j++) {
			str = obj.GetServers()[i].GetLocationContexts()[j].GetRoot() + obj.GetServers()[i].GetLocationContexts()[j].GetLocationUri().GetInputURI();
			int fd = open(str.c_str(), O_RDWR);
			if (fd == -1){
				close(fd);
				throw std::invalid_argument("Errqor: Missing Dir.");
			}
			close(fd);
		}
	}
}

int main(int ac, char *av[]) {
	try {
		ConfigFileParser obj;
		obj.ParseFile(ac, av);
		func(obj);

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
