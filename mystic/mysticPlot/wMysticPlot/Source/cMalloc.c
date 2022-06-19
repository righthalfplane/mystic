#include <stdio.h>
#include <stdlib.h>
#include "Xappl.h"
#include "Paux.h"

INT8_64 TotalUsedMaximun;

void *cMalloc(unsigned long r,int add);
void *cRealloc(char *p,unsigned long r,int add);
int cFree(char *p);
void checkall();
void *cMallocWindows(unsigned long length);
void *cReallocWindows(char *p,unsigned long r);
int cFreeWindows(char *ptr);


#define DEBUG


#ifdef DEBUG
#define END_OF_MEMORY 90000
static char  *given[END_OF_MEMORY];
static int givenadd[END_OF_MEMORY];
static int maxgiven;
void *cMalloc(unsigned long r,int add)
{
	char *give;
	char buff[256];
	int k;
	
	for(k=0;k<maxgiven;++k){
	    if(!given[k])goto empty;
	}
	
	if(++maxgiven >= END_OF_MEMORY){
	       sprintf(buff,"cMalloc Debug  out of table\n");
	       Warning(buff);
	       exit(1);
	}
	k=maxgiven-1;
empty:
	if(!(give=(char *)cMallocWindows(r))){
	       sprintf(buff,"cMalloc Debug   out of Memory");
	       Warning(buff);
	       exit(1);
	}
	given[k]=give;
	givenadd[k]=add;
	return give; 
}
void *cRealloc(char *p,unsigned long r,int add)
{
	char *give;
	char buff[256];
	int k;
	
	for(k=0;k<maxgiven;++k){
	    if(given[k] == p)goto found;
	}
	sprintf(buff,"Error Tried to realloc %ld add %d\n",p,add);
	Warning(buff);
	return 0;
found:
	if(!(give=(char *)cReallocWindows(p,r))){
	    printf("realloc out of Memory\n");
		sprintf(buff,"realloc out of Memory\n");
		Warning(buff);
	    exit(1);
	}
	given[k]=give;
	givenadd[k]=add;
	return give;
	
}
int cFree(char *p)
{
	char buff[256];
	int k;
	
	for(k=0;k<maxgiven;++k){
	    if(given[k] == p)goto found;
	}
	sprintf(buff,"Error Tried to free %p\n",p);
	//fprintf(stderr, "Error Tried to free %p\n", p);
	WarningBatch(buff);


	//Warning(buff);
	return 1;
found:

	cFreeWindows(p);

	given[k]=NULL;
	if(k == maxgiven-1){
	    --maxgiven;
	    if(maxgiven < 0 )maxgiven=0;
	}	
	return 0;
}
void checkall()
{
	int k,count;
	char buff[256];
	
	count=0;
	for(k=0;k<maxgiven;++k){
	    if(given[k]){
		sprintf(buff,"checkall %ld add %d\n",given[k],givenadd[k]);
		Warning(buff);
	        ++count;
	    }
	}
	if(count){
	    sprintf(buff,"checkall not free %d\n",count);
	    Warning(buff);
	}else{
	    maxgiven=0;
	    Warning("checkall all Free");
	}
}
#else
void *cMalloc(unsigned long length,int add)
{
	return cMallocWindows(length);
}
void *cRealloc(char *p,unsigned long r,int add)
{

	return cReallocWindows(p,r);	
}
int cFree(char *ptr)
{
	return cFreeWindows(ptr);
}
void checkall()
{
  ;
}
#endif


void *cMallocWindows(unsigned long length)
{
	char buff[256];
	char *ret,*retl;
	
	ret=(char *)GlobalAlloc(GHND,length+4L);
	if(ret == NULL){
	    sprintf(buff,"cMalloc Out of Memory Requested (%ld)",length);
	    Warning(buff);
	    return (char *)NULL;
	}else if( (retl=(char *)GlobalLock(ret)) == NULL){
	    GlobalFree(ret);
	    sprintf(buff,"cMalloc Could Not Lock Handle");
	    Warning(buff);
	    return (char *)NULL;
	}
	return retl;
}
void *cReallocWindows(char *p,unsigned long r)
{
	char buff[256];
	char *pn;
	char *ptr;

	if(ptr=GlobalHandle(p)){
	    GlobalUnlock(ptr);
	}else{
	   sprintf(buff,"cRealloc Could Not Get Handle");
	   Warning(buff);
	   return NULL;
	}

	if(!(pn=(char *)GlobalReAlloc(ptr,r+4,GMEM_MOVEABLE))){
	    sprintf(buff,"cRealloc GlobalReAlloc failed");
	    Warning(buff);
	    return (char *)NULL;
	}

	ptr=(char *)GlobalLock(pn);
	if(!ptr){
	    GlobalFree(pn);
	    sprintf(buff,"cRealloc Could Not Lock Handle");
	    Warning(buff);
	    return NULL;
	}

	return ptr;	
}
int cFreeWindows(char *ptr)
{
	if(ptr=GlobalHandle(ptr)){
	    GlobalUnlock(ptr);
	    GlobalFree(ptr);
	    return 0;
	}
	return 1;
}

int zerol(char *s,unsigned long n)
{
	unsigned long count;

	if(s == NULL)return 1;
	count=0;
	while(count++ < n)*s++ = 0;
	return 0;
}

