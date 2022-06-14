#define EXTERN22 extern
#include <stdio.h>
#include <stdlib.h>
#include "Xappl.h"
#include "xAux.h"
#include "Tools.h"
#include "ulibTypes.h"


int WarningBatch(char *buff);

/* #define DEBUG */
 
#define MALLOC_MAX    8446

void checkall(void);

INT8_64 TotalUsedMaximun;

static long total;

#ifdef DEBUG
#define END_OF_MEMORY 50000
static void  *given[END_OF_MEMORY];
static int tagm[END_OF_MEMORY];
static int maxgiven;


void *cMalloc(unsigned long r,int tag)
{
	void *give;
	char buff[256];
	int k;
	
	for(k=0;k<maxgiven;++k){
	    if(!given[k])goto empty;
	}
	
	if(++maxgiven >= END_OF_MEMORY){
	       sprintf(buff,"cMalloc Debug  out of table\n");
	       fprintf(stderr,"%s\n",buff);
	       exit(1);
	}
	k=maxgiven-1;
empty:
	if(!(give=(void *)calloc(r+4L,1))){
	       sprintf(buff,"cMalloc Requested (%ld) out of Memory total Used (%ld) \n",r,total);
	       fprintf(stderr,"%s\n",buff);
	       exit(1);
	}
	
	total += r+4L;
	given[k]=give;
	tagm[k]=tag;
	return give;
}
void *cRealloc(char *p,unsigned long r,int tag)
{
	void *give;
	char buff[256];
	int k;
	
	for(k=0;k<maxgiven;++k){
	    if(given[k] == p)goto found;
	}
	sprintf(buff,"Error Tried to realloc %ld\n",(long)p);
	fprintf(stderr,"%s\n",buff);
	return 0;
found:

	if(!(give=(void *)realloc(p,r+4L))){
		sprintf(buff,"realloc out of Memory\n");
		fprintf(stderr,"%s\n",buff);
	       	exit(1);
	}
	given[k]=give;
	tagm[k]=tag;
	return give;
	
}
int cFree(char *p)
{
	char buff[256];
	int k;
	
	for(k=0;k<maxgiven;++k){
	    if(given[k] == p)goto found;
	}
	/* sprintf(buff,"Error Tried to free %lu rank %d\n",(unsigned long)p,xg.rank); */
	sprintf(buff,"Error Tried to free %lu\n",(unsigned long)p);
	fprintf(stderr,"%s\n",buff);
	return 1;
found:
	free(p);
	given[k]=NULL;
	if(k == maxgiven-1){
	    --maxgiven;
	    if(maxgiven < 0 )maxgiven=0;
	}	
	return 0;
}
void checkall(void)
{
	int k,count;
	char buff[256];
	
	count=0;
	for(k=0;k<maxgiven;++k){
	    if(given[k]){
		sprintf(buff,"checkall k %d %ld tag %d\n",k,(long)given[k],tagm[k]);
		fprintf(stderr,"%s\n",buff);
	        ++count;
	    }
	}
	if(count){
	    sprintf(buff,"checkall not free %d\n",count);
	    fprintf(stderr,"%s\n",buff);
	}else{
	    maxgiven=0;
	   fprintf(stderr,"checkall all Ok\n");
	}
}
#else
void *cMalloc(unsigned long length,int tag)
{
	char buff[256];
	char *ret;
	
	//tag=tag;
	
	ret=(char *)calloc(length+4L,1);
	if(ret == NULL){
	    fprintf(stderr,"cMalloc Out of Memory Requested (%ld) Total Used (%ld) tag (%d)\n",length,total,tag);
	    sprintf(buff,"cMalloc Out of Memory Requested (%ld) Total Used (%ld) tag (%d)\n",length,total,tag);
	    WarningBatch(buff);
	    return (char *)NULL;
	}
	total += length+4L;
	return ret;
}
void *cRealloc(char *p,unsigned long r,int tag)
{
	char *pn;

	//tag=tag;
	
	if(!(pn=(char *)realloc(p,r))){
	    return (char *)NULL;
	}

	return pn;	
}
int cFree(char *ptr)
{
	if(ptr){
	    free(ptr);
	    return 0;
	}
	return 1;
}
void checkall(void)
{
	return;
}
#endif
