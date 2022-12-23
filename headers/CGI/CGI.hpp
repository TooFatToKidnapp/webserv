/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 18:33:36 by aabdou            #+#    #+#             */
/*   Updated: 2022/12/20 22:05:35 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "./../http/Request.hpp"

class CGI {

	public:
		CGI(Request const &, short const &);
		~CGI();
		CGI(const CGI &obj);

		void setEnv();
		void Exec();
		std::string &GetOutput();
		std::string const &GetExtention() const{
			return _ScriptExtension;
		}

	private:
		std::string ParsePath(std::string input) {
			size_t pos = input.find_last_of("/");
			pos++;
			std::string tmp = input.substr(pos);
			if (tmp.compare("php-cgi") == 0)
				return "../" + tmp;
			else if (tmp.compare("python") == 0)
				return "/usr/bin/python";
			throw std::invalid_argument("Error: Bad CGI");
		}
		std::string ParseScriptName(std::string input) {
			if(input.compare("") == 0)
				throw std::invalid_argument("Error: Bad Querry");
			size_t pos = input.find_last_of("/");
			pos++;
			return input.substr(pos);
		}
		std::string GetScriptExtention(std::string input) {
			size_t pos = input.find_last_of("/");
			pos++;
			std::string tmp = input.substr(pos);
			if (tmp != "python" || tmp != "php-cgi")
				throw std::invalid_argument("Error: CGI Extention Not Supported");
			return tmp;
		}
		CGI &operator=(const CGI &obj);
		Request const & _Request;
		short _Port;
		char *env[14];
		std::string _ScriptName;
		std::string _ScriptExtension;
		std::string _Root;
		std::string _CgiPath;
		std::string _CgiOutput;

};

#endif

