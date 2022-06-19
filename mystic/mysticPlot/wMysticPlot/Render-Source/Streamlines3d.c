#define EXTERN22 extern
#include "Xappl.h"
#include "Render.h"
#include "uFiles.h"
#include "uDialog.h"
#include "Message.h"
#include "NewEdit.h"
#include "FileManager.h"
#include "Universal.h"
#include "Defs.h"


void ScaleMatrix2(struct Matrix *m,struct System *p);
void MultMatrix(struct Matrix *t,struct Matrix *t2,struct Matrix *t1);
void BuildTransformMatrix(struct Matrix *m,struct System *p1,struct System *p2);
int Transform(struct P *p,struct Matrix *m);

int doNoShadowLines(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
		long nel,long CellCount,struct Scene *scene);	

static int doView1Action(struct uDialogStruct *uList);
static int doScale2Start(struct uDialogStruct *uList);
static int doScale2Move(struct uDialogStruct *uList);

int MergeBlock(struct Scene *scene,struct Block *block);


char *StreamLine3D(struct Scene *scene);

void CObjectCreate(struct Scene *scene,CObject *b);

static int getNumber(struct CExpress *e,double *x);

CPio3dStreamLinesPtr CPio3dStreamLinesCreate(struct Scene *scene,struct FileInfo2 *Files,char *streamName);


struct pathData{
     double x;
     double y;
     double z;
};

static struct pathData pd;
static pushpopPtr popStack;

static int CPio3dStreamLinesKill(CObject *o);

static long CPio3dStreamLinesSum(CObject *o);

static int CPio3dStreamLinesCount(CObject *o,long *nodes,long *cells);

static int CPio3dStreamLinesMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells);
		      		      
static int CPio3dStreamLinesDrawLines(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
		      
static int CPio3dStreamLinesDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
		      
		      
CObjectPtr uPio3dStreamWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);

static int CPio3dStreamLinesDraw2(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);

CObjectPtr uPio3dStreamWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CPio3dStreamLinesStruct b;
	
	static struct universalTableListStruct Local[] = {
		{"DOUB",5000,universal_TypeLong,(void *)&b.xyz.count,0L},				
		{"REAL",5001,universal_TypeDoubleArrayPtr,(void *)&b.xyz.x},	
		{"REAL",5002,universal_TypeDoubleArrayPtr,(void *)&b.xyz.y},	
		{"REAL",5003,universal_TypeDoubleArrayPtr,(void *)&b.xyz.z},	

		{"DOUB",4001,universal_TypeDouble,(void *)&b.Local.p.x},	
		{"DOUB",4002,universal_TypeDouble,(void *)&b.Local.p.y},	
		{"DOUB",4003,universal_TypeDouble,(void *)&b.Local.p.z},	
	
		{"DOUB",4004,universal_TypeDouble,(void *)&b.Local.Theta_x},	
		{"DOUB",4005,universal_TypeDouble,(void *)&b.Local.Theta_y},	
		{"DOUB",4006,universal_TypeDouble,(void *)&b.Local.Theta_z},	
	
		{"DOUB",4007,universal_TypeDouble,(void *)&b.Local.Scale_x},	
		{"DOUB",4008,universal_TypeDouble,(void *)&b.Local.Scale_y},	
		{"DOUB",4009,universal_TypeDouble,(void *)&b.Local.Scale_z},	
	
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		
		{"REAL",4019,universal_TypeLong,(void *)&b.nx},
		{"REAL",4020,universal_TypeLong,(void *)&b.ny},
		
		{"DOUB",4021,universal_TypeDouble,(void *)&b.radius},	
		{"DOUB",4022,universal_TypeDouble,(void *)&b.radiusStepFactor},	


		{"REAL",4024,universal_TypeLong,(void *)&b.lineType},
		{"REAL",4025,universal_TypeLong,(void *)&b.streamGradients},
		{"REAL",4026,universal_TypeLong,(void *)&b.streamSteps},
		{"REAL",4027,universal_TypeLong,(void *)&b.streamPasses},
		
		{"REAL",4028,universal_TypeInt,(void *)&b.pioIndex},	
		{"STRI",4029,universal_TypeString,(void *)&b.pioName[0],sizeof(b.pioName)},
		
		
		{"REAL",4030,universal_TypeInt,(void *)&b.pa.DrawZones},
		{"REAL",4031,universal_TypeInt,(void *)&b.pa.ZoneLevel},
		{"REAL",4032,universal_TypeInt,(void *)&b.pa.DrawZonesColor},
		{"REAL",4033,universal_TypeInt,(void *)&b.pa.flagGradients},
		

		{"DOUB",4034,universal_TypeDouble,(void *)&b.sd.plane.Plane_xmin},	
		{"DOUB",4035,universal_TypeDouble,(void *)&b.sd.plane.Plane_ymin},	
		{"DOUB",4036,universal_TypeDouble,(void *)&b.sd.plane.Plane_zmin},	
		
		{"DOUB",4037,universal_TypeDouble,(void *)&b.sd.plane.Plane_xmax},	
		{"DOUB",4038,universal_TypeDouble,(void *)&b.sd.plane.Plane_ymax},	
		{"DOUB",4039,universal_TypeDouble,(void *)&b.sd.plane.Plane_zmax},	
		
		{"DOUB",4041,universal_TypeLong,(void *)&b.sd.plane.Plane_xcount},	
		{"DOUB",4042,universal_TypeLong,(void *)&b.sd.plane.Plane_ycount},	
		{"DOUB",4043,universal_TypeLong,(void *)&b.sd.plane.Plane_zcount},		
	
		{"FILE",4134,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CPio3dStreamLinesPtr bp=(CPio3dStreamLinesPtr)o;
	char head[5];
	long length;
	long tag;
	
	
	if(!inOut || !l)return NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    Local[0].space=0;
		zerol((char *)&b,sizeof(b));
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CPio3dStreamLinesCreate(l->scene,b.Files,NULL);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    }else{
		        goto ErrorOut;
		    }
		}else{
			goto ErrorOut;
		}	
		if(!bp)goto ErrorOut;
		bp->xyz.x=b.xyz.x;
		bp->xyz.y=b.xyz.y;
		bp->xyz.z=b.xyz.z;
		b.xyz.x=NULL;
		b.xyz.y=NULL;
		b.xyz.z=NULL;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		if(bp->Files){
    		struct FilePIOInfo *pio;
			pio=&bp->Files->pioData;
			pio->plane=bp->sd.plane;
		}
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
	    Local[1].space=b.xyz.count;
	    Local[2].space=b.xyz.count;
	    Local[3].space=b.xyz.count;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (CObjectPtr)&b;
ErrorOut:
	return NULL;
}
static int CPio3dStreamLinesDraw2(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CPio3dStreamLinesPtr c=(CPio3dStreamLinesPtr)o;
	struct Scene *scene;
	struct Matrix BlockToWorld;
	struct Matrix BlockToScreen;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double xx,yy,zz;
	double xmin,xmax;
	double ymin,ymax;
	double zmin,zmax;
	double vmin,vmax;
	double *vdata;
	double *xdatal,*ydatal,*zdatal,*gdata;
	double xp,yp,zp,xt,yt,zt;
	float *nvdata;
	float *rdata,*sdata;
	double pi,dumx,dumy;
	long *ndata,nel;
	long nodes,ks,kk;
	Vec Lampl,Eyel;
	int i,j;
	long n,itIs;
	double *x,*y,*z,*e;
	long length,nsphere;
	double r,rstep,sum,radiusStepFactor;
	struct P V1s,V2s,Move,Move2;
	struct P V1,V2,V3,Vp;
	double len1,len2;

	scene=c->scene;

	if(flag == 1){
	    if((scene->material[c->Material].opacity != 1.0))return 0;
	}else if(flag == 2){
	    if((scene->material[c->Material].opacity == 1.0))return 0;
	}

	VecCopy(scene->Eyel,Eyel);
	VecCopy(scene->Lampl,Lampl);

	itIs=c->ItWas;
	if(itIs < 0)itIs=0;
	

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	vdata=scene->vdata;
	gdata=scene->gdata;
	ndata=scene->cdata;
	nvdata=scene->nvdata;
	rdata=scene->rdata;
	sdata=scene->sdata;
	
	V1s=p(0.,0.,1.);
	V2s=p(1.,0.,0.);

    c->FirstCell = *CellCount;

    /* Must Rotate local system to un-rotated local */
    BuildTransformMatrix(&BlockToWorld,&c->Local,&scene->Global);
    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

    v1x=BlockToScreen.x[0][0]*c->Local.Scale_x;
    v1y=BlockToScreen.x[0][1]*c->Local.Scale_y;
    v1z=BlockToScreen.x[0][2]*c->Local.Scale_z;
    v2x=BlockToScreen.x[1][0]*c->Local.Scale_x;
    v2y=BlockToScreen.x[1][1]*c->Local.Scale_y;
    v2z=BlockToScreen.x[1][2]*c->Local.Scale_z;
    v3x=BlockToScreen.x[2][0]*c->Local.Scale_x;
    v3y=BlockToScreen.x[2][1]*c->Local.Scale_y;
    v3z=BlockToScreen.x[2][2]*c->Local.Scale_z;
    xc=BlockToScreen.x[0][3];
    yc=BlockToScreen.x[1][3];
    zc=BlockToScreen.x[2][3];
	    	    
    r=c->radius;
    if(r <= 0)r=0.4*c->LengthGreatest;
    radiusStepFactor=c->radiusStepFactor;
    if(radiusStepFactor <= 0)radiusStepFactor=1.0;
    rstep=2*r*radiusStepFactor;
    
    pi=4*atan(1.0);
    dumy=1.0/((double)c->ny-1);
    dumx=1.0/((double)c->nx-1);
    	
	xmin = ymin = zmin = vmin =  1e60;
	xmax = ymax = zmax = vmax = -1e60;
	
    ks=0;
	nel=0;
    nodes=0;
	nsphere=0;
	
	
	if(scene->debug > 1){
	    sprintf(WarningBuff,"StreamLines %ld MostNodes %ld MostNormals %ld MostCells %ld\n",
	           (long)c->streamsReturned,(long)scene->MostNodes,
	           (long)scene->MostNormals,(long)scene->MostCells);
	    WarningBatch(WarningBuff);
	}
	
	for(n=0;n<c->streamsReturned;++n){
		double xl,s,delta;
		e=c->xyzReturned[n].v;
		x=c->xyzReturned[n].x;
		y=c->xyzReturned[n].y;
		z=c->xyzReturned[n].z;
	    if(!x || !y || !z || !e)continue;
		length=c->xyzReturned[n].count;	
		if(scene->debug > 1){
		    sprintf(WarningBuff,"%ld ",(long)length);
		    WarningBatch(WarningBuff);
		}
		xp=x[0];
		yp=y[0];
		zp=z[0];	
		sum=0;		    
		for(kk=0;kk<length-1;++kk){
		    
	        xl=sqrt((x[kk+1]-x[kk])*(x[kk+1]-x[kk])+(y[kk+1]-y[kk])*(y[kk+1]-y[kk])+(z[kk+1]-z[kk])*(z[kk+1]-z[kk]));
	        if(xl <= 0)continue;
		    
		    if(sum+xl < rstep){
		        sum += 	xl;	
		        continue;
		   	} 
		   			   			   	
		   	delta=rstep-sum;
		   	
		    do {
		       double vv;
	           if((nodes+2*c->nx) > scene->MostNodes){
	               sprintf(WarningBuff,"CPio3dStreamLinesDraw2 Skipping Memory Used %ld Avaiable %ld\n",
	                       nodes,scene->MostNodes);
	               WarningBatch(WarningBuff);
	               sprintf(WarningBuff,"delta %g rstep %g xl %g n %ld\n",delta,rstep,xl,(long)n);
	               WarningBatch(WarningBuff);
	               kk=length;
	               goto doContinue;
	           }
		        ++nsphere;
		        		    		   	   
		   	    s=delta/xl;
		   	    
		   	    xt=x[kk]+s*(x[kk+1]-x[kk]);
		   	    yt=y[kk]+s*(y[kk+1]-y[kk]);
		   	    zt=z[kk]+s*(z[kk+1]-z[kk]);
		   	    
		   	    V1.x=xt-xp;
		   	    V1.y=yt-yp;
		   	    V1.z=zt-zp;
		   	    
		   	    V1=Norm(&V1);
		   	    
		   	    len1=Dot(&V1,&V1s);
		   	    len2=Dot(&V1,&V2s);
		   	    
		   	    if(fabs(len1) < fabs(len2)){
		   	       V2.x=V1s.x-len1*V1.x;
		   	       V2.y=V1s.y-len1*V1.y;
		   	       V2.z=V1s.z-len1*V1.z;
		   	    }else{
		   	       V2.x=V2s.x-len2*V1.x;
		   	       V2.y=V2s.y-len2*V1.y;
		   	       V2.z=V2s.z-len2*V1.z;
		   	    }
		   	    
		   	    V2=Norm(&V2);
		   	    
		   	    V3=CrossN(&V1,&V2);
		   	    
		   	    Vp.x=xp;
		   	    Vp.y=yp;
		   	    Vp.z=zp;
		   	    
		        for(i=0;i<c->nx;++i){
		           Move=Mult(&V2,0.5*r*cos(2*pi*(1.-i*dumx)));
		           Move2=Mult(&V3,0.5*r*sin(2*pi*(1.-i*dumx)));
		           Move=Add(&Move,&Move2);
		           Move=Add(&Move,&Vp);
		           dx=Move.x;
		           dy=Move.y;
		           dz=Move.z;
		           
		           xx=xdatal[nodes]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
				   if(xx)xmin=xx;
				   if(xx > xmax)xmax=xx;
		           yy=ydatal[nodes]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
				   if(yy)ymin=yy;
				   if(yy > ymax)ymax=yy;
		           zz=zdatal[nodes]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
				   if(zz)zmin=zz;
				   if(zz > zmax)zmax=zz;
				   vv=e[kk];
				   vdata[nodes]=vv;
				   if(vv < vmin)vmin=vv;
				   if(vv > vmax)vmax=vv;
				   if(nvdata){				   
						dx=dx-xp;
						dy=dy-yp;
						dz=dz-zp;
						nvdata[3*nodes]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
						nvdata[3*nodes+1]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
						nvdata[3*nodes+2]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
						nvdata[3*nodes+c->nx*3]=nvdata[3*nodes];
						nvdata[3*nodes+1+c->nx*3]=nvdata[3*nodes+1];
						nvdata[3*nodes+2+c->nx*3]=nvdata[3*nodes+2];
					}
		           ++nodes;
		        }
			    
		        for(i=0;i<c->nx;++i){
		           dx=xp+V1.x*r*2+c->origin.x;
		           dy=yp+V1.y*r*2+c->origin.y;
		           dz=zp+V1.z*r*2+c->origin.z;
		           xx=xdatal[nodes]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
				   if(xx)xmin=xx;
				   if(xx > xmax)xmax=xx;
		           yy=ydatal[nodes]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
				   if(yy)ymin=yy;
				   if(yy > ymax)ymax=yy;
		           zz=zdatal[nodes]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
				   if(zz)zmin=zz;
				   if(zz > zmax)zmax=zz;
				   vv=e[kk];
				   vdata[nodes]=vv;
				   if(vv < vmin)vmin=vv;
				   if(vv > vmax)vmax=vv;
		           ++nodes;
		        }
			    
			    j=0;
		        for(i=0;i<c->nx-1;++i){
				    ndata[nel++]=c->Material;
				    ndata[nel++]=i+j*c->nx+ks;
				    ndata[nel++]=i+1+j*c->nx+ks;
				    ndata[nel++]=i+1+(j+1)*c->nx+ks;
				    ndata[nel++]=i+(j+1)*c->nx+ks;
		        }
			    
			    xp=xt;
			    yp=yt;
			    zp=zt;
			    
			    ks=nodes;			    			    
		   		delta += rstep;
			    
		    }while(delta < xl);
		    
		    sum=(1-s)*xl;
		}
doContinue:		
		;
	}
	    	    
	nel /= 5;
	    
	if(scene->debug > 1){
	    sprintf(WarningBuff,"\n nel %ld nodes %ld vmin %g vmax %g\n",nel,nodes,vmin,vmax);
	    WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g \n"
	            ,xmin,xmax,ymin,ymax,zmin,zmax);
	    WarningBatch(WarningBuff);
	}
		
	checkPaletteLevels(scene,c->Material,vmin,vmax);
	
	if(0 && gdata){
	    nodes=0;
		for(n=0;n<nsphere;++n){
		    for(j=0;j<2;++j){
		        double sinf,cosf;
		        sinf=sin(pi*(1.-j*dumy));
		        cosf=cos(pi*(1.-j*dumy));
			    for(i=0;i<c->nx;++i){
			        Vec P,N,D,L;
			        double len;
			        
			        dx=r*cos(2*pi*(1.-i*dumx))*sinf;
			        dy=r*sin(2*pi*(1.-i*dumx))*sinf;
			        dz=r*cosf;
			        
				MakeVector(xdatal[nodes],ydatal[nodes],zdatal[nodes],P);
					
				MakeVector((dx)*v1x+(dy)*v1y+(dz)*v1z,(dx)*v2x+(dy)*v2y+(dz)*v2z,
					           (dx)*v3x+(dy)*v3y+(dz)*v3z,N);
					
				/* Vector Eye To Point */
					
		    		VecSub(P,Eyel,D);
		    		
				/* Check Direction To Eye Point */

				if((VecDot(D,N)) > 0.0){
				     VecNegate(N);
				}


				/*  Vector from Point to Lamp */

				VecSub(Lampl,P,L);
					
				/* Check Direction of Lamp */


				 if((VecDot(L,N)) > 0.0){
				     if((len=VecLen(N)) > 0.0){
				         N[0] /= len;
				         N[1] /= len;
				         N[2] /= len;
				     }
				     if((len=VecLen(L)) > 0.0){
				         L[0] /= len;
				         L[1] /= len;
				         L[2] /= len;
				     }
				     gdata[nodes]=VecDot(N,L);
				 }else{
				     gdata[nodes]=0.;
				 }
				 ++nodes;
			    }
		    }
		}
		(*scene->doNoShadow4G)(ndata,xdatal,ydatal,zdatal,vdata,gdata,nel,*CellCount,scene);
		

	}else if(nvdata){
	/*
	    k=0;
		for(n=0;n<nsphere;++n){
	        for(j=0;j<2;++j){
	            double sinf,cosf;
	            sinf=sin(pi*(1.-j*dumy));
	            cosf=cos(pi*(1.-j*dumy));
			    for(i=0;i<c->nx;++i){		        
			        dx=r*cos(2*pi*(1.-i*dumx))*sinf;
			        dy=r*sin(2*pi*(1.-i*dumx))*sinf;
			        dz=r*cosf;
					 nvdata[k++]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
					 nvdata[k++]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
					 nvdata[k++]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
			    }
	    	}
	    }
*/
	    if(rdata && sdata){
		    dx=0;
		    if(c->nx > 1)dx=1.0/((double)(c->nx-1));

		    dy=0;
		    if(c->ny > 1)dy=1.0/((double)(c->ny-1));

    	    nodes=0;
			for(n=0;n<nsphere;++n){
	            for(j=0;j<2;++j){
			        for(i=0;i<c->nx;++i){
					    rdata[nodes]=(float)((i)*dx);
					    sdata[nodes++]=(float)((j)*dy);
			        }
		    	}
	    	}
	    }
		(*scene->doNoShadow4P)(ndata,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene);
	}else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
	}
	*CellCount += nel;
    c->LastCell = *CellCount;

	return 0;
}
static int CPio3dStreamLinesDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CPio3dStreamLinesPtr c=(CPio3dStreamLinesPtr)o;
	struct Scene *scene;
	struct Matrix BlockToWorld;
	struct Matrix BlockToScreen;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double vmin,vmax;
	double *vdata;
	double *xdatal,*ydatal,*zdatal,*gdata;
	float *nvdata;
	float *rdata,*sdata;
	double pi,dumx,dumy;
	long *ndata,nel;
	long k,ks,kk;
	Vec Lampl,Eyel;
	int i,j;
	long n,itIs;
	double *x,*y,*z,*e;
	long length,nsphere;
	double r,rstep,sum,radiusStepFactor;

	scene=c->scene;

	if(flag == 1){
	    if((scene->material[c->Material].opacity != 1.0))return 0;
	}else if(flag == 2){
	    if((scene->material[c->Material].opacity == 1.0))return 0;
	}

	VecCopy(scene->Eyel,Eyel);
	VecCopy(scene->Lampl,Lampl);

	itIs=c->ItWas;
	if(itIs < 0)itIs=0;
	

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	vdata=scene->vdata;
	gdata=scene->gdata;
	ndata=scene->cdata;
	nvdata=scene->nvdata;
	rdata=scene->rdata;
	sdata=scene->sdata;

    c->FirstCell = *CellCount;

    /* Must Rotate local system to un-rotated local */
    BuildTransformMatrix(&BlockToWorld,&c->Local,&scene->Global);
    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

    v1x=BlockToScreen.x[0][0]*c->Local.Scale_x;
    v1y=BlockToScreen.x[0][1]*c->Local.Scale_y;
    v1z=BlockToScreen.x[0][2]*c->Local.Scale_z;
    v2x=BlockToScreen.x[1][0]*c->Local.Scale_x;
    v2y=BlockToScreen.x[1][1]*c->Local.Scale_y;
    v2z=BlockToScreen.x[1][2]*c->Local.Scale_z;
    v3x=BlockToScreen.x[2][0]*c->Local.Scale_x;
    v3y=BlockToScreen.x[2][1]*c->Local.Scale_y;
    v3z=BlockToScreen.x[2][2]*c->Local.Scale_z;
    xc=BlockToScreen.x[0][3];
    yc=BlockToScreen.x[1][3];
    zc=BlockToScreen.x[2][3];
	    	    
    r=c->radius;
    if(r <= 0)r=0.2*c->LengthGreatest;
    radiusStepFactor=c->radiusStepFactor;
    if(radiusStepFactor <= 0)radiusStepFactor=1.0;
    rstep=2*r*radiusStepFactor;
    
    pi=4*atan(1.0);
    dumy=1.0/((double)c->ny-1);
    dumx=1.0/((double)c->nx-1);
    	
	vmin =  1e60;
	vmax = -1e60;
    k=0;
    ks=0;
	nel=0;
	nsphere=0;
	for(n=0;n<c->streamsReturned;++n){
		e=c->xyzReturned[n].v;
		x=c->xyzReturned[n].x;
		y=c->xyzReturned[n].y;
		z=c->xyzReturned[n].z;
	    if(!x || !y || !z || !e)continue;
		length=c->xyzReturned[n].count;		
		sum=0;		    
		for(kk=0;kk<length-1;++kk){
		    double xl,s,delta;
		    
	        xl=sqrt((x[kk+1]-x[kk])*(x[kk+1]-x[kk])+(y[kk+1]-y[kk])*(y[kk+1]-y[kk])+(z[kk+1]-z[kk])*(z[kk+1]-z[kk]));
	        if(xl <= 0)continue;
		    
		    if(sum+xl < rstep){
		        sum += 	xl;	
		        continue;
		   	} 
		   			   			   	
		   	delta=rstep-sum;
		   	
		    do {
	           if((k+c->ny*c->nx) > scene->MostNodes){
	               sprintf(WarningBuff,"CPio2dRayDraw Skipping Memory Used %ld Avaiable %ld\n",
	                       k,scene->MostNodes);
	               WarningBatch(WarningBuff);
	               sprintf(WarningBuff,"delta %g rstep %g xl %g\n",delta,rstep,xl);
	               WarningBatch(WarningBuff);
	               kk=length;
	               goto doContinue;
	           }
		        ++nsphere;
		        		    		   	   
		   	    s=delta/xl;
		   	    
			    for(j=0;j<c->ny;++j){
			        double sinf,cosf,vv;
			        sinf=sin(pi*(1.-j*dumy));
			        cosf=cos(pi*(1.-j*dumy));
			        for(i=0;i<c->nx;++i){
			           dx=x[kk]+s*(x[kk+1]-x[kk])+c->origin.x+r*cos(2*pi*(1.-i*dumx))*sinf;
			           dy=y[kk]+s*(y[kk+1]-y[kk])+c->origin.y+r*sin(2*pi*(1.-i*dumx))*sinf;
			           dz=z[kk]+s*(z[kk+1]-z[kk])+c->origin.z+r*cosf;
			           xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
			           ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
			           zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
					   vv=e[kk];
					   vdata[k]=vv;
					   if(vv < vmin)vmin=vv;
					   if(vv > vmax)vmax=vv;
			           ++k;
			        }
			    }			    
			    
			    for(j=0;j<c->ny-1;++j){
			        for(i=0;i<c->nx-1;++i){
					    ndata[nel++]=c->Material;
					    ndata[nel++]=i+j*c->nx+ks;
					    ndata[nel++]=i+1+j*c->nx+ks;
					    ndata[nel++]=i+1+(j+1)*c->nx+ks;
					    ndata[nel++]=i+(j+1)*c->nx+ks;
			        }
			    }
			    
			    ks=k;			    			    
		   		delta += rstep;
			    
		    }while(delta < xl);
		    
		    sum=(1-s)*xl;
		}
doContinue:		
		;
	}
	    	    
	nel /= 5;
	    
	checkPaletteLevels(scene,c->Material,vmin,vmax);
	
	if(gdata){
	    k=0;
		for(n=0;n<nsphere;++n){
		    for(j=0;j<c->ny;++j){
		        double sinf,cosf;
		        sinf=sin(pi*(1.-j*dumy));
		        cosf=cos(pi*(1.-j*dumy));
			    for(i=0;i<c->nx;++i){
			        Vec P,N,D,L;
			        double len;
			        
			        dx=r*cos(2*pi*(1.-i*dumx))*sinf;
			        dy=r*sin(2*pi*(1.-i*dumx))*sinf;
			        dz=r*cosf;
			        
				MakeVector(xdatal[k],ydatal[k],zdatal[k],P);
					
				MakeVector((dx)*v1x+(dy)*v1y+(dz)*v1z,(dx)*v2x+(dy)*v2y+(dz)*v2z,
					           (dx)*v3x+(dy)*v3y+(dz)*v3z,N);
					
				/* Vector Eye To Point */
					
		    		VecSub(P,Eyel,D);
		    		
				/* Check Direction To Eye Point */

				if((VecDot(D,N)) > 0.0){
				     VecNegate(N);
				}


				/*  Vector from Point to Lamp */

				VecSub(Lampl,P,L);
					
				/* Check Direction of Lamp */


				 if((VecDot(L,N)) > 0.0){
				     if((len=VecLen(N)) > 0.0){
				         N[0] /= len;
				         N[1] /= len;
				         N[2] /= len;
				     }
				     if((len=VecLen(L)) > 0.0){
				         L[0] /= len;
				         L[1] /= len;
				         L[2] /= len;
				     }
				     gdata[k]=VecDot(N,L);
				 }else{
				     gdata[k]=0.;
				 }
				 ++k;
			    }
		    }
		}
		(*scene->doNoShadow4G)(ndata,xdatal,ydatal,zdatal,vdata,gdata,nel,*CellCount,scene);
	}else if(nvdata){
	    k=0;
		for(n=0;n<nsphere;++n){
	        for(j=0;j<c->ny;++j){
	            double sinf,cosf;
	            sinf=sin(pi*(1.-j*dumy));
	            cosf=cos(pi*(1.-j*dumy));
			    for(i=0;i<c->nx;++i){		        
			        dx=r*cos(2*pi*(1.-i*dumx))*sinf;
			        dy=r*sin(2*pi*(1.-i*dumx))*sinf;
			        dz=r*cosf;
					 nvdata[k++]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
					 nvdata[k++]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
					 nvdata[k++]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
			    }
	    	}
	    }
	    if(rdata && sdata){
		    dx=0;
		    if(c->nx > 1)dx=1.0/((double)(c->nx-1));

		    dy=0;
		    if(c->ny > 1)dy=1.0/((double)(c->ny-1));

    	    k=0;
			for(n=0;n<nsphere;++n){
	            for(j=0;j<c->ny;++j){
			        for(i=0;i<c->nx;++i){
					    rdata[k]=(float)((i)*dx);
					    sdata[k++]=(float)((j)*dy);
			        }
		    	}
	    	}
	    }
		(*scene->doNoShadow4P)(ndata,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene);
	}else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
	}
	*CellCount += nel;
    c->LastCell = *CellCount;

	return 0;
}
static int CPio3dStreamLinesDrawLines(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CPio3dStreamLinesPtr c=(CPio3dStreamLinesPtr)o;
	struct Scene *scene;
	struct Matrix BlockToWorld;
	struct Matrix BlockToScreen;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double vmin,vmax;
	double *vdata;
	double *xdatal,*ydatal,*zdatal;
	long *ndata,nel;
	long k,ks,kk;
	int i;
	long n,itIs;
	double *x,*y,*z,*e;
	long length;
	
	
	if(c->lineType == 1){
	    return CPio3dStreamLinesDraw(o,WorldToScreen,CellCount,flag);
	}else if(c->lineType == 2){
	    return CPio3dStreamLinesDraw2(o,WorldToScreen,CellCount,flag);
	}
	

	scene=c->scene;

	if(flag == 1){
	    if((scene->material[c->Material].opacity != 1.0))return 0;
	}else if(flag == 2){
	    if((scene->material[c->Material].opacity == 1.0))return 0;
	}


	itIs=c->ItWas;
	if(itIs < 0)itIs=0;
	
	if(!c->xyzReturned || (c->streamsReturned < 0))goto ErrorOut;

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	vdata=scene->vdata;
	ndata=scene->cdata;

    c->FirstCell = *CellCount;

    /* Must Rotate local system to un-rotated local */
    BuildTransformMatrix(&BlockToWorld,&c->Local,&scene->Global);
    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

    v1x=BlockToScreen.x[0][0]*c->Local.Scale_x;
    v1y=BlockToScreen.x[0][1]*c->Local.Scale_y;
    v1z=BlockToScreen.x[0][2]*c->Local.Scale_z;
    v2x=BlockToScreen.x[1][0]*c->Local.Scale_x;
    v2y=BlockToScreen.x[1][1]*c->Local.Scale_y;
    v2z=BlockToScreen.x[1][2]*c->Local.Scale_z;
    v3x=BlockToScreen.x[2][0]*c->Local.Scale_x;
    v3y=BlockToScreen.x[2][1]*c->Local.Scale_y;
    v3z=BlockToScreen.x[2][2]*c->Local.Scale_z;
    xc=BlockToScreen.x[0][3];
    yc=BlockToScreen.x[1][3];
    zc=BlockToScreen.x[2][3];
	    	    
	vmin =  1e60;
	vmax = -1e60;
        
    k=0;
    ks=0;
	nel=0;
	
	for(n=0;n<c->streamsReturned;++n){
		e=c->xyzReturned[n].v;
		x=c->xyzReturned[n].x;
		y=c->xyzReturned[n].y;
		z=c->xyzReturned[n].z;
	    if(!x || !y || !z || !e)continue;
		length=c->xyzReturned[n].count;		
		for(kk=0;kk<length-1;++kk){
		    double xl,s;
		    
	        xl=sqrt((x[kk+1]-x[kk])*(x[kk+1]-x[kk])+(y[kk+1]-y[kk])*(y[kk+1]-y[kk])+(z[kk+1]-z[kk])*(z[kk+1]-z[kk]));
	        if(xl <= 0)continue;

			s=0;

			for(i=0;i<2;++i){
			   double vv;
			   dx=x[kk]+s*(x[kk+1]-x[kk])+c->origin.x;
			   dy=y[kk]+s*(y[kk+1]-y[kk])+c->origin.y;
			   dz=z[kk]+s*(z[kk+1]-z[kk])+c->origin.z;
			   xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
			   ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
			   zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
			   vv=e[kk];
			   vdata[k]=vv;
			   if(vv < vmin)vmin=vv;
			   if(vv > vmax)vmax=vv;
			   ++k;
			   s=1.0;
			}
			    
		    ndata[nel++]=c->Material;
		    ndata[nel++]=ks;
		    ndata[nel++]=ks+1;
			    
		    ks=k;
			    			    
		}
	}
	    	    
	nel /= 3;
	    
	checkPaletteLevels(scene,c->Material,vmin,vmax);

	doNoShadowLines(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);

	*CellCount += nel;
    c->LastCell = *CellCount;
ErrorOut:
	return 0;
}

		      
static int CPio3dStreamLinesLoad(CPio3dStreamLinesPtr s,long itIs)
{
	struct SetFrameData d;
	struct Scene *scene;
	long n;
	double *e,*x,*y,*z,len,sum,ll;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double vmin,vmax;
	long length;
	int k;

	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;
	
	if((s->xyz.count <= 0) || !s->xyz.x ||  !s->xyz.y ||  !s->xyz.z)return 0;
	if(s->ItWas == itIs)return 1;

	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    return 0;
	}
	
	if(s->ItWas == itIs)return 1;
	
	
	zerol((char *)&d,sizeof(struct SetFrameData));
	
	d=s->sd;
	
	d.pioIndex=s->pioIndex;
    mstrncpy(d.pioName,s->pioName,64);

	d.streamline3d.xyz=s->xyz;
	d.streamline3d.streamSteps=s->streamSteps;
	d.streamline3d.streamPasses=s->streamPasses;
	d.streamline3d.streamGradients=s->streamGradients;
	d.CurrentFrame=itIs;
	d.type = LINEDATA_STREAMLINE_DATA;
	if(SendMessageByName(s->BatchName,MessageType_GET_CELL_DATA,&d)){
		sprintf(WarningBuff,"CPio3dStreamLinesLoad - file (%s)  Error Getting Area Data\n",s->BatchName);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}	
	
	
	if(s->xyzReturned && (s->streamsReturned > 0)){
		for(k=0;k<s->streamsReturned;++k){
			if(s->xyzReturned[k].x)cFree((char *)s->xyzReturned[k].x);
			s->xyzReturned[k].x=NULL;
			if(s->xyzReturned[k].y)cFree((char *)s->xyzReturned[k].y);
			s->xyzReturned[k].y=NULL;
			if(s->xyzReturned[k].z)cFree((char *)s->xyzReturned[k].z);
			s->xyzReturned[k].z=NULL;
			if(s->xyzReturned[k].v)cFree((char *)s->xyzReturned[k].v);
			s->xyzReturned[k].v=NULL;
		}
		if(s->xyzReturned)cFree((char *)s->xyzReturned);
		s->xyzReturned=NULL;
		s->streamsReturned=0;
	}
	
	s->xyzReturned=d.streamline3d.xyzReturned;
	s->streamsReturned=d.streamline3d.streamsReturned;
	
	if(!s->xyzReturned || (s->streamsReturned < 0))goto ErrorOut;
	
	length=0;
	
	xmin=1e60;
	ymin=1e60;
	zmin=1e60;
	vmin=1e60;
	xmax=-1e60;
	ymax=-1e60;
	zmax=-1e60;
	vmax=-1e60;
	sum = 0;
	len = 0;
	for(n=0;n<s->streamsReturned;++n){
		e=s->xyzReturned[n].v;
		x=s->xyzReturned[n].x;
		y=s->xyzReturned[n].y;
		z=s->xyzReturned[n].z;
	    if(!x || !y || !z || !e)continue;
		length=s->xyzReturned[n].count;
		for(k=1;k<length;++k){
		    ll=sqrt((x[k-1]-x[k])*(x[k-1]-x[k])+(y[k-1]-y[k])*(y[k-1]-y[k])+(z[k-1]-z[k])*(z[k-1]-z[k]));
		    /*
		    sprintf(WarningBuff,"e %g x %g y %g z %g ll %g",e[k],x[k],y[k],z[k],ll);
		    WarningBatchFast(WarningBuff);
		    */
		    sum += ll;
		    if((ll > len))len = ll;
			if(x[k] < xmin)xmin=x[k];
			if(x[k] > xmax)xmax=x[k];
			if(y[k] < ymin)ymin=y[k];
			if(y[k] > ymax)ymax=y[k];
			if(z[k] < zmin)zmin=z[k];
			if(z[k] > zmax)zmax=z[k];
			if(e[k] < vmin)vmin=e[k];
			if(e[k] > vmax)vmax=e[k];
		}	

	}
/*
	WarningBatchAtEnd(WarningBuff);        
*/
	checkPaletteLevels(scene,s->Material,vmin,vmax);	
	
	s->LengthTotal=sum;
	s->LengthGreatest=len;

	s->xminData=xmin;
	s->yminData=ymin;
	s->zminData=zmin;
	s->xmaxData=xmax;
	s->ymaxData=ymax;
	s->ymaxData=ymax;
	s->setData=TRUE;
	
	s->ItWas = itIs;
ErrorOut:
	return 0;
}

static int CPio3dStreamLinesMessage(CObject *b,struct ObjectMessage *Message)
{
	CPio3dStreamLinesPtr s=(CPio3dStreamLinesPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CPio3dStreamLinesLoad(s,Message->time);
	}else if(Message->MessageType == OBJECT_MESSAGE_MESSAGE1000_TYPE){
		struct Message1000 *m;
		int MessageType;
		int n;
		
	    m=Message->m;
	    if(!m)return 0;
	    MessageType=(int)Message->MessageType1000;	    
	    switch(MessageType){
	    case MessageType_GET_FILE_LIST:
	    	{
			    int isFound;
			    
				if(!s->Files)return 0;
				
			    isFound=FALSE;
				for(n=0;n<m->FileCount;++n){
				    if(s->Files == m->FilesList[n]){isFound=TRUE;break;}
				}
				if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
				    m->FilesList[m->FileCount++]=s->Files;
				}		
			}	
		    break;
	    case MessageType_GET_FILE_OWNER:
			for(n=0;n<m->FileCount;++n){
			    if(s->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
			}
		    break;
	    case MessageType_GET_FRAME_COUNT:
		    if(s->ImageCount > m->FrameCount)m->FrameCount=s->ImageCount;
		    break;
		case MessageType_PRINT_FILE_INFORMATION:
	    	sprintf(WarningBuff,"  Sage3d Streamline Files\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    break;
		}
	}else if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA){ 
	    double xdata[8],ydata[8],zdata[8];
		struct Matrix GroupToWorld;
		struct Matrix GroupToScreen;
	    struct P p;
	    long n;
	   
	    if(!s->setData)return 0;
	    
	    BuildTransformMatrix(&GroupToWorld,&s->Local,&s->scene->Global);
	    MultMatrix(&GroupToScreen,Message->WorldToScreen,&GroupToWorld);
	    ScaleMatrix2(&GroupToScreen,&s->Local);
	    
	    xdata[0]=s->xminData;
	    xdata[1]=s->xmaxData;
	    xdata[2]=s->xmaxData;
	    xdata[3]=s->xminData;
	    xdata[4]=s->xminData;
	    xdata[5]=s->xmaxData;
	    xdata[6]=s->xmaxData;
	    xdata[7]=s->xminData;

	    ydata[0]=s->yminData;
	    ydata[1]=s->yminData;
	    ydata[2]=s->ymaxData;
	    ydata[3]=s->ymaxData;
	    ydata[4]=s->yminData;
	    ydata[5]=s->yminData;
	    ydata[6]=s->ymaxData;
	    ydata[7]=s->ymaxData;

	    zdata[0]=s->zminData;
	    zdata[1]=s->zminData;
	    zdata[2]=s->zminData;
	    zdata[3]=s->zminData;
	    zdata[4]=s->zmaxData;
	    zdata[5]=s->zmaxData;
	    zdata[6]=s->zmaxData;
	    zdata[7]=s->zmaxData;


	    for(n=0;n<8;++n){
	        p.x=xdata[n];
	        p.y=ydata[n];
	        p.z=zdata[n];
	        Transform(&p,&GroupToScreen);
	        if(p.x > Message->xmax)Message->xmax=p.x;
	        if(p.y > Message->ymax)Message->ymax=p.y;
	        if(p.z > Message->zmax)Message->zmax=p.z;
	        if(p.x < Message->xmin)Message->xmin=p.x;
	        if(p.y < Message->ymin)Message->ymin=p.y;
	        if(p.z < Message->zmin)Message->zmin=p.z;
	    }
	}
	return 0;
}


static int CPio3dStreamLinesMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells)
{
	CPio3dStreamLinesPtr s=(CPio3dStreamLinesPtr)o;
	double radiusStepFactor,rstep,r;
	long NodeCount;
	long CellCount;
	long sum,nsphere;

	if(!s || !MostNodes || !MostNormals || !MostCells)return 1;
	
	
	r=s->radius;
    if(r <= 0)r=0.2*s->LengthGreatest;
    radiusStepFactor=s->radiusStepFactor;
    if(radiusStepFactor <= 0)radiusStepFactor=1.0;
    rstep=2*r*radiusStepFactor;
    
    nsphere=(s->streamsReturned*2)+(long)(2+s->LengthTotal/rstep);
	
	sum=CPio3dStreamLinesSum(o);

	if(s->lineType == 1){
		NodeCount=s->nx*s->ny*(nsphere);
		CellCount=(s->nx-1)*(s->ny-1)*(nsphere)*5;
	}else if(s->lineType == 2) {
		NodeCount=s->nx*2*(nsphere);
		CellCount=(s->nx-1)*(nsphere)*5;
	}else{
		NodeCount=2*sum;
		CellCount=3*sum;
	}
	if(NodeCount > *MostNodes)*MostNodes = NodeCount;
	if(CellCount > *MostCells)*MostCells = CellCount;
	if(s->scene->SmoothNormals)*MostNormals=1;
	return 0;
}
static int CPio3dStreamLinesCount(CObject *o,long *nodes,long *cells)
{
	CPio3dStreamLinesPtr s=(CPio3dStreamLinesPtr)o;
	double radiusStepFactor,rstep,r;
	long sum,nsphere;

	if(!s || !nodes || !cells)return 1;

	r=s->radius;
    if(r <= 0)r=0.2*s->LengthGreatest;
    radiusStepFactor=s->radiusStepFactor;
    if(radiusStepFactor <= 0)radiusStepFactor=1.0;
    rstep=2*r*radiusStepFactor;
    
    nsphere=(s->streamsReturned*2)+(long)(2+s->LengthTotal/rstep);

	sum=CPio3dStreamLinesSum(o);
	
	if(s->lineType == 1){
		*nodes += s->nx*s->ny*(sum+nsphere);
		*cells += (s->nx-1)*(s->ny-1)*(sum+nsphere);
	}else if(s->lineType == 2) {
		*nodes += s->nx*2*(sum+nsphere);
		*cells += (s->nx-1)*(sum+nsphere);
	}else{
		*nodes += 2*sum;
		*cells += sum;
	}
	return 0;
}

static long CPio3dStreamLinesSum(CObject *o)
{
	CPio3dStreamLinesPtr s=(CPio3dStreamLinesPtr)o;

	long sum;
	long n;
	
	if(!s)return 0L;

	sum=s->xyz.count;
	if(s->xyzReturned && (s->streamsReturned > 0)){
		for(n=0;n<s->streamsReturned;++n){
		    sum += s->xyzReturned[n].count;
		}
	}
	return sum;
	
}

static CObjectPtr CPio3dStreamDuplicate(CObject *b)
{
	CPio3dStreamLinesPtr s=(CPio3dStreamLinesPtr)b;
	CPio3dStreamLinesPtr sdsNew;
	long n,k;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CPio3dStreamLinesPtr)cMalloc(sizeof(CPio3dStreamLines),8565);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;

	if(s->xyz.x && s->xyz.y && s->xyz.z && s->xyz.count){
	   sdsNew->xyz.x=(double *)cMalloc(sizeof(double)*s->xyz.count,8568);
	   sdsNew->xyz.y=(double *)cMalloc(sizeof(double)*s->xyz.count,8568);
	   sdsNew->xyz.z=(double *)cMalloc(sizeof(double)*s->xyz.count,8568);
	   if(!sdsNew->xyz.x || !sdsNew->xyz.z || !sdsNew->xyz.z){
	       cFree((char *)sdsNew);
	       return (CObjectPtr)NULL;
	   }
	   for(n=0;n<s->xyz.count;++n){
	       sdsNew->xyz.x[n]=s->xyz.x[n];
	       sdsNew->xyz.y[n]=s->xyz.y[n];
	       sdsNew->xyz.z[n]=s->xyz.z[n];
	   }
	}else{
	     sdsNew->xyz.x=NULL;
	     sdsNew->xyz.y=NULL;
	     sdsNew->xyz.z=NULL;
	}
	
	if(s->xyzReturned && (s->streamsReturned > 0)){
		sdsNew->xyzReturned=(struct dataStruct *)cMalloc(s->streamsReturned*sizeof(struct dataStruct),8721);
		if(!sdsNew->xyzReturned)goto ErrorOut;
		zerol((char *)sdsNew->xyzReturned,s->streamsReturned*sizeof(struct dataStruct));
		for(k=0;k<s->streamsReturned;++k){
		    sdsNew->xyzReturned[k].x=cMalloc(s->xyzReturned[k].count*sizeof(double),8721);
		    sdsNew->xyzReturned[k].y=cMalloc(s->xyzReturned[k].count*sizeof(double),8721);
		    sdsNew->xyzReturned[k].z=cMalloc(s->xyzReturned[k].count*sizeof(double),8721);
		    sdsNew->xyzReturned[k].v=cMalloc(s->xyzReturned[k].count*sizeof(double),8721);
		    if(!sdsNew->xyzReturned[k].x || !sdsNew->xyzReturned[k].y || !sdsNew->xyzReturned[k].z || !sdsNew->xyzReturned[k].v)goto ErrorOut;
		    sdsNew->xyzReturned[k].count=s->xyzReturned[k].count;
		    for(n=0;n<s->xyzReturned[k].count;++n){
		        sdsNew->xyzReturned[k].x[n]=s->xyzReturned[k].x[n];
		        sdsNew->xyzReturned[k].y[n]=s->xyzReturned[k].y[n];
		        sdsNew->xyzReturned[k].z[n]=s->xyzReturned[k].z[n];
		        sdsNew->xyzReturned[k].v[n]=s->xyzReturned[k].v[n];
		    }
		}
		sdsNew->streamsReturned=s->streamsReturned;
	}	

	return (CObjectPtr)sdsNew;
ErrorOut:
	return (CObjectPtr)NULL;
}
static int CPio3dStreamLinesKill(CObject *o)
{
	CPio3dStreamLinesPtr s=(CPio3dStreamLinesPtr)o;
	int k;
	
	if(!s)return 1;
	
	if(s->xyz.x)cFree((char *)s->xyz.x);
	s->xyz.x=NULL;
	if(s->xyz.y)cFree((char *)s->xyz.y);
	s->xyz.y=NULL;
	if(s->xyz.z)cFree((char *)s->xyz.z);
	s->xyz.z=NULL;

	s->xyz.countMax=0;
	s->xyz.count=0;


	if(s->xyzReturned && (s->streamsReturned > 0)){
		for(k=0;k<s->streamsReturned;++k){
			if(s->xyzReturned[k].x)cFree((char *)s->xyzReturned[k].x);
			s->xyzReturned[k].x=NULL;
			if(s->xyzReturned[k].y)cFree((char *)s->xyzReturned[k].y);
			s->xyzReturned[k].y=NULL;
			if(s->xyzReturned[k].z)cFree((char *)s->xyzReturned[k].z);
			s->xyzReturned[k].z=NULL;
			if(s->xyzReturned[k].v)cFree((char *)s->xyzReturned[k].v);
			s->xyzReturned[k].v=NULL;
		}
		if(s->xyzReturned)cFree((char *)s->xyzReturned);
		s->xyzReturned=NULL;
		s->streamsReturned=0;
	}


	if(s->scene && s->scene->Closing){
		if(s->BatchName[0])FileManagerCloseName(s->BatchName);
	}
	cFree((char *)s);
	return 0;
}

CPio3dStreamLinesPtr CPio3dStreamLinesCreate(struct Scene *scene,struct FileInfo2 *Files,char *editWindowName)
{
	CPio3dStreamLinesPtr s;
 	unsigned char *dat;
    struct CExpress e;
	double x,y,z;
 	IconPtr pWindow;
	struct Block b;
	char buff[256];
	static int Count;
	int type;
	int nn,n;

	if(!scene || !Files)return (CPio3dStreamLinesPtr)NULL;
	
	dat=NULL;

	zerol((char *)&b,sizeof(struct Block));
	if(MergeBlock(scene,&b))return (CPio3dStreamLinesPtr)NULL;

	s=(CPio3dStreamLinesPtr)cMalloc(sizeof(CPio3dStreamLines),8566);
	if(!s)return (CPio3dStreamLinesPtr)NULL;
	zerol((char *)s,sizeof(CPio3dStreamLines));

	popStack=pushpopStart(&pd);
	if(!popStack)goto ErrorOut;		

	sprintf(buff,"Pio3dStreamLines_%03d",Count++);	
    mstrncat(buff,".b",255);
    mstrncpy(s->BatchName,buff,255);
    if(FileManagerAddToList(Files,s->BatchName))goto ErrorOut;

	CObjectCreate(scene,(CObjectPtr)s);

	s->type = G_STREAMLINE_PIO3D;
	s->Files=Files;
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;
	s->Material=getNextMaterial(scene);

	s->nx=20;
	s->ny=20;
	
	s->radiusStepFactor=0.75;
	
	s->streamGradients=TRUE;
	
	if(editWindowName){
		type = MessageType_DO_AREA_DIALOG;
		if(SendMessageByName(s->BatchName,MessageType_DO_DIALOG,&type)){
			goto ErrorOut;
		}
	}
	
	if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
		goto ErrorOut;
	}
	
	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);
     
    if(editWindowName){

		pWindow=FindWindowByName(editWindowName);
		
		if(!pWindow){
		    sprintf(WarningBuff,"Could Not Find Path Window %s\n",editWindowName);
		    WarningBatch(WarningBuff);
		    goto ErrorOut;
		}
		
		dat=EditGetData(pWindow);
		if(!dat){
		    sprintf(WarningBuff,"Found No Path Data In Window %s\n",editWindowName);
		    WarningBatch(WarningBuff);
		    goto ErrorOut;
		}

		pd.x=x=0.0;
		pd.y=y=0.0;
		pd.z=z=0.0;

		nn=0;
		zerol((char *)&e,sizeof(struct CExpress));
		e.dat=(unsigned char *)dat;
		while(!CExpressNext(&e)){
			 if(e.tok == T_EOF){
			      break;	
			 }	 
			 mstrncpy(buff,e.ch,512);
			 if(!mstrcmp(buff,"x")){
			    if(getNumber(&e,&x))continue;
			 }else if(!mstrcmp(buff,"y")){
			    if(getNumber(&e,&y))continue;
			 }else if(!mstrcmp(buff,"z")){
			    if(getNumber(&e,&z))continue;
			    ++nn;
				pd.x=x;
				pd.y=y;
				pd.z=z;
			 	if(pushpopPush(&pd,popStack))goto ErrorOut;
			 }else{
			    sprintf(WarningBuff,"Unknown Command = %s",buff);
		        WarningBatch(WarningBuff);
		     }

		}	
	    nn=pushpopDepth(popStack);
		if(nn <= 0){
		    WarningBatch("No StreamLine Points Found\n");
		    goto ErrorOut;
		}   
		
		s->xyz.x=(double *)cMalloc(nn*sizeof(double),9921);
		s->xyz.y=(double *)cMalloc(nn*sizeof(double),9921);
		s->xyz.z=(double *)cMalloc(nn*sizeof(double),9921);
		
		if(!s->xyz.x || !s->xyz.y || !s->xyz.z){
		    WarningBatch("CPio3dStreamLinesCreate out of memory\n");
		    goto ErrorOut;
		}
		
	    for(n=0;n<nn;++n){
			pushpopPeek(&pd,n,popStack);
			s->xyz.x[n]=pd.x;
			s->xyz.y[n]=pd.y;
			s->xyz.z[n]=pd.z;
			/*
			sprintf(WarningBuff,"x %g y %g z %g",pd.x,pd.y,pd.z);
		    WarningBatchFast(WarningBuff);
		    */
		}	
		
		s->xyz.countMax=nn;
		s->xyz.count=nn;
	
	}

	s->killObject=CPio3dStreamLinesKill;
	s->duplicateObject=CPio3dStreamDuplicate;
	
	s->countObject=CPio3dStreamLinesCount;
	s->pGetMost=CPio3dStreamLinesMost;
	s->message=CPio3dStreamLinesMessage;		
	s->drawObject=CPio3dStreamLinesDrawLines;	
	s->ReadWrite=uPio3dStreamWrite;	
	
	s->nx=20;
	s->ny=20;
	s->radiusStepFactor=0.75;
	
	s->vscale=1.0;
	s->pa.ZoneLevel=1;
	s->pa.DrawZonesColor=1;
	
	s->streamSteps=4000;
		
	pushpopEND(popStack);
	
	if(dat)cFree((char *)dat);
		
	return s;
ErrorOut:
	if(s)cFree((char *)s);
	pushpopEND(popStack);
	if(dat)cFree((char *)dat);
	return (CPio3dStreamLinesPtr)NULL;
}

static int getNumber(struct CExpress *e,double *x)
{
    double sign;
    
	if(!e || !x)return 1;
	
     if(CExpressNext(e))return 1;
	 if(e->tok == T_MINUS){
         if(CExpressNext(e))return 1;
	     sign = -1.0;
	 }else{
	     sign = 1.0;
	 }
	
	*x=sign*atof(e->ch);
	
	return 0;
}
char *StreamLine3D(struct Scene *scene)
{

	static uDialogGroup Items[] =

        {
      /* 00 */      {"Ok", {230,270,70,20},uDialogButton,uDialogNone},
      /* 01 */      {"Cancel", {320,270,80,20},uDialogButton,uDialogNone},
      /* 02 */      {"File List",{20,40,400,200},uDialogScrollList,uDialogNone},
                 
        };

    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
 	static uRect  Rect1   = {100,50,420,310};
 	static char editWindowName[256];

 	uDialogList *uList;
	uDialogReturn *uR;

    int reti;
    int ret;
	
	if(!scene)return NULL;

	reti=1;
	
	uR=NULL;
	
	uList=NULL;
	

	editWindowName[0]=0;
	
	uList=uDialogOpen("Add Streamlines",&Rect1,ItemCount);
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	ret=uDialogSet(uList,0L,	   
		          uDialogSetStart,doScale2Start,
 	              uDialogSetMove,doScale2Move,
	              uDialogSetAction,doView1Action,	              
 
	              uDialogSetDone,uDialogSetDone
	    );
	if(ret)goto ErrorOut;
	
	if(uDialog(uList))goto ErrorOut;
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	
	
	{
		struct ScrollList *List2;
		long count,n;
		
		if(uDialogSet(uList,0L,
		              
	              uDialogSetItem,2L,
	             uDialogGetScrollList,&List2,
	              	              	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
	    
		count=ScrollListCount(List2);
		if(count){
		   n=ScrollListFirstSelect(List2);
		   if(count == 1)n=0;
		   if(n >= 0){
		       ScrollListGet(editWindowName,n,List2);
		   }else{
		       WarningBatch("No Window Was Selected For Streamlines\n");
		       goto ErrorOut;
		   }
		}		    	    
	}

	reti=0;
ErrorOut:	
	
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	if(reti== 0){
		return editWindowName;
	}
	
	return NULL;	
	
}
static int doView1Action(struct uDialogStruct *uList)
{
	uDialogReturn *uR;
	int reti;
	int ret;
	
	if(!uList)return 1;
	
	ret = 1;
	uR=NULL;
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
	if(uList->actionItem == 8){
	

		reti=uDialogSet(uList,0L,
          uDialogSetExitOk,
          uDialogSetDone,uDialogSetDone
		);

		if(reti)goto ErrorOut;	       
	}
	ret = 0;
ErrorOut:	
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}

static int doScale2Move(struct uDialogStruct *uList)
{

	struct ScrollList *list2;
	uRect rList2;
	int ret;
	
	uRect r;
	
	
	if(!uList)return 1;
	
	
	uGetPortBounds(uList->myIcon,&r);
	
	ret=uDialogSet(uList,0L,
              uDialogSetLocation,10,r.ysize-25,
              
              uDialogSetItem,1L,
              uDialogSetLocation,r.xsize-85,r.ysize-25,
              	              	              
              uDialogSetItem,8L,
              uDialogSetLocation,r.xsize-85,r.ysize-55,
              	              	              
              uDialogSetItem,2L,
              uDialogGetRect,&rList2,
              uDialogGetScrollList,&list2,
              
               uDialogSetItem,3L,
               uDialogSetLocation,100,r.ysize-100,
             	              	              
               uDialogSetItem,4L,
               uDialogSetLocation,100,r.ysize-75,
             	              	              
               uDialogSetItem,5L,
               uDialogSetLocation,10,r.ysize-50,
             	              	              
               uDialogSetItem,6L,
               uDialogSetLocation,10,r.ysize-100,
             	              	              
               uDialogSetItem,7L,
               uDialogSetLocation,10,r.ysize-75,
             	              	              
               uDialogSetItem,9L,
               uDialogSetLocation,200,r.ysize-100,
             	              	              
               uDialogSetItem,10L,
               uDialogSetLocation,250,r.ysize-100,
             	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret)return 1;
	
	
			
	rList2.x = 15;
	rList2.y = 25;
	rList2.xsize = (r.xsize-20);
	rList2.ysize = r.ysize-125;
	
	ScrollListFitRect(&rList2,list2);
	
	
	ret=uDialogSet(uList,0L,
	
              uDialogSetItem,2L,
              uDialogSetRect,&rList2,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	return 0;
}
static int doScale2Start(struct uDialogStruct *uList)
{
	struct ScrollList *List2;
	IconPtr p;
	int ret;
	int n;
	
	
	if(!uList)return 1;


	
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&List2,
                            
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	n=0;
	p=IconRoot;
	while(p){
	
 		char sourceName[256];
 		char *names[1];
		int type=2;
		
		if(p->WindowType == ExpressionType){
			GetWindowName(p,sourceName,0);
			if(mstrcmp(sourceName,"BatchPrint")){
				names[0]= sourceName;
				ScrollListInsert(names,&type,n++,1L,List2);
			}
		}
	    p=p->IconNext;
	}
	ScrollListSetTop(0,List2);
	
	if(uList->myIcon)InvalRectMyWindow(uList->myIcon);
	
	return 0;
}
