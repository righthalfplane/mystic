#define EXTERN22 extern
#include <ctype.h>
#include "Xappl.h"
#include "uLib.h"
#include "uFiles.h"
#include "paletteDraw.h"
#include "Linedata.h"
#include "Message.h"
#include "uDialog.h"
#include "FileManager.h"
#include "Tools.h"
#include "Raster2D.h"

static int netOptions(IconPtr myIcon);

static int doOptions(IconPtr myIcon);

int RasterNetMenu(IconPtr myIcon,int NetMeu,int item)
{
	if(!myIcon)return 1;
	if(NetMeu != NetMeu4)return 1;
	
	
	switch(item){
		case listenCommand:		
		    uNetStartListen(myIcon);		    		    
		break;
		case listenSetCommand:		
		    uNetSetStartListen(myIcon);		    
		break;
		case listenStopCommand:		
		    uNetStopListen(myIcon);		    
		break;
		case netOptionsCommand:				
			netOptions(myIcon);					
		break;
		case hdfToMemoryCommand:		
	    	myIcon->uNet.SaveHdfToMemory = !myIcon->uNet.SaveHdfToMemory;	    	
		break;
		case hdfToDiskCommand:		
	    	myIcon->uNet.SaveHdfToDisk = !myIcon->uNet.SaveHdfToDisk;	    	
		break;
		case hdfToAppendCommand:
	    	myIcon->uNet.AppendHdfToMemory = !myIcon->uNet.AppendHdfToMemory;
		break;
		case trueToMemoryCommand:
	    	myIcon->uNet.SaveTrueToMemory = !myIcon->uNet.SaveTrueToMemory;
		break;
		case trueToDiskCommand:
	    	myIcon->uNet.SaveTrueToDisk = !myIcon->uNet.SaveTrueToDisk;
		break;
		case optionsCommand:
	    	doOptions(myIcon);
		break;
		
		
		
	}	
	return 0;
}
static int netOptions(IconPtr myIcon)
{
	static uDialogGroup Items[] =
	
        {
      
      /*  0 */      {"Ok", {50,110,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {415,110,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"l-port", {170,34,311,21},uDialogText,uDialogGetLong},
      /*  3 */      {"l-pass", {170,64,315,19},uDialogText,uDialogNone},
      /*  9 */      {"Listen Port     :", {35,37,125,22},uDialogString,uDialogNone},
      /* 11 */      {"Listen Password :", {35,64,126,19},uDialogString,uDialogNone},
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,530,160};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	char out2[256];
	
	if(!myIcon)return 1;
	
	mstrncpy(out2,(char *)&myIcon->uNet.ListenPassword,255);	
		
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("Network Options",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)myIcon->uNet.ListenPort,
	              
	              uDialogSetItem,3L,
	              uDialogSetText,out2,
	              	              	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	mstrncpy(out2,uR[3].sreturn,255);
	
	mstrncpy((char *)&myIcon->uNet.ListenPassword,out2,255);	
	
	myIcon->uNet.ListenPort=(short int)uR[2].lreturn;
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	return ret;
}
static int doOptions(IconPtr myIcon)
{
	static uDialogGroup Items[] =
	
        {
      
      /*  0 */      {"Ok", {375,125,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {450,125,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"QED1", {87,59,45,20},uDialogText,uDialogNone},
      /*  3 */      {"TEXT", {154,59,45,20},uDialogText,uDialogNone},
      /*  4 */      {"DAD2", {87,89,45,20},uDialogText,uDialogNone},
      /*  5 */      {"PICT", {154,89,45,20},uDialogText,uDialogNone},
#ifdef MONTAGE_999      
      /*  6 */      {"Count", {361,50,45,20},uDialogText,uDialogGetLong},
      /*  7 */      {"X", {320,87,40,20},uDialogText,uDialogGetDouble},
      /*  8 */      {"Y", {450,90,45,20},uDialogText,uDialogGetDouble},
#endif  
      /*  9 */      {"Text  :", {30,63,47,20},uDialogString,uDialogNone},
      /* 10 */      {"Binary :", {20,93,54,20},uDialogString,uDialogNone},
      /* 11 */      {"Owner", {86,29,47,20},uDialogString,uDialogNone},
      /* 12 */      {"Type", {158,29,47,20},uDialogString,uDialogNone},
      /* 13 */      {"File Attributes", {56,9,106,17},uDialogString,uDialogNone},
#ifdef MONTAGE_999      
      /* 14 */      {"Montage Layout", {292,12,122,16},uDialogString,uDialogNone},
      /* 15 */      {"X Scale  :", {250,87,60,20},uDialogString,uDialogNone},
      /* 16 */      {"Column Count :", {248,50,106,20},uDialogString,uDialogNone},
      /* 17 */      {"Y Scale  :", {380,89,63,20},uDialogString,uDialogNone},
#endif  
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,530,174};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	static char out1[64],out2[64],out3[64],out4[64];
	
	if(!myIcon)return 1;
	
	mstrncpy(out1,(char *)myIcon->uNet.TEXTOwner,4L);
	mstrncpy(out2,(char *)myIcon->uNet.TEXTType,4L);
	mstrncpy(out3,(char *)myIcon->uNet.BinaryOwner,4L);
	mstrncpy(out4,(char *)myIcon->uNet.BinaryType,4L);
	
	out1[4]=0;
	out2[4]=0;
	out3[4]=0;
	out4[4]=0;
		
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
		
	uList=uDialogOpen("Options",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetText,out1,
	              
	              uDialogSetItem,3L,
	              uDialogSetText,out2,
	              
	              uDialogSetItem,4L,
	              uDialogSetText,out3,
	              
	              uDialogSetItem,5L,
	              uDialogSetText,out4,
#ifdef MONTAGE_999      
	              	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,(double)NWindow->mCount,
	              	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,(double)NWindow->mXscale,
	              	              
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)NWindow->mYscale,
	              	              
#endif	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
#ifdef MONTAGE_999      
Start:	
#endif	              


    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	mstrncpy((char *)myIcon->uNet.TEXTOwner,uR[2].sreturn,5L);
	mstrncpy((char *)myIcon->uNet.TEXTType,uR[3].sreturn,5L);
	mstrncpy((char *)myIcon->uNet.BinaryOwner,uR[4].sreturn,5L);
	mstrncpy((char *)myIcon->uNet.BinaryType,uR[5].sreturn,5L);
	
#ifdef MONTAGE_999      
	if(uR[6].lreturn <= 0){
	    Warning("Montage Scale Factor Must Be Greater Than Zero");
	    goto Start;
	}
	
	NWindow->mXscale=uR[7].dreturn;
	NWindow->mYscale=uR[8].dreturn;
	NWindow->mCount=(int)uR[6].lreturn;
#endif	              
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	InvalRectMyWindow(myIcon);
	
	return ret;
}
