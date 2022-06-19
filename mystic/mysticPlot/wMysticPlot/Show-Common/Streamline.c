#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "Streamline.h"

#define slROUNDOFF 1.0e-6
#define slTINY 1.0e-10
#define slHUGE 1.0e+10

typedef struct tagslCELLINFO {
	double cellVel[3],cellDvDx[3][3];
	double cellSize[3],cellExtentMin[3],cellExtentMax[3];
	double cellSidePt[6][3],cellSideNorm[6][3];
	double slStartPt[3],slEndPt[3],slStartVel[3];
	int slStartSide;
} slCELLINFO;

slCELLINFO gCellInfo, *gpCellInfo=NULL;

/* =============================================================================
DESCRIPTION:
Main driver routine to integrate along streamline. Initializes the
main structure used to hold cell and streamline info used throughout.

INPUT:
cellCenter0 - Array of cell center coordinates (x,y,z)
cellSize0 - Array of cell dimensions (dx,dy,dz)
cellVel0 - Array of cell center velocity (u,v,w)
cellDvDx0 - 2-D array of partial derivatives du/dx, du/dy, du/dz, dv/dx, etc.
slStartPt0 - Array of steamline starting coords, must be on a cell face
slStartSide0 - Index representing the cell side that the starting streamline is
on (0=xmin, 1=xmax, 2=ymin, 3=ymax, 4=zmin, 5=zmax), -1=none.

INPUT/OUTPUT:

OUTPUT:
slEndPt0 - Array of steamline ending coords, must be on a cell face. Can be 
(0,0,0) if steamline ended inside cell.
slEndSide0 - Index representing the cell side that the ending streamline is on
(0=xmin, 1=xmax, 2=ymin, 3=ymax, 4=zmin, 5=zmax). Can be -1 if streamline
ended inside cell.

RETURN:
1 = streamline passed through cell
0 = error
-1 = steamline stopped in cell
*/
int slCellPath(double *cellCenter0,double *cellSize0,double *cellVel0,double cellDvDx0[3][3],
			   double *slStartPt0,int slStartSide0,double speedCutoff,double *slEndPt0,
			   int *slEndSide0,int *icnt,int maxSteps,double eps,double hFrac,double *slxMidPts0,
			   double *slyMidPts0,double *slzMidPts0)
{
	double slEndPt[3],outflow,vel2;
	int i,j,ret;

	if (!cellCenter0) goto errorExit;
	if (!cellSize0) goto errorExit;
	if (!cellVel0) goto errorExit;
	if (!cellDvDx0) goto errorExit;
	if (!slStartPt0) goto errorExit;
	if (!slEndPt0) goto errorExit;
	if (!slEndSide0) goto errorExit;

	gpCellInfo=&gCellInfo;

	if (zerol((char*)gpCellInfo,sizeof(slCELLINFO))) goto errorExit;

	gpCellInfo->slStartSide=slStartSide0;

    /* Transform coords to cell center origin */
	for(j=0;j<3;j++) {
		for(i=0;i<3;i++) {
			gpCellInfo->cellDvDx[j][i]=cellDvDx0[j][i];
		}
		gpCellInfo->cellVel[j] = cellVel0[j];
		gpCellInfo->cellSize[j] = cellSize0[j];
		gpCellInfo->cellExtentMin[j] = -0.5*cellSize0[j];
		gpCellInfo->cellExtentMax[j] =  0.5*cellSize0[j];
		gpCellInfo->slStartPt[j] = slStartPt0[j] - cellCenter0[j];
	}

	for(j=0;j<3;j++) {
		gpCellInfo->slStartVel[j] = slFunction(0.0,gpCellInfo->slStartPt,j);
	}

	for(j=0;j<3;j++) {
		gpCellInfo->cellSidePt[2*j][j] = gpCellInfo->cellExtentMin[j];
		gpCellInfo->cellSideNorm[2*j][j] = -1.0;
		gpCellInfo->cellSidePt[2*j+1][j] = gpCellInfo->cellExtentMax[j];
		gpCellInfo->cellSideNorm[2*j+1][j] = 1.0;
	}

	/* Check for zero velocity */
	vel2 = 0.0;
	for(j=0;j<3;j++) {
		vel2 += gpCellInfo->slStartVel[j]*gpCellInfo->slStartVel[j];
	}
	if (vel2 < speedCutoff*speedCutoff) {
		for(j=0;j<3;j++) {
			slEndPt0[j] = slStartPt0[j];
		}
		*slEndSide0=slStartSide0;
		return -1;
	}

	/* Check for outflow when streamline is trying to enter cell */
	if (slStartSide0>=0) {
		outflow=0.0;
		for(j=0;j<3;j++) {
			outflow += gpCellInfo->cellSideNorm[slStartSide0][j]*gpCellInfo->slStartVel[j];
		}
		if (outflow>=0.0) {
			for(j=0;j<3;j++) {
				slEndPt0[j] = slStartPt0[j];
			}
			*slEndSide0=slStartSide0;
			return -1;
		}
	}

	/* Check that start point is in the cell, account for roundoff */
	for(j=0;j<3;j++) {
		if (gpCellInfo->slStartPt[j] < gpCellInfo->cellExtentMin[j]-slROUNDOFF*gpCellInfo->cellSize[j] ||
			gpCellInfo->slStartPt[j] > gpCellInfo->cellExtentMax[j]+slROUNDOFF*gpCellInfo->cellSize[j]) {
			goto errorExit;
		}
	}

	ret=slRK4Adaptive(0.0,gpCellInfo->slStartPt,3,slEndPt,slEndSide0,
		icnt,maxSteps,eps,hFrac,slxMidPts0,slyMidPts0,slzMidPts0);

	if (ret==1 || ret==-1) {
		for(j=0;j<3;j++) {
			slEndPt0[j] = slEndPt[j] + cellCenter0[j];
		}
		for(j=0;j<(*icnt);j++) {
			slxMidPts0[j] = slxMidPts0[j] + cellCenter0[0];
			slyMidPts0[j] = slyMidPts0[j] + cellCenter0[1];
			slzMidPts0[j] = slzMidPts0[j] + cellCenter0[2];
		}
		return ret;
	} else {
		goto errorExit;
	}

errorExit:
	return 0;
}

/* =============================================================================
DESCRIPTION:
Adaptive time-stepping scheme for solving a system of ODEs.

INPUT:
INPUT/OUTPUT:
OUTPUT:

RETURN:
1 = streamline properly exited the cell
0 = error
-1 = streamline did not exit the cell
*/
int slRK4Adaptive(double xim1,double *yim1,int nEqns,double *yi,int *sideIndex,int *icnt,
				  int maxSteps,double eps,double hFrac,double *slxMidPts0,double *slyMidPts0,
				  double *slzMidPts0)
{
	double fac=0.9;
	double x,x1,x2,h,htmp,cellSizeMin,sideHit[3];
	double y[3],y0[3],y1[3],y2[3],y1tmp[3],y2tmp[3];
	double delmax,dist,disttmp,delworst,del[3];
	int i,j,ret,err;

	x = xim1;
	for(j=0;j<nEqns;j++) {
		y[j] = yim1[j];
	}
	if (*icnt >= maxSteps+2) goto errorExit;
	slxMidPts0[*icnt] = y[0];
	slyMidPts0[*icnt] = y[1];
	slzMidPts0[*icnt] = y[2];
	(*icnt)++;

	/* Find the min cell size for cells of ANY length scale */
	cellSizeMin = 0.0; /* to prevent compiler warning about uninitialized use */
	err = 1;
	for(j=0;j<3;j++) {
		if (gpCellInfo->cellSize[j] > 0.0) {
			cellSizeMin = gpCellInfo->cellSize[j];
			err = 0;
			break;
		}
	}
	if (err==1) goto errorExit;
	for(j=0;j<3;j++) {
		if ((gpCellInfo->cellSize[j] > 0.0) && (gpCellInfo->cellSize[j] < cellSizeMin)) {
			cellSizeMin = gpCellInfo->cellSize[j];
		}
	}


	/* Find the starting step size for cells of ANY length scale */
	h = 0.0; /* to prevent compiler warning about uninitialized use */
	err = 1;
	for(j=0;j<3;j++) {
		if (gpCellInfo->slStartVel[j] != 0.0 && gpCellInfo->cellSize[j] > 0.0) {
			h = fabs(hFrac*gpCellInfo->cellSize[j]/gpCellInfo->slStartVel[j]); 
			err = 0;
			break;
		}
	}
	if (err==1) goto errorExit;
	for(j=0;j<3;j++) {
		if (gpCellInfo->slStartVel[j] != 0.0 && gpCellInfo->cellSize[j] > 0.0) {
			htmp = fabs(hFrac*gpCellInfo->cellSize[j]/gpCellInfo->slStartVel[j]); 
			if (htmp<h) h=htmp;
		}
	}


	delmax = cellSizeMin*eps;
	dist = 0.0;

	for(i=0;i<maxSteps;i++) {

begin:
		slRK4OneStep(x,y,y0,h,nEqns);

		x1 = x + h/2;
		slRK4OneStep(x,y,y1tmp,0.5*h,nEqns);
		for(j=0;j<nEqns;j++) {
			y1[j] = y1tmp[j];
		}
		x2 = x1 + h/2;
		slRK4OneStep(x1,y1,y2tmp,0.5*h,nEqns);
		for(j=0;j<nEqns;j++) {
			y2[j] = y2tmp[j];
		}

		/* Error is difference between one step and two half steps */
		delworst = 0.0;
		for(j=0;j<nEqns;j++) {
			del[j] = fabs(y2[j] - y1[j]);
			if (del[j]>delworst) delworst=del[j];
		}

		if (delworst>slTINY) {
			if (delworst<=delmax) {
				h *= fac*pow(fabs(delmax/delworst),0.2);
			} else {
				h *= fac*pow(fabs(delmax/delworst),0.25);
				goto begin;
			}
		}

		disttmp = 0.0;
		for(j=0;j<nEqns;j++) {
			disttmp += (y2[j]-y[j])*(y2[j]-y[j]);
		}
		dist += sqrt(disttmp);

		ret=slRK4Stop(i,y,y2,  sideHit,sideIndex);

		if (ret==1) { /* segment crossed cell side, stop */
			for(j=0;j<nEqns;j++) {
				yi[j] = sideHit[j];
			}
			if (*icnt >= maxSteps+2) goto errorExit;
			slxMidPts0[*icnt] = yi[0];
			slyMidPts0[*icnt] = yi[1];
			slzMidPts0[*icnt] = yi[2];
			(*icnt)++;
			return 1;
		} else if (ret==-1) { /* segment did not cross cell side, continue */
			x = x2;
			for(j=0;j<nEqns;j++) {
				y[j] = y2[j];
			}
			if (*icnt >= maxSteps+2) goto errorExit;
			slxMidPts0[*icnt] = y[0];
			slyMidPts0[*icnt] = y[1];
			slzMidPts0[*icnt] = y[2];
			(*icnt)++;
		} else { /* error */
			goto errorExit;
		}
	}

	*sideIndex = -1;
	for(j=0;j<nEqns;j++) {
		yi[j] = y2[j];
	}
	if (*icnt >= maxSteps+2) goto errorExit;
	slxMidPts0[*icnt] = yi[0];
	slyMidPts0[*icnt] = yi[1];
	slzMidPts0[*icnt] = yi[2];
	(*icnt)++;
	return -1; /* Max iterations reached */

errorExit:
	*sideIndex = -1;
	for(j=0;j<nEqns;j++) {
		yi[j] = 0.0;
	}
	return 0;
}

/* =============================================================================
DESCRIPTION:
Fourth order Runge-Kutta method for solving a systems of ODEs, just one time step.

INPUT:
INPUT/OUTPUT:
OUTPUT:

RETURN:
1 = OK
0 = error
*/
int slRK4OneStep(double xim1,double *yim1,double *yi,double h,int nEqns)
{
	int j;
	double u,v[3],t1[3],t2[3],t3[3],t4[3];

	if (!yim1) return 0;
	if (!yi) return 0;

    u = xim1;
	for(j=0;j<nEqns;j++) {
		v[j] = yim1[j];
	}
	for(j=0;j<nEqns;j++) {
		t1[j] = slFunction(u,v,j);
	}
    
    u+=0.5*h;
	for(j=0;j<nEqns;j++) {
		v[j]+=0.5*h*t1[j];
	}
	for(j=0;j<nEqns;j++) {
		t2[j] = slFunction(u,v,j);
	}

	for(j=0;j<nEqns;j++) {
		v[j] = yim1[j] + 0.5*h*t2[j];
	}
	for(j=0;j<nEqns;j++) {
		t3[j] = slFunction(u,v,j);
	}

    u+=0.5*h;
	for(j=0;j<nEqns;j++) {
		v[j] = yim1[j] + h*t3[j];
	}
	for(j=0;j<nEqns;j++) {
		t4[j] = slFunction(u,v,j);
	}

	for(j=0;j<nEqns;j++) {
		yi[j] = yim1[j] + h*(t1[j] + 2*t2[j] + 2*t3[j] + t4[j])/6.0;
	}
	return 1;
}

/* =============================================================================
DESCRIPTION:
Determine whether a streamline segment has crossed a cell side and the 
coordinates of the crossing.

INPUT:
INPUT/OUTPUT:
OUTPUT:

RETURN:
1 = segment crossed cell side
0 = error occured
-1 = segment did not cross cell side
*/
int slRK4Stop(int iter, double *yOld,double *y,  double *sideHit,int *sideIndex)
{
	int j,sideCrossFlag,is;
	double sMin,num,den,s,vecSeg[3],vecSide[3];


	iter=iter;
	
	for(j=0;j<3;j++) {
		vecSeg[j] = y[j]-yOld[j];
	}
	sMin = slHUGE;
	sideCrossFlag = 0;
	for (is=0;is<2*3;is++) {

		for(j=0;j<3;j++) {
			vecSide[j] = gpCellInfo->cellSidePt[is][j]-yOld[j];
		}
		num = 0.0;
		den = 0.0;
		for(j=0;j<3;j++) {
			num += vecSide[j]*gpCellInfo->cellSideNorm[is][j];
			den += vecSeg[j]*gpCellInfo->cellSideNorm[is][j];
		}

		if (fabs(den)<slTINY) {
			s=1.1*slHUGE; /* segment perpendicular to cell face or 0 length*/
		} else {
			s=num/den;
		}

		/* Check values of s that are the intersection between the streamline segment 
		   and this cell side: 
		   s<0 - intersection behind segment
		   s=0 - intersection at segment start
		   s>0 && s<1 - intersection within segment
		   s=1 - intersection at segment end
		   s>1 - intersection past segment

		   den>0 - segment vector is pointing outward from cell side
		  
		   Find the closest side crossed, accounting for roundoff
		*/
		if (den>0.0 && s>0.0-slROUNDOFF && s<1.0+slROUNDOFF && s<sMin) {
			*sideIndex=is;
			sMin=s;
			sideCrossFlag=1;
		}
	}
	if (!sideCrossFlag) goto stopExit;

	for(j=0;j<3;j++) {
		sideHit[j] = yOld[j]+sMin*(y[j]-yOld[j]);
	}
	return 1;

stopExit:
	*sideIndex = -1;
	for(j=0;j<3;j++) {sideHit[j] = 0.0;}
	return -1;

/*errorExit:
	*sideIndex = -1;
	for(j=0;j<3;j++) {sideHit[j] = 0.0;}
	return 0;*/
}

/* =============================================================================
DESCRIPTION:
Streamline velocity as a function of position. Function is transformed so that
the cell center is at the origin.

INPUT:
INPUT/OUTPUT:
OUTPUT:

RETURN:
velocity (xdot) in j direction at position x
*/
double slFunction(double s,double *x,int j)
{
	int i;
	double vel;

	s=s;
	
	vel = 0.0;
	for(i=0;i<3;i++) {
		vel += gpCellInfo->cellDvDx[j][i]*x[i];
	}
	vel += gpCellInfo->cellVel[j];
	return vel;
}
