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

    struct binarySearchTree *newBSTNode = NULL;

    char *cPtr_token = NULL;
    char *cPtr_filePath = NULL;
    char *cPtr_copyOfDirPath = NULL;

    unsigned int ui_inodeNo = 0;
    unsigned int ui_dientryBlockNo = 0;
    unsigned int ui_noOfCharactersParsed = 0;

    cPtr_copyOfDirPath = (char *)malloc(sizeof(char)*strlen(cPtr_directoryPath));
    strcpy(cPtr_copyOfDirPath,cPtr_directoryPath);
    previousNode = currentNode = sPtr_rootNAryTree;

    if( (0 == strcmp(cPtr_directoryPath,"/")) ){
         
         currentNode = s_searchNAryTreeNode(currentNode,cPtr_directoryName,NONRECURSIVE);

         if( NULL == currentNode){
              
              /* Get a inode number from the free List */
              ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
              /* Path of the directory */
              cPtr_filePath = (char *)malloc(sizeof(char)*strlen(cPtr_directoryPath));
              strcpy(cPtr_filePath,cPtr_directoryPath);
              /* Directory Entry Block Number */
              ui_dientryBlockNo = i_getFreeBlock(s_dataBlockFreeList);
              /* Copy the details into the file descriptor structure */
              s_newInode = s_createFileDescriptor(cPtr_directoryName,cPtr_filePath,'d',ui_inodeNo,0,ui_dientryBlockNo);
       
              currentNode = s_getNAryTreeNode();
              currentNode->s_inode = s_newInode;
              currentNode->parent = NULL;
              currentNode->leftChild = NULL;
              currentNode->rightSibling = NULL;

              previousNode = sPtr_rootNAryTree;
              previousNode = s_insertNAryTreeNode(previousNode,currentNode);

              newBSTNode = s_getBSTNode(cPtr_directoryName,ui_inodeNo);
              sPtr_rootBST = s_insertBSTNode(sPtr_rootBST,newBSTNode);

         } else if( 0 == strcmp(currentNode->s_inode->cptr_fileName,cPtr_directoryName)){
              printf("%s \n",ERR_VFS_MAKEDIR_03);
              return;
         } /* End of inner if-else-if */
    } else {

         /* Tokenize the directory path */
         cPtr_token = strtok(cPtr_copyOfDirPath,"/");
         while(cPtr_token != NULL){
              previousNode = currentNode;
              /* This variable helps in creating the file path when a new 
                 directory is found.
              */
              ui_noOfCharactersParsed = ui_noOfCharactersParsed + strlen(cPtr_token)+1;
              currentNode = s_searchNAryTreeNode(currentNode,cPtr_token,NONRECURSIVE);
              if(NULL == currentNode){

                   currentNode = previousNode;
                   while(cPtr_token != NULL){
     
                        /* Get a inode number from the free List */
                        ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
                        /* Path of the directory */
                        cPtr_filePath = (char *)malloc(sizeof(char)*((ui_noOfCharactersParsed+strlen(cPtr_token)+2)) );
                        strncpy(cPtr_filePath,cPtr_directoryPath,ui_noOfCharactersParsed);
                        strcpy(cPtr_filePath+ui_noOfCharactersParsed,"/");
                        strcpy(cPtr_filePath+ui_noOfCharactersParsed+1,cPtr_token);
                        *(cPtr_filePath + ui_noOfCharactersParsed + strlen(cPtr_token) + 2) = '\0';
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

                        /* Update Binary Search Tree */
                        newBSTNode = s_getBSTNode(cPtr_filePath,ui_inodeNo);
                        sPtr_rootBST = s_insertBSTNode(sPtr_rootBST,newBSTNode);

                        ui_noOfCharactersParsed = ui_noOfCharactersParsed + strlen(cPtr_token)+1;
                        cPtr_token = strtok(NULL,"/");
                   }/* End of Inner while */
                            
              }/* end of if(NULL == currentNode) */
              cPtr_token = strtok(NULL,"/");
         }/* End of Outer while */
         previousNode = currentNode;
         currentNode = s_searchNAryTreeNode(currentNode,cPtr_directoryName,NONRECURSIVE);
         if(NULL == currentNode){
              currentNode = previousNode;
              /* Get a inode number from the free List */
              ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
              /* Path of the directory */
              cPtr_filePath = (char *)malloc(sizeof(char)*(strlen(cPtr_directoryPath)+strlen(cPtr_directoryName)+2));
              strcpy(cPtr_filePath,cPtr_directoryPath);
              strcpy(cPtr_filePath+strlen(cPtr_directoryPath),"/");
              strcpy(cPtr_filePath+strlen(cPtr_directoryPath)+1,cPtr_directoryName);
              *(cPtr_filePath + strlen(cPtr_directoryPath) + strlen(cPtr_directoryName) + 2) = '\0';
              /* Directory Entry Block Number */
              ui_dientryBlockNo = i_getFreeBlock(s_dataBlockFreeList);
              
              /* Copy the details into the file descriptor structure */
              s_newInode = s_createFileDescriptor(cPtr_directoryName,cPtr_filePath,'d',ui_inodeNo,0,ui_dientryBlockNo);
              
              newNAryNode = s_getNAryTreeNode();
              newNAryNode->s_inode = s_newInode;
              newNAryNode->parent = NULL;
              newNAryNode->leftChild = NULL;
              newNAryNode->rightSibling = NULL;
              previousNode = s_insertNAryTreeNode(currentNode,newNAryNode);
              currentNode = newNAryNode;

              /* Update Binary Search Tree */
              printf(" BST PATH %s when makdir path is %s \n",cPtr_filePath,cPtr_directoryPath);
              newBSTNode = s_getBSTNode(cPtr_filePath,ui_inodeNo);
              sPtr_rootBST = s_insertBSTNode(sPtr_rootBST,newBSTNode);

         } else if( 0 == strcmp(currentNode->s_inode->cptr_fileName,cPtr_directoryName)){
              printf("%s \n",ERR_VFS_MAKEDIR_03);
              return;
         } /* End of 2nd inner if-else */         

    } /* end of outer if-else */
}

/*
Function Name: v_deletedir
Description: It deletes a directory specified by the directory path. 
Parameters: It takes a single parameter, which the path of the directory to be
            deleted
Return Type:void
*/
void v_deletedir(char *cPtr_directoryPath){

    struct nAryTreeNode *previousNode = NULL;
    struct nAryTreeNode *currentNode = NULL;
    struct binarySearchTree *nodeToBeDeleted = NULL;
    char *cPtr_token = NULL;

    previousNode = currentNode = sPtr_rootNAryTree;

    /* Tokenize the path */
    cPtr_token = strtok(cPtr_directoryPath,"/");
    if(NULL == cPtr_token){
         /* Code to be written when the user wishes to delete the root folder */
    }

    while( cPtr_token != NULL){
         previousNode = currentNode;
         currentNode = s_searchNAryTreeNode(currentNode,cPtr_token,NONRECURSIVE);
         if( NULL == currentNode){
              printf("%s \n",ERR_VFS_DELETEDIR_01);
              return;
         }
         cPtr_token = strtok(NULL,"/");
    }

    /* If the directory to be deleted has sub-directory then display error.
       Note : Remove the below if conditioned statements, if rm -R <dirName>
              command is to be implemented. 
    */
    if( NULL != currentNode->leftChild ){
         printf("%s \n",ERR_VFS_DELETEDIR_02);
         return;
    }
    /* Find the position of the node to be deleted and change the pointers 
       appropriately.
    */
    if( currentNode == previousNode->leftChild ){
         /* Case 1: Node to be deleted is the left-most child */
         if( NULL == currentNode->rightSibling ){
              /* Case 1.1: The leftmost child does not have a right Sibling */
              previousNode->leftChild = NULL;
         } else {
              /* Case 1.1: The leftmost child has right Siblings */
              previousNode->leftChild = currentNode->rightSibling;
         }
          
    } else {
         /* Case 2: Node to be deleted is a right Sibling Node */
         previousNode = previousNode->leftChild;
         while( previousNode->rightSibling != currentNode ){
              previousNode = previousNode->rightSibling;
         }
         previousNode->rightSibling = currentNode->rightSibling;
    }
    nodeToBeDeleted = s_searchBSTNode(sPtr_rootBST,currentNode->s_inode->cptr_filePath);
    if( nodeToBeDeleted != NULL ){
         v_deleteBSTNode(sPtr_rootBST,nodeToBeDeleted);
    }
    v_deleteNAryTreeNode(currentNode);
}

/*
Function Name: v_movedir
Description: The functions moves the directory specified in the source path
             to the destination specified in the destination path.
Parameters: It takes 2 parameters which are of type character pointer, namely
            the source path and the destination path.
Return Type: void
*/
void v_movedir(char *cPtr_sourcePath,char *cPtr_destinationPath){

    char *cPtr_token = NULL;
  
    struct nAryTreeNode *srcPathCurrentNode = NULL;
    struct nAryTreeNode *srcPathPreviousNode = NULL;

    struct nAryTreeNode *destPathCurrentNode = NULL;
    struct nAryTreeNode *destPathPreviousNode = NULL;

    srcPathPreviousNode = srcPathCurrentNode = sPtr_rootNAryTree;
    destPathPreviousNode = destPathCurrentNode = sPtr_rootNAryTree;

    /* Validation of the parameters passed */
    if( NULL == cPtr_sourcePath){
         printf("movedir %s \n",ERR_VFS_MOVEDIR_03);
         return;
    }

    if(NULL == cPtr_destinationPath){
         printf("movedir %s \n",ERR_VFS_MOVEDIR_04);
         return;
    }

    /* Parse the source path */
    cPtr_token = strtok(cPtr_sourcePath,"/");
    while( cPtr_token != NULL){
         srcPathPreviousNode = srcPathCurrentNode;
         srcPathCurrentNode = s_searchNAryTreeNode(srcPathCurrentNode,cPtr_token,NONRECURSIVE);
         if( NULL == srcPathCurrentNode ){
              printf("movedir %s \n",ERR_VFS_MOVEDIR_01);
              return;
         }
         cPtr_token = strtok(NULL,"/");
    }

    /* Parse the destination path */
    cPtr_token = strtok(cPtr_destinationPath,"/");
    while( cPtr_token != NULL){
         destPathPreviousNode = destPathCurrentNode;
         destPathCurrentNode = s_searchNAryTreeNode(destPathCurrentNode,cPtr_token,NONRECURSIVE);
         if( NULL == destPathCurrentNode ){
              printf("movedir %s \n",ERR_VFS_MOVEDIR_02);
              return;
         }
         cPtr_token = strtok(NULL,"/");
    }

    /* Isolate the source sub-tree from the parent */
    if( srcPathCurrentNode == srcPathPreviousNode->leftChild){
         /* Case 1: If the node to be moved is the left most child */
         srcPathPreviousNode->leftChild = NULL;
         srcPathCurrentNode->parent = NULL;
    } else {
         /* Case 2: The node to be moved is one of the right Siblings */
         srcPathPreviousNode = srcPathPreviousNode->leftChild;
         while( srcPathCurrentNode != srcPathPreviousNode->rightSibling ){
              srcPathPreviousNode = srcPathPreviousNode->rightSibling;
         }
         if( srcPathCurrentNode->rightSibling != NULL ){
              srcPathPreviousNode->rightSibling = srcPathCurrentNode->rightSibling; 
         } else {
              srcPathPreviousNode->rightSibling = NULL;
         }
         srcPathCurrentNode->parent = NULL;
    }

    /* Finding the appropriate position of the subtree to be moved in the 
       destination path.
    */
    if( NULL == destPathCurrentNode->leftChild ){
         /* Case 1: No directory of file exists .i.e left-most child is empty*/
         destPathCurrentNode->leftChild = srcPathCurrentNode;
         srcPathCurrentNode->parent = destPathCurrentNode;
    } else {
         /* Save the pointer pointing to the directory to which the subtree
            will be attached.
         */
         destPathPreviousNode = destPathCurrentNode; 
         destPathCurrentNode = destPathCurrentNode->leftChild;
         while( destPathCurrentNode->rightSibling != NULL ){
              destPathCurrentNode = destPathCurrentNode->rightSibling;
         }
         destPathCurrentNode->rightSibling = srcPathCurrentNode;
         srcPathCurrentNode->parent = destPathPreviousNode;
    }
}

/*
Function Name: v_listdir
Description: The function lists the contents of the directory specified in the
             directory path based on the flag and stores it in the hard disk
             at the specified output file.
             if flag is 1 then recursive mode
             if flag is 0 then non-recursive
Parameters: It takes 3 parameters .i.e the path of the directory to be 
            displayed, flag and the file path where the output should be stored.
Return Type: void
*/
void v_listdir(char *cPtr_directoryPath, int flag, char *cPtr_outputPath){

    struct nAryTreeNode *currentNode = NULL;
    struct nAryTreeNode *previousNode = NULL;
    struct nAryTreeNode *tempNode = NULL;
    struct hashTableNode *directoryList = NULL;

    FILE *fp = NULL;

    char *cPtr_token = NULL;

    if ( (flag < 0) || (flag > 1) ){
         printf("lisrdir_FAILURE %s \n",ERR_VFS_LISTDIR_02);
         return;
    }

    if( (fp = fopen(cPtr_outputPath,"r")) == NULL){
         printf("listdir_FAILURE %s \n",ERR_VFS_LISTDIR_03);
         return;
    } else {
         fclose(fp);
    }

    currentNode = sPtr_rootNAryTree;
    
    if( 0 == strcmp(cPtr_directoryPath,"/")){
         previousNode = currentNode;
    } else {
         cPtr_token = strtok(cPtr_directoryPath,"/");
         while(cPtr_token != NULL){
              previousNode = currentNode;
              currentNode = s_searchNAryTreeNode(currentNode,cPtr_token,NONRECURSIVE);
              if(NULL == currentNode){
                   printf("lisrdir_FAILURE %s \n",ERR_VFS_LISTDIR_01);
                   return;
              }
              cPtr_token = strtok(NULL,"/");
         }
    }

    if( 0 == flag ){
         directoryList = s_storeDirectoryNames(cPtr_outputPath,currentNode,NULL);
    } else {
         previousNode = currentNode;
         do{
              directoryList = s_storeDirectoryNames(cPtr_outputPath,currentNode,directoryList);
              /* Cross verify this logic */
              if( directoryList != NULL ){
                   currentNode = s_searchNAryTreeNode(previousNode,directoryList->c_fileName,RECURSIVE); 
                   directoryList = directoryList->sPtr_nextNode;
              } else {
                   currentNode = NULL;
              }  
         } while( directoryList != NULL || tempNode != NULL );
    }
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
struct hashTableNode *s_storeDirectoryNames(char *cPtr_outputPath,
                                            struct nAryTreeNode *sPtr_node,
                                            struct hashTableNode *headPtrOfDirList){

    FILE *fp = NULL;

    struct nAryTreeNode *currentNode = NULL;
    struct hashTableNode *dirList = NULL;
    struct hashTableNode *temp = NULL;
    struct hashTableNode *current = NULL;

    int i_count = -1;

    if( NULL == (fp = fopen(cPtr_outputPath,"a+")) ){
         printf("ERROR: Cannot Open File In Append Mode \n");
         return dirList;
    }

    currentNode = sPtr_node;
    dirList = headPtrOfDirList;
    if( (NULL != currentNode) && (NULL != currentNode->leftChild) ){
         fprintf(fp,"\n Contents of %s/%s : \n ",currentNode->s_inode->cptr_filePath,currentNode->s_inode->cptr_fileName);
         currentNode = currentNode->leftChild;
         fprintf(fp,"%s ",currentNode->s_inode->cptr_fileName);
         i_count++;
         if( currentNode->s_inode->c_fileType[0] == 'd' ){
              if(NULL == dirList){
                   dirList = s_createHashTableNode(currentNode->s_inode->cptr_fileName,
                                                   currentNode->s_inode->ui_inodeNo);
              } else {
                   temp = s_createHashTableNode(currentNode->s_inode->cptr_fileName,
                                                   currentNode->s_inode->ui_inodeNo);
                   current = dirList;
                   while(current->sPtr_nextNode != NULL){
                        current = current->sPtr_nextNode;
                   }
                   current->sPtr_nextNode = temp;
              } /* End of inner if-else */
         } /* end of if( currentNode->s_inode->c_fileType[0] == 'd' ) */
         while(currentNode->rightSibling != NULL){
              currentNode = currentNode->rightSibling;
              fprintf(fp,"%s ",currentNode->s_inode->cptr_fileName);
              i_count++;
              if( 2 == i_count){
                   i_count = 0;
                   fprintf(fp,"\n");
              }
              if( currentNode->s_inode->c_fileType[0] == 'd' ){
                   if(NULL == dirList){
                        dirList = s_createHashTableNode(currentNode->s_inode->cptr_fileName,
                                                   currentNode->s_inode->ui_inodeNo);
                   } else {
                        temp = s_createHashTableNode(currentNode->s_inode->cptr_fileName,
                                                   currentNode->s_inode->ui_inodeNo);
                        current = dirList;
                        while(current->sPtr_nextNode != NULL){
                             current = current->sPtr_nextNode;
                        }
                        current->sPtr_nextNode = temp;
                   } /* End of inner if-else */
              } /* end of if( currentNode->s_inode->c_fileType[0] == 'd' ) */
         }
    }
    fclose(fp);

    return dirList;
}