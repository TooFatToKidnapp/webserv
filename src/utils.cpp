/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:54:56 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/01 16:02:52 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>

size_t StringToSize_T (std::string str) {
	size_t number = 0;
	size_t i = str.find_first_not_of(" \n\t\r\v\f");
	if (str[i] == '-' || str[i] == '+') {
		if (str[i] == '-')
			throw std::invalid_argument("Error: Negative Number");
		i++;
	}
	while (std::isdigit(str[i]) && str[i]) {
		number = (number * 10) + (str[i] - '0');
		i++;
	}
	return number;
}

size_t CountArgs(std::string val) {
	size_t i = 0;
	size_t start = 0;
	size_t end = 0;
	size_t args = 0;

	while (val[i]) {
		start = val.find_first_not_of(" \n\t\r\v\f", i);
		if (start == std::string::npos)
			return args;
		end = val.find_first_of(" \n\t\v\f\r", start);
		args++;
		if (end == std::string::npos)
			break;
		i = end;
	}
	return args;
}

bool IsNumber(std::string input) {
	size_t i = 0;
	while (input[i])
	{
		if (std::isdigit(input[i]) == 0)
			return false;
		i++;
	}
	return true;
}
