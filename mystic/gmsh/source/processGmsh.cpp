#define EXTERN22 extern
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <vector>
#include <string>

#include "processGmsh.h"
#include "showlib.h"

using namespace std;

typedef double Flt ;
typedef Flt Vec[3] ;

#define VecSub(a,b,c)	 (c)[0]=(a)[0]-(b)[0];\
			 (c)[1]=(a)[1]-(b)[1];\
			 (c)[2]=(a)[2]-(b)[2]

#define VecCross(a,b,c)	 (c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1];\
			 (c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2];\
			 (c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0]
			 
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])

#define VecLen(a)	(sqrt(VecDot(a,a)))

#define VecNegate(a)	(a)[0]=0-(a)[0];\
			(a)[1]=0-(a)[1];\
			(a)[2]=0-(a)[2];

Flt VecNormalize(Vec vec);
			 


#define FILE_NAME_SEPERATOR_CHAR	'/'
// cc -c  showlib.c -Wall
//c++ -o processGmsh processGmsh.cpp showlib.o -Wall

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

struct nodeStruct{
   double x;
   double y;
   double z;
};

vector<nodeStruct> Nodes;

vector<nodeStruct> Forces;

struct lineStruct{
    long mat;
    long n1;
    long n2;
};

vector<lineStruct> Lines;

struct triStruct{
    int  mat;
    int  n1;
    int  n2;
    int  n3;
    int  n4;
};

vector<triStruct> Tri;

struct quadStruct{
    int  mat;
    int  n1;
    int  n2;
    int  n3;
    int  n4;
};

vector<quadStruct> Quads;

struct hex8Struct{
    int mat;
    int n1;
    int n2;
    int n3;
    int n4;
    int n5;
    int n6;
    int n7;
    int n8;
};

vector<hex8Struct> Hex8;

struct FixStruct{
    int ifix;
    int node;
};

vector<FixStruct> Fix;

vector<string> Mat;

int doPlot();

int WriteFile();


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
	    fprintf(stderr,"Could Not Open %s To Read Errno %d\n",pathname,errno);
        GetWorking((char *)working,(long)sizeof(working));
        fprintf(stderr,"Working Directory = %s\n",working);
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
	
	 WriteFile();
	
	fprintf(stderr,"Total Time in processFile %.2f Seconds\n",end-start);
		
	if(input)fclose(input);
    
	
	return 0;
}
int WriteFile()
{


	printf("gmsh example\n");
	printf("poff stre \n");
	printf("pon plot dof node equl\n");
	printf("poff elem comp dis\n");
	printf("pnt,50,1,1\n");
	printf("1,10,1\n");
	printf("end\n");

	if(Hex8.size() > 0){
		printf("nodes,%ld,3,3\n",Nodes.size());
	}else{
		printf("nodes,%ld,2,2\n",Nodes.size());
	}
	for(vector<string>::size_type n=0;n<Nodes.size();++n){
	    printf("%lu %.18g %.18g %.18g\n",n+1,Nodes[n].x,Nodes[n].y,Nodes[n].z);
	}
	for(vector<FixStruct>::size_type n=0;n<Fix.size();++n){
		printf("Fix %d %d\n",Fix[n].ifix,Fix[n].node);	
	}
	printf("end\n");
	
	if(Hex8.size() > 0){
		int nmat=0;
		for(vector<hex8Struct>::size_type n=0;n<Hex8.size();++n){
			if(Hex8[n].mat > nmat)nmat=Hex8[n].mat;
		}
		if(nmat == 0)nmat=1;
	
		printf("hex8,%ld,%d,0\n",Hex8.size(),nmat);

		for(int n=0;n<nmat;++n){
			if(Mat.size() > 0){
				if(n < Mat.size()){
						printf("%s\n",Mat[n].c_str());
				}else{
					printf("%s\n",Mat[0].c_str());
				}
			}else{
				printf("0 2 1 1 1 1. 2.56e11 .42 0. 2.64\n");
			}
		}
	
		for(vector<hex8Struct>::size_type n=0;n<Hex8.size();++n){
			printf("%lu %d %d %d %d %d %d %d %d %d\n",n+1,Hex8[n].mat,Hex8[n].n1+1,Hex8[n].n2+1,Hex8[n].n3+1,
			Hex8[n].n4+1,Hex8[n].n5+1,Hex8[n].n6+1,Hex8[n].n7+1,Hex8[n].n8+1);
		}
	}else if(Quads.size() > 0){
		int nmat=0;
		for(vector<hex8Struct>::size_type n=0;n<Quads.size();++n){
			if(Quads[n].mat > nmat)nmat=Quads[n].mat;
		}
		if(nmat == 0)nmat=1;
	
		printf("quad,%ld,%d,0\n",Quads.size(),nmat);
		
		for(int n=0;n<nmat;++n){
			if(Mat.size() > 0){
				if(n < Mat.size()){
						printf("%s\n",Mat[n].c_str());
				}else{
					printf("%s\n",Mat[0].c_str());
				}
			}else{
				printf("0 2 1 1 1. 2.56e11 .42 0. 2.64\n");
			}
		}
		
		for(vector<hex8Struct>::size_type n=0;n<Quads.size();++n){
			printf("%lu %d %d %d %d %d\n",n+1,Quads[n].mat,Quads[n].n1+1,Quads[n].n2+1,Quads[n].n3+1,
			Quads[n].n4+1);
		}
		
	}else if(Tri.size() > 0){
		int nmat=0;
		for(vector<triStruct>::size_type n=0;n<Tri.size();++n){
			if(Tri[n].mat > nmat)nmat=Tri[n].mat;
		}
		if(nmat == 0)nmat=1;
	
		printf("tri,%ld,%d,0\n",Tri.size(),nmat);
		
		for(int n=0;n<nmat;++n){
			if(Mat.size() > 0){
				if(n < Mat.size()){
						printf("%s\n",Mat[n].c_str());
				}else{
					printf("%s\n",Mat[0].c_str());
				}
			}else{
				printf("0 2 1 1 1. 2.56e11 .42 0. 2.64\n");
			}
		}
		
		for(vector<triStruct>::size_type n=0;n<Tri.size();++n){
			printf("%lu %d %d %d %d\n",n+1,Tri[n].mat,Tri[n].n1+1,Tri[n].n2+1,Tri[n].n3+1);
		}

	}
	printf("form\n");
	
	printf("load\n");
	for(vector<nodeStruct>::size_type n=0;n<Forces.size();++n){
		if(Forces[n].x != 0){
			printf("%lu %g 1\n",n+1,Forces[n].x);
		}
		if(Forces[n].y != 0){
			printf("%lu %g 2\n",n+1,Forces[n].y);
		}
		if(Forces[n].z != 0){
			printf("%lu %g 3\n",n+1,Forces[n].z);
		}
	}
	printf("end\n");
	printf("solv,stre,1,end\n");
	printf("stop\n");
	
	if(Mat.size() < 1){
	    fprintf(stderr,"No Materials Found\n");
	}
	
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
int doMat(BatchPtr Batch,struct CommandInfo *cp)
{
    char line2[4096];
    double value;
    int nmat;
    int ret;
    
    ++cp->n;
    ret=doubleCommand(&value,cp);
    nmat=value;
    fprintf(stderr,"nmat %d\n",nmat);
    for(int n=0;n<nmat;++n){
         if(BatchNextLine(Batch,line2,sizeof(line2)))return 1;
         fprintf(stderr,"%s\n",line2);
         Mat.push_back(line2);
    }


    
    return 1;
}
int doBC(BatchPtr Batch,struct CommandInfo *cp)
{
    char line2[4096];
    double totalArea;
	char *command;
    double value;
    int line;
    int point;
    int curve;
    int ifix;
    
    struct nodeStruct node2;
    
    struct FixStruct fix;
    
    node2.x=0;
    node2.y=0;
    node2.z=0;
    
 	for(int n=0;n<Nodes.size();++n){
 		Forces.push_back(node2);
    }
    
    while(1){
    	if(BatchNextLine(Batch,line2,sizeof(line2)))return 1;
    	if(getCommand(line2,cp))return 1;
 		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		if(!mstrcmp((char *)"end",command)){
		    break;
		}else if(!mstrcmp((char *)"force",command)){
		   	command=cp->command[++cp->n];
		   	if(!mstrcmp((char *)"curve",command)){
				curve=cp->value[++cp->n];
				double valuex=cp->value[++cp->n];
				double valuey=cp->value[++cp->n];
				double valuez=cp->value[++cp->n];
			    for(int n=0;n<Lines.size();++n){
			        if(Lines[n].mat == curve){
			            if(n == 0 ||  Lines[n].mat != Lines[n-1].mat){
							Forces[Lines[n].n1].x += valuex*0.5;
							Forces[Lines[n].n1].y += valuey*0.5;
							Forces[Lines[n].n1].z += valuez*0.5;
						}else{
							Forces[Lines[n].n1].x += valuex;
							Forces[Lines[n].n1].y += valuey;
							Forces[Lines[n].n1].z += valuez;
						}
						//fprintf(stderr,"n %d n1 %ld %lu\n",n,Lines[n].n1,Lines.size()-1);
			    		if(n == Lines.size()-1 || Lines[n].mat != Lines[n+1].mat){
						   //fprintf(stderr,"n %d n2 %ld\n",n,Lines[n].n2);
							Forces[Lines[n].n2].x += valuex*0.5;
							Forces[Lines[n].n2].y += valuey*0.5;
							Forces[Lines[n].n2].z += valuez*0.5;
			    		}
			    	}
			    }
			}else{
		         fprintf(stderr,"force unknown command %s\n",command);
			}		
		}else if(!mstrcmp((char *)"traction",command)){
			double valuex=cp->value[++cp->n];
			double valuey=cp->value[++cp->n];
			double valuez=cp->value[++cp->n];
			curve=(int)cp->value[++cp->n];
			//printf("--traction valuex %g valuey %g valuey %g curve %d\n",valuex,valuey,valuez,curve);
			valuex *= 0.3333333333333333;
			valuey *= 0.3333333333333333;
			valuez *= 0.3333333333333333;
			totalArea=0;
			for(int n=0;n<Quads.size();++n){
			    Vec v1,v2,normal1,normal2,points[3];
			    double area1,area2;
			    if(Quads[n].mat == curve){
			    	points[0][0]=Nodes[Quads[n].n1].x;
			    	points[0][1]=Nodes[Quads[n].n1].y;
			    	points[0][2]=Nodes[Quads[n].n1].z;
			    	points[1][0]=Nodes[Quads[n].n3].x;
			    	points[1][1]=Nodes[Quads[n].n3].y;
			    	points[1][2]=Nodes[Quads[n].n3].z;
			    	points[2][0]=Nodes[Quads[n].n2].x;
			    	points[2][1]=Nodes[Quads[n].n2].y;
			    	points[2][2]=Nodes[Quads[n].n2].z;
			    	VecSub(points[1],points[0],v1);
					VecSub(points[2],points[0],v2);
					VecCross(v1,v2,normal1);
					area1=0.5*VecNormalize(normal1);
				    totalArea += area1;
					
			    	points[1][0]=Nodes[Quads[n].n4].x;
			    	points[1][1]=Nodes[Quads[n].n4].y;
			    	points[1][2]=Nodes[Quads[n].n4].z;
			    	points[2][0]=Nodes[Quads[n].n3].x;
			    	points[2][1]=Nodes[Quads[n].n3].y;
			    	points[2][2]=Nodes[Quads[n].n3].z;
			    	VecSub(points[1],points[0],v1);
					VecSub(points[2],points[0],v2);
					VecCross(v1,v2,normal2);
					area2=0.5*VecNormalize(normal2);
				    totalArea += area2;
				
					Forces[Quads[n].n1].x += (area1)*valuex;
					Forces[Quads[n].n1].y += (area1)*valuey;
					Forces[Quads[n].n1].z += (area1)*valuez;
					
					Forces[Quads[n].n2].x += (area1)*valuex;
					Forces[Quads[n].n2].y += (area1)*valuey;
					Forces[Quads[n].n2].z += (area1)*valuez;
					
					Forces[Quads[n].n3].x += (area1)*valuex;
					Forces[Quads[n].n3].y += (area1)*valuey;
					Forces[Quads[n].n3].z += (area1)*valuez;
										
					
					Forces[Quads[n].n1].x += (area2)*valuex;
					Forces[Quads[n].n1].y += (area2)*valuey;
					Forces[Quads[n].n1].z += (area2)*valuez;
									
					Forces[Quads[n].n3].x += (area2)*valuex;
					Forces[Quads[n].n3].y += (area2)*valuey;
					Forces[Quads[n].n3].z += (area2)*valuez;
					
					Forces[Quads[n].n4].x += (area2)*valuex;
					Forces[Quads[n].n4].y += (area2)*valuey;
					Forces[Quads[n].n4].z += (area2)*valuez;
	/*
			     	printf("n %d area %g %g n1 %g %g %g n2 %g %g %g\n",n,area1,area2,
			     	       normal1[0],normal1[1],normal1[2],normal2[0],normal2[1],normal2[2]);
	*/
			       }
			   }
			   fprintf(stderr,"traction Surface %d Total Area %g\n",curve,totalArea);
		}else if(!mstrcmp((char *)"press",command)){
			value=cp->value[++cp->n];
			curve=(int)cp->value[++cp->n];
			//printf("--press value %g curve %d\n",value,curve);
			value *= 0.3333333333333333;
			totalArea=0;
			for(int n=0;n<Quads.size();++n){
			    Vec v1,v2,normal1,normal2,points[3];
			    double area1,area2;
			    if(Quads[n].mat == curve){
			    	points[0][0]=Nodes[Quads[n].n1].x;
			    	points[0][1]=Nodes[Quads[n].n1].y;
			    	points[0][2]=Nodes[Quads[n].n1].z;
			    	points[1][0]=Nodes[Quads[n].n3].x;
			    	points[1][1]=Nodes[Quads[n].n3].y;
			    	points[1][2]=Nodes[Quads[n].n3].z;
			    	points[2][0]=Nodes[Quads[n].n2].x;
			    	points[2][1]=Nodes[Quads[n].n2].y;
			    	points[2][2]=Nodes[Quads[n].n2].z;
			    	VecSub(points[1],points[0],v1);
					VecSub(points[2],points[0],v2);
					VecCross(v1,v2,normal1);
					area1=0.5*VecNormalize(normal1);
					totalArea += area1;

			    	points[1][0]=Nodes[Quads[n].n4].x;
			    	points[1][1]=Nodes[Quads[n].n4].y;
			    	points[1][2]=Nodes[Quads[n].n4].z;
			    	points[2][0]=Nodes[Quads[n].n3].x;
			    	points[2][1]=Nodes[Quads[n].n3].y;
			    	points[2][2]=Nodes[Quads[n].n3].z;
			    	VecSub(points[1],points[0],v1);
					VecSub(points[2],points[0],v2);
					VecCross(v1,v2,normal2);
					area2=0.5*VecNormalize(normal2);
					totalArea += area2;
					
					Forces[Quads[n].n1].x += value*(area1)*normal1[0];
					Forces[Quads[n].n1].y += value*(area1)*normal1[1];
					Forces[Quads[n].n1].z += value*(area1)*normal1[2];
					
					Forces[Quads[n].n2].x += value*(area1)*normal1[0];
					Forces[Quads[n].n2].y += value*(area1)*normal1[1];
					Forces[Quads[n].n2].z += value*(area1)*normal1[2];
					
					Forces[Quads[n].n3].x += value*(area1)*normal1[0];
					Forces[Quads[n].n3].y += value*(area1)*normal1[1];
					Forces[Quads[n].n3].z += value*(area1)*normal1[2];
										
					
					Forces[Quads[n].n1].x += value*(area2)*normal2[0];
					Forces[Quads[n].n1].y += value*(area2)*normal2[1];
					Forces[Quads[n].n1].z += value*(area2)*normal2[2];
									
					Forces[Quads[n].n3].x += value*(area2)*normal2[0];
					Forces[Quads[n].n3].y += value*(area2)*normal2[1];
					Forces[Quads[n].n3].z += value*(area2)*normal2[2];
					
					Forces[Quads[n].n4].x += value*(area2)*normal2[0];
					Forces[Quads[n].n4].y += value*(area2)*normal2[1];
					Forces[Quads[n].n4].z += value*(area2)*normal2[2];
	/*
			     	printf("n %d area %g %g n1 %g %g %g n2 %g %g %g\n",n,area1,area2,
			     	       normal1[0],normal1[1],normal1[2],normal2[0],normal2[1],normal2[2]);
	*/
			    }
			}
			fprintf(stderr,"press Surface %d Total Area %g\n",curve,totalArea);
		}else if(!mstrcmp((char *)"fix",command)){
		   	command=cp->command[++cp->n];
		   	if(!mstrcmp((char *)"point",command)){
				point=cp->value[++cp->n];
				ifix=(int)cp->value[++cp->n];				
				fix.ifix=ifix;
				fix.node=point;
				Fix.push_back(fix);
			    //printf("--fix point %d ifix %d\n",point,ifix);
			    //printf("fix %d %d\n",ifix,point);
	    	}else if(!mstrcmp((char *)"curve",command)){
				line=cp->value[++cp->n];
				ifix=(int)cp->value[++cp->n];
			    //fprintf(stderr,"--fix line %d ifix %d Lines.size() %ld\n",line,ifix,(long)Lines.size());
			    for(int n=0;n<Lines.size();++n){
			        if(Lines[n].mat == line){
						fix.ifix=ifix;
						fix.node=Lines[n].n1+1;
						Fix.push_back(fix);
			     		//printf("fix %ld %ld\n",(long)ifix,Lines[n].n1+1);
			    		if(n == Lines.size()-1 || Lines[n].mat != Lines[n+1].mat){
			    		   //printf("fix %ld %ld\n",(long)ifix,Lines[n].n2+1);
						   fix.node=Lines[n].n2+1;
						   Fix.push_back(fix);
			    		}
			    	}
			    }
		    }else if(!mstrcmp((char *)"surface",command)){
				curve=cp->value[++cp->n];
				ifix=(int)cp->value[++cp->n];
			    //printf("--fix curve %d ifix %d\n",curve,ifix);
			    for(int n=0;n<Quads.size();++n){
			        if(Quads[n].mat == curve){
			     		//printf("fix %ld %ld\n",(long)ifix,Quads[n].n1+1);
			    		//printf("fix %ld %ld\n",(long)ifix,Quads[n].n2+1);
			    		//printf("fix %ld %ld\n",(long)ifix,Quads[n].n3+1);
			    		//printf("fix %ld %ld\n",(long)ifix,Quads[n].n4+1);
			    		fix.ifix=ifix;
						fix.node=Quads[n].n1+1;
						Fix.push_back(fix);
						fix.node=Quads[n].n2+1;
						Fix.push_back(fix);
						fix.node=Quads[n].n3+1;
						Fix.push_back(fix);
						fix.node=Quads[n].n4+1;
						Fix.push_back(fix);
			    	}
			    }
		     }else{
		         fprintf(stderr,"Fix unknown command %s\n",command);
		     }
		}else{
		    fprintf(stderr,"BC unknown command %s\n",command);
		}
   }
   
  
 ErrorOut:   
    return 0;
}
Flt VecNormalize(Vec vec)
{
	Flt len ;
	len = (Flt) VecLen(vec);
	if(len > 0.0){
		vec[0]/=len ;
		vec[1]/=len ;
		vec[2]/=len ;
	}
	return(len) ;
}

int doBatch(BatchPtr Batch,CommandPtr cp)
{
	char *command;
	int ret;
	
	if(!Batch || !cp)return 1;
	
	if(cp->type[cp->n] == BATCH_DOUBLE){
		ret = 0;
		goto ErrorOut;
	}

	ret = 1;
	command=stringCommand(cp);
	if(!command)goto ErrorOut;

   // fprintf(stderr,"command %s\n",command);
	
	if(!mstrcmp((char *)"$Nodes",command)){
        doNodes(Batch,cp);
    }else if(!mstrcmp((char *)"$Elements",command)){
        doElements(Batch,cp);
    }else if(!mstrcmp((char *)"$EndElements",command)){
         doPlot();
    }else if(!mstrcmp((char *)"bc",command)){
         doBC(Batch,cp);
    }else if(!mstrcmp((char *)"mat",command)){
         doMat(Batch,cp);
	}else{
	    fprintf(stderr,"doBatch Unknown Command %s\n",command);
	}
	
	ret = 0;
	
ErrorOut:
	return ret;
}
int doElements(BatchPtr Batch,struct CommandInfo *cp)
{
    char line[4096];
    double value0;
    int *nodesconnect;
    int nel;
    int ret;
    
    
    
    struct lineStruct line2;
    struct quadStruct quad;
    struct triStruct tri;
    struct hex8Struct hex8;
    
    nodesconnect=(int *)malloc(nodes*sizeof(int));
    if(!nodesconnect){
         fprintf(stderr,"nodesconnect out of memory %ld\n",nodes*sizeof(int));
         exit(1);
    }

	memset(nodesconnect,0,nodes*sizeof(int));

    if(BatchNextLine(Batch,line,sizeof(line)))return 1;
    if(getCommand(line,cp))return 1;
    ret=doubleCommand(&value0,cp);
    nel=value0;
    fprintf(stderr,"nel %d\n",nel);
    for(int k=0;k<nel;++k){
        int type;
        int node;
        int mat;
        int n1,n2,n3,n4,n5,n6,n7,n8;
        if(BatchNextLine(Batch,line,sizeof(line)))break;
    	if(getCommand(line,cp))return 1;
    	type=cp->value[1];
        if(type == 15){
          //fprintf(stderr,"type %d \n",type);
          continue;
        }else if(type == 1){
        	mat=cp->value[4];
        	n1=cp->value[5];
        	n2=cp->value[6];
           // fprintf(stderr,"type %d mat %d %d %d\n",type,mat,n1,n2);
            line2.mat=mat;
            line2.n1=n1-1;
            line2.n2=n2-1;
            Lines.push_back(line2);
            continue;
        }else if(type == 2){
         	mat=cp->value[4];
        	n1=cp->value[5];
        	n2=cp->value[6];
        	n3=cp->value[7];
            tri.mat=mat;
            tri.n1=n1-1;
            tri.n2=n2-1;
            tri.n3=n3-1;
            Tri.push_back(tri);
            nodesconnect[n1-1]=1;
            nodesconnect[n2-1]=1;
            nodesconnect[n3-1]=1;
        //fprintf(stderr,"type %d mat %d %d %d %d %d\n",type,mat,n1,n2,n3,n4);
        }else if(type == 3){
         	mat=cp->value[4];
        	n1=cp->value[5];
        	n2=cp->value[6];
        	n3=cp->value[7];
        	n4=cp->value[8];
            quad.mat=mat;
            quad.n1=n1-1;
            quad.n2=n2-1;
            quad.n3=n3-1;
            quad.n4=n4-1;
            Quads.push_back(quad);
            nodesconnect[n1-1]=1;
            nodesconnect[n2-1]=1;
            nodesconnect[n3-1]=1;
            nodesconnect[n4-1]=1;
        //fprintf(stderr,"type %d mat %d %d %d %d %d\n",type,mat,n1,n2,n3,n4);
        }else if(type == 5){
         	mat=cp->value[4];
        	n1=cp->value[5];
        	n2=cp->value[6];
        	n3=cp->value[7];
        	n4=cp->value[8];
        	n5=cp->value[9];
        	n6=cp->value[10];
        	n7=cp->value[11];
        	n8=cp->value[12];
            hex8.mat=mat;
            hex8.n1=n1-1;
            hex8.n2=n2-1;
            hex8.n3=n3-1;
            hex8.n4=n4-1;
            hex8.n5=n5-1;
            hex8.n6=n6-1;
            hex8.n7=n7-1;
            hex8.n8=n8-1;
            Hex8.push_back(hex8);
         //   fprintf(stderr,"type %d mat %d %d %d %d %d %d %d %d %d\n",type,mat,n1,n2,n3,n4,n5,n6,n7,n8);
         	(cp->n) += 4;
        	for(int m=0;m<9;++m){
          		ret=doubleCommand(&value0,cp);
           		++(cp->n);
          		node=value0;
          		//fprintf(stderr,"%d ",node);
          		nodesconnect[node-1]=1;
        	}
        	//fprintf(stderr,"\n");
      	}
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
     
    struct nodeStruct node2;
 
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
    nodes=value1;
    fprintf(stderr,"nodes %d\n",nodes);
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
        node2.x=x;
        node2.y=y;
        node2.z=z;
        Nodes.push_back(node2);
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
int doPlot()
{
    fprintf(stderr,"Nodes %lu ",Nodes.size());
    fprintf(stderr,"Hex8 %lu ",Hex8.size());
    fprintf(stderr,"Tri %lu ",Tri.size());
    fprintf(stderr,"Quads %lu ",Quads.size());
    fprintf(stderr,"Lines %lu \n",Lines.size());
    
    showPtr show=showStart((char *)"plot.shw",(char *)"w");
    
    show->stressMax=1;
    
    double *x=(double *)malloc(Nodes.size()*sizeof(double));
    double *y=(double *)malloc(Nodes.size()*sizeof(double));
    double *z=(double *)malloc(Nodes.size()*sizeof(double));
    int nume=Hex8.size();
    if(Lines.size() > nume)nume=Lines.size();
    if(Quads.size() > nume)nume=Quads.size();
    long *elements=(long *)malloc(9*nume*sizeof(long));
    double *stress=(double *)malloc(nume*sizeof(double));
    if(!x || !y || !z || !elements)goto ErrorOut;
    
    for(long int n=0;n<Nodes.size();++n){
         x[n]=Nodes[n].x;
         y[n]=Nodes[n].y;
         z[n]=Nodes[n].z;
    }
    
    
    if(Hex8.size() > 0){
		showNodes(show,Nodes.size(),x,y,z);
		for(long int n=0;n<Hex8.size();++n){
		   elements[n*9]=Hex8[n].mat;
		   elements[n*9+1]=Hex8[n].n1;
		   elements[n*9+2]=Hex8[n].n2;
		   elements[n*9+3]=Hex8[n].n3;
		   elements[n*9+4]=Hex8[n].n4;
		   elements[n*9+5]=Hex8[n].n5;
		   elements[n*9+6]=Hex8[n].n6;
		   elements[n*9+7]=Hex8[n].n7;
		   elements[n*9+8]=Hex8[n].n8;
		   stress[n]=Hex8[n].mat;
		}    
		showElements(show,Hex8.size(),8L,elements);
		showStress(show,Hex8.size(),stress);
		showEnd(show);
	}

	if(Quads.size() > 0){
		showNodes(show,Nodes.size(),x,y,z);
		for(long int n=0;n<Quads.size();++n){
		   elements[n*5]=Quads[n].mat;
		   elements[n*5+1]=Quads[n].n1;
		   elements[n*5+2]=Quads[n].n2;
		   elements[n*5+3]=Quads[n].n3;
		   elements[n*5+4]=Quads[n].n4;
		   stress[n]=Quads[n].mat;
		}    
		showElements(show,Quads.size(),4L,elements);
		showStress(show,Quads.size(),stress);
    	showEnd(show);
	}
	
    showDone(show);
    
    return 0;
ErrorOut:
	return 1;
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

