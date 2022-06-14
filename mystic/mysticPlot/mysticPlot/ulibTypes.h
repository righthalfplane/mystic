#ifndef _ULIBTYPES_
#define _ULIBTYPES_


typedef  struct Menu *menuPtr;

typedef long long INT8_64;

typedef struct uPoint {
   int x;
   int y;
} uPoint;

typedef struct uRectStruct {
   int x;
   int y;
   int xsize;
   int ysize;
} uRect;

struct uMenuItems{
    char *ItemName;
    int ItemNumber;
    int ItemType;
    int ItemID;
};

struct uMenuList{
    struct uMenuItems *MenuItems;
};

typedef struct uPortInfo{
	int save;
} uPort;

#define FILE_NAME_SEPERATOR			'/'

int  HiliteMenu(int value);

#define HiWord(n)    		( (n >> 16) & 0xffff)
#define LoWord(n)    		( n & 0xffff)

#endif

