#define EXTERN22 extern
#include "Xappl.h"
#include "TracerDef.h"
#include <stdio.h>
#include <math.h>
#include "Render.h"
#include "Vrml.h"

#include "Defs.h"

int colorit(long count,double *level,Flt value,int *ic);
long doBumps(void *v1,void *v2,void *v3);
int Trace(int level, Flt weight, Ray *ray, Color color);
int TransmissionDirection(Surface *m1,Surface *m2,Vec I,Vec N,Vec T);
int SpecularDirection(Vec I,Vec N,Vec R);

 
 int Shade(Ray *ray,int level, Flt weight, Vec P,Vec N,Vec I, Isect *hit,Color col)
{
	Ray 	tray ;
	Color 	tcol ;
	Vec 	L, H ;
	Flt 	t ;
	Flt	diff ;
	Flt 	spec ;
	Flt 	costheta,costheta2 ;
	Surface	* surf ;
	Color	surf_color,surfcolor;
	double value;
	int l,ic;
	extern struct Material KdTreeMaterial;
	
	col[0] = col[1] = col[2] = 0.0 ;
	surf = hit -> isect_prim -> o_surf ;
	
	tray.Q=ray->Q;
	tray.Qsize=ray->Qsize;
	tray.traverseStack=ray->traverseStack;

	if(surf->flag & M_COLORLEVEL){
	    colorit(surf->ColorCount,surf->ColorLevel,
	    hit ->isect_value,&ic);
	    ColorToVec(surf->ColorValue[ic],surf_color);
	}else{
	    ColorToVec(surf->diffuse,surf_color);
	}
			    
	if(surf->flag & M_PATTERN){
	    (*surf->PatternRoutine)(surf->PatternData,hit,surf_color);
	}
	
	if(surf->flag & M_BUMP){
	    (*surf->BumpRoutine)(surf->BumpData,hit,N);
	}
	
	value=pow(10,KdTreeMaterial.ColorLevel[2]);
	for (l = 1; l < rd->nLights; l++) {
		if(rd->Lights[l].type != 1)continue;
		VecSub(rd->Lights[l].light_pos, P, L);
		if (VecDot(N,L) >= 0.0) {
			t = VecNormalize(L);
			VecCopy(P, tray.P);
			VecCopy(L, tray.D);
			if (rd->Shadow(&tray, t)) {
			    costheta=VecDot(N,L);
				if(t)value += costheta/(t*t);
			}
		}
	}
	
	colorit(KdTreeMaterial.ColorCount,KdTreeMaterial.ColorLevel,
			log10(value),&ic);
	ColorToVec(KdTreeMaterial.ColorValue[ic],surf_color);
	
	for (l = 0; l < rd->nLights; l++) {
		if(rd->Lights[l].type != 0)continue;
		VecSub(rd->Lights[l].light_pos, P, L);
		if (VecDot(N,L) >= 0.0) {
			t = VecNormalize(L);
			VecCopy(P, tray.P);
			VecCopy(L, tray.D);
			/*
			rd->nShadows ++ ;
			*/
			if (rd->Shadow(&tray, t)) {
			    costheta=VecDot(N,L);
			    diff = costheta * rd->Lights[l].light_brightness ;
			    surfcolor[0] = surf_color[0]*rd->Lights[l].red;
			    surfcolor[1] = surf_color[1]*rd->Lights[l].green;
			    surfcolor[2] = surf_color[2]*rd->Lights[l].blue;
			    VecAddS(diff,surfcolor, col, col) ;
			    if(surf->flag & M_SPECULAR){
					VecComb((-1.),L,(2.*costheta),N,H);
					VecNormalize(H) ;
					costheta2=VecDot(H,I);
					if(costheta2 < 0.0){
					    spec = surf -> specular 
						    * pow(-costheta2, surf -> phong) ;
					    col[0] += spec ;
					    col[1] += spec ;
					    col[2] += spec ;
					}
			    }
			}
		}
	}

	VecCopy(P, tray.P);

	if(surf -> flag & M_AMBIENT) {
		col[0] += surf_color[0]*surf->ambient.red ;
		col[1] += surf_color[1]*surf->ambient.green ;
		col[2] += surf_color[2]*surf->ambient.blue ;
	}
	
	if(surf -> flag & M_MIRROR) {
	/*
		rd->nReflected ++ ;
	*/
		SpecularDirection(I, N, tray.D);
		VecNormalize(tray.D);
		Trace(level + 1, surf -> mirrorfactor * weight, &tray, tcol);
		tcol[0] *= surf->mirror.red ;
		tcol[1] *= surf->mirror.green ;
		tcol[2] *= surf->mirror.blue ;
		VecAddS(surf -> mirrorfactor, tcol, col, col);
	}

	if (surf -> flag & M_TRANSMIT) { 
	/*
		rd->nRefracted ++ ;
	*/
		if (hit -> isect_enter) 
			TransmissionDirection((Surface	*)NULL, surf, I, N, tray.D) ;
		else	
			TransmissionDirection(surf, (Surface	*)NULL, I, N, tray.D) ;
		Trace(level + 1, surf -> transmitfactor * weight, &tray, tcol) ;
		VecAddS(surf -> transmitfactor, tcol, col, col) ;
	}

	if(surf->flag & M_TRANSPARENT){
	    VecCopy(I, tray.D);
		Trace(level + 1,weight, &tray, tcol) ;
		tcol[0] *= surf->transparent.red ;
		tcol[1] *= surf->transparent.green ;
		tcol[2] *= surf->transparent.blue ;
		VecAddS((1.0-surf->opacity), tcol, col, col) ;
	}
	return 0;
}

int SpecularDirection(Vec I,Vec N,Vec R)
{
	VecComb(1.0/fabs(VecDot(I,N)), I, 2.0, N, R);
	VecNormalize(R);
	return 0;
}

int TransmissionDirection(Surface *m1,Surface *m2,Vec I,Vec N,Vec T)
{
	Flt n1, n2, eta, c1, cs2 ;
	n1 = m1 ? m1 -> index : 1.0 ;
	n2 = m2 ? m2 -> index : 1.0 ;
	eta = n1/n2 ;

	c1 = -VecDot(I,N);
	cs2 = 1.0 - eta * eta*(1.0 - c1*c1);
	if (cs2 < 0.0)
		return 0;
	VecComb(eta, I, eta*c1-sqrt(cs2), N, T);
	return(1);
}
