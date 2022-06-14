#define EXTERN22 extern
#include <ctype.h>
#include "Xappl.h"
#include "uFiles.h"
#include "Message.h"
#include "uLib.h"
#include "uDialog.h"
#include "SaveAllImages.h"
#include "Universal.h"
#include "uDraw.h"
#include "FileManager.h"
#include "Message.h"

int uImageSet(uImagePtr b);
static void uImageDelete(DObjPtr o);
static int uImageMessage(DObjPtr o,long MessageType,void *MessageData);

static char *uImageCopy(DObjPtr o,long *Length);

static int uImageReplaceImage(uImagePtr b,long xsizei,long ysizei,unsigned char *image);

static DObjPtr uImageRotateDirection(uImagePtr b,int dold,int dnew);

static DObjPtr uImageDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff);

static uImagePtr uImagedoBoxes(uImagePtr b,rRect *r,struct uAttributes *vAttributes);

static DObjPtr uImageFlipDirection(uImagePtr b,int dold,int dnew);

static DObjPtr uImageMove(DObjPtr o,double *dx,double *dy);

static DObjPtr uImageDoRotateDirection(DObjPtr o,int direct);

static DObjPtr uImageDoFlipDirection(DObjPtr o,int direct);

static int uImageDraw(DObjPtr o,struct DOList *l);


int WriteRect8ToRect24(unsigned char *in, uRect *SRect,long xsize, long ysize,
                       unsigned char *out,uRect *DRect,long oxsize,long oysize,
                       unsigned char *palette);

static int uImageFrameCount(struct DObject *Object,long *FrameCount);

static int uImageSetFrame(struct DObject *Object,long CurrentFrame);

static struct DObject *uImageDoubleClick(struct DObject *o,rPoint *r);
static DObjPtr uImageDoInformation(DObjPtr o);


static struct DObject *uImageDoubleClick(struct DObject *o,rPoint *r)
{
	if(!o || !r)return NULL;

	
	if(r->x >= o->box.x && r->y >= o->box.y){
		if(r->x <= o->box.x+o->box.xsize && r->y <= o->box.y+o->box.ysize){
			return uImageDoInformation(o);
		}		
	}
		
	return NULL;
}

static struct DObject *uImageWrite(struct DObject *o,FILE8 *inOut,struct DOList *l)
{


	static struct uImage b;
	
	static struct universalTableListStruct Local[] = {
		{"STRI",4014,universal_TypeBytePtr,(void *)&b.dataPtr,0L},
		
		{"DOUB",4015,universal_TypeDouble,(void *)&b.Attributes.xLineWidth},	
		{"DOUB",4016,universal_TypeDouble,(void *)&b.Attributes.yLineWidth},	
		
		
		{"REAL",4017,universal_TypeInt,(void *)&b.Attributes.LinePattern},	
		{"REAL",4018,universal_TypeInt,(void *)&b.Attributes.LineSymbol},	
		{"REAL",4019,universal_TypeInt,(void *)&b.Attributes.SolidPattern},	
		{"REAL",4020,universal_TypeInt,(void *)&b.Attributes.nLineFore},	
		{"REAL",4021,universal_TypeInt,(void *)&b.Attributes.nLineBack},	
		{"REAL",4022,universal_TypeInt,(void *)&b.Attributes.nSolidFore},	
		{"REAL",4023,universal_TypeInt,(void *)&b.Attributes.nSolidBack},
		
		{"REAL",4024,universal_TypeInt,(void *)&b.AngleFlag},
		{"REAL",4025,universal_TypeInt,(void *)&b.FlipFlag},

		{"STRI",4026,universal_TypeString,(void *)&b.name[0],sizeof(b.name)},
		
		{"DOUB",4027,universal_TypeDouble,(void *)&b.box.x},	
		{"DOUB",4028,universal_TypeDouble,(void *)&b.box.y},	
		{"DOUB",4029,universal_TypeDouble,(void *)&b.box.xsize},	
		{"DOUB",4030,universal_TypeDouble,(void *)&b.box.ysize},	
			
		{"REAL",4031,universal_TypeLong,(void *)&b.xsize},
		{"REAL",4032,universal_TypeLong,(void *)&b.ysize},

		
		{"FILE",4033,universal_TypeFilesPtr,(void *)&b.Files},	

		
	};
	
	static struct universalTableStruct Global = {
		"uDraw parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	uImagePtr bp=(uImagePtr)o;
	char head[5];
	long length;
	long tag;
	
	if(!inOut || !l)return NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    Local[0].space=0;
	    b.dataPtr=NULL;
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp=uImageImage(b.xsize,b.ysize,&b.box,&b.Attributes,l);
		if(!bp)goto  ErrorOut;
		length=Local[0].space;
		if(length > 0 && b.dataPtr){
		    if(length >= b.xsize*b.ysize){
		        cFree((char *)bp->dataPtr);
		        bp->dataPtr=b.dataPtr;
		    }else{
		        cFree((char *)b.dataPtr);
		    }
		    b.dataPtr=NULL;
		}
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(struct uImage);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;	
		if(uPref.Files){
			bp->Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
		}		
		return (struct DObject *)bp;			
		
	}else{
	    b = *bp;
	    length=b.xsize*b.ysize;
	    if(b.Files || !b.dataPtr || (length <= 0)){
	    	Local[0].space=0;
	    }else{
	    	Local[0].space=length;
	    }
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (struct DObject *)&b;
ErrorOut:
	return NULL;
}
static int uImageSetFrame(struct DObject *Object,long CurrentFrame)
{
	uImagePtr b=(uImagePtr)Object;
    struct SetFrameData *s;
	
	if(!b)return 1;

	if(!b->Files)return 0;		
    if(!b->Attributes.Animation)return 0;
    if(!b->ReplaceImage)return 1;
	if(FilesSetFrame(CurrentFrame,b->Files))return 1;
	s = &b->Files->SetData;
	
	if((*b->ReplaceImage)(b,s->xsize,s->ysize,(unsigned char *)s->data)){
	    return 1;
	}
		
	return 0;
	
}
static int uImageFrameCount(struct DObject *Object,long *FrameCount)
{
	uImagePtr b=(uImagePtr)Object;
	
	if(!b && !FrameCount)return 1;
	
	if(b->Files){
	    if(b->Files->ImageCount > *FrameCount)*FrameCount=b->Files->ImageCount;
	}
	
	return 0;
	
}
static DObjPtr uImageDoInformation(DObjPtr o)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {240,120,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {335,120,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"0", {115,50,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"10", {115,75,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"1", {115,100,80,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"1", {115,125,80,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"Image", {223,11,80,20},uDialogText,uDialogNone},
      /*  7 */      {"0", {315,50,80,20},uDialogText,uDialogGetDouble},
      /*  8 */      {"0", {315,75,80,20},uDialogText,uDialogGetDouble},
      /*  9 */      {"Left        :", {25,75,70,20},uDialogString,uDialogNone},
      /* 10 */      {"Object Type", {75,9,90,20},uDialogString,uDialogNone},
      /* 11 */      {"Height    :", {25,100,70,20},uDialogString,uDialogNone},
      /* 12 */      {"Width     :", {25,125,70,20},uDialogString,uDialogNone},
      /* 13 */      {"Top         :", {25,50,70,20},uDialogString,uDialogNone},
      /* 14 */      {"x-scale  :", {220,50,65,16},uDialogString,uDialogNone},
      /* 15 */      {"y-scale  :", {220,75,80,20},uDialogString,uDialogNone},
      
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,410,170};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	uImagePtr pBox;
	uImagePtr b=(uImagePtr)o;
	double xscale,yscale;
	double xsize,ysize;
	struct DOList *l;

	if(!b)return NULL;
	l=b->l;
	if(!l)return NULL;
	
		            
	if(b->xsize > 0){
	    xscale=(double)l->dpi*b->box.xsize/(double)b->xsize;
	}else{
	    xscale=1.0;
	}
	if(b->ysize > 0){
	    yscale=(double)l->dpi*b->box.ysize/(double)b->ysize;
	}else{
	    yscale=1.0;
	}
	
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	pBox=NULL;
	
	uList=uDialogOpen("Image Information",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)b->box.y,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)b->box.x,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)b->box.ysize,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)b->box.xsize,
	              	              
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,(double)xscale,
	              	              
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)yscale,
	              	              
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	xscale=uR[7].dreturn;
	
	yscale=uR[8].dreturn;

Start:	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	if((uR[7].dreturn != xscale) || (uR[8].dreturn != yscale)){
	    xscale=uR[7].dreturn;
	    if(xscale <= 0.0){
	       Warning("xscale must be greater that zero");
	       goto Start;
	    }
	    yscale=uR[8].dreturn;
	    if(yscale <= 0.0){
	       Warning("yscale must be greater that zero");
	       goto Start;
	    }
	    
	    xsize=xscale*(double)b->xsize/(double)l->dpi;
	    ysize=yscale*(double)b->ysize/(double)l->dpi;

	    
	
	}else{
		xsize=uR[5].dreturn;
		ysize=uR[4].dreturn;
	}
	
	if((pBox=uImagedoBoxes(b,&b->box,&b->Attributes)) == NULL){
	    goto ErrorOut;
	}
	
	pBox->box.x=uR[3].dreturn;
	pBox->box.y=uR[2].dreturn;
	
	pBox->box.xsize=xsize;
	pBox->box.ysize=ysize;
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	if(ret && pBox){
	  uImageDelete((DObjPtr)pBox);
	  pBox=NULL;
	}
	
	return (DObjPtr)pBox;
}
static int uImageDraw(DObjPtr o,struct DOList *d)
{
	uRect SRect,DRect;
	uImagePtr b=(uImagePtr)o;
	rRect r;

	if(!b || !d)return 1;
	
	r=b->box;
	    
	SRect.x=0;
	SRect.y=0;
	    
	SRect.xsize=(int)b->xsize;
	SRect.ysize=(int)b->ysize;
	    
	DRect.x=LocalToPixel(r.x,d);
	DRect.y=LocalToPixel(r.y,d);
	    
	DRect.xsize=LocalToPixel(r.xsize,d);
	DRect.ysize=LocalToPixel(r.ysize,d);
	    
	WriteRect8ToRect24(b->dataPtr,&SRect,b->xsize, b->ysize,
                       d->data,&DRect,d->xsize,d->ysize,
                       (unsigned char *)d->palette);	  
	return 0;
}

static DObjPtr uImageDoRotateDirection(DObjPtr o,int direct)
{
	uImagePtr b=(uImagePtr)o;

	if(!b)return NULL;

	if(b->AngleFlag == direct)return NULL;
	return uImageRotateDirection(b,b->AngleFlag,direct);
}
static DObjPtr uImageDoFlipDirection(DObjPtr o,int direct)
{
	uImagePtr b=(uImagePtr)o;

	if(!b)return NULL;
	
	if(b->FlipFlag == direct)return NULL;
	return uImageFlipDirection(b,b->FlipFlag,direct);
}
static DObjPtr uImageMove(DObjPtr o,double *dx,double *dy)
{
	uImagePtr pBox;
	uImagePtr b=(uImagePtr)o;

	if(!b || !dx || !dy)return NULL;
	
	if((pBox=(uImagePtr)uImageDuplicate(o,0,dx,dy)) != NULL){
	    pBox->xDup = b->xDup;
	    pBox->yDup = b->yDup;
	}
	return (DObjPtr)pBox;
}
static DObjPtr uImageFlipDirection(uImagePtr b,int dold,int dnew)
{
	uImagePtr pBox,pBox2;
	double dx=0,dy=0;
	unsigned char save;
	int xx,yy,xxsize,yysize;
    unsigned char *ddataPtr;


	if(!b)return NULL;
	
	if(dold == dnew)return NULL;
	
	if(b->AngleFlag != 0){
	    if((pBox=(uImagePtr) uImageRotateDirection(b,b->AngleFlag,0)) == NULL){
	        return NULL;
	    }
	}else if((pBox=(uImagePtr) uImageDuplicate((DObjPtr)b,0,&dx,&dy)) == NULL){
	    return NULL;
	}
	xxsize=(int)pBox->xsize;
	yysize=(int)pBox->ysize;
	ddataPtr=pBox->dataPtr;
	
	if(dold == 0){
	    if(dnew == 1){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize/2;++xx){
	                save=ddataPtr[(xxsize-1-xx)+yy*xxsize];
	                ddataPtr[(xxsize-1-xx)+yy*xxsize]=ddataPtr[xx+yy*xxsize];
	                ddataPtr[xx+yy*xxsize]=save;
	            }
	        }
	    } else if(dnew == 2){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[xx+yy*xxsize];
	                ddataPtr[xx+yy*xxsize]=ddataPtr[xx+(yysize-1-yy)*xxsize];
	                ddataPtr[xx+(yysize-1-yy)*xxsize]=save;
	            }
	        }
	    } else if(dnew == 3){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[xxsize-1-xx+(yysize-1-yy)*xxsize];
	                ddataPtr[xxsize-1-xx+(yysize-1-yy)*xxsize]=ddataPtr[xx+yy*xxsize];
	                ddataPtr[xx+yy*xxsize]=save;
	            }
	        }
	    }
	}else if(dold == 1){
	    if(dnew == 0){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize/2;++xx){
	                save=ddataPtr[(xxsize-1-xx)+yy*xxsize];
	                ddataPtr[(xxsize-1-xx)+yy*xxsize]=ddataPtr[xx+yy*xxsize];
	                ddataPtr[xx+yy*xxsize]=save;
	            }
	        }
	    } else if(dnew == 2){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[xxsize-1-xx+(yysize-1-yy)*xxsize];
	                ddataPtr[xxsize-1-xx+(yysize-1-yy)*xxsize]=ddataPtr[xx+yy*xxsize];
	                ddataPtr[xx+yy*xxsize]=save;
	            }
	        }
	    } else if(dnew == 3){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[xx+yy*xxsize];
	                ddataPtr[xx+yy*xxsize]=ddataPtr[xx+(yysize-1-yy)*xxsize];
	                ddataPtr[xx+(yysize-1-yy)*xxsize]=save;
	            }
	        }
	    }
	}else if(dold == 2){
	    if(dnew == 0){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[xx+yy*xxsize];
	                ddataPtr[xx+yy*xxsize]=ddataPtr[xx+(yysize-1-yy)*xxsize];
	                ddataPtr[xx+(yysize-1-yy)*xxsize]=save;
	            }
	        }
	    } else if(dnew == 1){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[xxsize-1-xx+(yysize-1-yy)*xxsize];
	                ddataPtr[xxsize-1-xx+(yysize-1-yy)*xxsize]=ddataPtr[xx+yy*xxsize];
	                ddataPtr[xx+yy*xxsize]=save;
	            }
	        }
	    } else if(dnew == 3){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize/2;++xx){
	                save=ddataPtr[(xxsize-1-xx)+yy*xxsize];
	                ddataPtr[(xxsize-1-xx)+yy*xxsize]=ddataPtr[xx+yy*xxsize];
	                ddataPtr[xx+yy*xxsize]=save;
	            }
	        }
	    }
	}else if(dold == 3){
	    if(dnew == 0){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[xxsize-1-xx+(yysize-1-yy)*xxsize];
	                ddataPtr[xxsize-1-xx+(yysize-1-yy)*xxsize]=ddataPtr[xx+yy*xxsize];
	                ddataPtr[xx+yy*xxsize]=save;
	            }
	        }
	    } else if(dnew == 1){
	        for(yy=0;yy<yysize/2;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                save=ddataPtr[xx+yy*xxsize];
	                ddataPtr[xx+yy*xxsize]=ddataPtr[xx+(yysize-1-yy)*xxsize];
	                ddataPtr[xx+(yysize-1-yy)*xxsize]=save;
	            }
	        }
	    } else if(dnew == 2){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize/2;++xx){
	                save=ddataPtr[(xxsize-1-xx)+yy*xxsize];
	                ddataPtr[(xxsize-1-xx)+yy*xxsize]=ddataPtr[xx+yy*xxsize];
	                ddataPtr[xx+yy*xxsize]=save;
	            }
	        }
	    }
	}
	
	
	if(b->AngleFlag != 0){
	    if((pBox2=(uImagePtr)uImageRotateDirection(pBox,0,b->AngleFlag)) == NULL){
	        uImageDelete((DObjPtr)pBox);
	        return NULL;
	    }
	    uImageDelete((DObjPtr)pBox);
	    pBox = pBox2;
	}
	pBox->AngleFlag = b->AngleFlag;
	pBox->FlipFlag = dnew;
	
	if(!pBox->Files){
	    pBox->AngleFlag=0;
		pBox->FlipFlag=0;
	}

	return (DObjPtr)pBox;
}
static uImagePtr uImagedoBoxes(uImagePtr b,rRect *r,struct uAttributes *vAttributes)
{
	uImagePtr Image;

	if(!b || !r || !vAttributes)return NULL;
	
	Image = uImageImage(b->xsize,b->ysize,r,vAttributes,b->l);
	
	if(Image == NULL){
	    WarningBatch("uImagedoBoxes Out of Memory\n");
	    return NULL;
	}
	uMemcpy(Image->dataPtr,b->dataPtr,b->length);
	Image->box.x = b->box.x;
	Image->box.y = b->box.y;
	Image->box.xsize = b->box.xsize;
	Image->box.ysize = b->box.ysize;
	Image->AngleFlag = b->AngleFlag;
	Image->FlipFlag = b->FlipFlag;
	Image->Files = b->Files;
	return Image;
}
static DObjPtr uImageDuplicate(DObjPtr o,int iDup,double *xOff,double *yOff)
{
	uImagePtr pImage;
	double x,y;
	uImagePtr b=(uImagePtr)o;

	if(!b || !xOff || !yOff)return NULL;
	
	if((pImage=uImagedoBoxes(b,&b->box,&b->Attributes)) == NULL){
	    return NULL;
	}
	if(iDup){
	    x = b->box.x-b->xDup;
	    y = b->box.y-b->yDup;
	}else{
	    x = *xOff;
	    y = *yOff;
	}
	pImage->xDup = pImage->box.x;
	pImage->yDup = pImage->box.y;
	pImage->box.x += x;
	pImage->box.y += y;

	return (DObjPtr)pImage;
}
DObjPtr uImageRotateDirection(uImagePtr b,int dold,int dnew)
{
	uImagePtr pBox;
	double dx=0,dy=0;
	int xx,yy;
	long xxsize,yysize;
    unsigned char  *ddataPtr;
    double xsize,ysize;
	
	if(!b)return NULL;
	
	if(dold == dnew)return NULL;
	
	if((pBox=(uImagePtr)uImageDuplicate((DObjPtr)b,0,&dx,&dy)) != NULL){
	    pBox->AngleFlag = dnew;
	    pBox->FlipFlag = b->FlipFlag;
	    xxsize=pBox->xsize;
	    yysize=pBox->ysize;
	    ddataPtr=pBox->dataPtr;
	    xsize=pBox->box.xsize;
	    ysize=pBox->box.ysize;
	}else{
	    return NULL;
	}
	if(dold == 0){
	    if(dnew == 1){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[(xxsize-1-xx)*yysize+yy]=b->dataPtr[xx+yy*xxsize];
	            }
	        }
	    }else if(dnew == 2){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[xxsize-1-xx+(yysize-1-yy)*xxsize]=b->dataPtr[xx+yy*xxsize];
	            }
	        }
	    }else if(dnew == 3){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[xx*yysize+yysize-1-yy]=b->dataPtr[xx+yy*xxsize];
	            }
	        }
	    }
	}else if(dold == 1){
	    if(dnew == 0){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[xx*yysize+yysize-1-yy]=b->dataPtr[xx+yy*xxsize];
	            }
	        }
	    }else if(dnew == 2){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[(xxsize-1-xx)*yysize+yy]=b->dataPtr[xx+yy*xxsize];
	            }
	        }
	    }else if(dnew == 3){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[xxsize-1-xx+(yysize-1-yy)*xxsize]=b->dataPtr[xx+yy*xxsize];
	            }
	        }
	    }
	}else if( dold == 2){
	    if(dnew == 0){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[xxsize-1-xx+(yysize-1-yy)*xxsize]=b->dataPtr[xx+yy*xxsize];
	            }
	        }
	    }else if(dnew == 1){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[xx*yysize+yysize-1-yy]=b->dataPtr[xx+yy*xxsize];
	            }
	        }
	    }else if(dnew == 3){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[(xxsize-1-xx)*yysize+yy]=b->dataPtr[xx+yy*xxsize];
	            }
	        }
	    }
	}else if(dold == 3){
	    if(dnew == 0){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[(xxsize-1-xx)*yysize+yy]=b->dataPtr[xx+yy*xxsize];
	            }
	        }
	    }else if(dnew == 1){
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[xxsize-1-xx+(yysize-1-yy)*xxsize]=b->dataPtr[xx+yy*xxsize];
	            }
	        }
	    }else if(dnew == 2){
	        pBox->xsize=yysize;
	        pBox->ysize=xxsize;
	        pBox->box.xsize=ysize;
	        pBox->box.ysize=xsize;
	        for(yy=0;yy<yysize;++yy){
	            for(xx=0;xx<xxsize;++xx){
	                ddataPtr[xx*yysize+yysize-1-yy]=b->dataPtr[xx+yy*xxsize];
	            }
	        }
	    }
	}
	
	if(!pBox->Files)pBox->AngleFlag=0;
	
	return (DObjPtr)pBox;
}
static int uImageReplaceImage(uImagePtr b,long xsizei,long ysizei,unsigned char *image)
{
	long length;
	int flag;

	if(!b || !image)return 1;
	
	flag=1;
		
	length=xsizei*ysizei;
			
	if(xsizei != b->xsize || ysizei != b->ysize){
		if(length > b->xsize*b->ysize){
			if(b->dataPtr){
			    cFree((char *)b->dataPtr);
			    b->dataPtr=NULL;
			}
			if((b->dataPtr=(unsigned char *)cMalloc(length,8076)) == NULL){
			    sprintf(WarningBuff,"CImageReplaceImage Out Of Memory Requested (%ld)\n",length);
			    WarningBatch(WarningBuff);
			    return 1;
			}
		}
	}
	
	uMemcpy(b->dataPtr,image,(long)length);
	
	if(b->AngleFlag != 0 || b->FlipFlag != 0 ){
	
	    int AngleFlag=b->AngleFlag,FlipFlag=b->FlipFlag;
	    uImagePtr ImageN;
	    
	    b->AngleFlag=0;
	    b->FlipFlag=0;
		b->xsize=xsizei;
		b->ysize=ysizei;
			
	    if(FlipFlag){
	        ImageN=(uImagePtr)uImageFlipDirection(b,0,FlipFlag);
	        if(!ImageN)goto OutOfHere;
	        if(b->dataPtr)cFree((char *)b->dataPtr);
	        b->dataPtr=ImageN->dataPtr;
	        ImageN->dataPtr=NULL;
	        b->FlipFlag=ImageN->FlipFlag;
	        uImageDelete((DObjPtr)ImageN);
	    }
	    
	    if(AngleFlag){
	        ImageN=(uImagePtr)uImageRotateDirection(b,0,AngleFlag);
	        if(!ImageN)goto OutOfHere;
	        if(b->dataPtr)cFree((char *)b->dataPtr);
	        b->dataPtr=ImageN->dataPtr;
	        b->AngleFlag=ImageN->AngleFlag;
	        ImageN->dataPtr=NULL;
			b->xsize=ImageN->xsize;
			b->ysize=ImageN->ysize;
	        uImageDelete((DObjPtr)ImageN);
	    }
	}
	flag=0;
OutOfHere:
	return flag;
}
static int uImageMessage(DObjPtr o,long MessageType,void *MessageData)
{
    struct SetFrameData *s;
	uImagePtr b=(uImagePtr)o;
	struct Message1000 *m;
	long n;
	
	if(!b || !MessageData)return 1;
	
	m=(struct Message1000 *)MessageData;
	
	switch(MessageType){
	case MessageType_GET_FILE_OWNER:
		if(!b->Files)return 0;
		for(n=0;n<m->FileCount;++n){
		    if(b->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
		}
		break;
	case MessageType_GET_FILE_LIST:
	    {
		    int isFound;
		    
			if(!b->Files)return 0;
			
		    isFound=FALSE;
			for(n=0;n<m->FileCount;++n){
			    if(b->Files == m->FilesList[n]){isFound=TRUE;break;}
			}
			if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
			    m->FilesList[m->FileCount++]=b->Files;
			}
		}
		break;
	case MessageType_SET_FRAME_NUMBER:
		if(!b->Files)return 0;		
	    if(!b->Attributes.Animation)return 0;
	    if(!b->ReplaceImage)return 1;
		if(FilesSetFrame(m->CurrentFrame,b->Files))return 1;
		s = &b->Files->SetData;
		
		if((*b->ReplaceImage)(b,s->xsize,s->ysize,(unsigned char *)s->data)){
		    return 1;
		}
		break;
	case MessageType_GET_FRAME_COUNT:
		if(b->Files){
		    if(b->Files->ImageCount > m->FrameCount)m->FrameCount=b->Files->ImageCount;
		}
		break;
	case MessageType_PRINT_FILE_INFORMATION:
	    sprintf(WarningBuff,"  HDF Object x %g y %g\n%c",b->box.x,b->box.y,0);
	    WarningBatch(WarningBuff);
	    printFileInformation(b->Files);
		break;
	}	
	return 0;
}
static void uImageDelete(DObjPtr o)
{
	uImagePtr b=(uImagePtr)o;
	
	if(!b)return;
	
	if(b->dataPtr){
	    cFree((char *)b->dataPtr);
	    b->dataPtr=NULL;
	}
	
	ObjectKill((DObjPtr)b);
	
}
static char *uImageCopy(DObjPtr o,long *Length)
{
	uImagePtr b=(uImagePtr)o;
	long size,istart,len;
	uImagePtr pBox;
	struct Obj *obj;
	char *p;

	if(!b)return NULL;
	
	istart=sizeof(struct Obj)+sizeof(*pBox);
	len=b->xsize*b->ysize;
	*Length=size=istart+len;
	if((obj=(struct Obj *)cMalloc(size,8035)) == NULL){
	    return NULL;
	}
	pBox=(uImagePtr)(obj+1);
	obj->type=b->type;
	obj->Length=size;
	*pBox = *b;
	p=(char *)obj;
	p += istart;
	uMemcpy(p,b->dataPtr,(long)len);
	
	return (char *)obj;
}
int uImageSet(uImagePtr b)
{
	if(!b)return 1;
	
	b->Kill=uImageDelete;
	b->type=ImagePlotType;
	b->Message=uImageMessage;	
	b->Move=uImageMove;	
	b->ReplaceImage=uImageReplaceImage;	
	b->Duplicate=uImageDuplicate;	
	b->DoRotateDirection=uImageDoRotateDirection;
	b->DoFlipDirection=uImageDoFlipDirection;
	b->Draw=uImageDraw;
	b->DoInformation=uImageDoInformation;
    b->GetFrameCount=uImageFrameCount;    
    b->SetFrame=uImageSetFrame;    
	b->Copy=uImageCopy;	
	b->ReadWrite=uImageWrite;
	b->DoubleClick=uImageDoubleClick;
	
	return 0;
}
uImagePtr uImageImage(long xs,long ys,rRect *r,struct uAttributes *Attribute,struct DOList *l)
{
	char name[64];
	uImagePtr b;
	
	if(!r || !Attribute || !l)return NULL;
	
	sprintf(name,"Image%03ld%c",l->ObjectIDCount++,0);
	
    b=(uImagePtr)ObjectCreate(name,ImagePlotType,r,sizeof(struct uImage));
    if(!b)return NULL;
	
	if(uImageSet(b))goto ErrorOut;
		
	b->Attributes = *Attribute;
	b->xsize=xs;
	b->ysize=ys;
	b->length=b->xsize*b->ysize*3;
	if((b->dataPtr=(unsigned char *)cMalloc(b->length,8037)) == NULL){
	    sprintf(WarningBuff,"uImageImage Out Of Memory Requested (%ld)\n",b->length);
	    WarningBatch(WarningBuff);
	   goto ErrorOut;
	}
	b->box = *r;
	b->Attributes.Animation=TRUE;
	b->l=l;
	return b;
ErrorOut:
	uImageDelete((DObjPtr)b);
	return NULL;
}

uImagePtr uImageImage2(uImagePtr Pastin,DOListPtr l)
{
	long length;
	char *p;

	uImagePtr b;
	
	if(!Pastin || !l)return NULL;
	
	
	b=(uImagePtr)ObjectObject2((DObjPtr)Pastin);
	
	b->l=l;	
	
	if(uImageSet(b))goto ErrorOut;
	
	b->xsize=Pastin->xsize;
	b->ysize=Pastin->ysize;
	b->Files=Pastin->Files;
	b->CurrentFrame=Pastin->CurrentFrame;
	length=b->xsize*b->ysize;
	if((b->dataPtr=(unsigned char *)cMalloc(length,8039)) == NULL){
	    sprintf(WarningBuff,"uImageImage2 Out Of Memory Requested (%ld)\n",length);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	b->length=length;
	p=(char *)Pastin;
	p += sizeof(*b);
	uMemcpy(b->dataPtr,p,(long)length);
	return b;
ErrorOut:
	uImageDelete((DObjPtr)b);
	return NULL;
}

