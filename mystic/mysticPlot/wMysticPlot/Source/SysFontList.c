#define EXTERN22 extern
#include "Xappl.h"

#include "SysFontList.h"

int CreateSysFontList(struct Icon *myIcon,struct SYSFONTLIST *pSysFontList)
{
	int i,ntot,aFontCount[]={0,0,0};
  LOGFONT lf;
  LPLOGFONT lplf;

  lplf = &lf;
	
	if (!myIcon) return 0;
	if (!pSysFontList) return 0;

  zerol((char*)lplf,sizeof(LOGFONT));
  lplf->lfCharSet = DEFAULT_CHARSET;
  lplf->lfPitchAndFamily = 0;
  lplf->lfFaceName[0] = 0;

	EnumFontFamiliesEx(myIcon->hdc,lplf,(FONTENUMPROC)EnumFamCallBack,(LPARAM)aFontCount,0);
	
	pSysFontList->count = 0;
	pSysFontList->nRaster = aFontCount[0];
	pSysFontList->nVector = aFontCount[1];
	pSysFontList->nTrueType = aFontCount[2];
	ntot = pSysFontList->nTrueType + pSysFontList->nRaster + pSysFontList->nVector;

	if (!(pSysFontList->name = (char**)cMalloc(ntot*sizeof(char*),4736))) return 0;
	if (!(pSysFontList->type = (unsigned long*)cMalloc(ntot*sizeof(unsigned long),4737))) return 0;
	for (i=0;i<ntot;i++) {
		if (!(pSysFontList->name[i] = (char*)cMalloc(LF_FULLFACESIZE*sizeof(char),4738))) return 0;
	}

	EnumFontFamiliesEx(myIcon->hdc,lplf,(FONTENUMPROC)EnumFamCallBack2,(LPARAM)pSysFontList,0);

	SortSysFontList(pSysFontList,ntot);

/*
	for (i = 0;i < ntot;i++) {
		char name[2048];
		sprintf(name, " %d %s\n", i, pSysFontList->name[i]);
		WarningBatch(name);
	}
*/

	return 1;
}

int DestroySysFontList(struct SYSFONTLIST *pSysFontList)
{
	int i,ntot;

	if (!pSysFontList) return 0;
	
	ntot = pSysFontList->nTrueType + pSysFontList->nRaster + pSysFontList->nVector;
	
	for (i=0;i<ntot;i++) {
		if (pSysFontList->name[i]) {
			cFree(pSysFontList->name[i]);
			pSysFontList->name[i] = NULL;
		}
	}
	if (pSysFontList->name) cFree((char*)pSysFontList->name);
	pSysFontList->name = NULL;
	if (pSysFontList->type) cFree((char*)pSysFontList->type);
	pSysFontList->type = NULL;
	
	pSysFontList->nTrueType = 0;
	pSysFontList->nRaster = 0;
	pSysFontList->nVector = 0;

	return 1;
}

int SortSysFontList(struct SYSFONTLIST *pSysFontList,int ntot)
{
  char string[LF_FULLFACESIZE];
  int i,j,value;

  if (!pSysFontList) return 0;

  for(i=0;i<ntot-1;i++) {
  for(j=i+1;j<ntot;j++) {
    if(strcmp(pSysFontList->name[j],pSysFontList->name[i])<0) {
      strcpy(string,pSysFontList->name[j]);
      strcpy(pSysFontList->name[j],pSysFontList->name[i]);
      strcpy(pSysFontList->name[i],string);
      value = pSysFontList->type[j];
      pSysFontList->type[j] = pSysFontList->type[i];
      pSysFontList->type[i] = value;
    }
  }
  }

  return 1;
}

BOOL CALLBACK EnumFamCallBack(ENUMLOGFONTEX *lplf, NEWTEXTMETRICEX *lpntm, DWORD FontType, LPVOID aFontCount) 
{ 
	int far *aiFontCount = (int far *)aFontCount; 
	
	if (!aiFontCount) return FALSE;
	
	/* Record the number of raster, TrueType, and vector fonts in the font-count array. */
	if (FontType & RASTER_FONTTYPE) {
		aiFontCount[0]++; 
	} else if (FontType & TRUETYPE_FONTTYPE) {
		aiFontCount[2]++;
	} else {
		aiFontCount[1]++;
	}
	
	if (aiFontCount[0] || aiFontCount[1] || aiFontCount[2]) 
		return TRUE; 
	else 
		return FALSE; 

}

BOOL CALLBACK EnumFamCallBack2(ENUMLOGFONTEX *lplf, NEWTEXTMETRICEX *lpntm, DWORD FontType, LPVOID pvSysFonts) 
{ 
	struct SYSFONTLIST *pSysFontList = (struct SYSFONTLIST *)pvSysFonts; 
	
	if (!lplf) return FALSE;
	if (!pSysFontList) return FALSE;
	if (!pSysFontList->name) return FALSE;
	if (!pSysFontList->name[pSysFontList->count]) return FALSE;
	if (!pSysFontList->type) return FALSE;
	
//	mstrncpy(pSysFontList->name[pSysFontList->count],lplf->elfFullName,LF_FULLFACESIZE);
	mstrncpy(pSysFontList->name[pSysFontList->count],lplf->elfLogFont.lfFaceName,LF_FULLFACESIZE);
	if (FontType & RASTER_FONTTYPE) {
		pSysFontList->type[pSysFontList->count] = FontType;
	} else if (FontType & TRUETYPE_FONTTYPE) {
		pSysFontList->type[pSysFontList->count] = FontType;
	} else {
		pSysFontList->type[pSysFontList->count] = FontType;
	}
	pSysFontList->count++;
	
	return TRUE; 
	
} 
