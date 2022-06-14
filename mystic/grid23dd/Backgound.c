#include <stdio.h>
#ifdef THINK_C
#include <Quickdraw.h>
#define WaitNextTrap		 	0x60
#define TrapNotImplemented		0x9F
#define Sleep				0L
#define SUSPEND_RESUME_BIT		0x0001
#define RESUMING			1
static Boolean WaitNext;
MacEvents()
{

	static Point	diskInitPt = {120,100 };
	EventRecord myEvent;
	
	if (WaitNext)
	    WaitNextEvent(everyEvent,&myEvent,Sleep,NULL);
	else
	{
	    SystemTask();
	    GetNextEvent(everyEvent,&myEvent );
	}
	
	switch(myEvent.what){
	case mouseDown:
	    MacMouseDown(&myEvent);
	    break;
	case nullEvent:
	    return 0;
	    break;
	case updateEvt:
	    break;
	case activateEvt:
	    DrawMenuBar();
	    break;
	case keyDown:
	case autoKey:
	    break;
	case diskEvt:
	    if (HiWord (myEvent.message) != noErr){
		DILoad ();
		(void) DIBadMount (diskInitPt,myEvent.message);
		DIUnload ();
	    }
	    break;
	case app4Evt:
	    if ((myEvent.message & SUSPEND_RESUME_BIT) == RESUMING)
	        ;
	    else
	        ;
	    break;
	}
	return 1;
}
MacMouseDown(myEvent)
EventRecord *myEvent;
{
	WindowPtr whichWindow;
	int place;
	
	place=FindWindow(myEvent->where,&whichWindow);
	switch(place){
	    case inDesk:
		break;
	    case inMenuBar:
		MacMenu(MenuSelect(myEvent->where));
		break;
	    case inSysWindow:
		SystemClick(myEvent,whichWindow);
		break;
	    case inContent:
		break;
	    case inDrag:
		break;
	    case inGrow:
		break;
	    case inGoAway:
		break;
	    default:
		break;
	} 
}
MacMenu(menudata)
long menudata;
{
	GrafPtr Save;
	WindowPtr myWindow;

	int menu,item;
	char name[36];
	
	menu=HiWord(menudata);
	item=LoWord(menudata);
	if(menu == 1){
	    GetPort(&Save);
	    GetItem(GetMHandle(menu),item,name);
	    OpenDeskAcc(name);
	    SetPort(Save);
	}else if(menu == 2){
	    if(item == 1)exit(1);
	}
}
MacStart()
{
	pascal long ZoneError();
	MenuHandle menu,menuf;
	SysEnvRec SysRec;
	long Longest;
	int k;
/*
	InitGraf(&thePort);
	MaxApplZone ();
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(0L);
	SetFractEnable((Boolean)TRUE);
	InitCursor();
	FlushEvents(everyEvent,0);
*/
	WaitNext = (NGetTrapAddress(WaitNextTrap,ToolTrap) !=
		    NGetTrapAddress(TrapNotImplemented,ToolTrap));
/*
	menu=NewMenu(500,"\p\024");
	AddResMenu(menu,'DRVR');
	InsertMenu(menu,0);
	menuf=NewMenu(501,"\pFile");
	AppendMenu(menuf,"\pQuit");
	InsertMenu(menuf,0);
	DrawMenuBar();
*/
}
#endif

