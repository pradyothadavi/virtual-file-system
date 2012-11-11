#ifndef _HASH_TABLE_
#define _HASH_TABLE_

/*
Group No: 028
Version No: 1.0
*/

#include "nAryTree.h"

#define NOOFBUCKETS 26

struct hashTableNode{
    char c_fileName[VFS_NAMELEN];
    int i_inodeNo;
    struct hashTableNode *sPtr_nextNode;
};

struct hashTableNode *hashTable[NOOFBUCKETS];

void v_initializeHashTable();

struct hashTableNode *s_createHashTableNode(char *,unsigned int);

void v_loadHashTable(struct nAryTreeNode *);

struct hashTableNode *s_insertIntoHashTable(struct hashTableNode *,char *,unsigned int);

#endif