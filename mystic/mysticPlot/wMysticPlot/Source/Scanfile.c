#include <stdio.h>
//#include <regex.h>
#include <ctype.h>
//#include <sys/types.h>
//#include <sys/stat.h> 
//#include <dirent.h>
#include <stdlib.h>
#define EXTERN extern
#include "Xappl.h"
#include "Paux.h"
#include "Scanfile.h"
#include "Tools.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

     int re_exec(const char *s);
     char *re_comp(const char *filter);

#ifdef __cplusplus
}
#endif

extern int intcmp(const void *,const void *);

int CatalogHiddenShow = 0;

int goUpper(char *a,int len);

//        regex_t preg;

char *re_comp(const char *name)
{
      char *ret=0;
      return ret;
}
int re_exec(const char *name)
{
    return 0;
}
int free_dir(struct scanList *scan)
{
	int n;

	if(!scan)return 1;
	
	if(scan->namesFiles){
	    for(n=0;n<scan->countFiles;++n){
	        if(scan->namesFiles[n])cFree(scan->namesFiles[n]);
	    }
	    cFree((char *)scan->namesFiles);
	}

	if(scan->namesDirs){
	    for(n=0;n<scan->countDirs;++n){
	        if(scan->namesDirs[n])cFree(scan->namesDirs[n]);
	    }
	    cFree((char *)scan->namesDirs);
	}
	
	if(scan->namesPwd){
		for(n=0;n<scan->countPwd;++n){
		    if(scan->namesPwd[n])cFree(scan->namesPwd[n]);
		}
		cFree((char *)scan->namesPwd);
	}
/*
	if(scan->directory){
	    cFree((char *)scan->directory);
	}
*/
	cFree((char *)scan);
	return 0;
}
struct scanList *scan_dir(char *diri,char *filter)
{
 	struct scanList *scan;
	WIN32_FIND_DATA file;
	char buff[256],working[1024],dir[256];
	char *path,*p,*pp;
	HANDLE filelist;
	int nfile,ndir,npwd;
	long count;
	int ret,n;

	scan=NULL;

	if(!diri)return scan;
/*
	sprintf(WarningBuff,"scan_dir %s\n",diri);
	WarningBatch(WarningBuff);
*/
	if(!strcmp(diri,".")){
		mstrncpy(dir,"*.*",256);
	}else{
		goCD(diri);
		mstrncpy(dir,diri,256);
		mstrncat(dir,"\\*.*",256);
	}

	ret=1;
	count=0;
	filelist=FindFirstFile(dir,&file);
	if(filelist == INVALID_HANDLE_VALUE){
	   sprintf(buff,"FindFirstFile failed for name (%s)",dir);
	   return scan;
	}

	++count;

	while(FindNextFile(filelist,&file)){
		++count;
	}
	FindClose(filelist);
	filelist=NULL;



   if((scan=(struct scanList *)cMalloc(sizeof(struct scanList),8651)) == NULL){
        fprintf(stderr,"scan_dir  Out of Memroy\n");
        return scan;
    }
	zerol((char *)scan,sizeof(struct scanList));
	
	ndir=0;
	nfile=0;
	npwd=0;

	scan->namesFiles = NULL;
	scan->namesDirs  = NULL;

    scan->namesPwd=(char **)cMalloc(2000*sizeof(char *),98727);
	if(!scan->namesPwd)goto ErrorOut;
	zerol((char *)scan->namesPwd,2000*sizeof(char *));
	
	
    if(((scan->namesFiles)=(char **)cMalloc(sizeof(char *)*count,8652)) == NULL){
		fprintf(stderr,"scandir out of memory\n");
		goto ErrorOut;
    }
    if(((scan->namesDirs)=(char **)cMalloc(sizeof(char *)*(count+32),8653)) == NULL){
		fprintf(stderr,"scandir out of memory\n");
		goto ErrorOut;
    }


	GetCurrentDirectory(1024,working);
	/*
	scan->directory=strsave(working,9319);
	if(!scan->directory)goto ErrorOut;
	*/
	count = 0;

	filelist=FindFirstFile(dir,&file);
	if(filelist == INVALID_HANDLE_VALUE){
	   sprintf(buff,"FindFirstFile failed for name (%s)",dir);
	   filelist=NULL;
	   goto ErrorOut;
	}

	if(CatalogHiddenShow || file.cFileName[0] != '.'){
	    if(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
		    scan->namesDirs[ndir]=strsave(file.cFileName,9317);
		    if(!scan->namesDirs[ndir])goto ErrorOut;
		    ++ndir;
		}else{
		    scan->namesFiles[nfile]=strsave(file.cFileName,9317);
		    if(!scan->namesFiles[nfile])goto ErrorOut;
		    ++nfile;
		}
	}


	while(FindNextFile(filelist,&file)){
	    if(CatalogHiddenShow || file.cFileName[0] != '.'){
	        if(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
		        scan->namesDirs[ndir]=strsave(file.cFileName,9317);
		        if(!scan->namesDirs[ndir])goto ErrorOut;
		        ++ndir;
			}else{
		        scan->namesFiles[nfile]=strsave(file.cFileName,9317);
		        if(!scan->namesFiles[nfile])goto ErrorOut;
		        ++nfile;
			}
		}
	}


	
//	path=working+1;
/*
		sprintf(WarningBuff,"scan_dir working %s dir %s\n",working,dir);
		WarningBatch(WarningBuff);
*/
	path=working;
	if(path){
	    // if(!(scan->namesPwd[npwd++]=strsave("/",8888)))goto ErrorOut;
	    p=pp=path;
	    while(*p){
	       if(*p == '\\'){
	          *p = 0;
	          if(!(scan->namesPwd[npwd++]=strsave(pp,8888)))goto ErrorOut;
	          pp=p+1;
	       }
	       ++p;
	    } 
	    if(pp < p){
	          if(!(scan->namesPwd[npwd++]=strsave(pp,8888)))goto ErrorOut;
		}
		

		if(npwd > 1){
		    for(n=0;n<npwd/2;++n){
		        char *save;
		        save=scan->namesPwd[n];
		        scan->namesPwd[n]=scan->namesPwd[npwd-n-1];
		        scan->namesPwd[npwd-n-1]=save;
		    }
		}		
		

	}

	scan->countFiles  = nfile;
	scan->countPwd  = npwd;

	if(npwd == 1){
		char dir2[10];
		int n;
		unsigned int ds;

		ds=GetLogicalDrives();

		for(n=0;n<32;++n){
			if(ds & (1<<n)){
				dir2[0]='A'+n;
				dir2[1]=':';
				dir2[2]='\\';
				dir2[3]=0;
				scan->namesDirs[ndir]=strsave(dir2,9317);
				if(!scan->namesDirs[ndir])goto ErrorOut;
				++ndir;
			}
		}
	}

	scan->countDirs = ndir;

	qsort((char *)scan->namesFiles, scan->countFiles, sizeof(char *), intcmp);
	qsort((char *)scan->namesDirs, scan->countDirs, sizeof(char *), intcmp);
/*
	if(npwd == 1){
		char *p1,*p2;
		int n;

		for(n=0;n<ndir;++n){
			if(scan->namesDirs && scan->namesDirs[n]){
				p1=scan->namesDirs[n];
				if(*p1 == ' '){
					p2=p1+1;
					while(*p1){
						*p1++ = *p2++;
					}
				}
			}
		}
	}
*/

	/*
	if(scan->namesFiles && scan->countFiles > 0){
		sprintf(WarningBuff,"scan_dir file 1 %s\n",scan->namesFiles[0]);
		WarningBatch(WarningBuff);
	}
	if(scan->namesDirs && scan->countDirs > 0){
		sprintf(WarningBuff,"scan_dir dir 1 %s\n",scan->namesDirs[0]);
		WarningBatch(WarningBuff);
	}
*/

	ret=0;
ErrorOut:
	if(filelist)FindClose(filelist);

	return scan;
}

int intcmp(const void *xx,const  void *yy)
{
    char xb[4024],yb[4024];
	register char **x=(char **)xx;
	register char **y=(char **)yy;
	mstrncpy(xb,*x,4024);
	goUpper(xb,4024);
	mstrncpy(yb,*y,4024);
	goUpper(yb,4024);
	return strcmp(xb,yb);
	/* return strcmp(*x,*y); */
}
/*
int goUpper(char *a,int len)
{
	if(!a)return 1;

	while(*a && (--len >= 0)){
	    *a=toupper(*a);
	    ++a;
	}
	return 0;
}
*/
