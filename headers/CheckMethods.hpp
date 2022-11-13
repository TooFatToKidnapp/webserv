/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckMethods.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 15:03:05 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/13 15:36:57 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKMETHODS_HPP
#define CHECKMETHODS_HPP

#include <string>
#include <vector>
#include <exception>
#include "./utils.hpp"


class CheckMethods {
	public:

	CheckMethods():
	_Get(false),
	_Post(false),
	_Delete(false){}

	CheckMethods(std::string str):
	_Get(false),
	_Post(false),
	_Delete(false) {
		if (str.compare("") == 0)
			throw std::invalid_argument("Error: Missing Method Arguments");
		size_t args = CountArgs(str);
		if (args > 3)
			throw std::invalid_argument("Error: Too Many method Arguments");
		_Methods = ToVectorOfStrings(str);
		ValidateMethods();
	}
	~CheckMethods() {}
	CheckMethods &operator=(const CheckMethods &obj) {
		if (this != &obj) {
			_Get = obj._Get;
			_Post = obj._Post;
			_Delete = obj._Delete;
			_Methods = obj._Methods;
		}
		return *this;
	}
	CheckMethods(const CheckMethods &obj):
	_Get(obj._Get),
	_Post(obj._Post),
	_Delete(obj._Delete),
	_Methods(obj._Methods) {}

	bool GetGET() const {
		return _Get;
	}
	bool GetPost() const {
		return _Post;
	}
	bool GetDelete() const {
		return _Delete;
	}
	std::vector<std::string> GetMethods() const {
		return _Methods;
	}


	private:
		bool _Get;
		bool _Post;
		bool _Delete;
		std::vector<std::string> _Methods;

		void ValidateMethods() {
			for (size_t i = 0; i < _Methods.size(); i++) {
				if (_Methods.at(i).empty())
					break;
				switch (IsValidHttpMethod(_Methods.at(i)))
				{
				case 0:
					_Get = true;
					break;
				case 1:
					_Post = true;
					break;
				case 2:
					_Delete = true;
					break;
				}
			}
		}

};





#endif
