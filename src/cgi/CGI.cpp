/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 19:24:30 by aabdou            #+#    #+#             */
/*   Updated: 2022/12/21 16:06:57 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../headers/cgi/CGI.hpp"

const std::string names[] = {
	"DOCUMENT_ROOT", "SERVER_SOFTWARE"
	,"SERVER_PORT","GATEWAY_INTERFACE"
	,"SERVER_NAME","SCRIPT_FILENAME"
	,"REQUEST_METHOD", "SERVER_PROTOCOL"
	, "CONTENT_TYPE", "CONTENT_LENGTH"
	, "PATH_INFO", "QUERY_STRING"
	, "HTTP_COOKIE"};


CGI::CGI(Request const &rec, short const &port): _Request(rec), _Port(port), env(NULL) {
	_ScriptExtension = _Request.GetLocation().GetCGI().GetFileExtention();
	_ScriptName = "hello_script.py";
	_Root = rec.GetLocation().GetRoot();
	// this->_CgiPath = _Root + _Request.GetLocation().GetLocationUri().GetUri() + "/" +  _ScriptName;
	this->_CgiPath = _Request.GetLocation().GetCGI().GetFilePath();
	std::cout << _CgiPath << "\n";
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

void CGI::DeleteEnv(char **ptr, int l) {
	while (l-- > 0)
		delete [] ptr[l];
	delete [] ptr;
}

void CGI::setEnv() {
	if (0 > setenv("DOCUMENT_ROOT", _Root.c_str(), 1)
		|| 0 > setenv("SERVER_PORT", std::to_string(_Port).c_str(), 1)
		|| 0 > setenv("GATEWAY_INTERFACE", "CGI/1.1" , 1)
		|| 0 > setenv("SERVER_NAME", _Request.GetServerBlock().GetServerNames()[0].c_str(), 1)
		|| 0 > setenv("SCRIPT_FILENAME", _ScriptName.c_str(), 1)
		|| 0 > setenv("REQUEST_METHOD", _Request.GetMethod().c_str(), 1)
		|| 0 > setenv("QUERY_STRING", _Request.GetQuery().c_str(), 1)
		|| 0 > setenv("SERVER_PROTOCOL", "HTTP/1.1", 1)
		|| 0 > setenv("SERVER_SOFTWARE", "prj dial 13", 1)
		|| 0 > setenv("PATH_INFO", _CgiPath.c_str(), 1)) {
			throw std::runtime_error("Error: faild to set enviroment varialble");
		}
		std::map<std::string, std::string>::const_iterator tmp = _Request.getHeaders().find("Cookie");
		if (tmp != _Request.getHeaders().end()) {
			if (0 > setenv("HTTP_COOKIE", tmp->second.c_str(), 1))
				throw std::runtime_error("Error: faild to set enviroment varialble");
		}
		if (_Request.getHeaders().find("Content_Type") != _Request.getHeaders().end()) {
			if (0 > setenv("CONTENT_TYPE", _Request.getHeaders().find("Content_Type")->second.c_str(), 1))
				throw std::runtime_error("Error: faild to set enviroment varialble");
		}
		if (_Request.getHeaders().find("Content_Length") != _Request.getHeaders().end()) {
			if (0 > setenv("CONTENT_LENGTH", _Request.getHeaders().find("Content_Length")->second.c_str(), 1))
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
	std::string tmp =  _Root;
	tmp.append("/cgi-bin/");
	tmp.append(getenv("SCRIPT_FILENAME"));
	args[1] = tmp.c_str();
	if (args[1] == NULL)
		throw std::invalid_argument("Error: Can't find \"Script Name\" enviroment varialble.");
	args[2] = NULL;
	if (0 > pipe(write_fd) || 0 > pipe(read_fd))
		throw std::runtime_error("Error: Filed To Creat Discriptor Pair.");
	env = new char *[14];
	int k = 0;
	for (size_t i = 0; i < 13; i++) {
		char * str = getenv(names[i].c_str());
		if (str == NULL)
			continue;
		std::string full = names[i] + '='+ str;
		env[k] = new char[full.length() + 1];
		size_t j;
		for (j = 0; j < full.length(); j++){
			env[k][j] = full[j];
		}
		env[k][j] = '\0';
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
		{
			std::cout << "somthing\n";
			std::cout << strerror(errno);
			exit(1);
		}
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
	this->DeleteEnv(env, k);
}
