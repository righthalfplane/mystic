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

static char *DefaultPaletteName;

static int Flag;

int ItemToMark;

static char *paintLabel;

static uPoint uwhere;

void uDrawcPopUp(IconPtr myIcon,int iflag)
{
	struct uAttributes vAttributes;
	struct uAttributes *ap;
	long Result;
	drawPtr w;
	DOListPtr OList;
	
	if(!myIcon)return;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return;
	OList=&w->DList;
	if(!OList)return;

	uPointerGlobal(&uwhere,myIcon);
	
	Flag=iflag;
	
	Result=0;
	
	ap=DOListGetAttributes(OList);
	
	ItemToMark=0;
	
	if(Flag == sForeColor){
		ItemToMark=ap->nSolidFore;
		paintLabel=(char *)"Foreground - Fill";
	}else if(Flag == sBackColor){
		ItemToMark=ap->nSolidBack;
		paintLabel=(char *)"Background - Fill";
	}else if(Flag == lForeColor){
		ItemToMark=ap->nLineFore;
		paintLabel=(char *)"Foreground - Lines";
	}else if(Flag == lBackColor){
		ItemToMark=ap->nLineBack;
		paintLabel=(char *)"Background - Lines";
	}

	ItemToMark++;
	
	DefaultPaletteName=myIcon->DefaultPaletteName;
	
	if(doGetSelection(&Result))return;
		
    if(Flag == sForeColor){
        vAttributes.nSolidFore=(int)(Result-1);
    }else if(Flag == sBackColor){
        vAttributes.nSolidBack=(int)(Result-1);
    }else if(Flag == lForeColor){
        vAttributes.nLineFore=(int)(Result-1);
    }else if(Flag == lBackColor){
        vAttributes.nLineBack=(int)(Result-1);
    }
    DOListSaveAttributes(OList,&vAttributes,Flag);
}
static int doGetSelection(long *Result)
{
	static uDialogGroup Items[] =
	
        {
      	/*  0 */      {"Back Ground", {20,0,200,20},uDialogString,uDialogNone},
      	
      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  10 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  20 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  30 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  40 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  50 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  60 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  70 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  80 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  90 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  100 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  110 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  120 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  130 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  140 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  150 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  160 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  170 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  180 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  190 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  200 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  210 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  220 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  230 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  240 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  6 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  7 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  8 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  9 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  250 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},

      	/*  1 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  2 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  3 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  4 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  5 */      {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  256 */    {"FillRect", {20,5,100,20},uDialogFillRect,uDialogNone},
      	/*  257*/     {"FillRect", {20,5,100,20},uDialogFloatAux,uDialogNone},
      	/*  258*/     {"FillRect", {20,5,100,20},uDialogFloatAux,uDialogNone},
      	/*  259*/     {"FillRect", {20,5,100,20},uDialogFrameRect,uDialogNone},


        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {0,0,204,224};
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
	
	uList=uDialogOpen("Select Color",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,257L,	
	              uDialogSetFloatAux,(double)ItemToMark,
	              uDialogSetItem,258L,
	              uDialogSetFloatAux,0.0,
	              uDialogSetItem,0L,
	              uDialogSetText,paintLabel,
	              uDialogSetStart,doScale3Start,
	              uDialogSetAction,doScale3Action,
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	*Result=(long)uR[257].dreturn;
	
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
	if(uList->actionItem > 0 && uList->actionItem <= 256){
		if(uDialogSet(uList,257L,	
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
	
	mRect.xsize = 10;
	mRect.ysize = 10;

	c=0;
	for(k=0;k<16;++k){
	    mRect.y=k*12+5 +12;
	    for(j=0;j<16;++j){
	        mRect.x=j*12+5;
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
	
	mRect.xsize = 10;
	mRect.ysize = 10;

	c=0;
	for(k=0;k<16;++k){
	    mRect.y=k*12+5 +12;
	    for(j=0;j<16;++j){
	        mRect.x=j*12+5;
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

	if(uDialogSet(uList,257L,	
	              uDialogGetFloatAux,&d,
              	  uDialogSetItem,258L,
	              uDialogGetFloatAux,&f,
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;
/*
	if(f <= 0){
		if(GetRectItem(&r,(int)d))goto ErrorOut;
	    uInsetRect(&r,-2,-2);
	    uFrameRect(&r,uList->myIcon);
		if(uDialogSet(uList,258L,	
		              uDialogSetFloatAux,(double)1.0,
		              uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
	
	
	}
*/

	
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
		    	
		if(uDialogSet(uList,257L,	
		              uDialogSetFloatAux,(double)item,
		              uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
	    
		if(uDialogSet(uList,259L,	
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
		
		for(n=0;n<256;++n){		
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
		
		if(uDialogSet(uList,257L,	
			uDialogGetFloatAux,&d,
			uDialogSetPalette,DefaultPaletteName,
			uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
		
		if(GetRectItem(&r,(int)d))goto ErrorOut;
		
	    uInsetRect(&r,-2,-2);
	    
		if(uDialogSet(uList,259L,	
	        	uDialogSetRect,&r,	
	        	uDialogSetForeColor,255,		              
      			uDialogSetDone,uDialogSetDone
		))goto ErrorOut;

				
	}	
ErrorOut:	
	return 0;
}
