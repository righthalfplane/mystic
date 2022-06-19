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

int CreateSysFontList(struct Icon *myIcon,struct SYSFONTLIST *pSysFontList);
int DestroySysFontList(struct SYSFONTLIST *pSysFontList);
int SortSysFontList(struct SYSFONTLIST *pSysFontList,int ntot);
BOOL CALLBACK EnumFamCallBack(ENUMLOGFONTEX *lplf, NEWTEXTMETRICEX *lpntm, DWORD FontType, LPVOID aFontCount);
BOOL CALLBACK EnumFamCallBack2(ENUMLOGFONTEX *lplf, NEWTEXTMETRICEX *lpntm, DWORD FontType, LPVOID pvSysFonts);
#define FONTSTYLE_PLAIN			0
#define FONTSTYLE_BOLD			1
#define FONTSTYLE_ITALIC		2
#define FONTSTYPE_UNDERLINE		4
#define FONTSTYPE_STRIKEOUT		8


#endif
