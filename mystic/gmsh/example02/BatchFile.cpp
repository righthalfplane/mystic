#define EXTERN22 extern
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/timeb.h>

#include "BatchFile.h"

#define FILE_NAME_SEPERATOR_CHAR	'/'

//g++ -o BatchFile BatchFile.cpp

int doWindow(double *x,double *y,long length,int type);

char *DefaultPathString(void);

int doForce(BatchPtr Batch,struct CommandInfo *cp);


int doElements(BatchPtr Batch,struct CommandInfo *cp2);


int doBatch(BatchPtr Batch,CommandPtr cp);

int mstrcmp(char *out,char *in);

char *GetWorking(char *buf, long size);

int mstrncpy(char *out,char *in,long n);

char *strsave(char *s,int tag);

static int GetTime(long *Seconds,long *milliseconds);

int doNodes(BatchPtr Batch,struct CommandInfo *cp);

double rtime();

static char working[256];

static char fname[256];

static struct Icon myIcon;

int nodes;

int main(int argc,char **argv)
{
	processFile(argv[1]);
	
	return 0;
}

int BatchNextLine(BatchPtr Batch,char *line,long len)
{
	if(!Batch || !line || (len <= 0))return 1;
	if(!Batch->input){
	    if(Batch->getLine){
	    	return (*Batch->getLine)(Batch->myIcon2,line,len);
	    }else{
	        return 1;
	    }
	}
	
	return NextLine(Batch->input,line,(int)len);
}
int processFile(char *pathname)
{
	struct BatchInfo Batch;
	char line[4096];
	double start,end;
	FILE *input;

	if(!pathname)return 1;
	
	memset((char *)&myIcon,0,sizeof(struct Icon));
	
	memset((char *)&Batch,0,sizeof(struct BatchInfo));
    
    //myIcon.pl=NULL;

	Batch.myIcon=&myIcon;
	
	input=NULL;
	
	start=rtime();
    
    char *p=strrchr(pathname,FILE_NAME_SEPERATOR_CHAR);
    if(p){
        mstrncpy(fname,p,(long)(strlen(p)+1));
    }else{
        mstrncpy(fname,pathname,(long)(strlen(pathname)+1));
    }

	input=fopen(pathname,"r");
	if(input == NULL){
	    printf("Could Not Open %s To Read Errno %d\n",pathname,errno);
        GetWorking((char *)working,(long)sizeof(working));
        printf("Working Directory = %s\n",working);
	    return 1;
	}
	
	Batch.input=input;
    
    fprintf(stderr,"started %s\n",pathname);
    
    try{

        while(1){
            if(BatchNextLine(&Batch,line,sizeof(line)))break;
            if(ProcessLine(line,&Batch))break;
        }
        
        //delete myIcon.pl;
        
    }
    catch(...)
    {
        fprintf(stderr,"Exception Processing File\n");
    }

	end=rtime();
	
	printf("Total Time in processFile %.2f Seconds\n",end-start);
		
	if(input)fclose(input);
    
	
	return 0;
}
int ProcessLine(char *line,BatchPtr Batch)
{
	struct CommandInfo cp;
	int ret;

	if(!line || !Batch)return 1;
	
	ret=1;

	memset((char *)&cp,0,sizeof(struct CommandInfo));

	if(getCommand(line,&cp))goto ErrorOut;
	
	for(cp.n=0;cp.n<cp.nword;++cp.n){
	    if(doBatch(Batch,&cp))goto ErrorOut;
	}
	
	ret=0;
	
ErrorOut:	

	getCommand(NULL,&cp);
	
	return ret;
}
int doBatch(BatchPtr Batch,CommandPtr cp)
{
	char *command;
    double value,value1,value2,value3,value4;
	int ret;
	
	if(!Batch || !cp)return 1;
	
    double pi=4.0*atan(1.0);

	ret = 1;
	command=stringCommand(cp);
	if(!command)goto ErrorOut;

   // fprintf(stderr,"command %s\n",command);
	
	if(!mstrcmp((char *)"nodes",command)){
        doNodes(Batch,cp);
     }else if(!mstrcmp((char *)"elements",command)){
        doElements(Batch,cp);
	}else{
	    printf("doBatch Unknown Command %s\n",command);
	    goto ErrorOut;
	}
	
	ret = 0;
	
ErrorOut:
	return ret;
}
int doElements(BatchPtr Batch,struct CommandInfo *cp2)
{
    struct CommandInfo cp;
    char line[4096];
    double value0;
    int *nodesconnect;
    int nel;
    int ret;
    
    nodesconnect=(int *)malloc(nodes*sizeof(int));
    if(!nodesconnect){
         fprintf(stderr,"nodesconnect out of memory %ld\n",nodes*sizeof(int));
         exit(1);
    }

	memset(nodesconnect,0,nodes*sizeof(int));

    memset((char *)&cp,0,sizeof(struct CommandInfo));

    if(BatchNextLine(Batch,line,sizeof(line)))return 1;
    if(getCommand(line,&cp))return 1;
    ret=doubleCommand(&value0,&cp);
    fprintf(stderr,"value0 %g\n",value0);
    nel=value0;
    for(int k=0;k<nel;++k){
        int node;
        fprintf(stderr,"%d ",k+1);
        if(BatchNextLine(Batch,line,sizeof(line)))break;
    	if(getCommand(line,&cp))return 1;
        (cp.n) += 4;
        for(int m=0;m<9;++m){
          ret=doubleCommand(&value0,&cp);
           ++(cp.n);
          node=value0;
          fprintf(stderr,"%d ",node);
          nodesconnect[node-1]=1;
        }
        fprintf(stderr,"\n");
    }
    
    for(int n=0;n<nodes;++n){
        if(nodesconnect[n] == 0){
            fprintf(stderr,"fix 7 %d\n",n+1);
        }
    }

	return 0;
}
int doNodes(BatchPtr Batch,struct CommandInfo *cp2)
{
    struct CommandInfo cp;
    char line[4096];
    double value1,value0;
    double xmin,xmax,ymin,ymax,zmin,zmax;
    double x,y,z;
    int ret;
    
    
    xmin=  1e30;
    xmax= -1e30;
    ymin=  1e30;
    ymax= -1e30;
    zmin=  1e30;
    zmax= -1e30;

    memset((char *)&cp,0,sizeof(struct CommandInfo));

    if(BatchNextLine(Batch,line,sizeof(line)))return 1;
    if(getCommand(line,&cp))return 1;
    ret=doubleCommand(&value1,&cp);
    fprintf(stderr,"value1 %g\n",value1);
    nodes=value1;
    for(int k=0;k<nodes;++k){
        if(BatchNextLine(Batch,line,sizeof(line)))break;
        if(getCommand(line,&cp))break;
        ret=doubleCommand(&value0,&cp);
        ++(cp.n);
        ret=doubleCommand(&x,&cp);
        ++(cp.n);
        ret=doubleCommand(&y,&cp);
        ++(cp.n);
        ret=doubleCommand(&z,&cp);
        ++(cp.n);
   		if(y < -3.99){
   		   int nn=value0;
   		   fprintf(stderr,"fix 7 %d\n",nn);
   		}  
   		if(y > -0.001){
   		   int nn=value0;
   		  //fprintf(stderr,"%d -1.0 2\n",nn);
   		}  
   		if(z > 3.99){
   		   int nn=value0;
   		  //fprintf(stderr,"fix 4 %d\n",nn);
   		}  
   		if(x < xmin)xmin = x;
  		if(y < ymin)ymin = y;
  		if(z < zmin)zmin = z;
   		if(x > xmax)xmax = x;
   		if(y > ymax)ymax = y;
   		if(z > zmax)zmax = z;
    }
    
    fprintf(stderr,"xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g\n",xmin,xmax,ymin,ymax,zmin,zmax);

	return 0;
}
int doForce(BatchPtr Batch,struct CommandInfo *cp)
{
    char *command;
    double value;
    int ret;

    ++(cp->n);
    command=stringCommand(cp);
    if(!command)goto ErrorOut;
    ++(cp->n);
    ret=doubleCommand(&value,cp);
    if(ret)goto ErrorOut;
    ++(cp->n);
    
    if(!mstrcmp((char *)"step",command)){
          
    }
ErrorOut:
    return 0;
}
int getCommand(char *line,CommandPtr cp)
{
	static char number[]={
			'0','1','2','3','4','5','6','7','8','9',
			'+','-','.'};
	char buff[256];
	int c,n,nw,iret,k,inum;
	
	if(!cp)return 1;

	for(n=0;n<256;++n){
	    cp->type[n]=BATCH_DOUBLE;
	    cp->value[n]=0;
	    if(cp->command[n])free((char *)cp->command[n]);
	    cp->command[n]=NULL;
	}

	cp->line=line;
	cp->nword=0;
	cp->n=0;
	
    if(!line)return 1;
    
	nw=0;
	do{

		while((c = *line) != 0 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')){
			line++;
		}
		
		n=0;
		if(c == 0){
			break;
		}else if(c == '"'){	
		    line++;	
			while((c = *line++) != 0 && c != '"' && n < 255 ){
				buff[n++]=c;
			}
			buff[n]=0;
			iret = 0;
	    	cp->type[nw]=BATCH_STRING;
		    cp->command[nw]=strsave(buff,9153);
		    if(!cp->command[nw]){
				return 1;
		    }
			continue;
		}else{
			while((c = *line++) != 0 && c != ' ' && c != '\n' && c != '\r' && c != '\t'
							 && n < 255 ){
				buff[n++]=c;
			}
		}
		
		if(c == ' ' || c == '"' || c == '\t'){
			iret = 0;
		}else{
			iret = 1;
		}
	
		buff[n]=0;
	
		if(!mstrcmp(buff,(char *)".") || !mstrcmp(buff,(char *)"..")){
	    	cp->type[nw]=BATCH_STRING;
		    cp->command[nw]=strsave(buff,9154);
		    if(!cp->command[nw])return 1;
			continue;
		}
	
		inum = 0;
		for(k=0;k<(int)sizeof(number);++k){
			if(*buff == number[k]){
				inum = 1;
				break;
			}
		}
	
		if(inum && (cp->getKind != BATCH_STRING)){
			 cp->value[nw] = atof(buff);
		}else{
		    cp->command[nw]=strsave(buff,9155);
		    if(!cp->command[nw])return 1;
	    	cp->type[nw]=BATCH_STRING;
		}
	
    }while((++nw < 256) &&  !iret);

    cp->nword = nw;

	cp->getKind=0;
	    	
    return 0;
}
int NextLine(FILE *input,char *line,int linelength)
{
	int nc;
	int c;

	if(!input || !line || linelength <= 0)return 1;

	--linelength;
	
doWhite:

	while(1){
	    c=fgetc(input);
	    if(c != ' ' || c != '\t' || c != ','){
	        break;
	    }
	}

	if(c == EOF){
	    return 1;
    }else if(c == '#'){
	    while(1){
	        c=fgetc(input);
	        if(c == '\n' || c == '\r' || c == EOF){
	            break;
	        }
	    }
	    if(c == EOF){
	        return 1;
	    }else{
	       goto doWhite;
	    }
	}

	ungetc(c,input);

	nc=0;
	while(1){
	    c=fgetc(input);
	    if(c == '\n' || c == '\r' || c == EOF){
	        break;
	    }
	    if(nc >= linelength)break;
	    line[nc++] = c;
	}

	if(c == EOF){
	    return 1;
	}
	
	if((nc == 0) && (linelength > 0))goto doWhite;
	
	line[nc]=0;
	
	return 0;
}
int doubleCommand(double *value,CommandPtr cp)
{
	if(!cp || !value)return 1;
	if(cp->n >= cp->nword)return 1;
	if(cp->type[cp->n] != BATCH_DOUBLE)return 1;
	*value=cp->value[cp->n];
	return 0;
}

char *stringCommand(CommandPtr cp)
{
	if(!cp)return NULL;
	if(cp->n >= cp->nword)return NULL;
	if(cp->type[cp->n] != BATCH_STRING)return NULL;
	return cp->command[cp->n];
}
int mstrcmp(char *out,char *in)
{
    char out2[256],in2[256];
    int c,n;

        if(!out || !in)return 0;

        mstrncpy(out2,out,255);
        for(n=0;n<255;++n){
            c=out2[n];
            if(!c)break;
            out2[n]=toupper(c);
        }

        mstrncpy(in2,in,255);
        for(n=0;n<255;++n){
            c=in2[n];
            if(!c)break;
            in2[n]=toupper(c);
        }

        return strcmp(out2,in2);
}

char *strsave(char *s,int tag)
{
        long length;
        char *p;

        if(!s)return NULL;

        length=(long)strlen(s)+1;

        if((p=(char *)malloc(length+1)) != NULL)
                mstrncpy(p,s,length);
        return(p);
}
int mstrncpy(char *out,char *in,long n)
{
        if(!out || !in || (n <= 0))return 1;

        while(n-- > 0){
            if(*in == 0){
                        *out = 0;
                        break;
            }else{
                        *out++ = *in++;
            }
        }

        return 0;
}
char *GetWorking(char *buf, long size)
{
        return getcwd(buf,size);
}
double rtime(void)
{
        long milliseconds;
        long Seconds;
        double ret;


        GetTime(&Seconds,&milliseconds);

        ret=(double)Seconds+(double)milliseconds/1000.;

        return ret;

}
static int GetTime(long *Seconds,long *milliseconds)
{
        struct timeb t;

        if(!Seconds || !milliseconds)return 1;


        ftime(&t);

        *Seconds=(long)t.time;
        *milliseconds=t.millitm;

        return 0;
}

