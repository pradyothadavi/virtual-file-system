#ifndef _BST_
#define _BST_

#include "fileSystemOps.h"
#include "nAryTree.h"

#define INODE 301
#define FILEPATH 302

struct binarySearchTree{
    char cArr_filePath[VFS_FILEPATHLEN];
    unsigned int ui_inodeNo;
    struct binarySearchTree *parent;
    struct binarySearchTree *leftChild;
    struct binarySearchTree *rightChild;
};

void v_loadBST(struct nAryTreeNode *);

struct binarySearchTree *s_insertBSTNode(struct binarySearchTree *,struct binarySearchTree *);

struct binarySearchTree *s_getBSTNode(char *,unsigned int);

void v_traverseBST(struct binarySearchTree *,int);

struct binarySearchTree *s_searchBSTNode(struct binarySearchTree *, char *);

void v_transplantBSTNode(struct binarySearchTree *,struct binarySearchTree *,struct binarySearchTree *);

void v_deleteBSTNode(struct binarySearchTree *, struct binarySearchTree *);

struct binarySearchTree *s_getLeftMostBSTNode(struct binarySearchTree *);

#endif
