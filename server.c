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

int CLIENTTERMINE = 0;
int socketEcoute = 0;


void liste_for_look(char * buffer) {

  DIR * list;
  struct dirent * entry;

  bzero(buffer, strlen(buffer));
  strcat(buffer, "\n***********Welcome client***********\nVoici la liste de tous les fichiers:\n ");

  list = opendir("./fichierServer/");

  if (list == NULL) {

    strcpy(buffer, "Impossible d'ouvrir le fichier Serveur\n");
    exit(0);
  } else {

    while ((entry = readdir(list))!=NULL) {

      int len = strlen(entry -> d_name);
      char * temp = malloc((sizeof(char) * len) + 1);
      strcpy(temp, entry -> d_name);
      strcat(temp, "\n");
      strcat(buffer, temp);
      free(temp);

    }
    strcat(buffer, "Message Server: retirer(1) ou depot(2)?\n");

  }
  closedir(list);
}

void fermeture(int signal) {
  close(socketEcoute);
  return;

}
void end_child(int signal) {

  int stat;
  if (CLIENTTERMINE == 1) {
    if ((wait( & stat)) == -1) {

      perror("wait handler ");
      return;
    }
    printf("Processus enfant %d terminé \n", getpid());
  }

}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Donnez le bon nombre de arguments\n");
    return -1;

  }
  if (strlen(argv[1]) != 4) {
    fprintf(stderr, "Donnez le bon nombre pour le port\n");
    return -1;

  }
  struct sigaction ac; // declaration de la variable pour handler
  ac.sa_handler = end_child; //ici le champs sa_handler recoit le nom du gestionnaire de signal
  ac.sa_flags = SA_RESTART; // eviter les interruption de accept par la reception du signal
  sigaction(SIGCHLD, & ac, NULL); //associée au signal SIGCHLD 

  struct sigaction to_ctrl_c;
  ac.sa_handler = fermeture; //pour fermer le serveur apres SIGINT 
  ac.sa_flags = SA_RESTART;
  sigaction(SIGINT, & to_ctrl_c, NULL);

  struct sockaddr_in client;
  socketEcoute = initialisation(atoi(argv[1]));
  unsigned int size = sizeof(SOCKADDR_IN);
  int socketAccept;
  char bufferRetour[30000];
  char readBuff[256];

  bzero(readBuff, 256);//Mettre le buffer en zero
  Etat_Automate etat;
  printf("Server opened: \n");
  while (1) {
    if (etat == E_FIN) {
      CLIENTTERMINE = 0;

    }
    socketAccept = accept(socketEcoute, (SOCKADDR * ) & client, & size);

    if (socketAccept > 0) {
      printf("Connection from: %s\n", inet_ntoa(client.sin_addr));
      etat = E_INIT;
      printf("ETAT INIT\n");
      CLIENTTERMINE = 1;

    } else {
      fprintf(stderr, "Port n'est pas correct\n");
      return -1;
    }

    switch (fork()) {
    default:
      close(socketAccept);
      break;
    case -1:
      fprintf(stderr, "Erreur de fork\n");
      exit(-1);
      break;
    case 0:

      close(socketEcoute);
      while (etat != E_FIN) {
        switch (etat) {
        case E_INIT:
          liste_for_look(bufferRetour);
          write(socketAccept, bufferRetour, strlen(bufferRetour));
          bzero(bufferRetour, strlen(bufferRetour));
          bzero(readBuff, 256);
          read(socketAccept, readBuff, 255);
          printf("Client says: %s \n", readBuff);
          if (readBuff[0] == '1') {

            printf("Passage au E_RET\n");
            etat = E_RET;
            break;
          } else if (readBuff[0] == '2') {
            printf("Passage au E_DEPOT\n");
            etat = E_DEPOT;
            break;
          } else {
            write(socketAccept, "C'est 1 ou 2", 80);
            etat = E_FIN;
            break;
          }
          case E_RET:
            printf("ETAT RET\n");
            write(socketAccept, "RETIRATION\n", 255);
            read(socketAccept, readBuff, 255); //tapez
            printf("%s", readBuff);
            etat = E_VERIFRET;
            break;

          case E_VERIFRET:
            printf("ETAT VERIFRET\n");

            while (strcmp(readBuff, "end\n") != 0) {
              write(socketAccept, "\n", 150);
              bzero(readBuff, 256);
              read(socketAccept, readBuff, 255);
              printf("Client says: %s \n", readBuff);
              printf("le nom de fichier d'apres le client est %s", readBuff);
              size = sendFile(readBuff, "./fichierServer/", socketAccept);
              if (size == -1) {
                printf("Fichier n'existe pas\n");
                bzero(readBuff, 256);
                write(socketAccept, "ERROR FICHIER N'EXISTE PAS", 200);
                read(socketAccept, readBuff, 255);

                write(socketAccept, "****************\n", 150);

                bzero(readBuff, 256);
                read(socketAccept, readBuff, 255);

                etat = E_FIN;
                break;
              }else{
              	printf("L'image est retire avec succes!\n");
              	break;
              }
            }
            etat = E_FIN;
            break;

          case E_DEPOT:
            printf("ETAT NBDEP\n");
		write(socketAccept, "Donne le nom de fichier: ", 255);//demande d'envoi d'un fichier
		bzero(readBuff,256);
		read(socketAccept, readBuff, 255);
		printf("Client says: %s \n", readBuff);
		write(socketAccept, "OK On commence\n", 255);
		fileRecv(readBuff, socketAccept); //stocker le fichier dans temp
		etat= E_FIN;
		break;
          default:
            break;
        }

      }
      printf("\n*******CLient est sorti*******\n");
      exit(0);

    }

  }
}
