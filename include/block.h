#ifndef _BLOCK_
#define _BLOCK_

/*
Group No : 28
Version No : 1.0
*/

/* This header file has the datatype definitiosn for the Blocks of VFS */

#include "global.h"

struct dataBlock{
    char ui_dataBlock[VFS_BLOCKSIZE];
};
#endif
