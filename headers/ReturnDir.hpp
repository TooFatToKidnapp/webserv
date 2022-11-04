/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReturnDir.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 12:03:28 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/04 13:06:14 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RETURNDIR_HPP
#define RETURNDIR_HPP

#include <string>
#include "./utils.hpp"

class ReturnDir {

	private:
		bool _Is_Set;
		int _Code;
		std::string _Url;
		bool ValidReturnCode(size_t code){
			if (code < 0 || code > 999)
				return false;
			return true;
		}
		void FillReturn(std::string input) {
			size_t i = 0;
			size_t start = 0;
			size_t end = 0;
			size_t set = 0;
			std::string key;

			while (input[i]) {
				start = input.find_first_not_of(" \n\r\t\v\f", i);
				if (start == std::string::npos)
					break;
				end = input.find_first_of(" \t\v\n\r\f", start);
				key = input.substr(start, end - start);
				SetVariables(key, &set);
				if (end == std::string::npos)
					break;
				i = end;
			}
		}
		void SetReturnCode(std::string key) {
			_Code = StringToSize_T(key);
			if (!ValidReturnCode(_Code))
				throw std::invalid_argument("Error: Invalid Return Code");
		}
		void SetVariables(std::string key, size_t *set) {
			if (IsNumber(key) == false && *set == 0)
				throw std::invalid_argument("Error: Invalid Return Code");
			if (*set == 0) {
				SetReturnCode(key);
				*set += 1;
			}
			else if (*set > 1)
				throw std::invalid_argument("Error: Too Many Return Values");
			else {
				_Url = key;
				*set += 1;
			}
		}


	public:
		ReturnDir() : _Is_Set(false) {}
		ReturnDir(std::string input) : _Is_Set(false) {
			if (input.compare("") == 0)
				throw std::invalid_argument("Error: Missing return argument");
			FillReturn(input);
		}
		ReturnDir(ReturnDir const &obj): _Is_Set(obj._Is_Set), _Code(obj._Code), _Url(obj._Url) {}
		ReturnDir &operator=(const ReturnDir &obj) {
			if (this != &obj) {
				_Is_Set = obj._Is_Set;
				_Code = obj._Code;
				_Url = obj._Url;
			}
			return *this;
		}
		~ReturnDir() {}

		int GetCode() const {
			return _Code;
		}
		std::string GetUrl()const {
			return _Url;
		}
		bool IsSet() const {
			return _Is_Set;
		}

};

#endif
