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
Function Name:
Description:
Parameters:
Return Type:
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
Function Name:
Description:
Parameters:
Return Type:
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
Function Name:
Description:
Parameters:
Return Type:
*/
void v_hashFile(unsigned int ui_index,char *cPtr_fileName, unsigned int ui_inodeNo){

    hashTable[ui_index]->sPtr_nextNode = s_insertIntoHashTable(hashTable[ui_index]->sPtr_nextNode,cPtr_fileName,ui_inodeNo);
}

/*
Function Name:
Description:
Parameters:
Return Type:
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

