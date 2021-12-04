###############################################################################
# Makefile pour la compilation des programmes 
###############################################################################

#### options de compilation ####
COMPIL_OPTIONS = -Wall 
CFLAG =  -c

#### variables ####
CC = gcc 
BIN = gcc

EXECUTABLES = server client

all : $(EXECUTABLES)



###############################################################################
# compilation separee des packages (modules)
###############################################################################

sendRecv.o: sendRecv.c sendRecv.h

socketCreation.o: socketCreation.c socketCreation.h

server.o: server.c sendRecv.h

client.o: client.c sendRecv.h

%.o: %.c
	$(CC) $(CFLAG) $(COMPIL_OPTIONS) $<


###############################################################################
# edition de lien des modules separes 
# pour la creation des programmes executables 
###############################################################################

server: server.o sendRecv.o socketCreation.o

client: client.o sendRecv.o

%: %.o
	$(BIN) $(COMPIL_OPTIONS) $^ -o $@

###############################################################################
# nettoyage : suppression des fichiers .o et des executables
###############################################################################

clean :
	rm -rf *.o
delete:
	rm -rf *o $(EXECUTABLES)
