#define EXTERN22 extern
#include "Xappl.h"

#include "Sagevol.h"

int TraceFindLines(vIconPtr myIcon)
{
	struct vScene *scene;
	int ret;


	if(!myIcon)return 1;
	scene=&myIcon->scene;

	ret=1;

	WarningBatch("TraceFindLines Here\n");

	ret=0;
ErrorOut:
	return ret;
}