#ifndef __UFILESBATCH3D__
#define __UFILESBATCH3D__


#define USE_XY			0
#define USE_X			1
#define USE_Y 			2

#define X_PLANE	0
#define Y_PLANE	1
#define Z_PLANE	2
#define G_PLANE	3

#define max(x1,x2)    (((x1) > (x2)) ? (x1) : (x2))
#define min(x1,x2)    (((x1) < (x2)) ? (x1) : (x2))


int IsOption(void);

int FilesGetData(double **data,long *length,
                 long CurrentFrame,struct FileInfo2 *Files);


struct DrawInfo3{
    struct FilePIOInfo *pio;
	struct areadata *ai;
    double rx,ry,rz,sx,sy,sz,tx,ty,tz;
	double p1x,p1y,p1z,v1x,v1y,v1z,v2x,v2y,v2z,d,od;
	double v3x,v3y,v3z;
	double l1,l2,l3;
	double drHalf,dsHalf;
	double dr,ds;
	long nx,ny;
	double *sout;
};

struct dRange{
      double xmin,xmax,ymin,ymax,dxlim,dylim;
};


	struct DrawCellStuff{
		struct screenData *ss;
		struct dRange *r;
	    double vmaxLen;
	    double vmax;
	    double turn;
	    double arrow;
	    double vmax2;
	    double dx;
	    double dy;
	};


int doSage3DGetDataBlocks(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);
int doSage3DGetLineDataBlocks(struct FileInfo2 *Files,struct linedata *li);
int doSage3DGetAreaDataBlock(struct FileInfo2 *Files,struct areadata *ai);

#endif
