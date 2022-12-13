/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 14:21:59 by obouadel          #+#    #+#             */
/*   Updated: 2022/12/13 15:27:18 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codes.hpp"
#include "Utils.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Server
{
private:
	int estatus;
	std::vector<std::pair<int, addrinfo*> > servers;
public:
	Server(std::multimap<std::string, std::string> &ipport);
	void	Run(ConfigFileParser & conf);
	static std::string receive_data(int sockfd, int &errnum);
	~Server();
};