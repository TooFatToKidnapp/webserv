/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:48:53 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/31 15:25:15 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INDEX_HPP
#define INDEX_HPP

#include <vector>
#include <string>


class Index {
	private:
		std::vector<std::string> _Index;

	public:
		Index(std::string value);
		~Index();
		std::vector<std::string> &storIndex(std::string str);
		std::vector<std::string> GetIndex() const;;
};

#endif
