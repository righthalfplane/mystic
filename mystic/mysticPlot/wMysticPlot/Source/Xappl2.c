#define EXTERN22 extern
#include "Xappl.h"
#include "FileDialog.h"
#include "File8.h"
#include "uFiles.h"
#include "Message.h"
#include "FileManager.h"
#include "StatusWindow.h"
#include <malloc.h>

HFONT hfont;
int FontStart(IconPtr IconPtr,HFONT hfont);
void checkall(void);
IconPtr FindMyIcon(IconPtr w);
int RemoveControlFromWindow(controlPtr thisControl);

FILE *junk;

int FreeMenuList(struct uMenuList *menuList);

char *ProgramName="wShow 17.0e1";

int  pDrawGo(IconPtr myIcon,long CurrentFrame,uRect *DRect)
{
	
	if(!myIcon || !DRect)return 1;
	
	if(myIcon->uDraw){
	    return (*myIcon->uDraw)(myIcon,CurrentFrame,DRect);
	}
	return 1;
}
static int intcmp(const void *xx, const void *yy)
{
  char **x;
  char **y;
  int n;

  x=(char **)xx;
  y=(char **)yy;
  n=strcmp(*x,*y);
  return n;
}

struct FileList *FileDialog3(char *name)
{
	struct FileList *Files;
	OPENFILENAME file;
	char filename[256];
	int ret,count,n;
	char *p;
	char *pc;

	if(SetBuffers(250000))return NULL;

	xg.buffin[0]=0;

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
	file.lpstrTitle=name;
	file.Flags=OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST;
	ret=GetOpenFileName(&file);
	if(!ret)return NULL;

	Files=(struct FileList *)cMalloc(sizeof(struct FileList),9002);
	if(!Files){
		Warning("FileDialog Out Of Memeory");
		return NULL;
	}

	zerol((char *)Files,sizeof(struct FileList));

	count=0;
	{

		p=xg.buffin;
		while(strlen(p)){
			++count;
			p += strlen(p)+1;
		}
	}

	p=xg.buffin;

	Files->name=(char **)cMalloc(count*sizeof(char *),9003);
	if(!Files->name){
	    goto ErrorOut;
	}
	zerol((char *)Files->name,sizeof(count*sizeof(char *)));


	Files->directory=strsave(p,9315);
	if(!Files->directory){
		goto ErrorOut;
  }
  if(count == 1){
    pc=strrchr(Files->directory,'\\');
    if(!pc){
      Warning("Directory Not found\n");
      goto ErrorOut;
    }
    Files->name[0]=strsave(pc+1,9314);
    if(!Files->name[0]){
		    goto ErrorOut;
    }
    *pc=0;
  }else{
    n=0;
    {
      
		    p=xg.buffin;
        while(strlen(p)){
          if(n > 0){
            Files->name[n-1]=strsave(p,9313);
            if(!Files->name[n-1]){
              goto ErrorOut;
            }	
          }
          ++n;
          p += strlen(p)+1;
        }
    }
    --count;
	}
	Files->count=count;
	chdir(Files->directory);

	   qsort((void *)Files->name,(size_t)count,(size_t)sizeof(char*),
		      (int (__cdecl *)(const void *, const void *))intcmp);
/*
	sprintf(buff,"directory %s\n",Files->directory);
	Warning(buff);
	for(n=0;n<count;++n){
	    sprintf(buff,"name %s\n",Files->name[n]);
		Warning(buff);
	}
*/
    return Files;
ErrorOut:
	FreeFileList(Files);
	return NULL;
}

IconPtr uStartStatusWindow(IconPtr myIcon2,char *header)
{
	return StartStatusWindow(header);	
}
int uWriteStatusWindow(IconPtr myIcon,char *message)
{
	return WriteStatusWindow(myIcon,message);
}
int uCheckStatusWindow(IconPtr myIcon,char *Message)
{
	return CheckStatusWindow(myIcon);
}
int uStopStatusWindow(IconPtr myIcon)
{
	return StopStatusWindow(myIcon);
}
int doEvents(void)
{
	return 0;
}

int doAbout(IconPtr myIcon)
{
BOOL CALLBACK DialogAbout(HWND hdWnd, UINT message, WPARAM wParam,
                                     LPARAM lParam);
	if(!myIcon)return 1;
	DialogBox(xg.hInst,"ABOUT_DIALOG",myIcon->myWindow,DialogAbout);
	return 0;
}
int FreeFileList(struct FileList *Ret)
{
	if(!Ret)return 1;
	if(Ret->directory)cFree((char *)Ret->directory);
	Ret->directory=NULL;
/*	if(Ret->d){
	    long n;

	    for(n=0;n<Ret->count;++n){
	        if(Ret->d[n].name)cFree((char *)Ret->d[n].name);
	    }
	    cFree((char *)Ret->d);
		Ret->d=NULL;
	}*/
	if(Ret->name){
	    long n;

	    for(n=0;n<Ret->count;++n){
	        if(Ret->name[n])cFree((char *)Ret->name[n]);
	    }
	    cFree((char *)Ret->name);
		Ret->name=NULL;
	}
	cFree((char *)Ret);
	return 0;
}

IconPtr CreateMyWindow(int Type,uRect *wRect)
{
	char buff[256];
	IconPtr p,o;

	p=IconRoot;
	o=NULL;
	
	while(p){		/*	Search for end of List	*/
		o=p;
		p=p->IconNext;
	}
	if(!(p=(IconPtr)cMalloc((long)sizeof(Icon),9194))){ /*	Make new link	*/
	    Warning("CreateMyWindow Out of Memory");
	    return (IconPtr)NULL;
	}
	zerol((char *)p,sizeof(Icon));
	if(o){
	    o->IconNext=p;
	} else {
	    IconRoot=p;
	}
	if(wRect){
		HWND hWnd;

		if(Type == xStatusType ){
            hWnd = CreateWindow(
            "StatusWindowClass",
            "Status Window",
            WS_OVERLAPPEDWINDOW,
            wRect->x,
            wRect->y,
            wRect->xsize,
            wRect->ysize,
            HWND_DESKTOP,
            NULL,
            xg.hInst,
            NULL);
		}else{
			Warning("CreateMyWindow Unknown window type");
			return NULL;
		}

		p->psd.lStructSize     = sizeof( PAGESETUPDLG );
        p->psd.hwndOwner       = hWnd;
        p->psd.rtMargin.top    = 1000;
        p->psd.rtMargin.left   = 1000;
        p->psd.rtMargin.bottom = 1000;
        p->psd.rtMargin.right  = 1000;  
        p->psd.hDevMode        = GlobalAlloc( GHND, sizeof( DEVMODE ) );
        p->psd.hDevNames       = GlobalAlloc( GHND, sizeof( DEVNAMES ) );
        p->psd.Flags           = PSD_MARGINS | PSD_INTHOUSANDTHSOFINCHES; 

        ShowWindow(hWnd, xg.nCmdShow);
        UpdateWindow(hWnd);
	    p->myWindow=hWnd;
		p->hdc=GetDC(p->myWindow);;
		p->region=CreateRectRgn(0,0,100,100);
		{

			if(!hfont)hfont=CreateFont(16,7,0,0,FW_THIN,
							 0,0,0,ANSI_CHARSET,
							 OUT_DEFAULT_PRECIS,
							 CLIP_DEFAULT_PRECIS,
							 PROOF_QUALITY,
							 DEFAULT_PITCH | FF_MODERN,
							 "Monaco");	
			    SelectObject(p->hdc,hfont);
		        FontStart(p,hfont);
		}
	} else {
	    sprintf(buff,"wRect Has NULL pointer");
	    Warning(buff);
	    return (IconPtr)NULL;
	}
	p->IconNext=NULL;
	p->FileSave=1;
	p->viewRect= *wRect;
	p->WindowType=Type;
	return p;
}
int RemoveControlFromWindow(controlPtr thisControl)
{
	IconPtr myIcon;
	controlPtr c,last;

	myIcon=FindIWindow(thisControl->controlWindow);
	if(myIcon == NULL)return 1;

	last=NULL;
	c=myIcon->controlList;
	while(c){
	    if(c == thisControl){			
	        if(last){
	            last->controlNext=c->controlNext;
	        }else{
	            myIcon->controlList=c->controlNext;
	        }
	        break;
	    }
		last=c;
	    c=c->controlNext;
	}

	return 0;
}
int AddControlToWindow(IconPtr myIcon,controlPtr thisControl)
{
	controlPtr c;

	if(myIcon == NULL)return 1;

	c=myIcon->controlList;
	if(c){
	    while(c->controlNext){
	        c=c->controlNext;
	    }
	    c->controlNext=thisControl;
	}else{
	    myIcon->controlList=thisControl;
	}

	return 0;
}
controlPtr FindAnyWindowControl(controlPtr thisControl)
{
	controlPtr c;
	IconPtr p;
	
	p=IconRoot;
	
	while(p){		/*	Search for control	*/
		c=p->controlList;
		while(c){
			if(c == thisControl) {
				return c;
			}
			c=c->controlNext;
		}

		p=p->IconNext;
	}
	return (controlPtr)NULL;
}
IconPtr RemoveIcon(IconPtr i)
{
	IconPtr p,o;
	controlPtr c,cn;
	
	p=IconRoot;
	o=NULL;
	
	while(p){		/*	Search for Icon	*/
	    if(p == i)break;
	    o=p;
	    p=p->IconNext;
	}
	
	if(!p)return (IconPtr)NULL;
	
	if(o)o->IconNext=p->IconNext; /*	Connect link	*/


	if(p == IconRoot){
	    if((o=p->IconNext) != NULL){
		    IconRoot=o;
	    } else {
		    o=NULL;
	    	IconRoot=NULL;
	    }
	}

	if(p->psd.hDevMode)GlobalFree(p->psd.hDevMode );
	p->psd.hDevMode=NULL;
    if(p->psd.hDevMode)GlobalFree(p->psd.hDevNames );
	p->psd.hDevNames=NULL;
	if(p->hdc)ReleaseDC(p->myWindow,p->hdc);
	p->hdc=NULL;

	c=p->controlList;
	while(c){
		cn=c->controlNext;
		c->doNotRemoveFromList=TRUE;
		ControlDispose(c);
		c=cn;
	}

	p->controlList=NULL;

	if(p->myWindow){
		DestroyWindow (p->myWindow);
		p->myWindow=NULL;
	}


	FreeMenuList(p->MenuList);

	p->MenuList=NULL;
	

	FileDialogRemoveInfo(&p->infoLoad);
	FileDialogRemoveInfo(&p->infoSave);

	DestroySysFontList(&p->pSysFontList);

	cFree((char *)p);
	
	if(!IconRoot){
	    doQuit();
	}
	
	return o;
}
IconPtr FindIWindow(HWND w)
{
	IconPtr p;
	
	p=IconRoot;
	
	while(p){		/*	Search for Window	*/
		if(p->myWindow == w) break;
		if(p->UpAndDown == w) {
			break;
		}
		if(p->LeftAndRight == w) {
			break;
		}

		if(p->controlList){
			controlPtr c;
			c=p->controlList;
			if(c->controlWindow == w) {
				return p;
			}
			while(c->controlNext){
				c=c->controlNext;
				if(c->controlWindow == w) {
					return p;
				}
			}
		}

		p=p->IconNext;
	}
	if (p) {
		return p;
	} else {
		return (IconPtr)NULL;
	}
}
IconPtr FindMyIcon(IconPtr w)
{
	IconPtr p;
	
	p=IconRoot;
	
	while(p){		/*	Search for Window	*/
		if(p == w) break;
		p=p->IconNext;
	}
	if (p) {
		return p;
	} else {
		return (IconPtr)NULL;
	}
}
IconPtr FindWindowType(int Type)
{
	IconPtr p;


	p=IconRoot;
	
	while(p){
	    if(p->WindowType == Type){
	        return p;
	    }
	    p=p->IconNext;
	}
	return (IconPtr)NULL;
}
int doQuit()
{
    extern struct fontData *fd;
	IconPtr myIcon;
    int (*pClose)(struct Icon *);
	int ret;

    if(IconRoot) {
		ret=MessageBox(IconRoot->myWindow,"Quit the Program ?","Exit",MB_YESNO);
		if(ret != IDYES)return 0;

        while((myIcon=IconRoot) != NULL){
            if(myIcon){
                pClose=myIcon->pClose;
				myIcon->doNotClose=FALSE;
                if(pClose){
                    if((*pClose)(myIcon)){
                          pClose=NULL;
                    }
                }
            }
        }

	}

		if(xg.buffin)cFree(xg.buffin);
		xg.buffin=NULL;

		if(xg.buffout)cFree(xg.buffout);
		xg.buffout=NULL;

		xg.buffsize=0;

		if(fd)cFree((char *)fd);

		fd=NULL;

		FileManagerExit(0);

		FileDialogRemoveInfoAll();

		ZeroScrap();

		uZeroScrap();

		checkall();

		WSACleanup();

 		PostQuitMessage(0); 

		exit(0);

        return 0;
}
int doDestroyWindow(IconPtr win)
{

    if(win){
	   int (*pClose)(struct Icon *);
       pClose=win->pClose;
        if(pClose){
			/* win->myWindow=NULL;*/
            if((*pClose)(win)){
                  /* win->pClose=NULL;*/
            }
        }
    }else{
		return 0;
	}
	return 1;
}
