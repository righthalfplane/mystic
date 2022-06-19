#ifndef _HDFFILES_

#define _HDFFILES_ 1

#ifdef __cplusplus
extern "C" {
#endif
  int DFSDrestart(void);
  int DFSDgetdims(char *name,int *rank,int *sizes,int maxrank);
  int DFPrestart(void);
  int DFPgetpal(char *name,char *pal);
  int DFR8restart(void);
  int DFR8setpalette(char *name);
  int DFR8getdims(char *name ,int *ixmax,int *iymax,int *hasPalette);
  int DFR8lastref(void);
  int DFR8putimage(char *name,char *buffout,int ixmax,int iymax,int type);
  int DFR8addimage(char *name,char *buffout,int ixmax,int iymax,int type);
#ifdef __cplusplus
}
#endif

#define int16 short
#define uint16 unsigned short
#define int32 long

#define DFTAG_ID    ((uint16)300) /* Image DimRec */
#define DFTAG_LUT   ((uint16)301) /* Image Palette */
#define DFTAG_RI    ((uint16)302) /* Raster Image */
#define DFTAG_CI    ((uint16)303) /* Compressed Image */
#define DFTAG_RLE   ((uint16)11) /* run length encoding */

#define DFTAG_RIG   ((uint16)306) /* Raster Image Group */
#define DFTAG_NULL  ((uint16)1)	/* empty DD */
#define DFTAG_CI8   ((uint16)203) /* RLE compressed 8-bit image */
#define DFTAG_ID8   ((uint16)200) /* 8-bit Image dimension */
#define DFTAG_NT    ((uint16)106) /* number type */
#define DFTAG_IP8   ((uint16)201) /* 8-bit Image palette */
#define DFTAG_MT    ((uint16)107) /* machine type */
#define	DFMT_MAC        0x1111
#define	DFMT_UNICOS     0x3331



typedef struct DFddh {		/*format of data descriptor headers in file*/
    int16 dds;			/* number of dds in header block */
    int32 next;			/* offset of next header block */
} DFddh;

typedef struct DFdd {		/* format of data descriptors as in file */
    uint16 tag;			/* data tag */
    uint16 ref;			/* data reference number */
    int32 offset;		/* offset of data element in file */
    int32 length;		/* number of bytes */
} DFdd;


#endif
