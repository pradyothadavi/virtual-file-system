#include "freeList.h"

/*
Function Name: s_createFreeList
Description: It create a list of free data blocks and inode blocks.
Parameters: It takes two parameters
            a) Starting Data Block Number
            b) Total No.of.Data Blocks
Return Type: It return a pointer which points to the starting of the list.
*/
struct freeList *s_createFreeList(int i_startingBlockNo, int i_totalBlocks){

    struct freeList *headPtr = NULL;
    struct freeList *tempNode = NULL;

    int i = 0;

    /* Create a head node which will keep the count of total free blocks available */
    tempNode = s_createFreeListNode(0);

    headPtr = tempNode;
    
    for(i=i_startingBlockNo; i<=i_totalBlocks; i++){
         
         tempNode = s_createFreeListNode(i);
         headPtr = s_insertFreeListNode(headPtr,tempNode);
         
    }

    return headPtr;
}

/*
Function Name: s_insertFreeListNode
Description: It inserts a newly created node to the list
Parameters: It takes 2 parameters
            1) Head pointer of the list to which the node is to be inserted
            2) Pointer to the node to be inserted
Return Type: It returns a pointer to the head of the list
*/
struct freeList *s_insertFreeListNode(struct freeList *headPtr,struct freeList *tempNode){

    struct freeList *currentNode = NULL;

    if(NULL == headPtr){
         printf("ERROR: Cannot insert the node into a 'NULL' list \n");
    } else {
         currentNode = headPtr;

         /* Increase head counter value by 1 */
         currentNode->ui_blockNo++;

         while( currentNode->sPtr_nextFreeBlock != NULL){
              currentNode = currentNode->sPtr_nextFreeBlock;
         }

         currentNode->sPtr_nextFreeBlock = tempNode;
    }

    return headPtr;
}
/*
Function Name: s_createFreeListNode
Description: It creates a node of the type Free List
Parameters: It takes a integer parameter, which is the free data block number.
Return Type: It returns a pointer to the newly created node.
*/
struct freeList *s_createFreeListNode(int i_data){

    struct freeList *tempNode = NULL;

    if( (tempNode=(struct freeList *)malloc(sizeof(struct freeList)*1)) == NULL){
         printf("ERROR: Insufficient memory, cannot create a node \n");
    } else {
         tempNode->ui_blockNo = i_data;
         tempNode->sPtr_nextFreeBlock = NULL;
    }

    return tempNode;
}
/*
Function Name: i_getFreeDataBlock
Description: It gives a free data block number from the list to the calling
             module. It also deletes the block number allocated from the list.
Parameters: It takes the head pointer of the list from which the free data block
            has to be allocated. It can either be head pointer of data block list
            or head pointer of inode block list.
Return Type: It returns free data block number.
*/
int i_getFreeBlock(struct freeList *headPtr){

    struct freeList *currentNode = NULL;
    
    int i_freeBlockNo = 0;
 
    if( (NULL == headPtr) || (0 == headPtr->ui_blockNo) ){
         return i_freeBlockNo;
    } else {

         currentNode = headPtr; 

         /* Decrement the value in the head node by 1 */
         currentNode->ui_blockNo--;

         currentNode = currentNode->sPtr_nextFreeBlock;
         i_freeBlockNo = currentNode->ui_blockNo;
         v_deleteFreeListNode(headPtr, currentNode);
         
    }

    return i_freeBlockNo;
}

/*
Function Name: v_deleteFreeListNode
Description: It deletes a node from the list.
Parameters: It takes 2 parameters
            1) Pointer to the head of the list from which the node is to be
               deleted.
            2) Pointer to the node which is to be deleted.
Return Type: void
*/
void v_deleteFreeListNode(struct freeList *headPtr, struct freeList *currentNode){

#if DEBUG
    printf("DEBUG: The deleted data block no: %d \n",currentNode->ui_blockNo);
#endif
    headPtr->sPtr_nextFreeBlock = currentNode->sPtr_nextFreeBlock;
    
    free(currentNode);
}
/*
Function Name: s_addFreeDataBlock
Description: It adds the data block number to the free list that was released.
Parameters: It takes the free data block number that has to be added to the
            free list.
Return Type: It returns the head pointer of the list to which the free data
             block is added.
*/
struct freeList * s_addFreeBlock(struct freeList *headPtr, int i_dataBlockNo){

    struct freeList *tempNode = NULL;

    tempNode = s_createFreeListNode(i_dataBlockNo);
#if DEBUG
    printf("DEBUG: No.of.data blocks present : %d \n",headPtr->ui_blockNo);
#endif
    headPtr = s_insertFreeListNode(headPtr,tempNode);

    return headPtr;
}

/*
Function Name: v_displayList
Description: It displays the contents of the list
Parameters: It takes a single parameter, which is a pointer to the head of list
Return Type: void
*/
void v_displayList(struct freeList *headPtr){

    struct freeList *currentNode = NULL;

    currentNode = headPtr;

    printf("DEBUG: No.of.nodes present: %d \n",currentNode->ui_blockNo);

    currentNode = currentNode->sPtr_nextFreeBlock;

    printf("Free List Block No : ");
    while(currentNode != NULL){
         printf("%d ",currentNode->ui_blockNo);
         currentNode = currentNode->sPtr_nextFreeBlock;
    }
    printf("\n");
}
