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

static char *uRoundRectCopy(DObjPtr o,long *Length);
int uRoundRectSet(uRoundRectPtr b);
static void uRoundRectDelete(uRoundRectPtr b);
static int uRoundRectDraw(DObjPtr o,DOListPtr d);
static uRoundRectPtr uRoundRectdoBoxes(uRoundRectPtr b,rRect *r,struct uAttributes *Attributes);
static DObjPtr uRoundRectMove(DObjPtr o,double *dx,double *dy);

static struct DObject *uRoundRectDoubleClick(struct DObject *o,rPoint *r);
static DObjPtr uRoundRectDoInformation(DObjPtr o);


static struct DObject *uRoundRectDoubleClick(struct DObject *o,rPoint *r)
{
	if(!o || !r)return NULL;

	if(r->x < o->box.x || r->y < o->box.y){
		return NULL;
	}
	
	if(r->x > o->box.x+o->box.xsize || r->y > o->box.y+o->box.ysize){
		return NULL;
	}			
	return uRoundRectDoInformation(o);
}
static struct DObject *uRoundRectWrite(struct DObject *o,FILE8 *inOut,struct DOList *l)
{


	static struct uRoundRect b;
	
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
			
		{"DOUB",4031,universal_TypeDouble,(void *)&b.ovalWidth},	
		{"DOUB",4032,universal_TypeDouble,(void *)&b.ovalHeight},	
		{"REAL",4034,universal_TypeInt,(void *)&b.Attributes.hideLines},	
		{"REAL",4035,universal_TypeInt,(void *)&b.Attributes.doAreadFill},	
	};
	
	static struct universalTableStruct Global = {
		"uDraw parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	uRoundRectPtr bp=(uRoundRectPtr)o;
	static rRect r;
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
		bp=uRoundRectRoundRect(&r,&l->Attributes,l);
		if(!bp)goto  ErrorOut;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(struct uRoundRect);
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
	return NULL;
}
uRoundRectPtr uRoundRectRoundRect(rRect *r,struct uAttributes *Attribute,struct DOList *l)
{
	uRoundRectPtr b;
	char name[64];
	
	if(!r || !Attribute || !l)return NULL;
			
	sprintf(name,"uRoundRect%03ld%c",l->ObjectIDCount++,0);
	
    b=(uRoundRectPtr)ObjectCreate(name,RoundRectType,r,sizeof(struct uRoundRect));
    if(!b)return NULL;	
    
	if(uRoundRectSet(b))goto ErrorOut;

	b->box = *r;
	b->Attributes = *Attribute;
	b->l = l;
    b->ovalWidth=20.0/72.0;
    b->ovalHeight=20.0/72.0;
	return b;
ErrorOut:
	uRoundRectDelete(b);
	return NULL;
}
static int uRoundRectDraw(DObjPtr o,DOListPtr d)
{
	uRoundRectPtr b=(uRoundRectPtr)o;

	if(!b || !d)return 1;
	
	uSetAttributes(d,&b->Attributes,sPattern | sForeColor | sBackColor);
	if(!b->Attributes.doAreadFill)rPaintRoundRect(&b->box,b->ovalWidth,b->ovalHeight,d);
	
	uSetAttributes(d,&b->Attributes,lPattern | lForeColor | lBackColor | lWidth);
	if(!b->Attributes.hideLines)rFrameRoundRect(&b->box,b->ovalWidth,b->ovalHeight,d);
	return 0;
	
}
static DObjPtr uRoundRectDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff)
{
	uRoundRectPtr pBox;
	double x,y;
	uRoundRectPtr b=(uRoundRectPtr)o;
	
	if(!b || !xOff || !yOff)return NULL;
	
	if((pBox=uRoundRectdoBoxes(b,&b->box,&b->Attributes)) == NULL){
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
static uRoundRectPtr uRoundRectdoBoxes(uRoundRectPtr b,rRect *r,struct uAttributes *Attributes)
{
	uRoundRectPtr Box;
	
	if(!b || !r || !Attributes)return NULL;

	if((Box=uRoundRectRoundRect(r,Attributes,b->l)) == NULL){
	    WarningBatch("uRoundRectdoBoxes Out of Memory\n");
	    return NULL;
	}
	
	Box->box = b->box;
	Box->ovalWidth=b->ovalWidth;
	Box->ovalHeight=b->ovalHeight;
	
	return Box;
}
static DObjPtr uRoundRectMove(DObjPtr o,double *dx,double *dy)
{
	uRoundRectPtr pBox;
	uRoundRectPtr b=(uRoundRectPtr)o;
	
	if(!b || !dx || !dy)return NULL;
	
	if((pBox=(uRoundRectPtr)uRoundRectDuplicate((DObjPtr)b,0,dx,dy)) != NULL){
	    pBox->xDup = b->xDup;
	    pBox->yDup = b->yDup;
	}
	return (DObjPtr)pBox;
}
static DObjPtr uRoundRectDoInformation(DObjPtr o)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {240,220,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {335,220,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"0", {115,50,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"10", {115,75,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"1", {115,100,80,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"1", {115,125,80,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"Round Rect", {223,11,80,20},uDialogText,uDialogNone},
      /*  7 */      {"0", {315,50,80,20},uDialogText,uDialogGetDouble},
      /*  8 */      {"0", {315,75,80,20},uDialogText,uDialogGetDouble},
      /*  9 */      {"Left        :", {25,75,70,20},uDialogString,uDialogNone},
      /* 10 */      {"Object Type", {75,9,90,20},uDialogString,uDialogNone},
      /* 11 */      {"Height    :", {25,100,70,20},uDialogString,uDialogNone},
      /* 12 */      {"Width     :", {25,125,70,20},uDialogString,uDialogNone},
      /* 13 */      {"Top         :", {25,50,70,20},uDialogString,uDialogNone},
      /* 14 */      {"ovalWidth  :", {220,50,80,20},uDialogString,uDialogNone},
      /* 15 */      {"ovalHeight :", {220,75,80,20},uDialogString,uDialogNone},
      /* 16 */      {"Draw Lines", {70,165,95,20},uDialogCheck,uDialogGetLong},
      /* 17 */      {"Area Fill ", {70,185,95,20},uDialogCheck,uDialogGetLong},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,410,260};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	uRoundRectPtr pBox;
	uRoundRectPtr b=(uRoundRectPtr)o;
	
	if(!b)return NULL;

	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	pBox=NULL;
	
	uList=uDialogOpen("RoundRect Information",&Rect1,ItemCount);
	
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
	              uDialogSetDouble,(double)b->ovalWidth,
	              	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)b->ovalHeight,
	              	              
                uDialogSetItem,16L,
                uDialogSetLogical,(!b->Attributes.hideLines == 1),
	              
                uDialogSetItem,17L,
                uDialogSetLogical,(!b->Attributes.doAreadFill == 1),
	              
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

		
	if((pBox=uRoundRectdoBoxes(b,&b->box,&b->Attributes)) == NULL){
	    goto ErrorOut;
	}
	pBox->box.x=uR[3].dreturn;
	pBox->box.y=uR[2].dreturn;
	pBox->box.xsize=uR[5].dreturn;
	pBox->box.ysize=uR[4].dreturn;
	pBox->ovalWidth=uR[7].dreturn;
	pBox->ovalHeight=uR[8].dreturn;
	pBox->Attributes.hideLines=!uR[16].lreturn;
	pBox->Attributes.doAreadFill=!uR[17].lreturn;
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	if(ret && pBox){
	    uRoundRectDelete(pBox);
	    pBox=NULL;
	}
	
	return (DObjPtr)pBox;
}
static char *uRoundRectCopy(DObjPtr o,long *Length)
{
	
	uRoundRectPtr b=(uRoundRectPtr)o;
	uRoundRectPtr pBox;
	struct Obj *obj;
	long size;
	

	if(!b || !Length)return NULL;
	
	*Length=size=sizeof(struct Obj)+sizeof(*b);
	if((obj=(struct Obj *)cMalloc(size,8013)) == NULL){
	    return NULL;
	}
	pBox=(uRoundRectPtr)(obj+1);
	obj->type=b->type;
	obj->Length=size;
	*pBox = *b;
	return (char *)obj;
}
uRoundRectPtr uRoundRect2(uRoundRectPtr Pastin,DOListPtr l)
{	
	uRoundRectPtr b;
	
	if(!Pastin || !l)return NULL;
	
	if(Pastin->size != sizeof(*b))return NULL;
		
	b=(uRoundRectPtr)ObjectObject2((DObjPtr)Pastin);
	
	b->l=l;

    b->ovalWidth=Pastin->ovalWidth;
    b->ovalHeight=Pastin->ovalHeight;
		
	if(uRoundRectSet(b))goto ErrorOut;
	return b;
ErrorOut:
	uRoundRectDelete(b);
	return NULL;
}
int uRoundRectSet(uRoundRectPtr b)
{
	if(!b)return 1;
	b->type=RoundRectType;
	b->Draw=uRoundRectDraw;
	b->Duplicate=uRoundRectDuplicate;
	b->Move=uRoundRectMove;
	b->DoInformation=uRoundRectDoInformation;
	b->Copy=uRoundRectCopy;
	b->ReadWrite=uRoundRectWrite;
	b->DoubleClick=uRoundRectDoubleClick;
	
	/*
	b->Update=CBoxUpdate;
	b->Stretch=CBoxStretch;
	*/
	return 0;
}
static void uRoundRectDelete(uRoundRectPtr b)
{
	if(!b)return;
	ObjectKill((DObjPtr)b);
}

