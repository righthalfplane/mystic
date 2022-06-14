#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#define EXTERN22 extern
#include "Xappl.h"
#include "xText.h"
#include "Tools.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif



int DrawRect3D(uRect *controlRect,Window controlwindow,
               GC gc,int controlState,int controlSide,unsigned long *c);

int SetTime(struct timeval *curTime,long milliseconds);
int TimeUp(struct timeval *LastTime);

int TextSetInsertion(struct textData *Text);
int IsShift(void);       
int TextAline(struct textData *Text);

int TextAlineCursor(struct textData *Text);

int TextSize(struct textData *Text,int xsize,int ysize)
{
	if(!Text)return 1;
	Text->textRect.xsize=xsize;
	Text->textRect.ysize=ysize;
	return 0;
}
int TextMove(struct textData *Text,int ix,int iy)
{
	if(!Text)return 1;
	Text->textRect.x=ix;
	Text->textRect.y=iy;
	return 0;
}
struct textData *TextNew(uRect *cellRect,Window theWindow)
{
	struct textData *Text;


	Text=(struct textData *)cMalloc(sizeof(struct textData),8745);

	if(Text == NULL){
	    return (struct textData *)NULL;
	}
	
	zerol((char *)Text,(long)sizeof(struct textData));

	Text->textRect = *cellRect;
	Text->textSide =  2;
	Text->gc = XCreateGC(xg.mydisplay,theWindow,0,0);
	Text->gcX = XCreateGC(xg.mydisplay,theWindow,0,0);  
	{
	    XGCValues v;
	    unsigned long c[10];

	    getColors(c);

	    v.foreground=c[4];
	    v.background=c[2];


	    v.foreground = v.foreground ^ v.background ;

	    v.function = GXxor ;  

	     XChangeGC(xg.mydisplay,Text->gcX,  
	               GCForeground | GCBackground | GCFunction, &v);  
	}
	XSetFont(xg.mydisplay,Text->gc,xg.font13); 
	XSetFont(xg.mydisplay,Text->gcX,xg.font13); 
	Text->myWindow=theWindow;
	Text->firstSelected = Text->lastSelected = 0;
	Text->textDelete=127;
	Text->textBackspace=8;

	Text->info=xg.font13info;
	if(!Text->info)goto ErrorOut;

	SetTime(&Text->LastTime,500L);

	return Text;
ErrorOut:
	TextDispose(Text);
	return NULL;

}
int TextGetRect(uRect *cellRect,struct textData *Text)
{
	if(!Text || !cellRect)return 1;
	*cellRect=Text->textRect;
	return 0;
}
int TextDeSelect(struct textData *Text)
{
	if(!Text)return 1;
	Text->lastSelected=(int)strlen(Text->textName);
	Text->firstSelected=Text->lastSelected;
	return 0;
}
int TextSelect(struct textData *Text)
{
	if(!Text)return 1;
	Text->lastSelected=(int)strlen(Text->textName);
	Text->firstSelected=0;
	return 0;
}
int TextDispose(struct textData *Text)
{
	if(!Text)return 1;
	if(Text->gc)XFreeGC(xg.mydisplay,Text->gc);
	Text->gc=NULL;
	if(Text->gcX)XFreeGC(xg.mydisplay,Text->gcX);
	Text->gcX=NULL;
	cFree((char *)Text);
	return 0;
}
int TextIdle(struct textData *Text)
{
	int linestart,xp,yt,yb,end;

	if(!Text)return 1;


	if(!TimeUp(&Text->LastTime))return 0;


	linestart=Text->textRect.x+Text->textSide+2-Text->xOffSet;
	end=Text->textRect.x+Text->textRect.xsize;
	yt=Text->textRect.y+Text->textSide;
	yb=Text->textRect.y+Text->textRect.ysize-Text->textSide;
	if(Text->firstSelected > 0){
	    xp=linestart+XTextWidth(Text->info,Text->textName,Text->firstSelected);
	}else{
	    xp=linestart;
	}

	if(xp < Text->textRect.x || xp > end)return 0;

	if(Text->firstSelected == Text->lastSelected){
	    if(Text->textCursor >= 0){
		Text->textCursor = -1;
	    }else{
	        Text->textCursor = 1;       
	    }
	    MoveTo(xp,yt);
	    LineTo(xp,yb,Text->myWindow,Text->gcX);
	    MoveTo(xp-1,yt);
	    LineTo(xp-1,yb,Text->myWindow,Text->gcX);
	}else{
	   if(Text->textCursor >= 0){
		MoveTo(xp,yt);
		LineTo(xp,yb,Text->myWindow,Text->gcX);
		MoveTo(xp-1,yt);
		LineTo(xp-1,yb,Text->myWindow,Text->gcX);
		Text->textCursor = -1;
	   }
	}
	SetTime(&Text->LastTime,500L);
	return 0;
}
int TextPut(char *text,struct textData *Text)
{
	if(!text || !Text)return 1;
	mstrncpy(Text->textName,text,255);
	Text->firstSelected = Text->lastSelected = (int)strlen(text);
	return 0;
}
int TextGet(char *text,struct textData *Text)
{
	if(!text || !Text)return 1;
	mstrncpy(text,Text->textName,255);
	return 0;
}
int TextUpdate(struct textData *Text)
{
	XFontStruct *info;
	uRect textRect;
	XRectangle r;
	char out[2];
	int linestart,n,nc,xp;
	Pixmap pixmap;
	unsigned long c[10];

	if(!Text)return 1;


	textRect=Text->textRect;

	textRect.y=0;
	textRect.x=0;

	pixmap = XCreatePixmap(xg.mydisplay,RootWindow(xg.mydisplay, xg.myscreen),
	                       textRect.xsize, textRect.ysize,xg.vBits);

	r.x=0;
	r.y=0;
	r.width=textRect.xsize;
	r.height=textRect.ysize;

	XSetClipRectangles(xg.mydisplay,Text->gc,0,0,&r,1,0);

	if(!getColors(c)){
	    XSetBackground (xg.mydisplay, Text->gc, c[2]);
	    XSetForeground (xg.mydisplay, Text->gc, c[2]);
	}

	DrawRect3D(&(textRect),pixmap,
	              Text->gc,2,Text->textSide,c);


	r.x=Text->textSide;
	r.y=Text->textSide;
	r.width=textRect.xsize-2*Text->textSide;
	r.height=textRect.ysize-2*Text->textSide;

	XSetClipRectangles(xg.mydisplay,Text->gc,0,0,&r,1,0);

	XSetForeground (xg.mydisplay,Text->gc,  c[4]);
	XSetBackground (xg.mydisplay,Text->gc,  c[2]);

	nc=(int)strlen(Text->textName);
	linestart=Text->textSide+2-Text->xOffSet;

	info=Text->info;
	if(info){

		out[1]=0;
		for(n=0;n<nc;++n){
	            if(n == 0){
	                xp=linestart;
	            }else{
	                xp=linestart+XTextWidth(info,Text->textName,n);
	            }
	            if(n >= Text->firstSelected && n < Text->lastSelected){ /* selected */
					XSetForeground (xg.mydisplay,Text->gc, c[2]);
					XSetBackground (xg.mydisplay,Text->gc, c[4]);
	            }else{
					XSetForeground (xg.mydisplay,Text->gc, c[4]);
					XSetBackground (xg.mydisplay,Text->gc, c[2]);
	            }
			    MoveTo(xp,textRect.y+textRect.ysize/2+info->ascent/2);
	            out[0]=Text->textName[n];
			    DrawString((unsigned char *)out,pixmap,Text->gc);
		}
	}else{
	    fprintf(stderr,"info NULL\n");
	}

	r.x=Text->textRect.x;
	r.y=Text->textRect.y;
	r.width=textRect.xsize;
	r.height=textRect.ysize;

	XSetClipRectangles(xg.mydisplay,Text->gc,0,0,&r,1,0);

    XCopyArea(xg.mydisplay, pixmap, Text->myWindow,
	Text->gc, 0, 0, textRect.xsize, textRect.ysize,
	Text->textRect.x, Text->textRect.y);
    XFreePixmap(xg.mydisplay, pixmap);

	Text->textCursor = -1;
	return 0;
}
int TextKey(int c,struct textData *Text)
{
	char save[256];
	int n,nc,nn;


	/* printf("TextKey c = %d %c\n",c,c); */

	nc=(int)strlen(Text->textName);
    
    if(c == XK_Left){
        if(--Text->firstSelected < 0)Text->firstSelected = 0;
        Text->lastSelected=Text->firstSelected;
        Text->start = Text->end = 0;
        TextAline(Text);
        TextUpdate(Text);
    }else if(c == XK_Right){
        if(++Text->firstSelected > nc)Text->firstSelected = nc;
        Text->lastSelected=Text->firstSelected;
        Text->start = Text->end = 0;
        TextAline(Text);
        TextUpdate(Text);
    }else if(c == Text->textDelete || c == Text->textBackspace){
	    nn=0;
	    if(Text->firstSelected != Text->lastSelected){
	        for(n=0;n<nc;++n){
	            if(n < Text->firstSelected || n >= Text->lastSelected){
	                Text->textName[nn++]=Text->textName[n];
	            }
	        }
            Text->lastSelected=Text->firstSelected;
	    }else{
	        for(n=0;n<nc;++n){
	            if(n != Text->firstSelected-1){
	                Text->textName[nn++]=Text->textName[n];
	            }
	        }
	        if(--Text->firstSelected < 0)Text->firstSelected = 0;
            Text->lastSelected=Text->firstSelected;
	    }
	    Text->textName[nn++]=0;
	    Text->start = Text->end = 0;
	    TextAline(Text);
	    TextUpdate(Text);
	    return 0;
	}

	if(c < 32 || c > 126)return 0;

	if(Text->firstSelected != Text->lastSelected){
	    nn=0;
	    if(Text->firstSelected < Text->lastSelected){
	        for(n=0;n<nc;++n){
	            if(n >= Text->firstSelected && n < Text->lastSelected){
	                if(n == Text->firstSelected)save[nn++]=c;
	                continue;
	            }
                save[nn++]=Text->textName[n];
	        }
            save[nn++]=0;
            mstrncpy(Text->textName,save,255);
            ++Text->firstSelected;
            Text->lastSelected=Text->firstSelected;
	    }else{
	        nn=nc;
	        Text->textName[nn++]=c;
	        Text->textName[nn++]=0;
            Text->firstSelected = Text->lastSelected = nn;
	    }
    }else{
	    if(Text->firstSelected == nc){
	        nn=nc;
	        Text->textName[nn++]=c;
	        Text->textName[nn++]=0;
            Text->firstSelected=Text->lastSelected=nc+1;
	    }else{
	        nn=0;
	        for(n=0;n<nc;++n){
	            if(Text->firstSelected == n){
                    save[nn++]=c;
	            }
                save[nn++]=Text->textName[n];
	        }
            save[nn++]=0;
            mstrncpy(Text->textName,save,255);
            ++Text->firstSelected;
            Text->lastSelected=Text->firstSelected;
	    }
	}
	Text->start = Text->end = 0;
	TextAline(Text);
	TextUpdate(Text);
	return 0;
}
int TextAline(struct textData *Text)
{
	XFontStruct *info;
	int linestart,xp,nc,end,width;

	if(!Text)return 1;
	info=Text->info;
	if(!info)return 1;
	nc=(int)strlen(Text->textName);
	end=Text->textRect.x+Text->textRect.xsize-Text->textSide-2;
	width=XTextWidth(info,Text->textName,nc);
	Text->xOffSet=0;
	while(1){
	    linestart=Text->textRect.x+Text->textSide+2-Text->xOffSet;
	    xp=linestart+width;
	    if(xp < end)break;
	    ++Text->xOffSet;
	}
	return 0;
}
int TextAlineCursor(struct textData *Text)
{
	XFontStruct *info;
	int linestart,xp,nc,end,width;

	if(!Text)return 1;
	info=Text->info;
	if(!info)return 1;
	nc=(int)strlen(Text->textName);
	width=XTextWidth(info,Text->textName,nc);
	end=Text->textRect.x+Text->textRect.xsize-Text->textSide-2;
	linestart=Text->textRect.x+Text->textSide+2-Text->xOffSet;

	if(Text->end < Text->textRect.x){
	    Text->xOffSet -= Text->textRect.x-Text->end;
	    if(Text->xOffSet <= 0){
	        Text->xOffSet=0;
	        return 0;
	    }

	}else if(Text->end > end){
	    Text->xOffSet += Text->end-end;
	    linestart=Text->textRect.x+Text->textSide+2-Text->xOffSet;
	    xp=linestart+width;
	    if(xp > end)return 0;
	    Text->xOffSet=0;
	    while(1){
	        linestart=Text->textRect.x+Text->textSide+2-Text->xOffSet;
	        xp=linestart+width;
	        if(xp < end)break;
	        ++Text->xOffSet;
	    }
	}
	return 0;
}
int TextClick(XEvent *myevent,uPoint *pt,struct textData *Text)
{
	XEvent event;
	int xold;
	uPoint ps;

	if(!pt || !Text)return 1;
	
	//myevent=myevent;

	Text->DoubleClick=FALSE;

	ps.x=pt->x;
	ps.y=pt->y;
	if(IsShift()){
	   if(ps.x > Text->start){
	       Text->end=ps.x;
	   }else{
	       Text->start=ps.x;
	   }
	}else{
	   Text->start=ps.x;
	   Text->end=ps.x;
	}
	xold=ps.x;
	TextSetInsertion(Text);
	TextUpdate(Text);
	while(1){
		XMaskEvent(xg.mydisplay,ButtonReleaseMask | Button1MotionMask ,&event);
		switch(event.type){
		case EnterNotify:
		break;
		case LeaveNotify:
		break;
		case MotionNotify:		   
		    if(xold != event.xmotion.x){
				Text->end=event.xmotion.x;
				TextAlineCursor(Text);
				if(TextSetInsertion(Text))continue;
				TextUpdate(Text);
		    }		    
		    xold = event.xmotion.x;
		break;
		case ButtonRelease:
		    goto OutOfHere;
		default:
		    printf("TextClick  event.type %d MotionNotify %d\n",event.type,MotionNotify);
		}
	}
OutOfHere:
	if(TickCount() <  Text->LastClick+120){	/* Double Click  */
	    Text->DoubleClick=TRUE;
	    Text->firstSelected=0;
	    Text->lastSelected=(int)strlen(Text->textName);
	    TextUpdate(Text);
	}
	Text->LastClick=TickCount();
	return 0;
}
int TextSetInsertion(struct textData *Text)
{
	XFontStruct *info;
	int linestart,n,first,last,xp;
	int firstSelected,lastSelected;
	int nc;

	if(!Text)return 1;

	firstSelected=Text->firstSelected;
	lastSelected=Text->lastSelected;

	nc=(int)strlen(Text->textName);

	info=Text->info;
	if(info){


	    linestart=Text->textRect.x+Text->textSide+2-Text->xOffSet;


	    first=last=Text->start;
	    if(Text->start < Text->end){
	        first=Text->start;
	        last=Text->end;
	    }else if(Text->end < Text->start){
	        first=Text->end;
	        last=Text->start;
	    }
	    Text->firstSelected = -1;
	    Text->lastSelected  = -1;
	    if(first < last){
	        for(n=0;n<nc;++n){
	            if(n == 0){
	                xp=linestart;
	            }else{
	                xp=linestart+XTextWidth(info,Text->textName,n);
	            }
	            if(xp >= first && xp <= last){ /* selected */
			if(Text->firstSelected < 0)Text->firstSelected=n;
			Text->lastSelected=n;
	            }
	        }
	        ++Text->lastSelected;
	    }else{		
	        for(n=0;n<nc;++n){
	            if(n == 0){
	                xp=linestart;
	            }else{
	                xp=linestart+XTextWidth(info,Text->textName,n);
	            }
	            if(xp >= first){ /* selected */
			Text->firstSelected=Text->lastSelected=n;
			break;
	            }
	        }
	    }
	    if(Text->firstSelected < 0){
		Text->firstSelected=nc;
		Text->lastSelected=nc;
	    }
	}else{
	    Text->firstSelected = nc;
	    Text->lastSelected  = nc;
	}
	if(firstSelected == Text->firstSelected &&
	    lastSelected == Text->lastSelected)return 1;
/*
printf("first %d last %d firstSelected %d lastSelected %d\n",first,last,Text->firstSelected,Text->lastSelected);
*/
    return 0;
	
}
#ifdef OLDTEXTUPDATE
int TextUpdate(struct textData *Text)
{
	XFontStruct *info;
	uRect textRect;
	XRectangle r;
	char out[2];
	int linestart,n,nc,xp;

	if(!Text)return 1;

	textRect=Text->textRect;

	r.x=textRect.x;
	r.y=textRect.y;
	r.width=textRect.xsize;
	r.height=textRect.ysize;

	XSetClipRectangles(xg.mydisplay,Text->gc,0,0,&r,1,0);

	XSetForeground (xg.mydisplay,Text->gc, xg.c[2]);
	XSetBackground (xg.mydisplay,Text->gc, xg.c[2]);

	DrawRect3D(&(Text->textRect),Text->myWindow,
	              Text->gc,2,Text->textSide,NULL);


	r.x=textRect.x+Text->textSide;
	r.y=textRect.y+Text->textSide;
	r.width=textRect.xsize-2*Text->textSide;
	r.height=textRect.ysize-2*Text->textSide;

	XSetClipRectangles(xg.mydisplay,Text->gc,0,0,&r,1,0);

		/*
		XSetForeground (xg.mydisplay,Text->gc, xg.c[2]);
		XSetBackground (xg.mydisplay,Text->gc, xg.c[2]);
		XFillRectangle(xg.mydisplay,Text->myWindow,
	               Text->gc,
	               LineRect.x,LineRect.y,
	               LineRect.xsize,
	               LineRect.ysize);
	         */
		XSetForeground (xg.mydisplay,Text->gc, xg.myforeground);
		XSetBackground (xg.mydisplay,Text->gc, xg.mybackground);
		XSetBackground (xg.mydisplay,Text->gc,  xg.c[2]);

	nc=(int)strlen(Text->textName);
	linestart=Text->textRect.x+Text->textSide+2-Text->xOffSet;

	info=Text->info;
	if(info){

		out[1]=0;
		for(n=0;n<nc;++n){
	            if(n == 0){
	                xp=linestart;
	            }else{
	                xp=linestart+XTextWidth(info,Text->textName,n);
	            }
	            if(n >= Text->firstSelected && n < Text->lastSelected){ /* selected */
			XSetForeground (xg.mydisplay,Text->gc, xg.mybackground);
			XSetBackground (xg.mydisplay,Text->gc, xg.myforeground);
	            }else{
			XSetForeground (xg.mydisplay,Text->gc, xg.myforeground);
			XSetBackground (xg.mydisplay,Text->gc,  xg.c[2]);
	            }
		    MoveTo(xp,textRect.y+textRect.ysize/2+info->ascent/2);
	            out[0]=Text->textName[n];
		    DrawString((unsigned char *)out,Text->myWindow,Text->gc);
		}
	}else{
	    fprintf(stderr,"info NULL\n");
	}


	Text->textCursor = -1;
	return 0;
}
#endif
