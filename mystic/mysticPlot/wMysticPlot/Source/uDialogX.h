#ifndef __UDIALOG__
#define __UDIALOG__

#include "uLib.h"
#include "Control.h"
//#include "xText.h"
//#include "xStrings.h"
#include "ScrollList.h"
//#include "xMenu.h"


#ifndef __UDIALOGSTRUCT__
#define __UDIALOGSTRUCT__

struct popUpMenuStruct{
   struct MenuBar *menubar;
   struct Menu *menu;
   void *menuData;
   int hide;
   int IsCursorIn;
   int IsMaped;
//   struct controls control;
};

struct scrollStruct{
    struct ScrollList *scroll;
    void *scrollData;
    struct dialogStruct *dialog;
   int (*scrollAction)(struct ScrollList *scroll,void *scrollData,
                       struct dialogStruct *dialog,long itwas,long itIs);
   int hide;
   int IsCursorIn;
   int IsMaped;
};

struct buttonStruct{
    controlPtr button;
    void *buttonData;
    struct dialogStruct *dialog;
   int (*buttonAction)(controlPtr button,void *buttonData,
                                   struct dialogStruct *dialog);
   int hide;
   int IsMaped;
};

struct radioStruct{
    controlPtr radio;
    void *radioData;
    struct dialogStruct *dialog;
   int (*radioAction)(controlPtr radio,void *radioData,
                                   struct dialogStruct *dialog);
   int hide;
   int IsMaped;
};

struct checkStruct{
    controlPtr check;
    void *checkData;
    struct dialogStruct *dialog;
   int (*checkAction)(controlPtr check,void *checkData,
                                   struct dialogStruct *dialog);
   int hide;
   int IsMaped;
};


struct textStruct{
    struct textData *text;
    void *textData;
    struct dialogStruct *dialog;
    int (*textAction)(struct textData *text,void *textData,
                                   struct dialogStruct *dialog);
    int hide;
    int active;
};

#endif

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
#define uDialogGetLogical		14
#define uDialogGetDouble		15
#define uDialogSetLocation		16
#define uDialogSetKeys			17
#define uDialogGetLong	    	18
#define uDialogSetReturnType	19
#define uDialogGetRect	    	20
#define uDialogSetModeLess	    21
#define uDialogSetFloatAux	    22
#define uDialogGetFloatAux	    23
#define uDialogSetText			24
#define uDialogWindowType		25
#define uDialogSetItemClose	    26
#define uDialogGetScrollList	27
#define uDialogGetControl		28
#define uDialogSetExitOk	    29
#define uDialogSetExitCancel	30
#define uDialogGetControlMenu	31
#define uDialogSetDoubleClick	32
#define uDialogSetHidden		33
#define uDialogSetValue			34
#define uDialogSetRadioGroup	35




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
	struct popUpMenuStruct popList;
	struct buttonStruct buttonList;
	struct scrollStruct scrollList;
	struct radioStruct radioList;
	struct textStruct textList;
	struct stringData *stringList;
	struct checkStruct checkList;
	struct ScrollList *Scroll;
	char name[256];
	uRect Rect;
	int isActive;
	int RadioGroup;
	int isHidden;
	int IsMaped;
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
	 int (*pClose)(struct Icon *);
     char name[256];
	 char mess[256];
     uDialogItem *uDialogItemList;
     long uDialogItemCount;
     IconPtr myIconItem;
     IconPtr myIcon;
     uRect Rect;
	 int count;
     int isOpen;
     long actionItem;
     long activeText;
     int ModeLess;
     int WindowType;
     long menuCount;
//     Window  *mychild;    
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

#endif
