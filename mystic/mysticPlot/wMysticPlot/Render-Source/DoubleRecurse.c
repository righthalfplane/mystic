/*
 *
 * RayTrace Software Package, release 3.0.  May 3, 2006.
 *
 * Mathematics Subpackage (VrMath)
 *
 * Author: Samuel R. Buss
 *
 * Software accompanying the book
 *		3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *		by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.  Please acknowledge
 *   all use of the software in any publications or products based on it.
 *
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG
 *
 */

#include <math.h>
#include <assert.h>
#include "DoubleRecurse.h"
/*

//  Consider a function which satisfies the following properties:
//    1.   f(N) = 1 + A*f(alpha*N) + B*f(beta*N)
//           where A+B > 1 and alpha, beta in (0,1].
//    2    f(1) = 1.
//
//  These conditions are satisfied by setting f(x) = C N^exponent + D,
//    where C, exponent, D are the values returned by FindDoubleRecurseSoln
*/
void FindDoubleRecurseSolnOld ( double A, double B, double alpha, double beta,
							 double* C, double* exponent, double* D );

void FindDoubleRecurseSoln ( double A, double B, double alpha, double beta,
							 double* C, double* exponent, double* D )
{
	double denom;
	double ZdenomInv;
	double AlogAlpha;
	double BlogBeta;
	double X;				
	double alphaExpX;			
	double betaExpX;			
	double funcValue;		
	double deltaX;
	/*  Pre-conditions */
	assert ( A+B>1.0 );
	assert ( 0.0<alpha && alpha<=1.0 );
	assert ( 0.0<beta && beta<=1.0 );

	denom = A+B-1.0;
	ZdenomInv = 1.0/denom;
	AlogAlpha = A*log(alpha);
	BlogBeta = B*log(beta);

	*D = -ZdenomInv;
	*C = 1.0+ZdenomInv;			/* Same as (A+B)/(A+B-1) = (A+B-1+1)/(A+B-1). */

	/* Use Newton's method to find exponent */
	X = 0;				/*  Initial choice for exponent */
	funcValue = A+B;		/* Since alphaExpX and betaExpX equal 1 */
	deltaX = (1.0-funcValue)/(AlogAlpha+BlogBeta);
	while ( TRUE ) {
		X += deltaX;
		if ( deltaX<1.0e-12 ) {
			break;
		}
		alphaExpX = pow(alpha, X);
		betaExpX = pow(beta, X);
		funcValue = A*alphaExpX + B*betaExpX;
		deltaX = (1.0-funcValue)/(AlogAlpha*alphaExpX+BlogBeta*betaExpX);
	}
	*exponent = X;
}
/*
//  Consider a function which satisfies the following properties:
//    1.   f(N) = 1 + A*f(alpha*N) + B*f(beta*N)
//           where A+B > 1 and alpha, beta in (0,1].
//    2    f(1) = 1.
//
//  These conditions are satisfied by setting f(x) = C N^exponent + D,
//    where C, exponent, D are the values returned by FindDoubleRecurseSoln
//  This version takes an "exponentToBeat".   If the exponent cannot be
//	  beaten (i.e., if exponent would be greater than exponentToBeat, it
//	  returns false without computing C, D and exponent.  Otherwise, it
//	  sets values of C, D and exponent with exponent<exponentToBeat and
//	  returns TRUE.
*/
int FindDoubleRecurseSoln2 ( double A, double B, double alpha, double beta,
							 double* C, double* exponent, double* D,
							 double exponentToBeat )
{
	double X;			/*  Initial choice for exponent */
	double alphaExpX;
	double betaExpX;
	double funcValue;
	double denom;
	double ZdenomInv;
	double AlogAlpha;
	double BlogBeta;
	double deltaX;
	/* Pre-conditions */
	assert ( A+B>1.0 );
	assert ( 0.0<alpha && alpha<=1.0 );
	assert ( 0.0<beta && beta<=1.0 );

	/*  First check on exponentToBeat */
	/*	Also, set some of the values for the first Newton iteration. */
	X = exponentToBeat;			/* Initial choice for exponent */
	alphaExpX = pow(alpha, X);
	betaExpX = pow(beta, X);
	funcValue = A*alphaExpX + B*betaExpX;
	if ( funcValue>=1.0 ) {
		return FALSE;
	}

	/* We are going to beat the exponentToBeat.  */
	denom = A+B-1.0;
	ZdenomInv = 1.0/denom;
	AlogAlpha = A*log(alpha);
	BlogBeta = B*log(beta);

	*D = -ZdenomInv;
	*C = 1.0+ZdenomInv;			/* Same as (A+B)/(A+B-1) = (A+B-1+1)/(A+B-1). */

	deltaX = (1.0-funcValue)/(AlogAlpha*alphaExpX+BlogBeta*betaExpX);
	X += deltaX;
	if ( X<0.0 ) {
		/* If undershot zero, restart Newton iteration at zero */
		funcValue = A+B;		/* Since alphaExpX and betaExpX equal 1 */
		deltaX = (1.0-funcValue)/(AlogAlpha+BlogBeta);
		X = deltaX;
	}
	else {
		deltaX = -deltaX;
	}
	while ( TRUE ) {
		if ( deltaX<1.0e-12 ) {
			break;
		}
		alphaExpX = pow(alpha, X);
		betaExpX = pow(beta, X);
		funcValue = A*alphaExpX + B*betaExpX;
		deltaX = (1.0-funcValue)/(AlogAlpha*alphaExpX+BlogBeta*betaExpX);
		X += deltaX;
	}
	*exponent = X;

	return TRUE;
}

/*
//  Consider a function which satisfies the following properties:
//    1.   f(N) = 1 + A*f(alpha*N) + B*f(beta*N)
//           where A+B > 1 and alpha, beta in (0,1].
//    2    f(1) = 1.
//
//  These conditions are satisfied by setting f(x) = C N^exponent + D,
//    where C, exponent, D are the values returned by FindDoubleRecurseSoln
*/

void FindDoubleRecurseSolnOld ( double A, double B, double alpha, double beta,
							 double* C, double* exponent, double* D )
{
	double denom;
	double ZdenomInv;
	double AlogAlpha;
	double BlogBeta;
	double alphaExpX;
	double betaExpX;
	double funcValue;
	double deltaX;
	double X;	
	/* Pre-conditions */
	assert ( A+B>1.0 );
	assert ( 0.0<alpha && alpha<=1.0 );
	assert ( 0.0<beta && beta<=1.0 );

	denom = A+B-1.0;
	ZdenomInv = 1.0/denom;
	AlogAlpha = A*log(alpha);
	BlogBeta = B*log(beta);

	*D = -ZdenomInv;
	*C = 1.0+ZdenomInv;			/* Same as (A+B)/(A+B-1) = (A+B-1+1)/(A+B-1). */

	/* Use Newton's method to find exponent */
	X = 0;				/* Initial choice for exponent */
	while ( TRUE ) {
		alphaExpX = pow(alpha, X);
		betaExpX = pow(beta, X);
		funcValue = A*alphaExpX + B*betaExpX;
		deltaX = (1.0-funcValue)/(AlogAlpha*alphaExpX+BlogBeta*betaExpX);
		X += deltaX;
		if ( deltaX<1.0e-12 ) {
			break;
		}
	}
	*exponent = X;
}


