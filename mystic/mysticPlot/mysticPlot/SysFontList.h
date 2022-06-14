#ifndef _SYSFONTLISTH_
#define _SYSFONTLISTH_


struct SYSFONTLIST {
  unsigned long *type; /* 0=vector or other,1=raster,4=truetype */
  int nTrueType,nRaster,nVector,count;
  char **name;
  int lastFont;
  int lastSize;
  int lastFace;
};

int CreateSysFontList(struct SYSFONTLIST *pSysFontList);
int DestroySysFontList(struct SYSFONTLIST *pSysFontList);

#define FONTSTYLE_PLAIN			0
#define FONTSTYLE_BOLD			1
#define FONTSTYLE_ITALIC		2
#define FONTSTYPE_UNDERLINE		4
#define FONTSTYPE_STRIKEOUT		8


#endif
