#define EXTERN22 extern
#include "Xappl.h"
#include "TracerDef.h"
#include <stdio.h>
#include <math.h>
#include "Render.h"
#include "Vrml.h"

#include "Defs.h"

int AllocatePQueue(Ray 	*ray)
{
	long length;
	
	if(!ray)return 1;
	
	length=(long)sizeof(Qelem)*PQSIZE;
	if(!(ray->Q=(Qelem	*)cMalloc(length,8765))){
	      sprintf(WarningBuff,"AllocatePQueue Out of Memory Request = %ld\n",length);
	      WarningBatch(WarningBuff);
	      return 1;
	}
	zerol((char *)ray->Q,length);
	return 0;
}
int DallocatePQueue(Ray *ray)
{
	if(!ray)return 1;
	
	if(ray->Q)cFree((char *)ray->Q);
	ray->Q = NULL;
	return 0;
}
int PriorityQueueNull(Ray *ray)
{
	if(!ray)return 1;
	ray->Qsize = 0 ;
	/*
	rd->totalQueueResets ++ ;
	*/
#ifdef DEBUG	
	WarningBatch("resetting\n") ;
#endif /* DEBUG */
	return 0;
}

int PriorityQueueEmpty(Ray *ray)
{
	if(!ray)return 0;
	return (ray->Qsize == 0) ;
}

int PriorityQueueInsert(Ray *ray,Flt key, Object * obj)
{
	register int i ; 
	register Qelem tmp ;
	
	if(!ray)return 1;	
/*
	rd->totalQueues ++ ;
*/
#ifdef DEBUG
	sprintf(WarningBuff,"inserting element, key = %g\n", key) ;
	WarningBatch(WarningBuff);
#endif
 	ray->Qsize++ ;
 	/*
	if (ray->Qsize > rd->maxQueueSize)
		rd->maxQueueSize = ray->Qsize ;
	*/
	if (ray->Qsize >= PQSIZE) {
		sprintf(WarningBuff, "%s: exhausted priority queue space\n", 
			rd->Progname) ;
		WarningBatch(WarningBuff);
		return 1;
	}
	ray->Q[ray->Qsize].q_key = key ;
	ray->Q[ray->Qsize].q_obj = obj ;
	
	i = ray->Qsize;
	while (i > 1 && ray->Q[i].q_key < ray->Q[i/2].q_key) {
		tmp = ray->Q[i] ;
		ray->Q[i] = ray->Q[i/2] ;
		ray->Q[i/2] = tmp ;
		i = i / 2 ;
	}
	return 0;
}

int PriorityQueueDelete(Ray *ray,Flt * key, Object ** obj)
{
	register Qelem tmp ;
	register int i, j ;

	if(!ray)return 1;	

	if (ray->Qsize == 0) {
		sprintf(WarningBuff, "%s: priority queue is empty\n",
			rd->Progname) ;
		WarningBatch(WarningBuff);
		return 1;
	}

	*key = ray->Q[1].q_key ;
	*obj = ray->Q[1].q_obj ;


#ifdef DEBUG
	sprintf(WarningBuff,"deleting element, key = %g\n", *key) ;
	WarningBatch(WarningBuff);
#endif


	memcpy(ray->Q+1,ray->Q+ray->Qsize,sizeof(*(ray->Q))) ;
	
	ray->Qsize-- ;

	
	i = 1 ;

	while (2 * i <= ray->Qsize) {

		if (2 * i == ray->Qsize) {
			j = 2 * i ;
		} else if (ray->Q[2*i].q_key < ray->Q[2*i+1].q_key) {
			j = 2 * i ;
		} else {
			j = 2 * i + 1 ;
		}

		if (ray->Q[i].q_key > ray->Q[j].q_key) {
			tmp = ray->Q[i] ;
			ray->Q[i] = ray->Q[j] ;
			ray->Q[j] = tmp ;
			i = j ;
		} else {
			break ;
		}
	}
	return 0;
	
}
