/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:37:51 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/11/21 11:37:52 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket() {}

Socket::Socket(int port, int domain, int type, int protocol) {
    _port = port;
    _domain = domain;    
    _type = type;
    _protocol = protocol;
    init_address();
    create_socket();
}

void	Socket::init_address() {
    addrlen = sizeof(_address);
    _address.sin_family = _domain;
    _address.sin_port = htons( _port );
    memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
}

void	Socket::create_socket() {
    _socket_fd = socket(_domain, _type, _protocol);
    testConnection(!_socket_fd ? -1 : _socket_fd, "could not create a new socket");

    int connection = bind(_socket_fd, (struct sockaddr *)&_address, addrlen);
    testConnection(connection, "could not bind the socket");

    connection = listen(_socket_fd, 10);
    testConnection(connection, "could not listen");
}

void	Socket::testConnection(int connection_ret, std::string customErrMsg) {
    if (connection_ret < 0) {
        perror(customErrMsg.c_str());
        exit(1);
    }
}

Socket	Socket::acceptConnection(int socket, struct sockaddr_in address, int addrlen) {
    Socket new_socket;
    int new_connection;

    new_connection = accept(socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    testConnection(new_connection, "could not accept new connection");

    new_socket.setSocketFd(new_connection);

    return new_socket;
}

struct sockaddr_in Socket::getAddress() const { return _address; }
int Socket::getAddrlen() const { return addrlen; }
int Socket::getSocketFd() const{ return _socket_fd; }
void Socket::setSocketFd(int socket_fd){ _socket_fd = socket_fd;}