#define EXTERN22 extern
#include <ctype.h>
#include "Xappl.h"
#include "uFiles.h"
#include "Message.h"
#include "uLib.h"
#include "uDialog.h"
#include "SaveAllImages.h"
#include "Universal.h"
#include "uDraw.h"
#include "FileManager.h"
#include "Message.h"

static int drawTek2(IconPtr myIcon,uTekPtr t,uRect *DRect,struct DOList *d);

int uTekSet(uTekPtr b);
static void uTekDelete(DObjPtr o);
static int uTekMessage(DObjPtr o,long MessageType,void *MessageData);

static char *uTekCopy(DObjPtr o,long *Length);

static DObjPtr uTekDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff);

static uTekPtr uTekdoBoxes(uTekPtr b,rRect *r,struct uAttributes *vAttributes);


static DObjPtr uTekMove(DObjPtr o,double *dx,double *dy);

static int uTekDraw(DObjPtr o,struct DOList *l);

uTekPtr uTekImage2(uTekPtr Pastin,DOListPtr l);

int WriteRect8ToRect24(unsigned char *in, uRect *SRect,long xsize, long ysize,
                       unsigned char *out,uRect *DRect,long oxsize,long oysize,
                       unsigned char *palette);

static int uTekFrameCount(struct DObject *Object,long *FrameCount);

static int uTekSetFrame(struct DObject *Object,long CurrentFrame);

static struct DObject *uTekDoubleClick(struct DObject *o,rPoint *r);
static DObjPtr uTekDoInformation(DObjPtr o);


static struct DObject *uTekDoubleClick(struct DObject *o,rPoint *r)
{
	if(!o || !r)return NULL;

	if(r->x < o->box.x || r->y < o->box.y){
		return NULL;
	}
	
	if(r->x > o->box.x+o->box.xsize || r->y > o->box.y+o->box.ysize){
		return NULL;
	}			
	return uTekDoInformation(o);
}

static struct DObject *uTekWrite(struct DObject *o,FILE8 *inOut,struct DOList *l)
{


	static struct uTek b;
	
	static struct universalTableListStruct Local[] = {
		{"STRI",4014,universal_TypeBytePtr,(void *)&b.dataPtr,0L},
		
		{"DOUB",4015,universal_TypeDouble,(void *)&b.Attributes.xLineWidth},	
		{"DOUB",4016,universal_TypeDouble,(void *)&b.Attributes.yLineWidth},	
		
		
		{"REAL",4017,universal_TypeInt,(void *)&b.Attributes.LinePattern},	
		{"REAL",4018,universal_TypeInt,(void *)&b.Attributes.LineSymbol},	
		{"REAL",4019,universal_TypeInt,(void *)&b.Attributes.SolidPattern},	
		{"REAL",4020,universal_TypeInt,(void *)&b.Attributes.nLineFore},	
		{"REAL",4021,universal_TypeInt,(void *)&b.Attributes.nLineBack},	
		{"REAL",4022,universal_TypeInt,(void *)&b.Attributes.nSolidFore},	
		{"REAL",4023,universal_TypeInt,(void *)&b.Attributes.nSolidBack},
		
		{"REAL",4024,universal_TypeInt,(void *)&b.AngleFlag},
		{"REAL",4025,universal_TypeInt,(void *)&b.FlipFlag},

		{"STRI",4026,universal_TypeString,(void *)&b.name[0],sizeof(b.name)},
		
		{"DOUB",4027,universal_TypeDouble,(void *)&b.box.x},	
		{"DOUB",4028,universal_TypeDouble,(void *)&b.box.y},	
		{"DOUB",4029,universal_TypeDouble,(void *)&b.box.xsize},	
		{"DOUB",4030,universal_TypeDouble,(void *)&b.box.ysize},	
			
		{"REAL",4031,universal_TypeLong,(void *)&b.xsize},
		{"REAL",4032,universal_TypeLong,(void *)&b.ysize},
		
		{"FILE",4033,universal_TypeFilesPtr,(void *)&b.Files},	
		
	};
	
	static struct universalTableStruct Global = {
		"uDraw parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	uTekPtr bp=(uTekPtr)o;
	char head[5];
	long length;
	long tag;
	
	if(!inOut || !l)return NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    Local[0].space=0;
	    b.dataPtr=NULL;
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp=uTekImage(b.xsize,b.ysize,&b.box,&b.Attributes,l);
		if(!bp)goto  ErrorOut;
		length=Local[0].space;
		if(length > 0 && b.dataPtr){
		    if(bp->dataPtr)cFree((char *)bp->dataPtr);		        
		    bp->dataPtr=b.dataPtr;
		    bp->length=length;
		    b.dataPtr=NULL;
		}
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(struct uTek);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;	
		if(uPref.Files){
			bp->Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
		}		
		return (struct DObject *)bp;			
		
	}else{
	    b = *bp;
	    length=b.length;
	    if(b.Files || !b.dataPtr || (length <= 0)){
	    	Local[0].space=0;
	    }else{
	    	Local[0].space=length;
	    }
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (struct DObject *)&b;
ErrorOut:
	return NULL;
}
static int uTekSetFrame(struct DObject *Object,long CurrentFrame)
{
	uTekPtr b=(uTekPtr)Object;
	char *dataPtr;
	long length;
	long n;
	
	if(!b)return 1;
	if(!b->Files)return 0;		
    if(!b->Attributes.Animation)return 0;    
    
    length=0;
    dataPtr=NULL;
	if(FilesGetTEK(&(b->CurrentFileName),&length,(char **)&dataPtr,
	                 CurrentFrame,b->Files)){
	    return 1;
	}
	
	if((length > b->length) || !b->dataPtr){
		if(b->dataPtr)cFree((char *)b->dataPtr);
		b->dataPtr=(unsigned char *)cMalloc(length,8621);
		if(!b->dataPtr)return 1;
	}
	
	if(dataPtr && b->dataPtr){
		for(n=0;n<length;++n){
	    	b->dataPtr[n]=dataPtr[n];
	    }
	}
	
	b->length=length;
		
	return 0;
	
}
static int uTekFrameCount(struct DObject *Object,long *FrameCount)
{
	uTekPtr b=(uTekPtr)Object;
	
	if(!b && !FrameCount)return 1;
	
	if(b->Files){
	    if(b->Files->ImageCount > *FrameCount)*FrameCount=b->Files->ImageCount;
	}
	
	return 0;
	
}
static DObjPtr uTekDoInformation(DObjPtr o)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {240,120,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {335,120,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"0", {115,50,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"10", {115,75,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"1", {115,100,80,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"1", {115,125,80,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"Tek", {223,11,80,20},uDialogText,uDialogNone},
      /*  7 */      {"0", {315,50,80,20},uDialogText,uDialogGetDouble},
      /*  8 */      {"0", {315,75,80,20},uDialogText,uDialogGetDouble},
      /*  9 */      {"Left        :", {25,75,70,20},uDialogString,uDialogNone},
      /* 10 */      {"Object Type", {75,9,90,20},uDialogString,uDialogNone},
      /* 11 */      {"Height    :", {25,100,70,20},uDialogString,uDialogNone},
      /* 12 */      {"Width     :", {25,125,70,20},uDialogString,uDialogNone},
      /* 13 */      {"Top         :", {25,50,70,20},uDialogString,uDialogNone},
      /* 14 */      {"x-scale  :", {220,50,65,16},uDialogString,uDialogNone},
      /* 15 */      {"y-scale  :", {220,75,80,20},uDialogString,uDialogNone},
      
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,410,170};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	uTekPtr pBox;
	uTekPtr b=(uTekPtr)o;
	double xscale,yscale;
	double xsize,ysize;
	struct DOList *l;

	if(!b)return NULL;
	l=b->l;
	if(!l)return NULL;
	
		            
	if(b->xsize > 0){
	    xscale=(double)l->dpi*b->box.xsize/(double)b->xsize;
	}else{
	    xscale=1.0;
	}
	if(b->ysize > 0){
	    yscale=(double)l->dpi*b->box.ysize/(double)b->ysize;
	}else{
	    yscale=1.0;
	}
	
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	pBox=NULL;
	
	uList=uDialogOpen("Tek Information",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)b->box.y,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)b->box.x,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)b->box.ysize,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)b->box.xsize,
	              	              
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,(double)xscale,
	              	              
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)yscale,
	              	              
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	xscale=uR[7].dreturn;
	
	yscale=uR[8].dreturn;

Start:	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	if((uR[7].dreturn != xscale) || (uR[8].dreturn != yscale)){
	    xscale=uR[7].dreturn;
	    if(xscale <= 0.0){
	       Warning("xscale must be greater that zero");
	       goto Start;
	    }
	    yscale=uR[8].dreturn;
	    if(yscale <= 0.0){
	       Warning("yscale must be greater that zero");
	       goto Start;
	    }
	    
	    xsize=xscale*(double)b->xsize/(double)l->dpi;
	    ysize=yscale*(double)b->ysize/(double)l->dpi;

	    
	
	}else{
		xsize=uR[5].dreturn;
		ysize=uR[4].dreturn;
	}
	
	if((pBox=uTekdoBoxes(b,&b->box,&b->Attributes)) == NULL){
	    goto ErrorOut;
	}
	
	pBox->box.x=uR[3].dreturn;
	pBox->box.y=uR[2].dreturn;
	
	pBox->box.xsize=xsize;
	pBox->box.ysize=ysize;
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	if(ret && pBox){
	  uTekDelete((DObjPtr)pBox);
	  pBox=NULL;
	}
	
	return (DObjPtr)pBox;
}
static int uTekDraw(DObjPtr o,struct DOList *d)
{
	uRect DRect;
	uTekPtr b=(uTekPtr)o;
	rRect r;

	if(!b || !d)return 1;
	
	
	if(b->xsize > 0){
	    b->scale=(double)d->dpi*b->box.xsize/(double)b->xsize;
	}else{
	    b->scale=1.0;
	}
	
	r=b->box;
	    	    
	DRect.x=LocalToPixel(r.x,d);
	DRect.y=LocalToPixel(r.y,d);
	    
	DRect.xsize=LocalToPixel(r.xsize,d);
	DRect.ysize=LocalToPixel(r.ysize,d);
	    
	drawTek2(b->l->myIcon,b,&DRect,d);
	    
	return 0;
}
#define ANMODE 31
#define GRAPHMODE 29
static int drawTek2(IconPtr myIcon,uTekPtr t,uRect *DRect,struct DOList *d)
{
		int mode,imove,seen96,xvalue,yvalue,width;
		int hix,hiy,lox,loy,extx,exty,extxsave,extysave;
		int xoff,yoff;
		unsigned char c,v;
		double scale;
		int ix,iy,tsize;
	    uFontInfo Info;
		long n;
	    
		if(!t || !myIcon || !d)return 1;
		
		if(t->length <= 0)return 0;

		if(t->dataPtr == NULL)return 1;
		
				
		xoff=DRect->x;
		yoff=DRect->y;
		
		tsize=(int)(18*t->scale);
		
		uSetTextFace(myIcon,0);
		
		uSetTextFont(myIcon,DefaultFont);
		
		uSetFontSize(myIcon,tsize);
				
		uGetFontInfo(myIcon,&Info);
		
		uSetAttributes(d,&t->Attributes,lPattern | lForeColor | lBackColor | lWidth);	
		
	    width=Info.CWidth;
	    
		scale=t->scale*(1024.)/(4096.);

		xvalue=0;
		yvalue=0;		
		
		mode=ANMODE;
		seen96=FALSE;
		imove = TRUE;
		hix=hiy=lox=loy=extx=exty=extxsave=extysave=0;

		ix=0;
		iy=0;
		for(n=0;n<t->length;++n){
	    	c=t->dataPtr[n];
	    	if(c == 7){
				continue;
	    	}else if(c == 31){
				mode=ANMODE;
				continue;
	    	}else if(c == 29){
	        /* reset all move draw information */
				mode=GRAPHMODE;
				imove = TRUE;
				seen96=FALSE;
				continue;
	    	}else if(c == 27){
	        	if(n >= t->length)break;
	        	c=t->dataPtr[++n];
				continue;
	    	} else if(c < 32){
	        	continue;
	    	}

	    	if(mode == ANMODE){
	        	if(c <= 127){
	            	char buff[2];
	            	buff[0]=c;
	            	buff[1]=0;
		    		/* uDrawString(buff,myIcon); */		    		
		    		rDrawString(buff,d); 		    		
		    		ix += width;
		    		/*uMoveTo(ix+xoff,iy+yoff); */
		    		rMoveTo(PixelToLocal(ix+xoff,d),PixelToLocal(iy+yoff,d),d);
	        	}
				continue;
	    	}
	    	v=c & 31;
	    	if(c <= 63){
	        	if(seen96){
	           		hix=v;
	        	}else{
	           		hiy=v;
	        	}
	    	}else if(c <= 95){
	        	lox=v;
	        	ix=(hix << 7)+(lox<<2)+extx;
	        	ix=(int)((ix)*scale-xvalue+.5);
	        	iy=((hiy << 7)+(loy<<2)+exty);
	        	iy=(int)((3120-iy)*scale-yvalue+.5);
				if(imove){
		    		/* uMoveTo(ix+xoff,iy+yoff); */
		    		rMoveTo(PixelToLocal(ix+xoff,d),PixelToLocal(iy+yoff,d),d);
				}else{
		    		/* uLineTo(ix+xoff,iy+yoff,myIcon); */
		    		rLineTo(PixelToLocal(ix+xoff,d),PixelToLocal(iy+yoff,d),d);
				}
				imove=FALSE;
				seen96=FALSE;
	    	}else if(c <= 127){
	        	if(seen96){
	           		loy=v;
	           		extx=extxsave;
	           		exty=extysave;
	        	}else{
	           		extxsave=v & 3;
	           		extysave=(v >> 2) & 3;
	           		loy=v;
	        	}
				seen96=TRUE;
	    	}
		}


		return 0;
}

static DObjPtr uTekMove(DObjPtr o,double *dx,double *dy)
{
	uTekPtr pBox;
	uTekPtr b=(uTekPtr)o;

	if(!b || !dx || !dy)return NULL;
	
	if((pBox=(uTekPtr)uTekDuplicate(o,0,dx,dy)) != NULL){
	    pBox->xDup = b->xDup;
	    pBox->yDup = b->yDup;
	}
	return (DObjPtr)pBox;
}
static uTekPtr uTekdoBoxes(uTekPtr b,rRect *r,struct uAttributes *vAttributes)
{
	uTekPtr Image;

	if(!b || !r || !vAttributes)return NULL;
	
	Image = uTekImage(b->xsize,b->ysize,r,vAttributes,b->l);
	
	if(Image == NULL){
	    WarningBatch("uTekdoBoxes Out of Memory\n");
	    return NULL;
	}
	if(b->length > Image->length){
	    if(Image->dataPtr)cFree((char *)Image->dataPtr);
	    Image->dataPtr=(unsigned char *)cMalloc(b->length,6234);
	    if(!Image->dataPtr){
	        uTekDelete((DObjPtr)Image);
	        return NULL;
	    }
	}
	uMemcpy(Image->dataPtr,b->dataPtr,b->length);
	Image->box.x = b->box.x;
	Image->box.y = b->box.y;
	Image->box.xsize = b->box.xsize;
	Image->box.ysize = b->box.ysize;
	Image->AngleFlag = b->AngleFlag;
	Image->FlipFlag = b->FlipFlag;
	Image->Files = b->Files;
	Image->length = b->length;
	return Image;
}
static DObjPtr uTekDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff)
{
	uTekPtr pImage;
	double x,y;
	uTekPtr b=(uTekPtr)o;

	if(!b || !xOff || !yOff)return NULL;
	
	if((pImage=uTekdoBoxes(b,&b->box,&b->Attributes)) == NULL){
	    return NULL;
	}
	if(iDup){
	    x = b->box.x-b->xDup;
	    y = b->box.y-b->yDup;
	}else{
	    x = *xOff;
	    y = *yOff;
	}
	pImage->xDup = pImage->box.x;
	pImage->yDup = pImage->box.y;
	pImage->box.x += x;
	pImage->box.y += y;

	return (DObjPtr)pImage;
}
static int uTekMessage(DObjPtr o,long MessageType,void *MessageData)
{
	uTekPtr b=(uTekPtr)o;
	struct Message1000 *m;
	long n;
	
	if(!b || !MessageData)return 1;
	
	m=MessageData;
	
	switch(MessageType){
	case MessageType_GET_FILE_OWNER:
		if(!b->Files)return 0;
		for(n=0;n<m->FileCount;++n){
		    if(b->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
		}
		break;
	case MessageType_GET_FILE_LIST:
		{
		    int isFound;
		    
			if(!b->Files)return 0;
			
		    isFound=FALSE;
			for(n=0;n<m->FileCount;++n){
			    if(b->Files == m->FilesList[n]){isFound=TRUE;break;}
			}
			if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
			    m->FilesList[m->FileCount++]=b->Files;
			}
		}
		break;
	case MessageType_SET_FRAME_NUMBER:
		if(!b->Files)return 0;		
	    if(!b->Attributes.Animation)return 0;
	    
		uTekSetFrame(o,m->CurrentFrame);
		
		break;
	case MessageType_GET_FRAME_COUNT:
		if(b->Files){
		    if(b->Files->ImageCount > m->FrameCount)m->FrameCount=b->Files->ImageCount;
		}
		break;
	case MessageType_PRINT_FILE_INFORMATION:
	    sprintf(WarningBuff,"  Tek Object x %g y %g\n%c",b->box.x,b->box.y,0);
	    WarningBatch(WarningBuff);
	    printFileInformation(b->Files);
		break;
	}	
	return 0;
}
static void uTekDelete(DObjPtr o)
{
	uTekPtr b=(uTekPtr)o;
	
	if(!b)return;
	
	if(b->dataPtr){
	    cFree((char *)b->dataPtr);
	    b->dataPtr=NULL;
	}
	
	ObjectKill((DObjPtr)b);
	
}
static char *uTekCopy(DObjPtr o,long *Length)
{
	uTekPtr b=(uTekPtr)o;
	long size,istart,len;
	uTekPtr pBox;
	struct Obj *obj;
	char *p;

	if(!b)return NULL;
	
	istart=sizeof(struct Obj)+sizeof(*pBox);
	len=b->length;
	*Length=size=istart+len;
	if((obj=(struct Obj *)cMalloc(size,8035)) == NULL){
	    return NULL;
	}
	pBox=(uTekPtr)(obj+1);
	obj->type=b->type;
	obj->Length=size;
	*pBox = *b;
	p=(char *)obj;
	p += istart;
	uMemcpy(p,b->dataPtr,(long)len);
	
	return (char *)obj;
}
int uTekSet(uTekPtr b)
{
	if(!b)return 1;
	
	b->Kill=uTekDelete;
	b->type=TekPlotType;
	b->Message=uTekMessage;	
	b->Move=uTekMove;	
	b->Duplicate=uTekDuplicate;	
	b->Draw=uTekDraw;
	b->DoInformation=uTekDoInformation;
    b->GetFrameCount=uTekFrameCount;    
    b->SetFrame=uTekSetFrame;    
	b->Copy=uTekCopy;	
	b->ReadWrite=uTekWrite;
	b->DoubleClick=uTekDoubleClick;
	
	return 0;
}
uTekPtr uTekImage(long xs,long ys,rRect *r,struct uAttributes *Attribute,struct DOList *l)
{
	char name[64];
	uTekPtr b;
	
	if(!r || !Attribute || !l)return NULL;
	
	sprintf(name,"tek%03ld%c",l->ObjectIDCount++,0);
	
    b=(uTekPtr)ObjectCreate(name,TekPlotType,r,sizeof(struct uTek));
    if(!b)return NULL;
	
	if(uTekSet(b))goto ErrorOut;
		
	b->Attributes = *Attribute;
	b->xsize=xs;
	b->ysize=ys;
	b->length=b->xsize*b->ysize*3;
	if((b->dataPtr=(unsigned char *)cMalloc(b->length,8037)) == NULL){
	    sprintf(WarningBuff,"uTekImage Out Of Memory Requested (%ld)\n",b->length);
	    WarningBatch(WarningBuff);
	   goto ErrorOut;
	}
	b->box = *r;
	b->Attributes.Animation=TRUE;
	b->l=l;
	return b;
ErrorOut:
	uTekDelete((DObjPtr)b);
	return NULL;
}

uTekPtr uTekImage2(uTekPtr Pastin,DOListPtr l)
{
	long length;
	char *p;

	uTekPtr b;
	
	if(!Pastin || !l)return NULL;
		
	b=(uTekPtr)ObjectObject2((DObjPtr)Pastin);
	
	b->l=l;	
	
	if(uTekSet(b))goto ErrorOut;
	
	b->xsize=Pastin->xsize;
	b->ysize=Pastin->ysize;
	b->Files=Pastin->Files;
	b->CurrentFrame=Pastin->CurrentFrame;
	length=Pastin->length;
	if((b->dataPtr=(unsigned char *)cMalloc(length,8039)) == NULL){
	    sprintf(WarningBuff,"uTekImage2 Out Of Memory Requested (%ld)\n",length);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	b->length=length;
	p=(char *)Pastin;
	p += sizeof(*b);
	uMemcpy(b->dataPtr,p,(long)length);
	return b;
ErrorOut:
	uTekDelete((DObjPtr)b);
	return NULL;
}

