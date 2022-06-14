#define EXTERN22
#include <silo.h>
#include "Xappl.h"
#include "BatchFile.h"
#include "Universal.h"
#include "uLib.h"
#include "VoiceCommand.h"
#include <string.h>
#ifdef MEMEORY_INFO
#include <malloc.h>
#endif

#include <sys/time.h>

#include "FileManager.h"

int FilesSetFrameSDS3D(long CurrentFrame,struct FileInfo2 *Files);

int FilesSetFrameSDS2D(long CurrentFrame,struct FileInfo2 *Files);

static int doSDS3D(struct FileInfo2 *Files,char *name);

static int doSDS2D(struct FileInfo2 *Files,char *name);

static int doPIO2D(struct FileInfo2 *Files,char *name);

static int doPIO3D(struct FileInfo2 *Files,char *name);

int SetPIOData2D(struct FileInfo2 *Files,long CurrentFrame);

int doSage2DStart(struct FileInfo2 *Files);

int doSage3DStart(struct FileInfo2 *Files);

int GetPIOData3D(struct FileInfo2 *Files,long CurrentFrame);

int loadVectors2d(struct FileInfo2 *Files);

int loadVectors3d(struct FileInfo2 *Files);

static int doMesh2d(DBfile *dbfile,struct FilePIOInfo *pio,char *name,DBoptlist *optlist,struct FileInfo2 *Files);

static int debug=0;

static int cycleFromName=0;

static char *dir=NULL;

static int flip=1;

static char working[2048];

#define MAXPLOT 4000
static char *nameToPlot[MAXPLOT];
int nameToPlotCount;

int showToVisIt(int argc,char **argv)
{
	struct FileInfo2 *Files2;
	struct FileList Files;
	char name[256],*sp;
	char nameo[256];
	int ret;
	int n;
		
	getcwd(working,2048);
	
	nameToPlotCount=0;
	
	uPref.flipSDS2dY=FALSE;
	
	ret=1;
	
	for(n=2;n<argc;++n){
	    long FileMax;
	    
		if(debug > 0){
			printf("%d %s\n",n,argv[n]);
		}
		
        if(!mstrcmp(argv[n],"-dir")){
            dir=argv[++n];
            if(debug > 0){
            	printf("output directory : %s\n",dir);
            }
            continue;
        }else if(!mstrcmp(argv[n],"-debug")){
            debug=0;
            if(!mstrcmp(argv[++n],"on")){
             	debug=1;
           	}
            if(debug){
            	printf("debug : %d\n",debug);
            }
            continue;
        }else if(!mstrcmp(argv[n],"-cycleFromName")){
            cycleFromName=0;
            if(!mstrcmp(argv[++n],"on")){
             	cycleFromName=1;
           	}
            if(debug){
            	printf("cycleFromName: %d\n",cycleFromName);
            }
            continue;
        }else if(!mstrcmp(argv[n],"-flip")){
            flip=0;
            if(!mstrcmp(argv[++n],"on")){
             	flip=1;
           	}
            if(debug){
            	printf("flip: %d\n",flip);
            }
            continue;
        }else if(!mstrcmp(argv[n],"-flipY3d")){
           uPref.flipSDS2dY=FALSE;
            if(!mstrcmp(argv[++n],"on")){
             	uPref.flipSDS2dY=TRUE;
           	}
            if(debug){
            	printf("flip: %d\n",uPref.flipSDS2dY);
            }
            continue;
        }else if(!mstrcmp(argv[n],"-plot")){
          	
           	nameToPlot[nameToPlotCount]=strsave(argv[++n],9876);
           	
            if(debug){
            	printf("plot: %s\n",nameToPlot[nameToPlotCount]);
            }
            
            if(nameToPlotCount < MAXPLOT)
           	{
           		nameToPlotCount++;
           	}
          	continue;
        }
		
		strncpy(nameo,argv[n],255);
		strncpy(name,argv[n],255);
		sp=strrchr(name,'/');
		if(sp != NULL){
			strncpy(name,&sp[1],255);
		}
		
		sp=name+strlen(name);
		sp[0]='.';
		sp[1]='s';
		sp[2]='i';
		sp[3]='l';
		sp[4]='o';
		sp[5]=0;
	
		
		goCD(working);
	
    	zerol((char *)&Files,sizeof(struct FileList));
    	Files.donotfree=TRUE;  
    	Files.directory=DefaultPathString();
	    Files.count=0;
		FileMax = 2;
	    Files.name=(char **)cMalloc(FileMax*sizeof(char *),8750);
	    if(!Files.name)goto ErrorOut1;
 		Files.name[Files.count]=strsave(argv[n],7776);
	    if(!Files.name[Files.count])goto ErrorOut1;
 	    Files.count=1;
		Files2=FilesOpenList(&Files);
		if(!Files2){
ErrorOut1:
		    fprintf(stderr,"Error Setting Up to Read %s \n",argv[n]);
		    goto ErrorOut;
		}
		if(debug > 0){
			printf("%d %s - opened fileType %d fileClass %d SDS3D_DISK %d SDS3D_FILES %d\n",n,argv[n],Files2->fileType,Files2->fileClass,SDS3D_DISK,SDS3D_FILES);
		}
		
		if(Files2->fileClass == SDS3D_FILES){
			doSDS3D(Files2,name);
		}else if(Files2->fileClass == SDS2D_FILES){
			doSDS2D(Files2,name);
		}else if(Files2->fileClass == PIO2D_FILES){
			doPIO2D(Files2,name);
		}else if(Files2->fileClass == PIO3D_FILES){
			doPIO3D(Files2,name);
		}else{
		    printf("File '%s' skipped - fileClass %d tag %s\n",nameo,Files2->fileClass,FilesClassTag(Files2));
		}
		
		freeFileList(Files2);
		
	}
	ret=0;
	
ErrorOut:
	if(ret)printf("showToVisIt Error Exit\n");
	return 0;

}
static int doMesh3d(DBfile *dbfile,struct FilePIOInfo *pio,char *name,DBoptlist *optlist,struct FileInfo2 *Files)
{
	int shapesize[] = {8};
	int shapecounts[] = {1};
	int shapetype[] = {DB_ZONETYPE_HEX};
	double *coords[3];
	int lnodelist;
	int *nodelist;
	double *x,*y,*z,*value;
	long numcell;
	int nshapetypes = 1;
	int nnodes;
	int nzones;
	int ndims = 3;
	int nn,ne;
	int n;

	
	if(!pio || !name || !Files)return 1;
	
	x=NULL;
	y=NULL;
	z=NULL;
	value=NULL;
	nodelist=NULL;
	
	numcell=0;
	
	for(n=0;n<pio->numcell;++n){
		if(pio->daughter[n])continue;
		numcell++;
	}
	        	
   	
   	x=(double *)cMalloc(numcell*8*sizeof(double),8753);
   	y=(double *)cMalloc(numcell*8*sizeof(double),8754);
   	z=(double *)cMalloc(numcell*8*sizeof(double),8754);
   	nodelist=(int *)cMalloc(numcell*8*sizeof(int),8755);
    value=(double *)cMalloc(numcell*sizeof(double),8754);
  	
   	if(x == NULL || y == NULL || z == NULL || nodelist == NULL || value == NULL)goto ErrorOut;
   	
   	coords[0]=x;
   	
   	coords[1]=y;
   	
   	coords[2]=z;
   	
	lnodelist=numcell*8;
	
	nnodes = numcell*8;
	
	nzones = numcell;
	
	shapecounts[0]=numcell;
	
   	
 	if(debug > 0){
   		fprintf(stderr,"File : %s %g %g %g %g ",name,pio->xmin,pio->xmax,pio->ymin,pio->ymax);
   		fprintf(stderr,"active %p numcell %ld numcell %ld\n",pio->active,pio->numcell,numcell);
 	}
 	
 	ne=0;
	nn=0;
	for(n=0;n<pio->numcell;++n){
		double dx,dy,dz;
	    int k;
		if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    value[ne++]=pio->value[n];
	    dx=pio->dx2[k];
	    dy=pio->dy2[k];
	    dz=pio->dz2[k];
	    
		x[nn]=pio->xcenter[n]-dx;
		y[nn]=pio->ycenter[n]-dy;
		z[nn]=pio->zcenter[n]-dz;
		nodelist[nn]=nn;
		++nn;
		x[nn]=pio->xcenter[n]+dx;
		y[nn]=pio->ycenter[n]-dy;
		z[nn]=pio->zcenter[n]-dz;
		nodelist[nn]=nn;
		++nn;
		x[nn]=pio->xcenter[n]+dx;
		y[nn]=pio->ycenter[n]+dy;
		z[nn]=pio->zcenter[n]-dz;
		nodelist[nn]=nn;
		++nn;
		x[nn]=pio->xcenter[n]-dx;
		y[nn]=pio->ycenter[n]+dy;
		z[nn]=pio->zcenter[n]-dz;
		nodelist[nn]=nn;
		++nn;
	    
		x[nn]=pio->xcenter[n]-dx;
		y[nn]=pio->ycenter[n]-dy;
		z[nn]=pio->zcenter[n]+dz;
		nodelist[nn]=nn;
		++nn;
		x[nn]=pio->xcenter[n]+dx;
		y[nn]=pio->ycenter[n]-dy;
		z[nn]=pio->zcenter[n]+dz;
		nodelist[nn]=nn;
		++nn;
		x[nn]=pio->xcenter[n]+dx;
		y[nn]=pio->ycenter[n]+dy;
		z[nn]=pio->zcenter[n]+dz;
		nodelist[nn]=nn;
		++nn;
		x[nn]=pio->xcenter[n]-dx;
		y[nn]=pio->ycenter[n]+dy;
		z[nn]=pio->zcenter[n]+dz;
		nodelist[nn]=nn;
		++nn;
	}
	
 /*
 	DBPutZonelist(dbfile, "zonelist", nzones, ndims, nodelist, lnodelist,0, shapesize,
 	              shapecounts, nshapetypes);	
 */
 	              
 	DBPutZonelist2(dbfile, "zonelist", nzones, ndims, nodelist, lnodelist,0,0,0,
 	               shapetype,shapesize,shapecounts, nshapetypes,NULL);	
 	              
 	DBPutUcdmesh(dbfile, "mesh", ndims, NULL, coords, nnodes, nzones,
   "zonelist", NULL, DB_DOUBLE, optlist);            
      
   
   if(nameToPlotCount > 0){  
       char vname[256];
       int np;
       
       for(np=0;np<nameToPlotCount;++np){
           char *name;
           
        name=nameToPlot[np];
        
        if(!mstrcmp(name,"velocity")){
            char *varnames[3];
            double *vars[3];
            
			loadVectors3d(Files);
			ne=0;
			for(n=0;n<pio->numcell;++n){
				if(pio->daughter[n])continue;
				x[ne]=pio->vectorx[n];
				y[ne]=pio->vectory[n];
				z[ne++]=pio->vectorz[n];
			}
			vars[0]=x;
			vars[1]=y;
			vars[2]=y;
			varnames[0]=pio->vector.vectorxName;
			varnames[1]=pio->vector.vectoryName;
			varnames[2]=pio->vector.vectorxName;
			
				DBPutUcdvar(dbfile, "velocity", "mesh",
							3, varnames,vars,
							ne, NULL, 0,
							DB_DOUBLE,DB_NODECENT,
							NULL);
       		continue;
        }
        
        pio->pioIndex=0;
        
        mstrncpy(pio->pioName,name,sizeof(pio->pioName));
        
		if(GetPIOData3D(Files,0L))goto ErrorOut;
		
		
		
		
 	              
		ne=0;
		for(n=0;n<pio->numcell;++n){
			if(pio->daughter[n])continue;
			value[ne++]=pio->value[n];
		}
		
		
		if(isalpha(*name)){
        	mstrncpy(vname,name,sizeof(vname));
		}else{
		    vname[0]='_';
        	mstrncpy(&vname[1],name,sizeof(vname)-1);
		}
		
		ne=1;
		while(vname[ne]){
			if(!isalnum(vname[ne])){
			    vname[ne]='_';
			}
			++ne;
		}
		
		/* fprintf(stderr,"vname = %s\n",vname); */
	
		DBPutUcdvar1(dbfile, vname, "mesh", value, nzones, NULL, 0,
      		DB_DOUBLE, DB_ZONECENT, NULL);
      	}
 	}
ErrorOut:
	if(x)cFree((char *)x);
	if(y)cFree((char *)y);
	if(z)cFree((char *)z);
	if(value)cFree((char *)value);
	if(nodelist)cFree((char *)nodelist);
	return 0;
}

static int doPIO3D(struct FileInfo2 *Files,char *name)
{
    struct FilePIOInfo *pio;
	DBfile *dbfile;
	double dtime;
	int ret;
	
	if(!Files)return 1;
	
	pio = &Files->pioData;
	
	ret=1;
	
	goCD(working);
	   		
    if(doSage3DStart(Files))goto ErrorOut;
    
	if(GetPIOData3D(Files,0L))goto ErrorOut;
	
	if(dir != NULL)
	{
		goCD(dir);
	}
	
    dbfile = NULL;
    
    dbfile = DBCreate(name, DB_CLOBBER, DB_LOCAL,"3D ARM mesh", DB_HDF5);
    if(dbfile == NULL)
    {
        fprintf(stderr, "Could not create Silo file : %s \n",name);
   		goto ErrorOut;    
    }

	DBoptlist *optlist = DBMakeOptlist(2);
	
	dtime =pio->sage->time;
	
	DBAddOption(optlist, DBOPT_DTIME, &dtime);
	
	if(cycleFromName > 0){
		char name2[256];
	    char *sp;
	    int ncycle;
		strncpy(name2,name,255);
		sp=strrchr(name2,'.');
		if(sp != NULL){
			*sp=0;
			sp -= 1;
			while(isdigit(*sp)){
			    if(sp > name2){
			    	--sp;
			    }else{
			        break;
			    }
			}
			++sp;
			ncycle=atoi(sp);
			if(debug){
			    printf("cycle %d\n",ncycle);
			}
			DBAddOption(optlist, DBOPT_CYCLE, &ncycle);
		}
	}
	
   	doMesh3d(dbfile,pio,name,optlist,Files);
   		
   DBFreeOptlist(optlist);

    DBClose(dbfile);
    
    ret=0;
    
ErrorOut:
	if(ret == 1){
   		fprintf(stderr,"Error on File : %s \n",name);
	}
	return ret;
}
static int doMesh2d(DBfile *dbfile,struct FilePIOInfo *pio,char *name,DBoptlist *optlist,struct FileInfo2 *Files)
{
	int shapesize[] = {4};
	int shapecounts[] = {1};
	int shapetype[] = {DB_ZONETYPE_QUAD};
	double *coords[2];
	int lnodelist;
	int *nodelist;
	double *x,*y,*value;
	long numcell;
	int nshapetypes = 1;
	int nnodes;
	int nzones;
	int ndims = 2;
	int nn,ne;
	int n;

	
	if(!pio || !name || !Files)return 1;
	
	x=NULL;
	y=NULL;
	value=NULL;
	nodelist=NULL;
	
	numcell=0;
	
	for(n=0;n<pio->numcell;++n){
		if(pio->daughter[n])continue;
		numcell++;
	}
	        	
   	
   	x=(double *)cMalloc(numcell*4*sizeof(double),8753);
   	y=(double *)cMalloc(numcell*4*sizeof(double),8754);
   	nodelist=(int *)cMalloc(numcell*4*sizeof(int),8755);
    value=(double *)cMalloc(numcell*sizeof(double),8754);
  	
   	if(x == NULL || y == NULL || nodelist == NULL || value == NULL)goto ErrorOut;
   	
   	coords[0]=x;
   	
   	coords[1]=y;
   	
	lnodelist=numcell*4;
	
	nnodes = numcell*4;
	
	nzones = numcell;
	
	shapecounts[0]=numcell;
	
   	
 	if(debug > 0){
   		fprintf(stderr,"File : %s %g %g %g %g ",name,pio->xmin,pio->xmax,pio->ymin,pio->ymax);
   		fprintf(stderr,"active %p numcell %ld numcell %ld\n",pio->active,pio->numcell,numcell);
 	}
 	
 	ne=0;
	nn=0;
	for(n=0;n<pio->numcell;++n){
		double dx,dy;
	    int k;
		if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    value[ne++]=pio->value[n];
	    dx=pio->dx2[k];
	    dy=pio->dy2[k];
	    
		x[nn]=pio->xcenter[n]-dx;
		y[nn]=pio->ycenter[n]-dy;
		nodelist[nn]=nn;
		++nn;
		x[nn]=pio->xcenter[n]+dx;
		y[nn]=pio->ycenter[n]-dy;
		nodelist[nn]=nn;
		++nn;
		x[nn]=pio->xcenter[n]+dx;
		y[nn]=pio->ycenter[n]+dy;
		nodelist[nn]=nn;
		++nn;
		x[nn]=pio->xcenter[n]-dx;
		y[nn]=pio->ycenter[n]+dy;
		nodelist[nn]=nn;
		++nn;
	}
	
 	              
 	DBPutZonelist2(dbfile, "zonelist", nzones, ndims, nodelist, lnodelist,0,0,0,
 	               shapetype,shapesize,shapecounts, nshapetypes,NULL);	
 	              
 	DBPutUcdmesh(dbfile, "mesh", ndims, NULL, coords, nnodes, nzones,
   "zonelist", NULL, DB_DOUBLE, optlist);            
      
   
   if(nameToPlotCount > 0){  
       char vname[256];
       int np;
       
       for(np=0;np<nameToPlotCount;++np){
           char *name;
           
        name=nameToPlot[np];
        
        if(!mstrcmp(name,"velocity")){
            char *varnames[2];
            double *vars[2];
            
			loadVectors2d(Files);
			ne=0;
			for(n=0;n<pio->numcell;++n){
				if(pio->daughter[n])continue;
				x[ne]=pio->vectorx[n];
				y[ne++]=pio->vectory[n];
			}
			vars[0]=x;
			vars[1]=y;
			varnames[0]=pio->vector.vectorxName;
			varnames[1]=pio->vector.vectoryName;
			
				DBPutUcdvar(dbfile, "velocity", "mesh",
							2, varnames,vars,
							ne, NULL, 0,
							DB_DOUBLE,DB_ZONECENT,
							NULL);
       		continue;
        }
        
        pio->pioIndex=0;
        
        mstrncpy(pio->pioName,name,sizeof(pio->pioName));
        
		if(SetPIOData2D(Files,0L))goto ErrorOut;
 	              
		ne=0;
		for(n=0;n<pio->numcell;++n){
			if(pio->daughter[n])continue;
			value[ne++]=pio->value[n];
		}
		
		
		if(isalpha(*name)){
        	mstrncpy(vname,name,sizeof(vname));
		}else{
		    vname[0]='_';
        	mstrncpy(&vname[1],name,sizeof(vname)-1);
		}
		
		ne=1;
		while(vname[ne]){
			if(!isalnum(vname[ne])){
			    vname[ne]='_';
			}
			++ne;
		}
		
		/* fprintf(stderr,"vname = %s\n",vname); */
	
		DBPutUcdvar1(dbfile, vname, "mesh", value, nzones, NULL, 0,
      		DB_DOUBLE, DB_ZONECENT, NULL);
      	}
 	}
ErrorOut:
	if(x)cFree((char *)x);
	if(y)cFree((char *)y);
	if(value)cFree((char *)value);
	if(nodelist)cFree((char *)nodelist);
	return 0;
}


/*
	double xmin,xmax,ymin,ymax;
		double xx,yy;
	
 	xmin = 1e66;
 	ymin = 1e66;
 	xmax = -1e66;
 	ymax = -1e66;
 	

		xx=pio->xcenter[n]+dx;
		if(xx > xmax)xmax=xx;
		if(xx < xmin)xmin=xx;
		xx=pio->xcenter[n]-dx;
		if(xx > xmax)xmax=xx;
		if(xx < xmin)xmin=xx;
		yy=pio->ycenter[n]+dy;
		if(yy > ymax)ymax=yy;
		if(yy < ymin)ymin=yy;
		yy=pio->ycenter[n]-dy;
		if(yy > ymax)ymax=yy;
		if(yy < ymin)ymin=yy;
    fprintf(stderr,"File : %s xmin %g xmax %g ymin %g ymax %g nn %d\n",name,xmin,xmax,ymin,ymax,nn);
	
*/


static int doPIO2D(struct FileInfo2 *Files,char *name)
{
    struct FilePIOInfo *pio;
	DBfile *dbfile;
	double dtime;
	int ret;
	
	if(!Files)return 1;
	
	pio = &Files->pioData;
	
	ret=1;
	
	goCD(working);
	   		
    if(doSage2DStart(Files))goto ErrorOut;
    
	if(SetPIOData2D(Files,0L))goto ErrorOut;
	
	if(dir != NULL)
	{
		goCD(dir);
	}
	
    dbfile = NULL;
    
    dbfile = DBCreate(name, DB_CLOBBER, DB_LOCAL,"2D ARM mesh", DB_HDF5);
    if(dbfile == NULL)
    {
        fprintf(stderr, "Could not create Silo file : %s \n",name);
   		goto ErrorOut;    
    }


	DBoptlist *optlist = DBMakeOptlist(2);
	
	dtime =pio->sage->time;
	
	DBAddOption(optlist, DBOPT_DTIME, &dtime);
	
	if(cycleFromName > 0){
		char name2[256];
	    char *sp;
	    int ncycle;
		strncpy(name2,name,255);
		sp=strrchr(name2,'.');
		if(sp != NULL){
			*sp=0;
			sp -= 1;
			while(isdigit(*sp)){
			    if(sp > name2){
			    	--sp;
			    }else{
			        break;
			    }
			}
			++sp;
			ncycle=atoi(sp);
			if(debug){
			    printf("cycle %d\n",ncycle);
			}
			DBAddOption(optlist, DBOPT_CYCLE, &ncycle);
		}
	}
	
   	doMesh2d(dbfile,pio,name,optlist,Files);
   		
   DBFreeOptlist(optlist);

    DBClose(dbfile);
    
    ret=0;
    
ErrorOut:
	if(ret == 1){
   		fprintf(stderr,"Error on File : %s \n",name);
	}
	return ret;
}
static int doSDS2D(struct FileInfo2 *Files,char *name)
{
	struct SetFrameData *s;
	char *pioName;
	DBfile *dbfile;
	double dtime;
	double *data,*data2;
	double *x,*y;
    double *coords[3];
    double dv;
	int dims[3];
	int ndims=2;
	int ret;
	int n;
	
	
	if(!Files)return 1;
	
	x=NULL;
	y=NULL;
	
	data2=NULL;
	
	ret=1;
	
	goCD(working);
	
	FilesSetFrameSDS2D(0,Files);
	s = &Files->SetData;
	if(!Files->SDS2DList){
   		fprintf(stderr,"Error name %s No List Pointer\n",name);
	    goto ErrorOut;
	}
	s->limits=Files->SDS2DList[0].limits;
	s->name = FilesImageName(Files,0);
	
	pioName=(char *)s->limits.pioName;
	
   	if(s->limits.xmin == s->limits.xmax || s->limits.ymin == s->limits.ymax){
   		s->limits.xmin=0;
   		s->limits.xmax=s->xsize;
   		s->limits.ymin=0;
   		s->limits.ymax=s->ysize;
   	}
   	
	if(debug > 0){
   		fprintf(stderr,"name %s\n",name);
   		fprintf(stderr,"time %lg pioName %s\n",s->limits.time,s->limits.pioName);
   		fprintf(stderr,"xsize %ld ysize %ld\n",s->xsize,s->ysize);
   		fprintf(stderr,"xmin %lg ymin %lg\n",s->limits.xmin,s->limits.ymin);
   		fprintf(stderr,"xmax %lg ymax %lg \n",s->limits.xmax,s->limits.ymax);
   	}
   	
   	
	dims[0]=s->xsize+1;
	dims[1]=s->ysize+1;

	
	x=(double *)cMalloc((s->xsize+1)*sizeof(double),8751);
	y=(double *)cMalloc((s->ysize+1)*sizeof(double),8752);

	
	if(!x || !y ){
   		fprintf(stderr,"doSDS2D Out of Memory\n");	
   		goto ErrorOut;    
	}
	
	coords[0]=x;
	coords[1]=y;

	
	dv=(s->limits.xmax-s->limits.xmin)/(s->xsize);
	for(n=0;n<s->xsize+1;++n){
	    x[n]=s->limits.xmin+n*dv;
	}
	
	dv=(s->limits.ymax-s->limits.ymin)/(s->ysize);
	for(n=0;n<s->ysize+1;++n){
	    y[n]=s->limits.ymin+n*dv;
	}
	
	if(dir != NULL)
	{
		goCD(dir);
	}
	
    dbfile = NULL;
    
    dbfile = DBCreate(name, DB_CLOBBER, DB_LOCAL,"2D rectilinear mesh", DB_HDF5);
    if(dbfile == NULL)
    {
        fprintf(stderr, "Could not create Silo file : %s \n",name);
   		goto ErrorOut;    
    }


	DBoptlist *optlist = DBMakeOptlist(2);
	
	dtime =s->limits.time;
	DBAddOption(optlist, DBOPT_DTIME, &dtime);
	
	if(cycleFromName > 0){
		char name2[256];
	    char *sp;
	    int ncycle;
		strncpy(name2,name,255);
		sp=strrchr(name2,'.');
		if(sp != NULL){
			*sp=0;
			sp -= 1;
			while(isdigit(*sp)){
			    if(sp > name2){
			    	--sp;
			    }else{
			        break;
			    }
			}
			++sp;
			ncycle=atoi(sp);
			if(debug){
			    printf("cycle %d\n",ncycle);
			}
			DBAddOption(optlist, DBOPT_CYCLE, &ncycle);
		}
	}
    

    DBPutQuadmesh(dbfile, "quadmesh", NULL, coords, dims, ndims,
                  DB_DOUBLE, DB_COLLINEAR, optlist);

    dims[0] -= 1; 
    dims[1] -= 1; 
    
 	if(pioName == NULL){
 	    pioName="var1";
 	}
 	
 	data=s->data;

	if(flip)
	{
	    int i,j;
	    
		data2=(double *)cMalloc((s->xsize*s->ysize)*sizeof(double),8757);
		if(!data2){
   			fprintf(stderr,"doSDS2D Out of Memory\n");	
   			goto ErrorOut;    
		}
		
		for(j=0;j<s->ysize;++j){
			for(i=0;i<s->xsize;++i){
				data2[i+j*s->xsize]=data[i+(s->ysize-j-1)*s->xsize];
			}
		}
		
		data=data2;
		
		if(debug){
			printf("data flipped\n");
		}
	}
    
    DBPutQuadvar1(dbfile, pioName, "quadmesh", data, dims,
                  ndims, NULL, 0, DB_DOUBLE, DB_ZONECENT, NULL);

   DBFreeOptlist(optlist);

    DBClose(dbfile);
	
	ret=0;
	
ErrorOut:

	if(x)cFree((char *)x);
	if(y)cFree((char *)y);
	if(data2)cFree((char *)data2);
	
	return ret;
}



static int doSDS3D(struct FileInfo2 *Files,char *name)
{
	struct SetFrameData *s;
	char *pioName;
	double dtime;
	DBfile *dbfile;
	double *x,*y,*z;
    double *coords[3];
    double dv;
	int dims[3];
	int ndims=3;
	int ret;
	int n;
	
	
	if(!Files)return 1;
	
	x=NULL;
	y=NULL;
	z=NULL;
	
	ret=1;
	
	dtime = 0.0;
	
	goCD(working);
	
	FilesSetFrameSDS3D(0,Files);
	s = &Files->SetData;
	if(!Files->SDS3DList){
   		fprintf(stderr,"Error name %s No List Pointer\n",name);
	    goto ErrorOut;
	}
	s->limits=Files->SDS3DList[0].limits;
	s->name = FilesImageName(Files,0);
	
	pioName=(char *)s->limits.pioName;
	
   	if(s->limits.xmin == s->limits.xmax || s->limits.ymin == s->limits.ymax || s->limits.zmin == s->limits.zmax){
   		s->limits.xmin=0;
   		s->limits.xmax=s->xsize;
   		s->limits.ymin=0;
   		s->limits.ymax=s->ysize;
   		s->limits.zmin=0;
   		s->limits.zmax=s->zsize;
   	}
   	
	if(debug > 0){
   		printf("name %s\n",name);
   		printf("time %lg pioName %s\n",s->limits.time,s->limits.pioName);
   		printf("xsize %ld ysize %ld zsize %ld\n",s->xsize,s->ysize,s->zsize);
   		printf("xmin %lg ymin %lg zmin %lg\n",s->limits.xmin,s->limits.ymin,s->limits.zmin);
   		printf("xmax %lg ymax %lg zmax %lg\n",s->limits.xmax,s->limits.ymax,s->limits.zmax);
   	}
   	
   	

	dims[0]=s->xsize+1;
	dims[1]=s->ysize+1;
	dims[2]=s->zsize+1;
	
	x=(double *)(char **)cMalloc((s->xsize+1)*sizeof(double),8751);
	y=(double *)(char **)cMalloc((s->ysize+1)*sizeof(double),8752);
	z=(double *)(char **)cMalloc((s->zsize+1)*sizeof(double),8753);
	
	if(!x || !y || !z){
   		fprintf(stderr,"doSDS3D Out of Memory\n");	
   		goto ErrorOut;    
	}
	
	coords[0]=x;
	coords[1]=y;
	coords[2]=z;
	
	dv=(s->limits.xmax-s->limits.xmin)/(s->xsize);
	for(n=0;n<s->xsize+1;++n){
	    x[n]=s->limits.xmin+n*dv;
	}
	
	dv=(s->limits.ymax-s->limits.ymin)/(s->ysize);
	for(n=0;n<s->ysize+1;++n){
	    y[n]=s->limits.ymin+n*dv;
	}
	
	dv=(s->limits.zmax-s->limits.zmin)/(s->zsize);
	for(n=0;n<s->zsize+1;++n){
	    z[n]=s->limits.zmin+n*dv;
	}
	
	
	if(dir != NULL)
	{
		goCD(dir);
	}
	
    dbfile = NULL;
    
    dbfile = DBCreate(name, DB_CLOBBER, DB_LOCAL,"3D rectilinear mesh", DB_HDF5);
    if(dbfile == NULL)
    {
        fprintf(stderr, "Could not create Silo file : %s \n",name);
   		goto ErrorOut;    
    }
    

	DBoptlist *optlist = DBMakeOptlist(2);
	
	dtime =s->limits.time;
	DBAddOption(optlist, DBOPT_DTIME, &dtime);
	
	if(cycleFromName > 0){
		char name2[256];
	    char *sp;
	    int ncycle;
		strncpy(name2,name,255);
		sp=strrchr(name2,'.');
		if(sp != NULL){
			*sp=0;
			sp -= 1;
			while(isdigit(*sp)){
			    if(sp > name2){
			    	--sp;
			    }else{
			        break;
			    }
			}
			++sp;
			ncycle=atoi(sp);
			if(debug){
			    printf("cycle %d\n",ncycle);
			}
			DBAddOption(optlist, DBOPT_CYCLE, &ncycle);
		}
	}
    

    DBPutQuadmesh(dbfile, "quadmesh", NULL, coords, dims, ndims,
                  DB_DOUBLE, DB_COLLINEAR, optlist);

    dims[0] -= 1; 
    dims[1] -= 1; 
    dims[2] -= 1;
    
 	if(pioName == NULL){
 	    pioName="var1";
 	}
 	
    DBPutQuadvar1(dbfile, pioName, "quadmesh", s->data, dims,
                  ndims, NULL, 0, DB_DOUBLE, DB_ZONECENT, NULL);


   DBFreeOptlist(optlist);
 

    DBClose(dbfile);
	
	
	
	
	ret=0;
	
ErrorOut:

	if(x)cFree((char *)x);
	if(y)cFree((char *)y);
	if(z)cFree((char *)z);
	
	return ret;
}
