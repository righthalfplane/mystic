#ifndef __FILEMANAGER__
#define __FILEMANAGER__
#include "uFiles.h"
#include "BatchFile.h"

struct FileManagerStruct{
    FilePtr *Files; 
    long FileCount;
    long FileMax;
    
};

typedef struct FileManagerStruct *FileManagerPtr;

int FileManagerBatch(BatchPtr Batch,CommandPtr cp);

int FileManagerAdd(char *name,FilePtr Files,FileManagerPtr List);

FilePtr FileManagerGet(char *name);

int FileManagerCloseName(char *name);

int FileManagerClosePtr(FilePtr Files);

int FileManagerSendMessage(FilePtr Files,long MessageType,void *message);

int FileManagerExit(int flag);

int FileManagerAddToList(FilePtr Files,char *fileListName);

int FileManagerNotOwned(void);


#endif
