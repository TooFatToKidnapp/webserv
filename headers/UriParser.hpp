/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UriParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:33:20 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/10 20:47:24 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URIPARSER
#define URIPARSER

#include "./Uri.HPP"
#include "./utils.hpp"
#include "./HostStateParser.hpp"

enum HostState {
	h_start,
	h_literal,
	h_Ipv6,
	h_IpvF,
	h_Ipv4,
	h_literalend,
	h_regname,
	h_regnamepct,
	h_regnamepctdone,
	h_port,
	h_done,
	h_invalid,
};

class Uri;

/*
	parsing the host uri string into uri host string
	dose not eccept comma delimited list of multiple hosts "split the str first"
	b4 passing to parser
*/

class UriParser : public HostStatePrser<HostState> {
	public:

		UriParser(): HostStatePrser(h_start, h_done),
		_Uri(NULL),
		_Groops(0),
		_Colons(0),
		_Numbers(0),
		_Lit(false) {}

		~UriParser() {}

		size_t Parse(Uri &uri, std::string const &inpt) {
			_Groops = 0;
			_Colons = 0;
			_Numbers = 0;
			_Lit = false;
			_Uri = &uri;
			return ParseString(inpt);
		}

	private:
		HostState PushBuffer(std::string &field , HostState next) {
			if (!_Buf.empty()) {
				field = _Buf;
				_Buf.clear();
			}
			_SkipChar = true;
			return next;
		}


		HostState LiteralHandler(size_t index);
		HostState RegNameHndler(size_t index);
		HostState IpvFHndler(size_t index);

		// handels IP literals signald by '['
		HostState LiteralEndHndler(size_t index) {
			switch (_Input[index])
			{
			case 'v':
				return h_IpvF;
			case ':':
				return h_Ipv4;
			default:
				if (IsHexDig(_Input[index]))
					return h_Ipv6;
				else
					return h_invalid;
			}
		}
		HostState Ipv6Hndler(size_t index);
		HostState PushBufferHndler(size_t index);
		HostState PortHndler(size_t index);
		HostState Ipv4Hndler(size_t index);
		HostState RegNamePctDoneHndler(size_t index);
		HostState RegNamePctHndler(size_t index);

		// handels transition into IP literal, reg name or ipv4 parsing
		HostState StartHandler(size_t index) {
			switch (_Input[index]) {
				case '\0':
					return PushBuffer(_Uri->_Host, h_done);
				case '[':
					_Lit = true;
					return h_literal;
				case '%':
					return h_regnamepct;
				default:
					if (IsIpv4Format(_Input)) // check if its ipv4 format
						return h_Ipv4;
					else if (IsUnreservedSubDelim(_Input[index]))
						return h_regname;
					else
						return h_invalid;
			}
		}
		Uri *_Uri;
		size_t _Groops; // counting groups of IPV4 sequences
		size_t _Colons; // keep track of colons in ipv6
		size_t _Numbers; // counting nb id digits in a groop
		bool _Lit;



};





#endif
