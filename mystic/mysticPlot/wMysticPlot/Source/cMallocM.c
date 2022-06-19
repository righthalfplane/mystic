#include <stdio.h>
#include <stdlib.h>
#include "Xappl.h"
#include "Tools.h"

extern char WarningBuff[256];
INT8_64 TotalUsedMaximun;
/*
#define DEBUG 1
*/

/*
#define DEBUG 2
*/

void checkall();
int Warning(char *message);
int WarningBatch(char *message);


#ifndef DEBUG
void *cMalloc(unsigned long length,int tag)
{
	void *ret;
	
	tag=tag;
	ret=malloc(length);
	if(ret == NULL){
	    sprintf(WarningBuff,"cMalloc (%ld)\n",(long)length);
	    WarningBatch(WarningBuff);
	    return NULL;
	}
	return ret;
}
void *cRealloc(char *p,unsigned long r,int tag)
{
	char *pn;

	tag=tag;
	
	if(!(pn=(char *)realloc(p,r))){
	    return (char *)NULL;
	}

	return pn;	
}
int cFree(char *ptr)
{
	if(ptr)free(ptr);
	return 0;
}
void checkall()
{
	return;
}
#endif

#ifdef DEBUG
#define END_OF_MEMORY 90000
static void  *given[END_OF_MEMORY];
static int tagm[END_OF_MEMORY];
static int errorprint[END_OF_MEMORY];
static long givenlength[END_OF_MEMORY];
static int maxgiven;
static long memcount;

void checkall()
{
	int k,count;
	
	count=0;
	for(k=0;k<maxgiven;++k){
	    if(given[k]){
		    sprintf(WarningBuff,"checkall found %ld %lx tag %d\n",(long)given[k],(unsigned long)given[k],tagm[k]);
		    Warning(WarningBuff);
	        ++count;
	    }
	}
	if(count){
	    sprintf(WarningBuff,"checkall not free %d\n",count);
	    Warning(WarningBuff);
	}else{
	    maxgiven=0;
	}
}



#endif

#if defined(DEBUG) && DEBUG == 2

void CheckOverWrite(char *where);

void CheckOverWrite(char *where)
{
	unsigned char *add;
	unsigned char *give;
	long length;
	long *fill;
	int k;
	int n;
	
	for(k=0;k<maxgiven;++k){
	    if(given[k]){
	    	give=(unsigned char *)given[k];	
			fill=(long *)(give-2*sizeof(long));
			if((fill[0] != -1L || fill[1] != -1L) && errorprint[k] == 0){
				sprintf(WarningBuff,"%s Memory Over  Write Before k %d tag %d\n",where,k,tagm[k]);
				Warning(WarningBuff);	    
				errorprint[k] = 1;
			}	    
    		length=givenlength[k];
		    for(n=0;n<2*sizeof(long);++n){
		        add=give+length+n;
		        if(*(add) != 255 && errorprint[k] == 0){
					sprintf(WarningBuff,"%s Memory Over Write After k %d tag %d\n",where,k,tagm[k]);
					Warning(WarningBuff);	    
					errorprint[k] = 1;
					break;		        		        
		        }
		    }
	    
	    }
	}
}
void *cMalloc(unsigned long r,int tag)
{
	void *giver;
	unsigned char *give;
	unsigned char *add;
	long *fill,length;
	int k;
	int n;
	
	CheckOverWrite("cMalloc");
	
	for(k=0;k<maxgiven;++k){
	    if(!given[k])goto empty;
	}
	
	if(++maxgiven >= END_OF_MEMORY){
	       sprintf(WarningBuff,"cMalloc Debug out of table\n");
	       Warning(WarningBuff);
	       exit(1);
	}
	k=maxgiven-1;
empty:

	length = r;
	
	r += 4*sizeof(long);
	
	if(!(giver=(void *)malloc(r))){
	       sprintf(WarningBuff,"cMalloc Debug out of Memory maxgiven %ld memcount %ld\n",maxgiven,memcount);
	       Warning(WarningBuff);
	       exit(1);
	}

	give=(unsigned char *)giver;
	
	fill=(long *)give;
	
	fill[0] = -1L;
	fill[1] = -1L;
	
    memcount += r;
    
    givenlength[k]=length;
    
    for(n=0;n<2*sizeof(long);++n){
        add=give+2*sizeof(long)+length+n;
        *(add) = 255;
    }
    
	given[k]=give+2*sizeof(long);
	tagm[k]=tag;
	return give+2*sizeof(long);
}

void *cRealloc(char *p,unsigned long r,int tag)
{
	void *giver;
	char *give;
	
    unsigned long n,kl;
	long k;

	if(!p)return NULL;
	
	CheckOverWrite("cRealloc");
			
	for(k=0;k<maxgiven;++k){
	    if(given[k] == p)goto found;
	}
	
	sprintf(WarningBuff," Error Tried to cRealloc %ld\n",p);
	Warning(WarningBuff);
	return 0;
found:

	give=p-2*sizeof(long);
	
	n=givenlength[k];

	if(!(giver=(void *)cMalloc(r,tag))){
		sprintf(WarningBuff,"cRealloc out of Memory\n");
		Warning(WarningBuff);
	    return NULL;
	}
	
	give=(char *)giver;

	for(kl=0;kl<r;++kl){
	    if(kl < n){
 	        give[kl]=p[kl];
	    }else{
 	        give[kl]=0;
	    }
	}

	cFree((char *)p);


	return giver;
	
}

int cFree(char *p)
{
	char *give;
	int k;

	CheckOverWrite("cFree");
	
	for(k=0;k<maxgiven;++k){
	    if(given[k] == p)goto found;
	}
	sprintf(WarningBuff,"Error Tried to free %lx %ld\n",(unsigned long)p,(long)p);
	Warning(WarningBuff);
	return 1;
found:

	give=p-2*sizeof(long);
	
	free(give);
	given[k]=NULL;
	if(k == maxgiven-1){
	    --maxgiven;
	    if(maxgiven < 0 )maxgiven=0;
	}	
	return 0;
}
#elif defined(DEBUG)
void *cMalloc(unsigned long r,int tag)
{
	void *give;
	int k;
	
	for(k=0;k<maxgiven;++k){
	    if(!given[k])goto empty;
	}
	
	if(++maxgiven >= END_OF_MEMORY){
	       sprintf(WarningBuff,"cMalloc Debug out of table\n");
	       Warning(WarningBuff);
	       exit(1);
	}
	k=maxgiven-1;
empty:

	if(!(give=(void *)NewPtr(r))){
	       sprintf(WarningBuff,"cMalloc Debug out of Memory maxgiven %ld memcount %ld\n",maxgiven,memcount);
	       Warning(WarningBuff);
	       exit(1);
	}
    memcount += r;
	given[k]=give;
	tagm[k]=tag;
    givenlength[k]=r;
	return give;
}

void *cRealloc(char *p,unsigned long r,int tag)
{
	void *giver;
	char *give;
	
    unsigned long n,k,kl;

	if(!p)return NULL;
	
	for(k=0;k<maxgiven;++k){
	    if(given[k] == p)goto found;
	}
	sprintf(WarningBuff,"Error Tried to realloc %ld\n",p);
	Warning(WarningBuff);
	return 0;
found:

	n=givenlength[k];

	if(!(giver=(void *)cMalloc(r,tag))){
		sprintf(WarningBuff,"cRealloc out of Memory\n");
		Warning(WarningBuff);
	    return NULL;
	}
	
	give=(char *)giver;

	for(kl=0;kl<r;++kl){
	    if(kl < n){
 	        give[kl]=p[kl];
	    }else{
 	        give[kl]=0;
	    }
	}

	cFree((char *)p);


	return giver;
	
}

int cFree(char *p)
{
	int k;
	
	for(k=0;k<maxgiven;++k){
	    if(given[k] == p)goto found;
	}
	sprintf(WarningBuff,"Error Tried to free %lx %ld\n",(unsigned long)p,(long)p);
	Warning(WarningBuff);
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
#endif

int zerol(char *s,unsigned long n)
{
    if(!s || (n <= 0))return 1;

#if DEBUG == 2
	CheckOverWrite("zerol1");
#endif

	while(n-- > 0)*s++ = 0;
	
#if DEBUG == 2
	CheckOverWrite("zerol2");
#endif
	return 0;
}

