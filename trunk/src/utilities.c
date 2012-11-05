#include <stdio.h>
#include <string.h>
#include "fileDescriptor.h"

/*
Function Name:
Description:
Parameters:
Return Type:
*/
struct fileDescriptor *s_createFileDescriptor(char *fileName, char *filePath,
                                        char fileType, unsigned int ui_inodeNo,
                                        unsigned int ui_fileSize,
                                        unsigned int ui_dientryBlock){

    struct fileDescriptor *tempInode = NULL;

    strcpy(tempInode->cptr_fileName,fileName);
    strcpy(tempInode->cptr_filePath,filePath);
    tempInode->c_fileType[0] = fileType;
    tempInode->c_fileType[1] = '\0';
    tempInode->ui_inodeNo = ui_inodeNo;
    tempInode->ui_fileSize = ui_fileSize;
    tempInode->ui_locationDataBlockNo[0] = ui_dientryBlock;

    return tempInode;
}
