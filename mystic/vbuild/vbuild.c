#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/*
cc -o vbuild vbuild.c -ldf -ljpeg -lz
*/

typedef struct Icon {
      long ixmax;
      long iymax;
      long izmax;
      float xmin,xmax;
      float ymin,ymax;
      float zmin,zmax;
      float *data;
      char filename[256];
} *IconPtr;
      


struct Commands{
    char *name;
    int (*sub)(IconPtr myIcon,char command[][16],double *value,int *nword);
};

int doSphere(IconPtr myIcon,char command[][16],double *value,int *nword);

struct Commands clist[]=
    {
	"limits",doSphere,
	"name",doSphere,
	"sphere",doSphere,
	"torus",doSphere,
	"cone",doSphere,
	"disk",doSphere,
	"raw",doSphere,
	"ellipse",doSphere,
	"box",doSphere,
	"stop",doSphere,
    };

int NextLine(FILE *input,char *line,int linelength);
int closeSDS(IconPtr myIcon,int flag);
int getCommand(char *line,char command[][16],double *value,int *nword);
int doRenderKeys(char *line,IconPtr myIcon);
int zerol(char *p,unsigned long n);
int cFree(char *buffer);
char *cMalloc(unsigned long length);

int DFSDputdata(char *path,int rank,int *size,float *data); 
int DFSDlastref(void);
int DFANputdesc(char *file,unsigned int type,unsigned int tag,char *name,long lenght);

int DFerror;

int main(int argc,char **argv)
{
	struct Icon myIcon;
	char line[4096];
	FILE *input;

	zerol((char *)&myIcon,sizeof(struct Icon));

	input=fopen(argv[1],"r");
	if(input == NULL){
	    fprintf(stderr,"Could Not Open %s To Read\n",argv[1]);
	    exit(1);
	}

	while(1){
	    if(NextLine(input,line,sizeof(line)))break;
	    if(doRenderKeys(line,&myIcon))break;
	}


	return 0;
}
int NextLine(FILE *input,char *line,int linelength)
{
	int nc;
	int c;

	if(!input || !line || linelength <= 0)return 1;

	--linelength;

	nc=0;
	while(1){
	    c=fgetc(input);
	    if(c == '\n' || c == '\r' || c == EOF){
	        break;
	    }
	    if(nc >= linelength)break;
	    line[nc++] = c;
	}

	line[nc]=0;

	if(c == EOF && nc == 0){
	    return 1;
	}

	return 0;
}
int doRenderKeys(char *line,IconPtr myIcon)
{
	char command[256][16];
	double value[256];
	int k,n,nword,nList;

	if(!line || !myIcon)return 1;

	nList=sizeof(clist)/sizeof(struct Commands);

	getCommand(line,command,value,&nword);
	if(!nword)return 0;
	for(k=0;k<nword;++k){
		for(n=0;n<nList;++n){
				 if(!strcmp(clist[n].name,command[k])){
			(*(clist[n].sub))(myIcon,command,value,&k);
			break;
				 }
		}
		if(n >= nList){
				 printf("Unknown Command : %s\n",command[k]);
				 break;
		}
	}
	return 0;
}
int doSphere(IconPtr myIcon,char command[][16],double *value,int *nword)
{
	long ixmax,iymax,izmax;
	float xmin,ymin,zmin;
	float dx,dy,dz;
	float x,y,z;
	float *data;
	long i,j,k;

	if(!myIcon)return 1;

	ixmax=myIcon->ixmax;
	iymax=myIcon->iymax;
	izmax=myIcon->izmax;

	xmin=myIcon->xmin;
	ymin=myIcon->ymin;
	zmin=myIcon->zmin;

	if(ixmax){
	    dx=(myIcon->xmax-xmin)/(float)(ixmax);
	}else{
	    dx=0;
	}

	if(iymax){
	    dy=(myIcon->ymax-ymin)/(float)(iymax);
	}else{
	    dy=0;
	}


	if(izmax){
	    dz=(myIcon->zmax-zmin)/(float)(izmax);
	}else{
	    dz=0;
	}

	data=myIcon->data;

	if(!strcmp("sphere",command[*nword])){
	    float xc,yc,zc,r,r2,density,radius2;
	    ++(*nword);
	    xc=value[*nword];
	    ++(*nword);
	    yc=value[*nword];
	    ++(*nword);
	    zc=value[*nword];
	    ++(*nword);
	    r=value[*nword];
	    r2=r*r;
	    ++(*nword);
	    density=value[*nword];

	    printf("Sphere xc %f yc %f zc %f r %f density %f\n",xc,yc,zc,r,density);

	    for(k=0;k<izmax;++k){
	        z=zmin+((double)k+.5)*dz;
	        for(j=0;j<iymax;++j){
	            y=ymin+((double)j+.5)*dy;
	            for(i=0;i<ixmax;++i){
	                x=xmin+((double)i+.5)*dx;
	                radius2=(x-xc)*(x-xc)+(y-yc)*(y-yc)+(z-zc)*(z-zc);
	                if(radius2 <= r2){
	                    data[i+j*ixmax+k*ixmax*iymax]=density;
	                }
	            }
	        }
	    }
	}else if(!strcmp("box",command[*nword])){
	    float xc1,yc1,zc1,xc2,yc2,zc2,r,density,ra,rb;
	    ++(*nword);
	    xc1=value[*nword];
	    ++(*nword);
	    yc1=value[*nword];
	    ++(*nword);
	    zc1=value[*nword];
	    ++(*nword);
	    xc2=value[*nword];
	    ++(*nword);
	    yc2=value[*nword];
	    ++(*nword);
	    zc2=value[*nword];
	    ++(*nword);
	    density=value[*nword];

	    printf("box xc1 %f yc1 %f zc1 %f \n        xc2 %f yc2 %f zc2 %f\n            density %f\n",
	           xc1,yc1,zc1,xc2,yc2,zc2,density);

	    for(k=0;k<izmax;++k){
	        z=zmin+((double)k+.5)*dz;
	        for(j=0;j<iymax;++j){
	            y=ymin+((double)j+.5)*dy;
	            for(i=0;i<ixmax;++i){
	                x=xmin+((double)i+.5)*dx;
	                if(x >= xc1 && x <= xc2 && y >= yc1 && y <= yc2 && z >= zc1 && z <= zc2){
	                    data[i+j*ixmax+k*ixmax*iymax]=density;
	                }
	            }
	        }
	    }
	}else if(!strcmp("ellipse",command[*nword])){
	    float xc1,yc1,zc1,xc2,yc2,zc2,r,density,ra,rb;
	    ++(*nword);
	    xc1=value[*nword];
	    ++(*nword);
	    yc1=value[*nword];
	    ++(*nword);
	    zc1=value[*nword];
	    ++(*nword);
	    xc2=value[*nword];
	    ++(*nword);
	    yc2=value[*nword];
	    ++(*nword);
	    zc2=value[*nword];
	    ++(*nword);
	    r=value[*nword];
	    ++(*nword);
	    density=value[*nword];

	    printf("ellipse xc1 %f yc1 %f zc1 %f \n        xc2 %f yc2 %f zc2 %f\n         r %f density %f\n",
	           xc1,yc1,zc1,xc2,yc2,zc2,r,density);

	    for(k=0;k<izmax;++k){
	        z=zmin+((double)k+.5)*dz;
	        for(j=0;j<iymax;++j){
	            y=ymin+((double)j+.5)*dy;
	            for(i=0;i<ixmax;++i){
	                x=xmin+((double)i+.5)*dx;
	                ra=sqrt((x-xc1)*(x-xc1)+(y-yc1)*(y-yc1)+(z-zc1)*(z-zc1));
	                rb=sqrt((x-xc2)*(x-xc2)+(y-yc2)*(y-yc2)+(z-zc2)*(z-zc2));
	                if((ra+rb) <= r){
	                    data[i+j*ixmax+k*ixmax*iymax]=density;
	                }
	            }
	        }
	    }
	}else if(!strcmp("cone",command[*nword])){
	    float xc1,yc1,zc1,xc2,yc2,zc2,r1,r2,den,ra2;
	    float cx,cy,cz,nx,ny,nz,vx,vy,vz,dr,vr,dot;
	    float density,s;
	    ++(*nword);
	    xc1=value[*nword];
	    ++(*nword);
	    yc1=value[*nword];
	    ++(*nword);
	    zc1=value[*nword];
	    ++(*nword);
	    xc2=value[*nword];
	    ++(*nword);
	    yc2=value[*nword];
	    ++(*nword);
	    zc2=value[*nword];
	    ++(*nword);
	    r1=value[*nword];
	    ++(*nword);
	    r2=value[*nword];
	    ++(*nword);
	    density=value[*nword];

	    cx=xc2-xc1;
	    cy=yc2-yc1;
	    cz=zc2-zc1;
	    dr=r2-r1;

	    printf("cone    xc1 %f yc1 %f zc1 %f \n        xc2 %f yc2 %f zc2 %f\n         r1 %f r2 %f density %f\n",
	           xc1,yc1,zc1,xc2,yc2,zc2,r1,r2,density);

	    den = sqrt(cx*cx+cy*cy+cz*cz);

	    if(den <= 0){
	        fprintf(stderr,"Error cone end points must be different");
	        return 1;
	    }

	    nx=cx/den;
	    ny=cy/den;
	    nz=cz/den;

	    for(k=0;k<izmax;++k){
	        z=zmin+((double)k+.5)*dz;
	        for(j=0;j<iymax;++j){
	            y=ymin+((double)j+.5)*dy;
	            for(i=0;i<ixmax;++i){
	                x=xmin+((double)i+.5)*dx;
					dot=nx*(x-xc1)+ny*(y-yc1)+nz*(z-zc1);
					if(dot < 0)continue;
					s=dot/den;
					if(s > 1.0)continue;
					vx=s*cx+xc1;
					vy=s*cy+yc1;
					vz=s*cz+zc1;
					vr=r1+s*dr;
					ra2=(x-vx)*(x-vx)+(y-vy)*(y-vy)+(z-vz)*(z-vz);
	                if((ra2) <= (vr*vr)){
	                    data[i+j*ixmax+k*ixmax*iymax]=density;
	                }
	            }
	        }
	    }
	}else if(!strcmp("torus",command[*nword])){
	    float xc,yc,zc,r1,r2,r22,den;
	    float ax,ay,az,nx,ny,nz,dot,dotn,dotx;
	    float density;
	    ++(*nword);
	    xc=value[*nword];
	    ++(*nword);
	    yc=value[*nword];
	    ++(*nword);
	    zc=value[*nword];
	    ++(*nword);
	    nx=value[*nword];
	    ++(*nword);
	    ny=value[*nword];
	    ++(*nword);
	    nz=value[*nword];
	    ++(*nword);
	    r1=value[*nword];
	    ++(*nword);
	    r2=value[*nword];
	    ++(*nword);
	    density=value[*nword];

	    r22=r2*r2;


	    printf("torus   xc %f yc %f zc %f \n        nx %f ny %f nz %f\n         rmajor %f rminor %f density %f\n",
	           xc,yc,zc,nx,ny,nz,r1,r2,density);

	    den = sqrt(nx*nx+ny*ny+nz*nz);
	    if(den <= 0){
	        fprintf(stderr,"torus has zero normal\n");
	        return 1;
	    }

	    nx=nx/den;
	    ny=ny/den;
	    nz=nz/den;

	    for(k=0;k<izmax;++k){
	        z=zmin+((double)k+.5)*dz;
	        for(j=0;j<iymax;++j){
	            y=ymin+((double)j+.5)*dy;
	            for(i=0;i<ixmax;++i){
	                x=xmin+((double)i+.5)*dx;
	                ax=(x-xc);
	                ay=(y-yc);
	                az=(z-zc);

					dotn=nx*ax+ny*ay+nz*az;
		
					ax -= dotn*nx;
					ay -= dotn*ny;
					az -= dotn*nz;
		
					dotx=sqrt(ax*ax+ay*ay+az*az);
		
					dotx=r1-dotx;
	                if((dotn*dotn+dotx*dotx) <= r22){
	                    data[i+j*ixmax+k*ixmax*iymax]=density;
	                }
	            }
	        }
	    }
	}else if(!strcmp("disk",command[*nword])){
	    ;
	}else if(!strcmp("raw",command[*nword])){
	    unsigned char bout;
	    char *name;
	    FILE *out;
	    name="raw.raw";
	    out=fopen(name,"wb");
	    if(!out)
	    {
	    	printf("Error opening %s to write\n",name);
	    	return 0;
	    }
	    for(k=0;k<izmax;++k){
	        for(j=0;j<iymax;++j){
	            for(i=0;i<ixmax;++i){
	            	bout=data[i+j*ixmax+k*ixmax*iymax];
	            	fwrite(&bout,1,1,out);
	            }
	        }
	    }
	    printf("Write %s %ld %ld %ld\n",name,ixmax,iymax,izmax);
	}else if(!strcmp("limits",command[*nword])){
	    ++(*nword);
	    myIcon->xmin=value[*nword];
	    ++(*nword);
	    myIcon->xmax=value[*nword];
	    ++(*nword);
	    myIcon->ymin=value[*nword];
	    ++(*nword);
	    myIcon->ymax=value[*nword];
	    ++(*nword);
	    myIcon->zmin=value[*nword];
	    ++(*nword);
	    myIcon->zmax=value[*nword];
	    printf("limits xmin %f ymin %f zmin %f\n",
	            myIcon->xmin,myIcon->ymin,myIcon->zmin);
	    printf("       xmax %f ymax %f zmax %f\n",
	            myIcon->xmax,myIcon->ymax,myIcon->zmax);
	}else if(!strcmp("name",command[*nword])){
	    long length;
	    if(myIcon->data){
	        closeSDS(myIcon,0);
	    }
	    ++(*nword);
	    strncpy(myIcon->filename,command[*nword],256);
	    ++(*nword);
	    myIcon->ixmax=(long)value[*nword];
	    ++(*nword);
	    myIcon->iymax=(long)value[*nword];
	    ++(*nword);
	    myIcon->izmax=(long)value[*nword];

	    myIcon->xmin=0;
	    myIcon->ymin=0;
	    myIcon->zmin=0;

	    myIcon->xmax=(float)myIcon->ixmax;
	    myIcon->ymax=(float)myIcon->iymax;
	    myIcon->zmax=(float)myIcon->izmax;

	    printf("filename %s ixmax %ld iymax %ld izmax %ld\n",
	            myIcon->filename,myIcon->ixmax,myIcon->iymax,myIcon->izmax);
	    length=myIcon->ixmax*myIcon->iymax*myIcon->izmax;
	    myIcon->data=(float *)cMalloc(length*sizeof(float));
	    if(!myIcon->data){
	        fprintf(stderr,"file %s length %ld Out of Memory\n",
	                myIcon->filename,length*sizeof(float));
	        return 1;
	    }
	    zerol((char *)myIcon->data,length*sizeof(float));
	}else if(!strcmp("stop",command[*nword])){
	    closeSDS(myIcon,1);
	}
	return 0;
}
int closeSDS(IconPtr myIcon,int flag)
{
	extern int DFerror;
	char outbuff[2048];
	int lastref;
	int rank,size[3];
	long length;
	int ret;

	rank=3;
	size[0]=(int)myIcon->ixmax;
	size[1]=(int)myIcon->iymax;
	size[2]=(int)myIcon->izmax;
	length=size[0]*size[1]*size[2];
	if(length <= 0 || !myIcon->data){
            if(flag)
              fprintf(stderr,"DFSDputdata error File: %s length %ld data %lx\n",
                     myIcon->filename,length,(unsigned long)myIcon->data);
	    return 1;
	}

	if(ret=DFSDputdata(myIcon->filename,rank,size,myIcon->data)){
            fprintf(stderr,"DFSDputdata error File: %s ret %ld\n",myIcon->filename,(long)ret);
            return 1;
	}

	sprintf(outbuff,"xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g%c",
	       myIcon->xmin,myIcon->xmax,myIcon->ymin,myIcon->ymax,myIcon->zmin,myIcon->zmax,0);

	lastref=DFSDlastref();

	if(lastref < 0){
	    fprintf(stderr,"DFSDlastref %s lastref %d error %d",
		               myIcon->filename,lastref,DFerror);
	    return 1;
	}

	if(DFANputdesc(myIcon->filename,306,lastref,outbuff,(int)strlen(outbuff)) == -1){
		   fprintf(stderr,"DFANputdesc %s lastref %d error %d",
		               myIcon->filename,lastref,DFerror);
			return 1;
	}

	cFree((char *)myIcon->data);
	myIcon->data=NULL;
	return 0;
}
int getCommand(char *line,char command[][16],double *value,int *nword)
{
	static char number[]={
			'0','1','2','3','4','5','6','7','8','9',
			'+','-','.'};
	char buff[256];
	int c,n,nw,iret,k,inum;

	for(n=0;n<256;++n){
	    value[n]=0;
	    strcpy(command[n],"");
	}

	nw=0;
do{
	n=0;
	while((c = *line++) != 0 && c != ' ' && c != '\n' && c != '\r'
				     && c != EOF  && n < 15 ){
	    buff[n++]=c;
	}
	if(c == EOF){
	    strcpy(command[0],"stop");
	    *nword = 1;
	    return 1;	    
	}else if(c == ' '){
	    iret = 0;
	}else{
	    iret = 1;
	}

	buff[n]=0;

	if(!strcmp(buff,".") || !strcmp(buff,"..")){
	    strcpy(command[nw],buff);
	    continue;
	}

	inum = 0;
	for(k=0;k<sizeof(number);++k){
	    if(*buff == number[k]){
	        inum = 1;
	        break;
	    }
	}

	if(inum){
	     value[nw] = atof(buff);
	}else{
	    strcpy(command[nw],buff);
	}

    }while((++nw < 256) &&  !iret);

    *nword = nw;

    return 0;
}
int zerol(char *p,unsigned long n)
{
	if(!p)return 1;
	while(n-- > 0)*p++ = 0;
	return 0;
}
char *cMalloc(unsigned long length)
{
	char *buffer;

	buffer=malloc(length);

	return buffer;
}
int cFree(char *buffer)
{
    if(!buffer)return 1;
    free(buffer);
    return 0;
}
