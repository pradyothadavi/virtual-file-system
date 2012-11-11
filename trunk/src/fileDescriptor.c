#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileDescriptor.h"

/*
Function Name: s_createFileDescriptor
Description: It takes all the values that must be stored in a file descriptor
             of the virtual file system.
Parameters: It takes 6 parameters
            1) File Name
            2) File Path
            3) File Type
            4) Inode Number
            5) File Size
            6) If the file is a directory then the dientry block number or
               data block number for any other file type.
Return Type: It returns a pointer to the structure of the file descriptor.
*/
struct fileDescriptor *s_createFileDescriptor(char *fileName, char *filePath,
                                        char fileType, unsigned int ui_inodeNo,
                                        unsigned int ui_fileSize,
                                        unsigned int ui_dientryBlock){

    struct fileDescriptor *tempInode = NULL;

    tempInode = (struct fileDescriptor *)malloc(sizeof(struct fileDescriptor)*1);

    strcpy(tempInode->cptr_fileName,fileName);
    strcpy(tempInode->cptr_filePath,filePath);
    tempInode->c_fileType[0] = fileType;
    tempInode->c_fileType[1] = '\0';
    tempInode->ui_inodeNo = ui_inodeNo;
    tempInode->ui_fileSize = ui_fileSize;
    tempInode->ui_locationDataBlockNo[0] = ui_dientryBlock;

    return tempInode;
}

