#define EXTERN22 extern
#include "Xappl.h"
#include "CExpress.h"
#include "NewEdit.h"
#include "uFiles.h"
#include "uDialog.h"
#include "Message.h"
#include "Linedata.h"
int Find(IconPtr myIcon);



static int doScale2Action(struct uDialogStruct *uList);

static int doFindStringForward(struct uDialogStruct *uList);
static int doFindStringBackward(struct uDialogStruct *uList);



int Find(IconPtr myIcon)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Search Forward", {13,120,116,33},uDialogButton,uDialogNone},
      /*  1 */      {"Close", {239,120,82,33},uDialogButton,uDialogNone},
      /*  2 */      {"Reverse", {135,120,97,34},uDialogButton,uDialogNone},
      /*  3 */      {"", {20,58,300,23},uDialogText,uDialogString},
      /*  4 */      {"Search String :", {80,16,127,21},uDialogString,uDialogNone},
        };
	
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {117,45,340,170};
 	
 	uDialogList *uList;
 	
	char string[256];
	
    int ret;
	
    if(!myIcon)return 1;
    
	string[0]=0;
	if(FindFound(myIcon,string,0))return 0;
    
	ret=1;
	
	uList=uDialogOpen("Find String",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
    
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
    
	if(uDialogSet(uList,0L,
	              uDialogSetMyIcon,myIcon,
	              uDialogSetAction,doScale2Action,
	              uDialogSetModeLess,TRUE,	              
	              uDialogSetItem,3L,
	              uDialogSetText,string,
	              uDialogWindowType,xFindType,
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;
    
	if(uDialog(uList))goto ErrorOut;    
    
	ret=0;
 ErrorOut:
 
     if(ret && uList){
         uDialogClose(uList);
     }else if(uList){
         if(uList->myIcon)uList->myIcon->isOpen=TRUE;
     }

 	return ret;
}
static int doScale2Action(struct uDialogStruct *uList)
{
	
	if(!uList)return 1;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 0){
	   doFindStringForward(uList);
	}else if(uList->actionItem == 1){
	   uDialogClose(uList);
	   return 1;
	}else if(uList->actionItem == 2){
	   if(doFindStringBackward(uList))goto OutOfHere;
	}
OutOfHere:
	/* if(uList)uDialogUpdate(uList); */
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}
static int doFindStringBackward(struct uDialogStruct *uList)
{
 	IconPtr myIcon;
	IconPtr myIconSearch;
	char string[256],test[515];
	char *Data;
	long rFirst,cFirst,n,k,nc,m;
	CEditPtr t;
	int IgnoreCase;
	int c,ct,nt,jt;
	int stringmax;
	uDialogReturn *uR;

	if(!uList)return 1;
	myIcon=uList->myIcon;
	if(!myIcon)return 1;

	uR=NULL;
	myIconSearch=FindWindowInList(uList->myIconItem);

	if(!myIconSearch){
	    Warning("Window To Search Not Found");
	    return 1;
	}

	t=(CEditPtr)myIconSearch->DWindow;
	if(!t || !t->e){
	    Warning("NULL Window Pointer");
	    return 1;
	}


	uR=uDialogGetReturn(uList);

    if(!uR){
	    Warning("Error Getting Search String Specified");
	    return 1;
    }

	string[0]=0;
	mstrncpy(string,uR[3].sreturn,255);
	
    if(uR)uDialogFreeReturn(uR);

	if(!string[0]){
	    Warning("No Search String Specified");
	    return 1;
	}

	stringmax=(int)strlen(string);

	IgnoreCase=TRUE;

	if(IgnoreCase){
	    for(n=0;n<stringmax;++n){
	        c=string[n];
	        if(c >= 'a' && c <= 'z'){
	            c='A'+(c-'a');
	        }	        
	        string[n]=c;
	    }
	}


	rFirst=t->CursorRow;
	cFirst=CVideocColumn(myIconSearch,t->CursorColumn,rFirst);
	for(n=rFirst;n >= 0;--n){
	    Data=EditListLine(t->e,n);
	    if(!Data)continue;
	    nc=(long)strlen(Data);
	    if(nc < stringmax)continue;
	    for(m=0;m <nc;++m){
	        c = Data[m];
	        if(IgnoreCase && c >= 'a' && c <= 'z'){
	            ct='A'+(c-'a');	        
	        }else{
	            ct=c;
	        }
	        if(m > 512)break;
	        test[m]=ct;
	        test[m+1]=0;
	    }
	    if(n == rFirst){
			k=cFirst-1;
	    }else{
			k=nc-1;
	    }
	    nt=0;
	    for(m=k;m >= 0;--m){
			++nt;
	        if(nt < stringmax)continue;
	        for(jt=0;jt<stringmax;++jt){
	             if(test[m+jt] != string[jt])goto NoMatch;
	        }
			rFirst=n;
			cFirst=m;
			goto FoundIt;
NoMatch:
	        ;
	    }
	}

	Warning("String not found");

	return 0;
FoundIt:
	t->CursorRow=rFirst;
	t->CursorColumn=CVideocToColumn(myIconSearch,(short)(cFirst),t->CursorRow);	

	t->fRowHigh=t->lRowHigh=t->CursorRow;
	t->fColumnHigh=t->CursorColumn;
	t->lColumnHigh=t->CursorColumn+stringmax;
	CVideoCursorInView(myIconSearch);
	InvalRectMyWindow(myIconSearch);
	return 0;
}
static int doFindStringForward(struct uDialogStruct *uList)
{
 	IconPtr myIcon;
	IconPtr myIconSearch;
	char string[256];
	char *Data;
	long rFirst,cFirst,n,k,nc,m;
	CEditPtr t;
	int IgnoreCase;
	int c,ct;
	int stringcount,stringmax;
	uDialogReturn *uR;

	if(!uList)return 1;
	myIcon=uList->myIcon;
	if(!myIcon)return 1;

	uR=NULL;
	myIconSearch=FindWindowInList(uList->myIconItem);

	if(!myIconSearch){
	    Warning("Window To Search Not Found");
	    return 1;
	}

	t=(CEditPtr)myIconSearch->DWindow;
	if(!t || !t->e){
	    Warning("NULL Window Pointer");
	    return 1;
	}

	uR=uDialogGetReturn(uList);

    if(!uR){
	    Warning("Error Getting Search String Specified");
	    return 1;
    }

	string[0]=0;
	mstrncpy(string,uR[3].sreturn,255);
	
    if(uR)uDialogFreeReturn(uR);

	if(!string[0]){
	    Warning("No Search String Specified");
	    return 1;
	}

	stringmax=(int)strlen(string);

	IgnoreCase=TRUE;

	if(IgnoreCase){
	    for(n=0;n<stringmax;++n){
	        c=string[n];
	        if(c >= 'a' && c <= 'z'){
	            c='A'+(c-'a');
	        }	        
	        string[n]=c;
	    }
	}


	rFirst=t->CursorRow;
	cFirst=CVideocColumn(myIconSearch,t->CursorColumn,rFirst);
	for(n=rFirst;n<t->e->LineCount;++n){
	    Data=EditListLine(t->e,n);
	    if(!Data)continue;
	    nc=(long)strlen(Data);
	    if(nc < stringmax)continue;
	    if(n == rFirst){
			k=cFirst;
	    }else{
			k=0;
	    }
	    stringcount=0;
	    for(m=k;m<nc;++m){
	        c = Data[m];
	        if(IgnoreCase && c >= 'a' && c <= 'z'){
	            ct='A'+(c-'a');	        
	        }else{
	            ct=c;
	        }
	        if(ct == string[stringcount++]){
	            if(stringcount >= stringmax){
					rFirst=n;
					cFirst=m;
		        	goto FoundIt;
	            }
	        }else{
	            stringcount=0;
	        }
	    }
	}

	Warning("String not found");

	return 0;
FoundIt:
	t->CursorRow=rFirst;
	t->CursorColumn=CVideocToColumn(myIconSearch,(short)(cFirst+1),t->CursorRow);	

	t->fRowHigh=t->lRowHigh=t->CursorRow;
	t->lColumnHigh=t->CursorColumn;
	t->fColumnHigh=t->CursorColumn-stringmax;
	if(t->fColumnHigh < 0)t->fColumnHigh=0;
	CVideoCursorInView(myIconSearch);
	InvalRectMyWindow(myIconSearch);
	return 0;
}

