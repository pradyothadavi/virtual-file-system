/*
Version No: 1.0
Application Name: Virtual File System
*/

/*
    The file contains the code which is the entry point of the application.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "global.h"
#include "main.h"
#include "vfs_errorcodes.h"
#include "nAryTree.h"
#include "binarySearchTree.h"

/*
Function Name: main
Description: It is the entry point of the application, which calls different 
             modules based on the command.
Parameters: It takes 2 parameters from the command line
            1) No.of.command line arguments
            2) A file path to a text file, which has all the commands to be 
               executed.
Return Type: It return a integer type, to the operating system
             0 --> If programs exits successfully
             Greater than 0 --> On abnormal termination
*/
unsigned int ui_mountFlag = 0;

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
#if MAIN_DEBUG
	 printf("==================================================\n");
	 printf("Processing: %s", linebuffer);
#endif

	 /* Remove the extra newline character in the end of line */
	 linebuffer[ strlen(linebuffer)-1 ] = '\0';
		
	 /* Get the command and the parameters using tokenizer */
	 strcpy( command, (token = strtok(linebuffer, " ")) == NULL ? "" : token );
		
	 strcpy( par1, (token = strtok(NULL, " ")) == NULL ? "" : token );
	 strcpy( par2, (token = strtok(NULL, " ")) == NULL ? "" : token );
	 strcpy( par3, (token = strtok(NULL, " ")) == NULL ? "" : token );
#if MAIN_DEBUG
      printf("Command:%s:p1:%s:p2:%s:p3:%s\n",command, par1, par2, par3);
      printf("==================================================\n");
#endif

	 processcommand( command, par1, par2, par3 );
    }

    v_freeResources();

    return 0;
}

void processcommand( char *command, char *P1, char *P2, char *P3 ){
	
    if( strcmp(command, "createvfs") == 0 ){
	 int size = atoi(P2);
         if( (0 == strcmp(P1,"")) || 0 == P2 ){
              printf("createvfs_FAILURE %s \n",ERR_VFS_CREATE_00);
         } else {
              createvfs (P1,size);
         }
    }
    else if( strcmp(command, "mountvfs") == 0 ){
         if( (0 == strcmp(P1,"")) ){
              printf("mountvfs_FAILURE %s \n",ERR_VFS_MOUNT_05);
         } else {
              if( 1 == ui_mountFlag ){
                   printf("mountvfs_FAILURE %s \n",ERR_VFS_MOUNT_04);
              } else {
                   mountvfs (P1);
              }
         }
    }
    else if( strcmp(command, "unmountvfs") == 0 ){
         if( (0 == strcmp(P1,"")) ){
              printf("unmountvfs_FAILURE %s \n",ERR_VFS_UNMOUNT_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("unmountvfs_FAILURE %s \n",ERR_VFS_UNMOUNT_04);
              } else {
                   unmountvfs (P1);
              }
         }
    }
    else if( strcmp(command, "makedir") == 0 ){
         if( (0 == strcmp(P1,"")) || (0 == strcmp(P2,"")) ){
              printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_00);
         } else {
              if( 0 == ui_mountFlag ){
                   printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_05);
              } else {
                   makedir (P1,P2);
              }
         } 
    }
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
 
    int i_retVal = 0;

    i_retVal = i_createvfs(P1,P2);
    if( SUCCESS == i_retVal ){
         printf("createvfs_SUCCESS \n");
    }
}

void mountvfs ( char *P1 ){

    int i_retVal = 0;

    i_retVal = i_mountvfs(P1);
    if( SUCCESS == i_retVal ){
         ui_mountFlag = 1;
#if MAIN_DEBUG
         v_traverseNAryTree(sPtr_rootNAryTree,INODENUM);
         v_traverseBST(sPtr_rootBST,FILEPATH);
#endif
         printf("mountvfs_SUCCESS \n");
    }
}

void unmountvfs ( char *P1 ){

    int i_retVal = 0;

    i_retVal = i_unmountvfs(P1);
    if( SUCCESS == i_retVal ){
         v_freeResources();
         ui_mountFlag = 0;
         printf("unmountvfs_SUCCESS \n");
    }
}

void makedir ( char *P1, char *P2 ){

    int i_retVal = 0;

    i_retVal = i_makedir(P1,P2);
    if( SUCCESS == i_retVal ){
#if MAIN_DEBUG
         v_traverseNAryTree(sPtr_rootNAryTree,INODENUM);
         v_traverseBST(sPtr_rootBST,FILEPATH);
#endif
         printf("makedir_SUCCESS \n");
    }
}

void deletedir ( char *P1 ){	

}

void movedir ( char *P1, char *P2 ){

}

void listdir ( char *P1, int P2, char *P3 ){

}

void addfile ( char *P1, char *P2, char *P3 ){

}

void listfile ( char *P1, char *P2 ){
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("listfile_TO_BE_DONE\n");
}

void updatefile ( char *P1, char *P2 ){
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("updatefile_TO_BE_DONE\n");
}

void removefile ( char *P1 ){
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("removefile_TO_BE_DONE\n");
}

void movefile ( char *P1, char *P2 ){
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("movefile_TO_BE_DONE\n");
}

void copyfile ( char *P1, char *P2 ){
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("copyfile_TO_BE_DONE\n");
}

void exportfile ( char *P1, char *P2 ){
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("exportfile_TO_BE_DONE\n");
}

void searchfile ( char *P1, char *P2 ){
	/* Call the appropriate function with given arguments and display appropriate output on the screen */
	printf("searchfile_TO_BE_DONE\n");
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void v_freeResources(){
    free(cPtr_nameOfVfsMounted);
    v_deleteNAryTreeNode(sPtr_rootNAryTree);
}
