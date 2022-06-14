#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "NewNames.h"
#define BIT32
#ifdef BIT32
#define MASK 0x7fffffff		/* 32 bits */
#else
#define MASK 0x7fff		/* 16 bits */
#endif
static int itoa(),itoab(),reverse();
static char *fftype();


int ftoap(double d,char *sout,int eps);

int _print(int *fd,int **nxtarg,int (*iputc)())
{
	int left,pad,cc,len,maxchr,width,ftype;
	char *ctl,*sptr,strd[8],str[50],f[99],e[99],*args;
	double	d;
	long arg;
 
	cc=0;
	ctl=(char *)*nxtarg++;
	while(*ctl){
	    if(*ctl != '%'){(*iputc)(*ctl++,fd);++cc;continue;}
	    else ++ctl;
	    if(*ctl == '%'){(*iputc)(*ctl++,fd);++cc;continue;}
	    if(*ctl == '-'){left = 1;++ctl;} else left =0;
	    if(*ctl == '0') pad = '0'; else pad = ' ';
	    if(isdigit(*ctl)){
		width=atoi(ctl++);
		while(isdigit(*ctl))++ctl;
	    } else if(*ctl == '*'){
		ctl++;
		width=(int)*nxtarg++;
	    }	
	    else width=0;
	    if(*ctl == '.'){
		ctl++;
		if(*ctl == '*'){
		    ctl++;
		    maxchr=(int)*nxtarg++;
		} else {
		    maxchr=atoi(ctl);
		    while(isdigit(*ctl))++ctl;
		}
	    }	
	    else maxchr=-1;
	    args=(char *)*nxtarg;
	    arg=(long)*nxtarg++;
	    sptr=str;
	    if(*ctl == 'l' || *ctl == 'L')++ctl;
	    switch(*ctl++){
		case 'c':str[0]=arg;str[1]=0;break;
		case 's':sptr=(char *)args;break;
		case 'd':itoa(arg,str);break;
		case 'b':itoab(arg,str,2);break;
		case 'o':itoab(arg,str,8);break;
		case 'u':itoab(arg,str,10);break;
		case 'x':itoab(arg,str,16);break;
		case 'f':ftype='f';
			goto flabel;
		case 'g':ftype='g';
			goto flabel;
		case 'e': ftype='e';
flabel:
		   --nxtarg;
		   if(maxchr < 0)maxchr=6;
		   d=*((double *)nxtarg);
		   ftoap(d,str,maxchr);
		   sptr=str+strlen(str);
		   while(sptr < str+50)*sptr++='\0';
		   args=(char *)nxtarg;
		   args += sizeof(double);
		   nxtarg=(int **)args;
		   sptr=fftype(str,ftype,maxchr,e,f,d);
		   maxchr=0; 
		   break;
 
		default:return(cc);
	    }
	    len=strlen(sptr);
	    if(maxchr > 0 && maxchr<len)len=maxchr;
	    if(width>len)width=width-len;else width=0;
	    if(!left)while(width--){(*iputc)(pad,fd);++cc;}
	    while(len--){(*iputc)(*sptr++,fd);++cc;}
	    if(left)while(width--){(*iputc)(pad,fd);++cc;};
	}
	return(cc);
}
static char *fftype(str,ftype,n,f,e,d)
char *str;
int ftype,n;
char *f,*e;
double d;
{
	char *cp,*sp;
	int c,count,sign,k;
 
	k=n;
 
 
 
	if(ftype != 'f'){	/*	do 'e' format	*/
	    sp=str;
	    cp=e;
	    count=k;	
	    while((c=*sp++) && c != '.')*cp++=c;
	    if(k > 0)*cp++='.';
	    while((c=*sp++) && c != 'e'){	
		if(--count >=0)*cp++=c;
	    }
	    while(--count >= 0)*cp++='0';		
	    *cp++='e';
	    if(!*sp)*cp++='0';
	    while((*cp++=*sp++));			 	
	}
	if(ftype != 'e'){	/*	do 'f' format	*/
	    sp=str;
	    count=0;
	    cp=f;
	    while((c=*sp++) && c != 'e'){
		*cp++=c;
	    }
	    while(cp < f+99)*cp++='0';
	    sign=1;
     	    if(*sp == '-'){
		sign=-1;
		++sp;
	    }
	    while((c=*sp++)){
		count=10*count+c-'0';
	    }
	/*
	    if(sign == 1)
		ftoap(d,str,n+count);
	    else 
		ftoap(d,str,n-count);
	*/
		ftoap(d,str,n);
	    sp=str;
	    cp=f;
	    while((c=*sp++) && c != 'e'){
		*cp++=c;
	    }
	    while(cp < f+99)*cp++='0';
 
	    sp=f;
	    while(*sp++ != '.');
	    --sp;
	    if(count){
		if(sign > 0){
		    while(--count >= 0){
			*sp=*(sp+1);
			++sp;
		    }
		    if(k > 0)*sp='.';
		} else {
		    *sp=*(sp-1);
		    *(sp-1)='.';
		    cp=sp--;
		    if(count >= k+1){
			while(--count >= 0)*cp++='0';
		    } else {
			if(--count > 0){
			    cp=f+(98-count);
			    while(cp > sp){
				*(cp+count)=*cp;
				--cp;
			    }
			    while(--count >=0)*(++cp)='0';
			}
		    }	
		}
	    }
	    sp+=k+1;
	    *sp='\0';
	}
	switch(ftype){
	case 'f':
	    return f;
	case 'e':
	    return e;
	case 'g':
	    if(k > 0){
		while(*--sp == '0')*sp='\0';
		if(*sp == '.'){
		    if(isdigit(*(sp-1))){
			*sp='\0';
		    } else {
			*(sp+1)='0';
		    }	
		}
	    } else if(*(--sp) == '.'){
		if(isdigit(*(sp-1))){
		    *sp='\0';
		} else {
		    *sp='0';
		}
	    }
	    if(strlen(f) < strlen(e)){
		return f;
	    } else {
		return e;
	    }
	}
	return 0;
}
static int itoa(n,s)
long n;
char *s;
{
	long sign;
	char *ptr;
 
	ptr=s;
 
	if((sign=n) < 0)n=-n;
	do {
	    *ptr++=n % 10 + '0';
	} while((n=n/10) > 0);
	if(sign <0)*ptr++ = '-';
	*ptr='\0';
	reverse(s);
	return 0;
}
static int itoab(n,s,b)
long n;
char *s;
int b;
{
	char *ptr;
	long lowbit;
 
	ptr=s;
	b=b>>1;
	do {
	    lowbit=n & 1;
	    n=(n >> 1) & MASK;
	    *ptr=((n % b) << 1) + lowbit;
	    if(*ptr < 10)*ptr+='0';else *ptr+=55;
	    ++ptr;
	} while(n/=b);
	*ptr='\0';
	reverse(s);
	return 0;
}
static int reverse(s)
char *s;
{
	char *j;
	int c;
	
	j=s+strlen(s)-1;
	while(s < j){
	    c=*s;
	    *s++=*j;
	    *j--=c;
	}
	return 0;
}
 
