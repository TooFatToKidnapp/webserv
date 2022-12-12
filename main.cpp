/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:35:54 by aabdou            #+#    #+#             */
/*   Updated: 2022/12/12 01:19:58 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./headers/parsing/ConfigFileParser.hpp"
#include "./headers/parsing/utils.hpp"
#include <vector>
#include <map>

using namespace std;

int main(int ac, char *av[]) {
	try {
		ConfigFileParser obj;
		obj.ParseFile(ac, av);
		CheckDirectoryValidity(obj); // check if the directorys in server context are valid
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
