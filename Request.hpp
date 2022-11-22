/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:32:03 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/11/22 16:21:36 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "codes.hpp"

class Request {
private:
public:
	int									_Status;
	std::string 						_Buffer;
	std::string							_Method;
	std::string							_Path;
	std::string							_Query;
	std::string							_HttpVersion;
	std::map<std::string, std::string>	_Headers;
	std::map<std::string, std::string>	_QueryMap;
	std::vector<std::string>			_Body;

	Request(std::string & buffer);
	~Request();

	void RequestParsing();
	void ParseStartLine(std::string & str);
	void ParseHeaders(std::vector<std::string> & headers);
	void ParseChunckedBody(std::string &buffer);
	void ParseBody(std::string &buffer);
	void ParseQuery(std::string & query);
};
