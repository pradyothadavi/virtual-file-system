#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nAryTree.h"
#include "directoryOps.h"
#include "fileDescriptor.h"
#include "freeList.h"
#include "global.h"
#include "vfs_errorcodes.h"
#include "binarySearchTree.h"
#include "hashTable.h"

/*
Function Name: i_makedir
Description: It creates a directory in the virtual file system. If the 
             intermediate directories does not exists then it creates them.
Parameters: It takes 2 parameters.
            1) Path of the directory to be created
            2) Name of the directory to be created
Return Type: int
*/
int i_makedir(char *cPtr_directoryPath,char *cPtr_directoryName){
    
    struct nAryTreeNode *currentNode = NULL;
    struct nAryTreeNode *previousNode = NULL;
    struct nAryTreeNode *tempRootNode = NULL;
    struct nAryTreeNode *newNAryNode = NULL;

    struct fileDescriptor *s_newInode = NULL;

    struct binarySearchTree *newBSTNode = NULL;

    char *cPtr_token = NULL;
    char *cPtr_filePath = NULL;
    char *cPtr_copyOfDirPath = NULL;

    unsigned int ui_inodeNo = 0;
    unsigned int ui_dientryBlockNo = 0;
    unsigned int ui_noOfCharactersParsed = 0;
    unsigned int ui_dirPathLength = 0;
    unsigned int ui_tokenLength = 0;
    unsigned int ui_dirNameLength = 0;

    if( (NULL != (cPtr_token = strtok(cPtr_directoryName,"/")) ) &&
        (0 < strcmp(cPtr_token,cPtr_directoryName)) ) {
         printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_02);
         return FAILURE;
    }

    cPtr_copyOfDirPath = (char *)malloc(sizeof(char)*strlen(cPtr_directoryPath));
    strcpy(cPtr_copyOfDirPath,cPtr_directoryPath);
    ui_dirPathLength = strlen(cPtr_directoryPath);
    ui_dirNameLength = strlen(cPtr_directoryName);
    previousNode = currentNode = sPtr_rootNAryTree;

    if( (0 == strcmp(cPtr_directoryPath,"/")) ){        
         currentNode = s_searchNAryTreeNode(currentNode,
                                            cPtr_directoryName,
                                            NONRECURSIVE);

         if( NULL == currentNode){
              /* Get a inode number from the free List */
              ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
              if( 0 == ui_inodeNo ){
                   printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_01);
                   return FAILURE;
              }
              /* Path of the directory */
              cPtr_filePath = (char *)malloc(sizeof(char)*(ui_dirNameLength+2));
              strcpy(cPtr_filePath,cPtr_directoryPath);
              strcpy(cPtr_filePath+ui_dirPathLength,"/");
              *(cPtr_filePath + ui_dirPathLength + 2) = '\0';
              /* Directory Entry Block Number */
              ui_dientryBlockNo = i_getFreeBlock(s_dataBlockFreeList);
              if( 0 == ui_dientryBlockNo ){
                   printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_01);
                   return FAILURE;
              }
              /* Copy the details into the file descriptor structure */
              s_newInode = s_createFileDescriptor(cPtr_directoryName,
                                                  cPtr_filePath,'d',ui_inodeNo,
                                                  0,ui_dientryBlockNo);
       
              currentNode = s_getNAryTreeNode();
              currentNode->s_inode = s_newInode;
              currentNode->parent = NULL;
              currentNode->leftChild = NULL;
              currentNode->rightSibling = NULL;

              previousNode = sPtr_rootNAryTree;
              previousNode = s_insertNAryTreeNode(previousNode,currentNode);

              newBSTNode = s_getBSTNode(cPtr_directoryName,ui_inodeNo);
              sPtr_rootBST = s_insertBSTNode(sPtr_rootBST,newBSTNode);

              v_traverseNAryTreeAux(currentNode,HASHING);

         } else if( 0 == strcmp(currentNode->s_inode->cptr_fileName,
                                cPtr_directoryName)){
              printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_03);
              return FAILURE;
         } /* End of inner if-else-if */
    } else {

         /* Tokenize the directory path */
         cPtr_token = strtok(cPtr_copyOfDirPath,"/");
         while(cPtr_token != NULL){
              previousNode = currentNode;
              /* This variable helps in creating the file path when a new 
                 directory is found.
              */
              ui_noOfCharactersParsed = ui_noOfCharactersParsed 
                                        + strlen(cPtr_token)+1;
              currentNode = s_searchNAryTreeNode(currentNode,
                                                 cPtr_token,
                                                 NONRECURSIVE);
              if(NULL == currentNode){
                   currentNode = previousNode;
                   tempRootNode = previousNode;
                   while(cPtr_token != NULL){
                        ui_tokenLength = strlen(cPtr_token);
                        /* Get a inode number from the free List */
                        ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
                        if( 0 == ui_inodeNo ){
                             printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_01);
                             return FAILURE;
                        }
                        /* Path of the directory */
                        cPtr_filePath = (char *)malloc(sizeof(char)
                                                 *((ui_noOfCharactersParsed
                                                 +ui_tokenLength+3)) );
                        strncpy(cPtr_filePath,
                                cPtr_directoryPath,
                                ui_noOfCharactersParsed);
                        strcpy(cPtr_filePath+ui_noOfCharactersParsed,"/");
                        strcpy(cPtr_filePath
                               +ui_noOfCharactersParsed+1,cPtr_token);
                        strcpy(cPtr_filePath
                               +ui_noOfCharactersParsed+1+ui_tokenLength,"/");
                        *(cPtr_filePath 
                          + ui_noOfCharactersParsed+ui_tokenLength+3) = '\0';
                        /* Directory Entry Block Number */
                        ui_dientryBlockNo = i_getFreeBlock(s_dataBlockFreeList);
                        if( 0 == ui_dientryBlockNo ){
                             printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_01);
                             return FAILURE;
                        }
                        /*Copy the details into the file descriptor structure */
                        s_newInode = s_createFileDescriptor(cPtr_token,
                                                            cPtr_filePath,
                                                            'd',ui_inodeNo,0,
                                                             ui_dientryBlockNo);
              
                        newNAryNode = s_getNAryTreeNode();
                        newNAryNode->s_inode = s_newInode;
                        newNAryNode->parent = NULL;
                        newNAryNode->leftChild = NULL;
                        newNAryNode->rightSibling = NULL;
                        previousNode = s_insertNAryTreeNode(currentNode,
                                                            newNAryNode);
                        currentNode = newNAryNode;

                        /* Update Binary Search Tree */
                        newBSTNode = s_getBSTNode(cPtr_filePath,ui_inodeNo);
                        sPtr_rootBST = s_insertBSTNode(sPtr_rootBST,newBSTNode);

                        ui_noOfCharactersParsed = ui_noOfCharactersParsed
                                                  + ui_tokenLength+1;
                        cPtr_token = strtok(NULL,"/");
                   }/* End of Inner while */
                   v_traverseNAryTreeAux(tempRootNode,HASHING);                            
              } else {
                   cPtr_token = strtok(NULL,"/");
              }/* end of if(NULL == currentNode) else */
         }/* End of Outer while */
         previousNode = currentNode;
         currentNode = s_searchNAryTreeNode(currentNode,
                                            cPtr_directoryName,
                                            NONRECURSIVE);
         if(NULL == currentNode){
              currentNode = previousNode;
              /* Get a inode number from the free List */
              ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
              /* Path of the directory */
              cPtr_filePath = (char *)malloc(sizeof(char)
                                             *(ui_dirPathLength
                                             +ui_dirNameLength+3));
              strcpy(cPtr_filePath,cPtr_directoryPath);
              strcpy(cPtr_filePath+ui_dirPathLength,"/");
              strcpy(cPtr_filePath+ui_dirPathLength+1,cPtr_directoryName);
              strcpy(cPtr_filePath+ui_dirPathLength+1+ui_dirNameLength,"/");
              *(cPtr_filePath+ui_dirPathLength+ui_dirNameLength+3) = '\0';
              /* Directory Entry Block Number */
              ui_dientryBlockNo = i_getFreeBlock(s_dataBlockFreeList);
              /* Copy the details into the file descriptor structure */
              s_newInode = s_createFileDescriptor(cPtr_directoryName,
                                                  cPtr_filePath,
                                                  'd',ui_inodeNo,0,
                                                  ui_dientryBlockNo);
              newNAryNode = s_getNAryTreeNode();
              newNAryNode->s_inode = s_newInode;
              newNAryNode->parent = NULL;
              newNAryNode->leftChild = NULL;
              newNAryNode->rightSibling = NULL;
              previousNode = s_insertNAryTreeNode(currentNode,newNAryNode);
              currentNode = newNAryNode;

              /* Update Binary Search Tree */
              newBSTNode = s_getBSTNode(cPtr_filePath,ui_inodeNo);
              sPtr_rootBST = s_insertBSTNode(sPtr_rootBST,newBSTNode);

              v_traverseNAryTreeAux(currentNode,HASHING);

         } else if( 0 == strcmp(currentNode->s_inode->cptr_fileName,
                                cPtr_directoryName)){
              printf("makedir_FAILURE %s \n",ERR_VFS_MAKEDIR_03);
              return FAILURE;
         } /* end of if(NULL == currentNode) else */         
    } /* end of outer if-else */

    return SUCCESS;
}

