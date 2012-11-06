#ifndef _GLOBAL_
#define _GLOBAL_

#include "nAryTree.h"
#include "freeList.h"

struct freeList *s_dataBlockFreeList;
struct freeList *s_inodeBlockFreeList;
struct nAryTreeNode *sPtr_rootNAryTree;

#endif
