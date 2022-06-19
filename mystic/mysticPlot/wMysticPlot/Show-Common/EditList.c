#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXTERN22 extern

#include "Xappl.h"
#include "EditList.h"
#include "Universal.h"

static void EditInsertLine(EditPtr e,char *data);
static void EditShiftAtRow(EditPtr e,long number);
static int EditCheckLineSpace(EditPtr e,long number);
static void EditInsertCharacter(EditPtr e,char Data);

EditPtr EditNewFile(char *path)
{
	char buff[5120],word[20000];
	char **LineBuffer,*cdata;
	FILE *inout;
	int n,m,k,itWas,c;
	long LineMax,LineCount;
	EditPtr e;

	e=NULL;
	cdata=NULL;
	inout=NULL;
	LineBuffer=NULL;
	LineCount=0;
	LineMax=0;
	
	if(!path)goto Empty;

	if((inout=fopen(path,"rb")) == NULL){
	    sprintf(buff,"EditNewFile Cannot open file : %s to read%c\n",path,0);
	    Warning(buff);
	    return NULL;
	}

	itWas = -7777;
	LineMax=0;
	while((m=(int)fread(buff,1,5120,inout)) > 0){
	    for(n=0;n<m;++n){
	        c=buff[n];
			if(c == '\r'){
			    ++LineMax;
			}else if ((c == '\n')  && (itWas != '\r')){
			    ++LineMax;
			}
			itWas=c;
	    }
	}
	

	LineMax = (LineMax + LineMax/2);
	if(LineMax < 1000)LineMax=1000;
	if((LineBuffer=(char **)cMalloc(LineMax*sizeof(char *),8356)) == NULL){
	    goto Out;
	}

	zerol((char *)LineBuffer,LineMax*sizeof(char *));

	rewind(inout);

	itWas = -7777;
	
	k=0;
	while((m=(int)fread(buff,1,5120,inout)) > 0){
	    for(n=0;n<m;++n){	      
	        c=buff[n];
			if(c == '\n' || c == '\r' || (k >= 19999)){
			    if((c == '\n') && (itWas == '\r')){
			        continue;
			    }
			    word[k++]='\0';
			    if((cdata=(char *)cMalloc(k*sizeof(char),8314)) == NULL){
					goto Out;
			    }
			    mstrncpy(cdata,word,k);
			    LineBuffer[LineCount++]=cdata;
			    cdata=NULL;
			    k=0;
			}else{
			    word[k++]=buff[n];
			}
			itWas = c;
	    }
	}
	if(k != 0){
	    word[k++]='\0';
	    if((cdata=(char *)cMalloc(k*sizeof(char),8315)) == NULL){
	        goto Out;
	    }else{
			mstrncpy(cdata,word,k);
	        LineBuffer[LineCount++]=cdata;
			cdata=NULL;
	    }
	}
Empty:
	e=(EditPtr)cMalloc(sizeof(*e),8357);
	if(!e)goto Out;
	zerol((char *)e,sizeof(*e));
	e->LineMax=LineMax;
	e->LineCount=LineCount;
	e->LineBuffer=LineBuffer;
Out:
	if(inout)fclose(inout);
	if(!e){
	    Warning("EditNewFile out of Memory");
	    if(cdata)cFree((char *)cdata);
	    if(LineBuffer){
	        for(n=0;n<LineCount;++n){
	            if(LineBuffer[n]){
	                cFree((char *)LineBuffer[n]);
					LineBuffer[n]=NULL;
	            }
	        }
	        if(LineBuffer)cFree((char *)LineBuffer);
			LineBuffer=NULL;
	    }
	}
	return e;
}
int EditListAppendLine(EditPtr e,char *data)
{

	if(!e)return 1;
	if(!data)return 1;
	
	e->row=e->LineCount;
	e->column=0;

	EditInsertLine(e,data);	
	         
	return 0;
}
int EditListInsert(EditPtr e,char *data,long row,long column)
{
	long Length,n,nc;
	char *start;

	if(!e)return 1;
	if(!data)return 1;
	e->row=row;
	e->column=column;

	Length=(long)strlen(data);

	nc=0;
	start=data;
	for(n=0;n<Length;++n){
	    ++nc;
	    if(data[n] != '\r')continue;
	    data[n]=0;
	    --nc;
	     EditInsertLine(e,start);	    
	    start=data+n+1;
	    data[n]='\r';
	    nc = 0;
	}
	for(n=0;n<nc;++n) EditInsertCharacter(e,start[n]);
	return 0;
}
static void EditInsertLine(EditPtr e,char *data)
{
	long Length,n,rFirst,cFirst,nn,nl;
	char *bp,*od;

	if(!e)return;
	if(EditCheckLineSpace(e,2L))return;
	
	Length=(long)(strlen(data));
	
	rFirst=e->row;
	cFirst=e->column;

	if(rFirst >= e->LineCount){
	    if((bp=(char *)cMalloc(Length+1,8359)) == NULL){
	        return;
	    }
	    rFirst=e->LineCount++;
	    e->row=e->LineCount;
	    e->LineBuffer[rFirst]=bp;
	    mstrncpy(bp,data,Length+1);
	    return;
	}

	od=e->LineBuffer[rFirst];

	if(od){
	    nn=(long)(strlen((od)));
	    if(cFirst > nn){
	        cFirst=nn;
	        nn = 0;
	    }else{
	       nn -= cFirst;
	    }
	}else{
	    nn=0;
	    cFirst=1;
	}

	if((bp=(char *)cMalloc(Length+cFirst+1,8321)) == NULL){
	    return;
	}

	nl=0;
	if(od)for(n=0;n<cFirst;++n)bp[nl++]=*(od+n);
	for(n=0;n<Length;++n)bp[nl++]=*(data+n);
	bp[nl]=0;

	e->column=0;
	if(cFirst <= 0){		/* At Start of Line Just Insert New Line */
	    EditShiftAtRow(e,rFirst);
	    e->LineBuffer[rFirst]=bp;
	    ++e->row;
	}else{
	    if(nn == 0){		/* At End of Line Just Insert New Line	*/	
	        if(e->LineBuffer[rFirst])cFree((char *)e->LineBuffer[rFirst]);
	        e->LineBuffer[rFirst]=bp;
	        ++e->row;
	    }else{			/* Must Split Line Here */
	        EditShiftAtRow(e,rFirst);
	        e->LineBuffer[rFirst++]=bp;
	        bp=e->LineBuffer[rFirst];
		for(n=0;n<nn;++n)*bp++ = *(e->LineBuffer[rFirst]+cFirst+n);
		*bp=0;	        
	        ++e->row;
	    }
	}
	
}
static void EditInsertCharacter(EditPtr e,char Data)
{
	long Length,n,rFirst,cFirst,nn,nl,nc;
	char *bp;

	if(!e)return;
	
	Length=1;
	rFirst=e->row;
	cFirst=e->column;
	if(EditCheckLineSpace(e,rFirst+2L))return;
	if(!e->LineBuffer)return;

	if(e->LineBuffer[rFirst]){
	    nn=(long)(strlen((e->LineBuffer[rFirst]+cFirst)));
	    nc=(long)(strlen((e->LineBuffer[rFirst])));
	    if((bp=(char *)cMalloc(Length+nc+1,8320)) == NULL){
	        return;
	    }
	    nl=0;
	    for(n=0;n<cFirst;++n)bp[nl++]=*(e->LineBuffer[rFirst]+n);
	    bp[nl++]=Data;
	    for(n=0;n<nn;++n)bp[nl++] = *(e->LineBuffer[rFirst]+cFirst+n);
	    bp[nl]=0;
	    if(e->LineBuffer[rFirst])cFree((char *)e->LineBuffer[rFirst]);
	}else{
	    if((bp=(char *)cMalloc(2,8358)) == NULL){
	        return;
	    }
	    *bp=Data;
	    bp[1]=0;
	    cFirst=0;
	    if(rFirst > e->LineCount)rFirst=e->LineCount;
	    if(e->LineCount <= rFirst)e->LineCount=rFirst+1;
	}
	e->LineBuffer[rFirst]=bp;
	e->column=cFirst+1;
	e->row=rFirst;
}
int EditkillSelection(EditPtr e,long rFirst,long cFirst,long rLast,long cLast)
{
	long nr,nc,n,k,k1,k2;
	char *bp,*bpp;

	if(!e)return 1;
	if(!e->LineBuffer)return 1;
	if(cLast > 0 && (rLast >= e->LineCount))rLast = e->LineCount - 1;
	if(rLast < 0)return 1;

	if(rFirst < 0)rFirst=0;
	if(rFirst > rLast)rFirst=rLast;

	if(cFirst < 0)cFirst=0;


	if(rFirst == rLast){
	    n=0;
	    bp=e->LineBuffer[rFirst];
	    k=(long)(strlen(e->LineBuffer[rFirst]));
	    for(nc=0;nc<k;++nc){
	        if(nc >= cFirst  && nc < cLast)continue;
	        bp[n++] = *(e->LineBuffer[rFirst]+nc);
	    }
	    bp[n]=0;
	}else{
	    if(cFirst == 0){
	        k1 = rFirst;
	    }else{
	        k1 = -1;
	        *(e->LineBuffer[rFirst]+cFirst) = 0;
	    }
	  
	    if(cLast == 0){
	        k2 = rLast-1;
	        if(k2 >= e->LineCount)k2=e->LineCount - 1;
	    }else{
	        k2 = -1;
	        n=0;
	        bp=e->LineBuffer[rLast];
	        k=(long)(strlen(e->LineBuffer[rLast]));
	        for(nc=0;nc<k;++nc){
	            if(nc < cLast)continue;
	            bp[n++] = *(e->LineBuffer[rLast]+nc);
	        }
	        bp[n]=0;
	        if(k1 == -1){
	            k=(long)(n+strlen(e->LineBuffer[rFirst])+1);
	            if((bpp=(char *)cMalloc(k,8322)) == NULL){
	                Warning("EditkillSelection Out of Memory");
	                return 1;
	            }
	            mstrncpy(bpp,e->LineBuffer[rFirst],k);
	            mstrncat(bpp,bp,k);
	            if(e->LineBuffer[rFirst])cFree((char *)e->LineBuffer[rFirst]);
	            e->LineBuffer[rFirst]=bpp;
	            k2 = rLast;
	        }	        
	    }
	    
	    n=0;
	    for(nr=0;nr<e->LineCount;++nr){
	        if(((nr >= rFirst+1) && (nr <= rLast-1)) ||
	             (nr == k1) || (nr == k2)){
	            if(e->LineBuffer[nr]){
	                cFree((char *)e->LineBuffer[nr]);
	                e->LineBuffer[nr]=NULL;
	            }
	        }else{
	            e->LineBuffer[n++]=e->LineBuffer[nr];
	        }
	    }
	    
	    e->LineCount = n;
	    
	    for(nr=e->LineCount;nr<e->LineMax;++nr)e->LineBuffer[nr]=NULL;
	    
	}
	e->row=rFirst;
	e->column=cFirst;
	return 0;
}
char *EditgetSelection(EditPtr e,long rFirst,long cFirst,long rLast,long cLast)
{
	long nr,nc,Length,n;
	char *bp;
	int getLast;

	if(!e)return NULL;
	if(!e->LineBuffer)return NULL;

	if(rLast >= e->LineCount){
	    rLast = e->LineCount - 1;
	    getLast=TRUE;
	}else{
	    getLast=FALSE;
	}

	if(rLast < 0)return NULL;

	if(rFirst < 0)rFirst=0;
	if(rFirst > rLast)rFirst=rLast;

	if(cFirst < 0)cFirst=0;

	Length=0;
	for(nr=rFirst;nr<=rLast;++nr){
	    if(!e->LineBuffer[nr])return NULL;
	    Length += (strlen(e->LineBuffer[nr])+1);
	}

        Length += (rLast-rFirst)+16;

	if((bp=(char *)cMalloc(Length,8323)) == NULL){
	    Warning("EditgetSelection Out of Memory");
	    return bp;
	}
	n=0;
	if(rFirst == rLast){
	    for(nc=cFirst;nc<cLast;++nc){
	        bp[n++] = *(e->LineBuffer[rFirst]+nc);
	    }
	    if(getLast){
	        mstrncpy(&bp[n],e->LineBuffer[rLast],Length-n);
	        n += strlen(e->LineBuffer[rLast]);
	        bp[n++]='\r';
	    }
	}else{
	    mstrncpy(&bp[n],e->LineBuffer[rFirst]+cFirst,Length-n);
	    n += strlen(e->LineBuffer[rFirst]+cFirst);
	    bp[n++]='\r';
	    for(nr=rFirst+1;nr<=rLast-1;++nr){
	        mstrncpy(&bp[n],e->LineBuffer[nr],Length-n);
	        n += strlen(e->LineBuffer[nr]);
	        bp[n++]='\r';
	    }
	    if(getLast){
	        mstrncpy(&bp[n],e->LineBuffer[rLast],Length-n);
	        n += strlen(e->LineBuffer[rLast]);
	        bp[n++]='\r';
	    }else{
	        for(nc=0;nc<cLast;++nc){
	            bp[n++] = *(e->LineBuffer[rLast]+nc);
	        }
	    }
	}
	
	bp[n]=0;

	return bp;
}

static void EditShiftAtRow(EditPtr e,long number)
{
	long n;

	if(!e)return;

	if(EditCheckLineSpace(e,e->LineCount+1))return;
	
	for(n=e->LineCount;n > 0;--n){
	    if(n == number)break;
	    e->LineBuffer[n]=e->LineBuffer[n-1];
	}
	++e->LineCount;
}
int EditGetLast(EditPtr e,long *row,long *column)
{
	if(!e || !row || !column)return 1;
	*row=e->row;
	*column=e->column;
	return 0;
}
static int EditCheckLineSpace(EditPtr e,long number)
{
	long Lines,n;
	char **NewLines;

	if(!e)return 1;
	
	if((e->LineCount+number) <= e->LineMax)return 0;
	
	Lines = (long)(e->LineMax + number + e->LineMax/2 + 500);
	if((NewLines=(char **)cMalloc(Lines*sizeof(char *),8318)) == NULL){
	    Warning("EditCheckLineSpace out of Memory");
	    return 1;
	}
	for(n=0;n<Lines;++n){
	    if(n < e->LineCount){
	        NewLines[n]=e->LineBuffer[n];
	    }else{
	        NewLines[n]=NULL;
	    }
	}
	e->LineMax=Lines;
	if(e->LineBuffer)cFree((char *)e->LineBuffer);
	e->LineBuffer=NewLines;
	return 0;
}
int EditListDeleteOne(EditPtr e,long row,long column)
{
	long n,nc,nn,n1,n2,length;
	char *bp;

	if(!e)return 1;
	if(!e->LineBuffer)return 1;

	if(row >= e->LineCount)row = e->LineCount-1;
	if(row < 0)row=0;

	if(column < 0){
	    if(row <= 0)return 0;
	    if(!e->LineBuffer[row-1] || !e->LineBuffer[row])return 1;
	    n1=(long)strlen(e->LineBuffer[row-1]);
	    column=n1;
	    n2=(long)strlen(e->LineBuffer[row]);
	    length=n1+n2+1;
	    if((bp=(char *)cMalloc(length,8319)) == NULL){
	        return 1;
	    }
	    mstrncpy(bp,e->LineBuffer[row-1],length);
	    mstrncat(bp,e->LineBuffer[row],length);

	    if(e->LineBuffer[row-1])cFree((char *)e->LineBuffer[row-1]);
	    e->LineBuffer[row-1]=NULL;

	    if(e->LineBuffer[row])cFree((char *)e->LineBuffer[row]);
	    e->LineBuffer[row]=NULL;

	    --row;
	    for(n=row;n<e->LineCount-1;++n){
	        e->LineBuffer[n]=e->LineBuffer[n+1];
	    }

	    if(--e->LineCount < 1){
	        e->LineCount=1;
	    }else{
	        e->LineBuffer[e->LineCount]=NULL;
	    }
	    e->LineBuffer[row]=bp;
	}else{
	    bp=e->LineBuffer[row];
	    if(!bp)return 1;
	    nc=(long)strlen(bp);
	    nn=0;
	    for(n=0;n<nc;++n){
	        if(n == column)continue;
	        bp[nn++]=bp[n];
	    }
	    bp[nn]=0;
	}
	e->row=row;
	e->column=column;
	return 0;
}

char *EditListLine(EditPtr e,long Line)
{
	if(!e)return NULL;
	if(!e->LineBuffer)return NULL;
	if((Line < 0) || (Line >= e->LineCount))return NULL;
	return e->LineBuffer[Line];
}

int EditListClose(EditPtr e)
{
	long n;

	if(!e)return 1;

    	if(e->LineBuffer){
    	    for(n=0;n<e->LineCount;++n){
    	        if(e->LineBuffer[n]){
    	            cFree((char *)e->LineBuffer[n]);
		    		e->LineBuffer[n]=NULL;
    	        }
    	    }
    	    if(e->LineBuffer)cFree((char *)e->LineBuffer);
	    	e->LineBuffer=NULL;
    	}

	if(e)cFree((char *)e);

	return 0;
}
