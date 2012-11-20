#ifndef _GLOBAL_
#define _GLOBAL_

#include "nAryTree.h"
#include "freeList.h"
#include "binarySearchTree.h"


#define SUCCESS 1

#define FAILURE 0

#define SUPERBLOCK 1

#define VFS_UNIQUE_FILE_SYSTEM_NO 1316

#define VFS_INODE_STARTBLOCK 2

#define VFS_ROOT_INODE 4

#define VFS_BLOCKSIZE 1024

extern struct freeList *s_dataBlockFreeList;

extern struct freeList *s_inodeBlockFreeList;

extern struct nAryTreeNode *sPtr_rootNAryTree;

extern struct binarySearchTree *sPtr_rootBST;

extern char *cPtr_nameOfVfsMounted;

extern unsigned int ui_mountFlag;

#endif
