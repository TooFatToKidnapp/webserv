/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uri.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:58:36 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/12 18:48:14 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URI_HPP
#define URI_HPP

#include"./UriParser.hpp"
#include "./TargetRequestParser.hpp"
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
		explicit Uri(std::string const &str) :
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
		Uri &operator=(std::string const & str){
			_Host.clear();
			_Port.clear();
			_Path.clear();
			_Query.clear();
			_UriInput = str;
			ParseInput();
			return *this;
		}

		std::string const&	GetHost() const {
			return _Host;
		}
		std::string const &GetPath() const {
			return _Path;
		}
		std::string const &GetQuery() const {
			return _Query;
		}
		std::string const & Get()const {
			return _UriParsed;
		}
		std::string const & GetPort()const {
			return _Port;
		}
		std::string const & GetParsedURI()const {
			return _UriParsed;
		}
		std::string const & GetInputURI()const {
			return _UriInput;
		}
		std::string GetHostWithPort()const {
			if (_Port.empty())
				return _Host;
			else
				return _Host + ":" + _Port;
		}

		void SetHost(std::string const &str) {
			_UriInput = str;
			ParseInput(Part::Host);
		}
		void SetPath(std::string const &str) {
			_UriInput = str;
			ParseInput(Part::Path);
		}
		void SetQuery(std::string const &str) {
			_Query = str;
			_UriParsed = ConstructParsedUri();
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
			// if starts with '/' assumes its origin from uri
			else if (val == Part::Path || pos == 0) {
				TargetRequestParser parser;
				parser.Parse(*this, _UriInput);
			}
			else
				throw std::invalid_argument("Error: Bad Uri Format");
			_UriParsed = ConstructParsedUri();
		}

		std::string ConstructParsedUri() {
			std::string uri;
			if (_Host.empty() == false)
				uri += GetHostWithPort();
			if (_Path.empty() == false)
				uri += GetPath();
			if (_Query.empty() == false)
				uri += "?" + GetQuery();
			return uri;
		}

		std::string _UriInput;
		std::string _UriParsed;
		std::string _Host;
		std::string _Port;
		std::string _Path;
		std::string _Query;
};





#endif
