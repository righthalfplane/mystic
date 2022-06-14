#include <stdio.h>
#include <silo.h>
#include <string.h>
#include "showlib.h"

#define PC 1

static int debug=0;
static char *dir=NULL;

int convert(char *namein,char *nameout);

int getFloatList(FILE *in,double *FloatData,unsigned long count);
int getLongList(FILE *in,long *LongData,unsigned long count);
int getString(unsigned char *s,long n,FILE *in);
int getLong(long *n,FILE *in);

void *cMalloc(unsigned long r,int tag);
int cFree(char *p);

int siloit(struct SHOW *s,DBfile *dbfile,DBoptlist *optlist);		
int doMesh3d(struct SHOW *s,DBfile *dbfile,DBoptlist *optlist);
int doMesh2d(struct SHOW *s,DBfile *dbfile,DBoptlist *optlist);


struct SHOW ss;

#define TYPE_3D 0
#define TYPE_2D 1

int type=TYPE_3D;

// cc -I/opt/local/include -o showtovisit main.c showlib.c -L/opt/local/lib -lsiloh5 -lhdf5 

int main(int argc,char **argv)
{
	char name[256],*sp;
	char nameo[256];

	for(int n=1;n<argc;++n){
	    
		if(debug > 0){
			printf("%d %s\n",n,argv[n]);
		}
		
        if(!strcmp(argv[n],"-dir")){
            dir=argv[++n];
            if(debug > 0){
            	printf("output directory : %s\n",dir);
            }
            continue;
         }else if(!strcmp(argv[n],"-2d")){
        	type=TYPE_2D;
        }else if(!strcmp(argv[n],"-3d")){
         	type=TYPE_3D;
       	}else if(!strcmp(argv[n],"-debug")){
           debug=0;
            if(!strcmp(argv[++n],"on")){
             	debug=1;
           	}
            if(debug){
            	printf("debug : %d\n",debug);
            }
            continue;
        }else {
        
        	strncpy(nameo,argv[n],255);
			strncpy(name,argv[n],255);
			sp=strrchr(name,'/');
			if(sp != NULL){
				strncpy(name,&sp[1],255);
			}
		
			sp=strrchr(name,'.');
			if(sp != NULL){
			    *sp=0;
			}
		
			sp=name+strlen(name);
			sp[0]='.';
			sp[1]='s';
			sp[2]='i';
			sp[3]='l';
			sp[4]='o';
			sp[5]=0;
			convert(nameo,name);
          	continue;
        }
    }
	return 0;
}

int siloit(struct SHOW *s,DBfile *dbfile,DBoptlist *optlist);		

int convert(char *namein,char *nameout)
{
	DBfile *dbfile;
	DBoptlist *optlist;
    char buff[256];
	FILE *in;
	
	printf("namein %s nameout %s\n",namein,nameout);
	
	dbfile = NULL;
	if(type == TYPE_2D){
    	dbfile = DBCreate(nameout, DB_CLOBBER, DB_LOCAL,"2D ARM mesh", DB_HDF5);
    	if(dbfile == NULL)
    	{
        	fprintf(stderr, "Could not create Silo file : %s \n",nameout);
   			return 1;    
    	}
	
	}else{
		dbfile = DBCreate(nameout, DB_CLOBBER, DB_LOCAL,"3D ARM mesh", DB_HDF5);
		if(dbfile == NULL)
		{
			fprintf(stderr, "Could not create Silo file : %s \n",nameout);
   			return 1;    
		}
	}
	
	optlist = DBMakeOptlist(2);
	
	in=fopen(namein,"rb");
	if(in == NULL){
	   fprintf(stderr,"Could Not Open %s To Read\n",namein);
	   return 1;
	}

	if(getString((unsigned char *)buff,4L,in))goto Loop;

	buff[4]=0;

	if(!strcmp(buff,"SHOW")){
		//fprintf(stderr,"Found SHOW\n");;
	}else{
		fprintf(stderr,"File %s Skipped Not A Show File (%s)\n",
				namein,buff);
		goto Loop;
	}


	while(1){
		struct SHOW *s;
		long offset;

		if(getString((unsigned char *)buff,4L,in))goto Loop;

		buff[4]=0;

		if(!strcmp(buff,"BLK1")){
			//fprintf(stderr,"Found BLK1\n");;
		}else if(!strcmp(buff,"END1")){
			continue;
		}else if(!strcmp(buff,"DONE")){
			break;
		}else{
			fprintf(stderr,"File %s Skipped Not A BLK1 record (%s)\n",
				namein,buff);
			goto Loop;
		}
		
		s = &ss;

		if(getLongList(in,&s->nodeCount,1L))goto Loop;
		if(getLongList(in,&s->valueCount,1L))goto Loop;
		if(getLongList(in,&s->normalCount,1L))goto Loop;
		if(getLongList(in,&s->dxyzCount,1L))goto Loop;
		if(getLongList(in,&s->rsCount,1L))goto Loop;
		if(getLongList(in,&s->elementCount,1L))goto Loop;
		if(getLongList(in,&s->nodesMax,1L))goto Loop;
		if(getLongList(in,&s->localSystem,1L))goto Loop;
		if(getLongList(in,&s->stressCount,1L))goto Loop;
		if(getLongList(in,&s->stressMax,1L))goto Loop;
		if(getFloatList(in,&s->time,1L))goto Loop;
		if(getLongList(in,&s->velCount,1L))goto Loop;
			
		DBAddOption(optlist, DBOPT_DTIME, &s->time);		
		
/*
		fprintf(stderr,"s->nodeCount %ld\n",s->nodeCount);
		fprintf(stderr,"s->valueCount %ld\n",s->valueCount);
		fprintf(stderr,"s->normalCount %ld\n",s->normalCount);
		fprintf(stderr,"s->dxyzCount %ld\n",s->dxyzCount);
		fprintf(stderr,"s->rsCount %ld\n",s->rsCount);
		fprintf(stderr,"s->elementCount %ld\n",s->elementCount);
		fprintf(stderr,"s->nodesMax %ld\n",s->nodesMax);
		fprintf(stderr,"s->localSystem %ld\n",s->localSystem);
		fprintf(stderr,"s->stressCount %ld\n",s->stressCount);
		fprintf(stderr,"s->time %f\n\n",s->time);
*/
			if(s->nodeCount){
				s->x=(double *)cMalloc(s->nodeCount*sizeof(double),8454);
				s->y=(double *)cMalloc(s->nodeCount*sizeof(double),8455);
				s->z=(double *)cMalloc(s->nodeCount*sizeof(double),8456);
				if(!s->x || !s->y || !s->z)goto Loop;
		        if(getFloatList(in,s->x,s->nodeCount))goto Loop;
		        if(getFloatList(in,s->y,s->nodeCount))goto Loop;
		        if(getFloatList(in,s->z,s->nodeCount))goto Loop;
		/*
		        for(int k=0;k<s->nodeCount;++k){
		        	printf("%d %f %f %f\n",k,s->x[k],s->y[k],s->z[k]);
		        }
		*/
			}

			if(s->valueCount){
				s->v=(double *)cMalloc(s->valueCount*sizeof(double),8457);
				if(!s->v )goto Loop;
			    if(getFloatList(in,s->v,s->valueCount))goto Loop;
			}

			if(s->normalCount){
				s->nx=(double *)cMalloc(s->normalCount*sizeof(double),8458);
				s->ny=(double *)cMalloc(s->normalCount*sizeof(double),8459);
				s->nz=(double *)cMalloc(s->normalCount*sizeof(double),8460);
				if(!s->nx || !s->ny || !s->nz)goto Loop;
		        if(getFloatList(in,s->nx,s->normalCount))goto Loop;
		        if(getFloatList(in,s->ny,s->normalCount))goto Loop;
		        if(getFloatList(in,s->nz,s->normalCount))goto Loop;
			}

			if(s->dxyzCount){
				s->dx=(double *)cMalloc(s->dxyzCount*sizeof(double),8461);
				s->dy=(double *)cMalloc(s->dxyzCount*sizeof(double),8462);
				s->dz=(double *)cMalloc(s->dxyzCount*sizeof(double),8463);
				if(!s->dx || !s->dy || !s->dz)goto Loop;
			    if(getFloatList(in,s->dx,s->dxyzCount))goto Loop;
			    if(getFloatList(in,s->dy,s->dxyzCount))goto Loop;
			    if(getFloatList(in,s->dz,s->dxyzCount))goto Loop;
			}

			if(s->velCount){
				s->xvel=(double *)cMalloc(s->velCount*sizeof(double),8461);
				s->yvel=(double *)cMalloc(s->velCount*sizeof(double),8462);
				s->zvel=(double *)cMalloc(s->velCount*sizeof(double),8463);
				if(!s->xvel || !s->yvel || !s->zvel)goto Loop;
			    if(getFloatList(in,s->xvel,s->velCount))goto Loop;
			    if(getFloatList(in,s->yvel,s->velCount))goto Loop;
			    if(getFloatList(in,s->zvel,s->velCount))goto Loop;
			}

			if(s->rsCount){
				s->r=(double *)cMalloc(s->rsCount*sizeof(double),8464);
				s->s=(double *)cMalloc(s->rsCount*sizeof(double),8465);
				if(!s->r || !s->s)goto Loop;
			    if(getFloatList(in,s->r,s->rsCount))goto Loop;
			    if(getFloatList(in,s->s,s->rsCount))goto Loop;
			}

			if(s->elementCount){
				s->elements=(long *)cMalloc(s->elementCount*(s->nodesMax+1)*sizeof(long),8466);
				if(!s->elements)goto Loop;
			    if(getLongList(in,s->elements,s->elementCount*(s->nodesMax+1)))goto Loop;
			/*
			    for(int k=0;k<s->elementCount;++k){
			         printf("%ld %ld %ld %ld %ld\n",s->elements[k*5],s->elements[k*5+1],
			         s->elements[k*5+2],s->elements[k*5+3],s->elements[k*5+4]);
			    }
			*/
			}
			
			if(s->stressCount){
			//	printf("s->stressCount %ld s->stressMax %ld\n",s->stressCount,s->stressMax);
				s->stress=(double *)cMalloc(s->stressCount*s->stressMax*sizeof(double),8467);
				if(!s->stress)goto Loop;
			    if(getFloatList(in,s->stress,s->stressCount*s->stressMax))goto Loop;
			}
			
			siloit(s,dbfile,optlist);		
		
	}



Loop:

   	DBFreeOptlist(optlist);

    DBClose(dbfile);

	if(in)fclose(in);
	
	return 0;
}

int siloit(struct SHOW *s,DBfile *dbfile,DBoptlist *optlist)
{
	if(type == TYPE_2D){
	     doMesh2d(s,dbfile,optlist);
	}else{
	     doMesh3d(s,dbfile,optlist);
	}
	return 0;
}
int doMesh2d(struct SHOW *s,DBfile *dbfile,DBoptlist *optlist)
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


	if(!s || !dbfile || !optlist)return 1;
	
	x=NULL;
	y=NULL;
	value=NULL;
	nodelist=NULL;
	
	numcell=s->elementCount;
	  	
   	x=(double *)cMalloc(numcell*4*sizeof(double),8753);
   	y=(double *)cMalloc(numcell*4*sizeof(double),8754);
   	nodelist=(int *)cMalloc(numcell*4*sizeof(int),8755);
    value=(double *)cMalloc(numcell*sizeof(double),8754);
  	
   	if(x == NULL || y == NULL || nodelist == NULL || value == NULL)goto ErrorOut;
   	
   //	fprintf(stderr,"s->nodesMax %ld\n",s->nodesMax);
   	
   	int nskip=s->nodesMax+1;
	
   	coords[0]=s->x;
   	
   	coords[1]=s->y;
   	
   	shapesize[0]=s->nodesMax;
   	
	lnodelist=numcell*s->nodesMax;
	
	nnodes = s->nodeCount;
	
	nzones = numcell;
	
	shapecounts[0]=numcell;
	
/*
	for(n=0;n<s->nodeCount;++n){
		fprintf(stderr,"n %d %g %g\n",n,s->x[n],s->y[n]);
   	}
*/

 	if(debug > 0){
   		fprintf(stderr,"numcell %ld\n",numcell);
 	}
 	
 	ne=0;
	nn=0;
	for(n=0;n<numcell;++n){
	    value[ne++]=s->elements[nskip*n];
		nodelist[nn++]=s->elements[nskip*n+1];
		nodelist[nn++]=s->elements[nskip*n+2];
		nodelist[nn++]=s->elements[nskip*n+3];
		if(nskip == 5)nodelist[nn++]=s->elements[nskip*n+4];
	}
	
 	DBPutZonelist2(dbfile, "zonelist", nzones, ndims, nodelist, lnodelist,0,0,0,
 	               shapetype,shapesize,shapecounts, nshapetypes,NULL);	
 	              
 	DBPutUcdmesh(dbfile, "mesh", ndims, NULL, coords, nnodes, nzones,
   "zonelist", NULL, DB_DOUBLE, optlist);            
      
	DBPutUcdvar1(dbfile, "mat", "mesh", value, nzones, NULL, 0,
    DB_DOUBLE, DB_ZONECENT, NULL);
      	
    //fprintf(stderr,"stressCount %ld stressMax %ld numcell %ld\n",s->stressCount,s->stressMax,numcell);
    
    if(s->stressCount == numcell){
	
		for(n=0;n<numcell;++n){
	   	 	value[n]=s->stress[s->stressMax*n];
		}
		DBPutUcdvar1(dbfile, "sxx", "mesh", value, nzones, NULL, 0,
    	DB_DOUBLE, DB_ZONECENT, NULL);
	
		
		for(n=0;n<numcell;++n){
	   	 	value[n]=s->stress[s->stressMax*n+1];
		}
		DBPutUcdvar1(dbfile, "syy", "mesh", value, nzones, NULL, 0,
    	DB_DOUBLE, DB_ZONECENT, NULL);
	
		
		for(n=0;n<numcell;++n){
	   	 	value[n]=s->stress[s->stressMax*n+2];
		}
		DBPutUcdvar1(dbfile, "sxy", "mesh", value, nzones, NULL, 0,
    	DB_DOUBLE, DB_ZONECENT, NULL);
	
	}
	
    if(s->dxyzCount > 0){
        char *varnames[2];
        double *vars[2]; 
        const char **p=(const char **)&varnames[0];
            
		vars[0]=s->dx;
		vars[1]=s->dy;
		varnames[0]="dx";
		varnames[1]="dy";
		ne=s->dxyzCount;
		
				
		DBPutUcdvar(dbfile, "displacement", "mesh",
						2, p,vars,
						ne, NULL, 0,
						DB_DOUBLE,DB_NODECENT,
						NULL);
	}
	
    if(s->velCount > 0){
        char *varnames[2];
        double *vars[2]; 
        const char **p=(const char **)&varnames[0];
            
		vars[0]=s->xvel;
		vars[1]=s->yvel;
		varnames[0]="xvel";
		varnames[1]="yvel";
		ne=s->velCount;
		
				
		DBPutUcdvar(dbfile, "velocity", "mesh",
						2, p,vars,
						ne, NULL, 0,
						DB_DOUBLE,DB_NODECENT,
						NULL);
	}
	
	
	
ErrorOut:
	if(x)cFree((char *)x);
	if(y)cFree((char *)y);
	if(value)cFree((char *)value);
	if(nodelist)cFree((char *)nodelist);
	return 0;
}
int doMesh3d(struct SHOW *s,DBfile *dbfile,DBoptlist *optlist)
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


	if(!s || !dbfile || !optlist)return 1;
	
	x=NULL;
	y=NULL;
	z=NULL;
	value=NULL;
	nodelist=NULL;
	
	numcell=s->elementCount;

   	x=(double *)cMalloc(numcell*8*sizeof(double),8753);
   	y=(double *)cMalloc(numcell*8*sizeof(double),8754);
   	z=(double *)cMalloc(numcell*8*sizeof(double),8754);
   	nodelist=(int *)cMalloc(numcell*8*sizeof(int),8755);
    value=(double *)cMalloc(numcell*sizeof(double),8754);
  	
   	if(x == NULL || y == NULL || z == NULL || nodelist == NULL || value == NULL)goto ErrorOut;

   	coords[0]=s->x;
   	
   	coords[1]=s->y;
   	
   	coords[2]=s->z;
   	
	lnodelist=numcell*8;
	
	nnodes = s->nodeCount;
	
	nzones = numcell;
	
	shapecounts[0]=numcell;
	
   	
 	if(debug > 0){
   		fprintf(stderr,"numcell %ld\n",numcell);
 	}
 	
 	ne=0;
	nn=0;

	for(int n=0;n<numcell;++n){
		value[n]=s->elements[n*9];
		nodelist[nn++]=s->elements[n*9+1];
		nodelist[nn++]=s->elements[n*9+2];
		nodelist[nn++]=s->elements[n*9+3];
		nodelist[nn++]=s->elements[n*9+4];
		nodelist[nn++]=s->elements[n*9+5];
		nodelist[nn++]=s->elements[n*9+6];
		nodelist[nn++]=s->elements[n*9+7];
		nodelist[nn++]=s->elements[n*9+8];
	}
	
 	DBPutZonelist2(dbfile, "zonelist", nzones, ndims, nodelist, lnodelist,0,0,0,
 	               shapetype,shapesize,shapecounts, nshapetypes,NULL);	
 	              
 	DBPutUcdmesh(dbfile, "mesh", ndims, NULL, coords, nnodes, nzones,
   "zonelist", NULL, DB_DOUBLE, optlist);            
      
	DBPutUcdvar1(dbfile, "mat", "mesh", value, nzones, NULL, 0,
    DB_DOUBLE, DB_ZONECENT, NULL);
      	
    if(s->dxyzCount > 0){
        char *varnames[3];
        double *vars[3]; 
        const char **p=(const char **)&varnames[0];
            
		vars[0]=s->dx;
		vars[1]=s->dy;
		vars[2]=s->dz;
		varnames[0]="dx";
		varnames[1]="dy";
		varnames[2]="dz";
		ne=s->dxyzCount;
		
		
		fprintf(stderr,"nzones %d dxyzCount %ld nnodes %d\n",nzones,s->dxyzCount,nnodes);
		
			DBPutUcdvar(dbfile, "displacement", "mesh",
						3, p,vars,
						ne, NULL, 0,
						DB_DOUBLE,DB_NODECENT,
						NULL);
	}
	
    if(s->velCount > 0){
        char *varnames[3];
        double *vars[3]; 
        const char **p=(const char **)&varnames[0];
            
		vars[0]=s->xvel;
		vars[1]=s->yvel;
		vars[2]=s->zvel;
		varnames[0]="xvel";
		varnames[1]="yvel";
		varnames[2]="zvel";
		ne=s->velCount;
		
		
		fprintf(stderr,"nzones %d velCount %ld nnodes %d\n",nzones,s->velCount,nnodes);
		
			DBPutUcdvar(dbfile, "velocity", "mesh",
						3, p,vars,
						ne, NULL, 0,
						DB_DOUBLE,DB_NODECENT,
						NULL);
	}
	
    if(s->stressCount == numcell){
	
		for(n=0;n<numcell;++n){
	   	 	value[n]=s->stress[s->stressMax*n];
		}
		DBPutUcdvar1(dbfile, "sxx", "mesh", value, nzones, NULL, 0,
    	DB_DOUBLE, DB_ZONECENT, NULL);
	
		
		for(n=0;n<numcell;++n){
	   	 	value[n]=s->stress[s->stressMax*n+1];
		}
		DBPutUcdvar1(dbfile, "syy", "mesh", value, nzones, NULL, 0,
    	DB_DOUBLE, DB_ZONECENT, NULL);
	
		
		for(n=0;n<numcell;++n){
	   	 	value[n]=s->stress[s->stressMax*n+2];
		}
		DBPutUcdvar1(dbfile, "szz", "mesh", value, nzones, NULL, 0,
    	DB_DOUBLE, DB_ZONECENT, NULL);
	
	}
	
	
	
ErrorOut:
	if(x)cFree((char *)x);
	if(y)cFree((char *)y);
	if(z)cFree((char *)z);
	if(value)cFree((char *)value);
	if(nodelist)cFree((char *)nodelist);
	return 0;
}
int getString(unsigned char *data,long length,FILE *out)
{
	if(!data || !out)return 1;
	if((long)fread(data,1,length,out) != length){
	    return 1;
	}
	return 0;
}

int getLongList(FILE *in,long *LongData,unsigned long count)
{
	unsigned long n;

	if(!in || !LongData)return 1;

	for(n=0;n<count;++n){
	   if(getLong(&LongData[n],in))return 1;
	}

	return 0;
}
int getFloatList(FILE *in,double *FloatData,unsigned long count)
{
	unsigned long length;

	if(!in || !FloatData)return 1;

	length=count*sizeof(double);

	if(fread(FloatData,1,length,in) != length)return 1;
#ifdef PC
	{
		unsigned char c,*cp;
		unsigned long np;

	        for(np=0;np<count;np++){
	            cp=(unsigned char *)&FloatData[np];
			    c=cp[0];
			    cp[0]=cp[7];
			    cp[7]=c;
			    c=cp[1];
			    cp[1]=cp[6];
			    cp[6]=c;
			    c=cp[2];
			    cp[2]=cp[5];
			    cp[5]=c;
			    c=cp[3];
			    cp[3]=cp[4];
			    cp[4]=c;
	        }
	}
#endif

	return 0;
}
int getLong(long *n,FILE *in)
{
        unsigned long c1,c2,c3,c4;
 
        if((c1=fgetc(in)) == EOF)return 1;
        if((c2=fgetc(in)) == EOF)return 1;
        if((c3=fgetc(in)) == EOF)return 1;
        if((c4=fgetc(in)) == EOF)return 1;
        *n = (long)((c1 << 24) | (c2 << 16) | (c3 << 8) | c4);
        return 0;
 
}
