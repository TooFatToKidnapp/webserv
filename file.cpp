#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>

typedef struct sockaddr_in ad;
typedef struct addrinfo add;
int main(int ac , char **av) {

	if (ac != 2)
		return -1;
	add var, *res, *p;
	int socket;
	char str[INET_ADDRSTRLEN];
	memset(&var, 0, sizeof(var));
	var.ai_family = AF_INET;
	var.ai_socktype = SOCK_STREAM;
	//var.ai_flags = AI_PASSIVE;

	if ((socket = getaddrinfo(av[1], NULL, &var, &res)) != 0) {
		std::cerr << "Error getaddrinfo\n";
	}

	std::cout << "IP address for " << av[1] << " : \n";

	for (p = res; p != NULL; p = p->ai_next) {
		void *addr;
		char * ip;
		ad *ipv4 = (ad*)p->ai_addr;
		addr = &(ipv4->sin_addr);
		ip = "IPV4";
		inet_ntop(p->ai_family, addr, str, sizeof(str));
		std::cout << ip << " : " << str << "\n";
	}
	freeaddrinfo(res);

	return 0;


}
