/*
    This file contains the code for the Virtual File System commands.
    Namely 1) CreateVfs
           2) MountVfs
           3) UnmountVfs
*/

/*
Group No : 28
Version No : 1.0
*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>

#include "global.h"
#include "fileSystemOps.h"
#include "block.h"
#include "freeList.h"
#include "vfs_errorcodes.h"

/* Pointers to different data structures, which will be used by most of the
   modules which execute different commands.
*/
struct freeList *s_dataBlockFreeList = NULL;
struct freeList *s_inodeBlockFreeList = NULL;
struct nAryTreeNode *sPtr_rootNAryTree = NULL;
struct binarySearchTree *sPtr_rootBST = NULL;
char *cPtr_nameOfVfsMounted = NULL;

/*
Function Name: v_createvfs
Description: It creates a file system with the specified name and specified 
             size.
Parameters: It takes two parameters
            1)vfsLabel, which is the name of the VFS
            2)size, which is the size of the VFS ( in KB )
Return Type: void
*/
int i_createvfs(char *vfsLabel,unsigned int ui_size){

    FILE *fpVfs = NULL;

    struct mainHeader s_superBlock;
    struct fileDescriptor s_inode;
    struct dataBlock s_block;
    struct directoryEntry s_dientry;

    char *cPtr_token = NULL;

    int i = 0;
    int j = 0;
    int i_retVal = 0;

#if SUPERBLOCK
    struct mainHeader s_testSuperBlock;
    struct fileDescriptor s_testInode;
#endif

    /* If the data file is already present */
    if( NULL != (fpVfs = fopen(vfsLabel,"r")) ){
         printf("createvfs_FAILURE %s \n",ERR_VFS_CREATE_01);
         return FAILURE;
    }    

    /* Cannot Create the data file */
    if((fpVfs = fopen(vfsLabel,"wb+")) == NULL){
         printf("createvfs_FAILURE %s \n",ERR_VFS_CREATE_02);
         return FAILURE;
    }

    /* Invalid name, .i.e if '/' is present */
    if( NULL != (cPtr_token = strtok(vfsLabel,"/")) && 
        0 < strcmp(cPtr_token,vfsLabel) ){
         printf("createvfs_FAILURE %s \n",ERR_VFS_CREATE_04);
         return FAILURE;
    }   
    
    /* Validate the size of Vfs */
    if( ui_size <= 0 || ui_size > 1024 ){
         printf("createvfs_FAILURE %s \n",ERR_VFS_CREATE_05);
         return FAILURE;
    }

    /* Label should not exceed 30 characters */
    if( 30 < strlen(vfsLabel) ){
         printf("createvfs_FAILURE %s \n",ERR_VFS_CREATE_07);
         return FAILURE;
    }

    /* Convert the file size into bytes */
    ui_size = ui_size * 1024;
    
    /* Total Blocks is total size divided by size of each block when size is in bytes */
    s_superBlock.ui_totalBlocks = ui_size/VFS_BLOCKSIZE;

    /* Validation Check If the file pointer can move to the end of the file or not 
    if((fseek(fpVfs,(long)s_superBlock.ui_totalBlocks*VFS_BLOCKSIZE,SEEK_SET)) != 1){
         printf("createvfs_FAILURE %s \n",ERR_VFS_CREATE_03);
         return FAILURE;
    }*/

    /* Set the file pointer to the initial position of the file
    if((fseek(fpVfs,(long)0,SEEK_SET)) == -1){
         printf("createvfs_FALURE %s \n",ERR_VFS_CREATE_03);
         return FAILURE;
    }*/

    /* Creating a file of the mentioned size by writing the total no.of.blocks */
    for(i = 0; i < s_superBlock.ui_totalBlocks; i++){
         memset((void *)&s_block,'\0',VFS_BLOCKSIZE);
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
    strcpy(s_inode.cptr_filePath,"/lost+found/");
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

#if SUPERBLOCK
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

    return SUCCESS;
}


/*
Function Name: i_mountvfs
Description: This function loaded the specified file system into the main 
             memory by representing the information of the file system
             with various data structures.
Parameters: It takes the name of the file system to be loaded.
Return Type: void
*/
int i_mountvfs(const char *vfsLabel){   
    
    struct mainHeader s_superBlock;
    struct mainHeader * sPtr_superBlock;

    int i_retVal = 0;

    FILE *fpVfs;

    if((fpVfs = fopen(vfsLabel,"r")) == NULL){
         printf("mountvfs_FAILURE %s \n",ERR_VFS_MOUNT_01);
         return FAILURE;      
    }

    if((fseek(fpVfs,0,SEEK_SET)) == -1){
         printf("mountvfs_FAILURE %s \n",ERR_VFS_MOUNT_02);
         return FAILURE;
    }

    if((i_retVal = fread(&s_superBlock,sizeof(struct mainHeader),1,fpVfs)) != 1){
         printf("mountvfs_FAILURE %s \n",ERR_VFS_MOUNT_03);
         return FAILURE;
    }

    cPtr_nameOfVfsMounted = (char *)malloc(sizeof(char)*(VFSLABELLEN+1));
    strcpy(cPtr_nameOfVfsMounted,vfsLabel);
    *(cPtr_nameOfVfsMounted + strlen(vfsLabel) + 1) = '\0';

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

    /* Load the file system into the n-Ary Tree */
    sPtr_rootNAryTree = s_loadFileSystem(VFS_ROOT_INODE,fpVfs,sPtr_superBlock);
   
    /* Load the hash table */
    v_initializeHashTable();
    v_traverseNAryTreeAux(sPtr_rootNAryTree,HASHING);

    /* Load the Binary Search Tree */
    v_traverseNAryTreeAux(sPtr_rootNAryTree,BST);

    return SUCCESS;
}

/*
Function Name:
Description:
Parameters:
Return Type:
*/
int i_unmountvfs(char *vfsLabel){

    FILE *fpVfs = NULL;

    if( 0 == strcmp(vfsLabel,cPtr_nameOfVfsMounted) ){
         if( NULL == (fpVfs = fopen(vfsLabel,"r")) ){
              printf("unmountvfs_FAILURE %s \n",ERR_VFS_UNMOUNT_01);
              return FAILURE;
         } else { 
              fclose(fpVfs);
         }
         v_traverseBST(sPtr_rootBST,UNMOUNT);
         v_traverseNAryTreeAux(sPtr_rootNAryTree,UNMOUNTVFS);
         /* BST does not have '/' (root) stored in it. So store the directory
            entries of '/'.
         */
         s_searchNAryTreeNode(sPtr_rootNAryTree,NULL,UNMOUNTVFS);
#if FILESYS_OPS
         v_traverseBST(sPtr_rootBST,UNMOUNTVERIFICATION);
         v_traverseNAryTreeAux(sPtr_rootNAryTree,UNMOUNTVFSVERIFICATION);
         s_searchNAryTreeNode(sPtr_rootNAryTree,NULL,UNMOUNTVFSVERIFICATION);
#endif         
    } else {
         printf("unmountvfs_FAILURE %s \n",ERR_VFS_UNMOUNT_04);
         return FAILURE;
    }

    return SUCCESS;
}

