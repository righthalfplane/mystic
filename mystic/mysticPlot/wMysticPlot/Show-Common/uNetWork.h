
#ifndef __UNETWORK__
#define __UNETWORK__

#include "fileDialog.h"
#include "Net.h"

struct uNetWork{
    struct FileListInfo infoSave;
	struct FileInfo2 *Files;
    struct NetWork *Net;
    char ListenPassword[256];
    char SendPassword[256];
    char SendTarget[256];
    int ListenPort;
    int SendPort;
    int Connected;
	int ListenFlag;
	int SaveHdfToMemory;
	int SaveHdfToDisk;
	int SaveTrueToMemory;
	int SaveTrueToDisk;
	int AppendHdfToMemory;
    char TEXTOwner[5];
    char TEXTType[5];
    char BinaryOwner[5];
    char BinaryType[5];
    int mCount;
    double mXscale;
    double mYscale;
    int netErrorCheck;
	char *pathname;
	char paletteName[256];
	char palette[768];
	int igotPalette;
	char *limits;
};

int uNetStart(struct Icon *myIcon);
int uNetStop(struct Icon *myIcon);
int uNetStartListen(struct Icon *myIcon);
int uNetStopListen(struct Icon *myIcon);
int uNetSetStartListen(struct Icon *myIcon);
int uNetCheckConnection(struct Icon *myIcon);

int uSendDirectoryName(struct NetWork *Net,char *buff);

int uNetListenStop(struct uNetWork *uNet);

int WarningBatchHold(char *buff);

int WarningBatchHoldDump(void);

#endif
