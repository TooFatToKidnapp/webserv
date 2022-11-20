/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouadel <obouadel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 13:12:56 by obouadel          #+#    #+#             */
/*   Updated: 2022/11/19 18:47:58 by obouadel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

# define MAX_LEN 256

int main(int ac, char *av[])
{
	int sockfd, newsockfd, portno;
	char text[MAX_LEN];
	struct sockaddr_in serv_addr, client_addr;
	socklen_t clilen = sizeof(client_addr);
	int n;
	if (ac < 2)
		return (std::cerr << "ERROR, no port provided" << std::endl, 1);
	sockfd =  socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) 
		return (std::cerr << "socket: " << strerror(errno) << std::endl, 1);
	std::memset(&serv_addr, '\0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;  
	serv_addr.sin_addr.s_addr = INADDR_ANY;  
	serv_addr.sin_port = htons(atoi(av[1]));
	if (bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		return (std::cerr << "bind: " << strerror(errno) << std::endl, 1);
	if (listen(sockfd, 2) == -1)
		return (std::cerr << "listen: " << strerror(errno) << std::endl, 1);
	if ((newsockfd = accept(sockfd, (sockaddr *)&client_addr, &clilen)) == -1)
		return (std::cerr << "accept: " << strerror(errno) << std::endl, 1);
	std::cout << "Connection established with a client, see the client informations below:" << std::endl;
	std::cout << "Client IP: " << inet_ntoa(client_addr.sin_addr) << std::endl;
	std::cout << "Port: " << ntohs(serv_addr.sin_port) << std::endl;
	std::memset(text, 0, MAX_LEN);
	if (recv(newsockfd, text, MAX_LEN, 0) == -1)
		return (std::cerr << "recv: " << strerror(errno) << std::endl, 1);
	if (send(newsockfd, "Message received sucessfully.", 29, 0) == -1)
		return (std::cerr << "send: " << strerror(errno) << std::endl, 1);
	std::cout << "the message sent was: " << text << std::endl;
	close(newsockfd);
	close(sockfd);
	return (0);
}