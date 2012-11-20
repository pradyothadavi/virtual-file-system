#ifndef _BST_
#define _BST_

#define BST_DEBUG 0

#define INODE 301
#define FILEPATH 302
#define UNMOUNT 303
#define UNMOUNTVERIFICATION 304

#include "global.h"
#include "nAryTree.h"

struct binarySearchTree{
    char cArr_filePath[VFS_FILEPATHLEN];
    unsigned int ui_inodeNo;
    struct binarySearchTree *parent;
    struct binarySearchTree *leftChild;
    struct binarySearchTree *rightChild;
};

struct binarySearchTree *s_insertBSTNode(struct binarySearchTree *,struct binarySearchTree *);

struct binarySearchTree *s_getBSTNode(char *,unsigned int);

void v_traverseBST(struct binarySearchTree *,int);

struct binarySearchTree *s_searchBSTNode(struct binarySearchTree *, char *);

void v_transplantBSTNode(struct binarySearchTree *,struct binarySearchTree *,struct binarySearchTree *);

void v_deleteBSTNode(struct binarySearchTree *, struct binarySearchTree *);

struct binarySearchTree *s_getLeftMostBSTNode(struct binarySearchTree *);

void v_storeDirectoryEntries(struct binarySearchTree *,int);

#endif
