/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 18:33:36 by aabdou            #+#    #+#             */
/*   Updated: 2022/12/13 20:52:16 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "./../http/Request.hpp"

extern char **env;

class CGI {

	public:
		CGI(Request const &, short const &);
		~CGI();
		CGI(const CGI &obj);

		void setEnv();
		void Exec();
		std::string const &GetOutput()const;

	private:
		CGI &operator=(const CGI &obj);
		Request const & _Request;
		std::string _Root;
		std::string _CgiPath;
		std::string _CgiOutput;
		short _Port;

};



#endif

