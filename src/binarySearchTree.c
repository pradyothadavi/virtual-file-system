#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binarySearchTree.h"
 
/*
Function Name: s_insertBSTNode
Description: It inserts a node into the binary search tree.
Parameters: It takes 2 pointer parameters namely, the root of the BST into 
            which the node is to be inserted and pointer to the node.
Return Type: It returns the pointer to the root of the BST
*/
struct binarySearchTree *s_insertBSTNode(struct binarySearchTree *sPtr_root,
                                         struct binarySearchTree *sPtr_node){

    struct binarySearchTree *currentNode = NULL;
    struct binarySearchTree *previousNode = NULL;

    if(sPtr_root == NULL ){
         sPtr_root = sPtr_node;
         return sPtr_root;
    }
    currentNode = sPtr_root;

    while(currentNode != NULL){
         previousNode = currentNode;
         if( strcmp(sPtr_root->cArr_filePath,currentNode->cArr_filePath) < 0 ){
              currentNode = currentNode->leftChild;
         } else {
              currentNode = currentNode->rightChild;
         }
    }
    sPtr_root->parent = previousNode;

    if(NULL == previousNode){
         sPtr_root = sPtr_node;
    } else if( strcmp(sPtr_root->cArr_filePath,previousNode->cArr_filePath) < 0 ){
         previousNode->leftChild = sPtr_node;
    } else {
         previousNode->rightChild = sPtr_node;
    }

    return sPtr_root;
}

/*
Function Name: s_getBSTNode
Description: It creates a binary search tree node, with the specified filePath
             and inode number.
Parameters: It takes 2 parameters.
            1) FilePath
            2) Inode Number
Return Type: It returns a pointer to the new node created.
*/
struct binarySearchTree *s_getBSTNode(char *filePath,unsigned int ui_inodeNo){

    struct binarySearchTree *tempNode = NULL;

    tempNode = (struct binarySearchTree *)malloc(sizeof(struct binarySearchTree)*1);

    strcpy(tempNode->cArr_filePath,filePath);
    tempNode->ui_inodeNo = ui_inodeNo;
    tempNode->parent = NULL;
    tempNode->leftChild = NULL;
    tempNode->rightChild = NULL;

    return tempNode;
}

/*
Function Name: void v_traverseBST
Description: It traverses the Binary Search Tree in the Inorder fashion, from
             the node which is passed as a parameter to it. Based on the which
             mode the traversal is done, it prints either Inode Number or the 
             filePath.
Parameters: It takes a pointer to the node from which the traversal has to 
            begin.
            Mode = INODE, then prints Inode Numbers
            Mode = FILEPATH, then prints filePath
Return Type: void
*/
void v_traverseBST(struct binarySearchTree *sPtr_root,int mode){

    struct binarySearchTree *tempNode = NULL;

    tempNode = sPtr_root;
    if( tempNode != NULL){
         v_traverseBST(tempNode->leftChild,mode);
         if( INODE == mode ){
              printf("INODE NO: %d \n",tempNode->ui_inodeNo);
         }
         if( FILEPATH == mode ){
              printf("FILEPATH: %s \n",tempNode->cArr_filePath);
         }
         v_traverseBST(tempNode->rightChild,mode);
    }
}

/*
Function Name: s_searchBSTNode
Description: It searches for a node in the BST whose filePath is same as the 
             which is passed as the parameter.
Parameters: It takes 2 parameter, 
            1) A pointer to the root of the BST
            2) The filePath to be searched
Return Type: It a pointer to the node which filePath is same as the required
             filePath or NULL if no such filePath is found.
*/
struct binarySearchTree *s_searchBSTNode(struct binarySearchTree *sPtr_root,
                                         char *filePath){

    struct binarySearchTree *tempNode = NULL;

    if((tempNode == NULL ) || (strcmp(tempNode->cArr_filePath,filePath) == 0)){
         return tempNode;
    } 

    if( strcmp(filePath,tempNode->cArr_filePath) < 0 ){
         return s_searchBSTNode(tempNode->leftChild,filePath);
    } else {
         return s_searchBSTNode(tempNode->rightChild,filePath);
    }
}

/*
Function Name: v_transplantBSTNode
Description: It replaces the subtree rooted at node sPtr_u with the subtree
             rooted at node sPtr_v. Node sPtr_u's parent becomes node sPtr_v's 
             parent and sPtr_u's parent ends up having sPtr_v as its appropriate
             child.
Parameters: It takes 3 parameters
            1) Pointer to the subtree rooted at sPtr_u
            2) Pointer to a node u
            3) Pointer to a node v
Return Type: void
*/
void v_transplantBSTNode(struct binarySearchTree *sPtr_subTreeRoot,
                         struct binarySearchTree *sPtr_u,
                         struct binarySearchTree *sPtr_v){

    if( sPtr_u->parent == NULL ){
         sPtr_subTreeRoot = sPtr_v;
    } else if ( sPtr_u == sPtr_u->parent->leftChild ){
         sPtr_u->parent->leftChild = sPtr_v;
    } else {
         sPtr_u->parent->rightChild = sPtr_v;
    }

    if(sPtr_v != NULL){
         sPtr_v->parent = sPtr_u->parent;
    }
}

/*
Function Name: v_deleteBSTNode
Description: It deletes a node from Binary Search Tree
Parameters: It takes 2 parameters
            1) Pointer to the root of the BST
            2) Pointer to the node to be deleted
Return Type: void
*/
void v_deleteBSTNode(struct binarySearchTree *sPtr_root,
                     struct binarySearchTree *sPtr_nodeToBeDeleted){

    struct binarySearchTree *currentNode = NULL;

    if(sPtr_nodeToBeDeleted->leftChild == NULL){
         v_transplantBSTNode(sPtr_root,sPtr_nodeToBeDeleted,
                             sPtr_nodeToBeDeleted->rightChild);
    } else if ( sPtr_nodeToBeDeleted->rightChild == NULL ){
         v_transplantBSTNode(sPtr_root,sPtr_nodeToBeDeleted,
                             sPtr_nodeToBeDeleted->leftChild);
    } else {
         currentNode = s_getLeftMostBSTNode(sPtr_nodeToBeDeleted->rightChild);
         if( currentNode->parent != sPtr_nodeToBeDeleted){
              v_transplantBSTNode(sPtr_root,currentNode,currentNode->rightChild);
              currentNode->rightChild = sPtr_nodeToBeDeleted->rightChild;
              currentNode->rightChild->parent = currentNode;
         }
         v_transplantBSTNode(sPtr_root,sPtr_nodeToBeDeleted,currentNode);
         currentNode->leftChild = sPtr_nodeToBeDeleted->leftChild;
         currentNode->leftChild->parent = currentNode;
    }
}

/*
Function Name: s_getLeftMostBSTNode
Description: It finds the left most node of the BST from the subtree rooted
             at the pointer passed to it.
Parameters: It takes a single parameters.
            1) Pointer to the node from which the left most node is to be found.
Return Type: It returns a pointer to the left-most node in BST
*/
struct binarySearchTree *s_getLeftMostBSTNode(struct binarySearchTree *sPtr_node){
  
    struct binarySearchTree *tempNode = NULL;

    tempNode = sPtr_node;

    while( tempNode->leftChild != NULL ){
         tempNode = tempNode->leftChild;
    }

    return tempNode;
}
