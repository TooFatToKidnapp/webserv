/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouadel <obouadel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 19:08:13 by obouadel          #+#    #+#             */
/*   Updated: 2022/12/07 12:37:12 by obouadel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(void)
{
	try
	{
		std::multimap<std::string, std::string> nmap;
		std::pair<std::string, std::string> firstip("1336", "127.0.0.1");
		std::pair<std::string, std::string> isip("4242", "127.0.0.1");
		std::pair<std::string, std::string> secondip("1337", "127.0.0.1");
		nmap.insert(firstip);
		nmap.insert(secondip);
		nmap.insert(isip);
		Server server(nmap);
		server.Run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		
	}
	
	return (0);
}