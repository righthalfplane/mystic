#define EXTERN22 extern
#include "Xappl.h"
#include "GWorld.h"
#include "uDialog.h"
#include "uDraw.h"

int uInsetRect(uRect *r,int x,int y);

static int doScale3Start(struct uDialogStruct *uList);

static int nMouseMoved(IconPtr myIcon,uPoint pt);

static int FindItem(uPoint hitPt,int *whichItem);


static int GetRectItem(uRect *r,int whichItem);

static int doScale3Action(struct uDialogStruct *uList);

static int doGetSelection(long *Result);

static int Flag;

static int ItemToMark;

static char *paintLabel;

static int doScale1Draw(struct uDialogStruct *uList);


void uWriteLineBox(uRect *LineWidthRect,int xLineWidth,int yLineWidth,IconPtr myIcon);

static uPoint uwhere;

void uWriteLineBox(uRect *LineWidthRect,int xLineWidth,int yLineWidth,IconPtr myIcon)
{
	int xc,yc,xmin,xmax,ymin,ymax;
	uRect r;
	
	xc=(LineWidthRect->xsize)/2;
	yc=(LineWidthRect->ysize)/2;	    
	xmin=xc-xLineWidth/2;
	xmax=xc+xLineWidth/2;
	if(xmax <= xmin)xmax = xmin + 1;
	ymin=yc-yLineWidth/2;
	ymax=yc+yLineWidth/2;
	if(ymax <= ymin)ymax = ymin + 1;	    
	uSetRect(&r,xmin+LineWidthRect->x,ymin+LineWidthRect->y,xmax-xmin,ymax-ymin);
	uPaintRect(&r,myIcon);
}
static int doScale1Draw(struct uDialogStruct *uList)
{
	
	IconPtr myIcon;
	uRect r;
	int j,k;
	int n;
	
	if(!uList)return 1;
	myIcon=uList->myIcon;
	if(!myIcon)return 0;		
	
	for(n=0;n<64;++n){		
		if(GetRectItem(&r,n+1))break;
		uEraseRect(&r,myIcon);
	    k=n/8;
	    j=n-k*8;
	    uWriteLineBox(&r,j+1,k+1,myIcon);
	    if(n+1 == ItemToMark){
			uInsetRect(&r,-2,-2);
			uFrameRect(&r,myIcon);
		}
	}

	
	return 0;
}

void uDrawwPopUp(IconPtr myIcon,int iflag)
{
	struct uAttributes vAttributes;
	struct uAttributes *ap;
	long Result;
	drawPtr w;
	DOListPtr OList;
	int x,y;
	
	if(!myIcon)return;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return;
	OList=&w->DList;
	if(!OList)return;

	uPointerGlobal(&uwhere,myIcon);

	Flag=iflag;
	
	ap=DOListGetAttributes(OList);
	
	Result=8*((long)(ap->yLineWidth)-1)+((long)(ap->xLineWidth)-1);
	++Result;
	ItemToMark=(int)Result;
	
	paintLabel=(char *)"Line Width";

	
	if(doGetSelection(&Result))return;

	--Result;
	
    y=(int)(Result/8);
    x=(int)(Result-y*8);
    vAttributes.xLineWidth=1+x;
    vAttributes.yLineWidth=1+y;

    DOListSaveAttributes(OList,&vAttributes,Flag);
		
}
static int doGetSelection(long *Result)
{
	static uDialogGroup Items[] =
	
        {
      	/*  0 */      {"Back Ground", {20,0,200,20},uDialogString,uDialogNone},
      	
      	/*  1 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  10 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  20 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  30 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  40 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  50 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  60 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},

      	/*  61 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  62 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  63 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	/*  64 */      {"FillRect", {20,5,100,20},uDialogUser,uDialogNone},
      	
      	/*  65*/      {"FillRect", {20,5,100,20},uDialogFloatAux,uDialogNone},
      	/*  66*/      {"FillRect", {20,5,100,20},uDialogFloatAux,uDialogNone},
      	/*  67*/      {"FillRect", {20,5,100,20},uDialogFrameRect,uDialogNone},


        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {0,0,200,232};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    	
	
	if(!Result)return 1;
		
		
	uGetPort(&save);
	
	Rect1.x += uwhere.x+50;
	Rect1.y += uwhere.y-Rect1.ysize/2;
		
	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("Select Line Width",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,65L,	
	              uDialogSetFloatAux,(double)ItemToMark,
	              uDialogSetItem,66L,
	              uDialogSetFloatAux,0.0,
	              uDialogSetItem,0L,
	              uDialogSetText,paintLabel,
	              uDialogSetStart,doScale3Start,
	              uDialogSetAction,doScale3Action,
	 	          uDialogSetUserDraw,doScale1Draw,             
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	*Result=(long)uR[65].dreturn;
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	Rect1.x -= uwhere.x+50;
	Rect1.y -= uwhere.y-Rect1.ysize/2;
	
	return ret;
}

static int doScale3Action(struct uDialogStruct *uList)
{
		
	if(!uList)return 1;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem > 0 && uList->actionItem <= 64){
		if(uDialogSet(uList,65L,	
		              uDialogSetFloatAux,(double)uList->actionItem,
	          		  uDialogSetExitOk,
		              uDialogSetDone,uDialogSetDone
		))goto ErrorOut;	    
	}
ErrorOut:
/*
	if(uList)uDialogUpdate(uList);
	if(uList->myIcon)InvalRectMyWindow(uList->myIcon);
*/
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}
static int GetRectItem(uRect *r,int whichItem)
{
 

	uRect mRect;
	int c,k,j;
	
	if(!whichItem)return 1;
	
	mRect.xsize = 24;
	mRect.ysize = 24;

	c=0;
	for(k=0;k<8;++k){
	    mRect.y=k*24+5 +12;
	    for(j=0;j<8;++j){
	        mRect.x=j*24+5;
	        c++;
	        if(whichItem == c)goto out;
	    }
	      
	}
	return 1;
out:
	*r=mRect;
	return 0;
	
}

static int FindItem(uPoint hitPt,int *whichItem)
{
 

	uRect mRect;
	int c,k,j;
	
	if(!whichItem)return 1;
	
	mRect.xsize = 24;
	mRect.ysize = 24;

	c=0;
	for(k=0;k<8;++k){
	    mRect.y=k*24+5 +16;
	    for(j=0;j<8;++j){
	        mRect.x=j*24+5;
	        c++;
	        if(uPtInRect(hitPt,&mRect))goto out;
	    }
	      
	}
	*whichItem=0;
	return 1;
out:
	*whichItem=c;
	return 0;
	
}

static int nMouseMoved(IconPtr myIcon,uPoint pt)
{
 	uDialogList *uList;
 	int item;
 	double d;
 	double f;
	uRect r;

	if(!myIcon)return 1;
	uList=uDialogGetList(myIcon);
	if(!uList)return 1;

	uGetPortBounds(myIcon,&r);

	if(uDialogSet(uList,65L,	
	              uDialogGetFloatAux,&d,
              	  uDialogSetItem,66L,
	              uDialogGetFloatAux,&f,
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;
	
	if(uPtInRect(pt,&r)){
		    
		if(FindItem(pt,&item))goto ErrorOut;
		if(item == (int)d)goto ErrorOut;
		if(GetRectItem(&r,(int)d))goto ErrorOut;
		
		uSetPenXOR(TRUE,myIcon);	    
	    uInsetRect(&r,-2,-2);
	    uFrameRect(&r,myIcon);
		uSetPenXOR(FALSE,myIcon);
				
		if(GetRectItem(&r,item))goto ErrorOut;
	    uInsetRect(&r,-2,-2);
	    uFrameRect(&r,myIcon);
		    	
		if(uDialogSet(uList,65L,	
		              uDialogSetFloatAux,(double)item,
		              uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
	    
		if(uDialogSet(uList,66L,	
	        	uDialogSetRect,&r,	
      			uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
	    
	    
	}
ErrorOut:
	return 0;	
}
static int doScale3Start(struct uDialogStruct *uList)
{
	
	uRect r;
 	double d;
	int n;
	int ret;
	
	if(!uList)return 1;


	if(uList->myIcon){
	    InvalRectMyWindow(uList->myIcon);	
		uList->myIcon->pMouseMoved=nMouseMoved;
		
		for(n=0;n<64;++n){		
			if(GetRectItem(&r,n+1))break;
			ret=uDialogSet(uList,(long)(n+1),				
		        	uDialogSetRect,&r,	
		        	uDialogSetForeColor,n,		              
          			uDialogSetDone,uDialogSetDone
		    );
	        if(ret){
	            WarningBatch("doScale3Start Error Building Tables\n");
	            goto ErrorOut;
	        }
		}
		
		if(uDialogSet(uList,65L,	
		              uDialogGetFloatAux,&d,
		              uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
		
		if(GetRectItem(&r,(int)d))goto ErrorOut;
		
	    uInsetRect(&r,-2,-2);
	    
		if(uDialogSet(uList,67L,	
	        	uDialogSetRect,&r,	
	        	uDialogSetForeColor,255,		              
      			uDialogSetDone,uDialogSetDone
		))goto ErrorOut;

				
	}	
ErrorOut:	
	return 0;
}
