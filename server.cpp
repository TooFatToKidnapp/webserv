#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

typedef struct sockaddr_in ad;

int main() {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	ad obj;
	



}
