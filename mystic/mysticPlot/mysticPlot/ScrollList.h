#ifndef __SCROLLLIST__
#define __SCROLLLIST__
#include "Control.h"
struct ListList{
    char LineData[256];		/* Character Data			*/
    long Userdata;
    int Selected;		/* Selection Flag Array			*/
    int Type;			/* Type Flag				*/
};

struct ScrollList{
    struct ListList *ListInfo;	/* Pointer To List Information		*/
    long count; 		/* Total amount of Data 		*/
    long topline;		/* Line at Top of Displayed List 	*/
    long ListLast;		/* Internal Flag 			*/
    int LineCount;		/* Lines to Display			*/
    int LineHeight;		/* Height of Line to Display		*/
    int LineWidth;		/* Width of Line to Display		*/
    int ListError;		/* Internal Error Flag 			*/
   	int ListSide;		/* Side Width 				*/
    int xOffSet;		/*  x OffSet */
    
    uRect ListRect;			/* List View Rect 			*/
    uRect LineRect;			/* Rect size of one line 		*/
    IconPtr myIcon;			/* The Window of the control 		*/
    controlPtr control1;	/* The Scroll Control Handle 		*/
    controlPtr control2;	/* The Scroll Control Handle 		*/
    int listTextFont;		/* list font number			*/
    int listTextSize;		/* list font size			*/
    int listTextFace;		/* list font Face			*/
    unsigned long LastClick;
    int DoubleClick;
    int ListHide;
    long LastControlLocation;
    
    double *DataFloat;
    char *Flags;
    char Format[20];
};

struct ScrollList *ScrollListNew(int top,int left,int LineHeight,int LineWidth,
		    int LineCount,IconPtr myIcon);

int ScrollListDoControl(IconPtr myIcon,uPoint where,struct ScrollList *ListD);

int ScrollListUpdate(struct ScrollList *ListD);

int ScrollListResetDoubleClick(struct ScrollList *ListD);

int ScrollListInsert(char **data,int *type,long location,long count,struct ScrollList *ListD);

int ScrollListInsertOneType(char **data,int type,long location,long count,struct ScrollList *ListD);

int ScrollListUpdate(struct ScrollList *ListD);

int ScrollListDispose(struct ScrollList *ListD);

int ScrollListGet(char *buff,long location,struct ScrollList *ListD);

int ScrollListPut(char *buff,long location,struct ScrollList *ListD);

long ScrollListCount(struct ScrollList *ListD);

long ScrollListFirstSelect(struct ScrollList *ListD);

int ScrollListDelete(long first,long last,struct ScrollList *ListD);

int ScrollListIsSelected(long location,struct ScrollList *ListD);

int ScrollListDeSelect(struct ScrollList *ListD);

int ScrollListIsSelected(long location,struct ScrollList *ListD);

int ScrollListTop(long location,struct ScrollList *ListD);

int ScrollListSelect(long location,struct ScrollList *ListD);

int ScrollListDeleteSelected(struct ScrollList *ListD);

int ScrollListInsertSort(char *data,int type,long SortType,struct ScrollList *ListD);

int ScrollListSort(long SortType,struct ScrollList *ListD);

int ScrollListGetUser(long *ldata,long location,struct ScrollList *ListD);

int ScrollListPutUser(long  ldata,long location,struct ScrollList *ListD);

int ScrollListInsertUser(char **data,int *type,long *ldata,long location,long count,struct ScrollList *ListD);

int ScrollListSetString(char *name,struct ScrollList *ListD);

int ScrollListIsSelectAny(struct ScrollList *ListD);

long ScrollListGetItemCount(char *name,long location,struct ScrollList *ListD);

long ScrollListFindItem(char *name,long ItemCount,struct ScrollList *ListD);

int ScrollListSetItemSelected(long item,struct ScrollList *ListD);

int ScrollListSetTop(long item,struct ScrollList *ListD);

int ScrollListSetDouble(double *DataFloat,long Count,struct ScrollList *ListD);
int ScrollListSetFormat(char *Format,struct ScrollList *ListD);


#define LIST_SORT_ASCII         1
#define LIST_SORT_DOUBLE        2
#define LIST_SORT_LONG          3



int ScrollListGetRect(uRect *cellRect,struct ScrollList *ListD);

int ScrollListFitRect(uRect *cellRect,struct ScrollList *ListD);

int ScrollListgoUp(controlPtr control,short part,struct ScrollList *ListD); 
int ScrollListgoDown(controlPtr control,short part,struct ScrollList *ListD); 
int ScrollListdoPageUp(controlPtr control,short part,struct ScrollList *ListD); 
int ScrollListdoPageDown(controlPtr control,short part,struct ScrollList *ListD); 
int ScrollListdothumb(controlPtr control,short part,struct ScrollList *ListD);

#endif
