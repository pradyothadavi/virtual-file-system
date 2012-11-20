#ifndef _MAIN_HEADER_
#define _MAIN_HEADER_

/*
Group No : 28
Version No : 1.0
*/

#define MAIN_DEBUG 1

#define BUFSIZE 200

#define CMDSIZE 30

#define PARSIZE 100


void createvfs ( char *P1, int P2 );

void mountvfs ( char *P1 );

void unmountvfs ( char *P1 );

void makedir ( char *P1, char *P2 );

void deletedir ( char *P1 );

void movedir ( char *P1, char *P2 );

void listdir ( char *P1, int P2, char *P3 );

void addfile ( char *P1, char *P2, char *P3 );

void listfile ( char *P1, char *P2 );

void updatefile ( char *P1, char *P2 );

void removefile ( char *P1 );

void movefile ( char *P1, char *P2 );

void copyfile ( char *P1, char *P2 );

void exportfile ( char *P1, char *P2 );

void searchfile ( char *P1, char *P2 );

void processcommand( char *command, char *P1, char *P2, char *P3 );

void v_freeResources();

#endif 
