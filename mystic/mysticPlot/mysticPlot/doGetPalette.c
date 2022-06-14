#define EXTERN22 extern
#include "Xappl.h"
#include "uFiles.h"
extern int SetDefaultPaletteName(IconPtr myIcon,char *name);
int getString(unsigned char *s,long n,FILE *in);
int DFPgetpal(char *name,char *pal);
int doGetPalette(IconPtr myIcon);
short DFPrestart(void);

int DFPputpal(char *filename,unsigned char *pal,int overwrite,char *filemode);

int doSavePalette(IconPtr myIcon);

int doAddPalette(IconPtr myIcon,char *name);


int LoadToMenu(IconPtr myIcon);

int doLoadPalette(char *name,IconPtr myIcon);

char *strpathname(char *path,char *name);

int LoadToMenu(IconPtr myIcon)
{
	int TypeList[]={OPEN_ALL_FILE_TYPES,0};
	struct FileList *Files;
	char *pathname;
	int ret;
	long n;
	
	if(!myIcon)return 1;
	
	ret=1;
	
	pathname=NULL;
	Files=uOpenFileDialog(myIcon,TypeList);		
	if(!Files)goto ErrorOut;
	
	for(n=0;n<Files->count;++n){
	
		pathname=strpathname(Files->directory,Files->name[n]);
		if(!pathname)goto ErrorOut;
		
		if(doLoadPalette(Files->name[n],myIcon))goto ErrorOut;
	
		if(pathname)cFree(pathname);
		pathname=NULL;
	}	
	    
	ret=0;
ErrorOut:
	if(Files)FileListFree(Files);
	if(pathname)cFree(pathname);
	return ret;
}


int doGetPalette(IconPtr myIcon)
{
	char name[2560];
	char *fp;
	
	if(!myIcon)return 1;
	
	if(GetTheFileName((char *)name,"Load Palette File",2560,&myIcon->infoLoad))return 1;
	
	fp=strrchr((char *)name,FILE_NAME_SEPERATOR);
    if(fp){
        fp += 1;
    }else{
        fp=name;
    }

	
	return doLoadPalette(fp,myIcon);
	
}
int doLoadPalette(char *name,IconPtr myIcon)
{
 	char pal[256*3];
   	FILE *in;
   	int ret;
    int n;
    
	if(!myIcon || !name)return 1;
	
	ret=1;
	
	in=NULL;
	
	DFPrestart();
	if(DFPgetpal(name,pal)){ /* Try to Read Raw Palette if not HDF */
	    in=fopen(name,"rb");
	    if(!in){
	        sprintf(WarningBuff,"doGetPalette Could Not Open \'%s\' To Read",name);
	        WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    if(getString(myIcon->red,256,in))goto ErrorOut;
	    if(getString(myIcon->green,256,in))goto ErrorOut;
	    if(getString(myIcon->blue,256,in))goto ErrorOut;
	}else{
	    for(n=0;n<256;++n){
	        myIcon->red[n]=pal[3*n];
	        myIcon->green[n]=pal[3*n+1];
	        myIcon->blue[n]=pal[3*n+2];
	    }
	}
	
	SetOldColorPalette(myIcon);
	ScreenSetPalette(myIcon,myIcon->red,myIcon->green,myIcon->blue);
	doAddPalette(myIcon,(char *)name);
	SetDefaultPaletteName(myIcon,(char *)name);
	InvalRectMyWindow(myIcon);
	ret=0;
	
ErrorOut:
	if(in)fclose(in);
	return ret;
	
}

int doSavePalette(IconPtr myIcon)
{
	static char name[256]={'m','S','h','o','w','.','p','a','l'};
    unsigned char pal[3*256];
    int n;
    
    
    if(!myIcon)return 1;
    
	if(GetTheSaveFileName(name,"Save Palette",256,&myIcon->infoSave))return 1;
	
	for(n=0;n<256;++n){
	    pal[3*n]=myIcon->red[n];
	    pal[3*n+1]=myIcon->green[n];
	    pal[3*n+2]=myIcon->blue[n];
	}
	
	if(DFPputpal(name,pal,0,"w")){
	    sprintf(WarningBuff,"doSavePalette Could Not Open \'%s\' To Write",name);
	    WarningBatch(WarningBuff);
	    return 1;
	}

	return 0;
	
}
