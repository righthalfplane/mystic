/*
 *  SageTraceBangMeter.c
 *  mShow
 *
 *  Created by Dale Ranta on 10/15/12.
 *  Copyright 2012 SAIC. All rights reserved.
 *
 */

#define EXTERN22 extern
#include "Xappl.h"
#include "uLib.h"
#include "uDialog.h"
#include "ScrollList.h"
#include "BatchFile.h"

#include "Sagevol.h"

double cellDepthRay(long ne,struct rayData *rayin);

int FindTraceOrder(vIconPtr myIcon);

int getPalDouble(char *name,double *rr,double *gg,double *bb);

static int SageTraceCellDepth(long ne,struct rayData *rayin);

int SageTraceBangMeter(mThread *Threads)
{
	
	struct vScene *scene;
	vIconPtr myIcon;
	struct PIO *sage;
	SagePtr pio;
	double red[256],green[256],blue[256];
	struct Color BackGround;
	struct P s,r,Eye;
	struct P Lamp;
	double *cdata,*opacity,*mat_opacity;
	double mat_scale;
	double xmin,xmax;
	double ymin,ymax;
	double zmin,zmax;
	double dmin,dmax;
	double a,b,alpha;
	double xx,yy,zz;
	double s1,s2,ss;
	double didx,djdy,dkdz;
	long ixmax,iymax,izmax,iyxmax;
	long ystep,ostep;
	long samples;
	long maxstep;
	long i,j,nc,no;
	long nemax;
	long ierror;
	long *location;
	int ihit;
    char *hasData;
	float *gradx;
	float *grady;
	float *gradz;
	double *daughter;
	double *mean;
	struct rayData ray;
    tFlt *xcenter;
    tFlt *ycenter;
    tFlt *zcenter;
    unsigned char *level;
	tFlt *data;
    struct cell *nCell;
    unsigned char *nCellEnter;
	double *opacityToFireball;
    long sCount;
    long length;
    long nn;
    double dxdi;
    double dydj;
    double dzdk;
	
	if(!Threads)return 1;
	myIcon=(vIconPtr)Threads->data;
	if(!myIcon)return 1;
	scene=&myIcon->scene;
	pio=&myIcon->pio;
	sage=pio->sage;
	if(!sage)return 1;
	
	
	cdata=scene->cdata;
	if(!cdata)return 1;
	data=pio->value;
	if(!data)return 1;
	opacity=scene->opacity;
	if(!opacity)return 1;
	opacityToFireball=scene->opacityToFireball;
	if(!opacityToFireball)return 1;
	mat_opacity=scene->mat_opacity;
	if(!mat_opacity)return 1;
	samples=scene->samples;
	if(!samples)return 1;
	location=pio->location;
	if(!location)return 1;
	hasData=pio->hasData;
	if(!hasData)return 1;
	gradx=pio->gradx;
	if(!gradx)return 1;
	grady=pio->grady;
	if(!grady)return 1;
	gradz=pio->gradz;
	if(!gradz)return 1;
	daughter=pio->daughter;
	if(!daughter)return 1;
	
	xcenter=pio->xcenter;
	if(!xcenter)return 1;
	ycenter=pio->ycenter;
	if(!ycenter)return 1;
	zcenter=pio->zcenter;
	if(!zcenter)return 1;
	
	level=pio->level;
	if(!level)return 1;
	
	mean=pio->mean;
	if(!mean)return 1;
	
	FindTraceOrder(myIcon);
	
	getPalDouble(scene->palname,red,green,blue);
	
	Lamp=scene->Lamp;
	
	xmin=pio->xmin;
	xmax=pio->xmax;
	
	ixmax=sage->ixmax;
	
	didx=(double)(ixmax)/(xmax-xmin);
	
	ymin=pio->ymin;
	ymax=pio->ymax;
	
	iymax=sage->iymax;
	djdy=(double)(iymax)/(ymax-ymin);
	
	zmin=pio->zmin;
	zmax=pio->zmax;
	
	izmax=sage->izmax;
	dkdz=(double)(izmax)/(zmax-zmin);
	
	iyxmax=ixmax*iymax;
	
	
	length=2*(ixmax+iymax+izmax+3);
	
	
	dxdi=(xmax-xmin)/(double)ixmax;
	dydj=(ymax-ymin)/(double)iymax;
	dzdk=(zmax-zmin)/(double)izmax;
	
	
	
	nCell=NULL;
	
	nCellEnter=NULL;
	
	nCell=(struct cell *)cMalloc(sizeof(struct cell)*length,29725);
	
	if(!nCell)goto ErrorOut;
	
	
	Eye=scene->Eye;
	
	
	BackGround=scene->BackGround;
	
	ystep=3*scene->xResolution;
	ostep=scene->xResolution;;
	
	dmin = myIcon->dmin;
	dmax = myIcon->dmax;
	
	mat_scale=(((double)samples-1.0)/(dmax-dmin));
	
	maxstep=0;
	ierror = 0;
	
	nemax=(ixmax*iymax*izmax);
	
	nCellEnter=(unsigned char *)cMalloc(sizeof(unsigned char)*nemax,29725);
	
	if(!nCellEnter)goto ErrorOut;
	
	ray.hasData=hasData;
	ray.gradx=gradx;
	ray.grady=grady;
	ray.gradz=gradz;
	ray.daughter=daughter;
	ray.mean=mean;
	ray.opacityToFireball=opacityToFireball;
	
	ray.red=red;
	ray.green=green;
	ray.blue=blue;
	
	ray.data=data;
	
	ray.cdata=cdata;
	ray.opacity=opacity;
	ray.mat_opacity=mat_opacity;
	
	ray.dmin=dmin;
	ray.dmax=dmax;
	
	ray.logscale=myIcon->logscale;
	ray.debug=scene->debug;
	if(ray.logscale){
		if((ray.dmin > 0) && (ray.dmax > 0)){
			ray.a=(254.0-2.0)/(log10(ray.dmax)-log10(ray.dmin));
			ray.b=2.0-ray.a*log10(ray.dmin);
		}else{
			ray.logscale = 0;
			ray.a=(254.0-2.0)/(ray.dmax-ray.dmin);
			ray.b=2.0-ray.a*ray.dmin;
			if(Threads->smin == 0){
				sprintf(WarningBuff,"dmin or dmax less than zero log scale turnned off\n");
				if(scene->debug > 1)WarningBatch(WarningBuff);
			}
		}
	}else{
		ray.a=(254.0-2.0)/(ray.dmax-ray.dmin);
		ray.b=2.0-ray.a*ray.dmin;
	}
	ray.mat_scale=mat_scale;
	
	ray.Lamp=Lamp;
	
	ray.Eye=Eye;
	
	ray.samples=samples;
	
	ray.xcenter=xcenter;
	ray.ycenter=ycenter;
	ray.zcenter=zcenter;
	
	ray.dx2=pio->dx2;
	ray.dy2=pio->dy2;
	ray.dz2=pio->dz2;
	
	ray.nemax=pio->numcell;
	
	ray.level=level;
	
	if(myIcon->CellSize){
	    ray.CellSizeOver=1.0/myIcon->CellSize;
	}else{
	    ray.CellSizeOver=1.0;
	}
	ray.RayDirection=scene->RayDirection;
	
	
	if(scene->PrintEye && (Threads->smin == 0)){
		sprintf(WarningBuff,"Eye %g %g %g\n",Eye.x,Eye.y,Eye.z);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"Lamp %g %g %g\n",Lamp.x,Lamp.y,Lamp.z);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"Up %g %g %g\n",scene->Up.x,scene->Up.y,scene->Up.z);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"Target %g %g %g\n",scene->Target.x,scene->Target.y,scene->Target.z);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"Degrees %g\n",scene->Degrees);
		WarningBatch(WarningBuff);
	}
	
	for(j=Threads->smin;j<Threads->smax;++j){
	    b=scene->ymin+((double)j+.5)*scene->dy;
	    /*if(j <= 89 || j >= 92)continue; */
	    for(i=0;i<scene->xResolution;++i){
			double ox;
			double oy;
			double oz;
	        a=scene->xmin+((double)i+.5)*scene->dx;
	        /* if(i <= 403 || i >= 406)continue; */
			s.x=scene->pScreen.x+a*scene->V1.x+b*scene->V2.x;
			s.y=scene->pScreen.y+a*scene->V1.y+b*scene->V2.y;
			s.z=scene->pScreen.z+a*scene->V1.z+b*scene->V2.z;
			r.x=s.x-Eye.x;
			r.y=s.y-Eye.y;
			r.z=s.z-Eye.z;
			s1=1e33;
			s2=-1;
			ihit=0;
			if(r.x){
				ox=1.0/r.x;
				alpha=(xmin-Eye.x)*ox;
				if(alpha >= 0){
					yy=Eye.y+alpha*r.y;
					zz=Eye.z+alpha*r.z;
					if(yy >= ymin && yy <= ymax && zz >= zmin && zz <= zmax){
						if(alpha < s1 )s1 = alpha;
						if(alpha > s2 )s2 = alpha;
						ihit=1;
					}
				}
				alpha=(xmax-Eye.x)*ox;
				if(alpha >= 0){
					yy=Eye.y+alpha*r.y;
					zz=Eye.z+alpha*r.z;
					if(yy >= ymin && yy <= ymax && zz >= zmin && zz <= zmax){
						if(alpha < s1 )s1 = alpha;
						if(alpha > s2 )s2 = alpha;
						ihit=1;
					}
				}
			}else{
				ox=0.0;
			}
			if(r.y){
				oy=1.0/r.y;
				alpha=(ymin-Eye.y)*oy;
				if(alpha >= 0){
					xx=Eye.x+alpha*r.x;
					zz=Eye.z+alpha*r.z;
					if(xx >= xmin && xx <= xmax && zz >= zmin && zz <= zmax){
						if(alpha < s1 )s1 = alpha;
						if(alpha > s2 )s2 = alpha;
						ihit=1;
					}
				}
				alpha=(ymax-Eye.y)*oy;
				if(alpha >= 0){
					xx=Eye.x+alpha*r.x;
					zz=Eye.z+alpha*r.z;
					if(xx >= xmin && xx <= xmax && zz >= zmin && zz <= zmax){
						if(alpha < s1 )s1 = alpha;
						if(alpha > s2 )s2 = alpha;
						ihit=1;
					}
				}
			}else{
				oy=0.0;
			}
			
			if(r.z){
				oz=1.0/r.z;
				alpha=(zmin-Eye.z)*oz;
				if(alpha >= 0){
					xx=Eye.x+alpha*r.x;
					yy=Eye.y+alpha*r.y;
					if(xx >= xmin && xx <= xmax && yy >= ymin && yy <= ymax){
						if(alpha < s1 )s1 = alpha;
						if(alpha > s2 )s2 = alpha;
						ihit=1;
					}
				}
				alpha=(zmax-Eye.z)*oz;
				if(alpha >= 0){
					xx=Eye.x+alpha*r.x;
					yy=Eye.y+alpha*r.y;
					if(xx >= xmin && xx <= xmax && yy >= ymin && yy <= ymax){
						if(alpha < s1 )s1 = alpha;
						if(alpha > s2 )s2 = alpha;
						ihit=1;
					}
				}
			}else{
				oz=0.0;
			}
			nc=3*i+ystep*j;
			if(!ihit){
				cdata[nc]=BackGround.red;
				cdata[nc+1]=BackGround.green;
				cdata[nc+2]=BackGround.blue;
				continue;
			}
			
			if((s1 == s2) || (s1 < 0)){
				s1 = 0;
			}else{
				s1 -= 1.0e-8;
			}
			s2 += 1.0e-8;
			
			no=i+j*ostep;
			
			{
				long ne;
				int x,y,z;
				int n;
				
				sCount=0;
				if(r.x){
					xx=xmin-dxdi;
					for(n=0;n<=ixmax;++n){
						xx += dxdi;
						ss=(xx-Eye.x)*ox;
						if(ss >= s1 && ss <= s2){
							yy=Eye.y+ss*r.y;
							zz=Eye.z+ss*r.z;
							/* if(yy >= ymin && yy <= ymax && zz >= zmin && zz <= zmax){ */
							y=(int)((yy-ymin)*djdy);
							if(y < 0){
								y = 0;
							}else if(y >= iymax){
								y = (int)(iymax - 1);
							}
							z=(int)((zz-zmin)*dkdz);
							if(z < 0){
								z = 0;
							}else if(z >= izmax){
								z = (int)(izmax - 1);
							}
							if(n == 0){
								ne=n+y*ixmax+z*iyxmax;
								nCell[sCount++].ne = ne;
							}else if(n == ixmax){
								ne=n-1+y*ixmax+z*iyxmax;
								nCell[sCount++].ne = ne;
							}else{
								ne=(n-1)+y*ixmax+z*iyxmax;
								nCell[sCount++].ne = ne;
								ne=n+y*ixmax+z*iyxmax;
								nCell[sCount++].ne = ne;
							}
							/* } */
						}
					}
				}
				
				if(r.y){
					yy=ymin-dydj;
					for(n=0;n<=iymax;++n){
						yy += dydj;
						ss=(yy-Eye.y)*oy;
						if(ss >= s1 && ss <= s2){
							xx=Eye.x+ss*r.x;
							zz=Eye.z+ss*r.z;
							/* if(xx >= xmin && xx <= xmax && zz >= zmin && zz <= zmax){ */
							x=(int)((xx-xmin)*didx);
							if(x < 0){
								x = 0;
							}else if(x >= ixmax){
								x=(int)(ixmax-1);
							}
							z=(int)((zz-zmin)*dkdz);
							if(z < 0){
								z = 0;
							}else if(z >= izmax){
								z=(int)(izmax-1);
							}
							if(n == 0){
								ne=x+n*ixmax+z*iyxmax;
								nCell[sCount++].ne = ne;
							}else if(n == iymax){
								ne=x+(n-1)*ixmax+z*iyxmax;
								nCell[sCount++].ne = ne;
							}else{
								ne=x+(n-1)*ixmax+z*iyxmax;
								nCell[sCount++].ne = ne;
								ne=x+n*ixmax+z*iyxmax;
								nCell[sCount++].ne = ne;
							}
							/* } */
						}
					}
				}
				
				if(r.z){
					zz=zmin-dzdk;
					for(n=0;n<=izmax;++n){
						zz += dzdk;
						ss=(zz-Eye.z)*oz;
						if(ss >= s1 && ss <= s2){
							xx=Eye.x+ss*r.x;
							yy=Eye.y+ss*r.y;
							/* if(xx >= xmin && xx <= xmax && yy >= ymin && yy <= ymax){ */
							x=(int)((xx-xmin)*didx);
							if(x < 0){
								x = 0;
							}else if(x >= ixmax){
								x = (int)(ixmax - 1);
							}
							
							y=(int)((yy-ymin)*djdy);
							if(y < 0){
								y = 0;
							}else if(y >= iymax){
								y = (int)(iymax - 1);
							}
							
							if(n == 0){
								ne=x+y*ixmax+n*iyxmax;
								nCell[sCount++].ne = ne;
							}else if(n == izmax){
								ne=x+y*ixmax+(n-1)*iyxmax;
								nCell[sCount++].ne = ne;
							}else{
								ne=x+y*ixmax+(n-1)*iyxmax;
								nCell[sCount++].ne = ne;
								ne=x+y*ixmax+n*iyxmax;
								nCell[sCount++].ne = ne;
							}
							/* } */
						}
					}
				}
				
				
				if(!sCount)goto TraceIsDone;
				
				
				
				
				nn = 0;
				for(n=0;n<sCount;++n){
					double dx,dy,dz;
					long nex,ney,nez;
					ne=nCell[n].ne;
					
					if(nn > 0 && (ne == nCell[nn-1].ne)){
						continue;
					}
					
					if(ne < 0 || ne >= nemax){++ierror;continue;}
					
					if(!hasData[location[ne]])continue;
					nez=ne/iyxmax;
					ney=(ne-nez*iyxmax)/ixmax;
					nex=ne-nez*iyxmax-ney*ixmax;
					dx=Eye.x-(((double)nex+.5)*dxdi+xmin);
					dy=Eye.y-(((double)ney+.5)*dydj+ymin);
					dz=Eye.z-(((double)nez+.5)*dzdk+zmin);
					
					nCell[nn].s = dx*dx+dy*dy+dz*dz;
					nCell[nn++].ne = ne;
					nCellEnter[ne] = 0;
					
				}
				
				sCount=nn;
				if(!sCount)goto TraceIsDone;
				
				qsort((char *)nCell,sCount, sizeof(struct cell),intcmp1);
				
				if(sCount > maxstep)maxstep=sCount;
				
				
				{
					int RayDirection;
					
					if(r.z < 0){			/* in from +z */
						if(r.x < 0){		/* in from +x */
							if(r.y < 0){		/* in from +y */
								RayDirection = 7;
							}else{                  /* in from -y */
								RayDirection = 5;
							}
						}else{                      /* in from -x */
							if(r.y < 0){		/* in from +y */
								RayDirection = 6;
							}else{                  /* in from -y */
								RayDirection = 4;
							}
						}
					}else{                           /* in from -z */
						if(r.x < 0){		/* in from +x */
							if(r.y < 0){		/* in from +y */
								RayDirection = 3;
							}else{                  /* in from -y */
								RayDirection = 1;
							}
						}else{                      /* in from -x */
							if(r.y < 0){		/* in from +y */
								RayDirection = 2;
							}else{                  /* in from -y */
								RayDirection = 0;
							}
						}
					}
					
					
					ray.RayDirection = RayDirection;
				}
				
				ray.no=no;
				ray.nc=nc;
				ray.r=r;
				
				for(n=0;n<sCount;++n){
					
					ne=nCell[n].ne;
					
					
					if(nCellEnter[ne])continue;
					
					nCellEnter[ne] = 1;
					
					ne=location[ne];
					
					SageTraceCellDepth(ne,&ray);
					
					if(opacity[no] >= 1.0){
						goto TraceIsDone;
					}
				}
			TraceIsDone:
				if(opacity[no] < 1.0){
					cdata[nc]   +=(BackGround.red*(1.0-opacity[no]));
					cdata[nc+1] +=(BackGround.green*(1.0-opacity[no]));
					cdata[nc+2] +=(BackGround.blue*(1.0-opacity[no]));
				}
			}
			
	    }
	}
	
	
	if(scene->debug > 1){
	    sprintf(WarningBuff,"SageTraceLines4a smin %ld smax %ld maxstep %ld\n",
	            Threads->smin,Threads->smax,maxstep);
		WarningBatch(WarningBuff);
	}
ErrorOut:
	if(nCell)cFree((char *)nCell);
	if(nCellEnter)cFree((char *)nCellEnter);
	
	Threads->done=TRUE;
	
	return 0;
}
static int SageTraceCellDepth(long ne,struct rayData *rayin)
{
	register struct rayData *ray;
	static int order[] = {
		0,1,2,3,  4,5,6,7,
		1,0,3,2,  5,4,7,6,
		2,0,3,1,  6,7,4,5,
		3,1,2,0,  7,5,6,4,
		
		4,5,6,7,  0,1,2,3,
		5,4,7,6,  1,0,3,2,
		6,4,7,5,  2,3,0,1,
		7,5,6,4,  3,1,2,0
	};
	
	int k;
	double nx,ny,nz;
	double mopacity;
	double cellDepth;
	double value;
	double den;
	double lx,ly,lz;
	double d;
	long nomat;
	int kc;
	
	if(!rayin)return 1;
	ray=rayin;
	
	if(ray->opacity[ray->no] >= 1.0)return 0;
	
	if(!ray->hasData[ne])return 0;
	
	if(!ray->daughter[ne])goto ProcessRay;
	
	{
		char dd[8];
		
		if(!SageCellHitDaughter(ne,ray,dd))return 0;
		
	    for(k=0;k<8;++k){
	        long nd;
	        nd=order[k+8*ray->RayDirection]+(long)ray->daughter[ne]-1;
	        if(!ray->hasData[nd])continue;
	        if(dd[order[k+8*ray->RayDirection]]){
	            SageTraceCellDepth(nd,ray);
	        }
	    }
		
	}
	
	
	return 0;
	
ProcessRay:
	
	cellDepth=cellDepthRay(ne,ray);

	if(ray->mean[ne]){
	    ray->opacityToFireball[ray->no] += cellDepth/ray->mean[ne];
	}
	
	if(ray->opacityToFireball[ray->no] < 1.0){
		return 0;
	}
	
	d=ray->data[ne];
	if(d < ray->dmin || d > ray->dmax)return 0;
	
	nomat=(long)((d-ray->dmin)*ray->mat_scale);
	if(nomat < 0){
        nomat=0;
	}else if(nomat >= ray->samples){
        nomat = ray->samples-1;
	}
	
	mopacity=ray->mat_opacity[nomat];
	
	if(mopacity <= 0.0)return 0;
	
	
	mopacity *= cellDepth;
	
	if(mopacity <= 0.0)return 0;
	
    nx=ray->gradx[ne];
    ny=ray->grady[ne];
    nz=ray->gradz[ne];
	
	den=sqrt(nx*nx+ny*ny+nz*nz);
	if(den){
		nx /= den;
		ny /= den;
		nz /= den;
		
		if((ray->r.x*nx+ray->r.y*ny+ray->r.z*nz) > 0.0){    /* flip nornmal */
		    nx = -nx;
		    ny = -ny;
		    nz = -nz;
		}
		
		lx=(ray->Lamp.x-ray->xcenter[ne]);
		ly=(ray->Lamp.y-ray->ycenter[ne]);
		lz=(ray->Lamp.z-ray->zcenter[ne]);
		den=sqrt(lx*lx+ly*ly+lz*lz);
		if(den){
			lx /= den;
			ly /= den;
			lz /= den;
			value=nx*lx+ny*ly+nz*lz;
		}else{
			value=1.0;
		}
		if(value < 0)value = 0;
	}else{
    	value=1.0;
    }
	
	if(ray->logscale){
	    kc=(int)(ray->a*log10(d)+ray->b);
	}else{
	    kc=(int)(ray->a*d+ray->b);
	}
	
	
	if((mopacity+ray->opacity[ray->no]) > 1.0){
        mopacity = (1.0 - ray->opacity[ray->no])+1.0e-5;
	}
	
	
	ray->cdata[ray->nc]   += (ray->red[kc]*value*mopacity);
	ray->cdata[ray->nc+1] += (ray->green[kc]*value*mopacity);
	ray->cdata[ray->nc+2] += (ray->blue[kc]*value*mopacity);
	
	ray->opacity[ray->no] += mopacity;
	if(ray->opacity[ray->no] >= 1.0)return 0;
	return 0;
	
}
