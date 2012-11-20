#include <string.h>

#include "fileOps.h"
#include "global.h"
#include "fileDescriptor.h"
#include "fileSystemOps.h"
#include "block.h"
#include "vfs_errorcodes.h"

/*
Function Name:
Description:
Parameters:
Return Type:
*/
void v_addfile(char *cPtr_destinationPath,char *cPtr_fileName,char *cPtr_sysPath){

    char *cPtr_copyOfDestPath = NULL;
    char *cPtr_token = NULL;
    char *cPtr_dirPath = NULL;

    unsigned int ui_inodeNo = 0;
    unsigned int ui_dataBlockNo = 0;
    unsigned int ui_size = 0;
    unsigned int ui_noOfCharactersParsed = 0;

    FILE *fp = NULL;
    FILE *fpVfs = NULL;

    struct fileDescriptor *s_newInode = NULL;
    struct nAryTreeNode *newFileNode = NULL;
    struct nAryTreeNode *nAryRoot = NULL;
    struct nAryTreeNode *previousNode = NULL;
    struct nAryTreeNode *currentNode = NULL;
    struct nAryTreeNode *tempNode = NULL;
    struct dataBlock s_block;
    /* Validations */
    cPtr_token = strtok(cPtr_fileName,"/");
    if( cPtr_token != NULL && ( 0 != strcmp(cPtr_token,cPtr_fileName)) ){
         printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_02);
         return;
    }

    if( NULL == (fpVfs = fopen(cPtr_nameOfVfsMounted,"r")) ){
         printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_07);
         return;
    } else {
         fclose(fpVfs);
    }

    if( NULL == (fp = fopen(cPtr_sysPath,"rb")) ){
         printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_05);
         return;
    } else {
         fseek(fp,0,SEEK_END);
         ui_size = ftell(fp);
         if( VFS_BLOCKSIZE < ui_size ){
              printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_06);
              return;
         }
         fclose(fp);
    }
    nAryRoot = sPtr_rootNAryTree;
    currentNode = nAryRoot;
    cPtr_copyOfDestPath = (char *)malloc(sizeof(char)*strlen(cPtr_destinationPath));
    strcpy(cPtr_copyOfDestPath,cPtr_destinationPath);
    if( 0 == strcmp(cPtr_destinationPath,"/") ){
         currentNode = s_searchNAryTreeNode(nAryRoot,cPtr_fileName,NONRECURSIVE);
         if( ( currentNode != NULL ) && (0 == strcmp(currentNode->s_inode->cptr_fileName,cPtr_fileName)) ){
              printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_03);
              return;
         }
         ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
         ui_dataBlockNo = i_getFreeBlock(s_dataBlockFreeList);
         if( (0 == ui_inodeNo) || (0 == ui_dataBlockNo) ){
              printf("addfile_FAILURE %s %s \n",ERR_VFS_ADDFILE_01,ERR_VFS_ADDFILE_04);
              return;
         }
         s_newInode = s_createFileDescriptor(cPtr_fileName,cPtr_destinationPath,'r',ui_inodeNo,0,ui_dataBlockNo);
         newFileNode = s_getNAryTreeNode();
         newFileNode->s_inode = s_newInode;
         newFileNode->leftChild = NULL;
         newFileNode->rightSibling = NULL;
         newFileNode->parent = NULL;

         nAryRoot = s_insertNAryTreeNode(nAryRoot,newFileNode);
    } else {
         cPtr_token = strtok(cPtr_copyOfDestPath,"/");
         while( cPtr_token != NULL ){
              previousNode = currentNode;
              currentNode = s_searchNAryTreeNode(currentNode,cPtr_token,NONRECURSIVE);
              if( NULL == currentNode ){
                   do{
                        cPtr_dirPath = (char *)malloc(sizeof(char)*(ui_noOfCharactersParsed+1));
                        strncpy(cPtr_dirPath,cPtr_destinationPath,ui_noOfCharactersParsed);
                        *(cPtr_dirPath + (ui_noOfCharactersParsed+1)) = '\0';
                        v_makedir(cPtr_dirPath,cPtr_token);
                        free(cPtr_dirPath);
                        cPtr_dirPath = NULL;
                        ui_noOfCharactersParsed = ui_noOfCharactersParsed + strlen(cPtr_token)+1;
                        cPtr_token = strtok(NULL,"/");
                   } while ( cPtr_token != NULL );
                   /* Get the current Node to the newly formed directory */
              } else {
                   ui_noOfCharactersParsed = ui_noOfCharactersParsed + strlen(cPtr_token)+1;
                   cPtr_token = strtok(NULL,"/");
              }
         }
         tempNode = s_searchNAryTreeNode(currentNode,cPtr_fileName,NONRECURSIVE);
         if( (tempNode != NULL) && (0 == strcmp(tempNode->s_inode->cptr_fileName,cPtr_fileName)) ){
              printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_03);
              return;
         }
         ui_inodeNo = i_getFreeBlock(s_inodeBlockFreeList);
         ui_dataBlockNo = i_getFreeBlock(s_dataBlockFreeList);
         if( (0 == ui_inodeNo) || (0 == ui_dataBlockNo) ){
              printf("addfile_FAILURE %s %s \n",ERR_VFS_ADDFILE_01,ERR_VFS_ADDFILE_04);
              return;
         }
         s_newInode = s_createFileDescriptor(cPtr_fileName,cPtr_destinationPath,'r',ui_inodeNo,0,ui_dataBlockNo);
         newFileNode = s_getNAryTreeNode();
         newFileNode->s_inode = s_newInode;
         newFileNode->leftChild = NULL;
         newFileNode->rightSibling = NULL;
         newFileNode->parent = NULL;

         nAryRoot = s_insertNAryTreeNode(currentNode,newFileNode);
    }

    /* Store the data into the file System */
    if( NULL == (fpVfs = fopen(cPtr_nameOfVfsMounted,"ab+")) ){
         printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_07);
         return;
    }
    if( NULL == (fp = fopen(cPtr_sysPath,"r")) ){
         printf("addfile_FAILURE %s \n",ERR_VFS_ADDFILE_05);
         return;
    }
    fseek(fpVfs,ui_dataBlockNo*VFS_BLOCKSIZE,SEEK_SET);
    memset((void *)&s_block,0,sizeof(struct dataBlock));
    fread(&s_block,sizeof(struct dataBlock),1,fp);
    fclose(fp);
    fwrite(&s_block,sizeof(struct dataBlock),1,fpVfs);
    fclose(fpVfs);
    free(cPtr_copyOfDestPath);
}
