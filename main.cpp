/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:35:54 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/22 21:09:42 by aabdou           ###   ########.fr       */
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
		std::cout << obj.GetServers()[0].GetIpAddress() << "\n";
	}
	catch(const std::exception &e) {
		cerr << e.what() << std::endl;
	}
	return 0;
}
