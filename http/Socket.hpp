/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:37:49 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/11/21 11:52:16 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "codes.hpp"
// #include "Utils.hpp"

class Socket {
private:
    int _port;
    int _socket_fd;
    int _domain;    
    int _type;
    int _protocol;
    struct sockaddr_in _address;
    int addrlen;

    void    init_address();
    void    create_socket();

public:
    Socket();
    Socket(int port, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
 
    static void		testConnection(int connection_ret, std::string customErrMsg = "Connection failed");
    static Socket	acceptConnection(int socket, struct sockaddr_in address, int addrlen);

    int getSocketFd() const;
	struct sockaddr_in getAddress() const;
	int getAddrlen() const;
    void setSocketFd(int socket_fd);

    operator int() const {
        return _socket_fd;
    } 
};