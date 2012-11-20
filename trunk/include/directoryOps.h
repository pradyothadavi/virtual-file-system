#ifndef _DIRECTORY_OPS_
#define _DIRECTORY_OPS_

void v_makedir(char *,char *);

void v_deletedir(char *);

void v_movedir(char *,char *);

void v_listdir(char *, int , char *);

struct hashTableNode *s_storeDirectoryNames(char *,struct nAryTreeNode *,struct hashTableNode *);
#endif
