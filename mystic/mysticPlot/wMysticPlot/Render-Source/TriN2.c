#define EXTERN22 extern
#include "Xappl.h"
#include "TracerDef.h"
#include <stdio.h>
#include <math.h>
#include "Render.h"
#include "Vrml.h"

#include "Defs.h"

typedef struct t_trindata {
	Vec 		tri_end ;
	Vec 		v1;
	Vec 		v2;
	Vec 		vn;
	Vec 		n1;
	Vec 		n2;
	Vec 		n3;
	Vec			Col1;
	Vec			Col2;
	Vec			Col3;
	Flt 		c1;
	Flt 		c2;
	Flt 		c3;
	Flt 		c4;
	Flt 		dw;
} TriNData ;

int Shadow(Ray *ray, Flt tmax,Object *obj);

static int TriNPrint(Object *obj);

static int TriNIntersect(Object *obj, Ray *ray, Isect *hit);

static int TriNNormal(Object *obj, Isect *hit, Point P, Point N);

static int TriNShade(Object *obj,int level, Flt weight, Vec P,Vec N,Vec I, Isect *hit,Vec col,Ray *ray);


static ObjectProcs TriNProcs = {
	TriNPrint,
	TriNIntersect,
	TriNNormal,
	TriNShade,
} ;

static int TriNShade(Object *obj,int level, Flt weight, Vec P,Vec N,Vec I, Isect *hit,Vec col,Ray *ray)
{
	TriNData *qp;
	ColorVEC	surf_color/*,specular ambient*/;
	double s1x,s2x;
	int l;
	Vec 	L ;
	Flt 	t ;
	Ray 	tray ;
	Flt		diff ;
	Flt 	costheta;
	Vec	surfcolor;

	qp = (TriNData *) obj -> o_data ;
	if(!qp)return 0;
		
	I=I;
	hit=hit;
	level=level;
	weight=weight;
	
	tray.Q=ray->Q;
	tray.Qsize=ray->Qsize;
	tray.traverseStack=ray->traverseStack;

	col[0] = col[1] = col[2] = 0.0 ;
	
	/*
	specular[0] = specular[1] = specular[2] = 0.0 ;
	*/
	
	s1x=hit->f1;
	s2x=hit->f2;
	
	surf_color[0]=qp->Col1[0]+s1x*qp->Col2[0]+s2x*qp->Col3[0];
	surf_color[1]=qp->Col1[1]+s1x*qp->Col2[1]+s2x*qp->Col3[1];
	surf_color[2]=qp->Col1[2]+s1x*qp->Col2[2]+s2x*qp->Col3[2];

	for (l = 0; l < rd->nLights; l++) {
		VecSub(rd->Lights[l].light_pos, P, L);
		if (VecDot(N,L) >= 0.0) {
			t = VecNormalize(L);
			VecCopy(P, tray.P);
			VecCopy(L, tray.D);

			if (rd->Shadow(&tray, t)) {
			    costheta=VecDot(N,L);
			    diff = costheta * rd->Lights[l].light_brightness ;
			    surfcolor[0] = surf_color[0]*rd->Lights[l].red;
			    surfcolor[1] = surf_color[1]*rd->Lights[l].green;
			    surfcolor[2] = surf_color[2]*rd->Lights[l].blue;
			    VecAddS(diff,surfcolor, col, col) ;
			}
		}
	}

	return 0;
}



static int TriNPrint(Object *obj)
{
/*
	TriNData * sp ;
*/
	if(!obj)return 1;
	/*
	sp = (TriNData *) obj -> o_data ;
	*/
/*
	printf("s %g %g %g %g\n", sp -> sph_center[0], 
				   sp -> sph_center[1],
				   sp -> sph_center[2],
				   sp -> sph_radius) ;
*/
	return 0;
}

Object *MakeTriN2(int npoints,Surface *s, Vec *points,Vec *normals,Vec *col)
{
	Flt d,dmax,dmin,den;
	Vec v1,v2;
	TriNData *qp;
	Object * tmp;
	int i,j;

	qp=NULL;
	tmp=NULL;
	if(!(tmp = (Object *) cMalloc (sizeof(Object),9712))){
	    WarningBatch("MakeTriN out of Memory #1\n");
	    goto ErrorOut;
	}
	tmp -> o_type = T_TRIN ;
	tmp -> o_procs = & TriNProcs ;
	tmp -> o_surf = s ;
	if(!(qp = (TriNData *) cMalloc (sizeof(TriNData),9713))){
	    WarningBatch("MakeTriN out of Memory #2\n");
	    goto ErrorOut;
	}
	
	tmp -> o_data = (void *) qp ;
	
	if(npoints != 3){
	    WarningBatch("MakeTriN Expects 3 Points\n");
	    goto ErrorOut;
	}
	
	VecCopy(points[0],qp->tri_end);
	
	VecCopy(normals[0],qp->n1);
	VecSub(normals[1],normals[0],qp->n2);
	VecSub(normals[2],normals[0],qp->n3);
	
	VecCopy(col[0],qp->Col1);
	VecSub(col[1],col[0],qp->Col2);
	VecSub(col[2],col[0],qp->Col3);
	
	VecSub(points[1],qp->tri_end,qp->v1);
	VecSub(points[2],qp->tri_end,qp->v2);
	
	VecCopy(qp->v1,v1);
	VecCopy(qp->v2,v2);
		
	VecCross(v1,v2,qp->vn);
	VecNormalize(qp->vn);
	
	qp->dw=VecDot(qp->vn,qp->tri_end);
	
	
	qp->c1=VecDot(v2,v2);
	qp->c2=VecDot(v1,v2);
	qp->c3=VecDot(v1,v1);
	qp->c4=VecDot(v2,v1);
	
    den=qp->c1*qp->c3-qp->c2*qp->c4;
    if(den == 0.){
/*
     	sprintf(WarningBuff,"Triangle Error den = %f\n",den);
     	WarningBatch(WarningBuff);
*/
 		den=1;
    }
	den = 1./den;
	qp->c1 *= den;
	qp->c2 *= den;
	qp->c3 *= den;
	qp->c4 *= den;

	/*
	 * figure out dmin and dmax values for 
	 * each of the slabs...
	 */
	
	for (i = 0 ; i < NSLABS ; i ++) {
		dmin = HUGE ;
		dmax = - HUGE ;

		for (j = 0 ; j < npoints ; j ++) {
			d = VecDot(rd->Slab[i], points[j]) ;
			if (d < dmin) dmin = d ;
			if (d > dmax) dmax = d ;
		}
		tmp -> o_dmin[i] = dmin ;
		tmp -> o_dmax[i] = dmax ;
	}
	return tmp ;
ErrorOut:
	if(tmp)cFree((char *)tmp);
	if(qp)cFree((char *)qp);
	return NULL;
}
static int TriNIntersect(Object *obj, Ray *ray, Isect *hit)
{

	Flt den,f1,f2,t,s1,s2;
	Point P,PP;
	TriNData * qp ;

	qp = (TriNData *) obj -> o_data ;

	den=VecDot(ray->D,qp->vn);
	if(fabs(den) < rd->rayeps){
	    return 0;
	}

	t=(qp->dw-(VecDot(ray->P,qp->vn)))/den;
	
	VecAddS(t,ray->D,ray->P,P);
			
	VecSub(P,qp->tri_end,PP);
		
	f1=VecDot(PP,qp->v1);
	
	f2=VecDot(PP,qp->v2);
	
	s1=(f1*qp->c1-f2*qp->c2);
		
	if(s1 < 0. || s1 > 1.0){
	     return 0;
	}
	
	s2=(f2*qp->c3-f1*qp->c4);
	
	if(s2 < 0.){
	     return 0;
	}
	
	if((s1+s2) > 1.0){
	     return 0;
	}
	
	if (t < rd->rayeps) {
		return(0);
	}

	hit -> isect_t = t ;
	hit -> isect_enter = 0;
	hit -> isect_prim = obj ;
	hit -> isect_surf = obj -> o_surf ;
	hit -> f1=s1;
	hit -> f2=s2;
	return (1);
}

static int TriNNormal(Object *obj, Isect *hit, Point P, Point N)
{
	TriNData * qp ;
	Flt s1,s2;
	
	if(!obj)return 1;
	
	qp = (TriNData *) obj -> o_data ;
	
	P=P;
	
	s1=hit->f1;
	s2=hit->f2;

	N[0]=qp->n1[0]+s1*qp->n2[0]+s2*qp->n3[0];
	N[1]=qp->n1[1]+s1*qp->n2[1]+s2*qp->n3[1];
	N[2]=qp->n1[2]+s1*qp->n2[2]+s2*qp->n3[2];
	
	VecNormalize(N);
	
	return 0;
}

