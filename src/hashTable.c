#include <string.h>
#include <ctype.h>
#include "hashTable.h"

/*
Function Name: v_initializeHashTable
Description: It initializes the hash table with the required parameters
Parameters: It does not take any parameters
Return Type: It returns void
*/
void v_initializeHashTable(){

    int i = 0;
    char cArr_temp[2];

    for(i=0; i<= NOOFBUCKETS; i++){
         cArr_temp[0] = i+65;
         cArr_temp[1] = '\0';
         hashTable[i] = s_createHashTableNode(cArr_temp,i);
    }
}

/*
Function Name: s_createHashTableNode
Description: It creates a node of the type hashTable and stores the fileName
             and Inode number in it.
Parameters: It takes 2 parameters
            1) Character pointer a fileName
            2) Inode number of the fileName
Return Type: It return a pointer to the newly created node
*/
struct hashTableNode *s_createHashTableNode(char *cPtr_fileName,unsigned int ui_data){
    
    struct hashTableNode *tempNode = NULL;

    if( (tempNode=(struct hashTableNode *)malloc(sizeof(struct hashTableNode)*1)) == NULL){
         printf("ERROR: Insufficient memory, cannot create a node \n");
    } else {
         strcpy(tempNode->c_fileName,cPtr_fileName);
         tempNode->i_inodeNo = (int)ui_data;
         tempNode->sPtr_nextNode = NULL;
    }

    return tempNode;
}

/*
Function Name: v_loadHashTable
Description: It loads the hash table with all the files in the file system using
             the hash function.
Parameters: It takes inode number and fileName as input
Return Type: It return's void
*/
void v_loadHashTable(struct nAryTreeNode *root){

    v_traverseNAryTreeAux(root,HASHING);

}

/*
Function Name: ui_calculateHashIndex
Description: It takes the fileName to be hashed and calculates the hash index
             for the hash table.
Parameters: It takes the fileName as the parameter
Return Type: It returns the hash index
*/
unsigned int ui_calculateHashIndex(char *fileName){

    char c_firstCharacter = '\0';
    unsigned int ui_index = 0;

    c_firstCharacter = *fileName;

    ui_index = toascii(toupper(c_firstCharacter));

    ui_index = ui_index - 65;

    if(ui_index < 0 || ui_index > 25){
         ui_index = 26;
    }

    return ui_index;
}

/*
Function Name: v_hashFile
Description: It will hash a given file into the hash table
Parameters: It takes 3 parameters
            1) The hash index
            2) Name of the file to be hashed
            3) Inode number of the file Name to be hashed
Return Type: void
*/
void v_hashFile(unsigned int ui_index,char *cPtr_fileName, unsigned int ui_inodeNo){

    hashTable[ui_index]->sPtr_nextNode = s_insertIntoHashTable(hashTable[ui_index]->sPtr_nextNode,cPtr_fileName,ui_inodeNo);
}

/*
Function Name: s_insertIntoHashTable
Description: It hashes the fileName into the hash table.i.e. it inserts infront
             of the list which is present a each hash inex location.
Parameters: It takes 3 parameters
            1) Pointer to the list at a given hash index
            2) Name of the file to be hashed
            3) Inode number of the file to be hashed
Return Type: It returns a pointer pointing to the head of the list
*/
struct hashTableNode *s_insertIntoHashTable(struct hashTableNode *headPtr,char *cPtr_fileName,unsigned int ui_inodeNo){

    struct hashTableNode *tempNode = NULL;
    struct hashTableNode *currentNode = NULL;

    tempNode = s_createHashTableNode(cPtr_fileName,ui_inodeNo);
    currentNode = headPtr;
   
    if( NULL == currentNode ){
         currentNode = tempNode;
    } else {
         tempNode->sPtr_nextNode = currentNode->sPtr_nextNode;
         currentNode = tempNode;
    }

    return currentNode;
}

