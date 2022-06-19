#ifndef _FILESEND_
#define _FILESEND_

struct uNetFile{
    struct FileList *Files;
    struct NetWork *Net;    
    char SendTarget[256];
    char SendPassword[256];
    int netErrorCheck;
    int SendPort;
    uRect windowRect;
    
    int SendMode;
    int idialog;
	struct Icon *myIcon;
	char *directory;
	
	int defaultsSet;

};

typedef struct uNetFile *netPtr;


IconPtr NewFileSendWindow(IconPtr myIconi);

int FileListFree(struct FileList *);


int FileDialogRemoveInfo(struct FileListInfo *info);

int FileDialogRecordInfo(struct FileListInfo *info);

int FileDialogRemoveInfoAll(void);

int SendFileList(netPtr net);

int SendMacBinaryFile(char *name,char *pathname,netPtr s);


#define FileDialog_GetFiles 		0
#define FileDialog_SetDirectory		1
#define FileDialog_SaveOne			2

#define FileCDraw 		9903	
#define EditCDraw 		9904	
#define FontCDraw 		9906	
#define StyleCDraw 		9907	

#define FileNew   				1
#define FileClose 				2		
#define savePreferencesCommand 	4
#define PreferencesCommand 		5
#define FileQuit  				6

#define EditCut    		3
#define EditCopy   		4
#define EditPaste  		5
#define EditClear  		6
#define EditInformation 8


#define B_STATUS		1
#define B_LOCAL			0

#define BinaryFile		0
#define TextFile		1
#define MacBinaryFile	2
#define RasterFile		3


#endif
