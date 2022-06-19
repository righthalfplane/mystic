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

static int DOListPutTek(long length,unsigned char *image,DOListPtr l);

int WriteRect8ToRect24(unsigned char *in, uRect *SRect,long xsize, long ysize,
                       unsigned char *out,uRect *DRect,long oxsize,long oysize,
                       unsigned char *pal);
                       
int ResizeArray(void *Array,long Length,long Count,long *Maximun,int tag);
int uCStringWidth(char *name,IconPtr myIcon);
/*
static int LocalToPixel(double x,DOListPtr d);
*/

DObjPtr DOGetObjectPtr(int type,DOListPtr l);

void DOListSetChangeUndo(DOListPtr l);
int DOListUndoChange(void *v);
int  DOListRedoChange(void *v);
void DOListRedo(DOListPtr l);
void DOListUndo(DOListPtr l);
int DOListRedoNew(void *v);
int DOListUndoNew(void *v);
void DOListSetNewUndo(DOListPtr l);
int DOListRedoMove(void *v);
int  DOListUndoMove(void *v);
void DOListSetMoveUndo(DOListPtr l);
int DOListReDoDelete(void *v);
int DOListUnDoDelete(void *v);
void DOListSetDeleteUndo(DOListPtr l);
void DOListKillSelected(DOListPtr l);

int DOListPackAll(DOListPtr l);

static struct DObject *ObjectStretch(uPoint pt,struct DObject *Object,DOListPtr l);
static void DOListPasteLinesIn(DOListPtr l,uGridPlotPtr Plot,uGridPlotPtr b);
static int ObjectCheckStretch(uPoint pt,struct DObject *Object,DOListPtr l);
static int ObjectFramCount(struct DObject *Object,long *FrameCount);
static int ObjectSetFrame(struct DObject *Object,long FrameCount);
static int ObjectDraw(struct DObject *Object,DOListPtr myIcon);
static struct DObject *ObjectFindObject(struct DObject *Object,rPoint *r);
static struct uAttributes *ObjectAttributes(struct DObject *Object);
static int ObjectSetSelectedRegion(struct DObject *Object,DOListPtr l);
static int ObjectDrawSelected(struct DObject *o,DOListPtr l);

static struct DObject *ObjectDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff){yOff=yOff;iDup=iDup;xOff=xOff;o=o;return NULL;}
static struct DObject *ObjectMove(DObjPtr o,double *dx,double *dy){dx=dx;dy=dy;o=o;return NULL;}
static struct DObject *ObjectKillSelected(DObjPtr o){o=o;return NULL;}
static char *ObjectCopy(DObjPtr o,long *Length) {o=o;Length=Length;return NULL;}

static int  ObjectGetMessage(DObjPtr o,long MType,void *MData){o=o;MType=MType;MData=MData;return 0;}

static DObjPtr ObjectDoInformation(DObjPtr o){o=o;return NULL;}

static void ObjectAttribute(DObjPtr o,struct uAttributes *Attributes,int Flag);

static void ObjectDeSelectOne(DObjPtr o,rPoint *ClickPoint);

static void ObjectSelectOne(DObjPtr o,rPoint *ClickPoint);

static int ObjectSelectedPoint(DObjPtr o,rPoint *ClickPoint);

static int FrameRects(DOListPtr l,int dx,int dy);

int drawStretch(uRect *box,int n,IconPtr myIcon);

void DOListdoMoveGroup(DOListPtr l,double *dx,double *dy);

static void DOListCopyFileList(DOListPtr l);

static int DOListPutImageFile(DOListPtr l,struct FileInfo2 *Files);

static int DOListPutTrueFile(DOListPtr l,struct FileInfo2 *Files);

/*
static int DOListPutFiles(DOListPtr l,struct FileInfo2 *Files);
*/

int rDrawImageRect(iRect *r,DOListPtr d);

static void ObjectGetFontSelected(DObjPtr o);

static void ObjectSetFontSelected(DObjPtr o);

void DOListSetFontSelected(DOListPtr l);

static int DOListPutTrue(long xsize,long ysize,unsigned char *image,DOListPtr l);

int DOLineAttributes(DOListPtr l,uLineDataPtr Line,int n)
{
	
	if(!l || !Line)return 1;
	
	if(l->lAttributes && (l->lineAttributesCount > n)){
	   if(n < 0)n=0;
	   Line->Attributes=l->lAttributes[n];
	}
	
	return 0;
}
int DOListReadWrite(DOListPtr l,FILE8 *inOut,int flag)
{
	static struct DOList ll;
	
	static struct universalTableListStruct Local[] = {
		{"REAL",4319,universal_TypeRealArrayPtr,(void *)&ll.itemList,0L},				
		{"REAL",4320,universal_TypeLong,(void *)&ll.ObjectIDCount},
	};
	
	static struct universalTableStruct Global = {
		"uDraw parameters",4200,
		(void *)&ll,sizeof(ll),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};
	
	char head[5];
	long length;
	long tag;
	int n,nn,ret;
	
	if(!l || !inOut)return 1;
	
	ret=1;
		
	if(flag == 1){
		if(l->oCount > 0){
		    if(l->itemList)cFree((char *)l->itemList);
		    l->itemList=cMalloc(l->oCount*sizeof(float),8327);
		    if(!l->itemList)return 1;
		    
		    nn=0;
		    for(n=0;n<l->oCount;++n){
		        if(!l->oCurrent[n] || !l->oCurrent[n]->ReadWrite)continue;
		    	l->itemList[nn++]=(float)l->oCurrent[n]->type;
		    }
	    }else{
		    nn=0;
	    }
	    ll = *l;
	    Local[0].space=nn;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	    if(nn <= 0)return 0;
	    for(n=0;n<l->oCount;++n){
	        if(!l->oCurrent[n] || !l->oCurrent[n]->ReadWrite)continue;
	        if((*l->oCurrent[n]->ReadWrite)(l->oCurrent[n],inOut,l) == NULL)goto ErrorOut;
	    }
	    if(l->itemList)cFree((char *)l->itemList);
	    l->itemList=NULL;
	    ll.itemList=NULL;
	}else{
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    Local[0].space=0;
	    zerol((char *)&ll,sizeof(ll));
		if(getUniversalTable(&Global,inOut))goto ErrorOut;		
		nn=(int)Local[0].space;
		l->itemList=ll.itemList;
		if(nn <= 0 || !l->itemList)goto ErrorOut;
	    for(n=0;n<nn;++n){
			DObjPtr Return,Object=DOGetObjectPtr((int)l->itemList[n],l);
			if(!Object || !Object->ReadWrite)continue;
	        if((Return = (*Object->ReadWrite)(NULL,inOut,l)) == NULL)goto ErrorOut;
		    if(DOListADD(l,Return)){
		        if(Return->Kill)(*Return->Kill)(Return);
		        goto ErrorOut;
		    }
	    }
	    if(l->itemList)cFree((char *)l->itemList);
	    l->itemList=NULL;
		if(ll.ObjectIDCount > l->ObjectIDCount){
		    l->ObjectIDCount=ll.ObjectIDCount+1;
		}
	}
	
	ret=0;
ErrorOut:
	return ret;
}
DObjPtr DOGetObjectPtr(int type,DOListPtr l)
{
	static struct uBox box;
	static struct uOval oval;
	static struct uGroup group;
	static struct uImage image;
	static struct uTrue itrue;	
	static struct uGridPlot plot;
	static struct uAlpha aplha;
	static struct uLine line;
	static struct uRoundRect roundrect;
	static struct uLineData plotline;
	static struct uTek tek;
	
			
	    switch(type)
	    {
	        case BoxType:
				zerol((char *)&box,sizeof(struct uBox));
				box.l=l;
	        	uBoxSet(&box);
	        	return (DObjPtr)&box;
	        case OvalType:
				zerol((char *)&oval,sizeof(struct uOval));
				oval.l=l;
	        	uOvalSet(&oval);
	        	return (DObjPtr)&oval;
	        case LineType:
				zerol((char *)&line,sizeof(struct uLine));
				line.l=l;
	        	uLineSet(&line);
	        	return (DObjPtr)&line;
	        case RoundRectType:
				zerol((char *)&roundrect,sizeof(struct uRoundRect));
				roundrect.l=l;
	        	uRoundRectSet(&roundrect);
	        	return (DObjPtr)&roundrect;
	        case AlphaType:
				zerol((char *)&aplha,sizeof(struct uAlpha));
				aplha.l=l;
	        	uAlphaSet(&aplha);
	        	return (DObjPtr)&aplha;
	        case ImagePlotType:
				zerol((char *)&image,sizeof(struct uImage));
				image.l=l;
	        	uImageSet(&image);
	        	return (DObjPtr)&image;
	        case TruePlotType:
				zerol((char *)&itrue,sizeof(struct uTrue));
				itrue.l=l;
	        	uTrueSet(&itrue);
	        	return (DObjPtr)&itrue;
	        case GroupsType:
				zerol((char *)&group,sizeof(struct uGroup));
				group.l=l;
	        	uGroupSet(&group);
	        	return (DObjPtr)&group;	            
	        case GridPlotType:
				zerol((char *)&plot,sizeof(struct uGridPlot));
				plot.l=l;
	        	GridPlotSet(&plot);
	        	return (DObjPtr)&plot;
	            
	        case GridPlotLine:
				zerol((char *)&plotline,sizeof(struct uLineData));
				plotline.l=l;
	        	GridPlotLineSet(&plotline);
	        	return (DObjPtr)&plotline;
	            
	        case TekPlotType:
				zerol((char *)&tek,sizeof(struct uTek));
				tek.l=l;
	        	uTekSet(&tek);
	        	return (DObjPtr)&tek;
	        	
	        default:
	            sprintf(WarningBuff,"DOGetObjectPtr : Unknown Type (%ld)\n",(long)type);
	            WarningBatch(WarningBuff);
	            break;
	    }
	    return NULL;
}
void DOListdoBoxes(DOListPtr l,int Type,uRect *r)
{
	DObjPtr Object=NULL;
	rRect box;
	
	if(!l || !r)return;
	
	DOListPackAll(l);
	
	 box=DOrRectFromScreen(r,l);
	 
	if(Type == BoxType){
		box=DOrRectFromScreen(r,l);
	    Object=(DObjPtr)uBoxBox(&box,&l->Attributes,l);	
	}else if(Type == OvalType){
		box=DOrRectFromScreen(r,l);
	    Object=(DObjPtr)uOvalOval(&box,&l->Attributes,l);
	}else if(Type == RoundRectType){
		box=DOrRectFromScreen(r,l);
	    Object=(DObjPtr)uRoundRectRoundRect(&box,&l->Attributes,l);
	}else if(Type == LineType){
		rPoint rr;
		uPoint p;
		p.x=r->xsize;
		p.y=r->ysize;
		rr=screenToLocal(&p,l);
		box.xsize=rr.x;
		box.ysize=rr.y;
	    Object=(DObjPtr)uLineLine(&box,&l->Attributes,l);
	 
	}else{
	    WarningBatch("DOListdoBoxes Unknown Type\n");
	    return;
	}

	if(Object == NULL){
	    return;
	}
		
	if(DOListADD(l,Object)){
	    if(Object->Kill)(*Object->Kill)(Object);
	    return;
	}
	
	DOListSetNewUndo(l);
   	DOListUpdate(l,1);
 	return;
}
void DOListPasteLines(DOListPtr l)
{
	double xOff=0,yOff=0;
	long Length,size;
	struct Obj *obj;
	char *data;

	DObjPtr Object=NULL;
	uGridPlotPtr Plot;
	char *p;
	long n;
	
	if(!l)return;

	DOListPackAll(l);
	Plot=NULL;
	for(n=0;n<l->oCount;++n){
	    DObjPtr Current,nCurrent;
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(!Current->Selected  || (Current->type != GridPlotType))continue;
	    if(!Current->Duplicate)continue;
	    if((nCurrent=(DObjPtr)(*Current->Duplicate)(Current,0,&xOff,&yOff)) == NULL)break;
	    l->oCurrent[n]=nCurrent;
	    l->oOld[n]=Current;	    
		Plot=(uGridPlotPtr)nCurrent;
		break;
	}
	
	    
	if(!Plot){
		WarningBatch("No Destination Found For Line Paste\n");
		return;
	}
	
	DOListSetChangeUndo(l);
	
	data=NULL;
	Length=uGetScrap("DRW3",&data);
	if(Length <= 0){
	    return;
	}
	
	if(!data)return;	
	
	p = data;
	size=0;
	while(size < Length){
	    obj=(struct Obj *)p;
	    if((Object=DOGetScrapObject(obj,l)) == NULL)goto Continue;
	    if(Object->type == GridPlotType){
	    	goto Continue;
	    }
	    if(Object->Kill)(*Object->Kill)(Object);
	    p += obj->Length;
	    size += obj->Length;
	}
	Object=NULL;
Continue:
	if(data)cFree((char *)data);
	if(Object == NULL){
		WarningBatch("No Line Plots Found In Clipboard\n");
		return;
	}
	DOListPasteLinesIn(l,Plot,(uGridPlotPtr)Object);
}
static void DOListPasteLinesIn(DOListPtr l,uGridPlotPtr Plot,uGridPlotPtr b)
{
	int n,k;
	
	if(!l || !Plot || !b)return;
	
	
	if(b->LineCount <= 0 || !b->Lines){
		WarningBatch("No Line Plot Data Found\n");
		if(b->Kill)(*b->Kill)((DObjPtr)b);
		return;
	}
	
	k=0;
	for(n=0;n<b->LineCount;++n,++k){
		if(ResizeArray(&Plot->Lines,sizeof(struct CLineData *),Plot->LineCount,&Plot->LineMax,7117))break;	
		Plot->Lines[Plot->LineCount++]=b->Lines[n];    
	}
   	cFree((char *)b->Lines);
   	b->Lines=NULL;   
   	b->LineCount=0;
	if(b->Kill)(*b->Kill)((DObjPtr)b);
	
   	DOListUpdate(l,1);
	return;
}
static int DOListPutTrue(long xsize,long ysize,unsigned char *image,DOListPtr l)
{
	uTruePtr iTrue;
	rRect r;
	int ret;
	
	if(!l || !image || (xsize <= 0) || (ysize <= 0))return 1;
	
	ret=1;
	
	r.x=0;
	r.y=0;
	r.ysize=PixelToLocal(ysize,l);
	r.xsize=PixelToLocal(xsize,l);

	iTrue = uTrueTrue(xsize,ysize,&r,&l->Attributes,l);

	if(iTrue == NULL || !iTrue->ReplaceImage){
	    goto OutOfHere;
	}
	
	if((*iTrue->ReplaceImage)(iTrue,xsize,ysize,(unsigned char *)image)){
	    goto OutOfHere;
	}
	
	if(DOListADD(l,(DObjPtr)iTrue)){
	    goto OutOfHere;
	}

	DOListSetNewUndo(l);
   	DOListUpdate(l,1);
		
	ret=0;
	
OutOfHere:	
	if(ret){
	
		if(iTrue && iTrue->Kill){
	       (*iTrue->Kill)((DObjPtr)iTrue);
	    }
	
	}
	
	if(image)cFree((char *)image);
	
	return ret;
}

void DOListPaste(DOListPtr l)
{
	DObjPtr Object=NULL,Return;
	struct Obj *obj;
	double dx,dy;
	char *p;
	char *data;
	long Length,size;
	long xsize,ysize;
	unsigned char *image;

	
	
	if(!l)return;

	DOListPackAll(l);
	DOListDeSelectAll(l);	
	
	data=NULL;
	Length=uGetScrap("TEK3",&data);
	if(Length > 0){
 		DOListPutTek(Length,(unsigned char *)data,l);
 	    return;
	}

	data=NULL;
	Length=uGetScrap("DRW3",&data);
	if(Length <= 0){
	    if(getTrueFromScrap(&xsize,&ysize,&image))return;
	    DOListPutTrue(xsize,ysize,image,l);
	    return;
	}

	
	if(!data)return;	

	
	dx=9./72.;
	dy=9./72.;
	
	p = data;
	size=0;
	while(size < Length){
	    obj=(struct Obj *)p;
	    if((Object=DOGetScrapObject(obj,l)) == NULL)goto ErrorOut;
	    if(!Object->Move)break;
	    if((Return=(*Object->Move)(Object,&dx,&dy)) == NULL){
			break;
	    }
	    if(Object->Kill)(*Object->Kill)(Object);
	    if(DOListADD(l,Return)){
	        if(Return->Kill)(*Return->Kill)(Return);
	        goto ErrorOut;
	    }
	    p += obj->Length;
	    size += obj->Length;
	}
ErrorOut:
	if(data)cFree((char *)data);
	DOListSetChangeUndo(l);
   	DOListUpdate(l,1);
}

DObjPtr DOGetScrapObject(struct Obj *obj,DOListPtr l)
{
	DObjPtr Object=NULL;
	
		if(!obj)return NULL;
		
	    switch(obj->type)
	    {
	        case BoxType:
	    	    Object=(DObjPtr)uBoxBox2((uBoxPtr)(obj+1),l);
	            break;
	        case OvalType:
	    	    Object=(DObjPtr)uOvalOval2((uOvalPtr)(obj+1),l);
	            break;
	        case LineType:
	    	    Object=(DObjPtr)uLineLine2((uLinePtr)(obj+1),l);
	            break;
	        case RoundRectType:
	    	    Object=(DObjPtr)uRoundRect2((uRoundRectPtr)(obj+1),l);
	            break;
	        case AlphaType:
	    	    Object=(DObjPtr)uAlphaAlpha2((uAlphaPtr)(obj+1),l);
	            break;
	        case GroupsType:
	    	    Object=(DObjPtr)uGroupGroup2((uGroup2Ptr)(obj+1),l);
	            break;
	        case ImagePlotType:
	    	    Object=(DObjPtr)uImageImage2((uImagePtr)(obj+1),l);
	            break;
	        case TekPlotType:
	    	    Object=(DObjPtr)uTekImage2((uTekPtr)(obj+1),l);
	            break;
	        case TruePlotType:
	    	    Object=(DObjPtr)uTrueTrue2((uTruePtr)(obj+1),l);
	            break;
	        case GridPlotType:
	    	    Object=(DObjPtr)uGridPlot2((uGridPlotPtr)(obj+1),l);
	            break;
	            /*
	        case PictObjectType:
	    	    Object=(DObjPtr)CPictCPict2((CPictPtr)(obj+1),l);
	            break;
	            */
	        default:
	            sprintf(WarningBuff,"GetScrapObject : Unknown Type (%ld)\n",obj->type);
	            WarningBatch(WarningBuff);
	            return NULL;
	    }

	    return Object;
}

void DOListCut(DOListPtr l)
{
	if(!l)return;
	DOListCopy(l);DOListKillSelected(l);
}
void DOListCopy(DOListPtr l)
{
	long n,Length,Total;
	char *obj,*list;
	if(!l)return;
	
	list=NULL;
	Total=0;
	for(n=0;n<l->oCount;++n){
	    DObjPtr Current;
	    if((Current = l->oCurrent[n]) != NULL){
	    	if(Current->Selected && Current->Copy){
	    	    if((obj=(*Current->Copy)(Current,&Length)) != NULL){
	    	        if(SaveCopy(obj,Length,&list,&Total))return;
	    	    }
	    	}
	    }
	}
	
	uZeroScrap();
	if(Total){
	    Length=uPutScrap(Total,"DRW3",list);
	    if(Length < 0){
	        WarningBatch("DOListCopy Error Writing Scrap\n");
	    }
	    if(list)cFree((char *)list);
	}
	
	uDrawSaveScrap(l->myIcon);
	
	DOListCopyFileList(l);

}
int DOSetPalette(DOListPtr l)
{
	DObjPtr Current,nCurrent;
	long n;
	
	if(!l)return 1;

	DOListPackAll(l);
	for(n=0;n < l->oCount;++n){
	    if((Current = l->oCurrent[n]) != NULL){
	        if(Current->Selected && Current->SetPalette){
	            if((nCurrent=(*Current->SetPalette)(Current,(unsigned char *)l->palette)) == NULL)break;
	            l->oCurrent[n]=nCurrent;
	            l->oOld[n]=Current;
	        }
	    }
	}

	DOListSetChangeUndo(l);
	
 	InvalRectMyWindow(l->myIcon);
	
	return 0;
}
static void DOListCopyFileList(DOListPtr l)
{
	struct Message1000 m;
    struct FileInfo2 *FilesList[1000];
    long Length;
	
	if(!l)return;
	
	m.FilesList=FilesList;
	m.FileCount=0;
	m.FileCountMax=1000;
	
	DOListGetMessage(l,MessageType_GET_FILE_LIST,&m);
	if(m.FileCount > 0){
		Length=uPutScrap((long)(m.FileCount*sizeof(struct FileInfo2 *)),"FLST",(char *)FilesList);
		if(Length < 0){
		    WarningBatch("DOListCopyFileList - Error Writing Scrap\n");
		}
	}
}
void DOListSetFontSelected(DOListPtr l)
{
	DObjPtr Current;
	long n;
	
	if(!l)return;
	
	for(n=0;n < l->oCount;++n){
	    if((Current = l->oCurrent[n]) != NULL){
	        if(Current->Selected && Current->SetFontSelected){
	             (*Current->SetFontSelected)(Current);
	             return;
	        }
	    }
	}
	uSetFontStuff(&l->Attributes.font,l->myIcon);
}
static void ObjectGetFontSelected(DObjPtr o)
{	
	if(!o || !o->l)return;
	uGetFontStuff(&o->Attributes.font,o->l->myIcon);	
}
static void ObjectSetFontSelected(DObjPtr o)
{	
	if(!o || !o->l)return;
	uSetFontStuff(&o->Attributes.font,o->l->myIcon);	
}
void DOListChangeAlpha(DOListPtr l)
{
	double xOff=0,yOff=0;
	int New;
	long n;
	
	if(!l)return;
	
	DOListPackAll(l);
	New=FALSE;
	for(n=0;n<l->oCount;++n){
	    DObjPtr Current,nCurrent;
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(!Current->Selected || (Current->type != AlphaType &&
	       Current->type != GridPlotType && Current->type != TruePlotType))continue;
	    if(!Current->Duplicate)continue;
	    if((nCurrent=(DObjPtr)(*Current->Duplicate)(Current,0,&xOff,&yOff)) == NULL)break;
	    if(!nCurrent->GetFontSelected)continue;
	    (*nCurrent->GetFontSelected)(nCurrent);
	    New=TRUE;
	    l->oCurrent[n]=nCurrent;
	    l->oOld[n]=Current;	    
	}
	if(New){
	    DOListSetChangeUndo(l);
 		InvalRectMyWindow(l->myIcon);
	}else{
	    uGetFontStuff(&l->Attributes.font,l->myIcon);
	}
    DOListUpdate(l,1);
}
int rRectIntersect(rRect *r1,rRect *r2)
{
	if(!r1 || !r2)return 0;
	if(r1->x > r2->x+r2->xsize)return 0;
	if(r1->y > r2->y+r2->ysize)return 0;
	if(r1->x >= r2->x){
	   if(r1->y+r1->ysize >= r2->y){
	       return 1;
	   }else{
	       return 0;
	   }
	}else{
	   if(r1->x+r1->xsize >= r2->x){
		   if(r1->y+r1->ysize >= r2->y){
		       return 1;
		   }else{
		       return 0;
		   }
	   }else{
	       return 0;
	   }
	}
	
}
int DOListSelectBox(DOListPtr l,uRect *r)
{
	rRect box;
	long n;
	
	if(!l || !r)return 1;
	
	box=DOrRectFromScreen(r,l);

	for(n=0;n<l->oCount;++n){
	    DObjPtr Current;
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(!rRectIntersect(&box,&Current->box))continue;
		if(Current->SelectOne)(*Current->SelectOne)(Current,NULL);
	}
	
	
	return 0;
}
void DOListDoRotateDirection(DOListPtr l,int direct)
{
	int iFlip;
	long n;
	
	if(!l)return;
	if(!l->oCount)return;
	
	DOListPackAll(l);		
	
	iFlip=FALSE;
	for(n=0;n<l->oCount;++n){
	    DObjPtr Current,Return;
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(Current->Selected && Current->DoRotateDirection){
			if((Return=(*Current->DoRotateDirection)(Current,direct)) == NULL){
			    return;
			}else{
			    iFlip=TRUE;
			    l->oCurrent[n]=Return;
			    l->oOld[n]=Current;
			}
	    }
	}
	if(iFlip){
	    DOListSetChangeUndo(l);
   	    DOListUpdate(l,1);
   	}
}
void DOListDoFlipDirection(DOListPtr l,int direct)
{
	int iFlip;
	long n;
	
	if(!l)return;
	if(!l->oCount)return;
	
	DOListPackAll(l);		
	
	iFlip=FALSE;
	for(n=0;n<l->oCount;++n){
	    DObjPtr Current,Return;
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(Current->Selected && Current->DoFlipDirection){
			if((Return=(*Current->DoFlipDirection)(Current,direct)) == NULL){
			    return;
			}else{
			    iFlip=TRUE;
			    l->oCurrent[n]=Return;
			    l->oOld[n]=Current;
			}
	    }
	}
	if(iFlip){
	    DOListSetChangeUndo(l);
   	    DOListUpdate(l,1);
   	}
}
void DOListSetAnimation(DOListPtr l)
{
	static uDialogGroup Items[] =
	
        {      
      
      /*  0 */      {"Ok", {200,170,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {305,170,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"On", {35,49,74,20},uDialogButtonRadio,uDialogGetLong},
      /*  3 */      {"Off", {35,74,74,20},uDialogButtonRadio,uDialogGetLong},
      /*  4 */      {"0", {240,20,75,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"10", {240,60,75,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"\"%4.2f\"", {240,130,80,20},uDialogText,uDialogNone},
      /*  7 */      {"Right", {35,130,74,20},uDialogButtonRadio,uDialogGetLong},
      /*  8 */      {"Left", {35,160,74,20},uDialogButtonRadio,uDialogGetLong},
      /* 11 */      {"Start Value", {155,20,80,20},uDialogString,uDialogNone},
      /* 12 */      {"Animation", {24,20,74,20},uDialogString,uDialogNone},
      /* 13 */      {"Step Size", {155,60,80,20},uDialogString,uDialogNone},
      /* 16 */      {"Just", {26,110,74,20},uDialogString,uDialogNone},
      /* 17 */      {"Format", {155,130,80,20},uDialogString,uDialogNone},
           
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,377,220};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	DObjPtr Current,nCurrent;	
	long int n;
	
	struct uAttributes Aout;
	char Format[10];
	double xOff,yOff;
	
	if(!l)return;
	
	if(!l->oCount)return;
	for(n=0;n<l->oCount;++n){
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(Current->Selected &&
	      (Current->type == AlphaType          || 
               Current->type == ImagePlotType  || 
               Current->type == TruePlotType ||
               Current->type == GridPlotType )){
	        	goto Found;
	    }

	}
	return;
Found:
	if(Current->Attributes.AnimationFormat[0]){
	    mstrncpy(Format,Current->Attributes.AnimationFormat,10);
	}else{
	    mstrncpy(Format,"%.0f\0",10);
	}
	 	
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("Animation Parameters",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetLogical,(int)Current->Attributes.Animation,
	              
	              uDialogSetItem,3L,
	              uDialogSetLogical,(int)!Current->Attributes.Animation,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)Current->Attributes.AnimationStart,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)Current->Attributes.AnimationStep,
	              	              	              	              
	              uDialogSetItem,6L,
	              uDialogSetText,Format,
	              	              	              
	              uDialogSetItem,7L,
	          	  uDialogSetRadioGroup,1,
	              uDialogSetLogical,(int)Current->Attributes.AnimationJust,
	              	              	              
	              uDialogSetItem,8L,
	          	  uDialogSetRadioGroup,1,
	              uDialogSetLogical,(int)!Current->Attributes.AnimationJust,
	              	              
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

  	
	Aout.Animation=(int)uR[2].lreturn;
	Aout.AnimationStart=uR[4].dreturn;
	Aout.AnimationStep=uR[5].dreturn;
	mstrncpy(Aout.AnimationFormat,uR[6].sreturn,255);
	Aout.AnimationJust=(int)uR[7].lreturn;
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
    
    if(ret)return;
    
	DOListPackAll(l);
	
	for(n=0;n<l->oCount;++n){
	    if((Current == l->oCurrent[n]))goto Found2;
	}
	WarningBatch("COListSetAnimation Object Not found\n");
	return;
Found2:
	xOff=0;
	yOff=0;
	
	if(Current->Duplicate){
		if((nCurrent=(*Current->Duplicate)(Current,0,&xOff,&yOff)) == NULL)return;
		l->oCurrent[n]=nCurrent;
		if(nCurrent->Attribute)(*nCurrent->Attribute)(nCurrent,&Aout,lAnimate);
		l->oOld[n]=Current;
	}
	
	
	DOListSetChangeUndo(l);
 	InvalRectMyWindow(l->myIcon);
}
void DOListSaveAlpha(DOListPtr l,DObjPtr Ap)
{
	extern int print(char *message);
	uAlphaPtr Alpha=(uAlphaPtr)Ap;
	uAlphaPtr AlphaC;
	uEditPtr Edit;
	char *text;
	DObjPtr Current;
	int length,n,c,Old;
	long nn;
	
	if(!l || !Alpha)return;
	
	Edit=l->Edit;
	
	if(!Edit)return;

	Current=NULL;
	
	DOListPackAll(l);
	Old=FALSE;
	for(nn=0;nn < l->oCount;++nn){
	    if((Current = l->oCurrent[nn]) != NULL){
	        if(Current->type == AlphaType){
	              AlphaC=(uAlphaPtr)Current;
	              if(AlphaC->AlphaNumber == Alpha->AlphaNumber){
	                 Old=TRUE;
	                 break;
	             }
	        }
	    }
	}
	
	if(!Old || !Current){
	   Current=(DObjPtr)Alpha;
	}
	Alpha->box.x=Current->box.x;
	Alpha->box.y=Current->box.y;
	
	text=Edit->text;
	length=Edit->length;
	Alpha->n=length;
	for(n=0;n<length;++n){
	    c=text[n];
	    if(c == '\n')c='\r';
	    Alpha->save[n]=c;
	}
	Alpha->save[length]=0;
	
	Alpha->box=DOrRectFromScreen(&Edit->box,l);
	
	uEditDelete(Edit);
	
	l->Edit=NULL;
	l->Alpha=NULL;
	

 	InvalRectMyWindow(l->myIcon);
 	
	if(Old){
	    l->oCurrent[nn]=(DObjPtr)Alpha;
	    l->oOld[nn]=Current;
	}else{
	    if(DOListADD(l,(DObjPtr)Alpha)){
	        if(Alpha->Kill)(*Alpha->Kill)((DObjPtr)Alpha);
	        return;
	    }
	}
	DOListSetNewUndo(l);
    DOListUpdate(l,1);

}
void DOListUnGroup(DOListPtr l)
{
	uGroup2Ptr Return;
	long n,k;
	
	if(!l)return;
	if(!l->oCount)return;
	
	DOListPackAll(l);
	for(n=l->oCount-1;n >= 0 ;--n){
	    DObjPtr Current;
	    if((Current = l->oCurrent[n]) != NULL){
	        if(Current->Selected && (Current->type == GroupsType)){
	            Return=(uGroup2Ptr)Current;
	            for(k=0;k<Return->nGroups;++k){
	                Return->Groups[k]->Selected=TRUE;
		        	if(DOListADD(l,Return->Groups[k])){
		            	break;
		        	}else{
		            	Return->Groups[k]=NULL;
		        	}       
	            }
	            if(Return->Kill)(*Return->Kill)((DObjPtr)Return);
	            l->oCurrent[n]=NULL;
	            break;
	        }
	    }
	}
	DOListPackAll(l);
	DOListUpdate(l,1);
}
void DOListGroup(DOListPtr l)
{
	uGroup2Ptr Return;
	long n,nGroup,nSum;
	
	if(!l)return;
	
	DOListPackAll(l);
	nGroup=0;
	for(n=0;n < l->oCount;++n){
	    DObjPtr Current;
	    if((Current = l->oCurrent[n]) != NULL){
	        if(!Current->Selected)continue;
	        ++nGroup;
	    }
	}
	if(nGroup <= 1)return;
	if((Return=uGroupGroup(nGroup,l)) != NULL){
	    nSum=0;
	    nGroup=0;
	    for(n=0;n < l->oCount;++n){
	        DObjPtr Current;
	        if((Current = l->oCurrent[n]) != NULL){
	            l->oCurrent[n]=NULL;
	            if(Current->Selected){
	                Current->Selected=FALSE;
	                Return->Groups[nGroup++]=Current;
	            }else{
	                l->oCurrent[nSum++]=Current;
	            }
	        }
	    }
	    Return->nGroups=nGroup;
	    Return->Selected=TRUE;
	    l->oCurrent[nSum]=(DObjPtr)Return;
	    l->oOld[nSum++]=NULL;
	    l->oCount=nSum;
	}
	DOListUpdate(l,1);
}
uGridPlotPtr DOListDoNewPlot(DOListPtr l)
{
	uGridPlotPtr Plot;
	
	if(!l)return NULL;

		
	DOListDeSelectAll(l);
	DOListPackAll(l);
	
	Plot = DOGridPlotCreate(l);
		
	if(Plot == NULL){
	    return NULL;
	}
		
	DOListSetNewUndo(l);
   	DOListUpdate(l,1);
 	
	return Plot;
}
int DOListColorBar(DOListPtr l)
{
	static uDialogGroup Items[] =
	
        {
            
      /*  0 */      {"Ok", {45,210,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {140,210,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"Vertical Bar", {50,26,100,20},uDialogButtonRadio,uDialogGetLong},
      /*  3 */      {"Horizontal Bar", {50,87,117,20},uDialogButtonRadio,uDialogGetLong},
      /*  4 */      {"Top To Bottom", {78,46,130,20},uDialogButtonRadio,uDialogGetLong},
      /*  5 */      {"Bottom To Top", {78,65,130,20},uDialogButtonRadio,uDialogGetLong},
      /*  6 */      {"Left To Right", {78,107,129,20},uDialogButtonRadio,uDialogGetLong},
      /*  7 */      {"Right To Left", {78,127,129,20},uDialogButtonRadio,uDialogGetLong},
      /*  8 */      {"New Palette", {78,166,128,16},uDialogButtonRadio,uDialogGetLong},
      /*  9 */      {"Current Palette", {78,184,127,16},uDialogButtonRadio,uDialogGetLong},
      /* 10 */      {"Color Bar", {53,6,90,20},uDialogString,uDialogNone},
      /* 11 */      {"Palette Selection", {49,147,149,21},uDialogString,uDialogNone},
      
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,250,255};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
	static int b2lists[8]={1,0,1,0,1,0,0,1};
	int n;
	unsigned char *dataPtr;
	uImagePtr Image;
	rRect r;
	
	if(!l)return 1;
	   	
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	dataPtr=NULL;
	
	uList=uDialogOpen("Color Bar",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetLogical,(b2lists[0] == 1),
	              
	              uDialogSetItem,3L,
	              uDialogSetLogical,(b2lists[0] != 1),
	              
	              uDialogSetItem,4L,
	          	  uDialogSetRadioGroup,1,
	              uDialogSetLogical,(b2lists[2] == 1),
	              
	              uDialogSetItem,5L,
	          	  uDialogSetRadioGroup,1,
	              uDialogSetLogical,(b2lists[2] != 1),
	              	              
	              uDialogSetItem,6L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(b2lists[4] == 1),
	              	              
	              
	              uDialogSetItem,7L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(b2lists[4] != 1),
	              	              
	              
	              uDialogSetItem,8L,
	          	  uDialogSetRadioGroup,3,
	              uDialogSetLogical,(b2lists[6] == 1),
	              	              
	              uDialogSetItem,9L,
	          	  uDialogSetRadioGroup,3,
	              uDialogSetLogical,(b2lists[6] != 1),
	              	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

   	   	
	for(n=0;n<8;++n)b2lists[n]=(int)uR[n+2].lreturn;
	
	DOListDeSelectAll(l);
	
	if(DOListBuildColorBar(l,&dataPtr,&r,b2lists))goto ErrorOut;
	
	Image = uImageImage((int)(r.xsize*l->dpi),(int)(r.ysize*l->dpi),&r,&l->Attributes,l);
	
	DOListPackAll(l);
	if(Image == NULL){
	    if(dataPtr)cFree((char *)dataPtr);
	    goto ErrorOut;
	}
	
	if(Image->dataPtr)cFree((char *)Image->dataPtr);
	Image->dataPtr=dataPtr;
	
	
	if(DOListADD(l,(DObjPtr)Image)){
	    if(Image->Kill)(*Image->Kill)((DObjPtr)Image);
	    goto ErrorOut;
	}
	
	DOListSetNewUndo(l);
   	DOListUpdate(l,1);
 	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	 	
	return ret;
}
int DOListBuildColorBar(DOListPtr l,unsigned char **dataPtri,rRect *r,int *b2lists)
{
	unsigned char *dataPtr,*dataPtrs;
	int ix,iy,iFlag,xsize,ysize;
	long length;
	
	if(!l || !dataPtri || !r || !b2lists)return 1;
	
	*dataPtri=NULL;
	
	length=256*40*3;
	if((dataPtr=(unsigned char *)cMalloc(length,8050)) == NULL){
	    WarningBatch("Color Bar Not Enough Memory\n");
	    return 1;
	}
	dataPtrs=dataPtr;
	if(b2lists[0]){  /* Vertical */
	    xsize=40;
	    ysize=256;
	    iFlag=b2lists[2];
	    for(iy=0;iy<ysize;++iy){
	        for(ix=0;ix<xsize;++ix){
	            if(iFlag){ /* Top To Bottom */
	                *dataPtrs++ = iy;
	            }else{
	                *dataPtrs++ = 255-iy;
	            }
	        }
	    }
	}else{		/* Horzontial */
	    xsize=256;
	    ysize=40;
	    iFlag=b2lists[4];
	    for(iy=0;iy<ysize;++iy){
	        for(ix=0;ix<xsize;++ix){
	            if(iFlag){  /* Left To Right */
	                *dataPtrs++ = ix;
	            }else{
	                *dataPtrs++ = 255-ix;
	            }
	        }
	    }
	} 
	
	r->x=1.0;
	r->y=1.0;
	r->ysize=(double)ysize/(double)l->dpi;
	r->xsize=(double)xsize/(double)l->dpi;
	
	*dataPtri=dataPtr;
	
	return 0;
}
void DOListSendUp(DOListPtr l)
{
	DObjPtr Current,Save,oSave;
	long kill,n;
	
	if(!l)return;
	if(!l->oCount)return;
	
	DOListPackAll(l);
	
	kill = -1;
	Current=NULL;
	for(n=0;n<l->oCount;++n){
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(!(Current->Selected))continue;
	    kill=n;
	    break;
	}
	
	if(kill < 0 || kill >= (l->oCount-1))return;
	if(!Current)return;
	
	Save=Current;
	oSave=l->oOld[kill];

	l->oOld[kill] = l->oOld[kill+1];
	l->oCurrent[kill] = l->oCurrent[kill+1];

	l->oOld[kill+1] = oSave;
	l->oCurrent[kill+1] = Save;

	InvalRectMyWindow(l->myIcon);
}
void DOListSendFront(DOListPtr l)
{
	DObjPtr Current,Save,oSave;
	long kill,n;
	
	if(!l)return;
	if(!l->oCount)return;
	
	DOListPackAll(l);
	
	kill = -1;
	Current=NULL;
	for(n=0;n<l->oCount;++n){
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(!(Current->Selected))continue;
	    kill=n;
	    break;
	}
	
	if(kill < 0)return;
	if(!Current)return;
	
	Save=Current;
	oSave=l->oOld[kill];
	for(n=kill;n<l->oCount-1;++n){
	    l->oOld[n] = l->oOld[n+1];
	    l->oCurrent[n] = l->oCurrent[n+1];
	}
	l->oOld[l->oCount-1] = oSave;
	l->oCurrent[l->oCount-1] = Save;
	InvalRectMyWindow(l->myIcon);
}
void DOListSendDown(DOListPtr l)
{
	DObjPtr Current,Save,oSave;
	long kill,n;
	
	if(!l)return;
	
	if(!l->oCount)return;
	
	DOListPackAll(l);
	
	kill = -1;
	Current=NULL;
    for(n=0;n<l->oCount;++n){
        if((Current = l->oCurrent[n]) == NULL)continue;
        if(!(Current->Selected))continue;
        kill=n;
        break;
    }
    if(kill <= 0)return;
	if(!Current)return;
	
    Save=Current;
    oSave=l->oOld[kill];
    
    l->oOld[kill] = l->oOld[kill-1];
    l->oCurrent[kill] = l->oCurrent[kill-1];
    
    l->oOld[kill-1] = oSave;
    l->oCurrent[kill-1] = Save;
    
	InvalRectMyWindow(l->myIcon);
}
void DOListSendBack(DOListPtr l)
{
	DObjPtr Current,Save,oSave;
	long kill,n;
	
	if(!l)return;
	if(!l->oCount)return;
	
	DOListPackAll(l);
	
	kill = -1;
	Current=NULL;
	
	for(n=0;n<l->oCount;++n){
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(!(Current->Selected))continue;
	    kill=n;
	    break;
	}
	if(kill < 0)return;
	if(!Current)return;
	
	Save=Current;
	oSave=l->oOld[kill];
	for(n=kill;n > 0;--n){
	    l->oOld[n] = l->oOld[n-1];
	    l->oCurrent[n] = l->oCurrent[n-1];
	}
	l->oOld[0] = oSave;
	l->oCurrent[0] = Save;
	InvalRectMyWindow(l->myIcon);
}
static int DOListPutTek(long length,unsigned char *image,DOListPtr l)
{
	uTekPtr Image;
	rRect r;
	int ret;
	
	if(!l || !image || (length <= 0))return 1;
	
	ret=1;

	r.x=0;
	r.y=0;
	r.ysize=PixelToLocal(390,l);
	r.xsize=PixelToLocal(512,l);
		
	Image = uTekImage(1024,780,&r,&l->Attributes,l);
	
	if(Image == NULL){
	    goto OutOfHere;
	}
	
	if(Image->dataPtr)cFree((char *)Image->dataPtr);
	Image->dataPtr=image;
	image=NULL;

	Image->length=length;
	
	if(DOListADD(l,(DObjPtr)Image)){
	    goto OutOfHere;
	}

	DOListSetNewUndo(l);
   	DOListUpdate(l,1);
		
	ret=0;
	
OutOfHere:	
	if(ret){	
		if(Image && Image->Kill){
	       (*Image->Kill)((DObjPtr)Image);
	    }	
	}
	
	if(image)cFree((char *)image);
	
	return ret;
}
static int DOListPutTekFile(DOListPtr l,struct FileInfo2 *Files)
{
	uTekPtr Image;
	rRect r;
	
	if(!l)return 1;
	if(!Files)return 1;
	
	
	r.x=0;
	r.y=0;
	r.ysize=PixelToLocal(390,l);
	r.xsize=PixelToLocal(512,l);
		
	Image = uTekImage(1024,780,&r,&l->Attributes,l);
	
	if(Image == NULL){
	    goto OutOfHere;
	}
	
	Image->Files=Files;
	
	if(Image->SetFrame){
	    (*Image->SetFrame)((DObjPtr)Image,0L);
	}
			
	if(DOListADD(l,(DObjPtr)Image)){
	    goto OutOfHere;
	}
	
	DOListSetNewUndo(l);
   	DOListUpdate(l,1);

	return 0;
OutOfHere:
	
	if(Image){
	    Image->Files=Files;
	    if(Image->Kill)(*Image->Kill)((DObjPtr)Image);
	}else{
	    if(Files)freeFileList(Files);
	}
	return 1;
}
static int DOListPutImageFile(DOListPtr l,struct FileInfo2 *Files)
{
    struct SetFrameData *s;
    unsigned char *d;
	uImagePtr Image;
	rRect r;
	long n;
	
	if(!l)return 1;
	if(!Files)return 1;
	
	
	r.x=0;
	r.y=0;
	r.ysize=PixelToLocal(Files->ymax,l);
	r.xsize=PixelToLocal(Files->xmax,l);
	
	
	Image = uImageImage(Files->xmax,Files->ymax,&r,&l->Attributes,l);
	

	if(Image == NULL){
	    goto OutOfHere;
	}
	
	Image->Files=Files;
	
	if(FilesSetFrame(0,Files)) goto OutOfHere;
	
	s = &Files->SetData;
	
	if(Image->dataPtr && s->data){
	    d=(unsigned char *)s->data;
		for(n=0;n<s->length;++n){
		    Image->dataPtr[n]=d[n];
		}
	}
	
	{
	    unsigned char r[256],g[256],b[256];
	    int k;
		for(k=0;k<256;++k){
		    r[k]=Files->palette[3*k];
		    g[k]=Files->palette[3*k+1];
		    b[k]=Files->palette[3*k+2];
		}
		for(k=0;k<256*3;++k){
		    l->palette[k]=Files->palette[k];
		}
		ScreenSetPalette(l->myIcon,r,g,b);
	}
	

	if(DOListADD(l,(DObjPtr)Image)){
	    goto OutOfHere;
	}
	
	DOListSetNewUndo(l);
   	DOListUpdate(l,1);

	return 0;
OutOfHere:
	
	if(Image){
	    Image->Files=Files;
	    if(Image->Kill)(*Image->Kill)((DObjPtr)Image);
	}else{
	    if(Files)freeFileList(Files);
	}
	return 1;
}
static int DOListPutTrueFile(DOListPtr l,struct FileInfo2 *Files)
{
    struct SetFrameData *s;
    struct SetFrameData sd;
	long xsize,ysize;
	uTruePtr iTrue;
	static int Count;
    char BatchName[256];
	int TrueType;
	int isOpen;
	rRect r;
	
	
	if(!l)return 1;
	if(!Files)return 1;	
	
	iTrue=NULL;
	
	isOpen=FALSE;
	
	BatchName[0]=0;
	
	zerol((char *)&sd,sizeof(sd));
	
	TrueType=0;
	
	if(Files->fileClass == BMP_FILES){
		TrueType=3;
	}else if(Files->fileClass == RGB_FILES){
		TrueType=1;
	}else if(Files->fileClass == JPG_FILES){
		TrueType=2;
	}else if(Files->fileClass == GIF_FILES){
		TrueType=4;
	}else if(Files->fileClass == PICT_FILES){
		TrueType=5;
	}else if(Files->fileClass == QUICKTIME_FILES){
		TrueType=6;
	}else if(Files->fileClass == PIO2D_FILES){
		TrueType=7;
	}else if(Files->fileClass == PIO1D_FILES){
		TrueType=8;
	}else if(Files->fileClass == PIO3D_FILES){
		TrueType=9;
	}else if(Files->fileClass == SDS2D_FILES){
		TrueType=10;
	}else if(Files->fileClass == SDS3D_FILES){
		TrueType=11;
	}else if(Files->fileClass == HDF_FILES){
		TrueType=12;
	}

	
	if(TrueType < 7){		
		if(FilesSetFrame(0,Files)) goto ErrorOut;	
		s = &Files->SetData;	
		xsize=s->xsize;
		ysize=s->ysize;
	}else{	
		sprintf(BatchName,"Pio2dDraw_%03d",Count++);	
    	mstrncat(BatchName,".b",255);	
    	if(FileManagerAddToList(Files,BatchName))goto ErrorOut;
    	
		Files->doNotAutoRemove=FALSE;
		
		isOpen=TRUE;
		
		if(SendMessageByName(BatchName,MessageType_GET_INFORMATION,&sd)){
			goto ErrorOut;
		}
		
		
 		if(Files->fileClass == PIO1D_FILES){
			sd.range.ixminData=0;
			sd.range.iyminData=0;
		    sd.range.ixmax=sd.range.ixmaxData;
		    sd.range.iymax=sd.range.iymaxData;
		} else if(Files->fileClass == PIO2D_FILES || Files->fileClass == PIO3D_FILES ){
		    double dxi,dyi;
		    double dx,dy;
		    	    
		    dx=Files->xmaxr-Files->xminr;
		    dy=Files->ymaxr-Files->yminr;
			sd.range.ixminData=0;
			sd.range.iyminData=0;
		    if(dx > dy){
		        dxi=(double)sd.range.ixmaxData;
		        if(dx > 0.0){
		       		dyi=dy*dxi/dx;
		        }else{
		       		dyi=dxi;
		        }
				sd.range.iymaxData=(long)dyi;
				dyi=(double)sd.range.iymaxData;
				sd.range.yminData=Files->yminr;
				sd.range.ymaxData=Files->yminr+dyi*dx/dxi;
		    }else{
		        dyi=(double)sd.range.iymaxData;
		        if(dx > 0.0){
		       		dxi=dx*dyi/dy;
		        }else{
		       		dxi=dyi;
		        }
				sd.range.ixmaxData=(long)dxi;
				dxi=(double)sd.range.ixmaxData;
				sd.range.xminData=Files->xminr;
				sd.range.xmaxData=Files->xminr+dxi*dx/dxi;
		    }
		    sd.range.ixmax=sd.range.ixmaxData;
		    sd.range.iymax=sd.range.iymaxData;
		}
		
		

		xsize=sd.range.ixmax;
		ysize=sd.range.iymax;

	}
	
	r.x=0;
	r.y=0;
	r.ysize=PixelToLocal(ysize,l);
	r.xsize=PixelToLocal(xsize,l);
	
	iTrue = uTrueTrue(xsize,ysize,&r,&l->Attributes,l);

	if(iTrue == NULL){
	    goto ErrorOut;
	}
	
	iTrue->Files=Files;
	
	iTrue->TrueType=TrueType;
		
	iTrue->pioIndex=sd.pioIndex;
	mstrncpy(iTrue->pioName,sd.pioName,64);	

	mstrncpy(iTrue->BatchName,BatchName,64);		
	BatchName[0]=0;
	
	
	iTrue->sd=sd;

	iTrue->plane  =  sd.plane;
	iTrue->vector  =  sd.vector;
	iTrue->range   =  sd.range;
	iTrue->pd 	   =  sd.pd;
	if(TrueType == 12){
	    memcpy((char *)iTrue->pd.palette,(char *)Files->palette,sizeof(Files->palette));
	}
    
	if(DOListADD(l,(DObjPtr)iTrue)){
	    goto ErrorOut;
	}
	
	DOListSetNewUndo(l);
   	DOListUpdate(l,1);
 	    
	return 0;
ErrorOut:
	if(iTrue && iTrue->Kill){
	    iTrue->Files=Files;
	    (*iTrue->Kill)((DObjPtr)iTrue);
	}else{
	    if(Files)freeFileList(Files);
	}
	if(BatchName[0] && isOpen){
		FileManagerCloseName(BatchName);
	}
	return 1;
}
void DOListdoLoadImages(DOListPtr l)
{
	struct FileInfo2 *Files2;	
	struct FileList *Files;	
	int TypeList[]={HDF_DISK,HDF_MEMORY,TRUE_DISK,TRUE_MEMORY,
	                RGB_DISK,RGB_MEMORY,BMP_DISK,BMP_MEMORY,
	                JPG_DISK,JPG_MEMORY,PICT_DISK,PICT_MEMORY,
	                QUICKTIME_DISK,QUICKTIME_MEMORY,
	                GIF_DISK,GIF_MEMORY,SAGE2D_DISK,
	                SAGE1D_DISK,SAGE3D_DISK,TEK_DISK,TEK_MEMORY,
	                SDS2D_DISK,SDS2D_MEMORY,
	                SDS3D_DISK,SDS3D_MEMORY,TEXT_MEMORY,0};
	
	if(!l)return;
	
	
	Files=uOpenFileDialog(l->myIcon,TypeList);		
	if(!Files)return;
	
	DOListDeSelectAll(l);
	DOListPackAll(l);	

	switch(Files->type){
	    case TEK_DISK:
	    case TEK_MEMORY:
			Files2=FilesOpenList(Files);
			if(!Files2)return;
			DOListPutTekFile(l,Files2);
			break;
	    case 9988:
			Files2=FilesOpenList(Files);
			if(!Files2)return;
			DOListPutImageFile(l,Files2);
			break;
	    case HDF_DISK:
	    case HDF_MEMORY:
	    case SDS3D_DISK:
	    case SDS3D_MEMORY:
	    case SDS2D_DISK:
	    case SDS2D_MEMORY:
	    case TRUE_DISK:
	    case SAGE1D_DISK:
	    case SAGE2D_DISK:
	    case SAGE3D_DISK:
	    case TRUE_MEMORY:
	    case RGB_DISK:
	    case RGB_MEMORY:
	    case BMP_DISK:
	    case BMP_MEMORY:
	    case JPG_DISK:
	    case JPG_MEMORY:
	    case GIF_DISK:
	    case GIF_MEMORY:
	    case PICT_DISK:
	    case PICT_MEMORY:
	    case QUICKTIME_DISK:
	    case QUICKTIME_MEMORY:
			Files2=FilesOpenList(Files);
			if(!Files2)return;
			Files2->inputType=TRUE_EXPANDED;
			DOListPutTrueFile(l,Files2);
			break;
	    case TEXT_MEMORY:
	        {
		    	uGridPlotPtr Plot;
		    	uLineDataPtr Line;
		    	char name[2048];
		    	long n;
		    	
		    	if(!Files)return;
		    	
		    	Plot=DOListDoNewPlot(l);
		    	if(!Plot)goto ExitHere;
		    		    	
				for(n=0;n<Files->count;++n){
	    			mstrncpy(name,Files->directory,2048);
					strncatToPath(name,Files->name[n],2048);								
		    		Line=doLineRead(&l->Attributes,name);
		    		if(!Line)break;
					if(ResizeArray(&Plot->Lines,sizeof(struct uLineData *),Plot->LineCount,&Plot->LineMax,2016))break;
					Plot->Lines[Plot->LineCount++]=Line;
				}
ExitHere:
				FileListFree(Files);
		    }
			break;
		default:
		    WarningBatch("DOListdoLoadImages Bad Image Type\n");
		    break;

	}	
	
}
void DOListDoInformation(DOListPtr l)
{
	long int n;
	IconPtr myIcon;
	
	if(!l)return;
	myIcon=l->myIcon;
	if(!myIcon)return;
	
	if(!l->oCount)return;
	DOListPackAll(l);
	for(n=0;n<l->oCount;++n){
	    DObjPtr Current,nCurrent;	
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(!Current->Selected || !Current->DoInformation)continue;
	    nCurrent=(*Current->DoInformation)(Current);
	    if(!nCurrent)return;
	    l->oCurrent[n]=nCurrent;
	    l->oOld[n]=Current;
	    break;
	}
	DOListSetChangeUndo(l);
 	InvalRectMyWindow(myIcon);
}
void DOListSelectAll(DOListPtr l)
{
	long n;

	if(!l)return;
	for(n=0;n<l->oCount;++n){
	    DObjPtr Current;
	    if((Current = l->oCurrent[n]) != NULL){
	    	if(Current->SelectOne)(*Current->SelectOne)(Current,NULL);
	    }
	}
   	DOListUpdate(l,1);
}
void DOListDuplicate(DOListPtr l)
{
	int iDup,iFlag;
	double xOff,yOff;
	char name[64];
	long n;
	IconPtr myIcon;
	
	if(!l)return;
	myIcon=l->myIcon;
	if(!myIcon)return;

	if((iDup=l->DuplicateFlag) == 0){
	    xOff=l->xDuplicate=9./72.;
	    yOff=l->yDuplicate=9./72.;
	}
	DOListPackAll(l);
	iFlag=FALSE;
	for(n=l->oCount-1;n >= 0;--n){
	    DObjPtr Current,nCurrent;
	    if((Current = l->oCurrent[n]) != NULL){
	        if(!Current->Selected || !Current->Duplicate)continue;
	        if((nCurrent=(*Current->Duplicate)(Current,iDup,&xOff,&yOff)) == NULL)break;
			iFlag=TRUE;
			Current->Selected=FALSE;
			sprintf(name,"COPY%03ld%c",l->ObjectIDCount++,0);
			mstrncpy(nCurrent->name,name,64);
	        if(DOListADD(l,nCurrent)){
	            if(nCurrent->Kill)(*nCurrent->Kill)(nCurrent);
	            break;
	        }
	    }
	}
	if(iFlag){
	    l->DuplicateFlag=TRUE;
	    DOListSetNewUndo(l);
        DOListUpdate(l,1);
	}
}
void DOListClear(DOListPtr l)
{
   DOListKillSelected(l);
}
void DOListdoKeyMove(DOListPtr l,int c)
{
	double dx,dy;
	rPoint r1,r2;
	int dxi,dyi;
	uPoint p;
	
	if(!l)return;

	p.x=2;
	p.y=2;
	r1=screenToLocal(&p,l);
	
	dxi=0;
	dyi=0;
	
	switch(c){
	case leftArrow:
	    dxi = -1;
	    break;
	case rightArrow:
	    dxi = 1;
	    break;
	case upArrow:
	    dyi = -1;
	    break;
	case downArrow:
	    dyi = 1;
	    break;
	}
	
	p.x += dxi;
	p.y += dyi;
	r2=screenToLocal(&p,l);
	
	dx = r2.x-r1.x;
	dy = r2.y-r1.y;

	DOListdoMoveGroup(l,&dx,&dy);
}
int drawStretch(uRect *box,int n,IconPtr myIcon)
{
	uPoint start,current,last;
	int isDrawn;
	uRect oldR;
	
	if(!myIcon || !box)return 1;
	
	uSetPenXOR(TRUE,myIcon);
	
	
	switch(n)
	{
		case 0:
			start.x=box->x+box->xsize;
			start.y=box->y+box->ysize;
		break;
		case 1:
			start.x=box->x+box->xsize;
			start.y=box->y;
		break;
		case 2:
			start.x=box->x;
			start.y=box->y;
		break;
		case 3:
			start.x=box->x;
			start.y=box->y+box->ysize;
		break;
	}
	
	uDrawAdjustReal(&start,myIcon);
	last=start;
	
	oldR = *box;

	isDrawn=FALSE;
	while(uStillDown(myIcon)){
		    uPointer(&current,myIcon);
			uDrawAdjustReal(&current,myIcon);
		    if((current.x == last.x) && (current.y == last.y))continue;
		    if(isDrawn){
			    uFrameRect(&oldR,myIcon);
		    }
		    isDrawn=TRUE;
		    last=current;
		    if(start.x < last.x){
		        oldR.x=start.x;
		        oldR.xsize=last.x-start.x;
		    }else{
		        oldR.x=last.x;
		        oldR.xsize=start.x-last.x;
		    }
		    if(start.y < last.y){
		        oldR.y=start.y;
		        oldR.ysize=last.y-start.y;
		    }else{
		        oldR.y=last.y;
		        oldR.ysize=start.y-last.y;
		    }
		    uFrameRect(&oldR,myIcon);
	}

	if(isDrawn){
		uFrameRect(&oldR,myIcon);
	}
	
	box->x=oldR.x;
	box->y=oldR.y;
	
	box->xsize=oldR.xsize;
	box->ysize=oldR.ysize;
	
	uSetPenXOR(FALSE,myIcon);
	
	return 0;
}
int DOListDoStretch(uPoint pt,DOListPtr l)
{
	DObjPtr Save,Return,Current;
	long n;
	
	if(!l)return 1;
	
	DOListPackAll(l);
	
	Save=NULL;
	Return=NULL;
	
	for(n=l->oCount-1;n >= 0;--n){
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(!Current->Selected || !Current->CheckStretch)continue;
	    if((*Current->CheckStretch)(pt,Current,l)){
	        Save=Current;
	        break;
	    }
	}
	
	if(!Save)return 0;
	
	if(Save->Stretch)Return=(*Save->Stretch)(pt,Save,l);
	
	if(Return != NULL){
	    l->oCurrent[n]=Return;
	    l->oOld[n]=Save;
		DOListSetNewUndo(l);
   		DOListUpdate(l,1);
 		
	}
	
	return 0;
}
int DOStretchRgn(uPoint pt,DOListPtr l)
{
	int n;
	
	if(!l)return 0;
	
	for(n=0;n<l->stretchCount;++n){
	   if(uPtInRect(pt,&l->Stretch[n])){
	        return 1;
	   }
	}
	
	return 0;
}
rRect DOrRectFromScreen(uRect *box,DOListPtr l)
{
	static rRect zero;
	rRect ret;
	rPoint r;
	uPoint p;
	
	if(!box || !l)return zero;
	
	p.x=box->x;
	p.y=box->y;
	r=screenToLocal(&p,l);
	ret.x=r.x;
	ret.y=r.y;
	
	p.x=box->x+box->xsize;
	p.y=box->y+box->ysize;;
	r=screenToLocal(&p,l);

	ret.xsize=r.x-ret.x;
	ret.ysize=r.y-ret.y;
	
	return ret;	
}
uRect DOSelectRect(int n,rRect *box,DOListPtr l)
{
	static uRect zero;
	int size,size2;
	uRect ret;
	rPoint r;
	uPoint p;
	
	if(!box || !l || (n < 0) || (n > 3))return zero;
	
    size=4;
    size2=size/2;

	ret.x=0;
	ret.y=0;
	ret.xsize=size;
	ret.ysize=size;
	
	switch(n)
	{
		case 0:
		    r.x=box->x;
		    r.y=box->y;
		    p=localToScreen(&r,l);
			ret.x=p.x-size2;
			ret.y=p.y-size2;
		break;
		case 1:
		    r.x=box->x;
		    r.y=box->y+box->ysize;
		    p=localToScreen(&r,l);
			ret.x=p.x-size2;
			ret.y=p.y;
		break;
		case 2:
		    r.x=box->x+box->xsize;
		    r.y=box->y+box->ysize;
		    p=localToScreen(&r,l);
			ret.x=p.x;
			ret.y=p.y;
		break;
		case 3:
		    r.x=box->x+box->xsize;
		    r.y=box->y;
		    p=localToScreen(&r,l);
			ret.x=p.x;
			ret.y=p.y-size2;
		break;
	}
	
	return ret;
	
}
int DOSaveStretch(uRect *r,DOListPtr l)
{
	if(!r || !l)return 1;
	
	if(ResizeArray(&l->Stretch,sizeof(uRect),l->stretchCount,&l->stretchMax,2018))return 0;
	
	l->Stretch[l->stretchCount++]= *r; 
	
	return 0;
}
int DOListGetMessage(DOListPtr l,long MessageType,void *MessageData)
{
	struct Message1000 *m;
	long n;
	
	if(!l)return 1;
	
	m=MessageData;
	
	for(n=0;n<l->oCount;++n){
	    DObjPtr Current;	
	    if((Current = l->oCurrent[n]) != NULL){
	        if(Current->Message){
	        	if((*Current->Message)(Current,MessageType,MessageData))return 1;
	        }
	    }
		if(MessageType == MessageType_GET_FILE_OWNER  || MessageType == MessageType_GET_FILE_LIST){
		    if((Current = l->oOld[n]) != NULL){
	        	if(Current->Message){
	        	   (*Current->Message)(Current,MessageType,MessageData);
	        	}
		    }
		
		}else if(MessageType == MessageType_SET_FRAME_NUMBER){
			if(m){
			   l->CurrentFrame=m->CurrentFrame;
			}
		}
	    
	}	
	
	return 0;
	
}

uRect DOuRectFromLocal(rRect *ri,DOListPtr l)
{
	static uRect zero;
	uRect ret;
	rPoint r;
	uPoint p;
	
	if(!ri || !l)return zero;

    r.x=ri->x;
    r.y=ri->y;
    p=localToScreen(&r,l);
    
    ret.x=p.x;
    ret.y=p.y;
    
	ret.xsize=LocalToPixel(ri->xsize,l);
	ret.ysize=LocalToPixel(ri->ysize,l);

	return ret;
}
static int FrameRects(DOListPtr l,int dx,int dy)
{
	rPoint r;
	uPoint p;
	uPoint s;
	uRect oldR;
	long n;
	
	if(!l || (l->rCountCount < 1) || !l->Boxes)return 1;
	
	for(n=0;n<l->rCountCount;++n){
	    r.x=l->Boxes[n].x;
	    r.y=l->Boxes[n].y;
	    p=localToScreen(&r,l);	    
	    r.x += l->Boxes[n].xsize;
	    r.y += l->Boxes[n].ysize;
	    s=localToScreen(&r,l);
		oldR.x=p.x+dx;
		oldR.y=p.y+dy;
				
		oldR.xsize=s.x-p.x;
		oldR.ysize=s.y-p.y;
				
		uFrameRect(&oldR,l->myIcon);
	}
	return 0;
}

void DOListdoMoveGroup(DOListPtr l,double *dx,double *dy)
{
	int iMove,iDup;
	long n;
	
	if(!l || !dx || !dy)return;
		
	if(!l->oCount)return;
	
	if(*dx == 0 && *dy == 0)return;
	
	iDup=l->DuplicateFlag;
	DOListPackAll(l);
	l->DuplicateFlag=iDup;	
	
	iMove=FALSE;
	for(n=0;n<l->oCount;++n){
	    DObjPtr Current,Return;
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(Current->Selected && Current->Move){
			if((Return=(*Current->Move)(Current,dx,dy)) == NULL){
			    return;
			}else{
			    iMove=TRUE;
			    l->oCurrent[n]=Return;
			    l->oOld[n]=Current;
			}
	    }
	}
	if(iMove){
		DOListSetMoveUndo(l);
   	}
}
void DOListSetMoveUndo(DOListPtr l)
{
	IconPtr myIcon;
	
	if(!l)return;
	myIcon=l->myIcon;
	if(!myIcon)return;
	
	myIcon->pUndo=&DOListUndoMove;
	myIcon->cUndo=(char *)"Undo Move";
	myIcon->eUndo=NULL;
	myIcon->FileSave=FALSE;
   	DOListUpdate(l,1);
}
int  DOListUndoMove(void *v)
{
	DOListPtr l=v;
	IconPtr myIcon;
	
	if(!l)return 1;
	myIcon=l->myIcon;
	if(!myIcon)return 1;
	
	myIcon->pUndo=&DOListRedoMove;
	myIcon->cUndo=(char *)"Redo Move";
	DOListUndo(l);
    DOListUpdate(l,1);
	return 0;
}
int DOListRedoMove(void *v)
{
	DOListPtr l=v;
	IconPtr myIcon;
	
	if(!l)return 1;
	myIcon=l->myIcon;
	if(!myIcon)return 1;
	
	myIcon->pUndo=&DOListUndoMove;
	myIcon->cUndo=(char *)"Undo Move";
	DOListRedo(l);
    DOListUpdate(l,1);
	return 0;
}
void DOListDragIt(DOListPtr l,DObjPtr Object)
{
	uPoint start,current,last;
	int isDrawn,dx,dy;
	double dxr,dyr;
	rPoint r;
	uPoint p,Final;
	long n;
	
	if(!l || !Object)return;
	
	l->rCountCount=0;

	for(n=0;n<l->oCount;++n){
	    DObjPtr Current;
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(Current->Selected && Current->SetSelectedRegion){
	        (*Current->SetSelectedRegion)(Current,l);
	    }
	}
	
	if(l->rCountCount < 1 || l->Boxes == NULL){
	    return;
	}
	
    r.x=l->Boxes[0].x;
    r.y=l->Boxes[0].y;
    p=localToScreen(&r,l);
	
	uSetPenXOR(TRUE,l->myIcon);
	
	uPointer(&start,l->myIcon);
	last=start;	
	isDrawn=FALSE;	
    dx=0;
    dy=0;
	while(uStillDown(l->myIcon)){
		    uPointer(&current,l->myIcon);
		    if((current.x == last.x) && (current.y == last.y))continue;
		    if(isDrawn){
			    FrameRects(l,dx,dy);
		    }
		    isDrawn=TRUE;
		    dx=current.x-start.x;
		    dy=current.y-start.y;
			if(dx || dy){
			    Final.x = p.x + dx;
			    Final.y = p.y + dy;
			    uDrawAdjustReal(&Final,l->myIcon);
			    dx = Final.x - p.x;
			    dy = Final.y - p.y;
			}
			
		    last=current;
		    FrameRects(l,dx,dy);
	}

	if(isDrawn){
		FrameRects(l,dx,dy);
	}
	
	
	uSetPenXOR(FALSE,l->myIcon);
	
	if(dx != 0 || dy != 0){
	
		rPoint r1,r2;
		
		start.x=0;
		start.y=0;

		last.x=dx;
		last.y=dy;

		r1=screenToLocal(&start,l);

		r2=screenToLocal(&last,l);
		
		dxr=r2.x-r1.x;
		dyr=r2.y-r1.y;
		
		DOListdoMoveGroup(l,&dxr,&dyr);

	} 
	
	
	return;
}
void DOListSelectClick(DObjPtr Object,rPoint *ClickPoint)
{
	if(!Object || !Object->SelectOne)return;
	(*Object->SelectOne)(Object,ClickPoint);
}

void DOListDeSelectAll(DOListPtr l)
{
	DObjPtr Current;
	long n;
	
	if(!l)return;
	
	for(n=0;n<l->oCount;++n){
	    if((Current = l->oCurrent[n]) != NULL){
		    if(Current->DeSelectOne){
		        (*Current->DeSelectOne)(Current,NULL);
		    }
	    }
	}
}
void DOListDeSelectClick(DObjPtr Object,rPoint *ClickPoint)
{
	if(!Object || !Object->DeSelectOne)return;
	(*Object->DeSelectOne)(Object,ClickPoint);
}
void DOListSelectOne(DObjPtr Object)
{
	if(!Object || !Object->SelectOne)return;
	(*Object->SelectOne)(Object,NULL);
}
void DOListDeSelectOne(DObjPtr Object)
{
	if(!Object || !Object->DeSelectOne)return;
	(*Object->DeSelectOne)(Object,NULL);
}
static void ObjectAttribute(DObjPtr o,struct uAttributes *Attributes,int Flag)
{
	if(!o || !Attributes)return;
	uPutAttributes(&o->Attributes,Attributes,Flag);
}

int DOListSaveAttributes(DOListPtr l,struct uAttributes *Attributes,int Flag)
{
	DObjPtr Current,nCurrent;
	double xOff=0,yOff=0;
	int iMove;
	long n;
	
	if(!l || !Attributes)return 1;
	
	iMove=FALSE;
	for(n=0;n < l->oCount;++n){
	    if((Current = l->oCurrent[n]) != NULL){
	        if(Current->Selected){
	            iMove=TRUE;
	            break;
	        }
	    }
	}
	if(iMove == FALSE){
	    uPutAttributes(&l->Attributes,Attributes,Flag);
	    return 0;
	}
	DOListPackAll(l);
	for(n=0;n < l->oCount;++n){
	    if((Current = l->oCurrent[n]) != NULL){
	        if(Current->Selected && Current->Duplicate){
	            if((nCurrent=(*Current->Duplicate)(Current,0,&xOff,&yOff)) == NULL)break;
	            l->oCurrent[n]=nCurrent;
	            if(nCurrent->Attribute)(*nCurrent->Attribute)(nCurrent,Attributes,Flag);
	            l->oOld[n]=Current;
	        }
	    }
	}
	
	DOListSetChangeUndo(l);
	
 	InvalRectMyWindow(l->myIcon);
 	
 	return 0;
}
struct uAttributes *DOListGetAttributes(DOListPtr l)
{
	DObjPtr Current;
	int iMove;
	long n;
	
	if(!l)return NULL;
	
	iMove=FALSE;
	Current=NULL;
	for(n=0;n < l->oCount;++n){
	    if((Current = l->oCurrent[n]) != NULL){
	        if(Current->Selected){
	            iMove=TRUE;
	            break;
	        }
	    }
	}
	if(iMove && Current && Current->GetAttributes){
	  return (*Current->GetAttributes)(Current);
	}
	
	return &l->Attributes;
}
int DOErase(DOListPtr l)
{
	long n;
	
	if(!l)return 1;
	if(!l->data)return 1;
	
	if(l->raster == RASTER_24){
	    for(n=0;n<l->xsize*l->ysize;++n){
	       l->data[3*n]=l->red;
	       l->data[3*n+1]=l->green;
	       l->data[3*n+2]=l->blue;
	    }
	}else{
		zerol((char *)l->data,l->length);
	}
	
	return 0;
}
int DOListUpdate(DOListPtr l,int flag)
{
	DObjPtr Current;
	long n;
	
	if(!l)return 1;
	
	l->stretchCount=0;
	if(l->oCount){
	    for(n=0;n<l->oCount;++n){
	        if((Current = l->oCurrent[n]) != NULL){
		    	if(Current->Update)(*Current->Update)(Current);
			}
	    }
	}
	DOFrameCount(l);
	
	if(flag && l->myIcon){
	    InvalRectMyWindow(l->myIcon);
	}
	return 0;
}
int DODoubleClick(rPoint *r,DOListPtr l)
{
	int iMove;
	long n;
	
	if(!l || !r)return 1;
	if(!l->data)return 1;
	
	DOListPackAll(l);

	iMove=FALSE;
	for(n=l->oCount-1;n>=0;--n){
	    DObjPtr Current,Return;
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(!Current->DoubleClick)continue;
		if((Return=(*Current->DoubleClick)(Current,r)) == NULL){
		    continue;
		}else{
		    iMove=TRUE;
		    l->oCurrent[n]=Return;
		    l->oOld[n]=Current;
		    break;
		}
	}
	if(iMove){
		DOListSetChangeUndo(l);
   	    DOListUpdate(l,1);   	
   	}
	
	return 0;
}


void DOListKillSelected(DOListPtr l)
{
	int kill;
	long int n;
	IconPtr myIcon;
	
	if(!l)return;
	myIcon=l->myIcon;
	if(!myIcon)return;
	if(!l->oCount)return;
	
	DOListPackAll(l);
	kill=FALSE;
	for(n=0;n<l->oCount;++n){
	    DObjPtr Current,nCurrent;	
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(!Current->Selected || !Current->KillSelected)continue;
	    nCurrent=(*Current->KillSelected)(Current);
	    l->oCurrent[n]=nCurrent;
	    l->oOld[n]=Current;
	    kill=TRUE;
	}
	if(kill){
	    DOListSetDeleteUndo(l);
	}
}
void DOListSetDeleteUndo(DOListPtr l)
{
	IconPtr myIcon;
	
	if(!l)return;
	myIcon=l->myIcon;
	if(!myIcon)return;
	
	myIcon->pUndo=&DOListUnDoDelete;
	myIcon->eUndo=NULL;
	myIcon->cUndo=(char *)"Undo Delete";
	myIcon->FileSave=FALSE;
    DOListUpdate(l,1);
}
int DOListUnDoDelete(void *v)
{
	DOListPtr l=v;
	IconPtr myIcon;
	
	if(!l)return 1;
	myIcon=l->myIcon;
	if(!myIcon)return 1;
	
	myIcon->pUndo=&DOListReDoDelete;
	myIcon->cUndo=(char *)"Redo Delete";
	DOListRedo(l);
    DOListUpdate(l,1);
	return 0;
}
int DOListReDoDelete(void *v)
{
	DOListPtr l=v;
	IconPtr myIcon;
	
	if(!l)return 1;
	myIcon=l->myIcon;
	if(!myIcon)return 1;
	
	myIcon->pUndo=&DOListUnDoDelete;
	myIcon->cUndo=(char *)"Undo Delete";
	DOListUndo(l);
    DOListUpdate(l,1);
	return 0;
}
void DOListSetChangeUndo(DOListPtr l)
{
	IconPtr myIcon;
	
	if(!l)return;
	myIcon=l->myIcon;
	if(!myIcon)return;
	
	myIcon->pUndo=&DOListUndoChange;
	myIcon->eUndo=NULL;
	myIcon->cUndo=(char *)"Undo Change";
	myIcon->FileSave=FALSE;
    DOListUpdate(l,1);
}
int DOListUndoChange(void *v)
{
	DOListPtr l=v;
	IconPtr myIcon;
	
	if(!l)return 1;
	myIcon=l->myIcon;
	if(!myIcon)return 1;
	
	myIcon->pUndo=&DOListRedoChange;
	myIcon->cUndo=(char *)"Redo Change";
	DOListUndo(l);
    DOListUpdate(l,1);
	return 0;
}
int  DOListRedoChange(void *v)
{
	DOListPtr l=v;
	IconPtr myIcon;
	
	if(!l)return 1;
	myIcon=l->myIcon;
	if(!myIcon)return 1;
	
	myIcon->pUndo=&DOListUndoChange;
	myIcon->cUndo=(char *)"Undo Change";
	DOListRedo(l);
    DOListUpdate(l,1);
	return 0;
}
void DOListSetNewUndo(DOListPtr l)
{
	IconPtr myIcon;
	
	if(!l)return;
	myIcon=l->myIcon;	
	myIcon->pUndo=&DOListUndoNew;
	myIcon->cUndo=(char *)"Undo New";
	myIcon->eUndo=NULL;
	myIcon->FileSave=FALSE;
}
int DOListUndoNew(void *v)
{
	IconPtr myIcon;
	DOListPtr l=v;
	
	if(!l)return 1;
	myIcon=l->myIcon;
	if(!myIcon)return 1;
	
	myIcon->pUndo=DOListRedoNew;
	myIcon->cUndo=(char *)"Redo New";
	DOListUndo(l);
   	DOListUpdate(l,1);
	return 0;
}
int DOListRedoNew(void *v)
{
	IconPtr myIcon;
	DOListPtr l=v;
	
	if(!l)return 1;
	myIcon=l->myIcon;
	if(!myIcon)return 1;
		
	myIcon->pUndo=DOListUndoNew;
	myIcon->cUndo=(char *)"Undo New";
	DOListRedo(l);
   	DOListUpdate(l,1);
	return 0;
}
void DOListUndo(DOListPtr l)
{
	DObjPtr Current;
	long n;
	
	if(!l)return;
	for(n=0;n<l->oCount;++n){
	    if((Current = l->oCurrent[n]) != NULL){
	        if(Current->oStatus == StatusNew){
	            if((l->oCurrent[n]=l->oOld[n]) != NULL){
	                l->oCurrent[n]->oStatus=StatusNew;
	                l->oCurrent[n]->Selected=TRUE;
	            }
	            l->oOld[n]=Current;
	        }else{
	            Current->Selected=FALSE;
	        }
	    }
	}
}
void DOListRedo(DOListPtr l)
{
	DObjPtr Current;
	long n;
	
	if(!l)return;
	for(n=0;n<l->oCount;++n){
	    if((Current = l->oOld[n]) != NULL){
	        l->oOld[n]=l->oCurrent[n];
	        Current->oStatus = StatusNew;
	        Current->Selected=TRUE;
	        l->oCurrent[n]=Current;
	    }else if((Current = l->oCurrent[n]) != NULL){
	        Current->Selected=FALSE;
	    }
	}
}

int DOListPackAll(DOListPtr l)
{
	long n,sum;
	
	if(!l)return 1;
	
	sum=0;
	for(n=0;n<l->oCount;++n){
	    DObjPtr Current;
	    if((Current = l->oOld[n]) != NULL){
	        if(Current->Kill)(*Current->Kill)(Current);
	        l->oOld[n]=NULL;
	    }
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    l->oCurrent[n]=NULL;
	    Current->oStatus=StatusActive;
	    l->oCurrent[sum++]=Current;
	}
	l->oCount=sum;
	
	l->DuplicateFlag=FALSE;
	/*
	myIcon->pUndo=NULL;
	myIcon->cUndo=NULL;
	myIcon->eUndo=NULL;
	*/
	
	return 0;
}
DObjPtr DOListFindObject(rPoint *r,DOListPtr l)
{
	long n;
	
	if(!l)return NULL;
	if(!r)return NULL;
	if(!l->oCount)return NULL;
		
	for(n=l->oCount-1;n >= 0;--n){
	    DObjPtr Current;
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if((Current->FindObject) && (*Current->FindObject)(Current,r)){
	         return Current;
	    }
	}
	return NULL;
}
int DODraw(DOListPtr l,long CurrentFrame)
{
	int ret;
	long n;
	
	if(!l)return 1;
	if(!l->data)return 1;
	
	ret=1;
	
	DOErase(l);
	

	l->CurrentFrame=CurrentFrame;
	
	for(n=0;n<l->oCount;++n){
		DObjPtr Current;
	    if((Current = l->oCurrent[n]) == NULL)continue;
        if(Current->SetFrame){
            if((*Current->SetFrame)(Current,CurrentFrame))continue;
        }
	}

	for(n=0;n<l->oCount;++n){
		DObjPtr Current;
	    if((Current = l->oCurrent[n]) == NULL)continue;
        if(Current->Draw){
            if((*Current->Draw)(Current,l))goto ErrorOut;
        }
	}
	
	
	ret = 0;
	
ErrorOut:

	return ret;
}
int DODrawSelectedObject(DOListPtr l,DObjPtr Current)
{
	int ret;
	
	if(!l)return 1;
	if(!Current)return 1;
	
	ret=1;
	
	uSetPenXOR(TRUE,l->myIcon);	    
    if(Current->DrawSelected){
        if((*Current->DrawSelected)(Current,l))goto ErrorOut;
    }
	uSetPenXOR(FALSE,l->myIcon);	 
	
	  
	ret = 0;
	
ErrorOut:

	return ret;
}	

int DODrawSelected(DOListPtr l)
{
	int ret;
	long n;
	
	if(!l)return 1;
	if(!l->data)return 1;
	
	ret=1;
	
	DOListUpdate(l,0);

	uSetPenXOR(TRUE,l->myIcon);	    
	for(n=0;n<l->oCount;++n){
		DObjPtr Current;
	    if((Current = l->oCurrent[n]) == NULL)continue;
        if(Current->DrawSelected){
            if((*Current->DrawSelected)(Current,l))goto ErrorOut;
        }
	}	
	uSetPenXOR(FALSE,l->myIcon);	 
	
	  
	ret = 0;
	
ErrorOut:

	return ret;
}	
long DOFrameCount(DOListPtr l)
{
	long frameCount;
	long n;
	
	if(!l)return -1;
	
	frameCount = 0;
	
	for(n=0;n<l->oCount;++n){
		DObjPtr Current;
	    if((Current = l->oCurrent[n]) == NULL)continue;
        if(Current->GetFrameCount){
            if((*Current->GetFrameCount)(Current,&frameCount))goto ErrorOut;
        }
	}
ErrorOut:	
	return frameCount;
}
int DOListDelete(DOListPtr l)
{
	DObjPtr Current;
	long n;
	
	if(!l)return 1;
	
	if(l->oCurrent){
	    for(n=0;n<l->oCount;++n){
	        if((Current = l->oCurrent[n]) == NULL)continue;
	        if(Current->Kill){
	            (*Current->Kill)(Current);
	        }
	    }
	    cFree((char *)l->oCurrent);
	    l->oCurrent=NULL;
	}
	if(l->oOld){
	    for(n=0;n<l->oCount;++n){
	        if((Current = l->oOld[n]) == NULL)continue;
	        if(Current->Kill){
	            (*Current->Kill)(Current);
	        }
	    }
	    cFree((char *)l->oOld);
	    l->oOld=NULL;
	}

	if(l->data)cFree((char *)l->data);
	l->data=NULL;
	
	if(l->Boxes)cFree((char *)l->Boxes);
	l->Boxes=NULL;
	
	if(l->Stretch)cFree((char *)l->Stretch);
	l->Stretch=NULL;
	
	if(l->itemList)cFree((char *)l->itemList);
	l->itemList=NULL;
	
	if(l->lAttributes)cFree((char *)l->lAttributes);
	l->lAttributes=NULL;
	
	
	zerol((char *)l,sizeof(*l));
	
	return 0;
	
}

int DOSetDrawCanvas(DOListPtr d)
{
	long length;
	
	if(!d)return 1;
	
	if(d->raster == RASTER_8){
	    length=d->xsize*d->ysize;
	}else{
	    length=d->xsize*d->ysize*3;
	}
	
	d->s.ScreenBits=d->raster;
	d->s.viewRect.xsize=(int)d->xsize;
	d->s.viewRect.ysize=(int)d->ysize;
	
	if((length <= d->length) && d->data)return 0;
	
	d->length=0;
	
	if(d->data)cFree((char *)d->data);
	d->data=cMalloc(length,8264);
	
	if(!d->data)return 1;
	
	d->length=length;
	
	zerol((char *)d->data,d->length);


	d->s.buffer=d->data;
	d->s.palette=(unsigned char *)d->palette;
	
	return 0;
}

int DOListADD(DOListPtr l,struct DObject *Object)
{
	long oMaximun;
	int ret;
	
	if(!l || !Object)return 1;
	
	ret = 1;
	if(DOListFindName(l,Object->name,AnyType)){
		sprintf(WarningBuff,"DOListADD create name  '%s' already used\n",Object->name);
		WarningBatch(WarningBuff);		
	}

	oMaximun=l->oMaximun;
	if(ResizeArray(&l->oCurrent,sizeof(DObjPtr),l->oCount,&l->oMaximun,2006))goto ErrorOut;
	if(ResizeArray(&l->oOld,sizeof(DObjPtr),l->oCount,&oMaximun,2007))goto ErrorOut;
	l->oCurrent[l->oCount]=Object;
	Object->l=l;
	l->oOld[l->oCount]=NULL;
	++l->oCount;
		
	ret = 0;
ErrorOut:
	return ret;
}
DObjPtr DOListFindName(DOListPtr l,char *name,int type)
{
	DObjPtr Found;
	long n;
    
	if(!l || !name)return NULL;
	
	Found=NULL;
		
	for(n=0;n<l->oCount;++n){
		DObjPtr Current;
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(!strcmp(Current->name,name)){
	        Found=Current;
	        break;
	    }
	}
	
	if(type == AnyType)return Found;	
	
	if(Found && (Found->type == type)){
	    return Found;
	}
	
	return NULL;
}
double PixelToLocal(long x,DOListPtr d)
{
	double xx;
	
	if(d && d->dpi){
	    xx=(double)x/(double)d->dpi;
	}else{
	    xx=0;
	}
	
	return xx;
}
int LocalToPixel(double x,DOListPtr d)
{
	int ix;
	
	
	if(d){
		ix=(int)(x*(double)d->dpi + 0.5);
	}else{
		ix=0;
	}
	
	return ix;
}
int rBoxDraw(rRect *r,DOListPtr d)
{
	if(!r || !d)return 1;
	
	rMoveTo(r->x,r->y,d);
	rLineTo(r->x,r->y+r->ysize,d);
	rLineTo(r->x+r->xsize,r->y+r->ysize,d);
	rLineTo(r->x+r->xsize,r->y,d);
	rLineTo(r->x,r->y,d);
	
	return 0;
}
int	rMoveTo(double x,double y,DOListPtr d)
{
	int ix,iy;
	
	if(!d)return 1;
	
	ix=(int)(x*(double)d->dpi + 0.5);
	iy=(int)(y*(double)d->dpi + 0.5);

	if(d->PrintFlag){
	   uMoveTo(ix+d->PrintRect.x,iy+d->PrintRect.y);
	}else{
	   uMoveU(ix,iy,&d->s);
	}
		
	return 0;
}
int	riMoveTo(int ix,int iy,DOListPtr d)
{
	
	if(!d)return 1;
	
	if(d->PrintFlag){
	   uMoveTo(ix+d->PrintRect.x,iy+d->PrintRect.y);
	}else{
	   uMoveU(ix,iy,&d->s);
	}
		
	return 0;
}
int rLineTo(double x,double y,DOListPtr d)
{
    int ix,iy;
	
	if(!d)return 1;
	
	ix=(int)(x*(double)d->dpi + 0.5);
	iy=(int)(y*(double)d->dpi + 0.5);

	if(d->PrintFlag){
		uLineTo(ix+d->PrintRect.x,iy+d->PrintRect.y,d->myIcon);
	}else{
		uLineU(ix,iy,d->nLineFore,&d->s);
	}
	
	return 0;
}
int riLineTo(int ix,int iy,DOListPtr d)
{
	
	if(!d)return 1;
	
	if(d->PrintFlag){
		uLineTo(ix+d->PrintRect.x,iy+d->PrintRect.y,d->myIcon);
	}else{
		uLineU(ix,iy,d->nLineFore,&d->s);
	}
	
	return 0;
}
int rDrawString(char *name,DOListPtr d)
{
	if(!name || !d)return 1;
	
	if(d->PrintFlag){
		uDrawString(name,d->myIcon);		    
	}else{
		uDrawStringU(name,d->nLineFore,0,&d->s);		    
	}
	
	return 0;
	
}
int rDrawStringAngle(char *name,double angle,DOListPtr d)
{
	if(!name || !d)return 1;
	
	uDrawStringAngleU(name,angle,d->nLineFore,0,&d->s);		    
	
	return 0;
	
}
int rDrawImageRect(iRect *r,DOListPtr d)
{
	uRect SRect;
	int n;
	
	if(!r || !d)return 1;
	
	SRect=r->box;
	
	SRect.x=0;
	SRect.y=0;
	
	for(n=0;n<r->box.xsize*r->box.ysize;++n){
	    if(r->data[n] != 0 && d->nLineFore != 0)r->data[n]=d->nLineFore;
	}
	
	WriteRect8ToRect24(r->data,&SRect,r->box.xsize,r->box.ysize,
                       d->data,&r->box,d->xsize,d->ysize,
                       (unsigned char *)d->palette);	  
	
	return 0;
}
int rFrameRect(rRect *r,DOListPtr d)
{
	double xsize2,ysize2;
	
	
	if(!r || !d)return 1;
	
 	xsize2=PixelToLocal((long)(0.5*(d->s.xLineWidth)),d);
 	ysize2=PixelToLocal((long)(0.5*(d->s.yLineWidth)),d);
 	
	rMoveTo(r->x-xsize2,r->y+ysize2,d);	
	rLineTo(r->x-xsize2,r->y+ysize2+r->ysize,d);	
	rLineTo(r->x-xsize2+r->xsize,r->y+ysize2+r->ysize,d);	
	rLineTo(r->x-xsize2+r->xsize,r->y+ysize2,d);	
	rLineTo(r->x-xsize2,r->y+ysize2,d);	
	
	return 0;
}
int rPaintRect(rRect *r,DOListPtr d)
{
	uRect ro;
	
	if(!r || !d)return 1;
	
	ro.x=LocalToPixel(r->x,d);
	ro.y=LocalToPixel(r->y,d);

	ro.xsize=LocalToPixel(r->xsize,d);
	ro.ysize=LocalToPixel(r->ysize,d);

	uPaintRectU(&ro,&d->s);
	
	return 0;
}
int rFrameOval(rRect *r,DOListPtr d)
{
	uRect ro;
	
	
	if(!r || !d)return 1;
	
 	
	ro.x=LocalToPixel(r->x,d);
	ro.y=LocalToPixel(r->y,d);

	ro.xsize=LocalToPixel(r->xsize,d);
	ro.ysize=LocalToPixel(r->ysize,d);
	d->s.nLineFore=d->nLineFore;
	uFrameOvalU(&ro,&d->s);
	
	return 0;
}
int rPaintOval(rRect *r,DOListPtr d)
{
	uRect ro;
	
	if(!r || !d)return 1;
	
	ro.x=LocalToPixel(r->x,d);
	ro.y=LocalToPixel(r->y,d);

	ro.xsize=LocalToPixel(r->xsize,d);
	ro.ysize=LocalToPixel(r->ysize,d);

	d->s.nLineFore=d->nLineFore;
	uPaintOvalU(&ro,&d->s);
	
	return 0;
}
int rFrameRoundRect(rRect *r,double ovalWidth,double ovalHeight,DOListPtr d)
{
	uRect ro;
	
	
	if(!r || !d)return 1;
	
 	
	ro.x=LocalToPixel(r->x,d);
	ro.y=LocalToPixel(r->y,d);

	ro.xsize=LocalToPixel(r->xsize,d);
	ro.ysize=LocalToPixel(r->ysize,d);
	d->s.nLineFore=d->nLineFore;
	uFrameRoundRectU(&ro,LocalToPixel(ovalWidth,d),LocalToPixel(ovalHeight,d),&d->s);
	
	return 0;
}
int rPaintRoundRect(rRect *r,double ovalWidth,double ovalHeight,DOListPtr d)
{
	uRect ro;
	
	if(!r || !d)return 1;
	
	ro.x=LocalToPixel(r->x,d);
	ro.y=LocalToPixel(r->y,d);

	ro.xsize=LocalToPixel(r->xsize,d);
	ro.ysize=LocalToPixel(r->ysize,d);

	d->s.nLineFore=d->nLineFore;
	uPaintRoundRectU(&ro,LocalToPixel(ovalWidth,d),LocalToPixel(ovalHeight,d),&d->s);
	
	return 0;
}
static struct DObject *ObjectFindObject(struct DObject *Object,rPoint *r)
{
	if(!Object || !r)return NULL;
	
	if(r->x < Object->box.x || r->y < Object->box.y)return NULL;
	if(r->x > Object->box.x+Object->box.xsize || r->y > Object->box.y+Object->box.ysize)return NULL;
	
	return Object;
}
static struct DObject *ObjectDoubleClick(struct DObject *Object,rPoint *r)
{
	if(!Object || !r)return NULL;
	
	return NULL;
}
static int ObjectUpdate(struct DObject *o)
{
	DObjPtr b=(DObjPtr)o;
	
	if(!b || !b->l)return 1;
	
	if(b->Selected){
	    ObjectBoxStretch(&b->box,b->l);
	}

	return 0;
	
}
static struct uAttributes *ObjectAttributes(struct DObject *Object)
{
	if(!Object)return NULL;
	return &Object->Attributes;
}	
static int ObjectSetSelectedRegion(struct DObject *Object,DOListPtr l)
{
	if(!Object || !l)return 1;
	
	if(!Object->Selected)return 0;
	
	if(ResizeArray(&l->Boxes,sizeof(rRect),l->rCountCount,&l->rCountMax,2008))return 0;
	
	l->Boxes[l->rCountCount++]=Object->box; 
		   
	return 0;
	
}
static int ObjectCheckStretch(uPoint pt,struct DObject *Current,DOListPtr l)
{
	uRect r;
	int k;
	
	if(!Current || !l)return 0;
	
    for(k=0;k<4;++k){
        r=DOSelectRect(k,&Current->box,l);
	    if(uPtInRect(pt,&r)){
	        return 1;
	    }
    }
	    
	return 0;
}

static struct DObject *ObjectStretch(uPoint pt,struct DObject *o,DOListPtr l)
{
	double xOff=0,yOff=0;
	uRect r,s;
	rPoint pu;
	uPoint p,p2;
	int k;
	int n;
	
	if(!o || !l)return NULL;
	
	n = -1;
    for(k=0;k<4;++k){
        r=DOSelectRect(k,&o->box,l);
	    if(uPtInRect(pt,&r)){
	        n=k;
	        break;
	    }
    }
    
    if(n < 0)return NULL;
       
	pu.x=o->box.x;
    pu.y=o->box.y;
    p=localToScreen(&pu,l);
    
    pu.x += o->box.xsize;
    pu.y += o->box.ysize;
    
    p2=localToScreen(&pu,l);
    
	r.x=p.x;
	r.y=p.y;
	
	r.xsize=p2.x-p.x;
	r.ysize=p2.y-p.y;

	s=r;
	drawStretch(&r,n,l->myIcon);
	
	if(s.x==r.x && s.xsize==r.xsize && s.y==r.y && s.ysize==r.ysize)return NULL;

	if(o->Duplicate){
	    o=(*o->Duplicate)(o,0,&xOff,&yOff);
	    if(!o)return o;
	    o->box= DOrRectFromScreen(&r,l);
	    return o;
	}
    
	return NULL;
}

static int ObjectDrawSelected(struct DObject *o,DOListPtr l)
{
	DObjPtr b=(DObjPtr)o;
	
	if(!b || !l)return 1;
	
	if(b->Selected){
	    ObjectBoxFrame(&b->box,l);
	}

	return 0;
	
}
struct DObject *ObjectObject2(DObjPtr Pastin)
{
	DObjPtr o;
	
	if(!Pastin)return NULL;
	
	o=ObjectCreate(Pastin->name,Pastin->type,&Pastin->box,Pastin->size);
	if(o){
		o->FlipFlag=Pastin->FlipFlag;
		o->AngleFlag=Pastin->AngleFlag;
		o->Attributes = Pastin->Attributes;
	}
	
	return o;
}
struct DObject *ObjectCreate(char *name,int type,rRect *box,long size)
{
	struct DObject *Object;
	
	if(!name || !box )return NULL;
	
	Object=(struct DObject *)cMalloc(size,8213);
	if(!Object)return NULL;
	zerol((char *)Object,size);
	
	Object->box  = *box;
	Object->type = type;	
	Object->size = size;	
						
	mstrncpy(Object->name,name,64);
	
	Object->Selected=TRUE;
	
	Object->oStatus=StatusNew;
	
	Object->ItWas = -1;
	
	ObjectSet(Object);
	
	return Object;
}
static int ObjectSelectedPoint(DObjPtr o,rPoint *ClickPoint)
{
	if(!o)return 0;
	ClickPoint=ClickPoint;
	return o->Selected;
}

static void ObjectSelectOne(DObjPtr o,rPoint *ClickPoint)
{

	struct DOList *l;
	IconPtr myIcon;
	
	if(!o)return;
	l=o->l;
	if(!l)return;
	myIcon=l->myIcon;
	if(!myIcon)return;
	
	ClickPoint=ClickPoint;
	if(!(o->Selected)){
		uSetPenXOR(TRUE,myIcon);	    
	    o->Selected=TRUE;
	    if(o->DrawSelected){
	        (*o->DrawSelected)(o,l);
	    }
		uSetPenXOR(FALSE,myIcon);	 
	}
}

static void ObjectDeSelectOne(DObjPtr o,rPoint *ClickPoint)
{
	struct DOList *l;
	IconPtr myIcon;
	
	if(!o)return;
	l=o->l;
	if(!l)return;
	myIcon=l->myIcon;
	if(!myIcon)return;
	
	ClickPoint=ClickPoint;
	if(o->Selected){
		uSetPenXOR(TRUE,myIcon);	    
	    if(o->DrawSelected){
	        (*o->DrawSelected)(o,l);
	    }
		uSetPenXOR(FALSE,myIcon);	 
	    o->Selected=FALSE;
	}
}
int ObjectSet(DObjPtr Object)
{
	if(!Object)return 1;
	
	Object->GetAttributes=ObjectAttributes;
	
	Object->Kill=ObjectKill;

	Object->Draw=ObjectDraw;

	Object->GetFrameCount=ObjectFramCount;
	
	Object->SetFrame=ObjectSetFrame;
	
	Object->FindObject=ObjectFindObject;
	
	Object->DoubleClick=ObjectDoubleClick;
	
	Object->Update=ObjectUpdate;
	
	Object->Duplicate=ObjectDuplicate;
	
	Object->Attribute=ObjectAttribute;
	
	Object->DeSelectOne=ObjectDeSelectOne;
	
	Object->SelectOne=ObjectSelectOne;

	Object->SelectedPoint=ObjectSelectedPoint;
	
	Object->SetSelectedRegion=ObjectSetSelectedRegion;
	
	Object->Message=ObjectGetMessage;
	
	Object->CheckStretch=ObjectCheckStretch;
	
	Object->Stretch=ObjectStretch;
	
	Object->Move=ObjectMove;	

	Object->KillSelected=ObjectKillSelected;
	
	Object->GetFontSelected=ObjectGetFontSelected;
	
	Object->SetFontSelected=ObjectSetFontSelected;
	
	Object->Copy=ObjectCopy;
	
	Object->DoInformation=ObjectDoInformation;
	
	Object->DrawSelected=ObjectDrawSelected;
	
	return 0;
}

void ObjectKill(struct DObject *Object)
{
	if(!Object)return;
	
	zerol((char *)Object,Object->size);
	cFree((char *)Object);
}


static int ObjectDraw(struct DObject *Object,DOListPtr l)
{
	if(!Object || !l)return 1;
	
	return 0;
	
}
static int ObjectSetFrame(struct DObject *Object,long FrameCount)
{
	if(!Object)return 1;
	
	FrameCount=FrameCount;
	
	return 0;
	
}
static int ObjectFramCount(struct DObject *Object,long *FrameCount)
{
	if(!Object || !FrameCount)return 1;
	
	if(*FrameCount <= 0)*FrameCount = 1;
	
	return 0;
	
}
int uSetAttributes(DOListPtr l,struct uAttributes *a,int Flag)
{
	/*	Routine SetAttributes */
	
	/*
	uPattern *pat;
	*/
	
	if(!l || !a)return 1;
	
	if(Flag & sForeColor){
	    l->s.nSolidFore=a->nSolidFore;
	    uSetForeByIndex(a->nSolidFore,l->myIcon);
	}
	if(Flag & sBackColor){
	    l->s.nSolidBack=a->nSolidBack;
	    uSetBackByIndex(a->nSolidBack,l->myIcon);
	}
	if(Flag & sPattern){
	    l->s.SolidPattern=a->SolidPattern;
	    /*
		pat=PatternPattern(a->SolidPattern);
		if(pat){
	       uSetPenPattern(pat->pat,l->myIcon);
	    }
	    */
	}
	if(Flag & lForeColor){
		l->nLineFore=a->nLineFore;
	    l->s.nLineFore=a->nLineFore;
	    uSetForeByIndex(a->nLineFore,l->myIcon);
	}
	if(Flag & lBackColor){
		l->nLineBack=a->nLineBack;
	    l->s.nLineBack=a->nLineBack;
	    uSetBackByIndex(a->nLineBack,l->myIcon);
	}
	
	if(Flag & lPattern){
	    l->s.LinePattern=a->LinePattern;
	    /*
		pat=PatternPattern(a->LinePattern);
		if(pat){
	       uSetPenPattern(pat->pat,l->myIcon);
	    }
	    */
	}
	
	if(Flag & lWidth){
	    l->s.xLineWidth=(int)a->xLineWidth;
	    l->s.yLineWidth=(int)a->yLineWidth;
	    /*
	    uSetPenWidth(a->xLineWidth,a->yLineWidth,l->myIcon);
	    */
	}
	
	if(Flag & tFont){
		uSetTextFace(l->myIcon,a->font.txFace);
		
		uSetTextFont(l->myIcon,a->font.txFont);
		
		uSetFontSize(l->myIcon,a->font.txSize);
	}
	return 0;
}
int uPutAttributes(struct uAttributes *Aout,struct uAttributes *Ain,int Flag)
{
	if(!Aout || !Ain)return 1;
	
	if(Flag & sForeColor){
	    Aout->nSolidFore=Ain->nSolidFore;
	}
	if(Flag & sBackColor){
	    Aout->nSolidBack=Ain->nSolidBack;
	}
	if(Flag & sPattern){
	    Aout->SolidPattern=Ain->SolidPattern;
	}
	if(Flag & lForeColor){
	    Aout->nLineFore=Ain->nLineFore;
	}
	if(Flag & lBackColor){
	    Aout->nLineBack=Ain->nLineBack;
	}
	if(Flag & lPattern){
	    Aout->LinePattern=Ain->LinePattern;
	}
	
	if(Flag & lSymbol){
	    Aout->LineSymbol=Ain->LineSymbol;
	}
	
	if(Flag & lLine){
	    mstrncpy((char *)&Aout->LineDash,(char *)&Ain->LineDash,16);
	    Aout->DashFlag=Ain->DashFlag;
	    Aout->LineDashNumber=Ain->LineDashNumber;
	}
	
	if(Flag & lWidth){
	    Aout->xLineWidth=Ain->xLineWidth;
	    Aout->yLineWidth=Ain->yLineWidth;
	}
	
	if(Flag & lAnimate){
	    Aout->Animation=Ain->Animation;
	    Aout->AnimationStart=Ain->AnimationStart;
	    Aout->AnimationStep=Ain->AnimationStep;
	    Aout->AnimationJust=Ain->AnimationJust;
	    mstrncpy((char *)&Aout->AnimationFormat,(char *)&Ain->AnimationFormat,12);
	}
	return 0;
	
}
int SaveCopy(char *obj,long Length,char **list,long *Total)
{
	char *put,*save;
	long n,size;
	
	if(!obj || !Length || !list || !Total)return 0;
	
	save = *list;
	size = *Total + Length;
	if((put=cMalloc(size,8049)) == NULL){
	    cFree((char *)obj);
	    if(save)cFree((char *)save);
	    *list=NULL;
	    *Total=0;
	    return 1;
	}
	
	*list=put;
	
	if(save){
	    for(n=0;n < *Total;++n){
	        *put++ = save[n];
	    }
	    cFree((char *)save);
	}
	
	for(n=0;n<Length;++n){
	    *put++ = obj[n];
	}
	
	*Total = size;
	if(obj)cFree((char *)obj);
	return 0;
}

