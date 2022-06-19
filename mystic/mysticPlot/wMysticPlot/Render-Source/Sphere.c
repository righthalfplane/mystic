#define EXTERN22 extern
#include "Xappl.h"
#include "TracerDef.h"
#include <stdio.h>
#include <math.h>
#include "Render.h"
#include "Vrml.h"

#include "Defs.h"

typedef struct t_spheredata {
	Vec 		sph_center ;
	Flt 		sph_radius ;
	Flt 		sph_radius2 ;
} SphereData ;

static int  SpherePrint(Object *obj);
static int SphereIntersect (Object *obj, Ray *ray, Isect *hit);
static int SphereNormal(Object * obj, Isect * hit,Point P,Point N);

static ObjectProcs SphereProcs = {
	SpherePrint,
	SphereIntersect,
	SphereNormal,
} ;

static int  SpherePrint(Object *obj)
{
	SphereData * sp ;

	sp = (SphereData *) obj -> o_data ;

	sprintf(WarningBuff,"s %g %g %g %g\n", sp -> sph_center[0], 
				   sp -> sph_center[1],
				   sp -> sph_center[2],
				   sp -> sph_radius) ;
	WarningBatch(WarningBuff);
	return 0;
}

static int SphereIntersect(Object *obj, Ray *ray, Isect *hit)
{

	Flt b, disc, t;
	Point V ;
	SphereData * sp ;

	sp = (SphereData *) obj -> o_data ;

	VecSub((sp->sph_center), ray -> P, V);

	b = VecDot(V, ray -> D);

	disc = b * b - VecDot(V, V) + (sp -> sph_radius2) ;

	if (disc < 0.0)
		return(0);

	disc = sqrt(disc);

	t = (b - disc < rd->rayeps) ? b + disc : b - disc ;

	if (t < rd->rayeps) {
		return(0);
	}

	hit -> isect_t = t ;
	hit -> isect_enter = VecDot(V, V) > sp -> sph_radius2 + rd->rayeps ? 1 : 0 ;
	hit -> isect_prim = obj ;
	hit -> isect_surf = obj -> o_surf ;
	return (1);
}

static int SphereNormal(Object * obj, Isect * hit,Point P,Point N)
{
	SphereData * sp ;
	
	sp = (SphereData *) obj -> o_data ;
	
	//hit=hit;

	VecSub(P, sp -> sph_center, N);
	(void) VecNormalize(N);
	
	return 0;
}

Object *MakeSphere(Surface *s,Vec pos,Flt radius)
{
	Object * tmp ;
	int i ;
	SphereData *sp ;

	if(!(tmp = (Object *) cMalloc (sizeof(Object),8723))){
	    WarningBatch("MakeSphere out of Memory #1\n");
	    return NULL;
	}
	tmp -> o_type = T_SPHERE ;
	tmp -> o_procs = & SphereProcs ;
	tmp -> o_surf = s ;
	if(!(sp = (SphereData *) cMalloc (sizeof(SphereData),8724))){
	    WarningBatch("MakeSphere out of Memory #2\n");
	    if(tmp)cFree((char *)tmp);
	    return NULL;
	}
	VecCopy(pos, sp -> sph_center) ;
	sp -> sph_radius = radius ;
	sp -> sph_radius2 = radius * radius ;
	tmp -> o_data = (void *) sp ;

	/*
	 * figure out dmin and dmax values for 
	 * each of the slabs...
	 */
	
	for (i = 0 ; i < NSLABS; i ++) {
		tmp -> o_dmin[i] = VecDot(sp -> sph_center, rd->Slab[i]) 
			- sp -> sph_radius ;
		tmp -> o_dmax[i] = VecDot(sp -> sph_center, rd->Slab[i]) 
			+ sp -> sph_radius ;
	}

	return tmp ;
}
