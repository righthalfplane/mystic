#ifndef _ULIBTYPES_
#define _ULIBTYPES_

struct uRectStruct {
   int x;
   int y;
   int xsize;
   int ysize;
};
typedef struct uRectStruct uRect;

typedef struct xPoint {
   int x;
   int y;
} xPoint;

struct uPointStruct {
   int x;
   int y;
};
typedef struct uPointStruct uPoint;

struct uMenuItems{
    char *ItemName;
    int ItemNumber;
    int ItemType;
    int ItemID;
    HMENU nMenu;
};

struct uMenuList{
    struct uMenuItems *MenuItems;
};


extern int  HiliteMenu(int value);

#endif