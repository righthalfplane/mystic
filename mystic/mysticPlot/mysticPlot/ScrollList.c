#define EXTERN22 extern
#include "Xappl.h"

#include "ScrollList.h"
#include "uGraph.h"

int goUpper(char *a,int len);
int IsControl(void);

static int intcmp(const void *xx, const void *yy);
int goUpper(char *a,int len);
int ScrollListSetItemSee(long item,struct ScrollList *ListD);

static int doInRect(uPoint pt,struct ScrollList *ListData);


int uOffsetRect(uRect *LineRect,int x,int y);


static int DrawFolder(uRect *LineRectp,struct Icon *myIcon);
static int DrawFile(uRect *LineRectp,struct Icon *myIcon);
static int DrawDevice(uRect *LineRectp,struct Icon *myIcon);
static int DrawList(struct ScrollList *ListData);

static int tdothumb(controlPtr control,IconPtr myIcon);
static int tdoPageUp(controlPtr control,short part,IconPtr myIcon);
static int tdoPageDown(controlPtr control,short part,IconPtr myIcon);
static int tgoUp(controlPtr control,short part,IconPtr myIcon);
static int tgoDown(controlPtr control,short part,IconPtr myIcon);

int ScrollMoveDy(controlPtr control,struct ScrollList *ListD);


int ScrollListSetDouble(double *DataFloat,long Count,struct ScrollList *ListD)
{
	long MaxValue;
	
	if(!ListD || !DataFloat)return 1;
	
	if(ListD->ListInfo){
	    cFree((char *)ListD->ListInfo);
	    ListD->ListInfo=NULL;
	}
	
	if(ListD->DataFloat){
	    cFree((char *)ListD->DataFloat);
	    ListD->DataFloat=NULL;
	}
	
	if(ListD->Flags){
	    cFree((char *)ListD->Flags);
	    ListD->Flags=NULL;
	}
	
	ListD->Flags=(char *)cMalloc(sizeof(char)*Count,9825);
	
	if(!ListD->Flags)return 1;
	
	zerol((char *)ListD->Flags,sizeof(char)*Count);
	
	ListD->DataFloat=DataFloat;
	
	ListD->count=Count;

	MaxValue=ListD->count-ListD->LineCount;
	if(MaxValue < 0){
	    ListD->topline=0;
	    MaxValue=0;
	}
	uSetControlMaximum(ListD->control1,MaxValue);
	
	if(ListD->topline >= ListD->count)ListD->topline=ListD->count-1;
	if(ListD->topline < 0)ListD->topline=0;
		
	return 0;
}
int ScrollListSetFormat(char *Format,struct ScrollList *ListD)
{
	if(!Format || !ListD)return 1;
	mstrncpy(ListD->Format,Format,sizeof(ListD->Format));
	return 0;
}

int ScrollListResetDoubleClick(struct ScrollList *ListD)
{
	if(!ListD)return 1;
	
	ListD->LastClick=TickCount()-140;
	
	return 0;
}

int ScrollListPut(char *buff,long location,struct ScrollList *ListD)
{
	if(!ListD)return 1;
	if(ListD->count <= 0)return 1;
	if(location < 0)return 1;
	if(location >= ListD->count)return 1;
	if(ListD->ListInfo){
	    mstrncpy(ListD->ListInfo[location].LineData,buff,255);
 	    return 0;
	}
	return 1;
}

int ScrollListTop(long location,struct ScrollList *ListD)
{
	if(!ListD)return 1;
	ListD->topline=location;
	if(ListD->control1)uSetControlValue(ListD->control1,ListD->topline);
	return 0;
}
int ScrollListInsertSort(char *data,int type,long SortType,struct ScrollList *ListD)
{
	char buff[256],value[256];
	char valueu[256],datau[256];
	double vfloats,vfloat;
	long location,nc;
	long vints,vint;
	long count;

	if(!data || !ListD)return 1;
	
	if(ListD->DataFloat)return 0;
	
	count=ScrollListCount(ListD);
	if(!count){
	    return ScrollListInsert(&data,&type,0L,1L,ListD);
	}
	location = -1;
	for(nc=0;nc<count;++nc){
	    ScrollListGet(value,nc,ListD);
	    if(SortType == LIST_SORT_ASCII){	
		    mstrncpy(valueu,value,256);	
		    goUpper(valueu,256);
		    mstrncpy(datau,data,256);	
		    goUpper(datau,256);
			if(strcmp(valueu,datau) >= 0){
			    if(!strcmp(datau,valueu))return 0;
			    location = nc;
			    break;
			}
	    }else if(SortType == LIST_SORT_DOUBLE){
			sscanf(value,"%lf",&vfloats);
			sscanf(data,"%lf",&vfloat);
			if(vfloats >= vfloat){
			    if(vfloats == vfloat)return 0;
			    location = nc;
			    break;
			}
	    }else if(SortType == LIST_SORT_LONG){
			sscanf(value,"%ld",&vints);
			sscanf(data,"%ld",&vint);
			if(vints >= vint){
			    if(vints == vint)return 0;
			    location = nc;
			    break;
			}
	    }else{
	        sprintf(buff,"ScrollListInsertSort: Error Unknown LIST_SORT_TYPE  %ld\n",SortType);
	        WarningBatch("buff");
	        return 1;
	    }
	}
	return ScrollListInsert(&data,&type,location,1L,ListD);
}
int ScrollListSelect(long location,struct ScrollList *ListD)
{
    struct ListList *ListInfo;
    
    if(!ListD)return 0;    
   	if(location < 0 || location >= ListD->count)return 1;
   	
	ListInfo=ListD->ListInfo;	
	if(ListInfo){
		ListInfo[location].Selected=TRUE;
	}
   	if(ListD->Flags){
		ListD->Flags[location]=TRUE;
   	}
	return 0;
}
int ScrollListDeleteSelected(struct ScrollList *ListD)
{
    struct ListList *ListInfo;
	long count;
	long n,k;

	if(!ListD)return 1;
	
	if(ListD->DataFloat)return 0;

	ListD->ListError = FALSE;
	if((count=ListD->count) <= 0)return 0;
		
	if((ListInfo=ListD->ListInfo) == NULL){
	    ListD->count=0;
	    return 0;
	}
 
 	n=0;
	for(k=0;k<count;++k){
	    if(ListInfo[k].Selected)continue;
	    ListInfo[n++]=ListInfo[k];
	}

	if(n <= 0){
	    cFree((char *)ListInfo);
	    ListD->count=0;
	    ListD->ListInfo=NULL;
	}else{
	    ListD->count=n;
	}

	ListD->topline=0;

	return 0;
}
int ScrollListDeSelect(struct ScrollList *ListD)
{
    struct ListList *ListInfo;
    long n;
    	
    if(!ListD)return 1;
	ListInfo=ListD->ListInfo;	
	
	if(ListInfo){   	
		for(n=0;n<ListD->count;++n)ListInfo[n].Selected=FALSE;
	}
	
	if(ListD->Flags){   	
		for(n=0;n<ListD->count;++n)ListD->Flags[n]=FALSE;
	}
	return 0;
}

int ScrollListInsertOneType(char **data,int type,long location,long count,struct ScrollList *ListD)
{
    struct ListList *ListInfo;
	long Total,n,MaxValue;
	
	if(!ListD)return 1;

	if(ListD->DataFloat)return 0;
	
	Total=count+ListD->count;	
	ListD->ListError = FALSE;
	
	ListInfo=(struct ListList *)cMalloc(Total*sizeof(struct ListList),8161);	
	if(!ListInfo)return 1;    
	zerol((char *)ListInfo,Total*sizeof(struct ListList));
		
	if(ListD->ListInfo){
	    for(n=0;n<ListD->count;++n)ListInfo[n]=ListD->ListInfo[n];
	    cFree((char *)ListD->ListInfo);
	}else{
	    ListD->count=0;
	}
	
	ListD->ListInfo=ListInfo;
	
	if(location < 0 || location >= ListD->count)location = ListD->count;
	
	for(n=Total-1;n>location;--n){
	    ListInfo[n]=ListInfo[n-1];
	}
	
	for(n=0;n<count;++n){
	    ListInfo[n+location].Selected=FALSE;
	    if(type)ListInfo[n+location].Type=type;
	    if(data && data[n])mstrncpy(ListInfo[n+location].LineData,data[n],256L);
	}
	
	ListD->count=Total;

	MaxValue=ListD->count-ListD->LineCount;
	if(MaxValue < 0)MaxValue=0;
	uSetControlMaximum(ListD->control1,MaxValue);

	location=ListD->count-(ListD->LineCount)+1;
	if(location < 0 || MaxValue == 0)location=0;

	uSetControlValue(ListD->control1,location);
	ListD->topline=location;

	return 0;
}

int ScrollListDispose(struct ScrollList *ListD)
{
    	
	if(!ListD)return 1;
	
	if(ListD->ListInfo){
	    cFree((char *)ListD->ListInfo);
	}
	ListD->ListInfo=NULL;
	
	if(ListD->DataFloat){
	    cFree((char *)ListD->DataFloat);
	}
	ListD->DataFloat=NULL;
	
	if(ListD->Flags){
	    cFree((char *)ListD->Flags);
	}
	ListD->Flags=NULL;	
	
	if(ListD->control1)ControlDispose(ListD->control1);
	ListD->control1=NULL;
	
	if(ListD->control2)ControlDispose(ListD->control2);
	ListD->control2=NULL;
	
	cFree((char *)ListD);	

	return 0;
	
}
long ScrollListFirstSelect(struct ScrollList *ListD)
{
	long count,k;

	if(!ListD)return -1L;
	count=ScrollListCount(ListD);
	for(k=0;k<count;++k){
	    if(ScrollListIsSelected(k,ListD)){
	        return k;
	    }
	}
	return -1L;
}

int ScrollListGetUser(long *ldata,long location,struct ScrollList *ListD)
{
	if(!ListD || !ldata)return 1;
	if(ListD->count <= 0)return 1;
	if(location < 0)return 1;
	if(location >= ListD->count)return 1;
	if(ListD->ListInfo){
	    *ldata=ListD->ListInfo[location].Userdata;
	    return 0;
	}
	return 1;
}

int ScrollListGet(char *buff,long location,struct ScrollList *ListD)
{
	if(!ListD || !buff)return 1;
	if(ListD->count <= 0)return 1;
	if(location < 0)return 1;
	if(location >= ListD->count)return 1;
	if(ListD->ListInfo){
	    mstrncpy(buff,ListD->ListInfo[location].LineData,256L);
 	    return 0;
	}
	return 1;
}

int ScrollListIsSelected(long location,struct ScrollList *ListD)
{
    struct ListList *ListInfo;

    if(!ListD)return 0;
	
   	if(location < 0 || location >= ListD->count)return 0;

	ListInfo=ListD->ListInfo;	
	if(ListInfo){    
		if(ListInfo[location].Selected){
		    return TRUE;
		}
	}
	
	if(ListD->Flags){    
		if(ListD->Flags[location]){
		    return TRUE;
		}
	}
	
	return FALSE;
}

int ScrollListUpdate(struct ScrollList *ListD)
{
	if(!ListD)return 1;
	DrawList(ListD);
	return 0;
}

int ScrollListInsertUser(char **data,int *type,long *ldata,long location,long count,struct ScrollList *ListD)
{
	struct ListList *ListInfo;
	long Total,n;

	if(!ListD)return 1;
	
	if(ListD->DataFloat)return 0;
	
	Total=count+ListD->count;

	ListD->ListError = FALSE;
	ListInfo=(struct ListList *)cMalloc(Total*sizeof(struct ListList),8733);
	if(!ListInfo)return 1;    
	zerol((char *)ListInfo,Total*sizeof(struct ListList));

	if(ListD->ListInfo){
	    for(n=0;n<ListD->count;++n)ListInfo[n]=ListD->ListInfo[n];
	    cFree((char *)ListD->ListInfo);
	}else{
	    ListD->count=0;
	}

	ListD->ListInfo=ListInfo;

	if(location < 0 || location >= ListD->count)location = ListD->count;

	for(n=Total-1;n>location;--n){
	    ListInfo[n]=ListInfo[n-1];
	}

	for(n=0;n<count;++n){
	    if(ldata)ListInfo[n+location].Userdata=ldata[n];
	    if(type)ListInfo[n+location].Type=type[n];
	    if(data)mstrncpy(ListInfo[n+location].LineData,data[n],256L);
	    ListInfo[n+location].Selected=FALSE;
	}

	ListD->count=Total;
	location -= ListD->LineCount-1;
	if(location < 0)location=0;
	ListD->topline=location;
	return 0;
}
static long SortType;
int ScrollListSort(long SortTypeIn,struct ScrollList *ListD)
{
	if(!ListD)return 1;
	if(ListD->DataFloat)return 0;
	SortType=SortTypeIn;
	qsort((char *)ListD->ListInfo, ListD->count, sizeof(struct ListList), intcmp);
	ScrollListSetTop(0,ListD);
	return 0;
}

int goUpper(char *a,int len)
{
        if(!a)return 1;

        while(*a && (--len >= 0)){
            *a=toupper(*a);
            ++a;
        }
        return 0;
}
static int intcmp(const void *xx, const void *yy)
{
        struct ListList *x;
        struct ListList *y;
        char xl[1024],yl[1024];
        double f1,f2;
        long i1,i2;
        int n;

        x=(struct ListList *)xx;
        y=(struct ListList *)yy;

        n=0;
        if(SortType == LIST_SORT_ASCII){
            mstrncpy(xl,x->LineData,1024);
            goUpper(xl,1024);
            mstrncpy(yl,y->LineData,1024);
            goUpper(yl,1024);
            n=strcmp(xl,yl);
        }else if(SortType == LIST_SORT_DOUBLE){
            sscanf(x->LineData,"%lf",&f1);
            sscanf(y->LineData,"%lf",&f2);
            n=(int)(f1-f2);
        }else if(SortType == LIST_SORT_LONG){
            sscanf(x->LineData,"%ld",&i1);
            sscanf(y->LineData,"%ld",&i2);
            n=(int)(i1-i2);
        }

        return n;
}
int ScrollListFitRect(uRect *ri,struct ScrollList *t)
{
	uFontInfo Info;
	uRect r;
	int LineHeight;
	long MaxValue;

	if(!ri || !t)return 1;
	
	r = *ri;
	r.xsize -= 16;
	r.ysize -= 16;

	t->LineRect=r;	
	
	uSetTextFont(t->myIcon,t->listTextFont);
	uSetTextFace(t->myIcon,t->listTextFace);
	uSetFontSize(t->myIcon,t->listTextSize);

	uGetFontInfo(t->myIcon,&Info);		

	LineHeight=Info.LineHeight+2;	

	t->LineRect.ysize=LineHeight;

	t->LineCount=(r.ysize)/LineHeight;

	MaxValue=t->count-t->LineCount;
	if(MaxValue < 0)MaxValue=0;
	uSetControlMaximum(t->control1,MaxValue);
	if(MaxValue == 0){
	    t->topline=0;
	}
	r.ysize=LineHeight*t->LineCount;
	t->LineHeight=LineHeight;

	t->ListRect=r;
	MoveControl(t->control1,r.x+r.xsize-1,r.y);
	SizeControl(t->control1,16,r.ysize);

	MoveControl(t->control2,r.x-1,r.y+r.ysize);
	SizeControl(t->control2,r.xsize,16);

	return 0;
}
int ScrollListDelete(long first,long last,struct ScrollList *ListD)
{
    struct ListList *ListInfo;
	long count;
	long n,k;
	
	if(!ListD)return 1;
	
	ListD->ListError = FALSE;
	
	if(ListD->DataFloat)return 0;
	
	if((count=ListD->count) <= 0)return 0;
	ListInfo=ListD->ListInfo;
	if(!ListInfo){
	    ListD->count=0;
	    return 1;
	}
 
 	n=0;
	for(k=0;k<count;++k){
	    if(k >= first && k <= last)continue;
	    ListInfo[n++]=ListInfo[k];
	}
	if(n <= 0){
	    cFree((char *)ListInfo);
	    ListD->count=0;
	    ListD->ListInfo=NULL;
	    uSetControlMaximum(ListD->control1,0);
	}else{
	    ListD->count=n;
	}
	ListD->topline=0;
	return 0;
}


int ScrollListIsSelectAny(struct ScrollList *ListD)
{
    struct ListList *ListInfo;
    long n;
    	
    if(!ListD)return 0;
    
	ListInfo=ListD->ListInfo;		
	if(ListInfo){
		for(n=0;n<ListD->count;++n){
		    if(ListInfo[n].Selected){
		        return TRUE;
		    }
		}
	}
	if(ListD->Flags){
		for(n=0;n<ListD->count;++n){
		    if(ListD->Flags[n]){
		        return TRUE;
		    }
		}
	}
	return FALSE;
}

int ScrollListSetString(char *name,struct ScrollList *ListD)
{
	char name2[256],name3[256];
	long n,k,m,first,last;
	int c;

	if(!name || !ListD )return 1;
	if(!ListD->ListInfo)return 0;    

	first=0;
	last=ListD->count-1;

	mstrncpy(name3,name,255);
    for(m=0;m<(long)strlen(name3);++m){
        c=name3[m];
		if(c >= 'A' && c <= 'Z'){
		    c = 'a' + (c-'A');
		}
		name3[m]=c;
    }

	k = -1;
	for(n=first;n <= last;++n){
	    ListD->ListInfo[n].Selected=FALSE;
	    mstrncpy(name2,(char *)(ListD->ListInfo[n].LineData),255);
	    for(m=0;m<(long)strlen(name2);++m){
	        c=name2[m];
			if(c >= 'A' && c <= 'Z'){
			    c = 'a' + (c-'A');
			}
			name2[m]=c;
	    }
	    if(strcmp(name2,name3) >= 0){
			if(k == -1){
	 		    k=n;
			}
	    }
	}
	if(k < 0){
	    k=ListD->count-1;
	}
	ListD->ListInfo[k].Selected=TRUE;
	ScrollListSetTop(k-3,ListD);
	ScrollListUpdate(ListD);
	return 0;
}
long ScrollListCount(struct ScrollList *ListD)
{
	if(!ListD)return -1L;
	return ListD->count;
}
int ScrollListInsert(char **data,int *type,long location,long count,struct ScrollList *ListD)
{
    struct ListList *ListInfo;
	long Total,n,MaxValue;
	
	if(!ListD)return 1;

	if(ListD->DataFloat)return 0;

	Total=count+ListD->count;
	
	ListD->ListError = FALSE;
	ListInfo=(struct ListList *)cMalloc(Total*sizeof(struct ListList),8161);	
	if(!ListInfo)return 1;    
	zerol((char *)ListInfo,Total*sizeof(struct ListList));
		
	if(ListD->ListInfo){
	    for(n=0;n<ListD->count;++n)ListInfo[n]=ListD->ListInfo[n];
	    cFree((char *)ListD->ListInfo);
	}else{
	    ListD->count=0;
	}
	
	ListD->ListInfo=ListInfo;
	
	if(location < 0 || location >= ListD->count)location = ListD->count;
	
	for(n=Total-1;n>location;--n){
	    ListInfo[n]=ListInfo[n-1];
	}
	
	for(n=0;n<count;++n){
	    ListInfo[n+location].Selected=FALSE;
	    if(type)ListInfo[n+location].Type=type[n];
	    if(data && data[n])mstrncpy(ListInfo[n+location].LineData,data[n],256L);
	}
	
	ListD->count=Total;

	MaxValue=ListD->count-ListD->LineCount;
	if(MaxValue < 0){
	    MaxValue=0;
	}
	uSetControlMaximum(ListD->control1,MaxValue);

	location=ListD->count-(ListD->LineCount)+1;
	if(location < 0 || (MaxValue == 0))location=0;

	uSetControlValue(ListD->control1,location);
	ListD->topline=location;

	return 0;
}


int ScrollListSetTop(long top,struct ScrollList *ListD)
{
	if(!ListD)return 1;
	if(top < 0)top = 0;
	if(top >= ListD->count)top=ListD->count-1;
	if(uGetControlMaximum(ListD->control1) <= 0)top = 0;
	uSetControlValue(ListD->control1,top);
	ListD->topline=top;
	return 0;
}

long ScrollListGetItemCount(char *name,long location,struct ScrollList *ListD)
{
    struct ListList *ListInfo;
	long ListGetItemCount,n;
	
	if(!ListD || !name)return -1;
	ListInfo=ListD->ListInfo;	
	if(!ListInfo)return -1;    
	if((location < 0) || (location >= ListD->count))return -1;
	
    ListGetItemCount=0;
	for(n=0;n<ListD->count;++n){
	    if(!strcmp(ListInfo[n].LineData,name)){
	        if(n == location)return ListGetItemCount;
	        ++ListGetItemCount;
	    }
	}	
	
	return -1;
}

long ScrollListFindItem(char *name,long ItemCount,struct ScrollList *ListD)
{
	struct ListList *ListInfo;
	long n,Count;
	
	if(!name || !ListD)return -1l;
	ListInfo=ListD->ListInfo;	
	if(!ListInfo)return  -1l;
	   
	Count=0;
	for(n=0;n<ListD->count;++n){
	    if(!strcmp(ListInfo[n].LineData,name)){
	        if(Count == ItemCount)return n;
	        ++Count;
	    }
	}
	
	return -1l;
}
int ScrollListSetItemSee(long item,struct ScrollList *ListD)
{
    
	if(!ListD)return 1;
		
	if((item < 0) || (item >= ListD->count))return 1;
	
	if((item < ListD->topline) || (item >= (ListD->topline+ListD->LineCount))){
	    ScrollListSetTop(item-3,ListD);
	}	

	return 0;
}

int ScrollListSetItemSelected(long item,struct ScrollList *ListD)
{
    struct ListList *ListInfo;
    
	if(!ListD)return 1;
	
	if((item < 0) || (item >= ListD->count))return 1;
	
	ScrollListDeSelect(ListD);
	
	ListInfo=ListD->ListInfo;	
	if(ListInfo){ 
		ListInfo[item].Selected = TRUE;
	}
	
	if(ListD->Flags){ 
		ListD->Flags[item] = TRUE;
	}
	
	ScrollListSetItemSee(item,ListD);
	
	return 0;
}
struct ScrollList *ScrollListNew(int top,int left,int LineHeight,int LineWidth,
		    int LineCount,IconPtr myIcon)
{
	struct ScrollList *ListData;
 	controlPtr control;
 	uRect myRect;
 	uRect r;
 	
 	ListData=(struct ScrollList *)NULL;
 	
	if(!myIcon)goto Error;
 		
	ListData=(struct ScrollList *)cMalloc(sizeof(struct ScrollList),8160);

	if(ListData == (struct ScrollList *)NULL)return (struct ScrollList *)NULL;
	
	zerol((char *)ListData,(long)sizeof(struct ScrollList));
	
	ListData->topline=0;
	ListData->LineHeight=LineHeight;
	ListData->LineWidth=LineWidth;
	ListData->LineCount=LineCount;
	ListData->count = 0;
	ListData->ListError = FALSE;
	ListData->myIcon=myIcon;

	
 	myRect.y=top;
 	myRect.x=left;
 	
	myRect.ysize=ListData->LineHeight*ListData->LineCount+2*ListData->ListSide;
	myRect.xsize=ListData->LineWidth;


	ListData->ListRect=myRect;

	r.y=ListData->ListRect.y;
	r.x=ListData->ListRect.x+ListData->ListRect.xsize;
	r.ysize=ListData->ListRect.ysize;
	r.xsize=16;
		
	control=ControlCreate(myIcon,&r,"controlName",
		  controlTypeVScroll,1,0L,
		  0L,0L,10,0L);
	if(!control)goto Error;
	
	ListData->control1=control;
	
	r.y=ListData->ListRect.y+ListData->ListRect.ysize;
	r.x=ListData->ListRect.x;
	r.ysize=16;
	r.xsize=ListData->ListRect.xsize;

	control=ControlCreate(myIcon,&r,"controlName",
		  controlTypeHScroll,1,0L,
		  0L,0L,20,0L);
	if(!control)goto Error;
	
	ListData->control2=control;
	
	uSetControlMaximum(control,600);
			
	ListData->LineRect.y=myRect.x;	
	ListData->LineRect.x=myRect.y;	
	ListData->LineRect.xsize=ListData->LineWidth;	
	ListData->LineRect.ysize=ListData->LineHeight;
	
	myRect.xsize += 16;
	myRect.ysize += 16;
		
	ListData->listTextFont=DefaultFont;
	ListData->listTextSize=14;
	
	ListData->LastClick=TickCount()-140;
	
	mstrncpy(ListData->Format,"%g",20);
	
	return ListData;
Error:
	if(ListData)cFree((char *)ListData);
	return (struct ScrollList *)NULL;
}
static controlPtr ListControl;
static struct ScrollList *List;
static int thumb;
int ScrollListDoControl(IconPtr myIcon,uPoint where,struct ScrollList *ListD)
{
	struct Icon m;
	
	
	if(!myIcon || !ListD)return 1;
	
	ListControl=NULL;
			
	if(uPtInRect(where,&ListD->ListRect)){
	    if(doInRect(where,ListD))return 1;
 	    ListD->LastClick=TickCount();
 	    return 0;
	}
	
	thumb=0;
	
	m = *myIcon;

	myIcon->HControl=ListD->control2;
	myIcon->VControl=ListD->control1;
	myIcon->pPageUp=tdoPageUp;
	myIcon->pPageDown=tdoPageDown;
	myIcon->pLineUp=tgoUp;
	myIcon->pLineDown=tgoDown;
	myIcon->pThumb=tdothumb;

	List = ListD;
	
	uDoControls(myIcon);
	
	*myIcon = m;
	
	if(thumb){
		if(ListControl == ListD->control1)uSetControlValue(ListD->control1,ListD->topline);
		if(ListControl == ListD->control2)uSetControlValue(ListD->control2,ListD->xOffSet);
	}
		
	ListControl=NULL;
	
	return 1;
	
}
static int tdothumb(controlPtr control,IconPtr myIcon)
{
	
	if(!myIcon || !control)return 0;
	
	thumb=TRUE;
	ListControl=control;
		
	return ScrollListdothumb(control,0,List);
	
}
static int tdoPageUp(controlPtr control,short part,IconPtr myIcon)
{
	
		if(!myIcon || !control)return 0;
		
		return ScrollListdoPageUp(control,part,List);
		
}
static int tdoPageDown(controlPtr control,short part,IconPtr myIcon)
{
		
		if(!myIcon || !control)return 0;

		return ScrollListdoPageDown(control,part,List);

}
static int tgoUp(controlPtr control,short part,IconPtr myIcon)
{	
	
		if(!myIcon || !control)return 0;

		return ScrollListgoUp(control,part,List);
	
}
static int tgoDown(controlPtr control,short part,IconPtr myIcon)
{			
		
		if(!myIcon || !control)return 0;
		
		return ScrollListgoDown(control,part,List);

}
static int DrawList(struct ScrollList *ListD)
{
	uRect ListRect,LineRect;
   	struct ListList *ListInfo;
   	double *DataFloat;
	uRect r;
	char name[256];
   	long n,count,line;
    char format[40];
    uPort Port;
    int nl;
	
	if(!ListD)return 0;
	if(!ListD->myIcon)return 0;
	
	uGetPort(&Port);
	
	SetPortMyWindow(ListD->myIcon);
	  	
	ListInfo=ListD->ListInfo;
	DataFloat=ListD->DataFloat;
	
	count=ListD->count;
	
	
	uSetForeColor(0,0,0,ListD->myIcon);
	uSetBackColor(255,255,255,ListD->myIcon);

	uEraseRect(&ListD->ListRect,ListD->myIcon);

	if((!ListInfo && !DataFloat) || !count || ListD->ListError){
	    goto Exit;  
	}  
	
	nl=(int)(log10((double)ListD->count)+1);
	if(nl < 2)nl=2;
	if(nl > 9)nl=9;
	sprintf(format,"%s%d%s %s%c","%0",nl,"ld",ListD->Format,0);

	ListRect=ListD->ListRect;

	r.x=ListRect.x+ListD->ListSide;
	r.y=ListRect.y+ListD->ListSide;
	r.xsize=ListRect.xsize-2*ListD->ListSide;
	r.ysize=ListRect.ysize-2*ListD->ListSide;
	uClipRect(&r,ListD->myIcon);

	uSetTextFont(ListD->myIcon,ListD->listTextFont);
	uSetTextFace(ListD->myIcon,ListD->listTextFace);
	uSetFontSize(ListD->myIcon,ListD->listTextSize);
	
	for(n=ListD->topline,line=0;(line < ListD->LineCount) && (n < ListD->count);++n,++line){
	    int xshift;

	    if(n >= ListD->count)continue;

	    LineRect=ListD->LineRect;

	    uOffsetRect(&LineRect,0,(int)(line*ListD->LineHeight));

	    LineRect.x -= ListD->xOffSet;
	    
	    
	    *name = 0;
	    
	    xshift=0;
	    
	    if(ListInfo){
	    	mstrncat(name,ListInfo[n].LineData,255);
		    if(ListInfo[n].Type == 1){
		        DrawFolder(&LineRect,ListD->myIcon);
		        xshift=20;
		    } else if(ListInfo[n].Type == 0){
		        DrawFile(&LineRect,ListD->myIcon);
		        xshift=20;
		    } else if(ListInfo[n].Type == 4){
		        DrawDevice(&LineRect,ListD->myIcon);
		        xshift=20;
		    }	    	
	    }

	    if(DataFloat){
	    	sprintf(name,format,n+1,DataFloat[n]);
	    }


	    uMoveTo(LineRect.x+4+xshift,LineRect.y+LineRect.ysize-4);
	    uDrawString(name,ListD->myIcon);
	    
  	    if((ListInfo && ListInfo[n].Selected) || (ListD->Flags && ListD->Flags[n])){
	        uInvertRect(&LineRect,ListD->myIcon);
 	    }

	}

	uGetPortBounds(ListD->myIcon,&r);

	uClipRect(&r,ListD->myIcon);
	
Exit:
	
	uSetPort(&Port);
	
	return 0;
}
static int DrawDevice(uRect *LineRectp,struct Icon *myIcon)
{
	int top,left,bottom,right;

	if(!LineRectp)return 1;
	
	top=LineRectp->y;
	left=LineRectp->x;
	bottom=top+LineRectp->ysize;
	right=left+20;
	
	uMoveTo(left+4,bottom-5);
	uLineTo(right-4,bottom-5,myIcon);
	
	uMoveTo(left+4,bottom-11);
	uLineTo(right-4,bottom-11,myIcon);
	
	uMoveTo(left+3,bottom-10);
	uLineTo(left+3,bottom-6,myIcon);
	
	uMoveTo(right-3,bottom-10);
	uLineTo(right-3,bottom-6,myIcon);
	
	uMoveTo(left+6,bottom-7);
	uLineTo(left+6,bottom-7,myIcon);

	uMoveTo(right+5,bottom-4);

	return 0;
}
static int DrawFile(uRect *LineRectp,struct Icon *myIcon)
{
	int left;
	int bottom;

	left = LineRectp->x;
	bottom = LineRectp->y+LineRectp->ysize;
	
	left += 2;
	bottom -= 2;
	
	uMoveTo(left,bottom);
	uLineTo(left+12,bottom,myIcon);
	
	uMoveTo(left,bottom-15);
	uLineTo(left+9,bottom-15,myIcon);
	
	uMoveTo(left,bottom-15);
	uLineTo(left,bottom,myIcon);
	
	uMoveTo(left+12,bottom);
	uLineTo(left+12,bottom-12,myIcon);
	
	uMoveTo(left+12,bottom-12);
	uLineTo(left+9,bottom-12,myIcon);
	
	uMoveTo(left+9,bottom-12);
	uLineTo(left+9,bottom-15,myIcon);
	
	uMoveTo(left+9,bottom-15);
	uLineTo(left+12,bottom-12,myIcon);
	return 0;
}
static int DrawFolder(uRect *LineRectp,struct Icon *myIcon)
{
	int left;
	int bottom;

	left = LineRectp->x;
	bottom = LineRectp->y+LineRectp->ysize;
	
	left += 2;
	bottom -= 2;
	
	uMoveTo(left,bottom);
	uLineTo(left+15,bottom,myIcon);
	
	uMoveTo(left,bottom-10);
	uLineTo(left+14,bottom-10,myIcon);
	
	uMoveTo(left,bottom-10);
	uLineTo(left,bottom,myIcon);
	
	uMoveTo(left+15,bottom-9);
	uLineTo(left+15,bottom,myIcon);
	
	uMoveTo(left+2,bottom-12);
	uLineTo(left+6,bottom-12,myIcon);
	
	uMoveTo(left+1,bottom-11);
	uLineTo(left+1,bottom-11,myIcon);
	
	uMoveTo(left+7,bottom-11);
	uLineTo(left+7,bottom-11,myIcon);
	return 0;
}
static int doInRect(uPoint pt,struct ScrollList *ListData)
{
   	struct ListList *ListInfo;
   	char *Flags;
	uRect LineRect;
	int flag;
	long n,line,count,k,last,nf,ne;
    unsigned long CurrentClick;

	if(!ListData)return 1;
	ListData->DoubleClick=FALSE;
	ListInfo=ListData->ListInfo;
	Flags=ListData->Flags;
	
	
	count=ListData->count;
	if((!ListInfo && !Flags) || !count)return 1;    
	
	if(!IsControl() && !IsShift()){
	    for(n=0;n < ListData->count;++n){
	        if(ListInfo)ListInfo[n].Selected=FALSE;
	        if(Flags)Flags[n]=FALSE;
	    }
	}
	
	CurrentClick=TickCount();
	for(n=ListData->topline,line=0;n < ListData->count && line < ListData->LineCount;++n,++line){
	    LineRect=ListData->LineRect;
	    uOffsetRect(&LineRect,0,(int)(line*ListData->LineHeight));
	    if(uPtInRect(pt,&LineRect)){
	        if(IsShift()){
	        	if(ListInfo){
		            if(ListInfo[ListData->ListLast].Selected){
		                if(ListInfo[n].Selected){
		                   flag = FALSE;
		                }else{
		                   flag = TRUE;
		                }
		                last=ListData->ListLast;
		                if(last > n){
		                    nf=n;
		                    ne=last;
		                }else{
		                    nf=last;
		                    ne=n;
		                }
		                for(k=nf;k<=ne;++k)ListInfo[k].Selected= flag;
		            }else{
			        	ListInfo[n].Selected=TRUE;
		            }
	            }else if(Flags){
		            if(Flags[ListData->ListLast]){
		                if(Flags[n]){
		                   flag = FALSE;
		                }else{
		                   flag = TRUE;
		                }
		                last=ListData->ListLast;
		                if(last > n){
		                    nf=n;
		                    ne=last;
		                }else{
		                    nf=last;
		                    ne=n;
		                }
		                for(k=nf;k<=ne;++k)Flags[k]= flag;
		            }else{
			        	Flags[n]=TRUE;
		            }
	            }
	        }else if(IsControl()){
		    	if(ListInfo)ListInfo[n].Selected = !ListInfo[n].Selected;
		    	if(Flags)Flags[n] = !Flags[n];
	        }else{
	            if(ListInfo)ListInfo[n].Selected=TRUE;
	            if(Flags)Flags[n]=TRUE;
			    if(CurrentClick < ListData->LastClick+120){	/* Double Click  */
			        if(ListData->ListLast == n){
			            ListData->DoubleClick=TRUE;
			        }else{
			            ListData->DoubleClick=FALSE;
			        }
			    }
	        }
	        ListData->ListLast=n;
	        ListData->LastClick=TickCount();
			break;
	    }
	}
	DrawList(ListData);
	return 0;
}
int ScrollListdothumb(controlPtr control,short part,struct ScrollList *sData)
{
		long newp;

		//part=part;
		if(!sData)return 1;

		/* old=sData->LastControlLocation; */
		newp=uGetControlValue(control);
		/* if(old == newp)return 0; */
		if(sData->control1 == control){
		    ScrollMoveDy(control,sData);
		}else{
		    sData->xOffSet=(int)newp;
		    DrawList(sData);
		}
		sData->LastControlLocation=uGetControlValue(control);

		return 0;
}
int ScrollListdoPageUp(controlPtr control,short part,struct ScrollList *sData)
{
		long newp,current;
		
		//part=part;
		if(!sData)return 1;

		newp=uGetControlValue(control);
		if(sData->control1 == control){
		    uSetControlValue(control,newp-sData->LineCount);
		    if(uGetControlValue(control) == newp)return 0;
		    ScrollMoveDy(control,sData);
		}else{
		    uSetControlValue(control,newp-10);
		    if((current=uGetControlValue(control)) == newp)return 0;
		    sData->xOffSet=(int)current;
		    DrawList(sData);
		}
		return 0;
}
int ScrollListdoPageDown(controlPtr control,short part,struct ScrollList *sData)
{
		long newp,current;

		//part=part;
		if(!sData)return 1;

		newp=uGetControlValue(control);
		if(sData->control1 == control){
		    uSetControlValue(control,newp+sData->LineCount);
		    if(uGetControlValue(control) == newp)return 0;
		    ScrollMoveDy(control,sData);
		}else{
		    uSetControlValue(control,newp+10);
		    if((current=uGetControlValue(control)) == newp)return 0;
		    sData->xOffSet=(int)current;
		    DrawList(sData);
		}
		return 0;
}
int ScrollListgoUp(controlPtr control,short part,struct ScrollList *sData)
{	
	
		long old,current;
	
		//part=part;
		if(!sData)return 1;

		old=uGetControlValue(control);
		if(sData->control1 == control){
		    uSetControlValue(control,old-1);
		    if(uGetControlValue(control) == old)return 0;
		    ScrollMoveDy(control,sData);
		}else{
		    uSetControlValue(control,old-1);
		    if((current=uGetControlValue(control)) == old)return 0;
		    sData->xOffSet=(int)current;
		    DrawList(sData);
		}
	        return 0;
}
int ScrollListgoDown(controlPtr control,short part,struct ScrollList *sData)
{			
		long old,current;

		//part=part;
		if(!sData)return 1;

		old=uGetControlValue(control);
		if(sData->control1 == control){
		    uSetControlValue(control,old+1);
		    if(uGetControlValue(control) == old)return 0;
		    ScrollMoveDy(control,sData);
		}else{
		    uSetControlValue(control,old+1);
		    if((current=uGetControlValue(control)) == old)return 0;
		    sData->xOffSet=(int)current;
		    DrawList(sData);
		}
		return 0;
}

int ScrollMoveDy(controlPtr control,struct ScrollList *ListD)
{
	long newp;
	long dy;

	newp=uGetControlValue(control);
	if(newp == ListD->topline)return 0;
	if(newp > ListD->count-1)newp=ListD->count-1;
	if(newp < 0)newp=0;
	dy=newp-ListD->topline;
	if(!dy)return 0;

	ListD->topline=newp;
	DrawList(ListD);
	return 0;
}

