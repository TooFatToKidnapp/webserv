/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:48:56 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/31 15:26:22 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../headers/Index.hpp"

Index::Index(std::string val) {
	if (val.compare("") == 0)
		throw std::invalid_argument("Error: Bad Index");
	_Index = StorIndex(val);
}

std::vector<std::string> &StorIndex(std::string str) {
	size_t start = 0;
	size_t end = 0;

	std::string key;
	std::vector<std::string> vec;

	for (std::string::size_type i = 0; i < str.size(); i++) {
		start = str.find_first_not_of(" \t\n\r\f\v", i);
		if (start == std::string::npos)
			break;
		end = str.find_first_of(" \t\n\r\f\v", start);
		key = str.substr(start, end - start);
		vec.push_back(key);
		if (end == std::string::npos)
			break;
		i = end;
	}
	return vec;
}

std::vector<std::string> Index::GetIndex() const {
	return _Index;
}
