#define EXTERN22 extern
#include <ctype.h>
#include "Xappl.h"
#include "Message.h"
#include "uLib.h"
#include "uDialog.h"
#include "uEditText.h"
#include "uGraph.h"

int uCStringWidth(char *name,IconPtr myIcon);

int uEditDrawBox(uEditPtr t);

int uEditSetCursor(uEditPtr Edit);

int uEditStartCursor(uEditPtr t);

int uEditGetInsert(uEditPtr Edit,int *xp,int *yp);

int uEditSetInsert(uEditPtr Edit,int xp,int yp,int itWas);

int uEditSetInsert(uEditPtr Edit,int xp,int yp,int itWas)
{
	int n,nn,y;
	int isSet;
	
	if(!Edit)return 1;
	
	if(yp < 0)return 0;
		
	y=0;
	nn=0;
	isSet=FALSE;
	for(n=0;n<Edit->length;++n){
	    if(y == yp && nn >= xp){
			Edit->insertion=n;
			isSet=TRUE;
			break;
	    }
		if(Edit->text[n] == '\n' || Edit->text[n] == '\r'){		
		    if(y == yp){
				Edit->insertion=n;
				isSet=TRUE;
				break;
		    }
		    y += 1;
		    nn=0;	 
		}else{
		    nn++;	
		} 
	}
			
	if(!isSet && (itWas != y))Edit->insertion=Edit->length;

	return  0;
}

int uEditGetInsert(uEditPtr Edit,int *xp,int *yp)
{
	int n,nn,y;
	
	if(!Edit || !xp || !yp)return 1;
		
	y=0;
	nn=0;
	for(n=0;n<Edit->length;++n){
	    if(n == Edit->insertion){
	        *xp=nn;
	    	break;
	    }
		if(Edit->text[n] == '\n' || Edit->text[n] == '\r'){		
		    y += 1;
		    nn=0;	 
		}else{
		    nn++;	
		} 
	}
	
	if(Edit->length == Edit->insertion){
		*xp=nn;
	}
	
	*yp=y;
	
	return  0;
}

int uEditStartCursor(uEditPtr t)
{
	if(!t)return 1;
	
	t->CursorTime=TickCount()-60;
	
	return 0;
}
int uEditDrawCursor(uEditPtr t)
{
	uRect Box;
	
	if(!t)return 1;
	
	if(TickCount() > (t->CursorTime+50)){
	    if(t->CursorFlag){	    
	        Box.y=t->Cursor.y;
	        Box.ysize=t->Info.LineHeight+1;
	        --Box.y;
	        Box.x=t->Cursor.x;
	    
	        Box.xsize=3;
	        --Box.x;
	        uEraseRect(&Box,t->myIcon);
	    }else{
	        uMoveTo(t->Cursor.x,t->Cursor.y);
	        uLineTo(t->Cursor.x,t->Cursor.y+t->Info.LineHeight-1,t->myIcon);
	    }
	    t->CursorFlag = !t->CursorFlag;
	    t->CursorTime=TickCount();
	}
	
	return 0;
}
int uEditKey(int c,uEditPtr Edit)
{
	int xp,yp;
	int didIt;
	int n;
	int k;
	
	if(!Edit)return 1;
	
	xp=0;
	yp=0;

	if(Edit->CursorFlag){	  
	    uEditStartCursor(Edit);
	    uEditDrawCursor(Edit);
	}
	
	if(c == 127 || c == 8){
		k=0;
		didIt=FALSE;
		for(n=0;n<Edit->length;++n){
		   if(n == Edit->insertion-1){
				didIt=TRUE;
		        continue;
		   }else{
		  	 Edit->text[k++]=Edit->text[n];
		   }
		}
		if(didIt){
			--Edit->length;
			if(Edit->length < 0)Edit->length=0;
			Edit->text[Edit->length]=0;
			--Edit->insertion;
			if(Edit->insertion < 0)Edit->insertion=0;
		}
	} else if(c == rightArrow){
	     if(++Edit->insertion > Edit->length)Edit->insertion=Edit->length;
	} else if(c == leftArrow){
	     if(--Edit->insertion < 0)Edit->insertion=0;
	} else if(c == upArrow){	     
	     uEditGetInsert(Edit,&xp,&yp);
	     yp -= 1;
	     uEditSetInsert(Edit,xp,yp,yp+1);
	} else if(c == downArrow){	     
	     uEditGetInsert(Edit,&xp,&yp);
	     yp += 1;
	     uEditSetInsert(Edit,xp,yp,yp-1);
	}else{
	   if(Edit->insertion == Edit->length){
	   		Edit->text[Edit->length++]=c;
	   		++Edit->insertion;
	   }else{
	       for(n=Edit->length;n>=Edit->insertion;--n){
	           Edit->text[n+1]=Edit->text[n];
	       }
	       Edit->text[Edit->insertion]=c;
	       ++Edit->insertion;
	       ++Edit->length;
	       Edit->text[Edit->length]=0;
	   }
	}
	
	uEditDraw(Edit);

	uEditStartCursor(Edit);
	
	return 0;
}
int uEditDrawBox(uEditPtr Edit)
{
	char out[2048];
	uFontInfo Info;
	int xsize;
	int n,k;
	
	if(!Edit)return 1;

	uGetFontInfo(Edit->myIcon,&Info);
	
	Edit->box.ysize=Info.LineHeight;
		
	Edit->box.xsize=0;	
		
	k=0;
	for(n=0;n<Edit->length;++n){
	   if(Edit->text[n] == '\r'){
	       out[k]=0;
	       xsize=uCStringWidth(out,Edit->myIcon);
	       if(xsize > Edit->box.xsize)Edit->box.xsize=xsize;
		   Edit->box.ysize += Info.LineHeight;
	       k=0;
	   }else{
	       out[k++]=Edit->text[n];
	   }
	}
	
	if(k > 0){
	    out[k]=0;
	    xsize=uCStringWidth(out,Edit->myIcon);
	    if(xsize > Edit->box.xsize)Edit->box.xsize=xsize;
	}
	
	Edit->box.xsize += 4;
	
	return 0;
}
int uEditSetCursor(uEditPtr Edit)
{
	char buff[256];	
	int n,nn,x,y;
	
	if(!Edit)return 1;
		
	x=Edit->box.x;
	y=Edit->box.y;
	nn=0;
	for(n=0;n<Edit->length;++n){
	    if(n == Edit->insertion){
	    	buff[nn]=0;	
	    	x += uCStringWidth(buff,Edit->myIcon);
	    	break;
	    }
		if(Edit->text[n] == '\n' || Edit->text[n] == '\r'){		
		    buff[nn]=0;	
		    y += Edit->Info.LineHeight;
		    nn=0;	 
		}else{
		    buff[nn++]=Edit->text[n];	
		} 
	}
	
	if(Edit->length == Edit->insertion){
    	buff[nn]=0;	
    	x += uCStringWidth(buff,Edit->myIcon);
	}
		
	Edit->Cursor.x=x+4;
	Edit->Cursor.y=y;
	
	return  0;
}

int uEditDraw(uEditPtr Edit)
{
	char buff[256];
	int n,nn,x,y;
	
	if(!Edit)return 1;

	uSetFontStuff(&Edit->font,Edit->myIcon);

	uEraseRect(&Edit->box,Edit->myIcon);
	
	uEditDrawBox(Edit);
	
	x=Edit->box.x;
	y=Edit->box.y;
	nn=0;
	for(n=0;n<Edit->length;++n){
		if(Edit->text[n] == '\n' || Edit->text[n] == '\r'){		
		    buff[nn]=0;	
		    uMoveTo(x+4,y+Edit->Info.LineHeight-4);
		    uDrawString(buff,Edit->myIcon); 
		    y += Edit->Info.LineHeight;
		    nn=0;	 
		}else{
		    buff[nn++]=Edit->text[n];	
		} 
	}
	
	if(nn > 0){
	    buff[nn]=0;	
		uMoveTo(x+4,y+Edit->Info.LineHeight-4);
	    uDrawString(buff,Edit->myIcon); 
	}

	uEditSetCursor(Edit);

	return 0;
}

int uEditClick(uPoint *where,int extend,uEditPtr Edit)
{
	char out[2048];
	int xsize,ysize,xold,dx;
	int x,y;
	int n,ns;
	
	if(!where || !Edit)return 1;
	
	if(where->x < Edit->box.x || where->x > Edit->box.x+Edit->box.xsize)return 1;
	if(where->y < Edit->box.y || where->y > Edit->box.y+Edit->box.ysize)return 1;

	uSetFontStuff(&Edit->font,Edit->myIcon);

	if(Edit->CursorFlag){	  
	    uEditStartCursor(Edit);
	    uEditDrawCursor(Edit);
	}
		
	x=where->x-Edit->box.x;
	y=where->y-Edit->box.y;
	
	ysize=Edit->Info.LineHeight;
	y=y/ysize;
	xold=0;
    for(n=0;n<2048;++n)out[n]=0;
    Edit->insertion=Edit->length;
    ns=0;
    if(y > 0){
	    for(n=0;n<Edit->length;++n){
	        if(Edit->text[n] == '\r'){
	            --y;
	            ns=n+1;
	            if(y <= 0)break;
	        }
	    }
    }
    for(n=ns;n<Edit->length;++n){
        if(Edit->text[n] == '\r'){
            break;
        }
        out[n-ns]=Edit->text[n];
        xsize=uCStringWidth(out,Edit->myIcon);
		if(x <= xsize){
		    dx=(xsize-xold)/2;
			if((xsize - x) < dx){
				Edit->insertion=n+1;
			}else{
				Edit->insertion=n;
			}
			if(Edit->insertion < 0)Edit->insertion = 0;
			/*
    		sprintf(WarningBuff,"Edit->insertion %d x %d y %d dx %d xsize %d xold %d\n",Edit->insertion,x,y,dx,xsize,xold);
    		WarningBatch(WarningBuff);
    		*/
			break;
		}
		/*
    	sprintf(WarningBuff,"xsize %d xold %d %s\n",xsize,xold,out);
    	WarningBatch(WarningBuff);
    	*/
        xold=xsize;
    }
    
	
	//extend=extend;
	
	uEditSetCursor(Edit);
	
	uEditDraw(Edit);
	
	return 0;
}

uEditPtr uEditNew(uRect *box,IconPtr myIcon)
{
	uEditPtr Edit;
	
	if(!box || !myIcon)return NULL;
	
	Edit=(uEditPtr)cMalloc(sizeof(*Edit),8132);
	if(!Edit)return NULL;
	zerol((char *)Edit,sizeof(*Edit));
	
	Edit->box = *box;
	
	Edit->myIcon=myIcon;

    uGetFontInfo(myIcon,&Edit->Info);

	uGetFontStuff(&Edit->font,myIcon);
	
	Edit->CursorTime=TickCount()-50;
        
	return Edit;
	
}

int uEditInsert(char *save,int length,uEditPtr Edit)
{
	int n;
	
	if(!save || !Edit)return 1;
	
	if(length >= 2048)length=2047;
	
	for(n=0;n<length;++n){
	    Edit->text[n]=save[n];
	}
	
	Edit->text[length]=0;
	
	Edit->length=length;
	
	Edit->insertion=length;
	
	return 0;
}
int uEditDelete(uEditPtr Edit)
{
	if(!Edit)return 1;
	cFree((char *)Edit);
	return 0;
}

