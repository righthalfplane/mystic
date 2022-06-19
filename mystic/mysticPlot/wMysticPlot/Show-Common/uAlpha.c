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


int uCStringWidth(char *name,IconPtr myIcon);

static char *uAlphaCopy(DObjPtr o,long *Length);

int uAlphaSet(uAlphaPtr b);

static int uAlplaMessage(DObjPtr o,long MessageType,void *MessageData);

static int doScale1Action(struct uDialogStruct *uList);

static void uAlphaDelete(DObjPtr o);

static int uAlphaUpdate(DObjPtr o);

static int uAlphaDrawMyText(uAlphaPtr b,char *text,long length,rRect *r,int j,struct DOList *l);

static int uAlphaDraw(DObjPtr o,struct DOList *l);

static DObjPtr uAlphaDoInformation(DObjPtr o);

static DObjPtr uAlphaDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff);

static uAlphaPtr uAlphadoBoxes(uAlphaPtr b,rRect *r,struct uAttributes *Attributes);

static DObjPtr uAlphaDoRotateDirection(DObjPtr o,int direct);

static char *fileText;
static long fileLength;
static struct FileInfo2 *Files2;

static struct DObject *uAlphaDoubleClick(struct DObject *o,rPoint *r);
static DObjPtr uAlphaDoInformation(DObjPtr o);


static struct DObject *uAlphaDoubleClick(struct DObject *o,rPoint *r)
{
	if(!o || !r)return NULL;

	if(r->x < o->box.x || r->y < o->box.y){
		return NULL;
	}
	
	if(r->x > o->box.x+o->box.xsize || r->y > o->box.y+o->box.ysize){
		return NULL;
	}			
	return uAlphaDoInformation(o);
}
static int uAlphaFrameCount(struct DObject *Object,long *FrameCount)
{
	uAlphaPtr b=(uAlphaPtr)Object;
	
	if(!b && !FrameCount)return 1;
	
	if(b->Files){
	    if(b->Files->ImageCount > *FrameCount)*FrameCount=b->Files->ImageCount;
	}
	
	return 0;
	
}
static struct DObject *uAlphaWrite(struct DObject *o,FILE8 *inOut,struct DOList *l)
{


	static struct uAlpha b;
	
	static struct universalTableListStruct Local[] = {
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
			
		{"REAL",4031,universal_TypeInt,(void *)&b.Attributes.font.txFace},
		{"REAL",4032,universal_TypeInt,(void *)&b.Attributes.font.txFont},
		{"REAL",4033,universal_TypeInt,(void *)&b.Attributes.font.txSize},

		{"STRI",4034,universal_TypeString,(void *)&b.save[0],sizeof(b.save)},

		{"REAL",4035,universal_TypeInt,(void *)&b.n},
		
		{"DOUB",4036,universal_TypeDouble,(void *)&b.Angle},	
		
		{"FILE",4037,universal_TypeFilesPtr,(void *)&b.Files},	
		
		{"DOUB",4038,universal_TypeDouble,(void *)&b.scale},	
		
		{"STRI",4039,universal_TypeString,(void *)&b.Attributes.AnimationFormat[0],sizeof(b.Attributes.AnimationFormat)},
		
		{"DOUB",4040,universal_TypeDouble,(void *)&b.Attributes.AnimationStart},	
		{"DOUB",4041,universal_TypeDouble,(void *)&b.Attributes.AnimationStep},	

		{"REAL",4042,universal_TypeInt,(void *)&b.Attributes.Animation},
		{"REAL",4043,universal_TypeInt,(void *)&b.Attributes.AnimationJust},


	};
	
	static struct universalTableStruct Global = {
		"uDraw parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	uAlphaPtr bp=(uAlphaPtr)o;
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
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp=uAlphaAlpha(&l->Attributes,l);
		if(!bp)goto  ErrorOut;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(struct uAlpha);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		if(uPref.Files){
			bp->Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
		}		
		return (struct DObject *)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (struct DObject *)&b;
ErrorOut:
	return NULL;
}
static int uAlphaUpdate(DObjPtr o)
{
	uAlphaPtr b=(uAlphaPtr)o;
	char out[2048];
	uFontInfo Info;
	double xsize;
	DOListPtr l;
	int n,k;

	if(!b)return 1;
	l=b->l;
	if(!l)return 1;

	uSetFontStuff(&b->Attributes.font,l->myIcon);
	uGetFontInfo(l->myIcon,&Info);
	
	b->box.xsize=PixelToLocal(uCStringWidth(b->save,l->myIcon),l);
	
	b->box.ysize=PixelToLocal(Info.LineHeight,l);
		
	b->box.xsize=0;
	
	k=0;
	for(n=0;n<b->n;++n){
	   if(b->save[n] == '\r'){
	       out[k]=0;
	       xsize=PixelToLocal(uCStringWidth(out,l->myIcon),l);
	       if(xsize > b->box.xsize)b->box.xsize=xsize;
		   b->box.ysize += PixelToLocal(Info.LineHeight,l);
	       k=0;
	   }else{
	       out[k++]=b->save[n];
	   }
	}
	
	if(k > 0){
	    out[k]=0;
	    xsize=PixelToLocal(uCStringWidth(out,l->myIcon),l);
	    if(xsize > b->box.xsize)b->box.xsize=xsize;
	}
		
	if(b->Selected){
	    ObjectBoxStretch(&b->box,b->l);
	}

	return 0;
}
static uAlphaPtr uAlphadoBoxes(uAlphaPtr b,rRect *r,struct uAttributes *Attributes)
{
	uAlphaPtr pBox;
	int k;

	if(!b || !r || !Attributes)return NULL;
	
	if((pBox=uAlphaAlpha(Attributes,b->l)) == NULL){
	    WarningBatch("uAlphadoBoxes Out of Memory\n");
	    return NULL;
	}
	pBox->box.x = b->box.x;
	pBox->box.y = b->box.y;
	pBox->box.xsize = b->box.xsize;
	pBox->box.ysize = b->box.ysize;
	pBox->box = b->box;

	for(k=0;k<b->n;++k){
	    pBox->save[k]=b->save[k];
	}
	pBox->n=b->n;
	pBox->save[b->n]=0;
	pBox->AngleFlag=b->AngleFlag;
	pBox->Angle=b->Angle;
	pBox->AlphaNumber=b->AlphaNumber;
	
	if(b->fileText && b->fileLength > 0){
	    pBox->fileText=(char *)cMalloc(b->fileLength,8252);
	    if(!pBox->fileText){
	        WarningBatch("uAlphadoBoxes Out of Memory\n");
	        goto ErrorOut;
	    }
	    uMemcpy((char *)(pBox->fileText),(char *)b->fileText,b->fileLength);
	    pBox->fileLength=b->fileLength;
	}
	
	pBox->Files=b->Files;
	pBox->scale=b->scale;
	
	return pBox;
ErrorOut:
	uAlphaDelete((DObjPtr)pBox);
	return NULL;
}
static DObjPtr uAlphaDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff)
{
	uAlphaPtr pBox;
	double x,y;
	uAlphaPtr b=(uAlphaPtr)o;

	if(!b || !xOff || !yOff)return NULL;
	
	if((pBox=uAlphadoBoxes(b,&b->box,&b->Attributes)) == NULL){
	    return NULL;
	}
	if(iDup){
	    x = b->box.x-b->xDup;
	    y = b->box.y-b->yDup;
	}else{
	    x = *xOff;
	    y = *yOff;
	}
	pBox->xDup = pBox->box.x;
	pBox->yDup = pBox->box.y;
	pBox->box.x += x;
	pBox->box.y += y;
	return (DObjPtr)pBox;
}
DObjPtr uAlphaMove(DObjPtr o,double *dx,double *dy)
{
	uAlphaPtr pBox;
	uAlphaPtr b=(uAlphaPtr)o;

	if(!b || !dx || !dy)return NULL;
	
	if((pBox=(uAlphaPtr)uAlphaDuplicate((DObjPtr)b,0,dx,dy)) != NULL){
	    pBox->xDup = b->xDup;
	    pBox->yDup = b->yDup;
	}
	return (DObjPtr)pBox;
}
static int uAlphaDraw(DObjPtr o,struct DOList *l)
{
	struct SetFrameData *s;
	char buff[256];
	rRect r;
	uAlphaPtr b=(uAlphaPtr)o;
	
	if(!b || !l)return 1;

	r=b->box;
	uSetAttributes(b->l,&b->Attributes,lPattern | lForeColor | lBackColor  | tFont);
	if(b->Attributes.Animation){
	    if(b->fileText && b->fileLength){
	        char *fileText;
	        int n,nl;
	        
	        n=0;
	        nl=0;
	        fileText=b->fileText;
	        while((n < b->fileLength) && (nl < l->CurrentFrame)){
	           if(*fileText++ == '\n')++nl;
	           ++n;
	           if(nl >= l->CurrentFrame)break;
	        }
	        nl=0;
	        while((n < b->fileLength) && (nl < 254) && (*fileText != '\n')){
	           buff[nl++] = *fileText++;
	           ++n;
	        }
	        buff[nl]=0;
	    }else if(b->Files){
	        struct LIMITS *limits;	 
	        int fileClass; 
	        
	        fileClass=b->Files->fileClass;
			if(b->Attributes.ShowFileName && b->Files->FilesNames){				
				
				mstrncpy(buff,b->Files->FilesNames[0],sizeof(buff));
				if(l->CurrentFrame >= 0 && l->CurrentFrame < b->Files->FileCount){
					if(b->Files->FilesNames && b->Files->FilesNames[l->CurrentFrame]){
						mstrncpy(buff,b->Files->FilesNames[l->CurrentFrame],sizeof(buff));
					}
				}
				
	        
	        } else if(fileClass == PIO1D_FILES || fileClass == PIO2D_FILES || fileClass == PIO3D_FILES ){
	        	if(FilesSetFrame(l->CurrentFrame,b->Files))return 0;	              
				s = &b->Files->SetData;
		        sprintf(buff,b->Attributes.AnimationFormat,s->pioTime*b->scale);	        
	        }else if(!(FileGetLimits(&limits,l->CurrentFrame,b->Files))){	        
		        sprintf(buff,b->Attributes.AnimationFormat,limits->time*b->scale);	        
	        }else{
		        sprintf(buff,b->Attributes.AnimationFormat,0.0);	        
	        }
	    }else{
	        sprintf(buff,b->Attributes.AnimationFormat,
	        b->Attributes.AnimationStart+
	        b->Attributes.AnimationStep*(double)l->CurrentFrame);
	    }
	    if(b->AngleFlag == 0){
	    	uAlphaDrawMyText(b,buff,(long)strlen(buff),&r,b->Attributes.AnimationJust,l);
	    }else {
	        rMoveTo(r.x,r.y,l);			
			rDrawStringAngle(buff,b->Angle,l);	
	    }
	}else{
	
	    if(b->AngleFlag == 0){
	        b->save[b->n]=0;
	        uAlphaDrawMyText(b,b->save,(long)b->n,&r,0,l);
	    }else {
	        rMoveTo(r.x,r.y,l);			
			rDrawStringAngle(b->save,b->Angle,l);	
	    }
	
	}
	return 0;
}
static  int uAlphaDrawMyText(uAlphaPtr b,char *text,long length,rRect *r,int j,struct DOList *l)
{
	double x,y,height,width;
	uFontInfo Info;
	char out[2048];
	int k,n,flag;
	
	if(!b || !text || !r || !l)return 1;
	    
	uGetFontInfo(l->myIcon,&Info);
	height=PixelToLocal(Info.LineHeight,l);
	if(l->myIcon){
		width=PixelToLocal(uCStringWidth(text,l->myIcon),l);
	}else{
		width=0;
	}
	x=r->x;
	y=r->y+height;
	if(j){	
	    x = r->x+r->xsize-width;	    
	    rMoveTo(x,y,l);
	    rDrawString(text,l);
	}else{
	    for(k=0,n=0;k<length;++k){	 
	        flag=((k+1) == length);       
	        if(text[k] == '\r' || flag){
	            if(flag){
	               out[n++]=text[k];
	            }
	            out[n]=0;
	    	    rMoveTo(x,y,l);
	            rDrawString(out,l);
	            y += height;
	            n=0;
	        }else{
	            out[n++]=text[k];
	        }
	    }
	}
	
	return 0;
}

uAlphaPtr uAlphaAlpha2(uAlphaPtr Pastin,struct DOList *l)
{

	uAlphaPtr b;
	long k;	
	
	if(!Pastin || !l)return NULL;
		
    b=(uAlphaPtr)ObjectObject2((DObjPtr)Pastin);
    if(!b)return NULL;	
	
	if(uAlphaSet(b))goto ErrorOut;
		
	for(k=0;k<Pastin->n;++k){
	    b->save[k]=Pastin->save[k];
	}
	
	b->AlphaNumber=Pastin->AlphaNumber;
	b->n=Pastin->n;
	b->save[b->n]=0;
	if(Pastin->fileLength > 0){
	    b->fileText=(char *)cMalloc(Pastin->fileLength,8253);
	    if(!b->fileText){
	        WarningBatch("CAlphaCAlpha2 Out of Memory\n");
	        goto ErrorOut;
	    }
	    uMemcpy((char *)(b->fileText),(char *)(Pastin+1),Pastin->fileLength);
	    b->fileLength=Pastin->fileLength;
	}
	b->AngleFlag = Pastin->AngleFlag;
	b->l = l;
	b->Files=Pastin->Files;
	b->scale=Pastin->scale;

	return b;
ErrorOut:
	uAlphaDelete((DObjPtr)b);
	
	return NULL;
}
static void uAlphaDelete(DObjPtr o)
{
	uAlphaPtr b=(uAlphaPtr)o;
	
	if(!b)return;
	
	if(b->fileText)cFree((char *)b->fileText);
	b->fileText=NULL;
	
	ObjectKill((DObjPtr)b);
}

uAlphaPtr uAlphaAlpha(struct uAttributes *Attribute,struct DOList *l)
{
	static rRect box;
	char name[64];
	uAlphaPtr b;
	
	if(!Attribute || !l)return NULL;
	
	
	sprintf(name,"Alpha%03ld%c",l->ObjectIDCount++,0);
	
    b=(uAlphaPtr)ObjectCreate(name,AlphaType,&box,sizeof(struct uAlpha));
    if(!b)return NULL;	
	
	if(uAlphaSet(b))goto ErrorOut;
	
	b->Attributes = *Attribute;
	b->AlphaNumber=0;
	b->scale=1.0;
	b->l=l;
	return b;
ErrorOut:
	uAlphaDelete((DObjPtr)b);
	return NULL;
}
static int uAlplaMessage(DObjPtr o,long MessageType,void *MessageData)
{
	uAlphaPtr b=(uAlphaPtr)o;
	struct Message1000 *m;
	int n;
	
	if(!b || !MessageData)return 1;
	
	m=MessageData;
	
	switch(MessageType){
	case MessageType_GET_FILE_OWNER:
		if(!b->Files)return 0;
		for(n=0;n<m->FileCount;++n){
		    if(b->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
		}
		break;
	case MessageType_SET_FRAME_NUMBER:
		if(!b->Files)return 0;		
	    if(!b->Attributes.Animation)return 0;
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
	case MessageType_GET_FRAME_COUNT:
		if(b->Files){
		    if(b->Files->ImageCount > m->FrameCount)m->FrameCount=b->Files->ImageCount;
		}
		break;
	case MessageType_PRINT_FILE_INFORMATION:
	    if(b->Files){
	    	sprintf(WarningBuff,"  Text Object x %g y %g\n%c",b->box.x,b->box.y,0);
	    	WarningBatch(WarningBuff);
	    	printFileInformation(b->Files);
	    }
		break;
	}	
	
	return 0;
}

static DObjPtr uAlphaDoRotateDirection(DObjPtr o,int direct)
{
	uAlphaPtr b=(uAlphaPtr)o;
	static double dx,dy;
	uAlphaPtr out;

	if(!b)return NULL;

	if(b->AngleFlag == direct)return NULL;

	out = (uAlphaPtr)uAlphaMove((DObjPtr)b,&dx,&dy);
	if(!out)return NULL;
	out->Angle = direct*90;
	out->AngleFlag = direct;
	return (DObjPtr)out;
}

static char *uAlphaCopy(DObjPtr o,long *Length)
{
	uAlphaPtr pBox;
	struct Obj *obj;
	long size;
	uAlphaPtr b=(uAlphaPtr)o;

	if(!b || !Length)return NULL;
	
	if(b->fileText == NULL)b->fileLength=0;
	
	*Length=size=(long)(sizeof(struct Obj)+sizeof(*pBox)+b->fileLength);
	if((obj=(struct Obj *)cMalloc(size,8003)) == NULL){
	    return NULL;
	}
	pBox=(uAlphaPtr)(obj+1);
	obj->type=b->type;
	obj->Length=size;
	*pBox = *b;
	if(b->fileLength){
	    uMemcpy((char *)(pBox+1),(char *)pBox->fileText,b->fileLength);
	}
	return (char *)obj;
}
int uAlphaSet(uAlphaPtr b)
{
	if(!b)return 1;
	
	b->type=AlphaType;
	b->Message=uAlplaMessage;
	b->Kill=uAlphaDelete;
	b->Draw=uAlphaDraw;	
	b->Duplicate=uAlphaDuplicate;
	b->Move=uAlphaMove;	
	b->DoInformation=uAlphaDoInformation;
	b->Update=uAlphaUpdate;
	b->DoRotateDirection=uAlphaDoRotateDirection;	
	b->Copy=uAlphaCopy;
	b->ReadWrite=uAlphaWrite;
    b->GetFrameCount=uAlphaFrameCount;
	b->DoubleClick=uAlphaDoubleClick;
	
	return 0;
}
static DObjPtr uAlphaDoInformation(DObjPtr o)
{
	static uDialogGroup Items[] =
	
        {
      
      /*  0 */      {"Ok", {65,267,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {170,267,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"0", {160,50,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"10", {160,75,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"1", {160,100,80,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"1", {160,125,80,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"Alpha", {167,10,120,20},uDialogText,uDialogNone},
      /*  7 */      {"1", {160,150,80,20},uDialogText,uDialogGetDouble},
      /*  8 */      {"Left        :", {70,75,65,20},uDialogString,uDialogNone},
      /*  9 */      {"Object Type", {45,10,95,20},uDialogString,uDialogNone},
      /* 10 */      {"Height    :", {70,100,65,20},uDialogString,uDialogNone},
      /* 11 */      {"Width     :", {70,125,65,20},uDialogString,uDialogNone},
      /* 12 */      {"Top         :", {70,50,65,20},uDialogString,uDialogNone},
      /* 13 */      {"File", {65,237,65,20},uDialogButton,uDialogNone},
      /* 14 */      {"Time", {166,237,65,20},uDialogButton,uDialogNone},
      /* 15 */      {"Scale     :", {70,150,65,20},uDialogString,uDialogNone},
      /* 16 */      {"\"%4.2f\"", {160,175,80,20},uDialogText,uDialogNone},
	  /* 17 */      {"Format", {70,175,80,20},uDialogString,uDialogNone},
	  /* 18 */      {"File Name", {70,202,95,20},uDialogCheck,uDialogGetLong},
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,314,295};
	char Format[10];
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	uAlphaPtr pBox;
	uAlphaPtr b=(uAlphaPtr)o;

	if(!b)return NULL;
	if(!b->l)return NULL;
		            

	if(b->Attributes.AnimationFormat[0]){
	    mstrncpy(Format,b->Attributes.AnimationFormat,10);
	}else{
	    mstrncpy(Format,"%.0f\0",10);
	}
	
	fileText=NULL;
	fileLength=0;
	Files2=b->Files;
	
	
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	pBox=NULL;
	
	uList=uDialogOpen("Alpha Information",&Rect1,ItemCount);
	
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
	              uDialogSetDouble,(double)b->scale,
	              
	              uDialogSetItem,16L,
	              uDialogSetText,Format,
	              
	              uDialogSetItem,18L,
	              uDialogSetLogical,(int)b->Attributes.ShowFileName,
	              
	              uDialogSetAction,doScale1Action,
	              	              	              	              
	              uDialogSetMyIcon,b->l->myIcon,
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	
	if((pBox=uAlphadoBoxes(b,&b->box,&b->Attributes)) == NULL){
	    goto ErrorOut;
	}
	
	pBox->box.x=uR[3].dreturn;
	pBox->box.y=uR[2].dreturn;
	pBox->box.xsize=uR[5].dreturn;
	pBox->box.ysize=uR[4].dreturn;
	pBox->scale=uR[7].dreturn;
	mstrncpy(pBox->Attributes.AnimationFormat,uR[16].sreturn,255);
	pBox->fileText=fileText;
	pBox->fileLength=fileLength;
	pBox->Files=Files2;
	if(pBox->Files || (pBox->fileText && pBox->fileLength)){
		pBox->Attributes.Animation=TRUE;
	}
	
	pBox->Attributes.ShowFileName=(int)uR[18].lreturn;

	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	if(Files2){
	    Files2->doNotAutoRemove=FALSE;
	}	
	
	if(ret && pBox){
	    uAlphaDelete((DObjPtr)pBox);
	    pBox=NULL;
	}

	return (DObjPtr)pBox;
}
static int doScale1Action(struct uDialogStruct *uList)
{
	int TypeList[]={HDF_DISK,HDF_MEMORY,SDS2D_DISK,SDS2D_MEMORY,
	                SAGE1D_DISK,SAGE2D_DISK,SAGE3D_DISK,0};
	struct FileList *Files;
    char file[2560];
 	IconPtr myIcon;
	
	if(!uList)return 1;
	myIcon=FindWindowInList(uList->myIconItem);
	if(!myIcon)return 1;
	
	uSetCursor(NULL,uWATCH_CURSOR);
	
	if(uList->actionItem == 13){
		if(!GetTheFileName(file,"File to Read",2560,&myIcon->infoLoad)){
			int getString(unsigned char *s,long n,FILE *in);
		    long FileLength(FILE *in);
		    FILE *inout;
		    
		    if(fileText)cFree(fileText);
		    fileText=NULL;
			fileLength=0;
			
			if((inout=fopen(file,"r")) == NULL){
				sprintf(WarningBuff,"Cannot open file : %s to read%c\n",file,0);
				Warning(WarningBuff);
				goto OutOfHere;
			}
			
			fileLength=FileLength(inout);
			if(fileLength > 0){
			    fileText=(char *)cMalloc(fileLength,8251);
			    if(fileText){
			        if(getString((unsigned char *)fileText,fileLength,inout)){ 
		    			if(fileText)cFree(fileText);
			        }
			    }
			
			}
		    if(inout)fclose(inout);
 	    }
	}else if(uList->actionItem == 14){
		Files=uOpenFileDialog(myIcon,TypeList);		
		if(Files){
		    Files2=FilesOpenList(Files);
			Files2->doNotAutoRemove=TRUE;
		}	
	}
OutOfHere:	
	if(uList)uDialogUpdate(uList);
	uSetCursor(NULL,uARROW_CURSOR);
	return 0;
}


