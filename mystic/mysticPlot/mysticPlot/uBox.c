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

static char *uBoxCopy(DObjPtr o,long *Length);
int uBoxSet(uBoxPtr b);
static void uBoxDelete(uBoxPtr b);
static int uBoxDraw(DObjPtr o,DOListPtr d);
static uBoxPtr uBoxdoBoxes(uBoxPtr b,rRect *r,struct uAttributes *Attributes);
static DObjPtr uBoxMove(DObjPtr o,double *dx,double *dy);

static struct DObject *uBoxDoubleClick(struct DObject *o,rPoint *r);
static DObjPtr uBoxDoInformation(DObjPtr o);


static struct DObject *uBoxDoubleClick(struct DObject *o,rPoint *r)
{
	if(!o || !r)return (struct DObject *)NULL;

	if(r->x < o->box.x || r->y < o->box.y){
		return (struct DObject *)NULL;
	}
	
	if(r->x > o->box.x+o->box.xsize || r->y > o->box.y+o->box.ysize){
		return (struct DObject *)NULL;
	}			
	return uBoxDoInformation(o);
}
static struct DObject *uBoxWrite(struct DObject *o,FILE8 *inOut,struct DOList *l)
{


	static struct uBox b;
	
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
		{"REAL",4034,universal_TypeInt,(void *)&b.Attributes.hideLines},	
		{"REAL",4035,universal_TypeInt,(void *)&b.Attributes.doAreadFill},	
			
	};
	
	static struct universalTableStruct Global = {
		"uDraw parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	uBoxPtr bp=(uBoxPtr)o;
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
		bp=uBoxBox(&r,&l->Attributes,l);
		if(!bp)goto  ErrorOut;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(struct uBox);
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

uBoxPtr uBoxBox(rRect *r,struct uAttributes *Attribute,struct DOList *l)
{
	uBoxPtr b;
	char name[64];
	
	if(!r || !Attribute || !l)return NULL;
			
	sprintf(name,"Box%03ld%c",l->ObjectIDCount++,0);
	
    b=(uBoxPtr)ObjectCreate(name,BoxType,r,sizeof(struct uBox));
    if(!b)return NULL;	
    
	if(uBoxSet(b))goto ErrorOut;

	b->box = *r;
	b->Attributes = *Attribute;
	b->l = l;
	return b;
ErrorOut:
	uBoxDelete(b);
	return NULL;
}
static int uBoxDraw(DObjPtr o,DOListPtr d)
{
	uBoxPtr b=(uBoxPtr)o;

	if(!b || !d)return 1;
	
	uSetAttributes(d,&b->Attributes,sPattern | sForeColor | sBackColor);
	if(!b->Attributes.doAreadFill)rPaintRect(&b->box,d);
	
	uSetAttributes(d,&b->Attributes,lPattern | lForeColor | lBackColor | lWidth);
	if(!b->Attributes.hideLines)rFrameRect(&b->box,d);
	return 0;
	
}
static DObjPtr uBoxDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff)
{
	uBoxPtr pBox;
	double x,y;
	uBoxPtr b=(uBoxPtr)o;
	
	if(!b || !xOff || !yOff)return NULL;
	
	if((pBox=uBoxdoBoxes(b,&b->box,&b->Attributes)) == NULL){
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
static uBoxPtr uBoxdoBoxes(uBoxPtr b,rRect *r,struct uAttributes *Attributes)
{
	uBoxPtr Box;
	
	if(!b || !r || !Attributes)return NULL;

	if((Box=uBoxBox(r,Attributes,b->l)) == NULL){
	    WarningBatch("doBoxes Out of Memory\n");
	    return NULL;
	}
	
	Box->box = b->box;
	
	return Box;
}
static DObjPtr uBoxMove(DObjPtr o,double *dx,double *dy)
{
	uBoxPtr pBox;
	uBoxPtr b=(uBoxPtr)o;
	
	if(!b || !dx || !dy)return NULL;
	
	if((pBox=(uBoxPtr)uBoxDuplicate((DObjPtr)b,0,dx,dy)) != NULL){
	    pBox->xDup = b->xDup;
	    pBox->yDup = b->yDup;
	}
	return (DObjPtr)pBox;
}
static DObjPtr uBoxDoInformation(DObjPtr o)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {76,220,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {171,220,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"0", {160,67,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"10", {160,92,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"1", {160,117,80,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"1", {160,142,80,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"Rect", {14,27,295,31},uDialogText,uDialogNone},
      /*  7 */      {"Left        :", {70,92,65,20},uDialogString,uDialogNone},
      /*  8 */      {"Object Type", {95,3,95,20},uDialogString,uDialogNone},
      /*  9 */      {"Height    :", {70,117,65,20},uDialogString,uDialogNone},
      /* 10 */      {"Width     :", {70,142,65,20},uDialogString,uDialogNone},
      /* 11 */      {"Top         :", {70,67,65,20},uDialogString,uDialogNone},
      /* 12 */      {"Draw Lines", {70,165,95,20},uDialogCheck,uDialogGetLong},
      /* 13 */      {"Area Fill ", {70,185,95,20},uDialogCheck,uDialogGetLong},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,325,260};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	uBoxPtr pBox;
	uBoxPtr b=(uBoxPtr)o;
	
	if(!b)return NULL;

	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	pBox=NULL;
	
	uList=uDialogOpen("Box Information",&Rect1,ItemCount);
	
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
	              	              
                uDialogSetItem,12L,
                uDialogSetLogical,((!b->Attributes.hideLines) == 1),
	              
                uDialogSetItem,13L,
                uDialogSetLogical,((!b->Attributes.doAreadFill) == 1),
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

		
	if((pBox=uBoxdoBoxes(b,&b->box,&b->Attributes)) == NULL){
	    goto ErrorOut;
	}
	pBox->box.x=uR[3].dreturn;
	pBox->box.y=uR[2].dreturn;
	pBox->box.xsize=uR[5].dreturn;
	pBox->box.ysize=uR[4].dreturn;
	pBox->Attributes.hideLines=!uR[12].lreturn;
	pBox->Attributes.doAreadFill=!uR[13].lreturn;
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	if(ret && pBox){
	    uBoxDelete(pBox);
	    pBox=NULL;
	}
	
	return (DObjPtr)pBox;
}
static char *uBoxCopy(DObjPtr o,long *Length)
{
	
	uBoxPtr pBox;
	struct Obj *obj;
	long size;
	
	uBoxPtr b=(uBoxPtr)o;

	if(!b || !Length)return NULL;
	
	*Length=size=sizeof(struct Obj)+sizeof(*b);
	if((obj=(struct Obj *)cMalloc(size,8013)) == NULL){
	    return NULL;
	}
	pBox=(uBoxPtr)(obj+1);
	obj->type=b->type;
	obj->Length=size;
	*pBox = *b;
	return (char *)obj;
}
int uBoxSet(uBoxPtr b)
{
	if(!b)return 1;
	b->type=BoxType;
	b->Draw=uBoxDraw;
	b->Duplicate=uBoxDuplicate;
	b->Move=uBoxMove;
	b->DoInformation=uBoxDoInformation;
	b->Copy=uBoxCopy;
	b->ReadWrite=uBoxWrite;
	b->DoubleClick=uBoxDoubleClick;
	
	/*
	b->Update=CBoxUpdate;
	b->Stretch=CBoxStretch;
	*/
	return 0;
}
static void uBoxDelete(uBoxPtr b)
{
	if(!b)return;
	ObjectKill((DObjPtr)b);
}
uBoxPtr uBoxBox2(uBoxPtr Pastin,DOListPtr l)
{	
	uBoxPtr b;
	
	if(!Pastin || !l)return NULL;
	
	if(Pastin->size != sizeof(*b))return NULL;
		
	b=(uBoxPtr)ObjectObject2((DObjPtr)Pastin);
	
	b->l=l;
		
	if(uBoxSet(b))goto ErrorOut;
	return b;
ErrorOut:
	uBoxDelete(b);
	return NULL;
}
