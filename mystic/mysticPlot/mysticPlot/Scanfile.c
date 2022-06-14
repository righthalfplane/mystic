#include <stdio.h>
#include <regex.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <dirent.h>
#include <stdlib.h>
#define EXTERN22 extern
#include "xAux.h"
#include "Scanfile.h"
#include "Tools.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

     int re_exec(const char *s);
     int re_comp2(const char *filter);

#ifdef __cplusplus
}
#endif

int goCD(char *name);
char *DefaultPathString(void);
int MakeNewDirectory(char *name);
int domkdir(char *nb);

int mstrncat(char *out,char *in,long n);
int mstrncpy(char *out,char *in,long n);

extern int intcmp(const void *,const void *);

int CatalogHiddenShow = 1;

int goUpper(char *a,int len);

char *strsave(char *,int tag);

int re_done(void);

        regex_t preg;

int re_comp2(const char *name)
{
      int ret;
	  
      ret=regcomp(&preg,name,REG_NOSUB);

      return ret;
}
int re_exec(const char *name)
{
        static size_t nmatch;
        regmatch_t pmatch[3];

    return !regexec(&preg,name,nmatch,pmatch,0);
}
int re_done()
{
	regfree(&preg);
	return 0;
}
/*
main()
{
	struct scanList *scan;
	int count,n;

	scan=scan_dir(".");
	if(scan == NULL){
	    printf("scan_dir error\n");
	    exit(1);
	}
	printf("Files\n");
	for(n=0;n<scan->countFiles;++n){
	    printf("name %s \n",scan->namesFiles[n]);
	}
	printf("Dirs\n");
	for(n=0;n<scan->countDirs;++n){
	    printf("name %s \n",scan->namesDirs[n]);
	}
	free_dir(scan);
}
*/
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
	
	re_done();

	cFree((char *)scan);
	return 0;
}
struct scanList *scan_dir(char *dir,char *filter)
{
	register struct dirent *dirent;
	struct scanList		*scan;
	register DIR		*dirp;
	char				*name;
	int				    reg;
	int					count;
	int					n,nfile,ndir;
    char				working[2048];
    char				*path,*p,*pp;
    char				namefile[2048+256];
    int 				npwd;
    int					ret;

	ret=1;
	
	
	npwd=0;

	if((reg=re_comp2(filter)) != 0){
        fprintf(stderr,"scan_dir  error %d\n",reg);
	    return (struct scanList *)NULL;
	}
	
    if(strcmp(".",dir) && chdir(dir)){
        fprintf(stderr,"Could Not Change to Directory %s\n",dir);
    }
	

    if(!getcwd(working,2048)){
        fprintf(stderr,"scan_dir  Working directory error %s\n",working);
        return (struct scanList *)NULL;
    }

    if((scan=(struct scanList *)cMalloc(sizeof(struct scanList),8651)) == NULL){
        fprintf(stderr,"scan_dir  Out of Memroy\n");
        return (struct scanList *)NULL;
    }
	zerol((char *)scan,sizeof(struct scanList));
	
	ndir=0;
	nfile=0;
	scan->namesFiles = NULL;
	scan->namesDirs  = NULL;

	dirp = opendir(dir);

	if (dirp == NULL)goto ErrorOut;

	count = 0;

	while ((dirent = readdir(dirp)) != NULL) {
	    ++count;
	}

    rewinddir(dirp);
    
    scan->namesPwd=(char **)cMalloc(2000*sizeof(char *),98727);
	if(!scan->namesPwd)goto ErrorOut;
	zerol((char *)scan->namesPwd,2000*sizeof(char *));
	
	
    if(((scan->namesFiles)=(char **)cMalloc(sizeof(char *)*count,8652)) == NULL){
		fprintf(stderr,"scandir out of memory\n");
		goto ErrorOut;
    }
    if(((scan->namesDirs)=(char **)cMalloc(sizeof(char *)*count,8653)) == NULL){
		fprintf(stderr,"scandir out of memory\n");
		goto ErrorOut;
    }
    n=0;
	while ((dirent = readdir(dirp)) != NULL){
		struct stat buf;

		if(!stat(dirent->d_name,&buf) && (buf.st_mode & S_IFDIR)){
	        if(!strcmp(dirent->d_name,".")){
	            --count;
	            continue;
	        }
	        namefile[0]=0;
	        /*
	        mstrncpy(namefile,working,2048);
	        mstrncat(namefile,"/",2048);
	        */
	        mstrncat(namefile,dirent->d_name,2048);
	        
			if(!CatalogHiddenShow || (dirent->d_name[0] != '.')){
		        name=strsave(namefile,8827);
		        if(name == NULL){
		             fprintf(stderr,"scandir out of memory\n");
		             goto ErrorOut;
		        }
				scan->namesDirs[ndir++]=name;
			}
		}else{	   
			if(re_exec(dirent->d_name) == 1){
			    if(!CatalogHiddenShow || (dirent->d_name[0] != '.')){
		             name=strsave(dirent->d_name,8828);
		             if(name == NULL){
		                 fprintf(stderr,"scandir out of memory\n");
		                 goto ErrorOut;
		             }
			     	scan->namesFiles[nfile++]=name;
		     	}
			}
		}
		++n;
	}
	
	if(count != n){
	   fprintf(stderr,"scandir error %d %d\n",count,n);
	   goto ErrorOut;
	}
		
	path=working+1;
	if(path){
	    if(!(scan->namesPwd[npwd++]=strsave("/",8888)))goto ErrorOut;
	    p=pp=path;
	    while(*p){
	       if(*p == '/'){
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
	
	scan->countFiles=nfile;
	scan->countDirs=ndir;
	scan->countPwd=npwd;

	qsort((char *)scan->namesFiles, scan->countFiles, sizeof(char *), intcmp);
	qsort((char *)scan->namesDirs, scan->countDirs, sizeof(char *), intcmp);

	ret=0;
ErrorOut:
	if(dirp)closedir(dirp);
	if(ret == 0)return (scan);
	if(scan){
	   scan->countFiles=nfile;
	   scan->countDirs=ndir;
	   free_dir(scan);
	}
	regfree(&preg);

	return (struct scanList *)NULL;
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
int goCD(char *name)
{
	if(!name)return 1;
	return chdir(name);
}

int MakeNewDirectory(char *name)
{
	if(!name)return 1;
	
	if(domkdir(name))return 1;
	
	return chdir(name);
}

char *DefaultPathString(void)
{
    char working[2048];
    
    if(!getcwd(working,2048))return NULL;

	return strsave(working,1982);
}
int domkdir(char *nb)
{
        int ret;

        ret = mkdir(nb,0755);

        return ret;
}

