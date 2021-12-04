#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h> /* close */
#include <netdb.h>
#include "sendRecv.h"

char * fileContent;

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Donnez le bon nombre de arguments\n");
    return -1;

  }
  if (strcmp(argv[1], "localhost") != 0) {
    fprintf(stderr, "Vous etes pas sur local host le port\n");
    return -1;
  }
  if (strlen(argv[2]) != 4) {
    fprintf(stderr, "Donnez le bon nombre pour le port\n");
    return -1;

  }
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  int portno = atoi(argv[2]);
  struct sockaddr_in server_adresse;
  struct hostent * server = gethostbyname(argv[1]);
  char buffer[256] = {
    0
  };
  char nomfichier[256];

  char buff2[30000];

  server_adresse.sin_family = AF_INET;

  bcopy((char * ) server -> h_addr, (char * ) & server_adresse.sin_addr.s_addr, server -> h_length); /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
  server_adresse.sin_port = htons(portno);

  connect(clientSocket, (struct sockaddr * ) & server_adresse, sizeof(server_adresse));

  while (1) {
    bzero(buffer, 255);
    read(clientSocket, buffer, 255);
    if (strcmp(buffer, "RETIRATION\n") == 0) {
      write(clientSocket, "Tapez end pour sortir ou d'autre pour continuer", 255);
      bzero(buffer, 255);
    read(clientSocket, buffer, 255);
    printf("end(pour finir) OU D'AURE CHOSES(pour continuer ) ou (tapez 0 pour fermer la socket) %s\n", buffer);
    bzero(buffer, strlen(buffer));
    fgets(buffer, 255, stdin);

    if (buffer[0] == '0') {
      close(clientSocket);
      exit(0);
    }

    while (strcmp(buffer, "end") != 0) {

      printf("Donnez le nom de fichier : ");

      bzero(buffer, strlen(buffer));
      fgets(buffer, 255, stdin);

      if (buffer[0] == '0') {
        close(clientSocket);
        exit(0);
      }
      printf("Le nom de fichier est %s", buffer);
      strcpy(nomfichier, buffer);
      write(clientSocket, buffer, 255); //envoi le nom d'un fichier     		

      if (strcmp(buffer, "ERROR FICHIER N'EXISTE PAS") == 0) {
        read(clientSocket, "\n", 255);
        write(clientSocket, "Le fichier n'a pas bien recu\n", 256);
        bzero(buffer, strlen(buffer));
        read(clientSocket, buffer, 255);
        bzero(buff2, strlen(buff2));
      } else {
        fileRecv(nomfichier, clientSocket);
        bzero(buffer, strlen(buffer));
        read(clientSocket, buffer, 255);

      }
	}
    } else if (strcmp("Donne le nom de fichier: ", buffer)==0) {
	printf("%s", buffer);
	bzero(buffer,255);//on met le buffer à 0
	fgets(buffer,255,stdin);
	char tempForFileName[256];
	sprintf(tempForFileName, "%s", buffer);
	write(clientSocket, buffer, 255); //Envoi le file
	bzero(buffer,255);
	read(clientSocket, buffer, 255); // Reponse de server
	printf("Server now says: %s\n", buffer);
	sendFile(tempForFileName, "./clientImages/", clientSocket);
	printf("File has been sent!\n");
	write(clientSocket, "File has been sent", 255);
	bzero(buffer,255);
	read(clientSocket, buffer, 255);
	printf("Server message: %s\n", buffer);
	continue;
    } else {

      printf("VOUS AVEZ CHOISI (DANS LE CAS DE DEPOT CHOISISSEZ LES FICHIERS DIRECTEMENT) %s\n", buffer);
    
    }
    bzero(buffer, 255);
    printf("Message for server : ");
    //on met le buffer à 0

    fgets(buffer, 255, stdin);

    if (buffer[0] == '0') {
      close(clientSocket);
      exit(0);
    }
    write(clientSocket, buffer, 255);
  }

  return 0;
}
