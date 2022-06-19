#define EXTERN22 extern
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "Xappl.h"
#include "GWorld.h"
#include "CExpress.h"
#include "NewEdit.h"
#include "Linedata.h"
#include "Message.h"
#include "FileManager.h"

int doFFT3D(double *x,double *y,long xsize,long ysize,long zsize,int direction,int filter);
static int isTrue(struct stack *LD);
int putENDOFLINE(FILE *out);
IconPtr FindWindowByName(char *name);
int WriteToWindowFast(char *name,char *buff);
int WriteToWindowAtEnd(char *name,char *buff);
int MakeNewDirectory(char *name);

int CExpressFreeStack(struct stackHolder *s);

static int setE(CExpressPtr e,CExpressPtr en);
int CExpressNext(CExpressPtr e);
static int CExpressStatement(CExpressPtr e);
static int CExpressExpression(CExpressPtr e);
static int CExpressIf(CExpressPtr e);
static int CExpressFor(CExpressPtr e);
static int CExpressCurly(CExpressPtr e);
static int CExpressSkip(CExpressPtr e);
static int CExpressOR(CExpressPtr e);
static int CExpressAND(CExpressPtr e);
static int CExpressLogicalLOR(CExpressPtr e);
static int CExpressLogicalLAND(CExpressPtr e);
static int CExpressXOR(CExpressPtr e);
static int CExpressEQNOTEQ(CExpressPtr e);
static int CExpressLESSGREAT(CExpressPtr e);
static int CExpressSHIFT(CExpressPtr e);
static int CExpressPLUSMINUS(CExpressPtr e);
static int CExpressMULTDIVMOD(CExpressPtr e);
static int CExpressUNARY(CExpressPtr e);
static int CExpressPRIMARY(CExpressPtr e);
static int CExpressFUNCTION(CExpressPtr e);

static int CExpressDoFUNCTION(CExpressPtr e,int function,int count);
static int CExpressDoUnaryIncrnemt(CExpressPtr e,int tok,int flag);
static int CExpressDoUNARY(CExpressPtr e,int function);
static int CExpressDoBINARY(CExpressPtr e,int function);
static int CExpressDoEqual(CExpressPtr e,int function);

static int CExpressFreeNameStack(CExpressPtr e,char *name);
static struct stack *CExpressGetLogicalStack(CExpressPtr e,int location);
static struct stack *CExpressCreateNameStack(CExpressPtr e,char *name,int type);

static int CExpressPopConstantList(CExpressPtr e,double *xx,long n,char *message);

static int LinePlot(CExpressPtr e,int count);

int doReturn(int value);


static double doT_LOR(double r1,double r2);
static double doT_LAND(double r1,double r2);
static double doT_OR(double r1,double r2);
static double doT_AND(double r1,double r2);
static double doT_LESS(double r1,double r2);
static double doT_LEFTSHIFT(double r1,double r2);
static double doT_GREAT(double r1,double r2);
static double doT_RIGHTSHIFT(double r1,double r2);

static double doT_PLUS(double r1,double r2);
static double doT_MINUS(double r1,double r2);
static double doT_XOR(double r1,double r2);
static double doT_MULT(double r1,double r2);
static double doT_DIV(double r1,double r2);
static double doT_MOD(double r1,double r2);

static double doT_EQUALEQUAL(double r1,double r2);
static double doT_NOTEQUAL(double r1,double r2);
static double doT_LESSEQUAL(double r1,double r2);
static double doT_GREATEQUAL(double r1,double r2);

static double doT_NOT(double r1);
static double doT_NEGATE(double r1);
static double doT_MINUSUNARY(double r1);
static int CExpressDoIf(CExpressPtr e,int flag);






static double (*funOp[])(double r1,double r2)={
								NULL,
								doT_LOR,
								doT_LAND,
								doT_OR,
								doT_AND,								
								doT_EQUALEQUAL,
	(double (*)(double, double))doT_NOT,
								doT_NOTEQUAL,
								doT_LESSEQUAL,
								doT_LESS,
								doT_LEFTSHIFT,
								doT_GREAT,
								doT_GREATEQUAL,
								doT_RIGHTSHIFT,
								doT_PLUS,
								doT_MINUS,
	(double (*)(double, double))doT_MINUSUNARY,
								doT_XOR,
								doT_MULT,
								doT_DIV,
								doT_MOD,
	(double (*)(double, double))doT_NEGATE,
																
								};
							   
							   

struct function{
	char *name;
	int classf;
	double (*fun)(double r1);
	int pcount;
	int type;
	char *form;
	int popflag;
};

static int print(CExpressPtr e);
static int fprint(CExpressPtr e);
static double max2(struct stack *s);
static double min2(struct stack *s);
static double sum(struct stack *s);
static double atanEXP(double);
static double acosEXP(double);
static double asinEXP(double);
static double atan2EXP(double,double);
static double ceilEXP(double);
static double coshEXP(double);
static double cosEXP(double);
static double expEXP(double);
static double fabsEXP(double);
static double floorEXP(double);
static double fmodEXP(double,double);
static double logEXP(double);
static double log10EXP(double);
static double powEXP(double,double);
static double sinEXP(double);
static double sinhEXP(double);
static double sqrtEXP(double);
static double tanEXP(double);
static double tanhEXP(double);
static double atanEXP(double);

static struct function functions[]={
							{"if",T_IF,NULL,0,0,NULL,1},
							{"else",T_ELSE,NULL,0,0,NULL,1},
							{"for",T_FOR,NULL,0,0,NULL,1},
							{"acos",T_FUNCTION_NAME,acosEXP,1,0,"acos(a|d)",1},
							{"alert",T_FUNCTION_NAME,NULL,2,11,"alert(s,d)",1},
							{"areaplotset",T_FUNCTION_NAME,NULL,10,20,"areaplotset(s,x1,y1,z1,x2,y2,z2,n1,n2,n3)",-1},
							{"asin",T_FUNCTION_NAME,asinEXP,1,0,"asin(a|d)",1},
							{"atan",T_FUNCTION_NAME,atanEXP,1,0,"atan(a|d)",1},
							{"atan2",T_FUNCTION_NAME,(double (*)(double))atan2EXP,2,0,"atan2(a|d,a|d)",1},
							{"bhangmeterset",T_FUNCTION_NAME,NULL,-1,55,"bhangmeterset(s,x1,y1,z1,x2,y2,z2,nx,ny,nz)",-1},
							{"ceil",T_FUNCTION_NAME,ceilEXP,1,0,"ceil(a|d)",1},
							{"cellplotset",T_FUNCTION_NAME,NULL,1,30,"cellplotset(s)",1},
							{"clip",T_FUNCTION_NAME,NULL,5,8,"clip(a,xmin,xmax,ymin,ymax)",-1},
							
							{"copy",T_FUNCTION_NAME,NULL,5,37,"copy(s,ns,d,nd,nn)",-1},
							
							{"cosh",T_FUNCTION_NAME,coshEXP,1,0,"cosh(a|d)",1},
							{"cos",T_FUNCTION_NAME,cosEXP,1,0,"cos(a|d)",1},
							{"count",T_FUNCTION_NAME,NULL,2,10,"count(a,d)",1},
							{"ctime",T_FUNCTION_NAME,NULL,1,43,"ctime(0)",1},							
							{"exp",T_FUNCTION_NAME,expEXP,1,0,"exp(a|d)",1},
							{"fabs",T_FUNCTION_NAME,fabsEXP,1,0,"fabs(a|d)",1},
							{"fclose",T_FUNCTION_NAME,NULL,1,41,"fclose(file)",-1},
									
							{"fft",T_FUNCTION_NAME,NULL,4,49,"fft(real,imag,direction,filter)",1},	
														
							{"floor",T_FUNCTION_NAME,floorEXP,1,0,"floor(a|d)",1},
							{"fmod",T_FUNCTION_NAME,(double (*)(double))fmodEXP,2,0,"fmod(a|d,a|d)",1},			
							
							{"fopen",T_FUNCTION_NAME,NULL,1,40,"file=fopen(s)",1},							
							{"fprint",T_FUNCTION_NAME,NULL,3,42,"fprint(file,s,d)",1},
							{"free",T_FUNCTION_NAME,NULL,1,47,"free(s)",-1},							
				
							{"getoption",T_FUNCTION_NAME,NULL,2,45,"getoption(s,name)",1},
							{"gplane",T_FUNCTION_NAME,NULL,12,26,"gplane(s,x1,y1,z1,x2,y2,z2,x3,y3,z3,nx,ny)",-1},							
							{"histogram",T_FUNCTION_NAME,NULL,4,33,"out=histogram(out,i,j,k)",-1},							
							{"index",T_FUNCTION_NAME,NULL,4,32,"index(imax,t,tmin,tmin)",-1},							
							{"insert",T_FUNCTION_NAME,NULL,4,9,"insert(ad,as,xmin,ymin)",-1},	
							
							{"line",T_FUNCTION_NAME,NULL,-1,67,"line()",-1},	
						
							{"lineplotgather",T_FUNCTION_NAME,NULL,1,34,"lineplotgather(1)",1},
							{"lineplotout",T_FUNCTION_NAME,NULL,2,19,"lineplotout(v,d)",0},
							{"lineplotset",T_FUNCTION_NAME,NULL,8,17,"lineplotset(s,x1,y1,z1,x2,y2,z2,n)",-1},
							{"log",T_FUNCTION_NAME,logEXP,1,0,"log(a|d)",1},
							{"log10",T_FUNCTION_NAME,log10EXP,1,0,"log10(a|d)",1},
							{"map",T_FUNCTION_NAME,NULL,2,39,"s=map(a,a)",1},
							{"max",T_FUNCTION_NAME,(double (*)(double))max2,1,1,"max(a)",1},
							{"mean",T_FUNCTION_NAME,NULL,2,54,"s=mean(array,masksize)",1},
							{"min",T_FUNCTION_NAME,(double (*)(double))min2,1,1,"min(a)",1},
	
							
							{"pathname",T_FUNCTION_NAME,NULL,1,35,"pathname(s)",1},							
														
							{"pick",T_FUNCTION_NAME,NULL,2,31,"pick(a,d)",1},	
													
							{"play",T_FUNCTION_NAME,NULL,2,50,"play(array,rate)",1},	
												
							{"plotgetlimits",T_FUNCTION_NAME,NULL,12,52,"plotgetlimits(s,xmin,ymin,zmin,xmax,ymax,zmax,nx,ny,nz,time,framecount)",-1},
							{"plotsetlimits",T_FUNCTION_NAME,NULL,3,25,"plotsetlimits(s,dname,tname)",-1},
							{"plotsetrange",T_FUNCTION_NAME,NULL,9,23,"plotsetrange(s,dname,tname,xmin,ymin,zmin,xmax,ymax,zmax)",-1},
							{"pointplotout",T_FUNCTION_NAME,NULL,2,18,"pointplotout(v,d)",0},
							{"pointplotset",T_FUNCTION_NAME,NULL,4,16,"pointplotset(s,x,y,z)",1},
																				
							{"points",T_FUNCTION_NAME,NULL,-1,36,"points()",-1},	
																			
							{"pow",T_FUNCTION_NAME,(double (*)(double))powEXP,2,0,"pow(a|d,a|d)",1},
							{"print",T_FUNCTION_NAME,NULL,2,12,"print(s,d)",1},
							
							{"random",T_FUNCTION_NAME,NULL,1,46,"random(0|a)",1},							

							{"record",T_FUNCTION_NAME,NULL,2,51,"a=record(rate,samples)",1},	
			
							{"rotate",T_FUNCTION_NAME,NULL,1,24,"d=rotate(a)",1},
														
							{"sagewrite",T_FUNCTION_NAME,NULL,2,38,"sagewrite(s:data,datanew)",-1},
							
							{"seconds",T_FUNCTION_NAME,NULL,1,29,"seconds(0)",1},
							
							{"setframe",T_FUNCTION_NAME,NULL,1,53,"setframe(framenumber)",1},	
													
							{"setoption",T_FUNCTION_NAME,NULL,3,44,"setoption(s,name,value)",1},
						
							
							{"scale",T_FUNCTION_NAME,NULL,3,7,"scale(a,xnew,ynew)",-1},
							{"sin",T_FUNCTION_NAME,sinEXP,1,0,"sin(a|d)",1},
							{"sinh",T_FUNCTION_NAME,sinhEXP,1,0,"sinh(a|d)",1},
							{"size",T_FUNCTION_NAME,NULL,2,3,"size(xmax,ymax)",-1},
							{"size3",T_FUNCTION_NAME,NULL,3,27,"size3(xmax,ymax,zmax)",-1},
							{"sizeof",T_FUNCTION_NAME,NULL,1,4,"sizeof(a)",1},
							{"sqrt",T_FUNCTION_NAME,sqrtEXP,1,0,"sqrt(a|d)",1},
							{"sum",T_FUNCTION_NAME,(double (*)(double))sum,1,1,"sum(a)",1},
							{"table",T_FUNCTION_NAME,NULL,2,15,"table(s,dim)",1},
							{"tan",T_FUNCTION_NAME,tanEXP,1,0,"tan(a|d)",1},
							{"tanh",T_FUNCTION_NAME,tanhEXP,1,0,"tanh(a|d)",1},
							{"window",T_FUNCTION_NAME,NULL,2,2,"window(s,dim)",1},
							{"writesds",T_FUNCTION_NAME,NULL,1,14,"writesds(a)",1},
							{"xfill",T_FUNCTION_NAME,NULL,3,5,"s=xfill(array,start,increment)",1},
							{"xflip",T_FUNCTION_NAME,NULL,1,21,"d=xflip(a)",1},
							{"yfill",T_FUNCTION_NAME,NULL,3,6,"s=yfill(array,start,increment)",1},
							{"yflip",T_FUNCTION_NAME,NULL,1,22,"d=yflip(a)",1},
							{"zfill",T_FUNCTION_NAME,NULL,3,28,"s=zfill(array,start,increment)",1},
						  };

static int nfunctions=sizeof(functions)/sizeof(struct function);

static int checkStack(stackHolderPtr h);
static struct stack *dupStack(struct stack *s);
static int CExpressPushLogicalStack(CExpressPtr e,struct stack *LDP);
static int scale(double *out,long xnew,long ynew,double *in,long xsize,long ysize);
int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);
int sds3DOut(char *path,char *name,long xsize,long ysize,long zsize,double *data,long n,int flag,struct LIMITS *limits);
int CExpressMenu(menuPtr Function);
extern IconPtr TableWindow2(int XCount,int YCount,int XLength,int YLength);
int CExpressMenuInsert(IconPtr myIcon,int item);

static double acosEXP(double v){return acos(v);}
static double asinEXP(double v){return asin(v);}
static double atan2EXP(double v1,double v2){return atan2(v1,v2);}
static double ceilEXP(double v){return ceil(v);}
static double coshEXP(double v){return cosh(v);}
static double cosEXP(double v){return  cos(v);}
static double expEXP(double v){return exp(v);}
static double fabsEXP(double v){return fabs(v);}
static double floorEXP(double v){return floor(v);}
static double fmodEXP(double v1,double v2){return fmod(v1,v2);}
static double logEXP(double v){return log(v);}
static double log10EXP(double v){return log10(v);}
static double powEXP(double v1,double v2){return pow(v1,v2);}
static double sinEXP(double v){return sin(v);}
static double sinhEXP(double v){return sinh(v);}
static double sqrtEXP(double v){return sqrt(v);}
static double tanEXP(double v){return tan(v);}
static double tanhEXP(double v){return tanh(v);}
static double atanEXP(double v){return atan(v);}

static int mean(double *out,double *in,long xsize,long ysize,long size);

int atoo(char *s,unsigned int *ret);
int atoh(char *s,unsigned int *ret);

static int WarningExpression(CExpressPtr e,char *buff);

int recordSound(double *dp,long samples,double ratedouble);

int playSound(double *dp,long length,double ratedouble);

#ifdef Macintosh
int recordSound(double *dp,long samples,double ratedouble)
{
    int kAsync = TRUE;
	SndListHandle mySnd;
    SPB mySPB;            		/*a sound input parameter block*/
    long myInRefNum;      		/*device reference number*/
    long myBuffSize;      		/*size of buffer to record into*/
    short int myHeadrLen;       /*length of sound header*/
    int myNumChans;       		/*number of channels*/
    int mySampSize;       		/*size of a sample*/
    Fixed mySampRate;     		/*sample rate*/
    unsigned long rate;
    unsigned long hi,low;
    OSType myCompType;    		/*compression type*/
    short int *bufferPtr;
    SoundInfoList Listi;
    OSErr myErr;
    int nfac;
    int ret;
    long n;
    
	if(!dp || samples <= 2)return 1;
	
	ret=1;
	

/* intel Bug work around */
#ifdef __i386__
    nfac=2;
#else
    nfac=1;
#endif


    myNumChans=1*nfac;
	
	mySnd=(SndListHandle)NewHandle(2*samples*nfac+200);
	if(!mySnd)goto ErrorOut;
	
    myErr = SPBOpenDevice(NULL, siWritePermission, &myInRefNum);
    if(myErr != noErr)goto ErrorOut;
    
    mySampSize=16;
    myCompType=kSoundNotCompressed;
    
    hi=ratedouble;
    low=65536.0*(ratedouble-hi);
    
    rate=(hi << 16 ) | low;
    
    mySampRate=(Fixed)rate;
    
    myErr = SPBSetDeviceInfo(myInRefNum, siNumberChannels,&myNumChans);
    if(myErr != noErr)goto ErrorOut;
    
    myErr = SPBSetDeviceInfo(myInRefNum, siSampleRate, &mySampRate);
    if(myErr != noErr){
    	myErr = SPBGetDeviceInfo(myInRefNum, siSampleRateAvailable, &Listi);
    	if(myErr != noErr)goto ErrorOut;
    	WarningBatch("Available Sample Rates Are :\n");
    	if(Listi.count > 0 || Listi.infoHandle){
    	    double d;
    	    unsigned long *p;
    	    p=(unsigned long *)(*Listi.infoHandle);
    	    for(n=0;n<Listi.count;++n){
    	        d=p[n];
    	        d /= 65536.0;
    			sprintf(WarningBuff," %f \n",d);
    			WarningBatch(WarningBuff);
    		}
    		DisposeHandle(Listi.infoHandle);
    	}
    }
/*
    myErr = SPBSetDeviceInfo(myInRefNum, siSampleSize, &mySampSize);
    if(myErr != noErr)goto ErrorOut;
 */  
    myErr = SPBSetDeviceInfo(myInRefNum, siCompressionType,&myCompType);
    if(myErr != noErr)goto ErrorOut;
   
    
    myErr = SetupSndHeader(mySnd, myNumChans, mySampRate,mySampSize,
                                        myCompType, kMiddleC, 0, &myHeadrLen);
    if(myErr != noErr)goto ErrorOut;
    
   	myBuffSize = 2*samples*nfac;
	
	bufferPtr = (short int *)(*mySnd) + myHeadrLen;
	
    HLockHi((Handle)mySnd);

	/*Set up the sound input parameter block*/
    mySPB.inRefNum = myInRefNum;                     /*input device reference number*/
    mySPB.count = myBuffSize;                        /*number of bytes to record*/
    mySPB.milliseconds = 0;                          /*no milliseconds*/
    mySPB.bufferLength = myBuffSize;                 /*length of buffer*/
    mySPB.bufferPtr = (char *)bufferPtr;
                                                     /*put data after 'snd ' header*/
    mySPB.completionRoutine = NULL;                  /*no completion routine*/
    mySPB.interruptRoutine = NULL;                   /*no interrupt routine*/
    mySPB.userLong = 0;                              /*no user data*/
    mySPB.error = noErr;                             /*clear error field*/
    mySPB.unused1 = 0;                               /*clear reserved field*/

    /*Record synchronously through the open sound input device.*/
    myErr = SPBRecord(&mySPB, !kAsync);

    HUnlock((Handle)mySnd);                                /*unlock the handle*/
    
	if(myErr != noErr)goto ErrorOut;

	for(n=0;n<samples;++n){
	    dp[n]=bufferPtr[n*nfac];
	}
	ret=0;
	
ErrorOut:

	if(mySnd)DisposeHandle((Handle)mySnd);
	
	
	if(ret){
	    sprintf(WarningBuff,"Error %ld in subroutine recordSound\n",(long)myErr);
		WarningBatch(WarningBuff);
	}
	
	return ret;
}

int playSound(double *dp,long length,double ratedouble)
{
	SndListHandle mySnd;
    short int myHeadrLen;       /*length of sound header*/
    int myNumChans;       		/*number of channels*/
    int mySampSize;       		/*size of a sample*/
    Fixed mySampRate;     		/*sample rate*/
    unsigned long rate;
    unsigned long hi,low;
    OSType myCompType;    		/*compression type*/
    short int *bufferPtr;
    OSErr myErr;
    int ret;
    long n;
	
	if(!dp || length <= 2 )return 1;
	
	ret=1;
	
	mySnd=(SndListHandle)NewHandle(2*length+100);
	if(!mySnd)goto ErrorOut;

    myNumChans=1;

    mySampSize=16;
    myCompType=kSoundNotCompressed;
    
    hi=ratedouble;
    low=65536.0*(ratedouble-hi);
    
    rate=(hi << 16 ) | low;
    
    mySampRate=(Fixed)rate;
    
    myErr = SetupSndHeader(mySnd, myNumChans, mySampRate,mySampSize,
                                        myCompType, kMiddleC, 0, &myHeadrLen);
	if(myErr)goto  ErrorOut;                        

    myErr = SetupSndHeader(mySnd, myNumChans, mySampRate, mySampSize,
                                            myCompType, kMiddleC, 2*length,
                                            &myHeadrLen);
	if(myErr)goto  ErrorOut;                        

    HLockHi((Handle)mySnd);
        
	bufferPtr = (short int *)(*mySnd) + myHeadrLen;

	for(n=0;n<length;++n){
	    bufferPtr[n]=(short int)dp[n];
	}
	
	HUnlock((Handle)mySnd);                                /*unlock the handle*/

	myErr=SndPlay(NULL,mySnd,TRUE);
	if(myErr)goto  ErrorOut;                        

	ret=0;
ErrorOut:	
	if(mySnd)DisposeHandle((Handle)mySnd);
	
	if(ret){
	    sprintf(WarningBuff,"Error %ld in subroutine playSound\n",(long)myErr);
		WarningBatch(WarningBuff);
	}
	
	return ret;
}
#else
int recordSound(double *dp,long samples,double time)
{
	//dp=dp;
	//time=time;
	//samples=samples;
	WarningBatch("recordSound Not active on This System\n");
	return 0;
}
int playSound(double *dp,long length,double value)
{
	//dp=dp;
	//value=value;
	//length=length;
	WarningBatch("playSound Not active on This System\n");
	return 0;
}
#endif

static int WarningExpression(CExpressPtr e,char *buff)
{
	if(!buff)return 1;
	
	WarningBatch(buff);
	
	if(e){
		sprintf(WarningBuff,"** Error Line : %ld **\n",e->lineNumber+1);
		WarningBatch(WarningBuff);
	}
	
	return 0;
	
}
int CExpressMenuInsert(IconPtr myIcon,int item)
{
	CEditPtr w;
	int k,n;
	
	if(!myIcon)return doReturn(1);
	w=(CEditPtr)myIcon->DWindow;
	if(!w)return doReturn(1);
	k=1;
	for(n=0;n<nfunctions;++n){
	    if(functions[n].form){
	        if(k++ == item){
	            CVideoInsertAtCursorLong(myIcon,functions[n].form);
    		    break;
    		}
	    }
	}	
	return 0;
}
int doReturn(int value)
{


	return value;
}

int CExpressMenu(menuPtr Function)
{
	char buff[256];
	long n,mCount;
	if(!Function)return doReturn(1);
	
	mCount=uCountMenuItems(Function);
	for(n=0;n<mCount;++n){
	    uDeleteMenuItem(Function,1);
	}
	for(n=0;n<nfunctions;++n){
	    if(functions[n].form){
	       mstrncpy(buff,functions[n].name,256);
	       uInsertMenuItem(Function,buff,500);
	    }
	}

	return 0;	
	
}
CExpressPtr CExpressStart(int Mode)
{
	CExpressPtr e;
	
	e=(CExpressPtr)cMalloc(sizeof(*e),8021);
	if(!e)return NULL;
	zerol((char *)e,sizeof(*e));
	
	e->Mode=Mode;
	
	return e;
	
}
int CExpressEvaluate(CExpressPtr e,unsigned char *dat)
{
	struct stack *name;
	int ret;
	
	if(!e || !dat)return doReturn(1);
	
	ret=1;
	
	e->dat=dat;
	
	e->stackData.stackCount=0;
	e->logicalData.stackCount=0;
	e->nameData.stackCount=0;
	e->outData.stackCount=0;
	e->lineNumber=0;
	e->RR=NULL;
	
	e->lineStack=pushpopStart(&e->li);
	if(!e->lineStack)goto ErrorOut;		
		
	
	name=CExpressCreateNameStack(e,"frame",T_STACK_CONSTANT);
	if(!name)return doReturn(1);
	name->value=(double)e->CurrentFrame;

	name=CExpressCreateNameStack(e,"frameLast",T_STACK_CONSTANT);
	if(!name)return doReturn(1);
	name->value=(double)e->LastFrame;
	
	srand(1984);
	
	if(CExpressNext(e)){
		ret=1;
	    goto ErrorOut;
	}
	
	if(CExpressStatement(e)){
		ret=1;
	    goto ErrorOut;
    }
	
	ret=0;
	
ErrorOut:	

	if(ret || !e->KeepStack)CExpressFreeStacks(e);
	

	return ret;
}
int CExpressFreeStack(struct stackHolder *s)
{
	long n;

	if(!s)return 1;
	
	if(s->stackCount > 0){
	    for(n=0;n<s->stackCount;++n){
	        if(s->stackList[n].type == T_STACK_VARIABLE){
	            if(s->stackList[n].data){
	                cFree((char *)s->stackList[n].data);
	                s->stackList[n].data=NULL;
	            }
	            if(s->stackList[n].auxData){
	                cFree((char *)s->stackList[n].auxData);
	                s->stackList[n].auxData=NULL;
	            }
	        }
	    }
	    s->stackCount=0;
	}
	
	
	return 0;
	
}
int CExpressFreeStacks(CExpressPtr e)
{
	long n,count;
	
	if(!e)return doReturn(1);
	
	count=pushpopDepth(e->lineStack);
	for(n=0;n<count;++n){
		pushpopPeek(&e->li,n,e->lineStack);
		if(e->li.x)cFree((char *)e->li.x);
		e->li.x=NULL;
		if(e->li.y)cFree((char *)e->li.y);
		e->li.y=NULL;
		if(e->li.z)cFree((char *)e->li.z);
		e->li.z=NULL;
		if(e->li.v)cFree((char *)e->li.v);
		e->li.v=NULL;
	}
	pushpopEND(e->lineStack);
	e->lineStack=NULL;
	
	
	CExpressFreeStack(&e->outData);
	
	CExpressFreeStack(&e->nameData);
	
	CExpressFreeStack(&e->stackData);
	
	CExpressFreeStack(&e->logicalData);
	
	return 0;
	
}
static int setE(CExpressPtr e,CExpressPtr en)
{
	if(!e || !en)return 1;
	e->dat=en->dat;
	strncpy(e->ch,en->ch,sizeof(en->ch));
	e->tok=en->tok;
	e->tok2=en->tok2;
	e->lineNumber=en->lineNumber;
	return 0;
}

static int CExpressFor(CExpressPtr e)
{
	struct CExpress startTest;
	struct CExpress startIncrement;
	struct CExpress startBody;
	struct CExpress endBody;
	struct stack *R1;
	long depth;

	if(!e)return doReturn(1);

	if(CExpressNext(e) || (e->tok != T_PAREN_LEFT)){
		WarningExpression(e,"for missing left paren\n");
		return doReturn(1);
		
	}
	
	if(CExpressNext(e)){
		WarningExpression(e,"for missing start expression\n");
		return doReturn(1);
		
	}

	if(CExpressExpression(e))return doReturn(1);
	
	
	if(e->tok != T_SEMICOLON){
		WarningExpression(e,"for missing first semicolon\n");
		return doReturn(1);
		
	}
		
	if(CExpressNext(e)){
		WarningExpression(e,"for missing range expression\n");
		return doReturn(1);
		
	}
	
	startTest = *e;
	
	while(e->tok != T_SEMICOLON){
		if(CExpressNext(e)){
			WarningExpression(e,"for error reading range test data\n");
			return doReturn(1);		
		}	
	}


	if(CExpressNext(e)){
		WarningExpression(e,"for missing increment expression\n");
		return doReturn(1);
		
	}
	
	startIncrement = *e;


	while(e->tok != T_PAREN_RIGHT){
		if(CExpressNext(e)){
			WarningExpression(e,"for error reading range test data\n");
			return doReturn(1);		
		}	
	}


	if(CExpressNext(e)){
		WarningExpression(e,"for missing expression\n");
		return doReturn(1);
		
	}
	
	if(e->tok == T_CURLY_LEFT){
		startBody = *e;	
		if(CExpressNext(e)){
			WarningExpression(e,"for body\n");
			return doReturn(1);
			
		}
		if(e->tok == T_CURLY_RIGHT){
			WarningExpression(e,"Empty For expression not permitted\n");
			return doReturn(1);
		}
		depth=1;
		while(e->tok != T_CURLY_RIGHT || depth > 0){
			if(CExpressNext(e)){
				WarningExpression(e,"or error reading body data\n");
				return doReturn(1);		
			}	
			if(e->tok == T_CURLY_LEFT)++depth;
			if(e->tok == T_CURLY_RIGHT)--depth;
		}
	}else{
		startBody = *e;
		while(e->tok != T_SEMICOLON){
			if(CExpressNext(e)){
				WarningExpression(e,"for error reading body data\n");
				return doReturn(1);		
			}	
		}
	}

	if(CExpressNext(e)){
		WarningExpression(e,"for missing expression\n");
		return doReturn(1);
		
	}
	
	endBody = *e;	
	
	while(1){
	    setE(e,&startTest);
		if(CExpressExpression(e))return doReturn(1);
		if(CExpressPop(e,T_STACK_R1,1))return doReturn(1);
		R1 = &e->R1;
		if(R1->value <= 0)break;
	    setE(e,&startBody);
		if(CExpressStatement(e))return doReturn(1);
	    setE(e,&startIncrement);
		if(CExpressExpression(e))return doReturn(1);	
		CExpressFreeStack(&e->stackData);
		
	}
	
	setE(e,&endBody);

	
	return 0;
}
static int CExpressStatement(CExpressPtr e)
{
	if(!e)return doReturn(1);
	
	if(e->tok == T_IF){
		return CExpressIf(e);
	}else if(e->tok == T_CURLY_LEFT){
		return CExpressCurly(e);
	}else if(e->tok == T_SEMICOLON){
		if(CExpressNext(e))return doReturn(1);
		return 0;
	}else if(e->tok == T_EOF){
		return doReturn(1);
	}else if(e->tok == T_FOR){
		return CExpressFor(e);
	}
	
	if(CExpressExpression(e))return doReturn(1);
		
	if(e->tok != T_SEMICOLON){
		WarningExpression(e,"Expression missing SemiColon\n");
		return doReturn(1);
	}
	
	return CExpressNext(e);
	
	
}
static int CExpressIf(CExpressPtr e)
{
	int EqualToStack;
	int ret;
	
	if(!e)return doReturn(1);
	
	if(CExpressNext(e) || (e->tok != T_PAREN_LEFT)){
		WarningExpression(e,"If missing left paren\n");
		return doReturn(1);
		
	}

	if(CExpressNext(e)){
		WarningExpression(e,"If missing expression\n");
		return doReturn(1);
		
	}
	
	EqualToStack=e->EqualToStack;
	e->EqualToStack=TRUE;
	
	if(CExpressExpression(e))return doReturn(1);
	
	e->EqualToStack=EqualToStack;
	
	if(e->tok != T_PAREN_RIGHT){
		WarningExpression(e,"If missing right paren\n");
		return doReturn(1);
	}
	
	
	if(CExpressNext(e)){
		WarningExpression(e,"If missing statment\n");
		return doReturn(1);
	}
	
	
	if(CExpressDoIf(e,T_IF_START))return doReturn(1);
	
	if(CExpressStatement(e))return doReturn(1);

	if(e->tok != T_ELSE){
	    if(CExpressDoIf(e,T_IF_ENDSHORT))return doReturn(1);
	    return 0;
	}
		
	if(CExpressNext(e)){
		WarningExpression(e,"If missing else statement\n");
		return doReturn(1);
	}
	
    if(CExpressDoIf(e,T_IF_ELSE))return doReturn(1);
    
	ret=CExpressStatement(e);
	
    if(CExpressDoIf(e,T_IF_ENDLONG))return doReturn(1);
	    
	return ret;
}
static int CExpressCurly(CExpressPtr e)
{
	if(!e)return doReturn(1);
	
	if(CExpressNext(e))return doReturn(1);
	
	while (1){
		if(e->tok == T_CURLY_RIGHT)return CExpressNext(e);
		if(CExpressStatement(e))return doReturn(1);
	}
}

static int CExpressExpression(CExpressPtr e)
{
	int tok;
	
	if(!e)return doReturn(1);
	
	if(CExpressLogicalLOR(e))return doReturn(1);
	
	if(e->tok == T_EQUAL      		|| e->tok == T_PLUSEQUAL 		||
	   e->tok == T_MINUSEQUAL 		|| e->tok == T_MULTEQUAL 		||
	   e->tok == T_DIVEQUAL   		|| e->tok == T_MODEQUAL  		||
	   e->tok == T_XOREQUAL   		|| e->tok == T_ANDEQUAL  		||
	   e->tok == T_LEFTSHIFTEQUAL   || e->tok == T_RIGHTSHIFTEQUAL  ||
	   e->tok == T_OREQUAL   	   
	   ){
		tok=e->tok2;
		if(CExpressNext(e))return doReturn(1);
		if(CExpressExpression(e))return doReturn(1);
		if(CExpressDoEqual(e,tok))return doReturn(1);
	}
	
	
	return 0;
}
static int CExpressLogicalLOR(CExpressPtr e)
{
	if(!e)return doReturn(1);
	if(CExpressLogicalLAND(e))return doReturn(1);
	while(1){
		if(e->tok == T_LOR){
			if(CExpressNext(e))return doReturn(1);
			if(CExpressLogicalLAND(e))return doReturn(1);
			if(CExpressDoBINARY(e,T_LOR))return doReturn(1);
		}else{
			return 0;
		}
	}
}
static int CExpressLogicalLAND(CExpressPtr e)
{
	if(!e)return doReturn(1);
	if(CExpressOR(e))return doReturn(1);
	while(1){
		if(e->tok == T_LAND){
			if(CExpressNext(e))return doReturn(1);
			if(CExpressOR(e))return doReturn(1);
			if(CExpressDoBINARY(e,T_LAND))return doReturn(1);
		}else{
		    return 0;
		}
	}
}
static int CExpressOR(CExpressPtr e)
{
	if(!e)return doReturn(1);
	if(CExpressXOR(e))return doReturn(1);
	while(1){
		if(e->tok == T_OR){
			if(CExpressNext(e))return doReturn(1);
			if(CExpressXOR(e))return doReturn(1);
			if(CExpressDoBINARY(e,T_OR))return doReturn(1);
		}else{
		    return 0;
		}
	}
}
static int CExpressXOR(CExpressPtr e)
{
	if(!e)return doReturn(1);
	if(CExpressAND(e))return doReturn(1);
	while(1){
		if(e->tok == T_XOR){
			if(CExpressNext(e))return doReturn(1);
			if(CExpressAND(e))return doReturn(1);
			if(CExpressDoBINARY(e,T_XOR))return doReturn(1);
		}else{
			return 0;
		}
	}
}
static int CExpressAND(CExpressPtr e)
{
	if(!e)return doReturn(1);
	if(CExpressEQNOTEQ(e))return doReturn(1);
	while(1){
		if(e->tok == T_AND){
			if(CExpressNext(e))return doReturn(1);
			if(CExpressEQNOTEQ(e))return doReturn(1);
			if(CExpressDoBINARY(e,T_AND))return doReturn(1);
		}else{
		    return 0;
		}
	}
}
static int CExpressEQNOTEQ(CExpressPtr e)
{
	int tok;
	
	if(!e)return doReturn(1);
	if(CExpressLESSGREAT(e))return doReturn(1);
	while(1){
		if(e->tok == T_EQUALEQUAL || e->tok == T_NOTEQUAL ){
		    tok=e->tok;
			if(CExpressNext(e))return doReturn(1);
			if(CExpressLESSGREAT(e))return doReturn(1);
			if(CExpressDoBINARY(e,tok))return doReturn(1);
		}else{
			return 0;
		}
	}
}

static int CExpressLESSGREAT(CExpressPtr e)
{
	int tok;
	
	if(!e)return doReturn(1);
	if(CExpressSHIFT(e))return doReturn(1);
	while(1){
		if(e->tok == T_LESS  || e->tok == T_LESSEQUAL ||
		   e->tok == T_GREAT || e->tok == T_GREATEQUAL ){
		    tok=e->tok;
			if(CExpressNext(e))return doReturn(1);
			if(CExpressSHIFT(e))return doReturn(1);
			if(CExpressDoBINARY(e,tok))return doReturn(1);
		}else{
			return 0;
		}
	}
}

static int CExpressSHIFT(CExpressPtr e)
{
	int tok;
	
	if(!e)return doReturn(1);
	if(CExpressPLUSMINUS(e))return doReturn(1);
	while(1){
		if(e->tok == T_LEFTSHIFT  || e->tok == T_RIGHTSHIFT){
		    tok=e->tok;
			if(CExpressNext(e))return doReturn(1);
			if(CExpressPLUSMINUS(e))return doReturn(1);
			if(CExpressDoBINARY(e,tok))return doReturn(1);
		}else{
			return 0;
		}
	}
}
static int CExpressPLUSMINUS(CExpressPtr e)
{
	int tok;
	
	if(!e)return doReturn(1);
	if(CExpressMULTDIVMOD(e))return doReturn(1);
	while(1){
		if(e->tok == T_PLUS  || e->tok == T_MINUS){
		    tok=e->tok;
			if(CExpressNext(e))return doReturn(1);
			if(CExpressMULTDIVMOD(e))return doReturn(1);
			if(CExpressDoBINARY(e,tok))return doReturn(1);
		}else{
			return 0;
		}
	}
}
static int CExpressMULTDIVMOD(CExpressPtr e)
{
	int tok;
	
	if(!e)return doReturn(1);
	if(CExpressUNARY(e))return doReturn(1);
	while(1){
		if(e->tok == T_MULT  || e->tok == T_DIV ||
		   e->tok == T_MOD){
		    tok=e->tok;
			if(CExpressNext(e))return doReturn(1);
			if(CExpressUNARY(e))return doReturn(1);
			if(CExpressDoBINARY(e,tok))return doReturn(1);
		}else{
			return 0;
		}
	}
}

static int CExpressUNARY(CExpressPtr e)
{
	int tok,tok2;
	
	if(!e)return doReturn(1);
	if(e->tok == T_MINUS  || e->tok == T_NEGATE ||
	   e->tok == T_NOT    || e->tok == T_UNARYINCREMENT){
	   
	    tok=e->tok;
	    tok2=e->tok2;
	    if(tok == T_MINUS){
	        tok = T_MINUSUNARY;
	    }else if(e->tok == T_UNARYINCREMENT){
	         tok=e->tok;
	    }
	    
		if(CExpressNext(e))return doReturn(1);
		if(CExpressUNARY(e))return doReturn(1);
		if(tok == T_UNARYINCREMENT){
		    if(CExpressDoUnaryIncrnemt(e,tok2,0))return doReturn(1);
		}else{
		    if(CExpressDoUNARY(e,tok))return doReturn(1);
		}
	}else{
	    if(CExpressFUNCTION(e))return doReturn(1);
		if( e->tok == T_UNARYINCREMENT){
		    if(CExpressDoUnaryIncrnemt(e,e->tok2,1))return doReturn(1);
			if(CExpressNext(e))return doReturn(1);
		}
	}
	return 0;
}
static int CExpressFUNCTION(CExpressPtr e)
{
	int EqualToStack;
	int function;
	int count;
	
	if(!e)return doReturn(1);
	if(CExpressPRIMARY(e))return doReturn(1);
	while(1){
		if(e->tok == T_FUNCTION_NAME){
		    function=e->function;
			if(CExpressNext(e))return doReturn(1);
			if(e->tok != T_PAREN_LEFT){
			    sprintf(WarningBuff,"FUNCTION %s is Missing Left Parenthesis\n",functions[function].name);
			    WarningExpression(e,WarningBuff);
				return doReturn(1);
			}
			
			EqualToStack=e->EqualToStack;
			e->EqualToStack=TRUE;
			count=0;
			do{
			   if(CExpressNext(e))return doReturn(1);
			   if(CExpressExpression(e))return doReturn(1);
			   ++count;
			}while(e->tok == T_COMMA);
			
			e->EqualToStack=EqualToStack;
			
			if(e->tok != T_PAREN_RIGHT){
			    sprintf(WarningBuff,"FUNCTION %s is Missing Right Parenthesis\n",functions[function].name);
			    WarningExpression(e,WarningBuff);
				return doReturn(1);
			}
			
			if(CExpressNext(e))return doReturn(1);
			
			if(CExpressDoFUNCTION(e,function,count))return doReturn(1);
			
		}else{
			return 0;
		}
	}
}
static int CExpressPRIMARY(CExpressPtr e)
{
	char buff[256];
	
	if(!e)return doReturn(1);
	if(e->tok == T_PAREN_LEFT){
		if(CExpressNext(e))return doReturn(1);
		if(CExpressExpression(e))return doReturn(1);
		if(e->tok != T_PAREN_RIGHT){
		    WarningExpression(e,"Expression Missing Right Paren\n");
			return doReturn(1);
		}
		if(CExpressNext(e))return doReturn(1);
	}else if(e->tok == T_IDENTIFIER){
		int pioIndex;
		mstrncpy(buff,e->ch,256);
	    if(CExpressPush(e,T_STACK_NAME))return doReturn(1);
		if(CExpressNext(e))return doReturn(1);
		if(e->tok == T_PAREN_LEFT){
			sprintf(WarningBuff,"Function '%s' Not Found\n",buff);
			WarningExpression(e,WarningBuff);
			return doReturn(1);
		}else if(e->tok == T_COLON){
		    
		    pioIndex=0;
		    if(CExpressNext(e))return doReturn(1);
		    if(e->tok != T_IDENTIFIER && e->tok != T_STACK_STRING){
				sprintf(WarningBuff,"Colon Must be followed by name\n");
				WarningExpression(e,WarningBuff);
				return doReturn(1);
		    }
		    
	        if(CExpressPush(e,T_STACK_NAME))return doReturn(1);
		    if(CExpressNext(e))return doReturn(1);
		    if(e->tok == T_PAREN_LEFT){
				if(CExpressNext(e))return doReturn(1);
				if(CExpressExpression(e))return doReturn(1);
				if(e->tok != T_PAREN_RIGHT){
				    WarningExpression(e,"Expression Missing Right Paren\n");
					return doReturn(1);
				}
				if(CExpressPop(e,T_STACK_R1,1))return doReturn(1);
	       		if(e->R1.type != T_STACK_CONSTANT){
				    WarningExpression(e,"pioIndex Must be constant\n");
					return doReturn(1);
	       		}
				 pioIndex=(int)e->R1.value;
				if(CExpressNext(e))return doReturn(1);
		    }
	        if(CExpressPop(e,T_STACK_R1,0))return doReturn(1);
	        if(CExpressPop(e,T_STACK_R2,0))return doReturn(1);
	        e->R2.pioIndex=pioIndex;
	        mstrncpy(e->R2.pioName,e->R1.name,64);
	        if(pioIndex > 0){
	        	mstrncat(e->R2.pioName,"(",64);
	        	sprintf(buff,"%d%c",(int)pioIndex,0);
	        	mstrncat(e->R2.pioName,buff,64);
	        	mstrncat(e->R2.pioName,")",64);
	        }
	        if(CExpressPush(e,T_STACK_R2))return doReturn(1);
		}else if(e->tok == T_SQUARE_LEFT){
			if(CExpressNext(e))return doReturn(1);
			if(CExpressExpression(e))return doReturn(1);
			if(e->tok != T_SQUARE_RIGHT){
			    WarningExpression(e,"Square Brackets Missing Right Paren\n");
				return doReturn(1);
			}
			if(CExpressPop(e,T_STACK_R1,1))return doReturn(1);
       		if(e->R1.type != T_STACK_CONSTANT){
			    WarningExpression(e,"Square Brackets Index Must be constant\n");
				return doReturn(1);
       		}
			pioIndex=(int)e->R1.value;
	        if(CExpressPop(e,T_STACK_R2,0))return doReturn(1);
        	mstrncat(e->R2.name,"[",64);
        	sprintf(buff,"%d%c",(int)pioIndex,0);
        	mstrncat(e->R2.name,buff,64);
        	mstrncat(e->R2.name,"]",64);
	        if(CExpressPush(e,T_STACK_R2))return doReturn(1);
			if(CExpressNext(e))return doReturn(1);
		}
	}else if(e->tok == T_STACK_STRING){
	    if(CExpressPush(e,T_STACK_STRING))return doReturn(1);
		if(CExpressNext(e))return doReturn(1);
	}else if(e->tok == T_CONSTANT){
	    if(CExpressPush(e,T_STACK_CONSTANT))return doReturn(1);
		if(CExpressNext(e))return doReturn(1);
	}else if(e->tok == T_FUNCTION_NAME){
		return 0;
	}else{
		sprintf(WarningBuff,"invalid expression at token %d\n",e->tok);
		WarningExpression(e,WarningBuff);
		return doReturn(1);
	}
	return 0;
}
static int CExpressDoEqual(CExpressPtr e,int function)
{
	struct stack RS,*R1,*R2;
	struct stack *LD;
	struct stack *name;
	long length,n;
	int ret;
	
	if(!e)return doReturn(1);
	
	LD=CExpressGetLogicalStack(e,0);
	if(!LD)return doReturn(1);
	
	ret=1;
	R1=NULL;
	R2=NULL;
	
	if(CExpressPop(e,T_STACK_R2,1))return doReturn(1);
	if(CExpressPop(e,T_STACK_R1,0))return doReturn(1);
	
	RS=e->R1;
	
	if(function != T_EQUAL){
	    if(CExpressPush(e,T_STACK_R1))return doReturn(1);
	    if(CExpressPush(e,T_STACK_R2))return doReturn(1);
	    if(CExpressDoBINARY(e,function))return doReturn(1);
		if(CExpressPop(e,T_STACK_R2,1))return doReturn(1);
	}
	
	
	R2 = &e->R2;
		
	name=CExpressGetNameStack(e,RS.name);
	if(name){
	    if(name->type == T_STACK_CONSTANT){
Constant:   
		    if(R2->type == T_STACK_CONSTANT){
				if(LD->type == T_STACK_LCONSTANT){
	                if(LD->logical){
	                    name->value=R2->value;
	                    name->file=R2->file;
	                }
				}else if(LD->type == T_STACK_LARRAY){
		            length=LD->xsize*LD->ysize*LD->zsize;
	                for(n=0;n<length;++n){
	            		if(LD->ldata[n]){
	                        name->value=R2->value;
	                    	name->file=R2->file;
	            		}
	                }
		    	}
		    }else if(R2->type == T_STACK_ARRAY){
	            if(!R2 || !R2->data){
	        	    sprintf(WarningBuff,"CExpressDoEqual R2 %ld\n",(long)R2);
		    	    WarningExpression(e,WarningBuff);
	                goto ErrorOut;
	            }
		        length=R2->xsize*R2->ysize*R2->zsize;
				if(LD->type == T_STACK_LCONSTANT){
	                if(LD->logical){
	                    name->value=R2->data[length-1];
				    }		
				}else if(LD->type == T_STACK_LARRAY){
		            if(length != LD->xsize*LD->ysize*LD->zsize){
		        	    sprintf(WarningBuff,"CExpressDoEqual Array Size Missmatch Array %ld Logical %ld\n",
		        	            length,LD->xsize*LD->ysize*LD->zsize);
			    	    WarningExpression(e,WarningBuff);
		        	    goto ErrorOut;
		            }
	                for(n=0;n<length;++n){
	        		    if(LD->ldata[n]){
	                        name->value=R2->data[n];
	                    }
	            	}
		    	}
		    }
		    if(e->EqualToStack){
		        mstrncpy(e->ch,name->name,256);
	            if(CExpressPush(e,T_STACK_NAME))return doReturn(1);
	        }
	    }else if(name->type == T_STACK_FILE){
		    struct SetFrameData  sd;
		    e->R1 = *name;
		    e->R1.type = T_STACK_NAME;
	        if(CExpressPush(e,T_STACK_R1)){
        	    sprintf(WarningBuff,"CExpressDoEqual CExpressPush Error\n");
	    	    WarningExpression(e,WarningBuff);
	            goto ErrorOut;
	        }
		    if(CExpressPop(e,T_STACK_R1,1)){
        	    sprintf(WarningBuff,"CExpressDoEqual CExpressPop Error\n");
	    	    WarningExpression(e,WarningBuff);
		        goto ErrorOut;
		    }
		    R1 = &e->R1;
		    name->xsize=R1->xsize;
		    name->ysize=R1->ysize;
		    name->zsize=R1->zsize;
		    name->data=R1->data;
		   
		    
			zerol((char *)&sd,sizeof(struct SetFrameData));
		    sd.type=FRAME_DATA_FLOAT;
		    
	        if(!name || !name->data){
		       	sprintf(WarningBuff,"CExpressDoEqual T_STACK_FILE name %ld Error\n",(long)name);
			    WarningExpression(e,WarningBuff);
	            goto ErrorOut;
	        }
	        
		    length=name->xsize*name->ysize*name->zsize;
	        if(length <= 0){
		       	sprintf(WarningBuff,"CExpressDoEqual T_STACK_FILE length %ld\n",length);
			    WarningExpression(e,WarningBuff);
	            goto ErrorOut;
	        }
	        
	        
		    if(R2->type == T_STACK_CONSTANT){
				if(LD->type == T_STACK_LCONSTANT){
				    if(LD->logical){
		                for(n=0;n<length;++n){
		                    name->data[n]=R2->value;
		            	}
	            	}
				}else if(LD->type == T_STACK_LARRAY){
		            if(length != LD->xsize*LD->ysize*LD->zsize){
		        	    sprintf(WarningBuff,"CExpressDoEqual Array Size Missmatch Array %ld Logical %ld\n",
		        	            length,LD->xsize*LD->ysize*LD->zsize);
			    	    WarningExpression(e,WarningBuff);
		        	    goto ErrorOut;
		            }
	                for(n=0;n<length;++n){
	        		    if(LD->ldata[n]){
	                        name->data[n]=R2->value;
	                    }
	            	}
		    	}
		    }else if(R2->type == T_STACK_ARRAY){
		        if(length != R2->xsize*R2->ysize*R2->zsize){
		        	sprintf(WarningBuff,"CExpressDoEqual Array Size Missmatch Array %ld R2 %ld\n",
		        	            length,R2->xsize*R2->ysize*R2->zsize);
			    	WarningExpression(e,WarningBuff);
		        	goto ErrorOut;
		        }
				if(LD->type == T_STACK_LCONSTANT){
				    if(LD->logical){
		                for(n=0;n<length;++n){
		                    name->data[n]=R2->data[n];
		            	}
	            	}
				}else if(LD->type == T_STACK_LARRAY){
		            if(length != LD->xsize*LD->ysize*LD->zsize){
		        	    sprintf(WarningBuff,"CExpressDoBINARY Array Size Missmatch Array %ld Logical %ld\n",
		        	            length,LD->xsize*LD->ysize*LD->zsize);
			    	    WarningExpression(e,WarningBuff);
		        	    goto ErrorOut;
		            }
	                for(n=0;n<length;++n){
	        		    if(LD->ldata[n]){
	                        name->data[n]=R2->data[n];
	                    }
	            	}
		    	}
		    }
			    
	        sd.xsize=name->xsize;
	        sd.ysize=name->ysize;
	        sd.zsize=name->zsize;
	        sd.data=name->data;
	        	        	        	        
	    	sd.CurrentFrame=e->CurrentFrame;	    			    	
			if(SendMessageByName(name->name,MessageType_SET_REGION_DATA,&sd)){	        	        
				    sprintf(WarningBuff,"CExpressDoEqual - Window %s Error Puting Data\n",name->name);
				    WarningExpression(e,WarningBuff);
				   goto ErrorOut;
		    }
		}else if(name->type == T_STACK_VARIABLE) {
				    
		        if(!name->data){
			       sprintf(WarningBuff,"CExpressDoEqual STACK_VARIABLE %s NULL data Pointer\n",
			        	            name->name);
				    WarningExpression(e,WarningBuff);
		            goto ErrorOut;
		        }
			    length=name->xsize*name->ysize*name->zsize;
		        if(length <= 0){
			       	sprintf(WarningBuff,"CExpressDoEqual T_STACK_VARIABLE length %ld\n",length);
				    WarningExpression(e,WarningBuff);
		            goto ErrorOut;
		        }
		        
			    if(R2->type == T_STACK_CONSTANT){
					if(LD->type == T_STACK_LCONSTANT){
					    if(LD->logical){
			                for(n=0;n<length;++n){
			                    name->data[n]=R2->value;
			            	}
		            	}
					}else if(LD->type == T_STACK_LARRAY){
			            if(length != LD->xsize*LD->ysize*LD->zsize){
			        	    sprintf(WarningBuff,"CExpressDoEqual Array Size Missmatch Array %ld Logical %ld\n",
			        	            length,LD->xsize*LD->ysize*LD->zsize);
				    	    WarningExpression(e,WarningBuff);
			        	    goto ErrorOut;
			            }
		                for(n=0;n<length;++n){
		        		    if(LD->ldata[n]){
		                        name->data[n]=R2->value;
		                    }
		            	}
			    	}
			    }else if(R2->type == T_STACK_ARRAY){
			        if(length != R2->xsize*R2->ysize*R2->zsize){
			        	sprintf(WarningBuff,"CExpressDoEqual Array Size Missmatch Array %ld R2 %ld\n",
			        	            length,R2->xsize*R2->ysize*R2->zsize);
				    	WarningExpression(e,WarningBuff);
			        	goto ErrorOut;
			        }
					if(LD->type == T_STACK_LCONSTANT){
					    if(LD->logical){
			                for(n=0;n<length;++n){
			                    name->data[n]=R2->data[n];
			            	}
		            	}
					}else if(LD->type == T_STACK_LARRAY){
			            if(length != LD->xsize*LD->ysize*LD->zsize){
			        	    sprintf(WarningBuff,"CExpressDoBINARY Array Size Missmatch Array %ld Logical %ld\n",
			        	            length,LD->xsize*LD->ysize*LD->zsize);
				    	    WarningExpression(e,WarningBuff);
			        	    goto ErrorOut;
			            }
		                for(n=0;n<length;++n){
		        		    if(LD->ldata[n]){
		                        name->data[n]=R2->data[n];
		                    }
		            	}
			    	}
				}
	        
	    }else{
		    sprintf(WarningBuff,"CExpressDoEqual found name %s type %d bad\n",name->name,name->type);
		    WarningExpression(e,WarningBuff);
		    return doReturn(1);
		}
	}else {
	    if(R2->type == T_STACK_CONSTANT){
	        name=CExpressCreateNameStack(e,RS.name,T_STACK_CONSTANT);
	        if(!name)return doReturn(1);
	        goto Constant;
	    }else if(R2->type == T_STACK_ARRAY){
	        name=CExpressCreateNameStack(e,RS.name,T_STACK_VARIABLE);
	        if(!name)return doReturn(1);
		    name->xsize=R2->xsize;
		    name->ysize=R2->ysize;
		    name->zsize=R2->zsize;
		    name->data=R2->data;
		    
			name->PointLineCount=R2->PointLineCount;
			name->PointLineType=R2->PointLineType;
			name->integrate=R2->integrate;
			name->integral=R2->integral;
			name->auxData=R2->auxData;
			name->auxLength=R2->auxLength;
			name->n1=R2->n1;
			name->n2=R2->n2;
			name->n3=R2->n3;
			name->x1=R2->x1;
			name->x2=R2->x2;
			name->x3=R2->x3;
			name->y1=R2->y1;
			name->y2=R2->y2;
			name->y3=R2->y3;
			name->z1=R2->z1;
			name->z2=R2->z2;
			name->z3=R2->z3;
	    
	        R2=NULL;
	    }else if(R2->type == T_STACK_DIMENSIONS){
	        name=CExpressCreateNameStack(e,RS.name,T_STACK_VARIABLE);
	        if(!name)return doReturn(1);
		    name->xsize=R2->xsize;
		    name->ysize=R2->ysize;
		    name->zsize=R2->zsize;
		    length=name->xsize*name->ysize*name->zsize;
		    name->data=(double *)cMalloc(length*sizeof(double),8022);
		    if(!name->data){
        	    sprintf(WarningBuff,"CExpressDoBINARY T_STACK_DIMENSIONS Out Of Memory (%ld)\n",
        	            length);
	    	    WarningExpression(e,WarningBuff);
        	    goto ErrorOut;
		    }
		    for(n=0;n<length;++n){
		        name->data[n]=0;
		    }
	        R2=NULL;
	    }else{
		    sprintf(WarningBuff,"CExpressDoEqual name %s type %d bad Create\n",R2->name,R2->type);
		    WarningExpression(e,WarningBuff);
		}
	}
	
	ret=0;
ErrorOut:
    if(R1 && R1->data){
        cFree((char *)R1->data);
    }
    if(R1 && R1->auxData){
        cFree((char *)R1->auxData);
    }
    if(R2 && R2->data){
        cFree((char *)R2->data);
    }
    if(R2 && R2->auxData){
        cFree((char *)R2->auxData);
    }
        
	return ret;
}
static struct stack *CExpressCreateNameStack(CExpressPtr e,char *name,int type)
{
	struct stack RP;
	stackHolderPtr s;
	struct stack *r;
	IconPtr p;
	IconPtr  NewNetWindowDim(IconPtr myIcon,int xsize,int ysize);
	IconPtr NewPIO3DWindow(IconPtr myIcon);


	if(!e || !name)return NULL;
	
	r=CExpressGetNameStack(e,name);
	
	if(r)return r;
	
	s = &e->nameData;

	if((type == T_STACK_CONSTANT) || (type == T_STACK_ARRAY) || (type == T_STACK_VARIABLE)){
	    zerol((char *)&RP,sizeof(struct stack));
		RP.myIcon=NULL;
		RP.type=type;
		mstrncpy(RP.name,name,256);
	    if(CExpressPush(e,T_STACK_NAME_STACK))goto ErrorOut;
	    s->stackList[s->stackCount-1]=RP;
	    return &s->stackList[s->stackCount-1];
	}else if(type == T_STACK_FILE){
		char fname[256];
	    zerol((char *)&RP,sizeof(struct stack));
	    if(e->R1.zsize > 1){
	       p = NewPIO3DWindow(NULL);
	    }else{
		   p=NewNetWindowDim(NULL,(int)e->R1.xsize,(int)e->R1.ysize);
		}
		if(p == NULL){
    		sprintf(WarningBuff,"CExpressCreateNameStack - Error Opening data window type %d\n",type);
    		WarningExpression(e,WarningBuff);
			goto ErrorOut;    	 
		}
		RP.myIcon=p;
		RP.type=type;
		mstrncpy(RP.name,name,256);
		mstrncpy(fname,name,256);
		SetWindowName(p,fname);	
	    if(CExpressPush(e,T_STACK_NAME_STACK))goto ErrorOut;
	    s->stackList[s->stackCount-1]=RP;
	    return &s->stackList[s->stackCount-1];
	}else if(type == T_STACK_TABLE){
		char fname[256];
	    zerol((char *)&RP,sizeof(struct stack));
		p=TableWindow2((int)e->R1.xsize,(int)e->R1.ysize,85,16);
		if(p == NULL){
    		sprintf(WarningBuff,"CExpressCreateNameStack - Error Opening data table type %d\n",type);
    		WarningExpression(e,WarningBuff);
			goto ErrorOut;    	 
		}
		RP.myIcon=p;
		RP.type=T_STACK_FILE;
		mstrncpy(RP.name,name,256);
		mstrncpy(fname,name,256);
		SetWindowName(p,fname);	
	    if(CExpressPush(e,T_STACK_NAME_STACK))goto ErrorOut;
	    s->stackList[s->stackCount-1]=RP;
	    return &s->stackList[s->stackCount-1];
	
	}else{
    	sprintf(WarningBuff,"CExpressCreateNameStack - bad type %d\n",type);
    	WarningExpression(e,WarningBuff);
		goto ErrorOut;
	}
ErrorOut:	
	return NULL;
}
static int CExpressFreeNameStack(CExpressPtr e,char *name)
{
	stackHolderPtr s;
	long n,nn;
	
 	if(!e || !name)return 1;

	s = &e->nameData;
 	
	if(s->stackCount > 0){
	    nn=0;
	    for(n=0;n<s->stackCount;++n){
	    	if(!strcmp(name,s->stackList[n].name)){	    	
		        if(s->stackList[n].type == T_STACK_VARIABLE){
		            if(s->stackList[n].data){
		                cFree((char *)s->stackList[n].data);
		                s->stackList[n].data=NULL;
		            }
		            if(s->stackList[n].auxData){
		                cFree((char *)s->stackList[n].auxData);
		                s->stackList[n].auxData=NULL;
		            }
	        	}	    	
	    	    continue;
	    	}else{
	    		s->stackList[nn++]=s->stackList[n];
	    	}
	    }
	    s->stackCount=nn;
	    
	    return 0;
	}
	
 	return 1;
}
struct stack *CExpressGetNameStack(CExpressPtr e,char *name)
{
	stackHolderPtr s;
	struct stack RP;
    FilePtr Files; 
	IconPtr p;
	long n;
 
 
 	if(!e || !name)return NULL;
	
	s = &e->nameData;
		
	if(s->stackCount > 0){
	    for(n=0;n<s->stackCount;++n){
	    	if(!strcmp(name,s->stackList[n].name)){
	    	    return &s->stackList[n];
	    	}
	    }
	}
	
	p=FindWindowByName(name);
	
	zerol((char *)&RP,sizeof(struct stack));
	
	if(p){
		RP.myIcon=p;
	}else{
		Files=FileManagerGet(name);
		if(!Files)goto ErrorOut;	
	}
	
	RP.type=T_STACK_FILE;
	mstrncpy(RP.name,name,256);
		
    if(CExpressPush(e,T_STACK_NAME_STACK))goto ErrorOut;
    s->stackList[s->stackCount-1]=RP;
    return &s->stackList[s->stackCount-1];
	    
ErrorOut:
	return NULL;
}

static int CExpressDoBINARY(CExpressPtr e,int function)
{
	int fcount=sizeof(funOp)/sizeof(double (*)(double));
	register double (*f)(double,double);
	struct stack *R1;
	struct stack *R2;
	struct stack *LD;
	long length,n;
	double result;
	register double *data1,*data2;
	int ret;
	
	
	ret=1;
	R1=NULL;
	R2=NULL;
	data1=NULL;
	data2=NULL;
	e->RR=NULL;
	
	if(!e)return doReturn(1);
	
	if(function < 1 || function >= fcount)return doReturn(1);
	
	LD=CExpressGetLogicalStack(e,0);
	if(!LD)return doReturn(1);
	
	
	f = funOp[function];
	
	
	if(CExpressPop(e,T_STACK_R1,1))return doReturn(1);
	
	R1 = &e->R1;
	data1=R1->data;
    if(R1->type == T_STACK_ARRAY){
	    if(!data1){
		    sprintf(WarningBuff,"CExpressDoBINARY NULL Data1 Array Pointer\n");
		    WarningExpression(e,WarningBuff);
		    return doReturn(1);
		}
	}else if(R1->type != T_STACK_CONSTANT){
        sprintf(WarningBuff,"CExpressDoBINARY Bad Type %d in R1\n",R1->type);
	    WarningExpression(e,WarningBuff);
        goto ErrorOut;
    }
	
	
    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;
    R2 = &e->R2;
    data2=R2->data;
    if(R2->type == T_STACK_ARRAY){
	    if(!data2){
		    sprintf(WarningBuff,"CExpressDoBINARY NULL Data2 Array Pointer\n");
		    WarningExpression(e,WarningBuff);
		    return doReturn(1);
		}
	}else if(R2->type != T_STACK_CONSTANT){
        sprintf(WarningBuff,"CExpressDoBINARY Bad Type %d in R2\n",R2->type);
	    WarningExpression(e,WarningBuff);
        goto ErrorOut;
    }
    
    if(LD->type == T_STACK_LARRAY){
	    if(!LD->ldata){
		    sprintf(WarningBuff,"CExpressDoBINARY NULL Logical Array Pointer\n");
		    WarningExpression(e,WarningBuff);
		    return doReturn(1);
		}
    }
	
    if(R1->type == T_STACK_CONSTANT){
	    if(R2->type == T_STACK_CONSTANT){
			if(LD->type == T_STACK_LCONSTANT){
                if(LD->logical){
                    result=(*f)(R1->value,R2->value);
                    R1->value=result;
                }
			}else if(LD->type == T_STACK_LARRAY){
	            length=LD->xsize*LD->ysize*LD->zsize;
                for(n=0;n<length;++n){
            		if(LD->ldata[n]){
                        result=(*f)(R1->value,R2->value);
                        R1->value=result;
                        break;
            		}
                }
	    	}
    	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	    }else if(R2->type == T_STACK_ARRAY){
	        length=R2->xsize*R2->ysize*R2->zsize;
            if(length <= 0)goto ErrorOut;
			if(LD->type == T_STACK_LCONSTANT){
                if(LD->logical){
                    for(n=0;n<length;++n){
                        data2[n]=((*f)(R1->value,data2[n]));
            		}
			    }		
			}else if(LD->type == T_STACK_LARRAY){
	            if(length != LD->xsize*LD->ysize*LD->zsize){
	        	    sprintf(WarningBuff,"CExpressDoBINARY Array Size Missmatch Arrary %ld Logical %ld\n",
	        	            length,LD->xsize*LD->ysize*LD->zsize);
		    	    WarningExpression(e,WarningBuff);
	        	    goto ErrorOut;
	            }
                for(n=0;n<length;++n){
        		    if(LD->ldata[n]){
                        data2[n]=((*f)(R1->value,data2[n]));
                    }
            	}
	    	}
	    	if(CExpressPush(e,T_STACK_R2))goto ErrorOut;
    		R2=NULL;
	    }
    }else if(R1->type == T_STACK_ARRAY){
	    length=R1->xsize*R1->ysize*R1->zsize;
        if(length <= 0)goto ErrorOut;
	    if(R2->type == T_STACK_CONSTANT){
			if(LD->type == T_STACK_LCONSTANT){
			    if(LD->logical){
	                for(n=0;n<length;++n){
	                    data1[n]=((*f)(data1[n],R2->value));
	            	}
            	}
			}else if(LD->type == T_STACK_LARRAY){
	            if(length != LD->xsize*LD->ysize*LD->zsize){
	        	    sprintf(WarningBuff,"CExpressDoBINARY Array Size Missmatch Arrary %ld Logical %ld\n",
	        	            length,LD->xsize*LD->ysize*LD->zsize);
		    	    WarningExpression(e,WarningBuff);
	        	    goto ErrorOut;
	            }
                for(n=0;n<length;++n){
        		    if(LD->ldata[n]){
                        data1[n]=((*f)(data1[n],R2->value));
                    }
            	}
	    	}
	    }else if(R2->type == T_STACK_ARRAY){	    
            if(length != R2->xsize*R2->ysize*R2->zsize){
        	    sprintf(WarningBuff,"CExpressDoBINARY Array Missmatch %s size: %ld %s size: %ld\n",
        	            R1->name,length,R2->name,R2->xsize*R2->ysize*R2->zsize);
	    	    WarningExpression(e,WarningBuff);
        	    goto ErrorOut;
            }
			if(LD->type == T_STACK_LCONSTANT){
			    if(LD->logical){
			        switch(function){
			        case T_PLUS:
		                for(n=0;n<length;++n){
		                    data1[n] += data2[n];
		            	}
			        break;
			        case T_MULT:
		                for(n=0;n<length;++n){
		                    data1[n] *= data2[n];
		            	}
			        break;
			        case T_DIV:
		                for(n=0;n<length;++n){
		                    if(data1[n] != 0.0){
		                        data1[n]=data2[n]/data1[n];
		                    }
		            	}
			        break;
			        case T_MINUS:
		                for(n=0;n<length;++n){
		                    data1[n]=data2[n]-data1[n];
		            	}
			        break;
			        default:
		                for(n=0;n<length;++n){
		                    data1[n]=((*f)(data1[n],data2[n]));
		            	}
	            	}
	            	
            	}
			}else if(LD->type == T_STACK_LARRAY){
	            if(length != LD->xsize*LD->ysize*LD->zsize){
	        	    sprintf(WarningBuff,"CExpressDoBINARY Array Size Missmatch Arrary %ld Logical %ld\n",
	        	            length,LD->xsize*LD->ysize*LD->zsize);
		    	    WarningExpression(e,WarningBuff);
	        	    goto ErrorOut;
	            }
                for(n=0;n<length;++n){
        		    if(LD->ldata[n]){
                        data1[n]=((*f)(data1[n],data2[n]));
                    }
            	}
	    	}
	    }
    	if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
    	R1=NULL;
    }
	ret = 0;
ErrorOut:
	if(R1 && R1->data){
	   cFree((char *)R1->data);
	}
	if(R1 && R1->auxData){
	   cFree((char *)R1->auxData);
	}
	if(R2 && R2->data){
	   cFree((char *)R2->data);
	}
	if(R2 && R2->auxData){
	   cFree((char *)R2->auxData);
	}
	return ret;
}
static int CExpressDoIf(CExpressPtr e,int flag)
{
	struct stackHolder *sl;
	struct stack *R1;
	struct stack *LD;
	struct stack *LDE;
	struct stack LDP;
	long length,n;
	double *data;
	int ret;
	
	if(!e)return doReturn(1);
	
	sl=&e->logicalData;
	
	R1=NULL;
	ret=1;
	
	zerol((char *)&LDP,sizeof(struct stack));
	
	LD=CExpressGetLogicalStack(e,0);
	if(!LD)return doReturn(1);
	
	if(flag == T_IF_START){
	
	    if(CExpressPop(e,T_STACK_R1,1))return doReturn(1);
	    
		R1 = &e->R1;
		data=R1->data;
	    if(R1->type == T_STACK_ARRAY){
		    if(!data){
			    sprintf(WarningBuff,"CExpressDoIf NULL Data1 Array Pointer (3)\n");
			    WarningExpression(e,WarningBuff);
			    goto ErrorOut;
			}
		    LDP.type = T_STACK_LARRAY;
		    LDP.xsize = R1->xsize;
		    LDP.ysize = R1->ysize;
		    LDP.zsize = R1->zsize;
		    length=LDP.xsize*LDP.ysize*LDP.zsize;
		    LDP.ldata=(short int *)cMalloc(length*sizeof(short int),2300);
		    if(!LDP.ldata){
			    sprintf(WarningBuff,"CExpressDoIf Out of Memory (4)\n");
			    WarningExpression(e,WarningBuff);
			    goto ErrorOut;
			}
			if(LD->type == T_STACK_LCONSTANT){
			    if(LD->logical){
					for(n=0;n<length;++n){
					    LDP.ldata[n]=(short)R1->data[n];
		   			} 
			    }else{
					for(n=0;n<length;++n){
					    LDP.ldata[n]=FALSE;
		   			} 
			    }
			}else if(LD->type == T_STACK_LARRAY){
			    if(length != (LD->xsize*LD->ysize*LD->zsize)){
				    sprintf(WarningBuff,"CExpressDoIf Array Size Missmatch (5)\n");
				    WarningExpression(e,WarningBuff);
				    goto ErrorOut;
				}
				for(n=0;n<length;++n){
				    if(LD->ldata[n]){
				        LDP.ldata[n]=(short)R1->data[n];
				    }else{
				        LDP.ldata[n]=FALSE;
				    }
				}
			
			}
		}else if(R1->type == T_STACK_CONSTANT){
			if(LD->type == T_STACK_LCONSTANT){
			    if(LD->logical){
			        LDP.logical=(int)R1->value;
			    }else{
			        LDP.logical=FALSE;
			    }
			    LDP.type = T_STACK_LCONSTANT;
			}else if(LD->type == T_STACK_LARRAY){
			    LDP.type = T_STACK_LARRAY;
			    LDP.xsize = LD->xsize;
			    LDP.ysize = LD->ysize;
			    LDP.zsize = LD->zsize;
			    length=LDP.xsize*LDP.ysize*LDP.zsize;
			    LDP.ldata=(short int *)cMalloc(length*sizeof(short int),8024);
			    if(!LDP.ldata){
				    sprintf(WarningBuff,"CExpressDoIf Out of Memory (6)\n");
				    WarningExpression(e,WarningBuff);
				    goto ErrorOut;
				}
				for(n=0;n<length;++n){
				    if(LD->ldata[n]){
				        LDP.ldata[n]=(int)R1->value;
				    }else{
				        LDP.ldata[n]=FALSE;
				    }
				}
			
			}
	    }else{
	        sprintf(WarningBuff,"CExpressDoIf Bad Type %d in R1 (7)\n",R1->type);
		    WarningExpression(e,WarningBuff);
	        goto ErrorOut;
	    }
		if(CExpressPushLogicalStack(e,&LDP))goto ErrorOut;
    	if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
		R1 = NULL;
	}else if(flag == T_IF_ELSE){
	    LD=CExpressGetLogicalStack(e,1);
	    if(!LD)goto ErrorOut;
	    LDE=CExpressGetLogicalStack(e,0);
	    if(!LDE)goto ErrorOut;
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;
		R1 = &e->R1;
		data=R1->data;
	    if(R1->type == T_STACK_ARRAY){
		    if(!data){
			    sprintf(WarningBuff,"CExpressDoIf NULL Data Array Pointer (8)\n");
			    WarningExpression(e,WarningBuff);
			    goto ErrorOut;
			}
			if(!LDE->ldata){
			    sprintf(WarningBuff,"CExpressDoIf NULL Logical Array Pointer (9)\n");
			    WarningExpression(e,WarningBuff);
			    goto ErrorOut;
			}
		    length=R1->xsize*R1->ysize*R1->zsize;
		    if(length != LDE->xsize*LDE->ysize*LDE->zsize){
			    sprintf(WarningBuff,"CExpressDoIf Locaical Array Missmatch (10)\n");
			    WarningExpression(e,WarningBuff);
			    goto ErrorOut;
			}
			if(LD->type == T_STACK_LCONSTANT){
			    if(LD->logical){
					for(n=0;n<length;++n){
					    LDE->ldata[n] = !R1->data[n];
		   			} 
			    }else{
					for(n=0;n<length;++n){
					    LDE->ldata[n]=FALSE;
		   			} 
			    }
			}else if(LD->type == T_STACK_LARRAY){
			    if(length != (LD->xsize*LD->ysize*LD->zsize)){
				    sprintf(WarningBuff,"CExpressDoIf Array Size Missmatch (11)\n");
				    WarningExpression(e,WarningBuff);
				    goto ErrorOut;
				}
				for(n=0;n<length;++n){
				    if(LD->ldata[n]){
				        LDE->ldata[n] = !R1->data[n];
				    }else{
				        LDE->ldata[n]=FALSE;
				    }
				}
			
			}
		}else if(R1->type == T_STACK_CONSTANT){
			if(LD->type == T_STACK_LCONSTANT){
			    if(LD->logical){
			        LDE->logical= !R1->value;
			    }else{
			        LDE->logical=FALSE;
			    }
			    LDE->type = T_STACK_LCONSTANT;
			}else if(LD->type == T_STACK_LARRAY){
		    	length=LD->xsize*LD->ysize*LD->zsize;
			    if(!LDE->ldata){
				    sprintf(WarningBuff,"CExpressDoIf NULL Logical Array Pointer (12)\n");
				    WarningExpression(e,WarningBuff);
				    goto ErrorOut;
				}
			    if(length != (LDE->xsize*LDE->ysize*LDE->zsize)){
				    sprintf(WarningBuff,"CExpressDoIf Array Size Missmatch (13)\n");
				    WarningExpression(e,WarningBuff);
				    goto ErrorOut;
				}
				for(n=0;n<length;++n){
				    if(LD->ldata[n]){
				        LDP.ldata[n] = !R1->value;
				    }else{
				        LDP.ldata[n]=FALSE;
				    }
				}
			
			}
	    }else{
	        sprintf(WarningBuff,"CExpressDoIf Bad Type %d in R1 (14)\n",R1->type);
		    WarningExpression(e,WarningBuff);
	        goto ErrorOut;
	    }
	}else if(flag == T_IF_ENDSHORT || flag == T_IF_ENDLONG){
	    LD=CExpressGetLogicalStack(e,0);
	    if(!LD)goto ErrorOut;
	    if(LD->ldata)cFree((char *)LD->ldata);
	    LD->ldata=0;
	    if(--sl->stackCount <= 0)sl->stackCount = 0;
		R1 = NULL;
	    if(flag == T_IF_ENDSHORT){
	        if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;
		    R1 = &e->R1;
	    }
	}else{
        sprintf(WarningBuff,"CExpressDoIf Unknown Flag %d (15)\n",flag);
	    WarningExpression(e,WarningBuff);
        return doReturn(1);
	}
	ret=0;
ErrorOut:
    if(R1 && R1->data){
        cFree((char *)R1->data);
        R1->data=NULL;
    }
    if(R1 && R1->auxData){
        cFree((char *)R1->auxData);
        R1->auxData=NULL;
    }
	return ret;
	
}
static int CExpressDoUnaryIncrnemt(CExpressPtr e,int tok,int flag)
{
	struct stack *name;
	struct stack *R1;
	double value;
	int ret;
	
	if(!e)return doReturn(1);
	
	ret=1;
	
	if(CExpressPop(e,T_STACK_R1,0))return doReturn(1);
	R1 = &e->R1;

    if(R1->type == T_STACK_NAME){
	    name=CExpressGetNameStack(e,R1->name);
		if(!name){
	  		sprintf(WarningBuff,"CExpressDoUnaryIncrenemt Error Trying To Find Variable  %s\n",R1->name);
	    	WarningExpression(e,WarningBuff);
	    	goto ErrorOut;
		}
		if(name->type == T_STACK_CONSTANT){
		    value=name->value;
    		if(tok == T_PLUSPLUS){
    		   name->value += 1;
    		}else{
    		   name->value -= 1;
    		}
	        R1->type = T_STACK_CONSTANT;
	        if(flag){
	            R1->value = value;
	        }else{
	            R1->value = name->value;
	        }
	        R1->data = NULL;
		}
    	if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	}
	ret=0;
ErrorOut:
	return ret;
}
static int CExpressDoUNARY(CExpressPtr e,int function)
{
	int fcount=sizeof(funOp)/sizeof(double (*)(double));
	register double (*f)(double);
	struct stack *R1;
	struct stack *LD;
	long length,n;
	double result;
	double *data1;
	int ret;
	
	
	ret=1;
	R1=NULL;
	data1=NULL;
	
	if(!e)return doReturn(1);
	
	if(function < 0 || function >= fcount)return doReturn(1);
	
	LD=CExpressGetLogicalStack(e,0);
	if(!LD)return doReturn(1);	
	
	f = (double (*)(double))funOp[function];
	
	
	if(CExpressPop(e,T_STACK_R1,1))return doReturn(1);
	
	
	R1 = &e->R1;
	data1=R1->data;
    if(R1->type == T_STACK_ARRAY){
	    if(!data1){
		    sprintf(WarningBuff,"CExpressDoUNARY NULL Data1 Array Pointer\n");
		    WarningExpression(e,WarningBuff);
		    return doReturn(1);
		}
	}else if(R1->type != T_STACK_CONSTANT){
        sprintf(WarningBuff,"CExpressDoUNARY Bad Type %d in R1\n",R1->type);
	    WarningExpression(e,WarningBuff);
        goto ErrorOut;
    }
	
	
	
    if(R1->type == T_STACK_CONSTANT){
		if(LD->type == T_STACK_LCONSTANT){
            if(LD->logical){
                result=(*f)(R1->value);
                R1->value=result;
            }
        }else if(LD->type == T_STACK_LARRAY){
            length=LD->xsize*LD->ysize*LD->zsize;
            for(n=0;n<length;++n){
        		if(LD->ldata[n]){
            		result=(*f)(R1->value);
            		R1->value=result;
            		break;
        		}
            }
        }
    }else if(R1->type == T_STACK_ARRAY){
        length=R1->xsize*R1->ysize*R1->zsize;
		if(LD->type == T_STACK_LCONSTANT){
            if(LD->logical){
                for(n=0;n<length;++n){
                    data1[n]=((*f)(data1[n]));
                }
            }
        }else if(LD->type == T_STACK_LARRAY){
            if(length != LD->xsize*LD->ysize*LD->zsize){
        	    sprintf(WarningBuff,"CExpressDoUNARY Array Size Missmatch Arrary %ld Logical %ld\n",
        	            length,LD->xsize*LD->ysize*LD->zsize);
	    	    WarningExpression(e,WarningBuff);
        	    goto ErrorOut;
            }
            for(n=0;n<length;++n){
        		if(LD->ldata[n]){
                    data1[n]=((*f)(data1[n]));
        		}
            }
        }
    }
    
    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	
	ret = 0;
ErrorOut:
	return ret;
}
static double doT_NOT(double r1)
{
	int ret;
	
	ret=(int)r1;
	ret = !ret;
	
	return (double)ret;
}
static double doT_NEGATE(double r1)
{
	int ret;
	
	ret=(int)r1;
	ret = ~ret;
	
	return (double)ret;
}
static double doT_MINUSUNARY(double r1)
{
	return (-r1);
}
static double doT_LOR(double r1,double r2)
{
	return (r1 || r2);
}
static double doT_LAND(double r1,double r2)
{
	return (r1 && r2);
}
static double doT_OR(double r1,double r2)
{
	int i1,i2;
	
	i1=(int)r1;
	i2=(int)r2;
	return (double)(i1 | i2);
}
static double doT_AND(double r1,double r2)
{
	int i1,i2;
	
	i1=(int)r1;
	i2=(int)r2;
	return (double)(i1 & i2);
}
static double doT_LEFTSHIFT(double r1,double r2)
{
	int i1,i2;
	
	i1=(int)r1;
	i2=(int)r2;
	return (double)(i2 << i1);
}
static double doT_RIGHTSHIFT(double r1,double r2)
{
	int i1,i2;
	
	i1=(int)r1;
	i2=(int)r2;
	return (double)(i2 >> i1);
}
static double doT_XOR(double r1,double r2)
{
	int i1,i2;
	
	i1=(int)r1;
	i2=(int)r2;
	return (double)(i2 ^ i1);
}
static double doT_MOD(double r1,double r2)
{
	int i1,i2;
	
	i1=(int)r1;
	i2=(int)r2;
	
	if(i1 == 0 || i2 == 0){
	    return (double)0;
	}
	
	return (double)(i2 % i1);
}
static double doT_PLUS(double r1,double r2)
{
	return r1 + r2;
}
static double doT_MINUS(double r1,double r2)
{
	return r2 - r1;
}
static double doT_MULT(double r1,double r2)
{
	return r1*r2;
}
static double doT_DIV(double r1,double r2)
{
	if(r1 == 0)return 0;
	return r2/r1;
}
static double doT_EQUALEQUAL(double r1,double r2)
{
	return (double)(r1 == r2);
}
static double doT_NOTEQUAL(double r1,double r2)
{
	return (double)(r1 != r2);
}
static double doT_LESS(double r1,double r2)
{
	return r2 < r1;
}
static double doT_GREAT(double r1,double r2)
{
	return r2 > r1 ;
}
static double doT_LESSEQUAL(double r1,double r2)
{
	return (double)(r2 <= r1);
}
static double doT_GREATEQUAL(double r1,double r2)
{
	return (double)(r2 >= r1);
}

static double max2(struct stack *s)
{
	register double r,result;
	long length,n;
	
	if(!s)return doReturn(1);
	
	length = s->xsize*s->ysize*s->zsize;
	if(!length || !s->data)return doReturn(1);
	
	result = -1e60;
	for(n=0;n<length;++n){
	    r=s->data[n];
	    if(r > result)result = r;
	}
	
	return result;
}
static double min2(struct stack *s)
{
	register double r,result;
	long length,n;
	
	if(!s)return doReturn(1);
	
	length = s->xsize*s->ysize*s->zsize;
	if(!length || !s->data)return doReturn(1);
	
	result = 1e60;
	for(n=0;n<length;++n){
	    r=s->data[n];
	    if(r < result)result = r;
	}
	
	return result;
}
static double sum(struct stack *s)
{
	register double result;
	long length,n;
	
	if(!s)return doReturn(1);
	
	length = s->xsize*s->ysize*s->zsize;
	if(!length || !s->data)return doReturn(1);
	
	result = 0;
	for(n=0;n<length;++n){
	    result += s->data[n];
	}
	
	return result;
}
int WriteToWindowAtEnd(char *name,char *buff)
{
	IconPtr p;
	CEditPtr t;
	uPort save;
	
	if(!name || !buff)return doReturn(1);
	
	p=FindWindowByName(name);	
	if(!p){
		return 1;
	}
	
	t=(CEditPtr)p->DWindow;
	if(!t || (p->WindowType != ExpressionType))return doReturn(1);
	
	uGetPort(&save);
	SetPortMyWindow(p);
	CVideoCursorAtEnd(p);
	uSetPort(&save);

	return 0;
}
static int WriteToWindowFlag;
int WriteToWindowFast(char *name,char *buff)
{
	IconPtr p;
	CEditPtr t;
	uPort save;
	int ret;
	
	if(!name || !buff)return 1;

	if(WriteToWindowFlag > 0)return 0;
	++WriteToWindowFlag;

	uGetPort(&save);
		
	ret=1;
	
	p=FindWindowByName(name);	
	if(!p){
	    p=NewCEdit(NULL,NULL);
	    if(!p)goto ErrorOut;
		p->WindowNameSet=FALSE;
		SetWindowName(p,name);	
	}
	
	t=(CEditPtr)p->DWindow;
	if(!t || (p->WindowType != ExpressionType))goto ErrorOut;
	
	SetPortMyWindow(p);
	
	EditListAppendLine(t->e,buff);
	
	ret=0;
ErrorOut:

	uSetPort(&save);
	
	WriteToWindowFlag=0;
	
	return ret;
}
int WriteToWindow(char *name,char *buff)
{
	char buff2[2048];
	uPort save;
	IconPtr p;
	CEditPtr t;
	int n,mm,m;
	int ret;
	
	
	if(!name || !buff)return doReturn(1);
	
	if(WriteToWindowFlag > 0)return 0;
	++WriteToWindowFlag;

	ret=1;
	
	uGetPort(&save);
	
	p=FindWindowByName(name);	
	if(!p){
	    p=NewCEdit(NULL,NULL);
	    if(!p)goto ErrorOut;
		p->WindowNameSet=FALSE;
		SetWindowName(p,name);	
	}
	
	t=(CEditPtr)p->DWindow;
	if(!t || (p->WindowType != ExpressionType))goto ErrorOut;
	
	m=(int)strlen(buff);
	if(m >= 2048)m=2047;
	
	mm=0;
	for(n=0;n<m;++n){
	    if(buff[n] == '\\' && buff[n+1] == 'n'){
	        ++n;
	        buff2[mm++]='\r';
	    }else if(buff[n] == '\n'){
	        buff2[mm++]='\r';
	    }else if(buff[n] == '\\' && buff[n+1] == 't'){
	        ++n;
	        buff2[mm++]='\t';
	    }else{
	        buff2[mm++]=buff[n];
	    }
	}

	buff2[mm]=0;

	SetPortMyWindow(p);
	CVideoInsertAtCursorLong(p,buff2);
	
	ret=0;
ErrorOut:

	uSetPort(&save);

    WriteToWindowFlag=0;
    
	return ret;
}
static int fprint(CExpressPtr e)
{
	unsigned char buff[256],buff2[256];
	int n,mm,m;
	
	if(!e || !e->R1.file)return doReturn(1);
	
	zerol((char *)buff,256);
	
	if(e->R1.type ==  T_STACK_STRING){
    	sprintf((char *)buff,e->R2.name,e->R1.name);
    }else{
    	sprintf((char *)buff,e->R2.name,e->R1.value);
    }
	

	m=(int)strlen((char *)buff);
	mm=0;
	for(n=0;n<m;++n){
	    if(buff[n] == '\\' && buff[n+1] == 'n'){
	        ++n;
	        buff2[mm++]=0;
	        
	        if(putString(buff2,(long)mm,e->R1.file))goto ErrorOut;
	        
	    	if(putENDOFLINE(e->R1.file))goto ErrorOut;
	    	
	    	mm=0;
	    }else if(buff[n] == '\\' && buff[n+1] == 't'){
	        ++n;
	        buff2[mm++]='\t';
	    }else{
	        buff2[mm++]=buff[n];
	    }
	}

	buff2[mm]=0;
	
	if(mm > 0)if(putString(buff2,(long)mm,e->R1.file))goto ErrorOut;
	
	return 0;
	
ErrorOut:

	return 1;
}
static int print(CExpressPtr e)
{
	char buff[256],buff2[256];
	int n,mm,m;
	
	if(!e)return doReturn(1);
	
	if(e->R1.type ==  T_STACK_STRING){
    	sprintf((char *)buff,e->R2.name,e->R1.name);
    }else{
    	sprintf((char *)buff,e->R2.name,e->R1.value);
    }
	

	m=(int)strlen(buff);
	mm=0;
	for(n=0;n<m;++n){
	    if(buff[n] == '\\' && buff[n+1] == 'n'){
	        ++n;
	        buff2[mm++]='\r';
	    }else if(buff[n] == '\\' && buff[n+1] == 't'){
	        ++n;
	        buff2[mm++]='\t';
	    }else{
	        buff2[mm++]=buff[n];
	    }
	}

	buff2[mm]=0;

	WarningBatch(buff2);
		
	return 0;
}
static int CExpressPopConstantList(CExpressPtr e,double *xx,long n,char *message)
{
	struct stack *R1;
	long nn;
	
	if(!e || !xx || !message || (n <= 0))return doReturn(1);
	
	for(nn=0;nn<n;++nn){
	    if(CExpressPop(e,T_STACK_R1,1))return doReturn(1);  
	    R1 = &e->R1;
	    if((R1->type != T_STACK_CONSTANT)){
            sprintf(WarningBuff,"%s found %d instead of T_STACK_CONSTANT\n",message,R1->type);
	        WarningExpression(e,WarningBuff);
            return doReturn(1);
		}
		xx[n-1-nn]=R1->value;
	}
	
	return 0;
}

static int LinePlotLayers(CExpressPtr e,int count)
{
	struct stack *name;
	struct stack *R1;
	double *xdata,*ydata,*zdata,*vdata;
	double value;
	int ret;
	int n,nn,n1,n2,n3;
	
	if(!e)return doReturn(1);

	R1 = &e->R1;
	
	ret=1;
	
	xdata=NULL;
	ydata=NULL;
	zdata=NULL;
	vdata=NULL;
	
	if(count < 4){
        sprintf(WarningBuff,"points needs at least 4 items\n");
        WarningExpression(e,WarningBuff);
        return doReturn(1);
	}
	
	
	xdata=(double *)cMalloc(sizeof(double)*count/3,8721);
	ydata=(double *)cMalloc(sizeof(double)*count/3,8721);
	zdata=(double *)cMalloc(sizeof(double)*count/3,8721);
	vdata=(double *)cMalloc(sizeof(double)*count/3,8721);
	
	--count;
	
	if(!xdata || !ydata || !zdata || !vdata){
        sprintf(WarningBuff,"'points' Out Of Memory\n");
        WarningExpression(e,WarningBuff);
    	goto ErrorOut;
	}
	
	nn=0;
	n1=0;
	n2=0;
	n3=0;
	for(n=0;n<count;++n){
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;
	    if(R1->type == T_STACK_CONSTANT){
	       value=R1->value;
	       if(nn == 0){
	          zdata[n3++]=value;
	          nn++;
	       }else if(nn == 1){
	          ydata[n2++]=value;
	          nn++;
	       }else if(nn == 2){
	          xdata[n1++]=value;
	          nn=0;
	       }
	    }else{
            sprintf(WarningBuff,"'points' n %d ERROR Type %d\n",n,R1->type);
            WarningExpression(e,WarningBuff);
        	goto ErrorOut;
	    }
	}
	
	if(n1 != n2 || n2 != n3 || n3 != count/3){
	    sprintf(WarningBuff,"'points' Error\n");
		for(n=0;n<count/3;++n){
	        sprintf(WarningBuff,"'points' n %d %g %g %g\n",n,xdata[n],ydata[n],zdata[n]);
	        WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}
	
	}
	
    if(CExpressPop(e,T_STACK_R1,0))goto ErrorOut;   
    R1 = &e->R1;
    if(R1->type != T_STACK_NAME){
        sprintf(WarningBuff,"Error 'points' Found Type %d in string\n",R1->type);
        WarningExpression(e,WarningBuff);
        goto ErrorOut;
	}
	
    name=CExpressGetNameStack(e,R1->name);
	if(!name){
  		sprintf(WarningBuff,"points Error Trying To Find Window  %s\n",R1->name);
    	WarningExpression(e,WarningBuff);
    	goto ErrorOut;
	}
	
	name->lineIndex=pushpopDepth(e->lineStack);

	
    name->PointLineType=LINEDATA_LIST_DATA;
	name->PointLineCount=n1;	

	zerol((char *)&e->li,sizeof(struct linedata));
	e->li.x=xdata;
	e->li.y=ydata;
	e->li.z=zdata;
	e->li.v=vdata;
	e->li.PointLineCount=n1;
	e->li.type=LINEDATA_LIST_DATA;

	if(pushpopPush(&e->li,e->lineStack))goto ErrorOut;

	xdata=NULL;
	ydata=NULL;
	zdata=NULL;
	vdata=NULL;


	ret=0;

ErrorOut:	

	if(xdata)cFree((char *)xdata);
	if(ydata)cFree((char *)ydata);
	if(zdata)cFree((char *)zdata);
	if(vdata)cFree((char *)vdata);
	return ret;
}

static int isTrue(struct stack *LD)
{
	long length,n;
	
	if(!LD)return FALSE;
	
    if(LD->type == T_STACK_LCONSTANT){
        if(LD->logical){
			return TRUE;
        }
    }else if(LD->type == T_STACK_LARRAY){
	    length=LD->xsize*LD->ysize*LD->zsize;
        for(n=0;n<length;++n){
            if(LD->ldata[n]){
				return TRUE;
            }
        }
    }	 
	      
	return FALSE; 
}
static struct stack *popName(CExpressPtr e);

static struct stack *popName(CExpressPtr e)
{
	struct stack *name;
	struct stack *R2;
	
	if(!e)return NULL;

    if(CExpressPop(e,T_STACK_R2,0))goto ErrorOut;    
    R2 = &e->R2;
    if(R2->type != T_STACK_NAME){
        sprintf(WarningBuff,"popName : Error  Found Type %d in Stack Name\n",R2->type);
        WarningExpression(e,WarningBuff);
        goto ErrorOut;
	}
    name=CExpressGetNameStack(e,R2->name);
	if(!name){
	    name=CExpressCreateNameStack(e,R2->name,T_STACK_CONSTANT);
        if(!name){
      	    sprintf(WarningBuff,"popName :  Error Trying To Create name  %s\n",R2->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}
	}		

	return name;
ErrorOut:
	return NULL;
}
static int CExpressDoFUNCTION(CExpressPtr e,int function,int count)
{
	int fcount=sizeof(functions)/sizeof(struct function);
	static struct FileListInfo info;
	struct function *f;
	struct stack *R1;
	struct stack *R2;
	struct stack *RR;
	struct stack *LD;
	struct stack *name;
	struct stack *name2;
	struct stack R,R3,R4;
	long length,length2,n,i,j,k;
	double result;
	double *data1,*data2,*datar;
	register double (*fun1)(double);
	register double (*fun2)(double,double);
	double (*fun3)(struct stack *s);
	double xx[16];
	int ret;
	char filepath[2048];
	
    double start,increment;
    long xnew,ynew;
    double *dp,*dps;
    long imin,imax,jmin,jmax;
    double value,sum;
    char path[4096];
    double x,y,z;
	struct areadata *ai;
	struct LIMITS limits,*limitsout;
	double tmin,tmax;
	int filter;
	int ret2;
	
	if(!e)return doReturn(1);
	ai = &e->ai;
	
	
	ret=1;
	R1=NULL;
	R2=NULL;
	RR=NULL;
	data1=NULL;
	data2=NULL;
	datar=NULL;
	e->RR=NULL;
	
	
	if(function < 2 || function >= fcount)return doReturn(1);
	
	LD=CExpressGetLogicalStack(e,0);
	if(!LD)return doReturn(1);
	
	f = &functions[function];
	
	if((f->pcount > 0) && (f->pcount != count)){
	    sprintf(WarningBuff,"function %s requires %d parameters not %d\n",f->name,f->pcount,count);
	    WarningExpression(e,WarningBuff);
	    return doReturn(1);
	}
	
	fun1=f->fun;
	fun2=(double (*)(double, double))f->fun;
	fun3=(double (*)(struct stack *))f->fun;
	
	if(f->popflag >= 0){
		if(CExpressPop(e,T_STACK_R1,f->popflag))return doReturn(1);
		R1 = &e->R1;
		data1=R1->data;
	    if(R1->type == T_STACK_ARRAY){
	        if(!data1){
		        sprintf(WarningBuff,"CExpressDoFUNCTION NULL Data1 Array Pointer\n");
		        WarningExpression(e,WarningBuff);
		        return doReturn(1);
		    }
		}
	}
	
	switch(f->type){
	case 1:                /* min + max */
		if(R1->type != T_STACK_ARRAY){
			sprintf(WarningBuff,"CExpressDoFUNCTION Function %s Operates only on Arrays\n",
		        	f->name);
			WarningExpression(e,WarningBuff);
		    goto ErrorOut;
		}
	  	result=(*fun3)(R1);
	  	if(R1->data){
	  	    cFree((char *)R1->data);
	  	    R1->data=NULL;
	  	}
	  	if(R1->auxData){
	  	    cFree((char *)R1->auxData);
	  	    R1->auxData=NULL;
	  	}
	  	zerol((char *)R1,sizeof(*R1));
	  	R1->type = T_STACK_CONSTANT;
	    R1->value=result;
	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	    break;
	case 2:                /* window */
	    if(R1->type != T_STACK_DIMENSIONS){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'window' Expects Dimensions Found Type %d in R1\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
	    }
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;
	    R2 = &e->R2;
	    if(R2->type != T_STACK_STRING){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'window' Expects String Found Type %d in R2\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
	    }
	    name=CExpressGetNameStack(e,R2->name);
		if(!name){
		    name=CExpressCreateNameStack(e,R2->name,T_STACK_FILE);
	        if(!name){
          	    sprintf(WarningBuff,"CExpressDoFUNCTION Error Trying To Create Window  %s\n",R1->name);
	        	WarningExpression(e,WarningBuff);
            	goto ErrorOut;
			}
		}
		name->xsize=R1->xsize;
		name->ysize=R1->ysize;
		name->zsize=R1->zsize;
	    R2 = NULL;
	    R1 = NULL;
	    break;
	case 3:                /* size(xsize,ysize) */
	    
	    if(CExpressPopConstantList(e,xx,2,"size"))goto ErrorOut;      
		R1 = &e->R1;
	    zerol((char *)&R,sizeof(struct stack));
	    R.xsize=(long)xx[0];
	    R.ysize=(long)xx[1];
	    R.zsize=1L;
	    R.type=T_STACK_DIMENSIONS;
	    *R1=R;
	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	    R1 = NULL;	    
	    R2 = NULL;	    
	    break;
	case 27:                /* size3(xsize,ysize,zsize) */
	    
	    if(CExpressPopConstantList(e,xx,3,"size3"))goto ErrorOut;      
		R1 = &e->R1;
	    zerol((char *)&R,sizeof(struct stack));
	    R.xsize=(long)xx[0];
	    R.ysize=(long)xx[1];
	    R.zsize=(long)xx[2];
	    R.type=T_STACK_DIMENSIONS;
	    *R1=R;
	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	    R1 = NULL;	    
	    R2 = NULL;	    
	    break;
	case 4:                /* sizeof */
	    if(R1->type != T_STACK_ARRAY){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'sizeof' Bad Type %d in R1\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		zerol((char *)&R,sizeof(struct stack));
	    R.xsize=R1->xsize;
	    R.ysize=R1->ysize;
	    if(R1->zsize < 1)R1->zsize=1;
	    R.zsize=R1->zsize;
	    R.type=T_STACK_DIMENSIONS;
	    R2 = &e->R2;
	    *R2=R;
	    if(CExpressPush(e,T_STACK_R2))goto ErrorOut;
	    R2 = NULL;
	    break;
	case 5:                /* xfill */
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'xfill' Bad Type %d in R1\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		increment=R1->value;
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;
	    R1 = &e->R1;
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'xfill' Bad Type %d in R2\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		start=R1->value;
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;
	    R2 = &e->R2;
	    if((R2->type != T_STACK_ARRAY) || !R2->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'xfill' Bad Type %d in R3\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		length=R2->xsize*R2->ysize*R2->zsize;
		dp=R2->data;
		for(k=0;k<R2->zsize;++k){
			for(j=0;j<R2->ysize;++j){
			    for(i=0;i<R2->xsize;++i){
			        *dp++ = (start+(double)i*increment);
			    }
			}
		}
	    if(CExpressPush(e,T_STACK_R2))goto ErrorOut;
		R1=NULL;
		R2=NULL;
	    break;
	case 6:                /* yfill */
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'yfill' Bad Type %d in R1\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		increment=R1->value;
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;
	    R1 = &e->R1;
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'yfill' Bad Type %d in R2\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		start=R1->value;
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;
	    R2 = &e->R2;
	    if((R2->type != T_STACK_ARRAY) || !R2->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'yfill' Bad Type %d in R3\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		length=R2->xsize*R2->ysize*R2->zsize;
		if(R2->zsize > 1){
			dp=R2->data;
			for(k=0;k<R2->zsize;++k){
				for(j=0;j<R2->ysize;++j){
			    	double value;
			    	value=(start+(double)j*increment);
				    for(i=0;i<R2->xsize;++i){
				        *dp++ = value;
				    }
				}
			}
		}else{
			for(j=0;j<R2->ysize;++j){
			    double value;
			    value=(start+(double)j*increment);
				dp=R2->data+length-(1+j)*R2->xsize;
			    for(i=0;i<R2->xsize;++i){
			        *dp++ = value;
			    }
			}
		}
	    if(CExpressPush(e,T_STACK_R2))goto ErrorOut;
		R1=NULL;
		R2=NULL;
	    break;
	case 28:                /* zfill */
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'zfill' Bad Type %d in R1\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		increment=R1->value;
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;
	    R1 = &e->R1;
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'zfill' Bad Type %d in R2\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		start=R1->value;
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;
	    R2 = &e->R2;
	    if((R2->type != T_STACK_ARRAY) || !R2->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'zfill' Bad Type %d in R3\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		length=R2->xsize*R2->ysize*R2->zsize;
		dp=R2->data;
		for(k=0;k<R2->zsize;++k){
		    double value;		    
		    value=(start+(double)k*increment);		    
			for(j=0;j<R2->ysize;++j){
			    for(i=0;i<R2->xsize;++i){
			        *dp++ = value;
			    }
			}
		}
	    if(CExpressPush(e,T_STACK_R2))goto ErrorOut;
		R1=NULL;
		R2=NULL;
	    break;
	case 7:                /* scale(s,xnew,ynew) */
	    
	    
	    if(CExpressPopConstantList(e,xx,2,"scale"))goto ErrorOut;      
	    
	    
		xnew=(long)xx[0];
		ynew=(long)xx[1];
		
		
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;
	    R2 = &e->R2;
	    if((R2->type != T_STACK_ARRAY) || !R2->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'scale' Bad Type %d in R3\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		if(xnew <= 0 || ynew <= 0){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'scale' Bad Factor xnew %ld ynew %ld\n",xnew,ynew);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		dp=(double *)cMalloc(xnew*ynew*sizeof(double),8025);
		if(!dp){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'scale' Out Of Memory\n");
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		scale(dp,xnew,ynew,R2->data,R2->xsize,R2->ysize);
		if(R2->data)cFree((char *)R2->data);
		R2->xsize=xnew;
		R2->ysize=ynew;
		R2->zsize=1L;
		R2->data=dp;
	    if(CExpressPush(e,T_STACK_R2))goto ErrorOut;
		R1=NULL;
		R2=NULL;
	
	
	    break;
	case 8:                /* clip(s,imin,imax,jmin,jmax) */
	    
	    
	    if(CExpressPopConstantList(e,xx,4,"clip"))goto ErrorOut;      
	    		
		imin=(long)xx[0];
		imax=(long)xx[1];
		jmin=(long)xx[2];
		jmax=(long)xx[3];
		
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;    
	    R2 = &e->R2;
	    if((R2->type != T_STACK_ARRAY) || !R2->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'clip' Bad Type %d in array\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		if(imin < 0)imin=0;
		if(jmin < 0)jmin=0;
		if(imax >= R2->xsize)imax=R2->xsize-1;
		if(jmax >= R2->ysize)jmax=R2->ysize-1;
		
		if(imin >= imax || jmin >= jmax){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'clip' Bad range imin %ld imax %ld jmin %ld jmax %ld\n",
                    imin,imax,jmin,jmax);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		xnew=imax-imin+1;
		ynew=jmax-jmin+1;
		dps=dp=(double *)cMalloc(xnew*ynew*sizeof(double),8026);
		if(!dp){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'clip' Out Of Memory\n");
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		for(j=jmin;j<=jmax;++j){
		    for(i=imin;i<=imax;++i){
		        *dp++ = R2->data[i+R2->xsize*j];
		    }
		}
		
		if(R2->data)cFree((char *)R2->data);
		
		R2->xsize=xnew;
		R2->ysize=ynew;
		R2->zsize=1L;
		R2->data=dps;
	    if(CExpressPush(e,T_STACK_R2))goto ErrorOut;
		R1=NULL;
		R2=NULL;
	
	    break;
	case 9:                /* insert(target,source,imin,jmin) */
	    
	    if(CExpressPopConstantList(e,xx,2,"insert"))goto ErrorOut;      
		imin=(int)xx[0];
		jmin=(int)xx[1];
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;
	    R1 = &e->R1;
	    if((R1->type != T_STACK_ARRAY) || !R1->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'insert' Bad Type %d in source\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;    
	    R2 = &e->R2;
	    if((R2->type != T_STACK_ARRAY) || !R2->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'insert' Bad Type %d in target\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		if(imin < 0)imin=0;
		if(jmin < 0)jmin=0;
		imax=imin+R1->xsize-1;
		jmax=jmin+R1->ysize-1;
		if(imax >= R2->xsize)imax=R2->xsize-1;
		if(jmax >= R2->ysize)jmax=R2->ysize-1;
		
		if(imin >= imax || jmin >= jmax){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'insert' Bad range imin %ld imax %ld jmin %ld jmax %ld\n",
                    imin,imax,jmin,jmax);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		dp=R2->data;
		
		for(j=jmin;j<=jmax;++j){
		    for(i=imin;i<=imax;++i){
		        dp[i+j*R2->xsize] = R1->data[(i-imin)+R1->xsize*(j-jmin)];
		    }
		}
		
		if(R1->data)cFree((char *)R1->data);
		R1->data=NULL;
		if(R1->auxData)cFree((char *)R1->auxData);
		R1->auxData=NULL;
		
	    if(CExpressPush(e,T_STACK_R2))goto ErrorOut;
	    
		R1=NULL;
		R2=NULL;
	
	
	    break;
	case 10:                /* count(target,value) */
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'count' Bad Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		value=R1->value;
				
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;    
	    R2 = &e->R2;
	    if((R2->type != T_STACK_ARRAY) || !R2->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'count' Bad Type %d in array\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		sum = 0;
		dp=R2->data;
		for(j=0;j<R2->ysize;++j){
		    for(i=0;i<R2->xsize;++i){
		        if(*dp++ == value)sum += 1;
		    }
		}
		
		if(R2->data)cFree((char *)R2->data);
		R2->data=NULL;
		if(R2->auxData)cFree((char *)R2->auxData);
		R2->auxData=NULL;
		
		R1->value=sum;
		
	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	    
		R1=NULL;
		R2=NULL;
	
	    break;
	case 31:                /* pick(target,value) */
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'pick' Bad Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		value=R1->value;
				
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;    
	    R2 = &e->R2;
	    if((R2->type != T_STACK_ARRAY) || !R2->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'pick' Bad Type %d in array\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		sum = 0;
		dp=R2->data;
		
		length=R2->xsize*R2->ysize*R2->zsize;
		j=(long)value;	
		if(j < 0 || j >= length){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'pick' Bad index %ld in array length %ld\n",j,length);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		sum=dp[j];
		
		if(R2->data)cFree((char *)R2->data);
		R2->data=NULL;
		if(R2->data)cFree((char *)R2->auxData);
		R2->auxData=NULL;
		
		R1->value=sum;
		
	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	    
		R1=NULL;
		R2=NULL;
	
	    break;
	case 32:                /* index(imax,t,tmin,tmin) */
	    
	    if(CExpressPopConstantList(e,xx,2,"index"))goto ErrorOut;      
		tmin=xx[0];
		tmax=xx[1];
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;
	    R1 = &e->R1;
	    if((R1->type != T_STACK_ARRAY) || !R1->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'index' Bad Type %d in source\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;    
	    R2 = &e->R2;
	    if((R2->type != T_STACK_CONSTANT)){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'index' Bad Type %d in target\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		imin=(long)R2->value;		
				
		length=R1->xsize*R1->ysize*R1->zsize;
		
		if(tmin >= tmax){		
			tmin=tmax=R1->data[0];
		    for(i=1;i<length;++i){
		        value = R1->data[i];
		        if(value > tmax){
		            tmax=value;
		        }else if(value < tmin){
		            tmin=value;
		        }
		    }
		    
			if(tmin >= tmax){	
			   tmax=tmin+1;
			   tmin=tmax-1;	
			}
		}
		
		if(tmin >= tmax){	
            sprintf(WarningBuff,"CExpressDoFUNCTION 'index' min and max equal\n");
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
				
	    for(i=0;i<length;++i){
	        value = (double)imin*(R1->data[i]-tmin)/(tmax-tmin);
	        /*
	        if(value < 0){
	           value=0;
	        }else if(value >= imin){
	           value = (double)(imin-1);
	        }
	        */
	        R1->data[i] = value;
	    }
				
	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	    
		R1=NULL;
		R2=NULL;
		
	    break;
	case 33:                /* histogram(out,i,j,k) */
	    
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;
	    R4 = e->R1;
		if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;    
		R3 = e->R2;
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;    
	    R2 = &e->R2;
	    if((R2->type != T_STACK_ARRAY) || !R2->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'histogram' Bad Type %d for i\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;    
	    R1 = &e->R1;
	    if((R1->type != T_STACK_ARRAY) || !R1->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'histogram' Bad Type %d for out\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
	    
	    if((R4.type == T_STACK_ARRAY) && R4.data){
		    if((R3.type != T_STACK_ARRAY) || !R3.data){
	            sprintf(WarningBuff,"CExpressDoFUNCTION 'histogram' Bad Type %d for j\n",R1->type);
		        WarningExpression(e,WarningBuff);
	            goto ErrorOut;
			}
		    length=R4.xsize*R4.ysize*R4.zsize;
		    if(length != R3.xsize*R3.ysize*R3.zsize || length != R2->xsize*R2->ysize*R2->zsize){
	            sprintf(WarningBuff,"CExpressDoFUNCTION 'histogram' size missmatch i %ld j %ld k %ld\n",
	                    R2->xsize*R2->ysize*R2->zsize,R3.xsize*R3.ysize*R3.zsize,length);
		        WarningExpression(e,WarningBuff);
	            goto ErrorOut;
		    }
	    	for(i=0;i<length;++i){
	    	    long ii,jj,kk;
	    	    ii=(long)R2->data[i];
	    	    if(ii < 0){
	    	        continue;
	    	    }else if(ii >= R1->xsize){
	    	        if(ii > R1->xsize)continue;
	    	        ii=(long)R1->xsize-1;
	    	    }
	    	    jj=(long)R3.data[i];
	    	    if(jj < 0){
	    	        continue;
	    	    }else if(jj >= R1->ysize){
	    	        if(jj > R1->ysize)continue;
	    	        jj=(long)R1->ysize-1;
	    	    }
	    	    kk=(long)R4.data[i];
	    	    if(kk < 0){
	    	        continue;
	    	    }else if(kk >= R1->zsize){
	    	        if(kk > R1->zsize)continue;
	    	        kk=(long)R1->zsize-1;
	    	    }
	    	    
	    	    R1->data[ii+jj*R1->xsize+kk*R1->xsize*R1->ysize] += 1;
	    	}
	    
	    }else if((R3.type == T_STACK_ARRAY) && R3.data){
		    length=R3.xsize*R3.ysize*R3.zsize;
		    if(length != R2->xsize*R2->ysize*R2->zsize){
	            sprintf(WarningBuff,"CExpressDoFUNCTION 'histogram' size missmatch i %ld j %ld\n",
	                    R2->xsize*R2->ysize*R2->zsize,length);
		        WarningExpression(e,WarningBuff);
	            goto ErrorOut;
		    }
	    	for(i=0;i<length;++i){
	    	    long ii,jj;
	    	    ii=(long)R2->data[i];
	    	    if(ii < 0){
	    	        continue;
	    	    }else if(ii >= R1->xsize){
	    	        if(ii > R1->xsize)continue;
	    	        ii=(long)R1->xsize-1;
	    	    }
	    	    jj=(long)R3.data[i];
	    	    if(jj < 0){
	    	        continue;
	    	    }else if(jj >= R1->ysize){
	    	        if(jj > R1->ysize)continue;
	    	        jj=(long)R1->ysize-1;
	    	    }
	    	    
	    	    R1->data[ii+(R1->ysize-jj-1)*R1->xsize] += 1;
	    	}
	    }else{
	    
		    length=R2->xsize*R2->ysize*R2->zsize;
		    
	    	for(i=0;i<length;++i){
	    	    long ii;
	    	    ii=(long)R2->data[i];
	    	    if(ii < 0){
	    	        continue;
	    	    }else if(ii >= R1->xsize){
	    	        if(ii > R1->xsize)continue;
	    	        ii=(long)R1->xsize-1;
	    	    }
	    	    
	    	    R1->data[ii] += 1;
	    	}
	    }
	    
		
		
	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	    
		R1=NULL;
		
		if(R4.data)cFree((char *)R4.data);
		R4.data=NULL;
		if(R4.auxData)cFree((char *)R4.auxData);
		R4.auxData=NULL;
		
		if(R3.data)cFree((char *)R3.data);
		R3.data=NULL;
		if(R3.auxData)cFree((char *)R3.auxData);
		R3.auxData=NULL;
		
		
	    break;
	case 34:                /* lineplotgather() */
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'Seconds' Bad Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		e->lineplotgather=(int)R1->value;
	    break;
	case 29:                /* Seconds(0) */
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'Seconds' Bad Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		R1->value=rtime();
		
	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	    
		R1=NULL;
		R2=NULL;
	
	    break;
	case 11:                /* alert(string,value) */
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'alert' Found Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
				
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;    
	    R2 = &e->R2;
	    if((R2->type != T_STACK_STRING)){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'alert' Found Type %d in string\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
        sprintf(WarningBuff,R2->name,R1->value);
	    Warning(WarningBuff);
	    
		R1=NULL;
		R2=NULL;
	    break;
	case 12:                /* print(string,value) */
	  
	    if(R1->type != T_STACK_CONSTANT && R1->type != T_STACK_STRING){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'print' Found Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
				
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;    
	    R2 = &e->R2;
	    if((R2->type != T_STACK_STRING)){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'print' Found Type %d in string\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
	    if(LD->type == T_STACK_LCONSTANT){
	        if(LD->logical){
		        print(e);    
	        }
	    }else if(LD->type == T_STACK_LARRAY){
		    length=LD->xsize*LD->ysize*LD->zsize;
	        for(n=0;n<length;++n){
	            if(LD->ldata[n]){
		            print(e);    
	                break;
	            }
	        }
	    }
			    
		R1=NULL;
		R2=NULL;
	  
	
	    break;
	case 35:                /* pathname(string) */
	    if((R1->type != T_STACK_STRING)){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'pathname' Found Type %d in string\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		mstrncpy(e->path,R1->name,2047);
		
        if(goCD(e->path)){
            if(MakeNewDirectory(e->path)){
                  sprintf(WarningBuff,"Directory %s Could Not Be Found Or Created\n",e->path);
                  WarningExpression(e,WarningBuff);
                  goto ErrorOut;
            }
        }
		
		
		break;
	case 14:                /* writesds(array) */
	    if(R1->type != T_STACK_ARRAY){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'writesds' Found Type %d in array\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
	    }
	    if(e->path[0] == 0){
 		    char *pathname;
 		    
            sprintf(WarningBuff,"CExpressDoFUNCTION Output Folder Must Be Set Before writesds\n");
	        Warning(WarningBuff);
	        
	        pathname=FileGetPath("Put Output Data Here",&info);
	        if(!pathname)goto ErrorOut;
	        
		    mstrncpy(e->path,pathname,2047);

		    if(pathname)cFree((char *)pathname);

	    }
	    mstrncpy(filepath,e->path,2047);
	    
	    if(!strcmp(ai->targetName,R1->name)){
	        zerol((char *)&limits,sizeof(struct LIMITS));
			limits.xmin=ai->xminArea;
			limits.ymin=ai->yminArea;
			limits.zmin=ai->zminArea;
			limits.xmax=ai->xmaxArea;
			limits.ymax=ai->ymaxArea;
			limits.zmax=ai->zmaxArea;
			limits.time=ai->pioTime;
			limits.vmin=ai->vminArea;
			limits.vmax=ai->vmaxArea;
			limits.pioName=(unsigned char *)&ai->pioName[0];
			limits.pioIndex=ai->pioIndex;
			limitsout=&limits;
			FilePutCommand(limitsout);
	    }else{
	    	limitsout=NULL;
	    }
	    	    	    
	    if(R1->zsize > 1){
	    	sprintf(path,"%s.%04ld.s3d%c",R1->name,(long)e->CurrentFrame,0);
	    	strncatToPath(filepath,path,2048);
	    	ret2=sds3DOut(filepath,path,R1->xsize,R1->ysize,R1->zsize,R1->data,0L,1,limitsout);
	    }else{	    
	    	sprintf(path,"%s.%04ld.s2d%c",R1->name,(long)e->CurrentFrame,0);
	    	strncatToPath(filepath,path,2048);
	    	ret2=sds2DOut(filepath,path,R1->xsize,R1->ysize,R1->data,0L,1,limitsout);
	    }
	    
	    if(limitsout && limitsout->command){
	        cFree((char *)limitsout->command);
	        limitsout->command=NULL;	        
	    }
	    if(ret2)goto ErrorOut;
	    break;
	case 15:                /* table */
	    if(R1->type != T_STACK_DIMENSIONS){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'table' Expects Dimensions Found Type %d in R1\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
	    }
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;
	    R2 = &e->R2;
	    if(R2->type != T_STACK_STRING){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'table' Expects String Found Type %d in R2\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
	    }
	    name=CExpressGetNameStack(e,R2->name);
		if(!name){
		    name=CExpressCreateNameStack(e,R2->name,T_STACK_TABLE);
	        if(!name){
          	    sprintf(WarningBuff,"CExpressDoFUNCTION Error Trying To Create Window  %s\n",R1->name);
	        	WarningExpression(e,WarningBuff);
            	goto ErrorOut;
			}
			name->xsize=R1->xsize;
			name->ysize=R1->ysize;
			name->zsize=R1->zsize;
		}
	    R2 = NULL;
	    R1 = NULL;
	    break;
	case 30:                /* cellplotset */
	    if(R1->type != T_STACK_STRING){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'cellplotset' Expects String Found Type %d in R1\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
	    }
	    name=CExpressGetNameStack(e,R1->name);
		if(!name){
	  	    sprintf(WarningBuff,"CExpressDoFUNCTION cellplotset could not find Window %s\n",R1->name);
	    	WarningExpression(e,WarningBuff);
	    	goto ErrorOut;
		}
	    R1 = NULL;
		name->PointLineType=LINEDATA_CELL_DATA;
	    break;
	case 16:                /* pointplotset */
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'pointplotset' Found Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		z=R1->value;
				
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;    
	    R1 = &e->R1;
	    if((R1->type != T_STACK_CONSTANT)){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'pointplotset' Found Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		y=R1->value;
		
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;    
	    R1 = &e->R1;
	    if((R1->type != T_STACK_CONSTANT)){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'pointplotset' Found Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		x=R1->value;
		
	    if(CExpressPop(e,T_STACK_R1,0))goto ErrorOut;   
	    R1 = &e->R1;
	    if(R1->type != T_STACK_NAME){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'pointplotset' Found Type %d in string\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
	    name=CExpressGetNameStack(e,R1->name);
		if(!name){
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'pointplotset' Error Trying To Find Window  %s\n",R1->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}
		name->PointLineType=LINEDATA_POINT_DATA;
		name->x1=x;
		name->y1=y;
		name->z1=z;
	     
	    break;
	case 17:                /* lineplotset(s,x1,y1,z1,x2,y2,z2,n) */
	
		if(CExpressPopConstantList(e,xx,7,"lineplotset"))goto ErrorOut;      

	    if(CExpressPop(e,T_STACK_R1,0))goto ErrorOut;   
	    R1 = &e->R1;
	    if(R1->type != T_STACK_NAME){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'lineplotset' Found Type %d in string\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
	    name=CExpressGetNameStack(e,R1->name);
		if(!name){
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'lineplotset' Error Trying To Find Window  %s\n",R1->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}
		name->PointLineType=LINEDATA_LINE_DATA;
		name->x1=xx[0];
		name->y1=xx[1];
		name->z1=xx[2];
		name->x2=xx[3];
		name->y2=xx[4];
		name->z2=xx[5];
		name->PointLineCount=(int)xx[6];
		
	    break;
	case 18:                /* pointplotout */
		R1 = &e->R1;
	    if(R1->type != T_STACK_NAME){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'pointplotout' a2 Found Type %d in string\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
	    if(CExpressPop(e,T_STACK_R2,0))goto ErrorOut;   
	    R2 = &e->R2;
	    if(R2->type != T_STACK_NAME){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'pointplotout' a1 Found Type %d in string\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		R1->type=LINEDATA_POINT_DATA;
		mstrncpy(R1->pioName,R2->name,64);
	    if(CExpressPush(e,T_STACK_OUT_STACK))goto ErrorOut;
	    break;
	case 19:                /* lineplotout(v,d) */
		R1 = &e->R1;
	    if(R1->type != T_STACK_NAME){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'lineplotout' a2 Found Type %d in string\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
	    if(CExpressPop(e,T_STACK_R2,0))goto ErrorOut;   
	    R2 = &e->R2;
	    if(R2->type != T_STACK_NAME){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'lineplotout' a1 Found Type %d in string\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		R1->type=LINEDATA_LINE_DATA;
		mstrncpy(R1->pioName,R2->name,64);
	    if(CExpressPush(e,T_STACK_OUT_STACK))goto ErrorOut;
	    break;
	case 20:                /* areaplotset(s,x1,y1,z1,x2,y2,z2,n1,n2,n3) */
	    
	  	if(CExpressPopConstantList(e,xx,9,"areaplotset"))goto ErrorOut;      
	    		
	    if(CExpressPop(e,T_STACK_R1,0))goto ErrorOut;   
	    R1 = &e->R1;
	    if(R1->type != T_STACK_NAME){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'areaplotset' Found Type %d in string\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
	    name=CExpressGetNameStack(e,R1->name);
		if(!name){
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'areaplotset' Error Trying To Find Window  %s\n",R1->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}
		name->PointLineType=AREADATA_AREA_DATA;
		name->x1=xx[0];
		name->y1=xx[1];
		name->z1=xx[2];
		name->x2=xx[3];
		name->y2=xx[4];
		name->z2=xx[5];
		name->n1=(int)xx[6];
		name->n2=(int)xx[7];
		name->n3=(int)xx[8];
	    break;
	case 21:                /* d=xflip(s) */
		
		
	    if((R1->type != T_STACK_ARRAY) || !R1->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'flipx' Bad Type %d in array\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		xnew=R1->xsize;
		ynew=R1->ysize;
		dps=dp=(double *)cMalloc(xnew*ynew*sizeof(double),8026);
		if(!dp){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'flipx' Out Of Memory\n");
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		for(j=0;j<ynew;++j){
		    for(i=0;i<xnew;++i){
		        dp[xnew-1-i+xnew*j] = R1->data[i+xnew*j];
		    }
		}
		
		if(R1->data)cFree((char *)R1->data);
		
		R1->xsize=xnew;
		R1->ysize=ynew;
		R1->zsize=1L;
		R1->data=dps;
	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
		R1=NULL;
		R2=NULL;
	    break;
	case 22:                /* d=yflip(s) */
		
		
	    if((R1->type != T_STACK_ARRAY) || !R1->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'flipy' Bad Type %d in array\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		xnew=R1->xsize;
		ynew=R1->ysize;
		dps=dp=(double *)cMalloc(xnew*ynew*sizeof(double),8026);
		if(!dp){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'flipy' Out Of Memory\n");
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		for(j=0;j<ynew;++j){
		    for(i=0;i<xnew;++i){
		        dp[i+xnew*(ynew-1-j)] = R1->data[i+xnew*j];
		    }
		}
		
		if(R1->data)cFree((char *)R1->data);
		
		R1->xsize=xnew;
		R1->ysize=ynew;
		R1->zsize=1L;
		R1->data=dps;
	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
		R1=NULL;
		R2=NULL;
	    break;
	case 23:                /* plotsetrange(s,dname,tname,xmin,ymin,zmin,xmax,ymax,zmax) */
	
	  	if(CExpressPopConstantList(e,xx,6,"plotsetrange"))goto ErrorOut;      
				
	    zerol((char *)ai,sizeof(struct areadata));
	    
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;   
	    	    
	    R1 = &e->R1;
	    if(R1->type == T_STACK_CONSTANT){
			ai->pioTime=R1->value;		
	    }else if((R1->type == T_STACK_ARRAY) && R1->data){
			ai->pioTime=R1->pioTime;		
			if(R1->data)cFree((char *)R1->data);	    
			if(R1->auxData)cFree((char *)R1->auxData);	    
	    }else{
            sprintf(WarningBuff,"CExpressDoFUNCTION 'plotsetrange' Bad Type %d in array\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
						
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;
	    R2 = &e->R2;
	    if(R2->type != T_STACK_STRING){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'plotsetrange' Expects String Found Type %d in R2\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
	    }

        mstrncpy(ai->pioName,R2->name,64);
		ai->pioIndex=R2->pioIndex;
		
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;   
	    R1 = &e->R1;
	    if((R1->type != T_STACK_ARRAY) || !R1->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'plotsetrange' Bad Type %d in array\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
	    name=CExpressGetNameStack(e,R1->name);
		if(!name){
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'plotsetrange' Error Trying To Find Window  %s\n",R1->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}		
		
		ai->vminArea =  1e60;
		ai->vmaxArea = -1e60;

		for(j=0;j<R1->xsize*R1->ysize*R1->zsize;++j){
		    double r;
		    r = R1->data[j];
		    if(r > ai->vmaxArea)ai->vmaxArea = r;
		    if(r < ai->vminArea)ai->vminArea = r;
		}
		
        mstrncpy(ai->targetName,name->name,256);
		ai->xminArea=xx[0];
		ai->yminArea=xx[1];
		ai->zminArea=xx[2];
		ai->xmaxArea=xx[3];
		ai->ymaxArea=xx[4];
		ai->zmaxArea=xx[5];
		ai->CurrentFrame=e->CurrentFrame;
		ai->type = FRAME_DATA_FLOAT;
		if(SendMessageByName(name->name,MessageType_SET_AREA_RANGE,ai)){
		    if(FindWindowByName(name->name) || FileManagerGet(name->name)){
				sprintf(WarningBuff,"plotsetrange - file (%s)  Error Setting Area Range\n",name->name);
				WarningExpression(e,WarningBuff);
	        	goto ErrorOut;
        	}
		}
		break;
	case 24:                /* d=rotate(s) */
		
		
	    if((R1->type != T_STACK_ARRAY) || !R1->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'rotate' Bad Type %d in array\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		xnew=R1->ysize;
		ynew=R1->xsize;
		dps=dp=(double *)cMalloc(xnew*ynew*sizeof(double),8026);
		if(!dp){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'rotate' Out Of Memory\n");
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		for(j=0;j<ynew;++j){
		    for(i=0;i<xnew;++i){
		        dp[i+xnew*(ynew-1-j)] = R1->data[j+ynew*i];
		    }
		}
		
		if(R1->data)cFree((char *)R1->data);
		
		R1->xsize=xnew;
		R1->ysize=ynew;
		R1->zsize=1L;
		R1->data=dps;
	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
		R1=NULL;
		R2=NULL;
		break;
	case 25:                /* plotsetlimits(s,dname,tname) */
			
	    zerol((char *)ai,sizeof(struct areadata));
	    
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;   
	    R1 = &e->R1;
	    if((R1->type != T_STACK_ARRAY) || !R1->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'plotsetlimits' Bad Type %d in array\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
					    
		ai->pioTime=R1->pioTime;
				
		if(R1->data)cFree((char *)R1->data);
		if(R1->auxData)cFree((char *)R1->auxData);
		    
	    limits.CurrentFrame=e->CurrentFrame;
	    
	    if(R1->zsize > 1){
	        limits.type=LIMIT_TYPE3D;
	    }else{
	        limits.type=LIMIT_TYPE2D;
	    }
	    
		if(SendMessageByName(R1->name,MessageType_GET_LIMITS_DATA,&limits)){
			sprintf(WarningBuff,"plotsetlimits - file (%s)  Limits data Error\n",R1->name);
			WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
	    	    
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;
	    R2 = &e->R2;
	    if(R2->type != T_STACK_STRING){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'plotsetlimits' Expects String Found Type %d in R2\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
	    }

        mstrncpy(ai->pioName,R2->name,64);
		ai->pioIndex=R2->pioIndex;

	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;   
	    R1 = &e->R1;
	    if((R1->type != T_STACK_ARRAY) || !R1->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'plotsetlimits' Bad Type %d in array\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
	    name=CExpressGetNameStack(e,R1->name);
		if(!name){
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'plotsetlimits' Error Trying To Find Window  %s\n",R1->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}		
		
		ai->vminArea =  1e60;
		ai->vmaxArea = -1e60;

		for(j=0;j<R1->xsize*R1->ysize*R1->zsize;++j){
		    double r;
		    r = R1->data[j];
		    if(r > ai->vmaxArea)ai->vmaxArea = r;
		    if(r < ai->vminArea)ai->vminArea = r;
		}
		
        mstrncpy(ai->targetName,name->name,256);
		ai->xminArea=limits.xmin;
		ai->yminArea=limits.ymin;
		ai->zminArea=limits.zmin;
		ai->xmaxArea=limits.xmax;
		ai->ymaxArea=limits.ymax;
		ai->zmaxArea=limits.zmax;
		ai->pioTime=limits.time;
		ai->CurrentFrame=e->CurrentFrame;
		ai->type = FRAME_DATA_FLOAT;
		if(SendMessageByName(name->name,MessageType_SET_AREA_RANGE,ai)){
		    if(FindWindowByName(name->name) || FileManagerGet(name->name)){
			    sprintf(WarningBuff,"plotsetlimits - file (%s)  Error Setting Area Range\n",name->name);
			    WarningExpression(e,WarningBuff);
                goto ErrorOut;
            }
		}
		break;
	case 26:                /* gplane(s,x1,y1,z1,x2,y2,z2,x3,y3,z3,nx,ny) */
	
	  	if(CExpressPopConstantList(e,xx,11,"gplane"))goto ErrorOut;      
	    		
	    if(CExpressPop(e,T_STACK_R1,0))goto ErrorOut;   
	    R1 = &e->R1;
	    if(R1->type != T_STACK_NAME){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'gplane' Found Type %d in string\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
	    name=CExpressGetNameStack(e,R1->name);
		if(!name){
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'gplane' Error Trying To Find Window  %s\n",R1->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}
		name->PointLineType=GENERAL_PLANE_DATA;
		name->x1=xx[0];
		name->y1=xx[1];
		name->z1=xx[2];
		name->x2=xx[3];
		name->y2=xx[4];
		name->z2=xx[5];
		name->x3=xx[6];
		name->y3=xx[7];
		name->z3=xx[8];
		name->n1=(int)xx[9];
		name->n2=(int)xx[10];
		break;
	case 36: 
		LinePlotLayers(e,count);	
		break;
	case 37:   /* copy(s,ns,d,nd,nn) */
        
        {
 			struct stack *name2;
 			long nn1,nn2;
            long nn,nd,ns,n;
            
            
        
		    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;    
		    R1 = &e->R1;
	 	    if(R1->type != T_STACK_CONSTANT){
	            sprintf(WarningBuff,"CExpressDoFUNCTION 'copy' Bad Type %d in value\n",R1->type);
		        WarningExpression(e,WarningBuff);
	            goto ErrorOut;
			}
			nn=(long)R1->value;
					
		    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;    
		    R1 = &e->R1;
	 	    if(R1->type != T_STACK_CONSTANT){
	            sprintf(WarningBuff,"CExpressDoFUNCTION 'copy' Bad Type %d in value\n",R1->type);
		        WarningExpression(e,WarningBuff);
	            goto ErrorOut;
			}
			nd=(long)R1->value;
					
		    if(CExpressPop(e,T_STACK_R2,0))goto ErrorOut;    
		    R2 = &e->R2;
		    if((R2->type != T_STACK_NAME)){
	            sprintf(WarningBuff,"CExpressDoFUNCTION 'copy' Bad Type %d not T_STACK_NAME\n",R2->type);
		        WarningExpression(e,WarningBuff);
	            goto ErrorOut;
			}
			
		    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;    
		    R1 = &e->R1;
	 	    if(R1->type != T_STACK_CONSTANT){
	            sprintf(WarningBuff,"CExpressDoFUNCTION 'copy' Bad Type %d in value\n",R1->type);
		        WarningExpression(e,WarningBuff);
	            goto ErrorOut;
			}
			
			ns=(long)R1->value;
			
		    if(CExpressPop(e,T_STACK_R1,0))goto ErrorOut;    
		    R1 = &e->R1;
		    if((R1->type != T_STACK_NAME)){
	            sprintf(WarningBuff,"CExpressDoFUNCTION 'copy' Bad Type %d not T_STACK_NAME\n",R1->type);
		        WarningExpression(e,WarningBuff);
	            goto ErrorOut;
			}

		    name=CExpressGetNameStack(e,R1->name);
			if(!name){
	      		sprintf(WarningBuff,"CExpressDoFUNCTION 'copy' Error Trying To Find Window  %s\n",R1->name);
	        	WarningExpression(e,WarningBuff);
	        	goto ErrorOut;
			}
			
		    name2=CExpressGetNameStack(e,R2->name);
			if(!name2){
	      		sprintf(WarningBuff,"CExpressDoFUNCTION 'copy' Error Trying To Find Window  %s\n",R2->name);
	        	WarningExpression(e,WarningBuff);
	        	goto ErrorOut;
			}
			
			
			if(name->type == T_STACK_CONSTANT){
			    if(name2->type == T_STACK_CONSTANT){
			        name2->value=name->value;
			    }else if(name2->type != T_STACK_VARIABLE || !name2->data){
		      		sprintf(WarningBuff,"CExpressDoFUNCTION 'copy' Bad Data Type %ld\n",(long)name2->type);
		        	WarningExpression(e,WarningBuff);
		        	goto ErrorOut;
			    }
				nn2=name2->xsize*name2->ysize*name2->zsize;
				for(n=0;n<nn;++n){
				    if((nd+n < nn2)){
						name2->data[nd+n]=name->value;
					}
				}
				
			}else if(name2->type == T_STACK_CONSTANT){
				if(name->type != T_STACK_VARIABLE || !name->data){
		      		sprintf(WarningBuff,"CExpressDoFUNCTION 'copy' Bad Data Type %ld\n",(long)name->type);
		        	WarningExpression(e,WarningBuff);
		        	goto ErrorOut;
				}
			    nn1=name->xsize*name->ysize*name->zsize;
				for(n=0;n<nn;++n){
				    if((ns+n < nn1)){
						name2->value=name->data[ns+n];
					}
				}
			}else{			
				if(name->type != T_STACK_VARIABLE || name2->type != T_STACK_VARIABLE || !name->data || !name2->data){
		      		sprintf(WarningBuff,"CExpressDoFUNCTION 'copy' Error Needs Stack Variables\n");
		        	WarningExpression(e,WarningBuff);
		        	goto ErrorOut;
				}
				
				 nn1=name->xsize*name->ysize*name->zsize;
				 nn2=name2->xsize*name2->ysize*name2->zsize;
						 
				 
				for(n=0;n<nn;++n){
				    if((nd+n < nn2) && (ns+n < nn1)){
						name2->data[nd+n]=name->data[ns+n];
					}
				}
			}

			
			R1=NULL;
			R2=NULL;
        }
                                
		break;
	case 38:    /* sagewrite(s:data,newData) */
	
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;   
	    R1 = &e->R1;
	    if((R1->type != T_STACK_ARRAY) || !R1->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'sagewrite' Bad Type %d in array\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
					    
	    	    	    
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;   
	    R2 = &e->R2;
	    if((R2->type != T_STACK_ARRAY) || !R2->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'sagewrite' Bad Type %d in array\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
	    name=CExpressGetNameStack(e,R2->name);
		if(!name){
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'sagewrite' Error Trying To Find Window  %s\n",R2->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}		
		
		if((name->type == T_STACK_FILE) && (name->PointLineType == LINEDATA_CELL_DATA)){
		    struct SetFrameData  sd;
			zerol((char *)&sd,sizeof(struct SetFrameData));
		    if(R2->pioName[0]){
		        sd.pioIndex=R2->pioIndex;
		        mstrncpy(sd.pioName,R2->pioName,64);
		    }
			sd.CurrentFrame=e->CurrentFrame;
			sd.type = LINEDATA_CELL_DATA;
			sd.data=R1->data;
			sd.xsize=R1->xsize;
			sd.ysize=R1->ysize;
			sd.zsize=R1->zsize;
			if(SendMessageByName(name->name,MessageType_SET_CELL_DATA,&sd)){
				sprintf(WarningBuff,"sagewrite - file (%s)  MessageType_SET_CELL_DATA Error Getting Area Data\n",name->name);
				WarningExpression(e,WarningBuff);
				return doReturn(1);
			}
		}
		
		break;
	case 39:    /*s=map(a,a) */
	    if((R1->type != T_STACK_ARRAY) || !R1->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'map' Bad Type %d in R1\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;
	    R2 = &e->R2;
				
	    if((R2->type != T_STACK_ARRAY) || !R2->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'map' Bad Type %d in R2\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		length2=R2->xsize*R2->ysize*R2->zsize;
		
		length=R1->xsize*R1->ysize*R1->zsize;
		
		dp=(double *)cMalloc(length*sizeof(double),8027);
		if(!dp){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'map' Out Of Memory\n");
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		zerol((char *)dp,length*sizeof(double));

		for(k=0;k<length;++k){
		    long nn;
			nn=(long)R1->data[k];
			if(nn >= 0 && nn < length2){
				dp[k]=R2->data[nn];
			}	    
		}
		
		if(R2->data)cFree((char *)R2->data);
		R2->data=NULL;
		if(R2->auxData)cFree((char *)R2->auxData);
		R2->auxData=NULL;
		
		if(R1->data)cFree((char *)R1->data);
		R1->data=dp;
		if(R1->auxData)cFree((char *)R1->auxData);
		R1->auxData=dp;

	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
		R1=NULL;
		R2=NULL;	
		break;
	case 40:                /* fopen(string) */
	    if((R1->type != T_STACK_STRING)){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'fopen' Found Type %d in string\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}

		if(R1->data)cFree((char *)R1->data);
		R1->data=NULL;
		if(R1->auxData)cFree((char *)R1->auxData);
		R1->auxData=NULL;
				
		if(isTrue(LD)){
			R1->file=fopen(R1->name,"r+b");
			if(!R1->file){
	 			R1->file=fopen(R1->name,"wb");
	 			if(!R1->file){
	           		sprintf(WarningBuff,"CExpressDoFUNCTION 'fopen' error opening %s to write\n",R1->name);
		        	WarningExpression(e,WarningBuff);
	            	goto ErrorOut;
	            }
			}
			
			fseek(R1->file,0L,2);
			
	    }
		
		R1->type = T_STACK_CONSTANT;
							
		if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
		
		R1=NULL;
	break;
	case 41:                /* fclose(file) */
	    if(CExpressPop(e,T_STACK_R1,0))goto ErrorOut;    
	    R1 = &e->R1;
	    if(R1->type != T_STACK_NAME){
	        sprintf(WarningBuff,"Error 'fclose' Found Type %d in Stack Name\n",R1->type);
	        WarningExpression(e,WarningBuff);
	        goto ErrorOut;
		}
	    name=CExpressGetNameStack(e,R1->name);
		if(!name){
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'fclose' Error Trying To Find File  %s\n",R1->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}		
		if(isTrue(LD)){
		    if(name->file){
				if(fclose(name->file)){
		            sprintf(WarningBuff,"CExpressDoFUNCTION 'fclose' Error Closing file\n");
			        WarningExpression(e,WarningBuff);
		            goto ErrorOut;
				}
			}
		}
		name->file=NULL;
	break;
	case 42:                /* fprint(file,string,value) */
	  {
	      char names[256];
	      int type;
	    if((R1->type != T_STACK_CONSTANT) && (R1->type != T_STACK_STRING)){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'fprint' Found Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
				
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;    
	    R2 = &e->R2;
	    if((R2->type != T_STACK_STRING)){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'fprint' Found Type %d in string\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		value=R1->value;
		type=R1->type;
		mstrncpy(names,R1->name,256);
		
	    if(CExpressPop(e,T_STACK_R1,0))goto ErrorOut;    
	    R1 = &e->R1;
	    if(R1->type != T_STACK_NAME){
	        sprintf(WarningBuff,"Error 'fprint' Found Type %d in Stack Name\n",R1->type);
	        WarningExpression(e,WarningBuff);
	        goto ErrorOut;
		}
	    name=CExpressGetNameStack(e,R1->name);
		if(!name){
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'fprint' Error Trying To Find File  %s\n",R1->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}		
		
		
		R1->value=value;
		R1->file=name->file;
		R1->type=type;
		mstrncpy(R1->name,names,256);

	    if(R1->file){		
	        if(isTrue(LD)){
				fprint(e);    
			}
	    }
			    
		R1=NULL;
		R2=NULL;
	
	  }
	    break;
	case 43:                /* ctime(0) */
        {
            time_t now;
            
            time(&now);
            
            mstrncpy(e->ch,ctime(&now),256);
            
            e->ch[24]=0;
            
	    	if(CExpressPush(e, T_STACK_STRING))goto ErrorOut;
	    	
	    	R1=NULL;
            
        }
		
		
		break;
	case 44:                /* setoption(s,name,v) */
	  {
	      char names[256];
	      double *data;
	      int type;
	      
	    data=NULL;
	    length=0;
	    if(R1->type == T_STACK_ARRAY){
	        if(!R1->data){
            	sprintf(WarningBuff,"CExpressDoFUNCTION 'setoption' data Pointer\n");
	        	WarningExpression(e,WarningBuff);
            	goto ErrorOut;
            }
            data=R1->data;
	    	length=R1->xsize*R1->ysize*R1->zsize;
		}else{
		    if((R1->type != T_STACK_CONSTANT) && (R1->type != T_STACK_STRING)){
	            sprintf(WarningBuff,"CExpressDoFUNCTION 'setoption' Found Type %d in value\n",R1->type);
		        WarningExpression(e,WarningBuff);
	            goto ErrorOut;
			}
		}
		value=R1->value;
		type=R1->type;
		mstrncpy(names,R1->name,256);
		
				
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;    
	    R2 = &e->R2;
	    if((R2->type != T_STACK_STRING)){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'setoption' Found Type %d in string\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		
	    if(CExpressPop(e,T_STACK_R1,0))goto ErrorOut;    
	    R1 = &e->R1;
	    if(R1->type != T_STACK_NAME){
	        sprintf(WarningBuff,"CExpressDoFUNCTION 'setoption' Error  Found Type %d in Stack Name\n",R1->type);
	        WarningExpression(e,WarningBuff);
	        goto ErrorOut;
		}
	    name=CExpressGetNameStack(e,R1->name);
		if(!name){
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'setoption' Error Trying To Find Item  %s\n",R1->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}		
		
		if(!strcmp(R2->name,"integrate")){
			name->integrate=(int)value;
		}else if(!strcmp(R2->name,"line")){
			name->PointLineType=LINEDATA_LINE_DATA;
			name->x1=0;
			name->y1=0;
			name->z1=0;
			name->x2=1;
			name->y2=1;
			name->z2=1;
			name->PointLineCount=0;
		}else if(!strcmp(R2->name,"xdata")){
			if(!data || length != name->xsize*name->ysize*name->zsize){
      			sprintf(WarningBuff,"CExpressDoFUNCTION 'setoption' Error Trying To Set xdata for Item  %s\n",R1->name);
        		WarningExpression(e,WarningBuff);
        		goto ErrorOut;
			}
		    name->auxData=data;
		    name->auxLength=length;
		}else{
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'setoption' Unknown Option  %s\n",R2->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}
		R1=NULL;
		R2=NULL;	
	  }
		break;
	case 45:                /* getoption(s,name) */
	  {
				
	    R1 = &e->R1;
	    if((R1->type != T_STACK_STRING)){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'getoption' Found Type %d in string\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
				
	    if(CExpressPop(e,T_STACK_R2,0))goto ErrorOut;    
	    R2 = &e->R2;
	    if(R2->type != T_STACK_NAME){
	        sprintf(WarningBuff,"CExpressDoFUNCTION 'getoption' Error  Found Type %d in Stack Name\n",R1->type);
	        WarningExpression(e,WarningBuff);
	        goto ErrorOut;
		}
	    name=CExpressGetNameStack(e,R2->name);
		if(!name){
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'getoption' Error Trying To Find Item  %s\n",R1->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}		
		
		if(!strcmp(R1->name,"integral")){
		    R1->type = T_STACK_CONSTANT;
			R1->value=name->integral;
	    	if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
		}else if(!strcmp(R1->name,"xsize")){
		    R1->type = T_STACK_CONSTANT;
			R1->value=name->xsize;
	    	if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
		}else if(!strcmp(R1->name,"ysize")){
		    R1->type = T_STACK_CONSTANT;
			R1->value=name->ysize;
	    	if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
		}else if(!strcmp(R1->name,"zsize")){
		    R1->type = T_STACK_CONSTANT;
			R1->value=name->zsize;
	    	if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
		}else if(!strcmp(R1->name,"xdata")){
		    if(!name->auxData || name->auxLength <= 0){
      			sprintf(WarningBuff,"CExpressDoFUNCTION 'getoption' No Line data Found  %s\n",R2->name);
        		WarningExpression(e,WarningBuff);
        		goto ErrorOut;
		    }
		    
			R1->auxLength=0;
		    R1->auxData=NULL;
		    R1->data =  (double *)cMalloc(name->auxLength*sizeof(double),8032);
		    if(!R1->data){
				sprintf(WarningBuff,"CExpressPop - variable (%s) of type %d out of memory\n",name->name,name->type);
				WarningExpression(e,WarningBuff);
				return doReturn(1);
		    }
		    for(n=0;n<name->auxLength;++n)R1->data[n]=name->auxData[n];
		    
		    R1->type = T_STACK_ARRAY;
		    R1->xsize = name->xsize;
		    R1->ysize = name->ysize;
		    R1->zsize = name->zsize;

	    	if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
		}else{
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'getoption' Unknown Option  %s\n",R2->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}
		R1=NULL;
		R2=NULL;	
	  }
		break;
	case 46:                /* random(0|a) */
        {
	    	R1 = &e->R1;
	    	
	    	length=R1->xsize*R1->ysize*R1->zsize;
	    	if(R1->type == T_STACK_ARRAY && R1->data && length){
		    	for(n=0;n<length;++n)R1->data[n]=((double)rand())/RAND_MAX;
	    		if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	    	}else{
		   	 	R1->type = T_STACK_CONSTANT;
				R1->value=((double)rand())/RAND_MAX;
	    		if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	    	}
	    	
	    	R1=NULL;
            
        }
        break;
        
        
	case 47:                /* free(file) */
	    if(CExpressPop(e,T_STACK_R1,0))goto ErrorOut;    
	    R1 = &e->R1;
	    if(R1->type != T_STACK_NAME){
	        sprintf(WarningBuff,"Error 'free' Found Type %d in Stack Name\n",R1->type);
	        WarningExpression(e,WarningBuff);
	        goto ErrorOut;
		}
		if(CExpressFreeNameStack(e,R1->name))goto ErrorOut;
			
	break;
	
	case 49:                /* fft(real,imag,direction,filter) */
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'fft' Bad Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		filter=(int)R1->value;
		
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;
	    
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'fft' Bad Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		value=R1->value;
				
	    if(CExpressPop(e,T_STACK_R2,0))goto ErrorOut;    
	    R2 = &e->R2;
	    if(R2->type != T_STACK_NAME){
	        sprintf(WarningBuff,"CExpressDoFUNCTION 'fft' Error  Found Type %d in Stack Name\n",R2->type);
	        WarningExpression(e,WarningBuff);
	        goto ErrorOut;
		}
	    name=CExpressGetNameStack(e,R2->name);
		if(!name){
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'fft' Error Trying To Find Item  %s\n",R2->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}		
		
	    length=name->xsize*name->ysize*name->zsize;
		
	    if(!name->data ||  length <= 0){
  			sprintf(WarningBuff,"CExpressDoFUNCTION 'fft' No Imaginary data Found  %s\n",R2->name);
    		WarningExpression(e,WarningBuff);
    		goto ErrorOut;
	    }
	    
	    
	    if(CExpressPop(e,T_STACK_R2,0))goto ErrorOut;    
	    R2 = &e->R2;
	    if(R2->type != T_STACK_NAME){
	        sprintf(WarningBuff,"CExpressDoFUNCTION 'fft' Error  Found Type %d in Stack Name\n",R2->type);
	        WarningExpression(e,WarningBuff);
	        goto ErrorOut;
		}
	    name2=CExpressGetNameStack(e,R2->name);
		if(!name2){
      		sprintf(WarningBuff,"CExpressDoFUNCTION 'fft' Error Trying To Find Item  %s\n",R2->name);
        	WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}		
	
		
	    if(!name2->data ||  name2->xsize*name2->ysize*name2->zsize <= 0){
  			sprintf(WarningBuff,"CExpressDoFUNCTION 'fft' No Imaginary data Found  %s\n",R2->name);
    		WarningExpression(e,WarningBuff);
    		goto ErrorOut;
	    }
	    
	    if(length != name2->xsize*name2->ysize*name2->zsize){
  			sprintf(WarningBuff,"CExpressDoFUNCTION 'fft' Data Size Mismatch %s\n",R2->name);
    		WarningExpression(e,WarningBuff);
    		goto ErrorOut;
	    }
	    
	    if(name->type != T_STACK_VARIABLE){
	   		WarningExpression(e,"Use Local Array For Imaginary Part of FFT\n");
    		goto ErrorOut;
	    }
	    
	    if(name2->type != T_STACK_VARIABLE){
	   		WarningExpression(e,"Use Local Array For Real Part of FFT\n");
    		goto ErrorOut;
	    }
	    
	    if(doFFT3D(name2->data,name->data,name2->xsize,name2->ysize,name2->zsize,(int)value,filter))goto ErrorOut;
	        	
		R1=NULL;
		R2=NULL;	
	
	    break;
	
	case 50:                /* play(y,dt) */
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'play' Bad Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		value=R1->value;
				
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;    
	    R2 = &e->R2;
	    if((R2->type != T_STACK_ARRAY) || !R2->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'play' Bad Type %d in array\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		dp=R2->data;
		
		length=R2->xsize*R2->ysize*R2->zsize;
		
		if(playSound(dp,length,value))goto ErrorOut;
		
		if(R2->data)cFree((char *)R2->data);
		R2->data=NULL;
		if(R2->data)cFree((char *)R2->auxData);
		R2->auxData=NULL;
		
		R1=NULL;
		R2=NULL;
	
	    break;
	
           
	case 51:                /* a=record(time,samples) */
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'record' Bad Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		length=(long)R1->value;
				
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;    
	    R2 = &e->R2;
	    if(R2->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'record' Bad Type %d in array\n",R2->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		value=R2->value;
		
		dp=(double *)cMalloc(length*sizeof(double),8764);
	    if(!dp){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'record' Out Of Memory Requested %ld\n",length*sizeof(double));
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		if(recordSound(dp,length,value))goto ErrorOut;
		
		R2->type=T_STACK_ARRAY;
		R2->data=dp;
		R2->xsize=length;
		R2->ysize=1;
		R2->zsize=1;
		
	    if(CExpressPush(e,T_STACK_R2))goto ErrorOut;
		R1=NULL;
		R2=NULL;
	
	    break;
	
	case 52: /* plotgetlimits(s,xmin,ymin,zmin,xmax,ymax,zmax,xsize,ysize,zsize,time,framecount) */
	{
		struct stack *nameframecount;
		struct stack *nametime;
		struct stack *namexmin;
		struct stack *nameymin;
		struct stack *namezmin;
		struct stack *namexmax;
		struct stack *nameymax;
		struct stack *namezmax;
		struct stack *namexsize;
		struct stack *nameysize;
		struct stack *namezsize;
		
	    zerol((char *)ai,sizeof(struct areadata));


		if(!(nameframecount=popName(e))){
			sprintf(WarningBuff,"plotgetlimits - file (%s)  Error currentframe\n",R1->name);
			WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		nameframecount->type = T_STACK_CONSTANT;
		
		if(!(nametime=popName(e))){
			sprintf(WarningBuff,"plotgetlimits - file (%s)  Error time\n",R1->name);
			WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		nametime->type = T_STACK_CONSTANT;
		
		if(!(namezsize=popName(e))){
			sprintf(WarningBuff,"plotgetlimits - file (%s)  Error zsize\n",R1->name);
			WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		
		namezsize->type = T_STACK_CONSTANT;
		
		if(!(nameysize=popName(e))){
			sprintf(WarningBuff,"plotgetlimits - file (%s)  Error ysize\n",R1->name);
			WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		nameysize->type = T_STACK_CONSTANT;
		
		if(!(namexsize=popName(e))){
			sprintf(WarningBuff,"plotgetlimits - file (%s)  Error xsize\n",R1->name);
			WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		namexsize->type = T_STACK_CONSTANT;
		if(!(namezmax=popName(e))){
			sprintf(WarningBuff,"plotgetlimits - file (%s)  Error zmax\n",R1->name);
			WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		namezmax->type = T_STACK_CONSTANT;
		
		
		if(!(nameymax=popName(e))){
			sprintf(WarningBuff,"plotgetlimits - file (%s)  Error ymax\n",R1->name);
			WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		nameymax->type = T_STACK_CONSTANT;
		
		if(!(namexmax=popName(e))){
			sprintf(WarningBuff,"plotgetlimits - file (%s)  Error xmax\n",R1->name);
			WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		namexmax->type = T_STACK_CONSTANT;
		
		if(!(namezmin=popName(e))){
			sprintf(WarningBuff,"plotgetlimits - file (%s)  Error zmin\n",R1->name);
			WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		namezmin->type = T_STACK_CONSTANT;
		
		if(!(nameymin=popName(e))){
			sprintf(WarningBuff,"plotgetlimits - file (%s)  Error ymin\n",R1->name);
			WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		nameymin->type = T_STACK_CONSTANT;
		
		if(!(namexmin=popName(e))){
			sprintf(WarningBuff,"plotgetlimits - file (%s)  Error xmin\n",R1->name);
			WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		namexmin->type = T_STACK_CONSTANT;
		
	    
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;   
	    R1 = &e->R1;
	    if((R1->type != T_STACK_ARRAY) || !R1->data){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'plotgetlimits' Bad Type %d in array\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
					    
		ai->pioTime=R1->pioTime;
				
		if(R1->data)cFree((char *)R1->data);
		if(R1->auxData)cFree((char *)R1->auxData);
		
		R1->data=NULL;
		R1->auxData=NULL;
		    
	    limits.CurrentFrame=e->CurrentFrame;
	    
	    if(R1->zsize > 1){
	        limits.type=LIMIT_TYPE3D;
	    }else{
	        limits.type=LIMIT_TYPE2D;
	    }
	    
		if(SendMessageByName(R1->name,MessageType_GET_LIMITS_DATA,&limits)){
			sprintf(WarningBuff,"plotgetlimits - file (%s)  Limits data Error\n",R1->name);
			WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
	    	    
		nameymax->value=limits.ymax;
		namezmax->value=limits.zmax;
		nameysize->value=R1->ysize;
		namezsize->value=R1->zsize;
		namexsize->value=R1->xsize;
		nametime->value=limits.time;
		namexmin->value=limits.xmin;
		nameymin->value=limits.ymin;
		namezmin->value=limits.zmin;
		namexmax->value=limits.xmax;
		nameframecount->value=limits.ImageCount;
		
	}
	    break;
		
	case 67: 
		LinePlot(e,count);	
		break;
           
	case 53: /* setframe(framenumber) */
	
	    if(R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION 'setframe' Bad Type %d in value\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
		}
		
		{
			long n;
			
			n=(long)R1->value;
			if(n < 0)n=0;
			e->CurrentFrame=n;
			name=CExpressGetNameStack(e,"frame");
			if(!name){
            	sprintf(WarningBuff,"CExpressDoFUNCTION 'setframe' frame name not Found\n");
	        	WarningExpression(e,WarningBuff);
            	goto ErrorOut;
			}
			
			name->value=n;
		}
	    
		R1=NULL;
		R2=NULL;
	
	    break;
			
		case 54:                /* mean */
			if(R1->type != T_STACK_CONSTANT){
				sprintf(WarningBuff,"CExpressDoFUNCTION 'mean' Bad Type %d in R1\n",R1->type);
				WarningExpression(e,WarningBuff);
				goto ErrorOut;
			}
			increment=R1->value;
			if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;
			R2 = &e->R2;
			if((R2->type != T_STACK_ARRAY) || !R2->data){
				sprintf(WarningBuff,"CExpressDoFUNCTION 'mean' Bad Type %d in R3\n",R1->type);
				WarningExpression(e,WarningBuff);
				goto ErrorOut;
			}
			length=R2->xsize*R2->ysize*R2->zsize;
			
			dp=(double *)cMalloc(length*sizeof(double),8125);
			if(!dp){
				sprintf(WarningBuff,"CExpressDoFUNCTION 'scale' Out Of Memory\n");
				WarningExpression(e,WarningBuff);
				goto ErrorOut;
			}
			mean(dp,R2->data,R2->xsize,R2->ysize,(long)increment);
			if(R2->data)cFree((char *)R2->data);
			R2->data=dp;
			if(CExpressPush(e,T_STACK_R2))goto ErrorOut;
			R1=NULL;
			R2=NULL;
			break;
		case 55:                /* bhangmeterset */
		
			xx[9]=0;    
			xx[10]=0;    
			xx[11]=0;    
			xx[12]=0;    
			
			if(count == 10 || count == 11 || count == 14){
				if(CExpressPopConstantList(e,xx,count-1,"bhangmeterset"))goto ErrorOut;  
			}else{
				sprintf(WarningBuff,"bhangmeterset Bad count %d\n",count);
				WarningExpression(e,WarningBuff);
				goto ErrorOut;
			}
			
			if(CExpressPop(e,T_STACK_R1,0))goto ErrorOut;   
			R1 = &e->R1;
			if(R1->type != T_STACK_NAME){
				sprintf(WarningBuff,"CExpressDoFUNCTION 'bhangmeterset' Found Type %d in string\n",R1->type);
				WarningExpression(e,WarningBuff);
				goto ErrorOut;
			}
			name=CExpressGetNameStack(e,R1->name);
			if(!name){
				sprintf(WarningBuff,"CExpressDoFUNCTION 'bhangmeterset' Error Trying To Find Window  %s\n",R1->name);
				WarningExpression(e,WarningBuff);
				goto ErrorOut;
			}
			name->PointLineType=LINEDATA_BHANGMETER_DATA;
			name->x1=xx[0];
			name->y1=xx[1];
			name->z1=xx[2];
			name->x2=xx[3];
			name->y2=xx[4];
			name->z2=xx[5];
			name->x3=xx[6];
			name->y3=xx[7];
			name->z3=xx[8];
			name->multigroup=(int)xx[9];
			name->bhangMeterRadius=xx[10];
			name->bhangMeterSteps=(int)xx[11];
			name->bhangMeterHeight=xx[12];
			name->PointLineCount=0;
			break;
           
	default:
	if(f->pcount == 2){
	    if(R1->type != T_STACK_ARRAY && R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION Bad Type %d in R1\n",R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
	    }
	    if(CExpressPop(e,T_STACK_R2,1))goto ErrorOut;
	    R2 = &e->R2;
	    data2=R2->data;
        if(R2->type == T_STACK_ARRAY){
            if(!data2){
	            sprintf(WarningBuff,"CExpressDoFUNCTION NULL Data1 Array Pointer\n");
	            WarningExpression(e,WarningBuff);
	           goto ErrorOut;
	        }
    	}else if(R2->type != T_STACK_CONSTANT){
	        sprintf(WarningBuff,"CExpressDoFUNCTION Bad Type %d in R2\n",R2->type);
		    WarningExpression(e,WarningBuff);
	        goto ErrorOut;
	    }
    	
	    if(R1->type == T_STACK_CONSTANT){
		    if(R2->type == T_STACK_CONSTANT){
				if(LD->type == T_STACK_LCONSTANT){
	                if(LD->logical){
	                    result=(*fun2)(R2->value,R1->value);
	                    R1->value=result;
	                }
				}else if(LD->type == T_STACK_LARRAY){
		            length=LD->xsize*LD->ysize*LD->zsize;
	                for(n=0;n<length;++n){
	            		if(LD->ldata[n]){
	                        result=(*fun2)(R2->value,R1->value);
	                        R1->value=result;
	                        break;
	            		}
	                }
		    	}
	    	    if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
		    }else if(R2->type == T_STACK_ARRAY){
	            RR=dupStack(R2);
	            if(!RR || !RR->data)goto ErrorOut;
	        	datar=RR->data;
		        length=RR->xsize*RR->ysize*RR->zsize;
				if(LD->type == T_STACK_LCONSTANT){
	                if(LD->logical){
	                    for(n=0;n<length;++n){
	                        datar[n]=((*fun2)(data2[n],R1->value));
	            		}
				    }		
				}else if(LD->type == T_STACK_LARRAY){
		            if(length != LD->xsize*LD->ysize*LD->zsize){
		        	    sprintf(WarningBuff,"CExpressDoFUNCTION Array Size Missmatch Arrary %ld Logical %ld\n",
		        	            length,LD->xsize*LD->ysize*LD->zsize);
			    	    WarningExpression(e,WarningBuff);
		        	    goto ErrorOut;
		            }
	                for(n=0;n<length;++n){
            		    if(LD->ldata[n]){
	                        datar[n]=((*fun2)(data2[n],R1->value));
	                    }
	            	}
		    	}
	            e->RR=RR;
	    	    if(CExpressPush(e,T_STACK_RR))goto ErrorOut;
	    	    RR=NULL;
		    }
	    }else if(R1->type == T_STACK_ARRAY){
	        RR=dupStack(R1);
	        if(!RR || !RR->data)goto ErrorOut;
	        datar=RR->data;
		    length=RR->xsize*RR->ysize*RR->zsize;
		    if(R2->type == T_STACK_CONSTANT){
				if(LD->type == T_STACK_LCONSTANT){
				    if(LD->logical){
		                for(n=0;n<length;++n){
		                    datar[n]=((*fun2)(R2->value,data1[n]));
		            	}
	            	}
				}else if(LD->type == T_STACK_LARRAY){
		            if(length != LD->xsize*LD->ysize*LD->zsize){
		        	    sprintf(WarningBuff,"CExpressDoFUNCTION Array Size Missmatch Arrary %ld Logical %ld\n",
		        	            length,LD->xsize*LD->ysize*LD->zsize);
			    	    WarningExpression(e,WarningBuff);
		        	    goto ErrorOut;
		            }
	                for(n=0;n<length;++n){
            		    if(LD->ldata[n]){
	                        datar[n]=((*fun2)(R2->value,data1[n]));
	                    }
	            	}
		    	}
		    }else if(R2->type == T_STACK_ARRAY){
				if(LD->type == T_STACK_LCONSTANT){
				    if(LD->logical){
		                for(n=0;n<length;++n){
		                    datar[n]=((*fun2)(data2[n],data1[n]));
		            	}
	            	}
				}else if(LD->type == T_STACK_LARRAY){
		            if(length != LD->xsize*LD->ysize*LD->zsize){
		        	    sprintf(WarningBuff,"CExpressDoFUNCTION Array Size Missmatch Arrary %ld Logical %ld\n",
		        	            length,LD->xsize*LD->ysize*LD->zsize);
			    	    WarningExpression(e,WarningBuff);
		        	    goto ErrorOut;
		            }
	                for(n=0;n<length;++n){
            		    if(LD->ldata[n]){
	                        datar[n]=((*fun2)(data2[n],data1[n]));
	                    }
	            	}
		    	}
		    }
	        e->RR=RR;
	    	if(CExpressPush(e,T_STACK_RR))goto ErrorOut;
	    	RR=NULL;
	    }
	}else{
	    if(R1->type != T_STACK_ARRAY && R1->type != T_STACK_CONSTANT){
            sprintf(WarningBuff,"CExpressDoFUNCTION (%s) Bad Type %d in R1\n",f->name,R1->type);
	        WarningExpression(e,WarningBuff);
            goto ErrorOut;
	    }
	    if(R1->type == T_STACK_CONSTANT){
			if(LD->type == T_STACK_LCONSTANT){
	            if(LD->logical){
	                result=(*fun1)(R1->value);
	                R1->value=result;
	            }
	        }else if(LD->type == T_STACK_LARRAY){
	            length=LD->xsize*LD->ysize*LD->zsize;
                for(n=0;n<length;++n){
            		if(LD->ldata[n]){
                	    result=(*fun1)(R1->value);
                	    R1->value=result;
                	    break;
            		}
                }
	        }
	    	if(CExpressPush(e,T_STACK_R1))goto ErrorOut;
	    }else if(R1->type == T_STACK_ARRAY){
	        RR=dupStack(R1);
	        if(!RR)return doReturn(1);
	        datar=RR->data;
	        length=R1->xsize*R1->ysize*R1->zsize;
			if(LD->type == T_STACK_LCONSTANT){
	            if(LD->logical){
	                for(n=0;n<length;++n){
	                    datar[n]=((*fun1)(data1[n]));
	                }
	            }
	        }else if(LD->type == T_STACK_LARRAY){
	            if(length != LD->xsize*LD->ysize*LD->zsize){
	        	    sprintf(WarningBuff,"CExpressDoFUNCTION Array Size Missmatch Arrary %ld Logical %ld\n",
	        	            length,LD->xsize*LD->ysize*LD->zsize);
		    	    WarningExpression(e,WarningBuff);
	        	    goto ErrorOut;
	            }
                for(n=0;n<length;++n){
            		if(LD->ldata[n]){
	                    datar[n]=((*fun1)(data1[n]));
            		}
                }
	        }
	        e->RR=RR;
	    	if(CExpressPush(e,T_STACK_RR))return doReturn(1);
	    	RR=NULL;
	    }
	
	}
	}
	ret = 0;
ErrorOut:
	if(R1 && R1->data){
	   cFree((char *)R1->data);
	   R1=NULL;
	}
	if(R2 && R2->data){
	   cFree((char *)R2->data);
	   R2=NULL;
	}
	if(RR && RR->data){
	   cFree((char *)RR->data);
	   cFree((char *)RR);
	   RR=NULL;
	}
	return ret;
}
static int LinePlot(CExpressPtr e,int count)
{
	struct stack *name;
	struct stack *R1;
	double *xdata;
	double value;
	int ret;
	int n,n1;
	
	if(!e)return doReturn(1);

	R1 = &e->R1;
	
	ret=1;
	
	xdata=NULL;
	
	if(count < 2){
        sprintf(WarningBuff,"line needs at least 2 items\n");
        WarningExpression(e,WarningBuff);
        return doReturn(1);
	}
	
	
	xdata=(double *)cMalloc(sizeof(double)*count,8721);
	
	--count;
	
	if(!xdata){
        sprintf(WarningBuff,"'line' Out Of Memory\n");
        WarningExpression(e,WarningBuff);
    	goto ErrorOut;
	}
	
	n1=count-1;
	for(n=0;n<count;++n){
	    if(CExpressPop(e,T_STACK_R1,1))goto ErrorOut;
	    if(R1->type == T_STACK_CONSTANT){
	       value=R1->value;
			xdata[n1--]=value;
	    }else{
            sprintf(WarningBuff,"'line' n %d ERROR Type %d\n",n,R1->type);
            WarningExpression(e,WarningBuff);
        	goto ErrorOut;
	    }
	}
	
	if(n1 !=  -1){
	    sprintf(WarningBuff,"'line' Error\n");
		for(n=0;n<count;++n){
	        sprintf(WarningBuff,"'line' n %d %g\n",n,xdata[n]);
	        WarningExpression(e,WarningBuff);
        	goto ErrorOut;
		}
	
	}
	
    if(CExpressPop(e,T_STACK_R1,0))goto ErrorOut;   
    R1 = &e->R1;
    if(R1->type != T_STACK_NAME){
        sprintf(WarningBuff,"Error 'line' Found Type %d in string\n",R1->type);
        WarningExpression(e,WarningBuff);
        goto ErrorOut;
	}
	
    name=CExpressGetNameStack(e,R1->name);
	if(!name){
	    name=CExpressCreateNameStack(e,R1->name,T_STACK_VARIABLE);
		if(!name){
			sprintf(WarningBuff,"line Error Trying To create name  %s\n",R1->name);
			WarningExpression(e,WarningBuff);
			goto ErrorOut;
		}
	}
	name->xsize=count;
	name->ysize=1;
	name->zsize=1;
	name->data=xdata;
	
	xdata=NULL;

	ret=0;

ErrorOut:	

	if(xdata)cFree((char *)xdata);
	
	return ret;
}
static int mean(double *out,double *in,long xsize,long ysize,long size)
{
	int j,i,n,m,nstart,nend,mstart,mend;
	double sum1,sum2;
	
	if(!out || !in)return 1;
	
	size = (size - 1) / 2;
	
	for (j = 0; j < ysize; j++)
	{
		for (i = 0; i < xsize; i++)
		{
			nstart = j - size;
			if (nstart < 0)
			{
				nstart = 0;
			}
			
			nend = j + size;
			if (nend >= ysize)
			{
				nend = ysize - 1;
			}
			
			mstart = i - size;
			if (mstart < 0)
			{
				mstart = 0;
			}
			
			mend = i + size;
			if (mend >= xsize)
			{
				mend = xsize - 1;
			}
			
			sum1 = 0;
			sum2 = 0;
			for (n = nstart; n <= nend; ++n)
			{
				for (m = mstart; m <= mend; ++m)
				{
					sum1 += *(in+m+n*ysize);
					++sum2;
				}
			}
			*(out+i+j*xsize) = sum1 / sum2;
		}
	}
	
	return 0;
}
static int scale(double *out,long xnew,long ynew,double *in,long xsize,long ysize)
{
	double dx,dy,x,y,r,s;
	long n1,n2,n3,n4;
	long i,j,ic,jc,ic1,jc1;
	
	
	if(!out || !in)return doReturn(1);
	if(xnew  <= 1  || ynew  <= 1)return doReturn(1);
	if(xsize <= 1  || ysize <= 1)return doReturn(1);
	
	dx=(((double)xsize-1.0)/((double)xnew-1.0));
	dy=(((double)ysize-1.0)/((double)ynew-1.0));
	for(j=0;j<ynew;++j){
	    y=(double)j*dy;
	    jc1=jc=(long)y;
	    if(++jc1 >= ysize)jc1 = ysize-1;
	    s= (double)(-1.0 + 2.0*(y-(double)jc));
	    for(i=0;i<xnew;++i){ 
	        x=(double)i*dx;
	        ic1=ic=(long)x;
	        if(++ic1 >= xsize)ic1 = xsize-1;
	        n1=ic+xsize*jc;
	        n2=ic1+xsize*jc;
	        n3=ic1+xsize*jc1;
	        n4=ic+xsize*jc1;
	        r= (double)(-1.0 + 2.0*(x-(double)ic));
	        *out++ = (double)(0.25 *((1.0-r)*(1.0-s)*in[n1]+(1.0+r)*(1.0-s)*in[n2]+
	                        (1.0+r)*(1.0+s)*in[n3]+(1.0-r)*(1.0+s)*in[n4]));
	    }
	}
	
	return 0;
}
static int CExpressPushLogicalStack(CExpressPtr e,struct stack *sp)
{
	stackHolderPtr s;
	
	if(!e || !sp)return doReturn(1);
	
	s = &e->logicalData;
	if(checkStack(s))return doReturn(1);
	s->stackList[s->stackCount] = *sp;
	++(s->stackCount);

	return 0;
}
static struct stack *CExpressGetLogicalStack(CExpressPtr e,int location)
{
	static struct stack LTRUE;
	struct stack *LD;
	stackHolderPtr s;
	
	
	if(!e || (location < 0))return NULL;	
		
	s = &e->logicalData;
	
	if(s->stackCount <= 0 || (s->stackCount-location) <= 0){
	    LTRUE.type=T_STACK_LCONSTANT;
	    LTRUE.logical=TRUE;
	    LTRUE.ldata=NULL;
	    return &LTRUE;
	}

	LD=&s->stackList[s->stackCount-1-location];
	
	if((LD->type != T_STACK_LCONSTANT) && (LD->type != T_STACK_LARRAY)){
        sprintf(WarningBuff,"CExpressGetLogicalStack Bad Type %d (1)\n",LD->type);
	    WarningExpression(e,WarningBuff);
        return NULL;
    }
    
	if(LD->type == T_STACK_LARRAY){
        if(!LD->ldata){
    	    sprintf(WarningBuff,"CExpressGetLogicalStack NULL Logical Array Pointer (2)\n");
    	    WarningExpression(e,WarningBuff);
    	    return NULL;
    	}
	}
	
	return LD;

}
static struct stack *dupStack(struct stack *s)
{
	struct stack *r;
	double *fout,*fin;
	long length,n;
	
	if(!s){
	    sprintf(WarningBuff,"Stop - dupStack Found NULL pointer\n");
		WarningExpression(NULL,WarningBuff);
		return NULL;
	}
	r=(struct stack *)cMalloc(sizeof(struct stack),8027);
	if(!r){
	    sprintf(WarningBuff,"Stop - dupStack Out of Memory\n");
		WarningExpression(NULL,WarningBuff);
		return NULL;
	}
	zerol((char *)r,sizeof(struct stack));
	*r = *s;
    length=r->xsize*r->ysize*r->zsize;
    r->data = NULL;
    if(length <= 0 || (s->data == NULL)){
        r->xsize=r->ysize=r->zsize=0;
        return r;
    }
    r->data = (double *)cMalloc(length*sizeof(double),8028);
	if(!r->data){
	    cFree((char *)r);
	    sprintf(WarningBuff,"Stop - dupStack Out of Memory\n");
		WarningExpression(NULL,WarningBuff);
		return NULL;
	}
	fout=r->data;
	fin=s->data;
	
	for(n=0;n<length;++n)fout[n]=fin[n];
	
	
	if(s->auxData && s->auxLength >= 1){
		r->auxLength=s->auxLength;
	    r->auxData =  (double *)cMalloc(r->auxLength*sizeof(double),8031);
	    if(!r->auxData){
	        if(r->data)cFree((char *)r->data);
	    	cFree((char *)r);
			sprintf(WarningBuff,"CExpressPop - variable (%s) of type %d out of memory\n",r->name,r->type);
			WarningExpression(NULL,WarningBuff);
			return NULL;
	    }
	    for(n=0;n<r->auxLength;++n)r->auxData[n]=s->auxData[n];
    }else{
		r->auxData=NULL;
		r->auxLength=0;		    
    }
	
	return r;
}
int CExpressPop(CExpressPtr e,int popWhat,int load)
{
	stackHolderPtr s;
	
	if(!e)return doReturn(1);
	
	s = &e->stackData;
	
	return CExpressPopFrom(e,s,popWhat,load);
	
	
}
int CExpressPopFrom(CExpressPtr e,stackHolderPtr s,int popWhat,int load)
{
	struct stack *R;
	struct stack *sp;
	long length,n;
	
	if(!e)return doReturn(1);

	if(!s)return doReturn(1);
	
	if(s->stackCount <= 0)return doReturn(1);
	
	--(s->stackCount);
	
	if(popWhat == T_STACK_R1){
	   e->R1=s->stackList[s->stackCount];
	   R = &e->R1;
	}else if(popWhat == T_STACK_R2){
	   e->R2=s->stackList[s->stackCount];
	   R = &e->R2;
	}else{
	    return doReturn(1);
	}
	
	if(!load)return 0;
	
	if((R->type == T_STACK_CONSTANT) || (R->type == T_STACK_ARRAY) ||
	   (R->type == T_STACK_STRING)   || (R->type == T_STACK_DIMENSIONS))return 0;
	
	if(R->type == T_STACK_NAME){
	    sp=CExpressGetNameStack(e,R->name);
	    if(!sp){
			sprintf(WarningBuff,"CExpressPop - variable %s not found\n",R->name);
			WarningExpression(e,WarningBuff);
			return doReturn(1);
	    }
	    if(sp->type == T_STACK_CONSTANT){
	        R->type = T_STACK_CONSTANT;
	        R->value = sp->value;
	        R->file = sp->file;
	        R->data = NULL;
	        return 0;
		}else if(sp->type == T_STACK_FILE){
		    struct SetFrameData  sd;
		    struct linedata li;
		    struct areadata ai;
		    IconPtr myIcon;
		    
		    myIcon=sp->myIcon;
		    		    
			zerol((char *)&sd,sizeof(struct SetFrameData));
			
			zerol((char *)&li,sizeof(struct linedata));

			zerol((char *)&ai,sizeof(struct areadata));
			
		    if(R->pioName[0]){
		        li.pioIndex=R->pioIndex;
		        mstrncpy(li.pioName,R->pioName,64);
		        
		        ai.pioIndex=R->pioIndex;
		        mstrncpy(ai.pioName,R->pioName,64);
		        		        
		        sd.pioIndex=R->pioIndex;
		        mstrncpy(sd.pioName,R->pioName,64);
		    }
			    
		    sd.type=FRAME_DATA_FLOAT;
		    
			if(sp->PointLineType <= 0){
					    	
		    	sd.CurrentFrame=e->CurrentFrame;
		    	
				if(SendMessageByName(sp->name,MessageType_GET_REGION_DATA,&sd)){
			    	sd.xsize=sp->xsize;
			    	sd.ysize=sp->ysize;
			    	if(sp->zsize < 1)sp->zsize=1;			    	
			    	sd.zsize=sp->zsize;
				    length=sp->xsize*sp->ysize*sp->zsize;
				    
				    R->data =  (double *)cMalloc(length*sizeof(double),8028);
				    if(!R->data){
						sprintf(WarningBuff,"CExpressPop - file (%s)  out of memory\n",sp->name);
						WarningExpression(e,WarningBuff);
						return doReturn(1);
				    }
			        for(n=0;n<length;++n)R->data[n]=0;
			        sd.data=R->data;
			    }		    
		    }else if(sp->PointLineType == LINEDATA_CELL_DATA){
				sd.CurrentFrame=e->CurrentFrame;
				sd.type = LINEDATA_CELL_DATA;
				if(SendMessageByName(sp->name,MessageType_GET_CELL_DATA,&sd)){
					sprintf(WarningBuff,"CExpressPopFrom - file (%s)  LINEDATA_CELL_DATA Error Getting Area Data\n",sp->name);
					WarningExpression(e,WarningBuff);
					return doReturn(1);
				}
		    	sd.ysize=1L;
		    	sd.zsize=1L;
			    if(!sd.data){
					sprintf(WarningBuff,"CExpressPop - file (%s)  NULL data pointer\n",sp->name);
					WarningExpression(e,WarningBuff);
					return doReturn(1);
			    }
			}else if(sp->PointLineType == AREADATA_AREA_DATA){
				ai.xminArea=sp->x1;
				ai.yminArea=sp->y1;
				ai.zminArea=sp->z1;
				ai.xmaxArea=sp->x2;
				ai.ymaxArea=sp->y2;
				ai.zmaxArea=sp->z2;
				ai.xsize=sp->n1;
				ai.ysize=sp->n2;
				ai.zsize=sp->n3;
				ai.CurrentFrame=e->CurrentFrame;
				ai.type = AREADATA_AREA_DATA;
				if(SendMessageByName(sp->name,MessageType_GET_AREA_DATA,&ai)){
					sprintf(WarningBuff,"CExpressPopFrom - file (%s)  Error Getting Area Data\n",sp->name);
					WarningExpression(e,WarningBuff);
					return doReturn(1);
				}
				
				if(ai.xsize <= 1){
			    	sd.xsize=ai.ysize;
			    	sd.ysize=ai.zsize;
			    	sd.zsize=1;
		    	}else if(ai.ysize <= 1){
			    	sd.xsize=ai.xsize;
			    	sd.ysize=ai.zsize;
			    	sd.zsize=1;
		    	}else{
			    	sd.xsize=ai.xsize;
			    	sd.ysize=ai.ysize;
			    	sd.zsize=ai.zsize;
		    	}
			    sd.data =  ai.data;
			    sd.pioTime =  ai.pioTime;
			    
			    if(!sd.data){
					sprintf(WarningBuff,"CExpressPop - file (%s)  NULL data pointer\n",sp->name);
					WarningExpression(e,WarningBuff);
					return doReturn(1);
			    }
		    }else if(sp->PointLineType == LINEDATA_POINT_DATA || sp->PointLineType == LINEDATA_LINE_DATA || sp->PointLineType == LINEDATA_BHANGMETER_DATA){
		        GetWindowName(myIcon,li.sourceName,0);
				li.x1=sp->x1;
				li.y1=sp->y1;
				li.z1=sp->z1;
				li.x2=sp->x2;
				li.y2=sp->y2;
				li.z2=sp->z2;
				li.nx=sp->x3;
				li.ny=sp->y3;
				li.nz=sp->z3;
				li.multigroup=sp->multigroup;
				li.bhangMeterRadius=sp->bhangMeterRadius;
				li.bhangMeterSteps=sp->bhangMeterSteps;
				li.bhangMeterHeight=sp->bhangMeterHeight;
				li.PointLineCount=sp->PointLineCount;
				li.type=sp->PointLineType;
				li.integrate=sp->integrate;
				li.integral=0;
				li.CurrentFrame=e->CurrentFrame;
				if(SendMessageByName(sp->name,MessageType_GET_LINE_DATA,&li)){
					if(li.pioName[0]){
					    sprintf(WarningBuff,"CExpressPopFrom - file (%s)  pioName (%s) Error Getting Line or Point Data\n",sp->name,li.pioName);
					}else{
					    sprintf(WarningBuff,"CExpressPopFrom - file (%s)  Error Getting Line or Point Data\n",sp->name);
					}
					WarningExpression(e,WarningBuff);
					return doReturn(1);
				}
				
		    	sd.xsize=li.dCount;
		    	sd.ysize=1;
		    	sd.zsize=1;
		    			    	
			    length=li.dCount*2;
			    R->data =  (double *)cMalloc(length*sizeof(double),8028);
			    if(!R->data){
		    		if(li.xData)cFree((char *)li.xData);
		    		if(li.yData)cFree((char *)li.yData);
					sprintf(WarningBuff,"CExpressPop - file (%s)  out of memory\n",sp->name);
					WarningExpression(e,WarningBuff);
					return doReturn(1);
			    }
		        for(n=0;n<li.dCount;++n){
		            if(li.yData){
		                R->data[n]=li.yData[n];
		            }else{
		                R->data[n]=0;
		            }
		            if(li.xData){
		                R->data[n+li.dCount]=li.xData[n];
		            }else{
		                R->data[n+li.dCount]=0;
		            }
		        }
		        sd.data=R->data;
		        sp->integral=li.integral;
/*
		        e->ld.x=li.xData;
		        e->ld.length=li.dCount;
		        if(pushpopPush(&e->ld,e->lineDataStack)){
					sprintf(WarningBuff,"CExpressPop - file (%s)  out of data memory\n",sp->name);
					WarningExpression(e,WarningBuff);
					return doReturn(1);
		         }
*/
		        sp->auxData=li.xData;
		        sp->auxLength=li.dCount;
		        
		    	if(li.yData)cFree((char *)li.yData);
		    }else if(sp->PointLineType == GENERAL_PLANE_DATA){
				ai.x1=sp->x1;
				ai.y1=sp->y1;
				ai.z1=sp->z1;
				ai.x2=sp->x2;
				ai.y2=sp->y2;
				ai.z2=sp->z2;
				ai.x3=sp->x3;
				ai.y3=sp->y3;
				ai.z3=sp->z3;
				ai.xsize=sp->n1;
				ai.ysize=sp->n2;
				ai.CurrentFrame=e->CurrentFrame;
				ai.type = GENERAL_PLANE_DATA;
				if(SendMessageByName(sp->name,MessageType_GET_AREA_DATA,&ai)){
					sprintf(WarningBuff,"CExpressPopFrom - file (%s)  Error Getting Area Data\n",sp->name);
					WarningExpression(e,WarningBuff);
					return doReturn(1);
				}
				
		    	sd.xsize=ai.xsize;
		    	sd.ysize=ai.ysize;
		    	sd.zsize=1L;
			    	
			    sd.data =  ai.data;
			    sd.pioTime =  ai.pioTime;
			    
			    if(!sd.data){
					sprintf(WarningBuff,"CExpressPop - file (%s)  NULL data pointer\n",sp->name);
					WarningExpression(e,WarningBuff);
					return doReturn(1);
			    }
		    }else if(sp->PointLineType == LINEDATA_LIST_DATA){
		        GetWindowName(myIcon,li.sourceName,0);
				li.x1=sp->x1;
				li.y1=sp->y1;
				li.z1=sp->z1;
				li.x2=sp->x2;
				li.y2=sp->y2;
				li.z2=sp->z2;
				li.PointLineCount=sp->PointLineCount;
				li.type=sp->PointLineType;
				li.integrate=sp->integrate;
				li.integral=0;
				li.CurrentFrame=e->CurrentFrame;
				
				if(pushpopPeek(&e->li,sp->lineIndex,e->lineStack)){
					if(li.pioName[0]){
					    sprintf(WarningBuff,"CExpressPopFrom - file (%s)  pioName (%s) Error Line List\n",sp->name,li.pioName);
					}else{
					    sprintf(WarningBuff,"CExpressPopFrom - file (%s)  Error Getting Error Line List\\n",sp->name);
					}
					WarningExpression(e,WarningBuff);
					return doReturn(1);
				}
				
				li.x=e->li.x;
				li.y=e->li.y;
				li.z=e->li.z;
				li.v=e->li.v;
				li.PointLineCount=e->li.PointLineCount;
							
				if(SendMessageByName(sp->name,MessageType_GET_LINE_DATA,&li)){
						if(li.pioName[0]){
						    sprintf(WarningBuff,"CExpressPopFrom - file (%s)  pioName (%s) Error Getting Line or Point Data\n",sp->name,li.pioName);
						}else{
						    sprintf(WarningBuff,"CExpressPopFrom - file (%s)  Error Getting Line or Point Data\n",sp->name);
						}
						WarningExpression(e,WarningBuff);
						return doReturn(1);
				}	  
				
				li.dCount=li.PointLineCount;
				
		    	sd.xsize=li.dCount;
		    	sd.ysize=1;
		    	sd.zsize=1;
		    			    	
			    length=li.dCount*2;
			    R->data =  (double *)cMalloc(length*sizeof(double),8028);
			    if(!R->data){
		    		if(li.xData)cFree((char *)li.xData);
		    		if(li.yData)cFree((char *)li.yData);
					sprintf(WarningBuff,"CExpressPop - file (%s)  out of memory\n",sp->name);
					WarningExpression(e,WarningBuff);
					return doReturn(1);
			    }
		        for(n=0;n<li.dCount;++n){
		            if(li.v){
		                R->data[n]=li.v[n];
		            }else{
		                R->data[n]=0;
		            }
		            if(li.x){
		                R->data[n+li.dCount]=li.x[n];
		            }else{
		                R->data[n+li.dCount]=0;
		            }
		        }
		        sd.data=R->data;
		        sp->integral=li.integral;
/*
		        e->ld.x=li.xData;
		        e->ld.length=li.dCount;
		        if(pushpopPush(&e->ld,e->lineDataStack)){
					sprintf(WarningBuff,"CExpressPop - file (%s)  out of data memory\n",sp->name);
					WarningExpression(e,WarningBuff);
					return doReturn(1);
		         }
*/
		        sp->auxData=li.xData;
		        sp->auxLength=li.dCount;
		        
		    	if(li.yData)cFree((char *)li.yData);
	    	}else{
				sprintf(WarningBuff,"CExpressPop - file (%s)  Unknown Data Type (%d)\n",sp->name,sp->PointLineType);
				WarningExpression(e,WarningBuff);
	    	}
		    
		    R->type = T_STACK_ARRAY;
		    R->xsize = sd.xsize;
		    R->ysize = sd.ysize;
		    if(sd.zsize < 1)sd.zsize=1L;
		    R->zsize = sd.zsize;
		    R->data =  (double *)sd.data;
		    R->pioTime =  sd.pioTime;
		    
			R->PointLineCount=sp->PointLineCount;
			R->PointLineType=sp->PointLineType;
			R->integrate=sp->integrate;
			R->integral=sp->integral;
			R->auxData=sp->auxData;
			R->auxLength=sp->auxLength;
			R->n1=sp->n1;
			R->n2=sp->n2;
			R->n3=sp->n3;
			R->x1=sp->x1;
			R->x2=sp->x2;
			R->x3=sp->x3;
			R->y1=sp->y1;
			R->y2=sp->y2;
			R->y3=sp->y3;
			R->z1=sp->z1;
			R->z2=sp->z2;
			R->z3=sp->z3;
		
		}else if(sp->type == T_STACK_VARIABLE){
		    double *sin,*sout;
		    R->type = T_STACK_ARRAY;
		    R->xsize = sp->xsize;
		    R->ysize = sp->ysize;
		    if(sp->zsize < 1)sp->zsize = 1;
		    R->zsize = sp->zsize;
		    length=sp->xsize*sp->ysize*sp->zsize;
		    R->data =  (double *)cMalloc(length*sizeof(double),8030);
		    if(!R->data){
				sprintf(WarningBuff,"CExpressPop - variable (%s) of type %d out of memory\n",sp->name,sp->type);
				WarningExpression(e,WarningBuff);
				return doReturn(1);
		    }
		    sin=sp->data;
		    sout=R->data;
		    for(n=0;n<length;++n)sout[n]=sin[n];
		    
			R->PointLineCount=sp->PointLineCount;
			R->PointLineType=sp->PointLineType;
			R->integrate=sp->integrate;
			R->integral=sp->integral;
			if(sp->auxData && sp->auxLength >= 1){
				R->auxLength=sp->auxLength;
			    R->auxData =  (double *)cMalloc(R->auxLength*sizeof(double),8031);
			    if(!R->auxData){
					sprintf(WarningBuff,"CExpressPop - variable (%s) of type %d out of memory\n",sp->name,sp->type);
					WarningExpression(e,WarningBuff);
					return doReturn(1);
			    }
			    for(n=0;n<R->auxLength;++n)R->auxData[n]=sp->auxData[n];
		    }else{
				R->auxData=NULL;
				R->auxLength=0;		    
		    }
			R->n1=sp->n1;
			R->n2=sp->n2;
			R->n3=sp->n3;
			R->x1=sp->x1;
			R->x2=sp->x2;
			R->x3=sp->x3;
			R->y1=sp->y1;
			R->y2=sp->y2;
			R->y3=sp->y3;
			R->z1=sp->z1;
			R->z2=sp->z2;
			R->z3=sp->z3;
	    		    
	    }else{
			sprintf(WarningBuff,"CExpressPop - variable (%s) of type %d cannot be popped\n",sp->name,sp->type);
			WarningExpression(e,WarningBuff);
			return doReturn(1);
	    }
	}else{
			sprintf(WarningBuff,"CExpressPop - variable (%s) of type %d  cannot be popped\n",R->name,R->type);
			WarningExpression(e,WarningBuff);
			return doReturn(1);
	}
		
	return 0;
	
}
int CExpressPush(CExpressPtr e,int pushWhat)
{
	stackHolderPtr s;
	unsigned int vret;
	double value;
	
	if(!e)return doReturn(1);
	s = &e->stackData;
	if(checkStack(s))return doReturn(1);
	
	zerol((char *)&s->stackList[s->stackCount],sizeof(struct stack));
	
	s->stackList[s->stackCount].type=pushWhat;
	
	switch(pushWhat){
	case T_STACK_R1:
	   s->stackList[s->stackCount]=e->R1;
	break;		
	case T_STACK_R2:
	   s->stackList[s->stackCount]=e->R2;
	break;		
	case T_STACK_RR:
		if(e->RR){
			s->stackList[s->stackCount] = *(e->RR);
			cFree((char *)e->RR);
			e->RR = NULL;
		}else{
			sprintf(WarningBuff,"CExpressPush - NULL RR Pointer\n");
			WarningExpression(e,WarningBuff);
			return doReturn(1);
		}
	break;		
	case T_STACK_CONSTANT:
	   if(e->ch[0] == '0'){
	      if((e->ch[1] == 'x') || (e->ch[1] == 'X') ){
	          if(atoh(e->ch+2,&vret))goto vdouble;
	      }else{
	          if(atoo(e->ch+1,&vret))goto vdouble;
	      }
	      value=(double)vret;
	   }else{
vdouble:
	       value=atof(e->ch);
	   }
	   s->stackList[s->stackCount].value=value;
	break;		
	case T_STACK_NAME:
	case T_STACK_STRING:
	   mstrncpy(s->stackList[s->stackCount].name,e->ch,256);
	   s->stackList[s->stackCount].pioName[0]=0;
	   s->stackList[s->stackCount].pioIndex=0;
	break;		
	
	case T_STACK_NAME_STACK:
		s = &e->nameData;
		if(checkStack(s))return doReturn(1);
	    s->stackList[s->stackCount]=e->R1;
	break;		
		
	case T_STACK_OUT_STACK:
		s = &e->outData;
		if(checkStack(s))return doReturn(1);
	    s->stackList[s->stackCount]=e->R1;
	break;			
	
	
	default:
	    sprintf(WarningBuff,"CExpressPush Unknown Push %d\n",pushWhat);
		WarningExpression(e,WarningBuff);
		return doReturn(1);
	}
	++(s->stackCount);
	return 0;
}
int atoh(char *s,unsigned int *ret)
{
	char m,c;
	unsigned int n;

	n=0;
	while((c=toupper(*(s++))) != 0){
	    if((c >=  '0' &&  c <= '9') || 
	       (c >=  'A' &&  c <= 'F')){
	        if((m=c-'0') > 9)m-=7;
	        n=16*n+m;
	    }else{
	        return doReturn(1);
	    }
	}

	*ret = n;

	return 0;
}
int atoo(char *s,unsigned int *ret)
{
	char c;
	unsigned n;

	n=0;
	while((c = *s++) != 0){
	    if(c < '0' || c > '7')return doReturn(1);
	    n=8*n+c-'0';
	}
	*ret = n;

	return 0;
}
int CExpressNext(CExpressPtr e)
{
	int k,c,n;
	
	if(!e || !e->dat)return doReturn(1);
	
	
	if(CExpressSkip(e)){
		e->tok = T_EOF;
		return 0;
	}
/*	
	sprintf(WarningBuff,"CExpressNext %s\n",e->dat);
	WarningExpression(e,WarningBuff);
*/	
	c=e->dat[0];
	e->dat += 1;
	if(isalpha(c)){
		k=1;
        n=0;
		e->ch[0]=c;
        while(isalpha(c=e->dat[n]) || isdigit(c) || c == '_'){
        	n++;
        	if(k >= 255)break;
        	e->ch[k++]=c;
        }
        e->dat += n;
        e->ch[k]='\0';
        
        
        for(n=0;n<nfunctions;++n){
        	if(!strcmp(e->ch,functions[n].name)){
        		e->tok = functions[n].classf;
        		e->function = n;
 /*       		
sprintf(WarningBuff,"FUNCTION %s\n",e->ch);
WarningExpression(e,WarningBuff);
*/
        		return 0;
        	}
        }
        
		e->tok = T_IDENTIFIER;
/*
sprintf(WarningBuff,"IDENTIFIER %s\n",e->ch);
WarningExpression(e,WarningBuff);
*/
	}else if(c == '"'){
	
		k=0;
        n=0;
        while((c=e->dat[n]) && c != '"'){
        	n++;
        	if(k >= 255)break;
        	e->ch[k++]=c;
        }
        if(c == '"')++n;
        e->dat += n;
        e->ch[k]='\0';
        
        
		e->tok = T_STACK_STRING;
/*
sprintf(WarningBuff,"IDENTIFIER %s\n",e->ch);
WarningExpression(e,WarningBuff);
*/
	
	}else if(isdigit(c) || c == '.'){
        k=1;
        n=0;
        e->ch[0]=c;
        if(c != '.'){
            while(isdigit(c=e->dat[n])){
            	n++;
        		if(k >= 255)break;
            	e->ch[k++]=c;
           	}
            if(c == '.'){
                e->ch[k++]=c;
                ++n;
			}                   
        }
        if(c == '.'){
            while(isdigit(c=e->dat[n])){
            	n++;
        		if(k >= 255)break;
            	e->ch[k++]=c;
            }
        }
        if(c == 'E' || c == 'e'){
            e->ch[k++]='e';
            ++n;
            e->ch[k++]=e->dat[n++];
            while(isdigit(c=e->dat[n])){
            	n++;
        		if(k >= 255)break;
            	e->ch[k++]=c;
            }
        }
        e->dat += n;
        e->ch[k]='\0';
		e->tok = T_CONSTANT;
/*
sprintf(WarningBuff,"T_CONSTANT %s\n",e->ch[k]);
WarningExpression(e,WarningBuff);
*/
	}else{
		switch(c){
			case ',' :
				e->tok = T_COMMA;
				break;
			case '{' :
				e->tok = T_CURLY_LEFT;
				break;
			case '}' :
				e->tok = T_CURLY_RIGHT;
				break;
			case ';' :
				e->tok = T_SEMICOLON;
				break;
			case '(' :
				e->tok = T_PAREN_LEFT;
				break;
			case ')' :
				e->tok = T_PAREN_RIGHT;
				break;
			case '[' :
				e->tok = T_SQUARE_LEFT;
				break;
			case ']' :
				e->tok = T_SQUARE_RIGHT;
				break;
			case '<' :
				if(e->dat[0] == '='){
					e->tok = T_LESSEQUAL;
					e->dat += 1;
				}else if(e->dat[0] == '<'){
					e->tok = T_LEFTSHIFT;
					e->dat += 1;
					if(e->dat[0] == '='){
						e->tok = T_LEFTSHIFTEQUAL;
						e->tok2 = T_LEFTSHIFT;
						e->dat += 1;
					}
				}else{
					e->tok = T_LESS;
				}
				break;
			case '>' :
				if(e->dat[0] == '='){
					e->tok = T_GREATEQUAL;
					e->dat += 1;
				}else if(e->dat[0] == '>'){
					e->tok = T_RIGHTSHIFT;
					e->dat += 1;
					if(e->dat[0] == '='){
						e->tok = T_RIGHTSHIFTEQUAL;
						e->tok2 = T_RIGHTSHIFT;
						e->dat += 1;
					}
				}else{
					e->tok = T_GREAT;
				}
				break;
			case '=' :
				if(e->dat[0] == '='){
					e->tok = T_EQUALEQUAL;
					e->dat += 1;
				}else{
					e->tok = T_EQUAL;
					e->tok2 = T_EQUAL;
				}
				break;
			case '!' :
				if(e->dat[0] == '='){
					e->tok = T_NOTEQUAL;
					e->dat += 1;
				}else{
					e->tok = T_NOT;
				}
				break;
			case '^' :
				e->tok = T_XOR;
				if(e->dat[0] == '='){
					e->tok = T_XOREQUAL;
					e->tok2 = T_XOR;
					e->dat += 1;
				}
				break;
			case '+' :
				e->tok = T_PLUS;
				if(e->dat[0] == '='){
					e->tok = T_PLUSEQUAL;
					e->tok2 = T_PLUS;
					e->dat += 1;
				}else if(e->dat[0] == '+'){
					e->tok = T_UNARYINCREMENT;
					e->tok2 = T_PLUSPLUS;
					e->dat += 1;
				}
				break;
			case '-' :
				e->tok = T_MINUS;
				if(e->dat[0] == '='){
					e->tok = T_MINUSEQUAL;
					e->tok2 = T_MINUS;
					e->dat += 1;
				}else if(e->dat[0] == '-'){
					e->tok = T_UNARYINCREMENT;
					e->tok2 = T_MINUSMINUS;
					e->dat += 1;
				}
				break;
			case '|' :
				if(e->dat[0] == '|'){
					e->tok = T_LOR;
					e->dat += 1;
				}else if(e->dat[0] == '='){
					e->tok = T_OREQUAL;
					e->tok2 = T_OR;
					e->dat += 1;
				}else{
					e->tok = T_OR;
				}
				break;
			case '&' :
				if(e->dat[0] == '&'){
					e->tok = T_LAND;
					e->dat += 1;
				}else if(e->dat[0] == '='){
					e->tok = T_ANDEQUAL;
					e->tok2 = T_AND;
					e->dat += 1;
				}else{
					e->tok = T_AND;
				}
				break;
			case '*' :
				e->tok = T_MULT;
				if(e->dat[0] == '='){
					e->tok = T_MULTEQUAL;
					e->tok2 = T_MULT;
					e->dat += 1;
				}
				break;
			case '/' :
				e->tok = T_DIV;
				if(e->dat[0] == '='){
					e->tok = T_DIVEQUAL;
					e->tok2 = T_DIV;
					e->dat += 1;
				}
				break;
			case '%' :
				e->tok = T_MOD;
				if(e->dat[0] == '='){
					e->tok = T_MODEQUAL;
					e->tok2 = T_MOD;
					e->dat += 1;
				}
				break;
			case '~' :
				e->tok = T_NEGATE;
				break;
			case ':' :
				e->tok = T_COLON;
				break;
			default:
				e->tok = c;
				sprintf(WarningBuff,"Unknown token %d %c\n",e->tok,e->tok);
				WarningExpression(e,WarningBuff);
				return doReturn(1);
		}
	}
	return 0;
}
static int CExpressSkip(CExpressPtr e)
{
	int c,ncol;
	
	if(!e || !e->dat || !e->dat[0])return doReturn(1);
	ncol=0;
Start:
    while((c=e->dat[ncol]) == ' ' || c == '\t' || c == '\n'){
        if(c == '\n')e->lineNumber++;
        ncol++;
    }
    e->dat += ncol;
    if(!c)return 1;
    
    if(c != '/')return 0;
    if(e->dat[1] != '*')return 0;
	ncol=2;
    while((c=e->dat[ncol])){
        if(c == '\n')e->lineNumber++;
        ncol++;
        if(c == '*' && e->dat[ncol] == '/')break;
    }
    if(!c)return 0;
    ++ncol;
    goto Start;
}


int CExpressClose(CExpressPtr e)
{
	if(!e)return doReturn(1);
	if(e->stackData.stackList)cFree((char *)e->stackData.stackList);
	e->stackData.stackList=NULL;
	
	if(e->logicalData.stackList)cFree((char *)e->logicalData.stackList);
	e->logicalData.stackList=NULL;
	
	if(e->nameData.stackList)cFree((char *)e->nameData.stackList);
	e->nameData.stackList=NULL;
	
	if(e->outData.stackList)cFree((char *)e->outData.stackList);
	e->outData.stackList=NULL;
	
	cFree((char *)e);
	return 0;
}

static int checkStack(stackHolderPtr h)
{
	struct stack *stackList;
        long stackMax;

	if(!h)return doReturn(1);

	if(h->stackCount+1 < h->stackMax)return 0;

	stackMax = h->stackMax+STACK_INCREMENT;

	stackList=NULL;
	if(h->stackList){
	    stackList=(struct stack *)cRealloc((char *)h->stackList,stackMax*sizeof(struct stack),8258);
	    if(!stackList){
	        WarningExpression(NULL,"checkStack out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)&stackList[h->stackMax],STACK_INCREMENT*sizeof(struct stack));
	}else{
	    stackList=(struct stack *)cMalloc(stackMax*sizeof(struct stack),8035);
	    if(!stackList){
	        WarningExpression(NULL,"checkStack out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)stackList,stackMax*sizeof(struct stack));
	}

	h->stackMax=stackMax;
	h->stackList=stackList;
	return 0;
ErrorOut:
	return doReturn(1);
}
long CExpressGetFrameCount(CExpressPtr e)
{
	struct Message1000 mo;
	stackHolderPtr s;
	long n;
	long ret;
	
	ret = -1L;
	
	if(!e)return ret;
	
	s = &e->nameData;
		
	if(s->stackCount > 0){
	    for(n=0;n<s->stackCount;++n){
    		mo.FrameCount=0;
    		if(!SendMessageByName(s->stackList[n].name,MessageType_GET_FRAME_COUNT,&mo)){
		        if(mo.FrameCount > ret)ret=mo.FrameCount;
		    }
	    }
	}
	return ret;
}

