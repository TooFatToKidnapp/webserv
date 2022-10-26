/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 10:19:28 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/26 18:10:27 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../headers/ConfigFileParser.hpp"

ConfigFileParser::ConfigFileParser() {
	this->_FileName = "";
	this->_FileContent = "";
}

ConfigFileParser::~ConfigFileParser() {}

ConfigFileParser::ConfigFileParser(const ConfigFileParser &obj) {
	*this = obj;
}

ConfigFileParser &ConfigFileParser::operator=(const ConfigFileParser &obj) {
	if (this != &obj) {
		this->_FileName = obj._FileName;
		this->_FileContent = obj._FileContent;
	}
	return *this;
}

std::string ConfigFileParser::TrimContent(std::string str) {
	std::size_t end;
	std::size_t start = str.find_first_not_of("\r\t\f\n\v ");
	if (start != std::string::npos) {
		str = str.substr(start);
		end = str.find_last_not_of("\r\t\f\n\v ");
		if (end != std::string::npos)
			str = str.substr(0, end + 1);
	}
	return str;
}

bool ConfigFileParser::CheckFile(char * FileName) {
	std::string str = FileName;
	str = str.substr(str.rfind(".") + 1);
	if (str == "conf")
		return true;
	return false;
}

void ConfigFileParser::CheckArgs(int ac, char **av) {
	if (ac == 2 && CheckFile(av[1]) == true)
		this->_FileName = av[1];
	else if (ac == 1)
		this->_FileName = "./ConfigFiles/default.conf";
	else
		throw std::invalid_argument("Error: File Extention Is Invalid");
}

void ConfigFileParser::ParseFile(int ac, char **av) {
	CheckArgs(ac, av);
	std::ifstream file(this->_FileName);

	if (file.is_open() == false)
		throw std::invalid_argument("Error: Can't Open File");

	std::string line;
	while(std::getline(file, line)) {
		this->_FileContent.append(TrimContent(line));
		this->_FileContent.append("\n");
	}
	// TODO : (for now)
	// config file must start with a server context, anything outside of a server cintext will be seen as an invalid config file

	// should be able to choose the port and host of each server
	// setup the sever names or not
	// the first server for a host:port is the default host:port (it will answer all the requests that dont velong to an other server)
	// set up default and custom error pages
	// limit client body size ????
	if (this->_FileContent.find("server") == std::string::npos || this->_FileContent.find("{") == std::string::npos)
		throw( std::invalid_argument("Error: No Server Config Found"));



}
