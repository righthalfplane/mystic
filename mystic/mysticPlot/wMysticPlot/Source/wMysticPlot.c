#define EXTERN22  extern
#include <time.h>
#include "Xappl.h"
#include "FileDialog.h"
#include "uFiles.h"
#include "TracerDef.h"
#include "StatusWindow.h"
#include "Net.h"
#include "uLib.h"
#include "Universal.h"


int NetStop(struct NetWork *net);
int doSetDir(char *Name,char *Directory);
struct FileList *FileDialog3(char *name);

struct sendstruct{
	int dataType;
	int sendPalette;
	char SendTarget[256];
	char SendPassword[256];
	int SendPort;
	SOCKET sd;
	SOCKET rd;
	HANDLE hGetHostInfo; 
    char *pGetHostInfo;
    HANDLE hGetHostID;
	int netErrorCheck;

} sendData;


IconPtr openRender(struct Scene *scene);

extern int doOpen(void);


LRESULT CALLBACK AllWndProc(HWND hWnd, UINT message, WPARAM wParam,
                                     LPARAM lParam);

LRESULT CALLBACK StatusWndProc(HWND hWnd, UINT message, WPARAM wParam,
                                     LPARAM lParam);

BOOL CALLBACK DialogAbout(HWND hdWnd, UINT message, WPARAM wParam,
                                     LPARAM lParam);

char str[255]= "";


HWND sDlg;

int startp(HINSTANCE hInstance,HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow);


VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);

int doNULL(void);

int TranslateAcceleratorM(MSG *msg);

int  Start(void);

IconPtr NewNewWindow(IconPtr myIcon);

int DefaultFont=8,DefaultFontSize=16;


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow)
{
	char working[2048];
	MSG msg;

	getcwd(working,2048);

	Start();

    startp(hInstance,hPrevInstance,lpCmdLine,nCmdShow);

    SetTimer(NULL,1,33,(TIMERPROC)TimerProc);

	startUniversal();

	if(uPref.UseStartConfiguration && uPref.startConfiguration){
	    char *save=uPref.startConfiguration;
	    uPref.startConfiguration=NULL;
		readUniversal(save,1);   
		if(uPref.startConfiguration)cFree((char *)uPref.startConfiguration);
		uPref.startConfiguration=save;
	}else{
		NewNewWindow(NULL);
	}

    while (GetMessage(&msg, NULL, 0, 0)) {
		if(!IsDialogMessage(sDlg,&msg)){
			if(!TranslateAcceleratorM(&msg)){
			    TranslateMessage(&msg);
			    DispatchMessage(&msg);
			}
		}
    }

	KillTimer(NULL,1);

    return (msg.wParam);

}
int TranslateAcceleratorM(MSG *msg)
{
	IconPtr win;

	if(!msg || !msg)return 0;

	win=FindIWindow(msg->hwnd);
	if(!win || !win->hAccel)return 0;

	return TranslateAccelerator(msg->hwnd,win->hAccel,msg);
}
VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{
	doNULL();
}
int SetMyIconWindowFront(IconPtr FrontWindow)
{
	if(!FrontWindow)return 1;
	FrontWindow=FindWindowInList(FrontWindow);
	if(!FrontWindow)return 1;
	BringWindowToTop(FrontWindow->myWindow);
	return 0;
}

IconPtr FindMyFrontWindow()
{
	IconPtr p;
	HWND w;
	
	w=GetTopWindow(NULL);
	while(w) {
		p=IconRoot;
		while(p){
			if(p->myWindow == w) {
				return p;
			}
			p=p->IconNext;
		}
		w=GetNextWindow(w,GW_HWNDNEXT);
	}
	return NULL;
}
int doNULL(void)
{
	IconPtr myIcon;
	

	if(!IconRoot && !sDlg){
		PostQuitMessage(0);
		return 0;
	}


	for(myIcon=IconRoot;myIcon;myIcon=myIcon->IconNext){
		if(!myIcon->isOpen)continue;


	if(myIcon->ThreadId != GetCurrentThreadId()){
		return 1;
	}


		if(myIcon->pdoNULL){
			(*myIcon->pdoNULL)(myIcon);
		}
		if(myIcon->pNullEvent){
			(*myIcon->pNullEvent)(myIcon);
		}
	}

	return 0;
}

int startp(HINSTANCE hInstance,HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS  wcl;  /* Eventually wcl can go away and be replaced be wcl2 */
    /* WNDCLASS  wcl2; */

	WNDCLASSEX wcex;
/*
    HWND hWnd;	
    HACCEL  hAccel;
	MSG msg;
*/	

	/* window class for NON-dialogs */

    xg.hInst=hInstance;
    xg.nCmdShow=nCmdShow;

	zerol((char *)&wcex,sizeof(wcex));
    wcex.style = 0;
    wcex.cbSize = sizeof(wcex); 
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;   
    wcex.hInstance = hInstance;
    wcex.lpszClassName = "AllWindowClass";
    wcex.lpfnWndProc = AllWndProc;
     wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszMenuName = NULL;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    if(!RegisterClassEx(&wcex))return 0;

	/* window class for dialogs */
 	zerol((char *)&wcex,sizeof(wcex));
	wcex.style = 0;
    wcex.cbSize = sizeof(wcex); 
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;   
    wcex.hInstance = hInstance;
    wcex.lpszClassName = "AllWindowClassDialog";
    wcex.lpfnWndProc = AllWndProc;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszMenuName = NULL;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
//    wcl2.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
    wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
    xg.hInst=hInstance;
    xg.nCmdShow=nCmdShow;
    if(!RegisterClassEx(&wcex))return 0;


    wcl.hInstance = hInstance;
    wcl.lpszClassName = "StatusWindowClass";
    wcl.lpfnWndProc = StatusWndProc;
    wcl.style = 0;
    wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcl.lpszMenuName =  NULL;
    wcl.cbClsExtra = 0;
    wcl.cbWndExtra = 0;
    wcl.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
    xg.hInst=hInstance;
    xg.nCmdShow=nCmdShow;
    if(!RegisterClass(&wcl))return 0;
    
    return 0;
}


int doSetDir(char *Name,char *Directory)
{
	OPENFILENAME file;
	char *pc,*p;
	int ret;

	if(!Name || !Directory)return 1;


	if(SetBuffers(2500))return 1;

	memset(&file,0,sizeof(OPENFILENAME));
	file.lStructSize=sizeof(OPENFILENAME);
	file.hwndOwner=HWND_DESKTOP;
	file.lpstrFilter="all\0*.*\0\0\0";
	file.nFilterIndex=1;
	file.lpstrFile=xg.buffin;
	strcpy(xg.buffin,Name);
	file.nMaxFile=2500;
	file.lpstrFileTitle=Name;
	file.nMaxFileTitle=sizeof(Name)-1;
	file.lpstrTitle="Save Files:";
	file.Flags=OFN_EXPLORER | OFN_HIDEREADONLY;
	ret=GetSaveFileName(&file);
	if(!ret)return 1;

	pc=strrchr(xg.buffin,'\\');
	if(!pc){
		Warning("Directory Not found\n");
		return 1;
	}

	*pc=0;

	p=pc+1;
	while(*p)*Name++ = *p++;
	*Name=0;


	p=xg.buffin;
	while(*p)*Directory++ = *p++;
	*Directory++='\\';
	*Directory=0;

	return 0;
}
LRESULT CALLBACK StatusWndProc(HWND hWnd, UINT message, WPARAM wParam,
                                     LPARAM lParam)
{
	IconPtr win;
	statPtr s;
	HDC hdc;
	PAINTSTRUCT paint;
    int iRc; 
	
	win=FindIWindow(hWnd);
	s=NULL;
	if(win){
	    s=(statPtr)win->DWindow;
	}
 
   switch (message) {
    case WM_WSAASYNC:

		switch (WSAGETSELECTEVENT(lParam)) { 
        
        case FD_CONNECT:      
            iRc = WSAGETSELECTERROR(lParam);
            if (iRc) {
				Warning("FD_CONNECT error");
				if(s)s->iconnect = -1;
                return 0;
            }
			if(s)s->iconnect = 1;
            return 0;
        
        case FD_READ:       
 			if(s)s->iread = 1;
           break;
        
        case FD_WRITE:
 			if(s)s->iwrite = 1;
            break;
        
        case FD_CLOSE:
 			WriteStatusWindow(win,"Connection Closed");
            break;

         case FD_ACCEPT:       
 			if(s)s->iaccept = 0;
            sendData.rd = accept(wParam, NULL,NULL); 
            if (sendData.rd == INVALID_SOCKET) {
				Warning("accept error");
            }  
 			if(s)s->iaccept = 1;
           break;

             
        } 

        break;

    case WM_WSAGETHOST: 

        if ( (iRc = WSAGETASYNCERROR(lParam)) != 0) {
		   Warning("StatusWndProc Wrong hGetHostID");
		   if(s)s->gotname = -1;
            return 0;
        }    
		if(s)s->gotname=1;
		break;
		case WM_SIZE :		
			if(win){
				SizeIt(win);
			}			
        break;        
	    case WM_PAINT:
		hdc=BeginPaint(hWnd,&paint);
        EndPaint(hWnd,&paint);
		break;
		case WM_COMMAND:
		switch(LOWORD(wParam)){
 		   case BUTTON_NUMBER: 
			  if(win)win->hit=1;
		   break;
		}
		break;

		   default:
		   return (DefWindowProc(hWnd, message, wParam, lParam));
   }
   return 0;
}
int  Start(void)
{
 	extern int doNetWork;
   WSADATA wsadata;
	WORD wVer;
	int iRc; 

	SetTime(&xg.NetTime,1000);

	doNetWork=FALSE;

	wVer= MAKEWORD(1,1);
	iRc = WSAStartup(wVer,&wsadata);
	if (iRc != 0) {
		sprintf(WarningBuff,"Error WSAStartup (%d)",iRc);
		Warning(WarningBuff);
		return(0);
	}
	if (LOBYTE (wsadata.wVersion) != 1 ||
		HIBYTE (wsadata.wVersion) != 1 ) {
 		sprintf(WarningBuff,"Error Version (%d.%d)",
						LOBYTE (wsadata.wVersion),
						HIBYTE (wsadata.wVersion));
		Warning(WarningBuff);
		WSACleanup();
		return(0);
	} 

	doNetWork=TRUE;

	return 0;
}
BOOL CALLBACK DialogAbout(HWND hdWnd, UINT message, WPARAM wParam,
                                     LPARAM lParam)
{
	MEMORYSTATUS mem;
	char buff[256];
	

    switch (message) {
       case WM_INITDIALOG:
		   mem.dwLength=sizeof(mem);
		   GlobalMemoryStatus(&mem);
		   sprintf(buff,"Free Memory Available     : %d Bytes %c",(mem.dwAvailPhys),0);
		   SetDlgItemText(hdWnd,IDC_STATIC1,buff);
		   sprintf(buff,"Virtual Memory Available  : %d Bytes%c",(mem.dwAvailPageFile),0);
		   SetDlgItemText(hdWnd,IDC_STATIC2,buff);

          return 1;
       case WM_COMMAND:
		switch(LOWORD(wParam)){
		   case IDCANCEL:
		      EndDialog(hdWnd,0);
		      return 1;
		   case IDOK:
		      EndDialog(hdWnd,0);
		      return 1;
		}
         break;        
    }
    return 0;
}
char *ConvertPath(char *namein,int flag);

char *ConvertPath(char *namein,int flag)
{
	if(!namein)return NULL;
	
	if(flag){
	    cFree((char *)namein);
	}
	return NULL;
}
