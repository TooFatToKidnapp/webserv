/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 14:40:06 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/26 18:32:10 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/http/Server.hpp"
#include <limits.h>

Server::Server(const std::multimap<std::string, std::string> &ipport)
{
	std::multimap<std::string, std::string>::const_iterator it =  ipport.begin();
	std::multimap<std::string, std::string>::const_iterator it2 =  ipport.end();
	for (int i = 0; it != it2; ++it, i++) {
		int newsock;
		struct sockaddr_in servinfo;
		std::memset(&servinfo, (unsigned char)0, sizeof(sockaddr_in));
		std::cout << it->first << " " << it->second << "\n";
		servinfo.sin_family = AF_INET;
    	servinfo.sin_port = htons(atoi(it->first.c_str()));
		servinfo.sin_addr.s_addr = htonl(INADDR_ANY);
		if ((newsock = socket(AF_INET, SOCK_STREAM , 0)) == -1)
			throw std::invalid_argument(strerror(errno));
		int optval = 1;
		setsockopt(newsock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
		fcntl(newsock, F_SETFL, O_NONBLOCK);
		std::pair<int, sockaddr_in> var;
		var.first = newsock;
		var.second = servinfo;
		this->servers.push_back(var);
		if (bind(this->servers[i].first, (const sockaddr *)&this->servers[i].second, sizeof(this->servers[i].second)) < 0)
			throw std::invalid_argument(strerror(errno));
		if (listen(this->servers[i].first, 50) == -1)
			throw std::invalid_argument(strerror(errno));
	}
}

bool    in_stock(int sockfd, std::vector<std::pair<int,std::string> >& stock)
{
    for (std::vector<std::pair<int,std::string> >::iterator i = stock.begin(); i < stock.end(); i++)
    {
        if (i->first == sockfd)
            return (true);
    }
    return (false);
}

std::string    Server::receive_data(int sockfd, int& errnum)
{
    static std::vector<std::pair<int,std::string> > stock_rcv;
    int						valread = 0;
    size_t					nbytes = 0;
    ioctl(sockfd, FIONREAD, &nbytes);
    std::vector<char>		buffer(nbytes);
    std::string				rcv;
	
	valread = recv(sockfd, &buffer[0], nbytes, 0);
    if (valread < 0)
        throw std::invalid_argument("recv: cringe");
    else if (valread == 0)
        errnum = -1;
    else
    {
        if (in_stock(sockfd, stock_rcv) == true)
        {
            for (std::vector<std::pair<int,std::string> >::iterator i = stock_rcv.begin(); i < stock_rcv.end(); i++)
            {
                if (i->first == sockfd)
                {
                    i->second.append(buffer.begin(), buffer.end());
                    size_t pos = i->second.find("\r\n\r\n");
                    size_t contLenPos = i->second.find("Content-Length: ");
                    size_t bodyLen = i->second.substr(pos + 4).size();
                    int contLen = atoi(i->second.substr(contLenPos + 16, i->second.find("\r\n", contLenPos)).c_str());
                    if (contLen > (int)bodyLen)
                        return ("\n");
                    else
                    {
                        rcv = i->second;
                        stock_rcv.erase(i);
                        return (rcv);
                    }
                }
            }
        }
        rcv.append(buffer.begin(), buffer.end());
        size_t pos = rcv.find("\r\n\r\n");
        size_t contLenPos = rcv.find("Content-Length: ");
        if (pos == std::string::npos || contLenPos == std::string::npos)
            return (rcv);
        size_t bodyLen = rcv.substr(pos + 4).size();
        int contLen = atoi(rcv.substr(contLenPos + 16, rcv.find("\r\n", contLenPos)).c_str());
        if (contLen > (int)bodyLen)
        {
            std::pair<int, std::string> stock;
            stock.first = sockfd;
            stock.second = rcv;
            stock_rcv.push_back(stock);
            return ("\n");
        }
    }
    return (rcv);
}

void	Server::Run(ConfigFileParser & conf)
{
	fd_set	readfds;
	fd_set	writefds;
	int 	client_socket[FD_SETSIZE];
	int 	max_clients = FD_SETSIZE;
	std::vector<std::string> response(max_clients);
	std::vector<size_t>	chunksNbr(max_clients);
	int		newsockfd;
	int		max_sd;

	std::memset(client_socket, 0, max_clients * sizeof(int));
	while (RUNNING)
	{
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		for (std::vector<std::pair<int, sockaddr_in> >::iterator i = servers.begin(); i < servers.end(); i++)
		{
			FD_SET(i->first, &readfds);
			max_sd = i->first;
		}
		for (int i = 0 ; i < max_clients ; i++)
        {
            int sd = client_socket[i];
            if(sd > 0)
			{
                FD_SET(sd, &readfds);
                FD_SET(sd, &writefds);
			}
            if(sd > max_sd)
                max_sd = sd;
        }
        if (select(max_sd + 1, &readfds, &writefds, NULL, NULL) < 0)
			throw std::invalid_argument("select: error while selecting the fds");
		for (std::vector<std::pair<int, sockaddr_in> >::iterator i = servers.begin(); i < servers.end(); i++)
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
					id++;
				}
				break;
			}
		}
		for (int i = 0; i < max_clients; i++)
        {
            int temp = client_socket[i];
			std::string request;
			try
			{
            	if (FD_ISSET(temp, &readfds))
            	{
					int errnum = 0;
					request = Server::receive_data(temp, errnum);
					if (errnum == -1)
					{
						response[i].clear();
						close(temp);
						client_socket[i] = 0;
						continue;
					}
					if (request == "\n")
						continue;
					Request req(request, conf);
					Response res(req);
					if (res.getStatus() != OK)
						response[i] = res.sendErrorPage(res.getStatus());
					else if (req.GetMethod() == "POST" && request.find("Content-Disposition") != std::string::npos)
						response[i] = res.uploadFile();
					else if (req.GetMethod() == "DELETE")
						response[i] = res.deleteFile(req.getPath());
					else if (!pathIsFile(req.getPath())) {
						if (req.GetLocation().GetCGI().GetFilePath().compare("") != 0)
								response[i] = res.cgi(req);
						else
							response[i] = res.sendDir(req.getPath().c_str(), req.getHost());
					}
					else
						response[i] = res.sendFile(req.getPath());
					if (res.getStatus() == OK)
						chunksNbr[i] = getFileLength(req.getPath()) / 10;
					else
						chunksNbr[i] = 100;
					if (chunksNbr[i] > 150000)
						chunksNbr[i] = 150000;
				}
				if (FD_ISSET(temp, &writefds))
				{
					if (response[i] != "")
					{
						int valsent = 0;
						size_t c = chunksNbr[i];
						if (response[i].size() < chunksNbr[i])
							c = response[i].size();
						valsent = send(temp, response[i].c_str(), c, 0);
						if (valsent == 0)
							continue;
						else if (valsent < 0)
							throw std::runtime_error("couldn't send the response");
						response[i].assign(response[i].begin() + c, response[i].end());
						if (response[i] == "")
						{
							close(temp);
							client_socket[i] = 0;
						}
					}
				}
            }
			catch(const std::exception& e)
			{
				close(temp);
				client_socket[i] = 0;
				response[i].clear();
				std::cerr << e.what() << std::endl;
			}
		}
	}
}

Server::~Server()
{
	std::vector<std::pair<int, sockaddr_in> >::iterator it = this->servers.begin();
	for (; it != this->servers.end(); it++)
	{
		if (it->first)
			close(it->first);
	}
}
