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

int main(int ac, char *av[]) {
	try {
		ConfigFileParser obj;
		obj.ParseFile(ac, av);
		std::cout << "Valid Config File" << std::endl;
		
	}
	catch(const std::exception &e) {
		cerr << e.what() << std::endl;
	}

	return 0;
}
