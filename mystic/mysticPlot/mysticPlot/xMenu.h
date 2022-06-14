#ifndef _XMENU_
#define _XMENU_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "xAux.h"

#include "Control.h"

#include "xStrings.h"


struct MenuItems{
    char *menuNames;
    XColor *colors;
    int menuFlags;
    int disableFlag;
};


struct Menu{
    struct stringData *menuName;
    uRect menuRect;
    struct MenuItems *Items;
    long menuCount;
};

struct MenuBar{
    uRect menuRect;
    Window myWindow;
    Window menuWindow;
    int MenuBarSide;
    struct Menu **menuList;
    long menuCount;
    unsigned long c[10];
    GC gc;
    XFontStruct *info;
    int hide;
};

struct MenuItemsInsert{
    char *ItemName;
    int ItemType;
};

struct Menu *MenuBarAddMenus(char *MenuName,
                             struct MenuItemsInsert *Items,
                             struct MenuBar *MenuBarList);

struct MenuBar *MenuBarNew(uRect *menuRect,Window theWindow);

struct Menu *MenuNew(char *name,uRect *menuRect,Window theWindow);

int  MenuAdd(char *name,int flag,struct Menu *menuList);

int  MenuBarMove(struct MenuBar *Data,int x,int y);

int  MenuBarUpdate(struct MenuBar *Data);

int SizeMenuBar(struct MenuBar *Data,int xsize,int ysize);

int  MenuBarNext(char *name,uRect *menuRect,struct MenuBar *Data);

int  MenuBarColors(unsigned long *c,struct MenuBar *Data);

int  MenuBarAdd(struct Menu *menuList,struct MenuBar *Data);

int MenuClickList(uPoint *pt,struct MenuBar *Data,int *Menu,int *item);

int MenuClick(struct Menu *menuList);

int MenuBarDispose(struct MenuBar *Data);

int MenuDispose(struct Menu *menuList);

long MenuCount(struct Menu *menuList);

int MenuGetName(int n,char *name,struct Menu *menu);

int MenuBarGetName(int n,char *name,struct MenuBar *Data);

int MenuGetPalette(int n,XColor *colors,struct Menu *menu);

int MenuDelete(int n,struct Menu *menu);

int  MenuSetMark(int flag,int item,struct Menu *menuList);

int  MenuSetPopUpTitle(int item,struct MenuBar *MenuBarList);

int  MenuSetPalette(char *palette,int item,struct Menu *menuList);

int  MenuDisable(struct Menu *menuList,int item);

int  MenuEnable(struct Menu *menuList,int item);

int  MenuSetName(char *name,int item,struct Menu *menuList);

int  MenuSetTitle(char *name,struct Menu *menuList);

int  MenuInsert(char *name,int flag,struct Menu *menuList,int location);

#define MENU_SIMPLE		0
#define MENU_PALETTE_NAMED	1
#define MENU_NO_MARK		0x0000
#define MENU_RADIO_MARK		0x0100
#define MENU_CHECK_MARK		0x0200

#endif
