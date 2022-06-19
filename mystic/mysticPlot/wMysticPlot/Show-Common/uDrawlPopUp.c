#define EXTERN22 extern
#include "Xappl.h"
#include "GWorld.h"
#include "uDialog.h"
#include "uDraw.h"

int uInsetRect(uRect *r,int x,int y);
int uLinePat(uRect *r,char *pat,IconPtr myIcon);

static int doScale3Start(struct uDialogStruct *uList);

static int nMouseMoved(IconPtr myIcon,uPoint pt);

static int FindItem(uPoint hitPt,int *whichItem);


static int GetRectItem(uRect *r,int whichItem);

static int doScale3Action(struct uDialogStruct *uList);

static int doGetSelection(long *Result);


static int ItemToMark;

static char *paintLabel;

void uDrawlPopUp(IconPtr myIcon,IconPtr myIconDialog,int iflag);

static int doScale1Draw(struct uDialogStruct *uList);

static DOListPtr OList;

static uPoint uwhere;

void uDrawlPopUp(IconPtr myIcon,IconPtr myIconDialog,int iflag)
{
	struct uAttributes *ap;
	extern uLineDataPtr lineEXT;

	long Result;
	drawPtr w;
	
	if(!myIcon)return;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return;
	OList=&w->DList;
	if(!OList)return;
	if(!lineEXT)return;

	uPointerGlobal(&uwhere,myIconDialog);

	iflag=iflag;
		
	Result=0;
	
	ap=DOListGetAttributes(OList);
	
	ItemToMark=ap->LineDashNumber+1;
	
	paintLabel=(char *)"Line";
	
	if(doGetSelection(&Result))return;
		
	--Result;
	
	ap->LineDashNumber=(int)Result;
	
	if(Result >= 0 && Result <= 7){
	   mstrncpy((char *)&lineEXT->Attributes.LineDash,(char *)&OList->LinePatterns[Result],16);
       lineEXT->Attributes.DashFlag=(Result != 0);
	}
	
		
	/*
	uPutAttributes(&lineEXT->Attributes,ap,Flag);
    DOListSaveAttributes(OList,ap,Flag);
    */
}
static int doScale1Draw(struct uDialogStruct *uList)
{
	
	IconPtr myIcon;
	uRect r;
	int n;
	
	if(!uList)return 1;
	myIcon=uList->myIcon;
	if(!myIcon)return 0;		
	
	for(n=0;n<8;++n){		
		if(GetRectItem(&r,n+1))break;
		uEraseRect(&r,myIcon);
	    uLinePat(&r,OList->LinePatterns[n],myIcon);
	    if(n+1 == ItemToMark){
			uInsetRect(&r,-2,-2);
			uFrameRect(&r,myIcon);
		}
	}

	
	return 0;
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
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFloatAux,uDialogNone},
      	/*  10*/      {"FillRect", {20,5,100,20},uDialogFloatAux,uDialogNone},
      	/*  11*/      {"FillRect", {20,5,100,20},uDialogFrameRect,uDialogNone},


        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {0,0,90,140};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    	
	
	if(!Result)return 1;
		
		
	uGetPort(&save);
	
	Rect1.x += uwhere.x+30;
	Rect1.y += uwhere.y-Rect1.ysize/2;
		
	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("Select Line Pattern",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,9L,	
	              uDialogSetFloatAux,(double)ItemToMark,
	              uDialogSetItem,10L,
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

	*Result=(long)uR[9].dreturn;
		
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	Rect1.x -= uwhere.x+30;
	Rect1.y -= uwhere.y-Rect1.ysize/2;
	
	return ret;
}

static int doScale3Action(struct uDialogStruct *uList)
{
		
	if(!uList)return 1;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem >= 1 && uList->actionItem <= 8){
		if(uDialogSet(uList,9L,	
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
	int c,k;
		
	mRect.x=5;
	mRect.xsize = 80;
	mRect.ysize = 10;

	c=0;
	for(k=0;k<8;++k){
		mRect.y=k*12+5 +12;
		c++;
		if(whichItem == c)goto out;	      
	}
	return 1;
out:
	*r=mRect;
	return 0;
	
}

static int FindItem(uPoint hitPt,int *whichItem)
{
 

	uRect mRect;
	int c,k;
	
	if(!whichItem)return 1;
	
	mRect.x=5;
	mRect.xsize = 80;
	mRect.ysize = 10;

	c=0;
	for(k=0;k<8;++k){
		mRect.y=k*12+5 +12;
		c++;
		if(uPtInRect(hitPt,&mRect))goto out;
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

	if(uDialogSet(uList,9L,	
	              uDialogGetFloatAux,&d,
              	  uDialogSetItem,10L,
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
		    	
		if(uDialogSet(uList,9L,	
		              uDialogSetFloatAux,(double)item,
		              uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
	    
		if(uDialogSet(uList,11L,	
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
		

		for(n=0;n<8;++n){		
			if(GetRectItem(&r,n+1))break;
			ret=uDialogSet(uList,(long)(n+1),				
		        	uDialogSetRect,&r,	
          			uDialogSetDone,uDialogSetDone
		    );
	        if(ret){
	            WarningBatch("doScale3Start Error Building Tables\n");
	            goto ErrorOut;
	        }
		}
		
		
		if(uDialogSet(uList,9L,	
		              uDialogGetFloatAux,&d,
		              uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
		
		if(GetRectItem(&r,(int)d))goto ErrorOut;
		
	    uInsetRect(&r,-2,-2);
	    
		if(uDialogSet(uList,11L,	
	        	uDialogSetRect,&r,	
	        	uDialogSetForeColor,255,		              
      			uDialogSetDone,uDialogSetDone
		))goto ErrorOut;

				
	}	
ErrorOut:	
	return 0;
}
