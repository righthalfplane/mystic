#define EXTERN22 extern
#include "Xappl.h"
#include "TracerDef.h"
#include <stdio.h>
#include <math.h>
#include "Render.h"
#include "Vrml.h"

#include "Defs.h"

int doVrmlTexture(ColorVEC surf_color,double s1x,double s2x,long ns,long ip,struct tState *tS);

static int PolyPrint (Object * obj);
static int PolyIntersect (Object *obj, Ray *ray, Isect *hit);
static int PolyNormal (Object *obj, Isect *hit,Point P,Point N);
static int PolyShade(Object *obj,int level, Flt weight, Vec P,Vec N,Vec I, Isect *hit,Color7 col,Ray *ray);

typedef struct t_polydata {
	int 	poly_npoints ;
	Vec	* poly_point ;
	Vec	poly_normal ;
	Flt 	poly_d ;
	int	poly_p1, poly_p2 ;
	Flt 		f1;
	Flt 		f2;
	long 		ns;
	long 		ip;
	long		mat;
	long        m[3];
	struct tState *tS;
} PolyData ;


static ObjectProcs PolyProcs = {
	PolyPrint,
	PolyIntersect,
	PolyNormal,
	PolyShade,
} ;
static int PolyShade(Object *obj,int level, Flt weight, Vec P,Vec N,Vec I, Isect *hit,Color7 col,Ray *ray)
{
	PolyData *qp;
	struct tState *tS;
	ColorVEC	surf_color,specular /*ambient*/;
	struct Material surf;
	int MaterialType,mat;
	struct vrmlMaterial *vrmlMat;
	double s1x,s2x;
	int ns,ip,l;
	Vec 	L, H ;
	Flt 	t ;
	Ray 	tray ;
	Flt		diff ;
	Flt 	spec ;
	Flt 	costheta,costheta2 ;
	Color7	surfcolor;
	
	//hit=hit;
	
	tray.Q=ray->Q;
	tray.Qsize=ray->Qsize;
	tray.traverseStack=ray->traverseStack;

	col[0] = col[1] = col[2] = 0.0 ;
	
	specular[0] = specular[1] = specular[2] = 0.0 ;
	
	//level=level;
	//weight=weight;
	
	qp = (PolyData *) obj -> o_data ;
	if(!qp)return 0;
	
	tS=qp->tS;
	if(!qp)return 0;
	
	zerol((char *)&surf,sizeof(struct Material));
	surf.opacity=tS->opacity;

	MaterialType=tS->MaterialType;
	vrmlMat=tS->m;
	
	s1x=qp->f1;
	s2x=qp->f2;
	
	ns=(int)qp->ns;
	ip=(int)qp->ip;

	if(MaterialType == VRML_BY_FACE){
	    mat=(int)qp->mat;
	    if(vrmlMat && (mat < vrmlMat->max)){
	        surf_color[0]=vrmlMat->diffusedata[mat*3];
	        surf_color[1]=vrmlMat->diffusedata[mat*3+1];
	        surf_color[2]=vrmlMat->diffusedata[mat*3+2];

			surf.phong= 128*vrmlMat->shinedata[mat];

			specular[0]=vrmlMat->speculardata[mat*3];
			specular[1]=vrmlMat->speculardata[mat*3+1];
			specular[2]=vrmlMat->speculardata[mat*3+2];
/*
			ambient[0]=vrmlMat->ambientdata[mat*3];
			ambient[1]=vrmlMat->ambientdata[mat*3+1];
			ambient[2]=vrmlMat->ambientdata[mat*3+2];
*/
	        surf.flag=vrmlMat->flagsdata[mat];
	    }else{
	        surf_color[0]=.8f;
	        surf_color[1]=.8f;
	        surf_color[2]=.8f;
	        /*
			ambient[0]=.2f;
			ambient[1]=.2f;
			ambient[2]=.2f;
			*/
	        surf.flag = M_AMBIENT;
	    }
	}else if(MaterialType == VRML_BY_VERTEX){
	    long m[3];
	    double *d;


	    m[0]=qp->m[0];
	    m[1]=qp->m[1];
	    m[2]=qp->m[2];

	    surf.flag=vrmlMat->flagsdata[m[0]];
	    surf.phong= 128*vrmlMat->shinedata[m[0]];

	    if(surf.flag & M_DIFFUSE){

	        d=vrmlMat->diffusedata;
	        surf_color[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
	                                s2x*(d[m[2]*3]-d[m[0]*3]);
	        surf_color[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
	                                  s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
	        surf_color[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
	                                  s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
	    }else{
	        surf_color[0]=0;
	        surf_color[1]=0;
	        surf_color[2]=0;
	    }

	    if(surf.flag & M_SPECULAR){
	        d=vrmlMat->speculardata;

	        specular[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
	                              s2x*(d[m[2]*3]-d[m[0]*3]);
	        specular[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
	                                s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
	        specular[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
	                                s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
	    }

	    if(surf.flag & M_AMBIENT){
	    /*
	        d=vrmlMat->ambientdata;
	        ambient[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
	                             s2x*(d[m[2]*3]-d[m[0]*3]);
	        ambient[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
	                               s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
	        ambient[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
	                               s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
	    */
	    }
	}else{
	    surf.flag=0;
	    surf_color[0]=0;
	    surf_color[1]=0;
	    surf_color[2]=0;
	}

	if(tS->flagTexture)doVrmlTexture(surf_color,s1x,s2x,ns,ip,tS);

	for (l = 0; l < rd->nLights; l++) {
		VecSub(rd->Lights[l].light_pos, P, L);
		if (VecDot(N,L) >= 0.0) {
			t = VecNormalize(L);
			VecCopy(P, tray.P);
			VecCopy(L, tray.D);
		/*
			rd->nShadows ++ ;
		*/
			if (rd->Shadow(&tray, t)) {
			    costheta=VecDot(N,L);
			    diff = costheta * rd->Lights[l].light_brightness ;
			    surfcolor[0] = surf_color[0]*rd->Lights[l].red;
			    surfcolor[1] = surf_color[1]*rd->Lights[l].green;
			    surfcolor[2] = surf_color[2]*rd->Lights[l].blue;
			    VecAddS(diff,surfcolor, col, col) ;
			    if(surf.flag & M_SPECULAR){
					VecComb((-1.),L,(2.*costheta),N,H);
					VecNormalize(H) ;
					costheta2=VecDot(H,I);
					if(costheta2 < 0.0){
					    spec = pow(-costheta2, surf.phong) ;
					    col[0] += spec*specular[0] ;
					    col[1] += spec*specular[1] ;
					    col[2] += spec*specular[2] ;
					}
			    }
			}
		}
	}



	

	return 0;
}


 
static int PolyPrint(Object * obj)
{
	int i ;
	PolyData * pd ;

	if(!obj)return 1;
	
	pd = (PolyData *) obj -> o_data ;
	printf("p %d\n", pd -> poly_npoints) ;
	for (i = 0 ; i < pd -> poly_npoints ; i++) {
		printf("%g %g %g\n", pd -> poly_point[i][0],
					pd -> poly_point[i][1],
					pd -> poly_point[i][2]) ;
	}
	return 0;
}

static int PolyIntersect(Object *obj, Ray *ray, Isect *hit)
{
	Flt n,d,t,x,y,*p0,*p1 ;
	Point V ;
	int l, np ;
	int xi, yi ;
	PolyData * pd ;

	pd = (PolyData *) obj -> o_data ;
	n = VecDot(ray -> P, pd -> poly_normal) + pd -> poly_d ;
	d = VecDot(ray -> D, pd -> poly_normal) ;

	if ((Flt) fabs(d) < rd->rayeps) {
		return(0);
	}

	t = - n / d ;
	if (t < rd->rayeps)
		return 0 ;
		
		

	RayPoint(ray,t,V);
	
	xi = pd -> poly_p1 ;
	yi = pd -> poly_p2 ;
	
	x=V[xi];
	
	y=V[yi];
	
	np=pd -> poly_npoints;
	
	p1=(Flt *)(pd -> poly_point);
	
	p0=p1+3*(np-1);
	
	l = 0 ;
	
	
	while (np--) {
	    if((p0[yi] > y) ^ (p1[yi] > y)){
	        if(p0[xi] > x && p1[xi] > x){
	            ++l;
	        }else if (p0[xi] > x || p1[xi] > x){
	            l += (p1[yi] > p0[yi]) ^
	                 ((p0[yi] - y)*(p1[xi] - x) >
	                  (p0[xi] - x)*(p1[yi] - y));
	        }
	    }
	    p0  = p1;
	    p1 += 3;

	}

	if ((l % 2) == 0)
		return(0);
	
	hit -> isect_t = t ;
	hit -> isect_surf = obj -> o_surf ;
	hit -> isect_prim = obj ;
	hit -> isect_enter = 0 ;
	return(1);
}
static int PolyNormal(Object *obj, Isect *hit,Point P,Point N)
{

	PolyData * pd ;
	
	if(!obj)return 1;
	pd = (PolyData *) obj -> o_data ;
	//hit=hit;
	//P=P;
	VecCopy(pd -> poly_normal, N);
	return 0;
}

Object *MakePoly(Surface *s,int npoints, Vec * points,struct tState *tS)
{
	Object * obj ;
	PolyData * pd ;
	Vec P1, P2;
	Flt d, dmax, dmin;
	int i, j, ax, n;

	if(!(obj = (Object *) cMalloc (sizeof(Object),8713))){
	    WarningBatch("MakePoly Out of Memory #1\n");
	    return NULL;
	}
	obj -> o_type = T_POLY ;
	obj -> o_procs = & PolyProcs ;
	obj -> o_surf = s ;

	if(!(pd = (PolyData *) cMalloc (sizeof(PolyData)+npoints*sizeof(Vec),8714))){
	    WarningBatch("MakePoly Out of Memory #2\n");
	    if(obj)cFree((char *)obj);
	    return NULL;
	}
	
	pd -> poly_npoints = npoints ;
	pd -> poly_point = (Vec *)(pd+1) ;
	
	for(n=0;n<npoints;++n){
	   VecCopy(points[n], pd -> poly_point[n]);
	}

	/*
	 * calculate the normal by giving various cross products...
	 */
/*
	VecCopy(pd -> poly_point[1],Po);
*/
	
	VecSub(pd -> poly_point[0], pd -> poly_point[1], P1) ;
	VecSub(pd -> poly_point[2], pd -> poly_point[1], P2) ;

	VecCross(P1, P2, pd -> poly_normal) ;
	VecNormalize(pd -> poly_normal) ;
	
	
	ax= (fabs(pd -> poly_normal[0]) > fabs(pd -> poly_normal[1])) ? 0 : 1;
	if(fabs(pd -> poly_normal[2]) > fabs(pd -> poly_normal[ax])) ax=2;
	pd -> poly_p1 = (ax + 1) % 3;
	pd -> poly_p2 = (ax + 2) % 3;

	pd -> poly_d = - VecDot(pd -> poly_normal, pd -> poly_point[0]) ;
	
	pd->mat=(long)points[npoints][0];
	pd->ns=(long)points[npoints][1];
	pd->ip=(long)points[npoints][2];

	pd->m[0]=(long)points[npoints+1][0];
	pd->m[1]=(long)points[npoints+1][1];
	pd->m[2]=(long)points[npoints+1][2];


	pd->tS=tS;
	
	

	obj -> o_data = (void *) pd ;
	
	
	
	
	/*
	 * now, calculate the values of 
	 * the dmin and dmax 'es for the globally defined slabs...
	 */
	
	for (i = 0 ; i < NSLABS ; i ++) {
		dmin = HUGE ;
		dmax = - HUGE ;

		for (j = 0 ; j < pd -> poly_npoints ; j ++) {
			d = VecDot(rd->Slab[i], pd -> poly_point[j]) ;
			if (d < dmin) dmin = d ;
			if (d > dmax) dmax = d ;
		}
		obj -> o_dmin[i] = dmin ;
		obj -> o_dmax[i] = dmax ;
	}
	return(obj) ;
}
