/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 19:56:02 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/11 20:30:10 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTEN_HPP
#define LISTEN_HPP

#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>

class Listen {
	private:
		std::string _Listen;
		std::string _PortNumber;
		std::string _Ip;

		Listen() {}
		Listen(const Listen &obj) {}
		Listen &operator=(const Listen &obj) {}

	public:
		Listen(std::string str) :
		_Listen(str),
		_PortNumber("80"),
		_Ip("127.0.0.1") {
			if (str.empty()) {
				return;
			}
			SplitPortIp();
		}
		void SplitPortIp() {
			size_t pos;
			pos = _Listen.find_last_of(":");
			if (pos == _Listen.size() - 1) {
				if (pos == 0 )
					throw std::invalid_argument("Error: Invalid Ip");
				throw std::invalid_argument("Error: Invalid Port");
			}
			else if (pos == 0)
					throw std::invalid_argument("Error: Invalid Ip");
			else if (pos == std::string::npos || (_Listen.front() == '[' && _Listen.back() == ']')) {
				if (_Listen.find('.') != std::string::npos || _Listen.find(':') != std::string::npos
					|| _Listen.find("localhost") != std::string::npos || _Listen.find("*") != std::string::npos
					|| _Listen.find('[') != std::string::npos) {
						_PortNumber = "80";
						CheckIpAddress(_Listen);
					}
					else {
						_Ip = "127.0.0.1";
						CheckPortNb(_Listen);
					}
			}
			else {
				CheckIpAddress(_Listen.substr(0, pos));
				CheckPortNb(_Listen.substr(pos + 1, _Listen.size()));
			}
		}
		void CheckPortNb(std::string port) {
			if (port.find_first_not_of("0123456789") != std::string::npos)
				throw std::invalid_argument("Error: Invalid Port Number");
			else if (std::stoi(port) > 65535 || std::stoi(port) <= 0)
				throw std::invalid_argument("Error: Invalid Port Number");
			else if (port.front() == '0')
				throw std::invalid_argument("Error: Invalid Port Number");
			_PortNumber = port;
		}
		bool CheckValidAddress(std::string ip) {
			char buf[INET6_ADDRSTRLEN];
			int domain;

			if (ip.find("[") != std::string::npos) {
				domain = AF_INET6;
				ip = ip.substr(1, ip.size() - 2);
			}
			else
				domain = AF_INET;
			if (inet_pton(domain, ip.c_str(), buf) <= 0)
				return false;
			return true;
		}
		void CheckIpAddress(std::string ip) {
			if (ip.compare("localhost") == 0)
				_Ip = "127.0.0.1";
			else if (ip == "0" || ip == "*")
				_Ip = "127.0.0.1";
			else {
				if (CheckValidAddress(ip) == false)
					throw std::invalid_argument("Error: Invalid Ip Adress");
				_Ip = ip;
			}
		}
		std::string GetPortNb() {
			return _PortNumber;
		}
		std::string GetIpNb() {
			return _Ip;
		}


};




#endif
