#ifndef _FREE_LIST_
#define _FREE_LIST_

/*
Group No : 28
Version No : 1.0
*/

/* This header file has the datatype definitions for the FreeList */

#define FREELIST_DEBUG 0

struct freeList{
    unsigned int ui_blockNo;
    struct freeList *sPtr_nextFreeBlock;
};

struct freeList *s_createFreeList(int, int);

struct freeList *s_insertFreeListNode(struct freeList *,struct freeList *);

struct freeList *s_createFreeListNode(int);

int i_getFreeBlock(struct freeList *);

void v_deleteFreeListNode(struct freeList *, struct freeList *);

struct freeList * s_addFreeBlock(struct freeList *, int);

void v_displayList(struct freeList *);

#endif
