#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nAryTree.h"
#include "directoryOps.h"
#include "fileDescriptor.h"
#include "freeList.h"
#include "global.h"

/*
Function Name: v_makedir
Description: It creates a directory in the virtual file system. If the 
             intermediate directories does not exists then it creates them.
Parameters: It takes 2 parameters.
            1) Path of the directory to be created
            2) Name of the directory to be created
Return Type: void
*/
void v_makedir(char *cPtr_directoryPath,char *cPtr_directoryName){
    
    struct nAryTreeNode *currentNode = NULL;
    struct nAryTreeNode *previousNode = NULL;
    struct nAryTreeNode *newNAryNode = NULL;

    struct fileDescriptor *s_newInode = NULL;

    char *cPtr_token = NULL;
    char *cPtr_filePath = NULL;
    
    unsigned int ui_inodeNo = 0;
    unsigned int ui_dientryBlockNo = 0;
    unsigned int ui_noOfCharactersParsed = 0;

    previousNode = currentNode = sPtr_rootNAryTree;

    /* Tokenize the directory path */
    cPtr_token = strtok(cPtr_directoryPath,"/");
    while(cPtr_token != NULL){
         /* To the length of string, +1 is added because '/' is not included
            in the token. This variable helps in creating the file path when a 
            new directory is found */
         ui_noOfCharactersParsed = strlen(cPtr_token)+1;
         previousNode = currentNode;
         currentNode = s_searchNAryTreeNode(currentNode,cPtr_token);
         if(NULL == currentNode){

              currentNode = previousNode;
              while(cPtr_token != NULL){
     
                   /* Get a inode number from the free List */
                   ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
                   /* Path of the directory */
                   cPtr_filePath = (char *)malloc(sizeof(char)*(ui_noOfCharactersParsed+1));
                   strncpy(cPtr_filePath,cPtr_directoryPath,ui_noOfCharactersParsed);
                   *(cPtr_filePath + ui_noOfCharactersParsed + 1) = '\0';
                   /* Directory Entry Block Number */
                   ui_dientryBlockNo = i_getFreeBlock(s_dataBlockFreeList);
              
                   /* Copy the details into the file descriptor structure */
                   s_newInode = s_createFileDescriptor(cPtr_token,cPtr_filePath,'d',ui_inodeNo,0,ui_dientryBlockNo);
              
                   newNAryNode = s_getNAryTreeNode();
                   newNAryNode->s_inode = s_newInode;
                   newNAryNode->parent = NULL;
                   newNAryNode->leftChild = NULL;
                   newNAryNode->rightSibling = NULL;
                   previousNode = s_insertNAryTreeNode(currentNode,newNAryNode);
                   currentNode = newNAryNode;

                   ui_noOfCharactersParsed = strlen(cPtr_token)+1;
                   cPtr_token = strtok(NULL,"/");
              }/* End of Inner while */
                            
         }/* end of if(NULL == currentNode) */
         cPtr_token = strtok(NULL,"/");
    }/* End of Outer while */

    /* Get a inode number from the free List */
    ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
    /* Path of the directory */
    cPtr_filePath = (char *)malloc(sizeof(char)*strlen(cPtr_directoryPath));
    strcpy(cPtr_filePath,cPtr_directoryPath);
    /* Directory Entry Block Number */
    ui_dientryBlockNo = i_getFreeBlock(s_dataBlockFreeList);
              
    /* Copy the details into the file descriptor structure */
    s_newInode = s_createFileDescriptor(cPtr_directoryName,cPtr_directoryPath,'d',ui_inodeNo,0,ui_dientryBlockNo);
              
    newNAryNode = s_getNAryTreeNode();
    newNAryNode->s_inode = s_newInode;
    newNAryNode->parent = NULL;
    newNAryNode->leftChild = NULL;
    newNAryNode->rightSibling = NULL;
    previousNode = s_insertNAryTreeNode(currentNode,newNAryNode);
    currentNode = newNAryNode;

    /* Print the n-Ary Tree . Note : Remove it later */
    v_traverseNAryTree(sPtr_rootNAryTree);
}
