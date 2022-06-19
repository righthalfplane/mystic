#define EXTERN22 extern
#include "Xappl.h"
#include "uLib.h"
#include "uDialog.h"
#include "ScrollList.h"
#include "BatchFile.h"

#include "Sagevol.h"

int SageTraceLines4a(mThread *Threads);

int SageTraceCellDepth(long ne,struct rayData *rayin);

int SageTraceLines4b(mThread *Threads);

int getUniversalPaletteByName(char *name,unsigned char **pal);

double cellDepthRay(long ne,struct rayData *rayin);

int SageTraceLines5(mThread *Threads);

int SageDepthReflect5(long ne,struct rayData *rayin);

double cellDepthRayReflect5(long ne,struct rayData *rayin);


int getPalDouble(char *name,double *rr,double *gg,double *bb);

static int doAnaglyph(long xsize,long ysize,double *left,double *right);

int TraceLinesDAA2(mThread *Threads);
int TraceLinesDAA(mThread *Threads);
int FindTraceOrder(vIconPtr myIcon);
double cellDepthRayReflect(long ne,struct rayData *rayin);
int SageHitDaughterReflect(long ne,struct rayData *rayin,char *daughter);
int SageDepthReflect(long ne,struct rayData *rayin);


int doTrace(FilePtr Files)
{
	struct vScene *scene;
	vIconPtr myIcon;
	struct P EyeRight;
	double *vbuff;
	struct P Eye;
	long length;
	int n,np;
	int ret;

	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;

	ret =1;

	scene->start=rtime();

	Eye=scene->Eye;
	EyeRight=scene->EyeRight;

	np=1;
	vbuff=NULL;
	
	if(scene->EyeView == Scene_Anaglyph){
	    np=2;
		scene->Eye=scene->EyeLeft;
	}else if(scene->EyeView == Scene_EyeLeft){
		scene->Eye=scene->EyeLeft;
	}else if(scene->EyeView == Scene_EyeRight){
		scene->Eye=scene->EyeRight;
	}
	
	for(n=0;n<np;++n){

		if(runThreads(myIcon->threads,myIcon,scene->yResolution,TraceLines))goto ErrorOut;
	
		scene->Eye=EyeRight;

		if(!vbuff){
		    vbuff=scene->cdata;		
			if(np > 1){
				length=scene->xResolution*scene->yResolution;
				scene->cdata=(double *)cMalloc(3*length*sizeof(double),1934);
				if(!scene->cdata)goto ErrorOut;
				zerol((char *)scene->cdata,3*length*sizeof(double));		
				zerol((char *)scene->opacity,length*sizeof(double));
				zerol((char *)scene->opacityToFireball,length*sizeof(double));
			}
		}
	
	}

	if(scene->EyeView == Scene_Anaglyph){
	    doAnaglyph(scene->xResolution,scene->yResolution,vbuff,scene->cdata);
	    if(vbuff)cFree((char *)vbuff);
	}

	writeTRUE(Files);

	scene->Eye=Eye;

	ret=0;
ErrorOut:
	
	return ret;
}
int TraceSetUp(vIconPtr myIcon)
{
	struct vScene *scene;
	double xmin,xmax,ymin,ymax,delx,dely,odx,ody;
	long length;
	int ret;


	if(!myIcon)return 1;
	scene=&myIcon->scene;

	ret=1;

	SetScreen3(scene);

    ymax = (double)(scene->Height*tan((scene->Degrees*4.0*atan(1.0))/360.));
	ymin = -ymax;

    xmax = (double)(ymax*scene->AspectRatio*(double)(scene->xResolution)/(double)(scene->yResolution));
	xmin = - xmax;

	delx=xmax-xmin;
	dely=ymax-ymin;

	odx=(double)(scene->xResolution)/delx;	
	ody=(double)(scene->yResolution)/dely;	
	
	scene->delx=delx;
	scene->dely=dely;
	
	scene->odx=odx;
	scene->ody=ody;
	
	scene->dx=1.0/odx;
	scene->dy=1.0/ody;
	
	scene->xmin=xmin;
	scene->ymin=ymin;

	scene->xmax=xmax;
	scene->ymax=ymax;

	length=scene->xResolution*scene->yResolution;

	if(scene->cdata){
	    cFree((char *)scene->cdata);
	    scene->cdata=NULL;
	}

	if(scene->opacity){
	    cFree((char *)scene->opacity);
	    scene->opacity=NULL;
	}
	
	if(scene->opacityToFireball){
	    cFree((char *)scene->opacityToFireball);
	    scene->opacityToFireball=NULL;
	}
	
	if(scene->mat_opacity){
	    cFree((char *)scene->mat_opacity);
	    scene->mat_opacity=NULL;
	}


	scene->cdata=(double *)cMalloc(3*length*sizeof(double),1934);
	scene->opacity=(double *)cMalloc(length*sizeof(double),1935);
	scene->mat_opacity=(double *)cMalloc(scene->samples*sizeof(double),1936);
	scene->opacityToFireball=(double *)cMalloc(length*sizeof(double),1937);

	if(!scene->cdata || !scene->opacity || !scene->mat_opacity || !scene->opacityToFireball){
	    sprintf(WarningBuff,"TraceSetUp Out of Memeory Requested %ld\n",5*length*sizeof(double)+
	                   scene->yResolution*sizeof(int)+scene->samples*sizeof(double));
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	zerol((char *)scene->cdata,3*length*sizeof(double));
	zerol((char *)scene->opacity,length*sizeof(double));
	zerol((char *)scene->opacityToFireball,length*sizeof(double));
	zerol((char *)scene->mat_opacity,scene->samples*sizeof(double));

	{
	    double d;
	    int n;

	    for(n=0;n<scene->samples;++n){
	        d=myIcon->dmin+(double)n*(myIcon->dmax-myIcon->dmin)/(double)(scene->samples-1);
			scene->mat_opacity[n]=interpolate(d,scene->tcount,scene->tvalue,scene->topacity);
	    }
	}
	
	if(uPref.BlockSizeGlobal > 0){
	/*
		if(TraceFindLines(myIcon))goto ErrorOut;
	*/
	}


	ret=0;
ErrorOut:

	return ret;
}
double interpolate(double d,long count,double *x,double *y)
{
	double value,den;
	long n;

	if((count <= 0) || !x || !y)return -1.0;

	if(d <= x[0])return y[0];
	if(d >= x[count-1])return y[count-1];
	if(count == 1)return y[0];

	for(n=1;n<count;++n){
	    if(d >= x[n-1] && d <= x[n]){
	        den=(x[n]-x[n-1]);
	        if(!den)den=1.0;
	        value=(y[n-1]+(y[n]-y[n-1])*(d-x[n-1])/den);
	        return value;
	    }
	}


	return y[count-1];
}
int TraceLines(mThread *Threads)
{
	vIconPtr myIcon;
	struct vScene *scene;
	double red[256],green[256],blue[256];
	struct Color BackGround;
	struct P s,r,Eye;
	struct P Lamp;
	double *cdata,*data,*opacity,*mat_opacity;
	double mat_scale;
	double xmin,xmax;
	double ymin,ymax;
	double zmin,zmax;
	double dmin,dmax;
	double a,b,alpha;
	double xx,yy,zz;
	double s1,s2;
	double didx,djdy,dkdz;
	double dxdi,dydj,dzdk;
	double diagonal;
	long ixmax,iymax,izmax,iyxmax;
	long ixmaxNoReflect,iymaxNoReflect,izmaxNoReflect,iyxmaxNoReflect;
	long ystep,ostep;
	long samples;
	long maxstep;
	long i,j,nc,no;
	long nemax;
	long ierror;
	int ihit;
	int xReflect,yReflect,zReflect;
	long iBailed;
	static double small2=1.0e-6;
	double aa,bb;
	int logscale;


	if(!Threads)return 1;
	myIcon=(vIconPtr)Threads->data;
	if(!myIcon)return 1;
	scene=&myIcon->scene;


	cdata=scene->cdata;
	if(!cdata)return 1;
	data=myIcon->data;
	if(!data)return 1;
	opacity=scene->opacity;
	if(!opacity)return 1;
	mat_opacity=scene->mat_opacity;
	if(!mat_opacity)return 1;
	samples=scene->samples;


	iBailed=0;

	getPalDouble(scene->palname,red,green,blue);

	Lamp=scene->Lamp;

	xReflect=scene->xReflect;
	yReflect=scene->yReflect;
	zReflect=scene->zReflect;



	xmin=myIcon->xmin;
	xmax=myIcon->xmax;

	ixmaxNoReflect=myIcon->ixmax;
	if(xReflect){
	    ixmax=2*ixmaxNoReflect;
	    xmin = -xmax;
	}else{
	    ixmax=ixmaxNoReflect;
	}

	didx=(double)(ixmax)/(xmax-xmin);
	dxdi=1.0/didx;



	ymin=myIcon->ymin;
	ymax=myIcon->ymax;


	iymaxNoReflect=myIcon->iymax;
	if(yReflect){
	    iymax=2*iymaxNoReflect;
	    ymin = -ymax;
	}else{
	    iymax=iymaxNoReflect;
	}

	djdy=(double)(iymax)/(ymax-ymin);
	dydj=1.0/djdy;

	zmin=myIcon->zmin;
	zmax=myIcon->zmax;

	izmaxNoReflect=myIcon->izmax;
	if(zReflect){
	    izmax=2*izmaxNoReflect;
	    zmin = -zmax;
	}else{
	    izmax=izmaxNoReflect;
	}



	dkdz=(double)(izmax)/(zmax-zmin);
	dzdk=1.0/dkdz;

	diagonal=1.7/sqrt((dxdi)*(dxdi)+(dydj)*(dydj)+(dydj)*(dydj));

	iyxmax=ixmax*iymax;
	iyxmaxNoReflect=ixmaxNoReflect*iymaxNoReflect;

	Eye=scene->Eye;

	BackGround=scene->BackGround;

	ystep=3*scene->xResolution;
	ostep=scene->xResolution;

	dmin = myIcon->dmin;
	dmax = myIcon->dmax;
		
    logscale=myIcon->logscale;
    if(logscale){
        if((dmin > 0) && (dmax > 0)){
	        aa=(254.0-2.0)/(log10(dmax)-log10(dmin));
	        bb=2.0-aa*log10(dmin);
	    }else{
	       logscale = 0;
	       aa=(254.0-2.0)/(dmax-dmin);
	       bb=2.0-dmin;
	       if((Threads->smin == 0)){
	           sprintf(WarningBuff,"dmin or dmax less than zero log scale turnned off\n");
				if(scene->debug > 1)WarningBatch(WarningBuff);
	       }
	    }
    }else{
	    aa=(254.0-2.0)/(dmax-dmin);
	    bb=2.0-aa*dmin;
    }
	
	mat_scale=(((double)samples-1.0)/(dmax-dmin));

	maxstep=0;
	ierror = 0;
	nemax=(ixmax*iymax*izmax);

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
	    /* if(j < 304 || j > 305)continue; */
	    for(i=0;i<scene->xResolution;++i){
	        double ssx[2000],ssy[2000],ssz[2000];
	        int nssx,nssy,nssz;
	        long nnn;
			int nnx,nny,nnz;
	        double ox;
	        double oy;
	        double oz;
			double ss;

	        /* if(i < 416 || i > 417)continue; */

	        a=scene->xmin+((double)i+.5)*scene->dx;
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

		no=i+j*ostep;

		{
		    double xs,ys,zs;
		    long nl,ne;
		    int n;
		    int x, y, z;

		    if((s1 == s2) || (s1 < 0))s1 = 0.0;

		    s1 -= small2;
		    s2 += small2;

	        nssx=0;
		    if(r.x){
		        xx=xmin-dxdi;
		        for(n=0;n<=ixmax;++n){
		            xx += dxdi;
				    ss=(xx-Eye.x)*ox;
				    if(ss >= s1 && ss <= s2){
				        ssx[nssx++]=ss;
				    }
		        }



		        if(nssx > 1 && (ssx[0] > ssx[nssx-1])){
		            for(n=0;n<nssx/2;++n){
		                ss=ssx[n];
		                ssx[n]=ssx[nssx-1-n];
						ssx[nssx-1-n]=ss;
		            }
		        }
		    }

	        nssy=0;
		    if(r.y){
		        yy=ymin-dydj;
		        for(n=0;n<=iymax;++n){
		            yy += dydj;
				    ss=(yy-Eye.y)*oy;
				    if(ss >= s1 && ss <= s2){
				        ssy[nssy++]=ss;
				    }
		        }



		        if(nssy > 1 && (ssy[0] > ssy[nssy-1])){
		            for(n=0;n<nssy/2;++n){
		                ss=ssy[n];
		                ssy[n]=ssy[nssy-1-n];
						ssy[nssy-1-n]=ss;
		            }
		        }
		    }

	        nssz=0;
		    if(r.z){
		        zz=zmin-dzdk;
		        for(n=0;n<=izmax;++n){
		            zz += dzdk;
				    ss=(zz-Eye.z)*oz;
				    if(ss >= s1 && ss <= s2){
				        ssz[nssz++]=ss;
				    }
		        }


		        if(nssz > 1 && (ssz[0] > ssz[nssz-1])){
		            for(n=0;n<nssz/2;++n){
		                ss=ssz[n];
		                ssz[n]=ssz[nssz-1-n];
						ssz[nssz-1-n]=ss;
		            }
		        }
		    }

		    



		    nl = -1;

		    nnx=0;
		    nny=0;
		    nnz=0;
		    nnn=0;


		    while(1){
		        double nx,ny,nz,den;
		        double dnx,dny,dnz;
		        double lx,ly,lz;
		        double value,mopacity;
		        double depth;
		        double d;
		        int kc;
		        int xe,ye,ze;

			if(nnx < nssx){
			    if(nny < nssy){
			        if(ssx[nnx] < ssy[nny]){
			            if(nnz < nssz){
			                if(ssx[nnx] < ssz[nnz]){
			                    ss = ssx[nnx++];
			                }else{
			                    ss = ssz[nnz++];
			                }
			            }else{
			                ss = ssx[nnx++];
			            }
			        }else{
			            if(nnz < nssz){
			                if(ssy[nny] < ssz[nnz]){
			                    ss = ssy[nny++];
			                }else{
			                    ss = ssz[nnz++];
			                }
			            }else{
			                ss = ssy[nny++];
			            }
			        }
			    }else if(nnz < nssz){
		                if(ssx[nnx] < ssz[nnz]){
		                    ss = ssx[nnx++];
		                }else{
		                    ss = ssz[nnz++];
		                }
			    }else{
		                ss = ssx[nnx++];
			    }
			}else if(nny < nssy){
		            if(nnz < nssz){
		                if(ssy[nny] < ssz[nnz]){
		                    ss = ssy[nny++];
		                }else{
		                    ss = ssz[nnz++];
		                }
		            }else{
		                ss = ssy[nny++];
		            }
			}else if(nnz < nssz){
		            ss = ssz[nnz++];
			}else{
			    break;
			}

			++nnn;


	        xs=Eye.x+(ss+small2)*r.x;
	        ys=Eye.y+(ss+small2)*r.y;
	        zs=Eye.z+(ss+small2)*r.z;

	        x=(int)((xs-xmin)*didx);
	        if(x >= ixmax)x = (int)(ixmax - 1);
	        y=(int)((ys-ymin)*djdy);
	        if(y >= iymax)y = (int)(iymax - 1);
	        z=(int)((zs-zmin)*dkdz);
	        if(z >= izmax)z = (int)(izmax - 1);

	        ne=x+y*ixmax+z*iyxmax;
	        if(ne == nl)goto UpdateVariables;
	        nl=ne;

			if(xReflect){
			    if(x < ixmaxNoReflect){
			        xe=(int)ixmaxNoReflect-x-1;
			        dnx = -1.0;
			    }else{
			        xe=x-(int)ixmaxNoReflect;
			        dnx = 1.0;
			    }
			}else{
			    xe=x;
			    dnx = 1.0;
			}

			if(yReflect){
			    if(y < iymaxNoReflect){
			        ye=(int)iymaxNoReflect-y-1;
			        dny = -1.0;
			    }else{
			        ye=y-(int)iymaxNoReflect;
			        dny = 1.0;
			    }
			}else{
			    ye=y;
			    dny = 1.0;
			}

			if(zReflect){
			    if(z < izmaxNoReflect){
			        ze=(int)izmaxNoReflect-z-1;
			        dnz = -1.0;
			    }else{
			        ze=z-(int)izmaxNoReflect;
			        dnz = 1.0;
			    }
			}else{
			    ze=z;
			    dnz = 1.0;
			}

		    ne=xe+ye*ixmaxNoReflect+ze*iyxmaxNoReflect;

			if(ne < 0 || ne >= nemax){
			    if(++ierror < 10){
			        sprintf(WarningBuff,"ERROR : ne %ld x %d y %d z %d\n",ne,x,y,z);
					if(scene->debug > 1)WarningBatch(WarningBuff);
			    }
			    goto UpdateVariables;
			}

			d=data[ne];
			if(d < dmin || d > dmax)goto UpdateVariables;
/*
			    nomat=(int)((d-dmin)*mat_scale);
			    if(nomat < 0){
			        nomat=0;
			    }else if(nomat >= samples){
			       nomat = (int)(samples-1);
			    }

			    mopacity=mat_opacity[nomat];
*/
				
			    mopacity=interpolate(d,scene->tcount,scene->tvalue,scene->topacity);
				
			    if(mopacity <= 0.0)goto UpdateVariables;


		        {
			    double xn,xm,yn,ym,zn,zm;
			

			    xn=x*dxdi+xmin;
			    xm=xn+dxdi;

			    yn=y*dydj+ymin;
			    ym=yn+dydj;

			    zn=z*dzdk+zmin;
			    zm=zn+dzdk;

			    s1=1e33;
			    s2=-1;

			    ihit = 0;
				if(r.x){
				    alpha=(xn-Eye.x)*ox;
				    if(alpha >= 0){
				        yy=Eye.y+alpha*r.y;
				        zz=Eye.z+alpha*r.z;
				        if(yy >= yn && yy <= ym && zz >= zn && zz <= zm){
				            if(alpha < s1 )s1 = alpha;
				            if(alpha > s2 )s2 = alpha;
					    	ihit=1;
				        }
				    }
				    alpha=(xm-Eye.x)*ox;
				    if(alpha >= 0){
				        yy=Eye.y+alpha*r.y;
				        zz=Eye.z+alpha*r.z;
				        if(yy >= yn && yy <= ym && zz >= zn && zz <= zm){
				            if(alpha < s1 )s1 = alpha;
				            if(alpha > s2 )s2 = alpha;
					    	ihit=1;
				        }
				    }
				}
				if(r.y){
				    alpha=(yn-Eye.y)*oy;
				    if(alpha >= 0){
				        xx=Eye.x+alpha*r.x;
				        zz=Eye.z+alpha*r.z;
				        if(xx >= xn && xx <= xm && zz >= zn && zz <= zm){
				            if(alpha < s1 )s1 = alpha;
				            if(alpha > s2 )s2 = alpha;
					    	ihit=1;
				        }
				    }
				    alpha=(ym-Eye.y)*oy;
				    if(alpha >= 0){
				        xx=Eye.x+alpha*r.x;
				        zz=Eye.z+alpha*r.z;
				        if(xx >= xn && xx <= xm && zz >= zn && zz <= zm){
				            if(alpha < s1 )s1 = alpha;
				            if(alpha > s2 )s2 = alpha;
					    	ihit=1;
				        }
				    }
				}

				if(r.z){
				    alpha=(zn-Eye.z)*oz;
				    if(alpha >= 0){
				        xx=Eye.x+alpha*r.x;
				        yy=Eye.y+alpha*r.y;
				        if(xx >= xn && xx <= xm && yy >= yn && yy <= ym){
				            if(alpha < s1 )s1 = alpha;
				            if(alpha > s2 )s2 = alpha;
					    	ihit=1;
				        }
				    }
				    alpha=(zm-Eye.z)*oz;
				    if(alpha >= 0){
				        xx=Eye.x+alpha*r.x;
				        yy=Eye.y+alpha*r.y;
				        if(xx >= xn && xx <= xm && yy >= yn && yy <= ym){
				            if(alpha < s1 )s1 = alpha;
				            if(alpha > s2 )s2 = alpha;
					    	ihit=1;
				        }
				    }
				}


				if(ihit){
				    double xss,yss,zss,xee,yee,zee;

				    if(s1 == s2)s1 = 0.;

				    xss=Eye.x+s1*r.x;
				    yss=Eye.y+s1*r.y;
				    zss=Eye.z+s1*r.z;

				    xee=Eye.x+s2*r.x;
				    yee=Eye.y+s2*r.y;
				    zee=Eye.z+s2*r.z;

				    depth=sqrt((xee-xss)*(xee-xss)+(yee-yss)*(yee-yss)+(zee-zss)*(zee-zss))*diagonal;

				}else{
				    depth=0;
				    ++iBailed;
				    goto UpdateVariables;
				}



		        }
			    mopacity = mopacity*depth;

	            if(xe >= ixmaxNoReflect-1){
	                nx=(.5*(-data[ne-1]));
	            }else if(xe == 0){
	                nx=(.5*data[ne+1]);
	            }else{
	                nx=(.5*((data[ne+1])-(data[ne-1])));
	            }

		    	nx *= dnx;

	            if(ye >= iymaxNoReflect-1){
	                ny=(.5*(-data[ne-ixmaxNoReflect]));
	            }else if(ye == 0){
	                ny=(.5*data[ne+ixmaxNoReflect]);
	            }else{
	                ny=(.5*(data[ne+ixmaxNoReflect]-data[ne-ixmaxNoReflect]));
	            }

		    	ny *= dny;

	            if(ze >= izmaxNoReflect-1){
	                nz=(.5*(-data[ne-iyxmaxNoReflect]));
	            }else if(ze == 0){
	                nz=(.5*(data[ne+iyxmaxNoReflect]));
	            }else{
	                nz=(0.5*(data[ne+iyxmaxNoReflect]-data[ne-iyxmaxNoReflect]));
	            }

		    	nz *= dnz;

			    den=sqrt(nx*nx+ny*ny+nz*nz);
			    if(den){
			         nx /= den;
			         ny /= den;
			         nz /= den;

			        if((r.x*nx+r.y*ny+r.z*nz) > 0.0){    /* flip nornmal */
			            nx = -nx;
			            ny = -ny;
			            nz = -nz;
			        }

			        xx=xmin+(x+0.5)*dxdi;
			        yy=ymin+(y+0.5)*dydj;
			        zz=zmin+(z+0.5)*dzdk;

			        lx=(Lamp.x-xx);
			        ly=(Lamp.y-yy);
			        lz=(Lamp.z-zz);
			        den=sqrt(lx*lx+ly*ly+lz*lz);
			        if(den){
			            lx /= den;
			            ly /= den;
			            lz /= den;
			            value=nx*lx+ny*ly+nz*lz;
			        }else{
			             value=1.0;
			        }
			        if(value < 0)value = .25;
			    }else{
			        value=1.0;
			    }


			    /* kc=(int)(1+(d-dmin)*252.0/(dmax-dmin)); */
				if(logscale){
				    kc=(int)(aa*log10(d)+bb);
				}else{
				    kc=(int)(aa*d+bb);
				}


			    if((mopacity+opacity[no]) > 1.0){
			        mopacity = (1.0 - opacity[no])+1.0e-5;
			    }

	            cdata[nc]   += (red[kc]*value*mopacity);
	            cdata[nc+1] += (green[kc]*value*mopacity);
	            cdata[nc+2] += (blue[kc]*value*mopacity);


	            opacity[no] += mopacity;
	            if(opacity[no] >= 1.0){

	                opacity[no] = 1.0;
	                break;
			    }
UpdateVariables:        ;

		    }
		    if(nnn > maxstep)maxstep=nnn;
		    if(opacity[no] < 1.0){
		        cdata[nc]   += (BackGround.red*(1.0-opacity[no]));
		        cdata[nc+1] += (BackGround.green*(1.0-opacity[no]));
		        cdata[nc+2] += (BackGround.blue*(1.0-opacity[no]));
		    }
		}

	    }
	}


	if(iBailed > 10){
	    sprintf(WarningBuff,"iBailed %ld\n",iBailed);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}

	if(scene->debug > 1){
	    sprintf(WarningBuff,"TraceLines smin %ld smax %ld maxstep %ld\n",Threads->smin,Threads->smax,maxstep);
		WarningBatch(WarningBuff);
	}
	Threads->done=TRUE;

	return 0;
}
int TraceLinesDAA2(mThread *Threads)
{
	struct vScene *scene;
	vIconPtr myIcon;
	double red[256],green[256],blue[256];
	struct Color BackGround;
	struct P s,r,Eye;
	struct P Lamp;
	double *cdata,*data,*opacity,*mat_opacity;
	double mat_scale;
	double xmin,xmax;
	double ymin,ymax;
	double zmin,zmax;
	double dmin,dmax;
	double a,b,alpha;
	double xx,yy,zz;
	double s1,s2;
	double didx,djdy,dkdz;
	double dxdi,dydj,dzdk;
	double diagonal;
	long ixmax,iymax,izmax,iyxmax;
	long ixmaxNoReflect,iymaxNoReflect,izmaxNoReflect,iyxmaxNoReflect;
	long ystep,ostep;
	long samples;
	long maxstep;
	long i,j,nc,no;
	long nemax;
	long ierror;
	int ihit;
	int xReflect,yReflect,zReflect;
	long iBailed;


	if(!Threads)return 1;
	myIcon=(vIconPtr)Threads->data;
	if(!myIcon)return 1;
	scene=&myIcon->scene;


	cdata=scene->cdata;
	if(!cdata)return 1;
	data=myIcon->data;
	if(!data)return 1;
	opacity=scene->opacity;
	if(!opacity)return 1;
	mat_opacity=scene->mat_opacity;
	if(!mat_opacity)return 1;
	samples=scene->samples;


	iBailed=0;

	getPalDouble(scene->palname,red,green,blue);

	Lamp=scene->Lamp;

	xReflect=scene->xReflect;
	yReflect=scene->yReflect;
	zReflect=scene->zReflect;



	xmin=myIcon->xmin;
	xmax=myIcon->xmax;

	ixmaxNoReflect=myIcon->ixmax;
	if(xReflect){
	    ixmax=2*ixmaxNoReflect;
	    xmin = -xmax;
	}else{
	    ixmax=ixmaxNoReflect;
	}

	didx=(double)(ixmax)/(xmax-xmin);
	dxdi=1.0/didx;



	ymin=myIcon->ymin;
	ymax=myIcon->ymax;


	iymaxNoReflect=myIcon->iymax;
	if(yReflect){
	    iymax=2*iymaxNoReflect;
	    ymin = -ymax;
	}else{
	    iymax=iymaxNoReflect;
	}

	djdy=(double)(iymax)/(ymax-ymin);
	dydj=1.0/djdy;

	zmin=myIcon->zmin;
	zmax=myIcon->zmax;

	izmaxNoReflect=myIcon->izmax;
	if(zReflect){
	    izmax=2*izmaxNoReflect;
	    zmin = -zmax;
	}else{
	    izmax=izmaxNoReflect;
	}



	dkdz=(double)(izmax)/(zmax-zmin);
	dzdk=1.0/dkdz;

	diagonal=1.0/sqrt((dxdi)*(dxdi)+(dydj)*(dydj)+(dydj)*(dydj));

	iyxmax=ixmax*iymax;
	iyxmaxNoReflect=ixmaxNoReflect*iymaxNoReflect;


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


	for(j=Threads->smin;j<Threads->smax;++j){
	    b=scene->ymin+((double)j+.5)*scene->dy;
	    /* if(j < 450 || j > 450)continue; */
	    for(i=0;i<scene->xResolution;++i){
	        /* if(i < 299 || i > 302)continue; */
	        a=scene->xmin+((double)i+.5)*scene->dx;
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
		    alpha=(xmin-Eye.x)/r.x;
		    if(alpha >= 0){
		        yy=Eye.y+alpha*r.y;
		        zz=Eye.z+alpha*r.z;
		        if(yy >= ymin && yy <= ymax && zz >= zmin && zz <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		    alpha=(xmax-Eye.x)/r.x;
		    if(alpha >= 0){
		        yy=Eye.y+alpha*r.y;
		        zz=Eye.z+alpha*r.z;
		        if(yy >= ymin && yy <= ymax && zz >= zmin && zz <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		}
		if(r.y){
		    alpha=(ymin-Eye.y)/r.y;
		    if(alpha >= 0){
		        xx=Eye.x+alpha*r.x;
		        zz=Eye.z+alpha*r.z;
		        if(xx >= xmin && xx <= xmax && zz >= zmin && zz <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		    alpha=(ymax-Eye.y)/r.y;
		    if(alpha >= 0){
		        xx=Eye.x+alpha*r.x;
		        zz=Eye.z+alpha*r.z;
		        if(xx >= xmin && xx <= xmax && zz >= zmin && zz <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		}

		if(r.z){
		    alpha=(zmin-Eye.z)/r.z;
		    if(alpha >= 0){
		        xx=Eye.x+alpha*r.x;
		        yy=Eye.y+alpha*r.y;
		        if(xx >= xmin && xx <= xmax && yy >= ymin && yy <= ymax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		    alpha=(zmax-Eye.z)/r.z;
		    if(alpha >= 0){
		        xx=Eye.x+alpha*r.x;
		        yy=Eye.y+alpha*r.y;
		        if(xx >= xmin && xx <= xmax && yy >= ymin && yy <= ymax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		}
		nc=3*i+ystep*j;
		if(!ihit){
		    cdata[nc]=BackGround.red;
		    cdata[nc+1]=BackGround.green;
		    cdata[nc+2]=BackGround.blue;
		    continue;
		}

		no=i+j*ostep;

		{
		    double xs,ys,zs,xe,ye,ze;
		    int x2,y2,z2;
		    long nl,ne;
		    int n, sx, sy, sz, exy, exz, ezy,
		           ax, ay, az, bx, by, bz;
		    int x, y, z, dx, dy, dz;

		    if((s1 == s2) || (s1 < 0))s1 = 0.0;

		    xs=Eye.x+s1*r.x;
		    ys=Eye.y+s1*r.y;
		    zs=Eye.z+s1*r.z;

		    xe=Eye.x+s2*r.x;
		    ye=Eye.y+s2*r.y;
		    ze=Eye.z+s2*r.z;

		    x=(int)((xs-xmin)*didx);
		    if(x >= ixmax)x = (int)(ixmax - 1);
		    y=(int)((ys-ymin)*djdy);
		    if(y >= iymax)y = (int)(iymax - 1);
		    z=(int)((zs-zmin)*dkdz);
		    if(z >= izmax)z = (int)(izmax - 1);

		    x2=(int)((xe-xmin)*didx);
		    if(x2 >= ixmax)x2 = (int)(ixmax - 1);
		    y2=(int)((ye-ymin)*djdy);
		    if(y2 >= iymax)y2 = (int)(iymax - 1);
		    z2=(int)((ze-zmin)*dkdz);
		    if(z2 >= izmax)z2 = (int)(izmax - 1);



		    dx=x2-x;
		    dy=y2-y;
		    dz=z2-z;

		    sx = sgn(dx);  sy = sgn(dy);  sz = sgn(dz);
		    ax = abs(dx);  ay = abs(dy);  az = abs(dz);
		    bx = 2*ax;     by = 2*ay;     bz = 2*az;
		    exy = ay-ax;   exz = az-ax;   ezy = ay-az;
		    n = ax+ay+az+1;

		    if(n > maxstep)maxstep=n;

		    nl = -1;
		    while(--n > 0){
		        double nx,ny,nz,den;
		        double dnx,dny,dnz;
		        double lx,ly,lz;
		        double value,mopacity;
		        double depth;
		        double d;
		        int kc,nomat;
		        int xe,ye,ze;


		        ne=x+y*ixmax+z*iyxmax;
		        if(ne == nl)goto UpdateVariables;
		        nl=ne;

			if(xReflect){
			    if(x < ixmaxNoReflect){
			        xe=(int)ixmaxNoReflect-x-1;
			        dnx = -1.0;
			    }else{
			        xe=x-(int)ixmaxNoReflect;
			        dnx = 1.0;
			    }
			}else{
			    xe=x;
			    dnx = 1.0;
			}

			if(yReflect){
			    if(y < iymaxNoReflect){
			        ye=(int)iymaxNoReflect-y-1;
			        dny = -1.0;
			    }else{
			        ye=y-(int)iymaxNoReflect;
			        dny = 1.0;
			    }
			}else{
			    ye=y;
			    dny = 1.0;
			}

			if(zReflect){
			    if(z < izmaxNoReflect){
			        ze=(int)izmaxNoReflect-z-1;
			        dnz = -1.0;
			    }else{
			        ze=z-(int)izmaxNoReflect;
			        dnz = 1.0;
			    }
			}else{
			    ze=z;
			    dnz = 1.0;
			}

		        ne=xe+ye*ixmaxNoReflect+ze*iyxmaxNoReflect;

			if(ne < 0 || ne >= nemax){
			    if(++ierror < 10){
			        sprintf(WarningBuff,"ERROR : ne %ld x %d y %d z %d\n",ne,x,y,z);
					if(scene->debug > 1)WarningBatch(WarningBuff);
			    }
			    goto UpdateVariables;
			}

			d=data[ne];
			if(d < dmin || d > dmax)goto UpdateVariables;

			    nomat=(int)((d-dmin)*mat_scale);
			    if(nomat < 0){
			        nomat=0;
			    }else if(nomat >= samples){
			       nomat = (int)(samples-1);
			    }

			    mopacity=mat_opacity[nomat];
			    if(mopacity <= 0.0)goto UpdateVariables;


		        {
			    double xn,xm,yn,ym,zn,zm;
			

			    xn=x*dxdi+xmin;
			    xm=xn+dxdi;

			    yn=y*dydj+ymin;
			    ym=yn+dydj;

			    zn=z*dzdk+zmin;
			    zm=zn+dzdk;

			    s1=1e33;
			    s2=-1;

			    ihit = 0;
				if(r.x){
				    alpha=(xn-Eye.x)/r.x;
				    if(alpha >= 0){
				        yy=Eye.y+alpha*r.y;
				        zz=Eye.z+alpha*r.z;
				        if(yy >= yn && yy <= ym && zz >= zn && zz <= zm){
				            if(alpha < s1 )s1 = alpha;
				            if(alpha > s2 )s2 = alpha;
					    ihit=1;
				        }
				    }
				    alpha=(xm-Eye.x)/r.x;
				    if(alpha >= 0){
				        yy=Eye.y+alpha*r.y;
				        zz=Eye.z+alpha*r.z;
				        if(yy >= yn && yy <= ym && zz >= zn && zz <= zm){
				            if(alpha < s1 )s1 = alpha;
				            if(alpha > s2 )s2 = alpha;
					    ihit=1;
				        }
				    }
				}
				if(r.y){
				    alpha=(yn-Eye.y)/r.y;
				    if(alpha >= 0){
				        xx=Eye.x+alpha*r.x;
				        zz=Eye.z+alpha*r.z;
				        if(xx >= xn && xx <= xm && zz >= zn && zz <= zm){
				            if(alpha < s1 )s1 = alpha;
				            if(alpha > s2 )s2 = alpha;
					    ihit=1;
				        }
				    }
				    alpha=(ym-Eye.y)/r.y;
				    if(alpha >= 0){
				        xx=Eye.x+alpha*r.x;
				        zz=Eye.z+alpha*r.z;
				        if(xx >= xn && xx <= xm && zz >= zn && zz <= zm){
				            if(alpha < s1 )s1 = alpha;
				            if(alpha > s2 )s2 = alpha;
					    ihit=1;
				        }
				    }
				}

				if(r.z){
				    alpha=(zn-Eye.z)/r.z;
				    if(alpha >= 0){
				        xx=Eye.x+alpha*r.x;
				        yy=Eye.y+alpha*r.y;
				        if(xx >= xn && xx <= xm && yy >= yn && yy <= ym){
				            if(alpha < s1 )s1 = alpha;
				            if(alpha > s2 )s2 = alpha;
					    ihit=1;
				        }
				    }
				    alpha=(zm-Eye.z)/r.z;
				    if(alpha >= 0){
				        xx=Eye.x+alpha*r.x;
				        yy=Eye.y+alpha*r.y;
				        if(xx >= xn && xx <= xm && yy >= yn && yy <= ym){
				            if(alpha < s1 )s1 = alpha;
				            if(alpha > s2 )s2 = alpha;
					    ihit=1;
				        }
				    }
				}


				if(ihit){
				    double xss,yss,zss,xee,yee,zee;
				    xss=Eye.x+s1*r.x;
				    yss=Eye.y+s1*r.y;
				    zss=Eye.z+s1*r.z;

				    xee=Eye.x+s2*r.x;
				    yee=Eye.y+s2*r.y;
				    zee=Eye.z+s2*r.z;

				    depth=sqrt((xee-xss)*(xee-xss)+(yee-yss)*(yee-yss)+(zee-zss)*(zee-zss))*diagonal;

				}else{
				    depth=0;
				    ++iBailed;
				    goto UpdateVariables;
				}



		        }


			     mopacity = mopacity*depth;

		
		            if(xe >= ixmaxNoReflect-1){
		                nx=(.5*(-data[ne-1]));
		            }else if(xe == 0){
		                nx=(.5*data[ne+1]);
		            }else{
		                nx=(.5*((data[ne+1])-(data[ne-1])));
		            }

			    nx *= dnx;

		            if(ye >= iymaxNoReflect-1){
		                ny=(.5*(-data[ne-ixmaxNoReflect]));
		            }else if(ye == 0){
		                ny=(.5*data[ne+ixmaxNoReflect]);
		            }else{
		                ny=(.5*(data[ne+ixmaxNoReflect]-data[ne-ixmaxNoReflect]));
		            }

			    ny *= dny;

		            if(ze >= izmaxNoReflect-1){
		                nz=(.5*(-data[ne-iyxmaxNoReflect]));
		            }else if(ze == 0){
		                nz=(.5*(data[ne+iyxmaxNoReflect]));
		            }else{
		                nz=(0.5*(data[ne+iyxmaxNoReflect]-data[ne-iyxmaxNoReflect]));
		            }

			    nz *= dnz;

			    den=sqrt(nx*nx+ny*ny+nz*nz);
			    if(den){
			         nx /= den;
			         ny /= den;
			         nz /= den;

			        if((r.x*nx+r.y*ny+r.z*nz) > 0.0){    /* flip nornmal */
			            nx = -nx;
			            ny = -ny;
			            nz = -nz;
			        }

			        xx=xmin+(x+0.5)*dxdi;
			        yy=ymin+(y+0.5)*dydj;
			        zz=zmin+(z+0.5)*dzdk;

			        lx=(Lamp.x-xx);
			        ly=(Lamp.y-yy);
			        lz=(Lamp.z-zz);
			        den=sqrt(lx*lx+ly*ly+lz*lz);
			        if(den){
			            lx /= den;
			            ly /= den;
			            lz /= den;
			            value=nx*lx+ny*ly+nz*lz;
			        }else{
			             value=1.0;
			        }
			        if(value < 0)value = .25;
			    }else{
			        value=1.0;
			    }


			    kc=(int)(1+(d-dmin)*252.0/(dmax-dmin));


			    if((mopacity+opacity[no]) > 1.0){
			        mopacity = (1.0 - opacity[no])+1.0e-5;
			    }

		            cdata[nc]   += (red[kc]*value*mopacity);
		            cdata[nc+1] += (green[kc]*value*mopacity);
		            cdata[nc+2] += (blue[kc]*value*mopacity);


		            opacity[no] += mopacity;
		            if(opacity[no] >= 1.0){

		                opacity[no] = 1.0;
		                break;
			    }
UpdateVariables:
		            if ( exy < 0 ) {
		                if ( exz < 0 ) {
		                    x += sx;
		                    exy += by; exz += bz;
		                }
		                else  {
		                    z += sz;
		                    exz -= bx; ezy += by;
		                }
		            }
		            else {
		                if ( ezy < 0 ) {
		                    z += sz;
		                    exz -= bx; ezy += by;
		                }
		                else  {
		                    y += sy;
		                    exy -= bx; ezy -= bz;
		                }
		            }
		    }
		    if(opacity[no] < 1.0){
		        cdata[nc]   += (BackGround.red*(1.0-opacity[no]));
		        cdata[nc+1] += (BackGround.green*(1.0-opacity[no]));
		        cdata[nc+2] += (BackGround.blue*(1.0-opacity[no]));
		    }
		}

	    }
	}


	    sprintf(WarningBuff,"iBailed %ld\n",iBailed);
		if(scene->debug > 1)WarningBatch(WarningBuff);


	if(scene->debug > 1){
		sprintf(WarningBuff,"TraceLinesDAA2 smin %ld smax %ld maxstep %ld\n",Threads->smin,Threads->smax,maxstep);
		WarningBatch(WarningBuff);
	}
	Threads->done=TRUE;

	return 0;
}
int TraceLinesDAA(mThread *Threads)
{
	struct vScene *scene;
	vIconPtr myIcon;
	double red[256],green[256],blue[256];
	struct Color BackGround;
	struct P s,r,Eye;
	struct P Lamp;
	double *cdata,*data,*opacity,*mat_opacity;
	double mat_scale;
	double xmin,xmax;
	double ymin,ymax;
	double zmin,zmax;
	double dmin,dmax;
	double a,b,alpha;
	double xx,yy,zz;
	double s1,s2;
	double didx,djdy,dkdz;
	double dxdi,dydj,dzdk;
	long ixmax,iymax,izmax,iyxmax;
	long ixmaxNoReflect,iymaxNoReflect,izmaxNoReflect,iyxmaxNoReflect;
	long ystep,ostep;
	long samples;
	long maxstep;
	long i,j,nc,no;
	long nemax;
	long ierror;
	int ihit;
	int xReflect,yReflect,zReflect;


	if(!Threads)return 1;
	myIcon=(vIconPtr)Threads->data;
	if(!myIcon)return 1;
	scene=&myIcon->scene;


	cdata=scene->cdata;
	if(!cdata)return 1;
	data=myIcon->data;
	if(!data)return 1;
	opacity=scene->opacity;
	if(!opacity)return 1;
	mat_opacity=scene->mat_opacity;
	if(!mat_opacity)return 1;
	samples=scene->samples;


	getPalDouble(scene->palname,red,green,blue);

	Lamp=scene->Lamp;

	xReflect=scene->xReflect;
	yReflect=scene->yReflect;
	zReflect=scene->zReflect;



	xmin=myIcon->xmin;
	xmax=myIcon->xmax;

	ixmaxNoReflect=myIcon->ixmax;
	if(xReflect){
	    ixmax=2*ixmaxNoReflect;
	    xmin = -xmax;
	}else{
	    ixmax=ixmaxNoReflect;
	}

	didx=(double)(ixmax)/(xmax-xmin);
	dxdi=1.0/didx;



	ymin=myIcon->ymin;
	ymax=myIcon->ymax;


	iymaxNoReflect=myIcon->iymax;
	if(yReflect){
	    iymax=2*iymaxNoReflect;
	    ymin = -ymax;
	}else{
	    iymax=iymaxNoReflect;
	}

	djdy=(double)(iymax)/(ymax-ymin);
	dydj=1.0/djdy;

	zmin=myIcon->zmin;
	zmax=myIcon->zmax;

	izmaxNoReflect=myIcon->izmax;
	if(zReflect){
	    izmax=2*izmaxNoReflect;
	    zmin = -zmax;
	}else{
	    izmax=izmaxNoReflect;
	}



	dkdz=(double)(izmax)/(zmax-zmin);
	dzdk=1.0/dkdz;

	iyxmax=ixmax*iymax;
	iyxmaxNoReflect=ixmaxNoReflect*iymaxNoReflect;


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


	for(j=Threads->smin;j<Threads->smax;++j){
	    b=scene->ymin+((double)j+.5)*scene->dy;
	    /* if(j < 450 || j > 450)continue; */
	    for(i=0;i<scene->xResolution;++i){
	        /* if(i < 299 || i > 302)continue; */
	        a=scene->xmin+((double)i+.5)*scene->dx;
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
		    alpha=(xmin-Eye.x)/r.x;
		    if(alpha >= 0){
		        yy=Eye.y+alpha*r.y;
		        zz=Eye.z+alpha*r.z;
		        if(yy >= ymin && yy <= ymax && zz >= zmin && zz <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		    alpha=(xmax-Eye.x)/r.x;
		    if(alpha >= 0){
		        yy=Eye.y+alpha*r.y;
		        zz=Eye.z+alpha*r.z;
		        if(yy >= ymin && yy <= ymax && zz >= zmin && zz <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		}
		if(r.y){
		    alpha=(ymin-Eye.y)/r.y;
		    if(alpha >= 0){
		        xx=Eye.x+alpha*r.x;
		        zz=Eye.z+alpha*r.z;
		        if(xx >= xmin && xx <= xmax && zz >= zmin && zz <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		    alpha=(ymax-Eye.y)/r.y;
		    if(alpha >= 0){
		        xx=Eye.x+alpha*r.x;
		        zz=Eye.z+alpha*r.z;
		        if(xx >= xmin && xx <= xmax && zz >= zmin && zz <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		}

		if(r.z){
		    alpha=(zmin-Eye.z)/r.z;
		    if(alpha >= 0){
		        xx=Eye.x+alpha*r.x;
		        yy=Eye.y+alpha*r.y;
		        if(xx >= xmin && xx <= xmax && yy >= ymin && yy <= ymax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		    alpha=(zmax-Eye.z)/r.z;
		    if(alpha >= 0){
		        xx=Eye.x+alpha*r.x;
		        yy=Eye.y+alpha*r.y;
		        if(xx >= xmin && xx <= xmax && yy >= ymin && yy <= ymax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		}
		nc=3*i+ystep*j;
		if(!ihit){
		    cdata[nc]=BackGround.red;
		    cdata[nc+1]=BackGround.green;
		    cdata[nc+2]=BackGround.blue;
		    continue;
		}

		no=i+j*ostep;

		{
		    double xs,ys,zs,xe,ye,ze;
		    int x2,y2,z2;
		    long nl,ne;
		    int n, sx, sy, sz, exy, exz, ezy,
		           ax, ay, az, bx, by, bz;
		    int x, y, z, dx, dy, dz;


		    xs=Eye.x+s1*r.x;
		    ys=Eye.y+s1*r.y;
		    zs=Eye.z+s1*r.z;

		    xe=Eye.x+s2*r.x;
		    ye=Eye.y+s2*r.y;
		    ze=Eye.z+s2*r.z;

		    x=(int)((xs-xmin)*didx);
		    if(x >= ixmax)x = (int)(ixmax - 1);
		    y=(int)((ys-ymin)*djdy);
		    if(y >= iymax)y = (int)(iymax - 1);
		    z=(int)((zs-zmin)*dkdz);
		    if(z >= izmax)z = (int)(izmax - 1);

		    x2=(int)((xe-xmin)*didx);
		    if(x2 >= ixmax)x2 = (int)(ixmax - 1);
		    y2=(int)((ye-ymin)*djdy);
		    if(y2 >= iymax)y2 = (int)(iymax - 1);
		    z2=(int)((ze-zmin)*dkdz);
		    if(z2 >= izmax)z2 = (int)(izmax - 1);



		    dx=x2-x;
		    dy=y2-y;
		    dz=z2-z;

		    sx = sgn(dx);  sy = sgn(dy);  sz = sgn(dz);
		    ax = abs(dx);  ay = abs(dy);  az = abs(dz);
		    bx = 2*ax;     by = 2*ay;     bz = 2*az;
		    exy = ay-ax;   exz = az-ax;   ezy = ay-az;
		    n = ax+ay+az+1;

		    if(n > maxstep)maxstep=n;

		    nl = -1;
		    while(--n > 0){
		        double nx,ny,nz,den;
		        double dnx,dny,dnz;
		        double lx,ly,lz;
		        double value,mopacity;
		        double d;
		        int kc,nomat;
		        int xe,ye,ze;


		        ne=x+y*ixmax+z*iyxmax;
		        if(ne == nl)goto UpdateVariables;
		        nl=ne;

			if(xReflect){
			    if(x < ixmaxNoReflect){
			        xe=(int)ixmaxNoReflect-x-1;
			        dnx = -1.0;
			    }else{
			        xe=x-(int)ixmaxNoReflect;
			        dnx = 1.0;
			    }
			}else{
			    xe=x;
			    dnx = 1.0;
			}

			if(yReflect){
			    if(y < iymaxNoReflect){
			        ye=(int)iymaxNoReflect-y-1;
			        dny = -1.0;
			    }else{
			        ye=y-(int)iymaxNoReflect;
			        dny = 1.0;
			    }
			}else{
			    ye=y;
			    dny = 1.0;
			}

			if(zReflect){
			    if(z < izmaxNoReflect){
			        ze=(int)izmaxNoReflect-z-1;
			        dnz = -1.0;
			    }else{
			        ze=z-(int)izmaxNoReflect;
			        dnz = 1.0;
			    }
			}else{
			    ze=z;
			    dnz = 1.0;
			}

		        ne=xe+ye*ixmaxNoReflect+ze*iyxmaxNoReflect;

			if(ne < 0 || ne >= nemax){
			    if(++ierror < 10){
			        sprintf(WarningBuff,"ERROR : ne %ld x %d y %d z %d\n",ne,x,y,z);
					if(scene->debug > 1)WarningBatch(WarningBuff);
			    }
			    goto UpdateVariables;
			}

			d=data[ne];
			if(d < dmin || d > dmax)goto UpdateVariables;


			    nomat=(int)((d-dmin)*mat_scale);
			    if(nomat < 0){
			        nomat=0;
			    }else if(nomat >= samples){
			       nomat = (int)(samples-1);
			    }

			    mopacity=mat_opacity[nomat];
			    if(mopacity <= 0.0)goto UpdateVariables;

		
		            if(xe >= ixmaxNoReflect-1){
		                nx=(.5*(-data[ne-1]));
		            }else if(xe == 0){
		                nx=(.5*data[ne+1]);
		            }else{
		                nx=(.5*((data[ne+1])-(data[ne-1])));
		            }

			    nx *= dnx;

		            if(ye >= iymaxNoReflect-1){
		                ny=(.5*(-data[ne-ixmaxNoReflect]));
		            }else if(ye == 0){
		                ny=(.5*data[ne+ixmaxNoReflect]);
		            }else{
		                ny=(.5*(data[ne+ixmaxNoReflect]-data[ne-ixmaxNoReflect]));
		            }

			    ny *= dny;

		            if(ze >= izmaxNoReflect-1){
		                nz=(.5*(-data[ne-iyxmaxNoReflect]));
		            }else if(ze == 0){
		                nz=(.5*(data[ne+iyxmaxNoReflect]));
		            }else{
		                nz=(0.5*(data[ne+iyxmaxNoReflect]-data[ne-iyxmaxNoReflect]));
		            }

			    nz *= dnz;

			    den=sqrt(nx*nx+ny*ny+nz*nz);
			    if(den){
			         nx /= den;
			         ny /= den;
			         nz /= den;

			        if((r.x*nx+r.y*ny+r.z*nz) > 0.0){    /* flip nornmal */
			            nx = -nx;
			            ny = -ny;
			            nz = -nz;
			        }

			        xx=xmin+(x+0.5)*dxdi;
			        yy=ymin+(y+0.5)*dydj;
			        zz=zmin+(z+0.5)*dzdk;

			        lx=(Lamp.x-xx);
			        ly=(Lamp.y-yy);
			        lz=(Lamp.z-zz);
			        den=sqrt(lx*lx+ly*ly+lz*lz);
			        if(den){
			            lx /= den;
			            ly /= den;
			            lz /= den;
			            value=nx*lx+ny*ly+nz*lz;
			        }else{
			             value=1.0;
			        }
			        if(value < 0)value = .25;
			    }else{
			        value=1.0;
			    }


			    kc=(int)(1+(d-dmin)*252.0/(dmax-dmin));


			    if((mopacity+opacity[no]) > 1.0){
			        mopacity = (1.0 - opacity[no])+1.0e-5;
			    }

		            cdata[nc]   += (red[kc]*value*mopacity);
		            cdata[nc+1] += (green[kc]*value*mopacity);
		            cdata[nc+2] += (blue[kc]*value*mopacity);


		            opacity[no] += mopacity;
		            if(opacity[no] >= 1.0){

		                opacity[no] = 1.0;
		                break;
			    }
UpdateVariables:
		            if ( exy < 0 ) {
		                if ( exz < 0 ) {
		                    x += sx;
		                    exy += by; exz += bz;
		                }
		                else  {
		                    z += sz;
		                    exz -= bx; ezy += by;
		                }
		            }
		            else {
		                if ( ezy < 0 ) {
		                    z += sz;
		                    exz -= bx; ezy += by;
		                }
		                else  {
		                    y += sy;
		                    exy -= bx; ezy -= bz;
		                }
		            }
		    }
		    if(opacity[no] < 1.0){
		        cdata[nc]   += (BackGround.red*(1.0-opacity[no]));
		        cdata[nc+1] += (BackGround.green*(1.0-opacity[no]));
		        cdata[nc+2] += (BackGround.blue*(1.0-opacity[no]));
		    }
		}

	    }
	}


	if(scene->debug > 1){
	    sprintf(WarningBuff,"TraceLinesDAA smin %ld smax %ld maxstep %ld\n",Threads->smin,Threads->smax,maxstep);
		WarningBatch(WarningBuff);
	}
	Threads->done=TRUE;

	return 0;
}
int sageSetFrame(FilePtr Files,long CurrentFrame)
{
	vIconPtr myIcon;
	
	if(!Files)goto ErrorOut;
	myIcon=&Files->vmyIcon;
	
    myIcon->CurrentFrame=CurrentFrame;
	if(myIcon->TraceType == 2){
    	if(openPIO(Files,CurrentFrame))goto ErrorOut;
	    myIcon->ixmax=myIcon->xSamples;
	    myIcon->iymax=myIcon->ySamples;
	    myIcon->izmax=myIcon->zSamples;
	    if(sageSample(myIcon))goto ErrorOut;
	}else if(myIcon->TraceType == 3){
    	if(openSDS3D(Files,CurrentFrame))goto ErrorOut;
	    if(sageSDS3D(myIcon))goto ErrorOut;
	}else{
    	if(openPIO(Files,CurrentFrame))goto ErrorOut;
	    if(sageLocation(myIcon))goto ErrorOut;
	    if(sageGradients(Files))goto ErrorOut;
	    if(TraceSetUp(myIcon))goto ErrorOut;
	    if(sageGetFull(Files))goto ErrorOut;
	}
	return 0;
ErrorOut:
	return 1;
}
int sageTrace(struct FileInfo2 *Files,int flag)
{
	vIconPtr myIcon;
	long n;

	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	
	if(flag){	
	    for(n=0;n<Files->ImageCount;++n){
	        if(sageSetFrame(Files,n))goto ErrorOut;
			if(sageDoTrace(Files))goto ErrorOut;
		}
	}else{
	    if(sageSetFrame(Files,myIcon->CurrentFrame))goto ErrorOut;
		if(sageDoTrace(Files))goto ErrorOut;	
	}

	return 0;
ErrorOut:
	return 1;
}
int sageDoTrace(FilePtr Files)
{
	struct vScene *scene;
	vIconPtr myIcon;
	struct P EyeRight;
	double *vbuff;
	struct P Eye;
	long length;
	int n,np;
	int ret;

	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;

	ret =1;

	scene->start=rtime();

	Eye=scene->Eye;
	EyeRight=scene->EyeRight;

	np=1;
	vbuff=NULL;
	
	if(scene->EyeView == Scene_Anaglyph){
	    np=2;
		scene->Eye=scene->EyeLeft;
	}else if(scene->EyeView == Scene_EyeLeft){
		scene->Eye=scene->EyeLeft;
	}else if(scene->EyeView == Scene_EyeRight){
		scene->Eye=scene->EyeRight;
	}
	
	for(n=0;n<np;++n){

		if(runThreads(myIcon->threads,myIcon,scene->yResolution,SageTraceLines))goto ErrorOut;
	
		scene->Eye=EyeRight;

		if(!vbuff){
		    vbuff=scene->cdata;		
			if(np > 1){
				length=scene->xResolution*scene->yResolution;
				scene->cdata=(double *)cMalloc(3*length*sizeof(double),1934);
				if(!scene->cdata)goto ErrorOut;
				zerol((char *)scene->cdata,3*length*sizeof(double));		
				zerol((char *)scene->opacity,length*sizeof(double));
				zerol((char *)scene->opacityToFireball,length*sizeof(double));
			}
		}
	
	}

	if(scene->EyeView == Scene_Anaglyph){
	
	    doAnaglyph(scene->xResolution,scene->yResolution,vbuff,scene->cdata);
	
	    if(vbuff)cFree((char *)vbuff);
	}

	writeTRUE(Files);
	
	scene->Eye=Eye;

	ret=0;
ErrorOut:
	
	return ret;
}

int SageTraceCell(long ne,struct rayData *ray,long *icount);

int FindTraceOrder(vIconPtr myIcon)
{
	struct vScene *scene;
	struct P Target;
	struct P Eye;
	int RayDirection;

	if(!myIcon)return 1;
	scene=&myIcon->scene;

	Eye=scene->Eye;
	Target=scene->Target;

	if(Eye.z > Target.z){           /* in from +z */
	    if(Eye.x > Target.x){       /* in from +x */
	        if(Eye.y > Target.y){   /* in from +y */
	            RayDirection = 7;
	        }else{                  /* in from -y */
	            RayDirection = 5;
	        }
	    }else{                      /* in from -x */
	        if(Eye.y > Target.y){   /* in from +y */
	            RayDirection = 6;
	        }else{                  /* in from -y */
	            RayDirection = 4;
	        }
	    }
	}else{                           /* in from -z */
	    if(Eye.x > Target.x){       /* in from +x */
	        if(Eye.y > Target.y){   /* in from +y */
	            RayDirection = 3;
	        }else{                  /* in from -y */
	            RayDirection = 1;
	        }
	    }else{                      /* in from -x */
	        if(Eye.y > Target.y){   /* in from +y */
	            RayDirection = 2;
	        }else{                  /* in from -y */
	            RayDirection = 0;
	        }
	    }
	}
	scene->RayDirection=RayDirection;

	return 0;
}
int SageTraceLines(mThread *Threads)
{
	struct vScene *scene;
	vIconPtr myIcon;
	
	if(!Threads)return 1;
	myIcon=(vIconPtr)Threads->data;
	if(!myIcon)return 1;
	scene=&myIcon->scene;

	if(myIcon->TraceType == 0){
	    return SageTraceLines0(Threads);
	}else if(myIcon->TraceType == 1){
	    return SageTraceLines1(Threads);
	}else if(myIcon->TraceType == 2 || myIcon->TraceType == 3){
	    return TraceLines(Threads);
	}else if(myIcon->TraceType == 4){
	    if(scene->xReflect || scene->yReflect || scene->zReflect){
	        return SageTraceLines4b(Threads);
	    }else{
	        return SageTraceLines4a(Threads);
	    }
	}else if(myIcon->TraceType == 5){
		return SageTraceLines5(Threads);
	}else if(myIcon->TraceType == 6){
		return SageTraceBangMeter(Threads);
	}
	return 1;
}

double cellDepthRayReflect(long ne,struct rayData *rayin)
{
	register struct rayData *ray;
	double cellDepth;
	double alpha,dx,dy,dz;
	double xx[6],yy[6],zz[6];
	struct P r,Eye; 
	double xc,yc,zc;
	double x,y,z;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	static long ierror;
	int np,k;

	if(!rayin)return 0.0;

	ray=rayin;

	if(ne < 0 || ne >= ray->nemax){
	    if(++ierror < 10){
	        sprintf(WarningBuff,"cellDepthRayReflect ERROR : ne %ld nemax %ld\n",ne,ray->nemax);
			if(rayin->debug > 1)WarningBatch(WarningBuff);
	    }
	   return 0.0;
	}

	r=ray->r;
	Eye=ray->Eye;

	k=(int)ray->level[ne];
	if(ray->xReflect){
	    xc = -ray->xcenter[ne];
	}else{
	    xc =  ray->xcenter[ne];
	}
	xmin=xc-ray->dx2[k];
	xmax=xc+ray->dx2[k];

	if(ray->yReflect){
	    yc = -ray->ycenter[ne];
	}else{
	    yc =  ray->ycenter[ne];
	}
	ymin=yc-ray->dy2[k];
	ymax=yc+ray->dy2[k];

	if(ray->zReflect){
	    zc = -ray->zcenter[ne];
	}else{
	    zc =  ray->zcenter[ne];
	}
	zmin=zc-ray->dz2[k];
	zmax=zc+ray->dz2[k];

	np=0;

	if(r.x){
	    double ox;
	    ox=1.0/r.x;
	    alpha=(xmin-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
		    xx[np]=xmin;
		    yy[np]=y;
		    zz[np]=z;
		    ++np;
		  }
	        }
	    }
	    alpha=(xmax-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
		    xx[np]=xmax;
		    yy[np]=y;
		    zz[np]=z;
		    ++np;
		  }
	        }
	    }
	}
	if(r.y){
	    double oy;
	    oy=1.0/r.y;
	    alpha=(ymin-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
		    xx[np]=x;
		    yy[np]=ymin;
		    zz[np]=z;
		    ++np;
		  }
	        }
	    }
	    alpha=(ymax-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
		    xx[np]=x;
		    yy[np]=ymax;
		    zz[np]=z;
		    ++np;
		  }
	        }
	    }
	}

	if(r.z){
	    double oz;
	    oz=1.0/r.z;
	    alpha=(zmin-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
		    xx[np]=x;
		    yy[np]=y;
		    zz[np]=zmin;
		    ++np;
		  }
	        }
	    }
	    alpha=(zmax-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
		    xx[np]=x;
		    yy[np]=y;
		    zz[np]=zmax;
		    ++np;
		  }
	        }
	    }
	}

	if(np == 2){
	    dx=xx[1]-xx[0];
	    dy=yy[1]-yy[0];
	    dz=zz[1]-zz[0];
	}else if(np == 1){
	    dx=Eye.x-xx[0];
	    dy=Eye.y-yy[0];
	    dz=Eye.z-zz[0];
	}else{
	    if(++ierror < 10){ 
	        sprintf(WarningBuff,"cellDepthRayReflect ne %ld Intersections %d\n",ne,np);
			if(rayin->debug > 1)WarningBatch(WarningBuff);
	    }
	    return 0.0;
	}

	cellDepth=sqrt(dx*dx+dy*dy+dz*dz)*ray->CellSizeOver;

	return cellDepth;
}
int SageHitDaughterReflect(long ne,struct rayData *rayin,char *daughter)
{
	register struct rayData *ray;
	double alpha;
	struct P r,Eye; 
	double x,y,z;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double xc,yc,zc;
	int ihit;
	int k;

	static long ierror;

	if(!rayin)return 0;
	
	ihit = 0;

	ray=rayin;

	if(ne < 0 || ne >= ray->nemax){
	    if(++ierror < 10){
	        sprintf(WarningBuff,"ERROR : ne %ld nemax %ld\n",ne,ray->nemax);
			if(rayin->debug > 1)WarningBatch(WarningBuff);
	    }
	   return 0;
	}

	for(k=0;k<8;++k)daughter[k]=0;

	r=ray->r;
	Eye=ray->Eye;

	k=(int)ray->level[ne];
	if(ray->xReflect){
	    xc = -ray->xcenter[ne];
	}else{
	    xc =  ray->xcenter[ne];
	}
	xmin=xc-ray->dx2[k];
	xmax=xc+ray->dx2[k];

	if(ray->yReflect){
	    yc = -ray->ycenter[ne];
	}else{
	    yc =  ray->ycenter[ne];
	}
	ymin=yc-ray->dy2[k];
	ymax=yc+ray->dy2[k];

	if(ray->zReflect){
	    zc = -ray->zcenter[ne];
	}else{
	    zc =  ray->zcenter[ne];
	}
	zmin=zc-ray->dz2[k];
	zmax=zc+ray->dz2[k];


	if(r.x){
	    double ox;

	    ox=1.0/r.x;
	    alpha=(xmin-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
	            ihit=1;
	            if(y > yc){
	                if(z > zc){
			   			daughter[6]=1;
	                }else{
			   			daughter[2]=1;
	                }
	            }else{
	                if(z > zc){
			   			daughter[4]=1;
	                }else{
			   			daughter[0]=1;
	                }
	            }
	          }
	        }
	    }
	    alpha=(xc-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
	            ihit=1;
	            if(y > yc){
	                if(z > zc){
			   			daughter[6]=1;
			   			daughter[7]=1;
	                }else{
			   			daughter[2]=1;
			   			daughter[3]=1;
	                }
	            }else{
	                if(z > zc){
			   			daughter[4]=1;
			   			daughter[5]=1;
	                }else{
			   			daughter[0]=1;
			   			daughter[1]=1;
	                }
	            }
	          }
	        }
	    }
	    alpha=(xmax-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
	            ihit=1;
	            if(y > yc){
	                if(z > zc){
			   			daughter[7]=1;
	                }else{
			   			daughter[3]=1;
	                }
	            }else{
	                if(z > zc){
			   			daughter[5]=1;
	                }else{
			   			daughter[1]=1;
	                }
	            }
	          }
	        }
	    }
	}
	if(r.y){
	    double oy;

	    oy=1.0/r.y;

	    alpha=(ymin-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
	            ihit=1;
	            if(x > xc){
	                if(z > zc){
			   			daughter[5]=1;
	                }else{
			   			daughter[1]=1;
	                }
	            }else{
	                if(z > zc){
			   			daughter[4]=1;
	                }else{
			   			daughter[0]=1;
	                }
	            }
	          }
	        }
	    }
	    alpha=(yc-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
	            ihit=1;
	            if(x > xc){
	                if(z > zc){
			   			daughter[5]=1;
			   			daughter[7]=1;
	                }else{
			   			daughter[1]=1;
			  			daughter[3]=1;
	                }
	            }else{
	                if(z > zc){
			   			daughter[4]=1;
			   			daughter[6]=1;
	                }else{
			   			daughter[0]=1;
			   			daughter[2]=1;
	                }
	            }
	          }
	        }
	    }
	    alpha=(ymax-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
	            ihit=1;
	            if(x > xc){
	                if(z > zc){
			   			daughter[7]=1;
	                }else{
			   			daughter[3]=1;
	                }
	            }else{
	                if(z > zc){
			   			daughter[6]=1;
	                }else{
			   			daughter[2]=1;
	                }
	            }
	          }
	        }
	    }
	}

	if(r.z){
	    double oz;

	    oz=1.0/r.z;
	    alpha=(zmin-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
	            ihit=1;
	            if(x > xc){
	                if(y > yc){
			   			daughter[3]=1;
	                }else{
			   			daughter[1]=1;
	                }
	            }else{
	                if(y > yc){
			   			daughter[2]=1;
	                }else{
			   			daughter[0]=1;
	                }
	            }
	          }
	        }
	    }
	    alpha=(zc-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
	            ihit=1;
	            if(x > xc){
	                if(y > yc){
			   			daughter[3]=1;
			   			daughter[7]=1;
	                }else{
			   			daughter[1]=1;
			   			daughter[5]=1;
	                }
	            }else{
	                if(y > yc){
			   			daughter[2]=1;
			   			daughter[6]=1;
	                }else{
			   			daughter[0]=1;
			   			daughter[4]=1;
	                }
	            }
	          }
	        }
	    }
	    alpha=(zmax-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
	            ihit=1;
	            if(x > xc){
	                if(y > yc){
			   			daughter[7]=1;
	                }else{
			   			daughter[5]=1;
	                }
	            }else{
	                if(y > yc){
			   			daughter[6]=1;
			}else{
			   			daughter[4]=1;
	                }
	            }
	          }
	        }
	    }
	}
	return ihit;
}
int SageDepthReflect(long ne,struct rayData *rayin)
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
             7,5,6,4,  3,1,2,0,

             1,0,3,2,  5,4,7,6,
             0,1,2,3,  4,5,6,7,
             3,1,2,0,  7,6,5,4,
             2,0,3,1,  6,4,7,5,
             5,4,7,6,  1,0,3,2,
             4,5,6,7,  0,1,2,3,
             7,5,6,4,  3,2,1,0,
             6,4,7,5,  2,0,3,1,

             2,3,0,1,  6,7,4,5,
             3,2,1,0,  7,6,5,4,
             0,2,1,3,  4,5,6,7,
             1,3,0,2,  5,7,4,6,
             6,7,4,5,  2,3,0,1,
             7,6,5,4,  3,2,1,0,
             4,6,5,7,  0,1,2,3,
             5,7,4,6,  1,3,0,2,

             3,2,1,0,  7,6,5,4,
             2,3,0,1,  6,7,4,5,
             1,3,0,2,  5,4,7,6,
             0,2,1,3,  4,6,5,7,
             7,6,5,4,  3,2,1,0,
             6,7,4,5,  2,3,0,1,
             5,7,4,6,  1,0,3,2,
             4,6,5,7,  0,2,1,3,

             4,5,6,7,  0,1,2,3,
             5,4,7,6,  1,0,3,2,
             6,4,7,5,  2,3,0,1,
             7,5,6,4,  3,1,2,0,
             0,1,2,3,  4,5,6,7,
             1,0,3,2,  5,4,7,6,
             2,0,3,1,  6,7,4,5,
             3,1,2,0,  7,5,6,4,

             5,4,7,6,  1,0,3,2,
             4,5,6,7,  0,1,2,3,
             7,5,6,4,  3,2,1,0,
             6,4,7,5,  2,0,3,1,
             1,0,3,2,  5,4,7,6,
             0,1,2,3,  4,5,6,7,
             3,1,2,0,  7,6,5,4,
             2,0,3,1,  6,4,7,5,

             7,6,5,4,  3,2,1,0,
             6,7,4,5,  2,3,0,1,
             5,7,4,6,  1,0,3,2,
             4,6,5,7,  0,2,1,3,
             3,2,1,0,  7,6,5,4,
             2,3,0,1,  6,7,4,5,
             1,3,0,2,  5,4,7,6,
             0,2,1,3,  4,6,5,7,
                             };
	int k;
	double nx,ny,nz;
	double xe,ye,ze;
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
             int nreflect;

             if(!SageHitDaughterReflect(ne,ray,dd))return 0;

		    nreflect=ray->xReflect+2*ray->yReflect+4*ray->zReflect;
		    for(k=0;k<8;++k){
		        long nd;
		        nd=order[k+8*ray->RayDirection+nreflect*64]+(long)ray->daughter[ne]-1;
		        if(!ray->hasData[nd])continue;
		        if(dd[order[k+8*ray->RayDirection]]){
		            SageDepthReflect(nd,ray);
		        }
		    }

        }


	return 0;

ProcessRay:


	d=ray->data[ne];
	if(d < ray->dmin || d > ray->dmax)return 0;

	if(ray->scene){
		mopacity=interpolate(d,ray->scene->tcount,ray->scene->tvalue,ray->scene->topacity);
	}else{
		nomat=(long)((d-ray->dmin)*ray->mat_scale);
		if(nomat < 0){
            nomat=0;
		}else if(nomat >= ray->samples){
           nomat = ray->samples-1;
		}

		mopacity=ray->mat_opacity[nomat];
	}

	if(mopacity <= 0.0)return 0;

	cellDepth=cellDepthRayReflect(ne,ray);

	mopacity *= cellDepth;

	if(mopacity <= 0.0)return 0;

	if(ray->xReflect){
	    nx = -ray->gradx[ne];
	    xe = -ray->xcenter[ne];
	}else{
	    nx =  ray->gradx[ne];
	    xe =  ray->xcenter[ne];
	}

	if(ray->yReflect){
	    ny = -ray->grady[ne];
	    ye = -ray->ycenter[ne];
	}else{
	    ny =  ray->grady[ne];
	    ye =  ray->ycenter[ne];
	}

	if(ray->zReflect){
	    nz = -ray->gradz[ne];
	    ze = -ray->zcenter[ne];
	}else{
	    nz =  ray->gradz[ne];
	    ze =  ray->zcenter[ne];
	}

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

       lx=(ray->Lamp.x-xe);
       ly=(ray->Lamp.y-ye);
       lz=(ray->Lamp.z-ze);
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


	ray->cdata[ray->nc]   +=(ray->red[kc]*value*mopacity);
	ray->cdata[ray->nc+1] +=(ray->green[kc]*value*mopacity);
	ray->cdata[ray->nc+2] +=(ray->blue[kc]*value*mopacity);

	ray->opacity[ray->no] +=mopacity;
	if(ray->opacity[ray->no] >= 1.0)return 0;
	return 0;

}
int SageTraceLines4b(mThread *Threads)
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
	struct rayData ray;
    tFlt *xcenter;
    tFlt *ycenter;
    tFlt *zcenter;
    unsigned char *level;
	tFlt *data;
    struct cell *nCell;
    unsigned char *nCellEnter;
    long sCount;
    long length;
    long nn;
    double dxdi;
    double dydj;
    double dzdk;
	int xReflect,yReflect,zReflect;
	long ixmaxNoReflect,iymaxNoReflect,izmaxNoReflect,iyxmaxNoReflect;

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
	
	maxstep=0;
	ierror=0;

    FindTraceOrder(myIcon);

	getPalDouble(scene->palname,red,green,blue);

	Lamp=scene->Lamp;

	xReflect=scene->xReflect;
	yReflect=scene->yReflect;
	zReflect=scene->zReflect;

	xmin=pio->xmin;
	xmax=pio->xmax;

	ixmaxNoReflect=sage->ixmax;
	if(xReflect){
	    ixmax=2*ixmaxNoReflect;
	    xmin = -xmax;
	}else{
	    ixmax=ixmaxNoReflect;
	}

	didx=(double)(ixmax)/(xmax-xmin);
	dxdi=1.0/didx;

	ymin=pio->ymin;
	ymax=pio->ymax;


	iymaxNoReflect=sage->iymax;
	if(yReflect){
	    iymax=2*iymaxNoReflect;
	    ymin = -ymax;
	}else{
	    iymax=iymaxNoReflect;
	}

	djdy=(double)(iymax)/(ymax-ymin);
	dydj=1.0/djdy;

	zmin=pio->zmin;
	zmax=pio->zmax;

	izmaxNoReflect=sage->izmax;
	if(zReflect){
	    izmax=2*izmaxNoReflect;
	    zmin = -zmax;
	}else{
	    izmax=izmaxNoReflect;
	}

	dkdz=(double)(izmax)/(zmax-zmin);
	dzdk=1.0/dkdz;

	iyxmax=ixmax*iymax;
	iyxmaxNoReflect=ixmaxNoReflect*iymaxNoReflect;

	length=2*(ixmax+iymax+izmax+3);

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

		ray.scene=scene;
        ray.hasData=hasData;
        ray.gradx=gradx;
        ray.grady=grady;
        ray.gradz=gradz;
        ray.daughter=daughter;

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
		       if((Threads->smin == 0)){
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
	    /* if(j <= 359 || j >= 361)continue; */
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


		no=i+j*ostep;

		if((s1 == s2) || (s1 < 0)){
		   s1 = 0;
		}else{
		   s1 -= 1.0e-8;
		}
		s2 += 1.0e-8;

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
		        long xe,ye,ze;
		        long nee;
			unsigned char xReflectFlag;
			unsigned char yReflectFlag;
			unsigned char zReflectFlag;

		        ne=nCell[n].ne;
	
		        if(nn > 0 && (ne == nCell[nn-1].ne)){
		            continue;
		        }
	
			if(ne < 0 || ne >= nemax){++ierror;continue;}


			nez=ne/iyxmax;
			ney=(ne-nez*iyxmax)/ixmax;
			nex=ne-nez*iyxmax-ney*ixmax;

			xReflectFlag=0;

			if(xReflect){
			    if(nex < ixmaxNoReflect){
			        xe=(long)ixmaxNoReflect-nex-1;
			        xReflectFlag=1;
			    }else{
			        xe=nex-(long)ixmaxNoReflect;
			    }
			}else{
			    xe=nex;
			}

			yReflectFlag=0;
			if(yReflect){
			    if(ney < iymaxNoReflect){
			        ye=(long)iymaxNoReflect-ney-1;
			        yReflectFlag=1;
			    }else{
			        ye=ney-(long)iymaxNoReflect;
			    }
			}else{
			    ye=ney;
			}

			zReflectFlag=0;
			if(zReflect){
			    if(nez < izmaxNoReflect){
			        ze=(long)izmaxNoReflect-nez-1;
			        zReflectFlag=1;
			    }else{
			        ze=nez-(long)izmaxNoReflect;
			    }
			}else{
			    ze=nez;
			}

		        nee=xe+ye*ixmaxNoReflect+ze*iyxmaxNoReflect;

			if(!hasData[location[nee]])continue;

			dx=Eye.x-(((double)nex+.5)*dxdi+xmin);
			dy=Eye.y-(((double)ney+.5)*dydj+ymin);
			dz=Eye.z-(((double)nez+.5)*dzdk+zmin);

			nCell[nn].s = dx*dx+dy*dy+dz*dz;
			nCell[nn].nee = nee;
			nCell[nn].xReflect = xReflectFlag;
			nCell[nn].yReflect = yReflectFlag;
			nCell[nn].zReflect = zReflectFlag;
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

			ne=location[nCell[n].nee];
			ray.xReflect=nCell[n].xReflect;
			ray.yReflect=nCell[n].yReflect;
			ray.zReflect=nCell[n].zReflect;

			
			SageDepthReflect(ne,&ray);

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
	    sprintf(WarningBuff,"SageTraceLines4b smin %ld smax %ld maxstep %ld\n",
	            Threads->smin,Threads->smax,maxstep);
		WarningBatch(WarningBuff);
	}
ErrorOut:
	if(ierror > 0){
	    sprintf(WarningBuff,"SageTraceLines4b smin %ld smax %ld maxstep %ld ierror %ld\n",
	       Threads->smin,Threads->smax,maxstep,ierror);
		WarningBatch(WarningBuff);
	}

	if(nCell)cFree((char *)nCell);
	if(nCellEnter)cFree((char *)nCellEnter);

	Threads->done=TRUE;

	return 0;
}

int SageTraceLines4a(mThread *Threads)
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
	struct rayData ray;
    tFlt *xcenter;
    tFlt *ycenter;
    tFlt *zcenter;
    unsigned char *level;
	tFlt *data;
    struct cell *nCell;
    unsigned char *nCellEnter;
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

		ray.scene=scene;
		ray.hasData=hasData;
        ray.gradx=gradx;
        ray.grady=grady;
        ray.gradz=gradz;
        ray.daughter=daughter;

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
		       if((Threads->smin == 0)){
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
double cellDepthRay(long ne,struct rayData *rayin)
{
	register struct rayData *ray;
	double cellDepth;
	double alpha,dx,dy,dz;
	double xx[6],yy[6],zz[6];
	struct P r,Eye; 
	double x,y,z;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	static long ierror;
	int np,k;
	double ox;
	double oz;
	double oy;

	if(!rayin)return 0.0;

	ray=rayin;

	if(ne < 0 || ne >= ray->nemax){
	    if(++ierror < 10){
	        if(rayin->debug > 1){
	        	sprintf(WarningBuff,"ERROR : ne %ld nemax %ld\n",ne,ray->nemax);
				WarningBatch(WarningBuff);
			}
	    }
	   return 0.0;
	}

	r=ray->r;
	Eye=ray->Eye;

	k=(int)ray->level[ne];
	xmin=ray->xcenter[ne]-ray->dx2[k];
	xmax=ray->xcenter[ne]+ray->dx2[k];

	ymin=ray->ycenter[ne]-ray->dy2[k];
	ymax=ray->ycenter[ne]+ray->dy2[k];

	zmin=ray->zcenter[ne]-ray->dz2[k];
	zmax=ray->zcenter[ne]+ray->dz2[k];

	np=0;

	if(r.x){
	    ox=1.0/r.x;
	    alpha=(xmin-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
			    xx[np]=xmin;
			    yy[np]=y;
			    zz[np]=z;
			    ++np;
			  }
	        }
	    }
	    alpha=(xmax-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
			    xx[np]=xmax;
			    yy[np]=y;
			    zz[np]=z;
			    ++np;
			  }
	        }
	    }
	}
	if(r.y){
	    oy=1.0/r.y;
	    alpha=(ymin-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
			    xx[np]=x;
			    yy[np]=ymin;
			    zz[np]=z;
			    ++np;
			  }
	        }
	    }
	    alpha=(ymax-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
			    xx[np]=x;
			    yy[np]=ymax;
			    zz[np]=z;
			    ++np;
			  }
	        }
	    }
	}

	if(r.z){
	    oz=1.0/r.z;
	    alpha=(zmin-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
			    xx[np]=x;
			    yy[np]=y;
			    zz[np]=zmin;
			    ++np;
			  }
	        }
	    }
	    alpha=(zmax-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
			    xx[np]=x;
			    yy[np]=y;
			    zz[np]=zmax;
			    ++np;
			  }
	        }
	    }
	}

	if(np == 2){
	    dx=xx[1]-xx[0];
	    dy=yy[1]-yy[0];
	    dz=zz[1]-zz[0];
	}else if(np == 1){
	    dx=Eye.x-xx[0];
	    dy=Eye.y-yy[0];
	    dz=Eye.z-zz[0];
	}else{
	    if(++ierror < 10){ 
	        if(rayin->debug > 1){
	            sprintf(WarningBuff,"cellDepthRay ne %ld Intersections %d\n",ne,np);
			    WarningBatch(WarningBuff);
			}
	    }
	    return 0.0;
	}

	cellDepth=sqrt(dx*dx+dy*dy+dz*dz)*ray->CellSizeOver;

	return cellDepth;
}
int SageCellHitDaughter(long ne,struct rayData *rayin,char *daughter)
{
	register struct rayData *ray;
	double alpha;
	struct P r,Eye; 
	double x,y,z;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double xc,yc,zc;
	int ihit;
	int k;

	static long ierror;

	if(!rayin)return 0;

	ihit=1;
	
	ray=rayin;

	if(ne < 0 || ne >= ray->nemax){
	    if(++ierror < 10){
	        if(rayin->debug > 1){
	            sprintf(WarningBuff,"ERROR : ne %ld nemax %ld\n",ne,ray->nemax);
			    WarningBatch(WarningBuff);
			}
	    }
	   return 0;
	}

	for(k=0;k<8;++k)daughter[k]=0;

	r=ray->r;
	Eye=ray->Eye;

	k=(int)ray->level[ne];
	xc=ray->xcenter[ne];
	xmin=xc-ray->dx2[k];
	xmax=xc+ray->dx2[k];

	yc=ray->ycenter[ne];
	ymin=yc-ray->dy2[k];
	ymax=yc+ray->dy2[k];

	zc=ray->zcenter[ne];
	zmin=zc-ray->dz2[k];
	zmax=zc+ray->dz2[k];

	if(r.x){
	    double ox;

	    ox=1.0/r.x;
	    alpha=(xmin-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
	            ihit=1;
	            if(y > yc){
	                if(z > zc){
			   daughter[6]=1;
	                }else{
			   daughter[2]=1;
	                }
	            }else{
	                if(z > zc){
			   daughter[4]=1;
	                }else{
			   daughter[0]=1;
	                }
	            }
	          }
	        }
	    }
	    alpha=(xc-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
	            ihit=1;
	            if(y > yc){
	                if(z > zc){
			   daughter[6]=1;
			   daughter[7]=1;
	                }else{
			   daughter[2]=1;
			   daughter[3]=1;
	                }
	            }else{
	                if(z > zc){
			   daughter[4]=1;
			   daughter[5]=1;
	                }else{
			   daughter[0]=1;
			   daughter[1]=1;
	                }
	            }
	          }
	        }
	    }
	    alpha=(xmax-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
	            ihit=1;
	            if(y > yc){
	                if(z > zc){
			   daughter[7]=1;
	                }else{
			   daughter[3]=1;
	                }
	            }else{
	                if(z > zc){
			   daughter[5]=1;
	                }else{
			   daughter[1]=1;
	                }
	            }
	          }
	        }
	    }
	}
	if(r.y){
	    double oy;

	    oy=1.0/r.y;

	    alpha=(ymin-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
	            ihit=1;
	            if(x > xc){
	                if(z > zc){
			   daughter[5]=1;
	                }else{
			   daughter[1]=1;
	                }
	            }else{
	                if(z > zc){
			   daughter[4]=1;
	                }else{
			   daughter[0]=1;
	                }
	            }
	          }
	        }
	    }
	    alpha=(yc-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
	            ihit=1;
	            if(x > xc){
	                if(z > zc){
			   daughter[5]=1;
			   daughter[7]=1;
	                }else{
			   daughter[1]=1;
			   daughter[3]=1;
	                }
	            }else{
	                if(z > zc){
			   daughter[4]=1;
			   daughter[6]=1;
	                }else{
			   daughter[0]=1;
			   daughter[2]=1;
	                }
	            }
	          }
	        }
	    }
	    alpha=(ymax-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
	            ihit=1;
	            if(x > xc){
	                if(z > zc){
			   daughter[7]=1;
	                }else{
			   daughter[3]=1;
	                }
	            }else{
	                if(z > zc){
			   daughter[6]=1;
	                }else{
			   daughter[2]=1;
	                }
	            }
	          }
	        }
	    }
	}

	if(r.z){
	    double oz;

	    oz=1.0/r.z;
	    alpha=(zmin-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
	            ihit=1;
	            if(x > xc){
	                if(y > yc){
			   daughter[3]=1;
	                }else{
			   daughter[1]=1;
	                }
	            }else{
	                if(y > yc){
			   daughter[2]=1;
	                }else{
			   daughter[0]=1;
	                }
	            }
	          }
	        }
	    }
	    alpha=(zc-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
	            ihit=1;
	            if(x > xc){
	                if(y > yc){
			   daughter[3]=1;
			   daughter[7]=1;
	                }else{
			   daughter[1]=1;
			   daughter[5]=1;
	                }
	            }else{
	                if(y > yc){
			   daughter[2]=1;
			   daughter[6]=1;
	                }else{
			   daughter[0]=1;
			   daughter[4]=1;
	                }
	            }
	          }
	        }
	    }
	    alpha=(zmax-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
	            ihit=1;
	            if(x > xc){
	                if(y > yc){
			   daughter[7]=1;
	                }else{
			   daughter[5]=1;
	                }
	            }else{
	                if(y > yc){
			   daughter[6]=1;
			}else{
			   daughter[4]=1;
	                }
	            }
	          }
	        }
	    }
	}
	return ihit;
}
int SageTraceCellDepth(long ne,struct rayData *rayin)
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



	d=ray->data[ne];
	if(d < ray->dmin || d > ray->dmax)return 0;

	if(ray->scene){
		mopacity=interpolate(d,ray->scene->tcount,ray->scene->tvalue,ray->scene->topacity);
	}else{
		nomat=(long)((d-ray->dmin)*ray->mat_scale);
		if(nomat < 0){
			nomat=0;
		}else if(nomat >= ray->samples){
			nomat = ray->samples-1;
		}
		mopacity=ray->mat_opacity[nomat];
	}

	if(mopacity <= 0.0)return 0;

	cellDepth=cellDepthRay(ne,ray);

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
int SageTraceLines1(mThread *Threads)
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
	struct rayData ray;
    tFlt *xcenter;
    tFlt *ycenter;
    tFlt *zcenter;
    unsigned char *level;
	tFlt *data;
    struct cell *nCell;
    unsigned char *nCellEnter;
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
		       if((Threads->smin == 0)){
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
	    /* if(j <= 79 || j >= 81)continue; */
	    for(i=0;i<scene->xResolution;++i){
		double ox;
		double oy;
		double oz;
	        a=scene->xmin+((double)i+.5)*scene->dx;
	        /* if(i <= 99 || i >= 101)continue; */
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
			    if(ss >= 0){
			        yy=Eye.y+ss*r.y;
			        zz=Eye.z+ss*r.z;
			        if(yy >= ymin && yy <= ymax && zz >= zmin && zz <= zmax){
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
			        }
			    }
		        }
		    }

		    if(r.y){
		        yy=ymin-dydj;
		        for(n=0;n<=iymax;++n){
		            yy += dydj;
			    ss=(yy-Eye.y)*oy;
			    if(ss >= 0){
			        xx=Eye.x+ss*r.x;
			        zz=Eye.z+ss*r.z;
			        if(xx >= xmin && xx <= xmax && zz >= zmin && zz <= zmax){
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
			        }
			    }
		        }
		    }

		    if(r.z){
		        zz=zmin-dzdk;
		        for(n=0;n<=izmax;++n){
		            zz += dzdk;
			    ss=(zz-Eye.z)*oz;
			    if(ss >= 0){
			        xx=Eye.x+ss*r.x;
			        yy=Eye.y+ss*r.y;
			        if(xx >= xmin && xx <= xmax && yy >= ymin && yy <= ymax){
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
			        }
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
			
			SageTraceCellHit(ne,&ray);

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
	    sprintf(WarningBuff,"SageTraceLines1 smin %ld smax %ld maxstep %ld\n",
	            Threads->smin,Threads->smax,maxstep);
		WarningBatch(WarningBuff);
	}
ErrorOut:
	if(nCell)cFree((char *)nCell);
	if(nCellEnter)cFree((char *)nCellEnter);

	Threads->done=TRUE;

	return 0;
}
int SageTraceCellHit(long ne,struct rayData *rayin)
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

/*

        {
             char dd[8];
	     struct cell cells[8];
             int nn;

             if(!SageCellHitDaughter(ne,ray,dd))return 0;

	    nn=0;
	    for(k=0;k<8;++k){
	        double dx,dy,dz;
	        long nd;
	        nd=k+(long)ray->daughter[ne]-1;
	        if(!ray->hasData[nd])continue;
	        if(dd[k]){
		    dx=ray->Eye.x-ray->xcenter[nd];
		    dy=ray->Eye.y-ray->ycenter[nd];
		    dz=ray->Eye.z-ray->zcenter[nd];
	            cells[nn].s=sqrt(dx*dx+dy*dy+dz*dz);
		    cells[nn++].ne=nd;
	        }
	    }

	    if(nn > 0){
	        qsort((char *)cells,nn, sizeof(struct cell),intcmp1);

	        for(k=0;k<nn;++k){
	            SageTraceCellHit(cells[k].ne,ray);
	        }
	    }

        }
*/



        {
             char dd[8];

             if(!SageCellHitDaughter(ne,ray,dd))return 0;

	    for(k=0;k<8;++k){
	        int nd;
	        nd=order[k+8*ray->RayDirection]+(int)ray->daughter[ne]-1;
	        if(!ray->hasData[nd])continue;
	        if(dd[order[k+8*ray->RayDirection]]){
	            SageTraceCellHit(nd,ray);
	        }
	    }

        }


/*
        {
	     struct cell cells[8];
             int nn;


	    nn=0;
	    for(k=0;k<8;++k){
	        double dx,dy,dz;
	        long nd;
	        nd=k+(long)ray->daughter[ne]-1;
	        if(!ray->hasData[nd])continue;
		if(SageCellHit(nd,ray)){
		    dx=ray->Eye.x-ray->xcenter[nd];
		    dy=ray->Eye.y-ray->ycenter[nd];
		    dz=ray->Eye.z-ray->zcenter[nd];
	            cells[nn].s=sqrt(dx*dx+dy*dy+dz*dz);
		    cells[nn++].ne=nd;
		}
	    }

	    if(nn > 0){
	        qsort((char *)cells,nn, sizeof(struct cell),intcmp1);

	        for(k=0;k<nn;++k){
	            SageTraceCellHit(cells[k].ne,ray);
	        }
	    }

        }
*/

	return 0;

ProcessRay:


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


	ray->cdata[ray->nc]   +=(ray->red[kc]*value*mopacity);
	ray->cdata[ray->nc+1] +=(ray->green[kc]*value*mopacity);
	ray->cdata[ray->nc+2] +=(ray->blue[kc]*value*mopacity);

	ray->opacity[ray->no] +=mopacity;
	if(ray->opacity[ray->no] >= 1.0)return 0;
	return 0;

}
int intcmp1(const void *xx,const  void *yy)
{
	double sum;
	register struct cell *x=(struct cell *)xx;
	register struct cell *y=(struct cell *)yy;
	sum = x->s - y->s;
	if(sum == 0.0)return 0;
	return (int)((sum > 0.0) ? 1 : -1);
}

int SageCellHit(long ne,struct rayData *rayin)
{
	register struct rayData *ray;
	double alpha;
	struct P r,Eye; 
	double x,y,z;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	int k;

	static long ierror;

	if(!rayin)return 0;

	ray=rayin;

	if(ne < 0 || ne >= ray->nemax){
	    if(++ierror < 10){
	        sprintf(WarningBuff,"ERROR : ne %ld nemax %ld\n",ne,ray->nemax);
		    if(rayin->debug > 1)WarningBatch(WarningBuff);
	    }
	   return 0;
	}

	r=ray->r;
	Eye=ray->Eye;

	k=(int)ray->level[ne];
	xmin=ray->xcenter[ne]-ray->dx2[k];
	xmax=ray->xcenter[ne]+ray->dx2[k];

	ymin=ray->ycenter[ne]-ray->dy2[k];
	ymax=ray->ycenter[ne]+ray->dy2[k];

	zmin=ray->zcenter[ne]-ray->dz2[k];
	zmax=ray->zcenter[ne]+ray->dz2[k];

	if(r.x){
	    double ox;
	    ox=1.0/r.x;
	    alpha=(xmin-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
		    return 1;
		  }
	        }
	    }
	    alpha=(xmax-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
		    return 1;
		  }
	        }
	    }
	}
	if(r.y){
	    double oy;
	    oy=1.0/r.y;
	    alpha=(ymin-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
		    return 1;
		  }
	        }
	    }
	    alpha=(ymax-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
		    return 1;
		  }
	        }
	    }
	}

	if(r.z){
	    double oz;
	    oz=1.0/r.z;
	    alpha=(zmin-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
		    return 1;
		  }
	        }
	    }
	    alpha=(zmax-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
		    return 1;
		  }
	        }
	    }
	}


	return 0;
}
int SageProcessRay(long ne,struct rayData *rayin)
{
	register struct rayData *ray;
	double nx,ny,nz;
	double mopacity;
	double value;
	double den;
	double lx,ly,lz;
	double d;
	long nomat;
	int kc;

	if(!rayin)return 1;
	ray=rayin;

	/* if(ne != 10624 && ne != 10640 && ne != 10544 && ne != 10560)return 0; */
	/* if(ne != 10178 && ne != 10240 && ne != 12256 && ne != 12272)return 0; */
	/* if(ne != 9512 && ne != 9514 && ne != 15264 && ne != 15266)return 0; */

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


	ray->cdata[ray->nc]   +=(ray->red[kc]*value*mopacity);
	ray->cdata[ray->nc+1] +=(ray->green[kc]*value*mopacity);
	ray->cdata[ray->nc+2] +=(ray->blue[kc]*value*mopacity);

	ray->opacity[ray->no] +=mopacity;
	if(ray->opacity[ray->no] >= 1.0)return 0;
	return 0;

}
int SageTraceCell(long ne,struct rayData *rayin,long *icount)
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
	double value;
	double den;
	double lx,ly,lz;
	double d;
	long nomat;
	int kc;

	if(!rayin)return 1;
	ray=rayin;

	++(*icount);

	if(ray->opacity[ray->no] >= 1.0)return 0;

	if(!ray->hasData[ne])return 0;


	if(!SageCellHit(ne,ray))return 0;


	if(!ray->daughter[ne])goto ProcessRay;

/*
	for(k=0;k<8;++k){
	    long nd;
	    nd=order[k+8*ray->RayDirection]+(long)ray->daughter[ne]-1;
	    if(!ray->hasData[nd])continue;
	    if(SageCellHit(nd,ray)){
	        SageTraceCell(nd,ray,icount);
	    }
	}
*/

/*

        {
             char d[8];
	     struct cell cells[8];
             int nn;

             if(!SageCellHitDaughter(ne,ray,d))return 0;

	    nn=0;
	    for(k=0;k<8;++k){
	        double dx,dy,dz;
	        long nd;
	        nd=k+(long)ray->daughter[ne]-1;
		if(!ray->hasData[nd])continue;
	        if(d[k]){
		    dx=ray->Eye.x-ray->xcenter[nd];
		    dy=ray->Eye.y-ray->ycenter[nd];
		    dz=ray->Eye.z-ray->zcenter[nd];
	            cells[nn].s=sqrt(dx*dx+dy*dy+dz*dz);
		    cells[nn++].ne=nd;
	        }
	    }

	    if(nn > 0){
	        qsort((char *)cells,nn, sizeof(struct cell),intcmp1);

	        for(k=0;k<nn;++k){
	            SageTraceCell(cells[k].ne,ray,icount);
	        }
	    }

        }

*/

        {
             char d[8];

            if(!SageCellHitDaughter(ne,ray,d))return 0;

	    for(k=0;k<8;++k){
	        long nd;
	        nd=order[k+8*ray->RayDirection]+(long)ray->daughter[ne]-1;
	        if(!ray->hasData[nd])continue;
	        if(d[order[k+8*ray->RayDirection]]){
	            SageTraceCell(nd,ray,icount);
	        }
	    }
	}

	return 0;

ProcessRay:
	/* if(ne != 10624 && ne != 10640 && ne != 10544 && ne != 10560)return 0; */
	/* if(ne != 10178 && ne != 10240 && ne != 12256 && ne != 12272)return 0; */
	/* if(ne != 9512 && ne != 9514 && ne != 15264 && ne != 15266)return 0; */

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


	ray->cdata[ray->nc]   +=(ray->red[kc]*value*mopacity);
	ray->cdata[ray->nc+1] +=(ray->green[kc]*value*mopacity);
	ray->cdata[ray->nc+2] +=(ray->blue[kc]*value*mopacity);

	ray->opacity[ray->no] +=mopacity;
	if(ray->opacity[ray->no] >= 1.0)return 0;
	return 0;

}
int SageTraceLines0(mThread *Threads)
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
	double x,y,z;
	double s1,s2;
	double didx,djdy,dkdz;
	long ixmax,iymax,izmax,iyxmax;
	long ystep,ostep;
	long samples;
	long maxstep;
	long i,j,nc,no;
	long nemax,icount;
	long ierror;
	long *location;
	int ihit;
        char *hasData;
	float *gradx;
	float *grady;
	float *gradz;
	double *daughter;
	struct rayData ray;
    tFlt *xcenter;
    tFlt *ycenter;
    tFlt *zcenter;
    unsigned char *level;
	tFlt *data;
    long maxtest;
    long totaltest;
/*
        int iprint;

*/
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


	Eye=scene->Eye;


	BackGround=scene->BackGround;

	ystep=3*scene->xResolution;
	ostep=scene->xResolution;;

	dmin = myIcon->dmin;
	dmax = myIcon->dmax;

	mat_scale=(((double)samples-1.0)/(dmax-dmin));

	icount=0;
	maxstep=0;
	ierror = 0;
	maxtest = 0;
	totaltest = 0;

	nemax=(ixmax*iymax*izmax);

        ray.hasData=hasData;
        ray.gradx=gradx;
        ray.grady=grady;
        ray.gradz=gradz;
        ray.daughter=daughter;

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
		       if((Threads->smin == 0)){
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
	    /* if(j <= 180 || j >= 229)continue; */
	    for(i=0;i<scene->xResolution;++i){
	        a=scene->xmin+((double)i+.5)*scene->dx;
	        /* if(i <= 275 || i >= 280)continue; */
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
		    alpha=(xmin-Eye.x)/r.x;
		    if(alpha >= 0){
		        y=Eye.y+alpha*r.y;
		        z=Eye.z+alpha*r.z;
		        if(y >= ymin && y <= ymax && z >= zmin && z <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		    alpha=(xmax-Eye.x)/r.x;
		    if(alpha >= 0){
		        y=Eye.y+alpha*r.y;
		        z=Eye.z+alpha*r.z;
		        if(y >= ymin && y <= ymax && z >= zmin && z <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		}
		if(r.y){
		    alpha=(ymin-Eye.y)/r.y;
		    if(alpha >= 0){
		        x=Eye.x+alpha*r.x;
		        z=Eye.z+alpha*r.z;
		        if(x >= xmin && x <= xmax && z >= zmin && z <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		    alpha=(ymax-Eye.y)/r.y;
		    if(alpha >= 0){
		        x=Eye.x+alpha*r.x;
		        z=Eye.z+alpha*r.z;
		        if(x >= xmin && x <= xmax && z >= zmin && z <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		}

		if(r.z){
		    alpha=(zmin-Eye.z)/r.z;
		    if(alpha >= 0){
		        x=Eye.x+alpha*r.x;
		        y=Eye.y+alpha*r.y;
		        if(x >= xmin && x <= xmax && y >= ymin && y <= ymax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		    alpha=(zmax-Eye.z)/r.z;
		    if(alpha >= 0){
		        x=Eye.x+alpha*r.x;
		        y=Eye.y+alpha*r.y;
		        if(x >= xmin && x <= xmax && y >= ymin && y <= ymax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		}
		nc=3*i+ystep*j;
		if(!ihit){
		    cdata[nc]=BackGround.red;
		    cdata[nc+1]=BackGround.green;
		    cdata[nc+2]=BackGround.blue;
		    continue;
		}


		no=i+j*ostep;

		{
		    double xs,ys,zs,xe,ye,ze;
		    int x2,y2,z2;
		    long nl,ne;
		    int n, sx, sy, sz, exy, exz, ezy,
		           ax, ay, az, bx, by, bz;
		    int x, y, z, dx, dy, dz;


		    if(s1 >= s2){
		        s1=0;
		    }

		    xs=Eye.x+s1*r.x;
		    ys=Eye.y+s1*r.y;
		    zs=Eye.z+s1*r.z;

		    xe=Eye.x+s2*r.x;
		    ye=Eye.y+s2*r.y;
		    ze=Eye.z+s2*r.z;




		    x=(int)((xs-xmin)*didx);
		    if(x >= ixmax)x = (int)(ixmax - 1);
		    y=(int)((ys-ymin)*djdy);
		    if(y >= iymax)y = (int)(iymax - 1);
		    z=(int)((zs-zmin)*dkdz);
		    if(z >= izmax)z = (int)(izmax - 1);

		    x2=(int)((xe-xmin)*didx);
		    if(x2 >= ixmax)x2 = (int)(ixmax - 1);
		    y2=(int)((ye-ymin)*djdy);
		    if(y2 >= iymax)y2 = (int)(iymax - 1);
		    z2=(int)((ze-zmin)*dkdz);
		    if(z2 >= izmax)z2 = (int)(izmax - 1);

		    dx=x2-x;
		    dy=y2-y;
		    dz=z2-z;

		    sx = sgn(dx);  sy = sgn(dy);  sz = sgn(dz);
		    ax = abs(dx);  ay = abs(dy);  az = abs(dz);
		    bx = 2*ax;     by = 2*ay;     bz = 2*az;
		    exy = ay-ax;   exz = az-ax;   ezy = ay-az;
		    n = ax+ay+az+1;
		    
		    if(n > maxstep)maxstep=n;



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

		
		    nl = -1;
		    while(n-- > 0){

		        ne=x+y*ixmax+z*iyxmax;

		        if(ne == nl)goto UpdateVariables;

			if(ne < 0 || ne >= nemax){
			    if(++ierror < 10){
			        sprintf(WarningBuff,"ERROR : ne %ld x %d y %d z %d\n",ne,x,y,z);
					if(scene->debug > 1)WarningBatch(WarningBuff);
			    }
			    goto UpdateVariables;
			}

		        nl=ne;

			ne=location[ne];


/*
			if(SageCellHit1(8071,&ray) && ne == 1376){
			     
			    opacity[no] = 1; 
		            cdata[nc]   = red[254];
		            cdata[nc+1] = green[254];
		            cdata[nc+2] = blue[254];
			    break;
			    
			}else{
			    break;
			}
*/

			if(!hasData[ne])goto UpdateVariables;


			icount=0;

			SageTraceCell(ne,&ray,&icount);


			if(icount > maxtest)maxtest = icount;
			totaltest += icount;
		        if(opacity[no] >= 1.0){
		              goto TraceIsDone;
		        }
UpdateVariables:
		            if ( exy < 0 ) {
		                if ( exz < 0 ) {
		                    x += sx;
		                    exy += by; exz += bz;
		                }
		                else  {
		                    z += sz;
		                    exz -= bx; ezy += by;
		                }
		            }
		            else {
		                if ( ezy < 0 ) {
		                    z += sz;
		                    exz -= bx; ezy += by;
		                }
		                else  {
		                    y += sy;
		                    exy -= bx; ezy -= bz;
		                }
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
	    sprintf(WarningBuff,"SageTraceLines0 !!!! smin %ld smax %ld maxstep %ld maxtest %ld totaltest %ld direction %ld\n",
	       Threads->smin,Threads->smax,maxstep,maxtest,totaltest,(long)(scene->RayDirection));
		WarningBatch(WarningBuff);
	}
	
	Threads->done=TRUE;
	
	return 0;
}
int SageTraceLines2(mThread *Threads)
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
	double s1,s2;
	double didx,djdy,dkdz;
	long ixmax,iymax,izmax,iyxmax;
	long ystep,ostep;
	long samples;
	long maxstep;
	long i,j,nc,no;
	long nemax,icount;
	long ierror;
	long *location;
	int ihit;
        char *hasData;
	float *gradx;
	float *grady;
	float *gradz;
	double *daughter;
	struct rayData ray;
    tFlt *xcenter;
    tFlt *ycenter;
    tFlt *zcenter;
    unsigned char *level;
	tFlt *data;
    double *sCell;
    long   *nCell;
    long sCount;
    long sCountMax;
    long maxtest;
    long totaltest;
    long length;
    double dxdi;
    double dydj;
    double dzdk;
    long nn;

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



	sCell=NULL;
	nCell=NULL;

	sCell=(double *)cMalloc(sizeof(double)*length,29725);
	nCell=(long *)cMalloc(sizeof(long)*length,29725);

	sCountMax=0;

	if(!sCell || !nCell)goto ErrorOut;


	Eye=scene->Eye;


	BackGround=scene->BackGround;

	ystep=3*scene->xResolution;
	ostep=scene->xResolution;;

	dmin = myIcon->dmin;
	dmax = myIcon->dmax;

	mat_scale=(((double)samples-1.0)/(dmax-dmin));

	icount=0;
	maxstep=0;
	ierror = 0;
	maxtest = 0;
	totaltest = 0;

	nemax=(ixmax*iymax*izmax);

        ray.hasData=hasData;
        ray.gradx=gradx;
        ray.grady=grady;
        ray.gradz=gradz;
        ray.daughter=daughter;

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
		       if((Threads->smin == 0)){
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
	    /* if(j <= 226 || j >= 229)continue; */
	    for(i=0;i<scene->xResolution;++i){
	        a=scene->xmin+((double)i+.5)*scene->dx;
	        /* if(i <= 313 || i >= 316)continue; */
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
		    alpha=(xmin-Eye.x)/r.x;
		    if(alpha >= 0){
		        yy=Eye.y+alpha*r.y;
		        zz=Eye.z+alpha*r.z;
		        if(yy >= ymin && yy <= ymax && zz >= zmin && zz <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		    alpha=(xmax-Eye.x)/r.x;
		    if(alpha >= 0){
		        yy=Eye.y+alpha*r.y;
		        zz=Eye.z+alpha*r.z;
		        if(yy >= ymin && yy <= ymax && zz >= zmin && zz <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		}
		if(r.y){
		    alpha=(ymin-Eye.y)/r.y;
		    if(alpha >= 0){
		        xx=Eye.x+alpha*r.x;
		        zz=Eye.z+alpha*r.z;
		        if(xx >= xmin && xx <= xmax && zz >= zmin && zz <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		    alpha=(ymax-Eye.y)/r.y;
		    if(alpha >= 0){
		        xx=Eye.x+alpha*r.x;
		        zz=Eye.z+alpha*r.z;
		        if(xx >= xmin && xx <= xmax && zz >= zmin && zz <= zmax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		}

		if(r.z){
		    alpha=(zmin-Eye.z)/r.z;
		    if(alpha >= 0){
		        xx=Eye.x+alpha*r.x;
		        yy=Eye.y+alpha*r.y;
		        if(xx >= xmin && xx <= xmax && yy >= ymin && yy <= ymax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		    alpha=(zmax-Eye.z)/r.z;
		    if(alpha >= 0){
		        xx=Eye.x+alpha*r.x;
		        yy=Eye.y+alpha*r.y;
		        if(xx >= xmin && xx <= xmax && yy >= ymin && yy <= ymax){
		            if(alpha < s1 )s1 = alpha;
		            if(alpha > s2 )s2 = alpha;
			    ihit=1;
		        }
		    }
		}
		nc=3*i+ystep*j;
		if(!ihit){
		    cdata[nc]=BackGround.red;
		    cdata[nc+1]=BackGround.green;
		    cdata[nc+2]=BackGround.blue;
		    continue;
		}


		no=i+j*ostep;

		{
		    long nl,ne;
		    int n;

		    sCount=0;
		    if(r.x){
		        for(n=0;n<=ixmax;++n){
		            xx=xmin+(double)n*dxdi;
			    alpha=(xx-Eye.x)/r.x;
			    if(alpha >= 0){
			        yy=Eye.y+alpha*r.y;
			        zz=Eye.z+alpha*r.z;
			        if(yy >= ymin && yy <= ymax && zz >= zmin && zz <= zmax){
			            sCell[sCount++] = alpha;
			        }
			    }
		        }
		    }

		    if(r.y){
		        for(n=0;n<=iymax;++n){
		            yy=ymin+(double)n*dydj;
			    alpha=(yy-Eye.y)/r.y;
			    if(alpha >= 0){
			        xx=Eye.x+alpha*r.x;
			        zz=Eye.z+alpha*r.z;
			        if(xx >= xmin && xx <= xmax && zz >= zmin && zz <= zmax){
			            sCell[sCount++] = alpha;
			        }
			    }
		        }
		    }

		    if(r.z){
		        for(n=0;n<=izmax;++n){
		            zz=zmin+(double)n*dzdk;
			    alpha=(zz-Eye.z)/r.z;
			    if(alpha >= 0){
			        xx=Eye.x+alpha*r.x;
			        yy=Eye.y+alpha*r.y;
			        if(xx >= xmin && xx <= xmax && yy >= ymin && yy <= ymax){
			            sCell[sCount++] = alpha;
			        }
			    }
		        }
		    }


		    if(!sCount)goto TraceIsDone;


		    nl = -1;
		    nn=0;
		    for(n=0;n<sCount;++n){
		        int x,y,z;

			s1=sCell[n];
		
		        xx=Eye.x+s1*r.x;
			x=(int)((xx-xmin)*didx);
			if(x < 0){
			    x = 0;
			}else if(x >= ixmax){
			    x = (int)(ixmax - 1);
			}

		        yy=Eye.y+s1*r.y;
			y=(int)((yy-ymin)*djdy);
			if(y < 0){
			    y = 0;
			}else if(y >= iymax){
			    y = (int)(iymax - 1);
			}

		        zz=Eye.z+s1*r.z;
			z=(int)((zz-zmin)*dkdz);
			if(z < 0){
			    z = 0;
			}else if(z >= izmax){
			    z = (int)(izmax - 1);
			}

		        ne=x+y*ixmax+z*iyxmax;

			if(nl == ne)continue;
			if(ne < 0 || ne >= nemax)continue;
			if(!hasData[location[ne]])continue;
		        sCell[nn++]=sCell[n];
		        nl = ne;
		    }

		    sCount=nn;

		    if(!sCount)goto TraceIsDone;

		    qsort((char *)sCell,sCount, sizeof(double),intcmp2);

		    ray.no=no;
		    ray.nc=nc;
		    ray.r=r;

		    nl = -1;
		    nn=0;
		    for(n=0;n<sCount;++n){
		        int x,y,z;

			s1=sCell[n];
		
		        xx=Eye.x+s1*r.x;
			x=(int)((xx-xmin)*didx);
			if(x < 0){
			    x = 0;
			}else if(x >= ixmax){
			    x = (int)(ixmax - 1);
			}

		        yy=Eye.y+s1*r.y;
			y=(int)((yy-ymin)*djdy);
			if(y < 0){
			    y = 0;
			}else if(y >= iymax){
			    y = (int)(iymax - 1);
			}

		        zz=Eye.z+s1*r.z;
			z=(int)((zz-zmin)*dkdz);
			if(z < 0){
			    z = 0;
			}else if(z >= izmax){
			    z = (int)(izmax - 1);
			}

		        ne=x+y*ixmax+z*iyxmax;

			if(nl == ne)continue;
			if(ne < 0 || ne >= nemax)continue;
			if(!hasData[location[ne]])continue;
		        nCell[nn++]=ne;
		        nl = ne;
		    }

		    sCount=nn;

		    if(sCount > maxstep)maxstep=sCount;


		    nl = -1;
		    for(n=0;n<sCount;++n){

		        ne=nCell[n];

		        if(ne == nl)continue;

			if(ne < 0 || ne >= nemax){
			 
			    if(++ierror < 10){
			        sprintf(WarningBuff,"ERROR : ne %ld\n",ne);
					if(scene->debug > 1)WarningBatch(WarningBuff);
			    }
			    continue;
			}

		        nl=ne;

			ne=location[ne];

			
			if(!hasData[ne])continue;

			icount=0;

			SageTraceCell(ne,&ray,&icount);


			if(icount > maxtest)maxtest = icount;
			totaltest += icount;
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
	    sprintf(WarningBuff,"SageTraceLines2 !!!! smin %ld smax %ld maxstep %ld maxtest %ld totaltest %ld sCountMax %ld\n",
	            Threads->smin,Threads->smax,maxstep,maxtest,totaltest,(long)sCountMax);
		WarningBatch(WarningBuff);
	}
ErrorOut:
	if(sCell)cFree((char *)sCell);
	if(nCell)cFree((char *)nCell);
	
	Threads->done=TRUE;

	return 0;
}
int intcmp2(const void *xx,const  void *yy)
{
	register double sum;
	register double *x=(double *)xx;
	register double *y=(double *)yy;
	sum= *x - *y;
	if(!sum)return 0;
	return (int)((sum > 0.0) ? 1 : -1);
}
int SageTraceLines5(mThread *Threads)
{

	struct vScene *scene;
	vIconPtr myIcon;
	struct PIO *sage;
	SagePtr pio;
	double red[256],green[256],blue[256];
	struct Color BackGround;
	struct P r,Eye;
	struct P Lamp;
	double *cdata,*opacity,*mat_opacity;
	double mat_scale;
	double xmin,xmax;
	double ymin,ymax;
	double zmin,zmax;
	double dmin,dmax;
	double alpha;
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
	struct rayData ray;
    tFlt *xcenter;
    tFlt *ycenter;
    tFlt *zcenter;
    unsigned char *level;
	tFlt *data;
    struct cell *nCell;
    unsigned char *nCellEnter;
    long sCount;
    long length;
    long nn;
    double dxdi;
    double dydj;
    double dzdk;
	int xReflect,yReflect,zReflect;
	long ixmaxNoReflect,iymaxNoReflect,izmaxNoReflect,iyxmaxNoReflect;
	double thetaStep,phiStep;
	double *scratch;
	double pi180;
	long CellCountMax;
	char name[256];
	FILE *out;
	int n;

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

	maxstep=0;
	ierror=0;

	sprintf(name,"%s.%02d.%s",myIcon->filename,Threads->nthread,myIcon->dataname);
	out=fopen(name,"w");
	if(out == NULL){
	    sprintf(WarningBuff,"SageTraceLines5 Error Opening %s To Write\n",name);
		WarningBatch(WarningBuff);
	    return 1;
	}

        FindTraceOrder(myIcon);

	getPalDouble(scene->palname,red,green,blue);

	Lamp=scene->Lamp;

	xReflect=scene->xReflect;
	yReflect=scene->yReflect;
	zReflect=scene->zReflect;

	xmin=pio->xmin;
	xmax=pio->xmax;

	ixmaxNoReflect=sage->ixmax;
	if(xReflect){
	    ixmax=2*ixmaxNoReflect;
	    xmin = -xmax;
	}else{
	    ixmax=ixmaxNoReflect;
	}

	didx=(double)(ixmax)/(xmax-xmin);
	dxdi=1.0/didx;

	ymin=pio->ymin;
	ymax=pio->ymax;


	iymaxNoReflect=sage->iymax;
	if(yReflect){
	    iymax=2*iymaxNoReflect;
	    ymin = -ymax;
	}else{
	    iymax=iymaxNoReflect;
	}

	djdy=(double)(iymax)/(ymax-ymin);
	dydj=1.0/djdy;

	zmin=pio->zmin;
	zmax=pio->zmax;

	izmaxNoReflect=sage->izmax;
	if(zReflect){
	    izmax=2*izmaxNoReflect;
	    zmin = -zmax;
	}else{
	    izmax=izmaxNoReflect;
	}

	dkdz=(double)(izmax)/(zmax-zmin);
	dzdk=1.0/dkdz;

	iyxmax=ixmax*iymax;
	iyxmaxNoReflect=ixmaxNoReflect*iymaxNoReflect;

	length=2*(ixmax+iymax+izmax+3);

	nCell=NULL;

	nCellEnter=NULL;

	scratch=NULL;

	nCell=(struct cell *)cMalloc(sizeof(struct cell)*length,29725);

	if(!nCell)goto ErrorOut;

	Eye=scene->Eye;

	if((scene->groupThetan <= 1) || (scene->groupPhin <= 1)){
	      sprintf(WarningBuff,"groupTrace Error groupThetan %ld groupPhin%ld\n",
	              scene->groupThetan,scene->groupPhin);
		  if(scene->debug > 1)WarningBatch(WarningBuff);
	      goto ErrorOut;
	}

	if(out && (Threads->smin == 0)){
	    sprintf(WarningBuff,"thetacount %5ld phicount %5ld Eye %12.4e %12.4e %12.4e\n",
	          scene->groupThetan,scene->groupPhin,Eye.x,Eye.y,Eye.z);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}

        thetaStep=(scene->groupTheta2-scene->groupTheta1)/((double)(scene->groupThetan)-1.0);
        phiStep=(scene->groupPhi2-scene->groupPhi1)/((double)(scene->groupPhin)-1.0);

	BackGround=scene->BackGround;

	ystep=3*scene->xResolution;
	ostep=scene->xResolution;

	dmin = myIcon->dmin;
	dmax = myIcon->dmax;

	mat_scale=(((double)samples-1.0)/(dmax-dmin));

	maxstep=0;
	ierror = 0;

	nemax=(ixmax*iymax*izmax);


	nCellEnter=(unsigned char *)cMalloc(sizeof(unsigned char)*nemax,29725);

	if(!nCellEnter)goto ErrorOut;

	zerol((char *)&ray,sizeof(ray));

    ray.hasData=hasData;
    ray.gradx=gradx;
    ray.grady=grady;
    ray.gradz=gradz;
    ray.daughter=daughter;

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
	       if((Threads->smin == 0)){
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

	ray.CellCountMax=9000;

	scratch=(double *)cMalloc(sizeof(double)*ray.CellCountMax*3,29755);

	if(!scratch)goto ErrorOut;

	ray.r1=scratch;
	ray.r2=scratch+ray.CellCountMax;
	ray.v=scratch+2*ray.CellCountMax;
	ray.CellCount=0;

        ray.nemax=pio->numcell;

        ray.level=level;

	if(myIcon->CellSize){
	    ray.CellSizeOver=1.0/myIcon->CellSize;
	}else{
	    ray.CellSizeOver=1.0;
	}



	ray.RayDirection=scene->RayDirection;

	ray.cellDepthError=0;

	pi180=4.0*atan(1.0)/180.0;

	CellCountMax=0;

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
	    double phi,theta;
	    phi=scene->groupPhi1+(double)(j)*phiStep;
	    /* if(j <= 359 || j >= 361)continue; */
	    for(i=0;i<scene->groupThetan;++i){
		double ox;
		double oy;
		double oz;
	        /* if(i <= 403 || i >= 406)continue; */


		ray.CellCount=0;
		theta=scene->groupTheta1+(double)(i)*thetaStep;
	    r.x=cos(theta*pi180)*sin(phi*pi180);
		r.y=sin(theta*pi180)*sin(phi*pi180);
		r.z=cos(phi*pi180);
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


		no=i+j*ostep;

		{
		    long ne;
		    int x,y,z;

		    sCount=0;
		    if(r.x){
		        xx=xmin-dxdi;
		        for(n=0;n<=ixmax;++n){
		            xx += dxdi;
			    ss=(xx-Eye.x)*ox;
			    if(ss >= 0){
			        yy=Eye.y+ss*r.y;
			        zz=Eye.z+ss*r.z;
			        if(yy >= ymin && yy <= ymax && zz >= zmin && zz <= zmax){
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
			        }
			    }
		        }
		    }

		    if(r.y){
		        yy=ymin-dydj;
		        for(n=0;n<=iymax;++n){
		            yy += dydj;
			    ss=(yy-Eye.y)*oy;
			    if(ss >= 0){
			        xx=Eye.x+ss*r.x;
			        zz=Eye.z+ss*r.z;
			        if(xx >= xmin && xx <= xmax && zz >= zmin && zz <= zmax){
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
			        }
			    }
		        }
		    }

		    if(r.z){
		        zz=zmin-dzdk;
		        for(n=0;n<=izmax;++n){
		            zz += dzdk;
			    ss=(zz-Eye.z)*oz;
			    if(ss >= 0){
			        xx=Eye.x+ss*r.x;
			        yy=Eye.y+ss*r.y;
			        if(xx >= xmin && xx <= xmax && yy >= ymin && yy <= ymax){
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
			        }
			    }
		        }
		    }


		    if(!sCount)goto TraceIsDone;

		    nn = 0;
		    for(n=0;n<sCount;++n){
		        double dx,dy,dz;
		        long nex,ney,nez;
		        long xe,ye,ze;
		        long nee;
				unsigned char xReflectFlag;
				unsigned char yReflectFlag;
				unsigned char zReflectFlag;

		        ne=nCell[n].ne;
	
		        if(nn > 0 && (ne == nCell[nn-1].ne)){
		            continue;
		        }
	
			if(ne < 0 || ne >= nemax){++ierror;continue;}


			nez=ne/iyxmax;
			ney=(ne-nez*iyxmax)/ixmax;
			nex=ne-nez*iyxmax-ney*ixmax;

			xReflectFlag=0;

			if(xReflect){
			    if(nex < ixmaxNoReflect){
			        xe=(long)ixmaxNoReflect-nex-1;
			        xReflectFlag=1;
			    }else{
			        xe=nex-(long)ixmaxNoReflect;
			    }
			}else{
			    xe=nex;
			}

			yReflectFlag=0;
			if(yReflect){
			    if(ney < iymaxNoReflect){
			        ye=(long)iymaxNoReflect-ney-1;
			        yReflectFlag=1;
			    }else{
			        ye=ney-(long)iymaxNoReflect;
			    }
			}else{
			    ye=ney;
			}

			zReflectFlag=0;
			if(zReflect){
			    if(nez < izmaxNoReflect){
			        ze=(long)izmaxNoReflect-nez-1;
			        zReflectFlag=1;
			    }else{
			        ze=nez-(long)izmaxNoReflect;
			    }
			}else{
			    ze=nez;
			}

		        nee=xe+ye*ixmaxNoReflect+ze*iyxmaxNoReflect;

			/* if(!hasData[location[nee]])continue; */

			dx=Eye.x-(((double)nex+.5)*dxdi+xmin);
			dy=Eye.y-(((double)ney+.5)*dydj+ymin);
			dz=Eye.z-(((double)nez+.5)*dzdk+zmin);

			nCell[nn].s = dx*dx+dy*dy+dz*dz;
			nCell[nn].nee = nee;
			nCell[nn].xReflect = xReflectFlag;
			nCell[nn].yReflect = yReflectFlag;
			nCell[nn].zReflect = zReflectFlag;
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

			ne=location[nCell[n].nee];
			ray.xReflect=nCell[n].xReflect;
			ray.yReflect=nCell[n].yReflect;
			ray.zReflect=nCell[n].zReflect;

			
			SageDepthReflect5(ne,&ray);
                     /*
			if(opacity[no] >= 1.0){
		              goto TraceIsDone;
		        }
                     */
		    }
TraceIsDone:
		    if(opacity[no] < 1.0){
		        cdata[nc]   +=(BackGround.red*(1.0-opacity[no]));
		        cdata[nc+1] +=(BackGround.green*(1.0-opacity[no]));
		        cdata[nc+2] +=(BackGround.blue*(1.0-opacity[no]));
		    }
		}
		if(CellCountMax < ray.CellCount)CellCountMax=ray.CellCount;
	        sprintf(WarningBuff,"theta %12.4e phi %12.4e count %5ld \n",theta,phi,ray.CellCount);
			WarningBatch(WarningBuff);
	        for(n=0;n<ray.CellCount;++n){
	            sprintf(WarningBuff,"%12.4e %12.4e %12.4e\n",ray.r1[n],ray.r2[n],ray.v[n]);
				WarningBatch(WarningBuff);
	        }
	    }
	}


	/* if(scene->debug > 0) */

	    sprintf(WarningBuff,"SageTraceLines5 smin %ld smax %ld maxstep %ld CellCountMax %ld cellDepthError %ld\n",
	       Threads->smin,Threads->smax,maxstep,CellCountMax,ray.cellDepthError);
		if(scene->debug > 1)WarningBatch(WarningBuff);
/*
	    sprintf(WarningBuff,"END SageTraceLines5 smin %ld smax %ld \n",myIcon->smin,myIcon->smax);
		WarningBatch(WarningBuff);
*/
ErrorOut:
	if(ierror > 0)
	    sprintf(WarningBuff,"SageTraceLines5 smin %ld smax %ld maxstep %ld ierror %ld\n",
	       Threads->smin,Threads->smax,maxstep,ierror);
		WarningBatch(WarningBuff);


	if(nCell)cFree((char *)nCell);
	if(nCellEnter)cFree((char *)nCellEnter);
	if(out)fclose(out);
	if(scratch)cFree((char *)scratch);
	Threads->done=TRUE;


	return 0;
}
int SageDepthReflect5(long ne,struct rayData *rayin)
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
             7,5,6,4,  3,1,2,0,

             1,0,3,2,  5,4,7,6,
             0,1,2,3,  4,5,6,7,
             3,1,2,0,  7,6,5,4,
             2,0,3,1,  6,4,7,5,
             5,4,7,6,  1,0,3,2,
             4,5,6,7,  0,1,2,3,
             7,5,6,4,  3,2,1,0,
             6,4,7,5,  2,0,3,1,

             2,3,0,1,  6,7,4,5,
             3,2,1,0,  7,6,5,4,
             0,2,1,3,  4,5,6,7,
             1,3,0,2,  5,7,4,6,
             6,7,4,5,  2,3,0,1,
             7,6,5,4,  3,2,1,0,
             4,6,5,7,  0,1,2,3,
             5,7,4,6,  1,3,0,2,

             3,2,1,0,  7,6,5,4,
             2,3,0,1,  6,7,4,5,
             1,3,0,2,  5,4,7,6,
             0,2,1,3,  4,6,5,7,
             7,6,5,4,  3,2,1,0,
             6,7,4,5,  2,3,0,1,
             5,7,4,6,  1,0,3,2,
             4,6,5,7,  0,2,1,3,

             4,5,6,7,  0,1,2,3,
             5,4,7,6,  1,0,3,2,
             6,4,7,5,  2,3,0,1,
             7,5,6,4,  3,1,2,0,
             0,1,2,3,  4,5,6,7,
             1,0,3,2,  5,4,7,6,
             2,0,3,1,  6,7,4,5,
             3,1,2,0,  7,5,6,4,

             5,4,7,6,  1,0,3,2,
             4,5,6,7,  0,1,2,3,
             7,5,6,4,  3,2,1,0,
             6,4,7,5,  2,0,3,1,
             1,0,3,2,  5,4,7,6,
             0,1,2,3,  4,5,6,7,
             3,1,2,0,  7,6,5,4,
             2,0,3,1,  6,4,7,5,

             7,6,5,4,  3,2,1,0,
             6,7,4,5,  2,3,0,1,
             5,7,4,6,  1,0,3,2,
             4,6,5,7,  0,2,1,3,
             3,2,1,0,  7,6,5,4,
             2,3,0,1,  6,7,4,5,
             1,3,0,2,  5,4,7,6,
             0,2,1,3,  4,6,5,7,
                             };
	int k;
	double nx,ny,nz;
	double xe,ye,ze;
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

     /*
	if(ray->opacity[ray->no] >= 1.0)return 0;

	if(!ray->hasData[ne])return 0;
     */
	if(!ray->daughter[ne])goto ProcessRay;


        {
             char dd[8];
             int nreflect;

             if(!SageHitDaughterReflect(ne,ray,dd))return 0;

	    nreflect=ray->xReflect+2*ray->yReflect+4*ray->zReflect;
	    for(k=0;k<8;++k){
	        long nd;
	        nd=order[k+8*ray->RayDirection+nreflect*64]+(long)ray->daughter[ne]-1;
	     /*
	        if(!ray->hasData[nd])continue;
	     */
	        if(dd[order[k+8*ray->RayDirection]]){
	            SageDepthReflect5(nd,ray);
	        }
	    }

        }


	return 0;

ProcessRay:


	cellDepth=cellDepthRayReflect5(ne,ray);

	if(ray->opacity[ray->no] >= 1.0)return 0;

	if(!ray->hasData[ne])return 0;

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

	if(ray->xReflect){
	    nx = -ray->gradx[ne];
	    xe = -ray->xcenter[ne];
	}else{
	    nx =  ray->gradx[ne];
	    xe =  ray->xcenter[ne];
	}

	if(ray->yReflect){
	    ny = -ray->grady[ne];
	    ye = -ray->ycenter[ne];
	}else{
	    ny =  ray->grady[ne];
	    ye =  ray->ycenter[ne];
	}

	if(ray->zReflect){
	    nz = -ray->gradz[ne];
	    ze = -ray->zcenter[ne];
	}else{
	    nz =  ray->gradz[ne];
	    ze =  ray->zcenter[ne];
	}

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

	   lx=(ray->Lamp.x-xe);
	   ly=(ray->Lamp.y-ye);
	   lz=(ray->Lamp.z-ze);
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


	ray->cdata[ray->nc]   +=(ray->red[kc]*value*mopacity);
	ray->cdata[ray->nc+1] +=(ray->green[kc]*value*mopacity);
	ray->cdata[ray->nc+2] +=(ray->blue[kc]*value*mopacity);

	ray->opacity[ray->no] +=mopacity;
	if(ray->opacity[ray->no] >= 1.0)return 0;
	return 0;

}
double cellDepthRayReflect5(long ne,struct rayData *rayin)
{
	register struct rayData *ray;
	double cellDepth;
	double alpha,dx,dy,dz;
	double xx[6],yy[6],zz[6],ss[6];
	struct P r,Eye; 
	double xc,yc,zc;
	double r1,r2;
	double x,y,z;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	static long ierror;
	int np,k,n;

	if(!rayin)return 0.0;

	ray=rayin;

	if(ne < 0 || ne >= ray->nemax){
	    if(++ierror < 10){
	        sprintf(WarningBuff,"cellDepthRayReflect5 ERROR : ne %ld nemax %ld\n",ne,ray->nemax);
			if(rayin->debug > 1)WarningBatch(WarningBuff);
	    }
	   return 0.0;
	}

	r=ray->r;
	Eye=ray->Eye;

	k=(int)ray->level[ne];
	if(ray->xReflect){
	    xc = -ray->xcenter[ne];
	}else{
	    xc =  ray->xcenter[ne];
	}
	xmin=xc-ray->dx2[k];
	xmax=xc+ray->dx2[k];

	if(ray->yReflect){
	    yc = -ray->ycenter[ne];
	}else{
	    yc =  ray->ycenter[ne];
	}
	ymin=yc-ray->dy2[k];
	ymax=yc+ray->dy2[k];

	if(ray->zReflect){
	    zc = -ray->zcenter[ne];
	}else{
	    zc =  ray->zcenter[ne];
	}
	zmin=zc-ray->dz2[k];
	zmax=zc+ray->dz2[k];

	np=0;

	if(r.x){
	    double ox;
	    ox=1.0/r.x;
	    alpha=(xmin-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
		    xx[np]=xmin;
		    yy[np]=y;
		    zz[np]=z;
		    ss[np]=alpha;
		    ++np;
		  }
	        }
	    }
	    alpha=(xmax-Eye.x)*ox;
	    if(alpha >= 0){
	        y=Eye.y+alpha*r.y;
	        if(y >= ymin && y <= ymax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
		    xx[np]=xmax;
		    yy[np]=y;
		    zz[np]=z;
		    ss[np]=alpha;
		    ++np;
		  }
	        }
	    }
	}
	if(r.y){
	    double oy;
	    oy=1.0/r.y;
	    alpha=(ymin-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
		    xx[np]=x;
		    yy[np]=ymin;
		    zz[np]=z;
		    ss[np]=alpha;
		    ++np;
		  }
	        }
	    }
	    alpha=(ymax-Eye.y)*oy;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          z=Eye.z+alpha*r.z;
	          if(z >= zmin && z <= zmax){
		    xx[np]=x;
		    yy[np]=ymax;
		    zz[np]=z;
		    ss[np]=alpha;
		    ++np;
		  }
	        }
	    }
	}

	if(r.z){
	    double oz;
	    oz=1.0/r.z;
	    alpha=(zmin-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
		    xx[np]=x;
		    yy[np]=y;
		    zz[np]=zmin;
		    ss[np]=alpha;
		    ++np;
		  }
	        }
	    }
	    alpha=(zmax-Eye.z)*oz;
	    if(alpha >= 0){
	        x=Eye.x+alpha*r.x;
	        if(x >= xmin && x <= xmax){
	          y=Eye.y+alpha*r.y;
	          if(y >= ymin && y <= ymax){
		    xx[np]=x;
		    yy[np]=y;
		    zz[np]=zmax;
		    ss[np]=alpha;
		    ++np;
		  }
	        }
	    }
	}

	if(np == 2){
doTwo:
	    dx=xx[1]-Eye.x;
	    dy=yy[1]-Eye.y;
	    dz=zz[1]-Eye.z;
	    r1=sqrt(dx*dx+dy*dy+dz*dz);
	    dx=xx[0]-Eye.x;
	    dy=yy[0]-Eye.y;
	    dz=zz[0]-Eye.z;
	    r2=sqrt(dx*dx+dy*dy+dz*dz);
	    dx=xx[1]-xx[0];
	    dy=yy[1]-yy[0];
	    dz=zz[1]-zz[0];
	}else if(np == 1){
	    r1=0;
	    dx=Eye.x-xx[0];
	    dy=Eye.y-yy[0];
	    dz=Eye.z-zz[0];
	    r2=sqrt(dx*dx+dy*dy+dz*dz);
	}else if(np == 3){
	    if(fabs(ss[0]-ss[2]) > fabs(ss[1]-ss[2])){
	        dx=xx[2]-Eye.x;
	        dy=yy[2]-Eye.y;
	        dz=zz[2]-Eye.z;
	        r1=sqrt(dx*dx+dy*dy+dz*dz);
	        dx=xx[0]-Eye.x;
	        dy=yy[0]-Eye.y;
	        dz=zz[0]-Eye.z;
	        r2=sqrt(dx*dx+dy*dy+dz*dz);
	        dx=xx[0]-xx[2];
	        dy=yy[0]-yy[2];
	        dz=zz[0]-zz[2];
	    }else{
	        dx=xx[2]-Eye.x;
	        dy=yy[2]-Eye.y;
	        dz=zz[2]-Eye.z;
	        r1=sqrt(dx*dx+dy*dy+dz*dz);
	        dx=xx[1]-Eye.x;
	        dy=yy[1]-Eye.y;
	        dz=zz[1]-Eye.z;
	        r2=sqrt(dx*dx+dy*dy+dz*dz);
	        dx=xx[1]-xx[2];
	        dy=yy[1]-yy[2];
	        dz=zz[1]-zz[2];
	    }
	}else if(np >= 4){
	    double smin,smax;
	    smin = ss[0];
	    smax = ss[0];
	    for(n=1;n<np;++n){
	        if(smin > ss[n])smin = ss[n];
	        if(smax < ss[n])smax = ss[n];
	    }
	    xx[0]=Eye.x+smin*r.x;
	    yy[0]=Eye.y+smin*r.y;
	    zz[0]=Eye.z+smin*r.z;

	    xx[1]=Eye.x+smax*r.x;
	    yy[1]=Eye.y+smax*r.y;
	    zz[1]=Eye.z+smax*r.z;
	    goto doTwo;
	}else{
	    if(++ray->cellDepthError < 10){ 
	        sprintf(WarningBuff,"cellDepthRayReflect5 ne %ld Intersections %d ",ne,np);
			if(rayin->debug > 1)WarningBatch(WarningBuff);
	        for(n=0;n<np;++n){
	            sprintf(WarningBuff,"%g ",ss[n]);
				if(rayin->debug > 1)WarningBatch(WarningBuff);
	        }
	        sprintf(WarningBuff,"\n");
			if(rayin->debug > 1)WarningBatch(WarningBuff);
	    }
	    return 0.0;
	}

	cellDepth=sqrt(dx*dx+dy*dy+dz*dz);


	if((cellDepth > 0.0) && (ray->CellCount < ray->CellCountMax)){
	    if(r1 > r2){
	        ray->r1[ray->CellCount]=r2;
	        ray->r2[ray->CellCount]=r1;
	    }else{
	        ray->r1[ray->CellCount]=r1;
	        ray->r2[ray->CellCount]=r2;
	    } 
	    ray->v[ray->CellCount]=ray->data[ne];
            ray->CellCount++;
	}


	return cellDepth*ray->CellSizeOver;
}
int getPalDouble(char *palname,double *rr,double *gg,double *bb)
{

static unsigned char ps_pal[768]= {255,255,255,0,0,131,0,0,
  135,0,0,139,0,0,143,0,0,147,0,0,151,0,0,155,0,0,159,0,0,163,0,0,
  167,0,0,171,0,0,175,0,0,179,0,0,183,0,0,187,0,0,191,0,0,195,0,0,
  199,0,0,203,0,0,207,0,0,211,0,0,215,0,0,219,0,0,223,0,0,227,0,0,
  231,0,0,235,0,0,239,0,0,243,0,0,247,0,0,251,0,0,255,0,0,255,0,3,
  255,0,7,255,0,11,255,0,15,255,0,19,255,0,23,255,0,27,255,0,31,255,
  0,35,255,0,39,255,0,43,255,0,47,255,0,51,255,0,55,255,0,59,255,0,
  63,255,0,67,255,0,71,255,0,75,255,0,79,255,0,83,255,0,87,255,0,91,
  255,0,95,255,0,99,255,0,103,255,0,107,255,0,111,255,0,115,255,0,
  119,255,0,123,255,0,127,255,0,131,255,0,135,255,0,139,255,0,143,
  255,0,147,255,0,151,255,0,155,255,0,159,255,0,163,255,0,167,255,0,
  171,255,0,175,255,0,179,255,0,183,255,0,187,255,0,191,255,0,195,
  255,0,199,255,0,203,255,0,207,255,0,211,255,0,215,255,0,219,255,0,
  223,255,0,227,255,0,231,255,0,235,255,0,239,255,0,243,255,0,247,
  255,0,251,255,0,255,255,0,255,255,3,255,251,7,255,247,11,255,243,
  15,255,239,19,255,235,23,255,231,27,255,227,31,255,223,35,255,219,
  39,255,215,43,255,211,47,255,207,51,255,203,55,255,199,59,255,195,
  63,255,191,67,255,187,71,255,183,75,255,179,79,255,175,83,255,171,
  87,255,167,91,255,163,95,255,159,99,255,155,103,255,151,107,255,
  147,111,255,143,115,255,139,119,255,135,123,255,131,127,255,127,
  131,255,123,135,255,119,139,255,115,143,255,111,147,255,107,151,
  255,103,155,255,99,159,255,95,163,255,91,167,255,87,171,255,83,
  175,255,79,179,255,75,183,255,71,187,255,67,191,255,63,195,255,59,
  199,255,55,203,255,51,207,255,47,211,255,43,215,255,39,219,255,35,
  223,255,31,227,255,27,231,255,23,235,255,19,239,255,15,243,255,11,
  247,255,7,251,255,3,255,255,0,255,251,0,255,247,0,255,243,0,255,
  239,0,255,235,0,255,231,0,255,227,0,255,223,0,255,219,0,255,215,0,
  255,211,0,255,207,0,255,203,0,255,199,0,255,195,0,255,191,0,255,
  187,0,255,183,0,255,179,0,255,175,0,255,171,0,255,167,0,255,163,0,
  255,159,0,255,155,0,255,151,0,255,147,0,255,143,0,255,139,0,255,
  135,0,255,131,0,255,127,0,255,123,0,255,119,0,255,115,0,255,111,0,
  255,107,0,255,103,0,255,99,0,255,95,0,255,91,0,255,87,0,255,83,0,
  255,79,0,255,75,0,255,71,0,255,67,0,255,63,0,255,59,0,255,55,0,
  255,51,0,255,47,0,255,43,0,255,39,0,255,35,0,255,31,0,255,27,0,
  255,23,0,255,19,0,255,15,0,255,11,0,255,7,0,255,3,0,255,0,0,250,0,
  0,246,0,0,241,0,0,237,0,0,233,0,0,228,0,0,224,0,0,219,0,0,215,0,0,
  211,0,0,206,0,0,202,0,0,197,0,0,193,0,0,189,0,0,184,0,0,180,0,0,
  175,0,0,171,0,0,167,0,0,162,0,0,158,0,0,153,0,0,149,0,0,145,0,0,
  140,0,0,136,0,0,131,0,0,0,0,0 };

	unsigned char *pal;
    int k;

	if(!rr || !gg || !bb)return 1;

	if(!getUniversalPaletteByName(palname,&pal)){
	    for(k=0;k<256;++k){
	        rr[k]=((pal[3*k])/255.);
	        gg[k]=((pal[3*k+1])/255.);
	        bb[k]=((pal[3*k+2])/255.);
	    }
	       	
	}else{

		pal=ps_pal;
	    for(k=0;k<256;++k){
	        rr[k]=((pal[3*k])/255.);
	        gg[k]=((pal[3*k+1])/255.);
	        bb[k]=((pal[3*k+2])/255.);
	    }
	}
	return 0;
        

}
static int doAnaglyph(long xsize,long ysize,double *left,double *right)
{
	long length,n;
	
	if(!left || !right)return 1;
	
    length=xsize*ysize;
    
    for(n=0;n<length;++n){
        right[3*n]=left[3*n];
        right[3*n+1]=right[3*n+1];
        right[3*n+2]=right[3*n+2];      
    }
	
	return 0;
}

