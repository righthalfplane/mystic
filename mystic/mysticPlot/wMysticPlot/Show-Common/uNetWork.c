#define EXTERN22 extern
#include <ctype.h>
#include "Xappl.h"
#include "uLib.h"
#include "uFiles.h"
#include "paletteDraw.h"
#include "Linedata.h"
#include "Message.h"
#include "uDialog.h"
#include "FileManager.h"
#include "Tools.h"
#include "Raster2D.h"


int FrameDataToRaster(struct SetFrameData  *sd,unsigned char *out);

struct FileInfo2 *FilesTRUE(void);
IconPtr OpenTekFiles(struct FileInfo2 *Files);

int FilesSaveTRUEToMemory(char *name,unsigned char *bline,
			  unsigned char *bout,long xsize,
			  long ysize,struct FileInfo2 *Files);
			  
int FilesGetPICT(char *name,unsigned char *buff,long length,IconPtr myIcon,struct FileInfo2 **Files);

int doSystem(void);

int MakeNewDirectory(char *name);


int uMAB(struct uNetWork *uNet,IconPtr myIcon,char *path);

int isDirectory(char *pathname);

int uConnection(IconPtr myIcon,struct uNetWork *uNet,char *commandi,char *path);
static int uCATN(struct uNetWork *uNet,IconPtr myIcon,char *path);
static int uTEXT(struct uNetWork *uNet,IconPtr myIcon,char *path,int flag);
static int uPAL8(struct uNetWork *uNet,IconPtr myIcon);
static int uLIMI(struct uNetWork *uNet,IconPtr myIcon);
static int uARGC(struct uNetWork *uNet,IconPtr myIcon);
static int uTEKB(struct uNetWork *uNet,IconPtr myIcon);
static int uH8CM(struct uNetWork *uNet,IconPtr myIcon);
static int uFP32(struct uNetWork *uNet,IconPtr myIcon);
static int uTRUE(struct uNetWork *uNet,IconPtr myIcon);
static int uPICT(struct uNetWork *uNet,IconPtr myIcon);

char *strpathname(char *path,char *name);


int uNetStart(IconPtr myIcon)
{
	extern int doNetWork;
	char name[256];
	char buff[256];
	
	if(!myIcon)return 1;
	
	if(doNetWork){
	    myIcon->uNet.Net=NetStart(myIcon);
	    if(myIcon->uNet.Net == NULL){
	        WarningBatch("NetStart Failed - NetWork Operations Disabled\n");
	    	doNetWork=FALSE;
	    }else{
		    unsigned long inAddr;

 		    inAddr=myIcon->uNet.Net->ipNum;
 		    GetWindowName(myIcon,name,0);
            sprintf(buff,"%s [%d.%d.%d.%d]",name,	        
	           (unsigned int)((inAddr >> 24) & 0xff),
	           (unsigned int)((inAddr >> 16) & 0xff),
	           (unsigned int)((inAddr >> 8) & 0xff),
	           (unsigned int)((inAddr ) & 0xff));
			SetWindowName(myIcon,buff);	
	    }
	}


	
	return 0;
}
int uNetStop(IconPtr myIcon)
{
	if(!myIcon)return 1;
	
    uNetListenStop(&myIcon->uNet);
	
    if(myIcon->uNet.Net)NetStop(myIcon->uNet.Net);
    myIcon->uNet.Net=NULL;
 	if(myIcon->uNet.pathname)cFree(myIcon->uNet.pathname);
 	myIcon->uNet.pathname=NULL;
 	FileDialogRemoveInfo(&myIcon->uNet.infoSave);	
	return 0;	
}
int uNetListenStop(struct uNetWork *uNet)
{
	if(!uNet)return 1;
    if(uNet->Net)NetListenStop(uNet->Net);
    uNet->ListenFlag=FALSE;
	return 0;
}
int uNetStartListen(struct Icon *myIcon)
{
	long ret;
	
	if(!myIcon)return 1;
	
	uNetListenStop(&myIcon->uNet);
	
	ret=NetListenStart(myIcon->uNet.Net,&myIcon->uNet.ListenPort);
	if(ret == 0){
	    myIcon->uNet.ListenFlag=TRUE;
	    return 0;
	}else{
	    myIcon->uNet.ListenFlag=FALSE;
	    sprintf(WarningBuff,"uNetStartListen Error %ld \n",(long)ret);
	    WarningBatch(WarningBuff);
	    return 1;
	}
}
int uNetStopListen(struct Icon *myIcon)
{
	if(!myIcon)return 1;
 	uNetListenStop(&myIcon->uNet);
	return 0;
}
int uNetSetStartListen(struct Icon *myIcon)
{
	char *pathname;
	
	if(!myIcon)return 1;
	
	if(myIcon->uNet.pathname){
		if(myIcon->uNet.infoSave.directory){
		    cFree(myIcon->uNet.infoSave.directory);
		}		
	   	myIcon->uNet.infoSave.directory=strsave(myIcon->uNet.pathname,8866);
	}
	
	pathname=FileGetPath("Put In Coming Data Here",&myIcon->uNet.infoSave);
	if(!pathname)return 1;
	
 	if(myIcon->uNet.pathname)cFree(myIcon->uNet.pathname);
 	myIcon->uNet.pathname=pathname;
	
	return uNetStartListen(myIcon);
	
}
int uNetCheckConnection(struct Icon *myIcon)
{
	char command[256],*commandi;
	long ret;
	INT8_64 size;
	long sizel;
	int n;
	
	if(!myIcon)return 1;
	
	if(!myIcon->uNet.ListenFlag)return 0;
	
    ret=NetListenCheck(myIcon->uNet.Net);
    if(ret == 1){
         return 0;
    } else if(ret < 0){
        sprintf(WarningBuff,"NetListenCheck error %ld\n",(long)ret);
        WarningBatch(WarningBuff);
        uNetListenStop(&myIcon->uNet);
        return 0;
    }
    
    myIcon->uNet.igotPalette = FALSE;
    myIcon->uNet.Files  = NULL;
    myIcon->uNet.limits = NULL;
    myIcon->uNet.Net->iRead64=FALSE;

	SetPortMyWindow(myIcon);
	
    commandi=NULL;
    if(netGetType(myIcon->uNet.Net,command,&sizel))goto ErrorOut;
    if(!strcmp(command,"ERRC")){
        myIcon->uNet.Net->netErrorCheck=1;
        myIcon->uNet.Net->netsize=BUFFER_SIZE;
        myIcon->uNet.Net->netsizeMax=BUFFER_SIZE;
        myIcon->uNet.Net->netcount=BUFFER_SIZE;
        myIcon->uNet.Net->netblock=0;
    }else{
        myIcon->uNet.Net->netErrorCheck=0;
	    if(myIcon->uNet.ListenPassword[0])goto Start;
	    commandi=command;
    }
	
	if(myIcon->uNet.ListenPassword[0]){
	
		if(NetCommand(myIcon->uNet.Net,command,&size)){
		    WarningBatch("Error Trying to Read PASS Command\n");
	    	    goto ErrorOut;
		}
Start:
		if(!strcmp(command,"PASS")){
		    if(NetName(myIcon->uNet.Net,command)){
		        WarningBatch("Error Trying to Read Password\n");
	    	        goto ErrorOut;
		     }
		    if(strcmp(command,myIcon->uNet.ListenPassword)){
		        WarningBatch("Error Password incorrect\n");
	    	        goto ErrorOut;			
		    }
		}else{
		    WarningBatch("Error Password Command Not Found\n");
	    	    goto ErrorOut;
		}
		
	}
	

	while(!uConnection(myIcon,&myIcon->uNet,commandi,myIcon->uNet.pathname)){
	    commandi=NULL;
	    doSystem();
	}

ErrorOut:

	if(NetClose(myIcon->uNet.Net)){
	    WarningBatch("NetClose Error\n");
	}
    
    myIcon->uNet.Connected=FALSE;

    uNetStartListen(myIcon);
    	    	    
    myIcon->uNet.ListenFlag = TRUE;
    
    if(myIcon->uNet.Files){
        if(myIcon->uNet.igotPalette){
            for(n=0;n<768;++n)myIcon->uNet.Files->palette[n]=myIcon->uNet.palette[n];
        }
    	if(myIcon->uNet.Files->fileClass == TEK_FILES){
    		OpenTekFiles(myIcon->uNet.Files);
    		myIcon->uNet.Files=NULL;
    	}else if(myIcon->uNet.Files->fileClass == HDF_FILES){
    	    if(myIcon->uNet.SaveHdfToDisk){
    	        memcpy(myIcon->uNet.Files->palette,myIcon->uNet.palette,768);
    	        mstrncpy(myIcon->uNet.Files->outputDirectory,myIcon->uNet.pathname,1024);
    	        myIcon->uNet.Files->outputName[0]=0;
    	        myIcon->uNet.Files->outputType = HDF_FILES;
    	        FilesSaveFiles(myIcon->uNet.Files);    	         
    	    }
    	    if(myIcon->uNet.SaveHdfToMemory){
    	         if(myIcon->uAddFiles){
    	           (*myIcon->uAddFiles)(myIcon,myIcon->uNet.Files);
    	         }else{
    	             FilesOpenDefault(myIcon->uNet.Files);
    	         }
    	         myIcon->uNet.Files=NULL;
    	    }
		}else if(myIcon->uNet.Files->fileClass == SDS2D_FILES){
			if(myIcon->uAddFiles){
    	           (*myIcon->uAddFiles)(myIcon,myIcon->uNet.Files);
			}else{
				FilesOpenDefault(myIcon->uNet.Files);
			}
			myIcon->uNet.Files=NULL;
		}else if(myIcon->uNet.Files->fileClass == SDS3D_FILES){
			FilesOpenDefault(myIcon->uNet.Files);
			myIcon->uNet.Files=NULL;
    	}else if(myIcon->uNet.Files->fileClass == TRUE_FILES){
    	    if(myIcon->uNet.SaveTrueToDisk){
    	        memcpy(myIcon->uNet.Files->palette,myIcon->uNet.palette,768);
    	        mstrncpy(myIcon->uNet.Files->outputDirectory,myIcon->uNet.pathname,1024);
    	        myIcon->uNet.Files->outputName[0]=0;
    	        myIcon->uNet.Files->outputType = TRUE_FILES;
    	        FilesSaveFiles(myIcon->uNet.Files);    	         
    	    }
    	    if(myIcon->uNet.SaveTrueToMemory){
    	         if(myIcon->uAddFiles){
    	           (*myIcon->uAddFiles)(myIcon,myIcon->uNet.Files);
    	         }else{
    	             FilesOpenDefault(myIcon->uNet.Files);
    	         }
    	         myIcon->uNet.Files=NULL;
    	    }
    	}else if(myIcon->uNet.Files->fileClass == PICT_FILES){
	         if(myIcon->uAddFiles){
	           (*myIcon->uAddFiles)(myIcon,myIcon->uNet.Files);
	         }else{
	             FilesOpenDefault(myIcon->uNet.Files);
	         }
	         myIcon->uNet.Files=NULL;
    	}
	    if(myIcon->uNet.Files){
	        freeFileList(myIcon->uNet.Files);
	        myIcon->uNet.Files=NULL;
	    }
    	myIcon->uNet.Files=NULL;
    }
    
	return 0;
}


int uConnection(IconPtr myIcon,struct uNetWork *uNet,char *commandi,char *path)
{
	char command[256];
	INT8_64 size;
	
	if(!myIcon || !uNet || !path)return 1;

	size=0;
	
	if(commandi){
	    mstrncpy(command,commandi,255);
	}else{
		if(NetCommand(uNet->Net,command,&size)){
		    return 1;
		}
	}
	if(!strcmp(command,"H8CM")){
	    return uH8CM(uNet,myIcon);
	}else if(!strcmp(command,"FP32")){
	    return uFP32(uNet,myIcon);
	}else if(!strcmp(command,"LIMI")){
	    return uLIMI(uNet,myIcon);
	}else if(!strcmp(command,"PAL8")){
	    return uPAL8(uNet,myIcon);
	}else if(!strcmp(command,"TEKB")){
	    return uTEKB(uNet,myIcon);
	}else if(!strcmp(command,"TEXT")){
	    return uTEXT(uNet,myIcon,path,1);
	}else if(!strcmp(command,"BINA")){
	    return uTEXT(uNet,myIcon,path,0);
	}else if(!strcmp(command,"ARGC")){
	    return uARGC(uNet,myIcon);
	}else if(!strcmp(command,"MAB2")){
	    return uMAB(uNet,myIcon,path);
	}else if(!strcmp(command,"CATN")){
	    return uCATN(uNet,myIcon,path);
	}else if(!strcmp(command,"TRUE")){
	    return uTRUE(uNet,myIcon);
	}else if(!strcmp(command,"PICT")){
	    return uPICT(uNet,myIcon);
	}else if(!strcmp(command,"BT64")){
		uNet->Net->iRead64=TRUE;
	    return 0;
	}else if(!strcmp(command,"PASS")){
	     return Flush(uNet->Net,size);
	}else if(!strcmp(command,"ENDT")){
	    return 1;
	}else{
	    if(isalnum(command[0]) && isalnum(command[1]) &&
	       isalnum(command[2]) && isalnum(command[3])){
	        sprintf(WarningBuff,"Flushing Unknown Command Type (%s)",command);
	        WarningBatch(WarningBuff);
	        return Flush(uNet->Net,size);
	    }else{
	        sprintf(WarningBuff,"Bad Header (%d) (%d) (%d) (%d)",command[0],
	        	command[1],command[2],command[3]);
	        WarningBatch(WarningBuff);
	        return 1;
	    }
	}
}
int uTRUE(struct uNetWork *uNet,IconPtr myIcon)
{
	uRect BRect,DRect;
	long length;
	int xmax,ymax;
	unsigned int Esc1,Esc2;
	long UnCompressed,Compressed;
	long LengthC;
	char name[256];
	uPort save;
	int ret;
	
	if(!myIcon || !uNet)return 1;
	ret=1;
	
	if(NetName(uNet->Net,name)){
	    return 1;
	}
	
	if(NetLong(uNet->Net,&LengthC))return 1;	
			
 	if(NetTag(uNet->Net)){
	    sprintf(WarningBuff,"uTRUE (%s) Not A True File\n",name);
	    WarningBatch(WarningBuff);
	    goto EndIt;
	}
	if(NetIntr(uNet->Net,&xmax)){
	    sprintf(WarningBuff,"uTRUE (%s) Error Reading True xmax\n",name);
	    WarningBatch(WarningBuff);
	    goto EndIt;
	}
	if(NetIntr(uNet->Net,&ymax)){
	    sprintf(WarningBuff,"uTRUE (%s) Error Reading True ymax\n",name);
	    WarningBatch(WarningBuff);
	    goto EndIt;
	}
	if(NetIntr(uNet->Net,(int *)&Esc1)){
	    sprintf(WarningBuff,"uTRUE (%s) Error Reading True Esc1\n",name);
	    WarningBatch(WarningBuff);
	    goto EndIt;
	}
	if(NetIntr(uNet->Net,(int *)&Esc2)){
 	    sprintf(WarningBuff,"uTRUE (%s) Error Reading True Esc2\n",name);
	    WarningBatch(WarningBuff);
	    goto EndIt;
	}
	if(NetLongr(uNet->Net,&UnCompressed)){
	    sprintf(WarningBuff,"uTRUE (%s) Error Reading True UnCompressed\n",name);
	    WarningBatch(WarningBuff);
	    goto EndIt;
	}
	
	if(NetLongr(uNet->Net,&Compressed)){
 	    sprintf(WarningBuff,"uTRUE (%s) Error Reading True Compressed\n",name);
	    WarningBatch(WarningBuff);
	    goto EndIt;
	}
	
	
	length=3L*((long)ymax)*xmax;
	if(UnCompressed > length)length=UnCompressed;
	if(Compressed   > length)length=Compressed;
	
	if(SetBuffers(length+512L))goto EndIt;
	
    if(NetGet(uNet->Net,xg.buffout,Compressed)){
	    sprintf(WarningBuff,"uTRUE (%s) Error Reading True Data (Bytes %ld)\n",name,Compressed);
	    WarningBatch(WarningBuff);
	    goto EndIt;
    }

	if(UnCompress(Esc1,Esc2,UnCompressed,Compressed,(unsigned char *)xg.buffout,(unsigned char *)xg.buffin)){
	    sprintf(WarningBuff,"uTRUE (%s) Error UnCompress Error\n",name);
	    WarningBatch(WarningBuff);
	    goto EndIt;
	}
	
	if(uNet->Files == NULL){
        uNet->Files=FilesTRUE();
        if(!uNet->Files)goto EndIt;
	}
			    	    
	ret=FilesSaveTRUEToMemory(name,(unsigned char *)xg.buffin,(unsigned char *)xg.buffout,xmax,ymax,uNet->Files);
	
	BRect.x=0;
	BRect.ysize=ymax;
	BRect.y=0;
	BRect.xsize=xmax;
	
	DRect=BRect;

	ScreenSetSize(xmax,ymax,myIcon);
	
	uGetPort(&save);
	SetPortMyWindow(myIcon);
	ScreenSetPalette(myIcon,myIcon->red,myIcon->green,myIcon->blue);
	uScreenWrite(xmax,ymax,(unsigned char *)xg.buffin,(unsigned char *)xg.buffout,ScreenDataType24,&BRect,&DRect,myIcon);
	uSetPort(&save);
	
EndIt:	

	return ret;
	
}
static int uH8CM(struct uNetWork *uNet,IconPtr myIcon)
{
	int xsize,ysize;
	long length,HdfSize;
	uRect BRect,DRect;
	char name[256];
	uPort save;

	if(!uNet || !myIcon)return 1;
		
	if(NetName(uNet->Net,name)){
	    return 1;
	}
	
	if(NetInt(uNet->Net,&xsize))return 1;
	if(NetInt(uNet->Net,&ysize))return 1;
	if(NetLong(uNet->Net,&length))return 1;
	HdfSize=((long)xsize)*ysize;
	
	if(SetBuffers(HdfSize+512L))return 1; /* Must Make Extra Room for Unpack */ 

	if(NetGet(uNet->Net,xg.buffin,length)){
	    return 1;
	}
	
	
    if(FileAppendHdf(name,xsize,ysize,(unsigned char *)xg.buffin,length,&(uNet->Files))){
        return 1;
    }
	
	UnPack((unsigned char *)xg.buffout,(unsigned char *)xg.buffin,HdfSize);
	
	BRect.x=0;
	BRect.ysize=ysize;
	BRect.y=0;
	BRect.xsize=xsize;
	
	DRect=BRect;
	
	uGetPort(&save);
	SetPortMyWindow(myIcon);
	
	ScreenSetPalette(myIcon,myIcon->red,myIcon->green,myIcon->blue);
	
	ScreenSetSize(xsize,ysize,myIcon);
    uScreenWrite(xsize,ysize,(unsigned char *)xg.buffout,(unsigned char *)xg.buffin,ScreenDataType8,&BRect,&DRect,myIcon);
            			
	uSetPort(&save);
	
	return 0;
}
static int uFP32(struct uNetWork *uNet,IconPtr myIcon)
{
	int xsize,ysize,zsize,rank;
	long length,HdfSize;
	char name[256];
	uPort save;
	int ret;

	if(!uNet || !myIcon)return 1;
		
	if(NetName(uNet->Net,name)){
	    return 1;
	}
	
	if(NetInt(uNet->Net,&rank))return 1;
	
	xsize=ysize=zsize=0;

	/* Flip dimensions depending on rank */
	if (rank==2) {
		if(NetInt(uNet->Net,&ysize))return 1;
		if(NetInt(uNet->Net,&xsize))return 1;
		if(NetInt(uNet->Net,&zsize))return 1;
		zsize=1;
	} else if (rank==3) {
		if(NetInt(uNet->Net,&zsize))return 1;
		if(NetInt(uNet->Net,&ysize))return 1;
		if(NetInt(uNet->Net,&xsize))return 1;
	}
	if(NetLong(uNet->Net,&length))return 1;

	HdfSize=(long)xsize;
	if (rank>1) HdfSize*=ysize;
	if (rank>2) HdfSize*=zsize;
	HdfSize *= sizeof(double);
	
	if(SetBuffers(HdfSize+512L))return 1; /* Must Make Extra Room for Unpack */ 

	if(NetGet(uNet->Net,xg.buffin,length)){
	    return 1;
	}

#ifdef CRAY
	/*   do cray */
	return 1;
	
#else
	
#ifdef PC
	/*   do PC */
	{
		unsigned char c,*cp;
		long np;
		cp=(unsigned char *)xg.buffin;
		for(np=0;np<length;np += 4){
			c=cp[np];
			cp[np]=cp[np+3];
			cp[np+3]=c;
			c=cp[np+1];
			cp[np+1]=cp[np+2];
			cp[np+2]=c;
		}
	}
#endif
	
#endif
	
	{
		uRect BRect,DRect;
		float *data3d;
		double *data=NULL;
		int i,j,n,nn;
		double dmin,dmax;
		struct SetFrameData sd;


		data=(double *)cMalloc(xsize*ysize*zsize*sizeof(double),8661);
		if(!data) return 1;

		data3d = (float *)xg.buffin;

		
		for(nn=0;nn<xsize*ysize*zsize;++nn){
			data[nn]=data3d[nn];
		}
		
		if (rank==2) {
			
			if(FileAppendSDS2D(name,xsize,ysize,data,9999L,&(uNet->Files))){
				goto ErrorOut;
			}
			

		} else if (rank==3) {
			if(FileAppendSDS3D(name,xsize,ysize,zsize,data,9999L,&(uNet->Files))){
				goto ErrorOut;
			}


		} else {
			goto ErrorOut;
		}


		zerol((char *)&sd,sizeof(&sd));
		
		
		n=0;
		
		nn=0;
		for(j=0;j<ysize;++j){
			for(i=0;i<xsize;++i){
				data[i+j*xsize]=data3d[n*xsize*ysize+nn++];
			}
		}
		
		
				
		dmin =  1e33;
		dmax = -1e33;
		for(nn=0;nn<xsize*ysize;++nn){
			if(data[nn] < dmin)dmin=data[nn];
			if(data[nn] > dmax)dmax=data[nn];
		}
		if(dmin >= dmax){
			dmin = dmax - 1.0;
			++dmax;
		}
		
		sd.pd.sPmin = dmin;
		sd.pd.sPmax = dmax;
		
		sd.pd.dmin = dmin;
		sd.pd.dmax = dmax;
		sd.pd.sType = 0;
		
		for(n=0;n<256;++n){
			sd.pd.palette[3*n+0] = myIcon->red[n];
			sd.pd.palette[3*n+1] = myIcon->green[n];
			sd.pd.palette[3*n+2] = myIcon->blue[n];
		}
		
		sd.data = data;
		sd.xsize = xsize;
		sd.ysize = ysize;
		sd.zsize = 0;
		
		BRect.x=0;
		BRect.ysize=ysize;
		BRect.y=0;
		BRect.xsize=xsize;
		
		DRect=BRect;
		
		uGetPort(&save);
		SetPortMyWindow(myIcon);
		ScreenSetPalette(myIcon,myIcon->red,myIcon->green,myIcon->blue);
		ScreenSetSize(xsize,ysize,myIcon);
		FrameDataToRaster(&sd,xg.buffout);
		
		ret=uScreenWrite(xsize,ysize,(unsigned char *)xg.buffout,(unsigned char *)data,
			ScreenDataType8,&BRect,&DRect,myIcon);

		uSetPort(&save);
ErrorOut:
		sd.data = NULL;
		if (data) cFree((char*)data);
	}
	
	return 0;
}
static int uLIMI(struct uNetWork *uNet,IconPtr myIcon)
{
	char *limits;
	int n;
	
	if(!uNet || !myIcon){
	    return 1;
	}

	if(NetByte(uNet->Net,&n))return 1;
	
	limits=cMalloc(n+4,6291);
	if(!limits)return 1;
	
	if(n > 0){
	    if(NetGet(uNet->Net,(unsigned char *)limits,(long)n)){
	        return 1;
	    }
	}else{
	    n=0;
	}	

	limits[n]=0;
	
	if(uNet->Files){
	    FilePutLimitsString(limits,uNet->Files->ImageCount-1,uNet->Files);
	}
		
	return 0;
}
static int uTEKB(struct uNetWork *uNet,IconPtr myIcon)
{
	char name[256],*buff;
	INT8_64 length8;
	long length;
	
	if(!myIcon || !uNet)return 1;
		
	if(NetName(uNet->Net,name)){
	    return 1;
	}
	
	if(myIcon->uNet.Net->iRead64){
	   if(NetLongLong(uNet->Net,&length8))return 1;
	}else{	
	   if(NetLong(uNet->Net,&length))return 1;
	   length8=length;
	}

	length=(long)length8;	
	
	if(length8 > length)return 1;	
	
	if(!uNet->Files){
	    uNet->Files=FilesTEK();
	    if(!uNet->Files){
	        Flush(uNet->Net,length8);
	        return 1;
	    }
	}

	buff=cMalloc(length,8178);
	if(!buff){
	    Flush(uNet->Net,length8);
	    return 1;
	}

	if(NetGet(uNet->Net,(unsigned char *)buff,length8)){
 	    cFree((char *)buff);
	    return 1;
	}
	return FilesSaveTEKToMemory(name,(unsigned char *)buff,length,uNet->Files);
}
static int uARGC(struct uNetWork *uNet,IconPtr myIcon)
{
	int n;
	
	if(!uNet || !myIcon)return 1;
	
	if(NetInt(uNet->Net,&n))return 1;	
	
	return 0;
}
static int uPAL8(struct uNetWork *uNet,IconPtr myIcon)
{
	
	if(!uNet || !myIcon)return 1;
	
	uNet->igotPalette=FALSE;
	
	if(NetName(uNet->Net,uNet->paletteName)){
	    return 1;
	}
	
	if(NetGet(uNet->Net,(unsigned char *)uNet->palette,(long)768)){
	    return 1;
	}
	
	uNet->igotPalette=TRUE;
	
	return 0;
}
static int uTEXT(struct uNetWork *uNet,IconPtr myIcon,char *path,int text)
{
	FILE *out;
	char *newpath;
	char name[256];
	long length,n,k;
	INT8_64 length8,n64;
	int ret;
	
	if(!uNet || !myIcon || !path){
	    return 1;
	}
	
	ret=1;
	out=NULL;
	newpath=NULL;

	if(NetName(uNet->Net,name)){
	    return 1;
	}

	newpath=strpathname(path,name);
	if(!newpath)return 1;
	
	out=fopen(newpath,"wb");
	if(!out){
	    sprintf(WarningBuff,"uTEXT Error Opening \"%s\" To write\n",newpath);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	if(myIcon->uNet.Net->iRead64){
	   if(NetLongLong(uNet->Net,&length8))return 1;
	}else{	
	   if(NetLong(uNet->Net,&length))return 1;
	   length8=length;
	}	
			
	if(SetBuffers(IO_BUFFER+512L))return 1;
	
	while(length8 > 0){	
	    n64=length8;	    
	    if(n64 > IO_BUFFER)n64=IO_BUFFER;	 
	    n=(long)n64;
	    if(NetGet(uNet->Net,xg.buffin,n))goto ErrorOut;
	    
	    if(text){
	        for(k=0;k<n;++k){
	            if(xg.buffin[k] == '\n')xg.buffin[k]='\r';
	        }
	    }
	    
	    if(putString((unsigned char *)xg.buffin,n,out))goto ErrorOut;
	    length8 -= n;
		doSystem();
	    
	}
		
	ret=0;
ErrorOut:
	if(text){
	    SetFileTypeOwner(newpath,uNet->TEXTOwner,uNet->TEXTType);
	}else{
	    SetFileTypeOwner(newpath,uNet->BinaryOwner,uNet->BinaryType);
	}

	if(newpath)cFree(newpath);
	if(out)fclose(out);
	
	return ret;
}

static int uCATN(struct uNetWork *uNet,IconPtr myIcon,char *path)
{
	char name[256];
	char *newpath;
	INT8_64 size;
	int ret;
	
	if(!uNet || !myIcon || !path){
	    sprintf(WarningBuff,"doCATN Pointer %lx %lx %lx\n",(long)uNet,(long)myIcon,(long)path);
	    WarningBatch(WarningBuff);
	    return 1;
	}

	if(NetName(uNet->Net,name)){
	    return 1;
	}
	
	newpath=strpathname(path,name);
	if(!newpath)return 1;
	
	ret=1;
	
    if(!isDirectory(newpath)){
	    if(MakeNewDirectory(newpath)){
	          sprintf(WarningBuff,"Directory %s Could Not Be Found Or Created\n",newpath);
	          WarningBatch(WarningBuff);
	          goto ErrorOut;
	    }
    }
		
    while(1){
	    if(NetCommand(uNet->Net,name,&size)){
            ret=1;
	        break;
	    }
        if(!strcmp(name,"CATE")){
            ret=0;
            break;
        }
	    ret=uConnection(myIcon,uNet,name,newpath);
	    if(ret){
	        break;
	    }
		doSystem();	
	}
	
ErrorOut:

	goCD(path);

	if(newpath)cFree(newpath);


	return ret;
}
static int uPICT(struct uNetWork *uNet,IconPtr myIcon)
{

	char name[256];
	INT8_64 length8;
	long length;
	int ret;
	
	if(!uNet || !myIcon)return 1;


	ret=1;
	
	if(NetName(uNet->Net,name)){
	    return 1;
	}
	
	if(myIcon->uNet.Net->iRead64){
	   if(NetLongLong(uNet->Net,&length8))return 1;
	}else{	
	   if(NetLong(uNet->Net,&length))return 1;
	   length8=length;
	}

	length=(long)length8;		
	
	if(length8 > length)return 1;
	
	if(SetBuffers(length+512L))return 1;
	
	if(NetGet(uNet->Net,xg.buffin,length8))return 1;
	
	if(FilesGetPICT(name,(unsigned char *)xg.buffin,length,myIcon,&(uNet->Files))){
	    goto ErrorOut;
	}

	ret=0;
ErrorOut:

	return ret;
	
}
int uSendDirectoryName(struct NetWork *Net,char *buff)
{
	int n,ret;
	
	if(!Net)return 1;
	ret=1;
	
	n=(int)strlen(buff);
	
	SendLab(Net,"CATN",(long)(1+n));
	
	if(SendByte(Net,n))goto Out;
	if(n > 0){
	    if(SendData(Net,buff,(long)n))goto Out;
	}
	
	ret=0;
Out:
	return ret;
}

int Flush(struct NetWork *Net,INT8_64 size)
{
	INT8_64 n64;
	long n;
	
	while(size > 0){
	    n64=size;
	    if(n64 > xg.buffsize)n64=xg.buffsize;
	    n=(long)n64;
	    if(NetGet(Net,xg.buffin,n))return 1;
	    size -= n;
	}
	return 0;
}
int NetCommand(struct NetWork *Net,char *command,INT8_64 *size)
{
	long sizel;
	
	if(!Net || !command || !size)return 1;
	
    if(NetGet(Net,(unsigned char *)command,4L)){
        return 1;
    }
    command[4]=0;
	    
    if(Net->iRead64){
    
	    return NetLongLong(Net,size);
    
    }else{
	    
	    if(NetLong(Net,&sizel))return 0;
	    
	    *size=sizel;
    
    }
	    
	return 0;
}
int NetLong(struct NetWork *Net,long *xsize)
{
	unsigned char buff[4];
	
	if(NetGet(Net,buff,4L))return 1;
	*xsize =(long)( buff[0] + ((( unsigned long)buff[1]) << 8) + 
	         (((unsigned long)buff[2]) << 16) + (((unsigned long)buff[3]) << 24));
	return 0;
}
int NetLongLong(struct NetWork *Net,INT8_64 *xsize)
{
	unsigned char buff[8];
	
	if(NetGet(Net,buff,8L))return 1;
	*xsize = buff[0] + (((unsigned long long)buff[1]) << 8) + 
	         (((unsigned long long)buff[2]) << 16) + (((unsigned long long)buff[3]) << 24)+
	         (((unsigned long long)buff[4]) << 32) + (((unsigned long long)buff[5]) << 40)+
	         (((unsigned long long)buff[6]) << 48) + (((unsigned long long)buff[7]) << 56);
	return 0;
}
int NetByte(struct NetWork *Net,int *xsize)
{
	unsigned char buff[1];
	
	if(NetGet(Net,buff,1L))return 1;
	*xsize = buff[0];
	return 0;
}
int NetInt(struct NetWork *Net,int *xsize)
{
	unsigned char buff[2];
	
	if(NetGet(Net,buff,2L))return 1;
	*xsize = buff[0] + (((unsigned)buff[1]) << 8);
	return 0;
}
