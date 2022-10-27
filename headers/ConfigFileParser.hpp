/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:39:49 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/27 13:38:10 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
#define PARSING_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstddef> // size_t
#include <stdlib.h> // exit()

using std::cout;
using std::cerr;

class ConfigFileParser {

	private:
		std::string _FileName;
		std::string _FileContent;
	public:
		ConfigFileParser();
		ConfigFileParser(const ConfigFileParser &obj);
		~ConfigFileParser();
		ConfigFileParser &operator=(const ConfigFileParser &obj);

		void ParseFile(int ac, char **av);
		void CheckArgs(int ac, char **av);
		bool CheckFile(char *FileName);
		std::string TrimContent(std::string str);
		void CheckBrackets();
};




#endif

