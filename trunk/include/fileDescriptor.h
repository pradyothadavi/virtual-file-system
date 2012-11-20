#ifndef _FILE_DESCRIPTOR_
#define _FILE_DESCRIPTOR_

/*
Group No : 28
Version No : 1.0
*/

/* This header file has the datatype definitions for File Decriptors */

#define VFS_NAMELEN 32

#define VFS_FILEPATHLEN 32

#define VFS_DIRECT_DATABLOCK 16

#define FILE_CHARACTERS 2

struct fileDescriptor{
    char cptr_fileName[VFS_NAMELEN];
    char cptr_filePath[VFS_FILEPATHLEN];
    char c_fileType[FILE_CHARACTERS];
    unsigned int ui_inodeNo;
    unsigned int ui_fileSize;
    unsigned int ui_locationDataBlockNo[VFS_DIRECT_DATABLOCK];
};

struct fileDescriptor *s_createFileDescriptor(char *, char *, char, unsigned int, unsigned int, unsigned int);

#endif
