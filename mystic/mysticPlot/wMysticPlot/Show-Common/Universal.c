#define EXTERN22 extern
#include "Xappl.h"
#include "uFiles.h"
#include "uDialog.h"
#include "FileManager.h"
#include "Message.h"
#include "Universal.h"

struct functionTableStruct{
    int (*doUniversal)(IconPtr myIcon,int flag,FILE8 *inOut);
    IconPtr (*doNew)(IconPtr myIcon);
    int WindowType;
    int UniversalType;
    int OpenOne;
    
};

static struct functionTableStruct functionTable[]={
	{doUniversalGlobal,NULL,0,universal_WindowGlobal,0},
	{doUniversalTek,NewTekWindow,TekType,universal_WindowTEK,0},
	{doUniversalSagePIO3D,NewPIO3DWindow,Pio3dType,universal_WindowSagePIO3D,0},
	{doUniversalSagePIO2D,NewPIO2DWindow,Pio2dType,universal_WindowSagePIO2D,0},
	{doUniversalRaster2D,NewNetWindow,RasterType,universal_WindowRaster2D,0},
	{doUniversalFileSend,NewFileSendWindow,SendWindowType,universal_WindowSendFile,0},
	{doUniversalNewFile,NewNewWindow,NewWindowType,universal_WindowNewFile,1},
	{doUniversalNewEdit,NewEditWindow,ExpressionType,universal_WindowNewEdit,0},
	{doUniversalRender,NewRenderWindow,xRenderType,universal_WindowRender,0},
	{doUniversalVolumetric,NewVolumetricWindow,VolumetricType,universal_WindowVolumetric,0},
	{doUniversalDraw,NewDrawWindow,DrawNewType,universal_WindowDRAW,0},
	{doUniversalDump,NULL,DumpWindowType,universal_WindowDump,0},
	/* {doUniversalSageBLK3D,NewBLK3DWindow,Pio3dBklType,universal_WindowSageBLK3D,0}, */
};

static int functionTableSize=sizeof(functionTable)/sizeof(struct functionTableStruct);

int doCursorFree(struct CursorStruct *Cursor);

static int WindowType;

int checkUniversalCRC(char *path);

static int intcmp5(const void *xx,const  void *yy);

static int doScale2Action(struct uDialogStruct *uList);

struct universalPreferencesStruct uPref;

IconPtr FindTypeWindow(int Type);

#define UBUFFER_SIZE 20000

int saveUniversalCURSOR(struct CursorStruct *Cursor,long tag,FILE8 *out);
int getUniversalCURSOR(struct CursorStruct *Cursor,FILE8 *in);

int getPalette(int n,char *name,char *pal);
int addToPaletteList(struct universalListOfPalettes *palList,char *name,unsigned char *pal,int flag);
int  FilesOpenVolumetric(struct FileInfo2 *Files);
int doUniversalLoadPreferences(void);
int saveUniversalFILE(struct FileInfo2 **Files,long tag,FILE8 *out);
int getUniversalFILE(struct FileList *Files,FILE8 *in);
int LoadUniversalPalette(struct universalListOfPalettes *palList);
int FreeUniversalPalette(struct universalListOfPalettes *palList);
int FindFilesInList(struct FileList *Files);
int getUniversalPalette(struct universalListOfPalettes *palList,FILE8 *in);
int getUniversalBytes(char *data,long length,long space,FILE8 *in);
static int saveRelativePath(struct FileInfo2 *Files,long tag,FILE8 *out);

int setGlobalOptions(IconPtr myIcon);

int IsShift(void);

unsigned char *saveblock(unsigned char *buffin,long length,int tag);

int saveUniversalPalette(int windowType,struct universalListOfPalettes *palList,FILE8 *out);

int FilesOpenPIODUMP(struct FileInfo2 *Files);

INT8_64 EndOfFile;

int startUniversal()
{
	char *path;

	setUniversalDefaults();
	
	if(IsShift())return 0;
	
	path=getUniversalPath();
	if(!path)return 1;
	uPref.path=path;	
	
	return readUniversal(path,0);
	
}
int setUniversalDefaults(void)
{
	int n;
	
	for(n=0;n<functionTableSize;++n){
	    
	    if(functionTable[n].doUniversal){
	        (*functionTable[n].doUniversal)(NULL,universal_SetDefaults,NULL);
	    }
	}
	return 0;
}
int stopUniversal()
{
	int n;
	
	for(n=0;n<functionTableSize;++n){
	    
	    if(functionTable[n].doUniversal){
	        (*functionTable[n].doUniversal)(NULL,universal_Stop,NULL);
	    }
	}
	return 0;
	
}	
int doUniversalLoadPreferences()
{
	IconPtr p;
	int n;
	
	
	p=IconRoot;
	
    while(p != NULL){
		for(n=0;n<functionTableSize;++n){
		    if(functionTable[n].WindowType == p->WindowType){
			    if(functionTable[n].doUniversal){
			        (*functionTable[n].doUniversal)(p,universal_GetDefaults,NULL);
			    }
			    break;
		    }
		}
		p=p->IconNext;
	}
			
	return 0;
}
int doUniversalWindows(int operation,int flag,FILE8 *out)
{
	IconPtr p;
	int ret;
	int n;
	
	
	ret=1;
	
	if(!out)return 1;

	uPref.config = FALSE;
		
	p=IconRoot;
	
    while(p != NULL){
		for(n=0;n<functionTableSize;++n){
		    if(functionTable[n].WindowType == p->WindowType){
			    if(functionTable[n].doUniversal){
			        (*functionTable[n].doUniversal)(p,universal_SetDefaults,out);
			    }
			    break;
		    }
		}
		p=p->IconNext;
	}

	for(n=1;n<functionTableSize;++n){
	    if(functionTable[n].doUniversal){
	        (*functionTable[n].doUniversal)(NULL,operation,out);
	    }
	}
			
	
	if(!flag)return 0;
	
	if(saveUniversalHeader("CONF",0L,0L,out))goto ErrorOut;

	uPref.config = TRUE;
	
	p=IconRoot;


    while(p != NULL){
		for(n=0;n<functionTableSize;++n){
		    if(functionTable[n].WindowType == p->WindowType){
			    if(functionTable[n].doUniversal){
			        (*functionTable[n].doUniversal)(p,universal_SetDefaults,out);
			        (*functionTable[n].doUniversal)(p,operation,out);
			    }
			    break;
		    }
		}
		p=p->IconNext;
	}
		
	ret=0;
ErrorOut:
	uPref.config = FALSE;
	return ret;
}
int readUniversal(char *path,int flag)
{
	IconPtr myIcon;
	char head[5];
	long length;
	long tag;
	FILE8 *in;
	int ret;
	int n;
	
	
	ret = 1;
	in=NULL;
	uPref.config = FALSE;
	uPref.readWritePath = NULL;
			
	if(!path)return 1;
		
	if(checkUniversalCRC(path)){	
		testPreferenceFile(path,NULL,TRUE);		
	    goto ErrorOut;
	}

		
	in=fopen8(path,"rb");
	if(!in){
		sprintf(WarningBuff,"Warning Default Preferences File %s Not Found\n",path);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	uPref.readWritePath = path;
	
	if(getUniversalHeader(head,&tag,&length,in)){
		sprintf(WarningBuff,"Error Reading Preferences File '%s' Tag\n",path);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	head[4]=0;
	
	if(strcmp(head,"UNIV")){
		sprintf(WarningBuff,"Error Reading Preferences File '%s' Wrong header '%s'\n",path,head);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	while(!getUniversalHeader(head,&tag,&length,in)){
	    if(!strcmp(head,"EXIT")){
	    	break;
	    }else if(!strcmp(head,"CONF")){
			uPref.config = TRUE;
	    }else if(!strcmp(head,"WIND")){
	        WindowType= -1;
	        uPref.Files=NULL;
	        myIcon=NULL;
			for(n=0;n<functionTableSize;++n){
			    if(functionTable[n].UniversalType == tag){
				    if(functionTable[n].doUniversal){
			        	WindowType=functionTable[n].WindowType;
			            if(flag && uPref.config){
			                 if(functionTable[n].OpenOne){
			            	 	if(!FindTypeWindow(functionTable[n].WindowType)){
			            	 	    if(functionTable[n].doNew){
			            	 	        myIcon=(*functionTable[n].doNew)(NULL);
			            	 	    }
			            	 	}
			            	 }else{			            	 
		            	 	    if(functionTable[n].doNew){
		            	 	        myIcon=(*functionTable[n].doNew)(NULL);
		            	 	    }
			            	 }
			            }
				        (*functionTable[n].doUniversal)(myIcon,universal_ReadData,in);
				    }
				    break;
			    }
			}
	        
			if(WindowType == -1){
				int ibreak;
				int savetag=tag;
				sprintf(WarningBuff,"Window %s tag %ld Skipped\n",head,tag);
				WarningBatch(WarningBuff);
				if(skipUniversal(length,in))goto ErrorOut;
				ibreak=0;
				while(!getUniversalHeader(head,&tag,&length,in)){
	    		    if(!strcmp(head,"STOP")){
					     if(tag == savetag)ibreak=1;
					}
					if(skipUniversal(length,in))goto ErrorOut;
					if(ibreak)break;
				}
				continue;

			}
	        	        
	        if(uPref.Files){	        
	       		struct FileInfo2 *Files;
     			Files=FilesOpenList(uPref.Files);
	    		if(Files){
	    		    if(WindowType == VolumetricType){
	    		       	Files->pdefault=FilesOpenVolumetric;
	    		    }else if(WindowType == DumpWindowType){
	   					Files->pdefault=FilesOpenPIODUMP;
	    		    }
	    		    FilesOpenDefault(Files);
	    		}
	    		uPref.Files=NULL;
			}
	        if(uPref.Cursor.count > 0){	
				doCursorFree(&uPref.Cursor);
	        }        
	    }
	}
		
	ret = 0;
ErrorOut:	

	if(in)fclose8(in);	
	
	if(ret && !flag){
	    if(uPref.path)cFree((char *)uPref.path);
	    uPref.path=NULL;
	}
	
	uPref.config = FALSE;
	uPref.readWritePath = NULL;
	
	return ret;
}

int getUniversalTable(universalTablePtr Table,FILE8 *in)
{
	universalListPtr TableList;
	int (*funt)(void *,FILE8 *,int);
	char **p;
	char head[5];
	long length;
	long tag;
	int nwind;
	int ret;
	int n;
	
	if(!Table || !in)return 1;
	
	if(Table->TableCount <= 0)return 0;
	TableList=Table->TableList;
	if(!TableList)return 1;
	
	ret=1;
	
	while(!getUniversalHeader(head,&tag,&length,in)){
	    if(!strcmp(head,"STOP")){
	    	break;
	    }
		for(n=0;n<Table->TableCount;++n){
		    if(TableList[n].tag == tag)goto Found;
		}
		if(!strcmp(head,"RELP")){
		    if(!getUniversalHeader(head,&tag,&length,in)){
				if(!strcmp(head,"STRI")){
				    if(uPref.relativePath)cFree((char *)uPref.relativePath);
				    uPref.relativePath=NULL;
	    			if(getUniversalStringPtr(&uPref.relativePath,length,in))goto ErrorOut; 
	    			/*
	    			sprintf(WarningBuff,"path = %s\n",uPref.relativePath);
	    			WarningBatch(WarningBuff);
	    			*/
	    			continue;
				}
		    }
		}
		sprintf(WarningBuff,"getUniversalTable %s tag %ld Skipped\n",head,tag);
		WarningBatch(WarningBuff);
		if(skipUniversal(length,in))goto ErrorOut;
		nwind=1;
		if(!strcmp(head,"WIND")){     /* eat until balanced stop */
		    while(!getUniversalHeader(head,&tag,&length,in)){
	    		if(!strcmp(head,"STOP")){
	    		    --nwind;
	    		}else if(!strcmp(head,"WIND")){
	    		    ++nwind;
	    		}
	    		if(skipUniversal(length,in))goto ErrorOut;
	    		if(nwind <= 0)break;
		    }
		
		}
		continue;
Found:
	    if(!strcmp(head,"RECT")){
	    	if(TableList[n].dataType != universal_TypeRect){
				sprintf(WarningBuff,"getUniversalTable %s tag %ld Type Miss-Match Table Entry %d - Skipped\n",head,tag,n);
				WarningBatch(WarningBuff);
				if(skipUniversal(length,in))goto ErrorOut;
	    		continue;
	    	}
	    	if(getUniversalRect(TableList[n].data,in))goto ErrorOut; 
		}else if(!strcmp(head,"STRI")){
	    	if(TableList[n].dataType == universal_TypeString){
	    		if(getUniversalString(TableList[n].data,length,TableList[n].space,in))goto ErrorOut; 
	    	}else if(TableList[n].dataType == universal_TypeStringPtr){
	    		if(getUniversalStringPtr(TableList[n].data,length,in))goto ErrorOut; 
	    	}else if(TableList[n].dataType == universal_TypeBytePtr){
	    		if(getUniversalStringPtr(TableList[n].data,length,in))goto ErrorOut; 
	    		TableList[n].space=length;
	    	}else if(TableList[n].dataType == universal_TypeBytes){
	    		if(getUniversalBytes(TableList[n].data,length,TableList[n].space,in))goto ErrorOut; 
	    	}else{
				sprintf(WarningBuff,"getUniversalTable %s tag %d Type Miss-Match Table Entry %d - Skipped\n",
				        TableList[n].name,TableList[n].dataType,n);
				WarningBatch(WarningBuff);
				if(skipUniversal(length,in))goto ErrorOut;
	    		continue;
	    	}
		}else if(!strcmp(head,"REAL")){
	    	if(getUniversalReal(TableList[n].data,TableList[n].dataType,length,&TableList[n].space,in))goto ErrorOut; 
		}else if(!strcmp(head,"DOUB")){
	    	if(getUniversalDouble(TableList[n].data,TableList[n].dataType,length,TableList[n].space,in))goto ErrorOut; 
		}else if(!strcmp(head,"WIND")){
	    	if(TableList[n].dataType == universal_TypeTable){
	    		getUniversalTable((universalTablePtr)TableList[n].data,in);
	    	}		
		}else if(!strcmp(head,"FUNT")){
			     funt=(int (*)(void *,FILE8 *,int))TableList[n].data;
			     if(funt)(*funt)(Table->tableDynamic,in,0);
		}else if(!strcmp(head,"FILE")){
	    	if(TableList[n].dataType == universal_TypeFilesPtr){
	    	    if(uPref.Files)uPref.Files2=uPref.Files;
	    	    uPref.Files=cMalloc(sizeof(struct FileList),8765);
	    	    if(!uPref.Files)goto ErrorOut;
				zerol((char *)uPref.Files,sizeof(struct FileList));
	    		if(getUniversalFILE(uPref.Files,in))goto ErrorOut;
	    		p=TableList[n].data;
	    		if(p)*p=NULL;	
	    		if(!uPref.config){
	    		    FileListFree(uPref.Files);
	    		    uPref.Files=NULL;
	    		}else{
	    		    if(uPref.Files){		    		    
			       		if(!FindFilesInList(uPref.Files)){
							sprintf(WarningBuff,"getUniversalTable %s tag %ld No Files type %s Found To Load\n",head,tag,FilesTypeName(uPref.Files));
							WarningBatch(WarningBuff);
		    		    	FileListFree(uPref.Files);
		    		    	uPref.Files=NULL;
		    			}
	    			}
	    		}
				if(uPref.relativePath)cFree((char *)uPref.relativePath);
				uPref.relativePath=NULL;
	    	}else if(TableList[n].dataType == universal_TypeCursorPtr){
				zerol((char *)&uPref.Cursor,sizeof(struct CursorStruct));
	    		if(getUniversalCURSOR(&uPref.Cursor,in))goto ErrorOut;
	    	}		
		}else if(!strcmp(head,"PALS")){
	    	if(TableList[n].dataType == universal_TypePaletteList){
	    		if(getUniversalPalette(TableList[n].data,in))goto ErrorOut;
	    	}
		}else{
			sprintf(WarningBuff,"getUniversalTable %s tag %ld conversion Missing - Skipped\n",head,tag);
			WarningBatch(WarningBuff);
			if(skipUniversal(length,in))goto ErrorOut;
		}				
	}
	ret=0;
ErrorOut:
	return ret;
}
int writeUniversal(IconPtr myIcon,int flag)
{
	static char name[2048];
	unsigned int crcval;
	FILE8 *out;
	int ret;
		
	ret = 1;
	out=NULL;
	
	uPref.config = FALSE;
	uPref.readWritePath = NULL;
	
	if(!myIcon)return 1;
		
	if(flag){
	    if(GetTheSaveFileName(name,"Save Configuration File",2048,&myIcon->infoSave))goto ErrorOut;
		uPref.readWritePath = name;
	}else{
		if(!uPref.path){
	    	if(GetTheSaveFileName(name,"Save To Preferences File",2048,&myIcon->infoSave))goto ErrorOut;
			uPref.path=strsave(name,8001);
			if(!uPref.path)goto ErrorOut;
			saveUniversalPath(uPref.path);
		}else{
			mstrncpy(name,uPref.path,2048);
		}
	}
	out=fopen8(name,"wb");
	if(!out){
		sprintf(WarningBuff,"Warning Preferences File '%s' Could Not Open To Write\n",name);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	if(startCRC8(out)){
		sprintf(WarningBuff,"Warning Preferences File '%s' StartCRC8 Failed\n",name);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	if(saveUniversalHeader("UNIV",0L,0L,out)){
		sprintf(WarningBuff,"Warning Preferences File '%s' saveUniversalHeader UNIV Failed\n",name);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	if(doUniversalGlobal(NULL,universal_SaveData,out)){
		sprintf(WarningBuff,"Warning Preferences File '%s' doUniversalGlobal Failed\n",name);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	if(doUniversalWindows(universal_SaveData,flag,out)){
		sprintf(WarningBuff,"Warning Preferences File '%s' doUniversalWindows Failed\n",name);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	if(saveUniversalHeader("EXIT",universal_WindowGlobal,0L,out)){
		sprintf(WarningBuff,"Warning Preferences File '%s' saveUniversalHeader EXIT Failed\n",name);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	if(stopCRC8(&crcval,out)){
		sprintf(WarningBuff,"Warning Preferences File '%s' Stop Failed\n",name);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	if(saveUniversalHeader("CRCV",(long)crcval,0L,out)){
		sprintf(WarningBuff,"Warning Preferences File '%s' saveUniversalHeader CRCV Failed\n",name);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	
	if(out)fclose8(out);
	out=NULL;

	SetFileType(name,"CONF");
	
	if(checkUniversalCRC(name)){	
	    goto ErrorOut;
	}
	
		
	if(testPreferenceFile(name,NULL,FALSE)){
	    if(testPreferenceFile(name,NULL,TRUE)){
			sprintf(WarningBuff,"Warning Preferences File '%s' \n   Failed Test Read\n",name);
	    	WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}
	
	ret = 0;
ErrorOut:	
	if(out)fclose8(out);
	uPref.config = FALSE;
	uPref.readWritePath = NULL;
	return ret;
}
int LoadUniversalPalette(struct universalListOfPalettes *palList)
{
	char name[256],pal[768];
	int n;
	
	if(!palList)return 1;
	
	FreeUniversalPalette(palList);
	
	zerol((char *)palList,sizeof(struct universalListOfPalettes));
		
	for(n=0;n<29;++n){
		if(getPalette(n,name,pal))break;
		if(addToPaletteList(palList,name,(unsigned char *)pal,1))break;
	}
	
	return 0;
}
int FreeUniversalPalette(struct universalListOfPalettes *palList)
{
	int n;
	
	if(!palList)return 1;

	if(palList->pals){
		for(n=0;n<palList->palCount;++n){
			if(palList->pals[n].name)cFree((char *)palList->pals[n].name);
			palList->pals[n].name=NULL;
			if(palList->pals[n].pal)cFree((char *)palList->pals[n].pal);
			palList->pals[n].pal=NULL;		
		}		
		if(palList->pals)cFree((char *)palList->pals);
	}
	palList->pals=NULL;
	palList->palCount=0;
	palList->palMax=0;
	
	return 0;
}
int getUniversalPalette(struct universalListOfPalettes *palList,FILE8 *in)
{
	char head[5];
	char *name,*pal;
	long length;
	long tag;
	int ret;
	
	if(!palList || !in)return 1;

	if(FreeUniversalPalette(palList))return 1;
	
	ret=1;
	pal=NULL;
	name=NULL;	
	while(!getUniversalHeader(head,&tag,&length,in)){
		if(!strcmp(head,"STOP")){
			ret=0;
		    goto Ok;
		}
		if(strcmp(head,"STRI")){
		     WarningBatch("getUniversalPalette Looking for STRI one\n");
		     break; 
		}
		name=NULL;
		if(getUniversalStringPtr(&name,length,in))break; 
				
		if(getUniversalHeader(head,&tag,&length,in))break;
				
		if(strcmp(head,"STRI")){
		     WarningBatch("getUniversalPalette Looking for STRI two\n");
		     break; 
		}

		pal=NULL;
		if(getUniversalStringPtr(&pal,length,in))break; 
		
		if(length != 768){
		     sprintf(WarningBuff,"getUniversalPalette Palette Length %ld\n",length);
		     WarningBatch(WarningBuff);
		     break; 
		}	
		if(addToPaletteList(palList,name,(unsigned char *)pal,0))break;
		pal=NULL;
		name=NULL;		
	}
	
	
Ok:
	if(pal)cFree((char *)pal);
	if(name)cFree((char *)name);
	
	return ret;
}
int sortUniversalPalette()
{
	struct universalPalettes *pals;
		
	if(!uPref.palList.pals || (uPref.palList.palCount <= 0))return 1;
	
	pals=uPref.palList.pals;
	if(!pals)return 1;
	
	qsort((char *)pals,uPref.palList.palCount, sizeof(struct universalPalettes),intcmp5);
	
	return 0;
}
static int intcmp5(const void *xx,const  void *yy)
{
	register struct universalPalettes *x=(struct universalPalettes *)xx;
	register struct universalPalettes *y=(struct universalPalettes *)yy;
	return strcmp(x->name,y->name);
}
int addUniversalPalette(char *name,unsigned char *r,unsigned char *g,unsigned char *b)
{
	unsigned char pal[768];
	int n;
	
	if(!name || !r || !g || !b)return 1;
	
	for(n=0;n<256;++n){
	   pal[3*n]=r[n];
	   pal[3*n+1]=g[n];
	   pal[3*n+2]=b[n];
	}
	
	return addToPaletteList(&uPref.palList,name,pal,1);
}
#define PALLETTE_LIST_INCREMENT 5
int addToPaletteList(struct universalListOfPalettes *palList,char *name,unsigned char *pal,int flag)
{
	struct universalPalettes *palListNew;
	int ret;
	int n;
	
	if(!palList || !name || !pal)return 1;
	
	ret=1;
	
	if(palList->palCount+1 > palList->palMax){
		if(!palList->pals){
			palList->pals=(struct universalPalettes *)cMalloc(PALLETTE_LIST_INCREMENT*sizeof(struct universalPalettes),8726);
			if(!palList->pals)goto ErrorOut;
			zerol((char *)palList->pals,PALLETTE_LIST_INCREMENT*sizeof(struct universalPalettes));
			palList->palMax=PALLETTE_LIST_INCREMENT;
			palList->palCount=0;
		}else{
		    palListNew=(struct universalPalettes *)cRealloc((char *)palList->pals,(palList->palMax+PALLETTE_LIST_INCREMENT)*sizeof(struct universalPalettes),8727);
		    if(!palListNew)goto ErrorOut;
	    	zerol((char *)&palListNew[palList->palMax],PALLETTE_LIST_INCREMENT*sizeof(struct universalPalettes));
			palList->palCount=palList->palMax;
			palList->palMax += PALLETTE_LIST_INCREMENT;
			palList->pals=palListNew;
		}
	}
	
	n=palList->palCount;
			
    if(flag){
		palList->pals[n].name=strsave(name,9939);
		if(!palList->pals[n].name)goto ErrorOut;
		palList->pals[n].pal=saveblock((unsigned char *)pal,768,9938);
		if(!palList->pals[n].pal)goto ErrorOut;		
	}else{
		palList->pals[n].name=name;
		if(!palList->pals[n].name)goto ErrorOut;
		palList->pals[n].pal=pal;
		if(!palList->pals[n].pal)goto ErrorOut;		
	}
	++palList->palCount;
		
	ret=0;
ErrorOut:
	return ret;
}

int deleteUniversalPaletteByName(char *name)
{
	struct universalListOfPalettes *palList;
	int n,nn;
	
	if(!name)return 1;
	
	if(!uPref.palList.pals || (uPref.palList.palCount <= 0))return 1;
	
	palList=&uPref.palList;

	nn=0;
	for(n=0;n<palList->palCount;++n){
		if(!strcmp(palList->pals[n].name,name)){
			if(palList->pals[n].name)cFree((char *)palList->pals[n].name);
			palList->pals[n].name=NULL;
			if(palList->pals[n].pal)cFree((char *)palList->pals[n].pal);
			palList->pals[n].pal=NULL;		
		}else{
		    palList->pals[nn].name=palList->pals[n].name;
		    palList->pals[nn++].pal=palList->pals[n].pal;
		}
	}
	
	palList->palCount=nn;
	
	return 0;
}
int getUniversalPaletteItem(int n,char **name,unsigned char **pal)
{
	if(!name || !pal || (n < 0) || (n >= uPref.palList.palCount) ||
	   !uPref.palList.pals)return 1;
	
	*pal=uPref.palList.pals[n].pal;
	*name=uPref.palList.pals[n].name;
	
	if(!*name || !*pal)return 1;

	return 0;
}
int getUniversalPaletteByName(char *name,unsigned char **pal)
{
	int n;
	
	if(!name || !pal || !uPref.palList.pals)return 1;
	
	for(n=0;n<uPref.palList.palCount;++n){
	    if(!mstrcmp(name,uPref.palList.pals[n].name)){
			*pal=uPref.palList.pals[n].pal;
			if(!*pal)return 1;
			return 0;
	    }
	}
	
	return 1;
}
int saveUniversalPalette(int windowType,struct universalListOfPalettes *palList,FILE8 *out)
{
	int n;
	
	if(!palList || !palList->pals)return 1;

	if(palList->palCount <= 0)return 0;
	
	if(saveUniversalHeader("PALS",windowType,0L,out))return 1;


	for(n=0;n<palList->palCount;++n){
		if(palList->pals[n].name && palList->pals[n].pal){
			if(saveUniversalString((char *)palList->pals[n].name,n,out))break;
			if(saveUniversalBytes((char *)palList->pals[n].pal,n,768L,out))break;
		}
	}
	if(saveUniversalHeader("STOP",windowType,0L,out))return 1;


	return 0;
}

int doUniversalGlobal(IconPtr myIcon,int flag,FILE8 *inOut)
{
	static struct universalTableListStruct Local[] = {
		{"RECT",9000,universal_TypeRect,(void *)&uPref.SaveFileRect},
		{"RECT",9001,universal_TypeRect,(void *)&uPref.LoadFileRect},
		{"RECT",9002,universal_TypeRect,(void *)&uPref.SaveImageRect},
		{"RECT",9003,universal_TypeRect,(void *)&uPref.dataSelectRect},
		{"RECT",9004,universal_TypeRect,(void *)&uPref.newDialogRect},
		{"PALS",9005,universal_TypePaletteList,(void *)&uPref.palList},
		{"REAL",9006,universal_TypeInt,(void *)&uPref.SaveSDS64Bits},
		{"RECT",9007,universal_TypeRect,(void *)&uPref.dumpSelectRect},
		{"RECT",9008,universal_TypeRect,(void *)&uPref.cursorSelectRect},
		{"REAL",9009,universal_TypeInt,(void *)&uPref.LocalOpenDialog},
		{"REAL",9010,universal_TypeInt,(void *)&uPref.TextOutputFormat},
		{"REAL",9011,universal_TypeInt,(void *)&uPref.UseStartConfiguration},
		{"STRI",9012,universal_TypeStringPtr,(void *)&uPref.startConfiguration},
		{"REAL",9014,universal_TypeInt,(void *)&uPref.flipSDS2dY},
		{"REAL",9015,universal_TypeInt,(void *)&uPref.VoiceRecognition},
		{"REAL",9016,universal_TypeLong,(void *)&uPref.BlockSizeGlobal},
		{"STRI",9017,universal_TypeStringPtr,(void *)&uPref.OpenWithApplication},

		
	};
	
	static struct universalTableStruct Global= {
		"doUniversalGlobal parameters",universal_WindowGlobal,
		 &uPref,sizeof(uPref), 
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),						
	};	
	int ret;
	
	
	ret=1;
	
	myIcon=myIcon;
	
	if(flag == universal_ReadData){
		return getUniversalTable(&Global,inOut);
	}else if(flag == universal_SaveData){
		return putUniversalTable((int)Global.tableTag,&Global,inOut);
	}else if(flag == universal_SetDefaults){
		uSetRect(&uPref.SaveFileRect,117,45,575,434);
		uSetRect(&uPref.LoadFileRect,117,45,575,434);
		uSetRect(&uPref.SaveImageRect,117,45,575,434);
		uSetRect(&uPref.dataSelectRect,20,40,400,300);
		uSetRect(&uPref.dumpSelectRect,20,40,400,320+MenuOffSet);
		uSetRect(&uPref.newDialogRect,20,40,160,530);
		uSetRect(&uPref.cursorSelectRect,20,40,400,300);
		uPref.SaveSDS64Bits=FALSE;
		uPref.LocalOpenDialog=FALSE;		
		uPref.UseStartConfiguration=FALSE;		
		uPref.TextOutputFormat=DefaultTextType;
		uPref.startConfiguration=NULL;
		uPref.flipSDS2dY=FALSE;
		uPref.VoiceRecognition=FALSE;
		uPref.BlockSizeGlobal=0;
		LoadUniversalPalette(&uPref.palList);
	}else if(flag == universal_Stop){
		if(uPref.path)cFree((char *)uPref.path);
		uPref.path=NULL;
		if(uPref.startConfiguration)cFree((char *)uPref.startConfiguration);
		uPref.startConfiguration=NULL;
		if(uPref.OpenWithApplication)cFree((char *)uPref.OpenWithApplication);
		uPref.OpenWithApplication=NULL;
		FreeUniversalPalette(&uPref.palList);
	}else{
		sprintf(WarningBuff,"doUniversalGlobal unknown flag %d\n",flag );
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
	
	ret=0;
ErrorOut:
	return ret;
	
}
int putUniversalTable(int windowType,universalTablePtr Table,FILE8 *out)
{
	universalListPtr TableList;
	int (*funt)(void *,FILE8 *out,int);
	char **data;
	int n;
	
	if(!Table || !out)return 1;

	if(Table->TableCount <= 0)return 0;
	TableList=Table->TableList;
	if(!TableList)return 1;
	
	if(saveUniversalHeader("WIND",windowType,0L,out))return 1;
	
		for(n=0;n<Table->TableCount;++n){
		    if(!strcmp(TableList[n].name,"RECT")){
		    	if(TableList[n].dataType != universal_TypeRect){
					sprintf(WarningBuff,"putUniversalTable %s dataType %d Type Miss-Match Table Entry %d - Skipped\n",
					       TableList[n].name,TableList[n].dataType,n);
					WarningBatch(WarningBuff);
		    		continue;
		    	}
				if(saveUniversalRect(TableList[n].data,TableList[n].tag,out))return 1;
			}else if(!strcmp(TableList[n].name,"STRI")){
		    	if(TableList[n].dataType == universal_TypeString){
					if(saveUniversalString(TableList[n].data,TableList[n].tag,out))return 1;
		    	}else if(TableList[n].dataType == universal_TypeStringPtr){
		    	    data=TableList[n].data;
		    	    if(!data || !*data){
		    			continue;
		    	    }
					if(saveUniversalString(*data,TableList[n].tag,out))return 1;
		    	}else if(TableList[n].dataType == universal_TypeBytePtr){
		    	    data=TableList[n].data;
		    	    if((TableList[n].space <= 0) || !data || !*data){
		    			continue;
		    	    }		    	    
					if(saveUniversalBytes(*data,TableList[n].tag,TableList[n].space,out))return 1;
		    	}else if(TableList[n].dataType == universal_TypeBytes){
		    	    data=TableList[n].data;
		    	    if((TableList[n].space <= 0) || !data){
		    			continue;
		    	    }		    	    
					if(saveUniversalBytes((char *)data,TableList[n].tag,TableList[n].space,out))return 1;
		    	}else{
					sprintf(WarningBuff,"putUniversalTable %s dataType %d Type Miss-Match Table Entry %d - Skipped\n",
					       TableList[n].name,TableList[n].dataType,n);
					WarningBatch(WarningBuff);
		    		continue;
		    	}
			}else if(!strcmp(TableList[n].name,"REAL")){
				if(saveUniversalReal(TableList[n].data,TableList[n].tag,TableList[n].dataType,TableList[n].space,out))return 1;
			}else if(!strcmp(TableList[n].name,"FUNT")){			     
				 if(saveUniversalHeader("FUNT",TableList[n].tag,0L,out))return 1;
			     funt=(int (*)(void *,FILE8 *,int))TableList[n].data;
			     if(funt)(*funt)(Table->tableDynamic,out,1);
			}else if(!strcmp(TableList[n].name,"DOUB")){
				if(saveUniversalDouble(TableList[n].data,TableList[n].tag,TableList[n].dataType,TableList[n].space,out))return 1;
			}else if(!strcmp(TableList[n].name,"TABL")){
		    	if(TableList[n].dataType == universal_TypeTable){
		    		putUniversalTable((int)TableList[n].tag,(universalTablePtr)TableList[n].data,out);
		    	}		
			}else if(!strcmp(TableList[n].name,"PALS")){
		    	if(TableList[n].dataType == universal_TypePaletteList){
		    		saveUniversalPalette((int)TableList[n].tag,(struct universalListOfPalettes *)TableList[n].data,out);
		    	}		
			}else if(!strcmp(TableList[n].name,"FILE")){
		    	if(TableList[n].dataType == universal_TypeFilesPtr){
		    	    data=TableList[n].data;
		    	    if(!data || !*data){
		    			continue;
		    	    }
		    		saveUniversalFILE((struct FileInfo2 **)TableList[n].data,(int)TableList[n].tag,out);
	    		}else if(TableList[n].dataType == universal_TypeCursorPtr){
		    		saveUniversalCURSOR((struct CursorStruct *)TableList[n].data,(int)TableList[n].tag,out);
	    		}		
			}else{
				sprintf(WarningBuff,"putUniversalTable %s Entry %d conversion Missing - Skipped\n",
				       TableList[n].name,n);
				WarningBatch(WarningBuff);
			}
		}
	
	
	if(saveUniversalHeader("STOP",windowType,0L,out))return 1;
	return 0;
}
int moveUniversalTable(universalTablePtr Table,int flag)
{
	universalListPtr TableList;
	struct FileInfo2 **sFiles,**dFiles;
	struct universalListOfPalettes **sPal,**dPal;
	struct CursorStruct *sCursor,*dCursor;
	uRect *dRect,*sRect;
	long *dLong,*sLong;
	int *dInt,*sInt;
	double *dDouble,*sDouble;
	char **pS,**pD;
	char *start,*put;
	long offset,k;
	int n;
	
	void *out;
	long size;
	
	if(!Table || (Table->TableCount <= 0))return 1;
	TableList=Table->TableList;
	if(!TableList)return 1;
	out=Table->tableDynamic;
	size=Table->tableDynamicSpace;
	
	if(!out)return 1;
	
	if(Table->tableStaticSpace != size){
		sprintf(WarningBuff,"moveUniversalTable length %ld space %ld - Size Miss Match\n",Table->tableStaticSpace,size);
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
	start=Table->tableStatic;
	if(!start){
		sprintf(WarningBuff,"moveUniversalTable NULL start infomation\n");
	    WarningBatch(WarningBuff);
	    return 1;
	}

	put=out;

	for(n=0;n<Table->TableCount;++n){
	
		if(!strcmp(TableList[n].name,"TABL") || !strcmp(TableList[n].name,"FUNT") ){
	    	if(TableList[n].dataType == universal_TypeTable){
	    		moveUniversalTable((universalTablePtr)TableList[n].data,flag);
	    	}
	    	continue;
	   	}
	   		   	
	    offset=(char *)(TableList[n].data)-start;
    	if(offset < 0 || offset >= size){
			sprintf(WarningBuff,"moveUniversalTable %s Error offset %ld size %ld tag %ld Table Entry %d - Skipped\n",
			       TableList[n].name,offset,size,TableList[n].tag,n);
			WarningBatch(WarningBuff);
    		continue;
    	}
	    if(!strcmp(TableList[n].name,"RECT")){
	    	if(TableList[n].dataType != universal_TypeRect){
				sprintf(WarningBuff,"moveUniversalTable %s tag %ld Type Miss Match Table Entry %d - Skipped\n",
				        TableList[n].name,TableList[n].tag,n);
				WarningBatch(WarningBuff);
	    		continue;
	    	}
	    	sRect=(uRect *)(start+offset);
	    	dRect=(uRect *)(put+offset);
	    	if(flag == universal_MoveToDynamic){
	    		*dRect = *sRect;
	    	}else if(flag == universal_MoveToStatic){
	    		*sRect=*dRect;
	    	
	    	}
		}else if(!strcmp(TableList[n].name,"STRI")){
	    	if(TableList[n].dataType == universal_TypeString){
		    	if(offset+TableList[n].space > size){
					sprintf(WarningBuff,"moveUniversalTable %s Error offset %ld size %ld tag %ld Table Entry %d - Skipped\n",
					       TableList[n].name,offset,size,TableList[n].tag,n);
					WarningBatch(WarningBuff);
		    		continue;
		    	}
	    		if(flag == universal_MoveToDynamic){
		    		mstrncpy(put+offset,start+offset,TableList[n].space);
		    	}else if(flag == universal_MoveToStatic){
		    		mstrncpy(start+offset,put+offset,TableList[n].space);
		    	}
	    	}else if(TableList[n].dataType == universal_TypeBytes){
		    	if(offset+TableList[n].space > size){
					sprintf(WarningBuff,"moveUniversalTable %s Error offset %ld size %ld tag %ld Table Entry %d - Skipped\n",
					       TableList[n].name,offset,size,TableList[n].tag,n);
					WarningBatch(WarningBuff);
		    		continue;
		    	}
	    		if(flag == universal_MoveToDynamic){
		    		memcpy(put+offset,start+offset,TableList[n].space);
		    	}else if(flag == universal_MoveToStatic){
		    		memcpy(start+offset,put+offset,TableList[n].space);
		    	}
	    	}else if(TableList[n].dataType == universal_TypeStringPtr){
	    	    pD=(char **)(put+offset);
	    	    pS=(char **)(start+offset);
	    	    if(!pD || !pS)continue;
	    		if(flag == universal_MoveToDynamic){
		    	    if(!*pS)continue;
	    	    	if(*pD)cFree((char *)*pD);
	    	    	*pD=strsave(*pS,8373);
		    	}else if(flag == universal_MoveToStatic){
		    	    if(!*pD)continue;
	    	    	if(*pS)cFree((char *)*pS);
	    	    	*pS=strsave(*pD,8373);		    	
		    	}
	    	}else if(TableList[n].dataType == universal_TypeBytePtr){
	    		continue;
	    	}else{
				sprintf(WarningBuff,"moveUniversalTable %s tag %ld Type Miss-Match Table Entry %d - Skipped\n",
				       TableList[n].name,TableList[n].tag,n);
				WarningBatch(WarningBuff);
	    		continue;
	    	}
		}else if(!strcmp(TableList[n].name,"REAL")){
	    	if(TableList[n].dataType == universal_TypeInt){
	    		sInt=(int *)(start+offset);
	    		dInt=(int *)(put+offset);
		    	if(flag == universal_MoveToDynamic){
	    			*dInt = *sInt;
		    	}else if(flag == universal_MoveToStatic){
	    			*sInt = *dInt;
		    	
		    	}
	    	}else if(TableList[n].dataType == universal_TypeLong){
	    		sLong=(long *)(start+offset);
	    		dLong=(long *)(put+offset);
		    	if(flag == universal_MoveToDynamic){
	    			*dLong = *sLong;
		    	}else if(flag == universal_MoveToStatic){
	    			*sLong = *dLong;
		    	
		    	}
	    	}else if(TableList[n].dataType == universal_TypeDoubleArrayPtr){
	    	/*
	    		sLong=(long *)(start+offset);
	    		dLong=(long *)(put+offset);
		    	if(flag == universal_MoveToDynamic){
	    			*dLong = *sLong;
		    	}else if(flag == universal_MoveToStatic){
	    			*sLong = *dLong;
		    	
		    	}
		    */
	    	}else if(TableList[n].dataType == universal_TypeDoubleLongPtr){
	    		;
	    	}else{
				sprintf(WarningBuff,"moveUniversalTable %s tag %ld Type Missing Table Entry %d - Skipped\n",
				        TableList[n].name,TableList[n].tag,n);
				WarningBatch(WarningBuff);
	    		continue;
	    	}
		}else if(!strcmp(TableList[n].name,"DOUB")){
	    	if(TableList[n].dataType == universal_TypeInt){
	    		sInt=(int *)(start+offset);
	    		dInt=(int *)(put+offset);
		    	if(flag == universal_MoveToDynamic){
	    			*dInt = *sInt;
		    	}else if(flag == universal_MoveToStatic){
	    			*sInt = *dInt;
		    	
		    	}
	    	}else if(TableList[n].dataType == universal_TypeLong){
	    		sLong=(long *)(start+offset);
	    		dLong=(long *)(put+offset);
		    	if(flag == universal_MoveToDynamic){
	    			*dLong = *sLong;
		    	}else if(flag == universal_MoveToStatic){
	    			*sLong = *dLong;
		    	
		    	}
	    	}else if(TableList[n].dataType == universal_TypeDouble){
	    		sDouble=(double *)(start+offset);
	    		dDouble=(double *)(put+offset);
	    		
		    	if(flag == universal_MoveToDynamic){
	    			*dDouble = *sDouble;
		    	}else if(flag == universal_MoveToStatic){
	    			*sDouble = *dDouble;
		    	
		    	}
	    	}else if(TableList[n].dataType == universal_TypeDoubleArray){
	    		sDouble=(double *)(start+offset);
	    		dDouble=(double *)(put+offset);
	    		
	    		for(k=0;k<TableList[n].space;++k){
			    	if(flag == universal_MoveToDynamic){
		    			dDouble[k] = sDouble[k];
			    	}else if(flag == universal_MoveToStatic){
		    			sDouble[k] = dDouble[k];
			    	
			    	}
		    	}
	    	}else{
				sprintf(WarningBuff,"moveUniversalTable %s tag %ld Type Missing Table Entry %d - Skipped\n",
				        TableList[n].name,TableList[n].tag,n);
				WarningBatch(WarningBuff);
	    		continue;
	    	}
		}else if(!strcmp(TableList[n].name,"FILE")){		
	    	if(TableList[n].dataType == universal_TypeFilesPtr){
	    		sFiles=(struct FileInfo2 **)(start+offset);
	    		dFiles=(struct FileInfo2 **)(put+offset);
	    	    if(!sFiles || !dFiles)continue;
		    	if(flag == universal_MoveToDynamic){
		    	    if(!*sFiles)continue;
	    			/* *dFiles = *sFiles;  NO not for Files */
		    	}else if(flag == universal_MoveToStatic){
		    	    if(!*dFiles)continue;
	    			*sFiles = *dFiles;
		    	
		    	}
	    	}else if(TableList[n].dataType == universal_TypeCursorPtr){
	    		sCursor=(struct CursorStruct *)(start+offset);
	    		dCursor=(struct CursorStruct *)(put+offset);
	    	    if(!sCursor || !dCursor)continue;
		    	if(flag == universal_MoveToDynamic){
		    	    if(!sCursor)continue;
	    			/* *dFiles = *sFiles;  NO not for Cursor */
		    	}else if(flag == universal_MoveToStatic){
		    	    if(!sCursor)continue;
	    			*sCursor = *dCursor;		    	
		    	}
	    	}		
		}else if(!strcmp(TableList[n].name,"PALS")){		
	    	if(TableList[n].dataType == universal_TypePaletteList){
	    		sPal=(struct universalListOfPalettes **)(start+offset);
	    		dPal=(struct universalListOfPalettes **)(put+offset);
	    	    if(!sPal || !dPal)continue;
		    	if(flag == universal_MoveToDynamic){
		    	    if(!*dPal)continue;
	    			*dPal = *sPal;
		    	}else if(flag == universal_MoveToStatic){
		    	    if(!*dPal)continue;
	    			*sPal = *dPal;
		    	
		    	}
	    	}			
		}else{
			sprintf(WarningBuff,"moveUniversalTable %s Entry %d conversion Missing - Skipped\n",
			        TableList[n].name,n);
			WarningBatch(WarningBuff);
		}
	}
	
	
	return 0;
}
int freeUniversalTable(universalTablePtr Table)
{
	universalListPtr TableList;
	char **cp;
	int n;
	
	if(!Table || (Table->TableCount <= 0))return 1;
	TableList=Table->TableList;
	if(!TableList)return 1;
		
	for(n=0;n<Table->TableCount;++n){
		if(TableList[n].dataType == universal_TypeStringPtr){
		    cp=TableList[n].data;
			if(cp && *cp){
			    cFree((char *)(*cp));
				*cp=NULL;
			}
		}else if(TableList[n].dataType == universal_TypeTable){
		    freeUniversalTable((universalTablePtr)TableList[n].data);
		}
	}
	return 0;
}
int testPreferenceFile(char *path,unsigned int *crcval,int flag)
{
	FILE8 *in;
	char head[5];
	long length;
	long tag;
	int ret;
	
	if(!path || !path[0])return 1;

	in=fopen8(path,"rb");
	
	if(!in){
		sprintf(WarningBuff,"testPreferenceFile Could Not Open File '%s' To Read\n",path);
		WarningBatch(WarningBuff);
	    return 1;
	}	
	ret=1;

	if(getUniversalHeader(head,&tag,&length,in)){
	    if(flag)WarningBatchFast("testPreferenceFile Could Not Read File Header");
	    goto ErrorOut;
	}
	
	head[4]=0;
	
	if(strcmp(head,"UNIV")){
		if(flag){
			sprintf(WarningBuff,"testPreferenceFile Read Wrong File Header '%s' tag",head);
		    WarningBatchFast(WarningBuff);
	    }
	    goto ErrorOut;
	}
	
	while(!getUniversalHeader(head,&tag,&length,in)){
		if(flag){
		    if(!strcmp(head,"DOUB") && length == 8){
		        double data;
				if(getDouble8(&data,in))goto ErrorOut;
				sprintf(WarningBuff,"head %s tag %ld length %ld value %f",head,tag,length,data);
				WarningBatchFast(WarningBuff);
				continue;
		    }else{
				sprintf(WarningBuff,"head %s tag %ld length %ld",head,tag,length);
				WarningBatchFast(WarningBuff);
			}
		}
	    if(!strcmp(head,"CRCV")){
	        if(crcval){
	            *crcval=(unsigned int)tag;
	            EndOfFile=ftell8(in)-16;
	        }
			ret = 0;
	    	break;
	    }
		if(skipUniversal(length,in))break;
	}
ErrorOut:

	if(flag)WarningBatchAtEnd("");
	
	if(in)fclose8(in);
	    
	return ret;
}
int checkUniversalCRC(char *path)
{
	unsigned int crcval,crcvalr;
	INT8_64 end,n;
	FILE8 *in;
	int ret;
	
	ret = 1;
	
	in=NULL;	
	crcval=0;
	crcvalr=0;
	
	
	if(testPreferenceFile(path,&crcvalr,FALSE)){
		ret = 1;
	    goto ErrorOut;
	}

	end = EndOfFile;
	
	if(SetBuffers(UBUFFER_SIZE)){
		ret = 2;
	    goto ErrorOut;
	}	

	in=fopen8(path,"rb");
	if(!in){
		ret = 3;
	    goto ErrorOut;
	}
			
	
	if(startCRC8(in)){
		ret = 8;
	    goto ErrorOut;
	}
	
	while(end > 0){
	    n=end;
	    if(n > UBUFFER_SIZE)n=UBUFFER_SIZE;
	    if(getString8(xg.buffin,(long)n,in)){
			ret = 9;
	        goto ErrorOut;
	    }
	    end -= n;
	}
	
	if(stopCRC8(&crcval,in)){
		ret = 10;
	    goto ErrorOut;
	}
	
	if(crcvalr != crcval){
		ret = 11;
	    goto ErrorOut;
	}
	
	ret = 0;
ErrorOut:

	if(in)fclose8(in);
	
	if(ret){
	    if(ret == 11){
			sprintf(WarningBuff,"checkUniversalCRC File %s CRC Error %lx %lx\n",path,
		      	    (unsigned long)crcvalr,(unsigned long)crcval);
	    }else{
			sprintf(WarningBuff,"checkUniversalCRC File %s Error %d\n",path,ret);
		}
	    WarningBatch(WarningBuff);
	}
		
	return ret;
}
int getUniversalHeader(char *dataType,long *tag,long *length,FILE8 *in)
{
	char head[5];
	
	if(!dataType || !tag || !length || !in)return 1;
	
	if(getString8((unsigned char *)head,4L,in))return 1;
	
	head[4]=0;
	
	if(strcmp(head,"DATA")){
		sprintf(WarningBuff,"getUniversalHeader Wrong header '%s' (%d %d %d %d)\n",
		        head,head[0],head[1],head[2],head[3]);
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
	if(getString8((unsigned char *)dataType,4L,in))return 1;
	
	dataType[4]=0;
	
	if(getLong8(tag,in))return 1;
	
	if(getLong8(length,in))return 1;
	
	return 0;
}
int saveUniversalHeader(char *dataType,long tag,long length,FILE8 *out)
{
	if(!dataType || !out)return 1;
	
	if(putString8((unsigned char *)"DATA",4L,out))return 1;
	if(putString8((unsigned char *)dataType,4L,out))return 1;
	if(putLong8(tag,out))return 1;
	if(putLong8(length,out))return 1;
	
	return 0;
}
int getUniversalRect(uRect *Save,FILE8 *in)
{
	long Savex,Savey,Savexsize,Saveysize;
	
	if(!Save || !in)return 1;
	
	uSetRect(Save,0,0,0,0);
	
	if(getLong8(&Savex,in))return 1;
	if(getLong8(&Savey,in))return 1;
	if(getLong8(&Savexsize,in))return 1;
	if(getLong8(&Saveysize,in))return 1;

	uSetRect(Save,(int)Savex,(int)Savey,(int)Savexsize,(int)Saveysize);

	return 0;
}
int saveUniversalRect(uRect *Save,long tag,FILE8 *out)
{
	if(!Save || !out)return 1;
	
	if(saveUniversalHeader("RECT",tag,16L,out))return 1;
	if(putLong8(Save->x,out))return 1;
	if(putLong8(Save->y,out))return 1;
	if(putLong8(Save->xsize,out))return 1;
	if(putLong8(Save->ysize,out))return 1;

	return 0;
}
int getUniversalReal(void *Save,int type,long length,long *space,FILE8 *in)
{
	float *sFloat,**sPtr;
	double *dFloat,**dPtr;
	long *lFloat,**lPtr;
	float data;
	int *sInt;
	long *sLong,n;
	
	if(!Save || !in)return 1;
	
	data=0;

	if(type == universal_TypeRealArrayPtr){
		if(!space)return 1;
	    *space=0;	    
		if(length <= 0)return 0;
	    sPtr=Save;
	    if(*sPtr)cFree((char *)*sPtr);
	    *sPtr=cMalloc(length,9727);
	    if(!*sPtr)return 1;
	    *space=length/4;	    
	    sFloat = *sPtr;
	    for(n=0;n<*space;++n){
			if(getFloat8(&sFloat[n],in))return 1;
	    }
	    return 0;
	}else if(type == universal_TypeDoubleArrayPtr){
		if(!space)return 1;
	    *space=0;	    
		if(length <= 0)return 0;
	    dPtr=Save;
	    if(*dPtr)cFree((char *)*dPtr);
	    *dPtr=cMalloc(length,9728);
	    if(!*dPtr)return 1;
	    *space=length/8;	    
	    dFloat = *dPtr;
	    for(n=0;n<*space;++n){
			if(getDouble8(&dFloat[n],in))return 1;
	    }
	    return 0;
	}else if(type == universal_TypeDoubleLongPtr){
		if(!space)return 1;
	    *space=0;	    
		if(length <= 0)return 0;
	    lPtr=Save;
	    if(*lPtr)cFree((char *)*lPtr);
	    *lPtr=cMalloc(length,9729);
	    if(!*lPtr)return 1;
	    *space=length/4;	    
	    lFloat = *lPtr;
	    for(n=0;n<*space;++n){
			if(getLong8(&lFloat[n],in))return 1;
	    }
	    return 0;
	}



	if(getFloat8(&data,in))return 1;
	
	if(type == universal_TypeInt){
	    sInt=Save;
	    *sInt=(int)data;
	}else if(type == universal_TypeLong){
	    sLong=Save;
	    *sLong=(long)data;
	}

	return 0;
}
int saveUniversalReal(void *Save,long tag,int type,long space,FILE8 *out)
{
	float *sFloat,**sPtr;
	double *dFloat,**dPtr;
	long *lFloat,**lPtr;
	
	float data;
	int *sInt;
	long *sLong,n;
	
	if(!Save || !out)return 1;
	
	data=0;
	
	
	if(type == universal_TypeRealArrayPtr){
		if(space < 0)space=0;
		if(saveUniversalHeader("REAL",tag,4L*space,out))return 1;
	    sPtr=Save;
	    sFloat = *sPtr;
		for(n=0;n<space;++n){
			if(putFloat8(sFloat[n],out))return 1;
		}
		return 0;
	}else if(type == universal_TypeDoubleArrayPtr){
		if(space < 0)space=0;
		if(saveUniversalHeader("REAL",tag,8L*space,out))return 1;
	    dPtr=Save;
	    dFloat = *dPtr;
		for(n=0;n<space;++n){
			if(putDouble8(dFloat[n],out))return 1;
		}
		return 0;
	}else if(type == universal_TypeDoubleLongPtr){
		if(space < 0)space=0;
		if(saveUniversalHeader("REAL",tag,4L*space,out))return 1;
	    lPtr=Save;
	    lFloat = *lPtr;
		for(n=0;n<space;++n){
			if(putLong8(lFloat[n],out))return 1;
		}
		return 0;
	}
	
	if(saveUniversalHeader("REAL",tag,4L,out))return 1;
	
	if(type == universal_TypeInt){
	    sInt=Save;
	    data=(float)(*sInt);
	}else if(type == universal_TypeLong){
	    sLong=Save;
	    data= (float)(*sLong);
	}
	if(putFloat8(data,out))return 1;
	
	return 0;
}
int getUniversalDouble(void *Save,int type,long length,long space,FILE8 *in)
{
	double *sDouble;
	double data;
	int *sInt;
	long *sLong,n;
	
	if(!Save || !in)return 1;
	
	data=0;

	if(type == universal_TypeDoubleArray){
	    length /= 8;	    
	    if(length != space){
			sprintf(WarningBuff,"getUniversalDouble length %ld space %ld Error\n",length,space);
		    WarningBatch(WarningBuff);
	        return 1;
	    }
	    sDouble=Save;
	    for(n=0;n<space;++n){
			if(getDouble8(&sDouble[n],in))return 1;
	    }
	    return 0;
	}
	
	if(getDouble8(&data,in))return 1;
	
	if(type == universal_TypeInt){
	    sInt=Save;
	    *sInt=(int)data;
	}else if(type == universal_TypeLong){
	    sLong=Save;
	    *sLong=(long)data;
	}else if(type == universal_TypeDouble){
	    sDouble=Save;
	    *sDouble=data;
	}

	return 0;
}
int saveUniversalDouble(void *Save,long tag,int type,long space,FILE8 *out)
{
	double *sDouble;
	double data;
	int *sInt;
	long *sLong,n;
	if(!Save || !out)return 1;
	
	data=0;
	
	if(type == universal_TypeDoubleArray){
		if(space < 0)space=0;
		if(saveUniversalHeader("DOUB",tag,8L*space,out))return 1;
	    sDouble=Save;
		for(n=0;n<space;++n){
			if(putDouble8(sDouble[n],out))return 1;
		}
		return 0;
	}
	
	if(saveUniversalHeader("DOUB",tag,8L,out))return 1;
	
	if(type == universal_TypeInt){
	    sInt=Save;
	    data=(double)(*sInt);
	}else if(type == universal_TypeLong){
	    sLong=Save;
	    data=(double)*sLong;
	}else if(type == universal_TypeDouble){
	    sDouble=Save;
	    data=*sDouble;
	}
	
	if(putDouble8(data,out))return 1;
	
	return 0;
}

int getUniversalString(char *data,long length,long space,FILE8 *in)
{
	if(!data || !in)return 1;
	
	if(length >= space){
		sprintf(WarningBuff,"getUniversalString length %ld space %ld - Execss Data Skipped\n",length,space);
	    WarningBatch(WarningBuff);
		if(getString8((unsigned char *)data,space-1,in))return 1;
		data[space]=0;
		if(skipUniversal(length-(space-1),in))return 1;
	}else{
	    if(length > 0){
		    if(getString8((unsigned char *)data,length,in))return 1;
		}
		data[length]=0;
	}
	
	return 0;
}
int getUniversalBytes(char *data,long length,long space,FILE8 *in)
{
	if(!data || !in)return 1;
	
	if(length > space){
		sprintf(WarningBuff,"getUniversalBytes length %ld space %ld - Execss Data Skipped\n",length,space);
	    WarningBatch(WarningBuff);
		if(getString8((unsigned char *)data,space-1,in))return 1;
		if(skipUniversal(length-(space-1),in))return 1;
	}else{
	    if(length > 0){
		    if(getString8((unsigned char *)data,length,in))return 1;
		}
	}
	
	return 0;
}
int saveUniversalString(char *Save,long tag,FILE8 *out)
{
	long length;
	
	if(!Save || !out)return 1;
	
	length=(long)strlen(Save);
	
	if(saveUniversalHeader("STRI",tag,length,out))return 1;
	if(putString8((unsigned char *)Save,length,out))return 1;
	
	return 0;
}
int saveUniversalBytes(char *Save,long tag,long length,FILE8 *out)
{
	
	if(!Save || !out)return 1;
		
	if(saveUniversalHeader("STRI",tag,length,out))return 1;
	if(putString8((unsigned char *)Save,length,out))return 1;
	
	return 0;
}

int getUniversalStringPtr(char **data,long length,FILE8 *in)
{
	char *ptr;
	
	if(!data || !in || (length < 0))return 1;
	
	if(*data)cFree((char *)*data);
	
	ptr=(*data)=(char *)cMalloc(length+1,9875);
	if(!ptr)return 1;
	
    if(length > 0){
	    if(getString8((unsigned char *)(ptr),length,in))return 1;
	}
	
	ptr[length]=0;
		
	return 0;
}
int getUniversalFILE(struct FileList *Files,FILE8 *in)
{
	char head[5];
	long length;
	long tag;
	char *directory;
	char **names;
	int count,type;
	int n;
	
	if(!Files || !in)return 1;
	
	if(getUniversalHeader(head,&tag,&length,in))goto ErrorOut;
	
	if(strcmp(head,"REAL")){
	     WarningBatch("getUniversalFILE Looking for type\n");
	     goto ErrorOut; 
	}
	
	if(getUniversalReal(&type,universal_TypeInt,0L,NULL,in))goto ErrorOut; 
	
	Files->type=type;
	
	if(getUniversalHeader(head,&tag,&length,in))goto ErrorOut;
	
	if(strcmp(head,"REAL")){
	     WarningBatch("getUniversalFILE Looking for count\n");
	     goto ErrorOut; 
	}
	
	if(getUniversalReal(&count,universal_TypeInt,0L,NULL,in))goto ErrorOut; 
	
	
/*	
	sprintf(WarningBuff,"count %d\n",count);
	WarningBatch(WarningBuff);
*/	
	
	if(getUniversalHeader(head,&tag,&length,in))goto ErrorOut;
	
	if(strcmp(head,"STRI")){
	     WarningBatch("getUniversalFILE Looking for directory\n");
	     goto ErrorOut; 
	}
	
	directory=NULL;
	
	if(getUniversalStringPtr(&directory,length,in))goto ErrorOut; 
	
	Files->directory=directory;
	
/*
	sprintf(WarningBuff,"directory %s\n",directory);
	WarningBatch(WarningBuff);
*/	
		
	names=(char **)cMalloc(count*sizeof(char *),8372);
	if(!names)goto ErrorOut;
	
	Files->count=count;
	
	zerol((char *)names,count*sizeof(char *));

	Files->name=names;
	
	for(n=0;n<count;++n){
		if(getUniversalHeader(head,&tag,&length,in))goto ErrorOut;
		
		if(strcmp(head,"STRI")){
		     WarningBatch("getUniversalFILE Looking for STRI\n");
		     goto ErrorOut; 
		}
		if(getUniversalStringPtr(&names[n],length,in))goto ErrorOut; 
/*		
		sprintf(WarningBuff,"%d file %s\n",n,names[n]);
		WarningBatch(WarningBuff);
*/
	}
	
	if(getUniversalHeader(head,&tag,&length,in))goto ErrorOut;
	
	if(strcmp(head,"STOP")){
	     WarningBatch("getUniversalFILE Looking for STOP\n");
	     goto ErrorOut; 
	}

ErrorOut:
			
	return 0;
}
static int saveRelativePath(struct FileInfo2 *Files,long tag,FILE8 *out)
{
	char path1[1024];
	char path2[1024];
	char path3[2048];
	char *pc;
	int k,n,nc,j;
	int n3;
	
	if(!Files || !Files->directory || !out)return 1;
	if(!uPref.readWritePath)return 0;
	
	tag = -1;
	/*
	WarningBatch("saveRelativePath\n");
	sprintf(WarningBuff,"readWritePath %s\n",uPref.readWritePath);
	WarningBatch(WarningBuff);
	sprintf(WarningBuff,"directory %s\n",Files->directory);
	WarningBatch(WarningBuff);
	*/
	mstrncpy(path1,uPref.readWritePath,2048);
	mstrncpy(path2,Files->directory,2048);
	for(n=0;n<1024;++n){
	    if(path1[n] != path2[n])break;
	}
	if(n <= 0)return 0;
	
	for(k=n;k>=0;--k){
	    if(path1[k] == FILE_NAME_SEPERATOR)break;
	}
	if(k < 0)return 0;
	
	pc=strrchr(&path1[k],FILE_NAME_SEPERATOR);
	if(!pc)return 0;
	
	nc=0;
	for(j=k;pc != &path1[j];++j){
	    if(path1[j] == FILE_NAME_SEPERATOR)++nc;
	}

	n3=0;
	for(j=0;j<nc;++j){
		path3[n3++]=BS;
	}
	
	for(j=k;j<(int)strlen(path2);++j){
	    if(path2[j] == FILE_NAME_SEPERATOR){
	    	path3[n3++]=FS;
	    }else{
	    	path3[n3++]=path2[j];
	    }

	}	
	
	path3[n3]=0;
	
	
	if(saveUniversalHeader("RELP",tag,0L,out))return 1;
	
	if(saveUniversalString((char *)path3,tag,out))return 1;
/*
	sprintf(WarningBuff,"path3 %s\n",path3);
	WarningBatch(WarningBuff);
*/
	
	return 0;
}
int saveUniversalFILE(struct FileInfo2 **FilesIn,long tag,FILE8 *out)
{
	struct FileInfo2 *Files;
	int count,type;
	int n;
	
	if(!FilesIn || !out)return 1;
	
	if(!uPref.config)return 0;
	
	Files=*FilesIn;
	
	if(!FileOpenIsOpen(Files))return 0;
	
	if(!Files || !Files->directory || (Files->FileCount <= 0) || !Files->FilesNames)return 0;
	
	if(saveRelativePath(Files,tag,out))return 1;
	
	if(saveUniversalHeader("FILE",tag,0L,out))return 1;
	
	type=(int)Files->fileType;
	if(saveUniversalReal(&type,tag,universal_TypeInt,0L,out))goto ErrorOut;
	
	count=(int)Files->FileCount;
	
	if(saveUniversalReal(&count,tag,universal_TypeInt,0L,out))goto ErrorOut;

	if(saveUniversalString((char *)Files->directory,tag,out))goto ErrorOut;
	
	for(n=0;n<count;++n){
		if(saveUniversalString((char *)Files->FilesNames[n],tag,out))goto ErrorOut;
	}
ErrorOut:
	if(saveUniversalHeader("STOP",tag,0L,out))return 1;
	
	return 0;
}
int saveUniversalCURSOR(struct CursorStruct *Cursor,long tag,FILE8 *out)
{
	int count;
	int flag;
	int n;
	
	if(!Cursor || !out)return 1;
	
	if(!uPref.config)return 0;
	
	if(Cursor->count <= 0 || !Cursor->Items)return 0;
	
	if(saveUniversalHeader("FILE",tag,0L,out))return 1;
	
	count=(int)Cursor->count;
	
	if(saveUniversalReal(&count,tag,universal_TypeInt,0L,out))goto ErrorOut;
	
	for(n=0;n<count;++n){
		if(saveUniversalString((char *)Cursor->Items[n].pioName,tag,out))goto ErrorOut;
		if(saveUniversalString((char *)Cursor->Items[n].format,tag,out))goto ErrorOut;
		flag=Cursor->Items[n].valueFlag;
		if(saveUniversalReal(&flag,tag,universal_TypeInt,0L,out))goto ErrorOut;
		
	}
ErrorOut:
	if(saveUniversalHeader("STOP",tag,0L,out))return 1;
	
	return 0;
}
int getUniversalCURSOR(struct CursorStruct *Cursor,FILE8 *in)
{
	struct CursorDisplayItems *Items;
	int valueFlag;
	char head[5];
	long length;
	long tag;
	int count;
	int n;
	
	if(!Cursor || !in)return 1;
	
		
	if(getUniversalHeader(head,&tag,&length,in))goto ErrorOut;
	
	if(strcmp(head,"REAL")){
	     WarningBatch("getUniversalCURSOR Looking for count\n");
	     goto ErrorOut; 
	}
	
	
	if(getUniversalReal(&count,universal_TypeInt,0L,NULL,in))goto ErrorOut; 
	
		
	Cursor->count=count;
			
	
	Items=(struct CursorDisplayItems *)cMalloc(count*sizeof(struct CursorDisplayItems),8372);
	if(!Items)goto ErrorOut;
	
	Cursor->Items=Items;
		
	zerol((char *)Items,count*sizeof(struct CursorDisplayItems));

	
	for(n=0;n<count;++n){
		if(getUniversalHeader(head,&tag,&length,in))goto ErrorOut;
		
		if(strcmp(head,"STRI")){
		     WarningBatch("getUniversalCURSOR Looking for STRI\n");
		     goto ErrorOut; 
		}
		if(getUniversalString((char *)&Items[n].pioName,length,64,in))goto ErrorOut; 
		
		if(getUniversalHeader(head,&tag,&length,in))goto ErrorOut;
		
		if(strcmp(head,"STRI")){
		     WarningBatch("getUniversalCURSOR Looking for STRI\n");
		     goto ErrorOut; 
		}
		if(getUniversalString((char *)&Items[n].format,length,8,in))goto ErrorOut; 

		if(getUniversalHeader(head,&tag,&length,in))goto ErrorOut;
		
		if(strcmp(head,"REAL")){
		     WarningBatch("getUniversalCURSOR Looking for count\n");
		     goto ErrorOut; 
		}
		
		
		if(getUniversalReal(&valueFlag,universal_TypeInt,0L,NULL,in))goto ErrorOut; 
		
		Items[n].valueFlag=valueFlag;
		
	}
	
	if(getUniversalHeader(head,&tag,&length,in))goto ErrorOut;
	
	if(strcmp(head,"STOP")){
	     WarningBatch("getUniversalCURSOR Looking for STOP\n");
	     goto ErrorOut; 
	}

ErrorOut:
			
	return 0;
}
int skipUniversal(long end,FILE8 *in)
{
	long n;
	int ret;
	
	if(!in)return 1;
	if(end <= 0)return 0;
	
	ret=1;
	
	if(SetBuffers(UBUFFER_SIZE)){
		ret = 2;
	    goto ErrorOut;
	}	
	
	while(end > 0){
	    n=end;
	    if(n > UBUFFER_SIZE)n=UBUFFER_SIZE;
	    if(getString8(xg.buffin,(long)n,in)){
			ret = 9;
	        goto ErrorOut;
	    }
	    end -= n;
	}
	
	ret=0;
ErrorOut:
	if(ret){
		sprintf(WarningBuff,"skipUniversal Error %d\n",ret);
	    WarningBatch(WarningBuff);
	}

	return ret;
}
int saveUniversal(IconPtr myIcon)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok",     {10,465,65,25},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {100,465,65,25},uDialogButton,uDialogNone},
      
      /*  2 */      {"Save Preferences",        {10, 25,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  3 */      {"Load Preference File",    {10, 65,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  4 */      {"Save New Preference File",{10,105,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  5 */      {"Save Configuration File", {10,145,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  6 */      {"Load Configuration File", {10,185,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  7 */      {"Set Save Directory",      {10,225,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  8 */      {"Set Load Directory",      {10,265,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  9 */      {"Restore Defaults",        {10,305,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  10 */     {"Restore Defaults All",    {10,345,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  11 */     {"Set Global Options",      {10,385,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  12 */     {"Test Configuration File", {10,425,225,25},uDialogButtonRadio,uDialogGetLong},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,200,510};
	static char name[2048];
 	uDialogList *uList;
	uDialogReturn *uR;
    static int NewWindow=0;
    char *pathname;
    int retr;
    int ret;
    
    
    if(!myIcon)return 1;
	
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Preferences",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	retr=uDialogSet(uList,2L,
		          uDialogSetLogical,(NewWindow == 0),
	              
	              uDialogSetItem,3L,
	              uDialogSetLogical,(NewWindow == 1),
	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,(NewWindow == 2),
	              
	              uDialogSetItem,5L,
	              uDialogSetLogical,(NewWindow == 3),
	              
	              uDialogSetItem,6L,
	              uDialogSetLogical,(NewWindow == 4),
	              
	              uDialogSetItem,7L,
	              uDialogSetLogical,(NewWindow == 5),
	              
	              uDialogSetItem,8L,
	              uDialogSetLogical,(NewWindow == 6),

	              uDialogSetItem,9L,
	              uDialogSetLogical,(NewWindow == 7),

	              uDialogSetItem,10L,
	              uDialogSetLogical,(NewWindow == 8),

	              uDialogSetItem,11L,
	              uDialogSetLogical,(NewWindow == 9),

	              uDialogSetItem,12L,
	              uDialogSetLogical,(NewWindow == 10),


	              uDialogSetDone,uDialogSetDone
	    );
	if(retr)goto ErrorOut;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
	    
	if(uR[2].lreturn){
	    NewWindow = 0;
	}else if(uR[3].lreturn){
	    NewWindow = 1;
	}else if(uR[4].lreturn){
	    NewWindow = 2;
	}else if(uR[5].lreturn){
	    NewWindow = 3;
	}else if(uR[6].lreturn){
	    NewWindow = 4;
	}else if(uR[7].lreturn){
	    NewWindow = 5;
	}else if(uR[8].lreturn){
	    NewWindow = 6;
	}else if(uR[9].lreturn){
	    NewWindow = 7;
	}else if(uR[10].lreturn){
	    NewWindow = 8;
	}else if(uR[11].lreturn){
	    NewWindow = 9;
	}else if(uR[12].lreturn){
	    NewWindow = 10;
	}else{
	    NewWindow = 0;
	}
	
	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
    
    if(!ret){
    
    
    	switch(NewWindow){
    	case 0:
    	default:
	    	writeUniversal(myIcon,0);
    	break;
    	case 1:
			if(uPref.path){
			    cFree((char *)uPref.path);
			    uPref.path=NULL;
			}
			uPref.path=FileGetFilePath("Load Preference File",&myIcon->infoLoad);
			if(!uPref.path)break;
			saveUniversalPath(uPref.path);
	    	readUniversal(uPref.path,0);
	    	doUniversalLoadPreferences();
    	break;
    	case 2:
			if(uPref.path){
			    cFree((char *)uPref.path);
			    uPref.path=NULL;
			}
	    	writeUniversal(myIcon,0);
    	break;
    	case 3:
	    	writeUniversal(myIcon,1);
    	break;
    	case 4:
    	{
			struct FileList *Files;
 	    	char *save;
   			int n;
    		
			Files=FileDialog(NULL,"Load Configuration Files",FileDialog_GetFiles,&myIcon->infoLoad);		
			if(!Files)return 1;
    
			if(!Files)return 1;
			
			save=uPref.startConfiguration;
	    	uPref.startConfiguration=NULL;
		    		
			for(n=0;n<Files->count;++n){
			    mstrncpy(name,Files->directory,2048);
				strncatToPath(name,Files->name[n],2048);
				readUniversal(name,1);   
			}	
			
			if(Files)FileListFree(Files);
			if(uPref.startConfiguration)cFree((char *)uPref.startConfiguration);
			uPref.startConfiguration=save;

	  	}
    	break;
    	case 5:
			pathname=FileGetPath("Set Save Directory Here",&myIcon->infoSave);
			if(!pathname)return 1;
			if(myIcon->infoSave.directory)cFree(myIcon->infoSave.directory);
			myIcon->infoSave.directory=pathname;
    	break;
    	case 6:
			pathname=FileGetPath("Set Load Directory Here",&myIcon->infoLoad);
			if(!pathname)return 1;
			if(myIcon->infoLoad.directory)cFree(myIcon->infoLoad.directory);
			myIcon->infoLoad.directory=pathname;
    	break;
    	case 7:
    	    if(!myIcon)break;
    	    {
    	    
		    	int n;
			
				for(n=0;n<functionTableSize;++n){				        	    
				    if(functionTable[n].WindowType == myIcon->WindowType){
				        (*functionTable[n].doUniversal)(NULL,universal_SetDefaults,NULL);
			        	(*functionTable[n].doUniversal)(myIcon,universal_GetDefaults,NULL);
				        break;
				    }
				}
	    
	    	}
    	    
    	break;
    	case 8:
			setUniversalDefaults();
	    	doUniversalLoadPreferences();
    	break;
    	case 9:
			setGlobalOptions(myIcon);
    	break;
    	case 10:
	    	if(GetTheFileName(name,"Configuration File To Test",2048,&myIcon->infoSave))break;
			if(checkUniversalCRC(name)){	
			    ;
			}
			if(name[0]){
			    testPreferenceFile(name,NULL,TRUE);
			}		
    	break;
    	}
    
    }
    
	return ret;
}
int FilesOpenUniversal(struct FileInfo2 *Files)
{
	char name[2048];
    int n;
    
	if(!Files)return 1;
    		
	for(n=0;n<Files->FileCount;++n){
	    mstrncpy(name,Files->directory,2048);
		strncatToPath(name,Files->FilesNames[n],2048);
		readUniversal(name,1);   
	}	

	if(Files)freeFileList(Files);

	return 0;
}
static int doScale2Action(struct uDialogStruct *uList)
{
	int ret;
	
	if(!uList)return 1;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 9){
		if(uPref.startConfiguration)cFree((char *)uPref.startConfiguration);
		uPref.startConfiguration=FileGetFilePath("Find Start Confguration",&uList->myIcon->infoLoad);
		if(uPref.startConfiguration){
			if(uDialogSet(uList,2L,

	              uDialogSetItem,8L,
		          uDialogSetLogical,1,
		          
		          uDialogSetDone,uDialogSetDone
			))goto OutOfHere;
		}
	}else if(uList->actionItem == 14){
		char *OpenWithApplication;
		OpenWithApplication=FileGetFilePath("Set Open With Application",&uList->myIcon->infoLoad);
		ret=uDialogSet(uList,2L,
						
						uDialogSetItem,15L,	              
						uDialogSetText,OpenWithApplication,
						
						uDialogSetDone,uDialogSetDone
		);
		if(OpenWithApplication)cFree((char *)OpenWithApplication);
		if(ret)goto OutOfHere;
	}
OutOfHere:
	uDialogUpdate(uList); 
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}
int setGlobalOptions(IconPtr myIcon)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok",     {10,360,65,25},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {230,360,65,25},uDialogButton,uDialogNone},
      
      /*  2 */      {"Save SDS 64 Bits", {10, 20,225,25},uDialogCheck,uDialogGetLong},
      /*  3 */      {"Native Open Dialog", {10, 40,225,25},uDialogCheck,uDialogGetLong},
      
      /* 04 */      {"Macintosh Format", {10,145,180,20},uDialogButtonRadio,uDialogGetLong},
      /* 05 */      {"Unix Format",      {10,165,180,20},uDialogButtonRadio,uDialogGetLong},
      /* 06 */      {"Windows Format",   {10,185,180,20},uDialogButtonRadio,uDialogGetLong},
      
      /* 07 */      {" Text Output", {10,125,180,20},uDialogString,uDialogNone},

      /* 08 */      {"Use Start Configuraton", {10, 60,225,25},uDialogCheck,uDialogGetLong},

      /* 09 */      {"Set Start Configuration", {10,265,175,25},uDialogButton,uDialogNone},

      /* 10 */      {"Sds3D Flip Y", {10, 80,225,25},uDialogCheck,uDialogGetLong},
      /* 11 */      {"Voice Recognition", {10, 100,225,25},uDialogCheck,uDialogGetLong},

       /* 12 */      {"Zones Per Block", {10,215,115,20},uDialogString,uDialogNone},
       /* 13 */      {"Blocksizedata", {10,235,100,20},uDialogText,uDialogGetLong},
			
	   /* 14 */      {"Set Open With Appilcation", {10,295,175,25},uDialogButton,uDialogNone},
			
	   /* 15 */      {"/Applications/mShow.app", {10,330,290,20},uDialogText,uDialogNone},
    
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,320,400};
 	uDialogList *uList;
	uDialogReturn *uR;
    int retr;
    int ret;
    
    
    if(!myIcon)return 1;
	
	ret=1;
	
	uR=NULL;
	
	
	uList=uDialogOpen("Global Preferences",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	retr=uDialogSet(uList,2L,
		          uDialogSetLogical,(uPref.SaveSDS64Bits),

	              uDialogSetItem,3L,
		          uDialogSetLogical,(uPref.LocalOpenDialog),
	              
	              uDialogSetItem,4L,
		          uDialogSetLogical,(uPref.TextOutputFormat == MacintoshTextType),
	              
	              uDialogSetItem,5L,
		          uDialogSetLogical,(uPref.TextOutputFormat == UnixTextType),
	              
	              uDialogSetItem,6L,
		          uDialogSetLogical,(uPref.TextOutputFormat == WindowsTextType),
	              
	              uDialogSetItem,8L,
		          uDialogSetLogical,(uPref.UseStartConfiguration),
		          
	              uDialogSetItem,10L,
		          uDialogSetLogical,(uPref.flipSDS2dY),
	              uDialogSetAction,doScale2Action,
	              	              
	              uDialogSetItem,11L,
		          uDialogSetLogical,(uPref.VoiceRecognition),
	              uDialogSetAction,doScale2Action,
	              	              
	              uDialogSetItem,13L,
	              uDialogSetDouble,(double)uPref.BlockSizeGlobal,
					
				  uDialogSetItem,15L,	              
				  uDialogSetText,uPref.OpenWithApplication,
					
	              uDialogSetDone,uDialogSetDone
	    );
	if(retr)goto ErrorOut;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
	    
	uPref.SaveSDS64Bits=  (int)uR[2].lreturn;    
	uPref.LocalOpenDialog=(int)uR[3].lreturn;    
	uPref.TextOutputFormat=MacintoshTextType;
	if(uR[5].lreturn)uPref.TextOutputFormat=UnixTextType;
	if(uR[6].lreturn)uPref.TextOutputFormat=WindowsTextType;
	uPref.UseStartConfiguration=(int)uR[8].lreturn;    
	
	uPref.flipSDS2dY=(int)uR[10].lreturn;
	uPref.VoiceRecognition=(int)uR[11].lreturn;
	
	if(uPref.UseStartConfiguration && !uPref.startConfiguration){
		uPref.startConfiguration=FileGetFilePath("Find Start Confguration",&myIcon->infoLoad);
	}
	
	uPref.BlockSizeGlobal=uR[13].lreturn;
	if(uPref.BlockSizeGlobal < 0)uPref.BlockSizeGlobal=0;
	
	if(uPref.OpenWithApplication)cFree((char *)uPref.OpenWithApplication);
	uPref.OpenWithApplication=strsave(uR[15].sreturn,9934);
	
	writeUniversal(myIcon,0);
	    
	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
        
	return ret;
}


