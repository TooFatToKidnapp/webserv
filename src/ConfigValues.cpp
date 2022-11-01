/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValues.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:36:45 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/01 18:15:29 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../headers/ConfigValues.hpp"

ConfigValues::ConfigValues() :
_Root(false),
_Index(false),
_ClientMaxBodySize(false),
_ErrorPage(false),
_AutoIndex(false),
_ReturnValue(false),
_RootPath("/www/html"),
_ClientMaxSize(1),
_ErrorPages(),
_AutoIndexStatus(false) {
	Index input("index.php index.html index.htm index.nginx-debian.html");
	_IndexList = input.GetIndex();
}

ConfigValues::~ConfigValues(){}

ConfigValues& ConfigValues::operator=(const ConfigValues &obj) {
	if (this != &obj) {
		_Root = obj._Root;
		_Index = obj._Index;
		_ClientMaxBodySize = obj._ClientMaxBodySize;
		_ErrorPage = obj._ErrorPage;
		_AutoIndex = obj._AutoIndex;
		_ReturnValue = obj._ReturnValue;
		_RootPath = obj._RootPath;
		_IndexList = obj._IndexList;
		_ClientMaxSize = obj._ClientMaxSize;
		_ErrorPages = obj._ErrorPages;
		_AutoIndexStatus = _AutoIndexStatus;
	}

	return *this;
}

ConfigValues::ConfigValues(const ConfigValues &obj) {
	*this = obj;
}

void ConfigValues::SetRoot(std::string val) {
	_Root = true;
	_RootPath.clear();

}
// void ConfigValues::SetIndex(std::string val);
// void ConfigValues::SetCmbs(std::string val);
// void ConfigValues::SetErrorPage(std::string val);
// void ConfigValues::SetAutoIndexDir(std::string val);
