#ifndef __GWORLD__
#define __GWORLD__

#include <stdio.h>

struct sWorld{
    struct Icon *myIcon;
	uRect pictRect;
	long xsize;
	long ysize;
	int open;
	int depth;
};

#define GWorldDepth32	32
#define GWorldDepth8	8

int uOpenGWorld(struct Icon *myIcon,struct sWorld *World,uRect *Box,int depth);
int WriteHexFromGWorld(char *name,struct sWorld *World,FILE *out);
void StartGWorld(struct sWorld *World);
void StopGWorld(struct sWorld *World);
void CloseGWorld(struct sWorld *World);
int PutTrueGWorld(long xsize,long ysize,unsigned char *image,struct sWorld *World);
int SaveTrueFromGWorld(char *name,struct sWorld *World,FILE *out);
int GetTrueFromGWorld(long *xsize,long *ysize,unsigned char **image,struct sWorld *World);

int PutHdfGWorld(long xsize,long ysize,unsigned char *image,unsigned char *r,
                unsigned char *g,unsigned char *b,struct sWorld *World);
int GetHdfFromGWorld(long *xsize,long *ysize,unsigned char **image,struct sWorld *World);

#endif

