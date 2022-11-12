/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uri.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:58:36 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/12 17:34:57 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URI_HPP
#define URI_HPP

#include"./UriParser.hpp"
#include <string>

class Uri {
	friend class UriParser;
	friend class TargetRequestParser;
	public:
		enum class Part {
			Error = -1,
			Host = 0,
			Path = 1,
			Query = 2
		};
		Uri() {}
		Uri(std::string const &str) :
		_UriInput(str) {
			ParseInput();
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
		std::string const&	GetHost() const {
			return _Host;
		}

	private:
		// parses URI depending on staring charecter of input str
		// part uesd to specify which parser to use
		void ParseInput(Part val = Part::Error) {
			size_t pos = _UriInput.find('/');

			// if no '/' found assume its a host string
			if (val == Part::Host || pos == std::string::npos) {
				UriParser parser;
				parser.Parse(*this, _UriInput);
			}
			else if (val == Part::Path || pos == 0) {

			}
		}

		std::string ConstructParsedUri();


		std::string _UriInput;
		std::string _UriParsed;
		std::string _Host;
		std::string _Port;
		std::string _Path;
		std::string _Query;
};





#endif
