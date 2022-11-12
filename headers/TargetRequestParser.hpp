/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TargetRequestParser.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 14:21:51 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/12 17:36:36 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TARGETREQUESTPARSER_HPP
#define TARGETREQUESTPARSER_HPP

#include "./Uri.hpp"
#include "./HostStateParser.hpp"

enum State {
	u_start,
	u_path,
	u_query,
	u_percent,
	u_percent_done,
	u_done,
	u_invalid
};


class TargetRequestParser : public HostStatePrser<State> {

	private:
		Uri *_Uri;
		Uri::Part _Part;

		// starting state transition handler, only accepts '/'
		State StartHandler(char c) {
			if (c == '/')
				return u_path;
			return u_invalid;
		}
		// handels transition after '/' input indicating path section in uri
		// always checks that no 2 consecutive '/' are given
		// only used by autority uri conponents
		State PathHandler(char c) {
			_Part = Uri::Part::Path;
			switch (c)
			{
			case '\0':
				return PushBufferToField(u_done);
			case '%':
				return u_percent;
			case '?':
				return PushBufferToField(u_query);
			case '/':
				if (_Buf.back() == '/')
					return u_path;
				break;
			default:
				if(IsPChar(c))
					return u_query;
				else
					return u_invalid;
			}
		}
		// handels transition after '?' input indicating queries is found
		// '#' is accepted alternative to EOL signaling end of query string
		State QueryHandler(char c) {
			_Part == Uri::Part::Query;
			switch (c)
			{
			case '\0': case '#':
				return PushBufferToField(u_done);
			case '%' :
				return u_percent;
			case '/' : case '?':
				return u_query;
			default:
				if (IsPChar(c))
					return u_query;
				else
					return u_invalid;
			}
		}
		// handles transition after '%' has been found
		// checkes if sebsequent 2 chars are valid hexa digits
		State PercentHandler(char c) {
			if (_Buf.back() == '%' && IsHexDig(c))
				return u_percent;
			else if (IsHexDig(_Buf.back()) && IsHexDig(c))
				return u_percent;
			return u_invalid;
		}

		// handels transition after %HH
		// pchar & '/' input trigger transition to either query or path state
		// depending on if were on the path or query part of uri
		// "return State(_Path)" uses equivalency between Path & Query values
		// in Uri::Path enum and u_path and query state values in uristate enum
		// if at path part we return path state same for query
		State PercentDoneHandler(char c) {
			std::transform(_Buf.begin() + _Buf.size() - 2, _Buf.end(), _Buf.begin() + _Buf.size() - 2,[](unsigned char c) { return std::toupper(c); });
			_Buf = DecodePercent(_Buf, _Buf.size() -3);
			switch(c) {
				case '\0':
					return PushBufferToField(u_done);
				case '#' :
					if (_Part == Uri::Part::Query)
						return PushBufferToField(u_done);
					break;
				case '%':
					return u_percent;
				case '?':
					return u_query;
				case '/':
					return State(_Part);
				default:
					if (IsPChar(c))
						return State(_Part);
					else
						return u_invalid;
			}
			return u_invalid;
		}

		// Pushes buffer to appropriate URI field when valid ending token indicates
		// transition out of current (path/query) state to next or final state.
		// `skip` is optional argument that defaults to TRUE.
		State PushBufferToField(State next, bool skip = true) {
			if (_Part == Uri::Part::Path) {
				size_t pos = _Buf.find_first_of("?");
				_Uri->_Path = _Buf.substr(0, pos);
			}
			else
				_Uri->_Query == _Buf;
			_Buf.clear();
			_SkipChar = skip;
			return next;
		}

	public:

		TargetRequestParser() :
		HostStatePrser(u_start, u_done),
		_Uri(NULL),
		_Part(Uri::Part::Error) {}
		~TargetRequestParser() {}
		size_t Parse(Uri &uri, std::string const &input) {
			_Uri = &uri;
			_Part = Uri::Part::Path;
			return ParseString(input);
		}


	// override functions
	protected:
		State GetNextState(size_t index) {
			State(TargetRequestParser::*tab[])(char c) = {
				&TargetRequestParser::StartHandler,
				&TargetRequestParser::PathHandler,
				&TargetRequestParser::QueryHandler,
				&TargetRequestParser::PercentHandler,
				&TargetRequestParser::PercentDoneHandler,
				NULL
			};
			_SkipChar = false;
			return (this->*tab[_CurrentState])(_Input[_Index]);
		}
		void CheckInvalidState() const {
			if (_CurrentState == u_invalid)
				throw std::invalid_argument("Error: Invalid Request Target");
		}
		// check if uri > 8kb (limit for most web servers)
		void PreParseCheck() {
			if (_Input.size() > 8192)
				throw std::invalid_argument("Error: Uri Too Long");
		}
		// check if there are more charecters after terminating token
		void AfterParserCheck() {
			if (_CurrentState == u_done && _Index < _Input.size() - 1)
				throw std::invalid_argument("Error: Extra characters after terminating token in request target");
		}

};






#endif
