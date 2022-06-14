#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//cc -o ToTable ToTable.c
int main(int argc,char *argv[])
{
	FILE *inout;
	char *file=argv[1];
	int n,m,c,itWas;
	char buff[4096];
	char lab[4096],labold[4096];
	
	unsigned char *buffin,*bp;
	int blength=100000;
	
	buffin=(unsigned char *)malloc(blength);
	if(buffin == NULL){
		fprintf(stderr,"Memory Error blength %d\n",blength);
		exit(1);
	}
	
	
	if((inout=fopen(file,"rb")) == NULL){
		fprintf(stderr,"Cannot open file : %s to read%c\n",file,0);
		exit(1);
	}
	
	bp=buffin;
	
	int xsize=0;
	int ysize=0;
	while((m=(int)fread(buffin,1,blength,inout)) > 0){
		bp=(unsigned char *)buffin;
		for(n=0;n<m;++n){
			c = *bp++;
			if(((c == '\t') || (c == ',')) && (ysize == 0))++xsize;
			if(c == '\r')++ysize;
			if((c == '\n') && (itWas != '\r'))++ysize;
			itWas = c;			
		}
	}
	
	rewind(inout);
	++xsize;
	labold[0]=0;
	fprintf(stderr,"xsize %d ysize %d\n",xsize,ysize);
	int iprint=0;
	off_t where;
	for(int n=0;n<ysize;++n){
	    int fout=0;
	    int nn=0;
		while(1){
			c=fgetc(inout);
			//fprintf(stderr,"%c \n",c);
			if(c == EOF)goto OutOfHere;
			if(c == ','){
			   buff[nn++]=0;
			   float f=atof(buff);
			   if(fout == 1){
			        stpcpy(lab,buff);
			   		if(strcmp(lab,labold) == 0){
			   		    //fseeko(stdin,where,SEEK_SET);
			   		    //fprintf(stderr,"postion rset\n");
			   			//printf("\t");
			   		}else{
			   			if(n != 0){
			   			    printf("\n");
			   			    iprint=0;	
			   			}		   		
			   		}
			        stpcpy(labold,buff);
			   }
			   if(fout > 5){
			       if(iprint)printf("\t");
			       printf("%.02f",f);
			       iprint=1;
			   }
			   
			   
			   nn=0;
			   fout++;
			}else if(c == '\n' && (itWas != '\r')){
				if(nn > 2){
			   		buff[nn++]=0;
			   		float f=atof(buff);
			   		printf("%.02f",f);
			   		fout++;
				}
				//where=ftello(stdout);
				//printf("\n");
				break;
		
			}else if(c == '\r'){
				printf("CR Found\n");				
			}else{
			   buff[nn++]=c;
			}
			itWas = c;		
		}
		
	
	}
	
	printf("\n");

	
OutOfHere:
	
	
	if(buffin)free(buffin);
	
	if(inout)fclose(inout);
}