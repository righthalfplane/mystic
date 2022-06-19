
#define EXTERN22 extern
#include "Xappl.h"
#include "TracerDef.h"
#include <stdio.h>
#include <math.h>
#include "Render.h"
#include "Vrml.h"

#include "Defs.h"

typedef struct t_trincdata {
	Vec 		tri_end ;
	Vec 		v1;
	Vec 		v2;
	Vec 		vn;
	Vec 		n1;
	Vec 		n2;
	Vec 		n3;
	Flt 		c1;
	Flt 		c2;
	Flt 		c3;
	Flt 		c4;
	Flt 		dw;
	Flt 		value1;
	Flt 		value2;
	Flt 		value3;
} TriNCData ;

static int TriNCPrint(Object *obj);

static int TriNCIntersect(Object *obj, Ray *ray, Isect *hit);

static int TriNCNormal(Object *obj, Isect *hit, Point P, Point N);


static ObjectProcs TriNCProcs = {
	TriNCPrint,
	TriNCIntersect,
	TriNCNormal,
} ;

static int TriNCPrint(Object *obj)
{
	TriNCData * sp ;
	
	if(!obj)return 1;

	sp = (TriNCData *) obj -> o_data ;
	
	if(!sp)return 0;
/*
	printf("s %g %g %g %g\n", sp -> sph_center[0], 
				   sp -> sph_center[1],
				   sp -> sph_center[2],
				   sp -> sph_radius) ;
*/
	return 0;
}

Object *MakeTriNC(int npoints,Surface *s, Vec *points,Vec *normals,Flt *values)
{
	Flt d,dmax,dmin,den;
	char *message;
	Vec v1,v2;
	TriNCData *qp;
	Object * tmp;
	int i,j;

	tmp=NULL;
	qp=NULL;
	message=NULL;
	
	if(!(tmp = (Object *) cMalloc (sizeof(Object),8711))){
	    WarningBatch("MakeTriN out of Memory #1\n");
	    goto ErrorOut;
	}
	tmp -> o_type = T_TRINC ;
	tmp -> o_procs = &TriNCProcs ;
	tmp -> o_surf = s ;
	if(!(qp = (TriNCData *) cMalloc (sizeof(TriNCData),8712))){
	    WarningBatch("MakeTriN out of Memory #2\n");
	    goto ErrorOut;
	}
	
	tmp -> o_data = (void *) qp ;
	
	if(npoints != 3){
	    WarningBatch("MakeTriNC Expects 3 Points\n");
	    goto ErrorOut;
	}

	VecCopy(points[0],qp->tri_end);
	
	VecCopy(normals[0],qp->n1);
	VecSub(normals[1],normals[0],qp->n2);
	VecSub(normals[2],normals[0],qp->n3);
	
	VecSub(points[1],qp->tri_end,qp->v1);
	VecSub(points[2],qp->tri_end,qp->v2);
	
	qp->value1=values[0];
	qp->value2=values[1]-values[0];
	qp->value3=values[2]-values[0];
	
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
	return tmp;
ErrorOut:
	if(tmp)cFree((char *)tmp);
	if(qp)cFree((char *)qp);
	return NULL;
}
static int TriNCIntersect(Object *obj, Ray *ray, Isect *hit)
{

	Flt den,f1,f2,t,s1,s2;
	Point P,PP;
	TriNCData * qp ;
	
	if(!obj || !ray || !hit)return 0;

	qp = (TriNCData *) obj -> o_data ;

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
	hit -> f1 = s1;
	hit -> f2 = s2;
	
	
	
	return (1);
}

static int TriNCNormal(Object *obj, Isect *hit, Point P, Point N)
{
	TriNCData * qp ;
	Flt s1,s2;
	
	if(!obj || !hit)return 1;
	
	qp = (TriNCData *) obj -> o_data ;
	
	//P=P;
	
	s1=hit->f1;
	s2=hit->f2;

	N[0]=qp->n1[0]+s1*qp->n2[0]+s2*qp->n3[0];
	N[1]=qp->n1[1]+s1*qp->n2[1]+s2*qp->n3[1];
	N[2]=qp->n1[2]+s1*qp->n2[2]+s2*qp->n3[2];
	hit->isect_value=qp->value1+s1*qp->value2+s2*qp->value3;
	VecNormalize(N);
	return 0;
}

