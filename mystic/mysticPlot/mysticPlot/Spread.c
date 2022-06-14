#define EXTERN22 extern
#include "Xappl.h"
#include "uLib.h"
#include "GWorld.h"
#include "Tools.h"
#include "Spread.h"
#include "uGraph.h"
#include "uLib.h"

char *strsave(char *s,int tag);
int ZeroScrap(void);
long putStringToScrap(char *string);



int IsShift();
int WarningBatch(char *message);

static int tdothumb(controlPtr control,IconPtr myIcon);
static int tdoPageUp(controlPtr control,short part,IconPtr myIcon);
static int tdoPageDown(controlPtr control,short part,IconPtr myIcon);
static int tgoUp(controlPtr control,short part,IconPtr myIcon);
static int tgoDown(controlPtr control,short part,IconPtr myIcon);

static void SetEdit2(struct SpreadStruct *wp,int flag);
static void DoClick(struct SpreadStruct *wp,Point p,int extend);

static void InvertFloat(struct SpreadStruct *wp,int x,int y);
static void InvertFloat(struct SpreadStruct *wp,int x,int y);
static void pttocell(struct SpreadStruct *wp,Point *thept);
static void invcol(struct SpreadStruct *wp,int coln);
static void invrow(struct SpreadStruct *wp,int rown);
static void DrawCell(struct SpreadStruct *wp,int x,int y);
static void DumpText(struct SpreadStruct *wpp,int dh,int dv);

static void tMoveY(struct SpreadStruct *TWindow,controlPtr control);
static void tMoveX(struct SpreadStruct *TWindow,controlPtr control);

static void ListGet2(char *out,int x,int y,struct SpreadStruct *wp,int clearFlag);
static void ListColumns(char *out,int i,struct SpreadStruct *wp);
static void ListRows(char *out,int i,struct SpreadStruct *wp);
static void ListSet2(char *out,int x,int y,struct SpreadStruct *wp);


static Rect RectConvert(uRect *sd);


static uRect uRectConvert(Rect *sd);

int StringWidth(unsigned char *,struct Icon *myIcon);

static int ScrollRect(Rect *etr, int dx, int dy,struct Icon *myIcon)
{
	uRect r;
	
	if(!etr || !myIcon)return 1;
	//dx=dx;
	//dy=dy;
	r=uRectConvert(etr);
	uInvalWindowRect(myIcon,&r);
	return 0;
}
static int SetRect(Rect *r,int left,int top,int right,int bottom)
{
	if(!r)return 1;
	r->left=left;
	r->top=top;
	r->right=right;
	r->bottom=bottom;
	return 0;	
}


static uRect uRectConvert(Rect *sd)
{
	uRect r;
	
	if(!sd){
    	r.x=0;
    	r.y=0;
    	r.xsize=0;
    	r.ysize=0;
	}else{
    	r.x=sd->left;
    	r.y=sd->top;
    	r.xsize=sd->right-sd->left;
    	r.ysize=sd->bottom-sd->top;
	}
	return r;
}

static Rect RectConvert(uRect *sd)
{
	Rect r;
	
	if(!sd){
    	r.top=0;
    	r.left=0;
    	r.bottom=0;
    	r.right=0;
	}else{
    	r.left=sd->x;
    	r.top=sd->y;
    	r.bottom=sd->y+sd->ysize;
    	r.right=sd->x+sd->xsize;
	}
	return r;
}

struct SpreadStruct *SpreadNew(uRect *r,int xcount,int ycount,int LineHeight,int LineWidth,
		                       struct Icon *myIcon)
{
	struct SpreadStruct *Spread;
 	controlPtr control;
 	uRect controlRect;
	Rect rr;
	int ret;
	
	if(!r || !myIcon)return NULL;
	
	ret=1;
	
	Spread=(struct SpreadStruct *)cMalloc(sizeof(struct SpreadStruct),5132);
	if(!Spread)goto ErrorOut;
	zerol((char *)Spread,sizeof(struct SpreadStruct));

	Spread->myIcon=myIcon;	
	Spread->Format1=(char *)"%s";
	Spread->Format2=(char *)"%s";
	Spread->Format3=(char *)"%s%c";
	Spread->BreakFlag=TRUE;
	Spread->pListColumns=ListColumns;
	Spread->pListRows=ListRows;
	Spread->pListGet=ListGet2;
	Spread->pListSet=ListSet2;	
	Spread->ListSubType=3;
	Spread->RowOffSet=80;

	SetRect(&rr,0,104,25,130);	
	controlRect=uRectConvert(&rr);
	
	control=ControlCreate(myIcon,&controlRect,"controlName",
		  controlTypeVScroll,1,0L,
		  0L,0L,10,0L);
	if(!control)goto ErrorOut;
	Spread->VControl=control;
	uSetControlMaximum(control,ycount);
	
	SetRect(&rr,0,104,25,130);	
	controlRect=uRectConvert(&rr);	
	control=ControlCreate(myIcon,&controlRect,"controlName",
		  controlTypeHScroll,1,0L,
		  0L,0L,20,0L);
	if(!control)goto ErrorOut;
	Spread->HControl=control;
	uSetControlMaximum(control,xcount);
	
	if(SpreadConfigure(Spread,xcount,ycount,LineHeight,LineWidth,SpreadTypeFloat))goto ErrorOut;
		
	SpreadFitRect(Spread,r);
	
	ret=0;
ErrorOut:	
	if(ret){
		SpreadDispose(Spread);
		Spread=NULL;		
	}
	return Spread;
	
}
static struct SpreadStruct *wp;
static	int oXOff,oYOff;
static int old;
int SpreadInContent(struct SpreadStruct *Spread)
{
	struct Icon myIcon;
	controlPtr control;
	Point where;
	uPoint w;
	uRect r;
	
	if(!Spread)return 1;
	if(!Spread->myIcon)return 1;

	uPointer(&w,Spread->myIcon);
	where.h=w.x;
	where.v=w.y;

	r=uRectConvert(&Spread->TextRect);
	if(uPtInRect(w,&r)){
	    DoClick(Spread,where,IsShift());
		InvalRectMyWindow(Spread->myIcon);
	} else {
		zerol((char *)&myIcon,sizeof(struct Icon));
		myIcon.myevent=Spread->myIcon->myevent;
		myIcon.myWindow=Spread->myIcon->myWindow;
		myIcon.pThumb=tdothumb;
		myIcon.pLineDown=tgoDown;
		myIcon.pLineUp=tgoUp;
		myIcon.pPageDown=tdoPageDown;
		myIcon.pPageUp=tdoPageUp;
		wp=Spread;
		oXOff=wp->XOff;
		oYOff=wp->YOff;
		
		if(!(FindControl(myIcon.myevent,myIcon.myWindow,&control))){
			old=(int)uGetControlValue(Spread->HControl);
		}else{
			old=(int)uGetControlValue(control);
		}

	    uDoControls(&myIcon);
	}
	return 0;
}
static int TMoveDy(controlPtr control,int dy)
{	
	
	
	if(!wp)return 1;
	if(!dy)return 1;
	if(control == wp->VControl){
	    wp->YOff += dy;
	    if(wp->YOff < 0)wp->YOff = 0;
	    if(wp->YOff >= wp->YCount)wp->YOff = wp->YCount-1;
	} else if(control == wp->HControl){
	    wp->XOff += dy;
	    if(wp->XOff < 0)wp->XOff = 0;
	    if(wp->XOff >= wp->XCount)wp->XOff = wp->XCount-1;
	}
	DumpText(wp,wp->XOff - oXOff,wp->YOff - oYOff);
	oXOff=wp->XOff;
	oYOff=wp->YOff;
	return 0;
}
static int tdothumb(controlPtr control,IconPtr myIcon)
{	
		int newp;
		//myIcon=myIcon;
		newp=(int)uGetControlValue(control);
		TMoveDy(control,newp-old);
		old=(int)uGetControlValue(control);
		return 0;
		
}
static int tdoPageUp(controlPtr control,short part,IconPtr myIcon)
{
		int dy,newp;

		if(!wp)return 1;
		//part=part;
		//myIcon=myIcon;
		dy=0;
		if(control == wp->VControl){
		    dy=wp->YScreen-1;
		} else if(control == wp->HControl){
		    dy=wp->XScreen-1;
		}
		newp=(int)uGetControlValue(control);
		uSetControlValue(control,newp-dy);
		return TMoveDy(control,-dy);
}
static int tdoPageDown(controlPtr control,short part,IconPtr myIcon)
{
		int dy,newp;

		if(!wp)return 1;
		//part=part;
		//myIcon=myIcon;
		
		dy=0;
		if(control == wp->VControl){
		    dy=wp->YScreen-1;
		} else if(control == wp->HControl){
		    dy=wp->XScreen-1;
		}
		newp=(int)uGetControlValue(control);
		uSetControlValue(control,newp+dy);
		return TMoveDy(control,dy);
}
static int tgoUp(controlPtr control,short part,IconPtr myIcon)
{	
		int old;
		
		//part=part;	
		//myIcon=myIcon;
		old=(int)uGetControlValue(control);
		uSetControlValue(control,old-1);
		return TMoveDy(control,-1);
}
static int tgoDown(controlPtr control,short part,IconPtr myIcon)
{			
		int old;
		
		//part=part;
		//myIcon=myIcon;
		old=(int)uGetControlValue(control);
		uSetControlValue(control,old+1);
		return TMoveDy(control,1);
}
static void DoClick(struct SpreadStruct *wp,Point p,int extend)
{
	Point *where,Old;
	int i,j;
	Point tpt;
	Rect tr;
	
	if(!wp)return;
	
	/* Build Hashing  */
	
	where=&p;
	pttocell(wp,where);
	if(!extend || wp->Select.top < 0){
	    for (i= wp->Select.top; i<=wp->Select.bottom; i++)		/* remove visible old selection */
		for (j=wp->Select.left; j<= wp->Select.right; j++)
					InvertFloat(wp,j,i);
	    wp->Select.top = wp->Select.bottom = where->v;
	    wp->Select.left = wp->Select.right = where->h;
	    wp->Anchor = *where;
	}
	Old=wp->EntryLocation;				
	wp->EntryLocation = wp->Anchor;				
	DrawCell(wp,Old.h,Old.v);
	DrawCell(wp,wp->Anchor.h,wp->Anchor.v);
	tpt = wp->Anchor;				/* make a copy of anchor point */
	SetEdit2(wp,1);
	tr=wp->viewRect;
	tr.left += wp->XLength;
	tr.top  += wp->YLength;
	tr.right -= wp->XLength;
	tr.bottom  -= wp->YLength;
		while (uStillDown(wp->myIcon)) {
		    uPoint pt;
		    uRect r;
			uPointer(&pt,wp->myIcon);
			where->h=pt.x;
			where->v=pt.y;
			r=uRectConvert(&tr);
			if (!(uPtInRect(pt,&r))) {			/* Is outside of window, scroll */
				if (where->h > tr.right && wp->XOff < wp->XCount - 1) {
					where->h = tr.right;
					wp->XOff++;
					DumpText(wp, 1, 0);
				}
				if (where->h < tr.left && wp->XOff > 0) {
					where->h = tr.left;
					wp->XOff--;
					DumpText(wp, -1,0);
				}
				if (where->v < tr.top && wp->YOff > 0) {
					where->v = tr.top;
					wp->YOff--;
					DumpText(wp, 0, -1);
				}
				if (where->v > tr.bottom && wp->YOff < wp->YCount - 1) {
					where->v = tr.bottom;
					wp->YOff++;
					DumpText(wp,0, 1);
				}
					
			}	/* fall through to selection extension */
			{
				pttocell(wp,where);				/* make relative to data set */
				
			/* 
			*  determine the direction to extend the selection
			*/
				if (where->v <= tpt.v) {		/* move bottom to anchor, top to point */
					while (wp->Select.bottom > tpt.v)
						invrow(wp,wp->Select.bottom--);	/* make it visible */
					while (wp->Select.top > where->v)
						invrow(wp,--wp->Select.top);
					while (wp->Select.top < where->v)
						invrow(wp,wp->Select.top++);
				}
				else {							/* move top to anchor, bottom to point */
					while (wp->Select.top < tpt.v)
						invrow(wp,wp->Select.top++);
					while (wp->Select.bottom < where->v)
						invrow(wp,++wp->Select.bottom);
					while (wp->Select.bottom > where->v)
						invrow(wp,wp->Select.bottom--);
				}
				
				if (where->h <= tpt.h) {		/* move right to anchor, left to point */
					while (wp->Select.right > tpt.h)
						invcol(wp,wp->Select.right--);		/* make it visible */
					while (wp->Select.left > where->h)
						invcol(wp,--wp->Select.left);
					while (wp->Select.left < where->h)
						invcol(wp,wp->Select.left++);
				}
				else {							/* move left to anchor, right to point */
					while (wp->Select.left < tpt.h)
						invcol(wp,wp->Select.left++);
					while (wp->Select.right < where->h)
						invcol(wp,++wp->Select.right);
					while (wp->Select.right > where->h)
						invcol(wp,wp->Select.right--);
				}
				
			}
		}
		
		if(wp->pSyncTrack){
			Rect r;
			uRect r2;
			r=wp->Select;
			r.bottom += 1;
			r.right += 1;
			r2=uRectConvert(&r);
			(*wp->pSyncTrack)(wp->pSyncTrackData,&r2);
		}
		
}
static void SetEdit2(struct SpreadStruct *wp,int flag)
{
	Point p;
	uRect r;
	/* int n; */
	
	if(!wp)return;
	
	p=wp->EntryLocation;
	/* n=p.h+p.v*wp->XCount; */
	
	wp->out[0]=0;
	
	if(wp->pListGet){
	   (*wp->pListGet)(wp->out,p.h,p.v,wp,0);
	}
	
	if(flag){
		r=uRectConvert(&wp->cellRect);
		uInvalWindowRect(wp->myIcon,&r);
	}

}
static void DrawCell(struct SpreadStruct *wp,int x,int y)
{
	char out[256];
	/* char *f; 
	int nc;
	*/
	int i,j,widex,widey,widestring;
	Rect tr,tf,viewRect;
	struct SpreadStruct *TWindow;
	uRect r;
	
	
	TWindow=wp;
	viewRect=TWindow->viewRect;
	
	if (x < wp->XOff || y < wp->YOff ||
		x >= wp->XOff + wp->XScreen  ||
		y >= wp->YOff + wp->YScreen  ||
		x >= wp->XCount || y >= wp->YCount){
		
	    return;
	}
	/* nc=x+y*TWindow->XCount; */
	
	/* f = TWindow->Flags + nc; */

	j = x - wp->XOff+1;				
	i = y - wp->YOff+1;
	
	widex = wp->XLength;			
	widey = wp->YLength;
	
	tr.top = i*widey+viewRect.top;
	tr.bottom = (i+1)*widey+viewRect.top;
	tr.left = (j-1)*widex+wp->RowOffSet+viewRect.left;
	tr.right = j*widex+wp->RowOffSet+viewRect.left;
	r=uRectConvert(&tr);
	uClipRect(&r,wp->myIcon);
	uEraseRect(&r,wp->myIcon);
	tf=tr;
	++tf.bottom;
	++tf.right;
	r=uRectConvert(&tr);
	uFrameRect(&r,wp->myIcon);
	
#ifdef TEXTDATA
	if(*f & 1){
	    sprintf(out,Format1,*d);
	    uMoveTo(j*widex+2,(i+1)*widey-4);
	    uDrawString((char *)out,wp->myIcon);
	}
#endif

	if(wp->pListGet){
        (*wp->pListGet)(out,x,y,wp,0);
    }
	widestring=StringWidth((unsigned char *)out,wp->myIcon);
	if(widestring < widex){
	    uMoveTo(viewRect.left+wp->RowOffSet+(j-1)*widex+widex/2-widestring/2,viewRect.top+(i+1)*widey-4);
	}else{
	    uMoveTo(viewRect.left+wp->RowOffSet+(j-1)*widex+2,viewRect.top+(i+1)*widey-4);
	}
	uDrawString((char *)out,wp->myIcon);
	if (y >= wp->Select.top  && y <= wp->Select.bottom &&
	    x >= wp->Select.left && x <= wp->Select.right){
		r=uRectConvert(&tr);
	    uInvertRect(&r,wp->myIcon);
	    if(x == wp->EntryLocation.h && y == wp->EntryLocation.v){
	        tr.top += 2;
	        tr.left += 2;
	        tr.bottom -= 2;
	        tr.right -= 2;
			r=uRectConvert(&tr);
	        uInvertRect(&r,wp->myIcon);
	    }
	}
}
static void DumpText(struct SpreadStruct *wpp,int dh,int dv)
{
	uRect portRect;
	struct SpreadStruct wp;
	char out[255];
	int xc,yc,xl,yl,xinc,yinc;
	int x,y;
	Rect etr,viewRect,tr;
	int cx,cy,a,b;
	int widex,widey,widestring;
	int si,sj;
	char *f;
	int skipcnt,i,j,ii,jj;
	uRect r;
	
	if(!wpp)return;

    uGetPortBounds(wpp->myIcon,&portRect);
	viewRect=wpp->viewRect;
	r=uRectConvert(&wpp->TextRect);
	uClipRect(&r,wpp->myIcon);

	
	wp = *wpp;
	
	/* Build Hashing  */
	
	
	xc=wp.XScreen;
	yc=wp.YScreen;
	
	xinc=wp.XLength;
	yinc=wp.YLength;
	
	xl=(xc+1)*xinc;
	yl=(yc+1)*yinc;
	
	
	/*	Labels		*/
	
	cx = wp.XScreen;				
	cy = wp.YScreen-1;
	
	a = b = 0;

	if (wp.YOff + cy > wp.YCount) {	
		cy = wp.YCount - wp.YOff;
		a = 0;
	}
	if (wp.XOff + cx > wp.XCount) {	
		cx = wp.XCount - wp.XOff;
		b = 0;
	}
		
	widex = wp.XLength;			
	widey = wp.YLength;
	
	/* TextFace(bold); */
	if (!dv) {
		SetRect(&etr, viewRect.left, viewRect.top, viewRect.right, viewRect.top+widey );
		r=uRectConvert(&etr);
		uEraseRect(&r,wp.myIcon);
		for (i=0; i<cx; i++) {
		    if(wp.pListColumns){
				(*wp.pListColumns)((char *)out,i+wp.XOff,&wp);
			}else{
			   out[0]=0;
			}
    		x=viewRect.left+(i+1)*xinc-StringWidth((unsigned char *)out,wp.myIcon)-2+wp.RowOffSet;
    		y=viewRect.top+yinc-2;
    		uMoveTo(x,y);
   		    uDrawString((char *)out,wp.myIcon);
		}
	}
	
	if (!dh) {
		SetRect(&etr, viewRect.left, viewRect.top, viewRect.left+wp.RowOffSet, viewRect.bottom );
		r=uRectConvert(&etr);
		uEraseRect(&r,wp.myIcon);
		for (i=0; i<cy; i++) {
		    if(wp.pListRows){
			    (*wp.pListRows)((char *)out,i+wp.YOff,&wp);
			}else{
			   out[0]=0;
			}
    		x=viewRect.left+wp.RowOffSet-StringWidth((unsigned char *)out,wp.myIcon)-2;
    		y=viewRect.top+(i+2)*yinc-2;
    		uMoveTo(x,y);
   		    uDrawString((char *)out,wp.myIcon);
		}
	}

	/* TextFace(0); */
	si = sj = 0;
	
	if (dh > cx || -dh > cx || dv > cy || -dv > cy)
		dh = dv = 0;
	
	if (dh || dv) {						

	    SetRect(&etr, viewRect.left+wp.RowOffSet, viewRect.top+widey, viewRect.left+wp.RowOffSet+widex*wp.XScreen, viewRect.top+widey+widey*wp.YScreen );
		
		if (dh < 0) 
			cx = sj - dh;
		if (dh > 0) 
			sj = cx - dh;				
		if (dv < 0) 
			cy = si - dv;
		if (dv > 0) 
			si = cy - dv;
		
	     ScrollRect(&etr, -dh*widex, -dv*widey,wp.myIcon);
	    
	/*	inscrfloat(tw);		*/						
		
		if ((dv > 0 && a) || (dh > 0 && b))goto OutOfHere;

	}
	else {	
	    etr=viewRect;
	    etr.top += widey+1;
	    etr.left += wp.RowOffSet+1;	
		r=uRectConvert(&etr);
		uEraseRect(&r,wp.myIcon);
	}
	
	

	f = wp.Flags + (wp.YOff + si)*wp.XCount + 
		wp.XOff + sj;								
	skipcnt = wp.XCount - cx + sj;					
	
	for (i=si; i<cy; i++) {
		for (j=sj; j<cx; j++) {
			y = wp.YOff + i;
			x = wp.XOff + j;
			jj = x - wp.XOff+1;				
			ii = y - wp.YOff+1;
		    	tr.top = ii*widey+viewRect.top;
			tr.bottom = (ii+1)*widey+viewRect.top;
			tr.left = (jj-1)*widex+wp.RowOffSet+viewRect.left;
			tr.right = jj*widex+wp.RowOffSet+viewRect.left;
			r=uRectConvert(&tr);
			uClipRect(&r,wpp->myIcon);
			uEraseRect(&r,wp.myIcon);
			if(wp.pListGet){
			    (*wp.pListGet)((char *)out,x,y,&wp,0);
			}else{
			    out[0]=0;
			}
			widestring=StringWidth((unsigned char *)out,wp.myIcon);
			if(widestring < widex){
			  uMoveTo(viewRect.left+wp.RowOffSet+j*widex+widex/2-widestring/2,viewRect.top+(i+2)*widey-4);
			}else{
			  uMoveTo(viewRect.left+wp.RowOffSet+j*widex+2,viewRect.top+(i+2)*widey-4);
			}
   		    uDrawString((char *)out,wp.myIcon);

			if (y >= wp.Select.top  && y <= wp.Select.bottom &&
			    x >= wp.Select.left && x <= wp.Select.right){
			     InvertFloat(&wp,x,y);
			     *f++ = 1;
			}else{
			     *f++ = 0;
			}

		}
		
		f += skipcnt;
	}
OutOfHere:
	uClipRect(&portRect,wpp->myIcon);
	
	for(x=0;x<=xc+1;++x){
	    uMoveTo(viewRect.left+wp.RowOffSet+x*xinc,viewRect.top);
	    uLineTo(viewRect.left+wp.RowOffSet+x*xinc,viewRect.top+yl,wp.myIcon);
	}
	
	for(y=0;y<=yc+1;++y){
	    uMoveTo(viewRect.left,viewRect.top+y*yinc);
	    uLineTo(viewRect.left+xl,viewRect.top+y*yinc,wp.myIcon);
	}
	uSetControlValue(wpp->HControl,wp.XOff);
	uSetControlValue(wpp->VControl,wp.YOff);
	/* DrawCell(&wp,wp.EntryLocation.h,wp.EntryLocation.v); */
}
int SpreadUpdate(struct SpreadStruct *Spread)
{
	uRect r;
	
	if(!Spread)return 1;
	if(Spread->isHidden)return 1;
	r=uRectConvert(&Spread->viewRect);
	uEraseRect(&r,Spread->myIcon);
	uFrameRect(&r,Spread->myIcon);	
    /* TextFont(kFontIDMonaco); */
    /* TextSize(9); */
	DumpText(Spread,0,0);
	
	return 0;
}
int SpreadSetSyncTrack(struct SpreadStruct *Spread,int (*pSyncTrack)(void *data,uRect *r),void *data)
{
	if(!Spread || !pSyncTrack)return 1;
	Spread->pSyncTrack=pSyncTrack;
	Spread->pSyncTrackData=data;
	return 0;
}
int SpreadSetRegion(struct SpreadStruct *Spread,uRect *rr)
{
    int s;
	Rect r;
	
	if(!Spread || !rr)return 1;
	
	r=RectConvert(rr);
	
	if(r.bottom < r.top){
		s=r.bottom;
	    r.bottom=r.top;
		r.top=s;
	}

	if(r.right < r.left){
		s=r.right;
	    r.right=r.left;
		r.left=s;
	}

	r.bottom -= 1;
	r.right -= 1;

	if(r.top < 0)r.top = 0;
	if(r.bottom < r.top)r.bottom=r.top;
	if(r.bottom >= Spread->YCount)r.bottom=Spread->YCount-1;
	if(r.top > r.bottom)r.top = r.bottom;


	if(r.left < 0)r.left = 0;
	if(r.right < r.left)r.right=r.left;
	if(r.right >= Spread->XCount)r.right=Spread->XCount-1;
	if(r.left > r.right)r.left = r.right;

	if((r.top == Spread->syncRect.top) &&
	   (r.right == Spread->syncRect.right) &&
	   (r.left == Spread->syncRect.left) &&
	   (r.bottom == Spread->syncRect.bottom))return 0;
	Spread->syncRect= r;
	Spread->Select=r;
	Spread->XOff=r.left;
	if(Spread->XOff < 0)Spread->XOff=0;
	Spread->YOff=r.top;
	if(Spread->YOff < 0)Spread->YOff=0;
	
	return 0;
}
int SpreadConfigure(struct SpreadStruct *Spread,int xcount,int ycount,
                    int LineHeight,int LineWidth,int Type)
{
	long last;
	int ret;
	
	if(!Spread)return 1;
	
	ret = 1;
	
	if(Type != SpreadTypeFloat){
	    WarningBatch("SpreadConfigure Type != SpreadTypeFloat\n");
		goto ErrorOut;
	}

	Spread->XCount=xcount;
	Spread->YCount=ycount;
	Spread->XLength=LineWidth;
	Spread->YLength=LineHeight;
	last=(long)xcount*ycount;
	
	Spread->Length=last; 
	
	if(Spread->Flags){
	    cFree((char *)Spread->Flags);
	    Spread->Flags=NULL;
	    
	}
	
	if(Spread->DataFloat){
	    cFree((char *)Spread->DataFloat);
	    Spread->DataFloat=NULL;
	    
	}
	
	if(!(Spread->Flags=(char *)cMalloc((long)sizeof(char)*last,8164))){
	    WarningBatch("SpreadConfigure Out of Memory\n");
		goto ErrorOut;
	}
	zerol((char *)Spread->Flags,sizeof(char)*last);

	if(!(Spread->DataFloat=(double *)cMalloc(sizeof(double)*last,8165))){
	    WarningBatch("SpreadConfigure Out of Memory\n");
		goto ErrorOut;
	}
	zerol((char *)Spread->DataFloat,sizeof(double)*last);
	
	
	if(Spread->HControl){
	    uSetControlMaximum(Spread->HControl,xcount);
	    uSetControlValue(Spread->HControl,0);
	}
	if(Spread->VControl){
	    uSetControlMaximum(Spread->VControl,ycount);
	    uSetControlValue(Spread->VControl,0);
	}
	SetEdit2(Spread,0);
	ret = 0;
ErrorOut:
	return ret;
	
}
int SpreadFitRect(struct SpreadStruct *Spread,uRect *ri)
{
 	controlPtr control;
	Rect r;
	if(!Spread || !ri)return 1;
	r = RectConvert(ri);
	Spread->viewRect = r;

	Spread->TextRect = r;
	Spread->TextRect.right -= 16;
	Spread->TextRect.bottom -= 16;

	Spread->viewRect = Spread->TextRect;
	
	control=Spread->VControl;
	if(!control)goto ErrorOut;
	
	MoveControl(control,r.right-16,r.top+1);
	SizeControl(control,16,r.bottom-r.top-16);

	tMoveY(Spread,control); 

	control=Spread->HControl;
	if(!control)goto ErrorOut;
	MoveControl(control,r.left+1+Spread->eraseRectSize,r.bottom-16);
	SizeControl(control,r.right-r.left-16-Spread->eraseRectSize,16);
	
	tMoveX(Spread,control);
	
	Spread->XScreen=(ri->xsize-16-Spread->RowOffSet)/Spread->XLength;
	Spread->YScreen=(ri->ysize-16)/Spread->YLength;
			
ErrorOut:

	return 0;
}
static void tMoveX(struct SpreadStruct *TWindow,controlPtr control)
{
	struct SpreadStruct wp;
	int MaxValue,nx;
	Rect view;
	
	if(!TWindow)return;
	
	wp=*(TWindow);
	view=wp.TextView;
	nx=(TWindow->viewRect.right-TWindow->viewRect.left-1)/wp.XLength;
	MaxValue=wp.XCount-1;
	if(MaxValue < 0){
	    MaxValue=0;
	    view.left=0;
	    view.right=nx;
	} else{
	    view.right = view.left + nx;
	}
	TWindow->TextView=view;
	TWindow->viewRect=TWindow->viewRect;
	uSetControlMaximum(control,MaxValue);
}	
static void tMoveY(struct SpreadStruct *TWindow,controlPtr control)
{
	struct SpreadStruct wp;
	int MaxValue,nx;
	Rect view;
	
	if(!TWindow)return;
	
	wp=*(TWindow);
	view=wp.TextView;
	nx=(TWindow->viewRect.bottom-TWindow->viewRect.top-1)/wp.YLength;
	MaxValue=wp.YCount-1;
	if(MaxValue < 0){
	    MaxValue=0;
	    view.top=0;
	    view.bottom=nx;
	} else{
	    view.bottom = view.top + nx;
	}
	TWindow->TextView=view;
	TWindow->viewRect=TWindow->viewRect;
	uSetControlMaximum(control,MaxValue);
}	
static void ListSet2(char *out,int x,int y,struct SpreadStruct *wp)
{
	if((x >= wp->XCount) || (x < 0)){
	    return;
	}
	if((y >= wp->YCount) || (y < 0)){
	    return;
	}
	if(wp->DataFloat){
	    wp->DataFloat[x+y*wp->XCount]=atof(out);
	}
}
static void ListRows(char *out,int i,struct SpreadStruct *wp)
{
    //wp=wp;
	sprintf(out,"R%d",i);
}
static void ListColumns(char *out,int i,struct SpreadStruct *wp)
{
    //wp=wp;
	sprintf(out,"C%d",i);
}
static void ListGet2(char *out,int x,int y,struct SpreadStruct *wp,int clearFlag)
{
	if((x >= wp->XCount) || (x < 0)){
	    return;
	}
	if((y >= wp->YCount) || (y < 0)){
	    return;
	}
	
	if(wp->DataFloat){
	    sprintf(out,"%g",wp->DataFloat[x+y*wp->XCount]);
	  	if(clearFlag)wp->DataFloat[x+y*wp->XCount]=0;
	}

}
static void invrow(struct SpreadStruct *wp,int rown)
	{
	int i;
	
	for (i=wp->Select.left ; i <= wp->Select.right; i++)
		InvertFloat(wp, i, rown);
}

static void invcol(struct SpreadStruct *wp,int coln)
	{
	int i;
	
	for (i=wp->Select.top ; i <= wp->Select.bottom; i++)
		InvertFloat(wp, coln, i);
}
static void pttocell(struct SpreadStruct *wp,Point *thept)
{
	
	
	thept->v = (thept->v-wp->YLength-wp->viewRect.top)/wp->YLength + wp->YOff;
	thept->h = (thept->h-wp->viewRect.left-wp->RowOffSet)/wp->XLength + wp->XOff;
	
	if (thept->v >= wp->YCount)
		thept->v = wp->YCount - 1;
	if (thept->h >= wp->XCount)
		thept->h = wp->XCount - 1;
	if (thept->v < 0)
		thept->v = 0;
	if (thept->h < 0)
		thept->h = 0;
	
}
static void InvertFloat(struct SpreadStruct *wp,int x,int y)
{
	int i,j,widex,widey;
	Rect tr;
	uRect Box;
	
	
	
	
	if (x < wp->XOff || y < wp->YOff ||
		x >= wp->XOff + wp->XScreen  ||
		y >= wp->YOff + wp->YScreen  ||
		x >= wp->XCount || y >= wp->YCount)
		return;
	
	j = x - wp->XOff+1;				
	i = y - wp->YOff+1;
	
	widex = wp->XLength;			
	widey = wp->YLength;
	
	tr.top = i*widey+wp->viewRect.top;
	tr.bottom = tr.top+widey;
	tr.left = (j-1)*widex+wp->RowOffSet+wp->viewRect.left;
	tr.right = tr.left+widex;
	Box=uRectConvert(&tr);
	uInvertRect(&Box,wp->myIcon);

}
static int PasteData(struct SpreadStruct *wp)
{
	long n,Length;
 	Rect r;
 	int x,y;
 	char *bsave,*bp,*bps;
 	
	if(!wp)return 1;
	
	r=wp->Select;
		
	bp=bsave=getStringFromScrap();
	if(!bp)return 1;
	Length=(long)strlen(bp);
	bps=bp;

	y=wp->Select.top;
	x=wp->Select.left;
	for(n=0;n<Length;++n,++bp){
	    if(*bp == '\t'){
	        *bp = '\0';	    
	        if(x > r.right)r.right=x;
	        if(wp->pListSet){
	            (*wp->pListSet)(bps,x++,y,wp);
	        }
	        bps= bp + 1;
	    }else if(*bp == '\r'){	    
	        *bp = '\0';	    
	        if(x > r.right)r.right=x;
	        if(y > r.bottom)r.bottom=y;
	        if(wp->pListSet){
	            (*wp->pListSet)(bps,x,y++,wp);
	        }
	        bps= bp + 1;
	 		x=wp->Select.left;
	    }
	}
	wp->Select=r;
	InvalRectMyWindow(wp->myIcon);
	SetEdit2(wp,TRUE);
    cFree((char *)bsave);
	return 0;
}
static int CopyData(struct SpreadStruct *wp,int ClearFlag)
{
 	int x,y,last,i;
 	/* char *f; */
	unsigned char *bp,out[128];
	long count,size;

	if(!wp)return 1;
 	
	count=0;
	size=10000;
	if((bp=(unsigned char *)cMalloc(size,8169)) == NULL){
	     return 1;
	}
	zerol((char *)bp,size);
	for(y=wp->Select.top;y<=wp->Select.bottom;++y){
	    /* f=wp->Flags + y*wp->XCount + wp->Select.left; */
	    for(x=wp->Select.left;x<=wp->Select.right;++x){
	        if(count + 130 >= size){
	            unsigned char *bp2;
	            
				if((bp2=(unsigned char *)cMalloc(size+10000,8167)) == NULL){
				    if(bp)(cFree((char *)bp));
				    WarningBatch("CopyData Out of Memory\n");
	     			return 1;
				}
				zerol((char *)bp2,size+10000);
	    	    uMemcpy((char *)bp2,(char *)bp,size);
	            size=size+10000;
				if(bp)(cFree((char *)bp));
				bp=bp2;
		    }
		if(wp->pListGet){
            (*wp->pListGet)((char *)out,x,y,wp,ClearFlag);
        }
		last=(int)strlen((char *)out);
		for(i=0;i<last;++i)bp[count++]=out[i];
	        if(x < wp->Select.right)bp[count++]='\t';
	    }
	    bp[count++]='\r';
	}
	
	ZeroScrap();
	putStringToScrap((char *)bp);
	cFree((char *)bp);
	
	if(ClearFlag){
		InvalRectMyWindow(wp->myIcon);
	}
	
	return 0;

}
static int ClearText(struct SpreadStruct *wp)
{
 	char out[256];
 	int x,y;
 	/* char *f; */

	if(!wp)return 1;
	
	for(y=wp->Select.top;y<=wp->Select.bottom;++y){
	    /* f=wp->Flags + y*wp->XCount + wp->Select.left; */
	    for(x=wp->Select.left;x<=wp->Select.right;++x){
	        if(wp->pListGet){
        	    (*wp->pListGet)(out,x,y,wp,1);
        	}
	    }
	}
	
	InvalRectMyWindow(wp->myIcon);
	
	return 0;
}
int SpreadCopy(struct SpreadStruct *Spread)
{
	return CopyData(Spread,FALSE);
}
int SpreadPaste(struct SpreadStruct *Spread)
{
	return PasteData(Spread);
}
int SpreadCut(struct SpreadStruct *Spread)
{
	return CopyData(Spread,TRUE);
}
int SpreadClear(struct SpreadStruct *Spread)
{
	if(!Spread)return 1;
	return ClearText(Spread);
}
static void SetCell(struct SpreadStruct *wp,char *buff)
{
	Point p;
	char *f;
	int n,nc;
	uRect r;
	
	if(!wp || !buff)return;
	
	n=(int)strlen(buff);
	
	p=wp->EntryLocation;
	if(p.v >= wp->YCount){
	    p.v=wp->YCount-1;
	}
	if(p.h >= wp->XCount){
	    p.h=wp->XCount-1;
	}
	nc=p.h+p.v*wp->XCount;
	f = wp->Flags + nc;
	if(n){
	    *f=1;
	} else {
	    *f=0;
	}
	
	if(wp->pListSet){
	    (*wp->pListSet)(buff,p.h,p.v,wp);
	}
							
	r=uRectConvert(&wp->cellRect);
	
	uInvalWindowRect(wp->myIcon,&r);

	DrawCell(wp,p.h,p.v);
}
static void NextCell(struct SpreadStruct *wp,int c)
{
	Rect Select;
	Point p,po;
	int FlagV,FlagH;
		
	if(!wp)return;
	Select=wp->Select;
	p=wp->EntryLocation;
	po=p;
	FlagV=(Select.top != Select.bottom);
	FlagH=(Select.left != Select.right);
	if(c != '\t'){
	      if(++p.v >= wp->YCount){
	          p.v=wp->YCount-1;
	      }
	      if(p.v > Select.bottom){
	            if(FlagH || FlagV){
	                p.v=Select.top;
	                if(++p.h > Select.right)p.h=Select.left;
	            } else {
	                ++Select.top;
	                ++Select.bottom;
	            }
	      }
	} else {
	     if(++p.h >= wp->XCount){
	           p.h=wp->XCount-1;
	     }
	     if(p.h > Select.right){
	            if(FlagV || FlagH){
	                p.h=Select.left;
	                if(++p.v > Select.bottom)p.v=Select.top;
	            } else {
	                ++Select.right;
	                ++Select.left;
	            }
	     }
	}
	wp->Select=Select;
	wp->EntryLocation=p;
	SetEdit2(wp,1);
	DrawCell(wp,p.h,p.v);
	DrawCell(wp,po.h,po.v);

	if (p.h < wp->XOff){
	      --p.h;
	      p.v=wp->YOff;
	} else if ( p.v < wp->YOff){
	    --p.v;
	    p.h=wp->XOff;
	} else if (p.h >= wp->XOff + wp->XScreen ){
	    --p.h;
	      p.v=wp->YOff;
	} else if (p.v >= wp->YOff + wp->YScreen ){
	    --p.v;
	    p.h=wp->XOff;
	} else if (p.h >= wp->XCount){
	    p.h=wp->XCount-1;
	} else if (p.v >= wp->YCount){
	    p.v=wp->YCount-1;
	} else {
	    return;
	}
	if(p.h < 0)p.h=0;
	if(p.v < 0)p.v=0;
	if (p.h >= wp->XCount)p.h=wp->XCount-1;
	if (p.v >= wp->YCount)p.v=wp->YCount-1;
	wp->XOff=p.h;
	wp->YOff=p.v;
	DumpText(wp,0,0);
}
int SpreadSetCell(struct SpreadStruct *Spread,char *message,int c)
{
	if(!Spread || !message)return 1;
	
	SetCell(Spread,message);
	NextCell(Spread,c);
	
	return 0;
}
char *SpreadGetCell(struct SpreadStruct *Spread)
{
	int n;
	
	if(!Spread)return NULL;
	
	n=(int)strlen(Spread->out);
	if(n <= 0)return NULL;
	
	return strsave(Spread->out,6352);
	
}

int SpreadShow(struct SpreadStruct *Spread)
{
	if(!Spread)return 1;
	Spread->isHidden = FALSE;
	return 0;
}

int SpreadHide(struct SpreadStruct *Spread)
{
	if(!Spread)return 1;
	Spread->isHidden = TRUE;
	return 0;
}
int SpreadDispose(struct SpreadStruct *Spread)
{
	if(!Spread)return 1;
	
	if(Spread->Flags){
	    cFree((char *)Spread->Flags);
	    Spread->Flags=NULL;
	    
	}
	
	if(Spread->DataFloat){
	    cFree((char *)Spread->DataFloat);
	    Spread->DataFloat=NULL;
	    
	}
	
	if(Spread->VControl)uDisposeControl(Spread->myIcon,Spread->VControl);
	Spread->VControl=NULL;
	if(Spread->HControl)uDisposeControl(Spread->myIcon,Spread->HControl);
	Spread->HControl=NULL;
	
	if(Spread)cFree((char *)Spread);

	return 0;
}
