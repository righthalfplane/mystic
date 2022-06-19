#ifndef _UDRAW_
#define _UDRAW_

#include "Linedata.h"
#include "uAttributes.h"
#include "uFiles.h"
#include "uGraph.h"
#include "uEditText.h"

#define AnyType			0
#define GridPlotType	1
#define LineDataType	2
#define ImagePlotType	3
#define TruePlotType	4
#define GroupsType		5
#define AlphaType		6
#define LineType		7
#define BoxType			8
#define OvalType		9
#define RoundRectType	10
#define GridPlotLine	11
#define PIO2D_PlotType	12
#define TekPlotType		14

#define DISPLAY_PIXELS   0
#define DISPLAY_INCHES   1


typedef struct rPointStruct3{
	double x,y,z;
} rPoint3;

typedef struct rPointStruct2{
	double x,y;
} rPoint;

typedef struct rRectStruct{
    double x,y;
    double xsize,ysize;
} rRect;

extern struct DOList *ldummy777;

struct DObject {
/* Start of Object variables */
	struct uAttributes Attributes;
    char name[64];
	rRect box;
	int type;
	int AngleFlag;
	int FlipFlag;
	
	struct DOList *l;
	long size;	
	
    double xDup,yDup;
	int Selected;
	int oStatus;

    long ItWas;
    	
    struct DObject *((*ReadWrite)(struct DObject *o,FILE8 *inOut,struct DOList *l));
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct DOList *l);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);	
	struct DObject *(*FindObject)(struct DObject *Object,rPoint *r);	
	struct DObject *(*DoubleClick)(struct DObject *Object,rPoint *r);	
	int (*Update)(struct DObject *);
	struct uAttributes *(*GetAttributes)(struct DObject *Object);	
	void (*Attribute)(struct DObject *,struct uAttributes *vAttributes,int Flag);
    struct DObject *((*Duplicate)(struct DObject *,int iDup,double *xOff,double *yOff));
	void (*DeSelectOne)(struct DObject *,rPoint *r);
	void (*SelectOne)(struct DObject *,rPoint *r);
	int (*SelectedPoint)(struct DObject *,rPoint *ClickPoint);
	int (*SetSelectedRegion)(struct DObject *,struct DOList *l);
	int (*Message)(struct DObject *,long MessageType,void *MessageData);
	int (*DrawSelected)(struct DObject *,struct DOList *l);
 	int (*CheckStretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Stretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Move)(struct DObject *,double *xOff,double *yOff);
 	struct DObject * (*KillSelected)(struct DObject *);
	char *((*Copy)(struct DObject *,long *Length));
    struct DObject *((*DoInformation)(struct DObject *));
    struct DObject *((*DoRotateDirection)(struct DObject *,int direct));    
    struct DObject *((*DoFlipDirection)(struct DObject *,int direct));
 	void (*GetFontSelected)(struct DObject *);
 	void (*SetFontSelected)(struct DObject *);
    struct DObject *((*SetPalette)(struct DObject *,unsigned char *palette));
/* End of Object variables */
};

typedef struct DObject *DObjPtr;

struct uGroup{
/* Start of Object variables */
	struct uAttributes Attributes;
    char name[64];
	rRect box;
	int type;
	int AngleFlag;
	int FlipFlag;
	
	struct DOList *l;
	long size;	
	
    double xDup,yDup;
	int Selected;
	int oStatus;

    long ItWas;
    	
    struct DObject *((*ReadWrite)(struct DObject *o,FILE8 *inOut,struct DOList *l));
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct DOList *l);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);	
	struct DObject *(*FindObject)(struct DObject *Object,rPoint *r);	
	struct DObject *(*DoubleClick)(struct DObject *Object,rPoint *r);	
	int (*Update)(struct DObject *);
	struct uAttributes *(*GetAttributes)(struct DObject *Object);	
	void (*Attribute)(struct DObject *,struct uAttributes *vAttributes,int Flag);
    struct DObject *((*Duplicate)(struct DObject *,int iDup,double *xOff,double *yOff));
	void (*DeSelectOne)(struct DObject *,rPoint *r);
	void (*SelectOne)(struct DObject *,rPoint *r);
	int (*SelectedPoint)(struct DObject *,rPoint *ClickPoint);
	int (*SetSelectedRegion)(struct DObject *,struct DOList *l);
	int (*Message)(struct DObject *,long MessageType,void *MessageData);
	int (*DrawSelected)(struct DObject *,struct DOList *l);
 	int (*CheckStretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Stretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Move)(struct DObject *,double *xOff,double *yOff);
 	struct DObject * (*KillSelected)(struct DObject *);
	char *((*Copy)(struct DObject *,long *Length));
    struct DObject *((*DoInformation)(struct DObject *));
    struct DObject *((*DoRotateDirection)(struct DObject *,int direct));    
    struct DObject *((*DoFlipDirection)(struct DObject *,int direct));
 	void (*GetFontSelected)(struct DObject *);
 	void (*SetFontSelected)(struct DObject *);
    struct DObject *((*SetPalette)(struct DObject *,unsigned char *palette));
/* End of Object variables */
	DObjPtr *Groups;
	long nGroups;
	float *itemList;
};

typedef struct uGroup *uGroup2Ptr;

uGroup2Ptr uGroupGroup(long Count,struct DOList *l);

struct uImage{
/* Start of Object variables */
	struct uAttributes Attributes;
    char name[64];
	rRect box;
	int type;
	int AngleFlag;
	int FlipFlag;
	
	struct DOList *l;
	long size;	
	
    double xDup,yDup;
	int Selected;
	int oStatus;

    long ItWas;
    	
    struct DObject *((*ReadWrite)(struct DObject *o,FILE8 *inOut,struct DOList *l));
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct DOList *l);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);	
	struct DObject *(*FindObject)(struct DObject *Object,rPoint *r);	
	struct DObject *(*DoubleClick)(struct DObject *Object,rPoint *r);	
	int (*Update)(struct DObject *);
	struct uAttributes *(*GetAttributes)(struct DObject *Object);	
	void (*Attribute)(struct DObject *,struct uAttributes *vAttributes,int Flag);
    struct DObject *((*Duplicate)(struct DObject *,int iDup,double *xOff,double *yOff));
	void (*DeSelectOne)(struct DObject *,rPoint *r);
	void (*SelectOne)(struct DObject *,rPoint *r);
	int (*SelectedPoint)(struct DObject *,rPoint *ClickPoint);
	int (*SetSelectedRegion)(struct DObject *,struct DOList *l);
	int (*Message)(struct DObject *,long MessageType,void *MessageData);
	int (*DrawSelected)(struct DObject *,struct DOList *l);
 	int (*CheckStretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Stretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Move)(struct DObject *,double *xOff,double *yOff);
 	struct DObject * (*KillSelected)(struct DObject *);
	char *((*Copy)(struct DObject *,long *Length));
    struct DObject *((*DoInformation)(struct DObject *));
    struct DObject *((*DoRotateDirection)(struct DObject *,int direct));    
    struct DObject *((*DoFlipDirection)(struct DObject *,int direct));
 	void (*GetFontSelected)(struct DObject *);
 	void (*SetFontSelected)(struct DObject *);
    struct DObject *((*SetPalette)(struct DObject *,unsigned char *palette));
/* End of Object variables */
	int (*ReplaceImage)(struct uImage *,long xsize,long ysize,unsigned char *image);
	struct FileInfo2 *Files;	
	long CurrentFrame;
   	long xsize,ysize;
   	long length;
   	unsigned char *dataPtr;   	
   	struct uAttributes pAttributes;
};

typedef struct uImage *uImagePtr;

uImagePtr uImageImage(long xs,long ys,rRect *r,struct uAttributes *Attribute,struct DOList *l);


struct uTek{
/* Start of Object variables */
	struct uAttributes Attributes;
    char name[64];
	rRect box;
	int type;
	int AngleFlag;
	int FlipFlag;
	
	struct DOList *l;
	long size;	
	
    double xDup,yDup;
	int Selected;
	int oStatus;

    long ItWas;
    	
    struct DObject *((*ReadWrite)(struct DObject *o,FILE8 *inOut,struct DOList *l));
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct DOList *l);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);	
	struct DObject *(*FindObject)(struct DObject *Object,rPoint *r);	
	struct DObject *(*DoubleClick)(struct DObject *Object,rPoint *r);	
	int (*Update)(struct DObject *);
	struct uAttributes *(*GetAttributes)(struct DObject *Object);	
	void (*Attribute)(struct DObject *,struct uAttributes *vAttributes,int Flag);
    struct DObject *((*Duplicate)(struct DObject *,int iDup,double *xOff,double *yOff));
	void (*DeSelectOne)(struct DObject *,rPoint *r);
	void (*SelectOne)(struct DObject *,rPoint *r);
	int (*SelectedPoint)(struct DObject *,rPoint *ClickPoint);
	int (*SetSelectedRegion)(struct DObject *,struct DOList *l);
	int (*Message)(struct DObject *,long MessageType,void *MessageData);
	int (*DrawSelected)(struct DObject *,struct DOList *l);
 	int (*CheckStretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Stretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Move)(struct DObject *,double *xOff,double *yOff);
 	struct DObject * (*KillSelected)(struct DObject *);
	char *((*Copy)(struct DObject *,long *Length));
    struct DObject *((*DoInformation)(struct DObject *));
    struct DObject *((*DoRotateDirection)(struct DObject *,int direct));    
    struct DObject *((*DoFlipDirection)(struct DObject *,int direct));
 	void (*GetFontSelected)(struct DObject *);
 	void (*SetFontSelected)(struct DObject *);
    struct DObject *((*SetPalette)(struct DObject *,unsigned char *palette));
/* End of Object variables */
	struct FileInfo2 *Files;	
    char *CurrentFileName;
	long CurrentFrame;
   	long xsize,ysize;
   	long length;
   	unsigned char *dataPtr;
   	double scale;
};

typedef struct uTek *uTekPtr;


uTekPtr uTekImage(long xs,long ys,rRect *r,struct uAttributes *Attribute,struct DOList *l);

int FilesGetTEK(char **ImageName,long *teklength,char **buffer,
                long CurrentFrame,struct FileInfo2 *Files);


struct uLineData{
/* Start of Object variables */
	struct uAttributes Attributes;
    char name[64];
	rRect box;
	int type;
	int AngleFlag;
	int FlipFlag;
	
	struct DOList *l;
	long size;	
	
    double xDup,yDup;
	int Selected;
	int oStatus;

    long ItWas;
    	
    struct DObject *((*ReadWrite)(struct DObject *o,FILE8 *inOut,struct DOList *l));
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct DOList *l);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);	
	struct DObject *(*FindObject)(struct DObject *Object,rPoint *r);	
	struct DObject *(*DoubleClick)(struct DObject *Object,rPoint *r);	
	int (*Update)(struct DObject *);
	struct uAttributes *(*GetAttributes)(struct DObject *Object);	
	void (*Attribute)(struct DObject *,struct uAttributes *vAttributes,int Flag);
    struct DObject *((*Duplicate)(struct DObject *,int iDup,double *xOff,double *yOff));
	void (*DeSelectOne)(struct DObject *,rPoint *r);
	void (*SelectOne)(struct DObject *,rPoint *r);
	int (*SelectedPoint)(struct DObject *,rPoint *ClickPoint);
	int (*SetSelectedRegion)(struct DObject *,struct DOList *l);
	int (*Message)(struct DObject *,long MessageType,void *MessageData);
	int (*DrawSelected)(struct DObject *,struct DOList *l);
 	int (*CheckStretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Stretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Move)(struct DObject *,double *xOff,double *yOff);
 	struct DObject * (*KillSelected)(struct DObject *);
	char *((*Copy)(struct DObject *,long *Length));
    struct DObject *((*DoInformation)(struct DObject *));
    struct DObject *((*DoRotateDirection)(struct DObject *,int direct));    
    struct DObject *((*DoFlipDirection)(struct DObject *,int direct));
 	void (*GetFontSelected)(struct DObject *);
 	void (*SetFontSelected)(struct DObject *);
    struct DObject *((*SetPalette)(struct DObject *,unsigned char *palette));
/* End of Object variables */
    char save[64];
	struct linedata li;
    double *xData;
    double *yData;
    long dCount;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    double xShift;
    double yShift;
    double xScale;
    double yScale;
};

typedef struct uLineData *uLineDataPtr;


uLineDataPtr doLineRead(struct uAttributes *Attributes,char *name);

struct uGridPlot{
/* Start of Object variables */
	struct uAttributes Attributes;
    char name[64];
	rRect box;
	int type;
	int AngleFlag;
	int FlipFlag;
	
	struct DOList *l;
	long size;	
	
    double xDup,yDup;
	int Selected;
	int oStatus;

    long ItWas;
    	
    struct DObject *((*ReadWrite)(struct DObject *o,FILE8 *inOut,struct DOList *l));
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct DOList *l);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);	
	struct DObject *(*FindObject)(struct DObject *Object,rPoint *r);	
	struct DObject *(*DoubleClick)(struct DObject *Object,rPoint *r);	
	int (*Update)(struct DObject *);
	struct uAttributes *(*GetAttributes)(struct DObject *Object);	
	void (*Attribute)(struct DObject *,struct uAttributes *vAttributes,int Flag);
    struct DObject *((*Duplicate)(struct DObject *,int iDup,double *xOff,double *yOff));
	void (*DeSelectOne)(struct DObject *,rPoint *r);
	void (*SelectOne)(struct DObject *,rPoint *r);
	int (*SelectedPoint)(struct DObject *,rPoint *ClickPoint);
	int (*SetSelectedRegion)(struct DObject *,struct DOList *l);
	int (*Message)(struct DObject *,long MessageType,void *MessageData);
	int (*DrawSelected)(struct DObject *,struct DOList *l);
 	int (*CheckStretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Stretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Move)(struct DObject *,double *xOff,double *yOff);
 	struct DObject * (*KillSelected)(struct DObject *);
	char *((*Copy)(struct DObject *,long *Length));
    struct DObject *((*DoInformation)(struct DObject *));
    struct DObject *((*DoRotateDirection)(struct DObject *,int direct));    
    struct DObject *((*DoFlipDirection)(struct DObject *,int direct));
 	void (*GetFontSelected)(struct DObject *);
 	void (*SetFontSelected)(struct DObject *);
    struct DObject *((*SetPalette)(struct DObject *,unsigned char *palette));
/* End of Object variables */


    double xMinimum;
    double yMinimum;
    double xMaximum;
    double yMaximum;
    
    double xMajorIn,xMajorOut;
    double yMajorIn,yMajorOut;
    double rMajorIn,rMajorOut;
    double tMajorIn,tMajorOut;
    double xMinorIn,xMinorOut;
    double yMinorIn,yMinorOut;
    double rMinorIn,rMinorOut;
    double tMinorIn,tMinorOut;
    int lAxis;			     /* Left Axis Flag		*/
    int bAxis;			     /* Bottom Axis Flag	*/
    int rAxis;			     /* Right Axis Flag		*/
    int tAxis;			     /* Top Axis Flag		*/
    int lAxisNumber;		     /* Left Axis Flag		*/
    int bAxisNumber;		     /* Bottom Axis Flag	*/
    int rAxisNumber;		     /* Right Axis Flag		*/
    int tAxisNumber;		     /* Top Axis Flag		*/
    int xGrid;			     /* X Axis Cross Hash	*/
    int yGrid;			     /* Y Axis Cross Hash	*/
    int xLogScale;		     /* X Axis Log Flag		*/
    int yLogScale;		     /* Y Axis Log Flag		*/
    double xViewMin,xViewMax;        /* Window X Limits		*/
    double yViewMin,yViewMax;        /* Window Y Limits		*/
    double xViewMinLog,yViewMinLog;  /* Log10 of Limits		*/
    double xScale,yScale;            /* Window Scale Factors	*/
    double xScaleLog,yScaleLog;      /* Window Log of Scales	*/
    double xOff,yOff;                /* Window Off Sets		*/
    double xMajorStep,xMinorStep;    /* X Grid Step Sizes	*/
    double yMajorStep,yMinorStep;    /* Y Grid Step Sizes	*/
    double dxLim,dyLim;
    double Small;
    
    
	struct uAttributes xAttributes;
	struct uAttributes yAttributes;
	
    struct uLineData **Lines;
    long LineCount;
    long LineMax;
    
    rRect xBoxeSelect;
    rRect yBoxeSelect;
    
    rPoint xBoxOffset;    
    rPoint yBoxOffset;
    rPoint yLegendOffset;
    
    rRect *xBoxes;
    rRect *yBoxes;
    int xCount;
    int yCount;
    
	int xSelected;
	int ySelected;
    
	int pSelected;
	int lSelected;
	
	int gridHash;
    
	int boxLegend;
    
    int xAutoMinimum;
    int yAutoMinimum;
    int xAutoMaximum;
    int yAutoMaximum;
    int xMajorLimit;
    int xMinorLimit;
    int yMajorLimit;
    int yMinorLimit;
    
    double xSetMinimum;
    double ySetMinimum;
    double xSetMaximum;
    double ySetMaximum;
    
};

typedef struct uGridPlot *uGridPlotPtr;


struct uTrue {
/* Start of Object variables */
	struct uAttributes Attributes;
    char name[64];
	rRect box;
	int type;
	int AngleFlag;
	int FlipFlag;
	
	struct DOList *l;
	long size;	
	
    double xDup,yDup;
	int Selected;
	int oStatus;

    long ItWas;
    	
    struct DObject *((*ReadWrite)(struct DObject *o,FILE8 *inOut,struct DOList *l));
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct DOList *l);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);	
	struct DObject *(*FindObject)(struct DObject *Object,rPoint *r);	
	struct DObject *(*DoubleClick)(struct DObject *Object,rPoint *r);	
	int (*Update)(struct DObject *);
	struct uAttributes *(*GetAttributes)(struct DObject *Object);	
	void (*Attribute)(struct DObject *,struct uAttributes *vAttributes,int Flag);
    struct DObject *((*Duplicate)(struct DObject *,int iDup,double *xOff,double *yOff));
	void (*DeSelectOne)(struct DObject *,rPoint *r);
	void (*SelectOne)(struct DObject *,rPoint *r);
	int (*SelectedPoint)(struct DObject *,rPoint *ClickPoint);
	int (*SetSelectedRegion)(struct DObject *,struct DOList *l);
	int (*Message)(struct DObject *,long MessageType,void *MessageData);
	int (*DrawSelected)(struct DObject *,struct DOList *l);
 	int (*CheckStretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Stretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Move)(struct DObject *,double *xOff,double *yOff);
 	struct DObject * (*KillSelected)(struct DObject *);
	char *((*Copy)(struct DObject *,long *Length));
    struct DObject *((*DoInformation)(struct DObject *));
    struct DObject *((*DoRotateDirection)(struct DObject *,int direct));    
    struct DObject *((*DoFlipDirection)(struct DObject *,int direct));
 	void (*GetFontSelected)(struct DObject *);
 	void (*SetFontSelected)(struct DObject *);
    struct DObject *((*SetPalette)(struct DObject *,unsigned char *palette));
/* End of Object variables */
	int (*ReplaceImage)(struct uTrue *,long xsize,long ysize,unsigned char *image);
	struct FileInfo2 *Files;	
	long CurrentFrame;
   	long xsize,ysize;
   	unsigned char *dataPtr;
   	long length;
	int TrueType;
	
    char BatchName[255];
    struct PlotStreamlineData streamline;
    struct PlotVectorData vector;
    struct PlotRangeData range;
    struct PlotPlaneData plane;
	struct PlotAttributes pa;
    struct SetFrameData sd;  
    struct paletteDraw pd;
    char pioName[64];
    int pioIndex;
	
	struct uAttributes pAttributes;
    rRect pBoxeSelect;
    rPoint pBoxOffset;    
    rRect pBoxes;
    int pCount;
	int pSelected;
   	
   	char formatPalette[16];
   	char formatXaxis[16];
   	char formatYaxis[16];
   	
   	int countPalette;
   	int countXaxis;
   	int countYaxis;
   	
   	int showXaxis;
   	int showYaxis;
   	int showPalette;
   	
    unsigned char *pBoxPtr;
    rRect pBoxSource;
	
	int iSelected;
	
};

typedef struct uTrue *uTruePtr;

uTruePtr uTrueTrue(long xs,long ys,rRect *r,struct uAttributes *Attribute,struct DOList *l);


struct uAlpha {
/* Start of Object variables */
	struct uAttributes Attributes;
    char name[64];
	rRect box;
	int type;
	int AngleFlag;
	int FlipFlag;
	
	struct DOList *l;
	long size;	
	
    double xDup,yDup;
	int Selected;
	int oStatus;

    long ItWas;
    	
    struct DObject *((*ReadWrite)(struct DObject *o,FILE8 *inOut,struct DOList *l));
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct DOList *l);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);	
	struct DObject *(*FindObject)(struct DObject *Object,rPoint *r);	
	struct DObject *(*DoubleClick)(struct DObject *Object,rPoint *r);	
	int (*Update)(struct DObject *);
	struct uAttributes *(*GetAttributes)(struct DObject *Object);	
	void (*Attribute)(struct DObject *,struct uAttributes *vAttributes,int Flag);
    struct DObject *((*Duplicate)(struct DObject *,int iDup,double *xOff,double *yOff));
	void (*DeSelectOne)(struct DObject *,rPoint *r);
	void (*SelectOne)(struct DObject *,rPoint *r);
	int (*SelectedPoint)(struct DObject *,rPoint *ClickPoint);
	int (*SetSelectedRegion)(struct DObject *,struct DOList *l);
	int (*Message)(struct DObject *,long MessageType,void *MessageData);
	int (*DrawSelected)(struct DObject *,struct DOList *l);
 	int (*CheckStretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Stretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Move)(struct DObject *,double *xOff,double *yOff);
 	struct DObject * (*KillSelected)(struct DObject *);
	char *((*Copy)(struct DObject *,long *Length));
    struct DObject *((*DoInformation)(struct DObject *));
    struct DObject *((*DoRotateDirection)(struct DObject *,int direct));    
    struct DObject *((*DoFlipDirection)(struct DObject *,int direct));
 	void (*GetFontSelected)(struct DObject *);
 	void (*SetFontSelected)(struct DObject *);
    struct DObject *((*SetPalette)(struct DObject *,unsigned char *palette));
/* End of Object variables */
	struct FileInfo2 *Files;	
	char *fileText;
	long fileLength;
	int AlphaNumber;
	double Angle;
	double scale;
	char save[512];
	int n;
};

typedef struct uAlpha *uAlphaPtr;

uAlphaPtr uAlphaAlpha2(uAlphaPtr AlphaOld,struct DOList *l);
uAlphaPtr uAlphaAlpha(struct uAttributes *Attributes,struct DOList *l);
DObjPtr uAlphaMove(DObjPtr o,double *dx,double *dy);


struct uLine {
/* Start of Object variables */
	struct uAttributes Attributes;
    char name[64];
	rRect box;
	int type;
	int AngleFlag;
	int FlipFlag;
	
	struct DOList *l;
	long size;	
	
    double xDup,yDup;
	int Selected;
	int oStatus;

    long ItWas;
    	
    struct DObject *((*ReadWrite)(struct DObject *o,FILE8 *inOut,struct DOList *l));
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct DOList *l);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);	
	struct DObject *(*FindObject)(struct DObject *Object,rPoint *r);	
	struct DObject *(*DoubleClick)(struct DObject *Object,rPoint *r);	
	int (*Update)(struct DObject *);
	struct uAttributes *(*GetAttributes)(struct DObject *Object);	
	void (*Attribute)(struct DObject *,struct uAttributes *vAttributes,int Flag);
    struct DObject *((*Duplicate)(struct DObject *,int iDup,double *xOff,double *yOff));
	void (*DeSelectOne)(struct DObject *,rPoint *r);
	void (*SelectOne)(struct DObject *,rPoint *r);
	int (*SelectedPoint)(struct DObject *,rPoint *ClickPoint);
	int (*SetSelectedRegion)(struct DObject *,struct DOList *l);
	int (*Message)(struct DObject *,long MessageType,void *MessageData);
	int (*DrawSelected)(struct DObject *,struct DOList *l);
 	int (*CheckStretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Stretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Move)(struct DObject *,double *xOff,double *yOff);
 	struct DObject * (*KillSelected)(struct DObject *);
	char *((*Copy)(struct DObject *,long *Length));
    struct DObject *((*DoInformation)(struct DObject *));
    struct DObject *((*DoRotateDirection)(struct DObject *,int direct));    
    struct DObject *((*DoFlipDirection)(struct DObject *,int direct));
 	void (*GetFontSelected)(struct DObject *);
 	void (*SetFontSelected)(struct DObject *);
    struct DObject *((*SetPalette)(struct DObject *,unsigned char *palette));
/* End of Object variables */
    rPoint p1;
    rPoint p2;
};

typedef struct uLine *uLinePtr;
uLinePtr uLineLine(rRect *box,struct uAttributes *Attributes,struct DOList *l);


struct uBox {
/* Start of Object variables */
	struct uAttributes Attributes;
    char name[64];
	rRect box;
	int type;
	int AngleFlag;
	int FlipFlag;
	
	struct DOList *l;
	long size;	
	
    double xDup,yDup;
	int Selected;
	int oStatus;

    long ItWas;
    	
    struct DObject *((*ReadWrite)(struct DObject *o,FILE8 *inOut,struct DOList *l));
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct DOList *l);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);	
	struct DObject *(*FindObject)(struct DObject *Object,rPoint *r);	
	struct DObject *(*DoubleClick)(struct DObject *Object,rPoint *r);	
	int (*Update)(struct DObject *);
	struct uAttributes *(*GetAttributes)(struct DObject *Object);	
	void (*Attribute)(struct DObject *,struct uAttributes *vAttributes,int Flag);
    struct DObject *((*Duplicate)(struct DObject *,int iDup,double *xOff,double *yOff));
	void (*DeSelectOne)(struct DObject *,rPoint *r);
	void (*SelectOne)(struct DObject *,rPoint *r);
	int (*SelectedPoint)(struct DObject *,rPoint *ClickPoint);
	int (*SetSelectedRegion)(struct DObject *,struct DOList *l);
	int (*Message)(struct DObject *,long MessageType,void *MessageData);
	int (*DrawSelected)(struct DObject *,struct DOList *l);
 	int (*CheckStretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Stretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Move)(struct DObject *,double *xOff,double *yOff);
 	struct DObject * (*KillSelected)(struct DObject *);
	char *((*Copy)(struct DObject *,long *Length));
    struct DObject *((*DoInformation)(struct DObject *));
    struct DObject *((*DoRotateDirection)(struct DObject *,int direct));    
    struct DObject *((*DoFlipDirection)(struct DObject *,int direct));
 	void (*GetFontSelected)(struct DObject *);
 	void (*SetFontSelected)(struct DObject *);
    struct DObject *((*SetPalette)(struct DObject *,unsigned char *palette));
/* End of Object variables */
};

typedef struct uBox *uBoxPtr;
uBoxPtr uBoxBox(rRect *box,struct uAttributes *Attributes,struct DOList *l);

struct uOval {
/* Start of Object variables */
	struct uAttributes Attributes;
    char name[64];
	rRect box;
	int type;
	int AngleFlag;
	int FlipFlag;
	
	struct DOList *l;
	long size;	
	
    double xDup,yDup;
	int Selected;
	int oStatus;

    long ItWas;
    	
    struct DObject *((*ReadWrite)(struct DObject *o,FILE8 *inOut,struct DOList *l));
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct DOList *l);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);	
	struct DObject *(*FindObject)(struct DObject *Object,rPoint *r);	
	struct DObject *(*DoubleClick)(struct DObject *Object,rPoint *r);	
	int (*Update)(struct DObject *);
	struct uAttributes *(*GetAttributes)(struct DObject *Object);	
	void (*Attribute)(struct DObject *,struct uAttributes *vAttributes,int Flag);
    struct DObject *((*Duplicate)(struct DObject *,int iDup,double *xOff,double *yOff));
	void (*DeSelectOne)(struct DObject *,rPoint *r);
	void (*SelectOne)(struct DObject *,rPoint *r);
	int (*SelectedPoint)(struct DObject *,rPoint *ClickPoint);
	int (*SetSelectedRegion)(struct DObject *,struct DOList *l);
	int (*Message)(struct DObject *,long MessageType,void *MessageData);
	int (*DrawSelected)(struct DObject *,struct DOList *l);
 	int (*CheckStretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Stretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Move)(struct DObject *,double *xOff,double *yOff);
 	struct DObject * (*KillSelected)(struct DObject *);
	char *((*Copy)(struct DObject *,long *Length));
    struct DObject *((*DoInformation)(struct DObject *));
    struct DObject *((*DoRotateDirection)(struct DObject *,int direct));    
    struct DObject *((*DoFlipDirection)(struct DObject *,int direct));
 	void (*GetFontSelected)(struct DObject *);
 	void (*SetFontSelected)(struct DObject *);
    struct DObject *((*SetPalette)(struct DObject *,unsigned char *palette));
/* End of Object variables */
};

typedef struct uOval *uOvalPtr;
uOvalPtr uOvalOval(rRect *box,struct uAttributes *Attributes,struct DOList *l);


struct uRoundRect {
/* Start of Object variables */
	struct uAttributes Attributes;
    char name[64];
	rRect box;
	int type;
	int AngleFlag;
	int FlipFlag;
	
	struct DOList *l;
	long size;	
	
    double xDup,yDup;
	int Selected;
	int oStatus;

    long ItWas;
    	
    struct DObject *((*ReadWrite)(struct DObject *o,FILE8 *inOut,struct DOList *l));
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct DOList *l);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);	
	struct DObject *(*FindObject)(struct DObject *Object,rPoint *r);	
	struct DObject *(*DoubleClick)(struct DObject *Object,rPoint *r);	
	int (*Update)(struct DObject *);
	struct uAttributes *(*GetAttributes)(struct DObject *Object);	
	void (*Attribute)(struct DObject *,struct uAttributes *vAttributes,int Flag);
    struct DObject *((*Duplicate)(struct DObject *,int iDup,double *xOff,double *yOff));
	void (*DeSelectOne)(struct DObject *,rPoint *r);
	void (*SelectOne)(struct DObject *,rPoint *r);
	int (*SelectedPoint)(struct DObject *,rPoint *ClickPoint);
	int (*SetSelectedRegion)(struct DObject *,struct DOList *l);
	int (*Message)(struct DObject *,long MessageType,void *MessageData);
	int (*DrawSelected)(struct DObject *,struct DOList *l);
 	int (*CheckStretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Stretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Move)(struct DObject *,double *xOff,double *yOff);
 	struct DObject * (*KillSelected)(struct DObject *);
	char *((*Copy)(struct DObject *,long *Length));
    struct DObject *((*DoInformation)(struct DObject *));
    struct DObject *((*DoRotateDirection)(struct DObject *,int direct));    
    struct DObject *((*DoFlipDirection)(struct DObject *,int direct));
 	void (*GetFontSelected)(struct DObject *);
 	void (*SetFontSelected)(struct DObject *);
    struct DObject *((*SetPalette)(struct DObject *,unsigned char *palette));
/* End of Object variables */
    double ovalWidth,ovalHeight;
};

typedef struct uRoundRect *uRoundRectPtr;
uRoundRectPtr uRoundRectRoundRect(rRect *box,struct uAttributes *Attributes,struct DOList *l);



struct uPIO2D {
/* Start of Object variables */
	struct uAttributes Attributes;
    char name[64];
	rRect box;
	int type;
	int AngleFlag;
	int FlipFlag;
	
	struct DOList *l;
	long size;	
	
    double xDup,yDup;
	int Selected;
	int oStatus;

    long ItWas;
    	
    struct DObject *((*ReadWrite)(struct DObject *o,FILE8 *inOut,struct DOList *l));
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct DOList *l);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);	
	struct DObject *(*FindObject)(struct DObject *Object,rPoint *r);	
	struct DObject *(*DoubleClick)(struct DObject *Object,rPoint *r);	
	int (*Update)(struct DObject *);
	struct uAttributes *(*GetAttributes)(struct DObject *Object);	
	void (*Attribute)(struct DObject *,struct uAttributes *vAttributes,int Flag);
    struct DObject *((*Duplicate)(struct DObject *,int iDup,double *xOff,double *yOff));
	void (*DeSelectOne)(struct DObject *,rPoint *r);
	void (*SelectOne)(struct DObject *,rPoint *r);
	int (*SelectedPoint)(struct DObject *,rPoint *ClickPoint);
	int (*SetSelectedRegion)(struct DObject *,struct DOList *l);
	int (*Message)(struct DObject *,long MessageType,void *MessageData);
	int (*DrawSelected)(struct DObject *,struct DOList *l);
 	int (*CheckStretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Stretch)(uPoint pt,struct DObject *,struct DOList *l);
 	struct DObject * (*Move)(struct DObject *,double *xOff,double *yOff);
 	struct DObject * (*KillSelected)(struct DObject *);
	char *((*Copy)(struct DObject *,long *Length));
    struct DObject *((*DoInformation)(struct DObject *));
    struct DObject *((*DoRotateDirection)(struct DObject *,int direct));    
    struct DObject *((*DoFlipDirection)(struct DObject *,int direct));
 	void (*GetFontSelected)(struct DObject *);
 	void (*SetFontSelected)(struct DObject *);
    struct DObject *((*SetPalette)(struct DObject *,unsigned char *palette));
/* End of Object variables */
	int (*ReplaceImage)(struct uPIO2D *,long xsize,long ysize,unsigned char *image);
	struct FileInfo2 *Files;	
    char BatchName[255];
    struct PlotStreamlineData streamline;
    struct PlotVectorData vector;
    struct PlotRangeData range;
    struct SetFrameData sd;    
    struct paletteDraw pd;
    char pioName[64];
    int pioIndex;

	long CurrentFrame;
   	long xsize,ysize;
   	unsigned char *dataPtr;
   	long length;
	int TrueType;
};

typedef struct uPIO2D *uPIO2DPtr;

uPIO2DPtr uPIO2DPIO2D(struct FileInfo2 *Files,struct DOList *l);



struct DOList{
	struct uAttributes Attributes;
	DObjPtr *oOld;
	long oMaximun;
	DObjPtr *oCurrent;
	long oCount;
    struct screenData s;
    long ObjectIDCount;
	int displayType;
	unsigned char *data;
	long xsize;
	long ysize;
	long dpi;
	int raster;
	long length;
	char palette[256*3];
	char palname[256];
	unsigned char red,green,blue;
	IconPtr myIcon;
	
	uAlphaPtr Alpha;
	uEditPtr Edit;
	int AlphaNumber;
	
	rRect *Boxes;
    long rCountCount;
    long rCountMax;
    
	uRect *Stretch;
    long stretchCount;
    long stretchMax;
    
	float *itemList;
    
    double xDuplicate,yDuplicate;
    int DuplicateFlag;
    
    long CurrentFrame;
    
    char LinePatterns[8][16];
    int NumberOfPatterns;
    
	iRect Picts[40];
    
	int nLineFore;
	int nLineBack;
	
	uRect PrintRect;
	int PrintFlag;

	struct uAttributes *lAttributes;
	long lineAttributesCount;
	long lineAttributesMax;

};

typedef struct DOList *DOListPtr;

uGridPlotPtr DOGridPlotCreate(DOListPtr l);
uLineDataPtr DOLineDataCreate(struct linedata *li,struct uAttributes *Attributes);
DObjPtr DOListFindName(DOListPtr l,char *name,int type);
int DOListADD(DOListPtr l,struct DObject *Object);
int DOSetDrawCanvas(DOListPtr l);
int DOListDelete(DOListPtr l);
long DOFrameCount(DOListPtr l);
int DODraw(DOListPtr l,long CurrentFrame);
int DOErase(DOListPtr l);
int DOSaveStretch(uRect *r,DOListPtr l);
int DOStretchRgn(uPoint pt,DOListPtr l);
int DOListDoStretch(uPoint pt,DOListPtr l);
uRect DOSelectRect(int n,rRect *box,DOListPtr l);
rRect DOrRectFromScreen(uRect *box,DOListPtr l);
void DOListDuplicate(DOListPtr l);
int DOSetPalette(DOListPtr l);

void DOListClear(DOListPtr l);

void DOListChangeAlpha(DOListPtr l);

int DOListUpdate(DOListPtr l,int flag);

struct stackT{
   long y;
   long x;
   double scale;
};

struct DrawWindow{
    struct DOList DList;
    struct stackT *stackList;
    long stackCount;
    long stackMax;
    char *CurrentFileName;
    long ImageCount;
    long CurrentFrame;
    unsigned char *buffer;
    long length;
    long teklength;
    int ixmax,iymax;
    double scale;
    long milliSecondsPerFrame;
    unsigned long TimeToFlip;
    unsigned long LastClick;
    short int AnimationOn;
    
    int ActiveControl;
    int  LoopBackAndForth;
    int LoopFlag;

	uRect windowRect;
	int defaultsSet;
	
	uRect LineForeRect;
	uRect LineBackRect;
	uRect LinePatternRect;
       
	uRect SolidForeRect;
	uRect SolidBackRect;
	uRect SolidPatternRect;
       
	uRect TextRect;
	uRect ArrowRect;
	uRect LineRect;
	uRect LineWidthRect;
	uRect BoxRect;
	uRect OvalRect;
	uRect RoundRectRect;
	
	iRect TextImage;
	iRect ArrowImage;
	iRect LineImage;
	iRect BoxImage;
	iRect OvalImage;
	iRect RoundRectImage;

	iRect TextImageHigh;
	iRect ArrowImageHigh;
	iRect LineImageHigh;
	iRect BoxImageHigh;
	iRect OvalImageHigh;
	iRect RoundRectImageHigh;
	
	
    int ActiveLocked;
    
    int DuplicateFlag;
    
    int GridLines;
    
    int GridDelta;
    
    int BackGround;
    
    int SnapTo;
	double  vSnapTo;

   	int AntiAlias;
    
	struct FileInfo2 Files;
	
};

typedef struct DrawWindow *drawPtr;


struct Obj{long type;long Length;};

int GridPlotAdd(DOListPtr l,char *name,struct uLineData *Line);


int DODoubleClick(rPoint *r,DOListPtr l);

DObjPtr DOListFindObject(rPoint *r,DOListPtr l);

struct uAttributes *DOListGetAttributes(DOListPtr l);

int DOListSaveAttributes(DOListPtr l,struct uAttributes *vAttributes,int Flag);

void uDrawcPopUp(IconPtr myIcon,int iflag);
void uDrawwPopUp(IconPtr myIcon,int iflag);
void uDrawpPopUp(IconPtr myIcon,int iflag);

int uPutAttributes(struct uAttributes *Aout,struct uAttributes *Ain,int Flag);

struct DObject *ObjectCreate(char *name,int type,rRect *box,long size);
struct DObject *ObjectObject2(DObjPtr Pastin);


DObjPtr DOGetScrapObject(struct Obj *obj,DOListPtr l);
int ObjectSet(DObjPtr Object);
uBoxPtr uBoxBox2(uBoxPtr obj,DOListPtr d);
uOvalPtr uOvalOval2(uOvalPtr obj,DOListPtr d);
uLinePtr uLineLine2(uLinePtr obj,DOListPtr d);
uRoundRectPtr uRoundRect2(uRoundRectPtr obj,DOListPtr d);
uAlphaPtr uAlphaAlpha2(uAlphaPtr obj,DOListPtr d);
uGroup2Ptr uGroupGroup2(uGroup2Ptr obj,DOListPtr d);
uImagePtr uImageImage2(uImagePtr obj,DOListPtr d);
uTruePtr uTrueTrue2(uTruePtr obj,DOListPtr d);
uGridPlotPtr uGridPlot2(uGridPlotPtr obj,DOListPtr d);
uTekPtr uTekImage2(uTekPtr obj,DOListPtr d);

void ObjectKill(struct DObject *Object);
int ObjectBoxFrame(rRect *r,DOListPtr l);
int DODrawSelected(DOListPtr l);
int DODrawSelectedObject(DOListPtr l,DObjPtr Object);

void DOListDeSelectClick(DObjPtr Object,rPoint *ClickPoint);
void DOListSelectOne(DObjPtr Object);
void DOListDeSelectOne(DObjPtr Object);
void DOListDeSelectAll(DOListPtr l);
void DOListSelectClick(DObjPtr Object,rPoint *ClickPoint);
void DOListDragIt(DOListPtr l,DObjPtr Object);
int DOListGetMessage(DOListPtr l,long MessageType,void *MessageData);
void DOListKillSelected(DOListPtr l);
void DOListdoKeyMove(DOListPtr l,int c);
void DOListSelectAll(DOListPtr l);
void DOListCut(DOListPtr l);
void DOListCopy(DOListPtr l);
int uSetAttributes(DOListPtr l,struct uAttributes *Attribute,int Flag);
int SaveCopy(char *obj,long Length,char **list,long *Total);
void DOListDoInformation(DOListPtr l);
void DOListdoLoadImages(DOListPtr l);
void DOListSendBack(DOListPtr l);
void DOListSendDown(DOListPtr l);
void DOListSendFront(DOListPtr l);
void DOListSendUp(DOListPtr l);
int DOListColorBar(DOListPtr l);
void DOListGroup(DOListPtr l);
void DOListUnGroup(DOListPtr l);
uGridPlotPtr DOListDoNewPlot(DOListPtr l);
void DOListSetAnimation(DOListPtr l);
void DOListSaveAlpha(DOListPtr l,DObjPtr Ap);
int DOListSelectBox(DOListPtr l,uRect *r);
int rRectIntersect(rRect *r1,rRect *r2);
void DOListDoFlipDirection(DOListPtr l,int direct);
void DOListDoRotateDirection(DOListPtr l,int direct);
int DOListBuildColorBar(DOListPtr l,unsigned char **dataPtr,rRect *r,int *b2lists);

void DOListSetFontSelected(DOListPtr l);
void DOListPaste(DOListPtr l);
void DOListdoBoxes(DOListPtr l,int Type,uRect *r);
void DOListPasteLines(DOListPtr l);


int DOListReadWrite(DOListPtr l,FILE8 *out,int flag);

int	rLineTo(double x,double y,DOListPtr d);
int	rMoveTo(double x,double y,DOListPtr d);
int rBoxDraw(rRect *r,DOListPtr d);
int rDrawString(char *name,DOListPtr d);
int rFrameRect(rRect *r,DOListPtr d);
int rPaintRect(rRect *r,DOListPtr d);
int rFrameOval(rRect *r,DOListPtr d);
int rPaintOval(rRect *r,DOListPtr d);
int rFrameRoundRect(rRect *r,double ovalWidth,double ovalHeight,DOListPtr d);
int rPaintRoundRect(rRect *r,double ovalWidth,double ovalHeight,DOListPtr d);
int rDrawStringAngle(char *name,double angle,DOListPtr d);
int uDrawAdjustReal(uPoint *start,IconPtr myIcon);

int uDrawSaveScrap(IconPtr myIcon);

uPoint localToScreen(rPoint *r,DOListPtr l);
uRect DOuRectFromLocal(rRect *ri,DOListPtr l);
rPoint screenToLocal(uPoint *uwhere,DOListPtr l);

double PixelToLocal(long x,DOListPtr d);
int LocalToPixel(double x,DOListPtr d);

int	riMoveTo(int ix,int iy,DOListPtr d);
int riLineTo(int ix,int iy,DOListPtr d);


int uBoxSet(uBoxPtr b);
int uOvalSet(uOvalPtr b);
int uLineSet(uLinePtr b);
int uRoundRectSet(uRoundRectPtr b);
int uAlphaSet(uAlphaPtr b);
int uImageSet(uImagePtr b);
int uTrueSet(uTruePtr b);
int uGroupSet(uGroup2Ptr b);
int GridPlotSet(uGridPlotPtr Object);
int GridPlotLineSet(uLineDataPtr Object);
int uTekSet(uTekPtr b);

DObjPtr DOGetObjectPtr(int type,DOListPtr l);

int DrawSetEditAlpha(DOListPtr l,uPoint *where);

int ResizeArray(void *Array,long Length,long Count,long *Maximun,int tag);
int DOLineAttributes(DOListPtr l,uLineDataPtr Line,int n);


int uGridPlotInit(uGridPlotPtr b,struct uAttributes *Attributes);
int GridPlotPlotGrid(struct uGridPlot *b,DOListPtr d);
int GridPlotScale(struct uGridPlot *b);
int CPlotPlotNumbers(struct uGridPlot *b,DOListPtr d);

int ObjectBoxStretch(rRect *box,DOListPtr l);

#define StatusInActive	0
#define StatusActive	1
#define StatusDeleted	2
#define StatusNew	4

#define sForeColor    1
#define sBackColor    2
#define sPattern      4
#define lForeColor    8
#define lBackColor   16
#define lPattern     32
#define lWidth       64
#define lSymbol     128
#define lLine       256
#define lAnimate    512
#define tFont      1024

#define ControlArrow		1
#define ControlAlpha		2
#define ControlLine			3
#define ControlBox			4
#define ControlOval			5
#define ControlRoundRect	6


#define DEL 8

#endif
