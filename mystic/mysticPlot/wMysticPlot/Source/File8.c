#include "Xappl.h"
#include "File8.h"
#include <io.h>
#include <string.h>
#include <fcntl.h>
/*
extern char *cMalloc(long Length,int Tag);
extern int cFree(char *data);
extern void zerol(char *ptr,long length);
*/

INT8_64 FileSeeks8,FileReads8,FileWrite8,FileReadBytes8;

FILE8 *fopen8(char *name,char *mode)
{
	FILE8 *file8;
	FILE *out;
	int open;

	if(!name || !mode)return NULL;

  if (strcmp(mode,"rb")==0) {
	  open=_open(name,_O_RDONLY | _O_BINARY);
	  if(open==-1) return NULL;
  } else if (strcmp(mode,"wb")==0){
	  open=_open(name,_O_TRUNC | _O_RDWR | _O_BINARY);
	  if(open==-1){
		  out=fopen(name,"wb");
		  if(out)fclose(out);
		  open=_open(name,_O_TRUNC | _O_RDWR | _O_BINARY);
	  }
	  if(open==-1) return NULL;
  } else {
    return NULL;
  }

	file8=(FILE8 *)cMalloc(sizeof(FILE8),6476);
	if(!file8){
	    if(open!=-1) _close(open);
	    return NULL;
	}
	zerol((char *)file8,sizeof(FILE8));
	file8->ifile=open;
	return file8;

}

int fclose8(FILE8 *file8)
{
	if(!file8)return 1;
  if(file8->ifile!=-1) {
    _close(file8->ifile);
  } else {
    return 1;
  }
	file8->ifile=-1;
	cFree((char *)file8);
	return 0;
}

INT8_64 ftell8(FILE8 *file8)
{
	if(!file8)return -1;
	if(file8->ifile==-1)return -1;
	return _telli64(file8->ifile);
}

int fseek8(FILE8 *file8,INT8_64 offset,int from)
{
	if(!file8)return -1;
	if(file8->ifile==-1)return -1;
	++FileSeeks8;
  if(_lseeki64(file8->ifile,offset,from)==-1) {
    return -1;
  } else {
    return 0;
  }
}

int fput8(FILE8 *file8,char *data,INT8_64 lengthin)
{
	if(!data)return -1;
	if(!file8)return -1;
	if(file8->ifile==-1)return -1;
	if(_write(file8->ifile,(char *)data,(unsigned int)lengthin)!=(int)lengthin) return -1;
	++FileWrite8;
	return 0;
}

int fget8(FILE8 *file8,char *data,INT8_64 lengthin)
{
	if(!data)return -1;
	if(!file8)return -1;
	if(file8->ifile==-1)return -1;
	if(_read(file8->ifile,(char *)data,(unsigned int)lengthin)!=(int)lengthin) return -1;
	++FileReads8;
	FileReadBytes8 += lengthin;
	return 0;
}
