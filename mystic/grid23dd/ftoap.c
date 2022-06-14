#include <math.h>
#include "NewNames.h"
static double pow10();
static int itoa(),itoa2(),fracout(),n10;
static char *itoc;
int ftoap(double d,char *sout,int eps)
{
  
	double frexp(),ldexp(),log10(),e,fabs();
	static double log2=.301029995663981;
	int exp;
	 
	if(eps > 15)eps=15;
	
	if(d < 0.){
	    *sout++='-';
	    d = -d;
	}
	
	e=frexp(d,&exp);
	n10=exp*log2+.5;
	d=d/pow10(n10);
	if(d){
		while(d < .1){
		    --n10;
		    d *= 10;
		}
		while(d > 1.){
		    ++n10;
		    d /= 10;
		}
	}
	itoc=sout;
	fracout(d,eps);
	if(n10){
	    *itoc++='e';
	    itoa(n10,itoc);
	}
	*itoc='\0';
	return 0;
}
static int fracout(u,epss)
double u;int epss;
{
	double B=10.,eps;
	int i,U[20],m,*n;
 
	eps=pow10(-(epss+1));
	m=0;
	n=U;
	for(i=0;i < 20;++i)*n++=0;
	if(u < eps || u > 1.-eps)goto fini;
start:
	i=B*u+eps;
	U[++m]=i;
	u=B*u-i;
	eps=B*eps;
	if(m <= epss)goto start;
fini:
	if(u >= .5)++U[m];
	if(U[0]){
	    *itoc++=U[0]+'0';
	} else {
	    if(U[1]){
		*itoc++=U[1]+'0';
		--n10;
	    } else {
		n10=0;
		*itoc++='0';
		*itoc++='.';
		return 0;
	    }
	}
	*itoc++='.';
	for(i=2;i <= 16; ++i){
		*itoc++=U[i]+'0';
	}	
	return 0;
}
static int itoa(n,c)
int n;
char *c;
{
	itoc=c;
	itoa2(n);
	*itoc='\0';
	return 0;
}
static int itoa2(n)
int n;
{
	int i;
 
	if(n < 0){
	   *itoc++='-';
	    n=-n;
	}
	if((i = n/10))
	    itoa2(i);
	*itoc++=n % 10 + '0';
	return 0;
}
 
static double pow10(n)
int n;
{
	double a;
 
	a=1.;
 
	if(n < 0){
	   n=-n;
	   while(n--)a/=10.;
	} else {
	    while(n--)a*=10.;
	}
	return a;
}


