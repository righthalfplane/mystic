#ifndef __NEWEDIT__
#define __NEWEDIT__


#include "EditList.h"

#include "fileDialog.h"
#include "uDialog.h"
#include "uLib.h"
#include "CExpress.h"

struct CEdit{
	uRect windowRect;
	unsigned long LastClick;
    char *CurrentFileName;
    long milliSecondsPerFrame;
    unsigned char *buffer;
    
    double scale;
/*		Variables		*/
	long lRowInsert,lColumnInsert;
	long fRowHigh,fColumnHigh;
	long lRowHigh,lColumnHigh;
	uRect dataRect;
	long CursorFlag;
	unsigned long CursorTime;
	long CursorRow;
	long CursorColumn;
	long DeleteN;
	char *DeleteBp;
	long DeleteRow;
	long DeleteColumn;
	int xPageSize;
	int yPageSize;
	long LineTop;
	long LineLeft;
	int LineHeight;
	int Descent;
	int CWidth;
    char *clipboard;
	EditPtr e;
	int TabSize;

	CExpressPtr Express;
	char controller[256];
	char path[2048];
	long CurrentFrame;
	long FrameFirst;
	long FrameLast;
	long FrameIncrement;
	char *pathname;
	
	long FrameCount;
	
	int defaultsSet;
	
	int LineCount;
	
	int ShowLineNumbers;
	
};

typedef struct CEdit *CEditPtr;

int CVideocColumn(IconPtr myIcon,long x,long row);
int CVideocToColumn(IconPtr myIcon,short x,long row);
void CVideoCursorInView(IconPtr myIcon);
void CVideodoCut(IconPtr myIcon);
void CVideoInsertAtCursorLong(IconPtr myIcon,char *data);
void CVideoCursorAtEnd(IconPtr myIcon);
int EditDrawIt(IconPtr myIcon);
void CVideoDrawRow(IconPtr myIcon,long CursorRow);   
IconPtr NewEdit(struct FileList *ret);
unsigned char *EditGetData(IconPtr myIcon);
#endif
