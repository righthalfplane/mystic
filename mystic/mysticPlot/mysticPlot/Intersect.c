#define EXTERN22 extern
#include "Xappl.h"
#include "TracerDef.h"
#include <stdio.h>
#include <math.h>
#include "Render.h"
#include "Vrml.h"

#include "Defs.h"


int CheckAndEnqueue(Ray * ray,register Object * obj,Flt *num,Flt *den);

int CheckAndEnqueue(Ray * ray,register Object * obj,Flt *num,Flt *den)
{
	register int i ;
	Flt tmp ;
	Flt tmin, tmax ;
	Flt dmin = -HUGE ;
	Flt dmax = HUGE ;
/*
	rd->nChecked ++ ;
*/
	for (i = 0 ; i < NSLABS ; i ++) {

		/* enters the slab here...	*/
		tmin = (obj -> o_dmin[i] - num[i]) * den[i] ;
		/* and exits here...		*/
		tmax = (obj -> o_dmax[i] - num[i]) * den[i] ;

		/* but we may have to swap...	*/
		if (tmin > tmax) {
			tmp = tmin ; tmin = tmax ; tmax = tmp ;
		}

		/* if exited closer than we thought, update 	*/
		if (tmax < dmax)
			dmax = tmax ;
		/* if entered farther than we thought, update 	*/
		if (tmin > dmin)
			dmin = tmin ;

		if (dmin > dmax || dmax < rd->rayeps){
			return 0;
		}
	}
	PriorityQueueInsert(ray,dmin, obj) ;
/*
	rd->nEnqueued ++ ;
*/
	return 0;
}

