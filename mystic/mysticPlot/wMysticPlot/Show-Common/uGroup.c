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

DObjPtr GetScrapObject(struct Obj *obj);
int SaveCopy(char *obj,long Length,char **list,long *Total);

static int uGroupMessage(DObjPtr o,long MessageType,void *MessageData);
static void uGroupDelete(uGroup2Ptr g);
static char *uGroupCopy(DObjPtr o,long *iLength);
static DObjPtr uGroupDoInformation(DObjPtr o);
int uGroupSet(uGroup2Ptr b);
static void uGroupAttribute(DObjPtr o,struct uAttributes *vAttributes,int Flag);
static void uGroupKill(DObjPtr b);
static int uGroupDraw(DObjPtr o,struct DOList *l);
static DObjPtr uGroupMove(DObjPtr o,double *dx,double *dy);
static DObjPtr uGroupDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff);
static int uGroupUpdate(DObjPtr o);

rRect rUnion(rRect *rin,rRect *xBoxes);


static int uGroupSetFrame(struct DObject *o,long CurrentFrame);

static int uGroupFrameCount(struct DObject *o,long *FrameCount);

static struct DObject *uGroupDoubleClick(struct DObject *o,rPoint *r);
static DObjPtr uGroupDoInformation(DObjPtr o);


static struct DObject *uGroupDoubleClick(struct DObject *o,rPoint *r)
{
	if(!o || !r)return (struct DObject *)NULL;

	if(r->x < o->box.x || r->y < o->box.y){
		return (struct DObject *)NULL;
	}
	
	if(r->x > o->box.x+o->box.xsize || r->y > o->box.y+o->box.ysize){
		return (struct DObject *)NULL;
	}			
	return (struct DObject *)uGroupDoInformation(o);
}

static struct DObject *uGroupWrite(struct DObject *o,FILE8 *inOut,struct DOList *l)
{


	static struct uGroup b;
	
	static struct universalTableListStruct Local[] = {
		{"REAL",4319,universal_TypeRealArrayPtr,(void *)&b.itemList,0L},				
		{"DOUB",4320,universal_TypeDouble,(void *)&b.box.x},	
		{"DOUB",4321,universal_TypeDouble,(void *)&b.box.y},	
		{"DOUB",4322,universal_TypeDouble,(void *)&b.box.xsize},	
		{"DOUB",4323,universal_TypeDouble,(void *)&b.box.ysize},	
	};
	
	static struct universalTableStruct Global = {
		"uDraw parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	uGroup2Ptr bp=(uGroup2Ptr)o;
	char head[5];
	long length;
	long tag;
	int n,nn;
	
	if(!inOut || !l)return (struct DObject *)NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    Local[0].space=0;
	    b.itemList=NULL;
		if(getUniversalTable(&Global,inOut))goto ErrorOut;		
		nn=(int)Local[0].space;
		if(nn <= 0 || !b.itemList)goto ErrorOut;
		bp=uGroupGroup(nn,l);
		if(!bp)goto  ErrorOut;
		bp->itemList=b.itemList;
	    for(n=0;n<nn;++n){
			DObjPtr Return,Object=DOGetObjectPtr((int)bp->itemList[n],l);
			if(!Object || !Object->ReadWrite)continue;
	        if((Return = (*Object->ReadWrite)(NULL,inOut,l)) == NULL)goto ErrorOut;
	        /*
	        if(Return->DeSelectOne)(*Return->DeSelectOne)((DObjPtr)Return,NULL);
	        */
	        bp->Groups[n]=Return;
	    }		
	    uGroupUpdate((DObjPtr)bp);
		return (struct DObject *)bp;			
		
	}else{
		if(bp->nGroups <= 0)return (struct DObject *)NULL;
	    if(bp->itemList)cFree((char *)bp->itemList);
	    bp->itemList=(float *)cMalloc(bp->nGroups*sizeof(float),8327);
	    if(!bp->itemList)return (struct DObject *)NULL;
	    
	    nn=0;
	    for(n=0;n<bp->nGroups;++n){
	        if(!bp->Groups[n] || !bp->Groups[n]->ReadWrite)continue;
	    	bp->itemList[nn++]=(float)bp->Groups[n]->type;
	    }
	    if(nn <= 0)return (struct DObject *)NULL;
	    b = *bp;
	    Local[0].space=nn;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	    for(n=0;n<bp->nGroups;++n){
	        if(!bp->Groups[n] || !bp->Groups[n]->ReadWrite)continue;
	        if((*bp->Groups[n]->ReadWrite)(bp->Groups[n],inOut,l) == NULL)goto ErrorOut;
	    }
	}
	
	return (struct DObject *)&b;
ErrorOut:
	return (struct DObject *)NULL;
}

static int uGroupFrameCount(struct DObject *o,long *FrameCount)
{
	uGroup2Ptr g=(uGroup2Ptr)o;
	long n;
	
	if(!g || !FrameCount)return 1;
	
	for(n=0;n<g->nGroups;++n){
		DObjPtr Current;
		Current=g->Groups[n];
	    if(Current && Current->GetFrameCount){
	        (*Current->GetFrameCount)(Current,FrameCount);
	    }
	}
	return 0;
}
static int uGroupSetFrame(struct DObject *o,long CurrentFrame)
{
	uGroup2Ptr g=(uGroup2Ptr)o;
	long n;
	
	if(!g)return 1;
	
	for(n=0;n<g->nGroups;++n){
		DObjPtr Current;
		Current=g->Groups[n];
	    if(Current && Current->SetFrame){
	        (*Current->SetFrame)(Current,CurrentFrame);
	    }
	}
	return 0;
}
int uGroupMessage(DObjPtr o,long MessageType,void *MessageData)
{
	uGroup2Ptr g=(uGroup2Ptr)o;
	long n;
	
	if(!g)return 1;
	
	for(n=0;n<g->nGroups;++n){
		DObjPtr Current;
		Current=g->Groups[n];
	    if(Current && Current->Message){
	        (*Current->Message)(Current,MessageType,MessageData);
	    }
	}
	return 0;
}
static void uGroupDelete(uGroup2Ptr g)
{
	long n;
	
	if(!g)return;
	
	if(g->Groups){
	    for(n=0;n<g->nGroups;++n){
		    DObjPtr Current;
		    Current=g->Groups[n];
	        if(Current){
	            if(Current->Kill)(*Current->Kill)(Current);
	            Current=NULL;
	        }
	    }
	    cFree((char *)g->Groups);
	    g->Groups=NULL;
	}
	
	if(g->itemList)cFree((char *)g->itemList);
	g->itemList=NULL;
	
	ObjectKill((DObjPtr)g);
	
}
static DObjPtr uGroupDoInformation(DObjPtr o)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {77,135,80,30},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {172,135,80,30},uDialogButton,uDialogNone},
      /*  2 */      {"0", {160,50,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"10", {160,75,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"1", {160,100,80,20},uDialogText,uDialogGetLong},
      /*  5 */      {"Group", {177,10,80,20},uDialogText,uDialogNone},
      /*  6 */      {"Left        :", {70,75,65,20},uDialogString,uDialogNone},
      /*  7 */      {"Object Type", {45,10,90,20},uDialogString,uDialogNone},
      /*  8 */      {"Items    :", {70,100,65,20},uDialogString,uDialogNone},
      /*  9 */      {"Top         :", {70,50,65,20},uDialogString,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,325,220};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	uGroup2Ptr pBox;
	double dx,dy;
	uGroup2Ptr g=(uGroup2Ptr)o;
	
	if(!g)return NULL;
		            

	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	pBox=NULL;
	
	uList=uDialogOpen("Group Information",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)g->box.y,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)g->box.x,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)g->nGroups,
	              	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

		
	dx=uR[3].dreturn-g->box.x;
	dy=uR[2].dreturn-g->box.y;
	
	if(g->Move){
		if((pBox=(uGroup2Ptr)(*g->Move)((DObjPtr)g,&dx,&dy)) == NULL){
		    goto ErrorOut;
		}
	}
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	if(ret && pBox){
	   uGroupKill((DObjPtr)pBox);
	   pBox=NULL;
	}

	return (DObjPtr)pBox;
}
uGroup2Ptr uGroupGroup(long Count,struct DOList *l)
{
	static rRect box={2.0,1.0,6.0,4.0};
	uGroup2Ptr g;
	char name[64];
	long n;
	
	if(!l)return NULL;
	
	sprintf(name,"Group%03ld%c",l->ObjectIDCount++,0);
	
    g=(uGroup2Ptr)ObjectCreate(name,TruePlotType,&box,sizeof(struct uGroup));
    if(!g)goto ErrorOut;
		
	if(uGroupSet(g))goto ErrorOut;
	
	g->nGroups=0;
	g->Groups=NULL;
	g->type=GroupsType;
	g->nGroups=Count;
	g->l=l;
	g->Groups = (DObjPtr *)cMalloc(g->nGroups*sizeof(DObjPtr),8034);
	if(g->Groups == NULL){
	    WarningBatch("uGroupuGroup Could Not Allocate New Object List\n");
	    return g;
	}
	for(n=0;n<g->nGroups;++n){
	    g->Groups[n]=NULL;
	}

	return g;
ErrorOut:
	uGroupDelete(g);
	return NULL;
}

static void uGroupAttribute(DObjPtr o,struct uAttributes *vAttributes,int Flag)
{
	uGroup2Ptr g=(uGroup2Ptr)o;
	long n;
	
	if(!g || !vAttributes || !vAttributes)return;
	
	for(n=0;n<g->nGroups;++n){
	    DObjPtr Current;
	    if((Current=g->Groups[n]) != NULL){
	        if(Current->Attribute)(*Current->Attribute)(Current,vAttributes,Flag);
	    }
	}
}

static void uGroupKill(DObjPtr b)
{
	uGroup2Ptr g=(uGroup2Ptr)b;
	if(!g)return;
	 uGroupDelete(g);
}


static int uGroupDraw(DObjPtr o,struct DOList *l)
{
	long n;
	uGroup2Ptr g=(uGroup2Ptr)o;
	
	if(!g || !l)return 1;
	
	for(n=0;n<g->nGroups;++n){
		DObjPtr Current;
		Current=g->Groups[n];
	    if(Current && Current->Draw){
	        (*Current->Draw)(Current,l);
	    }
	}
	
	return 0;
}


static DObjPtr uGroupMove(DObjPtr o,double *dx,double *dy)
{
	uGroup2Ptr Return;
	long n,nSum;
	uGroup2Ptr g=(uGroup2Ptr)o;
	
	if(!g || !dx || !dy)return NULL;
	
	if((Return=uGroupGroup(g->nGroups,g->l)) == NULL){
	    return NULL;
	}
	nSum=0;
	for(n=0;n<g->nGroups;++n){
	    DObjPtr Current,gReturn;
	    if((Current=g->Groups[n]) != NULL){
	        if(!Current->Move)continue;
	        if((gReturn=(*Current->Move)(Current,dx,dy)) == NULL){
	            uGroupDelete(Return);
	            return NULL;
	        }else{
	            Return->Groups[nSum++]=gReturn;
	        }
	    }
	}
	Return->nGroups=nSum;
	return (DObjPtr)Return;
}
static DObjPtr uGroupDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff)
{
	uGroup2Ptr Return;
	long n,nSum;
	uGroup2Ptr g=(uGroup2Ptr)o;
	
	if(!g || !xOff || !yOff)return NULL;
	
	if((Return=uGroupGroup(g->nGroups,g->l)) == NULL){
	    return NULL;
	}
	nSum=0;
	for(n=0;n<g->nGroups;++n){
	    DObjPtr Current,gReturn;
	    if((Current=g->Groups[n]) != NULL){
	        if(!Current->Duplicate)continue;
	        if((gReturn=(*Current->Duplicate)(Current,iDup,xOff,yOff)) == NULL){
	            uGroupDelete(Return);
	            return NULL;
	        }else{
	        /*
	        	if(gReturn->DeSelectOne)(*gReturn->DeSelectOne)((DObjPtr)gReturn,NULL);
	        */
	            Return->Groups[nSum++]=gReturn;
	        }
	    }
	}
	Return->nGroups=nSum;
	return (DObjPtr)Return;
}
static int uGroupUpdate(DObjPtr o)
{
	uGroup2Ptr g=(uGroup2Ptr)o;
	rRect r;
	long n;
	int pass;
	static long Count;
	
	if(!g)return 1;
	
	Count++;
	pass=0;
	for(n=0;n<g->nGroups;++n){
	    DObjPtr Current;
	    if((Current=g->Groups[n]) != NULL){
	        if(Current->type == GroupsType){
	           if(Current->Update)(*Current->Update)(Current);
	        }
	        if(pass++ == 0){
	            r=Current->box;
	        }else{
	           r=rUnion(&r,&Current->box);
	        }
	    }
	}
	
	g->box=r;

	if(g->Selected && (Count == 1)){
	    ObjectBoxStretch(&g->box,g->l);
	}
	
	Count--;
	return 0;
}

rRect rUnion(rRect *rin,rRect *xBoxes)
{
	static rRect zero;
	double rmax,dr;
	rRect r;
	
	if(!rin || !xBoxes)return zero;
	
	r=*rin;
	
    dr=r.x-xBoxes->x;
	if(dr > 0){
	    r.x=xBoxes->x;
	    r.xsize += dr;
	}
	rmax=xBoxes->x + xBoxes->xsize;
	if(rmax > (r.x + r.xsize))r.xsize=rmax-r.x;
	
    dr=r.y-xBoxes->y;
	if(dr > 0){
	    r.y=xBoxes->y;
	    r.ysize += dr;
	}
	rmax=xBoxes->y + xBoxes->ysize;
	if(rmax > (r.y + r.ysize))r.ysize=rmax-r.y;
			
	return r;	
}


static char *uGroupCopy(DObjPtr o,long *iLength)
{
	uGroup2Ptr pBox;
	struct Obj *objr;
	long size;
	char *obj,*list;
	long Total,Length,n;
	uGroup2Ptr g=(uGroup2Ptr)o;
	
	if(!g || !iLength)return NULL;
	
	size=sizeof(struct Obj)+sizeof(*pBox);
	if((objr=(struct Obj *)cMalloc(size,8032)) == NULL){
	    return NULL;
	}
	pBox=(uGroup2Ptr)(objr+1);
	objr->type=g->type;
	objr->Length=size;
	*pBox = *g;


	Total=0;	
	list=NULL;
	if(SaveCopy((char *)objr,size,&list,&Total)){
	    return NULL;
	}
	
	for(n=0;n<g->nGroups;++n){
		DObjPtr Current;
		Current=g->Groups[n];
	    if(Current && Current->Copy){
	    	if((obj=(*Current->Copy)(Current,&Length)) != NULL){
	    	    if(SaveCopy(obj,Length,&list,&Total)){
	    	        return NULL;
	    	    }
	    	}
	    }
	}
	*iLength = Total;
	objr=(struct Obj *)list;
	objr->Length=Total;
	return list;
}
/*
static void uGroupDeSelectOne(DObjPtr o,rPoint *ClickPoint)
{
	struct DOList *l;
	IconPtr myIcon;
	long n;
	uGroup2Ptr g=(uGroup2Ptr)o;
	static long Count;
	
	if(!g)return;
	l=g->l;
	if(!l)return;
	myIcon=l->myIcon;
	if(!myIcon)return;
	
	Count++;
	ClickPoint=ClickPoint;
	for(n=0;n<g->nGroups;++n){
		DObjPtr Current;
		Current=g->Groups[n];
	    if(Current && Current->DeSelectOne){
	        Current->Selected=FALSE;
	        (*Current->DeSelectOne)((DObjPtr)Current,NULL);
	    }
	}
	
	if(g->Selected && (Count == 1)){
		uSetPenXOR(TRUE,myIcon);	    
	    if(g->DrawSelected){
	        (*g->DrawSelected)(o,l);
	    }
		uSetPenXOR(FALSE,myIcon);	 
	    g->Selected=FALSE;
	}	
	
	Count--;
}
*/
int uGroupSet(uGroup2Ptr b)
{
	if(!b)return 1;
	
	b->type=GroupsType;
	b->Attribute=uGroupAttribute;
	b->Kill=uGroupKill;
	b->Draw=uGroupDraw;
	b->Message=uGroupMessage;
	b->DoInformation=uGroupDoInformation;
	b->Move=uGroupMove;
	b->Duplicate=uGroupDuplicate;	
	b->Update=uGroupUpdate;
	b->SetFrame=uGroupSetFrame;
    b->GetFrameCount=uGroupFrameCount;
	b->Copy=uGroupCopy;
	b->ReadWrite=uGroupWrite;
	b->DoubleClick=uGroupDoubleClick;		
	/*
	b->DeSelectOne=uGroupDeSelectOne;
	*/
	return 0;
}
uGroup2Ptr uGroupGroup2(uGroup2Ptr Pastin,DOListPtr d)
{
	struct Obj *obj;
	char *p;
	long n;
	uGroup2Ptr g;

	if(!Pastin || !d)return NULL;

	g=uGroupGroup(Pastin->nGroups,d);
	if(!g)return NULL;
	p=(char *)Pastin;
	p += sizeof(*g);
	for(n=0;n<g->nGroups;++n){
	    DObjPtr Object;
	    obj=(struct Obj *)p;
	    if((Object=DOGetScrapObject(obj,d)) == NULL)return g;
	    g->Groups[n]=Object;
	    p += obj->Length;
	}
	
	return g;
}
