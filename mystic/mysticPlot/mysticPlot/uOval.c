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

int uOvalSet(uOvalPtr b);
static void uOvalDelete(uOvalPtr b);
static int uOvalDraw(DObjPtr o,DOListPtr d);
static uOvalPtr uOvaldoBoxes(uOvalPtr b,rRect *r,struct uAttributes *Attributes);
static DObjPtr uOvalMove(DObjPtr o,double *dx,double *dy);
static char *uOvalCopy(DObjPtr o,long *Length);

static struct DObject *uOvalDoubleClick(struct DObject *o,rPoint *r);
static DObjPtr uOvalDoInformation(DObjPtr o);


static struct DObject *uOvalDoubleClick(struct DObject *o,rPoint *r)
{
	if(!o || !r)return NULL;

	if(r->x < o->box.x || r->y < o->box.y){
		return NULL;
	}
	
	if(r->x > o->box.x+o->box.xsize || r->y > o->box.y+o->box.ysize){
		return NULL;
	}			
	return uOvalDoInformation(o);
}
static struct DObject *uOvalWrite(struct DObject *o,FILE8 *inOut,struct DOList *l)
{


	static struct uOval b;
	
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

	uOvalPtr bp=(uOvalPtr)o;
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
		bp=uOvalOval(&r,&l->Attributes,l);
		if(!bp)goto  ErrorOut;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(struct uOval);
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
uOvalPtr uOvalOval(rRect *r,struct uAttributes *Attribute,struct DOList *l)
{
	uOvalPtr b;
	char name[64];
	
	if(!r || !Attribute || !l)return NULL;
			
	sprintf(name,"Oval%03ld%c",l->ObjectIDCount++,0);
	
    b=(uOvalPtr)ObjectCreate(name,BoxType,r,sizeof(struct uOval));
    if(!b)return NULL;	
    
	if(uOvalSet(b))goto ErrorOut;

	b->box = *r;
	b->Attributes = *Attribute;
	b->l = l;
	return b;
ErrorOut:
	uOvalDelete(b);
	return NULL;
}
static int uOvalDraw(DObjPtr o,DOListPtr d)
{
	uOvalPtr b=(uOvalPtr)o;

	if(!b || !d)return 1;
	
	uSetAttributes(d,&b->Attributes,sPattern | sForeColor | sBackColor);
	if(!b->Attributes.doAreadFill)rPaintOval(&b->box,d);
	
	uSetAttributes(d,&b->Attributes,lPattern | lForeColor | lBackColor | lWidth);
	if(!b->Attributes.hideLines)rFrameOval(&b->box,d);
	return 0;
	
}
static DObjPtr uOvalDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff)
{
	uOvalPtr pBox;
	double x,y;
	uOvalPtr b=(uOvalPtr)o;
	
	if(!b || !xOff || !yOff)return NULL;
	
	if((pBox=uOvaldoBoxes(b,&b->box,&b->Attributes)) == NULL){
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
static uOvalPtr uOvaldoBoxes(uOvalPtr b,rRect *r,struct uAttributes *Attributes)
{
	uOvalPtr Box;
	
	if(!b || !r || !Attributes)return NULL;

	if((Box=uOvalOval(r,Attributes,b->l)) == NULL){
	    WarningBatch("doBoxes Out of Memory\n");
	    return NULL;
	}
	
	Box->box = b->box;
	
	return Box;
}
static DObjPtr uOvalMove(DObjPtr o,double *dx,double *dy)
{
	uOvalPtr pBox;
	uOvalPtr b=(uOvalPtr)o;
	
	if(!b || !dx || !dy)return NULL;
	
	if((pBox=(uOvalPtr)uOvalDuplicate((DObjPtr)b,0,dx,dy)) != NULL){
	    pBox->xDup = b->xDup;
	    pBox->yDup = b->yDup;
	}
	return (DObjPtr)pBox;
}
static DObjPtr uOvalDoInformation(DObjPtr o)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {76,220,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {171,220,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"0", {160,67,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"10", {160,92,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"1", {160,117,80,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"1", {160,142,80,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"Oval", {14,27,295,31},uDialogText,uDialogNone},
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
    
	uOvalPtr pBox;
	uOvalPtr b=(uOvalPtr)o;
	
	if(!b)return NULL;

	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	pBox=NULL;
	
	uList=uDialogOpen("Oval Information",&Rect1,ItemCount);
	
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

		
	if((pBox=uOvaldoBoxes(b,&b->box,&b->Attributes)) == NULL){
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
	    uOvalDelete(pBox);
	    pBox=NULL;
	}
	
	return (DObjPtr)pBox;
}
static char *uOvalCopy(DObjPtr o,long *Length)
{
	
	uOvalPtr pBox;
	struct Obj *obj;
	long size;
	
	uOvalPtr b=(uOvalPtr)o;

	if(!b || !Length)return NULL;
	
	*Length=size=sizeof(struct Obj)+sizeof(*b);
	if((obj=(struct Obj *)cMalloc(size,8013)) == NULL){
	    return NULL;
	}
	pBox=(uOvalPtr)(obj+1);
	obj->type=b->type;
	obj->Length=size;
	*pBox = *b;
	return (char *)obj;
}
int uOvalSet(uOvalPtr b)
{
	if(!b)return 1;
	b->type=OvalType;
	b->Draw=uOvalDraw;
	b->Duplicate=uOvalDuplicate;
	b->Move=uOvalMove;
	b->DoInformation=uOvalDoInformation;
	b->Copy=uOvalCopy;
	b->ReadWrite=uOvalWrite;
	b->DoubleClick=uOvalDoubleClick;
	/*
	b->Update=CBoxUpdate;
	b->Stretch=CBoxStretch;
	*/
	return 0;
}
static void uOvalDelete(uOvalPtr b)
{
	if(!b)return;
	ObjectKill((DObjPtr)b);
}
uOvalPtr uOvalOval2(uOvalPtr Pastin,DOListPtr l)
{	
	uOvalPtr b;
	
	if(!Pastin || !l)return NULL;
	
	if(Pastin->size != sizeof(*b))return NULL;
		
	b=(uOvalPtr)ObjectObject2((DObjPtr)Pastin);
	
	b->l=l;
		
	if(uOvalSet(b))goto ErrorOut;
	return b;
ErrorOut:
	uOvalDelete(b);
	return NULL;
}
