#define EXTERN22 extern
#include "Xappl.h"
#include "TracerDef.h"
#include <stdio.h>
#include <math.h>
#include "Render.h"
#include "Vrml.h"

#include "Defs.h"

typedef struct t_Tridata {
	Vec 		Tri_end ;
	Vec 		v1;
	Vec 		v2;
	Vec 		vn;
	Flt 		c1;
	Flt 		c2;
	Flt 		c3;
	Flt 		c4;
	Flt 		f1;
	Flt 		f2;
	Flt 		dw;
} TriData ;

static int TriPrint( Object *obj);
static int TriIntersect(Object * obj, Ray * ray, Isect * hit);
static int TriNormal(Object * obj, Isect * hit,  Point P, Point N);

ObjectProcs TriProcs = {
	TriPrint,
	TriIntersect,
	TriNormal,
} ;

static int TriPrint( Object *obj)
{
/*
	TriData * sp ;
*/
	if(!obj)return 1;
	
/*
	sp = (TriData *) obj -> o_data ;
	printf("s %g %g %g %g\n", sp -> sph_center[0], 
				   sp -> sph_center[1],
				   sp -> sph_center[2],
				   sp -> sph_radius) ;
*/
    return 0;
}


Object *MakeTri(int npoints,Surface *s, Vec * points)
{
	Flt d,dmax,dmin,v1_len,v2_len,den;
	Vec v1,v2;
	TriData *qp;
	Object * tmp;
	int i,j;

	qp=NULL;
	tmp=NULL;
	
	if(npoints != 3){
	    WarningBatch("MakeTri Expects 3 Points\n");
	    goto ErrorOut;
	}
	
	if(!(tmp = (Object *) cMalloc (sizeof(Object),8711))){
	    WarningBatch("MakeTri out of Memory #1\n");
	    goto ErrorOut;
	}
	tmp -> o_type = T_TRI ;
	tmp -> o_procs = & TriProcs ;
	tmp -> o_surf = s ;
	if(!(qp = (TriData *) cMalloc (sizeof(TriData),8711))){
	    WarningBatch("MakeTri out of Memory #2\n");
	    goto ErrorOut;
	}
	
	tmp -> o_data = (void *) qp ;
	
	VecCopy(points[0],qp->Tri_end);
	
	
	VecSub(points[1],qp->Tri_end,qp->v1);
	VecSub(points[2],qp->Tri_end,qp->v2);
	
	VecCopy(qp->v1,v1);
	VecCopy(qp->v2,v2);
	
	v1_len=VecNormalize(qp->v1);
	v2_len=VecNormalize(qp->v2);
	
	qp->v1[0]/=v1_len;
	qp->v1[1]/=v1_len;
	qp->v1[2]/=v1_len;
	
	
	qp->v2[0]/=v2_len;
	qp->v2[1]/=v2_len;
	qp->v2[2]/=v2_len;
	
	
	VecCross(qp->v1,qp->v2,qp->vn);
	VecNormalize(qp->vn);
	VecCross(qp->vn,qp->v1,qp->v2);
	
	
	qp->dw=VecDot(qp->vn,qp->Tri_end);
	
	
	qp->c1=VecDot(v1,qp->v1);
	qp->c2=VecDot(v2,qp->v1);
	qp->c3=VecDot(v1,qp->v2);
	qp->c4=VecDot(v2,qp->v2);
	
        den=qp->c1*qp->c4-qp->c3*qp->c2;
    if(den == 0.){
        /*
             printf("Triangle Error\n");
             exit(1);
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
static int TriIntersect(Object * obj, Ray * ray, Isect * hit)
{

	Flt den,f1,f2,t,s1,s2;
	Point P,PP;
	TriData * qp ;

	qp = (TriData *) obj -> o_data ;

	den=VecDot(ray->D,qp->vn);
	if(fabs(den) < rd->rayeps){
	    return 0;
	}

	t=(qp->dw-(VecDot(ray->P,qp->vn)))/den;

	VecAddS(t,ray->D,ray->P,P);
	
	VecSub(P,qp->Tri_end,PP);
		
	f1=VecDot(PP,qp->v1);
	
	f2=VecDot(PP,qp->v2);
	
    s1=(f1*qp->c4-f2*qp->c2);
	if(s1 < 0.){
	     return 0;
	}
	
    s2=(f2*qp->c1-f1*qp->c3);
	if(s2 < 0.){
	     return 0;
	}
	
	if((s1+s2) > 1.0){
	     return 0;
	}
	
	if (t < rd->rayeps) {
		return(0);
	}

	qp->f1=f1;
	qp->f2=f2;

	hit -> isect_t = t ;
	hit -> isect_enter = 0;
	hit -> isect_prim = obj ;
	hit -> isect_surf = obj -> o_surf ;
	return (1);
}

static int TriNormal(Object * obj, Isect * hit,  Point P, Point N)
{
	TriData * qp ;
	
	if(!obj || !hit)return 1;
	
	qp = (TriData *) obj -> o_data ;
	
	P=P;
	
	
	VecCopy(qp->vn,N);
	
	return 0;

}

