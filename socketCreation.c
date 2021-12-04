#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
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
#include "sendRecv.h"
#include "socketCreation.h"

int creationServerSocket() {
  int newSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (newSocket == -1) {
    perror("ERREUR: Socket d'ecoute n'a pas ete cree!!\n");
    return -1;
  }
  return newSocket;
}

void adresseBind(int serverSocket, int portNo) {
  //Creation de l'adresse pour une socketEcoute
  /* Le serveur accepte n'importe quelle adresse */
  SOCKADDR_IN s = {
    0
  };
  s.sin_addr.s_addr = htonl(INADDR_ANY);
  s.sin_family = AF_INET;
  s.sin_port = htons(portNo);

  //Bind de socket avec s, renvoi erreur en cas echeant
  int bindSocket = bind(serverSocket, (SOCKADDR * ) & s, sizeof(s));
  if (bindSocket == -1) {
    perror("ERREUR: Erreur dans adresseBind: ");
    exit(-1);
  }
}

void listenModeServerSocket(int serverSocket) {
  int listenMode = listen(serverSocket, 3);//Attends des clients (queue est 3)
  if (listenMode == -1) {
    perror("Erreur: error in listenModeServerSocket!\n");
    exit(-1);
  }
}

int initialisation(int portNo) {
  //Creation d'une socket qu'on va utiliser comme soket d'ecoute
  int serverSocket = creationServerSocket();
  adresseBind(serverSocket, portNo);
  listenModeServerSocket(serverSocket);
  printf("Server socket in listen mode has been created.\n");
  return serverSocket;
}
