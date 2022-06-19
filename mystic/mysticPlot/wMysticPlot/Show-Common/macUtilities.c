#define EXTERN22 extern
#include <ctype.h>
#include "Xappl.h"
#include "uDialog.h"

#include "uFiles.h"
#include "CExpress.h"
#include "GWorld.h"
#include "NewEdit.h"
#include "Scanfile.h"

extern IconPtr  doNewRenderWindow();

struct searchInfo{
    struct FileInfo2 *Files;
    IconPtr myIcon;
    IconPtr edit;
	char *string,*stringfilename;
	int stringmax,stringcount,character,stringfound;
	int OncePerFile,IgnoreCase,FoundIt;
	int skipBinary;
	long linecount;
	long Start,End,Directories,FilesSearched,Bytes,Hits,LinesSearched;
	char *buffin;
	char charb[256];
	char *dirpathname;
	IconPtr StatusWindow;
	int doExit;
};

typedef struct searchInfo *searchPtr;

int TextFileConvert(char *filename,char *path,int outType);

int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);

static int GrayToHdf(IconPtr myIcon);

static int RecursiveFileConvert(char *name,char *directory,IconPtr myIcon,int typeOS,int recursive);

static int doConvert(IconPtr myIcon);

static void tMoveControls(IconPtr myIcon);

static int CheckMenu(IconPtr myIcon,int flag);

static int testForBinary(unsigned char *buffin,long nt,searchPtr r);

#define IO_BUFFER2	100000

int OpenViewLib(struct FileList *Files);

int doLoadImages2(IconPtr myIcon,struct FileList *Files);

char *strpathname(char *path,char *name);

long FileLength(FILE *out);

int getString(unsigned char *s,long n,FILE *in);

int OpenTek(struct FileList *Files);

int isDirectory(char *path);

static int SearchForString(char *name,char *pathname,IconPtr myIcon);

static int SearchFileForString(char *name,char *pathname,IconPtr myIcon);



int GetNameDirID(char *name,short *vRefNum,long *dirIDToSearch);

#define File_Menu  		7000
#define About_Item 		1
#define Open_Item 		2
#define Find_Item 		3
#define Close_Item 		4
#define Quit_Item 		5
#define New_Item 		6
#define Convert_Item 	7
#define Teos2sds_Item 	8
#define GrayToHdf_Item 	9


#define Edit_Menu  		7001
#define Frame_Item		1
#define Options_Item	2
#define View_Item		3
#define Scales_Item		4


static int doClose(IconPtr myIcon);
static int doMenus(IconPtr myIcon,int menu,int item);
static int UpdateWrite(IconPtr myIcon);
static int doConvertQuickTime(IconPtr myIcon);

int stringsearch(char *buffin,long nt,searchPtr r);
static int screen(unsigned char *buf,int n,searchPtr r);


int Teos2sds(struct FileInfo2 *Files,long n);

IconPtr  macUtilities(IconPtr myIconIn);

IconPtr  macUtilities(IconPtr myIconIn)
{
	static struct uMenuItems File[]={
	    {"File",  		File_Menu,uWindowStandardMenu,0,},
	    {"New                 /N",		New_Item,0,0,},
	    {"Close               /W",		Close_Item,0,0,},
	    {"----------------------", 	About_Item,0,0,},
	    {"Find In Files...    /F",	Find_Item,0,0,},
	    {"Text File Convert.../T",	Convert_Item,0,0,},
	    /*
	    {"Flatten QuickTime... ",	Open_Item,0,0,},
	    */
	    {"View TEOS File...   /E",	Teos2sds_Item,0,0,},
	    /*
	    {"Gray To HDF...        ",	GrayToHdf_Item,0,0,},
	    */
	    {"Quit                /Q",	Quit_Item,0,0,},
	    {NULL,0,0,0,},
	};
/*	
	static struct uMenuItems Edit[]={
	    {"Edit",  			Edit_Menu,uWindowStandardMenu,0,},
	    {"Frame Image", 		Frame_Item,0,0,},
	    {"Options...",		Options_Item,0,0,},
	    {"View...",			View_Item,0,0,},
	    {"Scales",			Scales_Item,0,0,},
	    {NULL,0,0,0,},
	};
	
*/	
	static struct uMenuList menuList[]={
	    {File},
	    {NULL}
	};
	
	IconPtr myIcon;
	uRect Rect={40,40,400,200};
	char buff[256];
	static int Count;
	searchPtr r;
	int ret;
		
    ret=1;
	if(!(myIcon=uCreateIcon(UtilityType,&Rect))){
		WarningBatch("macUtilities uCreateIcon Failed\n");
		goto ErrorOut;
	}
	uSetCursor(myIcon,uWATCH_CURSOR);
	
	if(!(r=(searchPtr)cMalloc((long)sizeof(struct searchInfo),8202))){
	    WarningBatch("macUtilities Out of Memory\n");
	    goto ErrorOut;
	}
	
	zerol((char *)r,(long)sizeof(struct searchInfo));
	
	r->IgnoreCase=TRUE;   
	r->OncePerFile=FALSE;
	
	myIcon->DWindow=(struct DumWindow *)r;
	myIcon->pClose=doClose;
	myIcon->pMenu=doMenus;
	myIcon->pUpdateWrite=UpdateWrite;
	myIcon->pCheckMenu=CheckMenu;
	myIcon->pMoveControls=tMoveControls;
	
	sprintf(buff,"UtilitiesWindow.%03d",Count++);
	
	if(uSetWindow(myIcon,	       
	              uWindowSetWindowName,buff,
				  uWindowSetMenuList,menuList,				  
	              uWindowSetDone,uWindowSetDone)
				  )goto ErrorOut;
				  
	r->myIcon=myIcon;

	SetWindowPath(myIconIn,myIcon);
	
    ret=0;
ErrorOut:
    if(ret){
        if(myIcon)doClose(myIcon);
	}else{
		if(myIcon){
		    myIcon->isOpen=TRUE;
		    tMoveControls(myIcon);
		}
	}

	uSetCursor(myIcon,uARROW_CURSOR);
	return myIcon;
}
static void tMoveControls(IconPtr myIcon)
{
	
	controlPtr control;
	uRect r;
	int bottom;
	int right;	
	int top;
	int left;
	
	if(!myIcon)return;
	
	uGetPortBounds(myIcon,&r);
	
	top=0;
	left=0;
	bottom=r.ysize;
	right=r.xsize;
	
	
	control=myIcon->VControl;
	if(control){
		MoveControl(control,right-ScrollOffSet,top+MenuOffSet);
		SizeControl(control,ScrollOffSet,bottom-top-ScrollOffSet-MenuOffSet);
		uSetControlMaximum(control,0L);
	}
		
	control=myIcon->HControl;	
	if(control){
		MoveControl(control,left,bottom-ScrollOffSet);
		SizeControl(control,right-(ScrollOffSet),ScrollOffSet);
		uSetControlMaximum(control,0L);
	}
	
	InvalRectMyWindow(myIcon);
}
static int CheckMenu(IconPtr myIcon,int flag)
{


	if(!myIcon)return 1;
		
	if(flag){	
		uActivateMenuItem(myIcon,File_Menu,Open_Item,isInstalled(Installed_QuickTimeFlatten));	    
	}
	return 0;
}
static int UpdateWrite(IconPtr myIcon)
{
	searchPtr r;
	
	if(!myIcon || !myIcon->isOpen)return 1;
	r=(searchPtr)myIcon->DWindow;
	if(!r)return 1;
	
	uDrawControls(myIcon);
	
	return 0;
}
static int doConvert(IconPtr myIcon)
{
	double start,end;
	int TypeList[]={OPEN_ALL_FILE_TYPES,0};
	int toMAC=1,toWIN=0,toUNIX=0,typeOS=2,recursive=0;
	char *pathfilename;
	IconPtr StatusWindow;
	searchPtr r;
	int ret;
	static uDialogGroup Items[] =
        {
      /*  0 */      {"Ok", {20,190,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel",{150,190,65,20},uDialogButton,uDialogNone},      
      /*  2 */      {"Convert text file end-of-line markers to:",{20,20,200,20},uDialogString,uDialogNone},
      /*  3 */      {"Macintosh format (CR)", {20,50,180,20},uDialogButtonRadio,uDialogGetLong},
      /*  4 */      {"Windows format (CR-LF)",{20,80,180,20},uDialogButtonRadio,uDialogGetLong},
      /*  5 */      {"Unix format (LF)",{20,110,150,20},uDialogButtonRadio,uDialogGetLong},
      /*  6 */      {"Recursive",{20,150,150,20},uDialogCheck,uDialogGetLong},
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
 	static uRect  Rect1   = {70,90,250,240};
	struct FileList *Files;
 	uDialogList *uList;
	uDialogReturn *uR;
	int n;
	uPort save;
	


	if(!myIcon)return 1;
	r=(searchPtr)myIcon->DWindow;
	if(!r)return 1;
	
	uGetPort(&save);
	
	uR=NULL;
	
	Files=NULL;
	
	pathfilename = NULL;

	StatusWindow=NULL;
		
	uList=uDialogOpen("Text File Convert",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	ret=uDialogSet(uList,0L,
	              
	              uDialogSetMyIcon,myIcon,
	              
	          	  uDialogSetItem,3L,
	          	  uDialogSetLogical,toMAC,
	          	  
	          	  uDialogSetItem,4L,
	          	  uDialogSetLogical,toWIN,

	          	  uDialogSetItem,5L,
	          	  uDialogSetLogical,toUNIX,

	          	  uDialogSetItem,6L,
	          	  uDialogSetLogical,recursive,

	              uDialogSetDone,uDialogSetDone
	    );
	if(ret)goto ErrorOut;
	
	if(uDialog(uList))goto ErrorOut;
	
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
	
	if ((uR[3].lreturn==1) && (uR[4].lreturn==0) && (uR[5].lreturn==0)) {
		typeOS = 2;
	} else if ((uR[3].lreturn==0) && (uR[4].lreturn==1) && (uR[5].lreturn==0)) {
		typeOS = 1;
	} else if ((uR[3].lreturn==0) && (uR[4].lreturn==0) && (uR[5].lreturn==1)) {
		typeOS = 3;
	} else {
		return 1;
	}

	recursive = (int)uR[6].lreturn;
	
    if(uList)uDialogClose(uList);
    uList=NULL;
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;	
	
	
	Files=uOpenFileDialog(myIcon,TypeList);		
	if(!Files)goto ErrorOut;
	    
	    	
	start=rtime();

	StatusWindow=uStartStatusWindow(myIcon,"Convert Text Files");
	r->StatusWindow=StatusWindow;
	
	for(n=0;n<Files->count;++n){
		RecursiveFileConvert(Files->name[n],Files->directory,myIcon,typeOS,recursive);
	}	
		
	end=rtime();
	
	sprintf(WarningBuff,"Time To Convert File(s) - Seconds %.2f",end-start);
	WarningBatchFast(WarningBuff);
	
	WarningBatchAtEnd(WarningBuff);
	
	
ErrorOut:	
	if (pathfilename) cFree(pathfilename);
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	if(r->buffin)cFree(r->buffin);
	r->buffin=NULL;
	uStopStatusWindow(StatusWindow);
	if(Files)FileListFree(Files);
	uSetPort(&save);
	return 0;
}
static int RecursiveFileConvert(char *name,char *directory,IconPtr myIcon,int typeOS,int recursive)
{	
	struct scanList *filelist;
	char *pathname,message[256],*namep;
	searchPtr r;
	long n;
	int ret;

	if(!directory || !myIcon)return 1;
	r=(searchPtr)myIcon->DWindow;
	if(!r)return 1;
	
	ret = 1 ;

	filelist=NULL;
	
	pathname=strpathname(directory,name);
	if(!pathname)goto ErrorOut;
	
	if(isDirectory(pathname)){
		if (recursive) {
			++r->Directories;
			filelist=scan_dir(pathname,".*");
			if(filelist == NULL){
				sprintf(WarningBuff,"SendFileName: scan_dir Error");
				WarningBatchFast(WarningBuff);
				goto ErrorOut;
			}
			sprintf(WarningBuff,"Searching Directory %s",pathname);
			uWriteStatusWindow(r->StatusWindow,WarningBuff);
			for(n=0;n<filelist->countDirs;++n){
				if(RecursiveFileConvert(filelist->namesDirs[n],pathname,myIcon,typeOS,recursive))goto ErrorOut;
			}
			for(n=0;n<filelist->countFiles;++n){
				if(!TextFileConvert(filelist->namesFiles[n],pathname,typeOS)) {
					mstrncpy(message,"-->Skip ",sizeof(message));
					namep=strpathname(pathname,filelist->namesFiles[n]);
					if(namep){
					    mstrncat(message,namep,sizeof(message));
					    cFree(namep);
					}
					mstrncat(message," ",sizeof(message));
					WarningBatchFast(message);
				} else {
				    message[0]=0;
					namep=strpathname(pathname,filelist->namesFiles[n]);
					if(namep){
						mstrncpy(message,namep,sizeof(message));
						cFree(namep);
					}
					mstrncat(message," ",sizeof(message));
					WarningBatchFast(message);
				}
			}
		}
	}else{
		if(!TextFileConvert(name,directory,typeOS)) {
			mstrncpy(message,"-->Skip ",sizeof(message));
			namep=strpathname(pathname,name);
			if(namep){
			    mstrncat(message,namep,sizeof(message));
			    cFree(namep);
			}
			mstrncat(message," ",sizeof(message));
			WarningBatchFast(message);
		} else {
			message[0]=0;
			namep=strpathname(pathname,name);
			if(namep){
				mstrncpy(message,namep,sizeof(message));
				cFree(namep);
			}
			mstrncat(message," ",sizeof(message));
			WarningBatchFast(message);
		}
	}
	ret = 0 ;
ErrorOut:
    if(pathname)cFree((char *)pathname);
	if(filelist)free_dir(filelist);
	return ret;
}

static int doTeos2sds(IconPtr myIcon)
{
	double start,end;
	int TypeList[]={SAGEEOS_DISK,SAGE_DUMP,0};
	char *pathname,message[256];
	IconPtr StatusWindow;
	searchPtr r;
	int ret;
	static uDialogGroup Items[] =
        {
      /*  0 */      {"Ok", {20,180,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel",{150,180,65,20},uDialogButton,uDialogNone},      
	  /*  2 */      {"TEOS View:",{20,20,200,20},uDialogString,uDialogNone},
	  /*  3 */      {"Rho(Prs,Tev), SIE(Prs,Tev)",{30,40,200,20},uDialogString,uDialogNone},
	  /*  4 */      {"X-axis is log10(Prs)",{30,60,200,20},uDialogString,uDialogNone},
	  /*  5 */      {"Y-axis is log10(Tev)",{30,80,200,20},uDialogString,uDialogNone},
	  /*  6 */      {"One pixel per table data point",{30,100,200,20},uDialogString,uDialogNone},
	  /*  7 */      {"Select TEOS file...",{20,140,200,20},uDialogString,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
 	static uRect  Rect1   = {70,90,250,240};
	struct FileList *FileIn;
    struct FileInfo2 *Files;
 	uDialogList *uList;
	uDialogReturn *uR;
	int n;
	uPort save;

	if(!myIcon)return 1;
	r=(searchPtr)myIcon->DWindow;
	if(!r)return 1;
	
	uGetPort(&save);
	
	uR=NULL;
	Files=NULL;
	FileIn=NULL;
	pathname = NULL;
	StatusWindow=NULL;
		
	uList=uDialogOpen("View TEOS File",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	ret=uDialogSet(uList,0L,
	              
	              uDialogSetMyIcon,myIcon,
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(ret)goto ErrorOut;
	
	if(uDialog(uList))goto ErrorOut;
	
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
	
    if(uList)uDialogClose(uList);
    uList=NULL;
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;	
	
	
	FileIn=uOpenFileDialog(myIcon,TypeList);		
	if(!FileIn)goto ErrorOut;
	Files=FilesOpenList(FileIn);
	if(!Files)goto ErrorOut;
	    	    	
	start=rtime();

	StatusWindow=uStartStatusWindow(myIcon,"View TEOS File");
	r->StatusWindow=StatusWindow;
	
	for(n=0;n<Files->FileCount;++n){
		pathname=strpathname(Files->directory,Files->FilesNames[n]);
		if(!pathname)goto ErrorOut;

		if(isDirectory(pathname)){
				mstrncpy(message,"Skip directory --> ",sizeof(message));
				mstrncat(message,pathname,sizeof(message));
				mstrncat(message," ",sizeof(message));
				WarningBatchFast(message);
		} else{
			if(Teos2sds(Files,n)) {
				mstrncpy(message,"Failed to process ",sizeof(message));
				mstrncat(message,pathname,sizeof(message));
				mstrncat(message," ",sizeof(message));
				WarningBatchFast(message);
			} else {
				message[0]=0;
				mstrncpy(message,pathname,sizeof(message));
				mstrncat(message," ",sizeof(message));
				WarningBatchFast(message);
			}
		}
	}	

	end=rtime();

	sprintf(WarningBuff,"Time To Get TEOS File(s) - Seconds %.2f",end-start);
	WarningBatchFast(WarningBuff);

	WarningBatchAtEnd(WarningBuff);


ErrorOut:	
    if(pathname)cFree((char *)pathname);
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	if(r->buffin)cFree(r->buffin);
	r->buffin=NULL;
	uStopStatusWindow(StatusWindow);
	if(Files)freeFileList(Files);
	uSetPort(&save);
	return 0;
}
static int doFind(IconPtr myIcon)
{
	static char string[256]={'h','d','f','\0'};
	char buffstring[256];
	double start,end;
	char buff[256];
	int TypeList[]={OPEN_ALL_FILE_TYPES,0};
	
	IconPtr StatusWindow;
	
	searchPtr r;
	int ret;
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {320,81,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel",{400,81,65,20},uDialogButton,uDialogNone},      
      /*  2 */      {"Search in files for the following text string:",{20,15,400,20},uDialogString,uDialogNone},
      /*  3 */      {string,{20,40,460,20},uDialogText,uDialogNone},
      /*  4 */      {"Ignore case", {20,85,111,20},uDialogCheck,uDialogGetLong},
      /*  5 */      {"First occurrence only",{140,85,180,20},uDialogCheck,uDialogGetLong},
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,500,130};
	struct FileList *Files;
 	
 	uDialogList *uList;
	uDialogReturn *uR;
	int n,c;
	uPort save;
	
	if(!myIcon)return 1;
	r=(searchPtr)myIcon->DWindow;
	if(!r)return 1;
	
	uGetPort(&save);
	
	uR=NULL;
	
	Files=NULL;
	
	StatusWindow=NULL;
	
	uList=uDialogOpen("Find in files",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	ret=uDialogSet(uList,0L,
	              
	              uDialogSetMyIcon,myIcon,
	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,r->IgnoreCase,
	              
	              uDialogSetItem,5L,
	              uDialogSetLogical,r->OncePerFile,

	              uDialogSetDone,uDialogSetDone
	    );
	if(ret)goto ErrorOut;
	
	if(uDialog(uList))goto ErrorOut;
	
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;

	mstrncpy(string,uR[3].sreturn,256);
	r->string=string;
	r->IgnoreCase  = (int)uR[4].lreturn;
	r->OncePerFile = (int)uR[5].lreturn;
	
    if(uList)uDialogClose(uList);
    uList=NULL;
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
        
	 r->stringmax=(int)strlen(string);
	
	if(r->IgnoreCase){
	    for(n=0;n<r->stringmax;++n){
	        c=string[n];
	        if(c >= 'a' && c <= 'z'){
	            c='A'+(c-'a');
	        }	        
	        buffstring[n]=c;
	    }
	    buffstring[r->stringmax]=0;
	    r->string = buffstring;
	}   
	
	
	r->skipBinary=TRUE;
	r->doExit=FALSE;
	r->Bytes=0;
	r->Hits=0;
	r->LinesSearched=0;
    r->edit=NULL;
	r->Directories=0;
	r->FilesSearched=0;
	r->linecount=0;
	r->LinesSearched=0;
	r->buffin=cMalloc(IO_BUFFER2,92872);
	if(!r->buffin)goto ErrorOut;
	    
	Files=uOpenFileDialog(myIcon,TypeList);		
	if(!Files)goto ErrorOut;
	    
	    	
	start=rtime();

	StatusWindow=uStartStatusWindow(myIcon,"Search For String");
	r->StatusWindow=StatusWindow;
	
	for(n=0;n<Files->count;++n){
	    if(SearchForString(Files->name[n],Files->directory,myIcon)){
	        if(r->doExit)break;
	        continue;
	    }
	}	
		
	end=rtime();
	
	sprintf(buff,
	  "Directories Searched: %ld  Files Searched: %ld  Total Bytes: %ld  Lines Searched: %ld Hits : %ld Seconds %.2f",
	        r->Directories,r->FilesSearched,r->Bytes,r->LinesSearched,r->Hits,end-start);
	        
	screen((unsigned char *)buff,(int)strlen(buff),r);
	CVideoCursorAtEnd(r->edit);
ErrorOut:	
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	if(r->buffin)cFree(r->buffin);
	r->buffin=NULL;
	uStopStatusWindow(StatusWindow);
	if(Files)FileListFree(Files);
	uSetPort(&save);
	return 0;
}
static int SearchForString(char *name,char *directory,IconPtr myIcon)
{	
	struct scanList *filelist;
	char *pathname;
	searchPtr r;
	long n;
	int ret;

	if(!directory || !myIcon)return 1;
	r=(searchPtr)myIcon->DWindow;
	if(!r)return 1;
	
	ret = 1 ;

	filelist=NULL;
	
	pathname=strpathname(directory,name);
	if(!pathname)goto ErrorOut;
	
	if(isDirectory(pathname)){
		++r->Directories;
	    filelist=scan_dir(pathname,".*");
	    if(filelist == NULL){
	        sprintf(WarningBuff,"SendFileName: scan_dir Error\n");
	        WarningBatch(WarningBuff);
			goto ErrorOut;
	    }
		sprintf(WarningBuff,"Searching Directory %s",pathname);
		uWriteStatusWindow(r->StatusWindow,WarningBuff);
	    for(n=0;n<filelist->countDirs;++n){
	        if(SearchForString(filelist->namesDirs[n],pathname,myIcon)){
	        	if(r->doExit)goto ErrorOut;
	            continue;
	        }
	    }
	    for(n=0;n<filelist->countFiles;++n){
	        if(SearchFileForString(filelist->namesFiles[n],pathname,myIcon)){
	        	if(r->doExit)goto ErrorOut;
	            continue;
	        }
	    }
	}else{
	      if(SearchFileForString(name,directory,myIcon))goto ErrorOut;
	}
	ret = 0 ;
ErrorOut:
    if(pathname)cFree((char *)pathname);
	if(filelist)free_dir(filelist);
	return ret;
}
static int SearchFileForString(char *name,char *directory,IconPtr myIcon)
{
	char *pathname;
	char buff[256];
	searchPtr r;
	long length;
	FILE *in;
	int reti;
	long nt;
	
	if(!myIcon)return 1;
	r=(searchPtr)myIcon->DWindow;
	if(!r)return 1;

	++r->FilesSearched;

	reti = 1;
	
	in=NULL;
	
	pathname=strpathname(directory,name);
	if(!pathname)goto ErrorOut;
	

    in=fopen(pathname,"rb");
    if(!in){
    	sprintf(WarningBuff,"SendFile Error Opening (%s) To Read\n",pathname);
    	WarningBatch(WarningBuff);
    	goto ErrorOut;
    }
	length=FileLength(in);
	if(length < 0){
	    goto ErrorOut;
	}
	
	 r->Bytes += length;		
	 
	sprintf(buff,"%s%c",name,0);
	uWriteStatusWindow(r->StatusWindow,buff);
	r->stringfilename=buff;
	r->stringcount=0;
	r->linecount=0;
	r->character=0;
	r->stringfound=FALSE;
	r->FoundIt=FALSE;
	r->dirpathname=pathname;
		
	while(length > 0){
	    nt=length;
	    if(nt > IO_BUFFER2)nt=IO_BUFFER2;
	    if(getString((unsigned char *)r->buffin,nt,in))goto ErrorOut;	    
	    if(testForBinary((unsigned char *)r->buffin,nt,r))break;
	    stringsearch(r->buffin,nt,r);
	    if(r->OncePerFile && r->FoundIt)break;
	    length -= nt;
		if(uCheckStatusWindow(r->StatusWindow,"Stop Search ?")){
	        r->doExit=TRUE;
		    goto ErrorOut;
		}
	}
	
	reti = 0;
ErrorOut:
	if(in)fclose(in);
	
    if(pathname)cFree((char *)pathname);
	
	return reti;
}
int testForBinary(unsigned char *buffin,long nt,searchPtr r)
{
	int n,c;
	
	if(!r || !buffin)return 1;
	
	if(!r->skipBinary)return 0;
	
	if(nt > 1000)nt =1000;
	
	for(n=0;n<nt;++n){
	    c=buffin[n];
	    if(isprint(c))continue;
	    if(c == '\t' || c == '\r' || c == '\n' ||
	       c >= 32)continue;
	    return 1;
	}
	return 0;
}
int stringsearch(char *buffin,long nt,searchPtr r)
{
	char buff[2048];
	long k;
	int c,ct;
	
	if(!r || !buffin)return 1;
	
	for(k=0;k<nt;++k){
	    c=*buffin++;
	    if(r->IgnoreCase && c >= 'a' && c <= 'z'){
	        ct='A'+(c-'a');	        
	    }else{
	        ct=c;
	    }
	    if(c == '\r' || c == '\n'){
	        ++r->linecount;
	        ++r->LinesSearched;
	        if(r->stringfound){
	            r->charb[r->character]=0;
	            sprintf(buff,"%s;%ld:%s%c",r->dirpathname,r->linecount,r->charb,0);
	 	    	screen((unsigned char *)buff,(int)strlen(buff),r);
	 	    	++r->Hits;
	 	    	if(r->OncePerFile)return 1;
	        }
	        r->stringcount=0;
	        r->character=0;
			r->stringfound=FALSE;
			continue;
	    }else if(ct == r->string[r->stringcount++]){
	        if(r->stringcount >= r->stringmax){ /* found it */
		    	r->stringfound=TRUE;
		    	r->FoundIt=TRUE;
	            r->stringcount=0;	            
	        }
	    }else{
	        r->stringcount=0;
	    }
	    if(r->character < 254){
	        r->charb[r->character++]=c;
	    }
	}
	return 0;
	
}
static int screen(unsigned char *buff,int nn,searchPtr r)
{

	CEditPtr myEdit;
	char buff2[2048];
	
	if(!r || !buff)return 1;
		
	if(!r->edit){
	    r->edit=NewCEdit(NULL,NULL);
	    if(!r->edit)return 1;
	    sprintf(buff2,"Search(%s)",r->string);
		SetWindowName(r->edit,buff2);	
	}
	myEdit=(CEditPtr)r->edit->DWindow;
	if(!myEdit || (r->edit->WindowType != ExpressionType))return 1;
	
	
	mstrncpy(buff2,(char *)buff,2047);
	
	buff2[nn]=0;
    
	EditListAppendLine(myEdit->e,buff2);
	
	return 0;
}
static int doMenus(IconPtr myIcon,int menu,int item)
{
     
	uSetCursor(myIcon,-uWATCH_CURSOR);
	     
		switch(menu){
		case AppleMenu:
			if(item == 1){
			    doAbout(myIcon);
			    break;
			}
		break;
		case File_Menu:
		    if(item == Open_Item){
		        doConvertQuickTime(myIcon);
		    }else if(item == Find_Item){
		        doFind(myIcon);
		    }else if(item == Convert_Item){
		        doConvert(myIcon);
		    }else if(item == Teos2sds_Item){
		        doTeos2sds(myIcon);
		    }else if(item == Close_Item){
		        doClose(myIcon);
		        myIcon=NULL;
			} else if(item == GrayToHdf_Item){
		    	GrayToHdf(myIcon);
			} else if(item == New_Item){
		    	NewNewWindow(myIcon);
		    }else if(item == Quit_Item){
		        doQuit();
		 	    myIcon=NULL;
		 	    goto OutOfHere;
		    }
		    
		    
		break;
		case Edit_Menu:
		    if(item == Frame_Item){
		    }else if(item == Options_Item){
		    }else if(item == Scales_Item){
		    }
		break;
		case WindowMenu:
			pWindowMenu(myIcon,item);
			break;
		}  
		
OutOfHere:		   
	uSetCursor(myIcon,-uARROW_CURSOR);
	HiliteMenu(0);
	return 0;
}
static int doClose(IconPtr myIcon)
{
	searchPtr r;
	
	if(!myIcon)return 1;
	r=(searchPtr)myIcon->DWindow;
	
	
	if(r){
	    cFree((char *)r);
	    myIcon->DWindow=NULL;
	}
	
	myIcon=RemoveIcon(myIcon);
	    
	return 0;
}
static int GrayToHdf(IconPtr myIcon)
{
	struct FileInfo2 *Files2;
	struct FileList *Files;
	static char *pathname;
    struct SetFrameData *s;
    char name[2048],*namef;
    char palette[3*256];
    unsigned char *p,*data;
	long n,nn;
	
	if(!myIcon)return 1;	

	if(pathname)cFree((char *)pathname);
	pathname=FileGetPath("Put Files Here",&myIcon->infoSave);
	if(!pathname)return 1;
	
	Files=FileDialog(NULL,"Files To Convert",FileDialog_GetFiles,&myIcon->infoLoad);		
	if(!Files)return 1;
	Files2=FilesOpenList(Files);
	if(!Files2)return 1;
	
	for(n=0;n<256;++n){
	    palette[3*n]=(char)n;
	    palette[3*n+1]=(char)n;
	    palette[3*n+2]=(char)n;
	}
	
	for(n=0;n<Files2->ImageCount;++n){
		if(FilesSetFrame(n,Files2))return 1;
		s = &Files2->SetData;
		data=(unsigned char *)s->data;
		if(!data)continue;
		mstrncpy(name,pathname,sizeof(name));
		namef=FilesImageName(myIcon->Files,n);
		mstrncat(name,namef,sizeof(name));
		mstrncat(name,".hdf",sizeof(name));
		if(SetBuffers(s->xsize*s->ysize*3+512L))break;
		p=(unsigned char *)xg.buffin;
		for(nn=0;nn<s->xsize*s->ysize;++nn){
		    p[nn]=data[3*nn];
		}
		if(hdfOut(name,namef,(char *)palette,s->xsize,s->ysize,(char *)p,0,1,NULL))continue;
	}
	
	return 0;
	
}

#ifdef Macintosh
static int doConvertQuickTime(IconPtr myIcon)
{
	IconPtr StatusWindow;
    char pname[2048];
	char buff[256];
	struct FileList *Files;
	static char *pathname;
	FSSpec theFSSpec;
 	long dirIDToSearch;
 	short vRefNum;
	OSErr ret;
	int n;
	
	if(!myIcon)return 1;	

	if(pathname)cFree((char *)pathname);
	pathname=FileGetPath("Put QuickTime Movies Here",&myIcon->infoSave);
	if(!pathname)return 1;
	
	Files=FileDialog(NULL,"QuickTime Files To Flatten",FileDialog_GetFiles,&myIcon->infoLoad);		
	if(!Files)return 1;

    ret=HGetVol(NULL,&vRefNum,&dirIDToSearch);
	if(ret){
	    sprintf(WarningBuff,"initQUICKTIME HGetVol error (%ld)",(long)ret);
	    Warning(WarningBuff);
	    return 1;
	}
	
	StatusWindow=uStartStatusWindow(myIcon,"ConvertQuickTime");
	
	for(n=0;n<Files->count;++n){
		short refNum,resID;
		Movie movie;
		char *pc;
		
		resID = 0;
		refNum = -1;
		movie = 0;
	    mstrncpy(pname,Files->directory,2047);
	    strncatToPath(pname,Files->name[n],2047);
	    
		pc=strrchr(pname,FILE_NAME_SEPERATOR);
		if(pc){
     		
	        sprintf(buff,"%s%s","     Movie: ",pc+1);
	        uWriteStatusWindow(StatusWindow,buff);
		}else{
	        sprintf(buff,"%s%s","     Movie: ",pname);
	        uWriteStatusWindow(StatusWindow,buff);
	    }

		CopyCStringToPascal((Ptr)pname,(unsigned char *)pname);
	    
		ret=FSMakeFSSpec(vRefNum,dirIDToSearch,(unsigned char *)pname,&theFSSpec);
		if(ret){
			CopyPascalStringToC((unsigned char *)pname,(char *)pname);
		    sprintf(WarningBuff,"FSmakeFSSpec Error %ld File %s",(long)ret,(char *)pname);
		    Warning(WarningBuff);
		    goto ErrorOut;
		}
	    
		ret = OpenMovieFile( &theFSSpec, &refNum, fsRdPerm );
		if(ret){
		    sprintf(WarningBuff,"OpenMovieFile error (%ld)",(long)ret);
		    Warning(WarningBuff);
		    goto ErrorOut;
		}
		ret = NewMovieFromFile( &movie, refNum, &resID, NULL, newMovieActive, NULL );
		if(ret){
		    sprintf(WarningBuff,"NewMovieFromFile error (%ld)",(long)ret);
		    Warning(WarningBuff);
		    goto ErrorOut;
		}
		ret = CloseMovieFile( refNum );
		if(ret){
		    sprintf(WarningBuff,"CloseMovieFile error (%ld)",(long)ret);
		    Warning(WarningBuff);
		    goto ErrorOut;
		}
		
	    mstrncpy(pname,pathname,2047);
	    mstrncat(pname,Files->name[n],2047);
		CopyCStringToPascal((Ptr)pname,(unsigned char *)pname);
	    
		ret=FSMakeFSSpec(vRefNum,dirIDToSearch,(unsigned char *)pname,&theFSSpec);
		if(ret && (ret != fnfErr)){
			CopyPascalStringToC((unsigned char *)pname,(char *)pname);
		    sprintf(WarningBuff,"FSmakeFSSpec Error %ld File %s",(long)ret,(char *)pname);
		    Warning(WarningBuff);
		    goto ErrorOut;
		}
		
	    FlattenMovie(movie,
			flattenActiveTracksOnly | flattenAddMovieToDataFork,
			&(theFSSpec), 
			'TVOD', 
			0, 
			createMovieFileDeleteCurFile, 
			&(resID),
			0L);
			
		ret = GetMoviesError();
		
	    if (ret != noErr){
			sprintf(WarningBuff,"FlattenMovie  Error (%ld)",(long)ret);
		    Warning(WarningBuff);
		    goto ErrorOut;
	    }
	    
	    DisposeMovie(movie );
	    
	    if(uCheckStatusWindow(StatusWindow,"Flatten Movies ?"))goto ErrorOut;
	}
ErrorOut:
	uStopStatusWindow(StatusWindow);
	return 0;
}
#else
static int doConvertQuickTime(IconPtr myIcon)
{
	if(!myIcon)return 1;
	WarningBatch("doConvertQuickTime Not Active\n");
	return 0;
}
#endif
