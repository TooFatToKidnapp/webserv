/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 10:19:28 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/27 18:26:45 by aabdou           ###   ########.fr       */
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
	else if (ac == 1){
		this->_FileName = "./ConfigFiles/default.conf";
		cout << "Warning: no config file given, the default configuration file is now used" << std::endl;
	}
	else
		throw std::invalid_argument("Error: File Extention Is Invalid");
}

void ConfigFileParser::CheckBrackets() {
	unsigned int i = 0;
	int OpenBrackets = 0;
	int CloseBrackets = 0;

	while (this->_FileContent[i] != '\0'){
		if (this->_FileContent[i] == '{')
			OpenBrackets++;
		else if (this->_FileContent[i] == '}')
			CloseBrackets++;
		i++;
	}
	if (OpenBrackets == CloseBrackets)
		return;
	else
		throw std::invalid_argument("Error: Missing Brackets");
}

// bool ConfigFileParser::CheckServerValidity(std::string file, size_t *pos) {
//
//
// }

void ConfigFileParser::CheckServerBlock() {
	if (_FileContent.find("server") == std::string::npos)
			throw std::invalid_argument("Error: Missing Server Contex");
	size_t i		= 0;
	size_t StartPos	= 0;
	size_t EndPos	= 0;
	std::string tmp  = _FileContent;
	//cout << _FileContent << "\n";

	while (tmp[i]) {
		StartPos = tmp.find("server", i);
		i = StartPos + 6;
		EndPos = tmp.find("}", StartPos);
		// if (EndPos == std::string::npos) {
			// i = StartPos;
			// if (CheckServerValidity(_FileContent.substr(StartPos, EndPos - StartPos), &i))
				// cout << "";
		// }
		// if (i == std::string::npos)
			// i++;
		cout << tmp.substr(StartPos, EndPos+1 - StartPos)<< "\n";
	//break;
	}

}


void ConfigFileParser::ParseFile(int ac, char **av) {
	CheckArgs(ac, av);
	std::ifstream file(this->_FileName);

	if (file.is_open() == false)
		throw std::invalid_argument("Error: Can't Open File");

	std::string line;
	size_t pos;
	while(std::getline(file, line)) {
		if (file.fail())
			throw std::ios_base::failure("Error: faild to read from file");
		pos = line.find("#");
		if (pos != std::string::npos)
			line.erase(pos);
		if (line.empty() == false && line.find_first_not_of(" \n\t\r\v\f") != std::string::npos)
		this->_FileContent.append(TrimContent(line) + "\n");
	}
	file.close();
	if (_FileContent == "")
		throw std::invalid_argument("Error: Empty Config File");
	CheckBrackets();
	//cout << _FileContent;
	//cout << "\n\n\n";
	CheckServerBlock();

	// TODO : (for now)
	// config file must start with a server context, anything outside of a server cintext will be seen as an invalid config file

	// should be able to choose the port and host of each server
	// setup the sever names or not
	// the first server for a host:port is the default host:port (it will answer all the requests that dont velong to an other server)
	// set up default and custom error pages
	// limit client body size ????
	// if (this->_FileContent.find("server") == std::string::npos || this->_FileContent.find("{") == std::string::npos)
	//	throw( std::invalid_argument("Error: No Server Config Found"));
}

