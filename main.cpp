/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:35:54 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/04 13:04:13 by aabdou           ###   ########.fr       */
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
		std::cout << obj.GetServers()[0].GetRoot() << std::endl;
		std::cout << obj.GetServers()[0].GetCmbs() << std::endl;
		std::cout << obj.GetServers()[0].GetAutoIndexDir() << std::endl;
		// std::cout << obj.GetServers()[0].PrintIndex() << std::endl;
		// 
		// std::cout << "******************`\n";
		// std::map<int , std::string>::iterator it = obj.GetServers()[0].GetErrorPage().begin();
		// std::map<int , std::string>::iterator it2 = obj.GetServers()[0].GetErrorPage().end();
		// for(;it != it2; ++it)
			// std::cout << it->first << " " << it->second << std::endl;
	}
	catch(const std::exception &e) {
		cerr << e.what() << std::endl;
	}

	return 0;
}
