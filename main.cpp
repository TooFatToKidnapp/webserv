/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:35:54 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/21 19:10:41 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./headers/ConfigFileParser.hpp"
#include "./headers/utils.hpp"
#include<vector>
#include<map>

int main(int ac, char *av[]) {
	try {
		ConfigFileParser obj;
		obj.ParseFile(ac, av);
		std::cout << "Valid Config File" << std::endl;
		std::cout << obj.GetNumberOfServers() << "\n";
		std::multimap<std::string, std::string> var = obj.GetServers()[0].GetListen();
		std::multimap<std::string , std::string>::iterator it =  var.begin();
		std::multimap<std::string , std::string>::iterator it2 = var.end();
		for(;it != it2; ++it)
			std::cout << "[" << it->first << "]"<< " " <<"["<<it->second << "]"<< std::endl;

		std::cout << "*********************\n";
		std::multimap<std::string, std::string> var2 = obj.GetServers()[1].GetListen();
		std::multimap<std::string , std::string>::iterator _it =  var2.begin();
		std::multimap<std::string , std::string>::iterator _it2 = var2.end();
		for(;_it != _it2; ++_it)
			std::cout << "[" << _it->first << "]"<< " " <<"["<< _it->second << "]"<< std::endl;
		std::cout << "*********************\n";
		std::multimap<std::string, std::string> var3 = obj.GetServers()[2].GetListen();
		std::multimap<std::string , std::string>::iterator __it =  var3.begin();
		std::multimap<std::string , std::string>::iterator __it2 = var3.end();
		for(;__it != __it2; ++__it)
			std::cout << "[" << __it->first << "]"<< " " <<"["<< __it->second << "]"<< std::endl;
	}
	catch(const std::exception &e) {
		cerr << e.what() << std::endl;
	}

	return 0;
}
