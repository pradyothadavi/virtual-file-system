/*
    This file contains the code for the Virtual File System commands.
    Namely 1) CreateVfs
           2) MountVfs
           3) UnmountVfs
*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>

#include "freeList.h"
#include "fileSystemOps.h"
#include "fileDescriptor.h"
#include "mainHeader.h"
#include "nAryTree.h"
#include "global.h"
#include "vfs_errorcodes.h"
#include "binarySearchTree.h"

/*
Group No : 28
Version No : 1.0
*/

/* Pointers to different data structures, which will be used by most of the
   modules which execute different commands.
*/
struct freeList *s_dataBlockFreeList = NULL;
struct freeList *s_inodeBlockFreeList = NULL;
struct nAryTreeNode *sPtr_rootNAryTree = NULL;
struct binarySearchTree *sPtr_rootBST = NULL;

/*
Function Name: create_vfs
Description: It creates a file system with the specified name and specified 
             size.
Parameters: It takes two parameters
            1)vfsLabel, which is the name of the VFS
            2)size, which is the size of the VFS ( in KB )
Return Type: void
*/
void create_vfs(const char *vfsLabel,unsigned int ui_size){

    struct mainHeader s_superBlock;
    struct fileDescriptor s_inode;
    struct dataBlock s_block;
    struct directoryEntry s_dientry;

    int i = 0;
    int j = 0;
    int i_retVal = 0;

#if DEBUG
    struct mainHeader s_testSuperBlock;
    struct fileDescriptor s_testInode;
#endif

    FILE *fpVfs = NULL;

    /* Error handling, to check for a valid name of Vfs */

    /* Validate the size of Vfs */
    if( ui_size <= 0){
         printf("%s \n",ERR_VFS_CREATE_05);
         exit(1);
    }

    if((fpVfs = fopen(vfsLabel,"wb+")) == NULL){
         printf("%s \n",ERR_VFS_CREATE_02);
         exit(1);
    }

    /* Convert the file size into bytes */
    ui_size = ui_size * 1024;
    
    /* Total Blocks is total size divided by size of each block when size is in bytes */
    s_superBlock.ui_totalBlocks = ui_size/VFS_BLOCKSIZE;

    /* Validation Check If the file pointer can move to the end of the file or not */
    if((fseek(fpVfs,(long)s_superBlock.ui_totalBlocks*VFS_BLOCKSIZE,SEEK_SET)) == -1){
         printf(" %s \n",ERR_VFS_CREATE_03);
         exit(1);
    }

    /* Set the file pointer to the initial position of the file */
    if((fseek(fpVfs,(long)0,SEEK_SET)) == -1){
         printf(" %s \n",ERR_VFS_CREATE_03);
         exit(1);
    }

    /* Creating a file of the mentioned size by writing the total no.of.blocks */
    for(i = 0; i < s_superBlock.ui_totalBlocks; i++){
         memset((void *)&s_block,0,VFS_BLOCKSIZE);
         fwrite(&s_block,sizeof(struct dataBlock),1,fpVfs);
    }

    /* 
       Initialize the superblock structure with the appropriate values and
       store it at Data Block 0 of the file system.
    */
    strcpy(s_superBlock.c_fileSystemLabel,vfsLabel);
    s_superBlock.ui_uniqueNo = VFS_UNIQUE_FILE_SYSTEM_NO;
       
    /* 
       The maximum file size that can be created in the file system is 16KB. 
       To calculate the number of file descriptors supported, divide the given
       size by the maximum file size.
    */
    s_superBlock.ui_maxNoOfFileDescriptors = s_superBlock.ui_totalBlocks/VFS_DIRECT_DATABLOCK;

    /*
       Four directories namely 0, 1, root, lost+found are present in the file 
       system by default.
    */
    s_superBlock.ui_noOfUsedFileDescriptors = 4;
       
    s_superBlock.ui_startBlockNoOfInodeArray = VFS_INODE_STARTBLOCK;
    s_superBlock.ui_startBlockNoOfDataBlockArray = VFS_INODE_STARTBLOCK + s_superBlock.ui_maxNoOfFileDescriptors - 1;

    if((fseek(fpVfs,0,SEEK_SET))==-1){
         printf("ERROR: Cannot Travese To The First Data Block \n");
         printf("STATUS: Program Terminated \n");
         exit(1);     
    }
    
    /* Write the superblock to the file system */   
    if((i_retVal=fwrite(&s_superBlock,sizeof(struct mainHeader),1,fpVfs)) != 1){
         printf("ERROR: Could not write the superblock \n");
         printf("STATUS: Program Terminated \n");
         exit(1);
    }

    /* Initialize the various parameters of the Root Directory */
    memset((void *)&s_inode,0,sizeof(struct fileDescriptor));
    strcpy(s_inode.cptr_fileName,"/");
    strcpy(s_inode.cptr_filePath,"/");
    s_inode.c_fileType[0]='d';
    s_inode.c_fileType[1]='\0';
    s_inode.ui_inodeNo = 4;
    s_inode.ui_fileSize = sizeof(struct fileDescriptor);
    s_inode.ui_locationDataBlockNo[0] = s_superBlock.ui_startBlockNoOfDataBlockArray;
    
    if((fseek(fpVfs,VFS_INODE_STARTBLOCK*VFS_BLOCKSIZE+2*VFS_BLOCKSIZE,SEEK_SET))==-1){
         printf("ERROR: Error In Creating Root Inode \n");
         printf("STATUS: Root Directory Could Not Initialized \n");
    }

    if((i_retVal=fwrite(&s_inode,sizeof(struct fileDescriptor),1,fpVfs)) != 1){
         printf("ERROR: Could not write the '/' information to the data block \n");
    }

    /* Code To Assign a datablock which will be the dentry of the root
       and store the value "lost+found" in it.
    */
    if((fseek(fpVfs,s_inode.ui_locationDataBlockNo[0]*VFS_BLOCKSIZE,SEEK_SET))==-1){
         printf("ERROR: Error In Fetching A Data Block \n");
    }
    memset((void *)&s_block,0,VFS_BLOCKSIZE);
    if((i_retVal=fwrite(&s_block,sizeof(struct dataBlock),1,fpVfs)) != 1){
         printf("ERROR: Error In Writing A Data Block \n");
    }
    if((fseek(fpVfs,s_inode.ui_locationDataBlockNo[0]*VFS_BLOCKSIZE,SEEK_SET))==-1){
         printf("ERROR: Error In Fetching A Data Block \n");
    }
    s_dientry.ui_inodeNo = 5;
    strcpy(s_dientry.c_fileName,"lost+found");
    if((i_retVal=fwrite(&s_dientry,sizeof(struct directoryEntry),1,fpVfs)) != 1){
         printf("ERROR: Error In Writing Directory Entry \n");
    }
    s_inode.ui_fileSize = s_inode.ui_fileSize + sizeof(struct directoryEntry);

    /* Rewrite the root Inode, after modifying the size of the file */    
    if((fseek(fpVfs,VFS_INODE_STARTBLOCK*VFS_BLOCKSIZE+2*VFS_BLOCKSIZE,SEEK_SET))==-1){
         printf("ERROR: Error In Creating Root Inode \n");
         printf("STATUS: Root Directory Could Not Initialized \n");
    }

    if((i_retVal=fwrite(&s_inode,sizeof(struct fileDescriptor),1,fpVfs)) != 1){
         printf("ERROR: Could not write the '/' information to the data block \n");
    }

    /* Initialize the lost+found directory */
    memset((void *)&s_inode,0,sizeof(struct fileDescriptor));
    strcpy(s_inode.cptr_fileName,"lost+found");
    strcpy(s_inode.cptr_filePath,"/lost+found");
    s_inode.c_fileType[0]='d';
    s_inode.c_fileType[1]='\0';
    s_inode.ui_inodeNo = 5;
    s_inode.ui_fileSize = sizeof(struct fileDescriptor);
    s_inode.ui_locationDataBlockNo[0] = s_superBlock.ui_startBlockNoOfDataBlockArray + 1;

    if((fseek(fpVfs,VFS_INODE_STARTBLOCK*VFS_BLOCKSIZE+3*VFS_BLOCKSIZE,SEEK_SET))==-1){
         printf("ERROR: Error In Creating Root Inode \n");
         printf("STATUS: Root Directory Could Not Initialized \n");
    }

    if((i_retVal=fwrite(&s_inode,sizeof(struct fileDescriptor),1,fpVfs)) != 1){
         printf("ERROR: Could not write the lost+found directory information to the data block \n");
    }

    /* Code To Assign a dentry for lost+found folder */
    if((fseek(fpVfs,s_inode.ui_locationDataBlockNo[0]*VFS_BLOCKSIZE,SEEK_SET))==-1){
         printf("ERROR: Error In Fetching A Data Block \n");
    }
    memset((void *)&s_block,0,VFS_BLOCKSIZE);
    if((i_retVal=fwrite(&s_block,sizeof(struct dataBlock),1,fpVfs)) != 1){
         printf("ERROR: Error In Writing A Data Block \n");
    }
    if((fseek(fpVfs,s_inode.ui_locationDataBlockNo[0]*VFS_BLOCKSIZE,SEEK_SET))==-1){
         printf("ERROR: Error In Fetching A Data Block \n");
    }

#if DEBUG
    fseek(fpVfs,0,SEEK_SET);
    if((i_retVal=fread(&s_testSuperBlock,sizeof(struct mainHeader),1,fpVfs)) != 1){
         printf("DEBUG: Error in reading superblock \n");   
    }
    printf("DEBUG: Details of File System \n");
    printf("       FILE SYSTEM NAME: ");
    i = 0;
    while(s_testSuperBlock.c_fileSystemLabel[i] != '\0'){
         printf("%c",s_testSuperBlock.c_fileSystemLabel[i]);
         i++;
    }       
    printf("\n");
    printf("       FILE SYSTEM ID: %d \n",s_testSuperBlock.ui_uniqueNo);
    printf("       MAX FILE DESCRIPTORS: %d \n",s_testSuperBlock.ui_maxNoOfFileDescriptors);
    printf("       USED FILE DESCRIPTORS: %d \n",s_testSuperBlock.ui_noOfUsedFileDescriptors);
    printf("       TOTAL DATA BLOCKS: %d \n",s_testSuperBlock.ui_totalBlocks);
    printf("       INODE START BLOCK NO: %d \n",s_testSuperBlock.ui_startBlockNoOfInodeArray);
    printf("       DATA BLOCK START NO: %d \n",s_testSuperBlock.ui_startBlockNoOfDataBlockArray);

    
    for(j = 2; j < s_testSuperBlock.ui_noOfUsedFileDescriptors; j++){
         
         fseek(fpVfs,VFS_INODE_STARTBLOCK*VFS_BLOCKSIZE+j*VFS_BLOCKSIZE,SEEK_SET);
         
         if((i_retVal=fread(&s_testInode,sizeof(struct fileDescriptor),1,fpVfs)) != 1){
              printf("DEBUG: Error in reading inode \n");
         }
         printf("DEBUG: Details of Inode %d \n",s_testInode.ui_inodeNo);
         printf("       FILE NAME: ");
         i=0;
         while(s_testInode.cptr_fileName[i] != '\0'){
              printf("%c",s_testInode.cptr_fileName[i]);
              i++;
         }
         printf("\n");
         
         printf("       FILE PATH: ");
         i=0;
         while(s_testInode.cptr_filePath[i] != '\0'){
              printf("%c",s_testInode.cptr_filePath[i]);
              i++;
         }
         printf("\n");
    
         if(s_testInode.c_fileType[0] == 'd'){
              printf("       FILE TYPE: Directory \n");
         }
     
         printf("       FILE SIZE: %d Bytes\n",s_testInode.ui_fileSize);

         if((fseek(fpVfs,s_testInode.ui_locationDataBlockNo[0]*VFS_BLOCKSIZE,SEEK_SET)) == -1){
              printf("ERROR: Cannot read the dientry. \n");
         }
         if((i_retVal=fread(&s_dientry,sizeof(struct directoryEntry),1,fpVfs)) != 1){
              printf("DEBUG: Error in reading dientry. \n");
         }
         printf("       INODE NO | DIRECTORY or FILE NAME \n");
         printf("          %d      ",s_dientry.ui_inodeNo);
         i = 0;
         while(s_dientry.c_fileName[i] != '\0'){
              printf("%c",s_dientry.c_fileName[i]);
              i++;
         }
         printf("\n");
         
    }
#endif
    fclose(fpVfs);
}


/*
Function Name: mount_vfs
Description: This function loaded the specified file system into the main 
             memory by representing the information of the file system
             with various data structures.
Parameters: It takes the name of the file system to be loaded.
Return Type: void
*/
void mount_vfs(const char *vfsLabel){   
    
    struct mainHeader s_superBlock;
    struct mainHeader * sPtr_superBlock;

    int i_retVal = 0;

    FILE *fpVfs;

    if((fpVfs = fopen(vfsLabel,"rb+")) == NULL){
         printf("%s \n",ERR_VFS_MOUNT_01);
         exit(1);
    }

    if((fseek(fpVfs,0,SEEK_SET)) == -1){
         printf("%s \n",ERR_VFS_MOUNT_02);
    }

    if((i_retVal = fread(&s_superBlock,sizeof(struct mainHeader),1,fpVfs)) != 1){
         printf("%s \n",ERR_VFS_MOUNT_02);
    }

    sPtr_superBlock = &s_superBlock;

    /* Create a list a free data blocks.
       Note: Value 2 is added because 2 data blocks have already been used for
             storing the directory entries of root and lost+found folder.
    */
    s_dataBlockFreeList = s_createFreeList(
                          s_superBlock.ui_startBlockNoOfDataBlockArray+2,
                          s_superBlock.ui_totalBlocks);  

    /* Create a list a free inode blocks */
    s_inodeBlockFreeList = s_createFreeList(
                           VFS_INODE_STARTBLOCK+s_superBlock.ui_noOfUsedFileDescriptors,
                           s_superBlock.ui_maxNoOfFileDescriptors);

#if DEBUG
    v_displayList(s_dataBlockFreeList);
    v_displayList(s_inodeBlockFreeList);
#endif
    /* Load the file system into the n-Ary Tree */
    sPtr_rootNAryTree = s_loadFileSystem(VFS_ROOT_INODE,fpVfs,sPtr_superBlock);

#if DEBUG
    printf("DEBUG: Root Pointer : %d \n",sPtr_rootNAryTree->s_inode->ui_inodeNo);
    v_traverseNAryTree(sPtr_rootNAryTree);
#endif

    v_initializeHashTable();
    v_traverseNAryTreeAux(sPtr_rootNAryTree,HASHING);
    v_traverseNAryTreeAux(sPtr_rootNAryTree->leftChild,BST);

}
