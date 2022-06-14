#define EXTERN22 extern
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/timeb.h>

#include "Minimize.h"


#include <boost/config.hpp>
#include <vector>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cuthill_mckee_ordering.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/bandwidth.hpp>


#define FILE_NAME_SEPERATOR_CHAR	'/'

//c++ -o Minimize -I/opt/local/include Minimize.cpp -Wall

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

long int nodes;

using namespace boost;
using namespace std;

typedef adjacency_list< vecS, vecS, undirectedS,
        property< vertex_color_t, default_color_type,
            property< vertex_degree_t, int > > >
        Graph;
typedef graph_traits< Graph >::vertex_descriptor Vertex;
typedef graph_traits< Graph >::vertices_size_type size_type;

//Graph G(19125);

Graph *G;

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
	FILE *input,*output;

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
    
    
    fprintf(stderr,"nodes %ld\n",nodes);
    
    output=fopen((char *)"Minimize.dat","wb");
	if(output == NULL){
	    printf("Could Not Open %s To Write Errno %d\n",(char *)"Minimize.dat",errno);
	    exit(1);
	}

    
    graph_traits< Graph >::vertex_iterator ui, ui_end;

    property_map< Graph, vertex_degree_t >::type deg = get(vertex_degree, *G);
    for (boost::tie(ui, ui_end) = vertices(*G); ui != ui_end; ++ui)
        deg[*ui] = degree(*ui, *G);

    property_map< Graph, vertex_index_t >::type index_map
        = get(vertex_index, *G);

    std::cout << "original bandwidth: " << bandwidth(*G) << std::endl;

    std::vector< Vertex > inv_perm(num_vertices(*G));
    
    std::vector< size_type > perm(num_vertices(*G));
 
    {
        // reverse cuthill_mckee_ordering
        cuthill_mckee_ordering(
            *G, inv_perm.rbegin(), get(vertex_color, *G), make_degree_map(*G));
        long long nnodes=nodes;
        fwrite(&nnodes, sizeof(long long), 1,output);

        cout << "Reverse Cuthill-McKee ordering:" << endl;
        cout << "  " << inv_perm.size() << "  ";
      //  long int nn=0;
        for (std::vector< Vertex >::const_iterator i = inv_perm.begin();
             i != inv_perm.end(); ++i){
            //cout << nn++ << " ";
            //cout << index_map[*i] << " ";
            //cout << endl;
        	nnodes=index_map[*i];
        	fwrite(&nnodes, sizeof(long long), 1,output);
        }

        for (size_type c = 0; c != inv_perm.size(); ++c)
            perm[index_map[inv_perm[c]]] = c;
        std::cout << "  bandwidth: "
                  << bandwidth(*G,
                         make_iterator_property_map(
                             &perm[0], index_map, perm[0]))
                  << std::endl;
    }
   


	end=rtime();
	

	printf("Total Time in Minimize %.2f Seconds\n",end-start);
		
	if(input)fclose(input);
	if(output)fclose(output);
    
	
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
int doTri(BatchPtr Batch,struct CommandInfo *cp2);
int doHex8(BatchPtr Batch,struct CommandInfo *cp2);
int doQuad(BatchPtr Batch,struct CommandInfo *cp2);
int doBatch(BatchPtr Batch,CommandPtr cp)
{
	char *command;
	int ret;
	
	if(!Batch || !cp)return 1;
	
    
    if(cp->type[cp->n] == BATCH_DOUBLE){
        ret = 0;
      //  fprintf(stderr,"doBatch Skip double  cp->n %d\n", cp->n);
        cp->n=cp->nword+1;
        goto ErrorOut;
    }

	ret = 1;
	command=stringCommand(cp);
	if(!command)goto ErrorOut;

   // fprintf(stderr,"command %s\n",command);
	
	if(!mstrcmp((char *)"nodes",command) || !mstrcmp((char *)"node",command) ){
        doNodes(Batch,cp);
     }else if(!mstrcmp((char *)"tri",command)){
        doTri(Batch,cp);
     }else if(!mstrcmp((char *)"hex8",command)){
        doHex8(Batch,cp);
     }else if(!mstrcmp((char *)"quad",command)){
        doQuad(Batch,cp);
     }else if(!mstrcmp((char *)"fix",command)){
        ;
	}else{
	    printf("doBatch Unknown Command %s\n",command);
	    cp->n=cp->nword+1;
	}
	
	ret = 0;
	
ErrorOut:
	return ret;
}
int doTri(BatchPtr Batch,struct CommandInfo *cp)
{
    char line[4096];
    double value0,value1;
    long int nel,mat;
    int ret;
    
    cp->n++;
    ret=doubleCommand(&value0,cp);
    if(ret){
       fprintf(stderr,"doTri Error \n");
    }
    
    nel=(long int )value0;
    
    cp->n++;
    ret=doubleCommand(&value0,cp);
    if(ret){
       fprintf(stderr,"doTri Error \n");
    }
    
    mat=(long int )value0;
    
    fprintf(stderr,"doTri nel %ld mat %ld\n",nel,mat);
  
  	for(long int k=0;k<mat;++k){
        if(BatchNextLine(Batch,line,sizeof(line)))break;
    	if(getCommand(line,cp))return 1;
  	}
  	
    for(long int k=0;k<nel;++k){
        size_t ind[4];
        if(BatchNextLine(Batch,line,sizeof(line)))break;
    	if(getCommand(line,cp))return 1;
        ret=doubleCommand(&value0,cp);
        ++(cp->n);
        ret=doubleCommand(&value1,cp);
        ++(cp->n);
        ind[0]=cp->value[cp->n];
        ind[1]=cp->value[cp->n+1];
        ind[2]=cp->value[cp->n+2];
       // fprintf(stderr," k %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld \n",(long int)value0,(long int)value1,n1,n2,n3,n4,n5,n6,n7,n8);
		for (int i=0; i<3; i++){
			for (int j=0; j<3; j++)
			{
				int n1 = ind[i];
				int n2 = ind[j];
				if (n2 > n1) add_edge(n1, n2, *G);
			}    
		}

  }
  	
  	return 0;
  
}
int doQuad(BatchPtr Batch,struct CommandInfo *cp)
{
    char line[4096];
    double value0,value1;
    long int nel,mat;
    int ret;
    
    cp->n++;
    ret=doubleCommand(&value0,cp);
    if(ret){
       fprintf(stderr,"doQuad Error \n");
    }
    
    nel=(long int )value0;
    
    cp->n++;
    ret=doubleCommand(&value0,cp);
    if(ret){
       fprintf(stderr,"doQuad Error \n");
    }
    
    mat=(long int )value0;
    
    fprintf(stderr,"doQuad nel %ld mat %ld\n",nel,mat);
  
  	for(long int k=0;k<mat;++k){
        if(BatchNextLine(Batch,line,sizeof(line)))break;
    	if(getCommand(line,cp))return 1;
  	}
  	
    for(long int k=0;k<nel;++k){
        size_t ind[4];
        if(BatchNextLine(Batch,line,sizeof(line)))break;
    	if(getCommand(line,cp))return 1;
        ret=doubleCommand(&value0,cp);
        ++(cp->n);
        ret=doubleCommand(&value1,cp);
        ++(cp->n);
        ind[0]=cp->value[cp->n];
        ind[1]=cp->value[cp->n+1];
        ind[2]=cp->value[cp->n+2];
        ind[3]=cp->value[cp->n+3];
       // fprintf(stderr," k %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld \n",(long int)value0,(long int)value1,n1,n2,n3,n4,n5,n6,n7,n8);
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++)
			{
				int n1 = ind[i];
				int n2 = ind[j];
				if (n2 > n1) add_edge(n1, n2, *G);
			}    
		}

  }
  	
  	return 0;
  
}
int doHex8(BatchPtr Batch,struct CommandInfo *cp)
{
    char line[4096];
    double value0,value1;
    long int nel,mat;
    int ret;
    
    cp->n++;
    ret=doubleCommand(&value0,cp);
    if(ret){
       fprintf(stderr,"doHex8 Error \n");
    }
    
    nel=(long int )value0;
    
    cp->n++;
    ret=doubleCommand(&value0,cp);
    if(ret){
       fprintf(stderr,"doHex8 Error \n");
    }
    
    mat=(long int )value0;
    
    fprintf(stderr,"doHex8 nel %ld mat %ld\n",nel,mat);
  
  	for(long int k=0;k<mat;++k){
        if(BatchNextLine(Batch,line,sizeof(line)))break;
    	if(getCommand(line,cp))return 1;
  	}
  
  
    for(long int k=0;k<nel;++k){
        size_t ind[8];
        if(BatchNextLine(Batch,line,sizeof(line)))break;
    	if(getCommand(line,cp))return 1;
        ret=doubleCommand(&value0,cp);
        ++(cp->n);
        ret=doubleCommand(&value1,cp);
        ++(cp->n);
        ind[0]=cp->value[cp->n];
        ind[1]=cp->value[cp->n+1];
        ind[2]=cp->value[cp->n+2];
        ind[3]=cp->value[cp->n+3];
        ind[4]=cp->value[cp->n+4];
        ind[5]=cp->value[cp->n+5];
        ind[6]=cp->value[cp->n+6];
        ind[7]=cp->value[cp->n+7];
       // fprintf(stderr," k %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld \n",(long int)value0,(long int)value1,n1,n2,n3,n4,n5,n6,n7,n8);

		for (int i=0; i<8; i++){
			for (int j=0; j<8; j++)
			{
				int n1 = ind[i];
				int n2 = ind[j];
				if (n2 > n1) add_edge(n1, n2, *G);
			}    
		}
  }
    
	return 0;
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
int doNodes(BatchPtr Batch,struct CommandInfo *cp)
{
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
    
    cp->n++;
    ret=doubleCommand(&value1,cp);
    if(ret){
       printf("doNodes Error\n");
    }
    nodes=(long)value1;
    
    
    G=new Graph(nodes);
    
    printf("doNodes nodes %ld\n",nodes);

    for(int k=0;k<nodes;++k){
        if(BatchNextLine(Batch,line,sizeof(line)))break;
        if(getCommand(line,cp))break;
        ret=doubleCommand(&value0,cp);
        ++(cp->n);
        ret=doubleCommand(&x,cp);
        ++(cp->n);
        ret=doubleCommand(&y,cp);
        ++(cp->n);
        ret=doubleCommand(&z,cp);
        ++(cp->n);
      //  printf("%g %g %g %g\n",value0,x,y,z);
      
/*
   		if(z == 0){
   		   int nn=value0;
   		   fprintf(stderr,"fix 7 %d\n",nn);
   		}  
   		
*/
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
    while(line[nw]){
	   if(line[nw] == '/'){
		    printf("Backslach found\n");
		}
       nw++;
    }
    
	nw=0;
	do{

		while((c = *line) != 0 && (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == ',')){
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
							 && c != ',' && n < 255 ){
				buff[n++]=c;
			}
		}
		
		if(c == ' ' || c == '"' || c == '\t' || c == ','){
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


	nc=0;
	
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

	while(1){
	    c=fgetc(input);
		if(c == '/'){
	    	c=fgetc(input);
			goto doWhite;
		}
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

