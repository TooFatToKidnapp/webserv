/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Root.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 12:03:25 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/01 16:03:31 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOT_HPP
#define ROOT_HPP

#include <string>
#include "./../src/utils.cpp"
#include <stdexcept>

class Root {
	private:
		Root(){}
	public:
		Root(std::string input) {
			if (input.compare("") == 0)
				throw std::invalid_argument("Error: Missing Root Argument");
			size_t arg = CountArgs(input);
			if (arg == 1)
				throw std::invalid_argument("Error: Too Many Root Arguments");
		}
		~Root() {}

};

#endif
