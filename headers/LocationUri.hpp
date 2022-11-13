/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUri.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:56:00 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/13 19:30:57 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONURI_HPP
#define LOCATIONURI_HPP

#include "./LocationContext.hpp"
#include "./Uri.hpp"
#include "./utils.hpp"
//https://www.f5.com/pdf/white-papers/http-fundamentals-wp.pdf
class LocationUri {
	private:
		Uri _Uri;
		bool _IsDir;

	public:
		LocationUri(): _Uri("/"), _IsDir(true) {}
		LocationUri(std::string str) : _Uri(str) {
			if (str.compare("") == 0)
				throw std::invalid_argument("Error: Empty Uri");
			if (IsUri(str) == false)
				throw std::invalid_argument("Error: invalid Uri");
			_IsDir = IsDirectory(str);
		}

		~LocationUri() {}
		LocationUri(const LocationUri &obj): _Uri(obj._Uri), _IsDir(obj._IsDir) {}
		LocationUri &operator=(LocationUri const &obj) {
			if (this != &obj) {
				_Uri = obj._Uri;
				_IsDir = obj._IsDir;
			}
			return *this;
		}
		std::string GetUri()const {
			return _Uri.GetParsedURI();
		}
		Uri GetUriOBJ() const {
			return _Uri;
		}

		std::string GetInputURI() const {
			return _Uri.GetInputURI();
		}
		bool IsDir() const {
			return IsDir;
		}

};




#endif
