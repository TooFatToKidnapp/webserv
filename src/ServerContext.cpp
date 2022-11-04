/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:46:12 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/04 17:47:06 by aabdou           ###   ########.fr       */
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
		GetDirectiveValuePair(start, file);
		// if ()

}

// bool ServerContext::HasLocation(std::string target) {
// 	for (size_t i = 0; i < _LocationContext.size() ; i++) {
// 		if (target.compare(_LocationContext.at(i).))
// 	}
// }


void ServerContext::GetDirectiveValuePair(size_t *pos , std::string file) {
	size_t i = *pos;
	size_t start = 0;
	size_t end = 0;
	size_t ValEnd = 0;
	int directive;

	while (file[i] != '}') {
		start = file.find_first_not_of(" \n\t\f\v\r", i);
		if (start == std::string::npos || file[start] == '}') {
			i = start;
			break;
		}
		end = file.find_first_of(" \n\t\v\f\r", start);
		directive = IsDirective(file.substr(start, end - start));
		i = FindVal(directive, file, end);
		if (end != std::string::npos)
			i++;
	}
	*pos = i;
}

ServerContext::ServerContext() :
_LocationPos(0),
_IsListening(false),
_CustomServerName(false),
_ServerID(0),
_Listen("80", "0") {
		_LocationContext.push_back(LocationContext());
		_ServerNames.push_back("localhost");
}

ServerContext::~ServerContext() {}

ServerContext::ServerContext(const ServerContext &obj) {
	*this = obj;
}

ServerContext &ServerContext::operator=(const ServerContext& obj) {
	return *this;
}


int ServerContext::GetDirective(std::string const directive) {
	const std::string locations[] = {"location", "listen", "server_name", "root", "index", "client_max_body_size", "error_page", "autoindex", "return"};
	int index = std::find(locations , locations + 9, directive) - locations;
	return index;
}

int ServerContext::IsDirective(std::string const directive) {
	int IsDerective = GetDirective(directive);
	if (IsDerective < 0 || IsDerective > 8)
		throw std::invalid_argument("Error: Invalid Derective");
	else
		return IsDerective;
}

size_t	ServerContext::FindLocationContextEnd(std::string ConfigFile, size_t Pos) {
	size_t i = Pos;
	size_t OpenBrackets = 0;

	while (ConfigFile[i]) {
		if (ConfigFile[i] == '{')
			OpenBrackets++;
		else if (ConfigFile[i] == '}' && OpenBrackets > 0)
			OpenBrackets--;
		if (ConfigFile[i] == '{' && OpenBrackets == 0)
			break;
		i++;
	}
	return i;
}


void ServerContext::SetServerRoot(std::string val) {

}
void ServerContext::SetServerIndex(std::string val) {

}
void ServerContext::SetServerCMBS(std::string val) {

}
void ServerContext::SetServerErrorPage(std::string val) {

}
void ServerContext::SetServerAutoIndexDir(std::string val) {

}
void ServerContext::SetServerReturn(std::string val) {

}

void ServerContext::SetServerName(std::string val) {

}
void ServerContext::SetServerLocation(std::string val) {

}
void ServerContext::SetServerListen(std::string val) {

}


std::string ServerContext::Trim(std::string value) {
	size_t start = 0;
	size_t end = 0;

	start = value.find_first_not_of(" \n\r\v\f\t");
	if (start == std::string::npos)
		return "";
	end = value.find_last_not_of(" \n\t\f\r\v");
	return (value.substr(start, end - start + 1));
}

void ServerContext::SetValue(int directive, std::string value) {
	std::string TrimedVal;

	void (ServerContext::*SetDirective[])(std::string) = {
		&ServerContext::SetServerLocation,
		&ServerContext::SetServerListen,
		&ServerContext::SetServerName,
		&ServerContext::SetServerRoot,
		&ServerContext::SetServerIndex,
		&ServerContext::SetServerCMBS,
		&ServerContext::SetServerErrorPage,
		&ServerContext::SetServerAutoIndexDir,
		&ServerContext::SetServerReturn,
		NULL
	};
	if (value.compare("") == 0)
		throw std::invalid_argument("Error: Invalid Server Values");
	TrimedVal = Trim(value);
	// check for repeted directives

}



bool ServerContext::HasContent(char EndChar, size_t EndPos, size_t EndValue, std::string ConfigFile) {
	size_t i = 0;
	while (std::isspace(ConfigFile[EndPos +1]) && EndPos - i < EndValue)
		i++;
	if (ConfigFile[EndPos+i] == EndChar)
		return false;
	return true;
}

size_t ServerContext::FindVal(int Directive, std::string ConfigFile, size_t End) {
	std::string input;
	size_t value_end;
	if (Directive == 0) {
		value_end = FindLocationContextEnd(ConfigFile, End);
		if (!HasContent('{', End, value_end, ConfigFile) || !HasContent('}', End, value_end, ConfigFile) || !HasContent('\n', End, value_end, ConfigFile))
			throw std::invalid_argument("Error: Invalid Location Block");
		input = ConfigFile.substr(End, value_end - End + 1);
	}
	else {
		value_end = ConfigFile.find_first_of(';', End);
		if (!HasContent(';', End, value_end, ConfigFile))
			throw std::invalid_argument("Error: Invalid Location Block");
		input = ConfigFile.substr(End, value_end - End);
	}
	SetValue(Directive, input);
	return value_end;

}


void ServerContext::GetValuePairs(size_t *StartPos, std::string ConfigFile ) {
	size_t i = *StartPos;
	size_t Start = 0;
	size_t End = 0;
	size_t ValueEnd = 0;
	int Derective;

	while (ConfigFile[i] != '}') {
		Start = ConfigFile.find_first_not_of(" \t\n\v\r\f", i);
		if (ConfigFile[Start] == '{' || Start == std::string::npos) {
			i = Start;
			break;
		}
		End = ConfigFile.find_first_of(" \t\n\v\f\r", Start);
		Derective = IsDirective(ConfigFile.substr(Start, End - Start));
		i = FindVal(Derective, ConfigFile, End);
		if (ValueEnd != std::string::npos)
			i++;
	}
	*StartPos = i;
}
