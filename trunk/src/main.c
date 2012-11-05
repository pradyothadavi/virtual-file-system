#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mainHeader.h"
#include "limits.h"
#include "fileSystemOps.h"
#include "nAryTree.h"
#include "hashTable.h"

int main( int argc, char *argv[] ){
    
    FILE *scriptfp;
    
    char linebuffer[BUFSIZE];
    char command[CMDSIZE], par1[PARSIZE], par2[PARSIZE], par3[PARSIZE];
    char *token;

    if( argc != 2 ){
         fprintf(stderr,"Usage: vfsdriver <scriptfile>\n");
	 return(1);
    }

    if( (scriptfp=fopen(argv[1],"r")) == NULL ){
         fprintf(stderr,"Unable to open script file: %s\n", argv[1]);
	 return(2);
    }

    while( fgets(linebuffer, sizeof(linebuffer), scriptfp) != NULL ){
	 /* This output is for debugging... do not uncomment in final version */
	 /*
	 printf("==================================================\n");
	 printf("Processing: %s", linebuffer);
	 printf("==================================================\n");
	 */

	 /* Remove the extra newline character in the end of line */
	 linebuffer[ strlen(linebuffer)-1 ] = '\0';
		
	 /* Get the command and the parameters using tokenizer */
	 strcpy( command, (token = strtok(linebuffer, " ")) == NULL ? "" : token );
		
	 strcpy( par1, (token = strtok(NULL, " ")) == NULL ? "" : token );
	 strcpy( par2, (token = strtok(NULL, " ")) == NULL ? "" : token );
	 strcpy( par3, (token = strtok(NULL, " ")) == NULL ? "" : token );
	
         /* printf("Command:%s:p1:%s:p2:%s:p3:%s\n",command, par1, par2, par3); */

	 processcommand( command, par1, par2, par3 );
    }

    return 0;
}

void processcommand( char *command, char *P1, char *P2, char *P3 ){
	
    if( strcmp(command, "createvfs") == 0 ){
	 int size = atoi(P2);
	 createvfs (P1,size);
    }
    else if( strcmp(command, "mountvfs") == 0 )
	 mountvfs (P1);
    else if( strcmp(command, "unmountvfs") == 0 )
	 unmountvfs (P1);
    else if( strcmp(command, "makedir") == 0 )
	 makedir (P1,P2);
    else if( strcmp(command, "deletedir") == 0 )
	 deletedir (P1);
    else if( strcmp(command, "movedir") == 0 )
	 movedir (P1,P2);
    else if( strcmp(command, "listdir") == 0 ){
	 int flag = atoi(P2);
	 listdir (P1,flag,P3);
    }
    else if( strcmp(command, "addfile") == 0 )
	 addfile (P1,P2,P3);
    else if( strcmp(command, "listfile") == 0 )
	 listfile (P1,P2);
    else if( strcmp(command, "updatefile") == 0 )
	 updatefile (P1,P2);
    else if( strcmp(command, "removefile") == 0 )
	 removefile (P1);
    else if( strcmp(command, "movefile") == 0 )
	 movefile (P1,P2);
    else if( strcmp(command, "copyfile") == 0 )
	 copyfile (P1,P2);
    else if( strcmp(command, "exportfile") == 0 )
	 exportfile (P1,P2);
    else if( strcmp(command, "searchfile") == 0 )
	 searchfile (P1,P2);
    else
	 printf("Ignoring invalid command %s\n", command);
}

void createvfs ( char *P1, int P2 ){
    create_vfs(P1,P2);
}

void mountvfs ( char *P1 ){
    mount_vfs(P1);
}

void unmountvfs ( char *P1 )
{
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("unmountvfs_TO_BE_DONE\n");
}

void makedir ( char *P1, char *P2 ){
    v_makedir(P1,P2);	
}

void deletedir ( char *P1 )
{
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("deletedir_TO_BE_DONE\n");
}

void movedir ( char *P1, char *P2 )
{
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("movedir_TO_BE_DONE\n");
}

void listdir ( char *P1, int P2, char *P3 )
{
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("listdir_TO_BE_DONE\n");
}

void addfile ( char *P1, char *P2, char *P3 )
{
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("addfile_TO_BE_DONE\n");
}

void listfile ( char *P1, char *P2 )
{
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("listfile_TO_BE_DONE\n");
}

void updatefile ( char *P1, char *P2 )
{
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("updatefile_TO_BE_DONE\n");
}

void removefile ( char *P1 )
{
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("removefile_TO_BE_DONE\n");
}

void movefile ( char *P1, char *P2 )
{
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("movefile_TO_BE_DONE\n");
}

void copyfile ( char *P1, char *P2 )
{
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("copyfile_TO_BE_DONE\n");
}

void exportfile ( char *P1, char *P2 )
{
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("exportfile_TO_BE_DONE\n");
}

void searchfile ( char *P1, char *P2 )
{
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("searchfile_TO_BE_DONE\n");
}


