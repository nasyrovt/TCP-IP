#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
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

typedef enum {
  E_INIT,
  E_RET,
  E_NBRET,
  E_VERIFRET,
  E_ACTIONRET,
  E_DEPOT,
  E_FIN
}
Etat_Automate;

int sendFile(char * buffer,char* repos, int socket);


void fileRecv(char * fileName, int socket);
