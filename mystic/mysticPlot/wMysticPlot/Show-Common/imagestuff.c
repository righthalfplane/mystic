#define EXTERN22 extern
#include "Xappl.h"

/*
 *	iopen -
 *
 *				Paul Haeberli - 1984
 *
 */
 
#define I_IORW  256
#define I_IOWRT   2
#define I_IOREAD  1
#define I_IOERR  32

#include	<stdio.h> 
#include	<stdlib.h>
#include    <string.h>
#include	"image.h"

#define INTSIZE 4
int intsize = INTSIZE;

unsigned short *ibufalloc(IMAGE *image);

void cvtints( register int buffer[],register int n);

int putLong(long n,FILE *out);
int putInt(long n,FILE *out);

int crayimgwrite(FILE *f, IMAGE *image, int count);


int cimg_write(FILE *f, unsigned int *buffer,int count);
int cwrtshort(FILE *f, short *shortptr, int nitems);
int cwrtlint(FILE *f, int *intptr, int nitems);

void isetcolormap(IMAGE *image, int colormap);

void img_rle_expand(unsigned short *rlebuf, int ibpp,
			unsigned short *expbuf, int obpp);
			
unsigned int img_optseek(IMAGE *image, unsigned int offset);
int img_read(IMAGE *image, char *buffer, int count);

void img_setrowsize(IMAGE *image, int cnt, int y, int z);
int img_getrowsize(IMAGE *image);

int iclose(IMAGE *image);

void cvtshorts( register unsigned short buffer[],register int n);

unsigned int reverse(register unsigned int lwrd);

int img_rle_compact(unsigned short *expbuf, int ibpp,
			unsigned short *rlebuf, int obpp, int cnt);
			
int img_write(IMAGE *image, char *buffer,int count);

int img_badrow(IMAGE *image, unsigned int y, unsigned int z);

int iflush(IMAGE *image);
void cvtimage(register int buffer[]);

void i_errhdlr();

unsigned int img_optseek(IMAGE *image, unsigned int offset);
unsigned int img_seek(IMAGE *image, unsigned int y, unsigned int z);
void img_setrowsize(IMAGE *image, int cnt, int y, int z);
void isetname(IMAGE *image, char *name);
void cvtcrayshort(short *buffer, int nitems);

static FILE *out;

IMAGE *imgopen(long, char *, char *,unsigned int, unsigned int,
		unsigned int, unsigned int, unsigned int);

IMAGE *iopen(char *file, char *mode, unsigned int type, unsigned int dim,
		unsigned int xsize, unsigned int ysize, unsigned int zsize)
{
    return(imgopen(0, file, mode, type, dim, xsize, ysize, zsize));
}

IMAGE *fiopen(long f, char *mode, unsigned int type, unsigned int dim,
		unsigned int xsize, unsigned int ysize, unsigned int zsize)
{
    return(imgopen(f, 0, mode, type, dim, xsize, ysize, zsize));
}

IMAGE *imgopen(long f, char *file, char *mode,
		unsigned int type, unsigned int dim,
		unsigned int xsize, unsigned int ysize, unsigned int zsize)
{
	register IMAGE 	*image;
	register int rw;
	int tablesize, acttablesize;
	register int i, max;

	image = (IMAGE*)cMalloc(sizeof(IMAGE),9998);
	if(!image ) {
	    sprintf(WarningBuff,"fopen: error on image struct alloc\n");
	    WarningBatch(WarningBuff);
	    return NULL;
	}
	zerol((char *)image,sizeof(IMAGE));
	
	rw = mode[1] == '+';
	if(rw) {
	    sprintf(WarningBuff,"fopen: read/write mode not supported\n");
	    WarningBatch(WarningBuff);
		return NULL;
	}
	if (*mode=='w') {
		if (file) {
		    out=fopen(file,"wb");
		}
		if (!out) {
		    sprintf(WarningBuff,"fopen: can't open output file %s\n",file);
	    	WarningBatch(WarningBuff);
		    return NULL;
		}
		image->imagic = IMAGIC;
		image->type = type;
		image->xsize = xsize;
		image->ysize = 1;
		image->zsize = 1;
		if (dim>1)
		    image->ysize = ysize;
		if (dim>2)
		    image->zsize = zsize;
		if(image->zsize == 1) {
		    image->dim = 2;
		    if(image->ysize == 1)
			image->dim = 1;
		} else {
		    image->dim = 3;
		}
		image->min = 10000000;
		image->max = 0;
		isetname(image,"no name"); 
		image->wastebytes = 0;
		image->dorev = 0;

	} else {
		if (file)
		    out=fopen(file,"wb");
		if (!out)
		    return(NULL);
		if (fread((char *)image,1,sizeof(IMAGE),out) != sizeof(IMAGE)) {
		    sprintf(WarningBuff,"fopen: error on read of image header\n");
	    	WarningBatch(WarningBuff);
		    return NULL;
		}
		if( ((image->imagic>>8) | ((image->imagic&0xff)<<8)) 
							     == IMAGIC ) {
		    image->dorev = 1;
		    cvtimage((int *)image);
		} else
		    image->dorev = 0;
		if (image->imagic != IMAGIC) {
			sprintf(WarningBuff,"fopen: bad magic in image file %x\n",image->imagic);
	    	WarningBatch(WarningBuff);
		    return NULL;
		}
	}
	if (rw)
	    image->flags = I_IORW;
	else if (*mode != 'r')
	    image->flags = I_IOWRT;
	else
	    image->flags = I_IOREAD;
	if(ISRLE(image->type)) {
	    tablesize = image->ysize*image->zsize*sizeof(int);
	    acttablesize = image->ysize*image->zsize*intsize;
	    image->rowstart = (unsigned int *)cMalloc(tablesize,8474);
	    image->rowsize = (int *)cMalloc(tablesize,8584);
	    if( image->rowstart == NULL || image->rowsize == NULL ) {
			sprintf(WarningBuff,"iopen: error on table alloc\n");
	    	WarningBatch(WarningBuff);
			return NULL;
	    }
	    image->rleend = 512L+2*acttablesize;
	    if (*mode=='w') {
			max = image->ysize*image->zsize;
			for(i=0; i<max; i++) {
			    image->rowstart[i] = 0;
			    image->rowsize[i] = -1;
			}
	    } else {
			tablesize = image->ysize*image->zsize*sizeof(int);
			fseek(out, 512L, 0);
			if (fread((char *)image->rowstart,1,tablesize,out) != tablesize) {
			    sprintf(WarningBuff,"iopen: error on read of rowstart\n");
	    		WarningBatch(WarningBuff);
			    return NULL;
			}
			if(image->dorev)
			    cvtints((int *)image->rowstart,tablesize);
			if (fread((char *)image->rowsize,1,tablesize,out) != tablesize) {
			    sprintf(WarningBuff,"iopen: error on read of rowsize\n");
	    		WarningBatch(WarningBuff);
			    return NULL;
			}
			if(image->dorev)
			    cvtints(image->rowsize,tablesize);
	    }
	}
	image->cnt = 0;
	image->ptr = 0;
	image->base = 0;
	if( (image->tmpbuf = ibufalloc(image)) == 0 ) {	
	    sprintf(WarningBuff,"iopen: error on tmpbuf alloc %d\n",image->xsize);
	    WarningBatch(WarningBuff);
	    return NULL;
	}
	image->x = image->y = image->z = 0;
	image->file = f;
	image->offset = 512L;			/* set up for img_optseek */
	fseek(out, 512L, 0);
	return(image);
}

unsigned short *ibufalloc(IMAGE *image)
{
    return (unsigned short *)cMalloc(IBUFSIZE(image->xsize),8674);
}

unsigned int reverse(register unsigned int lwrd) 
{
    return ((lwrd>>24) 		| 
	   (lwrd>>8 & 0xff00) 	| 
	   (lwrd<<8 & 0xff0000) | 
	   (lwrd<<24) 		);
}

void cvtshorts( register unsigned short buffer[],register int n)
{
    register short i;
    register int nshorts = n>>1;
    register unsigned short swrd;

    for(i=0; i<nshorts; i++) {
		swrd = *buffer;
		*buffer++ = (swrd>>8) | (swrd<<8);
    }
}

void cvtints( register int buffer[],register int n)
{
    register short i;
    register int nints = n>>2;
    register unsigned int lwrd;

    for(i=0; i<nints; i++) {
	lwrd = buffer[i];
	buffer[i] =     ((lwrd>>24) 		| 
	   		(lwrd>>8 & 0xff00) 	| 
	   		(lwrd<<8 & 0xff0000) 	| 
	   		(lwrd<<24) 		);
    }
}

void cvtimage( register int buffer[])
{

	if(!buffer)return;

}

void i_errhdlr(char *fmt,int a1,int  a2,int  a3,int  a4);
void i_errhdlr(char *fmt,int a1,int  a2,int  a3,int  a4)	/* most args currently used is 2 */
{
	sprintf(WarningBuff, fmt, a1, a2, a3, a4);
	WarningBatch(WarningBuff);
}


/*
 *	iclose and iflush -
 *
 *				Paul Haeberli - 1984
 *
 */

int iclose(IMAGE *image)
{
    int tablesize /* acttablesize*/;
    int retval;

    iflush(image);
    img_optseek(image, 0);

#if defined(PC)
    image->dorev = 1;
#endif

    if (image->flags&I_IOWRT) {


    retval =  crayimgwrite(out,image,sizeof(IMAGE));
    if(retval == sizeof(IMAGE))
        image->offset += 152;
    else {
        image->offset = -1;
		sprintf(WarningBuff,"close: error on write of image header\n");
	    WarningBatch(WarningBuff);
		return EOF;
    }


	if(ISRLE(image->type)) {
	    img_optseek(image, 512L);
	    tablesize = image->ysize*image->zsize*intsize;
/*
	    acttablesize = image->ysize*image->zsize*sizeof(int);
*/
	    tablesize = image->ysize*image->zsize;
	    if (cimg_write(out,(unsigned int *)(image->rowstart),tablesize) 
               != tablesize) {
			sprintf(WarningBuff,"close: error on write of rowstart\n");
	    	WarningBatch(WarningBuff);
			return EOF;
	    }
	    
	    if (cimg_write(out,(unsigned int *)(image->rowsize),tablesize) 
               != tablesize) {
			sprintf(WarningBuff,"close: error on write of rowsize\n");
	    	WarningBatch(WarningBuff);
			return EOF;
	    }
	}
    }
    if(image->base) {
		cFree((char *)image->base);
		image->base = NULL;
    }
    if(image->tmpbuf) {
		cFree((char *)image->tmpbuf);
		image->tmpbuf = NULL;
    }
    if(ISRLE(image->type)) {
		cFree((char *)image->rowstart);
		image->rowstart = NULL;
		cFree((char *)image->rowsize);
		image->rowsize = NULL;
    }
    if(out)fclose(out);
    out=NULL;
    
    cFree((char *)image);
    return 0;
}

int iflush(IMAGE *image)
{
    unsigned short *base;

    if ( (image->flags&I_IOWRT)
     && (base=image->base)!=NULL && (image->ptr-base)>0) {
	    if (putrow(image, base, image->y,image->z)!=image->xsize) {
		    image->flags |= I_IOERR;
		    return(EOF);
	    }
    }
    return(0);
}


/*
 *	putrow, getrow -
 *
 *				Paul Haeberli - 1984
 *
 */

int putrow(IMAGE *image, unsigned short *buffer,
		unsigned int y, unsigned int z) 
{
    register unsigned short 	*sptr;
    register unsigned char      *cptr;
    register unsigned int x;
    register unsigned int min, max;
    register int cnt;

    if( !(image->flags & (I_IORW|I_IOWRT)) )
	return -1;
    if(image->dim<3)
	z = 0;
    if(image->dim<2)
	y = 0;
    if(ISVERBATIM(image->type)) {
	switch(BPP(image->type)) {
	    case 1: 
		min = image->min;
		max = image->max;
		cptr = (unsigned char *)image->tmpbuf;
		sptr = buffer;
		for(x=image->xsize; x--;) { 
		    *cptr = (unsigned char)(*sptr++);
		    if (*cptr > max) max = *cptr;
		    if (*cptr < min) min = *cptr;
		    cptr++;
		}
		image->min = min;
		image->max = max;
		img_seek(image,y,z);
		cnt = image->xsize;
		if (img_write(image,(char *)(image->tmpbuf),cnt) != cnt) 
		    return -1;
		else
		    return cnt;
		/* NOTREACHED */

	    case 2: 
		min = image->min;
		max = image->max;
		sptr = buffer;
		for(x=image->xsize; x--;) { 
		    if (*sptr > max) max = *sptr;
		    if (*sptr < min) min = *sptr;
		    sptr++;
		}
		image->min = min;
		image->max = max;
		img_seek(image,y,z);
		cnt = image->xsize<<1;
		if(image->dorev)	
		    cvtshorts(buffer,cnt);
		if (img_write(image,(char *)(buffer),cnt) != cnt) {
		    if(image->dorev)	
			cvtshorts(buffer,cnt);
		    return -1;
		} else {
		    if(image->dorev)	
			cvtshorts(buffer,cnt);
		    return image->xsize;
		}
		/* NOTREACHED */

	    default:
		sprintf(WarningBuff,"putrow: unknown bpp\n");
	    WarningBatch(WarningBuff);
	}
    } else if(ISRLE(image->type)) {
	switch(BPP(image->type)) {
	    case 1: 
		min = image->min;
		max = image->max;
		sptr = buffer;
		for(x=image->xsize; x--;) { 
		    if (*sptr > max) max = *sptr;
		    if (*sptr < min) min = *sptr;
                    sptr++;
		}
		image->min = min;
		image->max = max;
		cnt = img_rle_compact(buffer,2,image->tmpbuf,1,image->xsize);
		img_setrowsize(image,cnt,y,z);
		img_seek(image,y,z);

		if (img_write(image,(char *)(image->tmpbuf),cnt) != cnt) 
		    return -1;
		else
		    return image->xsize;
		/* NOTREACHED */

	    case 2: 
		min = image->min;
		max = image->max;
		sptr = buffer;
		for(x=image->xsize; x--;) { 
		    if (*sptr > max) max = *sptr;
		    if (*sptr < min) min = *sptr;
		    sptr++;
		}
		image->min = min;
		image->max = max;
		cnt = img_rle_compact(buffer,2,image->tmpbuf,2,image->xsize);
		cnt <<= 1;
		img_setrowsize(image,cnt,y,z);
		img_seek(image,y,z);
		if(image->dorev)
		    cvtshorts(image->tmpbuf,cnt);
		if (img_write(image,(char *)(image->tmpbuf),cnt) != cnt) {
		    if(image->dorev)
			cvtshorts(image->tmpbuf,cnt);
		    return -1;
		} else {
		    if(image->dorev)
			cvtshorts(image->tmpbuf,cnt);
		    return image->xsize;
		}
		/* NOTREACHED */

	    default:
		sprintf(WarningBuff,"putrow: Unknown bpp\n");
	    WarningBatch(WarningBuff);
	}
    } else{
		sprintf(WarningBuff,"putrow: weird image type\n");
		WarningBatch(WarningBuff);
	}
    return(-1);
}


/*
 *	img_seek, img_write, img_read, img_optseek -
 *
 *				Paul Haeberli - 1984
 *
 */

unsigned int img_seek(IMAGE *image, unsigned int y, unsigned int z)
{
    if(img_badrow(image,y,z)) {
		sprintf(WarningBuff,"img_seek: row number out of range\n");
		WarningBatch(WarningBuff);
		return EOF;
    }
    image->x = 0;
    image->y = y;
    image->z = z;
    if(ISVERBATIM(image->type)) {
	switch(image->dim) {
	    case 1:
		return img_optseek(image, 512L);
	    case 2: 
		return img_optseek(image,512L+(y*image->xsize)*BPP(image->type));
	    case 3: 
		return img_optseek(image,
		    512L+(y*image->xsize+z*image->xsize*image->ysize)*
							BPP(image->type));
	    default:
		sprintf(WarningBuff,"img_seek: weird dim\n");
		break;
	}
    } else if(ISRLE(image->type)) {
	switch(image->dim) {
	    case 1:
		return img_optseek(image, image->rowstart[0]);
	    case 2: 
		return img_optseek(image, image->rowstart[y]);
	    case 3: 
		return img_optseek(image, image->rowstart[y+z*image->ysize]);
	    default:
		sprintf(WarningBuff,"img_seek: weird dim\n");
		WarningBatch(WarningBuff);
		break;
	}
    } else {
		sprintf(WarningBuff,"img_seek: weird image type\n");
		WarningBatch(WarningBuff);
	}
    return((unsigned int)-1);
}

int img_badrow(IMAGE *image, unsigned int y, unsigned int z)
{
    if(y>=image->ysize || z>=image->zsize)
		return 1;
    else
        return 0;
}

int img_write(IMAGE *image, char *buffer,int count)
{
    int retval;

	retval=count;
    if(putString((unsigned char *)buffer,count,out))retval=0;
    if(retval == count) 
		image->offset += count;
    else
		image->offset = -1;
    return retval;
}

int img_read(IMAGE *image, char *buffer, int count)
{
    int retval;

    retval =  fread((char *)buffer,1,count,out);
    if(retval == count) 
	image->offset += count;
    else
	image->offset = -1;
    return retval;
}

unsigned int img_optseek(IMAGE *image, unsigned int offset)
{
    if(image->offset != offset) {
       image->offset = offset;
       fseek(out,offset,0);
       return (unsigned int)ftell(out);
   }
   return offset;
}

/*
 *	img_getrowsize, img_setrowsize, img_rle_compact, img_rle_expand -
 *
 *				Paul Haeberli - 1984
 *
 */

int img_getrowsize(IMAGE *image)
{
    switch(image->dim) {
	case 1:
	    return image->rowsize[0];
	case 2:
	    return image->rowsize[image->y];
	case 3:
	    return image->rowsize[image->y+image->z*image->ysize];
    }
    
    return 0;
}

void img_setrowsize(IMAGE *image, int cnt, int y, int z)
{
    int *sizeptr;

	if(!image)return;
	
    if(img_badrow(image,y,z)) 
	return;
	
	sizeptr=NULL;
	
    switch(image->dim) {
	case 1:
	    sizeptr = &image->rowsize[0];
	    image->rowstart[0] = image->rleend;
	    break;
	case 2:
	    sizeptr = &image->rowsize[y];
	    image->rowstart[y] = image->rleend;
	    break;
	case 3:
	    sizeptr = &image->rowsize[y+z*image->ysize];
	    image->rowstart[y+z*image->ysize] = image->rleend;
    }	
    
	if(!sizeptr)return;
	
    if(*sizeptr != -1) 
	image->wastebytes += *sizeptr;
    *sizeptr = cnt;
    image->rleend += cnt;
}

int img_rle_compact(unsigned short *expbuf, int ibpp,
			unsigned short *rlebuf, int obpp, int cnt)
{
    if(ibpp == 1 && obpp == 1) {
		register unsigned char *iptr = (unsigned char *)expbuf;
		register unsigned char *ibufend = iptr+cnt;
		register unsigned char *sptr;
		register unsigned char *optr = (unsigned char *)rlebuf;
		register short todo, cc;
		register int count;

		while(iptr<ibufend) {						
		    sptr = iptr;						
		    iptr += 2;							
		    while((iptr<ibufend)&&((iptr[-2]!=iptr[-1])||(iptr[-1]!=iptr[0])))
			iptr++;							
		    iptr -= 2;							
		    count = iptr-sptr;						
		    while(count) {						
			todo = count>126 ? 126:count; 				
			count -= todo;						
			*optr++ = (unsigned char)(0x80|todo);					
			while(todo--)						
			    *optr++ = *sptr++;					
		    }								
		    sptr = iptr;						
		    cc = *iptr++;						
		    while( (iptr<ibufend) && (*iptr == cc) )			
			iptr++;							
		    count = iptr-sptr;						
		    while(count) {						
			todo = count>126 ? 126:count; 				
			count -= todo;						
			*optr++ = (unsigned char)todo;						
			*optr++ = (unsigned char)cc;						
		    }								
		}								
		*optr++ = 0;
		return optr - (unsigned char *)rlebuf;
    } else if(ibpp == 1 && obpp == 2) {
		register unsigned char *iptr = (unsigned char *)expbuf;
		register unsigned char *ibufend = iptr+cnt;
		register unsigned char *sptr;
		register unsigned short *optr = rlebuf;
		register short todo, cc;
		register int count;

		while(iptr<ibufend) {						
		    sptr = iptr;						
		    iptr += 2;							
		    while((iptr<ibufend)&&((iptr[-2]!=iptr[-1])||(iptr[-1]!=iptr[0])))
			iptr++;							
		    iptr -= 2;							
		    count = iptr-sptr;						
		    while(count) {						
			todo = count>126 ? 126:count; 				
			count -= todo;						
			*optr++ = 0x80|todo;					
			while(todo--)						
			    *optr++ = *sptr++;					
		    }								
		    sptr = iptr;						
		    cc = *iptr++;						
		    while( (iptr<ibufend) && (*iptr == cc) )			
			iptr++;							
		    count = iptr-sptr;						
		    while(count) {						
			todo = count>126 ? 126:count; 				
			count -= todo;						
			*optr++ = todo;						
			*optr++ = cc;						
		    }								
		}								
		*optr++ = 0;
		return optr - rlebuf;
    } else if(ibpp == 2 && obpp == 1) {
		register unsigned short *iptr = expbuf;
		register unsigned short *ibufend = iptr+cnt;
		register unsigned short *sptr;
		register unsigned char *optr = (unsigned char *)rlebuf;
		register short todo, cc;
		register int count;

		while(iptr<ibufend) {						
		    sptr = iptr;						
		    iptr += 2;							
		    while((iptr<ibufend)&&((iptr[-2]!=iptr[-1])||(iptr[-1]!=iptr[0])))
			iptr++;							
		    iptr -= 2;							
		    count = iptr-sptr;						
		    while(count) {						
			todo = count>126 ? 126:count; 				
			count -= todo;						
			*optr++ = (0x80|todo);					
			while(todo--)						
			    *optr++ = (unsigned char)(*sptr++);					
		    }								
		    sptr = iptr;						
		    cc = *iptr++;						
		    while( (iptr<ibufend) && (*iptr == cc) )			
			iptr++;							
		    count = iptr-sptr;						
		    while(count) {						
			todo = count>126 ? 126:count; 				
			count -= todo;						
			*optr++ = (unsigned char)todo;						
			*optr++ = (unsigned char)cc;						
		    }								
		}								
		*optr++ = 0;
		return optr - (unsigned char *)rlebuf;
    } else if(ibpp == 2 && obpp == 2) {
		register unsigned short *iptr = expbuf;
		register unsigned short *ibufend = iptr+cnt;
		register unsigned short *sptr;
		register unsigned short *optr = rlebuf;
		register short todo, cc;
		register int count;

		while(iptr<ibufend) {						
		    sptr = iptr;						
		    iptr += 2;							
		    while((iptr<ibufend)&&((iptr[-2]!=iptr[-1])||(iptr[-1]!=iptr[0])))
			iptr++;							
		    iptr -= 2;							
		    count = iptr-sptr;						
		    while(count) {						
			todo = count>126 ? 126:count; 				
			count -= todo;						
			*optr++ = (0x80|todo);					
			while(todo--)						
			    *optr++ = (*sptr++);					
		    }								
		    sptr = iptr;						
		    cc = *iptr++;						
		    while( (iptr<ibufend) && (*iptr == cc) )			
			iptr++;							
		    count = iptr-sptr;						
		    while(count) {						
			todo = count>126 ? 126:count; 				
			count -= todo;						
			*optr++ = todo;						
			*optr++ = cc;						
		    }								
		}								
		*optr++ = 0;
		return optr - rlebuf;
    } else  {
		sprintf(WarningBuff,"rle_compact: bad bpp: %d %d\n",ibpp,obpp);
	    WarningBatch(WarningBuff);
	return 0;
    }
}

void img_rle_expand(unsigned short *rlebuf, int ibpp,
			unsigned short *expbuf, int obpp)
{
    if(ibpp == 1 && obpp == 1) {
		register unsigned char *iptr = (unsigned char *)rlebuf;
		register unsigned char *optr = (unsigned char *)expbuf;
		register unsigned short pixel,count;

		while(1) {				
		    pixel = *iptr++;			
		    if ( !(count = (pixel & 0x7f)) )	
			return;				
		    if(pixel & 0x80) {			
		       while(count--)			
			    *optr++ = *iptr++;		
		    } else {				
		       pixel = *iptr++;			
		       while(count--)			
			    *optr++ = (unsigned char)pixel;		
		    }					
		}
	
    } else if(ibpp == 1 && obpp == 2) {
		register unsigned char *iptr = (unsigned char *)rlebuf;
		register unsigned short *optr = expbuf;
		register unsigned short pixel,count;

		while(1) {				
		    pixel = *iptr++;			
		    if ( !(count = (pixel & 0x7f)) )	
			return;				
		    if(pixel & 0x80) {			
		       while(count--)			
			    *optr++ = *iptr++;		
		    } else {				
		       pixel = *iptr++;			
		       while(count--)			
			    *optr++ = pixel;		
		    }					
		}
    } else if(ibpp == 2 && obpp == 1) {
		register unsigned short *iptr = rlebuf;
		register unsigned char  *optr = (unsigned char *)expbuf;
		register unsigned short pixel,count;

		while(1) {				
		    pixel = *iptr++;			
		    if ( !(count = (pixel & 0x7f)) )	
			return;				
		    if(pixel & 0x80) {			
		       while(count--)			
			    *optr++ = (unsigned char)(*iptr++);		
		    } else {				
		       pixel = *iptr++;			
		       while(count--)			
			    *optr++ = (unsigned char)pixel;		
		    }					
		}
    } else if(ibpp == 2 && obpp == 2) {
		register unsigned short *iptr = rlebuf;
		register unsigned short *optr = expbuf;
		register unsigned short pixel,count;

		while(1) {				
		    pixel = *iptr++;			
		    if ( !(count = (pixel & 0x7f)) )	
			return;				
		    if(pixel & 0x80) {			
		       while(count--)			
			    *optr++ = *iptr++;		
		    } else {				
		       pixel = *iptr++;			
		       while(count--)			
			    *optr++ = pixel;		
		    }					
		}
    } else {
		sprintf(WarningBuff,"rle_expand: bad bpp: %d %d\n",ibpp,obpp);
	    WarningBatch(WarningBuff);
	}
}


/*
 *	isetname and isetcolormap -
 *
 *				Paul Haeberli - 1984
 *
 */

void isetname(IMAGE *image, char *name)
{
    strncpy(image->name,name,80);
}

void isetcolormap(IMAGE *image, int colormap)
{
    image->colormap = colormap;
}


int cimg_write(FILE *f, unsigned int  *buffer,int count)
{
	int n;
	
	for(n=0;n<count;++n){
      if(putLong(buffer[n],f))return 0;	    
	}
	
	return count;
	
}
int crayimgwrite(FILE *f, IMAGE *image, int count)
{
  int zero = 0;


/*
  Write out imagic, type, dim, xsize, ysize, zsize
*/
   if(putInt(image->imagic,f))return 0;
   if(putInt(image->type,f))return 0;
   if(putInt(image->dim,f))return 0;
   if(putInt(image->xsize,f))return 0;
   if(putInt(image->ysize,f))return 0;
   if(putInt(image->zsize,f))return 0;

/*
  Write out min, max, wastebytes
*/
      if(putLong(image->min,f))return 0;
      if(putLong(image->max,f))return 0;
      if(putLong(image->wastebytes,f))return 0;

/*
  Write out name[80]
*/
      if(putString((unsigned char *)image->name,80,f))return 0;


/*
  Write out colormap
*/
      if(putLong(image->colormap,f))return 0;

/*
  Write out file
*/
      if(putLong(image->file,f))return 0;

/*
  Write out flags, dorev, x, y, z, cnt
*/
   if(putInt(image->flags,f))return 0;
   if(putInt(image->dorev,f))return 0;
   if(putInt(image->x,f))return 0;
   if(putInt(image->y,f))return 0;
   if(putInt(image->z,f))return 0;
   if(putInt(image->cnt,f))return 0;

/*
  Write out *ptr, *base, *tmpbuf, offset, rleend, *rowstart, *rowsize
*/
    if(putLong(zero,f))return 0;
    if(putLong(zero,f))return 0;
    if(putLong(zero,f))return 0;
    if(putLong(image->offset,f))return 0;
    if(putLong(image->rleend,f))return 0;
    if(putLong(zero,f))return 0;
    if(putLong(zero,f))return 0;

	return count;
}

