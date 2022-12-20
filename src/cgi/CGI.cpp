/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 19:24:30 by aabdou            #+#    #+#             */
/*   Updated: 2022/12/19 22:03:29 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/cgi/CGI.hpp"

char **env;

const std::string names[] = {
	"DOCUMENT_ROOT", "SERVER_SOFTWARE"
	,"SERVER_PORT","GATEWAY_INTERFACE"
	,"SERVER_NAME","SCRIPT_FILENAME"
	,"REQUEST_METHOD", "SERVER_PROTOCOL"
	, "CONTENT_TYPE", "CONTENT_LENGTH"
	, "PATH_INFO", "QUERY_STRING"
	, "HTTP_COOKIE", "SCRIPT_NAME"
	, "REDIRECT_STATUS", };


CGI::CGI(Request const &rec, short const &port): _Request(rec), _Port(port) {

	size_t pos = _Request.GetLocation().GetCGI().GetFilePath().find_last_of('/');
	pos++;
	_ScriptName = _Request.GetLocation().GetCGI().GetFilePath().substr(pos);
	_Root = rec.GetLocation().GetRoot();
	this->_CgiPath = _Root + _Request.GetLocation().GetLocationUri().GetUri(); //+ '/' + _ScriptName;
	if (access(this->_CgiPath.c_str(), F_OK) == -1)
		throw std::runtime_error("Error: Missing File Or Directory \"" + _CgiPath + "\"");
	if (access(this->_CgiPath.c_str(), X_OK) == -1)
		throw std::runtime_error("Error: Invalid Permissions \"" + _CgiPath + "\"");
	this->setEnv();
	this->Exec();

}

CGI::CGI(const CGI & obj):
_Request(obj._Request),
_Root(obj._Root),
_CgiPath(obj._CgiPath),
_CgiOutput(obj._CgiOutput) {}

CGI::~CGI() {}

std::string const &CGI::GetOutput() const {
	return this->_CgiOutput;
}

void CGI::setEnv() {

	const std::string tab[] = {
	"Server_Software",
	"Server_Protocol"
	, "Content_Type", "Content_Length"
	, "Path_Info", "Query_String"
	, "Http_Cookie"
	, "Redirect_Status"};

	if (0 > setenv("DOCUMENT_ROOT", _Root.c_str(), 1) || 0 > setenv("SERVER_SOFTWARE", "WebServ", 1)
		|| 0 > setenv("SERVER_PORT", std::to_string(_Port).c_str(), 1)
		|| 0 > setenv("GATEWAY_INTERFACE", "CGI/1.1" , 1)
		|| 0 > setenv("SERVER_NAME", _Request.GetServerBlock().GetServerNames()[0].c_str(), 1)
		|| 0 > setenv("SCRIPT_FILENAME", _ScriptName.c_str(), 1)
		|| 0 > setenv("REQUEST_METHOD", _Request.GetMethod().c_str(), 1)) {
			throw std::runtime_error("Error: faild to set enviroment varialble");
		}

	std::map<std::string, std::string>::const_iterator it;
	for(int i = 0 ; i < 8 ; i++) {
		it = _Request.getHeaders().find(tab[i]);
		if (it == _Request.getHeaders().end())
			continue;
		if (0 > setenv(tab[i].c_str(), it->second.c_str(), 1))
			throw std::runtime_error("Error: faild to set enviroment varialble");
	}
}

void CGI::Exec() {
	char const *args[3];
	int write_fd[2];
	int read_fd[2];
	pid_t ChildId;
	char buff[1025];
	int ReadCount = 0;

	args[0] = _CgiPath.c_str();
	std::cout << _CgiPath << "\n";
	args[1] = getenv("SCRIPT_FILENAME");
	if (args[1] == NULL)
		throw std::invalid_argument("Error: Can't find \"Script Name\" enviroment varialble.");
	args[2] = NULL;
	if (0 > pipe(write_fd) || 0 > pipe(read_fd))
		throw std::runtime_error("Error: Filed To Creat Discriptor Pair.");
	env = new char *[15];
	int k = 0;
	for (size_t i = 0; i < 15; i++) {
		std::cout << "[" << names[i] << "]\n";
		char * str = getenv(names[i].c_str());
		if (str == NULL)
			continue;
		std::string full = names[i] + '='+ str;
		env[k] = new char[full.length()];
		size_t j;
		std::cout << full << "\n";
		for (j = 0; j < full.length(); j++){
			env[k][j] = full[j];
		}
		std::cout << env[k] << "here \n";
		k++;
	}
	env[k] = NULL;
	ChildId = fork();
	if (ChildId < 0)
		throw std::runtime_error("Error: Can't Creat Child Prosses");

	if (ChildId == 0) {
		dup2(write_fd[0], 0); // stdin
		close(write_fd[1]);
		dup2(read_fd[1], 1); // stdout
		close(read_fd[0]);
		chdir(_Root.c_str());
		if (execve(_CgiPath.c_str(), (char * const*)args, env) == -1)
			exit(1);
	}
	else {
		close(read_fd[1]);
		close(write_fd[0]);
		std::string body;
		for (size_t i = 0; i < this->_Request.GetBody().size(); i++) {
			body += this->_Request.GetBody()[i];
		}
		write(write_fd[1], body.c_str(), body.size());
		close(write_fd[1]);
		bzero(buff, 1024);
		while ((ReadCount = read(read_fd[0], buff, 1024)) > 0) {
			buff[ReadCount] = '\0';
			_CgiOutput.append(buff, ReadCount);
		}
		close(read_fd[0]);
		wait(0);
	}
}
