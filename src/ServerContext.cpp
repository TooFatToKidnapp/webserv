/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:46:12 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/29 10:30:53 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../headers/ServerContext.hpp"

ServerContext::ServerContext(size_t *start, std::string file, size_t ServerId) :
_LocationPos(0),
_IsListening(false),
_CustomServerName(false),
_ServerID(ServerId),
_Listen("80", "0") {
		_LocationContext.push_back(LocationContext());
		_ServerNames.push_back("localhost");


}

ServerContext::ServerContext() :
_LocationPos(0),
_IsListening(false),
_CustomServerName(false),
_ServerID(0),
_Listen("80", "0") {
		_ServerNames.push_back("localhost");

}

ServerContext::~ServerContext() {}

ServerContext::ServerContext(const ServerContext &obj) {
	*this = obj;
}

ServerContext &ServerContext::operator=(const ServerContext& obj) {
	return *this;
}


int ServerContext::GetDerective(std::string const directive) {
	const std::string locations[] = {"location", "listen", "server_name", "root", "index", "client_max_body_size", "error_page", "autoindex", "return"};
	int index = std::find(locations , locations + 9, directive) - locations;
	return index;
}

int ServerContext::IsDerective(std::string const directive) {
	int IsDerective = GetDerective(directive);
	if (IsDerective < 0 || IsDerective > 8)
		throw std::invalid_argument("Error: Invalid Derective");
	else
		return IsDerective;
}

size_t FindVal(int const Directive, std::string ConfigFile, size_t End) {
	std::string input;
	size_t value_end;
	if (Directive == 0) {
		value_end

	}

}


void ServerContext::GetValuePairs(size_t *StartPos, std::string ConfigFile ) {
	size_t I = *StartPos;
	size_t Start = 0;
	size_t End = 0;
	size_t ValueEnd = 0;
	int Derective;

	while (ConfigFile[I] != '}') {
		Start = ConfigFile.find_first_not_of(" \t\n\v\r\f", I);
		if (ConfigFile[Start] == '{' || Start == std::string::npos) {
			I = Start;
			break;
		}
		End = ConfigFile.find_first_of(" \t\n\v\f\r", Start);
		Derective = IsDerective(ConfigFile.substr(Start, End - Start));
		I = FindVal(Derective, ConfigFile, End);


	}
}
