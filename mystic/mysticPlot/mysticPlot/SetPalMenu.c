#define EXTERN22 extern
#include "Xappl.h"
#include "uDialog.h"
#include <ctype.h>
#include "Universal.h"
extern int SetDefaultPaletteName(IconPtr myIcon,char *name1);

int doAddPalette(IconPtr myIcon,char *name);

int doAddPalette(IconPtr myIcon,char *name)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {390,73,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {390,36,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"Palette-Name", {14,47,362,20},uDialogText,uDialogNone},
      /*  3 */      {"Enter Palette Name", {95,8,156,19},uDialogString,uDialogNone},
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,466,124};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	static char out1[64]={'M','y','.','p','a','l',};
	unsigned char rgb[256*3];
	
	if(!myIcon)return 1;
	if(GetPaletteColors(myIcon,&rgb[0],&rgb[256],&rgb[512]))return 1;

	if(!name){
	
		uGetPort(&save);
		
		ret = 1;
		
		uR=NULL;
		
		uList=uDialogOpen("New Palette",&Rect1,ItemCount);
		
		if(!uList)goto ErrorOut;
		
		
		if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
		
		if(uDialogSet(uList,2L,
		
		              uDialogSetText,out1,
		              
		              
		              uDialogSetDone,uDialogSetDone
		    ))goto ErrorOut;
	    
	    if(uR)uDialogFreeReturn(uR);
	    uR=NULL;
	    
		if(uDialog(uList))goto ErrorOut;
		
		
		uR=uDialogGetReturn(uList);

		if(!uR)goto ErrorOut;
		
		mstrncpy(out1,uR[2].sreturn,255);
		
	    name=out1;
	    
		ret = 0;
		
	ErrorOut:

	    if(uList)uDialogClose(uList);
	    if(uR)uDialogFreeReturn(uR);
	    
		
		uSetPort(&save);
	
	    if(ret)return 1;
	}

	SetDefaultPaletteName(myIcon,name);
		
	addUniversalPalette(name,&rgb[0],&rgb[256],&rgb[512]);

	return 0;
}
