#ifndef _GLOBAL_
#define _GLOBAL_

#include "nAryTree.h"
#include "freeList.h"

extern struct freeList *s_dataBlockFreeList;
extern struct freeList *s_inodeBlockFreeList;
extern struct nAryTreeNode *sPtr_rootNAryTree;

#endif
