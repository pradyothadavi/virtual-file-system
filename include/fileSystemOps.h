#ifndef _FILE_SYSTEM_OPS_
#define _FILE_SYSTEM_OPS_
/*
Group No : 28
Version No : 1.0
*/

#include "global.h"

#define FILESYS_DEBUG 0

#define VFS_INODE_INUSE 1

#define VFS_INODE_NOTUSED 0

#define VFS_DATABLOCK_INUSE 1

#define VFS_DATABLOCK_NOTUSED 0

#define VFSLABELLEN 30

struct mainHeader{
    char c_fileSystemLabel[VFS_NAMELEN];
    unsigned int ui_uniqueNo;
    unsigned int ui_maxNoOfFileDescriptors;
    unsigned int ui_noOfUsedFileDescriptors;
    unsigned int ui_totalBlocks;
    unsigned int ui_startBlockNoOfInodeArray;
    unsigned int ui_startBlockNoOfDataBlockArray;
};

struct directoryEntry{
    unsigned int ui_inodeNo;
    char c_fileName[VFS_NAMELEN];
};

int i_createvfs(char *,unsigned int);

int i_mountvfs(const char *);

int i_unmountvfs(char *);

#endif


