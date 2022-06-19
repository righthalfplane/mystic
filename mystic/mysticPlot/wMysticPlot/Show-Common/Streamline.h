#ifndef Streamline_h  /* prevent multiple inclusions of header file */
#define Streamline_h

int slCreateList(int nsl);
int slDestroyList(int nsl);
int slAddPointToList(int is, int nsl, double *pt);
int slGetPointFromList(int is, int nsl, int ip, double *pt);
int slCellPath(double *cellCenter0,double *cellSize0,double *cellVel0,
			   double cellDvDx0[3][3],double *slStartPt0,int slStartSide0,double speedCutoff,
			   double *slEndPt0,int *slEndSide0,
			   int *icnt,int maxSteps,double eps,double hFrac,double *slxMidPts0,double *slyMidPts0,double *slzMidPts0);
int slRK4Adaptive(double xim1,double *yim1,int nEqns,double *yi,int *sideIndex,
				  int *icnt,int maxSteps,double eps,double hFrac,double *slxMidPts0,double *slyMidPts0,double *slzMidPts0);
int slRK4OneStep(double xim1,double *yim1,double *yi,double h,int nEqns);
int slRK4Stop(int iter,double *yOld,double *y,  double *sideHit,int *sideIndex);
double slFunction(double s,double *x,int j);
int zerol(char *s,unsigned long n);

#endif
