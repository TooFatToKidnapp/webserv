/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 14:40:06 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/17 03:34:46 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/http/Server.hpp"
#include <signal.h>

Server::Server(const std::multimap<std::string, std::string> &ipport)
{
	addrinfo hints, *servinfo;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	std::multimap<std::string, std::string>::const_iterator it =  ipport.begin();
	std::multimap<std::string, std::string>::const_iterator it2 =  ipport.end();
	for (int i = 0; it != it2; ++it, i++) {
		int newsock;
		std::cout << it->first << " " << it->second << "\n";
		if ((this->estatus = getaddrinfo(it->second.c_str(), it->first.c_str(), &hints, &servinfo)) != 0)
			throw std::invalid_argument(gai_strerror(this->estatus));
		if ((newsock = socket(AF_INET, SOCK_STREAM , 0)) == -1)
			throw std::invalid_argument(strerror(errno));
		int optval = 1;
		setsockopt(newsock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
		fcntl(newsock, F_SETFL, O_NONBLOCK);
		std::pair<int, addrinfo *> var;
		var.first = newsock;
		var.second = servinfo;
		this->servers.push_back(var);
		if (bind(this->servers[i].first, (const struct sockaddr*)(this->servers[i].second->ai_addr), this->servers[i].second->ai_addrlen) == -1)
			throw std::invalid_argument(strerror(errno));
		if (listen(this->servers[i].first, 50) == -1)
			throw std::invalid_argument(strerror(errno));
	}
}

std::string	Server::receive_data(int sockfd, int& errnum)
{
	size_t 				valread = 0;
	size_t 				nbytes = 0;
	ioctl(sockfd, FIONREAD, &nbytes);
	std::vector<char> 	buffer(nbytes);
	std::string			rcv;


	valread = recv(sockfd, &buffer[0], nbytes, 0);
	if (valread < 0)
	{
		throw std::invalid_argument("recv: cringe");
	}
	else if (valread == 0)
		errnum = -1;
	else
		rcv.append(buffer.begin(), buffer.end());
	return (rcv);
}

void	Server::Run(ConfigFileParser & conf)
{
	fd_set	readfds;
	int 	client_socket[1024];
	int 	max_clients = 1024;
	int		newsockfd;
	int		max_sd;

	std::memset(client_socket, 0, max_clients * sizeof(int));
	while (RUNNING)
	{
		FD_ZERO(&readfds);
		for (std::vector<std::pair<int, addrinfo*> >::iterator i = servers.begin(); i < servers.end(); i++)
		{
			FD_SET(i->first, &readfds);
			max_sd = i->first;
		}
		for (int i = 0 ; i < max_clients ; i++)
        {
            int sd = client_socket[i];
            if(sd > 0)
                FD_SET(sd, &readfds);
            if(sd > max_sd)
                max_sd = sd;
        }
        if (select(max_sd + 1, &readfds, NULL, NULL, NULL) < 0)
			throw std::invalid_argument("select: error while selecting the fds");
		for (std::vector<std::pair<int, addrinfo*> >::iterator i = servers.begin(); i < servers.end(); i++)
		{
			if (FD_ISSET(i->first, &readfds))
			{
				int id = 0;
				if ((newsockfd = accept(i->first, NULL, NULL)) < 0)
					throw std::invalid_argument("accept: cringe");
				int optval = 1;
				setsockopt(newsockfd, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval));
				while (id < max_clients)
				{
					if(client_socket[id] == 0)
					{
						client_socket[id] = newsockfd;
						break;
					}
				}
				break;
			}
		}
		for (int i = 0; i < max_clients; i++)
        {
            int temp = client_socket[i];
			std::string request;
            if (FD_ISSET(temp, &readfds))
            {
				try
				{
					int errnum = 0;
					request = Server::receive_data(temp, errnum);
					if (errnum == -1)
					{
						close(temp);
						client_socket[i] = 0;
						continue;
					}
					if (request == "\n")
						continue;
					Request req(request, conf);
					Response res(temp, req);
					if (res.getStatus() != OK)
					{
						res.sendErrorPage(res.getStatus());
					}
					else if (!pathIsFile(req.getPath())) {

						res.sendDir(req.getPath().c_str(), req.getHost());
					}
					else
					{

						res.sendFile(req.getPath());
					}
					close(temp);
					client_socket[i] = 0;
				}
				catch(const std::exception& e)
				{
					close(temp);
					client_socket[i] = 0;
					std::cerr << e.what() << '\n';
				}
            }
		}
	}
}

Server::~Server()
{
	std::vector<std::pair<int, addrinfo*> >::iterator it = this->servers.begin();
	for (; it != this->servers.end(); it++)
	{
		if (it->first)
			close(it->first);
		if (it->second)
			freeaddrinfo(it->second);
	}
}
