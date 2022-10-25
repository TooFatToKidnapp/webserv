/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 10:19:28 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/25 10:57:09 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../headers/ConfigFileParser.hpp"

ConfigFileParser::ConfigFileParser() {
	this->_FileName = "";
}

ConfigFileParser::~ConfigFileParser() {}

ConfigFileParser::ConfigFileParser(const ConfigFileParser &obj) {
	*this = obj;
}

ConfigFileParser &ConfigFileParser::operator=(const ConfigFileParser &obj) {
	if (this != &obj) {
		this->_FileName = obj._FileName;
	}
	return *this;
}

void ConfigFileParser::ParseFile(int ac, char **av) {
	CheckArgs(ac, av);

}

bool ConfigFileParser::CheckFile(std::string FileName) {
	if (FileName != "") {
		std::string str = FileName.substr(FileName.rfind(".") + 1);
		if (str == "conf")
			return true;
	}
		return false;
}

void ConfigFileParser::CheckArgs(int ac, char **av) {
	if (ac == 2 && CheckFile(av[1]))
		this->_FileName = av[1];
	else {
		if (ac != 2) {
			throw std::invalid_argument("Error: Wrong Number Of Arguments");
			return;
		}
		throw std::invalid_argument("Error: File Extention Is Invalid");
	}
}


