#ifndef _FILE_DESCRIPTOR_
#define _FILE_DESCRIPTOR_

/*
Group No : 28
Version No : 1.0
*/

/* This header file has the datatype definitions for File Decriptors */

#include "limits.h"
#include "block.h"
#include "fileSystemOps.h"

struct fileDescriptor{
    char cptr_fileName[VFS_NAMELEN];
    char cptr_filePath[VFS_NAMELEN];
    char c_fileType[FILE_CHARACTERS];
    unsigned int ui_inodeNo;
    unsigned int ui_fileSize;
    unsigned int ui_locationDataBlockNo[VFS_DIRECT_DATABLOCK];
};

struct fileDescriptor *s_createFileDescriptor(char *, char *, char, unsigned int, unsigned int, unsigned int);

#endif
