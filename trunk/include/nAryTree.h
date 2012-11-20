#ifndef _N_ARY_TREE_
#define _N_ARY_TREE_

/*
Group No : 28
Version No : 1.0
*/
#include <stdlib.h>

#include "fileDescriptor.h"
#include "fileSystemOps.h"

#define NARY_DEBUG 0

#define HASHING 101
#define BST 102
#define NONRECURSIVEPRINT 103

#define RECURSIVE 201
#define NONRECURSIVE 202
#define UNMOUNTVFS 203
#define UNMOUNTVFSVERIFICATION 204

#define FILENAME 301
#define FILEPATH 302
#define INODENUM 303

struct nAryTreeNode{
    struct fileDescriptor *s_inode;
    struct nAryTreeNode *parent;
    struct nAryTreeNode *leftChild;
    struct nAryTreeNode *rightSibling;
};

struct nAryTreeNode *s_loadFileSystem(int,FILE *,struct mainHeader *);

void v_loadFileSystemAux(struct nAryTreeNode *, FILE *);

struct nAryTreeNode * s_createNAryTreeNode(int,FILE *);

struct nAryTreeNode * s_insertNAryTreeNode(struct nAryTreeNode *,struct nAryTreeNode *);

struct nAryTreeNode *s_searchNAryTreeNode(struct nAryTreeNode *,char *,int);

struct nAryTreeNode *s_getNAryTreeNode();

struct nAryTreeNode *s_deleteNAryTreeNodeAux(struct nAryTreeNode *);

void v_deleteNAryTreeNode(struct nAryTreeNode *);

void v_traverseNAryTree(struct nAryTreeNode *,int);

void v_traverseNAryTreeAux(struct nAryTreeNode *,int);

unsigned int ui_calculateHashIndex(char *);

void v_hashFile(unsigned int,char *, unsigned int);

#endif
