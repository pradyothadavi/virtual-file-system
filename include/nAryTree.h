#ifndef _N_ARY_TREE_
#define _N_ARY_TREE_

/*
Group No : 28
Version No : 1.0
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include "fileDescriptor.h"
#include "mainHeader.h"

#define HASHING 101

struct nAryTreeNode{
    struct fileDescriptor *s_inode;
    struct nAryTreeNode *parent;
    struct nAryTreeNode *leftChild;
    struct nAryTreeNode *rightSibling;
};

struct nAryTreeNode *s_loadFileSystem(int,FILE *,struct mainHeader *);

struct nAryTreeNode * s_loadFileSystemAux(struct nAryTreeNode *, FILE *);

struct nAryTreeNode * s_createNAryTreeNode(int,FILE *);

struct nAryTreeNode * s_insertNAryTreeNode(struct nAryTreeNode *,struct nAryTreeNode *);

struct nAryTreeNode *s_searchNAryTreeNode(struct nAryTreeNode *,char *);

struct nAryTreeNode *s_getNAryTreeNode();

struct nAryTreeNode *s_deleteNAryTreeNodeAux(struct nAryTreeNode *);

void v_deleteNAryTreeNode(struct nAryTreeNode *);

void v_traverseNAryTree(struct nAryTreeNode *);

void v_traverseNAryTreeAux(struct nAryTreeNode *,int);

unsigned int ui_calculateHashIndex(char *);

void v_hashFile(unsigned int,char *, unsigned int);

#endif
