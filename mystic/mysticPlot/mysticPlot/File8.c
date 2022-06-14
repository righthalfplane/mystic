#include "File8.h"
#include "ulibTypes.h"

extern char *cMalloc(long Length,int Tag);
extern int cFree(char *data);
extern void zerol(char *ptr,long length);

INT8_64 FileSeeks8,FileReads8,FileWrite8,FileReadBytes8;

FILE8 *fopen8(char *name,char *mode)
{
	FILE8 *file;
	FILE *open;

	if(!name || !mode)return NULL;

	open=fopen(name,mode);
	if(!open)return NULL;

	file=(FILE8 *)cMalloc(sizeof(FILE8),6476);
	if(!file){
	    if(open)fclose(open);
	    return NULL;
	}
	zerol((char *)file,sizeof(FILE8));
	file->file=open;
#ifdef PC
	file->convertRead=1;
	file->convertWrite=1;
#endif
	return file;

}

int fclose8(FILE8 *file)
{
	if(!file)return 1;
	if(file->file)fclose(file->file);
	file->file=NULL;
	cFree((char *)file);
	return 0;
}

INT8_64 ftell8(FILE8 *file)
{
	
	if(!file)return -1;
	if(!file->file)return -1;
	return ftello(file->file);;
}

int fseek8(FILE8 *file,INT8_64 offset,int from)
{
	if(!file)return -1;
	if(!file->file)return -1;
	++FileSeeks8;
	return fseeko(file->file,offset,from);
}

int fput8(FILE8 *file,char *data,INT8_64 lengthin)
{
	long length;

	if(!data)return -1;
	if(!file)return -1;
	if(!file->file)return -1;
	length=(long)lengthin;
	++FileWrite8;
	if(fwrite((char *)data,1,length,file->file) != length){ 
	    return -1;
	}
	return 0;
}

int fget8(FILE8 *file,char *data,INT8_64 lengthin)
{
	long length;

	if(!data)return -1;
	if(!file)return -1;
	if(!file->file)return -1;
	length=(long)lengthin;
	++FileReads8;
	FileReadBytes8 += length;
	if(fread((char *)data,1,length,file->file) != length){ 
	    return -1;
	}

	return 0;
}
