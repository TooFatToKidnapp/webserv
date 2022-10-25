/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:35:54 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/25 10:43:59 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./headers/ConfigFileParser.hpp"

int main(int ac, char *av[]) {

	try {
		ConfigFileParser obj;
		obj.ParseFile(ac, av);
	}
	catch(const std::exception &e) {
		cerr << e.what() << std::endl;
	}

	return 0;
}
