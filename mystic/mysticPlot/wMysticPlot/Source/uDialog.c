#define EXTERN22  extern
#include "Xappl.h"
#include "uDialog.h"
#include "uLib.h"
#include "uFiles.h"
//#include <sound.h>
//#include <TextEdit.h>
#include "fileDialog.h"
//#include "Attributes.h"
//#include "List2.h"
//#include "DirectSet.h"

IconPtr FindMyIcon(IconPtr w);
int TranslateAcceleratorM(MSG *msg);
int GetDefaultPaletteName(IconPtr myIcon,char *name1);
int getPaletteRGB(int n,unsigned char *r,unsigned char *g,unsigned char *b);
#define DATA_MAX 4

struct dialogFile{
    struct FontStuff systemFont2;
    struct FontStuff listFont;
    uDialogList *uList;
    unsigned long KeyTicks;
};

typedef struct dialogFile *dialogPtr;


//int GetFontStuff(struct FontStuff *f);
int SetFontStuff(struct FontStuff *f);

#define PLAIN				0
#define Vscroll1			258
int SetDefaultPaletteName(IconPtr myIcon,char *name1);
static int fUpdateWrite(IconPtr myIcon);
static int fUpdateWrite2(IconPtr myIcon,int flag);
static int fDoInContent(IconPtr myIcon);
static int DoControlFile(WindowPtr window,uPoint where,IconPtr myIcon);
static int fKeyBoard(IconPtr myIcon,int c);
static void fMoveControls(IconPtr myIcon);
static int fClose(IconPtr myIcon);
static int SetButtons(dialogPtr s);
//static int fCursor(IconPtr myIcon);

static void DialogKeyDown(IconPtr myIcon);

static int doSystemFile(IconPtr myIcon);

//static int GetCellText (char *buff,TEHandle textH);

static int fCopyPaste(IconPtr myIcon,int c);
long putStringToScrap(char *string);
IconPtr FindMyFrontWindow();
int SetMyIconWindowFront(IconPtr FrontWindow);

int getUniversalPaletteByName(char *name,unsigned char **pal);
int uInsetRect(uRect *r,int x,int y);
int uFillRect(uRect *r,struct Icon *myIcon);

int uSetForeColor(int r,int g,int b,IconPtr myIcon);
int uSetBackColor(int r,int g,int b,IconPtr myIcon);
int uSetForeColor(int r,int g,int b,IconPtr myIcon)
{
	
	if(!myIcon)return 1;

		
	if(myIcon->b)DeleteObject(myIcon->b);

	myIcon->b=CreateSolidBrush(RGB(r,g,b));
/*	
    Colorv.red=(r << 8);
    Colorv.green=(g << 8);
    Colorv.blue=(b << 8);
*/	    
	    
	return 0;	    
}
int uSetBackColor(int r,int g,int b,IconPtr myIcon)
{
	
	if(!myIcon)return 1;
/*	
    Colorv.red=(r << 8);
    Colorv.green=(g << 8);
    Colorv.blue=(b << 8);
*/	    
	return 0;	    
}


int SetFillAttributes(IconPtr myIcon,struct FillAttributes *FillData)
{
	unsigned char r,g,b;
	unsigned char *pal;
	char name[256];
	
	
	if(!myIcon)return 1;

	if(GetDefaultPaletteName(myIcon,name)){
	   mstrncpy(name,"ps",255);
	}

	if(!getUniversalPaletteByName(name,&pal)){
		r=pal[3*FillData->ForeGround];
		g=pal[3*FillData->ForeGround+1];
		b=pal[3*FillData->ForeGround+2];
		uSetForeColor(r,g,b,myIcon);

		
		r=pal[3*FillData->BackGround];
		g=pal[3*FillData->BackGround+1];
		b=pal[3*FillData->BackGround+2];

		uSetBackColor(r,g,b,myIcon);
			
	    return 0;
	}
	return 1;
}

struct ScrollList *uDialogGetScroll(IconPtr myIcon,controlPtr c)
{
	struct ScrollList *List;
	uDialogList *uList;
	long n;

	if(!myIcon || !c)return NULL;
	uList=uDialogGetList(myIcon);
	if(!uList)return NULL;

	for(n=0;n<uList->uDialogItemCount;++n){
		uDialogItem *uD;
		    
		uD = &uList->uDialogItemList[n];
		switch(uD->type){
	       case uDialogScrollList:
			List=uD->Scroll;
			if(List){
				if(List->control1 == c)return List;
				if(List->control2 == c)return List;
			}
	        break;
		}
		    
	}
	return NULL;
}
uDialogList *uDialogOpen(char *name,uRect *Rect,long Count)
{
    uDialogItem *uDialogItemList;
	uDialogList *uList;
	int ret;
	
	if(!name || !Rect || (Count < 1))return NULL;
	
	uList=NULL;
	uDialogItemList=NULL;
	
	ret = 1;
	
	uList=(uDialogList *)cMalloc(sizeof(uDialogList),649123);
	if(!uList)goto ErrorOut;
	zerol((char *)uList,sizeof(uDialogList));
	
	uDialogItemList=(uDialogItem *)cMalloc(sizeof(uDialogItem)*Count,649122);
	if(!uDialogItemList)goto ErrorOut;
	zerol((char *)uDialogItemList,sizeof(uDialogItem)*Count);
	
	uList->uDialogItemList=uDialogItemList;
	uList->uDialogItemCount=Count;
	uList->Rect = *Rect;
	uList->RectPtr = Rect;
	
	uList->WindowType=DialogType;
	
	uList->myIconFrontWindow=FindMyFrontWindow();
	
	mstrncpy(uList->name,name,256);
	
	uList->activeText = -1;

	ControlSetIDStart(2000);
	
	ret = 0;
ErrorOut:
	if(ret){
	    uDialogClose(uList);
	    uList=NULL;
	}
	return uList;
}
uDialogReturn *uDialogGetReturn(uDialogList *uList)
{
    uDialogReturn *uReturn,*uR;
	double value;
    long n;
    
	if(!uList || (uList->uDialogItemCount <= 0))return NULL;
	
	uReturn=(uDialogReturn *)cMalloc(sizeof(uDialogReturn)*uList->uDialogItemCount,923872);
	zerol((char *)uReturn,sizeof(uDialogReturn)*uList->uDialogItemCount);
	
	
		for(n=0;n<uList->uDialogItemCount;++n){
		    uDialogItem *uD;
		    
			uD = &uList->uDialogItemList[n];
			uR = &uReturn[n];
			uR->returnType=uD->returnType;
			switch(uD->type){
	           case uDialogNone:
	           break;
	           case uDialogButton:
	               mstrncpy(uR->sreturn,uD->name,256);
	               break;
	           case uDialogScrollList:
	               mstrncpy(uR->sreturn,uD->name,256);
	           break;
	           case uDialogFloatAux:
	               uR->dreturn=uD->DoubleAux;
	           break;
	           case uDialogText:
		            mstrncpy(uR->sreturn,uD->name,256);
		            if(uD->Control){
		                GetControlText(uD->Control,uR->sreturn);
		            }
		            if(uD->returnType == uDialogGetLong){
		                value=atof(uR->sreturn);
	                   	uR->lreturn = atol(uR->sreturn);
	                   	if((long)value > uR->lreturn)uR->lreturn=(long)value;         
		            }else if(uD->returnType == uDialogGetDouble){
	                   	uR->dreturn = atof(uR->sreturn);	            
		            }
	           break;
	           case uDialogPopUpMenuControl:
	           case uDialogButtonRadio:
	           case uDialogCheck:
		            mstrncpy(uR->sreturn,uD->name,256);
		            if(uD->returnType == uDialogGetLong){
	                   	if(uD->Control)uR->lreturn = uGetControlValue(uD->Control);	            
		            }else if(uD->returnType == uDialogGetDouble){
	                   	if(uD->Control)uR->dreturn = uGetControlValue(uD->Control);	            
		            }
	            break;
		    }
		    
		}
	
	return uReturn;
}
int uDialogFreeReturn(uDialogReturn *uReturn)
{
	if(!uReturn)return 1;
	cFree((char *)uReturn);
	return 0;
}

int uDialogSetList(uDialogList *uList,uDialogGroup *Items,long ItemCount)
{
	long n;
	int ret;
	
	if(!uList || !Items || (ItemCount <= 0))return 1;
	
	for(n=0;n<ItemCount;++n){
	
		ret=uDialogSet(uList,(long)n,
		              uDialogSetType,Items[n].type,
		              uDialogSetName,Items[n].name,
		              uDialogSetRect,&Items[n].Rect,
		              uDialogSetReturnType,Items[n].returnType,

		              uDialogSetDone,uDialogSetDone
		    );
		if(ret)return 1;

    }
    
	
	
	return 0;
}

int uDialogSet(uDialogList *uList,long Item,...)
{
	struct ScrollList **List;
	controlPtr *ControlPtr;
	menuPtr *menu;
	int isActive,lflag;
	int command,type;
	char buff[256];
	va_list ap;
	long *plflag;
	uRect  *Rect2;
	char *name;
	IconPtr myIcon,myIconItem;
	int (*uDialogMove)(struct uDialogStruct *uList);
	int (*uDialogStart)(struct uDialogStruct *uList);
	int (*uDialogAction)(struct uDialogStruct *uList);
	int (*uDialogButtons)(struct uDialogStruct *uList);
	int (*uDialogKeys)(struct uDialogStruct *uList);
	uDialogItem *uD;
	double value,*pvalue;
	long *lvalue,valuel;
	int ix,iy;
	int returnType;
	//	uRect r;
	
	
	if(!uList)return 1;
	
	if(Item < 0 || Item >= uList->uDialogItemCount)return 1;
	
	ControlPtr = NULL;
	myIcon = uList->myIcon;
	va_start(ap,Item);
	while((command=va_arg(ap,int)) != uDialogSetDone){
		switch(command){
		case uDialogSetExitOk:
			myIcon=uList->myIcon;
			if(!myIcon)return 1;
			myIcon->done = 1;
			break;
		case uDialogSetExitCancel:
			myIcon=uList->myIcon;
			if(!myIcon)return 1;
			myIcon->done = -1;
			break;
		case uDialogSetType:
			type=va_arg(ap,int);
			uList->uDialogItemList[Item].type=type;
			uList->uDialogItemList[Item].isActive=TRUE;
			break;
		case uDialogSetName:
			name=va_arg(ap,char *);
			if(!name)return 1;
			mstrncpy(uList->uDialogItemList[Item].name,name,256);
			break;
		case uDialogSetRect:
			Rect2=va_arg(ap,uRect  *);
			if(!Rect2)return 1;
			uD = &uList->uDialogItemList[Item];
			uD->Rect = *Rect2;
			if(uD->Control){
			    MoveControl(uD->Control,uD->Rect.x,uD->Rect.y);
				SizeControl(uD->Control,uD->Rect.xsize,uD->Rect.ysize);
			}
			break;
		case uDialogSetMyIcon:
			myIconItem=va_arg(ap,IconPtr);
			/* if(!myIconItem)return 1; */
			uList->myIconItem = myIconItem;
			break;
		case uDialogSetItemClose:           
			uList->pClose = va_arg(ap,void *);           
			break;
		case uDialogSetMove:
			uDialogMove=va_arg(ap,void *);
			if(!uDialogMove)return 1;
			uList->uDialogMove = uDialogMove;
			break;
		case uDialogSetDoubleClick:
			uDialogMove=va_arg(ap,void *);
			if(!uDialogMove)return 1;
			uList->uDialogDoubleClick = uDialogMove;
			break;
		case uDialogSetStart:
			uDialogStart=va_arg(ap,void *);
			if(!uDialogStart)return 1;
			uList->uDialogStart = uDialogStart;
			break;
		case uDialogSetAction:
			uDialogAction=va_arg(ap,void *);
			if(!uDialogAction)return 1;
			uList->uDialogAction = uDialogAction;
			break;
			
		case uDialogSetUserDraw:
			uDialogAction=va_arg(ap,void *);
			if(!uDialogAction)return 1;
			uList->uDialogUserDraw = uDialogAction;
			break;
			
		case uDialogSetModeLess:
			uList->ModeLess=va_arg(ap,int);
			break;			
           case uDialogSetForeColor:
               uD = &uList->uDialogItemList[Item];
               uD->FillData.ForeGround = va_arg(ap,int);
           break;
           
           case uDialogSetBackColor:
               uD = &uList->uDialogItemList[Item];
               uD->FillData.BackGround = va_arg(ap,int);
           break;
           
           case uDialogSetPattern:
               uD = &uList->uDialogItemList[Item];
               uD->FillData.Pattern = va_arg(ap,int);
           break;
		case uDialogWindowType:
			uList->WindowType=va_arg(ap,int);
			break;
			
		case uDialogSetKeys:
			uDialogKeys=va_arg(ap,void *);
			if(!uDialogKeys)return 1;
			uList->uDialogKeys = uDialogKeys;
			break;
			
		case uDialogSetButtons:
			uDialogButtons=va_arg(ap,void *);
			if(!uDialogButtons)return 1;
			uList->uDialogButtons = uDialogButtons;
			break;
		case uDialogSetItem:
			Item=va_arg(ap,long);
			if(Item < 0 || Item >= uList->uDialogItemCount)return 1;
			uD = &uList->uDialogItemList[Item];
			break;
		case uDialogSetActive:
			isActive=va_arg(ap,int);
			uList->uDialogItemList[Item].isActive=isActive;
			break;
		case uDialogSetHidden:
			isActive=va_arg(ap,int);
			uList->uDialogItemList[Item].isHidden=isActive;
			break;
		case uDialogSetRadioGroup:
			isActive=va_arg(ap,int);
			uList->uDialogItemList[Item].RadioGroup=isActive;
			break;
		case uDialogSetLogical:
			lflag=va_arg(ap,int);
			uD = &uList->uDialogItemList[Item];
			uD->isStart=lflag;
			if(uD->Control){
				uSetControlValue(uD->Control,uD->isStart);
			}
			break;
		case uDialogSetDouble:
			value=va_arg(ap,double);
			uD = &uList->uDialogItemList[Item];
			sprintf(uD->name,"%g",value);
			if(uD->Control){
				if(myIcon && uD->Control->controlType == controlTypeEdit) {
					SetDlgItemText(myIcon->myWindow,uD->Control->controlID,uD->name);
				}
			}
			break;


          case uDialogSetLong:
               valuel=va_arg(ap,long);
               uD = &uList->uDialogItemList[Item];
	           sprintf(uD->name,"%ld",valuel);
			if(uD->Control){
				if(myIcon && uD->Control->controlType == controlTypeEdit) {
					SetDlgItemText(myIcon->myWindow,uD->Control->controlID,uD->name);
				}
			}
           break;
		case uDialogSetText:
			name=va_arg(ap,char *);
			uD = &uList->uDialogItemList[Item];
			mstrncpy(uD->name,name,255);
			if(uD->Control){
				if(myIcon && uD->Control->controlType == controlTypeEdit) {
					SetDlgItemText(myIcon->myWindow,uD->Control->controlID,uD->name);
				}
			}
			break;

          case uDialogSetPalette:
                name=va_arg(ap,char *);
				myIcon=uList->myIcon;
               SetDefaultPaletteName(myIcon,name);
           break;

		case uDialogSetFloatAux:
			value=va_arg(ap,double);
			uD = &uList->uDialogItemList[Item];
			uD->DoubleAux=value;
			break;
		case uDialogSetLocation:
			ix=va_arg(ap,int);
			iy=va_arg(ap,int);
			uD = &uList->uDialogItemList[Item];
			if(uD->Control){
				MoveControl(uD->Control,ix,iy);
			}
	           if(uD->type == uDialogString){
				   uD->Rect.x =ix;
				   uD->Rect.y =iy;
			   }
			   
			   break;
		case uDialogSetReturnType:
			returnType=va_arg(ap,int);
			uList->uDialogItemList[Item].returnType=returnType;
			break;
		case uDialogGetRect:
			Rect2=va_arg(ap,uRect  *);
			if(!Rect2)return 1;
			*Rect2=uList->uDialogItemList[Item].Rect;
			break;
		case uDialogGetFloatAux:
			pvalue=va_arg(ap,double *);
			uD = &uList->uDialogItemList[Item];
			if(pvalue){
				*pvalue = uD->DoubleAux;
			}else{
				return 1;
			}
			break;
		case uDialogGetLogical:
			plflag=va_arg(ap,long *);
			uD = &uList->uDialogItemList[Item];
			if(plflag && uD->Control){
				*plflag = uGetControlValue(uD->Control);
			}else{
				return 1;
			}
			break;
		case uDialogGetDouble:
			pvalue=va_arg(ap,double *);
			uD = &uList->uDialogItemList[Item];

			if(uD->Control){
				if(myIcon && uD->Control->controlType == controlTypeEdit) {
					GetDlgItemText(myIcon->myWindow,uD->Control->controlID,buff,255);
					if(pvalue)*pvalue = atof(buff);
				}
			}
			break;
		case uDialogGetLong:
			lvalue=va_arg(ap,long *);
			uD = &uList->uDialogItemList[Item];
			if(uD->Control){
				if(myIcon && uD->Control->controlType == controlTypeEdit) {
					GetDlgItemText(myIcon->myWindow,uD->Control->controlID,buff,255);
					if(lvalue)*lvalue = atol(buff);
				}
			}
			break;
		case uDialogGetScrollList:
			List=va_arg(ap,struct ScrollList **);
			if(!List || !uList->uDialogItemList[Item].Scroll)return 1;
			*List=uList->uDialogItemList[Item].Scroll;
			break;
		case uDialogGetControl:
			ControlPtr=va_arg(ap,controlPtr *);
			if(!ControlPtr || !uList->uDialogItemList[Item].Control)return 1;
			*ControlPtr=uList->uDialogItemList[Item].Control;
			break;
		case uDialogGetControlMenu:
			menu=va_arg(ap,menuPtr *);
			if(!menu || !uList->uDialogItemList[Item].Control)return 1;
			*menu=(menuPtr)uList->uDialogItemList[Item].Control;
			break;
			
			
			
		}
	}

	if(myIcon)SetFocus(myIcon->myWindow);

	return 0;
}
static int fClose(IconPtr myIcon)
{
	uDialogList *uList;
	dialogPtr s;
			
	if(!myIcon)return 1;

	if(myIcon->doNotClose){
	     myIcon->done = -1;
	     return 1;
	}	
	

	if(myIcon){
	    s=(dialogPtr)myIcon->DWindow;
	    SetPortMyWindow(myIcon);
	    
   	    if(s){
			uList=s->uList;
			if(uList){
			   uList->myIcon=NULL;
			   uDialogClose(uList);
			}
		    cFree((char *)s);
	    }
	       
	    
	    myIcon->DWindow=NULL;
	    
	    myIcon=RemoveIcon(myIcon);
	    
	}
	return 0;

}
int uDialogClose(uDialogList *uList)
{
    uDialogItem *uDialogItemList;
    IconPtr myIconFrontWindow;
    IconPtr myIconItem;
    IconPtr myIcon;
    long n;
    
	if(!uList)return 1;
	
	myIcon=uList->myIcon;
    uList->myIcon=NULL;
	myIconItem=uList->myIconItem;
	myIconFrontWindow=uList->myIconFrontWindow;
	
	uDialogItemList=uList->uDialogItemList;

	if(uList->RectPtr){

		if(!uGetPortBoundsGlobal(myIcon,uList->RectPtr)){
			uList->RectPtr->xsize=uList->Rect.xsize;
			uList->RectPtr->ysize=uList->Rect.ysize;
		}
	}
	
	if(uDialogItemList){
		for(n=0;n<uList->uDialogItemCount;++n){
		    uDialogItem *uD;
		    
			uD = &uList->uDialogItemList[n];
			
		    switch(uD->type){
	           case uDialogNone:
	           break;
	           case uDialogButton:
	               break;
	           case uDialogScrollList:
				if(uD->Scroll ){
				    ScrollListDispose(uD->Scroll);
				    uD->Scroll=NULL;
				}
	           break;
	           case uDialogText:
/*				if(uD->textH){
				    TEDispose(uD->textH);
				    uD->textH=NULL;
				}*/
	           break;
	           case uDialogButtonRadio:
	               break;
	           case uDialogCheck:
	               break;
		    }
		    
		}
	    if(uDialogItemList)cFree((char *)uDialogItemList);
	    uDialogItemList=NULL;
	}
	
	if(uList->pClose)(*uList->pClose)(myIconItem);
	
	if(uList)cFree((char *)uList);
	uList=NULL;
	
	if(myIcon){
		dialogPtr s;
	    s=(dialogPtr)myIcon->DWindow;
	    if(s)s->uList=NULL;
	    myIcon->doNotClose = 0;
	    fClose(myIcon);
	}
	
	if(myIconFrontWindow){
	    SetMyIconWindowFront(myIconFrontWindow);
	}
	
	return 0;
}
int uDialog(uDialogList *uList)
{
	controlPtr scroll;
	char buff[256],title[256];
	IconPtr myIcon;
	dialogPtr d;
	uRect r;
	uPort save;
	long n;
	int ret;
	
	ret=1;
	
	if(!uList)return 1;
	
	uGetPort(&save);
	
	d=NULL;
	
	if(!uList->isOpen || !uList->myIcon){

		if(!(myIcon=uCreateIcon(uList->WindowType,&uList->Rect))){
		    Warning("uDialog Out of Memory");
			return 1;
		}
		
		if((d=(dialogPtr)cMalloc(sizeof(struct dialogFile),8198)) == NULL){
		    Warning("uDialog Out of Memory");
		    goto EndIt;
		}
		zerol((char *)d,sizeof(struct dialogFile));
		
		myIcon->DWindow=(struct DumWindow *)d;
		
	    myIcon->WindowClass=DialogType;

		sprintf(buff,"%s",uList->name);
		SetWindowName(myIcon,buff);

		d->uList=uList;
		
		uList->myIcon=myIcon;
		
/*		HideWindow(myIcon->myWindow);*/
		if (myIcon->VControl) {
			ControlDispose(myIcon->VControl);
		}
		if (myIcon->HControl) {
			ControlDispose(myIcon->HControl);
		}
		
		myIcon->VControl=NULL;
		myIcon->HControl=NULL;

		uSizeWindowGlobal(myIcon,uList->Rect.xsize,uList->Rect.ysize+20);
	
		myIcon->pUpdateWrite=fUpdateWrite;
		myIcon->pInContent=fDoInContent;
		myIcon->pKeyBoard=fKeyBoard;
		myIcon->pMoveControls=fMoveControls;
//		myIcon->pGrow=fGrow;
		myIcon->pClose=fClose;
//		myIcon->pCursor=fCursor;
//		myIcon->pKeyDown=DialogKeyDown;
		

/*		TextFont(systemFont);
		TextFace(PLAIN);
		TextSize(14);*/

//		GetFontStuff(&d->systemFont);
//		GetFontStuff(&d->listFont);
		
		d->KeyTicks=TickCount();

		getPaletteRGB(12,myIcon->red,myIcon->green,myIcon->blue);
		ScreenSetPalette(myIcon,myIcon->red,myIcon->green,myIcon->blue);

		for(n=0;n<uList->uDialogItemCount;++n){
		    uDialogItem *uD;
		    
			uD = &uList->uDialogItemList[n];
			
			r=uD->Rect;
			
		    switch(uD->type){
	           case uDialogNone:
	           break;
	           case uDialogButton:
	               mstrncpy(title,uD->name,256);
	               if(!strcmp(title,"Ok"))uD->type=uDialogButtonOK;
	               if(!strcmp(title,"Cancel"))uD->type=uDialogButtonCancel;
	               
	               scroll=ControlCreate(myIcon,&r,(unsigned char *)title,
	                             controlTypeButton,0,0,1,0,0,0);
	               if(!scroll){
	                    sprintf(WarningBuff,"Error Creating Item %ld (Button)\n",n);
	                    WarningBatch(WarningBuff);
	                    goto EndIt;
	               }
	               uD->Control=scroll;
	               break;
	           case uDialogScrollList:
			 	uD->Scroll=ScrollListNew(uD->Rect.y,uD->Rect.x,20,315,10,myIcon);
				if(uD->Scroll == NULL){
	                sprintf(WarningBuff,"Error Creating Item %ld (ScrollList)\n",n);
	                WarningBatch(WarningBuff);
	                goto EndIt;
				}
/*				uD->Scroll->ListData->listTextFont=systemFont;
				ListSetStringRange(0L,9999999L,uD->Scroll->ListData);
				uD->Scroll->ListData->listTextSize=14;
				uD->Scroll->ListData->listTextFace=PLAIN;*/
	       	    ScrollListFitRect(&r,uD->Scroll);
 	           break;

	           case uDialogString:
	               mstrncpy(title,uD->name,256);
	               
	               scroll=ControlCreate(myIcon,&r,(unsigned char *)title,
	                             controlTypeStatic,0,0,1,0,0,0);
	               if(!scroll){
	                    sprintf(WarningBuff,"Error Creating Item %ld (Button)\n",n);
	                    WarningBatch(WarningBuff);
	                    goto EndIt;
	               }
	               uD->Control=scroll;
				   uSetControlValue(uD->Control,uD->isStart);
	               break;

			   case uDialogText:
	               mstrncpy(title,uD->name,256);
	               
	               scroll=ControlCreate(myIcon,&r,(unsigned char *)title,
	                             controlTypeEdit,0,0,1,0,0,0);
	               if(!scroll){
	                    sprintf(WarningBuff,"Error Creating Item %ld (Button)\n",n);
	                    WarningBatch(WarningBuff);
	                    goto EndIt;
	               }
	               uD->Control=scroll;
				   uSetControlValue(uD->Control,uD->isStart);
	               break;

	           case uDialogButtonRadio:
	               mstrncpy(title,uD->name,256);
	               
	               scroll=ControlCreate(myIcon,&r,(unsigned char *)title,
	                             controlTypeRadio,0,0,1,0,0,uD->RadioGroup);
	               if(!scroll){
	                    sprintf(WarningBuff,"Error Creating Item %ld (Button)\n",n);
	                    WarningBatch(WarningBuff);
	                    goto EndIt;
	               }
	               uD->Control=scroll;
				   uSetControlValue(uD->Control,uD->isStart);
	               break;
	           case uDialogPopUpMenuControl:
	               mstrncpy(title,uD->name,256);
	               
	               scroll=ControlCreate(myIcon,&r,(unsigned char *)title,
	                             controlTypePopUpMenu,0,0,1,0,0,0);
	               if(!scroll){
	                    sprintf(WarningBuff,"Error Creating NewControl Item %ld (uDialogPopUpMenu)\n",n);
	                    WarningBatch(WarningBuff);
	                    goto EndIt;
	               }
	               uD->Control=scroll;
				   uSetControlValue(uD->Control,uD->isStart);
	               break;
	               
	           case uDialogCheck:
	               mstrncpy(title,uD->name,256);
	               
	               scroll=ControlCreate(myIcon,&r,(unsigned char *)title,
	                             controlTypeCheck,0,0,1,0,0,0);
	               if(!scroll){
	                    sprintf(WarningBuff,"Error Creating Item %ld (Button)\n",n);
	                    WarningBatch(WarningBuff);
	                    goto EndIt;
	               }
	               uD->Control=scroll;
				   uSetControlValue(uD->Control,uD->isStart);
	               break;
		    }
		    
		}
					

	   if(uList->uDialogStart)(*uList->uDialogStart)(uList);
   
   }else{
   		myIcon=uList->myIcon;
   		if(!myIcon)return 1;
   		d=(dialogPtr)myIcon->DWindow;
   		if(!d)goto EndIt;
   }

	myIcon->done = 0;
   		
	uList->isOpen = TRUE;
   
	if(uList->uDialogMove)(*uList->uDialogMove)(uList);
		
	SetButtons(d);
	
	
	myIcon->doNotClose=1;

	if(!uList->ModeLess){
		if(myIcon)myIcon->isOpen=TRUE;
 	    doSystemFile(myIcon);
	}else{
	   myIcon->done = 1;
	}
	
	myIcon->doNotClose=0;	
	
	ret=0;
	
EndIt:

    if(ret){
        ;
	}else{
	    if(myIcon->done != 1)ret = 1;
	}
	    
	myIcon->done = 0;
	
	uSetPort(&save);
	
	return ret;
}

static int doSystemFile(IconPtr myIcon)
{
	IconPtr FindMyFront;
	unsigned long time;
	int bRet;
	MSG msg;
	
	if(!myIcon)return 1;
	
	time=TickCount();

	while((bRet = GetMessage(&msg,NULL , 0, 0 )) != 0){ 
		if(bRet == -1)break;
		if(TickCount()  > time + 15){
			FindMyFront= FindMyFrontWindow();
			if(FindMyFront && (FindMyFront != myIcon) && FindMyFront->isOpen){			
				BringWindowToTop(myIcon->myWindow);
			}
			time=TickCount();

		}

		if(FindIWindow(msg.hwnd) && FindIWindow(msg.hwnd) != myIcon && 
			((msg.message & 0xFFFF) == WM_LBUTTONDOWN)){
				myIcon->done=-1;
				break;
			}


		if(!TranslateAcceleratorM(&msg)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if(!FindMyIcon(myIcon) || myIcon->done){
			break;
		}
    }

	return 0;
}

uDialogList *uDialogGetList(IconPtr myIcon)
{
	uDialogList *uList;
	dialogPtr s;
	
	if(!myIcon|| (s=(dialogPtr)myIcon->DWindow) == NULL)return NULL;
	uList=s->uList;
	return uList;
}
void fMoveControls(IconPtr myIcon)
{
	uDialogList *uList;
	dialogPtr s;

	if(!myIcon || (s=(dialogPtr)myIcon->DWindow) == NULL)return;
	uList=s->uList;
	if(!uList)return;

	if(uList->uDialogMove)(*uList->uDialogMove)(uList);
	if(uList)uDialogUpdate(uList);
}
static int fKeyBoard(IconPtr myIcon,int c)
{
	int found;
	uPoint pt;
	dialogPtr s;
	uDialogList *uList;
	uRect cellRect;
	int n,nn,nf;
	unsigned long timeTick;
	
	if(!myIcon || (s=(dialogPtr)myIcon->DWindow) == NULL)return 1;
	uList=s->uList;
	if(!uList)return 1;
	
	uPointer(&pt,myIcon);
	found=FALSE;
	
	for(n=0;n<uList->uDialogItemCount;++n){
	    uDialogItem *uD;
	    uD = &uList->uDialogItemList[n];
	    cellRect=uD->Rect;
	    switch(uD->type){	    	    
        case uDialogScrollList:
		    if(uD->isActive  && !uD->isHidden && uPtInRect(pt,&cellRect)){
		        goto doKeys;
		    }
		    break;	    	    
        case uDialogText:
		    if(uD->isActive  && !uD->isHidden){
			    if(n == uList->activeText){
			       if(c == '\t'){			
			       		found=FALSE;
			            nf=n+1;  
TryAgain:  
						for(nn=nf;nn<uList->uDialogItemCount;++nn){
							uD = &uList->uDialogItemList[nn];
							if((uD->type == uDialogText) && uD->isActive  && !uD->isHidden){
							    uList->activeText=nn;
//							    TESetSelect(0L,(long)1000,uD->textH);
							    found=TRUE;
							    break;
							}
						}
						if(!found && nf != 0){
						    nf=0;
						    goto TryAgain;
						}			       
			       }else{
//			       		TEKey(c,uD->textH);
			       }
			       found=TRUE;
			       goto OutOfHere;
		    	}
       	     }
        break;
	    }
	}
OutOfHere:
	if(found)return 0;
		
doKeys:

	
	timeTick=TickCount();
	if((timeTick-s->KeyTicks) > 60){
 	    uList->count=0;
	}
	uList->mess[uList->count++]=c;
	uList->mess[uList->count]=0;
	

	
	if(uList->uDialogKeys){
	    (*uList->uDialogKeys)(uList);
	}

	if(FindWindowInList(myIcon)){
		s->KeyTicks=TickCount();
		SetButtons(s);
	}
    return 0;			
}
int fDoInContent(IconPtr myIcon)
{
	uDialogList *uList;
	dialogPtr s;
	struct ScrollList *List;
	uPoint uwhere;
	uRect cellRect;
	int found;
	int n;
	
	
	if(!myIcon|| (s=(dialogPtr)myIcon->DWindow) == NULL)return 1;
	uList=s->uList;
	if(!uList)return 1;

	SetFocus(myIcon->myWindow);

	uPointer(&uwhere,myIcon);

	found=FALSE;
	for(n=0;n<uList->uDialogItemCount;++n){
	    uDialogItem *uD;
	    uD = &uList->uDialogItemList[n];
	    cellRect= uD->Rect;
	    switch(uD->type){
        case uDialogScrollList:
            List=uD->Scroll;
		    if(uD->isActive  && !uD->isHidden && uPtInRect(uwhere,&cellRect)){
			    ScrollListDoControl(myIcon,uwhere,List);
			    if(List->DoubleClick){
					if(uList->uDialogDoubleClick){
						uList->actionItem=n;
					    if((*uList->uDialogDoubleClick)(uList))goto OutOfHere;
					}
					List->DoubleClick=FALSE;
			    }
   				if(uList->uDialogAction){
		            uList->actionItem=n;
   				    if((*uList->uDialogAction)(uList))goto OutOfHere;
   				 }
			    goto OutOfHere;
       	     }
        break;
        case uDialogFillRect:
            uInsetRect(&cellRect,-2,-2);
		    if(uD->isActive  && !uD->isHidden && uPtInRect(uwhere,&cellRect)){
   				if(uList->uDialogAction){
		            uList->actionItem=n;
   				    if((*uList->uDialogAction)(uList))goto OutOfHere;
   				 }
			    goto OutOfHere;
       	     }
        break;
        case uDialogText:
		    if(uD->isActive && !uD->isHidden && uPtInRect(uwhere,&cellRect)){
		        uList->activeText=n;
	    	    found = TRUE;
			    goto OutOfHere;
       	     }
        break;
        case uDialogUser:
		    if(uD->isActive && !uD->isHidden && uPtInRect(uwhere,&cellRect)){
   				if(uList->uDialogAction){
		            uList->actionItem=n;
   				    if((*uList->uDialogAction)(uList))return 1;
   				}
	    	    found = TRUE;
			    goto OutOfHere;
       	     }
        break;
	    }
	}
		
OutOfHere:

 	if(!FindWindowInList(myIcon))return 1;

	if(found){
		for(n=0;n<uList->uDialogItemCount;++n){
		    uDialogItem *uD;
		    uD = &uList->uDialogItemList[n];
	    	cellRect=uD->Rect;
		    switch(uD->type){
	        case uDialogText:
		        if(uD->isActive && !uD->isHidden)uInvalWindowRect(myIcon,&cellRect);
		        break;
		    }
		}
	    return 1;
	}

    SetButtons(s);

   if(myIcon->done && uList->ModeLess){
    	uDialogClose(uList);
    }

	return 0;
}

static int SetButtons(dialogPtr s)
{
	uDialogList *uList;
	if(!s)return 1;
	uList=s->uList;
	if(!uList)return 0;
	
	if(uList->uDialogButtons)(*uList->uDialogButtons)(uList);
	
	return 0;
}
int DoControlDialog(IconPtr myIcon,long controlID)
{
	dialogPtr nf;
	uDialogList *uList;
	int newp;
	long label,n,k;

	if(!myIcon)return 0;
	nf=(dialogPtr)myIcon->DWindow;
	if(!nf)return 0;
	uList=nf->uList;
	if(!uList)return 0;
	
	label=-1;
	for(n=0;n<uList->uDialogItemCount;++n){
		uDialogItem *uD;
		uD = &uList->uDialogItemList[n];
		switch(uD->type){
        case uDialogPopUpMenuControl:
        case uDialogButtonRadio:
        case uDialogButton:
        case uDialogButtonOK:
        case uDialogButtonCancel:
        case uDialogCheck:
        case uDialogText:
			if(uD->Control) {
				if(uD->Control->controlID==controlID) {
					label = n;
					break;
				}
			}
			break;
		}
	}
	if(label<0) return 1;
	
	for(n=0;n<uList->uDialogItemCount;++n){
		uDialogItem *uD;
		uD = &uList->uDialogItemList[n];
		if(label != n)continue;
		switch(uD->type){
        case uDialogText:
		    uList->activeText=n;
			break;
		case uDialogPopUpMenuControl:
			if(uList->uDialogAction){
				uList->actionItem=n;
				if((*uList->uDialogAction)(uList))return 1;
			}
			break;
		case uDialogCheck:
			newp=uGetControlValue(uD->Control);
			uSetControlValue(uD->Control,newp);
			if(uList->uDialogAction){
				uList->actionItem=n;
				if((*uList->uDialogAction)(uList))return 1;
			}
			break;
		case uDialogButtonRadio:
			for(k=0;k<uList->uDialogItemCount;++k){
				uDialogItem *u;
				u = &uList->uDialogItemList[k];
				if((u->type != uDialogButtonRadio) || 
					(u->RadioGroup != uD->RadioGroup ))continue;
				uSetControlValue(u->Control,(k == n));
			}
			if(uList->uDialogAction){
				uList->actionItem=n;
				if((*uList->uDialogAction)(uList))return 1;
			}
			break;
		case uDialogButton:
			if(uList->uDialogAction){
				uList->actionItem=n;
				if((*uList->uDialogAction)(uList))return 1;
			}
			break;
		case uDialogButtonOK:
			if(uList->uDialogAction){
				uList->actionItem=n;
				if((*uList->uDialogAction)(uList))return 1;
			}
			myIcon->done = 1;
			break;
		case uDialogButtonCancel:
			myIcon->done = -1;
			break;	
		}
	}
	
 	if(!FindWindowInList(myIcon))return 1;

    if(myIcon->done && uList->ModeLess){
    	uDialogClose(uList);
    }


	return 0;
}

int uDialogUpdate(uDialogList *uList)
{
	if(!uList)return 1;
	fUpdateWrite2(uList->myIcon,1);
	return 0;
}
int fUpdateWrite(IconPtr myIcon)
{
	return fUpdateWrite2(myIcon,0);
}
int fUpdateWrite2(IconPtr myIcon,int flag)
{
	RECT xr;
	dialogPtr s;
    uRect portRect;
	uRect cellRect;
	uDialogList *uList;
    long n;
	
	if(myIcon && (s=(dialogPtr)myIcon->DWindow) != NULL){
	   
	   	   
        uGetPortBounds(myIcon,&portRect);
        uClipRect(&portRect,myIcon);
	    // uEraseRect(&portRect,myIcon);

		xr.left=portRect.x;
		xr.top=portRect.y;	
		xr.right=portRect.x + portRect.xsize;
		xr.bottom=portRect.y + portRect.ysize;


	    uList=s->uList;	    
	    
	    
	    if(uList){
			for(n=0;n<uList->uDialogItemCount;++n){
			    uDialogItem *uD;
			    
			    uD = &uList->uDialogItemList[n];
			    
				cellRect=uD->Rect;
				
			    switch(uD->type){
	            case uDialogScrollList:
	                if(uD->Scroll){
	           	        if(!uD->isHidden){
		           	        ShowControl(uD->Scroll->control1);
		           	        ShowControl(uD->Scroll->control2);
	           	            ScrollListUpdate(uD->Scroll);
	           	        }else{
		           	        HideControl(uD->Scroll->control1);
		           	        HideControl(uD->Scroll->control2);
	           	        }
	           	    }
	            break;
	            case uDialogFillRect:
					
	            	SetFillAttributes(myIcon,&uD->FillData);
	    			uFillRect(&uD->Rect,myIcon);
					uSetForeColor(255,255,255,myIcon);
					uSetBackColor(0,0,0,myIcon);
					
	            break;	            
	            case uDialogFrameRect:
	            	SetFillAttributes(myIcon,&uD->FillData);
	    			uFrameRect(&uD->Rect,myIcon);
	            	uSetForeColor(0,0,0,myIcon);
					uSetBackColor(255,255,255,myIcon);
	            break;	            
	            case uDialogString:
	            case uDialogText:
	            case uDialogPopUpMenuControl:
	            case uDialogCheck:
	            case uDialogButtonRadio:
	            case uDialogButton:
	            case uDialogButtonOK:
	            case uDialogButtonCancel:
	            	if(uD->Control){
		           	    if(uD->isHidden){
		           	        ControlHide(uD->Control);
		           	    }else{
		           	        ControlShow(uD->Control);
		           	    }
	           	    }
	            break;
			    }
			}
			
			if(uList->uDialogUserDraw){
			    (*uList->uDialogUserDraw)(uList);
			}
	 
	    }
	    
	    uDrawControls(myIcon);
		if(flag)InvalidateRect(myIcon->myWindow,&xr,TRUE);        	    

	}
	return 0;
}
int FindFound(IconPtr myIconSearch,char *string,int flag)
{
	uDialogReturn *uR;
 	uDialogList *uList;
	IconPtr p,iclose;
	dialogPtr s;
	int ret;

	uR=NULL;
	p=IconRoot;
	iclose=NULL;
	ret=0;
	while(p){
	    if(p->WindowType == xChangeType){
	    	s=(dialogPtr)p->DWindow;
	    	if(!s)continue;
	    	uList=s->uList;
	    	if(!uList)continue;	 
	    	if(flag == 0){  	
				uR=uDialogGetReturn(uList);
			    if(!uR){
				    Warning("Error Getting Search String Specified");
				    return 1;
			    }
				string[0]=0;
				mstrncpy(string,uR[5].sreturn,255);
		        iclose=p;
	        }else{
				uList->myIconItem=myIconSearch;
				if(uList->myIcon)BringWindowToTop(uList->myIcon->myWindow);
				ret=1;	        
	        }
			break;
	    }else if(p->WindowType == xFindType){
	    	s=(dialogPtr)p->DWindow;
	    	if(!s)continue;
	    	uList=s->uList;
	    	if(!uList)continue;
	    	if(flag == 0){
				uList->myIconItem=myIconSearch;
				if(uList->myIcon)BringWindowToTop(uList->myIcon->myWindow);
				ret=1;
			}else{
				uR=uDialogGetReturn(uList);
			    if(!uR){
				    Warning("Error Getting Search String Specified");
				    return 1;
			    }
				string[0]=0;
				mstrncpy(string,uR[3].sreturn,255);
		        iclose=p;
			}
			break;
	    }
	    p=p->IconNext;
	}

	if(iclose)(*iclose->pClose)(iclose);
	
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
