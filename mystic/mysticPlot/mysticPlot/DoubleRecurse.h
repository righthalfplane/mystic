
#include <math.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  1
#endif

void FindDoubleRecurseSoln ( double A, double B, double alpha, double beta,
							 double *C, double *exponent, double *D );


int FindDoubleRecurseSoln2 ( double A, double B, double alpha, double beta,
							 double *C, double *exponent, double *D,
							 double exponentToBeat );

