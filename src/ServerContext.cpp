/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:46:12 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/13 19:33:09 by aabdou           ###   ########.fr       */
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
		if (HasLocation("/") == false) {
			LocationContext loc;
			_LocationContext.push_back(loc);
			_LocationPos++;
		}
}

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
	if (this != &obj) {
		_LocationContext = obj._LocationContext;
		_IsListening = obj._IsListening;
		_CustomServerName = obj._CustomServerName;
		_ServerID = obj._ServerID;
		_Listen = obj._Listen;
		_ServerNames = obj._ServerNames;
		_LocationContext = obj._LocationContext;
		// config val data
		_Root = obj._Root;
		_Index = obj._Index;
		_ClientMaxBodySize = obj._ClientMaxBodySize;
		_ErrorPage = obj._ErrorPage;
		_AutoIndex = obj._AutoIndex;
		_ReturnValue = obj._ReturnValue;
		_RootPath = obj._RootPath;
		_IndexList = obj._IndexList;
		_ClientMaxSize = obj._ClientMaxSize;
		_ErrorPages = obj._ErrorPages;
		_AutoIndexStatus = obj._AutoIndexStatus;
		_ReturnDir = obj._ReturnDir;
	}
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
	ConfigValues::SetRoot(val);
}
void ServerContext::SetServerIndex(std::string val) {
	ConfigValues::SetIndex(val);
}
void ServerContext::SetServerCMBS(std::string val) {
	ConfigValues::SetCmbs(val);
}
void ServerContext::SetServerErrorPage(std::string val) {
	ConfigValues::SetErrorPage(val);
}
void ServerContext::SetServerAutoIndexDir(std::string val) {
	ConfigValues::SetAutoIndexDir(val);
}
void ServerContext::SetServerReturn(std::string val) {
	ConfigValues::SetReturnDir(val);
}


void ServerContext::SetServerName(std::string val) {
	_CustomServerName = true;
	_ServerNames.clear();
	ServerName obj(val);
	_ServerNames = obj.GetServerNames();
}


void ServerContext::SetServerLocation(std::string val) {
	if (_LocationPos == 0)
		_LocationContext.clear();
	_LocationPos++;
	LocationContext loca(val);

	for (size_t i = 0; i < _LocationContext.size(); i++){
		if (_LocationContext[i].GetLocationUri().GetUri().compare(loca.GetLocationUri().GetUri()) == 0)
			throw std::invalid_argument("Error: Duplicate URI");
	}
	_LocationContext.push_back(loca);

}
void ServerContext::SetServerListen(std::string val) {
	_IsListening = true;
	Listen obj(val);
	_Listen.first = obj.GetIpNb();
	_Listen.second = obj.GetPortNb();
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
	DoubleDirectiveCheck(directive);
	(this->*SetDirective[directive])(value);

}

void ServerContext::DoubleDirectiveCheck(int directive) {
	switch (directive)
	{
	case 1:
		if (_IsListening == true)
			throw std::invalid_argument("Error: Multiple Listening Ports");
		return;
	case 2:
		if (_CustomServerName == true)
			throw std::invalid_argument("Error: Multipul Server Names");
		return;
	case 3:
		if (_Root == true)
			throw std::invalid_argument("Error: Multipul Root Paths");
		return;
	case 4:
		if (_Index == true)
			throw std::invalid_argument("Error: Multipul Server Indexes");
		return;
	case 5:
		if (_ClientMaxBodySize == true)
			throw std::invalid_argument("Error: Multipul ClientMaxBodySizes");
		return;
	case 6:
		if (_ErrorPage == true)
			throw std::invalid_argument("Error: Multipul Error Pages");
		return;
	case 7:
		if (_AutoIndexStatus == true)
			throw std::invalid_argument("Error: Multipul Auto Index expressions");
		return;
	case 8:
		if (_ReturnValue == true)
			throw std::invalid_argument("Error: Multipul Return expressions");
		return;
	}
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

std::vector<LocationContext> ServerContext::GetLocationContexts() const {
	return _LocationContext;
}

std::pair<std::string, std::string> ServerContext::GetListen() const {
	return _Listen;
}
std::string ServerContext::GetIpAddress() const {
	return _Listen.first;
}
std::string ServerContext::GetPortNumber() const {
	return _Listen.second;
}
std::vector<std::string> ServerContext::GetServerNames() const {
	return _ServerNames;
}
