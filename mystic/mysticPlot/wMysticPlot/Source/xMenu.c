#include <stdio.h>
#include <math.h>
#include <string.h>

#define EXTERN    extern
#include "xMenu.h"
#include "Tools.h"

#define GC int
#define XColor int
#define Visual int

int IsControl(void);
int IsShift(void);

char *strsave(char *,int tag);   

//int xapplWindow(XEvent *myevent);

struct popMenu{
    HWND menuWindow;
    struct MenuBar *Data;
    struct Menu *menu;
//    XFontStruct *info;
    uRect menuRect;
    long menuCount;
    long LineCount;
    long topline;
    int MenuBarSide;
    int isExposed;
    long whichItem;
    int whichMenu;
    int screenBits;
    int xmax;
    int ymax;
    unsigned long c[8];
//    GC gc;
};

int  MenuBarUpdate(struct MenuBar *Data);
int DisposePop(struct popMenu *pop);
int DrawPopPt(struct popMenu *pop);
int DrawPop(struct popMenu *pop);
struct popMenu *BuildPop(struct Menu *menu,struct MenuBar *Data);
int Pointer(uPoint *pt,HWND window);
int MenuClick(struct Menu *menuList);
int  MenuUpdate(struct Menu *menuList);
struct Menu *MenuNew(char *name,uRect *menuRect,HWND theWindow);
int  MenuBarNext(char *name,uRect *menuRect,struct MenuBar *Data);
int  MenuBarAdd(struct Menu *menuListin,struct MenuBar *Data);
int  MenuSize(struct Menu *menuList);
int DrawTriangle3D(uRect *controlRect,HWND controlwindow,GC gc,int controlState,
                   int controlSide,unsigned long *c);

static int DrawPopItem(struct popMenu *pop,long int n,int flag);

static int DrawPalette(uRect *controlRect,HWND controlwindow,GC gc,XColor *colors,struct popMenu *pop);

int SearchVisualList(int image_depth,int  visual_class, Visual **visual);

int getPalette(int n,char *name,char *pal);

int getXcolors(char *pal,XColor *colors);

unsigned long ColorsLong(XColor *colors);


int  MenuInsert(char *name,int flag,struct Menu *menuList,int location)
{
	return 0;
}
int MenuBarGetName(int n,char *name,struct MenuBar *MenuBarList)
{
	if(!name || !MenuBarList || !MenuBarList->menuList)return 1;
	if(n < 0 || n >= MenuBarList->menuCount)return 1;
	if(!MenuBarList->menuList[n] || !MenuBarList->menuList[n]->menuName)return 1;
	mstrncpy(name,MenuBarList->menuList[n]->menuName->stringName,256);
	return 0;
}

int  MenuSetPopUpTitle(int item,struct MenuBar *MenuBarList)
{
	char buff[256];
	
	if(!MenuBarList)return 1;
	
	if(!MenuGetName(item,buff,MenuBarList->menuList[0])){
		MenuSetTitle(buff,MenuBarList->menuList[0]);
		MenuBarUpdate(MenuBarList);
	}			   
	
	
	
	return 0;
}

struct MenuBar *MenuBarNew(uRect *menuRect,HWND theWindow){
	struct MenuBar *aMenuBar;

	aMenuBar=(struct MenuBar *)cMalloc(sizeof(struct MenuBar),8734);
	if(aMenuBar == NULL)return (struct MenuBar *)NULL;
	zerol((char *)aMenuBar,(long)sizeof(struct MenuBar));

	aMenuBar->menuRect = *menuRect;
	aMenuBar->myWindow=theWindow;
//	aMenuBar->menuWindow = CreateMenu();
//	if(!aMenuBar->menuWindow){
//	    fprintf(stderr,"MenuBarNew not created\n");
//	    goto ErrorOut;
//	}
	aMenuBar->MenuBarSide=0;

	return aMenuBar;

//ErrorOut:
//	return NULL;
}
struct Menu *MenuBarAddMenus(char *MenuName,
                             struct MenuItems *Items,
                             struct MenuBar *MenuBarList)
{
	struct Menu *Menur;
	uRect menuRect;
	long n;

	if(!MenuName || !MenuBarList || !Items)goto ErrorOut;

	if(MenuBarNext(MenuName,&menuRect,MenuBarList))goto ErrorOut;
    Menur=MenuNew(MenuName,&menuRect,MenuBarList->myWindow);
    if(!Menur)goto ErrorOut;

	n=0;
	while(Items[n].ItemName){
	    if(MenuAdd(Items[n].ItemName,Items[n].ItemType,Menur))goto ErrorOut;
	    ++n;
	}

	if(MenuBarAdd(Menur,MenuBarList))goto ErrorOut;

	return Menur;
ErrorOut:
	return (struct Menu *)NULL;
}
int  MenuBarColors(unsigned long *c,struct MenuBar *Data)
{
	int n;
	if(!c || !Data)return 1;

	for(n=0;n<8;++n)Data->c[n]=c[n];

	for(n=0;n<Data->menuCount;++n){
	    StringColors(c,Data->menuList[n]->menuName);
	}
	return 0;
}
int MenuBarDispose(struct MenuBar *Data)
{
	return 0;
}
int SizeMenuBar(struct MenuBar *Data,int xsize,int ysize)
{
	return 0;
}
int  MenuBarMove(struct MenuBar *Data,int x,int y)
{
	return 0;
}
int  MenuBarUpdate(struct MenuBar *Data)
{
	return 0;
}
int  MenuUpdate(struct Menu *menuList)
{
	if(!menuList)return 1;
	if(menuList->menuName)StringUpdate(menuList->menuName);
	return 0;
}
int DisposePop(struct popMenu *pop)
 {
 	return 0;
}
int DrawPopPt(struct popMenu *pop)
{
	return 0;
}
static int DrawPopItem(struct popMenu *pop,long int n,int flag)
{
	return 0;
}
static int DrawPalette(uRect *controlRect,HWND controlwindow,GC gc,XColor *colors,struct popMenu *pop)
{
	return 0;
}
unsigned long ColorsLong(XColor *colors)
{
	return 0;
}
int DrawPop(struct popMenu *pop)
{
	return 0;
}
int DrawTriangle3D(uRect *controlRect,HWND controlwindow,GC gc,int controlState,
                  int controlSide,unsigned long *c)
{
	return 0;

}
struct popMenu *BuildPop(struct Menu *menu,struct MenuBar *Data)
{
	return 0;
}
int MenuClickList(uPoint *pt,struct MenuBar *Data,int *Menu,int *item)
{
	return 1;
}
int Pointer(uPoint *pt,HWND window)
{
		    return 0;
}
int MenuClick(struct Menu *menuList)
{
	if(!menuList)return 1;
	printf("MenuClick in %s\n",menuList->menuName->stringName);
	return 0;
}
struct Menu *MenuNew(char *name,uRect *menuRect,HWND theWindow)
{
  return NULL;
}
int  MenuBarNext(char *name,uRect *menuRect,struct MenuBar *Data)
{
	return 0;
}
int  MenuAdd(char *name,int flag,struct Menu *menuList)
{
	char **menuNames;
//	XColor **colors;
	int *menuFlags;
	long menuCount,n;
	
	if(!menuList || !name)return 1;
	menuCount=menuList->menuCount+1;
	
	menuNames=(char **)cMalloc(menuCount*sizeof(char *),8739);
	if(!menuNames)return 1;
	zerol((char *)menuNames,menuCount*sizeof(char *));
	
	menuFlags=(int *)cMalloc(menuCount*sizeof(int),8740);
	if(!menuFlags){
		if(menuNames)cFree((char *)menuNames);
		return 1;
	}
	zerol((char *)menuFlags,menuCount*sizeof(int));

/*	colors=(XColor **)cMalloc(menuCount*sizeof(XColor *),8741);
	if(!colors){
	    if(menuNames)cFree((char *)menuNames);
	    if(menuFlags)cFree((char *)menuFlags);
	    return 1;
	}
	zerol((char *)colors,menuCount*sizeof(XColor *));
*/
	for(n=0;n<menuCount-1;++n){
	    if(menuList->menuNames){
	        menuNames[n]=menuList->menuNames[n];
	    }
	    if(menuList->menuFlags){
	        menuFlags[n]=menuList->menuFlags[n];
	    }
/*	    if(menuList->colors){
	        colors[n]=menuList->colors[n];
	    }*/
	}
	menuNames[menuCount-1] = strsave(name,8832);
	menuFlags[menuCount-1] = flag;
	if(menuList->menuNames)cFree((char *)menuList->menuNames);
	if(menuList->menuFlags)cFree((char *)menuList->menuFlags);
//	if(menuList->colors)cFree((char *)menuList->colors);
	menuList->menuNames=menuNames;
	menuList->menuFlags=menuFlags;
//	menuList->colors=colors;
	menuList->menuCount=menuCount;

	return 0;
}
int MenuDispose(struct Menu *menuList)
{
	return 0;
}
int  MenuBarAdd(struct Menu *menuListin,struct MenuBar *Data)
{
	return 0;
}
int  MenuSize(struct Menu *menuList)
{
	return 0;
}
int  MenuSetMark(int flag,int item,struct Menu *menu)
{
	if(!menu)return 1;
	if((item < 0) || (item >= menu->menuCount))return 1;
	if(!menu->menuFlags)return 1;
	menu->menuFlags[item]= (flag & 0xff00) | (menu->menuFlags[item] & 0xff);
	return 0;
}
int  MenuSetPalette(char *pal,int item,struct Menu *menu)
{
	return 0;
}
int  MenuSetTitle(char *name,struct Menu *menu)
{

	if(!menu || !name || !menu->menuName)return 1;
	mstrncpy(menu->menuName->stringName,name,255);
	menu->menuName->stringRect.xsize=menu->menuRect.xsize;
	return 0;
}
int  MenuSetName(char *name,int item,struct Menu *menu)
{

	if(!menu || !name)return 1;
	if((item < 0) || (item >= menu->menuCount))return 1;
	if(menu->menuNames){
	    if(menu->menuNames[item])cFree((char *)menu->menuNames[item]);
	    menu->menuNames[item]=strsave(name,8833);
	}else{
	    return 1;
	}
	return 0;
}
long MenuCount(struct Menu *menu)
{
	if(!menu)return 0;
	return menu->menuCount;
}
int MenuGetName(int item,char *name,struct Menu *menu)
{
	if(!menu || !name)return 1;
	if((item < 0) || (item >= menu->menuCount))return 1;
	if(!menu->menuNames || !menu->menuNames[item])return 1;
	mstrncpy(name,menu->menuNames[item],255);
	return 0;
}

int MenuGetPalette(int item,/*XColor *colors,*/struct Menu *menu)
{
	return 0;
}
int MenuDelete(int item,struct Menu *menu)
{
	return 0;
}

