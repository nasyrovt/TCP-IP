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
#include "sendRecv.h"

int sendFile(char * buffer,char* repos, int socket){

	buffer[strlen(buffer)-1]='\0';
	char path[20]; 
	strcpy(path, repos);
	char final[1000];
	int size;
	sprintf(final, "%s%s", path, buffer);

	FILE *f=fopen(final,"rb");
	if(f==NULL)
	{	
	    printf("ERROR IN OPEN\n");
	    return -1;
	}
	else
	{	
		//calcul de la taille de fichier
		fseek(f, 0,SEEK_END);
		size = ftell(f);
		fseek(f, 0,SEEK_SET);
		char sizebuffer[100];
		sprintf(sizebuffer, "%d", size);
		write(socket, sizebuffer, 100);//send size of file
		bzero(sizebuffer, 100);		
		read(socket, sizebuffer, sizeof(sizebuffer));//Get OK from server
		
		//send byte by byte
		char str[100];
		while (!feof(f))
		{
		    fread(str, 1, 8, f);
		    write(socket, str, sizeof(str));//envoi de'un octet
		    bzero(str, sizeof(str));
		    read(socket, str, sizeof(str));//OK de server
		    bzero(str, sizeof(str));
		}
	}
	fclose(f);
	return 1;
}


void fileRecv(char * fileName, int socket)
{
	char fileContent[10000];
	char sizeContent[100];
	bzero(fileContent, 10000);
	int size = 0;
	char filePath[20] = "./temp/";
	char fileFullPathTemp[100];
	int sizeoffile;
	read(socket, sizeContent, 100);
	sizeoffile=atoi(sizeContent);
	write(socket, "Size recu OK", 255);
	sprintf(fileFullPathTemp, "%s%s", filePath, fileName);
	FILE *newFile=fopen(fileFullPathTemp,"wb");
	if(newFile==NULL)
	{
		bzero(fileName,256);
		strcpy(fileName,"ERROR: Error openning file.\n");
	}
	else
	{	
		bzero(fileContent, 10000);
		do
		{
			read(socket, fileContent, 100);
			fwrite(fileContent, 1, 8, newFile);
			write(socket, "OK", 20);
			size+=strlen(fileContent);
			bzero(fileContent, 10000);
		}
		while(size!=sizeoffile);

	}
	fclose(newFile);
}
