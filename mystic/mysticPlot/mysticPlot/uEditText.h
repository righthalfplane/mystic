#ifndef _EDITTEXT_
#define _EDITTEXT_
#include "uAttributes.h"

typedef struct editTextStruct{
	unsigned long CursorTime;
	long CursorFlag;
	uPoint Cursor;
	uRect box;
	char text[2048];
	int insertion;
	int length;
	IconPtr myIcon;
	uFontInfo Info;
	struct uFontStuff font;
} *uEditPtr;


uEditPtr uEditNew(uRect *box,IconPtr myIcon);
int uEditClick(uPoint *where,int extend,uEditPtr Edit);
int uEditInsert(char *save,int lenght,uEditPtr Edit);
int uEditKey(int c,uEditPtr Edit);
int uEditDraw(uEditPtr Edit);
int uEditDelete(uEditPtr Edit);
int uEditDrawCursor(uEditPtr Edit);
#endif
