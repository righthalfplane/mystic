
#define EXTERN22 extern
#include "Xappl.h"
#include "TracerDef.h"
#include <stdio.h>
#include <math.h>
#include "Render.h"
#include "Vrml.h"

#include "Defs.h"

int doVrmlTexture(ColorVEC surf_color,double s1x,double s2x,long ns,long ip,struct tState *tS);
int getColor(float *color,int mat,int *m,struct tState *tS);

typedef struct t_trincdata2 {
	Vec 		tri_end ;
	Vec 		v1;
	Vec 		v2;
	Vec 		vn;
	Vec 		n1;
	Vec 		n2;
	Vec 		n3;
	Flt 		c1;
	Flt 		c2;
	Flt 		c3;
	Flt 		c4;
	Flt 		dw;
	Flt 		value1;
	Flt 		value2;
	Flt 		value3;
	long 		ns;
	long 		ip;
	long		mat;
	long        m[3];
	struct tState *tS;
} TriNCData2 ;

static int TriNCPrint(Object *obj);

static int TriNCIntersect(Object *obj, Ray *ray, Isect *hit);

static int TriNCNormal(Object *obj, Isect *hit, Point P, Point N);

static int TriNCShade(Object *obj,int level, Flt weight, Vec P,Vec N,Vec I, Isect *hit,Color7 col,Ray *ray);

static ObjectProcs TriNCProcs = {
	TriNCPrint,
	TriNCIntersect,
	TriNCNormal,
	TriNCShade,
} ;

static int TriNCShade(Object *obj,int level, Flt weight, Vec P,Vec N,Vec I, Isect *hit,Color7 col,Ray *ray)
{
	TriNCData2 *qp;
	struct tState *tS;
	ColorVEC	surf_color,specular /*ambient */;
	struct Material surf;
	int MaterialType,mat;
	struct vrmlMaterial *vrmlMat;
	double s1x,s2x;
	int ns,ip,l;
	Vec 	L, H ;
	Flt 	t ;
	Ray 	tray ;
	Flt		diff ;
	Flt 	spec ;
	Flt 	costheta,costheta2 ;
	Color7	surfcolor;
	
	//hit=hit;

	tray.Q=ray->Q;
	tray.Qsize=ray->Qsize;
	tray.traverseStack=ray->traverseStack;

	col[0] = col[1] = col[2] = 0.0 ;
	specular[0]=specular[1]=specular[2]=0;
		
	//level=level;
	//weight=weight;
	
	qp = (TriNCData2 *) obj -> o_data ;
	if(!qp)return 0;
	
	tS=qp->tS;
	if(!qp)return 0;
	
	zerol((char *)&surf,sizeof(struct Material));
	surf.opacity=tS->opacity;

	MaterialType=tS->MaterialType;
	vrmlMat=tS->m;
	
	s1x=hit->f1;
	s2x=hit->f2;
	
	ns=(int)qp->ns;
	ip=(int)qp->ip;

	if(MaterialType == VRML_BY_FACE){
	    mat=(int)qp->mat;
	    if(vrmlMat && (mat < vrmlMat->max)){
	        surf_color[0]=vrmlMat->diffusedata[mat*3];
	        surf_color[1]=vrmlMat->diffusedata[mat*3+1];
	        surf_color[2]=vrmlMat->diffusedata[mat*3+2];

			surf.phong= 128*vrmlMat->shinedata[mat];

			specular[0]=vrmlMat->speculardata[mat*3];
			specular[1]=vrmlMat->speculardata[mat*3+1];
			specular[2]=vrmlMat->speculardata[mat*3+2];
/*
			ambient[0]=vrmlMat->ambientdata[mat*3];
			ambient[1]=vrmlMat->ambientdata[mat*3+1];
			ambient[2]=vrmlMat->ambientdata[mat*3+2];
*/

	        surf.flag=vrmlMat->flagsdata[mat];
	    }else{
	        surf_color[0]=.8f;
	        surf_color[1]=.8f;
	        surf_color[2]=.8f;
	  /*
			ambient[0]=.2f;
			ambient[1]=.2f;
			ambient[2]=.2f;
	  */
	        surf.flag = M_AMBIENT;
	    }
	}else if(MaterialType == VRML_BY_VERTEX){
	    long m[3];
	    double *d;


	    m[0]=qp->m[0];
	    m[1]=qp->m[1];
	    m[2]=qp->m[2];

	    surf.flag=vrmlMat->flagsdata[m[0]];
	    surf.phong= 128*vrmlMat->shinedata[m[0]];

	    if(surf.flag & M_DIFFUSE){

	        d=vrmlMat->diffusedata;
	        surf_color[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
	                                s2x*(d[m[2]*3]-d[m[0]*3]);
	        surf_color[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
	                                  s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
	        surf_color[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
	                                  s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
	    }else{
	        surf_color[0]=0;
	        surf_color[1]=0;
	        surf_color[2]=0;
	    }

	    if(surf.flag & M_SPECULAR){
	        d=vrmlMat->speculardata;

	        specular[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
	                              s2x*(d[m[2]*3]-d[m[0]*3]);
	        specular[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
	                                s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
	        specular[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
	                                s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
	    }

	    if(surf.flag & M_AMBIENT){
	    /*
	        d=vrmlMat->ambientdata;
	        ambient[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
	                             s2x*(d[m[2]*3]-d[m[0]*3]);
	        ambient[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
	                               s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
	        ambient[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
	                               s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
	    */
	    }
	}else{
	    surf.flag=0;
	    surf_color[0]=0;
	    surf_color[1]=0;
	    surf_color[2]=0;
	}

	if(tS->flagTexture)doVrmlTexture(surf_color,s1x,s2x,ns,ip,tS);
	

	for (l = 0; l < rd->nLights; l++) {
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
			    if(surf.flag & M_SPECULAR){
					VecComb((-1.),L,(2.*costheta),N,H);
					VecNormalize(H) ;
					costheta2=VecDot(H,I);
					if(costheta2 < 0.0){
					    spec = pow(-costheta2, surf.phong) ;
					    col[0] += spec*specular[0] ;
					    col[1] += spec*specular[1] ;
					    col[2] += spec*specular[2] ;
					}
			    }
			}
		}
	}



	

	return 0;
}


static int TriNCPrint(Object *obj)
{
	TriNCData2 * sp ;

	if(!obj)return 1;
	
	sp = (TriNCData2 *) obj -> o_data ;
	
	if(!sp)return 0;

	return 0;
}

static Object *CheckForSimple(int npoints,Surface *s, Vec *points,Vec *normals,struct tState *tS)
{
	float color[9];
	Vec col[3];
	int mat,m[3];
	int nSame;
	int cSame;
	
	if(npoints != 3)return NULL;
	if(!tS || tS->flagTexture)return NULL;
	
    mat=(int)points[3][0];
	m[0]=(int)points[4][0];
	m[1]=(int)points[4][1];
	m[2]=(int)points[4][2];
    getColor(color,mat,m,tS);
    
    cSame=FALSE;
    if(color[0] == color[3] && color[0] == color[6] &&
       color[1] == color[4] && color[1] == color[7] &&
       color[2] == color[5] && color[2] == color[8]){
    	cSame=TRUE;
    }
    
    nSame=FALSE;
    if(normals[0][0] == normals[1][0] && normals[0][0] == normals[2][0] &&
       normals[0][1] == normals[1][1] && normals[0][1] == normals[2][1] &&
       normals[0][2] == normals[1][2] && normals[0][2] == normals[2][2]){
    	nSame=TRUE;
    }
    
    col[0][0]=color[0];
    col[0][1]=color[1];
    col[0][2]=color[2];
	
    col[1][0]=color[3];
    col[1][1]=color[4];
    col[1][2]=color[5];
	
    col[2][0]=color[6];
    col[2][1]=color[7];
    col[2][2]=color[8];
    
    if(cSame && nSame){
        return MakeTriC1N1(npoints,s,points,normals,col);
    }
	
	return MakeTriN2(npoints,s,points,normals,col);
}

Object *MakeTriNCV(int npoints,Surface *s, Vec *points,Vec *normals,struct tState *tS)
{
	Flt d,dmax,dmin,den;
	Vec v1,v2;
	TriNCData2 *qp;
	Object * tmp;
	int i,j;

	tmp=NULL;
	qp=NULL;
	
	tmp=CheckForSimple(npoints,s,points,normals,tS);
	if(tmp)return tmp;
	
	if(npoints != 3){
	    WarningBatch("MakeTriNCV Expects 3 Points\n");
	    goto ErrorOut;
	}

	if(!(tmp = (Object *) cMalloc (sizeof(Object),8711))){
	    WarningBatch("MakeTriNCV out of Memory #1\n");
	    goto ErrorOut;
	}
	tmp -> o_type = T_VRML ;
	tmp -> o_procs = &TriNCProcs ;
	tmp -> o_surf = s ;
	if(!(qp = (TriNCData2 *) cMalloc (sizeof(TriNCData2),8712))){
	    WarningBatch("MakeTriNCV out of Memory #2\n");
	    goto ErrorOut;
	}
	
	tmp -> o_data = (void *) qp ;
	
	VecCopy(points[0],qp->tri_end);
	
	VecCopy(normals[0],qp->n1);
	VecSub(normals[1],normals[0],qp->n2);
	VecSub(normals[2],normals[0],qp->n3);
	
	VecSub(points[1],qp->tri_end,qp->v1);
	VecSub(points[2],qp->tri_end,qp->v2);
/*
	qp->value1=values[0];
	qp->value2=values[1]-values[0];
	qp->value3=values[2]-values[0];
*/


	qp->mat=(long)points[3][0];
	qp->ns=(long)points[3][1];
	qp->ip=(long)points[3][2];

	qp->m[0]=(long)points[4][0];
	qp->m[1]=(long)points[4][1];
	qp->m[2]=(long)points[4][2];


	qp->tS=tS;


	VecCopy(qp->v1,v1);
	VecCopy(qp->v2,v2);
		
	VecCross(v1,v2,qp->vn);
	VecNormalize(qp->vn);
	
	qp->dw=VecDot(qp->vn,qp->tri_end);
	
	
	qp->c1=VecDot(v2,v2);
	qp->c2=VecDot(v1,v2);
	qp->c3=VecDot(v1,v1);
	qp->c4=VecDot(v2,v1);
	
    den=qp->c1*qp->c3-qp->c2*qp->c4;
    if(den == 0.){
/*    
         sprintf(WarningBuff,"Triangle Error den = %f\n",den);
         WarningBatch(WarningBuff);
*/    
     	den=1;
    }
	den = 1./den;
	qp->c1 *= den;
	qp->c2 *= den;
	qp->c3 *= den;
	qp->c4 *= den;



	/*
	 * figure out dmin and dmax values for 
	 * each of the slabs...
	 */
	
	for (i = 0 ; i < NSLABS ; i ++) {
		dmin = HUGE ;
		dmax = - HUGE ;

		for (j = 0 ; j < npoints ; j ++) {
			d = VecDot(rd->Slab[i], points[j]) ;
			if (d < dmin) dmin = d ;
			if (d > dmax) dmax = d ;
		}
		tmp -> o_dmin[i] = dmin ;
		tmp -> o_dmax[i] = dmax ;
	}
	return tmp;
ErrorOut:
	if(tmp)cFree((char *)tmp);
	if(qp)cFree((char *)qp);
	return NULL;
}
static int TriNCIntersect(Object *obj, Ray *ray, Isect *hit)
{

	Flt den,f1,f2,t,s1,s2;
	Point P,PP;
	TriNCData2 * qp ;
	
	if(!obj || !ray || !hit)return 0;

	qp = (TriNCData2 *) obj -> o_data ;

	den=VecDot(ray->D,qp->vn);
	if(fabs(den) < rd->rayeps){
	    return 0;
	}

	t=(qp->dw-(VecDot(ray->P,qp->vn)))/den;

	VecAddS(t,ray->D,ray->P,P);
			
	VecSub(P,qp->tri_end,PP);
		
	f1=VecDot(PP,qp->v1);
	
	f2=VecDot(PP,qp->v2);
	
        s1=(f1*qp->c1-f2*qp->c2);
	if(s1 < 0. || s1 > 1.0){
	     return 0;
	}
	
        s2=(f2*qp->c3-f1*qp->c4);
	
	if(s2 < 0.){
	     return 0;
	}
	
	if((s1+s2) > 1.0){
	     return 0;
	}
	
	if (t < rd->rayeps) {
		return(0);
	}

	hit -> isect_t = t ;
	hit -> isect_enter = 0;
	hit -> isect_prim = obj ;
	hit -> isect_surf = obj -> o_surf ;
	hit -> f1 = s1;
	hit -> f2 = s2;
	
	return (1);
}

static int TriNCNormal(Object *obj, Isect *hit, Point P, Point N)
{
	TriNCData2 * qp ;
	Flt s1,s2;
	
	if(!obj || !hit)return 1;
	
	qp = (TriNCData2 *) obj -> o_data ;
	
	//P=P;
	
	s1=hit->f1;
	s2=hit->f2;

	N[0]=qp->n1[0]+s1*qp->n2[0]+s2*qp->n3[0];
	N[1]=qp->n1[1]+s1*qp->n2[1]+s2*qp->n3[1];
	N[2]=qp->n1[2]+s1*qp->n2[2]+s2*qp->n3[2];
	hit->isect_value=qp->value1+s1*qp->value2+s2*qp->value3;
	VecNormalize(N);
	return 0;
}

