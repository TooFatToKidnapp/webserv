/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouadel <obouadel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 14:21:59 by obouadel          #+#    #+#             */
/*   Updated: 2022/12/07 13:26:06 by obouadel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <netdb.h>
#include <sys/ioctl.h>
#define RUNNING 1

class Server
{
private:
	int estatus;
	std::vector<std::pair<int, addrinfo*> > servers;
public:
	Server(std::multimap<std::string, std::string> &ipport);
	void	Run();
	static std::string receive_data(int sockfd);
	~Server();
};

Server::Server(std::multimap<std::string, std::string> &ipport)
{
	addrinfo hints, *servinfo;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	std::multimap<std::string, std::string>::iterator it =  ipport.begin();
	std::multimap<std::string, std::string>::iterator it2 =  ipport.end();
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
		if (bind(this->servers[i].first, this->servers[i].second->ai_addr, this->servers[i].second->ai_addrlen) == -1)
			throw std::invalid_argument(strerror(errno));
		if (listen(this->servers[i].first, 2) == -1)
			throw std::invalid_argument(strerror(errno));
	}
}

std::string	Server::receive_data(int sockfd)
{
	size_t 				valread = 0;
	size_t 				nbytes = 0;
	ioctl(sockfd, FIONREAD, &nbytes);
	std::vector<char> 	buffer(nbytes);
	std::string			rcv;
	

	valread = recv(sockfd, &buffer[0], nbytes, 0);
	if (valread < 0)
		throw std::invalid_argument("recv: cringe");
	rcv.append(buffer.cbegin(), buffer.cend());
	return (rcv);
}

void	Server::Run()
{
	fd_set	readfds;
	int 	client_socket[30];
	int 	max_clients = 30;
	int		newsockfd;
	int		max_sd;

	std::memset(client_socket, 0, 30 * sizeof(int));
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
					request = Server::receive_data(temp);
					std::cout << "==============STR===============" << std::endl;
					std::cout << request << std::endl;
					std::cout << "==============END===============" << std::endl;
					send(temp, request.c_str(), strlen(request.c_str()), 0);
					close(temp);
					client_socket[i] = 0;
				}
				catch(const std::exception& e)
				{
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