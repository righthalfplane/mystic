static int  EditSetFolder(IconPtr myIcon)
{
 	struct FileData FileData;
	CEditPtr w;
	
	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w)return 1;
	
	
   

	if(SetDirectory(&FileData,"Put Output Data Here")){
	    return 1;
	}
	
	path[0]=0;
	
	if(PathNameFromWD(FileData.reply.vRefNum,path))return 1;
	
	strncpy(w->path,path,512);
	
	return 0;
}
static int  EditdoAuto(IconPtr myIcon)
{
	CEditPtr w;
	static int lflag[]={DialogInt,DialogInt,DialogInt};
	static char  **svalues;
	double din[3];
	int iin[3];
	long n;
	
	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w || !w->textH)return 1;
	
	
	
		
	iin[0]=w->FrameFirst;
	iin[1]=w->FrameLast;
	iin[2]=w->FrameIncrement;
Start:	
	if(GetDialog(6714,3,iin,svalues,din,iin,lflag))return 1;

	if(iin[0] < 0){
	    Warning("Frame Number Must Be Zero Or Greater");
	    goto Start;
	}
	if(iin[1] < iin[0]){
	    Warning("Final Frame Number Must Be Greater That First Frame Number");
	    goto Start;
	}
	if(iin[2] < 0){
	    Warning("Frame Increment Must Be Zero Or Greater");
	    goto Start;
	}
	
	w->FrameFirst=iin[0];
	w->FrameLast=iin[1];
	w->FrameIncrement=iin[2];
	
	for(n=w->FrameFirst;n<=w->FrameLast;n+=w->FrameIncrement){
	    w->CurrentFrame=n;
	    if(EditEvaluate(myIcon))break;
	    if(Stop("Stop Auto ?"))break;

	}
	
	return 0;
	
}
static int  EditSetFrame(IconPtr myIcon)
{
	CEditPtr w;
	static int lflag[]={DialogInt};
	static char  **svalues;
	double din[2];
	int iin[2];
	
	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w || !w->textH)return 1;
	
	
	
		
	iin[0]=w->CurrentFrame;
Start:	
	if(GetDialog(3775,1,iin,svalues,din,iin,lflag))return 1;

	if(iin[0] < 0){
	    Warning("Frame Number Must Be Zero Or Greater");
	    goto Start;
	}
	
	w->CurrentFrame=iin[0];
	
	return 0;
	
}
static int  EditEvaluate(IconPtr myIcon)
{
	CEditPtr w;
	CharsHandle text;
	unsigned char *dat;
	int c,n,length;
	int ret;
	
	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w || !w->textH)return 1;
	
	
	text=TEGetText(w->textH);
	if(!text){
		Warning("EditEvaluate Error Getting text pointer");
		return 1;
	}
	
	length=(*w->textH)->teLength;
	dat=(unsigned char *)cMalloc(length+1,8020);
	if(!dat){
		Warning("EditEvaluate Out Of Memory");
		return 1;
	}
	
	for(n=0;n<length;++n){
		c=(*text)[n];
		if(c == '\r')c='\n';
		dat[n]=c;
	}
	
	dat[length]=0;
	
	w->Express->CurrentFrame=w->CurrentFrame;
	
	strncpy(w->Express->path,w->path,512);
	
	ret=CExpressEvaluate(w->Express,dat);
	
	if(dat)cFree((char *)dat);
	
	return ret;
}
	
