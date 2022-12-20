/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 14:21:59 by obouadel          #+#    #+#             */
/*   Updated: 2022/12/20 14:04:40 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codes.hpp"
#include "Utils.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Server
{
private:
	std::vector<std::pair<int, sockaddr_in> > servers;
public:
	Server(const std::multimap<std::string, std::string> &ipport);
	void Run(ConfigFileParser & conf);
	static std::string receive_data(int sockfd, int &errnum);
	~Server();
};
