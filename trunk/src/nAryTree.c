#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nAryTree.h"
#include "fileSystemOps.h"
#include "freeList.h"
#include "binarySearchTree.h"
#include "global.h"

/*
Function Name: s_loadFileSystem
Description: It loads the file system into the main memory in the form of 
             n-Ary Tree.
Parameters: It takes a single parameter, which is the root inode number.
Return Type: It return a pointer which points to the root of the n-Ary Tree.
*/
struct nAryTreeNode * s_loadFileSystem(int i_rootInodeNo, FILE * fpVfs,struct mainHeader *s_superBlock){

    struct fileDescriptor s_inode;
    struct directoryEntry s_dientry;
    
    struct nAryTreeNode *root = NULL;
    struct nAryTreeNode *currentNode = NULL;
   
    int i_retVal = 0;
    
    if((fseek(fpVfs,i_rootInodeNo*VFS_BLOCKSIZE,SEEK_SET)) == -1){
         printf("ERROR: Cannot fetch the Inode Block \n");
    }

    if((i_retVal=fread(&s_inode,sizeof(struct fileDescriptor),1,fpVfs)) != 1){
         printf("ERROR: Cannot read a Data Block \n");
    }

    currentNode = s_createNAryTreeNode(i_rootInodeNo,fpVfs);
    if(NULL == root){
         root = s_insertNAryTreeNode(root,currentNode);
    }
    if(s_inode.c_fileType[0] == 'd'){
         if((fseek(fpVfs,s_inode.ui_locationDataBlockNo[0]*VFS_BLOCKSIZE,SEEK_SET)) == -1){
              printf("ERROR: Cannot fetch the Data Block \n");
         }
         
         do{
              if((i_retVal=fread(&s_dientry,sizeof(struct directoryEntry),1,fpVfs)) != 1){
                   printf("ERROR: Cannot read the dientry block \n");
              }
              if(s_dientry.ui_inodeNo > 0){
                   currentNode = s_createNAryTreeNode(s_dientry.ui_inodeNo,fpVfs);
                   root = s_insertNAryTreeNode(root,currentNode);
              }       
              
         }while(s_dientry.ui_inodeNo != 0);
    }
 
    root = s_loadFileSystemAux(root, fpVfs);

    return root;
}


/*
Funtion Name: s_loadFileSystemAux
Description: The function helps in building the subtree of the n-Ary Tree which
             represents the file system mounted
Parameters: It takes two pointers.
            a) Pointer to a node in the n-Ary Tree
            b) File pointer to the file system to be loaded into the main memory.
Return Type: It returns a pointer which points to the root of the n-Ary subtree.
*/
struct nAryTreeNode * s_loadFileSystemAux(struct nAryTreeNode *nodePtr, FILE *fpVfs){

    struct nAryTreeNode *temp;
    struct nAryTreeNode *currentNode;

    struct directoryEntry s_dientry;

    int i_retVal = 0;

    temp = nodePtr;

    /* Move One Level Down */
    if(temp->leftChild != NULL){
         temp = temp->leftChild;
       
         while( (temp->s_inode->c_fileType[0] != 'd') && (temp->rightSibling != NULL) ){
              temp = temp->rightSibling;
         }
        
         if( (temp->rightSibling == NULL) && (temp->s_inode->c_fileType[0] != 'd') ){
              return temp->parent;
         }else{
         
              if(temp->s_inode->c_fileType[0] == 'd'){
                   if((fseek(fpVfs,temp->s_inode->ui_locationDataBlockNo[0]*VFS_BLOCKSIZE,SEEK_SET)) == -1){
                        printf("ERROR: Cannot fetch the Data Block \n");
                   }
         
                   do{
                        if((i_retVal=fread(&s_dientry,sizeof(struct directoryEntry),1,fpVfs)) != 1){
                             printf("ERROR: Cannot read the dientry block \n");
                        }
                        if(s_dientry.ui_inodeNo > 0){
                             currentNode = s_createNAryTreeNode(s_dientry.ui_inodeNo,fpVfs);
                             temp = s_insertNAryTreeNode(temp,currentNode);
                        }          
   
                   }while(s_dientry.ui_inodeNo != 0);
              }
              temp = s_loadFileSystemAux(temp,fpVfs);
         }
    }

    return nodePtr;
}

/*
Funtion Name: s_createNAryTreeNode
Description: It creates a node of the n-Ary Tree and loads the required data 
             into the node.
Parameters: It takes two parameters.
            a) Inode Number of the file system
            b) File pointer pointing to the file system to be loaded into the
               main memory.
Return Type:It returns a pointer, pointing to the node created.
*/
struct nAryTreeNode * s_createNAryTreeNode(int i_inodeNo,FILE *fpVfs){

    struct nAryTreeNode *temp;
    struct fileDescriptor s_inode;

    int i_retVal = 0;
    int i_tempInodeNo = 0;

    i_tempInodeNo = i_inodeNo;

    if((fseek(fpVfs,i_tempInodeNo*VFS_BLOCKSIZE,SEEK_SET)) == -1){
         printf("ERROR: Cannot fetch the Inode Block \n");
    }

    if((i_retVal=fread(&s_inode,sizeof(struct fileDescriptor),1,fpVfs)) != 1){
         printf("ERROR: Cannot read a Data Block \n");
    }

    /* 
      Code to Create a temp node here in n-Ary Tree 
    */
    temp = (struct nAryTreeNode *)malloc(sizeof(struct nAryTreeNode)*1);
    temp->s_inode = (struct fileDescriptor *)malloc(sizeof(struct fileDescriptor)*1);

    strcpy(temp->s_inode->cptr_fileName,s_inode.cptr_fileName);
    strcpy(temp->s_inode->cptr_filePath,s_inode.cptr_filePath);
    strcpy(temp->s_inode->c_fileType,s_inode.c_fileType);
    temp->s_inode->ui_inodeNo = s_inode.ui_inodeNo;
    temp->s_inode->ui_fileSize = s_inode.ui_fileSize;
    /* Modify this line of Code */
    temp->s_inode->ui_locationDataBlockNo[0] = s_inode.ui_locationDataBlockNo[0];
    temp->parent = NULL;
    temp->leftChild = NULL;
    temp->rightSibling = NULL;

    return temp;
}

/*
Funtion Name: s_insertNAryTreeNode
Description: It inserts a node into the n-Ary Tree.
Parameters: It takes two parameters
            a) A pointer pointing to the root of the n-Ary Tree
            b) A pointer pointing to the current node being inserted.
Return Type: It return a pointer, pointing to the root the n-Ary Tree
*/
struct nAryTreeNode * s_insertNAryTreeNode(struct nAryTreeNode *root,struct nAryTreeNode *currentNode){

    struct nAryTreeNode *temp;
    struct nAryTreeNode *tempParent;

    temp = tempParent = root;
    /* Base Case */
    if(NULL == root){
         root = currentNode;
    } else {

         if(NULL == temp->leftChild){
              temp->leftChild = currentNode;
              currentNode->parent = temp;
              currentNode->rightSibling = NULL;
         } else {
              tempParent = temp;
              temp = temp->leftChild;
              while(temp->rightSibling != NULL){
                   temp = temp->rightSibling;
              }
              temp->rightSibling = currentNode;
              currentNode->parent = tempParent;
              currentNode->rightSibling = NULL; 
         }

    }

    return root;
}
/*
Function Name: v_traverseNAryTree
Description: It traverses the n-Ary Tree from the node pointed by the pointer
             which is passed as a parameter and prints the a field in the nAry
             tree based on a particular mode.
Parameters : It takes a 2 parameter.
             1) A pointer to a particular node in the n-Ary Tree.
             2) Mode of traversal
                FILENAME
                FILEPATH
                INODE
Return Type: It returns void
*/
void v_traverseNAryTree(struct nAryTreeNode *ptrToANode,int i_mode){
    struct nAryTreeNode *temp = NULL;

    temp = ptrToANode;

    if(NULL == temp){
         return;
    } else {
         v_traverseNAryTree(temp->leftChild,i_mode);
         if( i_mode == FILENAME ){
              printf("DEBUG: FILENAME : %s \n",temp->s_inode->cptr_fileName);
         } else if ( i_mode == FILEPATH ){
              printf("DEBUG: FILEPATH : %s \n",temp->s_inode->cptr_filePath);
         } else if ( i_mode == INODENUM ){
              printf("DEBUG: FD : %d \n",temp->s_inode->ui_inodeNo);
         }
         v_traverseNAryTree(temp->rightSibling,i_mode);
    }
}

/*
Function Name: v_traverseNAryTreeAux
Description: It traverses a n-Ary Tree based on the mode which is passed to it.
Parameters: It takes 2 parameters
            1) Pointer to the root of the n-Ary Tree
            2) Integers value named as mode, which specifies for what the n-Ary
               Tree is being traversed.
Return Type: void
*/
void v_traverseNAryTreeAux(struct nAryTreeNode *ptrToANode,int i_mode){

    struct nAryTreeNode *temp = NULL;
    struct binarySearchTree *tempNode = NULL;

    temp = ptrToANode;

    if(NULL == temp){
         return;
    } else {
         v_traverseNAryTreeAux(temp->leftChild,i_mode);
         if(HASHING == i_mode){
              if(temp->s_inode->c_fileType[0] == 'r'){
                   int ui_index = 0;
                   ui_index = ui_calculateHashIndex(temp->s_inode->cptr_fileName);
                   v_hashFile(ui_index,temp->s_inode->cptr_fileName,temp->s_inode->ui_inodeNo);
              }
         }
         if( BST == i_mode ){
              tempNode = s_getBSTNode(temp->s_inode->cptr_filePath,temp->s_inode->ui_inodeNo);
              sPtr_rootBST = s_insertBSTNode(sPtr_rootBST,tempNode);         
         }
         v_traverseNAryTreeAux(temp->rightSibling,i_mode);
    }

}

/*
Function Name: s_searchNAryTreeNode
Description: It searches for a given file in the n-Ary Tree
Parameters: It takes 2 parameters
            1) Pointer to the root of the n-Ary Tree
            2) Name of the file to be searched in the n-Ary Tree
Return Type: It return a pointer to the node if the file is found in the
             n-Ary Tree else NULL
*/
struct nAryTreeNode *s_searchNAryTreeNode(struct nAryTreeNode *ptrToANode,char *cPtr_fileName,int mode){

    struct nAryTreeNode *temp = NULL;
    struct nAryTreeNode *retNode = NULL;
    /* Assign temp to the root Node */
    temp = ptrToANode;

    if( RECURSIVE == mode){
         if( NULL == temp ){
              return NULL;
         }

         if( 0 == strcmp(temp->s_inode->cptr_fileName,cPtr_fileName) ){
              return temp;
         }
      
         temp = temp->leftChild;
         for( ;temp->rightSibling != NULL;temp = temp->rightSibling ){
              if( (retNode = s_searchNAryTreeNode(temp,cPtr_fileName,RECURSIVE)) != NULL ){
                   return temp;
              }
              if( retNode == NULL ){

              }
         }
    }
    
    if( NONRECURSIVE == mode ){
         
         if( NULL != temp && temp->leftChild != NULL){
              temp = temp->leftChild;
              if( (0 == strcmp(temp->s_inode->cptr_fileName,cPtr_fileName)) ){
                   return temp;
              }
         }
         temp = temp->rightSibling;
         while( temp != NULL){
              if( (0 == strcmp(temp->s_inode->cptr_fileName,cPtr_fileName)) ){
                   return temp;
              }
              temp = temp->rightSibling;
         }
    }

    return temp;
}

/*
Function Name: s_getNAryTreeNode
Description: It creates a node of the type n-Ary Tree
Parameters: void
Return Type: It returns a pointers to the newly created n-Ary Tree Node
*/
struct nAryTreeNode *s_getNAryTreeNode(){

    struct nAryTreeNode *temp = NULL;

    temp = (struct nAryTreeNode *)malloc(sizeof(struct nAryTreeNode)*1);
    temp->s_inode = (struct fileDescriptor *)malloc(sizeof(struct fileDescriptor)*1);

    return temp;
}

/*
Function Name: v_deleteNAryTreeNode
Description: It deletes a node in the n-Ary Tree and the subtree of the node
             deleted by this function will also be deleted using another
             auxillary function.
Parameters: It takes a single parameter.i.e. a pointer to the node to be deleted
Return Type: void
*/
void v_deleteNAryTreeNode(struct nAryTreeNode *ptrToANode){

    struct nAryTreeNode *ptrNodeToBeDeleted = NULL;
    struct nAryTreeNode *temp = NULL;

    ptrNodeToBeDeleted = ptrToANode;

    if( NULL == ptrNodeToBeDeleted){
         return;
    }

    /* Node to be deleted has the information about a regular file, then it 
       won't have any children.
    */
    if(ptrNodeToBeDeleted->s_inode->c_fileType[0] == 'r'){
         free(ptrNodeToBeDeleted->s_inode);
         free(ptrNodeToBeDeleted);
    } else if (ptrNodeToBeDeleted->s_inode->c_fileType[0] == 'd'){
         /* The directory to be deleted has sub-directories in it */
         if(ptrNodeToBeDeleted->leftChild != NULL){
              /* Auxillary Function which will delete the sub-directory */
              temp = s_deleteNAryTreeNodeAux(ptrNodeToBeDeleted->leftChild);
              ptrNodeToBeDeleted->leftChild = NULL;
              free(temp->s_inode);
              free(temp);
         }
         free(ptrNodeToBeDeleted->s_inode);
         free(ptrNodeToBeDeleted);
    }
}

/*
Function Name: s_deleteNAryTreeNodeAux
Description: It will delete the subtree of a node that is to be deleted in a 
             n-Ary Tree.
Parameters: It takes a single pointer pointing to the root of the subtree to 
            be deleted.
Return Type: It returns a pointer, pointing to the root of the subtree.
*/
struct nAryTreeNode *s_deleteNAryTreeNodeAux(struct nAryTreeNode *ptrToANode){

    struct nAryTreeNode *temp = NULL;
    struct nAryTreeNode *nodeToBeDeleted = NULL;

    temp = ptrToANode;
    
    if( (NULL == temp->rightSibling) && (NULL == temp->leftChild) ){
         return temp;
    } else if( NULL != temp->leftChild ) {
         nodeToBeDeleted = s_deleteNAryTreeNodeAux(temp->leftChild);
         temp->leftChild = NULL;
         free(nodeToBeDeleted->s_inode);
         free(nodeToBeDeleted);
    }else if( NULL != temp->rightSibling) {
         nodeToBeDeleted = s_deleteNAryTreeNodeAux(temp->rightSibling);
         temp->rightSibling = NULL;
         free(nodeToBeDeleted->s_inode);
         free(nodeToBeDeleted);     
    }
    
    return temp;
}

