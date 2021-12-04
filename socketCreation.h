#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
typedef int SOCKET;
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <dirent.h>//opendir and readir

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

int creationServerSocket();

void adresseBind(int serverSocket, int portNo);

void listenModeServerSocket(int serverSocket);

int initialisation(int portNo);
