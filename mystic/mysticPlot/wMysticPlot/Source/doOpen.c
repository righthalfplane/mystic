#define EXTERN22 extern     
#include "Xappl.h"

int doOpen(void)
{
	OPENFILENAME file;
	char buff[256],filename[256];
	int ret;

	if(SetBuffers(250000))return 1;

	memset(&file,0,sizeof(OPENFILENAME));
	file.lStructSize=sizeof(OPENFILENAME);
	file.hwndOwner=HWND_DESKTOP;
	file.lpstrFilter="all\0*.*\0\0\0";
	file.nFilterIndex=1;
	file.lpstrFile=xg.buffin;
	file.nMaxFile=250000;
	strcpy(filename,"This is Filename");
	file.lpstrFileTitle=filename;
	file.nMaxFileTitle=sizeof(filename)-1;
	file.Flags=OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST;
	ret=GetOpenFileName(&file);
	if(!ret)return 0;

	{
		char *p;

		p=xg.buffin;
		while(strlen(p)){
			sprintf(buff,"filename %s",p);
			p += strlen(p)+1;
			Warning(buff);
		}
	}
    return 0;
}
