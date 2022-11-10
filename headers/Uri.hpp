/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uri.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:58:36 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/10 18:45:42 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URI_HPP
#define URI_HPP

#include <string>

class Uri {
	private:
		std::string _UriInput;
		std::string _UriParsed;
		std::string _Host;
		std::string _Port;
		std::string _Path;
		std::string _Query;

	public:
		enum class Part {
			Error = -1,
			Host = 0,
			Path = 1,
			Query = 2
		};
		Uri() {}
		Uri(std::string const &str) {

		}
		~Uri() {}
		Uri(const Uri &obj) {
			*this = obj;
		}
		Uri &operator=(const Uri &obj) {
			if (this != &obj) {
				this->_UriInput = obj._UriInput;
				this->_UriParsed = obj._UriParsed;
				this->_Host = obj._Host;
				this->_Port = obj._Port;
				this->_Path = obj._Path;
				this->_Query = obj._Query;
			}
			return *this;
		}
		// parses URI depending on staring charecter of input str
		// part uesd to specify which parser to use
		void ParseInput(Part val = Part::Error) {
			size_t pos = _UriInput.find('/');

			// if no '/' found assume its a host string
			if (val == Part::Host || pos == std::string::npos) {

			}
		}
		std::string const&	GetHost() const {
			return _Host;
		}

};





#endif
