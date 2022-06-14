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

int ResizeArray(void *Array,long Length,long Count,long *Maximun,int tag);
static int uLineCheckStretch(uPoint pt,struct DObject *Current,DOListPtr l);
static int drawStretch(uPoint *p1,uPoint *p2,IconPtr myIcon);

static int uLineUpdateBox(DObjPtr o);

static char *uLineCopy(DObjPtr o,long *Length);
int uLineSet(uLinePtr b);
static void uLineDelete(uLinePtr b);
static int uLineUpdate(DObjPtr o);

static uLinePtr uLinedoLines(DObjPtr o,rPoint *p1,rPoint *p2,struct uAttributes *Attributes);
static DObjPtr uLineDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff);

static DObjPtr uLineMove(DObjPtr o,double *dx,double *dy);

static struct DObject *uLineStretch(uPoint pt,struct DObject *o,DOListPtr l);

static DObjPtr uLineDoInformation(DObjPtr o);

static struct DObject *uLineDoubleClick(struct DObject *o,rPoint *r);
static DObjPtr uLineDoInformation(DObjPtr o);


static struct DObject *uLineDoubleClick(struct DObject *o,rPoint *r)
{
	if(!o || !r)return (struct DObject *)NULL;

	if(r->x < o->box.x || r->y < o->box.y){
		return (struct DObject *)NULL;
	}
	
	if(r->x > o->box.x+o->box.xsize || r->y > o->box.y+o->box.ysize){
		return (struct DObject *)NULL;
	}			
	return uLineDoInformation(o);
}
static struct DObject *uLineWrite(struct DObject *o,FILE8 *inOut,struct DOList *l)
{


	static struct uLine b;
	
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
		
		
		{"DOUB",4031,universal_TypeDouble,(void *)&b.p1.x},	
		{"DOUB",4032,universal_TypeDouble,(void *)&b.p1.y},	
		{"DOUB",4033,universal_TypeDouble,(void *)&b.p2.x},	
		{"DOUB",4034,universal_TypeDouble,(void *)&b.p2.y},	
		
		
			
	};
	
	static struct universalTableStruct Global = {
		"uDraw parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	uLinePtr bp=(uLinePtr)o;
	static rRect r;
	char head[5];
	long length;
	long tag;
	
	if(!inOut || !l)return (struct DObject *)NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp=uLineLine(&r,&l->Attributes,l);
		if(!bp)goto  ErrorOut;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(struct uLine);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;	
		return (struct DObject *)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (struct DObject *)&b;
ErrorOut:
	return (struct DObject *)NULL;
}
static DObjPtr uLineDoInformation(DObjPtr o)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {75,160,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {170,160,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"0", {160,50,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"10", {160,75,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"1", {160,100,80,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"1", {160,125,80,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"Line", {177,10,80,20},uDialogText,uDialogNone},
      /*  7 */      {"Object Type", {45,10,90,20},uDialogString,uDialogNone},
      /*  8 */      {"x1  :", {70,50,65,20},uDialogString,uDialogNone},
      /*  9 */      {"y1  :", {70,75,65,20},uDialogString,uDialogNone},
      /* 10 */      {"x2  :", {70,100,65,20},uDialogString,uDialogNone},
      /* 11 */      {"y2  :", {70,125,65,20},uDialogString,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,325,220};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	uLinePtr b=(uLinePtr)o;
	uLinePtr pBox;

	if(!b)return NULL;
	
	uGetPort(&save);
	
	ret = 1;
	
	uR=(uDialogReturn *)NULL;
	
	pBox=(uLinePtr)NULL;
	
	uList=uDialogOpen("Line Information",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)b->p1.x,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)b->p1.y,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)b->p2.x,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)b->p2.y,
	              	              
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

		       	
	if((pBox=uLinedoLines((DObjPtr)b,&b->p1,&b->p2,&b->Attributes)) == NULL){
	    goto ErrorOut;
	}	
	
	pBox->p1.x=uR[2].dreturn;
	pBox->p1.y=uR[3].dreturn;
	pBox->p2.x=uR[4].dreturn;
	pBox->p2.y=uR[5].dreturn;
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	if(ret && pBox){
		uLineDelete(pBox);
	    pBox=NULL;
	}
	
	return (DObjPtr)pBox;
}
static DObjPtr uLineMove(DObjPtr o,double *dx,double *dy)
{ 
	uLinePtr pLine;
	uLinePtr b=(uLinePtr)o;

	if(!b || !dx || !dy)return NULL;
	
	if((pLine=(uLinePtr)uLineDuplicate((DObjPtr)b,0,dx,dy)) != NULL){
	    pLine->xDup = b->xDup;
	    pLine->yDup = b->yDup;
	}
	
	uLineUpdateBox((DObjPtr)pLine);
	
	return (DObjPtr)pLine;
}
static DObjPtr uLineDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff)
{
	uLinePtr pLine;
	double x,y;
	uLinePtr b=(uLinePtr)o;

	if(!b || !xOff || !yOff)return NULL;

	
	if((pLine=uLinedoLines((DObjPtr)b,&b->p1,&b->p2,&b->Attributes)) == NULL){
	    return NULL;
	}
	if(iDup){
	    x = b->box.x-b->xDup;
	    y = b->box.y-b->yDup;
	}else{
	    x = *xOff;
	    y = *yOff;
	}
	pLine->xDup = pLine->box.x;
	pLine->yDup = pLine->box.y;
	pLine->box.x += x;
	pLine->box.y += y;
	pLine->p1.x += x;
	pLine->p1.y += y;
	pLine->p2.x += x;
	pLine->p2.y += y;
	
	uLineUpdateBox((DObjPtr)pLine);
	
	return (DObjPtr)pLine;
}
static uLinePtr uLinedoLines(DObjPtr o,rPoint *p1,rPoint *p2,struct uAttributes *Attributes)
{
	uLinePtr pLine;
	rRect r;
	uLinePtr b=(uLinePtr)o;

	if(!b || !Attributes || !p1 || !p2)return NULL;

	r.x=p1->x;
	r.y=p1->y;
	r.xsize=p2->x;
	r.ysize=p2->y;
	
	pLine=uLineLine(&r,Attributes,b->l);
	if(pLine == NULL){
	    return NULL;
	}

	pLine->p1= *p1;
	pLine->p2= *p2;

	return pLine;
}

uLinePtr uLineLine(rRect *r,struct uAttributes *Attribute,struct DOList *l)
{
	char name[64];
	uLinePtr b;
	
	if(!r || !Attribute || !l)return NULL;
	
	sprintf(name,"Line%03ld%c",l->ObjectIDCount++,0);
	
    b=(uLinePtr)ObjectCreate(name,LineType,r,sizeof(struct uLine));
    if(!b)return NULL;	
    
	if(uLineSet(b))goto ErrorOut;
		
	b->p1.x=r->x;
	b->p1.y=r->y;
	b->p2.x=r->xsize;
	b->p2.y=r->ysize;

	b->l = l;
	b->Attributes = *Attribute;
	return b;
ErrorOut:
	uLineDelete(b);
	return NULL;
}
static int uLineDraw(DObjPtr o,DOListPtr d)
{
	double xsize2,ysize2;
	double x,y;
	uLinePtr b=(uLinePtr)o;

	if(!b || !d)return 1;


 	xsize2=PixelToLocal((long)(0.5*(b->Attributes.xLineWidth)),d);
 	ysize2=PixelToLocal((long)(0.5*(b->Attributes.yLineWidth)),d);
	
	uSetAttributes(d,&b->Attributes,lPattern | lForeColor | lBackColor | lWidth);

	x=b->p1.x;
	y=b->p1.y;
	rMoveTo(x-xsize2,y+ysize2,d);
	
	x=b->p2.x;
	y=b->p2.y;
	rLineTo(x-xsize2,y+ysize2,d);
	return 0;
}
static int uLineUpdate(DObjPtr o)
{
	uLinePtr b=(uLinePtr)o;

	if(!b)return 1;

	
	uLineUpdateBox(o);
	
	if(b->Selected){
	    ObjectBoxStretch(&b->box,b->l);
	}

	return 0;
}
static int uLineUpdateBox(DObjPtr o)
{
	uLinePtr b=(uLinePtr)o;
	double x,y;
	rPoint r1,r2;
	uPoint p;

	if(!b)return 1;

	x=b->p1.x;
	if(x > b->p2.x)x = b->p2.x;
	y=b->p1.y;
	if(y > b->p2.y)y = b->p2.y;
	
	b->box.x=x;
	b->box.y=y;
	b->box.xsize=fabs(b->p1.x-b->p2.x);
	b->box.ysize=fabs(b->p1.y-b->p2.y);

	p.x=0;
	p.y=0;
	r1=screenToLocal(&p,b->l);

	p.x=2;
	p.y=2;
	r2=screenToLocal(&p,b->l);
	
	if(b->box.xsize < (r2.x-r1.x))b->box.xsize=r2.x-r1.x;

	if(b->box.ysize < (r2.y-r1.y))b->box.ysize=r2.y-r1.y;
	
	return 0;
}
static void uLineDelete(uLinePtr b)
{
	if(!b)return;
	ObjectKill((DObjPtr)b);
}

static int uLineCheckStretch(uPoint pt,struct DObject *Current,DOListPtr l)
{
	uLinePtr b=(uLinePtr)Current;
	rRect box;
	uRect r1;
	uRect r2;
	
	if(!Current || !l)return 0;
	
	box.x=b->p1.x;
	box.y=b->p1.y;
	
    r1=DOSelectRect(0,&box,l);
    
	box.x=b->p2.x;
	box.y=b->p2.y;
	
    r2=DOSelectRect(0,&box,l);
    
    if(uPtInRect(pt,&r1)){
        return 1;
    }else if(uPtInRect(pt,&r2)){    
        return 1;
    }
	    
	return 0;
}
static int drawStretch(uPoint *p2,uPoint *p1,IconPtr myIcon)
{
	uPoint po;
	int start;
	
	if(!myIcon || !p1 || !p2 )return 1;
		
	start=TRUE;
	po = *p1;
	while(uStillDown(myIcon)){
		uPointer(p2,myIcon);
		uDrawAdjustReal(p2,myIcon);
		uSetPenXOR(TRUE,myIcon);
	    if(po.x != p2->x || po.y != p2->y){
	        if(!start){
		        uMoveTo(p1->x,p1->y);
		        uLineTo(po.x,po.y,myIcon);
	        } 
		    po = *p2;
		    uMoveTo(p1->x,p1->y);
		    uLineTo(po.x,po.y,myIcon);
		    
			uSetPenXOR(FALSE,myIcon);
			if(myIcon->uWriteCursor){
				(*myIcon->uWriteCursor)(po,myIcon);
			}
		    
		    start=FALSE;
	    }
	}
	if(!start){
		uSetPenXOR(TRUE,myIcon);
	    uMoveTo(p1->x,p1->y);
	    uLineTo(po.x,po.y,myIcon);
	}

	uSetPenXOR(FALSE,myIcon);
	
	if(start)return 1;

	return 0;
}
static struct DObject *uLineStretch(uPoint pt,struct DObject *o,DOListPtr l)
{
	uLinePtr b=(uLinePtr)o;
	double xOff=0,yOff=0;
	rRect box;
	uPoint p1,p2;
	uPoint p1s,p2s;
	uRect r1;
	uRect r2;
	
	if(!b || !l)return (struct DObject *)NULL;
	
	box.x=b->p1.x;
	box.y=b->p1.y;
	
    r1=DOSelectRect(0,&box,l);
    
	box.x=b->p2.x;
	box.y=b->p2.y;
	
    r2=DOSelectRect(0,&box,l);
    
    if(uPtInRect(pt,&r1)){
    	p1=localToScreen(&b->p1,l);
    	p2=localToScreen(&b->p2,l);
    }else if(uPtInRect(pt,&r2)){    
    	p1=localToScreen(&b->p2,l);
    	p2=localToScreen(&b->p1,l);
    }else{
    	return (struct DObject *)NULL;
    }
       

	p1s=p1;
	p2s=p2;
	
	drawStretch(&p1,&p2,l->myIcon);
	
	if(p1s.x==p1.x && p1s.y==p1.y && p2s.x==p2.x && p2s.y==p2.y)return NULL;

	if(b->Duplicate){
	    b=(uLinePtr)(*b->Duplicate)(o,0,&xOff,&yOff);
	    if(!b)return (struct DObject *)NULL;
	    b->p1= screenToLocal(&p1,l);
	    b->p2= screenToLocal(&p2,l);
	    return (DObjPtr)b;
	}
    
	return (struct DObject *)NULL;
}
static char *uLineCopy(DObjPtr o,long *Length)
{
	uLinePtr b=(uLinePtr)o;
	uLinePtr pBox;
	struct Obj *obj;
	long size;

	if(!b || !Length)return NULL;
	
	*Length=size=sizeof(struct Obj)+sizeof(*pBox);
	if((obj=(struct Obj *)cMalloc(size,8044)) == NULL){
	    return NULL;
	}
	pBox=(uLinePtr)(obj+1);
	obj->type=b->type;
	obj->Length=size;
	*pBox = *b;
	return (char *)obj;
}
uLinePtr uLineLine2(uLinePtr Pastin,DOListPtr l)
{	
	uLinePtr b;
	
	if(!Pastin || !l)return NULL;
	
	if(Pastin->size != sizeof(*b))return NULL;
		
	b=(uLinePtr)ObjectObject2((DObjPtr)Pastin);
	
	b->l=l;
	
	b->p1=Pastin->p1;
	b->p2=Pastin->p2;	
		
	if(uLineSet(b))goto ErrorOut;
	return b;
ErrorOut:
	uLineDelete(b);
	return NULL;
}
int uLineSet(uLinePtr b)
{
	if(!b)return 1;
	
	b->type=LineType;
	b->Draw=uLineDraw;
	b->Update=uLineUpdate;	
	b->CheckStretch=uLineCheckStretch;
	b->Duplicate=uLineDuplicate;
	b->Move=uLineMove;
	b->Stretch=uLineStretch;	
	b->DoInformation=uLineDoInformation;
	b->Copy=uLineCopy;
	b->ReadWrite=uLineWrite;
	b->DoubleClick=uLineDoubleClick;

	return 0;
}
