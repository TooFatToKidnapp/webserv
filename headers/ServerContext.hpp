/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:49:22 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/04 15:22:05 by aabdou           ###   ########.fr       */
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
#include "./ConfigValues.hpp"

class ServerContext : public ConfigValues {
	private:
		size_t _LocationPos;
		bool _IsListening;
		bool _CustomServerName;
		size_t _ServerID;
		std::pair<std::string, std::string> _Listen;
		std::vector<std::string> _ServerNames;
		std::vector<LocationContext> _LocationContext;

		void GetValuePairs(size_t *StartPos, std::string ConfigFile );
		int GetDirective(std::string const directive);
		size_t FindVal(int Directive, std::string ConfigFile, size_t End);
		size_t	FindLocationContextEnd(std::string ConfigFile, size_t Pos);
		bool HasContent(char EndChar, size_t EndPos, size_t EndValue, std::string ConfigFile);
		bool HasLocation(std::string target);
		int IsDirective(std::string const directive);
		void SetValue(int directive, std::string value); // override func


		void SetServerListen(std::string val);
		void SetServerLocation(std::string val);
		void SetServerName(std::string val);
		void SetServerRoot(std::string val);
		void SetServerIndex(std::string val);
		void SetServerCMBS(std::string val);
		void SetServerErrorPage(std::string val);
		void SetServerAutoIndexDir(std::string val);
		void SetServerReturn(std::string val);
		std::string Trim(std::string value);
		void GetDirectiveValuePair(size_t *pos, std::string file);

	public:
		ServerContext(size_t *start, std::string file, size_t ServerId);
		~ServerContext();
		ServerContext();
		ServerContext(const ServerContext &obj);
		ServerContext &operator=(const ServerContext& obj);

		bool IsSet(std::string directive);



};




#endif
