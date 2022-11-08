/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UriParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:33:20 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/08 13:42:04 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URIPARSER
#define URIPARSER

#include "./headers/Uri.HPP"


enum HostState{
	h_start,
	h_literal,
	h_ipv6,
	h_ipv4,
	h_literalend,
	h_regname,
	h_regnamepct,
	h_regnamepctdone,
	h_port,
	h_done,
	g_invalid,
};

class Uri;

/*
	parsing the host uri string into uri host string
	dose not eccept comma delimited list of multiple hosts "split the str first"
	b4 passing to parser
*/

class UriParser {




};





#endif
