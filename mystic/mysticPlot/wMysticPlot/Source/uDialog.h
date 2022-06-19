#ifndef __UDIALOG__
#define __UDIALOG__

#include "uLib.h"
#include "ScrollList.h"

#define uDialogNone					0
#define uDialogString				1
#define uDialogFloat				2
#define uDialogInt					3
#define uDialog2Button				4
#define uDialog3Button				5
#define uDialogUser					6
#define uDialogCheck				7
#define uDialogButton				8
#define uDialogButtonOK				9
#define uDialogButtonCancel			10
#define uDialogScrollList			11
#define uDialogText					12
#define uDialogButtonRadio			14
#define uDialogFloatAux				15
#define uDialogPopUpMenuControl		16
#define uDialogPopUpMenuMenu		17
#define uDialogSpread				18
#define uDialogFillRect			    19
#define uDialogFrameRect			20

#define uDialogSetDone			0
#define uDialogSetType			1
#define uDialogSetName			2
#define uDialogSetRect			3
#define uDialogSetMyIcon		4
#define uDialogSetMove			5
#define uDialogSetStart			6
#define uDialogSetAction		7
#define uDialogSetButtons		8
#define uDialogSetItem	    	9
#define uDialogSetActive		10
#define uDialogSetLogical		11
#define uDialogSetDouble		12
#define uDialogSetLocation		14
#define uDialogSetKeys			15
#define uDialogSetReturnType	16
#define uDialogSetModeLess	    17
#define uDialogSetFloatAux	    18
#define uDialogSetText			19
#define uDialogWindowType		20
#define uDialogSetItemClose	    21
#define uDialogSetExitOk	    22
#define uDialogSetExitCancel	23
#define uDialogSetDoubleClick	24
#define uDialogSetHidden		25
#define uDialogSetRadioGroup	26
#define uDialogSetUserDraw		27
#define uDialogSetItemSelect	28
#define uDialogSetFontStyle		29
#define uDialogSetFont			30
#define uDialogSetForeColor		38
#define uDialogSetBackColor		39
#define uDialogSetPattern		40
#define uDialogSetPalette		41
#define uDialogSetLong			42



#define uDialogGetFloatAux	    51
#define uDialogGetLogical		52
#define uDialogGetLong	    	53
#define uDialogGetRect	    	54
#define uDialogGetDouble		55
#define uDialogGetScrollList	56
#define uDialogGetControl		57
#define uDialogGetControlMenu	58
#define uDialogGetSpread		59

struct FillAttributes{
	int BackGround;
	int ForeGround;
	int Pattern;
};

struct FontStuff{
	int txFont;
	int txFace;
	int txSize;
	int txMode;
	int txdum1;
	int txdum2;
};

typedef struct uDialogGroupStruct{
     char *name;
     uRect Rect;
     int type;
     int returnType;
} uDialogGroup;

typedef struct uDialogReturnStruct{
     char sreturn[256];
     double dreturn;
     long lreturn;
     int returnType;
} uDialogReturn;

typedef struct uDialogItemStruct{
	 menuPtr menu;
     controlPtr Control;
     struct ScrollList *Scroll;
     struct FillAttributes FillData;
//	 TEHandle textH;
     char name[256];
     uRect Rect;
     int isActive;
     int RadioGroup;
     int isHidden;
     int isStart;
     int type;
     int returnType;
     double DoubleAux;
} uDialogItem;

typedef struct uDialogStruct{
     int (*uDialogMove)(struct uDialogStruct *uList);
     int (*uDialogStart)(struct uDialogStruct *uList);
     int (*uDialogAction)(struct uDialogStruct *uList);
     int (*uDialogButtons)(struct uDialogStruct *uList);
     int (*uDialogKeys)(struct uDialogStruct *uList);
     int (*uDialogDoubleClick)(struct uDialogStruct *uList);
     int (*uDialogUserDraw)(struct uDialogStruct *uList);
	 int (*pClose)(struct Icon *);
     char name[256];
	 char mess[256];
     uDialogItem *uDialogItemList;
     long uDialogItemCount;
     IconPtr myIconFrontWindow;
     IconPtr myIconItem;
     IconPtr myIcon;
     uRect Rect;
     uRect *RectPtr;
	 int count;
     int isOpen;
     int actionItem;
     int activeText;
     int ModeLess;
     int WindowType;
     
} uDialogList;


int uDialog(uDialogList *uList);

uDialogList *uDialogOpen(char *name,uRect *Rect,long Count);

int uDialogClose(uDialogList *uList);

int uDialogSet(uDialogList *uList,long Item,...);

int uDialogSetList(uDialogList *uList,uDialogGroup *uGroup,long ItemCount);

int uDialogUpdate(uDialogList *uList);

uDialogReturn *uDialogGetReturn(uDialogList *uList);

int uDialogFreeReturn(uDialogReturn *uReturn);

int FindFound(IconPtr myIconSearch,char *string,int flag);

uDialogList *uDialogGetList(IconPtr myIconSearch);

struct ScrollList *uDialogGetScroll(IconPtr myIcon,controlPtr c);


#endif