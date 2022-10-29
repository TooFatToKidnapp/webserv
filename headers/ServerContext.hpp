/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:49:22 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/29 10:27:49 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONTEXT_HPP
#define SERVERCONTEXT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstddef> // size_t
#include <stdlib.h> // exit()
#include <utility> // std::pair
#include <vector>
#include "./LocationContext.hpp"

class ServerContext {
	private:
		size_t _LocationPos;
		bool _IsListening;
		bool _CustomServerName;
		size_t _ServerID;
		std::pair<std::string, std::string> _Listen;
		std::vector<std::string> _ServerNames;
		std::vector<LocationContext> _LocationContext;

		void GetValuePairs(size_t *StartPos, std::string ConfigFile );
		int GetDerective(std::string const directive);
		int IsDerective(std::string const directive);
		size_t FindVal(int const Directive, std::string ConfigFile, size_t End);

	public:
		ServerContext(size_t *start, std::string file, size_t ServerId);
		~ServerContext();
		ServerContext();
		ServerContext(const ServerContext &obj);
		ServerContext &operator=(const ServerContext& obj);





};




#endif
