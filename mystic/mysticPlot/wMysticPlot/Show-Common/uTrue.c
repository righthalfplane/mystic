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


int uCStringWidth(char *name,IconPtr myIcon);

static int uTrueUpdate(struct DObject *o);

static int uTrueDrawAxis(uTruePtr b,DOListPtr d);

static char *uTrueCopy(DObjPtr o,long *Length);

static DObjPtr uTrueDoFlipDirection(DObjPtr o,int direct);

static DObjPtr uTrueDoRotateDirection(DObjPtr o,int direct);

static DObjPtr uTrueFlipDirection(uTruePtr b,int dold,int dnew);

static DObjPtr uTrueRotateDirection(uTruePtr b,int dold,int dnew);

int uTrueSet(uTruePtr b);

int sear(double cmax,double cmin,double *zcon,int *ncon,int nmax);

static void uTrueDelete(DObjPtr o);

static int uTrueMessage(DObjPtr o,long MessageType,void *MessageData);

static int uTrueReplaceImage(uTruePtr b,long xsizei,long ysizei,unsigned char *image);

static DObjPtr uTrueDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff);

static uTruePtr uTruedoBoxes(uTruePtr b,rRect *r,struct uAttributes *vAttributes);

int WriteRect24ToRect24(unsigned char *in, uRect *SRect,long xsize, long ysize,
                       unsigned char *out,uRect *DRect,long oxsize,long oysize);

static int uTrueDraw(DObjPtr o,struct DOList *d);

static int uTrueFrameCount(struct DObject *Object,long *FrameCount);

static int uTrueSetFrame(struct DObject *Object,long CurrentFrame);

static struct DObject *uTrueDoubleClick(struct DObject *Object,rPoint *r);

static DObjPtr uTrueDoInformation(DObjPtr o);

static int uTrueCreateTimeLabel(uTruePtr b);

static void uTrueGetFontSelected(DObjPtr o);
static void uTrueSetFontSelected(DObjPtr o);
static int uTrueSetSelection(uTruePtr b,DOListPtr d);
static void uTrueAttribute(DObjPtr o,struct uAttributes *Attributes,int Flag);
static struct DObject *uTrueFindObject(struct DObject *Object,rPoint *r);
static int uTrueSelectedPoint(DObjPtr o,rPoint *r);
static void uTrueSelectOne(DObjPtr o,rPoint *r);
static void uTrueDeSelectOne(DObjPtr o,rPoint *r);
static int uTrueDrawPalette(uTruePtr b,DOListPtr d);


int WriteRect8ToRect24(unsigned char *in, uRect *SRect,long xsize, long ysize,
                       unsigned char *out,uRect *DRect,long oxsize,long oysize,
                       unsigned char *palette);

static struct DObject *uTrueWrite(struct DObject *o,FILE8 *inOut,struct DOList *l)
{


	static struct uTrue b;
	
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

		{"REAL",4034,universal_TypeInt,(void *)&b.TrueType},
		{"REAL",4035,universal_TypeInt,(void *)&b.showPalette},
		

		{"REAL",4036,universal_TypeInt,(void *)&b.pAttributes.nLineFore},	
		{"REAL",4037,universal_TypeInt,(void *)&b.pAttributes.nLineBack},	
		{"REAL",4038,universal_TypeInt,(void *)&b.pAttributes.font.txFace},
		{"REAL",4039,universal_TypeInt,(void *)&b.pAttributes.font.txFont},
		{"REAL",4040,universal_TypeInt,(void *)&b.pAttributes.font.txSize},
		{"DOUB",4041,universal_TypeDouble,(void *)&b.pBoxOffset.x},	
		{"DOUB",4042,universal_TypeDouble,(void *)&b.pBoxOffset.y},	
		{"REAL",4043,universal_TypeInt,(void *)&b.showXaxis},
		{"REAL",4044,universal_TypeInt,(void *)&b.showYaxis},
		
		{"REAL",4045,universal_TypeInt,(void *)&b.Attributes.font.txFace},
		{"REAL",4046,universal_TypeInt,(void *)&b.Attributes.font.txFont},
		{"REAL",4047,universal_TypeInt,(void *)&b.Attributes.font.txSize},
		
		{"REAL",4048,universal_TypeInt,(void *)&b.countXaxis},
		{"REAL",4049,universal_TypeInt,(void *)&b.countYaxis},
		{"STRI",4050,universal_TypeString,(void *)&b.formatXaxis[0],sizeof(b.formatXaxis)},
		{"STRI",4051,universal_TypeString,(void *)&b.formatYaxis[0],sizeof(b.formatYaxis)},
		{"STRI",4052,universal_TypeString,(void *)&b.formatPalette[0],sizeof(b.formatPalette)},
		{"REAL",4053,universal_TypeInt,(void *)&b.countPalette},
				
		{"REAL",5010,universal_TypeInt,(void *)&b.pioIndex},	
		{"STRI",5011,universal_TypeString,(void *)&b.pioName[0],sizeof(b.pioName)},
		{"REAL",5012,universal_TypeInt,(void *)&b.plane.Plane_type},	
		{"REAL",5013,universal_TypeInt,(void *)&b.pa.DrawZones},	
		{"REAL",5014,universal_TypeInt,(void *)&b.pa.flagGradients},	
		{"REAL",5015,universal_TypeInt,(void *)&b.pd.LabeledPalette},	
		{"STRI",5016,universal_TypeBytes,(void *)&b.pd.palette[0],sizeof(b.pd.palette)},

		{"REAL",5017,universal_TypeInt,(void *)&b.pd.sType},	
		{"DOUB",5018,universal_TypeDouble,(void *)&b.pd.sPmin},	
		{"DOUB",5019,universal_TypeDouble,(void *)&b.pd.sPmax},	
		{"REAL",5020,universal_TypeInt,(void *)&b.pd.AreaFill},	
		{"REAL",5021,universal_TypeInt,(void *)&b.pa.ZoneLevel},	
		{"REAL",5022,universal_TypeInt,(void *)&b.pd.Rotate1d},
		
		{"DOUB",5023,universal_TypeDouble,(void *)&b.pd.red},	
		{"DOUB",5024,universal_TypeDouble,(void *)&b.pd.green},	
		{"DOUB",5025,universal_TypeDouble,(void *)&b.pd.blue},	
		{"REAL",5026,universal_TypeInt,(void *)&b.pa.DrawZonesColor},
		
		{"REAL",5027,universal_TypeInt,(void *)&b.pd.UsePlotScales},
		{"REAL",5028,universal_TypeInt,(void *)&b.pd.UsePlotTime},
		{"REAL",5029,universal_TypeInt,(void *)&b.pd.UsePlotRange},
		{"REAL",5030,universal_TypeInt,(void *)&b.pa.xReflect},
		
		{"DOUB",6000,universal_TypeDouble,(void *)&b.vector.arrowLength},	
		{"DOUB",6001,universal_TypeDouble,(void *)&b.vector.arrowAngle},	
		{"DOUB",6002,universal_TypeDouble,(void *)&b.vector.vspace},	
		{"DOUB",6003,universal_TypeDouble,(void *)&b.vector.vfac},	
		{"DOUB",6004,universal_TypeDouble,(void *)&b.vector.vmax},	
		{"REAL",6005,universal_TypeInt,(void *)&b.vector.DrawVectors},	
		{"REAL",6006,universal_TypeInt,(void *)&b.vector.arrowHeads},	
		
		{"STRI",6007,universal_TypeString,(void *)&b.vector.vectorxName[0],sizeof(b.vector.vectorxName)},
		{"STRI",6008,universal_TypeString,(void *)&b.vector.vectoryName[0],sizeof(b.vector.vectoryName)},
		{"STRI",6009,universal_TypeString,(void *)&b.vector.vectorzName[0],sizeof(b.vector.vectorzName)},
		
		
		{"DOUB",7000,universal_TypeDouble,(void *)&b.streamline.speedCutoff},	
		{"DOUB",7001,universal_TypeDouble,(void *)&b.streamline.error},	
		{"DOUB",7002,universal_TypeDouble,(void *)&b.streamline.xStart},	
		{"DOUB",7003,universal_TypeDouble,(void *)&b.streamline.yStart},	
		{"DOUB",7004,universal_TypeDouble,(void *)&b.streamline.xEnd},	
		{"DOUB",7005,universal_TypeDouble,(void *)&b.streamline.yEnd},	
		{"DOUB",7006,universal_TypeInt,(void *)&b.streamline.num},	
		{"DOUB",7007,universal_TypeInt,(void *)&b.streamline.dir},	
		{"DOUB",7008,universal_TypeInt,(void *)&b.streamline.maxCells},	
		{"DOUB",7009,universal_TypeInt,(void *)&b.streamline.maxSteps},	
		{"DOUB",7010,universal_TypeInt,(void *)&b.streamline.doGrad},	
		{"DOUB",7011,universal_TypeInt,(void *)&b.streamline.DrawStreamlines},	
		

		{"DOUB",8000,universal_TypeDouble,(void *)&b.plane.Plane_dmin},	
		{"DOUB",8001,universal_TypeDouble,(void *)&b.plane.Plane_dmax},	
		{"DOUB",8002,universal_TypeDouble,(void *)&b.plane.Plane_xmin},	
		{"DOUB",8003,universal_TypeDouble,(void *)&b.plane.Plane_xmax},	
		{"DOUB",8004,universal_TypeDouble,(void *)&b.plane.Plane_xloc},	
		{"DOUB",8005,universal_TypeDouble,(void *)&b.plane.Plane_ymin},	
		{"DOUB",8006,universal_TypeDouble,(void *)&b.plane.Plane_ymax},	
		{"DOUB",8007,universal_TypeDouble,(void *)&b.plane.Plane_yloc},	
		{"DOUB",8008,universal_TypeDouble,(void *)&b.plane.Plane_zmin},	
		{"DOUB",8009,universal_TypeDouble,(void *)&b.plane.Plane_zmax},	
		{"DOUB",8010,universal_TypeDouble,(void *)&b.plane.Plane_zloc},	
		
		{"DOUB",8011,universal_TypeInt,(void *)&b.plane.Plane_xstep},	
		{"DOUB",8012,universal_TypeInt,(void *)&b.plane.Plane_xcount},	
		{"DOUB",8013,universal_TypeInt,(void *)&b.plane.Plane_ystep},	
		{"DOUB",8014,universal_TypeInt,(void *)&b.plane.Plane_ycount},	
		{"DOUB",8015,universal_TypeInt,(void *)&b.plane.Plane_zstep},	
		{"DOUB",8016,universal_TypeInt,(void *)&b.plane.Plane_zcount},	
		{"DOUB",8017,universal_TypeInt,(void *)&b.plane.Plane_gstep},	
		{"DOUB",8018,universal_TypeInt,(void *)&b.plane.Plane_gcount},	
		
		
		{"DOUB",9000,universal_TypeDouble,(void *)&b.range.AspectRatio},	
		{"DOUB",9001,universal_TypeDouble,(void *)&b.range.xminData},	
		{"DOUB",9002,universal_TypeDouble,(void *)&b.range.xmaxData},	
		{"DOUB",9003,universal_TypeDouble,(void *)&b.range.yminData},	
		{"DOUB",9004,universal_TypeDouble,(void *)&b.range.ymaxData},	
		{"DOUB",9005,universal_TypeDouble,(void *)&b.range.zminData},	
		{"DOUB",9006,universal_TypeDouble,(void *)&b.range.zmaxData},	
		{"DOUB",9007,universal_TypeDouble,(void *)&b.range.xScale},	
		{"DOUB",9008,universal_TypeDouble,(void *)&b.range.yScale},	

		{"REAL",9009,universal_TypeLong,(void *)&b.range.ixminData},
		{"REAL",9010,universal_TypeLong,(void *)&b.range.ixmaxData},
		{"REAL",9011,universal_TypeLong,(void *)&b.range.iyminData},
		{"REAL",9012,universal_TypeLong,(void *)&b.range.iymaxData},
		{"REAL",9013,universal_TypeLong,(void *)&b.range.izminData},
		{"REAL",9014,universal_TypeLong,(void *)&b.range.izmaxData},
		
		{"REAL",9015,universal_TypeInt,(void *)&b.range.xminSet},
		{"REAL",9016,universal_TypeInt,(void *)&b.range.xmaxSet},
		{"REAL",9017,universal_TypeInt,(void *)&b.range.yminSet},
		{"REAL",9018,universal_TypeInt,(void *)&b.range.ymaxSet},
		{"REAL",9019,universal_TypeInt,(void *)&b.range.zminSet},
		{"REAL",9020,universal_TypeInt,(void *)&b.range.zmaxSet},
		
		{"REAL",9021,universal_TypeLong,(void *)&b.range.ixmax},
		{"REAL",9022,universal_TypeLong,(void *)&b.range.iymax},
		{"REAL",9023,universal_TypeLong,(void *)&b.range.izmax},
		
		{"REAL",9024,universal_TypeInt,(void *)&b.range.MaintainAspectRatio},
		{"REAL",9025,universal_TypeInt,(void *)&b.range.receiveRange},
	};
	
	static struct universalTableStruct Global = {
		"uDraw parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};
	

	uTruePtr bp=(uTruePtr)o;
	struct FilesSetList list;
	char BatchName[255];
	static int Count;
	char head[5];
	long length;
	long tag;
	
	if(!inOut || !l)return NULL;
	
	
	if(!bp){
		/* read and create */
	    Local[0].space=0;
	    zerol((char *)&b,sizeof(b));
	    
		mstrncpy(b.vector.vectorxName,"xdt",32);						
		mstrncpy(b.vector.vectoryName,"ydt",32);						
		mstrncpy(b.vector.vectorzName,"zdt",32);	
		
		b.countXaxis=5;
		b.countYaxis=5;
		b.countPalette=5;
		mstrncpy(b.formatXaxis,"%9.2e",sizeof(b.formatXaxis));
		mstrncpy(b.formatYaxis,"%9.2e",sizeof(b.formatYaxis));
		mstrncpy(b.formatPalette,"%9.2e",sizeof(b.formatPalette));
				
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp=uTrueTrue(b.xsize,b.ysize,&b.box,&b.Attributes,l);
		if(!bp)goto  ErrorOut;
		length=Local[0].space;
		if(length > 0 && b.dataPtr){
		    if(length >= b.xsize*b.ysize*3){
		        cFree((char *)bp->dataPtr);
		        bp->dataPtr=b.dataPtr;
		    }else{
		        cFree((char *)b.dataPtr);
		    }
		    b.dataPtr=NULL;
		}
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(struct uTrue);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;	
		if(uPref.Files){
			bp->Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(bp->Files){
				bp->Files->inputType=TRUE_EXPANDED;
			    if(bp->TrueType >= 7){
					sprintf(BatchName,"Pio2dDrawc_%03d",Count++);	
			    	mstrncat(BatchName,".b",255);	
			    	if(FileManagerAddToList(bp->Files,BatchName))goto ErrorOut;
					bp->Files->doNotAutoRemove=FALSE;
					mstrncpy(bp->BatchName,BatchName,64);		
			
				}
		        
		        if(b.pa.xReflect){
    				list.Reflect_x=b.pa.xReflect;
					FilesSetParameters(bp->Files,&list,FILES_SET_REFLECT_X);
		        }
		    }
		}		
		return (struct DObject *)bp;			
		
	}else{
	    b = *bp;
	    length=b.xsize*b.ysize*3;
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
static int uTrueSetFrame(struct DObject *Object,long CurrentFrame)
{
	uTruePtr b=(uTruePtr)Object;
    struct SetFrameData *s;
    unsigned char *data;
    int ret;
	
	if(!b)return 1;

	if(!b->Files)return 0;		
    if(!b->Attributes.Animation)return 0;
    if(!b->ReplaceImage)return 1;
    
    ret=1;
    
    data=NULL;
    
    if(b->TrueType < 7){		
		if(FilesSetFrame(CurrentFrame,b->Files))return 1;
		s = &b->Files->SetData;
		if((*b->ReplaceImage)(b,s->xsize,s->ysize,(unsigned char *)s->data)){
		    goto ErrorOut;
		}
	}else{
	
		s=&b->sd;		
		
		zerol((char *)s,sizeof(struct SetFrameData));
		
		s->CurrentFrame=CurrentFrame;
		s->type = FRAME_DATA_RGB;
		
		s->plane  =  b->plane;
		s->range  =  b->range;
		s->vector =  b->vector;
		s->pd 	  =  b->pd;
		s->pa 	  =  b->pa;

		s->typePlotRange = FRAME_RANGE_LIMITS;
				
		if(b->pioName){
			mstrncpy(s->pioName,b->pioName,255);
			s->pioIndex=b->pioIndex;			    
		}						
		
		if(SendMessageByName(b->BatchName,MessageType_GET_REGION_DATA,s)){
		    if(strcmp(b->pioName,"density")){
		        InvalRectMyWindow(b->l->myIcon);
		    	mstrncpy(b->pioName,"density",255);
		   	 	b->pioIndex=0;
		    }
    		data=s->data;
			goto ErrorOut;
		}		

    	data=s->data;

    	b->pd=s->pd;
    		
		if((*b->ReplaceImage)(b,s->xsize,s->ysize,(unsigned char *)s->data)){
		    goto ErrorOut;
		}
				
	}
	
    ret=0;
	
ErrorOut:	

	if(data)cFree((char *)data);
	
	return ret;
	
}
static struct DObject *uTrueDoubleClick(struct DObject *o,rPoint *r)
{
	if(!o || !r)return NULL;

	
	if(r->x >= o->box.x && r->y >= o->box.y){
		if(r->x <= o->box.x+o->box.xsize && r->y <= o->box.y+o->box.ysize){
			return uTrueDoInformation(o);
		}		
	}
		
	return NULL;
	
}
static int doScale2Action(struct uDialogStruct *uList)
{
	int actionItem;
	
	if(!uList)return 1;
	
	actionItem=(int)uList->actionItem;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(actionItem == 17){
		uDialogSet(uList,16L,
		              uDialogSetFloatAux,(double)0.0,
		              uDialogSetExitOk,
		              uDialogSetDone,uDialogSetDone
		);
	}else if(actionItem == 18){
		uDialogSet(uList,16L,
		              uDialogSetFloatAux,(double)1.0,
		              uDialogSetExitOk,
		              uDialogSetDone,uDialogSetDone
		);
	}else if(actionItem == 19){
		uDialogSet(uList,16L,
		              uDialogSetFloatAux,(double)2.0,
		              uDialogSetExitOk,
		              uDialogSetDone,uDialogSetDone
		);
	}else if(actionItem == 20){
		uDialogSet(uList,16L,
		              uDialogSetFloatAux,(double)3.0,
		              uDialogSetExitOk,
		              uDialogSetDone,uDialogSetDone
		);
	}else if(actionItem == 21){
		uDialogSet(uList,16L,
		              uDialogSetFloatAux,(double)4.0,
		              uDialogSetExitOk,
		              uDialogSetDone,uDialogSetDone
		);
	}else if(actionItem == 22){
		uDialogSet(uList,16L,
		              uDialogSetFloatAux,(double)5.0,
		              uDialogSetExitOk,
		              uDialogSetDone,uDialogSetDone
		);
	}else if(actionItem == 23){
		uDialogSet(uList,16L,
		              uDialogSetFloatAux,(double)6.0,
		              uDialogSetExitOk,
		              uDialogSetDone,uDialogSetDone
		);
	}

	if(uList)uDialogUpdate(uList);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}

static DObjPtr uTrueDoInformation(DObjPtr o)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {240,255,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {335,255,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"0", {115,50,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"10", {115,75,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"1", {115,100,80,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"1", {115,125,80,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"Image", {223,11,80,20},uDialogText,uDialogNone},
      /*  7 */      {"0", {315,50,80,20},uDialogText,uDialogGetDouble},
      /*  8 */      {"0", {315,75,80,20},uDialogText,uDialogGetDouble},
      /*  9 */      {"Left        :", {25,75,70,20},uDialogString,uDialogNone},
      /* 10 */      {"Object Type", {75,9,90,20},uDialogString,uDialogNone},
      /* 11 */      {"Height    :", {25,100,70,20},uDialogString,uDialogNone},
      /* 12 */      {"Width     :", {25,125,70,20},uDialogString,uDialogNone},
      /* 13 */      {"Top         :", {25,50,70,20},uDialogString,uDialogNone},
      /* 14 */      {"x-scale  :", {220,50,65,16},uDialogString,uDialogNone},
      /* 15 */      {"y-scale  :", {220,75,80,20},uDialogString,uDialogNone},
      /* 16 */      {"last", {69,70,169,19},uDialogFloatAux,uDialogFloatAux},
      /* 17 */      {"Data Selection", {240,160,125,20},uDialogButton,uDialogNone},
      /* 18 */      {"Data Range", {240,100,125,20},uDialogButton,uDialogNone},
      /* 19 */      {"Data Domain", {240,130,125,20},uDialogButton,uDialogNone},
      /* 20 */      {"Data Vectors", {240,190,125,20},uDialogButton,uDialogNone},
      /* 21 */      {"Data Streamlines", {240,220,125,20},uDialogButton,uDialogNone},
      /* 22 */      {"Data Plane", {240,225,120,20},uDialogButton,uDialogNone},
      /* 23 */      {"Time Label", {20,160,125,20},uDialogButton,uDialogNone},
      /* 24 */      {"Show Palette",{20,190,100,20},uDialogCheck,uDialogGetLong},
      /* 25 */      {"Show x-Axis",{20,215,100,20},uDialogCheck,uDialogGetLong},
      /* 26 */      {"Show y-Axis",{20,240,100,20},uDialogCheck,uDialogGetLong},
      /* 27 */      {"Count x-Axis",{135,215,30,20},uDialogText,uDialogGetLong},
      /* 28 */      {"Count y-Axis",{135,240,30,20},uDialogText,uDialogGetLong},
      /* 29 */      {"x-Axis",{171,215,55,20},uDialogText,uDialogNone},
      /* 30 */      {"y-Axis",{171,240,55,20},uDialogText,uDialogNone},
      /* 31 */      {"palette-Axis",{171,190,55,20},uDialogText,uDialogNone},
      /* 32 */      {"Countpalette",{135,190,30,20},uDialogText,uDialogGetLong},
      
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,410,300};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
 	int type;
    int ret;
    
	static char iTrue[16]={'T','r','u','e','C','o','l','o','r'};
	static char RGB[16]={'R','G','B'};
	static char JPEG[16]={'J','P','E','G'};
	static char BMP[16]={'B','M','P'};
	static char GIF[16]={'G','I','F'};
	static char PICT[16]={'P','I','C','T'};
	static char QUICK[16]={'Q','u','i','c','k','T','i','m','e',};
	static char PIO2D[16]={'P','I','O','2','D'};
	static char PIO1D[16]={'P','I','O','1','D'};
	static char PIO3D[16]={'P','I','O','3','D'};
	static char SDS2D[16]={'S','D','S','2','D'};
	static char SDS3D[16]={'S','D','S','3','D'};
	static char HDF[16]={'H','D','F'};
	static char *Type[]={iTrue,RGB,JPEG,BMP,GIF,PICT,QUICK,PIO2D,PIO1D,PIO3D,
	                     SDS2D,SDS3D,HDF};
	char *llist;
	uTruePtr pBox;
	uTruePtr b=(uTruePtr)o;
	double xscale,yscale;
	double xsize,ysize;
	struct DOList *l;
	int dreturn;
	
	if(!b)return NULL;
	l=b->l;
	if(!l)return NULL;
	
	uGetPort(&save);
		            
	if(b->TrueType >= 0 && b->TrueType <= 12){
	    llist=Type[b->TrueType];
	}else{
		llist="UnKnown";
	}
	
	uTrueSetFrame(o,0L);
	
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
	
	
	ret = 1;
	
	uR=NULL;
	
	pBox=NULL;
	
	uList=uDialogOpen("Image Information",&Rect1,ItemCount);
	
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
			              
			uDialogSetItem,6L,
			uDialogSetText,llist,
			              

			uDialogSetItem,7L,
			uDialogSetDouble,(double)xscale,
			              

			uDialogSetItem,8L,
			uDialogSetDouble,(double)yscale,
			              

			uDialogSetItem,16L,
			uDialogSetFloatAux,(double)-1,

			uDialogSetItem,17L,
			uDialogSetHidden,(int)(b->TrueType < 7),
			              
			uDialogSetItem,18L,
			uDialogSetHidden,(int)(b->TrueType < 7),

			uDialogSetItem,19L,
			uDialogSetHidden,(int)(b->TrueType < 7),

			uDialogSetItem,20L,
			uDialogSetHidden,(int)(b->TrueType < 7),

			uDialogSetItem,21L,
			uDialogSetHidden,(int)(b->TrueType < 7 || (b->TrueType == 9 || b->TrueType == 11)),

			uDialogSetItem,22L,
			uDialogSetHidden,(int)(b->TrueType != 9 && b->TrueType != 11),

			uDialogSetItem,23L,
			uDialogSetHidden,(int)(b->TrueType < 7),

			uDialogSetItem,24L,
			uDialogSetHidden,(int)(b->TrueType < 7),
	        uDialogSetLogical,(int)b->showPalette,

			uDialogSetItem,25L,
			uDialogSetHidden,(int)(b->TrueType < 7),
	        uDialogSetLogical,(int)b->showXaxis,

			uDialogSetItem,26L,
			uDialogSetHidden,(int)(b->TrueType < 7),
	        uDialogSetLogical,(int)b->showYaxis,

			uDialogSetItem,27L,
			uDialogSetHidden,(int)(b->TrueType < 7),
			uDialogSetDouble,(double)b->countXaxis,

			uDialogSetItem,28L,
			uDialogSetHidden,(int)(b->TrueType < 7),
			uDialogSetDouble,(double)b->countYaxis,

			uDialogSetItem,29L,
			uDialogSetHidden,(int)(b->TrueType < 7),
	        uDialogSetText,b->formatXaxis,

			uDialogSetItem,30L,
			uDialogSetHidden,(int)(b->TrueType < 7),
	        uDialogSetText,b->formatYaxis,

			uDialogSetItem,31L,
			uDialogSetHidden,(int)(b->TrueType < 7),
	        uDialogSetText,b->formatPalette,

			uDialogSetItem,32L,
			uDialogSetHidden,(int)(b->TrueType < 7),
			uDialogSetDouble,(double)b->countPalette,

			uDialogSetAction,doScale2Action,

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
	
	if((pBox=uTruedoBoxes(b,&b->box,&b->Attributes)) == NULL){
	    goto ErrorOut;
	}
	
	pBox->box.x=uR[3].dreturn;
	pBox->box.y=uR[2].dreturn;
	
	pBox->box.xsize=xsize;
	pBox->box.ysize=ysize;
	
	dreturn=(int)uR[16].dreturn;

	pBox->showPalette=(int)uR[24].lreturn;
	
	pBox->showXaxis=(int)uR[25].lreturn;
	
	pBox->showYaxis=(int)uR[26].lreturn;
	
	pBox->countXaxis=(int)uR[27].lreturn;
	
	pBox->countYaxis=(int)uR[28].lreturn;
	
	mstrncpy(pBox->formatXaxis,uR[29].sreturn,255);
	mstrncpy(pBox->formatYaxis,uR[30].sreturn,255);
	mstrncpy(pBox->formatPalette,uR[31].sreturn,255);
	
	pBox->countPalette=(int)uR[32].lreturn;
	
    if(uList)uDialogClose(uList);
    uList=NULL;
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    	
	if(dreturn == 0){
	
		type = MessageType_DO_SELECTION_DIALOG;
		if(SendMessageByName(pBox->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}

		if(SendMessageByName(pBox->BatchName,MessageType_GET_INFORMATION,&pBox->sd)){
			goto ErrorOut;
		}

		if(pBox->sd.pioName[0]){
			mstrncpy(pBox->pioName,pBox->sd.pioName,255);
			pBox->pioIndex=pBox->sd.pioIndex;			    
		    if(!pBox->pa.DrawZones)pBox->pd.AreaFill=TRUE;
		}						
	
    	
	}else if(dreturn == 1){
	
		type = MessageType_DO_SCALES_DIALOG;
		if(SendMessageByName(pBox->BatchName,MessageType_DO_DIALOG,&type)){
			goto ErrorOut;
		}
		if(SendMessageByName(pBox->BatchName,MessageType_GET_INFORMATION,&pBox->sd)){
			goto ErrorOut;
		}
		pBox->pd=pBox->sd.pd;
		pBox->pa=pBox->sd.pa;
		pBox->range=pBox->sd.range;
	}else if(dreturn == 2){
		type = MessageType_DO_RANGE_DIALOG;
		if(SendMessageByName(pBox->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
		if(SendMessageByName(pBox->BatchName,MessageType_GET_INFORMATION,&pBox->sd)){
			goto ErrorOut;
		}
		pBox->range=pBox->sd.range;	
	}else if(dreturn == 3){
		type = MessageType_DO_VECTOR_DIALOG;
		if(SendMessageByName(pBox->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
		if(SendMessageByName(pBox->BatchName,MessageType_GET_INFORMATION,&pBox->sd)){
			goto ErrorOut;
		}
		pBox->vector=pBox->sd.vector;
	}else if(dreturn == 4){
		type = MessageType_DO_STREAMLINE_DIALOG;
		if(SendMessageByName(pBox->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
		if(SendMessageByName(pBox->BatchName,MessageType_GET_INFORMATION,&pBox->sd)){
			goto ErrorOut;
		}
		pBox->streamline=pBox->sd.streamline;
	}else if(dreturn == 5){
		type = MessageType_DO_PLANE_DIALOG;
		if(SendMessageByName(pBox->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
		if(SendMessageByName(pBox->BatchName,MessageType_GET_INFORMATION,&pBox->sd)){
			goto ErrorOut;
		}
		pBox->plane=pBox->sd.plane;
		pBox->pd=pBox->sd.pd;	
		pBox->pa=pBox->sd.pa;	
	}else if(dreturn == 6){	    
	    uTrueCreateTimeLabel(pBox);	    
	    uTrueDelete((DObjPtr)pBox);
	    pBox=NULL;
	    
	}
		
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	
	if(ret && pBox){
		uTrueDelete((DObjPtr)pBox);
		pBox=NULL;
	}
	
	uSetPort(&save);
	
	return (DObjPtr)pBox;
}
static int uTrueCreateTimeLabel(uTruePtr b)
{
	uAlphaPtr Alpha;
	DOListPtr l;
	uPoint where;
	
	if(!b)return 1;
	l=b->l;
	if(!l)return 1;

	DOListDeSelectAll(l);
		
	where.x=(int)b->box.x;
	where.y=(int)b->box.y;
	
	if(DrawSetEditAlpha(l,&where))return 1;
	
	Alpha=l->Alpha;
	if(!Alpha)return 1;
	if(!l->Edit)return 1;
	
	mstrncpy(l->Edit->text,"Time",sizeof(l->Edit->text));
	l->Edit->length=strlen(l->Edit->text);
	DOListSaveAlpha(l,(DObjPtr)Alpha);
	Alpha->box.x=b->box.x;
	Alpha->box.y=b->box.y-.2;
	
	Alpha->Files=b->Files;
	Alpha->scale=1;
	if(Alpha->Files){
		Alpha->Attributes.Animation=TRUE;
	}
	mstrncpy(Alpha->Attributes.AnimationFormat,"%g",255);
	
	Alpha->Selected=TRUE;
		
	return 0;
}
static int uTrueFrameCount(struct DObject *Object,long *FrameCount)
{
	uTruePtr b=(uTruePtr)Object;
	
	if(!b && !FrameCount)return 1;
	
	if(b->Files){
	    if(b->Files->ImageCount > *FrameCount)*FrameCount=b->Files->ImageCount;
	}
	
	return 0;
	
}
static int uTrueDraw(DObjPtr o,struct DOList *d)
{
	uRect SRect,DRect;
	uTruePtr b=(uTruePtr)o;
	rRect r;

	if(!b || !d)return 1;
	
	r=b->box;
	    
	SRect.x=0;
	SRect.y=0;
	    
	SRect.xsize=(int)b->xsize;
	SRect.ysize=(int)b->ysize;
	    
	DRect.x=LocalToPixel(r.x,d);
	DRect.y=LocalToPixel(r.y,d);
	    
	DRect.xsize=LocalToPixel(r.xsize,d);
	DRect.ysize=LocalToPixel(r.ysize,d);
	    
	WriteRect24ToRect24(b->dataPtr,&SRect,b->xsize, b->ysize,
                       d->data,&DRect,d->xsize,d->ysize);	  
                       
    if(uTrueSetSelection(b,d))return 1;
    
    uTrueDrawPalette(b,d);
    
    uTrueDrawAxis(b,d);

	return 0;
}

static int uTrueDrawAxis(uTruePtr b,DOListPtr d)
{
    double cmin,cmax,dz,dh;
    double zcon[200];
    int ncon,n;
	uFontInfo Info;
	double xh,yh;
	char out[256];
	
	if(!b || !d)return 1;
	
	
	if(b->showXaxis || b->showYaxis){
	
		uSetAttributes(d,&b->Attributes,lPattern | lForeColor | lBackColor | tFont  | lWidth);
		
		uGetFontInfo(d->myIcon,&Info);		
		xh=PixelToLocal(Info.CWidth,d);
		yh=PixelToLocal(Info.LineHeight,d);		

		rFrameRect(&b->box,d);
	    
	    if(b->showXaxis){
		    cmin=b->sd.limits.xmin;
		    cmax=b->sd.limits.xmax;

		    if(cmin >= cmax)cmax=cmin+1.0;

		    sear(cmax,cmin,zcon,&ncon,b->countXaxis);
		    dz=cmax-cmin;
		    for(n=0;n<ncon;++n){
		        if(zcon[n] > (cmax+1.0e-4*dz)){
		            break;
		        }else if(zcon[n] < cmin-1.0e-4*dz){
		            continue;
		        }
		        dh=(zcon[n]-cmin)/dz;
		        
				rMoveTo(b->box.x+dh*b->box.xsize,b->box.y+b->box.ysize,d);	
				rLineTo(b->box.x+dh*b->box.xsize,b->box.y+b->box.ysize+.05,d);	
	        
			    sprintf((char *)out,b->formatXaxis,zcon[n]);
	        	xh=PixelToLocal(uCStringWidth(out,d->myIcon),d);

				rMoveTo(b->box.x+dh*b->box.xsize-xh*.5,b->box.y+b->box.ysize+.1+yh,d);	
		    	rDrawString(out,d);	
		    }	    
	    }
	    if(b->showYaxis){

		    cmin=b->sd.limits.ymin;
		    cmax=b->sd.limits.ymax;

		    if(cmin >= cmax)cmax=cmin+1.0;

		    sear(cmax,cmin,zcon,&ncon,b->countYaxis);
		    dz=cmax-cmin;
		    for(n=0;n<ncon;++n){
		        if(zcon[n] > (cmax+1.0e-4*dz)){
		            break;
		        }else if(zcon[n] < cmin-1.0e-4*dz){
		            continue;
		        }
		        dh=(zcon[n]-cmin)/dz;
		        
				rMoveTo(b->box.x-.05,b->box.y+b->box.ysize-dh*b->box.ysize,d);	
				rLineTo(b->box.x,b->box.y+b->box.ysize-dh*b->box.ysize,d);	
	        
			    sprintf((char *)out,b->formatYaxis,zcon[n]);
	        	xh=PixelToLocal(uCStringWidth(out,d->myIcon),d);

				rMoveTo(b->box.x-.15-xh,b->box.y+b->box.ysize-dh*b->box.ysize+.5*yh,d);	
		    	rDrawString(out,d);	
		    }	    
	    }
	    

	}
	
	return 0;
}

static int uTrueDrawPalette(uTruePtr b,DOListPtr d)
{
	struct paletteDraw *pd;
	char out[256];
	uRect SRect,DRect;
	static int b2lists[6]={1,0,0,0,1,0};
	double Small=1.0e-6;
	uFontInfo Info;
	double xh,yh;
	rRect r;
	
	if(!b || !d)return 1;
	
	if(b->showPalette){
	    if(!b->pBoxPtr){
	    	DOListBuildColorBar(d,&b->pBoxPtr,&b->pBoxSource,b2lists);	    
	    	b->pBoxSource.xsize *= d->dpi;
	    	b->pBoxSource.ysize *= d->dpi;
	    }
	
		r=b->pBoxes;
		    
		SRect.x=0;
		SRect.y=0;
		    
		SRect.xsize=(int)b->pBoxSource.xsize;
		SRect.ysize=(int)b->pBoxSource.ysize;
		    
		DRect.x=LocalToPixel(r.x,d);
		DRect.y=LocalToPixel(r.y,d);
		    
		DRect.xsize=LocalToPixel(r.xsize,d);
		DRect.ysize=LocalToPixel(r.ysize,d);
	    
		uSetAttributes(d,&b->pAttributes,lPattern | lForeColor | lBackColor | tFont  | lWidth);
	    
		if(b->pBoxPtr){
			WriteRect8ToRect24(b->pBoxPtr,&SRect,(long)b->pBoxSource.xsize,(long)b->pBoxSource.ysize,
                       d->data,&DRect,d->xsize,d->ysize,
                       (unsigned char *)b->pd.palette);	  
		}
		rFrameRect(&b->pBoxes,d);
		
		pd=&b->pd;
		uGetFontInfo(d->myIcon,&Info);		
		xh=PixelToLocal(Info.CWidth,d);
		yh=PixelToLocal(Info.LineHeight,d);		
		
		if(pd->sType == 0 || pd->sType == 2){
		    double cmin,cmax,dz,dh;
		    double zcon[200];
		    int ncon,n;

		    if(pd->sType == 0){
		        cmin=pd->dmin;
		        cmax=pd->dmax;
		    }else{
		        cmin=pd->sPmin;
		        cmax=pd->sPmax;
		    }

		    if(cmin >= cmax)cmax=cmin+1.0;

		    sear(cmax,cmin,zcon,&ncon,b->countPalette);
		    dz=cmax-cmin;
		    for(n=0;n<ncon;++n){
		        if(zcon[n] > (cmax+1.0e-4*dz)){
		            break;
		        }else if(zcon[n] < cmin-1.0e-4*dz){
		            continue;
		        }
		        dh=(zcon[n]-cmin)/dz;
		        
				rMoveTo(b->pBoxes.x-.05,b->pBoxes.y+b->pBoxes.ysize-dh*b->pBoxes.ysize,d);	
				rLineTo(b->pBoxes.x,b->pBoxes.y+b->pBoxes.ysize-dh*b->pBoxes.ysize,d);	
	        
			    sprintf((char *)out,b->formatPalette,zcon[n]);
	        	xh=PixelToLocal(uCStringWidth(out,d->myIcon),d);

				rMoveTo(b->pBoxes.x-.1-xh,b->pBoxes.y+b->pBoxes.ysize-dh*b->pBoxes.ysize+.5*yh,d);	
		    	rDrawString(out,d);		    
		    }
		  }else if(pd->sType == 1){
		    double y,End,Start,Step,yViewMinLog,yViewMaxLog,scale,slide;
		    int iEnd,iStart,iDiff;
		    double ys,iy,ysize;

		    ys=b->pBoxes.y;
		    ysize=b->pBoxes.ysize;

		    yViewMaxLog=log10(pd->sPmax);
		    if(yViewMaxLog < 0.0){
		       End=yViewMaxLog*(1-Small);
		    }else{
		       End=yViewMaxLog*(1+Small);
		    }


		    yViewMinLog=log10(pd->sPmin);
		    if(yViewMinLog < 0.0){
		       slide=yViewMinLog*(1+Small);
		    }else{
		       slide=yViewMinLog*(1-Small);
		    }
		    iEnd=(int)End;
		    iStart=(int)(yViewMinLog-.9999);
		    iDiff=iEnd-iStart;
		    if((iDiff) <= 9){
		        Step=1;
		    }else{
		        Step=4;
		    }
		    Start=iStart;
		    scale=ysize/(log10(pd->sPmax)-yViewMinLog);
	  	    for(y=Start;y <= End;y += Step){
	  	        double yy,yn;
		        if(y > End)break;	   
		        if(y < slide)continue;
			    iy=(ys+ysize-scale*(y-yViewMinLog));
		        sprintf((char *)out,"%g%c",pow(10.,y),0);
		        
	        	xh=PixelToLocal(uCStringWidth(out,d->myIcon),d);

				rMoveTo(b->pBoxes.x-.1-xh,iy+.5*yh,d);	
		    	rDrawString(out,d);		    
		        
				rMoveTo(b->pBoxes.x-.05,iy,d);	
				rLineTo(b->pBoxes.x,iy,d);	
		        
		        if(iDiff > 4)continue;
		        for(yn=2;yn < 10;++yn){
		            yy=y+log10(yn);
		            if(yy > End)break;
			        iy=(ys+ysize-scale*(yy-yViewMinLog));
					rMoveTo(b->pBoxes.x-.025,iy,d);	
					rLineTo(b->pBoxes.x,iy,d);	
			    }
		    }
		  }
		
		
		
		
	
	}
	return 0;
}
static int uTrueSetSelection(uTruePtr b,DOListPtr d)
{
	IconPtr myIcon;
	drawPtr w;
	rRect r;
	
	if(!b || !d)return 1;
	myIcon=d->myIcon;
	w=NULL;
	if(myIcon){
		w=(drawPtr)myIcon->DWindow;
	}
	if(b->showPalette){
	    r.x=b->box.x+b->box.xsize+.1+b->pBoxOffset.x;
	    r.y=b->box.y+b->pBoxOffset.y;
	    r.xsize=.25;
	    r.ysize=3.56;
	    if(w && w->AntiAlias){
	       r.xsize *= 2;
	       r.ysize *= 2;
	    }
	    b->pBoxeSelect=b->pBoxes=r;
	}
	
	
	return 0;
}

static DObjPtr uTrueMove(DObjPtr o,double *dx,double *dy)
{
	uTruePtr b=(uTruePtr)o;
	double dxl=0,dyl=0;
	uTruePtr pBox;
	
	if(!b || !dx || !dy)return NULL;
	
	if(b->iSelected){
	    if((pBox=(uTruePtr)uTrueDuplicate(o,0,dx,dy)) != NULL){
	        pBox->xDup = b->xDup;
	        pBox->yDup = b->yDup;
	    }
	    return (DObjPtr)pBox;
	}else{
	    if((pBox=(uTruePtr)uTrueDuplicate(o,0,&dxl,&dyl)) != NULL){
	        pBox->xDup = b->xDup;
	        pBox->yDup = b->yDup;
	    }else{
	    	return (DObjPtr)pBox;
	    }
	}
	
	if(b->pSelected){
	    pBox->pBoxOffset.x += *dx;
	    pBox->pBoxOffset.y += *dy;
	}
	
	return (DObjPtr)pBox;
}

static uTruePtr uTruedoBoxes(uTruePtr b,rRect *r,struct uAttributes *vAttributes)
{
	uTruePtr iTrue;
	
	if(!b || !r || !vAttributes)return NULL;
	
	iTrue = uTrueTrue(b->xsize,b->ysize,r,vAttributes,b->l);
	if(iTrue == NULL){
	    WarningBatch("uTruedoBoxes Out of Memory\n");
	    return NULL;
	}
	uMemcpy(iTrue->dataPtr,b->dataPtr,(long)b->xsize*b->ysize*3);
	iTrue->box.x = b->box.x;
	iTrue->box.y = b->box.y;
	iTrue->box.xsize = b->box.xsize;
	iTrue->box.ysize = b->box.ysize;
	iTrue->AngleFlag = b->AngleFlag;
	iTrue->FlipFlag = b->FlipFlag;
	iTrue->Files = b->Files;
	iTrue->TrueType = b->TrueType;
	iTrue->plane  =  b->plane;
	iTrue->range  =  b->range;
	iTrue->vector =  b->vector;
	iTrue->pa 	   =  b->pa;
	iTrue->pd 	   =  b->pd;
	iTrue->sd 	   =  b->sd;
	mstrncpy(iTrue->BatchName,b->BatchName,sizeof(iTrue->BatchName));
	mstrncpy(iTrue->pioName,b->pioName,sizeof(iTrue->pioName));
	iTrue->pioIndex=b->pioIndex;			    

	iTrue->pAttributes=b->pAttributes;		
	iTrue->Attributes=b->Attributes;		
	    
	iTrue->pBoxeSelect=b->pBoxeSelect;			    
	iTrue->pBoxOffset=b->pBoxOffset;			    
	iTrue->pBoxes=b->pBoxes;			    
	iTrue->pCount=b->pCount;			    
	iTrue->pSelected=b->pSelected;			    
	iTrue->iSelected=b->iSelected;			    
	iTrue->showPalette=b->showPalette;		
	if(!b->showPalette)iTrue->pSelected=FALSE;	    
	iTrue->showXaxis=b->showXaxis;			    
	iTrue->showYaxis=b->showYaxis;			    
	iTrue->countXaxis=b->countXaxis;			    
	iTrue->countYaxis=b->countYaxis;			    
	iTrue->countPalette=b->countPalette;			    
	mstrncpy(iTrue->formatXaxis,b->formatXaxis,sizeof(iTrue->formatXaxis));
	mstrncpy(iTrue->formatYaxis,b->formatYaxis,sizeof(iTrue->formatYaxis));
	mstrncpy(iTrue->formatPalette,b->formatPalette,sizeof(iTrue->formatPalette));

	if(iTrue->ReplaceImage)(*iTrue->ReplaceImage)(iTrue,b->xsize,b->ysize,(unsigned char *)b->dataPtr);
	return iTrue;
}


static DObjPtr uTrueDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff)
{
	uTruePtr pImage;
	double x,y;
	uTruePtr b=(uTruePtr)o;

	if(!b || !xOff || !yOff)return NULL;
	
	if((pImage=uTruedoBoxes(b,&b->box,&b->Attributes)) == NULL){
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

static void uTrueSetFontSelected(DObjPtr o)
{
	uTruePtr b=(uTruePtr)o;
	
	if(!b || !b->l)return;
	
	if(b->pSelected){
	    uSetFontStuff(&b->pAttributes.font,b->l->myIcon);
	}
	
	if(b->iSelected){
	    uSetFontStuff(&b->Attributes.font,b->l->myIcon);
	}
}
static void uTrueGetFontSelected(DObjPtr o)
{
	uTruePtr b=(uTruePtr)o;
	
	if(!b || !b->l)return;
	
	
	if(b->pSelected){
	    uGetFontStuff(&b->pAttributes.font,b->l->myIcon);
	}

	if(b->iSelected){
	    uGetFontStuff(&b->Attributes.font,b->l->myIcon);
	}
		
}


static DObjPtr uTrueDoRotateDirection(DObjPtr o,int direct)
{
	uTruePtr b=(uTruePtr)o;

	if(!b)return NULL;

	if(b->AngleFlag == direct)return NULL;
	return uTrueRotateDirection(b,b->AngleFlag,direct);
}
static DObjPtr uTrueDoFlipDirection(DObjPtr o,int direct)
{
	uTruePtr b=(uTruePtr)o;

	if(!b)return NULL;
	
	if(b->FlipFlag == direct)return NULL;
	return uTrueFlipDirection(b,b->FlipFlag,direct);
}
static DObjPtr uTrueRotateDirection(uTruePtr b,int dold,int dnew)
{
	uTruePtr pBox;
	double dx=0,dy=0;
	int xx,yy;
	long xxsize,yysize;
    unsigned char  *ddataPtr;
    double xsize,ysize;
    
    if(!b)return NULL;
	
	if(dold == dnew)return NULL;
	
	if((pBox=(uTruePtr)uTrueDuplicate((DObjPtr)b,0,&dx,&dy)) != NULL){
	    pBox->AngleFlag = dnew;
	    pBox->FlipFlag = b->FlipFlag;
	    xxsize=pBox->xsize;
	    yysize=pBox->ysize;
	    ddataPtr=pBox->dataPtr;
	    xsize=pBox->box.xsize;
	    ysize=pBox->box.ysize;
	}else{
	    return NULL;
	}
	if(dold == 0){
	    if(dnew == 1){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[(xxsize-1-xx)*yysize*3+yy*3]=b->dataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*yysize*3+yy*3+1]=b->dataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*yysize*3+yy*3+2]=b->dataPtr[xx*3+2+yy*xxsize*3];
	            }
	        }
	    }else if(dnew == 2){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[3*(xxsize-1-xx)+(yysize-1-yy)*xxsize*3]=b->dataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[3*(xxsize-1-xx)+1+(yysize-1-yy)*xxsize*3]=b->dataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[3*(xxsize-1-xx)+2+(yysize-1-yy)*xxsize*3]=b->dataPtr[xx*3+2+yy*xxsize*3];
	            }
	        }
	    }else if(dnew == 3){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[xx*yysize*3+3*(yysize-1-yy)]=b->dataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[xx*yysize*3+3*(yysize-1-yy)+1]=b->dataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[xx*yysize*3+3*(yysize-1-yy)+2]=b->dataPtr[xx*3+2+yy*xxsize*3];
	            }
	        }
	    }
	}else if(dold == 1){
	    if(dnew == 0){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[xx*yysize*3+3*(yysize-1-yy)]=b->dataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[xx*yysize*3+3*(yysize-1-yy)+1]=b->dataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[xx*yysize*3+3*(yysize-1-yy)+2]=b->dataPtr[xx*3+2+yy*xxsize*3];
	            }
	        }
	    }else if(dnew == 2){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[(xxsize-1-xx)*yysize*3+yy*3]=b->dataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*yysize*3+yy*3+1]=b->dataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*yysize*3+yy*3+2]=b->dataPtr[xx*3+2+yy*xxsize*3];
	            }
	        }
	    }else if(dnew == 3){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[3*(xxsize-1-xx)+(yysize-1-yy)*xxsize*3]=b->dataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[3*(xxsize-1-xx)+1+(yysize-1-yy)*xxsize*3]=b->dataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[3*(xxsize-1-xx)+2+(yysize-1-yy)*xxsize*3]=b->dataPtr[xx*3+2+yy*xxsize*3];
	            }
	        }
	    }
	}else if( dold == 2){
	    if(dnew == 0){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[3*(xxsize-1-xx)+(yysize-1-yy)*xxsize*3]=b->dataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[3*(xxsize-1-xx)+1+(yysize-1-yy)*xxsize*3]=b->dataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[3*(xxsize-1-xx)+2+(yysize-1-yy)*xxsize*3]=b->dataPtr[xx*3+2+yy*xxsize*3];
	            }
	        }
	    }else if(dnew == 1){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[xx*yysize*3+(yysize-1-yy)*3]=b->dataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[xx*yysize*3+(yysize-1-yy)*3+1]=b->dataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[xx*yysize*3+(yysize-1-yy)*3+2]=b->dataPtr[xx*3+2+yy*xxsize*3];
	            }
	        }
	    }else if(dnew == 3){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[(xxsize-1-xx)*yysize*3+3*yy]=b->dataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*yysize*3+3*yy+1]=b->dataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*yysize*3+3*yy+2]=b->dataPtr[xx*3+2+yy*xxsize*3];
	            }
	        }
	    }
	}else if(dold == 3){
	    if(dnew == 0){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[(xxsize-1-xx)*yysize*3+yy*3]=b->dataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*yysize*3+yy*3+1]=b->dataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*yysize*3+yy*3+2]=b->dataPtr[xx*3+2+yy*xxsize*3];
	            }
	        }
	    }else if(dnew == 1){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[(xxsize-1-xx)*3+(yysize-1-yy)*xxsize*3]=b->dataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+1+(yysize-1-yy)*xxsize*3]=b->dataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+2+(yysize-1-yy)*xxsize*3]=b->dataPtr[xx*3+2+yy*xxsize*3];
	            }
	        }
	    }else if(dnew == 2){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[xx*yysize*3+(yysize-1-yy)*3]=b->dataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[xx*yysize*3+(yysize-1-yy)*3+1]=b->dataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[xx*yysize*3+(yysize-1-yy)*3+2]=b->dataPtr[xx*3+2+yy*xxsize*3];
	            }
	        }
	    }
	}
	
	if(!pBox->Files)pBox->AngleFlag=0;
		
	return (DObjPtr)pBox;
}

static DObjPtr uTrueFlipDirection(uTruePtr b,int dold,int dnew)
{
	uTruePtr pBox,pBox2;
	double dx=0,dy=0;
	unsigned char save;
	int xx,yy,xxsize,yysize;
    unsigned char *ddataPtr;

	if(!b)return NULL;
	
	if(dold == dnew)return NULL;
	
	if(b->AngleFlag != 0){
	    if((pBox=(uTruePtr) uTrueRotateDirection(b,b->AngleFlag,0)) == NULL){
	        return NULL;
	    }
	}else if((pBox=(uTruePtr) uTrueDuplicate((DObjPtr)b,0,&dx,&dy)) == NULL){
	    return NULL;
	}
	xxsize=(int)pBox->xsize;
	yysize=(int)pBox->ysize;
	ddataPtr=pBox->dataPtr;
	
	if(dold == 0){
	    if(dnew == 1){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize/2;++xx){
	                save=ddataPtr[3*(xxsize-1-xx)+yy*xxsize*3];
	                ddataPtr[3*(xxsize-1-xx)+yy*xxsize*3]=ddataPtr[3*xx+yy*xxsize*3];
	                ddataPtr[3*xx+yy*xxsize*3]=save;
	                
	                save=ddataPtr[3*(xxsize-1-xx)+1+yy*xxsize*3];
	                ddataPtr[3*(xxsize-1-xx)+1+yy*xxsize*3]=ddataPtr[3*xx+1+yy*xxsize*3];
	                ddataPtr[3*xx+1+yy*xxsize*3]=save;
	                
	                save=ddataPtr[3*(xxsize-1-xx)+2+yy*xxsize*3];
	                ddataPtr[3*(xxsize-1-xx)+2+yy*xxsize*3]=ddataPtr[3*xx+2+yy*xxsize*3];
	                ddataPtr[3*xx+2+yy*xxsize*3]=save;
	            }
	        }
	    } else if(dnew == 2){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[3*xx+yy*xxsize*3];
	                ddataPtr[3*xx+yy*xxsize*3]=ddataPtr[3*xx+(yysize-1-yy)*xxsize*3];
	                ddataPtr[3*xx+(yysize-1-yy)*xxsize*3]=save;
	                
	                save=ddataPtr[3*xx+1+yy*xxsize*3];
	                ddataPtr[3*xx+1+yy*xxsize*3]=ddataPtr[3*xx+1+(yysize-1-yy)*xxsize*3];
	                ddataPtr[3*xx+1+(yysize-1-yy)*xxsize*3]=save;
	                	                
	                save=ddataPtr[3*xx+2+yy*xxsize*3];
	                ddataPtr[3*xx+2+yy*xxsize*3]=ddataPtr[3*xx+2+(yysize-1-yy)*xxsize*3];
	                ddataPtr[3*xx+2+(yysize-1-yy)*xxsize*3]=save;
	            }
	        }
	    } else if(dnew == 3){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[(xxsize-1-xx)*3+(yysize-1-yy)*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+(yysize-1-yy)*xxsize*3]=ddataPtr[3*xx+yy*xxsize*3];
	                ddataPtr[3*xx+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+1+(yysize-1-yy)*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+1+(yysize-1-yy)*xxsize*3]=ddataPtr[3*xx+1+yy*xxsize*3];
	                ddataPtr[3*xx+1+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+2+(yysize-1-yy)*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+2+(yysize-1-yy)*xxsize*3]=ddataPtr[3*xx+2+yy*xxsize*3];
	                ddataPtr[3*xx+2+yy*xxsize*3]=save;
	                
	            }
	        }
	    }
	}else if(dold == 1){
	    if(dnew == 0){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize/2;++xx){
	                save=ddataPtr[(xxsize-1-xx)*3+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+yy*xxsize*3]=ddataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[xx*3+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+1+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+1+yy*xxsize*3]=ddataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[xx*3+1+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+2+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+2+yy*xxsize*3]=ddataPtr[xx*3+2+yy*xxsize*3];
	                ddataPtr[xx*3+2+yy*xxsize*3]=save;	                
	            }
	        }
	    } else if(dnew == 2){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[(xxsize-1-xx)*3+(yysize-1-yy)*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+(yysize-1-yy)*xxsize*3]=ddataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[xx*3+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+1+(yysize-1-yy)*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+1+(yysize-1-yy)*xxsize*3]=ddataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[xx*3+1+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+2+(yysize-1-yy)*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+2+(yysize-1-yy)*xxsize*3]=ddataPtr[xx*3+2+yy*xxsize*3];
	                ddataPtr[xx*3+2+yy*xxsize*3]=save;                
	            }
	        }
	    } else if(dnew == 3){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[xx*3+yy*xxsize*3]=ddataPtr[xx*3+(yysize-1-yy)*xxsize*3];
	                ddataPtr[xx*3+(yysize-1-yy)*xxsize*3]=save;
	                
	                save=ddataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[xx*3+1+yy*xxsize*3]=ddataPtr[xx*3+1+(yysize-1-yy)*xxsize*3];
	                ddataPtr[xx*3+1+(yysize-1-yy)*xxsize*3]=save;
	                
	                save=ddataPtr[xx*3+2+yy*xxsize*3];
	                ddataPtr[xx*3+2+yy*xxsize*3]=ddataPtr[xx*3+2+(yysize-1-yy)*xxsize*3];
	                ddataPtr[xx*3+2+(yysize-1-yy)*xxsize*3]=save;	                
	            }
	        }
	    }
	}else if(dold == 2){
	    if(dnew == 0){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[xx*3+yy*xxsize*3]=ddataPtr[3*xx+(yysize-1-yy)*xxsize*3];
	                ddataPtr[3*xx+(yysize-1-yy)*xxsize*3]=save;
	                
	                save=ddataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[xx*3+1+yy*xxsize*3]=ddataPtr[3*xx+1+(yysize-1-yy)*xxsize*3];
	                ddataPtr[3*xx+1+(yysize-1-yy)*xxsize*3]=save;
	                
	                save=ddataPtr[xx*3+2+yy*xxsize*3];
	                ddataPtr[xx*3+2+yy*xxsize*3]=ddataPtr[3*xx+2+(yysize-1-yy)*xxsize*3];
	                ddataPtr[3*xx+2 +(yysize-1-yy)*xxsize*3]=save;
	            }
	        }
	    } else if(dnew == 1){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[(xxsize-1-xx)*3+(yysize-1-yy)*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+(yysize-1-yy)*xxsize*3]=ddataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[xx*3+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+1+(yysize-1-yy)*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+1+(yysize-1-yy)*xxsize*3]=ddataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[xx*3+1+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+2+(yysize-1-yy)*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+2+(yysize-1-yy)*xxsize*3]=ddataPtr[xx*3+2+yy*xxsize*3];
	                ddataPtr[xx*3+2+yy*xxsize*3]=save;
	            }
	        }
	    } else if(dnew == 3){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize/2;++xx){
	                save=ddataPtr[(xxsize-1-xx)*3+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+yy*xxsize*3]=ddataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[xx*3+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+1+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+1+yy*xxsize*3]=ddataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[xx*3+1+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+2+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+2+yy*xxsize*3]=ddataPtr[xx*3+2+yy*xxsize*3];
	                ddataPtr[xx*3+2+yy*xxsize*3]=save;
	            }
	        }
	    }
	}else if(dold == 3){
	    if(dnew == 0){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[(xxsize-1-xx)*3+(yysize-1-yy)*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+(yysize-1-yy)*xxsize*3]=ddataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[xx*3+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+1+(yysize-1-yy)*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+1+(yysize-1-yy)*xxsize*3]=ddataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[xx*3+1+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+2+(yysize-1-yy)*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+2+(yysize-1-yy)*xxsize*3]=ddataPtr[xx*3+2+yy*xxsize*3];
	                ddataPtr[xx*3+2+yy*xxsize*3]=save;
	            }
	        }
	    } else if(dnew == 1){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[xx*3+yy*xxsize*3]=ddataPtr[xx*3+(yysize-1-yy)*xxsize*3];
	                ddataPtr[xx*3+(yysize-1-yy)*xxsize*3]=save;
	                
	                save=ddataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[xx*3+1+yy*xxsize*3]=ddataPtr[xx*3+1+(yysize-1-yy)*xxsize*3];
	                ddataPtr[xx*3+1+(yysize-1-yy)*xxsize*3]=save;
	                
	                save=ddataPtr[xx*3+2+yy*xxsize*3];
	                ddataPtr[xx*3+2+yy*xxsize*3]=ddataPtr[xx*3+2+(yysize-1-yy)*xxsize*3];
	                ddataPtr[xx*3+2+(yysize-1-yy)*xxsize*3]=save;
	            }
	        }
	    } else if(dnew == 2){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize/2;++xx){
	                save=ddataPtr[(xxsize-1-xx)*3+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+yy*xxsize*3]=ddataPtr[xx*3+yy*xxsize*3];
	                ddataPtr[xx*3+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+1+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+1+yy*xxsize*3]=ddataPtr[xx*3+1+yy*xxsize*3];
	                ddataPtr[xx*3+1+yy*xxsize*3]=save;
	                
	                save=ddataPtr[(xxsize-1-xx)*3+2+yy*xxsize*3];
	                ddataPtr[(xxsize-1-xx)*3+2+yy*xxsize*3]=ddataPtr[xx*3+2+yy*xxsize*3];
	                ddataPtr[xx*3+2+yy*xxsize*3]=save;
	                
	            }
	        }
	    }
	}
	
	
	if(b->AngleFlag != 0){
	    if((pBox2=(uTruePtr)uTrueRotateDirection(pBox,0,b->AngleFlag)) == NULL){
	        uTrueDelete((DObjPtr)pBox);
	        return NULL;
	    }
	    uTrueDelete((DObjPtr)pBox);
	    pBox = pBox2;
	}
	pBox->AngleFlag = b->AngleFlag;
	pBox->FlipFlag = dnew;
	if(!pBox->Files){
	    pBox->AngleFlag=0;
		pBox->FlipFlag=0;
	}
	return (DObjPtr)pBox;
}
static int uTrueReplaceImage(uTruePtr b,long xsizei,long ysizei,unsigned char *image)
{
	long length;
	int flag;

	if(!b || !image)return 1;
	
	flag=1;
	
	length=xsizei*ysizei*3;
			
	if(xsizei != b->xsize || ysizei != b->ysize){
		if(length > b->xsize*b->ysize*3){
			if(b->dataPtr){
			    cFree((char *)b->dataPtr);
			    b->dataPtr=NULL;
			}
			if((b->dataPtr=cMalloc(length,8076)) == NULL){
			    sprintf(WarningBuff,"uTrueReplaceImage Out Of Memory Requested (%ld)\n",length);
			    WarningBatch(WarningBuff);
			    return 1;
			}
		}
	}
	
	uMemcpy(b->dataPtr,image,(long)length);

	b->xsize=xsizei;
	b->ysize=ysizei;
	
	if(b->AngleFlag != 0 || b->FlipFlag != 0 ){
	
	    int AngleFlag=b->AngleFlag,FlipFlag=b->FlipFlag;
	    uTruePtr ImageN;
	    
	    b->AngleFlag=0;
	    b->FlipFlag=0;
			
	    if(FlipFlag){
	        ImageN=(uTruePtr)uTrueFlipDirection(b,0,FlipFlag);
	        if(!ImageN)goto OutOfHere;
	        if(b->dataPtr)cFree((char *)b->dataPtr);
	        b->dataPtr=ImageN->dataPtr;
	        ImageN->dataPtr=NULL;
	        b->FlipFlag=ImageN->FlipFlag;
	        uTrueDelete((DObjPtr)ImageN);
	    }
	    
	    if(AngleFlag){
	        ImageN=(uTruePtr)uTrueRotateDirection(b,0,AngleFlag);
	        if(!ImageN)goto OutOfHere;
	        if(b->dataPtr)cFree((char *)b->dataPtr);
	        b->dataPtr=ImageN->dataPtr;
	        b->AngleFlag=ImageN->AngleFlag;
	        ImageN->dataPtr=NULL;
			b->xsize=ImageN->xsize;
			b->ysize=ImageN->ysize;
	        uTrueDelete((DObjPtr)ImageN);
	    }
	}
	flag=0;
OutOfHere:
	return flag;
}

static int uTrueMessage(DObjPtr o,long MessageType,void *MessageData)
{
    struct SetFrameData *s;
	uTruePtr b=(uTruePtr)o;
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
	case MessageType_SET_FRAME_NUMBER:
		if(!b->Files)return 0;		
	    if(!b->Attributes.Animation)return 0;
	    if(!b->ReplaceImage)return 1;
		if(FilesSetFrame(m->CurrentFrame,b->Files))return 1;
		s = &b->Files->SetData;		
		if((*b->ReplaceImage)(b,s->xsize,s->ysize,(unsigned char *)s->data)){
		    return 1;
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
	case MessageType_GET_FRAME_COUNT:
		if(b->Files){
		    if(b->Files->ImageCount > m->FrameCount)m->FrameCount=b->Files->ImageCount;
		}
		break;
	case MessageType_PRINT_FILE_INFORMATION:
	    sprintf(WarningBuff,"  True Object x %g y %g\n%c",b->box.x,b->box.y,0);
	    WarningBatch(WarningBuff);
	    printFileInformation(b->Files);
		break;
	}	
	return 0;
}

static void uTrueDelete(DObjPtr o)
{
	uTruePtr b=(uTruePtr)o;
	
	if(!b)return;
	
	if(b->dataPtr){
	    cFree((char *)b->dataPtr);
	    b->dataPtr=NULL;
	}
	
	if(b->pBoxPtr){
	    cFree((char *)b->pBoxPtr);
	    b->pBoxPtr=NULL;
	}
	
	ObjectKill((DObjPtr)b);
	
}

static char *uTrueCopy(DObjPtr o,long *Length)
{
	uTruePtr pBox;
	struct Obj *obj;
	char *p;
	long size,istart,len;
	uTruePtr b=(uTruePtr)o;
	
	if(!b || !Length)return NULL;
	
	istart=sizeof(struct Obj)+sizeof(*pBox);
	len=b->xsize*b->ysize*3;
	*Length=size=istart+len;
	if((obj=(struct Obj *)cMalloc(size,8079)) == NULL){
	    return NULL;
	}
	pBox=(uTruePtr)(obj+1);
	obj->type=b->type;
	obj->Length=size;
	*pBox = *b;
	p=(char *)obj;
	p += istart;
	uMemcpy(p,b->dataPtr,(long)len);
	return (char *)obj;
}
static struct DObject *uTrueSetPalette(struct DObject *o,unsigned char *palette)
{
	uTruePtr b=(uTruePtr)o;
	uTruePtr iTrue;
	static double dx,dy;
	
	if(!b || !palette)return NULL;
	
	iTrue=(uTruePtr)uTrueMove(o,&dx,&dy);
	if(iTrue){
	     memcpy(iTrue->pd.palette,palette,256*3L);
	}

	
	return (DObjPtr)iTrue;
	
}

static DObjPtr uTrueKillSelected(DObjPtr o)
{
	uTruePtr b=(uTruePtr)o;
	uTruePtr Plot;
	double dx=0;
	double dy=0;

	if(!b)return NULL;
	if(b->iSelected)return NULL;
	if((Plot=(uTruePtr)uTrueMove((DObjPtr)b,&dx,&dy)) == NULL)return NULL;
	
	if(b->pSelected){
	    Plot->showPalette = FALSE;
	    Plot->pSelected = FALSE;
	}

	return (DObjPtr)Plot;
}
static int uTrueUpdate(struct DObject *o)
{
	uTruePtr b=(uTruePtr)o;
	
	if(!b || !b->l)return 1;
			
	if(b->iSelected){
	    ObjectBoxStretch(&b->box,b->l);
	}

	if(b->pSelected){
	    ObjectBoxStretch(&b->pBoxes,b->l);
	}

	return 0;
	
}
static int uTrueDrawSelected(struct DObject *o,DOListPtr l)
{
	uTruePtr b=(uTruePtr)o;
	
	if(!b || !l)return 1;
			
	if(b->iSelected){
	    ObjectBoxFrame(&b->box,l);
	}

	if(b->pSelected){
	    ObjectBoxFrame(&b->pBoxes,l);
	}

	return 0;
	
}
static int uTrueSetSelectedRegion(struct DObject *o,DOListPtr l)
{
	uTruePtr b=(uTruePtr)o;
	
	if(!b || !l)return 1;
	
	
	
	if(b->iSelected){
			if(ResizeArray(&l->Boxes,sizeof(rRect),l->rCountCount,&l->rCountMax,2012))return 0;			
			l->Boxes[l->rCountCount++]=b->box; 	    
	}

	if(b->pSelected){
			if(ResizeArray(&l->Boxes,sizeof(rRect),l->rCountCount,&l->rCountMax,2012))return 0;			
			l->Boxes[l->rCountCount++]=b->pBoxes; 	    
	}

	return 0;
	
}
static void uTrueAttribute(DObjPtr o,struct uAttributes *Attributes,int Flag)
{
	uTruePtr b=(uTruePtr)o;
	
	if(!b || !Attributes)return;
	
	if(b->pSelected)uPutAttributes(&b->pAttributes,Attributes,Flag);
	if(b->iSelected)uPutAttributes(&b->Attributes,Attributes,Flag);
		
}
static struct DObject *uTrueFindObject(struct DObject *Object,rPoint *r)
{
	uTruePtr b=(uTruePtr)Object;
	
	if(!b || !r)return NULL;

	
	if(b->showPalette){
		if(r->x >= b->pBoxes.x && r->y >= b->pBoxes.y){
			if(r->x <= b->pBoxes.x+b->pBoxes.xsize && r->y <= b->pBoxes.y+b->pBoxes.ysize){    
					return Object;
			}
		}
	}

	if(r->x < b->box.x || r->y < b->box.y)return NULL;
	if(r->x > b->box.x+b->box.xsize || r->y > b->box.y+b->box.ysize)return NULL;
	
	return Object;
}

static int uTrueSelectedPoint(DObjPtr o,rPoint *r)
{
	uTruePtr b=(uTruePtr)o;
	
	if(!b)return 0;
	if(!r){
	    return b->Selected;
	}
		
	if(b->showPalette){
		if(r->x >= b->pBoxes.x && r->y >= b->pBoxes.y){
			if(r->x <= b->pBoxes.x+b->pBoxes.xsize && r->y <= b->pBoxes.y+b->pBoxes.ysize){    
					return b->pSelected;
			}
		}
	}
	
	return b->iSelected;
}
static void uTrueSelectOne(DObjPtr o,rPoint *r)
{
	uTruePtr b=(uTruePtr)o;
	struct DOList *l;
	IconPtr myIcon;
	int flag;
	
	if(!b)return;
	l=b->l;
	if(!l)return;
	myIcon=l->myIcon;
	if(!myIcon)return;
	
	flag=0;
	
	if(!r){
		if(b->showPalette){
			if(!b->pSelected){
				uSetPenXOR(TRUE,myIcon);	    
			    b->pSelected=TRUE;
				ObjectBoxFrame(&b->pBoxes,l);
				uSetPenXOR(FALSE,myIcon);	 
			}
		}else{
			b->pSelected=FALSE;
		}
		
		if(!b->iSelected){
			uSetPenXOR(TRUE,myIcon);	    
			b->iSelected=TRUE;
			ObjectBoxFrame(&b->box,l);
			uSetPenXOR(FALSE,myIcon);	 
		}
	    b->Selected=TRUE;
		b->iSelected=TRUE;
	    return;
	}
		
	if(b->showPalette){
		if(r->x >= b->pBoxes.x && r->y >= b->pBoxes.y){
			if(r->x <= b->pBoxes.x+b->pBoxes.xsize && r->y <= b->pBoxes.y+b->pBoxes.ysize){    
			    if(!b->pSelected){
					uSetPenXOR(TRUE,myIcon);	    
				    b->pSelected=TRUE;
	    			ObjectBoxFrame(&b->pBoxes,l);
					uSetPenXOR(FALSE,myIcon);	 
			    }
			 	flag=1;
			}
		}
	}else{
		b->pSelected=FALSE;
	}
	

	
	if(r->x >= o->box.x && r->y >= o->box.y && r->x <= o->box.x+o->box.xsize &&
	   r->y <= o->box.y+o->box.ysize){
		  	if(!b->iSelected){
				uSetPenXOR(TRUE,myIcon);	    
			    b->iSelected=TRUE;
	    		ObjectBoxFrame(&b->box,l);
				uSetPenXOR(FALSE,myIcon);	 
		  	}
		  	flag=1;
	}

	if(flag)b->Selected=TRUE;
}
static void uTrueDeSelectOne(DObjPtr o,rPoint *r)
{
	uTruePtr b=(uTruePtr)o;
	struct DOList *l;
	IconPtr myIcon;
	
	if(!b)return;
	l=b->l;
	if(!l)return;
	myIcon=l->myIcon;
	if(!myIcon)return;
	
	
	if(!r){
		if(b->showPalette){
		    if(b->pSelected){
				uSetPenXOR(TRUE,myIcon);	    
			    b->pSelected=FALSE;
				ObjectBoxFrame(&b->pBoxes,l);
				uSetPenXOR(FALSE,myIcon);	 
		    }
	    }
	  	if(b->iSelected){
			uSetPenXOR(TRUE,myIcon);	    
		    b->iSelected=FALSE;
    		ObjectBoxFrame(&b->box,l);
			uSetPenXOR(FALSE,myIcon);	 
	  	}
	    b->Selected=FALSE;
		b->pSelected=FALSE;
		b->iSelected=FALSE;
	    return;
	}
	
	if(b->showPalette){
		if(r->x >= b->pBoxes.x && r->y >= b->pBoxes.y){
			if(r->x <= b->pBoxes.x+b->pBoxes.xsize && r->y <= b->pBoxes.y+b->pBoxes.ysize){    
			    if(b->pSelected){
					uSetPenXOR(TRUE,myIcon);	    
				    b->pSelected=FALSE;
	    			ObjectBoxFrame(&b->pBoxes,l);
					uSetPenXOR(FALSE,myIcon);	 
			    }
			}
		}
	}else{
		b->pSelected=FALSE;
	}
	
	
	if(r->x >= o->box.x && r->y >= o->box.y && r->x <= o->box.x+o->box.xsize &&
	   r->y <= o->box.y+o->box.ysize){
		  	if(b->iSelected){
				uSetPenXOR(TRUE,myIcon);	    
			    b->iSelected=FALSE;
	    		ObjectBoxFrame(&b->box,l);
				uSetPenXOR(FALSE,myIcon);	 
		  	}
	}
	
	if(b->iSelected || b->pSelected){
	    b->Selected=TRUE;
	}else{
	    b->Selected=FALSE;
	}
}
int uTrueSet(uTruePtr b)
{
	if(!b)return 1;
	
	b->type=TruePlotType;
	b->Kill=uTrueDelete;
	b->Message=uTrueMessage;
	b->ReplaceImage=uTrueReplaceImage;
	b->Duplicate=uTrueDuplicate;
	b->Move=uTrueMove;
	b->Draw=uTrueDraw;
    b->GetFrameCount=uTrueFrameCount;
	b->DoInformation=uTrueDoInformation;	
	b->SetFrame=uTrueSetFrame;
	b->DoRotateDirection=uTrueDoRotateDirection;
	b->DoFlipDirection=uTrueDoFlipDirection;	
	b->Copy=uTrueCopy;	
	b->ReadWrite=uTrueWrite;	
	b->DoubleClick=uTrueDoubleClick;
	b->SetPalette=uTrueSetPalette;
	b->SetFontSelected=uTrueSetFontSelected;	
	b->GetFontSelected=uTrueGetFontSelected;	
	b->KillSelected=uTrueKillSelected;
	b->SetSelectedRegion=uTrueSetSelectedRegion;
    b->Attribute=uTrueAttribute;
    b->FindObject=uTrueFindObject;
	b->SelectedPoint=uTrueSelectedPoint;
  	b->SelectOne=uTrueSelectOne;
	b->DeSelectOne=uTrueDeSelectOne;
	b->DrawSelected=uTrueDrawSelected;
	b->Update=uTrueUpdate;
	return 0;
}
uTruePtr uTrueTrue(long xs,long ys,rRect *r,struct uAttributes *Attribute,struct DOList *l)
{
	char name[64];
	uTruePtr b;
	
	if(!r || !Attribute || !l)return NULL;
	
	sprintf(name,"True%03ld%c",l->ObjectIDCount++,0);
	
    b=(uTruePtr)ObjectCreate(name,TruePlotType,r,sizeof(struct uTrue));
    if(!b)return NULL;
	
	if(uTrueSet(b))goto ErrorOut;
		
	b->Attributes  = *Attribute;
	b->pAttributes = *Attribute;
	b->xsize=xs;
	b->ysize=ys;
	b->length=b->xsize*b->ysize*3;
	if((b->dataPtr=cMalloc(b->length,8037)) == NULL){
	    sprintf(WarningBuff,"uTrueTrue Out Of Memory Requested (%ld)\n",b->length);
	    WarningBatch(WarningBuff);
	   goto ErrorOut;
	}
	b->box = *r;
	b->Attributes.Animation=TRUE;
	b->l=l;
	
	b->countXaxis=5;
	b->countYaxis=5;
	b->countPalette=5;
	b->iSelected=TRUE;
	mstrncpy(b->formatXaxis,"%9.2e",sizeof(b->formatXaxis));
	mstrncpy(b->formatYaxis,"%9.2e",sizeof(b->formatYaxis));
	mstrncpy(b->formatPalette,"%9.2e",sizeof(b->formatPalette));
		
	return b;
ErrorOut:
	uTrueDelete((DObjPtr)b);
	return NULL;
}

uTruePtr uTrueTrue2(uTruePtr Pastin,DOListPtr l)
{
	long length;
	char *p;
	uTruePtr b;
	
	if(!Pastin || !l)return NULL;
	
	
	if(Pastin->size != sizeof(*b))return NULL;
		
	b=(uTruePtr)ObjectObject2((DObjPtr)Pastin);
	
	b->l=l;	
	
	if(uTrueSet(b))goto ErrorOut;
	
	b->Attributes = Pastin->Attributes;
	b->pAttributes = Pastin->pAttributes;
	b->pBoxOffset = Pastin->pBoxOffset;
	b->pBoxes = Pastin->pBoxes;
	b->pBoxPtr = NULL;
	b->showPalette = Pastin->showPalette;
	b->showYaxis = Pastin->showYaxis;
	b->showXaxis = Pastin->showXaxis;
	b->countYaxis = Pastin->countYaxis;
	b->countXaxis = Pastin->countXaxis;
	b->countPalette = Pastin->countPalette;
	mstrncpy(b->formatXaxis,Pastin->formatXaxis,sizeof(b->formatXaxis));
	mstrncpy(b->formatYaxis,Pastin->formatYaxis,sizeof(b->formatYaxis));
	mstrncpy(b->formatPalette,Pastin->formatPalette,sizeof(b->formatPalette));
	b->xsize=Pastin->xsize;
	b->ysize=Pastin->ysize;
	b->Files = Pastin->Files;
	b->TrueType = Pastin->TrueType;
	b->plane  =  Pastin->plane;
	b->range  =  Pastin->range;
	b->vector =  Pastin->vector;
	b->pd 	   =  Pastin->pd;
	b->sd 	   =  Pastin->sd;
	b->iSelected=Pastin->iSelected;
	
	mstrncpy(b->BatchName,Pastin->BatchName,sizeof(b->BatchName));
	mstrncpy(b->pioName,Pastin->pioName,sizeof(b->pioName));
	b->pioIndex=Pastin->pioIndex;			    	
	length=b->xsize*b->ysize*3;
	if((b->dataPtr=cMalloc(length,8078)) == NULL){
	    sprintf(WarningBuff,"CTrueCTrue2 Out Of Memory Requested (%ld)\n",length);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	p=(char *)Pastin;
	p += sizeof(*b);
	uMemcpy(b->dataPtr,p,(long)length);
	
	return b;
ErrorOut:
	uTrueDelete((DObjPtr)b);
	return NULL;
}
