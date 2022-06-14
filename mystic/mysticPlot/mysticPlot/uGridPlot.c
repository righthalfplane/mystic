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

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#define max(a,b) 	((a)>(b)?(a):(b))
#define min(a,b) 	((a)<(b)?(a):(b))
#define sign(a,b) 	((b)>=(0)?(fabs(a)):(-fabs(a)))


int cramin(double x,char *word);

static void GridPlotGetFontSelected(DObjPtr o);
static void GridPlotSetFontSelected(DObjPtr o);

void uDrawlPopUp(IconPtr myIcon,IconPtr myIconDialog,int iflag);
void uDrawsPopUp(IconPtr myIcon,IconPtr myIconDialog,int iflag);

int uInsetRect(uRect *r,int x,int y);
int uLinePat(uRect *r,char *pat,IconPtr myIcon);

static int doScale1Draw(struct uDialogStruct *uList);

int rLinePat(uRect *r,char *pat,DOListPtr d);

uLineDataPtr lineEXT;

int rDrawImageRect(iRect *r,DOListPtr d);

int doFourier(double *x,double *y,long nodes,int Order,int PolynomialPrint);
int doSmooth(double *x,double *y,long n,int Order,int PolynomialPrint);
int doPolynomial(double *x,double *y,long n,int Order,int PolynomialPrint);

int ResizeArray(void *Array,long Length,long Count,long *Maximun,int tag);
int uCStringWidth(char *name,IconPtr myIcon);

struct buffStuff{
   char *buffer;
   long length;
   long lengthMax;
};

int doLineToBuff(char *buff,struct buffStuff *buffer,int flag);


static int GridPlotStretchDraw(struct DObject *o,DOListPtr l,int flag);

int doLineRange(struct linedata *li);
static int doLineCopy(uLineDataPtr nLine,int flag);
static int CPlotrPoint(struct uGridPlot *b,double *x,double *y,double *xr,double *yr);

static char *GridPlotCopy(DObjPtr o,long *Length);
int GridPlotSet(uGridPlotPtr Object);
static int GridPlotSetSelection(struct uGridPlot *b,DOListPtr d);
static struct uAttributes *GridPlotGetAttributes(DObjPtr o);
static void GridPlotSelectOne(DObjPtr o,rPoint *ClickPoint);
static int GridPlotSelectedPoint(DObjPtr o,rPoint *ClickPoint);
static DObjPtr GridPlotCreate(char *name,rRect *box,struct uAttributes *Attributes);
static void GridPlotxGridDraw(struct uGridPlot *b,DOListPtr d,double y,double xMajorIn,double xMajorOut,double xMinorIn,double xMinorOut,double mult,rRect *r);
static void GridPlotyGridDraw(struct uGridPlot *b,DOListPtr d,double x,double yMajorIn,double yMajorOut,double yMinorIn,double yMinorOut,double mult,rRect *r);
static void GridPlotKill(struct DObject *Object);
static int GridPlotDraw(struct DObject *Object,DOListPtr d);
static int GridPlotRange(struct uGridPlot *Plot);
static int GridPlotLineGrid(struct uGridPlot *b,DOListPtr d);
static double GridPlotpow10(int n);
static void GridPlotNeat(double *xmnc,double *xmxc,double *Large,double *Small);
static int CPlotxLog(struct uGridPlot *b,rRect *r,DOListPtr d);
static int CPlotxLin(struct uGridPlot *b,rRect *r,DOListPtr d);
static int CPlotyLin(struct uGridPlot *b,rRect *r,DOListPtr d);
static int CPlotPlotLegends(struct uGridPlot *b,DOListPtr d);
static struct DObject *GridPlotFindObject(struct DObject *Object,rPoint *r);
static struct DObject *GridPlotDoubleClick(struct DObject *Object,rPoint *r);
static struct DObject *GridPlotDoubleClickX(struct uGridPlot *Plot);
static DObjPtr GridPlotMove(DObjPtr o,double *dx,double *dy);
static DObjPtr GridPlotDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff);
static DObjPtr GriddoBoxes(uGridPlotPtr b,rRect *r,struct uAttributes *Attributes);
struct DObject *GridPlotDoubleClickY(struct uGridPlot *Plot);
static int CPlotyLog(struct uGridPlot *b,rRect *r,DOListPtr d);
static struct DObject *GridPlotDoubleClickL(struct uGridPlot *Plot,long n);
static int GridPlotUpdate(struct DObject *o);
static int GridPlotSetFrame(struct DObject *Object,long CurrentFrame);
static int GridPlotFrameCount(struct DObject *Object,long *FrameCount);
static int PlotrPoint(uGridPlotPtr b,double *x,double *y,double *ix,double *iy);
static void GridPlotAttribute(DObjPtr o,struct uAttributes *Attributes,int Flag);


static DObjPtr LineDataCreate(char *name,struct linedata *li,struct uAttributes *Attributes);
static void LineDataKill(struct DObject *Object);
static int LineDataFrameCount(struct DObject *Object,long *FrameCount);
static int LineDataSetFrame(struct DObject *Object,long CurrentFrame);

static int GridPlotMessage(DObjPtr o,long MessageType,void *MessageData);

static int doScale1Action(struct uDialogStruct *uList);

static uGridPlotPtr dPlot;

static long dPlotNLine;

static DObjPtr GridPlotDoInformation(DObjPtr o);

static void CPlotDashedLine(struct uGridPlot *b,double *x,double *y,int nn,char *pat,DOListPtr d,struct uLineData *Line);


static struct DObject *uLineWrite(struct DObject *o,FILE8 *inOut,struct DOList *l);

static int clip2(struct uGridPlot *b,double *x1i,double *y1i,double *x2i,double *y2i,int *i1i);

static struct DObject *uGridPlotWrite(struct DObject *o,FILE8 *inOut,struct DOList *l)
{


	static struct uGridPlot b;
	
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
		

		{"DOUB",4034,universal_TypeDouble,(void *)&b.xAttributes.xLineWidth},	
		{"DOUB",4035,universal_TypeDouble,(void *)&b.xAttributes.yLineWidth},			
		{"REAL",4036,universal_TypeInt,(void *)&b.xAttributes.LinePattern},	
		{"REAL",4037,universal_TypeInt,(void *)&b.xAttributes.LineSymbol},	
		{"REAL",4038,universal_TypeInt,(void *)&b.xAttributes.SolidPattern},	
		{"REAL",4039,universal_TypeInt,(void *)&b.xAttributes.nLineFore},	
		{"REAL",4040,universal_TypeInt,(void *)&b.xAttributes.nLineBack},	
		{"REAL",4041,universal_TypeInt,(void *)&b.xAttributes.nSolidFore},	
		{"REAL",4042,universal_TypeInt,(void *)&b.xAttributes.nSolidBack},
		
				

		{"DOUB",4045,universal_TypeDouble,(void *)&b.yAttributes.xLineWidth},	
		{"DOUB",4046,universal_TypeDouble,(void *)&b.yAttributes.yLineWidth},			
		{"REAL",4047,universal_TypeInt,(void *)&b.yAttributes.LinePattern},	
		{"REAL",4048,universal_TypeInt,(void *)&b.yAttributes.LineSymbol},	
		{"REAL",4049,universal_TypeInt,(void *)&b.yAttributes.SolidPattern},	
		{"REAL",4050,universal_TypeInt,(void *)&b.yAttributes.nLineFore},	
		{"REAL",4051,universal_TypeInt,(void *)&b.yAttributes.nLineBack},	
		{"REAL",4052,universal_TypeInt,(void *)&b.yAttributes.nSolidFore},	
		{"REAL",4053,universal_TypeInt,(void *)&b.yAttributes.nSolidBack},
		
		{"REAL",4054,universal_TypeInt,(void *)&b.yAttributes.font.txFace},
		{"REAL",4055,universal_TypeInt,(void *)&b.yAttributes.font.txFont},
		{"REAL",4056,universal_TypeInt,(void *)&b.yAttributes.font.txSize},
		
		
		{"REAL",4057,universal_TypeInt,(void *)&b.gridHash},
		{"REAL",4058,universal_TypeInt,(void *)&b.boxLegend},

		{"DOUB",4059,universal_TypeDouble,(void *)&b.xBoxOffset.x},	
		{"DOUB",4060,universal_TypeDouble,(void *)&b.xBoxOffset.y},	
		
		{"DOUB",4061,universal_TypeDouble,(void *)&b.yBoxOffset.x},	
		{"DOUB",4062,universal_TypeDouble,(void *)&b.yBoxOffset.y},	
		
		{"DOUB",4063,universal_TypeDouble,(void *)&b.yLegendOffset.x},	
		{"DOUB",4064,universal_TypeDouble,(void *)&b.yLegendOffset.y},	
		

		{"DOUB",4065,universal_TypeDouble,(void *)&b.xMinimum},	
		{"DOUB",4066,universal_TypeDouble,(void *)&b.yMinimum},	
		{"DOUB",4067,universal_TypeDouble,(void *)&b.xMaximum},	
		{"DOUB",4068,universal_TypeDouble,(void *)&b.yMaximum},	
		
		
		
		{"DOUB",4069,universal_TypeDouble,(void *)&b.xMajorIn},	
		{"DOUB",4070,universal_TypeDouble,(void *)&b.xMajorOut},	
		{"DOUB",4071,universal_TypeDouble,(void *)&b.yMajorIn},	
		{"DOUB",4072,universal_TypeDouble,(void *)&b.yMajorOut},	
		{"DOUB",4073,universal_TypeDouble,(void *)&b.rMajorIn},	
		{"DOUB",4074,universal_TypeDouble,(void *)&b.rMajorOut},	
		{"DOUB",4075,universal_TypeDouble,(void *)&b.tMajorIn},	
		{"DOUB",4076,universal_TypeDouble,(void *)&b.tMajorOut},	
		{"DOUB",4077,universal_TypeDouble,(void *)&b.xMinorIn},	
		{"DOUB",4078,universal_TypeDouble,(void *)&b.xMinorOut},	
		{"DOUB",4079,universal_TypeDouble,(void *)&b.yMinorIn},	
		{"DOUB",4080,universal_TypeDouble,(void *)&b.yMinorOut},	
		{"DOUB",4081,universal_TypeDouble,(void *)&b.rMinorIn},	
		{"DOUB",4082,universal_TypeDouble,(void *)&b.rMinorOut},	
		{"DOUB",4083,universal_TypeDouble,(void *)&b.tMinorIn},	
		{"DOUB",4084,universal_TypeDouble,(void *)&b.tMinorOut},	
	

		{"REAL",4085,universal_TypeInt,(void *)&b.lAxis},
		{"REAL",4086,universal_TypeInt,(void *)&b.bAxis},
		{"REAL",4087,universal_TypeInt,(void *)&b.rAxis},
		{"REAL",4088,universal_TypeInt,(void *)&b.tAxis},
		{"REAL",4089,universal_TypeInt,(void *)&b.lAxisNumber},
		{"REAL",4090,universal_TypeInt,(void *)&b.bAxisNumber},
		{"REAL",4091,universal_TypeInt,(void *)&b.rAxisNumber},
		{"REAL",4092,universal_TypeInt,(void *)&b.tAxisNumber},
		{"REAL",4093,universal_TypeInt,(void *)&b.xGrid},
		{"REAL",4094,universal_TypeInt,(void *)&b.yGrid},
		{"REAL",4095,universal_TypeInt,(void *)&b.xLogScale},
		{"REAL",4096,universal_TypeInt,(void *)&b.yLogScale},
		{"REAL",4097,universal_TypeInt,(void *)&b.xAutoMinimum},
		{"REAL",4098,universal_TypeInt,(void *)&b.yAutoMinimum},
		{"REAL",4101,universal_TypeInt,(void *)&b.xAutoMaximum},
		{"REAL",4102,universal_TypeInt,(void *)&b.yAutoMaximum},
		
		{"REAL",4103,universal_TypeInt,(void *)&b.xMajorLimit},
		{"REAL",4104,universal_TypeInt,(void *)&b.yMajorLimit},
		{"REAL",4105,universal_TypeInt,(void *)&b.xMinorLimit},
		{"REAL",4106,universal_TypeInt,(void *)&b.yMinorLimit},		
		
		{"DOUB",5069,universal_TypeDouble,(void *)&b.xSetMinimum},	
		{"DOUB",5070,universal_TypeDouble,(void *)&b.xSetMaximum},	
		{"DOUB",5071,universal_TypeDouble,(void *)&b.ySetMinimum},	
		{"DOUB",5072,universal_TypeDouble,(void *)&b.ySetMaximum},	
		{"DOUB",5073,universal_TypeDouble,(void *)&b.xViewMinLog},	
		{"DOUB",5074,universal_TypeDouble,(void *)&b.yViewMinLog},	
		{"DOUB",5075,universal_TypeDouble,(void *)&b.xScale},	
		{"DOUB",5076,universal_TypeDouble,(void *)&b.yScale},	
		{"DOUB",5077,universal_TypeDouble,(void *)&b.xScaleLog},	
		{"DOUB",5078,universal_TypeDouble,(void *)&b.yScaleLog},	
		{"DOUB",5079,universal_TypeDouble,(void *)&b.xOff},	
		{"DOUB",5080,universal_TypeDouble,(void *)&b.yOff},	
		{"DOUB",5081,universal_TypeDouble,(void *)&b.xMajorStep},	
		{"DOUB",5082,universal_TypeDouble,(void *)&b.xMinorStep},	
		{"DOUB",5083,universal_TypeDouble,(void *)&b.yMajorStep},	
		{"DOUB",5084,universal_TypeDouble,(void *)&b.yMinorStep},	
		{"REAL",5085,universal_TypeLong,(void *)&b.LineCount},	
		
		{"REAL",7043,universal_TypeInt,(void *)&b.xAttributes.font.txFace},
		{"REAL",7044,universal_TypeInt,(void *)&b.xAttributes.font.txFont},
		{"REAL",7045,universal_TypeInt,(void *)&b.xAttributes.font.txSize},
		
			
	};
	
	static struct universalTableStruct Global = {
		"uDraw parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	uGridPlotPtr bp=(uGridPlotPtr)o;
	static int SetDefaults;
	char head[5];
	long length;
	long tag;
	long n;
	
	if(!inOut || !l)return NULL;
	
	if(!SetDefaults){	
		b.xAutoMinimum=1;
		b.yAutoMinimum=1;
		b.xAutoMaximum=1;
		b.yAutoMaximum=1;
		SetDefaults=1;
	}
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp=(uGridPlotPtr)GridPlotCreate(b.name,&b.box,&b.Attributes);
		if(!bp)goto  ErrorOut;
		bp->l=l;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(struct uGridPlot);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;	
		
		if(bp->LineCount > 0){
		   length=bp->LineCount;
		   bp->Lines=NULL;
		   bp->LineCount=0;
		   bp->LineMax=0;
		    for(n=0;n<length;++n){
				DObjPtr Return,Object=DOGetObjectPtr(GridPlotLine,l);
				if(!Object || !Object->ReadWrite)continue;
		        if((Return = (*Object->ReadWrite)(NULL,inOut,l)) == NULL)goto ErrorOut;		        
				if(ResizeArray(&bp->Lines,sizeof(struct uLineData *),bp->LineCount,&bp->LineMax,2219))goto ErrorOut;
				bp->Lines[bp->LineCount++]=(uLineDataPtr)Return;
				Return->l=l;
		    }				   
		}
				
		if(bp->yMajorLimit <= 0)bp->yMajorLimit=25;
		if(bp->yMinorLimit <= 0)bp->yMinorLimit=20;
		if(bp->xMajorLimit <= 0)bp->xMajorLimit=25;
		if(bp->xMinorLimit <= 0)bp->xMinorLimit=20;

		return (struct DObject *)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}		
		if(bp->Lines && (bp->LineCount > 0)){	
			for(n=0;n<bp->LineCount;++n){
				struct uLineData *Line;
				Line=bp->Lines[n];
				if(!Line || !Line->ReadWrite)continue;			    
	        	if((*Line->ReadWrite)((DObjPtr)Line,inOut,l) == NULL)goto ErrorOut;
			}
		}
		
		
			
	}
	
	return (struct DObject *)&b;
ErrorOut:
	return NULL;
}
static DObjPtr GridPlotDoCheck(DObjPtr o,int *flag)
{
    uGridPlotPtr b=(uGridPlotPtr)o;
    
	if(!b || !flag || b->pSelected)return NULL;
	
	 *flag=0;
	if(b->xSelected){
	    *flag=1;
	    return GridPlotDoubleClickX(b);
	}
	
	if(b->ySelected){
	    *flag=1;
	    return GridPlotDoubleClickY(b);
	}
	
	if(b->lSelected && (b->lSelected <= b->LineCount)){
	    *flag=1;
	    return GridPlotDoubleClickL(b,b->lSelected-1);
	}
	
	return NULL;
}
static DObjPtr GridPlotDoInformation(DObjPtr o)
{
	static uDialogGroup Items[] =
	
        {
      
      /*  0 */      {"Ok", {265,160,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {360,160,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"0", {160,50,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"10", {160,75,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"1", {160,100,80,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"1", {160,125,80,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"Plot", {167,10,110,20},uDialogText,uDialogNone},
      /*  7 */      {"Grid Lines", {292,48,106,18},uDialogCheck,uDialogGetLong},
      /*  8 */      {"Legend Box", {293,73,106,18},uDialogCheck,uDialogGetLong},
      /*  9 */      {"Left        :", {70,75,65,20},uDialogString,uDialogNone},
      /* 10 */      {"Object Type", {45,10,95,20},uDialogString,uDialogNone},
      /* 11 */      {"Height    :", {70,100,65,20},uDialogString,uDialogNone},
      /* 12 */      {"Width     :", {70,125,65,20},uDialogString,uDialogNone},
      /* 13 */      {"Top         :", {70,50,65,20},uDialogString,uDialogNone},
      /* 14 */      {"x Axis Label", {293,93,106,18},uDialogCheck,uDialogGetLong},
      /* 15 */      {"y Axis Label", {293,113,106,18},uDialogCheck,uDialogGetLong},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,438,213};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
    DObjPtr ts;
    uGridPlotPtr b=(uGridPlotPtr)o;
	uGridPlotPtr pBox;
	int flag;
	
	if(!b)return NULL;
	
	flag=0;
	ts=GridPlotDoCheck(o,&flag);
	if(flag)return ts;
		            
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	pBox=NULL;
	
	uList=uDialogOpen("Plot Information",&Rect1,ItemCount);
	
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
	              uDialogSetLogical,(int)b->gridHash,
	              	              
	              uDialogSetItem,8L,
	              uDialogSetLogical,(int)b->boxLegend,
	              	              
	              uDialogSetItem,14L,
	              uDialogSetLogical,(int)b->bAxisNumber,
	              	              
	              uDialogSetItem,15L,
	              uDialogSetLogical,(int)b->lAxisNumber,
	              	              
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	if((pBox=(uGridPlotPtr)GriddoBoxes(b,&b->box,&b->Attributes)) == NULL){
	    goto ErrorOut;
	}
	
	pBox->box.x=uR[3].dreturn;
	pBox->box.y=uR[2].dreturn;
	pBox->box.xsize=uR[5].dreturn;
	pBox->box.ysize=uR[4].dreturn;
	pBox->gridHash=(int)uR[7].lreturn;
	pBox->boxLegend=(int)uR[8].lreturn;
	pBox->bAxisNumber=(int)uR[14].lreturn;
	pBox->lAxisNumber=(int)uR[15].lreturn;
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	if(ret && pBox){
	    GridPlotKill((DObjPtr)pBox);
	    pBox=NULL;
	}
	
	return (DObjPtr)pBox;
}
static int GridPlotMessage(DObjPtr o,long MessageType,void *MessageData)
{
	uGridPlotPtr b=(uGridPlotPtr)o;
	struct Message1000 *m;
	struct Message1000 mo;
	int n;
	
	if(!b || !MessageData)return 1;
	
	m=(struct Message1000 *)MessageData;
	
	switch(MessageType){
	case MessageType_GET_FILE_OWNER:
	    return 0;
	case MessageType_SET_FRAME_NUMBER:
	    /* if(!b->Attributes.Animation)return 0; */
		return GridPlotSetFrame(o,m->CurrentFrame);
	case MessageType_GET_FILE_LIST:
	    return 0;
	case MessageType_GET_FRAME_COUNT:
	
		if(b->Lines && (b->LineCount > 0)){	
			for(n=0;n<b->LineCount;++n){
				struct uLineData *Line;
				Line=b->Lines[n];
				if(!Line)continue;
			    if(Line->li.sourceName[0]){
		    		mo.FrameCount=0;
		    		SendMessageByName(Line->li.sourceName,MessageType_GET_FRAME_COUNT,&mo);
				    if(mo.FrameCount > m->FrameCount)m->FrameCount=mo.FrameCount;
			    }
			}
		}	
		break;
	case MessageType_PRINT_FILE_INFORMATION:
		if(b->Lines && (b->LineCount > 0)){	
	    	sprintf(WarningBuff,"  Plot x %g y %g\n%c",b->box.x,b->box.y,0);
	    	WarningBatch(WarningBuff);
			for(n=0;n<b->LineCount;++n){
				struct uLineData *Line;
				Line=b->Lines[n];
				if(!Line)continue;
			    if(Line->li.sourceName[0]){
	    			sprintf(WarningBuff,"    Line %d x1 %g y1 %g x2 %g y2 %g n %ld\n    Souce '%s' pioName '%s' pioIndex %ld  \n%c",
	    			        n,Line->li.x1,Line->li.y1,Line->li.x2,Line->li.y2,Line->dCount,Line->li.sourceName,Line->li.pioName,(long)Line->li.pioIndex,0);
	    			WarningBatch(WarningBuff);
			    }
			}
		}	
		break;
	}	
	
	return 0;
	
}
uGridPlotPtr DOGridPlotCreate(DOListPtr l)
{
	uGridPlotPtr Plot;
	static rRect box={2.0,1.0,6.0,4.0};
	char name[256];
	int ret;
		
	if(!l)return NULL;
	
	ret=1;
	
	sprintf(name,"Grid%03ld%c",l->ObjectIDCount++,0);
	
	Plot=(uGridPlotPtr)GridPlotCreate(name,&box,&l->Attributes);
	if(!Plot)goto ErrorOut;
	
	if(DOListADD(l,(DObjPtr)Plot))goto ErrorOut; 

	ret=0;
ErrorOut:	    
	if(ret){
	    if(Plot){
			GridPlotKill((DObjPtr)Plot);
	        Plot=NULL;
	    }
	}
	return Plot;
}
static void GridPlotSetFontSelected(DObjPtr o)
{
	uGridPlotPtr b=(uGridPlotPtr)o;
	long n;
	
	if(!b || !b->l)return;
	
	if(b->xSelected){
	    uSetFontStuff(&b->xAttributes.font,b->l->myIcon);
	}
	
	if(b->ySelected){
	    uSetFontStuff(&b->yAttributes.font,b->l->myIcon);
	}
	
	
	if(b->pSelected){
	    uSetFontStuff(&b->Attributes.font,b->l->myIcon);
	}
	

	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			struct uLineData *Line;
			Line=b->Lines[n];
			if(!Line)continue;
		    if(Line->Selected)uSetFontStuff(&Line->Attributes.font,b->l->myIcon);
		}
	}
	        
	
}
static void GridPlotGetFontSelected(DObjPtr o)
{
	uGridPlotPtr b=(uGridPlotPtr)o;
	long n;
	
	if(!b || !b->l)return;
	
	if(b->xSelected){
	    uGetFontStuff(&b->xAttributes.font,b->l->myIcon);
	}
	
	if(b->ySelected){
	    uGetFontStuff(&b->yAttributes.font,b->l->myIcon);
	}
	
	
	if(b->pSelected){
	    uGetFontStuff(&b->Attributes.font,b->l->myIcon);
	}
	

	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			struct uLineData *Line;
			Line=b->Lines[n];
			if(!Line)continue;
		    if(Line->Selected)uGetFontStuff(&Line->Attributes.font,b->l->myIcon);
		}
	}
	        
	
}
static void GridPlotAttribute(DObjPtr o,struct uAttributes *Attributes,int Flag)
{
	uGridPlotPtr b=(uGridPlotPtr)o;
	long n;
	
	if(!b || !Attributes)return;
	if(b->pSelected)uPutAttributes(&b->Attributes,Attributes,Flag);
	if(b->xSelected)uPutAttributes(&b->xAttributes,Attributes,Flag);
	if(b->ySelected)uPutAttributes(&b->yAttributes,Attributes,Flag);
	
	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			struct uLineData *Line;
			Line=b->Lines[n];
			if(!Line)continue;
		    if(Line->Selected)uPutAttributes(&Line->Attributes,Attributes,Flag);;
		}
	}
		
}
static int GridPlotSelectedPoint(DObjPtr o,rPoint *r)
{
	uGridPlotPtr b=(uGridPlotPtr)o;
	long n;
	
	if(!b)return 0;
	if(!r){
	    return b->Selected;
	}
		
	if(b->xBoxes && (b->xCount > 0)){
	    for(n=0;n<b->xCount;++n){
			if(r->x < b->xBoxes[n].x || r->y < b->xBoxes[n].y)continue;
			if(r->x > b->xBoxes[n].x+b->xBoxes[n].xsize || r->y > b->xBoxes[n].y+b->xBoxes[n].ysize)continue;	    
			 return b->xSelected;
	    }
	}
	
	if(b->yBoxes && (b->yCount > 0)){
	    for(n=0;n<b->yCount;++n){
			if(r->x < b->yBoxes[n].x || r->y < b->yBoxes[n].y)continue;
			if(r->x > b->yBoxes[n].x+b->yBoxes[n].xsize || r->y > b->yBoxes[n].y+b->yBoxes[n].ysize)continue;	    
			 return b->ySelected;
	    }
	}

	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			struct uLineData *Line;
			Line=b->Lines[n];
			if(!Line)continue;
			if(r->x < Line->box.x || r->y < Line->box.y)continue;
			if(r->x > Line->box.x+Line->box.xsize || r->y > Line->box.y+Line->box.ysize)continue;	    
			return Line->Selected;
		    
		}
	}
	
	return b->pSelected;
}
static void GridPlotSelectOne(DObjPtr o,rPoint *r)
{
	uGridPlotPtr b=(uGridPlotPtr)o;
	struct uLineData *Line;
	struct DOList *l;
	IconPtr myIcon;
	int flag;
	int n;
	
	if(!b)return;
	l=o->l;
	if(!l)return;
	myIcon=l->myIcon;
	if(!myIcon)return;
	
	flag=0;
	
	if(!r){
	    if(b->bAxisNumber){
	    	if(!b->xSelected)flag |= 1;
			b->xSelected=TRUE;
	    }else{
	        b->xSelected=FALSE;
	    }
	    
	    if(b->lAxisNumber){
	    	if(!b->ySelected)flag |= 2;
			b->ySelected=TRUE;
	    }else{
			b->ySelected=FALSE;
	    }
	    
	    if(!b->pSelected)flag |= 4;
	    b->Selected=TRUE;
		b->pSelected=TRUE;
		b->lSelected=FALSE;
		if(b->Lines && (b->LineCount > 0)){	
			for(n=0;n<b->LineCount;++n){
				Line=b->Lines[n];
				if(!Line)continue;
				if(!Line->Selected)flag |= 8;
				Line->Selected=TRUE;
				b->lSelected=n+1;			    
			}
		}
	    goto DrawItHere;
	}
		
	if(b->bAxisNumber && b->xBoxes && (b->xCount > 0)){
	    for(n=0;n<b->xCount;++n){
			if(r->x < b->xBoxes[n].x || r->y < b->xBoxes[n].y)continue;
			if(r->x > b->xBoxes[n].x+b->xBoxes[n].xsize || r->y > b->xBoxes[n].y+b->xBoxes[n].ysize)continue;	    
	    		if(!b->xSelected)flag |= 1;
			 	b->xSelected=TRUE;
	    }
	}else{
		b->xSelected=FALSE;
	}
	
	if(b->lAxisNumber && b->yBoxes && (b->yCount > 0)){
	    for(n=0;n<b->yCount;++n){
			if(r->x < b->yBoxes[n].x || r->y < b->yBoxes[n].y)continue;
			if(r->x > b->yBoxes[n].x+b->yBoxes[n].xsize || r->y > b->yBoxes[n].y+b->yBoxes[n].ysize)continue;	    
	    		if(!b->ySelected)flag |= 2;
			 	b->ySelected=TRUE;
	    }
	}else{
		b->ySelected=FALSE;
	}

	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			Line=b->Lines[n];
			if(!Line)continue;
			if(r->x < Line->box.x || r->y < Line->box.y)continue;
			if(r->x > Line->box.x+Line->box.xsize || r->y > Line->box.y+Line->box.ysize)continue;	    
			if(!Line->Selected)flag |= 8;
			b->lSelected=n+1;			    		    
		}
	}
	
	if(!flag){
		if(r->x >= o->box.x && r->y >= o->box.y && r->x <= o->box.x+o->box.xsize &&
		   r->y <= o->box.y+o->box.ysize){
				if(!b->pSelected)flag |= 4;
				b->pSelected=TRUE;
		}
	}

DrawItHere:

	uSetPenXOR(TRUE,myIcon);	    
	
	if(flag & 1){
	    ObjectBoxFrame(&b->xBoxeSelect,o->l);
	}
	if(flag & 2){
	    ObjectBoxFrame(&b->yBoxeSelect,o->l);
	}
	if(flag & 4){
	    ObjectBoxFrame(&b->box,o->l);
	}
	if(flag & 8){
		if(b->Lines && (b->LineCount > 0)){	
			for(n=0;n<b->LineCount;++n){
				Line=b->Lines[n];
				if(!Line)continue;
				if(r){
					if(r->x < Line->box.x || r->y < Line->box.y)continue;
					if(r->x > Line->box.x+Line->box.xsize || r->y > Line->box.y+Line->box.ysize)continue;
				}	    
				if(Line->Selected)continue;
				Line->Selected=TRUE;
	    		ObjectBoxFrame(&Line->box,o->l);
			}
		}
	}	

	uSetPenXOR(FALSE,myIcon);	 

	if(flag)b->Selected=TRUE;
}
static void GridPlotDeSelectOne(DObjPtr o,rPoint *r)
{
	uGridPlotPtr b=(uGridPlotPtr)o;
	struct uLineData *Line;
	struct DOList *l;
	IconPtr myIcon;
	int flag;
	int n;
	
	if(!b)return;
	l=o->l;
	if(!l)return;
	myIcon=l->myIcon;
	if(!myIcon)return;
	
	flag=0;
	if(!r){
		if(b->xBoxes){
	    	if(b->xSelected)flag |= 1;
	    }
		b->xSelected=FALSE;
		
		if(b->yBoxes){
	    	if(b->ySelected)flag |= 2;
	    }
	    b->Selected=FALSE;
	    
	    if(b->pSelected)flag |= 4;
		b->ySelected=FALSE;
		b->pSelected=FALSE;
		b->lSelected=FALSE;
		if(b->Lines && (b->LineCount > 0)){	
			for(n=0;n<b->LineCount;++n){
				struct uLineData *Line;
				Line=b->Lines[n];
				if(!Line)continue;
				if(Line->Selected)flag |= 8;

			}
		}
	    goto DrawItHere;
	}
		
	if(b->xBoxes && (b->xCount > 0)){
	    for(n=0;n<b->xCount;++n){
			if(r->x < b->xBoxes[n].x || r->y < b->xBoxes[n].y)continue;
			if(r->x > b->xBoxes[n].x+b->xBoxes[n].xsize || r->y > b->xBoxes[n].y+b->xBoxes[n].ysize)continue;	    
	    		if(b->xSelected)flag |= 1;
			 	b->xSelected=FALSE;
	    }
	}else{
		b->xSelected=FALSE;
	}
	
	if(b->yBoxes && (b->yCount > 0)){
	    for(n=0;n<b->yCount;++n){
			if(r->x < b->yBoxes[n].x || r->y < b->yBoxes[n].y)continue;
			if(r->x > b->yBoxes[n].x+b->yBoxes[n].xsize || r->y > b->yBoxes[n].y+b->yBoxes[n].ysize)continue;	    
	    		if(b->ySelected)flag |= 2;
			 	b->ySelected=FALSE;
	    }
	}else{
		b->ySelected=FALSE;
	}

	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			struct uLineData *Line;
			Line=b->Lines[n];
			if(!Line)continue;
			if(r->x < Line->box.x || r->y < Line->box.y)continue;
			if(r->x > Line->box.x+Line->box.xsize || r->y > Line->box.y+Line->box.ysize)continue;	    
			if(Line->Selected)flag |= 8;
		}
	}
	
	if(r->x >= o->box.x && r->y >= o->box.y && r->x <= o->box.x+o->box.xsize &&
	   r->y <= o->box.y+o->box.ysize){
	    	if(b->pSelected)flag |= 4;
		 	b->pSelected=FALSE;
	}
	

DrawItHere:

	uSetPenXOR(TRUE,myIcon);	    
	
	if(flag & 1){
	    ObjectBoxFrame(&b->xBoxeSelect,l);
	}
	if(flag & 2){
	    ObjectBoxFrame(&b->yBoxeSelect,l);
	}
	if(flag & 4){
	    ObjectBoxFrame(&b->box,l);
	}
	if(flag & 8){
		if(b->Lines && (b->LineCount > 0)){	
			for(n=0;n<b->LineCount;++n){
				Line=b->Lines[n];
				if(!Line)continue;
				if(r){
					if(r->x < Line->box.x || r->y < Line->box.y)continue;
					if(r->x > Line->box.x+Line->box.xsize || r->y > Line->box.y+Line->box.ysize)continue;	
				}    
				if(!Line->Selected)continue;
				Line->Selected=FALSE;
	    		ObjectBoxFrame(&Line->box,l);
			}
		}
	}
	
	uSetPenXOR(FALSE,myIcon);	 
	
	if(b->ySelected || b->xSelected || b->pSelected){
	    b->Selected=TRUE;
	}else{
	    b->Selected=FALSE;
	}
	
	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			struct uLineData *Line;
			Line=b->Lines[n];
			if(!Line)continue;
			if(Line->Selected)b->Selected=TRUE;
		    
		}
	}	
}
static struct uAttributes *GridPlotGetAttributes(DObjPtr o)
{
	uGridPlotPtr b=(uGridPlotPtr)o;
	long n;
	
	if(!b)return NULL;
	
	if(b->xSelected){
	    return &b->xAttributes;
	}else if(b->ySelected){
	    return &b->yAttributes;
	}
	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			struct uLineData *Line;
			Line=b->Lines[n];
			if(!Line)continue;
			if(Line->Selected)return &Line->Attributes;
		    
		}
	}
	return &b->Attributes;
}
static int GridPlotSetSelectedRegion(struct DObject *o,DOListPtr l)
{
	uGridPlotPtr b=(uGridPlotPtr)o;
	int n;
	
	if(!b || !l)return 1;
	
	
	if(b->xSelected){
	    for(n=0;n<b->xCount;++n){
			if(ResizeArray(&l->Boxes,sizeof(rRect),l->rCountCount,&l->rCountMax,2010))return 0;			
			l->Boxes[l->rCountCount++]=b->xBoxes[n]; 	    
	    }
	}
	
	if(b->ySelected){
	    for(n=0;n<b->yCount;++n){
			if(ResizeArray(&l->Boxes,sizeof(rRect),l->rCountCount,&l->rCountMax,2011))return 0;			
			l->Boxes[l->rCountCount++]=b->yBoxes[n]; 	    
	    }
	}
	
	if(b->pSelected){
			if(ResizeArray(&l->Boxes,sizeof(rRect),l->rCountCount,&l->rCountMax,2012))return 0;			
			l->Boxes[l->rCountCount++]=b->box; 	    
	}

	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			struct uLineData *Line;
			Line=b->Lines[n];
			if(!Line)continue;
			if(Line->Selected){
				if(ResizeArray(&l->Boxes,sizeof(rRect),l->rCountCount,&l->rCountMax,2014))return 0;			
				l->Boxes[l->rCountCount++]=Line->box; 	    						
			}
		    
		}
	}

	return 0;
	
}
int ObjectBoxFrame(rRect *box,DOListPtr l)
{
	uRect r;
	int n;

    if(!box || !l)return 1;
    
    for(n=0;n<4;++n){
	    r= DOSelectRect(n,box,l);
		uFrameRect(&r,l->myIcon);
		r.x += 1;
		r.y += 1;
		r.xsize -= 2;
		r.ysize -= 2;
		uFrameRect(&r,l->myIcon);
    }
    
	return 0;
}
int ObjectBoxStretch(rRect *box,DOListPtr l)
{
	uRect r;
	int n;

    if(!box || !l)return 1;
    
    for(n=0;n<4;++n){
	    r= DOSelectRect(n,box,l);
		DOSaveStretch(&r,l);
    }
    
	return 0;
}
static int GridPlotDrawSelected(struct DObject *o,DOListPtr l)
{

	return GridPlotStretchDraw(o,l,1);
	
}
static int GridPlotStretchDraw(struct DObject *o,DOListPtr l,int flag)
{
	uGridPlotPtr b=(uGridPlotPtr)o;
	int n;
	
	if(!b || !l)return 1;
	
	
	if(b->xSelected){
	    if(flag){
	    	ObjectBoxFrame(&b->xBoxeSelect,l);
	    }else{
	    	ObjectBoxStretch(&b->xBoxeSelect,l);
	    }
	}
	
	if(b->ySelected){
	    if(flag){
	    	ObjectBoxFrame(&b->yBoxeSelect,l);
	    }else{
	    	ObjectBoxStretch(&b->yBoxeSelect,l);
	    }
	}
	
	if(b->pSelected){
	    if(flag){
	    	ObjectBoxFrame(&b->box,l);
	    }else{
	    	ObjectBoxStretch(&b->box,l);
	    }
	}

	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			struct uLineData *Line;
			Line=b->Lines[n];
			if(!Line)continue;
			if(Line->Selected){
			    if(flag){
			    	ObjectBoxFrame(&Line->box,l);
			    }else{
			    	ObjectBoxStretch(&Line->box,l);
			    }
			}
		    
		}
	}

	return 0;
	
}
static DObjPtr GridPlotKillSelected(DObjPtr o)
{
	uGridPlotPtr b=(uGridPlotPtr)o;
	uGridPlotPtr Plot;
	double dx=0;
	double dy=0;

	if(!b)return NULL;
	if(b->pSelected)return NULL;
	if((Plot=(uGridPlotPtr)GridPlotMove((DObjPtr)b,&dx,&dy)) == NULL)return NULL;
	if(b->bAxisNumber && b->xSelected){
	    Plot->bAxisNumber = FALSE;
	    Plot->xSelected = FALSE;
	}
	if(b->lAxisNumber && b->ySelected){
	    Plot->lAxisNumber = FALSE;
	    Plot->ySelected = FALSE;
	}
	return (DObjPtr)Plot;
}

static struct DObject *GridPlotCreate(char *name,rRect *box,struct uAttributes *Attributes)
{
	uGridPlotPtr b;
	struct DObject *Object;
	
	
	if(!name || !box || !Attributes)return NULL;
	
    Object=ObjectCreate(name,GridPlotType,box,sizeof(struct uGridPlot));
    if(!Object)return NULL;    
    
    Object->Attributes= *Attributes;    
		
    b=(uGridPlotPtr)Object;
	        
	uGridPlotInit(b,Attributes);
		   	
	return Object;
}
int uGridPlotInit(uGridPlotPtr b,struct uAttributes *Attributes)
{
	if(!b || !Attributes)return 1;

	b->xMinimum=0;
	b->yMinimum=0;
	b->xMaximum=1;
	b->yMaximum=1;

	b->xSetMinimum=0;
	b->ySetMinimum=0;
	b->xSetMaximum=1;
	b->ySetMaximum=1;

	b->rMajorOut=b->tMajorOut=b->yMajorOut=b->xMajorOut=.1;
	b->rMinorOut=b->tMinorOut=b->yMinorOut=b->xMinorOut=.05;
	b->xAutoMinimum=b->yAutoMinimum=TRUE;
	b->xAutoMaximum=b->yAutoMaximum=TRUE;
	

	b->xMajorLimit=25;
	b->yMajorLimit=25;
	
	b->xMinorLimit=20;
	b->yMinorLimit=20;
	
	b->rAxis=b->tAxis=TRUE;
	b->lAxis=b->bAxis=TRUE;
	b->rAxisNumber=b->tAxisNumber=TRUE;
	b->lAxisNumber=b->bAxisNumber=TRUE;
	b->xLogScale=b->yLogScale=FALSE;		     
	b->xMajorIn=b->yMajorIn=b->rMajorIn=b->tMajorIn=0;
	b->xMinorIn=b->yMinorIn=b->rMinorIn=b->tMinorIn=0;
	b->xAttributes= *Attributes;
	b->yAttributes= *Attributes;
	
 	b->pSelected=TRUE;
	b->xSelected=TRUE;
	b->ySelected=TRUE;
	b->Small=1.0e-6;

	GridPlotSet(b);
	
	return 0;
}

uGridPlotPtr uGridPlot2(uGridPlotPtr Pastin,DOListPtr l)
{
	uGridPlotPtr b;
	char *p;
	long n;
	
	if(!Pastin || !l)return NULL;
	
	if(Pastin->size != sizeof(*b))return NULL;
		
	b=(uGridPlotPtr)ObjectObject2((DObjPtr)Pastin);
	
	*b = *Pastin;
	
	b->Lines = NULL;
	b->LineCount = 0;
	b->LineMax = 0;

	
	b->xBoxes = NULL;
	b->xCount = 0;
	b->yBoxes = NULL;
	b->yCount = 0;
	
	if(ObjectSet((DObjPtr)b))goto ErrorOut;		
	
	if(GridPlotSet((uGridPlotPtr)b))goto ErrorOut;
	
	sprintf(b->name,"Grid%03ld%c",l->ObjectIDCount++,0);

	b->l=l;

	p=(char *)Pastin;
	p += sizeof(*b);
		
    for(n=0;n<Pastin->LineCount;++n){
		struct uLineData *Line,*Lineo;
		Line=(struct uLineData *)p;
	    p += sizeof(struct uLineData);
		if(!Line)continue;
		if(ResizeArray(&b->Lines,sizeof(struct uLineData *),b->LineCount,&b->LineMax,7117))break;
		Lineo=(struct uLineData *)LineDataCreate(Line->name,&Line->li,&Line->Attributes);
		if(!Lineo)goto ErrorOut;
		mstrncpy(Lineo->save,Line->save,64);
    	if((Line->dCount > 0) && Line->xData  && Line->yData){
	        double *xD,*yD;
	        	long count;
	        	
	        count=Line->dCount;
           
		    if(!(xD=(double *)cMalloc((long)sizeof(double)*(count),7163))){
		        WarningBatch("uGridPlot2 Out of List Memory\n");
		        goto ErrorOut;
		    }
		    if(!(yD=(double *)cMalloc((long)sizeof(double)*(count),7164))){
		        WarningBatch("uGridPlot2 Out of List Memory\n");
		        goto ErrorOut;
		    }
		    
			memcpy(xD,p,(long)sizeof(double)*(count));
	        p += sizeof(double)*count;
			
			memcpy(yD,p,(long)sizeof(double)*(count));
	        p += sizeof(double)*count;

            Lineo->dCount=count;
            Lineo->xData=xD;
            Lineo->yData=yD;
            
            Lineo->Attributes=Line->Attributes;
            Lineo->Selected=Line->Selected;
            Lineo->l=b->l;

            Lineo->xShift=Line->xShift;
            Lineo->yShift=Line->yShift;
            Lineo->xScale=Line->xScale;
            Lineo->yScale=Line->yScale;

    	}	            
		
		
		b->Lines[b->LineCount++]=Lineo;    
    }
		
	return b;
ErrorOut:
	GridPlotKill((DObjPtr)b);
	return NULL;
}

static char *GridPlotCopy(DObjPtr o,long *Length)
{
	uGridPlotPtr pBox;
	struct Obj *obj;
	char *p;
	long size,istart,len;
	uGridPlotPtr b=(uGridPlotPtr)o;
	int n;
	
	if(!b || !Length)return NULL;
	
	istart=sizeof(struct Obj)+sizeof(struct uGridPlot);
	len=0;
	if(b->LineCount && b->Lines){
		long count;
				
	    len += sizeof(struct uLineData)*b->LineCount;
	
	    for(n=0;n<b->LineCount;++n){
	        if(b->Lines[n]->dCount){
	        	
	        	count=b->Lines[n]->dCount;
	        	
	        	if(count && b->Lines[n]->xData && b->Lines[n]->yData){
	            	len += 2*sizeof(double)*count;
		            
	        	}	            
	        }
	    }
	
	}
	*Length=size=istart+len;
	if((obj=(struct Obj *)cMalloc(size,8061)) == NULL){
	    return NULL;
	}
	pBox=(uGridPlotPtr)(obj+1);
	obj->type=b->type;
	obj->Length=size;
	*pBox = *b;
	p=(char *)obj;
	p += istart;
	
	if(b->LineCount && b->Lines){
		long count;
		
		count=b->LineCount;
		
	
	    for(n=0;n<b->LineCount;++n){
	    	memcpy(p,b->Lines[n],(long)sizeof(struct uLineData));
	    	p += sizeof(struct uLineData);
	        if(b->Lines[n]->dCount){
	        	
	        	count=b->Lines[n]->dCount;
	        	
	        	if(count && b->Lines[n]->xData && b->Lines[n]->yData){	        	
	        		memcpy(p,b->Lines[n]->xData,(long)sizeof(double)*count);
	            	p += sizeof(double)*count;
					memcpy(p,b->Lines[n]->yData,(long)sizeof(double)*count);
	            	p += sizeof(double)*count;
		            
	        	}	            
	        }
	    }
	
	}
	
	return (char *)obj;
}
int GridPlotSet(uGridPlotPtr Object)
{
	if(!Object)return 1;
	
    Object->Kill=GridPlotKill;
    
    Object->Draw=GridPlotDraw;
    
    Object->FindObject=GridPlotFindObject;
    
    Object->DoubleClick=GridPlotDoubleClick;
    
    Object->Update=GridPlotUpdate;
    
    Object->SetFrame=GridPlotSetFrame;
    
    Object->GetFrameCount=GridPlotFrameCount;
    
    Object->Attribute=GridPlotAttribute;
    
	Object->SelectedPoint=GridPlotSelectedPoint;

  	Object->SelectOne=GridPlotSelectOne;
  	
	Object->DeSelectOne=GridPlotDeSelectOne;
  
	Object->Duplicate=GridPlotDuplicate;
	
	Object->GetAttributes=GridPlotGetAttributes;

	Object->SetSelectedRegion=GridPlotSetSelectedRegion;
	
	Object->Message=GridPlotMessage;	
	
	Object->DrawSelected=GridPlotDrawSelected;
	
	Object->Move=GridPlotMove;
	
	Object->KillSelected=GridPlotKillSelected;
	
	Object->DoInformation=GridPlotDoInformation;	
	
	Object->GetFontSelected=GridPlotGetFontSelected;	
	
	Object->SetFontSelected=GridPlotSetFontSelected;	
	
	Object->Copy=GridPlotCopy;
	
	Object->ReadWrite=uGridPlotWrite;
		
	return 0;
}
static int GridPlotUpdate(struct DObject *o)
{
	uGridPlotPtr b=(uGridPlotPtr)o;
	struct DOList *l;
	int n;
	
	if(!b || !b->l)return 1;
	l=b->l;

	if(b->Lines && (b->LineCount > 0)){	
	    if(b->LineCount > l->lineAttributesCount){
	        l->lineAttributesCount=0;
			for(n=0;n<b->LineCount;++n){				
				if(ResizeArray(&l->lAttributes,sizeof(struct uAttributes),l->lineAttributesCount,&l->lineAttributesMax,2115))return 0;	
				l->lAttributes[l->lineAttributesCount++]= (b->Lines[n]->Attributes);	
			}
		}else{
			for(n=0;n<b->LineCount;++n){				
				l->lAttributes[n]= (b->Lines[n]->Attributes);	
			}
		}
	}
	
	return GridPlotStretchDraw(o,l,0);
	
}
static int GridPlotFrameCount(struct DObject *Object,long *FrameCount)
{
	struct uGridPlot *b=(struct uGridPlot *)Object;
	int n;
	
	if(!b || !FrameCount)return 1;
	
	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			LineDataFrameCount((struct DObject *)b->Lines[n],FrameCount);
		}
	}
	
	return 0;
	
}
static int GridPlotSetFrame(struct DObject *Object,long CurrentFrame)
{
	struct uGridPlot *b=(struct uGridPlot *)Object;
	double xmin,xmax,ymin,ymax,amax;
	int n;
	
	if(!b)return 1;
	
	if(b->Lines && (b->LineCount > 0)){	
	    struct uLineData *Line;
		amax=1e60;
		xmin=amax;
		xmax=-amax;
		ymin=amax;
		ymax=-amax;
		for(n=0;n<b->LineCount;++n){
		    Line=b->Lines[n];
			LineDataSetFrame((struct DObject *)Line,CurrentFrame);
		    if(Line->xmin < xmin)xmin=Line->xmin;
		    if(Line->ymin < ymin)ymin=Line->ymin;
		    if(Line->xmax > xmax)xmax=Line->xmax;
		    if(Line->ymax > ymax)ymax=Line->ymax;
		}
	}else{
		xmin=0;
		xmax=1;
		ymin=0;
		ymax=1;
	}
	b->xMinimum=xmin;
	b->xMaximum=xmax;
	b->yMinimum=ymin;
	b->yMaximum=ymax;
	return 0;
}
static int GridPlotDraw(struct DObject *Object,DOListPtr d)
{
	struct uGridPlot *Plot;
	
	if(!Object || !d)return 1;

	Plot=(struct uGridPlot *)Object;

	if(GridPlotRange(Plot))return 1;
	
	if(GridPlotScale(Plot))return 1;
		
	if(GridPlotPlotGrid(Plot,d))return 1;

	if(GridPlotLineGrid(Plot,d))return 1;
	
	if(CPlotPlotNumbers(Plot,d))return 1;
	
	if(CPlotPlotLegends(Plot,d))return 1;
	
	if(GridPlotSetSelection(Plot,d))return 1;
	
	return 0;
	
}
static int GridPlotRange(struct uGridPlot *Plot)
{
	struct uLineData *Line;
	double xmin,xmax,ymin,ymax,amax,yminPlus,xminPlus;
	double xx,yy;
	long n,k;
	
	if(!Plot)return 1;
	
	amax=1e60;
	if(Plot->Lines && (Plot->LineCount > 0)){	
		xmin=amax;
		xmax=-amax;
		ymin=amax;
		ymax=-amax;
		yminPlus=amax;
		xminPlus=amax;
		for(n=0;n<Plot->LineCount;++n){
		    Line=Plot->Lines[n];
		    if(!Line || !Line->xData || !Line->yData)continue;
		    for(k=0;k<Line->dCount;++k){
		        xx=Line->xShift+Line->xScale*Line->xData[k];
		        yy=Line->yShift+Line->yScale*Line->yData[k];
			    if(!Plot->xAutoMinimum && (xx < Plot->xSetMinimum))continue;
			    if(!Plot->yAutoMinimum && (yy < Plot->ySetMinimum))continue;
			    if(!Plot->xAutoMaximum && (xx > Plot->xSetMaximum))continue;
			    if(!Plot->yAutoMaximum && (yy > Plot->ySetMaximum))continue;
			    if(xx < xmin)xmin=xx;
			    if(yy < ymin)ymin=yy;
			    if(xx > xmax)xmax=xx;
			    if(yy > ymax)ymax=yy;
			    if(xx > 0 && xx < xminPlus)xminPlus=xx;
			    if(yy > 0 && yy < yminPlus)yminPlus=yy;
		    }
		    if(Line->li.integrate){
		        sprintf(WarningBuff,"Current Frame : %ld Line Number : %ld  Line Name : %s Line Integral : %g\n",
		          Line->li.CurrentFrame,n+1,Line->save,Line->li.integral);
		        WarningBatch(WarningBuff);
		    }
		}
	}else{
		xmin=0;
		xmax=1;
		ymin=0;
		ymax=1;
		yminPlus=.1;
		xminPlus=.1;
	}
	
	if(Plot->xAutoMinimum){
		if(xmin <= xmax)Plot->xMinimum=xmin;
	}else{
		Plot->xMinimum=Plot->xSetMinimum;
	}
	
	
	if(Plot->xAutoMaximum){
		if(xmin <= xmax)Plot->xMaximum=xmax;
	}else{
		Plot->xMaximum=Plot->xSetMaximum;
	}
	
	if(Plot->xLogScale){	
		if(Plot->xMaximum <= 0){
			Plot->xMaximum=1.0;
		}		
		if(Plot->xMinimum <= 0){
		    if(xminPlus < amax){
				Plot->xMinimum=xminPlus;
			}else{
				Plot->xMinimum=0.09*Plot->xMaximum;
			}
		}
		if(Plot->xMinimum >= 0.09*Plot->xMaximum){
			Plot->xMinimum=0.09*Plot->xMaximum;
		}
		Plot->xMinimum *= 0.9;
		Plot->xMaximum *= 1.1;
	}	
	
	if(Plot->yAutoMinimum){
		if(ymin <= ymax)Plot->yMinimum=ymin;
	}else{
		Plot->yMinimum=Plot->ySetMinimum;
	}
	
	if(Plot->yAutoMaximum){
		if(ymin <= ymax)Plot->yMaximum=ymax;
	}else{
		Plot->yMaximum=Plot->ySetMaximum;
	}
	
	if(Plot->yLogScale){	
		if(Plot->yMaximum <= 0){
			Plot->yMaximum=1.0;
		}		
		if(Plot->yMinimum <= 0){
		    if(yminPlus < amax){
				Plot->yMinimum=yminPlus;
			}else{
				Plot->yMinimum=0.09*Plot->yMaximum;
			}
		}
				
		if(Plot->yMinimum >= 0.09*Plot->yMaximum){
			Plot->yMinimum=0.09*Plot->yMaximum;
		}
		Plot->yMinimum *= 0.9;
		Plot->yMaximum *= 1.1;
	}	
	
	return 0;
}
int GridPlotScale(struct uGridPlot *b)
{
	double xmax,xmin,ymax,ymin;
	double xLarge,xSmall,yLarge,ySmall;
	rRect r;
	
	
	if(!b)return 1;
	if(b->xAutoMinimum || b->xAutoMaximum || (b->xSetMinimum >= b->xSetMaximum)){
	    xmin=b->xMinimum;
	    xmax=b->xMaximum;
	    if(xmax <= xmin){
			xmin=xmax-1.0-0.001*fabs(xmax);
			xmax=xmax+1.0+0.001*fabs(xmax);
	    }
	    if(!b->xLogScale){
			GridPlotNeat(&xmin,&xmax,&xLarge,&xSmall);
			b->xMajorStep=xLarge;
			b->xMinorStep=xSmall;
	    }
	    b->xViewMin=xmin;
	    b->xViewMax=xmax;
	}else{
	    b->xViewMin=b->xSetMinimum;
	    b->xViewMax=b->xSetMaximum;
	}

	if(b->yAutoMinimum || b->yAutoMaximum || (b->ySetMinimum >= b->ySetMaximum)){
	    ymin=b->yMinimum;
	    ymax=b->yMaximum;
	    if(ymax <= ymin){
			ymin=ymax-1.0-0.001*fabs(ymax);
			ymax=ymax+1.0+0.001*fabs(ymax);
	    }
	    if(!b->yLogScale){
			GridPlotNeat(&ymin,&ymax,&yLarge,&ySmall);
			b->yMajorStep=yLarge;
			b->yMinorStep=ySmall;
	    }
	    b->yViewMin=ymin;
	    b->yViewMax=ymax;
	}else{
	    b->yViewMin=b->ySetMinimum;
	    b->yViewMax=b->ySetMaximum;
	}
	r= b->box;
	
	b->xOff=r.x;
	b->yOff=r.y+r.ysize;

	b->xScale=(r.xsize)/(b->xViewMax-b->xViewMin);
	b->yScale=(r.ysize)/(b->yViewMin-b->yViewMax);
	if(b->xLogScale){
	    b->xViewMinLog=log10(b->xViewMin);
	    b->xScaleLog=(r.xsize)/(log10(b->xViewMax)-b->xViewMinLog);
	}
	if(b->yLogScale){
	    b->yViewMinLog=log10(b->yViewMin);
	    b->yScaleLog=(r.ysize)/(b->yViewMinLog-log10(b->yViewMax));
	}
	
	b->dxLim=1e-10*(b->xViewMax-b->xViewMin);
	b->dyLim=1e-10*(b->yViewMax-b->yViewMin);
	
	return 0;
}
static int GridPlotSetSelection(struct uGridPlot *b,DOListPtr d)
{
	double rmax,dr;
	rRect r;
	int n;
	
	if(!b || !d)return 1;
	
	if(b->xBoxes && (b->xCount > 0)){
	    r=b->xBoxes[0];
	    for(n=1;n<b->xCount;++n){
	        dr=r.x-b->xBoxes[n].x;
			if(dr > 0){
			    r.x=b->xBoxes[n].x;
			    r.xsize += dr;
			}
			rmax=b->xBoxes[n].x + b->xBoxes[n].xsize;
			if(rmax > (r.x + r.xsize))r.xsize=rmax-r.x;
			
	        dr=r.y-b->xBoxes[n].y;
			if(dr > 0){
			    r.y=b->xBoxes[n].y;
			    r.ysize += dr;
			}
			rmax=b->xBoxes[n].y + b->xBoxes[n].ysize;
			if(rmax > (r.y + r.ysize))r.ysize=rmax-r.y;
			
	    }
	    b->xBoxeSelect=r;
	}
	
	if(b->yBoxes && (b->yCount > 0)){
	    r=b->yBoxes[0];
	    for(n=1;n<b->yCount;++n){
	        dr=r.x-b->yBoxes[n].x;
			if(dr > 0){
			    r.x=b->yBoxes[n].x;
			    r.xsize += dr;
			}
			rmax=b->yBoxes[n].x + b->yBoxes[n].xsize;
			if(rmax > (r.x + r.xsize))r.xsize=rmax-r.x;
			
	        dr=r.y-b->yBoxes[n].y;
			if(dr > 0){
			    r.y=b->yBoxes[n].y;
			    r.ysize += dr;
			}
			
			
			
			rmax=b->yBoxes[n].y + b->yBoxes[n].ysize;
			if(rmax > (r.y + r.ysize))r.ysize=rmax-r.y;
	    }
	    b->yBoxeSelect=r;
	}
	
	return 0;
}

static struct DObject *GridPlotFindObject(struct DObject *Object,rPoint *r)
{
	struct uGridPlot *b=(struct uGridPlot *)Object;
	int n;
	
	if(!b || !r)return NULL;

	
	if(b->xBoxes && (b->xCount > 0)){
	    for(n=0;n<b->xCount;++n){
			if(r->x < b->xBoxes[n].x || r->y < b->xBoxes[n].y)continue;
			if(r->x > b->xBoxes[n].x+b->xBoxes[n].xsize || r->y > b->xBoxes[n].y+b->xBoxes[n].ysize)continue;	    
			return Object;
	    }
	}
	
	if(b->yBoxes && (b->yCount > 0)){
	    for(n=0;n<b->yCount;++n){
			if(r->x < b->yBoxes[n].x || r->y < b->yBoxes[n].y)continue;
			if(r->x > b->yBoxes[n].x+b->yBoxes[n].xsize || r->y > b->yBoxes[n].y+b->yBoxes[n].ysize)continue;	    
			return Object;
	    }
	}

	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			struct uLineData *Line;
			Line=b->Lines[n];
			if(!Line)continue;
			if(r->x < Line->box.x || r->y < Line->box.y)continue;
			if(r->x > Line->box.x+Line->box.xsize || r->y > Line->box.y+Line->box.ysize)continue;	    
			return Object;
		    
		}
	}
	if(r->x < Object->box.x || r->y < Object->box.y)return NULL;
	if(r->x > Object->box.x+Object->box.xsize || r->y > Object->box.y+Object->box.ysize)return NULL;
	
	return Object;
}
static struct DObject *GridPlotDoubleClick(struct DObject *Object,rPoint *r)
{
	struct uGridPlot *b=(struct uGridPlot *)Object;
	int n;
	
	
	if(!b || !r)return NULL;
		
	if(b->xBoxes && (b->xCount > 0)){
	    for(n=0;n<b->xCount;++n){
			if(r->x < b->xBoxes[n].x || r->y < b->xBoxes[n].y)continue;
			if(r->x > b->xBoxes[n].x+b->xBoxes[n].xsize || r->y > b->xBoxes[n].y+b->xBoxes[n].ysize)continue;	    
			return GridPlotDoubleClickX(b);
	    }
	}
	
	if(b->yBoxes && (b->yCount > 0)){
	    for(n=0;n<b->yCount;++n){
			if(r->x < b->yBoxes[n].x || r->y < b->yBoxes[n].y)continue;
			if(r->x > b->yBoxes[n].x+b->yBoxes[n].xsize || r->y > b->yBoxes[n].y+b->yBoxes[n].ysize)continue;	    
			return GridPlotDoubleClickY(b);
	    }
	}
	
	
	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			struct uLineData *Line;
			Line=b->Lines[n];
			if(!Line)continue;
			if(r->x < Line->box.x || r->y < Line->box.y)continue;
			if(r->x > Line->box.x+Line->box.xsize || r->y > Line->box.y+Line->box.ysize)continue;	    
			return GridPlotDoubleClickL(b,n);
		    
		}
	}

	if(r->x >= b->box.x && r->y >=  b->box.y){
		if(r->x <= b->box.x+b->box.xsize && r->y <= b->box.y+b->box.ysize){
			return GridPlotDoInformation(Object);
		}			
		
	}
	

		
	return NULL;
}

int uLinePat(uRect *r,char *pat,IconPtr myIcon)
{
	int rast[16],ion[16];
	int ix1,ix2,iy;
	int k,i,nCount,np,nLoc,nPart,nFlag;
	
	if(!r || !pat || !myIcon)return 1;
	
	nPart=0;
	for(i=0;i<16;++i){
	  if(pat[i] == '0' || pat[i] == 0)break;	
	  ++nPart;
	  np=pat[i]-'0';
	  rast[i]=np;
	  ion[i]=FALSE;
	  if(np & 1)ion[i]=TRUE;
	}
	
	if(nPart <=0 )return 1;
	
	iy=r->y+(r->ysize)/2;
	ix1=r->x+2;
	ix2=r->x+r->xsize-4;
	nLoc=0;
	nCount=rast[0];
	nFlag=ion[0];
	uMoveTo(ix1,iy);
	for(k=ix1;k<=ix2;++k){
		if(nFlag){
			uLineTo(k,iy,myIcon);
		}
		if(--nCount <= 0){
			if(++nLoc >= nPart)nLoc=0;
			nCount=rast[nLoc];
			if(!nFlag && ion[nLoc])uMoveTo(k,iy);
			nFlag=ion[nLoc];
		}

	}
	return 0;
}
int rLinePat(uRect *r,char *pat,DOListPtr d)
{
	int rast[16],ion[16];
	int ix1,ix2,iy;
	int k,i,nCount,np,nLoc,nPart,nFlag;
	
	if(!r || !pat || !d)return 1;
	
	nPart=0;
	for(i=0;i<16;++i){
	  if(pat[i] == '0' || pat[i] == 0)break;	
	  ++nPart;
	  np=pat[i]-'0';
	  rast[i]=np;
	  ion[i]=FALSE;
	  if(np & 1)ion[i]=TRUE;
	}
	
	if(nPart <=0 )return 1;
	
	iy=r->y+(r->ysize)/2;
	ix1=r->x+2;
	ix2=r->x+r->xsize-4;
	nLoc=0;
	nCount=rast[0];
	nFlag=ion[0];
	uMoveU(ix1,iy,&d->s);
	for(k=ix1;k<=ix2;++k){
		if(nFlag){
			uLineU(k,iy,d->nLineFore,&d->s);
		}
		if(--nCount <= 0){
			if(++nLoc >= nPart)nLoc=0;
			nCount=rast[nLoc];
			if(!nFlag && ion[nLoc])uMoveU(k,iy,&d->s);
			nFlag=ion[nLoc];
		}

	}
	return 0;
}
static int doScale1Draw(struct uDialogStruct *uList)
{
	
	IconPtr myIcon;
	short int Symbol;
	uRect rList1,rList2;
	uRect box;
	iRect image;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIcon;
	if(!myIcon)return 0;	
	if(!lineEXT)return 1;
	if(!lineEXT->l)return 1;
	
	
	
	ret=uDialogSet(uList,0L,
              uDialogSetItem,3L,
              uDialogGetRect,&rList1,
              
              uDialogSetItem,4L,
              uDialogGetRect,&rList2,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret)return 1;
	
	
	box=rList1;

	Symbol=lineEXT->Attributes.LineSymbol;
	
	uEraseRect(&box,myIcon);
	if(Symbol >= 0 && Symbol < 40){
	    image=lineEXT->l->Picts[Symbol];
	    image.box.x=box.x+5;
	    image.box.y=box.y+5;
	    uDrawImageRect(&image,myIcon);	
	}
	uInsetRect(&box,-2,-2);
	uFrameRect(&box,myIcon);
	
	
	box=rList2;
	uEraseRect(&box,myIcon);
	uLinePat(&box,(char *)&lineEXT->Attributes.LineDash,myIcon);
	uInsetRect(&box,-2,-2);
	uFrameRect(&box,myIcon);
	
	
	return 0;
}
static struct DObject *GridPlotDoubleClickL(struct uGridPlot *Plot,long n)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {40,370,62,25},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {131,370,71,26},uDialogButton,uDialogNone},
      /*  2 */      {"d-max", {180,120,70,20},uDialogText,uDialogGetLong},
      /*  3 */      {"userItem",{200,90,22,22},uDialogUser,uDialogNone},
      /*  4 */      {"userItem",{180,55,70,20},uDialogUser,uDialogNone},
      /*  5 */      {"Title Data", {54,15,254,22},uDialogText,uDialogNone},
      /*  6 */      {"Delete", {230,290,65,25},uDialogButton,uDialogNone},
      /*  7 */      {"Add", {40,290,62,25},uDialogButton,uDialogNone},
      /*  8 */      {"Replace", {131,290,71,27},uDialogButton,uDialogNone},
      /*  9 */      {"Symbol Frequency", {40,120,125,17},uDialogString,uDialogNone},
      /* 10 */      {"Symbol ", {40,90,70,20},uDialogString,uDialogNone},
      /* 11 */      {"Line", {40,55,70,20},uDialogString,uDialogNone},
      /* 12 */      {"Title", {5,15,46,18},uDialogString,uDialogNone},
      /* 13 */      {"Read", {40,330,62,25},uDialogButton,uDialogNone},
      /* 14 */      {"Save", {131,330,71,26},uDialogButton,uDialogNone},
      /* 15 */      {"Copy", {230,330,65,25},uDialogButton,uDialogNone},
      /* 16 */      {"Direction", {69,70,169,19},uDialogFloatAux,uDialogFloatAux},
      /* 17 */      {"xShift", {40,155,70,20},uDialogString,uDialogNone},
      /* 18 */      {"xScale", {40,185,70,20},uDialogString,uDialogNone},
      /* 19 */      {"yShift", {40,215,70,20},uDialogString,uDialogNone},
      /* 20 */      {"yScale", {40,245,70,20},uDialogString,uDialogNone},
      /* 21 */      {"d-xShift", {140,155,140,20},uDialogText,uDialogGetDouble},
      /* 22 */      {"d-xScale", {140,185,140,20},uDialogText,uDialogGetDouble},
      /* 23 */      {"d-yShift", {140,215,140,20},uDialogText,uDialogGetDouble},
      /* 24 */      {"d-yScale", {140,245,140,20},uDialogText,uDialogGetDouble},
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {80,55,360,435};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
	double dx=0,dy=0;
	
	
	if(!Plot || !Plot->l)return NULL;
	
	if(lineEXT)return NULL;
	
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	uList=NULL;
	
	if((dPlot=(uGridPlotPtr)GridPlotMove((DObjPtr)Plot,&dx,&dy)) == NULL)goto ErrorOut;
	
	dPlotNLine=n;
	
	if(!dPlot || !dPlot->Lines || (dPlotNLine < 0) || (dPlotNLine >= dPlot->LineCount))goto ErrorOut;
	lineEXT = dPlot->Lines[dPlotNLine];
	
	uList=uDialogOpen("Curve Information",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)lineEXT->Attributes.LineIncrement,
	 	              	             	              
	              uDialogSetUserDraw,doScale1Draw,
  	              	              	              
	              uDialogSetItem,5L,
	              uDialogSetText,lineEXT->save,
	              
	              uDialogSetAction,doScale1Action,
	              
	              uDialogSetItem,16L,
	              uDialogSetFloatAux,(double)0.0,

	              uDialogSetItem,21L,
	              uDialogSetDouble,lineEXT->xShift,

	              uDialogSetItem,22L,
	              uDialogSetDouble,lineEXT->xScale,

	              uDialogSetItem,23L,
	              uDialogSetDouble,lineEXT->yShift,

	              uDialogSetItem,24L,
	              uDialogSetDouble,lineEXT->yScale,

	              uDialogSetMyIcon,Plot->l->myIcon,
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	if(uR[16].dreturn == 0.0){
		mstrncpy(lineEXT->save,uR[5].sreturn,64);
		lineEXT->Attributes.LineIncrement=(int)uR[2].lreturn;
		lineEXT->xShift=uR[21].dreturn;
		lineEXT->xScale=uR[22].dreturn;
		lineEXT->yShift=uR[23].dreturn;
		lineEXT->yScale=uR[24].dreturn;
	}
		
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	if(ret && dPlot){
		GridPlotKill((DObjPtr)dPlot);
		dPlot=NULL;
	}else{
 		/* InvalRectMyWindow(nPlot->myIcon); */
	}
	

	lineEXT=NULL;
 	
	return (DObjPtr)dPlot;
}
static int doScale1Action(struct uDialogStruct *uList)
{
	IconPtr myIcon;
	struct linedata line;
	uLineDataPtr nLine;
	int item;
	int n,nn;
	
	if(!uList)return 1;
	myIcon=FindWindowInList(uList->myIconItem);
	if(!myIcon)return 1;
	if(!uList->myIcon)return 1;

	uSetCursor(NULL,uWATCH_CURSOR);
	
	item=(int)uList->actionItem;
	
	if(item == 3){
	    uDrawsPopUp(myIcon,uList->myIcon,lLine);
	}else if(item == 4){
	    uDrawlPopUp(myIcon,uList->myIcon,lLine);
	}else if(item == 6){
	    if(!dPlot || !dPlot->Lines || (dPlotNLine < 0) || (dPlotNLine >= dPlot->LineCount))return 1;
		nLine= dPlot->Lines[dPlotNLine];
		
		nn=0;
		for(n=0;n<dPlot->LineCount;++n){
		    if(n == dPlotNLine)continue;
		    dPlot->Lines[nn++]=dPlot->Lines[n];
		}
		
		dPlot->LineCount=nn;
		
		if(nn <= 0){
		    if(dPlot->Lines)cFree((char *)dPlot->Lines);
		    dPlot->Lines=NULL;
		}
		
	    LineDataKill((struct DObject *)nLine);
		
	    
	}else if(item == 7){
	    if(!dPlot || !dPlot->Lines || (dPlotNLine < 0) || (dPlotNLine >= dPlot->LineCount))return 1;
		line= dPlot->Lines[dPlotNLine]->li;
			    
	    if(doLineRange(&line))return 1;
	    
	    dPlot->Lines[dPlotNLine]->Selected=FALSE;
	    
	    nLine=DOLineDataCreate(&line,&dPlot->Lines[dPlotNLine]->Attributes);
	    
	    if(!nLine)return 1;
	    
		if(ResizeArray(&dPlot->Lines,sizeof(struct uLineData *),dPlot->LineCount,&dPlot->LineMax,2015))return 1;
		dPlot->Lines[dPlot->LineCount++]=nLine;
		dPlot->lSelected=dPlot->LineCount;
		
		DOLineAttributes(dPlot->l,nLine,dPlot->LineCount-1);
    	    		
	}else if(item == 8){
	    if(!dPlot || !dPlot->Lines || (dPlotNLine < 0) || (dPlotNLine >= dPlot->LineCount))return 1;
		line= dPlot->Lines[dPlotNLine]->li;
			    
	    if(doLineRange(&line))return 1;
	    
	    nLine=DOLineDataCreate(&line,&dPlot->Lines[dPlotNLine]->Attributes);
	    
	    if(!nLine)return 1;
	    
	    LineDataKill((struct DObject *)dPlot->Lines[dPlotNLine]);
	    
	    dPlot->Lines[dPlotNLine]=nLine;
	    		
	}else if(item == 13){
	    if(!dPlot || !dPlot->Lines || (dPlotNLine < 0) || (dPlotNLine >= dPlot->LineCount))return 1;
	    nLine=doLineRead(&dPlot->Lines[dPlotNLine]->Attributes,NULL);
	    
	    if(!nLine)return 1;
	    
		if(ResizeArray(&dPlot->Lines,sizeof(struct uLineData *),dPlot->LineCount,&dPlot->LineMax,2016))return 1;
		dPlot->Lines[dPlot->LineCount++]=nLine;
		
	}else if(item == 14){
	    if(!dPlot || !dPlot->Lines || (dPlotNLine < 0) || (dPlotNLine >= dPlot->LineCount))return 1;
		nLine= dPlot->Lines[dPlotNLine];
				
	    doLineCopy(nLine,1);
	}else if(item == 15){
	    if(!dPlot || !dPlot->Lines || (dPlotNLine < 0) || (dPlotNLine >= dPlot->LineCount))return 1;
		nLine= dPlot->Lines[dPlotNLine];
				
	    doLineCopy(nLine,0);
	}
	
	
		
	if(item ==  6 || item ==  7 || item == 8 || 
	   item == 13 || item == 14 || item == 15  ){
	
		uDialogSet(uList,0L,
	              uDialogSetExitOk,
	              
	              uDialogSetItem,16L,
	              uDialogSetFloatAux,(double)1.0,
	              
	              uDialogSetDone,uDialogSetDone
	    );
    }

	if(uList)uDialogUpdate(uList);
	uSetCursor(NULL,uARROW_CURSOR);
	return 0;
}

uLineDataPtr doLineRead(struct uAttributes *Attributes,char *name)
{
	uLineDataPtr nLine;
	FILE *inout;
	char file[2560];
	double *xD,*yD;
	unsigned char *bp;
	int ret;
	long n,blength;
	long m,xsize,ysize;
	char *fp;
	int c,itwas;
	
	
	if(!Attributes)return NULL;
	
	nLine=NULL;
	
	if(name == NULL){
	    if(GetTheFileName(file,"Read Line data",2560,NULL))return NULL;
	}else{
		mstrncpy(file,name,2560);
	}
	
	ret=1;
	xD=NULL;
	yD=NULL;
	inout=NULL;
			
	blength=100000;
	if(SetBuffers(blength))return NULL;
	if((inout=fopen(file,"rb")) == NULL){
		sprintf(WarningBuff,"Cannot open file : %s to read%c\n",file,0);
		WarningBatch(WarningBuff);
		return NULL;
	}

	itwas = -1;
	xsize=0;
	ysize=0;
	while((m=(long)fread(xg.buffin,1,blength,inout)) > 0){
		bp=(unsigned char *)xg.buffin;
		for(n=0;n<m;++n){
			c = *bp++;
			if((c == '\t') && (ysize == 0))++xsize;
			if(c == '\r')++ysize;
			if((c == '\n') && (itwas != '\r')){
			    ++ysize;
			}
			itwas = c;
		}
	}
	
	if(++xsize != 2){
	   sprintf(WarningBuff,"Error Found %ld Columns in %s\n",xsize,file);
	   WarningBatch(WarningBuff);
	   goto ErrorOut;
	}

	rewind(inout);
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ysize),81871))){
	    WarningBatch("doLineRead Out of Memory\n");
	    goto ErrorOut;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ysize),81881))){
	    WarningBatch("doLineRead Out of Memory\n");
	    goto ErrorOut;
	}
	
	
	for(n=0;n<ysize;++n){
		fscanf(inout,"%lf %lf",&xD[n],&yD[n]);
	}
	
	nLine=DOLineDataCreate(NULL,Attributes);
	    
	if(!nLine)goto ErrorOut;
	
   	nLine->xData=xD;
   	xD=NULL;
	nLine->yData=yD;
	yD=NULL;

	nLine->dCount=ysize;

	fp=strrchr((char *)file,FILE_NAME_SEPERATOR);
    if(fp){
        fp += 1;
    }else{
        fp=file;
    }
	
	mstrncpy(nLine->save,fp,64);

	ret=0;

ErrorOut:
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	if(inout)fclose(inout);
	
	if(ret){
	   if(nLine){
	   	   LineDataKill((struct DObject *)nLine);
	       nLine=NULL;
	   }
	}

   	return nLine;    
}
int cramin(double x,char *word)
{
	long n;
	
	if(!word)return 1;

	if(x <= -1.e7){
		sprintf(word,"%10.3e%c",x,0);
	}else if(fabs(x) <= 1.e-4 || fabs(x) >= 1.e8){
		sprintf(word,"%10.3e%c",x,0);
	}else if(fabs(x) < 0.9999){
		sprintf(word,"%10.8f%c",x,0);
	}else if(fabs(x) < 99.9999){
		sprintf(word,"%10.6f%c",x,0);
	}else if(fabs(x) < 9999.){
		sprintf(word,"%10.4f%c",x,0);
	}else if(fabs(x) < 1.e6){
		sprintf(word,"%10.2f%c",x,0);
	}else{
		sprintf(word,"%10.0f%c",x,0);
	}
	if((n=strlen(word)) > 10){
		sprintf(WarningBuff,"Warning %ld character word %s \n",n,word);
		WarningBatch(WarningBuff);
	}
	
	return 0;
}
static int doLineCopy(uLineDataPtr nLine,int flag)
{
	static uDialogGroup Items[] =
	
        {      
      
      /*  0 */      {"Ok", {200,120,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {305,120,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"\"%4.2f\"", {100,60,80,20},uDialogText,uDialogNone},
      /*  3 */      {"Format 1", {15,60,80,20},uDialogString,uDialogNone},
      /*  4 */      {"\"%4.2f\"", {285,60,80,20},uDialogText,uDialogNone},
      /*  5 */      {"Format 2", {200,60,80,20},uDialogString,uDialogNone},
           
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,377,170};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
	static char Format1[10]={'%','8','.','6','e'};
	static char Format2[10]={'%','8','.','6','e'};

	static char fName[256]={'p','o','i','n','t','s','.','d','a','t',0};
/*	char word1[32],word2[32]; */
	FILE *inout;
	struct buffStuff buffer;
	char buff[256],buff1[256];
	int ret;
	long n;
	
	if(!nLine)return 1;
    if(!nLine->xData || !nLine->yData || (nLine->dCount <= 0))return 1;
    
	uGetPort(&save);
    
    ret=1;
    inout=NULL;
    
    uR=NULL;
    
	uList=uDialogOpen("Data Format",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,		              	              	              	              
	              uDialogSetItem,2L,
	              uDialogSetText,Format1,
	              uDialogSetItem,4L,
	              uDialogSetText,Format2,
	              uDialogSetDone,uDialogSetDone
	              
	    ))goto ErrorOut;
    
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
  	
	mstrncpy(Format1,uR[2].sreturn,255);
	mstrncpy(Format2,uR[4].sreturn,255);
        
    zerol((char *)&buffer,sizeof(struct buffStuff));
    for(n=0;n<nLine->dCount;++n){
/*    
        cramin(nLine->xShift+nLine->xScale*nLine->xData[n],word1);
        cramin(nLine->yShift+nLine->yScale*nLine->yData[n],word2);
        sprintf(buff,"%s\t%s%c",word1,word2,0);
*/        
        zerol((char *)buff,sizeof(buff));
        sprintf(buff,Format1,nLine->xShift+nLine->xScale*nLine->xData[n]);
        sprintf(buff1,Format2,nLine->yShift+nLine->yScale*nLine->yData[n]);
	    mstrncat(buff,"\t",sizeof(buff));
	    mstrncat(buff,buff1,sizeof(buff));
        
        
        if(doLineToBuff(buff,&buffer,flag))goto ErrorOut;
    }
    
    if(flag == 0){
	    ZeroScrap();
	    putStringToScrap(buffer.buffer);
	}else if(flag == 1){
	    if(GetTheSaveFileName(fName,"Save Point Data",256,NULL))goto ErrorOut;
	    if((inout=fopen(fName,"wb")) == NULL){
	        sprintf(WarningBuff,"Cannot open file : %s to write\n",fName);
	        WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    if(putString((unsigned char *)buffer.buffer,buffer.length,inout))goto ErrorOut;
	}

    ret=0;

ErrorOut:
	if(buffer.buffer)cFree((char *)buffer.buffer);
	buffer.buffer = NULL;
	if(inout){
	    fclose(inout);
	    if(SetFileType(fName,"TEXT"))return 1;
	}
	inout = NULL;

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	return ret;
	
}
struct DObject *GridPlotDoubleClickY(struct uGridPlot *Plot)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {335,210,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {415,210,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"d-max", {110,40,105,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"d-min", {110,70,105,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"d-major", {110,100,105,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"d-minor", {110,130,105,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"d-major in", {305,40,40,20},uDialogText,uDialogGetDouble},
      /*  7 */      {"d-major out", {305,70,40,20},uDialogText,uDialogGetDouble},
      /*  8 */      {"d-minor in", {305,100,40,20},uDialogText,uDialogGetDouble},
      /*  9 */      {"d-minor out", {305,130,40,20},uDialogText,uDialogGetDouble},
      /* 10 */      {"d-major in", {445,40,40,20},uDialogText,uDialogGetDouble},
      /* 11 */      {"d-major in", {445,70,40,20},uDialogText,uDialogGetDouble},
      /* 12 */      {"d-major in", {445,100,40,20},uDialogText,uDialogGetDouble},
      /* 13 */      {"d-major in", {445,130,40,20},uDialogText,uDialogGetDouble},
      /* 14 */      {"Linear", {10,210,70,20},uDialogButtonRadio,uDialogGetLong},
      /* 15 */      {"Log", {80,210,60,20},uDialogButtonRadio,uDialogGetLong},
      /* 16 */      {"Auto Min", {140,210,80,21},uDialogCheck,uDialogGetLong},
      /* 17 */      {"Minimum", {30,40,75,20},uDialogString,uDialogNone},
      /* 18 */      {"Maximum", {30,70,75,20},uDialogString,uDialogNone},
      /* 19 */      {"Major Step", {30,100,75,20},uDialogString,uDialogNone},
      /* 20 */      {"Minor Step", {30,130,75,20},uDialogString,uDialogNone},
      /* 21 */      {"Ticks", {334,8,49,20},uDialogString,uDialogNone},
      /* 22 */      {"Major In", {220,40,75,20},uDialogString,uDialogNone},
      /* 23 */      {"Major Out", {220,70,75,20},uDialogString,uDialogNone},
      /* 24 */      {"Minor In", {220,100,75,20},uDialogString,uDialogNone},
      /* 25 */      {"Minor Out", {220,130,75,20},uDialogString,uDialogNone},
      /* 26 */      {"tMajor In", {362,40,75,20},uDialogString,uDialogNone},
      /* 27 */      {"tMajor Out", {361,70,75,20},uDialogString,uDialogNone},
      /* 28 */      {"tMinor In", {361,100,75,20},uDialogString,uDialogNone},
      /* 29 */      {"tMinor Out", {363,131,75,20},uDialogString,uDialogNone},
      /* 30 */      {"Auto Max", {230,210,80,21},uDialogCheck,uDialogGetLong},
      /* 31 */      {"Tick Count Limits", {180,155,150,20},uDialogString,uDialogNone},
      /* 32 */      {"Major", {65,180,75,20},uDialogString,uDialogNone},
      /* 33 */      {"t-major", {110,180,105,20},uDialogText,uDialogGetLong},
      /* 34 */      {"Minor", {230,180,75,20},uDialogString,uDialogNone},
      /* 35 */      {"t-minor", {275,180,105,20},uDialogText,uDialogGetLong},
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {50,50,510,250};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	double flist[15];
	uGridPlotPtr nPlot;
	double dx=0,dy=0;
		
	if(!Plot)return NULL;
	
	if(Plot->yAutoMinimum){
		flist[0]=Plot->yMinimum;
	}else{
		flist[0]=Plot->ySetMinimum;
	}
	
	if(Plot->yAutoMaximum){
		flist[1]=Plot->yMaximum;
	}else{
		flist[1]=Plot->ySetMaximum;
	}
	
	flist[2]=Plot->yMajorStep;
	flist[3]=Plot->yMinorStep;
	
	flist[4]=Plot->yMajorIn;
	flist[5]=Plot->yMajorOut;
	flist[6]=Plot->yMinorIn;
	flist[7]=Plot->yMinorOut;
	
	flist[8]=Plot->rMajorIn;
	flist[9]=Plot->rMajorOut;
	flist[10]=Plot->rMinorIn;
	flist[11]=Plot->rMinorOut;
	
	
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	nPlot=NULL;
	
	uList=uDialogOpen("Y Axis Parameters",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)flist[0],
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)flist[1],
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)flist[2],
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)flist[3],
	              	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,(double)flist[4],
	              	              
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,(double)flist[5],
	              	              
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)flist[6],
	              	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,(double)flist[7],
	              	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,(double)flist[8],
	              	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,(double)flist[9],
	              	              
	              uDialogSetItem,12L,
	              uDialogSetDouble,(double)flist[10],
	              	              
	              uDialogSetItem,13L,
	              uDialogSetDouble,(double)flist[11],
	              	              
	              uDialogSetItem,14L,
	              uDialogSetLogical,(int)(!Plot->yLogScale),
	              	              
	              uDialogSetItem,15L,
	              uDialogSetLogical,(int)(Plot->yLogScale),
	              	              
	              uDialogSetItem,16L,
	              uDialogSetLogical,(int)(Plot->yAutoMinimum),
	              	              
	              uDialogSetItem,30L,
	              uDialogSetLogical,(int)(Plot->yAutoMaximum),
	              	              
	              uDialogSetItem,33L,
	              uDialogSetDouble,(double)(Plot->yMajorLimit),
	              	              
	              uDialogSetItem,35L,
	              uDialogSetDouble,(double)(Plot->yMinorLimit),
	              	              
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    

start:
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
		
	if(!uR[16].lreturn && !uR[30].lreturn && (uR[2].dreturn >= uR[3].dreturn)){
	    Warning("Minimum Must Be Less Than Maximum");
	    goto start;
	}
	if(uR[4].dreturn <= 0 || uR[5].dreturn <= 0){
	    Warning("Steps Must Be Positive");
	    goto start;
	}
	if(uR[15].lreturn && !uR[16].lreturn && !uR[30].lreturn && (uR[2].dreturn <= 0)){
	    Warning("Minimum Must Be Greater Than Zero For Log Plots");
	    goto start;
	}
	
	
	if((nPlot=(uGridPlotPtr)GridPlotMove((DObjPtr)Plot,&dx,&dy)) == NULL)goto ErrorOut;
	
		
	nPlot->yMajorStep=uR[4].dreturn;
	nPlot->yMinorStep=uR[5].dreturn;;
	
	nPlot->yMajorIn=uR[6].dreturn;
	nPlot->yMajorOut=uR[7].dreturn;
	nPlot->yMinorIn=uR[8].dreturn;
	nPlot->yMinorOut=uR[9].dreturn;
	
	nPlot->rMajorIn=uR[10].dreturn;
	nPlot->rMajorOut=uR[11].dreturn;
	nPlot->rMinorIn=uR[12].dreturn;
	nPlot->rMinorOut=uR[13].dreturn;
	
		
	nPlot->yLogScale =  (int)uR[15].lreturn;
	
	nPlot->yAutoMinimum = (int)uR[16].lreturn;
		
	nPlot->yAutoMaximum = (int)uR[30].lreturn;

	nPlot->yMajorLimit=(int)uR[33].lreturn;	
	if(nPlot->yMajorLimit <= 0)nPlot->yMajorLimit=25;

	nPlot->yMinorLimit=(int)uR[35].lreturn;
	if(nPlot->yMinorLimit <= 0)nPlot->yMinorLimit=20;
		
	if(!nPlot->yAutoMinimum){
		nPlot->ySetMinimum=uR[2].dreturn;
	}
	
	if(!nPlot->yAutoMaximum){
		nPlot->ySetMaximum=uR[3].dreturn;
	}	
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
    
	if(ret){
	    if(nPlot)GridPlotKill((DObjPtr)nPlot);
	    nPlot=NULL;
	}    
	
	uSetPort(&save);
		
	return (struct DObject *)nPlot;
}

static struct DObject *GridPlotDoubleClickX(struct uGridPlot *Plot)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {335,210,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {415,210,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"d-max", {110,40,105,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"d-min", {110,70,105,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"d-major", {110,100,105,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"d-minor", {110,130,105,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"d-major in", {305,40,40,20},uDialogText,uDialogGetDouble},
      /*  7 */      {"d-major out", {305,70,40,20},uDialogText,uDialogGetDouble},
      /*  8 */      {"d-minor in", {305,100,40,20},uDialogText,uDialogGetDouble},
      /*  9 */      {"d-minor out", {305,130,40,20},uDialogText,uDialogGetDouble},
      /* 10 */      {"d-major in", {445,40,40,20},uDialogText,uDialogGetDouble},
      /* 11 */      {"d-major in", {445,70,40,20},uDialogText,uDialogGetDouble},
      /* 12 */      {"d-major in", {445,100,40,20},uDialogText,uDialogGetDouble},
      /* 13 */      {"d-major in", {445,130,40,20},uDialogText,uDialogGetDouble},
      /* 14 */      {"Linear", {10,210,70,20},uDialogButtonRadio,uDialogGetLong},
      /* 15 */      {"Log", {80,210,60,20},uDialogButtonRadio,uDialogGetLong},
      /* 16 */      {"Auto Min", {140,210,80,21},uDialogCheck,uDialogGetLong},
      /* 17 */      {"Minimum", {30,40,75,20},uDialogString,uDialogNone},
      /* 18 */      {"Maximum", {30,70,75,20},uDialogString,uDialogNone},
      /* 19 */      {"Major Step", {30,100,75,20},uDialogString,uDialogNone},
      /* 20 */      {"Minor Step", {30,130,75,20},uDialogString,uDialogNone},
      /* 21 */      {"Ticks", {334,8,49,20},uDialogString,uDialogNone},
      /* 22 */      {"Major In", {220,40,75,20},uDialogString,uDialogNone},
      /* 23 */      {"Major Out", {220,70,75,20},uDialogString,uDialogNone},
      /* 24 */      {"Minor In", {220,100,75,20},uDialogString,uDialogNone},
      /* 25 */      {"Minor Out", {220,130,75,20},uDialogString,uDialogNone},
      /* 26 */      {"tMajor In", {362,40,75,20},uDialogString,uDialogNone},
      /* 27 */      {"tMajor Out", {361,70,75,20},uDialogString,uDialogNone},
      /* 28 */      {"tMinor In", {361,100,75,20},uDialogString,uDialogNone},
      /* 29 */      {"tMinor Out", {363,131,75,20},uDialogString,uDialogNone},
      /* 30 */      {"Auto Max", {230,210,80,21},uDialogCheck,uDialogGetLong},
      /* 31 */      {"Tick Count Limits", {180,155,150,20},uDialogString,uDialogNone},
      /* 32 */      {"Major", {65,180,75,20},uDialogString,uDialogNone},
      /* 33 */      {"t-major", {110,180,105,20},uDialogText,uDialogGetLong},
      /* 34 */      {"Minor", {230,180,75,20},uDialogString,uDialogNone},
      /* 35 */      {"t-minor", {275,180,105,20},uDialogText,uDialogGetLong},
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {50,50,510,250};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	double flist[15];
	uGridPlotPtr nPlot;
	double dx=0,dy=0;
		
	if(!Plot)return NULL;
	
	
	if(Plot->xAutoMinimum){
		flist[0]=Plot->xMinimum;
	}else{
		flist[0]=Plot->xSetMinimum;
	}
	
	if(Plot->xAutoMaximum){
		flist[1]=Plot->xMaximum;
	}else{
		flist[1]=Plot->xSetMaximum;
	}
	
	
	flist[2]=Plot->xMajorStep;
	flist[3]=Plot->xMinorStep;
	
	flist[4]=Plot->xMajorIn;
	flist[5]=Plot->xMajorOut;
	flist[6]=Plot->xMinorIn;
	flist[7]=Plot->xMinorOut;
	
	flist[8]=Plot->tMajorIn;
	flist[9]=Plot->tMajorOut;
	flist[10]=Plot->tMinorIn;
	flist[11]=Plot->tMinorOut;
	
	
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	nPlot=NULL;
	
	uList=uDialogOpen("X Axis Parameters",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)flist[0],
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)flist[1],
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)flist[2],
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)flist[3],
	              	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,(double)flist[4],
	              	              
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,(double)flist[5],
	              	              
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)flist[6],
	              	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,(double)flist[7],
	              	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,(double)flist[8],
	              	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,(double)flist[9],
	              	              
	              uDialogSetItem,12L,
	              uDialogSetDouble,(double)flist[10],
	              	              
	              uDialogSetItem,13L,
	              uDialogSetDouble,(double)flist[11],
	              	              
	              uDialogSetItem,14L,
	              uDialogSetLogical,(int)(!Plot->xLogScale),
	              	              
	              uDialogSetItem,15L,
	              uDialogSetLogical,(int)(Plot->xLogScale),
	              	              
	              uDialogSetItem,16L,
	              uDialogSetLogical,(int)(Plot->xAutoMinimum),
	              	              
	              uDialogSetItem,30L,
	              uDialogSetLogical,(int)(Plot->xAutoMaximum),
	              	              
	              uDialogSetItem,33L,
	              uDialogSetDouble,(double)(Plot->xMajorLimit),
	              	              
	              uDialogSetItem,35L,
	              uDialogSetDouble,(double)(Plot->xMinorLimit),	              	              
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    

start:
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	
	if(!uR[16].lreturn && !uR[30].lreturn && (uR[2].dreturn >= uR[3].dreturn)){
	    Warning("Minimum Must Be Less Than Maximum");
	    goto start;
	}
	if(uR[4].dreturn <= 0 || uR[5].dreturn <= 0){
	    Warning("Steps Must Be Positive");
	    goto start;
	}
	if(uR[15].lreturn && !uR[16].lreturn && !uR[30].lreturn && (uR[2].dreturn <= 0)){
	    Warning("Minimum Must Be Greater Than Zero For Log Plots");
	    goto start;
	}
	
	
	if((nPlot=(uGridPlotPtr)GridPlotMove((DObjPtr)Plot,&dx,&dy)) == NULL)goto ErrorOut;
	
	nPlot->xMajorStep=uR[4].dreturn;
	nPlot->xMinorStep=uR[5].dreturn;;
	
	nPlot->xMajorIn=uR[6].dreturn;
	nPlot->xMajorOut=uR[7].dreturn;
	nPlot->xMinorIn=uR[8].dreturn;
	nPlot->xMinorOut=uR[9].dreturn;
	
	nPlot->tMajorIn=uR[10].dreturn;
	nPlot->tMajorOut=uR[11].dreturn;
	nPlot->tMinorIn=uR[12].dreturn;
	nPlot->tMinorOut=uR[13].dreturn;
	
	nPlot->xLogScale  = (int)uR[15].lreturn;
	
	nPlot->xAutoMinimum = (int)uR[16].lreturn;
		
	nPlot->xAutoMaximum = (int)uR[30].lreturn;
		
	nPlot->xMajorLimit=(int)uR[33].lreturn;	
	if(nPlot->xMajorLimit <= 0)nPlot->xMajorLimit=25;

	nPlot->xMinorLimit=(int)uR[35].lreturn;
	if(nPlot->xMinorLimit <= 0)nPlot->xMinorLimit=20;
		
	if(!nPlot->xAutoMinimum){
		nPlot->xSetMinimum=uR[2].dreturn;
	}
	
	if(!nPlot->xAutoMaximum){
		nPlot->xSetMaximum=uR[3].dreturn;
	}	
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	
	if(ret){
	    if(nPlot)GridPlotKill((DObjPtr)nPlot);
	    nPlot=NULL;
	}
	
	uSetPort(&save);
	
	return (DObjPtr)nPlot;
}
DObjPtr GridPlotMove(DObjPtr o,double *dx,double *dy)
{
	uGridPlotPtr pBox;
	double dxl=0,dyl=0;
	uGridPlotPtr b=(uGridPlotPtr)o;
	int n;
	
	if(!b || !dx || !dy)return NULL;

	if(b->pSelected){
	    if((pBox=(uGridPlotPtr)GridPlotDuplicate(o,0,dx,dy)) != NULL){
	        pBox->xDup = b->xDup;
	        pBox->yDup = b->yDup;
	    }
	    return (DObjPtr)pBox;
	}else{
	    if((pBox=(uGridPlotPtr)GridPlotDuplicate(o,0,&dxl,&dyl)) != NULL){
	        pBox->xDup = b->xDup;
	        pBox->yDup = b->yDup;
	    }else{
	    	return (DObjPtr)pBox;
	    }
	}
	if(b->xSelected){
	    pBox->xBoxOffset.x += *dx;
	    pBox->xBoxOffset.y += *dy;
	}
	if(b->ySelected){
	    pBox->yBoxOffset.x += *dx;
	    pBox->yBoxOffset.y += *dy;
	}
		
	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			struct uLineData *Line;
			Line=b->Lines[n];
			if(!Line)continue;
			if(Line->Selected){
			    pBox->yLegendOffset.x += *dx;
			    pBox->yLegendOffset.y += *dy;
			    break;
			}
		    
		}
	}
	
	
	return (DObjPtr)pBox;
}
static DObjPtr GridPlotDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff)
{
	DObjPtr pBox;
	double x,y;
	uGridPlotPtr b=(uGridPlotPtr)o;
	
	if(!b || !xOff || !yOff)return NULL;

	
	if((pBox=GriddoBoxes(b,&b->box,&o->Attributes)) == NULL){
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
static DObjPtr GriddoBoxes(uGridPlotPtr b,rRect *r,struct uAttributes *Attributes)
{
	uGridPlotPtr pBox;
	int ret;
	int n;
	
	if(!b || !r || !Attributes)return NULL;
	
	ret=1;

	if((pBox=(uGridPlotPtr)GridPlotCreate(b->name,r,Attributes)) == NULL){
	    WarningBatch("GriddoBoxes Out of Memory\n");
	    return NULL;
	}
	
	pBox->box = *r;

	
	pBox->yMinimum=b->yMinimum;
	pBox->yMaximum=b->yMaximum;
	pBox->yViewMin=b->yViewMin;
	pBox->yViewMax=b->yViewMax;
	pBox->yMajorStep=b->yMajorStep;
	pBox->yMinorStep=b->yMinorStep;
	
	pBox->yMajorIn=b->yMajorIn;
	pBox->yMajorOut=b->yMajorOut;
	pBox->yMinorIn=b->yMinorIn;
	pBox->yMinorOut=b->yMinorOut;
	
	pBox->rMajorIn=b->rMajorIn;
	pBox->rMajorOut=b->rMajorOut;
	pBox->rMinorIn=b->rMinorIn;
	pBox->rMinorOut=b->rMinorOut;
	
	pBox->yLogScale = b->yLogScale;
	pBox->xMinimum=b->xMinimum;
	pBox->xMaximum=b->xMaximum;
	pBox->xViewMin=b->xViewMin;
	pBox->xViewMax=b->xViewMax;
	pBox->xMajorStep=b->xMajorStep;
	pBox->xMinorStep=b->xMinorStep;
	
	pBox->xMajorIn=b->xMajorIn;
	pBox->xMajorOut=b->xMajorOut;
	pBox->xMinorIn=b->xMinorIn;
	pBox->xMinorOut=b->xMinorOut;
	
	pBox->tMajorIn=b->tMajorIn;
	pBox->tMajorOut=b->tMajorOut;
	pBox->tMinorIn=b->tMinorIn;
	pBox->tMinorOut=b->tMinorOut;
	
	pBox->xLogScale = b->xLogScale;
	
	pBox->xAutoMinimum=b->xAutoMinimum;
	pBox->yAutoMinimum=b->yAutoMinimum;
	pBox->xAutoMaximum=b->xAutoMaximum;
	pBox->yAutoMaximum=b->yAutoMaximum;
	
	pBox->xSetMinimum=b->xSetMinimum;
	pBox->ySetMinimum=b->ySetMinimum;
	pBox->xSetMaximum=b->xSetMaximum;
	pBox->ySetMaximum=b->ySetMaximum;
	
	pBox->pSelected=b->pSelected;
	pBox->lSelected=b->lSelected;
	
	pBox->xSelected=b->xSelected;
	pBox->ySelected=b->ySelected;
	pBox->xAttributes= b->xAttributes;
	pBox->yAttributes= b->yAttributes;

	pBox->xBoxOffset= b->xBoxOffset;
	pBox->yBoxOffset= b->yBoxOffset;
	pBox->yLegendOffset= b->yLegendOffset;

	pBox->xMajorLimit=b->xMajorLimit;
	pBox->yMajorLimit=b->yMajorLimit;

	pBox->xMinorLimit=b->xMinorLimit;
	pBox->yMinorLimit=b->yMinorLimit;
	pBox->gridHash=b->gridHash;
	
	pBox->bAxisNumber=b->bAxisNumber;
	pBox->lAxisNumber=b->lAxisNumber;
	

	pBox->l= b->l;

    for(n=0;n<b->LineCount;++n){
		struct uLineData *Line,*Lineo;
		Line=b->Lines[n];
		if(!Line)continue;
		if(ResizeArray(&pBox->Lines,sizeof(struct uLineData *),pBox->LineCount,&pBox->LineMax,2017))break;
		Lineo=(struct uLineData *)LineDataCreate(Line->name,&Line->li,&Line->Attributes);
		if(!Lineo)goto ErrorOut;
		mstrncpy(Lineo->save,Line->save,64);
    	if((Line->dCount > 0) && Line->xData  && Line->yData){
	        double *xD,*yD;
	        	long count;
	        	
	        count=Line->dCount;
           
		    if(!(xD=(double *)cMalloc((long)sizeof(double)*(count),8063))){
		        WarningBatch("GriddoBoxes Out of List Memory\n");
		        goto ErrorOut;
		    }
		    if(!(yD=(double *)cMalloc((long)sizeof(double)*(count),8064))){
		        WarningBatch("GriddoBoxes Out of List Memory\n");
		        goto ErrorOut;
		    }
		    
			memcpy(xD,Line->xData,(long)sizeof(double)*(count));
			memcpy(yD,Line->yData,(long)sizeof(double)*(count));

            Lineo->dCount=count;
            Lineo->xData=xD;
            Lineo->yData=yD;
            
            Lineo->Attributes=Line->Attributes;
            Lineo->Selected=Line->Selected;
            Lineo->l=b->l;
            Lineo->ItWas=Line->ItWas;
            Lineo->xmin=Line->xmin;
            Lineo->ymin=Line->ymin;
            Lineo->xmax=Line->xmax;
            Lineo->ymax=Line->ymax;

            Lineo->xShift=Line->xShift;
            Lineo->yShift=Line->yShift;
            Lineo->xScale=Line->xScale;
            Lineo->yScale=Line->yScale;

    	}	            
		
		
		pBox->Lines[pBox->LineCount++]=Lineo;    
    }
    
	ret=0;
ErrorOut:
	if(ret){
	    if(pBox){	    
			GridPlotKill((struct DObject *)pBox);
	    	pBox=NULL;
	    }
	}
	return (DObjPtr)pBox;
}
static int CPlotPlotLegends(struct uGridPlot *b,DOListPtr d)
{
	double LineHeight,width,widthMax,cWidth;
	struct uLineData *Line;
	double ix,iy,i30;
	uFontInfo Info;
	iRect image;
	int Symbol;
	uRect box;
	rRect r;
	int n;
	
	if(!b || !d)return 1;
			
	if(b->Lines && (b->LineCount > 0)){	
		Line=b->Lines[0];
		if(Line)uSetAttributes(d,&Line->Attributes,lPattern | lForeColor | lBackColor | tFont);
						
		r=b->box;

		uGetFontInfo(d->myIcon,&Info);
		
		LineHeight=PixelToLocal(Info.LineHeight,d);
		
		i30=PixelToLocal(30,d);

		cWidth=PixelToLocal(Info.CWidth,d);
		
		
		widthMax=0;
				
		for(n=0;n<b->LineCount;++n){
			Line=b->Lines[n];
			if(!Line)continue;
			uSetAttributes(d,&Line->Attributes,lPattern | lForeColor | lBackColor | lWidth);
/*		    
		    if(!Line->xData || !Line->yData || !Line->save[0])continue;
*/
			ix=r.x+r.xsize+b->xMajorOut+cWidth+b->yLegendOffset.x;
		
			iy=r.y+LineHeight+b->yLegendOffset.y;
		    
		    width=PixelToLocal(uCStringWidth(Line->save,d->myIcon),d);
		    
		    if(width > widthMax)widthMax=width;
		    
	        rMoveTo(ix+i30,iy+n*LineHeight,d);
		    rDrawString(Line->save,d);		    

		    box.x=LocalToPixel(ix,d);
		    box.y=LocalToPixel(iy+n*LineHeight-LineHeight,d);
		    box.xsize=30;
		    box.ysize=Info.LineHeight;
			    
			if(Line->Attributes.DashFlag){
				rLinePat(&box,(char *)&Line->Attributes.LineDash,d);
			}else{
			    rMoveTo(ix,iy+n*LineHeight-0.5*LineHeight,d);
			    rLineTo(ix+i30,iy+n*LineHeight-0.5*LineHeight,d);
			}
			if(Line->Attributes.LineIncrement > 0){
				Symbol=Line->Attributes.LineSymbol;
				if(Symbol >= 0 && Symbol < 40){
				    image=d->Picts[Symbol];
				    image.box.x=box.x+10;
				    image.box.y=box.y;
				    rDrawImageRect(&image,d);	
				}
			}
		    
			Line->box.x=ix;
			Line->box.y=iy+n*LineHeight-LineHeight;
			Line->box.xsize=widthMax+cWidth+i30;
			Line->box.ysize=LineHeight;
			/*
			rBoxDraw(&Line->box,d);
			*/
		}				
	}
	
	return 0;
}
int CPlotPlotNumbers(struct uGridPlot *b,DOListPtr d)
{
	rRect r;
	
	if(!b || !d)return 1;
	
	r=b->box;
	
	if(b->bAxisNumber){
		uSetAttributes(d,&b->xAttributes,lPattern | lForeColor | lBackColor | tFont | lWidth);
		if(b->xLogScale){
		    CPlotxLog(b,&r,d);		 
		}else{		
		    CPlotxLin(b,&r,d);
		}
	}
	if(b->lAxisNumber){
		uSetAttributes(d,&b->yAttributes,lPattern | lForeColor | lBackColor | tFont | lWidth);
		if(b->yLogScale){
		    CPlotyLog(b,&r,d);		 
		}else{	
		    CPlotyLin(b,&r,d);		
		}
	}
	return 0;
}
static int CPlotxLin(struct uGridPlot *b,rRect *r,DOListPtr d)
{
	double LineHeight,width,cWidth;
	double ix,iyy;
	char Label[256];
	uFontInfo Info;
	double x,End,xSmall;
	int n;
	
	if(!b || !r || !d)return 1;
				
	
	xSmall=1.0e-5*(b->xMaximum-b->xMinimum);
	End=b->xViewMax+xSmall;
	
	uGetFontInfo(d->myIcon,&Info);
	
	LineHeight=PixelToLocal(Info.LineHeight,d);
	
	cWidth=PixelToLocal(Info.CWidth,d);

	iyy = r->y+r->ysize+b->xMajorOut+LineHeight+0.5*cWidth;
	
	if(((b->xViewMax-b->xViewMin)/b->xMajorStep) > b->xMajorLimit){
	    b->xMajorStep=(b->xViewMax-b->xViewMin)/(double)b->xMajorLimit;
	}
	
	b->xCount=0;
	for(x=b->xViewMin;x <= End;x += b->xMajorStep){
	    b->xCount++;
	}
	
	if(b->xBoxes)cFree((char *)b->xBoxes);
	b->xBoxes=(rRect *)cMalloc(sizeof(rRect)*(1+b->xCount),8742);
	
	n=0;
				
	for(x=b->xViewMin;x <= End;x += b->xMajorStep){
		ix=(x-b->xViewMin)*b->xScale+b->xOff;
		sprintf(Label,"%g%c",x,0);

		width=PixelToLocal(uCStringWidth(Label,d->myIcon),d);

		ix -= 0.5*width;

		rMoveTo(ix+b->xBoxOffset.x,iyy+b->xBoxOffset.y,d);
		rDrawString(Label,d);		    
		if(b->xBoxes && (n < b->xCount)){
		    b->xBoxes[n].x=ix+b->xBoxOffset.x;
		    b->xBoxes[n].y=iyy-LineHeight+b->xBoxOffset.y;
		    b->xBoxes[n].xsize=width+cWidth;
		    b->xBoxes[n].ysize=LineHeight;
		    /*
			rBoxDraw(&b->xBoxes[n],d);
			*/
			++n;
		}
	}
	
	b->xCount=n;
	
	return 0;
}
static int CPlotxLog(struct uGridPlot *b,rRect *r,DOListPtr d)
{
	double LineHeight,width,cWidth;
	double iyy,ix,iy;
	int iStart,iEnd;
	char Label[256];
	uFontInfo Info;
	double x,y,End,Start,Step,Exp;
	int n;
	
	if(!b || !r || !d)return 1;
	

	uGetFontInfo(d->myIcon,&Info);
	
	LineHeight=PixelToLocal(Info.LineHeight,d);
	
	cWidth=PixelToLocal(Info.CWidth,d);

	iyy = r->y+r->ysize+b->xMajorOut+LineHeight+0.5*cWidth;	
	
	End=log10(b->xViewMax)*(1+b->Small);
	iEnd=(int)End;
	iStart=(int)(b->xViewMinLog-.9999);
	Start=iStart;
	y=(b->yViewMax+b->yViewMin)/2.;
	if((iEnd-iStart) <= 9){
	    Step=1;
	}else{
	    Step=4;
	}

	if(((End-Start)/Step) > b->xMajorLimit){
	    Step=(End-Start)/(double)b->xMajorLimit;
	}


	b->xCount=0;
	for(x=Start;x <= End;x += Step){
	    b->xCount++;
	}
	
	if(b->xBoxes)cFree((char *)b->xBoxes);
	b->xBoxes=(rRect *)cMalloc(sizeof(rRect)*(1+b->xCount),8742);

	n=0;

	for(x=Start;x <= End;x += Step){
		Exp=pow(10.,x);

		if(PlotrPoint(b,&Exp,&y,&ix,&iy))continue;
		    
		sprintf(Label,"%g%c",Exp,0);
		width=PixelToLocal(uCStringWidth(Label,d->myIcon),d);

		ix -= 0.5*width;

		rMoveTo(ix+b->xBoxOffset.x,iyy+b->xBoxOffset.y,d);
		rDrawString(Label,d);		    
		if(b->xBoxes && (n < b->xCount)){
		    b->xBoxes[n].x=ix+b->xBoxOffset.x;
		    b->xBoxes[n].y=iyy-LineHeight+b->xBoxOffset.y;
		    b->xBoxes[n].xsize=width+cWidth;
		    b->xBoxes[n].ysize=LineHeight;
		    /*
			rBoxDraw(&b->xBoxes[n],d);
			*/
			++n;
		}
	}

	b->xCount=n;
	
	return 0;
}
static int CPlotyLin(struct uGridPlot *b,rRect *r,DOListPtr d)
{
	double LineHeight,width,cWidth;
	double ixx,iy;
	char Label[256];
	uFontInfo Info;
	double y,End,ySmall;
	int n;
	
	if(!b || !r || !d)return 1;

	ySmall=1.0e-5*(b->yViewMax-b->yViewMin);
	End=b->yViewMax+ySmall;
	uGetFontInfo(d->myIcon,&Info);
	
	LineHeight=PixelToLocal(Info.LineHeight,d);
	
	cWidth=PixelToLocal(Info.CWidth,d);
	
	ixx=r->x-b->yMajorOut-0.75*cWidth;

	if(((b->yViewMax-b->yViewMin)/b->yMajorStep) > b->yMajorLimit){
	    b->yMajorStep=(b->yViewMax-b->yViewMin)/(double)b->yMajorLimit;
	}
	
	b->yCount=0;
	for(y=b->yViewMin;y <= End;y += b->yMajorStep){
	    b->yCount++;
	}
	
	if(b->yBoxes)cFree((char *)b->yBoxes);
	b->yBoxes=(rRect *)cMalloc(sizeof(rRect)*(1+b->yCount),8743);
		
	n=0;
	for(y=b->yViewMin;y <= End;y += b->yMajorStep){
	    iy=(y-b->yViewMin)*b->yScale+b->yOff;
		sprintf(Label,"%g%c",y,0);

		iy += (LineHeight)/2;


		width=PixelToLocal(uCStringWidth(Label,d->myIcon),d);

		rMoveTo(ixx-width+b->yBoxOffset.x,iy+b->yBoxOffset.y,d);
		rDrawString(Label,d);		    
		if(b->yBoxes && (n < b->yCount)){
		    b->yBoxes[n].x=ixx-width+b->yBoxOffset.x;
		    b->yBoxes[n].y=iy-LineHeight+b->yBoxOffset.y;
		    b->yBoxes[n].xsize=width+cWidth;
		    b->yBoxes[n].ysize=LineHeight;
		    /*
			rBoxDraw(&b->yBoxes[n],d);
			*/
			++n;
		}
	}
	
	b->yCount=n;
	
	return 0;
}
static int CPlotyLog(struct uGridPlot *b,rRect *r,DOListPtr d)
{
	double ix,ixx,iy,LineHeight,cWidth,width;
	int iEnd,iStart;
	char Label[256];
	uFontInfo Info;
	double x,y,End,Start,Exp,Step;
	int n;
	
	if(!b || !r || !d)return 1;

	uGetFontInfo(d->myIcon,&Info);
	
	LineHeight=PixelToLocal(Info.LineHeight,d);
	
	cWidth=PixelToLocal(Info.CWidth,d);
	
	End=log10(b->yViewMax);
	End=End+fabs(End)*b->Small;
	iEnd=(int)End;
	iStart=(int)(b->yViewMinLog-.9999);
	Start=iStart;
	x=(b->xViewMax+b->xViewMin)/2.;
	if((iEnd-iStart) <= 9){
	    Step=1;
	}else{
	    Step=4;
	}
		
	ixx=r->x-b->yMajorOut-0.75*cWidth;
	
	if(((End-Start)/Step) > b->yMajorLimit){
	    Step=(End-Start)/(double)b->yMajorLimit;
	}
	
	b->yCount=0;

	for(y=Start;y <= End;y += Step){
	    b->yCount++;
	}
	
	if(b->yBoxes)cFree((char *)b->yBoxes);
	b->yBoxes=(rRect *)cMalloc(sizeof(rRect)*(1+b->yCount),8743);
		
	n=0;	
	for(y=Start;y <= End;y += Step){
		Exp=pow(10.,y);
		if(PlotrPoint(b,&x,&Exp,&ix,&iy))continue;
		sprintf(Label,"%g%c",Exp,0);
		width=PixelToLocal(uCStringWidth(Label,d->myIcon),d);


		iy += 0.5*LineHeight;
		rMoveTo(ixx-width+b->yBoxOffset.x,iy+b->yBoxOffset.y,d);
		rDrawString(Label,d);		    
		if(b->yBoxes && (n < b->yCount)){
		    b->yBoxes[n].x=ixx-width+b->yBoxOffset.x;
		    b->yBoxes[n].y=iy-LineHeight+b->yBoxOffset.y;
		    b->yBoxes[n].xsize=width+cWidth;
		    b->yBoxes[n].ysize=LineHeight;
		    /*
			rBoxDraw(&b->yBoxes[n],d);
			*/
			++n;
		}
	}
	
	b->yCount=n;
	
	return 0;
}

static int clip2(struct uGridPlot *b,double *x1i,double *y1i,double *x2i,double *y2i,int *i1i)
{
	double sxn,syn,sxx,syx;
	double x1, y1, x2, y2;
	int i1,i2,iflip;
	double s[3],dnom,t;
	double xt,yt,sn,sx;
	
	if(!b || !x1i || !y1i || !x2i || !y2i || !i1i)return 1;
	
	x1 = *x1i;
	x2 = *x2i;
	y1 = *y1i;
	y2 = *y2i;

	i1=0;
	i2=0;
	
      if(x1 < b->xViewMin || x1 > b->xViewMax)i1=1;
      if(x2 < b->xViewMin || x2 > b->xViewMax)i2=1;
      if(y1 < b->yViewMin || y1 > b->yViewMax)i1=1;
      if(y2 < b->yViewMin || y2 > b->yViewMax)i2=1;
      
      *i1i=i1;
      
      if(i1 == 0 && i2 == 0){
          return 0;
      }
      sxn=-1.;
      syn=-1.;
      sxx=-1.;
      syx=-1.;
      iflip=0;
      dnom=x2-x1;
      if(fabs(dnom) <= b->dxLim)goto l20;
      sxn=(b->xViewMin-x1)/dnom;
      sxx=(b->xViewMax-x1)/dnom;
 l20: dnom=y2-y1;
      if(fabs(dnom) <= b->dyLim)goto l30;
      syn=(b->yViewMin-y1)/dnom;
      syx=(b->yViewMax-y1)/dnom;
 l30: if(sxn < 0.0 || sxn > 1.)goto l40;
      t=sxn*y2+(1.-sxn)*y1;
      if(t < b->yViewMin || t > b->yViewMax) goto l40;
      iflip=iflip+1;
      s[iflip]=sxn;
 l40: if(sxx < 0.0 || sxx > 1.)goto l50;
      t=sxx*y2+(1.-sxx)*y1;
      if(t < b->yViewMin || t > b->yViewMax) goto l50;
      iflip=iflip+1;
      s[iflip]=sxx;
      if(iflip > 1) goto l70;
 l50: if(syn < 0.0 || syn > 1.)goto l60;
      t=syn*x2+(1.-syn)*x1;
      if(t < b->xViewMin || t > b->xViewMax) goto l60;
      iflip=iflip+1;
      s[iflip]=syn;
      if(iflip > 1) goto l70;
 l60: if(syx < 0.0 || syx > 1.)goto l70;
      t=syx*x2+(1.-syx)*x1;
      if(t < b->xViewMin || t > b->xViewMax) goto l70;
      iflip=iflip+1;
      s[iflip]=syx;
 l70: 
      if(iflip == 0) return 1;
      if(iflip > 1)goto l90;
      if(i1 == 0) goto l80;
      *x1i=s[1]*x2+(1.-s[1])*x1;
      *y1i=s[1]*y2+(1.-s[1])*y1;
      return 0;
 l80: 
      *x2i=s[1]*x2+(1.-s[1])*x1;
      *y2i=s[1]*y2+(1.-s[1])*y1;
      return 0;
l90: 
      xt=x1;
      yt=y1;
      sn=min(s[1],s[2]);
      sx=max(s[1],s[2]);
      *x1i=sn*x2+(1.-sn)*xt;
      *y1i=sn*y2+(1.-sn)*yt;
      *x2i=sx*x2+(1.-sx)*xt;
      *y2i=sx*y2+(1.-sx)*yt;
      return 0;
}
static void CPlotDashedLine(struct uGridPlot *b,double *xx,double *yy,int nn,char *pat,DOListPtr d,struct uLineData *Line)
{
	double dxy,dist,xl,yl,xs,ys,xp,yp,xt,yt; 
	double x,y;
	double rast[16];
	int ion[16];
	int  flag,back,n,i,np;
	int ns;
	int i1;
      
      
	  if(!b || !xx || !yy || !pat || !d || !Line)return;
      n=0;
      for(i=0;i<16;++i){
		  if(pat[i] == '0' || pat[i] == 0)break;	
		  ++n;
		  np=pat[i]-'0';
		  rast[i]=(double)np/(double)d->dpi;
		  ion[i]=FALSE;
		  if(np & 1)ion[i]=TRUE;
      }
/*
c
c        start pattern
c
*/
	if(n <= 0)return;


	ns=0;
	flag=ion[ns];
	dxy=rast[ns];
	back=FALSE;
	
	x=Line->xShift+Line->xScale*xx[0];
	y=Line->yShift+Line->yScale*yy[0];

	if(!PlotrPoint(b,&x,&y,&xs,&ys)){
	    rMoveTo(xs,ys,d);
	}
	xl=Line->xShift+Line->xScale*xx[0];
	yl=Line->yShift+Line->yScale*yy[0];
	for(np=1;np<nn;++np){
		xt=Line->xShift+Line->xScale*xx[np];
		yt=Line->yShift+Line->yScale*yy[np];	
		if(clip2(b,&xl,&yl,&xt,&yt,&i1))goto l115;
		if(i1 == 0)goto l60;
				
		if(CPlotrPoint(b,&xl,&yl,&xp,&yp)){
		    goto l115;
		}
		rMoveTo(xp,yp,d);
		xs=xp;
		ys=yp;
l60:	
		back=FALSE;
		if(CPlotrPoint(b,&xt,&yt,&xp,&yp)){
		    goto l115;
		}
		dist=sqrt((xp-xs)*(xp-xs)+(yp-ys)*(yp-ys));
		if(dist >= dxy){
			xp=xs+(xp-xs)*dxy/dist;
			yp=ys+(yp-ys)*dxy/dist;
			back=TRUE;
		}
		if(flag){
			rLineTo(xp,yp,d);
		}else{
			rMoveTo(xp,yp,d);
		}
		xs=xp;
		ys=yp;
		dxy=dxy-dist;
		if(back){
			ns=ns+1;
			if(ns >= n)ns=0;
			dxy=rast[ns];
			flag=ion[ns];
			goto l60;
		}
l115:
		xl=Line->xShift+Line->xScale*xx[np];
		yl=Line->yShift+Line->yScale*yy[np];
	}
}

static int CPlotrPoint(struct uGridPlot *b,double *x,double *y,double *xr,double *yr)
{

	if(!b || !x || !y || !xr || !yr)return 0;
	
	if(b->xLogScale){
	    if(*x < b->xViewMin){
	        return 1;
	    }else{
	        *xr = (log10(*x)-b->xViewMinLog)*b->xScaleLog+b->xOff;
	    }
	}else{
	    *xr = (*x-b->xViewMin)*b->xScale+b->xOff;
	}
	if(b->yLogScale){
	    if(*y < b->yViewMin){
	        return 1;
	    }else{
	        *yr = (log10(*y)-b->yViewMinLog)*b->yScaleLog+b->yOff;
	    }
	}else{
	    *yr = (*y-b->yViewMin)*b->yScale+b->yOff;
	}
	return 0;
}

static int GridPlotLineGrid(struct uGridPlot *b,DOListPtr d)
{
	struct uLineData *Line;
	int Increment,Symbol;
	double ix,iy;
	double xl,yl,xt,yt,xp,yp;
	double x,y;
	int i1,iMove;
	iRect image;
	long n,k;
	
	if(!b || !d)return 1;

	if(!b->Lines || (b->LineCount <= 0))return 0;	
		
	for(n=0;n<b->LineCount;++n){
		Line=b->Lines[n];
		if(!Line)continue;
	    
	    if(!Line->xData || !Line->yData )continue;
	    
		uSetAttributes(d,&Line->Attributes,lPattern | lForeColor | lBackColor | lWidth);
		
		
		Increment=Line->Attributes.LineIncrement;
		Symbol=Line->Attributes.LineSymbol;
		if(Increment > 0 && (Symbol >=0 && Symbol < 40)){
		    for(k=0;k<Line->dCount;++k){
				if(Increment > 0 && !(k % Increment)){
				    x=Line->xShift+Line->xScale*Line->xData[k];
				    y=Line->yShift+Line->yScale*Line->yData[k];
				    if(PlotrPoint(b,&x,&y,&ix,&iy))continue;
			        rMoveTo(ix,iy,d);
			        image=d->Picts[Symbol];
				    image.box.x=LocalToPixel(ix,d)-5;
				    image.box.y=LocalToPixel(iy,d)-5;
				    rDrawImageRect(&image,d);	
				}
		    }
		}		

		if(Line->Attributes.DashFlag){
	    	FlushCurrentPort();		
	        CPlotDashedLine(b,Line->xData,Line->yData,(int)Line->dCount,(char *)&Line->Attributes.LineDash,d,Line);
	    	FlushCurrentPort();	      
	      	continue;
	    }

		iMove=FALSE;
	    xl=Line->xShift+Line->xScale*Line->xData[0];
	    yl=Line->yShift+Line->yScale*Line->yData[0];
		if(!CPlotrPoint(b,&xl,&yl,&xp,&yp)){
			rMoveTo(xp,yp,d);
			iMove=TRUE;
		}	    
	    for(k=1;k<Line->dCount;++k){
			xl=Line->xShift+Line->xScale*Line->xData[k-1];
			yl=Line->yShift+Line->yScale*Line->yData[k-1];	
			xt=Line->xShift+Line->xScale*Line->xData[k];
			yt=Line->yShift+Line->yScale*Line->yData[k];	
			if(clip2(b,&xl,&yl,&xt,&yt,&i1)){
			    continue;
			}
			if(i1 != 0){
				if(CPlotrPoint(b,&xl,&yl,&xp,&yp)){
				    continue;
				}
				rMoveTo(xp,yp,d);
				iMove=TRUE;
			}

			if(CPlotrPoint(b,&xt,&yt,&xp,&yp)){
		    	continue;
			}
			if(!iMove){
			    rMoveTo(xp,yp,d);
				iMove=TRUE;
			}
			rLineTo(xp,yp,d);
	    }	    	    
	}
		
	return 0;
	
}
static int PlotrPoint(struct uGridPlot *b,double *x,double *y,double *ix,double *iy)
{

	int ret;
	
	if(!b || !ix || !iy || !x ||  !y)return 0;
	
	ret=0;
	
	if(b->xLogScale){
	    if(*x < b->xViewMin*(1-sign(b->Small,b->xViewMin))){
	        *ix = (log10(b->xViewMin)-b->xViewMinLog)*b->xScaleLog+b->xOff;
			ret=1;
	    }else if(*x > b->xViewMax*(1+sign(b->Small,b->xViewMax))){
	        *ix = (log10(b->xViewMax)-b->xViewMinLog)*b->xScaleLog+b->xOff;
			ret=1;
	    }else{
	        *ix = (log10(*x)-b->xViewMinLog)*b->xScaleLog+b->xOff;
	    }
	}else{
	    if(*x < b->xViewMin*(1-sign(b->Small,b->xViewMin))){
	    	*ix = b->xOff;
			ret=1;
	    }else if(*x > b->xViewMax*(1+sign(b->Small,b->xViewMax))){
	    	*ix = (b->xViewMax-b->xViewMin)*b->xScale+b->xOff;
			ret=1;
	    }else{
	    	*ix = (*x-b->xViewMin)*b->xScale+b->xOff;
	    }
	}
	if(b->yLogScale){
	    if(*y < b->yViewMin*(1-sign(b->Small,b->yViewMin))){
	        *iy = (log10(b->yViewMin)-b->yViewMinLog)*b->yScaleLog+b->yOff;
			ret=1;
	    }else if(*y > b->yViewMax*(1+sign(b->Small,b->yViewMax))){
	        *iy = (log10(b->yViewMax)-b->yViewMinLog)*b->yScaleLog+b->yOff;
			ret=1;
	    }else{
	        *iy = (log10(*y)-b->yViewMinLog)*b->yScaleLog+b->yOff;
	    }
	}else{
	    if(*y < b->yViewMin*(1-sign(b->Small,b->yViewMin))){
	    	*iy = b->yOff;
			ret=1;
	    }else if(*y > b->yViewMax*(1+sign(b->Small,b->yViewMax))){
	    	*iy = (b->yViewMax-b->yViewMin)*b->yScale+b->yOff;
			ret=1;
	    }else{
	    	*iy = (*y-b->yViewMin)*b->yScale+b->yOff;
	    }
	}
	return ret;
}
int GridPlotPlotGrid(struct uGridPlot *b,DOListPtr d)
{
	double xsize2,ysize2;
	double right,bottom;
	double mult;
/*	IconPtr myIcon; */
	rRect r;
	
	if(!b || !d)return 1;
	
	r=b->box;
	
/*
 	xsize2=b->vAttributes.xLineWidth/2;
 	ysize2=b->vAttributes.yLineWidth/2;
*/
	xsize2=0;
	ysize2=0;
	
	mult=1;
	
	right=r.x+r.xsize;
	bottom=r.y+r.ysize;
 	 	
 	
	uSetAttributes(d,&b->Attributes,lPattern | lForeColor | lBackColor | lWidth);

	if(b->lAxis){
	    GridPlotxGridDraw(b,d,bottom,b->xMajorIn,b->xMajorOut,b->xMinorIn,b->xMinorOut,mult,&r);
	    rMoveTo(r.x-xsize2,  bottom-ysize2,d);
	    rLineTo(right-xsize2,bottom-ysize2,d);
	}
	if(b->rAxis){
	    GridPlotxGridDraw(b,d,r.y,b->tMajorIn,b->tMajorOut,b->tMinorIn,b->tMinorOut,-mult,&r);
	    rMoveTo(r.x-xsize2,  r.y-ysize2,d);
	    rLineTo(right-xsize2,r.y-ysize2,d);
	}
	if(b->bAxis){
	    GridPlotyGridDraw(b,d,r.x,b->yMajorIn,b->yMajorOut,b->yMinorIn,b->yMinorOut,mult,&r);
	    rMoveTo(r.x-xsize2,r.y-ysize2,d);
	    rLineTo(r.x-xsize2,bottom-ysize2,d);
	}
	if(b->tAxis){
	    GridPlotyGridDraw(b,d,right,b->rMajorIn,b->rMajorOut,b->rMinorIn,b->rMinorOut,-mult,&r);
	    rMoveTo(right-xsize2,r.y-ysize2,d);
	    rLineTo(right-xsize2,bottom-ysize2,d);
	}
	/* SetPenNormal(myIcon); */
	return  0;
}
static void GridPlotNeat(double *xmnc,double *xmxc,double *Large,double *Small)
{

    double xmn = *xmnc,xmx = *xmxc;
    static double small2=1.e-6,dnum[4]={1.,2.,5.,10.};
    double delx,temp,t,fac,td,tmn,znd,zndt;
    int i,j,jj,itm;
    
    if(!xmnc || !xmxc || !Large || !Small)return;
    
	delx=xmx-xmn;
	temp=delx/6.;
	t=0.;
	if(temp != 0.)t=log10(fabs(temp)+small2*fabs(temp));
	i=(int)t;
	if(t < 0.)i=i-1;
	fac=GridPlotpow10(i);
	temp=temp/fac;
	for(j=0;j<4;++j){
	    jj=j;
	    if(dnum[j] >= temp) break;
    }
	td=dnum[jj]*fac;
	tmn=xmn/td;
	if(fabs(tmn) > 3e10){
blastOut:
	    *xmxc = *xmnc+1.0;
	    *xmnc = *xmnc-1.0;
	    temp=(*xmxc - *xmnc)/4.;
	    *Large = temp;
	    *Small = temp*.5;
	    return;
	}
	if(tmn < 0.)tmn=tmn-small2*tmn;
	itm=(int)tmn;
	if(tmn < 0.)itm=itm-1;
	tmn=itm;
	tmn=tmn*td;
	znd=2.;
l50:	znd=znd+1.;
	if(znd > 8.){
	    goto blastOut;
	}
	zndt=znd*td+tmn;
	if(zndt >= xmx) goto l60;
	goto l50;
l60:
	*xmnc=tmn;
	*xmxc=zndt;
	delx=xmx-xmn;
	*Large=td;
	if(jj == 1){
	    *Small = td/4;
	}else{
	    *Small = td/5;
	}
}
static double GridPlotpow10(int n)
{
	double a;
 
	a=1.;
 
	if(n < 0){
	   n = -n;
	   while(n--)a/=10.;
	} else {
	    while(n--)a*=10.;
	}
	return a;
}
static void GridPlotyGridDraw(struct uGridPlot *b,DOListPtr d,double x,double yMajorIn,double yMajorOut,double yMinorIn,double yMinorOut,double mult,rRect *r)
{
	double MajorIn,MajorOut,MinorIn,MinorOut;
	double xx,y,yy,End,mEnd,Start,Exp,Step,Expm,ySmall;
	int iStart,iEnd,n;
	double xsize2,ysize2;
	double ix,iy;
	
	if(!b || !r || !d)return;
	
	/*
 	xsize2=b->vAttributes.xLineWidth/2;
 	ysize2=b->vAttributes.yLineWidth/2;
	*/

 	xsize2=0;
 	ysize2=0;
	
	MajorIn=x+yMajorIn*mult;
	MajorOut=x-yMajorOut*mult;
	MinorIn=x+yMinorIn*mult;
	MinorOut=x-yMinorOut*mult;
	End=b->yViewMax*(1+b->Small);
	mEnd=b->yMajorStep*(1-b->Small);
	if(b->yLogScale){
	    End=log10(b->yViewMax)*(1+b->Small);
	    iEnd=(int)End;
	    iStart=(int)(b->yViewMinLog-.9999);
	    Start=iStart;
	    xx=(b->xViewMax+b->xViewMin)/2.;
	    if((iEnd-iStart) <= 9){
	        Step=1;
	    }else{
	        Step=4;
	    }

		if(((End-Start)/Step) > b->yMajorLimit){
		    Step=(End-Start)/(double)b->yMajorLimit;
		}
	    	    
	    for(y=Start;y <= End;y += Step){
	        Exp=pow(10.,y);
	        if(!PlotrPoint(b,&xx,&Exp,&ix,&iy)){
	            rMoveTo(MajorIn-xsize2,iy-ysize2,d);
	            rLineTo(MajorOut-xsize2,iy-ysize2,d);
	            if(b->gridHash){
	            	double xx,yy,x1,x2;
	            	yy=LocalToPixel(iy,d);
	            	x1=LocalToPixel(r->x,d);
	            	x2=LocalToPixel(r->x+r->xsize,d);
	                for(xx=x1;xx<x2;xx += 5){
	            	    riMoveTo((int)xx,(int)yy,d);
	            	    riLineTo((int)(xx+1),(int)yy,d);
	            	}
	            }
	        }
	        if(Step == 1){
	            Expm=Exp+Exp;
	            for(n=0;(n<8) && (log10(Expm) < End);++n,Expm += Exp){
					if(!PlotrPoint(b,&xx,&Expm,&ix,&iy)){
					    rMoveTo(MinorIn-xsize2,iy-ysize2,d);
					    rLineTo(MinorOut-xsize2,iy-ysize2,d);
					}
		        }
	        }
	    }
	}else{

		if(((b->yViewMax-b->yViewMin)/b->yMajorStep) > b->yMajorLimit){
	    	b->yMajorStep=(b->yViewMax-b->yViewMin)/(double)b->yMajorLimit;
		}

	    ySmall=1.0e-5*(b->yViewMax-b->yViewMin);
	    End=b->yViewMax+ySmall;
	    mEnd=b->yMajorStep-ySmall;
	   	Start=b->yViewMin;
	   	
		if(((b->yMajorStep)/b->yMinorStep) > b->yMinorLimit){
	    	b->yMinorStep=(b->yMajorStep)/(double)b->yMinorLimit;
		}	   	
	   	
	    for(y=Start;y <= End;y += b->yMajorStep){
	            iy=(y-b->yViewMin)*b->yScale+b->yOff;
	            rMoveTo(MajorIn-xsize2,iy-ysize2,d);
	            rLineTo(MajorOut-xsize2,iy-ysize2,d);
	            if(b->gridHash){
	            	double xx,yy,x1,x2;
	            	yy=LocalToPixel(iy,d);
	            	x1=LocalToPixel(r->x,d);
	            	x2=LocalToPixel(r->x+r->xsize,d);
	                for(xx=x1;xx<x2;xx += 5){
	            	    riMoveTo((int)xx,(int)yy,d);
	            	    riLineTo((int)(xx+1),(int)yy,d);
	            	}
	            }
	        for(yy=y+b->yMinorStep;
	            yy < y+mEnd && yy < End;yy += b->yMinorStep){
	                iy=(yy-b->yViewMin)*b->yScale+b->yOff;
	                rMoveTo(MinorIn-xsize2,iy-ysize2,d);
	                rLineTo(MinorOut-xsize2,iy-ysize2,d);
	        }
	    }
	}
}
static void GridPlotxGridDraw(struct uGridPlot *b,DOListPtr d,double y,double xMajorIn,double xMajorOut,double xMinorIn,double xMinorOut,double mult,rRect *r)
{
	double MajorIn,MajorOut,MinorIn,MinorOut;
	double x,xx,End,mEnd,Start,Exp,yy,Step,Expm,xSmall;
	int iStart,iEnd,n;
	double xsize2,ysize2;
	double ix,iy;
	
	if(!b || !r || !d)return;
/*
 	xsize2=b->vAttributes.xLineWidth/2;
 	ysize2=b->vAttributes.yLineWidth/2;
*/

 	xsize2=0;
 	ysize2=0;
	
	MajorIn=y-xMajorIn*mult;
	MajorOut=y+xMajorOut*mult;
	MinorIn=y-xMinorIn*mult;
	MinorOut=y+xMinorOut*mult;
	End=b->xViewMax*(1+b->Small);
	mEnd=b->xMajorStep*(1-b->Small);
	if(b->xLogScale){
	    End=log10(b->xViewMax)*(1+b->Small);
	    iEnd=(int)End;
	    iStart=(int)(b->xViewMinLog-.9999);
	    Start=iStart;
	    yy=(b->yViewMax+b->yViewMin)/2.;
	    if((iEnd-iStart) <= 9){
	        Step=1;
	    }else{
	        Step=4;
	    }
	    
		if(((End-Start)/Step) > b->xMajorLimit){
		    Step=(End-Start)/(double)b->xMajorLimit;
		}
	    
	    
	    for(x=Start;x <= End;x += Step){
	        Exp=pow(10.,x);
	        if(!PlotrPoint(b,&Exp,&yy,&ix,&iy)){
	            rMoveTo(ix-xsize2,MajorIn-ysize2,d);
	            rLineTo(ix-xsize2,MajorOut-ysize2,d);
	            if(b->gridHash){
	            	double xx,yy,y1,y2;
	            	xx=LocalToPixel(ix,d);
	            	y1=LocalToPixel(r->y,d);
	            	y2=LocalToPixel(r->y+r->ysize,d);
	                for(yy=y1;yy<y2;yy += 5){
	            	    riMoveTo((int)xx,(int)yy,d);
	            	    riLineTo((int)xx,(int)(yy+1),d);
	            	}
	            }
	        }
	        if(Step == 1){
	            Expm=Exp+Exp;
	            for(n=0;(n<8) && (log10(Expm) < End);++n,Expm += Exp){
					if(!PlotrPoint(b,&Expm,&yy,&ix,&iy)){
					    rMoveTo(ix-xsize2,MinorIn-ysize2,d);
					    rLineTo(ix-xsize2,MinorOut-ysize2,d);
					}
		    	}
	        }
	    }
	}else{
	
		if(((b->xViewMax-b->xViewMin)/b->xMajorStep) > b->xMajorLimit){
	    	b->xMajorStep=(b->xViewMax-b->xViewMin)/(double)b->xMajorLimit;
		}
	
	    xSmall=1.0e-5*(b->xViewMax-b->xViewMin);
	    End=b->xViewMax+xSmall;
	    mEnd=b->xMajorStep-xSmall;
	    Start=b->xViewMin;
	    
		if(((b->xMajorStep)/b->xMinorStep) > b->xMinorLimit){
	    	b->xMinorStep=(b->xMajorStep)/(double)b->xMinorLimit;
		}	   	
	    	    
	    for(x=Start;x <= End;x += b->xMajorStep){
	            ix=(x-b->xViewMin)*b->xScale+b->xOff;
	            rMoveTo(ix-xsize2,MajorIn-ysize2,d);
	            rLineTo(ix-xsize2,MajorOut-ysize2,d);
	            if(b->gridHash){
	            	double xx,yy,y1,y2;
	            	xx=LocalToPixel(ix,d);
	            	y1=LocalToPixel(r->y,d);
	            	y2=LocalToPixel(r->y+r->ysize,d);
	                for(yy=y1;yy<y2;yy += 5){
	            	    riMoveTo((int)xx,(int)yy,d);
	            	    riLineTo((int)xx,(int)(yy+1),d);
	            	}
	            }
	        for(xx=x+b->xMinorStep;
	            xx < x+mEnd && xx < End;xx += b->xMinorStep){
	                ix=(xx-b->xViewMin)*b->xScale+b->xOff;
	                rMoveTo(ix-xsize2,MinorIn-ysize2,d);
	                rLineTo(ix-xsize2,MinorOut-ysize2,d);
	        }
  	    }
	}
}
static void GridPlotKill(struct DObject *Object)
{
	struct uGridPlot *b=(struct uGridPlot *)Object;
	int n;
	
	if(!b)return;
	
	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
			struct uLineData *Line;
			Line=b->Lines[n];
			if(!Line || !Line->Kill)continue;
			(*Line->Kill)((DObjPtr)Line);
		}
	}	
	
	
	if(b->Lines)cFree((char *)b->Lines);
	b->Lines=NULL;
	
	if(b->xBoxes)cFree((char *)b->xBoxes);
	b->xBoxes=NULL;
	
	if(b->yBoxes)cFree((char *)b->yBoxes);
	b->yBoxes=NULL;
			
	ObjectKill(Object);
		
}
int GridPlotAdd(DOListPtr l,char *plot,struct uLineData *Line)
{
	DObjPtr Object;
	struct uGridPlot *Plot;
	int ret;
	
	if(!l || !plot || !Line)return 1;
	
	ret=1;
	
	Object=DOListFindName(l,plot,GridPlotType);
	if(!Object){
		sprintf(WarningBuff,"GridPlotAdd plot '%s' not found\n",plot);
		WarningBatch(WarningBuff);		
		goto ErrorOut;
	}
	Plot=(struct uGridPlot *)Object;
	
		
	if(ResizeArray(&Plot->Lines,sizeof(struct uLineData *),Plot->LineCount,&Plot->LineMax,2018))goto ErrorOut;
	Plot->Lines[Plot->LineCount++]=Line;
	Line->l=l;
	
	ret=0;
ErrorOut:	
	return ret;
}

uLineDataPtr DOLineDataCreate(struct linedata *li,struct uAttributes *Attributes)
{
	static long ObjectCount;
	uLineDataPtr Line;
	char name[256];
	int ret;
	
	if(!Attributes)return NULL;
	
	ret=1;

	sprintf(name,"Line%03ld%c",ObjectCount++,0);
		
	Line=(uLineDataPtr)LineDataCreate(name,li,Attributes);
	if(!Line)goto ErrorOut;
	
	
	ret=0;
ErrorOut:	    
	if(ret){
	    if(Line){
			LineDataKill((DObjPtr)Line);
	        Line=NULL;
	    }
	}
	
	return Line;
}
static struct DObject *uLineWrite(struct DObject *o,FILE8 *inOut,struct DOList *l)
{


	static struct uLineData b;
	
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
		
		
		{"STRI",4039,universal_TypeString,(void *)&b.Attributes.AnimationFormat[0],sizeof(b.Attributes.AnimationFormat)},
		
		{"DOUB",4040,universal_TypeDouble,(void *)&b.Attributes.AnimationStart},	
		{"DOUB",4041,universal_TypeDouble,(void *)&b.Attributes.AnimationStep},	

		{"REAL",4042,universal_TypeInt,(void *)&b.Attributes.Animation},
		{"REAL",4043,universal_TypeInt,(void *)&b.Attributes.AnimationJust},

		{"STRI",4044,universal_TypeString,(void *)&b.li.sourceName[0],sizeof(b.li.sourceName)},

		{"DOUB",4045,universal_TypeDouble,(void *)&b.li.x1},	
		{"DOUB",4046,universal_TypeDouble,(void *)&b.li.x2},	

		{"DOUB",4047,universal_TypeDouble,(void *)&b.li.y1},	
		{"DOUB",4048,universal_TypeDouble,(void *)&b.li.y2},	

		{"DOUB",4049,universal_TypeDouble,(void *)&b.li.z1},	
		{"DOUB",4050,universal_TypeDouble,(void *)&b.li.z2},	

		{"STRI",4051,universal_TypeString,(void *)&b.li.pioName[0],sizeof(b.li.pioName)},
		{"REAL",4052,universal_TypeLong,(void *)&b.li.pioIndex},
		
		{"REAL",4053,universal_TypeInt,(void *)&b.li.type},
		
		{"REAL",4054,universal_TypeLong,(void *)&b.li.CurrentFrame},
		
		{"REAL",4055,universal_TypeLong,(void *)&b.li.FirstFrame},
		
		{"REAL",4056,universal_TypeLong,(void *)&b.li.LastFrame},
		
		{"DOUB",4057,universal_TypeDouble,(void *)&b.li.FirstTime},	
		{"DOUB",4058,universal_TypeDouble,(void *)&b.li.LastTime},	

		{"REAL",4059,universal_TypeInt,(void *)&b.li.Polynomial},
		{"REAL",4060,universal_TypeInt,(void *)&b.li.PolynomialOrder},
		{"REAL",4061,universal_TypeInt,(void *)&b.li.PolynomialPrint},
		{"REAL",4062,universal_TypeInt,(void *)&b.li.PointLineCount},
		
		{"REAL",4063,universal_TypeLong,(void *)&b.li.FrameCount},

		{"STRI",4064,universal_TypeString,(void *)&b.save[0],sizeof(b.save)},

		{"REAL",4065,universal_TypeInt,(void *)&b.li.LineSelectionMode},
		
		{"REAL",4066,universal_TypeInt,(void *)&b.li.LineSourceMode},
		
		{"REAL",4067,universal_TypeInt,(void *)&b.li.pioIndex2},	
		{"STRI",4068,universal_TypeString,(void *)&b.li.pioName2[0],sizeof(b.li.pioName2)},
				
		{"STRI",4069,universal_TypeString,(void *)&b.Attributes.LineDash[0],sizeof(b.Attributes.LineDash)},
		
		{"REAL",4070,universal_TypeInt,(void *)&b.Attributes.DashFlag},	
		
		{"REAL",4071,universal_TypeInt,(void *)&b.Attributes.LineIncrement},	
		
		{"REAL",4072,universal_TypeInt,(void *)&b.li.integrate},
		
		{"DOUB",4073,universal_TypeDouble,(void *)&b.xShift},
		
		{"DOUB",4074,universal_TypeDouble,(void *)&b.yShift},
		
		{"DOUB",4075,universal_TypeDouble,(void *)&b.xScale},
		
		{"DOUB",4076,universal_TypeDouble,(void *)&b.yScale},
		

	};
	
	static struct universalTableStruct Global = {
		"uDraw parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	uLineDataPtr bp=(uLineDataPtr)o;
	static struct linedata li;
	char name[256];
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
		zerol((char *)&b,sizeof(b));
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		sprintf(name,"Line%03ld%c",l->ObjectIDCount++,0);
		bp=(uLineDataPtr)LineDataCreate(name,&li,&l->Attributes);
		if(!bp)goto  ErrorOut;
		bp->l=l;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(struct uLineData);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		if(bp->xScale == 0)bp->xScale=1.0;
		if(bp->yScale == 0)bp->yScale=1.0;
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
static DObjPtr LineDataCreate(char *name,struct linedata *li,struct uAttributes *Attributes)
{
	struct uLineData *Line;
	DObjPtr Object;
	rRect box;
	
	
	if(!name || !Attributes)return NULL;
	
	Object=NULL;
	
	if(li && li->sourceName[0]){
		if(!FindWindowByName(li->sourceName)){
		    if(!FileManagerGet(li->sourceName)){
				sprintf(WarningBuff,"LineDataCreate data source '%s' not found\n",li->sourceName);
				WarningBatch(WarningBuff);		
		    }
		}
	}

	
	zerol((char *)&box,sizeof(box));	
	
    Object=ObjectCreate(name,LineDataType,&box,sizeof(struct uLineData));
    if(!Object)goto ErrorOut;
    
    Line=(struct uLineData *)Object;
    
    Line->Attributes= *Attributes;    
    
	if(li && li->sourceName[0]){
	    if(li->pioName[0]){
	        if(li->pioIndex > 0){
		        sprintf(Line->save,"%s(%d)",li->pioName,(int)li->pioIndex);
	        }else{
		        sprintf(Line->save,"%s",li->pioName);
		    }
		}else{
		    sprintf(Line->save,"Range (%g,%g)",li->rymin,li->rymax);
		}
		Line->li= *li;
    }
    
    Line->xShift=0;
    Line->yShift=0;
    Line->xScale=1.0;
    Line->yScale=1.0;
       
    GridPlotLineSet(Line);
        
ErrorOut:	
	return Object;
}
int GridPlotLineSet(uLineDataPtr Line)
{
	if(!Line)return 1;
    Line->Kill=LineDataKill;
    Line->GetFrameCount=LineDataFrameCount;
    Line->SetFrame=LineDataSetFrame;    
    Line->ReadWrite=uLineWrite;    
	
	return 0;
}
static void LineDataKill(struct DObject *Object)
{
	struct uLineData *Line=(struct uLineData *)Object;
	
	if(!Line)return;
	
		
	if(Line->xData)cFree((char *)Line->xData);
	Line->xData=NULL;

	if(Line->yData)cFree((char *)Line->yData);
	Line->yData=NULL;

	Line->dCount=0;
		
	ObjectKill(Object);
	
}
int LineDataSetFrame(struct DObject *Object,long CurrentFrame)
{
	struct uLineData *Line=(struct uLineData *)Object;
	double xmin,xmax,ymin,ymax;
	struct linedata li;
	long n;
	int ret;
	
	if(!Line)return 1;	
	if(Line->ItWas == CurrentFrame)return 0;
	
	ret=1;
	
   if(Line->li.sourceName[0]){
    	zerol((char *)&li,sizeof(li));
    	
    	Line->ItWas = -1;
    	li = Line->li;
		Line->li.CurrentFrame=li.CurrentFrame=CurrentFrame;
				
		if(SendMessageByName(li.sourceName,MessageType_GET_LINE_DATA,&li)){
		    sprintf(WarningBuff,"source '%s' Name '%s' Index %ld Not Found x1 %g y1 %g z1 %g\n",
		           li.sourceName,li.pioName,li.pioIndex,li.x1,li.y1,li.z1);
		    WarningBatch(WarningBuff);
		    goto ErrorOut;
		}
				
		if(Line->xData)cFree((char *)Line->xData);
		Line->xData=li.xData;
		
		if(Line->yData)cFree((char *)Line->yData);
		Line->yData=li.yData;
		
		Line->dCount=li.dCount;

		Line->li.integral=li.integral;
		
		if(li.LineSourceMode == LINEDATA_CROSSPLOT_MODE){
		    li.xData=NULL;
		    li.yData=NULL;
		    li.dCount=0;

		    li.pioIndex=Line->li.pioIndex2;
			mstrncpy(li.pioName,Line->li.pioName2,64);						
    		    
			if(SendMessageByName(li.sourceName,MessageType_GET_LINE_DATA,&li)){
			    sprintf(WarningBuff,"source '%s' Name '%s' Index %ld Not Found x1 %g y1 %g z1 %g\n",
			           li.sourceName,li.pioName,li.pioIndex,li.x1,li.y1,li.z1);
			    WarningBatch(WarningBuff);
			    goto ErrorOut;
			}
		    
		    if(Line->dCount != li.dCount){
			    WarningBatch("LineDataSetFrame Cross Plot Count Error\n");
		    }
		    
			if(Line->xData)cFree((char *)Line->xData);
			Line->xData=li.yData;
			
		    		    
		}
				
		if(!Line->xData || !Line->yData || (li.dCount <= 0))goto ErrorOut;
		
		
		if(li.Polynomial == 1){
		    doPolynomial(li.xData,li.yData,li.dCount,li.PolynomialOrder,li.PolynomialPrint);
		} else if(li.Polynomial == 2){
		    doSmooth(li.xData,li.yData,li.dCount,li.PolynomialOrder,li.PolynomialPrint);
		} else if(li.Polynomial == 3){
		    doFourier(li.xData,li.yData,li.dCount,li.PolynomialOrder,li.PolynomialPrint);
		}
		
	}
	
	if(Line->xData && Line->yData && (Line->dCount > 0)){
		xmin=Line->xShift+Line->xScale*Line->xData[0];
		xmax=Line->xShift+Line->xScale*Line->xData[0];
		
		ymin=Line->yShift+Line->yScale*Line->yData[0];
		ymax=Line->yShift+Line->yScale*Line->yData[0];
		
		for(n=1;n<Line->dCount;++n){
		    if(Line->xShift+Line->xScale*Line->xData[n] < xmin){
		        xmin=Line->xShift+Line->xScale*Line->xData[n];
		    }else if(Line->xShift+Line->xScale*Line->xData[n] > xmax){
		        xmax=Line->xShift+Line->xScale*Line->xData[n];
		    }
		    
		    if(Line->yShift+Line->yScale*Line->yData[n] < ymin){
		        ymin=Line->yShift+Line->yScale*Line->yData[n];
		    }else if(Line->yShift+Line->yScale*Line->yData[n] > ymax){
		        ymax=Line->yShift+Line->yScale*Line->yData[n];
		    }
		}
		
		Line->xmin=xmin;
		Line->xmax=xmax;
		Line->ymin=ymin;
		Line->ymax=ymax;
		
		Line->ItWas=CurrentFrame;
	}
	ret=0;
ErrorOut:	
	return ret;
	
}
int LineDataFrameCount(struct DObject *Object,long *FrameCount)
{
	struct uLineData *Line=(struct uLineData *)Object;
	struct Message1000 mo;
	
	if(!Line || !FrameCount)return 1;

	
    if(Line->li.sourceName[0]){
		mo.FrameCount=0;
		SendMessageByName(Line->li.sourceName,MessageType_GET_FRAME_COUNT,&mo);
	    if((mo.FrameCount > 0) && (mo.FrameCount != Line->li.FrameCount)){
	        Line->li.FrameCount=mo.FrameCount;
	        Line->li.FirstFrame=0;
	        Line->li.LastFrame=mo.FrameCount-1;
	    }
	    if(mo.FrameCount > *FrameCount)*FrameCount=mo.FrameCount;
    }
	
	return 0;
	
}
