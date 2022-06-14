#include <stdio.h>
#include <string.h>
#include <time.h>
#include "Simple.h"
#include "Simpleop.h"
 
static struct Register{
	union{
	    int i;
	    char c;
	    long l;
	    long *lp;
	    float f;
	    float *fp;
	    double d;
	    double *dp;
	    char *p;
	    unsigned long u;
	}t;
}R0,R2,R4;
 
int doRun();
 
static int getLong();
 
static int getInt();
 
static int popd();
 
static int pushd();
 
int popl();
 
int pushl();
 

int adjust(int n);
 
static long stackMax;			/*  Run Subroutine Maximum Stack Size 	*/
unsigned char *stack;		        /*  Run Subroutine stack Location     	*/
static unsigned char *stackBuff;	/*  Run Subroutine stack Buffer	      	*/
static char *SR0;			/*  Structure Register		    	*/
 
static  unsigned char *codebuff,*databuff;
static long code;
static long data;
 
static int JumpRoutine();
 
static int _sputc();

int RunEnd();
 
int dfopen();

int _print();

int runCode(int argc,char *argv[])
{
	
	SR0=NULL;
	
	stackBuff=NULL;
	
	stackMax=4000;
	
	codebuff=g.codebuff;
	
	databuff=g.databuff;
	
	code=g.code;
	
	data=g.data;
 
	if(g.LargestStructure){
	    if(!(SR0=(char *)malloc(g.LargestStructure))){
	        printf("runCode out of memory (%ld)\n",g.LargestStructure);
	        return 1;
	    }
	}
	
	if(!(stackBuff=(unsigned char *)malloc(stackMax))){
	    printf("runCode out of memory (%ld)\n",stackMax);
	    return 1;
	}
	
	stack=stackBuff+stackMax;
	
	pushl((long)argv);
	pushl((long)argc);
	pushl((long)0xFFFFFFFF);
	
	doRun(0L);
 
	RunEnd();
	return 0;
}
int RunEnd()
{
 
	if(stackBuff){
	    free((char *)stackBuff);
	    stackBuff=NULL;
	}
	if(SR0){
	    free((char *)SR0);
	    SR0=NULL;
	}
	if(g.codebuff){
	    free((char *)g.codebuff);
	    g.codebuff=NULL;
	}
	if(g.databuff){
	    free((char *)g.databuff);
	    g.databuff=NULL;
	}
	return 0;

}
int doRun(long codein)
{
	unsigned char *stackTop;
	int (*routine)();
	long *l,li,lu,Si,tt;
	int *p,c,i;
	unsigned char *cu;
	double *d;
	float *f;
	
	stackTop=stack;
	tt=0;
	code=codein;
	while(1){ 
	    p=(int *)(codebuff+code);
	    c = *p;
	    code += sizeof(int);
	    switch(c){	
	        case NZD:
	             R0.t.l = (R0.t.d != 0.);
	            break;
	        case JUMP_DEFAULT:
	            getLong(&li);
	            code = li;
	            break;
	        case JUMP_CASE:
	            getLong(&Si);
	            getLong(&li);
	            if(Si == R0.t.l)code = li;
	            break;
	        case NOT_P0:
	             R0.t.l = !(R0.t.p);
	            break;
	        case NOT_D0:
	             R0.t.l = !(R0.t.d);
	            break;
	        case NOT_L0:
	             R0.t.l = !(R0.t.l);
	            break;
	        case UCL0:
	             R0.t.l = ~R0.t.l;
	            break;
	        case UML0:
	             R0.t.l = -R0.t.l;
	            break;
	        case UMD0:
	             R0.t.d = -R0.t.d;
	            break;
	        case LIF0:
	             R0.t.d = *(R0.t.fp);
	            break;
	        case LID0:
	             R0.t.d = *(R0.t.dp);
	            break;
	        case LIC0:
	             R0.t.l = *(R0.t.p);
	            break;
	        case LIP0:
	        case LII0:
	        case LIL0:
	             R0.t.l = *(R0.t.lp);
	            break;
	        case LIT0:
	            getLong(&Si);
	    	    memcpy((char *)SR0,(char *)R0.t.p,(long)Si);
	            break;
	        case SIT2:
	            getLong(&Si);
	    	    memcpy((char *)R2.t.p,(char *)SR0,(long)Si);
	            break;
	        case SIC2:
	            *(R2.t.p) = R4.t.c;
	            break;
	        case SIP2:
	        case SII2:
	        case SIL2:
	            *(R2.t.lp) = R4.t.l;
	            break;
	        case SIF2:
	            *(R2.t.fp)=R4.t.f;
	            break;
	        case SID2:
	            *(R2.t.dp)=R4.t.d;
	            break;
	        case POINTER_PLUS:
	            R0.t.p = (R0.t.l + R2.t.p);
	            break;
	        case LONG_MOD:
	            R0.t.l = (R2.t.l % R0.t.l);
	            break;
	        case LONG_OR:
	            R0.t.l = (R0.t.l | R2.t.l);
	            break;
	        case LONG_XOR:
	            R0.t.l = (R0.t.l ^ R2.t.l);
	            break;
	        case LONG_AND:
	            R0.t.l = (R0.t.l & R2.t.l);
	            break;
	        case LONG_LEFT:
	            R0.t.l = (R2.t.l << R0.t.l);
	            break;
	        case LONG_RIGHT:
	            R0.t.l =  (R2.t.l >> R0.t.l);
	            break;
	        case LONG_PLUS:
	            R0.t.l = (R0.t.l + R2.t.l);
	            break;
	        case POINTER_MINUS: 
	            R0.t.l= (R2.t.p - R0.t.p);
	            break;
	        case LONG_MINUS:
	            R0.t.l = (R2.t.l - R0.t.l);
	            break;
	        case LONG_MULT:
	            R0.t.l = (R0.t.l * R2.t.l);
	            break;
	        case LONG_DIV:
	            R0.t.l = (R2.t.l / R0.t.l);
	            break;
	        case POINTER_LESS:
	            R0.t.l = (R2.t.p < R0.t.p);
	            break;
	        case LONG_LESS:
	            R0.t.l = (R2.t.l < R0.t.l);
	            break;
	        case POINTER_GREATER:
	            R0.t.l = (R2.t.p > R0.t.p);
	            break;
	        case LONG_GREATER:
	            R0.t.l = (R2.t.l > R0.t.l);
	            break;
	        case POINTER_NE:
	            R0.t.l = (R0.t.p != R2.t.p);
	            break;
	        case LONG_NE:
	            R0.t.l = (R0.t.l != R2.t.l);
	            break;
	        case POINTER_EQ:
	            R0.t.l = (R0.t.p == R2.t.p);
	            break;
	        case LONG_EQ:
	            R0.t.l = (R0.t.l == R2.t.l);
	            break;
	        case POINTER_LE:
	            R0.t.l  = (R2.t.p <= R0.t.p);
	            break;
	        case LONG_LE:
	            R0.t.l  = (R2.t.l <= R0.t.l);
	            break;
	        case POINTER_GE:
	            R0.t.l = (R2.t.p >= R0.t.p);
	            break;
	        case LONG_GE:
	            R0.t.l = (R2.t.l >= R0.t.l);
	            break;
	        case CVTLC4:
	            R4.t.c=R0.t.l;
	            break;
	        case CVTCL4:
	            R4.t.l=R0.t.c;
	            break;
	        case CVTDF4:
	            R4.t.f=R0.t.d;
	            break;
	        case CVTDD4:
	            R4.t.d=R0.t.d;
	            break;
	        case CVTLF4:
	            R4.t.f=R0.t.l;
	            break;
	        case CVTLD4:
	            R4.t.d=R0.t.l;
	            break;
	        case CVTDL4:
	            R4.t.l=R0.t.d;
	            break;
	        case CVTPL4:
	        case CVTIL4:
	        case CVTLL4:
	            R4.t.l=R0.t.l;
	            break;
	        case CVTLD0:
	            R0.t.d=R0.t.l;
	            break;	            
	        case CVTLD2:
	            R2.t.d=R2.t.l;
	            break;	            
	        case PUSHD:
	            pushd(R0.t.d);
	            break;
	        case PUSHP:
	        case PUSHL:
	            pushl(R0.t.l);
	            break;
	        case POPD:
		    popd(&R2.t.d);
	            break;
	        case POPP:
	        case POPL:
		    popl(&R2.t.l);
	            break;
	        case MSP:
	            getLong(&li);
	    	    c = -(li);
	    	    adjust(c);
	            break;
	        case FE:
	            getLong(&li);
	            c=li;
	    	    adjust(c);
	            break;	            
	        case ACP:
	            getLong(&li);
	    	    R0.t.p += li;
	            break;
	        case PSCALE2:
	            getLong(&li);
	    	    R2.t.l *= li;
	            break;
	        case PDIVIDE0:
	            getLong(&li);
	    	    R0.t.l /= li;
	            break;
	        case PSCALE0:
	            getLong(&li);
	    	    R0.t.l *= li;
	            break;
	        case LSF0:
	            getLong(&li);
	    	    f=(float *)(stack+li);
	    	    R0.t.d = *f;
	            break;
	        case LSD0:
	            getLong(&li);
	    	    d=(double *)(stack+li);
	    	    R0.t.d = *d;
	            break;
	        case LST0:
	            getLong(&Si);
	            getLong(&li);
	    	    cu=(unsigned char *)(stack+li);
	    	    R0.t.p = (char *)cu;
	    	    memcpy(SR0,R0.t.p,(long)Si);
	            break;
	        case LSP0:
	        case LSI0:
	        case LSL0:
	            getLong(&li);
	    	    l=(long *)(stack+li);
	    	    R0.t.l = *l;
	            break;
	        case LSA0:
	            getLong(&li);
	    	    cu=(unsigned char *)(stack+li);
	    	    R0.t.p = (char *)cu;
	            break;
	        case LEA0:
	            getLong(&li);
	    	    cu=(unsigned char *)(databuff+li);
	    	    R0.t.p = (char *)cu;
	            break;
	        case LET0:
	            getLong(&Si);
	            getLong(&li);
	    	    cu=(unsigned char *)(databuff+li);
	    	    R0.t.p = (char *)cu;
	    	    memcpy(SR0,R0.t.p,(long)Si);
	            break;
	        case LEP0:
	        case LEI0:
	        case LEL0:
	            getLong(&li);
	    	    l=(long *)(databuff+li);
	    	    R0.t.l = *l;
	            break;
	        case LEF0:
	            getLong(&li);
	    	    f=(float *)(databuff+li);
	    	    R0.t.d = *f;
	            break;
	        case LED0:
	            getLong(&li);
	    	    d=(double *)(databuff+li);
	    	    R0.t.d = *d;
	            break;
	        case LEB0:
	            getLong(&li);
	    	    cu=(unsigned char *)(databuff+li);
	    	    R0.t.l = *cu;
	            break;
	        case DOUBLE_MOD:
		    R0.t.l=R0.t.d;
		    R2.t.l=R2.t.d;
	            R0.t.d = (R0.t.l % R2.t.l);
	            break;
	        case DOUBLE_OR:
		    R0.t.l=R0.t.d;
		    R2.t.l=R2.t.d;
	            R0.t.d = (R0.t.l | R2.t.l);
	            break;
	        case DOUBLE_XOR:
		    R0.t.l=R0.t.d;
		    R2.t.l=R2.t.d;
	            R0.t.d = (R0.t.l ^ R2.t.l);
	            break;
	        case DOUBLE_AND:
		    R0.t.l=R0.t.d;
		    R2.t.l=R2.t.d;
	            R0.t.d = (R0.t.l & R2.t.l);
	            break;
	        case DOUBLE_LEFT:
		    R0.t.l=R0.t.d;
		    R2.t.l=R2.t.d;
	            R0.t.d = (R2.t.l << R0.t.l);
	            break;
	        case DOUBLE_RIGHT:
		    R0.t.l=R0.t.d;
		    R2.t.l=R2.t.d;
	            R0.t.d =  (R2.t.l >> R0.t.l);
	            break;
	        case DOUBLE_PLUS:
	            R0.t.d = (R0.t.d + R2.t.d);
	            break;
	        case DOUBLE_MINUS:
	            R0.t.d = (R2.t.d - R0.t.d);
	            break;
	        case DOUBLE_MULT:
	            R0.t.d = (R0.t.d * R2.t.d);
	            break;
	        case DOUBLE_DIV:
	            R0.t.d = (R2.t.d / R0.t.d);
	            break;
	        case DOUBLE_LESS:
	            R0.t.l = (R2.t.d < R0.t.d);
	            break;
	        case DOUBLE_GREATER:
	            R0.t.l = (R2.t.d > R0.t.d);
	            break;
	        case DOUBLE_NE:
	            R0.t.l = (R0.t.d != R2.t.d);
	            break;
	        case DOUBLE_EQ:
	            R0.t.l = (R0.t.d == R2.t.d);
	            break;
	        case DOUBLE_LE:
	            R0.t.l  = (R2.t.d <= R0.t.d);
	            break;
	        case DOUBLE_GE:
	            R0.t.l = (R2.t.d >= R0.t.d);
	            break;
	        case JP:
	            getLong(&li);
		    code = li;
	            break;
	        case JF:
	            getLong(&li);
		    if(!R0.t.l)code = li;
	            break;
	        case JT:
	            getLong(&li);
		    if(R0.t.l)code = li;
	            break;
	        case CF:
	            getLong(&li);
	            getInt(&i);
	            pushl(code);
	            if(i < 0){
	               routine = (int (*)())li;
	               JumpRoutine(routine,i);
	               l =(long *)stack;
	               code = *l;
	               adjust(sizeof(long));
	            } else if(i == 0){
	               routine = (int (*)())li;
	               (*routine)();
	               adjust(sizeof(long));
	            }else{
	               code = li;
	            }
	            break;	
	        case RET:
	            if(stack == stackTop){
	                return 0;
	            }else{
	                l =(long *)stack;
	                code = *l;
	                adjust(sizeof(long));
	            }
	            break;	
		default:
		    printf("Unknown Code = %d\n",c);
		    return 1;
	   }
	}
	return 0; 
}
static int JumpRoutine(routine,i)
int (*routine)();
{
	char **p1;
	int ret;
	
	i = -i;
	p1=(char **)(stack+sizeof(long));
	switch(i){
	case 1:
	    ret=(*routine)(*p1);
	    break;
	case 2:
	    ret=(*routine)(*p1,*(p1+1));
	    break;
	case 3:
	    ret=(*routine)(*p1,*(p1+1),*(p1+2));
	    break;
	case 4:
	    ret=(*routine)(*p1,*(p1+1),*(p1+2),*(p1+3));
	    break;
	case 5:
	    ret=(*routine)(*p1,*(p1+1),*(p1+2),*(p1+3),*(p1+4));
	    break;
	case 6:
	    ret=(*routine)(*p1,*(p1+1),*(p1+2),*(p1+3),*(p1+4),*(p1+5));
	    break;
	case 7:
	    ret=(*routine)(*p1,*(p1+1),*(p1+2),*(p1+3),*(p1+4),*(p1+5),*(p1+6));
	    break;
	case 8:
	    ret=(*routine)(*p1,*(p1+1),*(p1+2),*(p1+3),*(p1+4),*(p1+5),*(p1+6),*(p1+7));
	    break;
	case 9:
	    ret=(*routine)(*p1,*(p1+1),*(p1+2),*(p1+3),*(p1+4),*(p1+5),*(p1+6),*(p1+7),*(p1+8));
	    break;
	default:
	     printf("Warning Only 9 Parameters May Be Passed To User Routines\n");
	    ret=(*routine)(*p1,*(p1+1),*(p1+2),*(p1+3),*(p1+4),*(p1+5),*(p1+6),*(p1+7),*(p1+8));
	    break;
	}
	R0.t.l=ret;
	return 0;
}
static int getLong(li)
long *li;
{
	long *l;
	
	l=(long *)(codebuff+code);
	code += sizeof(long);
	*li = *l;
	return 0;
}
static int getInt(i)
int *i;
{
	int *l;
	
	l=(int *)(codebuff+code);
	code += sizeof(int);
	*i = *l;
	return 0;
}
static int popd(d)
double *d;
{
	double *pl;
	
	pl=(double *)stack;
	*d = *pl;
	stack += sizeof(double);
	if(stack > stackBuff + stackMax){
	    printf("popd stack Underflow\n");
	    longjmp(env77,1);
	}
 
	return 0;
}
static int pushd(d)
double d;
{
	double *pl;
	
	stack -= sizeof(double);
	if(stack < stackBuff){
	    printf("pushd stack Overflow\n");
	    longjmp(env77,1);
	}
	pl=(double *)stack;
	*pl=d;
 
	return 0;
}
int popl(l)
long *l;
{
	long *pl;
	
	pl=(long *)stack;
	*l = *pl;
	stack += sizeof(long);
	if(stack > stackBuff + stackMax){
	    printf("popl stack Underflow\n");
	    longjmp(env77,1);
	}
 
	return 0;
}
int pushl(long code)
{
	long *pl;
	
	stack -= sizeof(long);
	if(stack < stackBuff){
	    printf("pushl stack Overflow\n");
	    longjmp(env77,1);
	}
	pl=(long *)stack;
	*pl=code;
 
	return 0;
}
int adjust(int n)
{
	stack += n;
	if(stack > stackBuff + stackMax){
	    printf("adjust stack UnderFlow");
	    longjmp(env77,1);
	} else if(stack < stackBuff){
	    printf("adjust stack Overflow\n");
	    longjmp(env77,1);
	}
	return 0;
}
int dsqrt()
{
	double *d,sqrt();
	
	d=(double *)(stack+sizeof(long));
	R0.t.d=sqrt(*d);
	return 0;
}
int datan()
{
	double *d,atan();
	
	d=(double *)(stack+sizeof(long));
	R0.t.d=atan(*d);
	return 0;
}
int datan2()
{
	double *d1,*d2,atan2();
	
	d1=(double *)(stack+sizeof(long));
	d2=d1+1;

	R0.t.d=atan2(*d1,*d2);
	return 0;
}
int dfmod()
{
	double *d1,*d2,fmod();
	
	d1=(double *)(stack+sizeof(long));
	d2=d1+1;

	R0.t.d=fmod(*d1,*d2);
	return 0;
}
int dsin()
{
	double *d,sin();
	
	d=(double *)(stack+sizeof(long));
	R0.t.d=sin(*d);
	return 0;
}
int dcos()
{
	double *d,cos();
	
	d=(double *)(stack+sizeof(long));
	R0.t.d=cos(*d);
	return 0;
}
int dfabs()
{
	double *d,fabs();
	
	d=(double *)(stack+sizeof(long));
	R0.t.d=fabs(*d);
	return 0;
}
int dfopen()
{
	FILE *file;
	
	char *a1,*a2,**p;
	
	p=(char **)(stack+sizeof(long));
	a1=*p;
	p=(char **)(stack+2*sizeof(long));
	a2=*p;
	file=fopen(a1,a2);
	R0.t.p=(char *)file;
	return 0;
}
int dsecond()
{
	
	long **p;
	
	p=(long **)(stack+sizeof(long));
	R0.t.l=time(*p);
	return 0;
}
int dmalloc()
{
	
	char *a1,**p;
	
	p=(char **)(stack+sizeof(long));
	a1=*p;
	R0.t.p=(char *)malloc((long)a1);
	return 0;
}
int dfree()
{
	char **p,*f;
	int ret;
	
	p=(char **)(stack+sizeof(long));
	f=*p;
	if(f){
	    free(f);
	    ret=0;
	}else{
	    ret=0;
	}
	R0.t.l=ret;
	return 0;
}
int dfclose()
{
	FILE *file;
	char **p;
	int ret;
	
	p=(char **)(stack+sizeof(long));
	file=(FILE *)*p;
	ret=fclose(file);
	R0.t.l=ret;
	return 0;
}
int dfprintf()
{
	int fputc();
	char **p;
		
	p=(char **)(stack+sizeof(long));
	_print((char *)(*p),stack+2*sizeof(long),fputc);
 
	return 0;
}
int dsprintf()
{
	char *a,**p;
	
	p=(char **)(stack+sizeof(long));
	a=*p;
	_print((char *)&a,stack+2*sizeof(long),_sputc);
	*a='\0';
	return 0;
 
}
static int _sputc(c,a)
int c;
char **a;
{
	*(*a)++=c;
	return 0;
}
int dprintf()
{
	int fputc();
 
	_print((char *)stdout,stack+sizeof(long),fputc);
	return 0;
}
int dputchar()
{
	long *l;
	int c,r;
	
	l=(long *)(stack+sizeof(long));
	c= *l;
	r=putchar(c);
	R0.t.l=r;
	return 0;
}
int dexit()
{
	exit(1);
	return 0;
}

int dgetchar()
{
	int c;
	
	c=getchar();
	R0.t.l=c;
	return 0;
}
 
 
