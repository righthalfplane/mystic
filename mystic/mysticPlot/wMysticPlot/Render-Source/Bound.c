#define EXTERN22 extern
#include "Xappl.h"
#include "TracerDef.h"
#include <stdio.h>
#include <math.h>
#include "Render.h"
#include "Vrml.h"

#include "Defs.h"

extern int debug; 
/*
 * This function attempts to use median cut
 * to generate tighter bounding volumes than the old
 * code...
 */
 
static int SortAndSplit(int first, int last, int axis);
static int compslabs(const void *a,const void *b);

int BuildBoundingSlabs()
{
	int low = 0 ;
	int axis = 0 ;
	int high, i ;
	
	if(!rd || !rd->nPrims || (rd->nPrims <= 0))return 1;
 
	high = rd->nPrims ;
	while (SortAndSplit(low, high, axis) == 0) {
		axis = (axis + 1) % NSLABS ;
		low = high ;
		high = rd->nPrims ;
	}

	if(!debug)return 0;
	
	sprintf(WarningBuff, "%s: after adding bounding volumes, %d prims\n",
		rd->Progname, rd->nPrims) ;
	WarningBatch(WarningBuff);
	
	sprintf(WarningBuff, "%s: extent of scene\n", rd->Progname) ;
	WarningBatch(WarningBuff);
	for (i = 0 ; i < NSLABS; i++) {
		sprintf(WarningBuff, "%s: <%g -- %g>\n",
				rd->Progname,
				rd->Root -> o_dmin[i],
				rd->Root -> o_dmax[i]) ;
		WarningBatch(WarningBuff);
	}
	
	return 0;
}
 
static int Axis ;
 
static int compslabs(const void *aa,const void *bb)
{
	Object **a=(Object **)aa;
	Object **b=(Object **)bb;
	Flt am, bm ;
 
	am = (*a) -> o_dmin[Axis] + (*a) -> o_dmax[Axis] ;
	bm = (*b) -> o_dmin[Axis] + (*b) -> o_dmax[Axis] ;
 
	if (am < bm)
		return (-1) ;
	else if (am == bm)
		return (0) ;
	else
		return (1) ;
}
 
static int SortAndSplit(int first, int last, int axis)
{
	Object * cp ;
	CompositeData * cd ;
	int size, i, j ;
	Flt dmin, dmax ;
	int m ;
 
	Axis = axis ;
 
	size = last - first ;
#ifdef THINK_C
	qsort((char *) (Prims + first), size, sizeof (Object *),(__cmp_func) compslabs) ;
#else
	qsort((char *) (rd->Prims + first), size, sizeof (Object *), compslabs) ;
#endif
 
	if (size <= BUNCHINGFACTOR) {
		/* build a box to contain them */
 
		if(!(cp = (Object *) cMalloc (sizeof(Object),8764))){
		    WarningBatch("SortAndSplit Out Of Memory #1\n");
		    return 1;
		}
		cp -> o_type = T_COMPOSITE ;
		cp -> o_procs = & rd->NullProcs ; 	/* die if you call any 	*/
		cp -> o_surf = NULL ;		/* no surface...	*/
		if(!(cd = (CompositeData *) cMalloc (sizeof(CompositeData),8765))){
		    if(cp)cFree((char *)cp);
		    WarningBatch("SortAndSplit Out Of Memory #2\n");
		    return 1;
		}
		cd -> c_size = size ;
 
		for(i = 0 ; i < size ; i++) {
			cd -> c_object[i] = rd->Prims[first + i] ;
		}
 
		for (i = 0 ; i < NSLABS ; i++ ) {
			dmin = HUGE ;
			dmax = -HUGE ;
			for (j = 0 ; j < size ; j++) {
				if (cd -> c_object[j] -> o_dmin[i] < dmin)
					dmin = cd -> c_object[j] -> o_dmin[i] ;
				if (cd -> c_object[j] -> o_dmax[i] > dmax)
					dmax = cd -> c_object[j] -> o_dmax[i] ;
			}
			cp -> o_dmin[i] = dmin ;
			cp -> o_dmax[i] = dmax ;
		}
		cp -> o_data = (void *) cd ;
		rd->Root = cp ;
		
		SaveObject(cp,1);

		return (1) ;
	} else {
		m = (first + last) / 2 ;
		SortAndSplit(first, m, (axis + 1) % NSLABS) ;
		SortAndSplit(m , last, (axis + 1) % NSLABS) ;
		return (0) ;
	}
}
	
 
int InitSlabs()
{
	int i ;
	
	if(!rd || !rd->Slab)return 1;
 
	for (i = 0 ; i < NSLABS ; i ++) {
		VecNormalize(rd->Slab[i]) ;
	}
	
	return 0;
}
