#ifndef _FILEDIALOG_
#define _FILEDIALOG_

#include "ulibTypes.h"

struct FileList{
    long ImageIncrement;
    char *directory;
    char **name;
    long count;
    long type;
    int donotfree;
    int loadToMemory;

};

struct FileListInfo{
    char *directory;
    char *name;
    uRect WindowRect;
    int initialized;
};

struct FileList *FileDialog(char *name,char *message,int Operation,struct FileListInfo *info);

int FileListFree(struct FileList *);

char *FileGetPath(char *message,struct FileListInfo *info);

char *FileGetFilePath(char *message,struct FileListInfo *info);

char *FileSaveFilePath(char *name,char *message,struct FileListInfo *info);

int GetTheFileName(char *name,char *message,int length,struct FileListInfo *info);

int GetTheSaveFileName(char *name,char *message,int length,struct FileListInfo *info);

int FileDialogRemoveInfo(struct FileListInfo *info);

int FileDialogRecordInfo(struct FileListInfo *info);

int FileDialogRemoveInfoAll(void);

struct FileList *FileDialog2(char *name);

int FilesSetType(struct FileList *Files);

#define FileDialog_GetFiles 		0
#define FileDialog_SetDirectory		1
#define FileDialog_SaveOne			2

#endif

