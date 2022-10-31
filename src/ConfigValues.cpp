/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValues.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:36:45 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/31 15:27:15 by aabdou           ###   ########.fr       */
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

ConfigValues::~ConfigValues(){

}

ConfigValues& ConfigValues::operator=(const ConfigValues &obj) {
	if (this != &obj) {

	}

	return *this;
}

ConfigValues::ConfigValues(const ConfigValues &obj) {
	*this = obj;
}
