#ifndef _FILE_SYSTEM_OPS_
#define _FILE_SYSTEM_OPS_
/*
Group No : 28
Version No : 1.0
*/

#define DEBUG 1

#define VFS_NAMELEN 32
#define VFS_UNIQUE_FILE_SYSTEM_NO 1316
#define VFS_INODE_STARTBLOCK 2
#define VFS_ROOT_INODE 4
#define VFS_DIRECT_DATABLOCK 16
#define VFS_BLOCKSIZE 1024
#define VFS_INODE_INUSE 1
#define VFS_INODE_NOTUSED 0
#define VFS_DATABLOCK_INUSE 1
#define VFS_DATABLOCK_NOTUSED 0

struct directoryEntry{
    unsigned int ui_inodeNo;
    char c_fileName[VFS_NAMELEN];
};

void create_vfs(const char *,unsigned int);

void mount_vfs(const char *);

#endif


