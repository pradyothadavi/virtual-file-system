#ifndef _MAIN_HEADER_
#define _MAIN_HEADER_

/*
Group No : 28
Version No : 1.0
*/

/* This header file has the datatype definitions used for the main header */

#include "fileSystemOps.h"

#define BUFSIZE 200
#define CMDSIZE 30
#define PARSIZE 100

struct mainHeader{
    char c_fileSystemLabel[VFS_NAMELEN];
    unsigned int ui_uniqueNo;
    unsigned int ui_maxNoOfFileDescriptors;
    unsigned int ui_noOfUsedFileDescriptors;
    unsigned int ui_totalBlocks;
    unsigned int ui_startBlockNoOfInodeArray;
    unsigned int ui_startBlockNoOfDataBlockArray;
};

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

#endif 
