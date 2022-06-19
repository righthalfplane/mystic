#define EXTERN22 extern
#include "Xappl.h"
#include "Render.h"
#include "uFiles.h"
#include "uDialog.h"
#include "Message.h"
#include "Vrml.h"
int rotate(struct System *Local);
CObjectListPtr duplicateObjectList(CObjectListPtr o);

int freeObjectList(CObjectListPtr o,int flag);

#define FRAME_INCREMENT 10

int FilesOpenDefault(struct FileInfo2 *Files);

int TrueWriteMerge(char *Name,unsigned char *bline,
			  unsigned char *bout,long xsize,
			  long ysize,FILE *out);
int Dump(struct Scene *scene,unsigned char *bline,
           unsigned char *bout,long xsize,long ysize,void *DumpData);
           
int TrueMergedDone(FILE *out);

int doSetImageInfo(char *Name,char *Directory,int flag);
           
int SetWorld(double s1,double s2,struct System *w,struct System *w1,struct System *w2);
int SetObjects(double s1,double s2,CObjectPtr o,CObjectPtr o1,CObjectPtr o2);
CObjectListPtr TweenKey(CObjectListPtr o1,CObjectListPtr o2,long m);
int SetKeyScene(struct Scene *scene,CObjectListPtr o);
CKeyFramePtr duplicateKeyFrames(CKeyFramePtr k);
int freeKeyFrames(CKeyFramePtr k,int flag);
static int checkKeyFrames(CKeyFramePtr k);
int AddKeyFrame(IconPtr myIcon);
int ReplaceKeyFrame(IconPtr myIcon);
CKeyFramePtr duplicateKeyFrames(CKeyFramePtr k);
int freeKeyFrames(CKeyFramePtr k,int flag);
int RenderScene(renderPtr openData);

int KeyFrames(IconPtr myIcon);
static int doKeyStart(struct uDialogStruct *uList);
static int upDatekeyList(CKeyFramePtr k,struct uDialogStruct *uList,int flag);
static int doKeyAction(struct uDialogStruct *uList);
static int SaveKeyData(struct uDialogStruct *uList);
static int deleteoKey(renderPtr r,struct uDialogStruct *uList);

static int doKeyFrameAction(struct uDialogStruct *uList);
int KeyFrameAnimation(IconPtr myIcon);

static int doKeyFrameAction(struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	IconPtr myIcon;	
	struct Scene *scene;
	CKeyFramePtr k;
	renderPtr r;
	uDialogReturn *uR;
	long count,n;
	int reti;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	k = &r->CKeyFrames;
	if(!k)return 1;
	
	ret = 1;
	uR=NULL;

	reti=uDialogSet(uList,1L,
          uDialogGetScrollList,&scroll,
          uDialogSetDone,uDialogSetDone          
    );
    
	if(reti) goto ErrorOut;
	
    uR=uDialogGetReturn(uList);
    if(!uR)goto ErrorOut;
	
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 1){		
		SaveKeyData(uList);	
	}else if(uList->actionItem == 11){
		SaveKeyData(uList);	
		if(r)RenderScene(r);
	}else if(uList->actionItem == 12){
		count=ScrollListCount(scroll);
		for(n=0;n<count;++n){
		    if(n < k->oCount){
			    if(ScrollListIsSelected(n,scroll)){
			        k->oCurrent[n]->Selected=TRUE;
			    }else{
			        k->oCurrent[n]->Selected=FALSE;
			    }
			}
		}
	}
	
	

	ret = 0;
	
ErrorOut:	
	if(uList)uDialogUpdate(uList);
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}

int KeyFrameAnimation(IconPtr myIcon)
{
	static uDialogGroup Items[] =

        {
      /*  0 */      {"Frames", {40,20,75,20},uDialogString,uDialogNone},
      /*  1 */      {"Frame List",{20,50,115,143},uDialogScrollList,uDialogNone},      
      /*  2 */      {"Frame Title", {180,50,80,20},uDialogString,uDialogNone},
      /*  3 */      {"0", {270,50,100,20},uDialogText,uDialogNone},
      /*  4 */      {"Frame Time", {180,80,80,20},uDialogString,uDialogNone},
      /*  5 */      {"0", {270,80,100,20},uDialogText,uDialogGetLong},
      /*  6 */      {"Root Name", {180,170,80,20},uDialogString,uDialogNone},
      /*  7 */      {"0", {270,170,100,20},uDialogText,uDialogNone},
      /*  8 */      {"Sequence", {180,200,80,20},uDialogString,uDialogNone},
      /*  9 */      {"0", {270,200,100,20},uDialogText,uDialogGetLong},
      /* 10 */      {"Save Images", {180,140,106,20},uDialogCheck,uDialogGetLong},
      /* 11 */      {"Render Frame", {20,210,110,20},uDialogButton,uDialogNone},
      /* 12 */      {"Ok", {230,270,70,20},uDialogButton,uDialogNone},
      /* 13 */      {"Cancel", {320,270,70,20},uDialogButton,uDialogNone},
      
      
      
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
 	static uRect  Rect1   = {100,50,410,310};

 	uDialogList *uList;
	uDialogReturn *uR;
	
	static char saveFileName[128]={'k','e','y','.','t','r','u',};
	char fileName[256],Directory[512],FileOut[1280];
	char buff[256];
	IconPtr StatusWindow;
	long n,nk,m,minc;
	int saveWhere;
	
 	struct Scene *scene;
	CKeyFramePtr k;
	renderPtr r;
    int reti;
    int ret;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	k=&r->CKeyFrames;

	if(!k || (k->oCount <= 0)){
	    Warning("KeyFrameAnimation No Key Frames To Animate");
	    return 1;
	}
	reti=1;
	
	uR=NULL;
	
	uList=NULL;
	
	r->saveWhere=0;
	
	StatusWindow=NULL;
	
	scene->itIs = -1;
	
	uList=uDialogOpen("Key Frame Animation",&Rect1,ItemCount);
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	ret=uDialogSet(uList,0L,	              
	              uDialogSetMyIcon,myIcon,	 
	              
	              uDialogSetItem,7L,
	              uDialogSetText,scene->sequenceName,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,(double)scene->sequence,
	              
	              uDialogSetItem,10L,
	              uDialogSetLogical,(scene->sequenceSave == 1),
	              
	              
	              uDialogSetAction,doKeyFrameAction,	
	                          
	              uDialogSetStart,doKeyStart,
	              uDialogSetDone,uDialogSetDone
	    );
	if(ret)goto ErrorOut;
	
	if(uDialog(uList))goto ErrorOut;
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	mstrncpy(scene->sequenceName,uR[7].sreturn,255);
	
	scene->sequence=uR[9].lreturn;
	
	scene->sequenceSave=uR[10].lreturn;	
	
	nk=0;
	for(n=0;n<k->oCount;++n){
	    if(k->oCurrent[n]->Selected)++nk;
	}
	if(nk <= 0){
	    Warning("No Key Frames Selected To Render");
	    RenderScene(r);
	    goto ErrorOut;
	}


    if(uList)uDialogClose(uList);
    uList=NULL;
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;


	if(scene->sequenceSave > 0){
	    saveWhere=doSetImageInfo(saveFileName,Directory,1);
	    if(saveWhere == 1)goto ErrorOut;
	}else{
	    saveWhere = 1;
	}


	StatusWindow=uStartStatusWindow(myIcon,"Saving Time Animation");
	if(StatusWindow == NULL){
		sprintf(WarningBuff,"StatusWindow NULL\n");
		WarningBatch(WarningBuff);
	}

	if(saveWhere == 3){
	    scene->FileOut=FileOut;
	}else if(saveWhere == 2 || saveWhere == 4){
	    scene->Dump=Dump;
	    scene->DumpData=(void *)myIcon;
	    r->saveWhere=saveWhere;
	    r->saveDirectory=Directory;
	    r->saveName=fileName;
	    r->saveFileName=saveFileName;
	}

	if(scene->MotionBlur > 1)scene->MotionBlurGather=1;

	for(n=0;n<k->oCount;++n){
	    CObjectListPtr o1,o,o2;
	    if(k->oCurrent[n]->Selected){
	        if(SetKeyScene(scene,k->oCurrent[n]))goto ErrorOut;
	        sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
	        sprintf(buff,"Creating: %s",fileName);
	        mstrncpy(FileOut,Directory,1280);
	        strncatToPath(FileOut,fileName,1280);
	        uWriteStatusWindow(StatusWindow,buff);
	        if(uCheckStatusWindow(StatusWindow,"Stop Animation ?"))goto ErrorOut;
	        RenderScene(r);
	        ++scene->sequence;
	        nk=n+1;
	        if(nk >= k->oCount)continue;
			o1=k->oCurrent[n];
			o2=k->oCurrent[nk];
			if(o1->KeyFrameTime > o2->KeyFrameTime){
			   minc = -1;
			}else{
			   minc = 1;
			}
	        for(m=o1->KeyFrameTime+minc;
	            (minc == 1) ? (m<o2->KeyFrameTime) : (m > o2->KeyFrameTime);
	                                    m += minc){
	            o=TweenKey(o1,o2,m);
	            if(!o)continue;
	            if(SetKeyScene(scene,o))goto ErrorOut;
	            sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
	            sprintf(buff,"Creating: %s",fileName);
	            mstrncpy(FileOut,Directory,1280);
	            strncatToPath(FileOut,fileName,1280);
	            uWriteStatusWindow(StatusWindow,buff);
	            if(uCheckStatusWindow(StatusWindow,"Stop Animation ?"))goto ErrorOut;
	            RenderScene(r);
	            ++scene->sequence;
	            if(o)freeObjectList(o,1);
	            {
	                long n;
	                o=scene->ObjectList;
	                if(o && o2 && (o->oCount == o2->oCount)){
	                    for(n=0;n<o->oCount;++n){
	                       if(o->oCurrent[n] && o2->oCurrent[n]){
	                           o2->oCurrent[n]->ItWas=o->oCurrent[n]->ItWas;
	                       }
	                    }
	                }
	            }
	        }
	    }
	}
	
	reti=0;
ErrorOut:	

	scene->MotionBlurGather=0;
	
	uStopStatusWindow(StatusWindow);
	if(r->saveWhere == 2){
	    if(r->saveFiles){
	       FilesOpenDefault(r->saveFiles);
	    }
	    r->saveFiles=NULL;
	} else if(r->saveWhere == 4){
	    if(r->saveFILE){
	       TrueMergedDone(r->saveFILE);
	       r->saveFILE=NULL;
	    }
	}
	scene->FileOut=NULL;
	scene->Dump=NULL;
	scene->DumpData=NULL;
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	if(r)RenderScene(r);
    
	return reti;
}
CObjectListPtr TweenKey(CObjectListPtr o1,CObjectListPtr o2,long m)
{
	struct Scene *scene,*scene1,*scene2;
	CObjectListPtr o;
	double s1,s2;
	long n;

	if(!o1 || !o2)return (CObjectListPtr)NULL;
	 if(o1->oCount != o2->oCount){
	     sprintf(WarningBuff,"TweenKey Time %ld KeyFrame Miss Match\n o1 count %ld o2 count %ld\n",
	                     m,o1->oCount,o2->oCount);
	     WarningBatch(WarningBuff);
	     return (CObjectListPtr)NULL;
	 }
	o=duplicateObjectList(o2);
	if(!o){
	    sprintf(WarningBuff,"TweenKey duplicateObjectList error \n");
		WarningBatch(WarningBuff);
	     return (CObjectListPtr)NULL;
	}
	scene1 = &o1->sceneKeyFrame;
	scene2 = &o2->sceneKeyFrame;
	scene = &o->sceneKeyFrame;
	o->KeyFrameTime=m;
	s2=((double)(m-o1->KeyFrameTime))/((double)(o2->KeyFrameTime-o1->KeyFrameTime));
	s1=1.0-s2;
	scene->Eye.x=s1*scene1->Eye.x+s2*scene2->Eye.x;
	scene->Eye.y=s1*scene1->Eye.y+s2*scene2->Eye.y;
	scene->Eye.z=s1*scene1->Eye.z+s2*scene2->Eye.z;
	scene->Target.x=s1*scene1->Target.x+s2*scene2->Target.x;
	scene->Target.y=s1*scene1->Target.y+s2*scene2->Target.y;
	scene->Target.z=s1*scene1->Target.z+s2*scene2->Target.z;
	scene->Up.x=s1*scene1->Up.x+s2*scene2->Up.x;
	scene->Up.y=s1*scene1->Up.y+s2*scene2->Up.y;
	scene->Up.z=s1*scene1->Up.z+s2*scene2->Up.z;
	scene->Degrees=s1*scene1->Degrees+s2*scene2->Degrees;
	SetWorld(s1,s2,&scene->World,&scene1->World,&scene2->World);
	for(n=0;n<o1->oCount;++n){
	    SetObjects(s1,s2,o->oCurrent[n],o1->oCurrent[n],o2->oCurrent[n]);
	}
	return o;
}
int SetObjects(double s1,double s2,CObjectPtr o,CObjectPtr o1,CObjectPtr o2)
{
	CGroupPtr g,g1,g2;
	long n;

	if(!o || !o1 || !o2)return 1;
	SetWorld(s1,s2,&o->Local,&o1->Local,&o2->Local);

	if((o->type != G_GROUP) || (o1->type != G_GROUP) ||
	   (o2->type != G_GROUP))return 0;
	g=(CGroupPtr)o;
	g1=(CGroupPtr)o1;
	g2=(CGroupPtr)o2;
	if(g->ObjectCount  != g1->ObjectCount)return 1;
	if(g1->ObjectCount != g2->ObjectCount)return 1;
	for(n=0;n<g->ObjectCount;++n){
	    SetObjects(s1,s2,g->Objects[n],g1->Objects[n],g2->Objects[n]);
	}
	return 0;
}
int SetWorld(double s1,double s2,struct System *w,struct System *w1,struct System *w2)
{
	if(!w || !w1 || !w2)return 1;
	w->x.x=s1*w1->x.x+s2*w2->x.x;
	w->x.y=s1*w1->x.y+s2*w2->x.y;
	w->x.z=s1*w1->x.z+s2*w2->x.z;
	w->y.x=s1*w1->y.x+s2*w2->y.x;
	w->y.y=s1*w1->y.y+s2*w2->y.y;
	w->y.z=s1*w1->y.z+s2*w2->y.z;
	w->z.x=s1*w1->z.x+s2*w2->z.x;
	w->z.y=s1*w1->z.y+s2*w2->z.y;
	w->z.z=s1*w1->z.z+s2*w2->z.z;
	w->p.x=s1*w1->p.x+s2*w2->p.x;
	w->p.y=s1*w1->p.y+s2*w2->p.y;
	w->p.z=s1*w1->p.z+s2*w2->p.z;
	w->Theta_x=s1*w1->Theta_x+s2*w2->Theta_x;
	w->Theta_y=s1*w1->Theta_y+s2*w2->Theta_y;
	w->Theta_z=s1*w1->Theta_z+s2*w2->Theta_z;
	w->Scale_x=s1*w1->Scale_x+s2*w2->Scale_x;
	w->Scale_y=s1*w1->Scale_y+s2*w2->Scale_y;
	w->Scale_z=s1*w1->Scale_z+s2*w2->Scale_z;
	rotate(w);
	return 0;
}
int SetKeyScene(struct Scene *scene,CObjectListPtr o)
{
	CObjectListPtr oo;

	if(!scene || !o)return 1;
	   oo=duplicateObjectList(o);
	   if(!oo){
	   		sprintf(WarningBuff,"SetKeyScene duplicateObjectList error \n");
			WarningBatch(WarningBuff);
	      	return 1;
	   }
	   freeObjectList(scene->ObjectList,1);
	   scene->ObjectList=oo;
	   scene->time=oo->KeyFrameTime;
	   scene->Eye=oo->sceneKeyFrame.Eye;
	   scene->Target=oo->sceneKeyFrame.Target;
	   scene->Up=oo->sceneKeyFrame.Up;
	   scene->Degrees=oo->sceneKeyFrame.Degrees;
	   scene->World=oo->sceneKeyFrame.World;
	return 0;
}
int freeKeyFrames(CKeyFramePtr k,int flag)
{
	long n;

	if(!k)return 1;
	
	if(k->oCurrent){
		for(n=0;n<k->oCount;++n){
		    CObjectListPtr Current;
		    if((Current=k->oCurrent[n]) == NULL)continue;
		    freeObjectList(Current,1);
		}
		cFree((char *)k->oCurrent);
		k->oCurrent=NULL;
	}
	if(flag)cFree((char *)k);
	return 0;
}
CKeyFramePtr duplicateKeyFrames(CKeyFramePtr k)
{
	CKeyFramePtr c;
	long n;

	if(!k)return k;
	c=(CKeyFramePtr)cMalloc(sizeof(CKeyFrame),8531);
	if(!c)return c;
	zerol((char *)c,sizeof(CKeyFrame));

	for(n=0;n<k->oCount;++n){
	    CObjectListPtr o;
	    CObjectListPtr Current;
	    if(checkKeyFrames(c))goto ErrorOut;
	    if((Current=k->oCurrent[n]) == NULL)continue;
	    o=duplicateObjectList(Current);
	    if(!o)goto ErrorOut;
	    c->oCurrent[c->oCount++]=o;
	}
	return c;
ErrorOut:
	if(c){
	    freeKeyFrames(c,1);
	}
	return (CKeyFramePtr)NULL;
}
int ReplaceKeyFrame(IconPtr myIcon)
{
	struct Scene *scene;
	CObjectListPtr o;
	CKeyFramePtr k;
	renderPtr r;
	char KeyFrameTitle[256];

	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;

	k = &r->CKeyFrames;
	if(k->KeyFrame >= k->oCount)k->KeyFrame = k->oCount-1;
	if(k->KeyFrame < 0)k->KeyFrame = 0;
	if(k->KeyFrame >= k->oMaximun)return 1;
	o=k->oCurrent[k->KeyFrame];
	if(o){
	    mstrncpy(KeyFrameTitle,o->KeyFrameTitle,255);
	}else{
	    mstrncpy(KeyFrameTitle,"",255);
	}
	freeObjectList(k->oCurrent[k->KeyFrame],1);
	k->oCurrent[k->KeyFrame]=NULL;
	o=duplicateObjectList(scene->ObjectList);
	if(!o)return 1;
	k->oCurrent[k->KeyFrame]=o;
	o->KeyFrameTime=scene->time;
	mstrncpy(o->KeyFrameTitle,KeyFrameTitle,255);
	o->sceneKeyFrame = *scene;
	return 0;
}
int AddKeyFrame(IconPtr myIcon)
{
	struct Scene *scene;
	CObjectListPtr o;
	CKeyFramePtr k;
	renderPtr r;

	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;

	k = &r->CKeyFrames;
	if(checkKeyFrames(k))return 1;
	o=duplicateObjectList(scene->ObjectList);
	if(!o)return 1;
	k->KeyFrame=k->oCount;
	k->oCurrent[k->oCount++]=o;
	o->KeyFrameTime=scene->time;
	o->sceneKeyFrame = *scene;
	o->KeyFrameTitle[0]=0;
	return 0;
}
static int checkKeyFrames(CKeyFramePtr k)
{
	CObjectListPtr *oCurrent;
	long oMaximun;

	if(!k)return 1;

	if(k->oCount+1 < k->oMaximun)return 0;

	oMaximun = k->oMaximun+FRAME_INCREMENT;

	oCurrent=NULL;
	if(k->oCurrent){
	    oCurrent=(CObjectListPtr *)cRealloc((char *)k->oCurrent,oMaximun*sizeof(CObjectListPtr),8777);
	    if(!oCurrent){
	        goto ErrorOut;
	    }
	    zerol((char *)&oCurrent[k->oMaximun],FRAME_INCREMENT*sizeof(CObjectListPtr));
	}else{
	    oCurrent=(CObjectListPtr *)cMalloc(oMaximun*sizeof(CObjectListPtr),8532);
	    if(!oCurrent){
	        goto ErrorOut;
	    }
	    zerol((char *)oCurrent,oMaximun*sizeof(CObjectListPtr));
	}

	k->oMaximun=oMaximun;
	k->oCurrent=oCurrent;

	return 0;
ErrorOut:
	sprintf(WarningBuff,"checkKeyFrames out of Memory\n");
	WarningBatch(WarningBuff);
	if(oCurrent){
	    k->oCurrent=oCurrent;
	}
	return 0;
}
static int deleteoKey(renderPtr r,struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	struct Scene *scene;
	long itWas,count;
	long oCount;
	CKeyFramePtr k;
	long n,add;
	int ret;

	if(!r || !uList)return 1;
	scene=r->scene;
	if(!scene)return 1;
	k = &r->CKeyFrames;

	ret=uDialogSet(uList,1L,
          uDialogGetScrollList,&scroll,
          uDialogSetDone,uDialogSetDone          
    );
    
	if(ret) return 1;
	itWas=ScrollListFirstSelect(scroll);
	if(itWas <0)return 0;

	count=ScrollListCount(scroll);

	oCount=k->oCount;
	if(oCount != count){
	    sprintf(WarningBuff,"deleteo oCount %ld != count %ld\n",oCount,count);
		WarningBatch(WarningBuff);
	    return 1;
	}

	add=0;
	for(n=0;n<count;++n){
	    CObjectListPtr Current;
	    if(ScrollListIsSelected(n,scroll)){
	        Current=k->oCurrent[n];
		if(Current){
		   freeObjectList(Current,1);
	           k->oCurrent[n]=NULL;
		}
	    }else{
	       k->oCurrent[add++]=k->oCurrent[n];
	    }
	}

	k->oCount=add;

	upDatekeyList(k,uList,0);

	scene->itIs = -1;
	
	return 0;
}
static int SaveKeyData(struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	struct Scene *scene;
	uDialogReturn *uR;
	char value[256];
	renderPtr r;
	IconPtr myIcon;
	CObjectListPtr o;
	CKeyFramePtr k;
	int reti;
    int ret;
    long n;
    
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	k = &r->CKeyFrames;
	if(!k)return 1;
		
	ret = 1;
	uR=NULL;
	
	reti=uDialogSet(uList,1L,
          uDialogGetScrollList,&scroll,
          uDialogSetDone,uDialogSetDone          
    );
    
	if(reti) goto ErrorOut;
	
    uR=uDialogGetReturn(uList);
    if(!uR)goto ErrorOut;
        	
	n=ScrollListFirstSelect(scroll);
	if(n >= 0){		
	    if(scene->itIs >= 0){
			o=k->oCurrent[scene->itIs];

			mstrncpy(o->KeyFrameTitle,uR[3].sreturn,255);
			o->KeyFrameTime=uR[5].lreturn;

			sprintf(value,"%s<%ld>",o->KeyFrameTitle,scene->itIs);
			ScrollListPut(value,scene->itIs,scroll);

	    }
		scene->itIs=n;
		
		o=k->oCurrent[scene->itIs];
			   		
		reti=uDialogSet(uList,1L,
			uDialogSetItem,3,
			uDialogSetText,o->KeyFrameTitle,
			uDialogSetItem,5,
			uDialogSetDouble,(double)o->KeyFrameTime,
			uDialogSetDone,uDialogSetDone          
		);

		if(reti) goto ErrorOut;	   		

		if(SetKeyScene(scene,o))goto ErrorOut;
		k->KeyFrame=scene->itIs;
	}
        	
        	
 		ret = 0;           
ErrorOut:	

    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int doKeyAction(struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	IconPtr myIcon;	
	struct Scene *scene;
	CKeyFramePtr k;
	renderPtr r;
	uDialogReturn *uR;
	int reti;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	k = &r->CKeyFrames;
	if(!k)return 1;
	
	ret = 1;
	uR=NULL;

	reti=uDialogSet(uList,1L,
          uDialogGetScrollList,&scroll,
          uDialogSetDone,uDialogSetDone          
    );
    
	if(reti) goto ErrorOut;
	
    uR=uDialogGetReturn(uList);
    if(!uR)goto ErrorOut;
	
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 1){		
		SaveKeyData(uList);	
	}else if(uList->actionItem == 6){
		SaveKeyData(uList);	
	    RenderScene(r);
	}else if(uList->actionItem == 7 || uList->actionItem == 10){
		SaveKeyData(uList);	
		reti=uDialogSet(uList,0L,
          uDialogSetExitOk,
          uDialogSetDone,uDialogSetDone
		);
		if(reti)goto ErrorOut;
	}else if(uList->actionItem == 9){
	    deleteoKey(r,uList);
	}
	

	ret = 0;
	
ErrorOut:	
	if(uList)uDialogUpdate(uList);
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}
static int upDatekeyList(CKeyFramePtr k,struct uDialogStruct *uList,int flag)
{
	struct ScrollList *scroll;
	CObjectListPtr o;
	long count,first,n;
	int ret;

	if(!k || !uList)return 1;

	ret=uDialogSet(uList,1L,
          uDialogGetScrollList,&scroll,
          uDialogSetDone,uDialogSetDone          
    );
    
	if(ret)return 1;
	
	if(!scroll)return 1;

	if(flag){
	    first=ScrollListFirstSelect(scroll);
	}else{
	    first = -1;
	}

 	count=ScrollListCount(scroll);
	for(n=0;n<count;++n){
            ScrollListSelect(n,scroll);
	}
	ScrollListDeleteSelected(scroll);
    for(n=0;n<k->oCount;++n){
		char buff[256];
		char *data;
		
		o=k->oCurrent[n];
		data = &buff[0];
		sprintf(buff,"%s<%ld>",o->KeyFrameTitle,n);
		if(ScrollListInsertOneType(&data,3,n,1L,scroll))return 1;
	}
	if(flag){
	    ScrollListSelect(first,scroll);
	}else{
	    ScrollListTop(0L,scroll);
	}
	return 0;
}
static int doKeyStart(struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	IconPtr myIcon;
	struct Scene *scene;
	renderPtr r;
	int itis;
	int ret;
	

	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	upDatekeyList(&r->CKeyFrames,uList,0);
		
	
	itis = 0;
	
	ret=uDialogSet(uList,1L,
              uDialogGetScrollList,&scroll,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	ScrollListDeSelect(scroll);
	ScrollListSelect(itis,scroll);
	
	/* SetKeyData(uList,itis); */

	scene->itIs = -1;
		
	return 0;
}
int KeyFrames(IconPtr myIcon)
{
	static uDialogGroup Items[] =

        {
    

      /*  0 */      {"Frames", {40,20,75,20},uDialogString,uDialogNone},
      /*  1 */      {"Frame List",{20,50,115,143},uDialogScrollList,uDialogNone},
      /*  2 */      {"Frame Title", {180,50,80,20},uDialogString,uDialogNone},
      /*  3 */      {"0", {260,50,100,20},uDialogText,uDialogNone},
      /*  4 */      {"Frame Time", {180,80,80,20},uDialogString,uDialogNone},
      /*  5 */      {"0", {260,80,100,20},uDialogText,uDialogGetLong},
      /*  6 */      {"Render Frame", {20,270,110,20},uDialogButton,uDialogNone},
      /*  7 */      {"Edit Frame",   {20,240,110,20},uDialogButton,uDialogNone},
      /*  8 */      {"Cancel", {290,320,70,20},uDialogButton,uDialogNone},
      /*  9 */      {"Delete Frame", {20,210,110,20},uDialogButton,uDialogNone},
      /* 10 */      {"Ok",  {190,320,70,20},uDialogButton,uDialogNone},

        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);

	static uRect  Rect1   = {100,50,380,360};
	
 	uDialogList *uList;
	uDialogReturn *uR;
 	
	CObjectListPtr o;
	struct Scene *scene;
	CKeyFramePtr k;
	renderPtr r;
    int reti;
    int ret;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	reti=1;
	
	uR=NULL;
	
	uList=NULL;
	
	scene->itIs = -1;
	
	k=duplicateKeyFrames(&r->CKeyFrames);
	if(!k){
	    sprintf(WarningBuff,"KeyFrames duplicateKeyFrames error \n");
		WarningBatch(WarningBuff);
	    return 1;
	}

	o=duplicateObjectList(scene->ObjectList);
	if(!o){
	    if(k)freeKeyFrames(k,1);
	    sprintf(WarningBuff,"KeyFrames duplicateObjectList error \n");
		WarningBatch(WarningBuff);
	    return 1;
	}

	o->KeyFrameTime=scene->time;
	o->sceneKeyFrame = *scene;

	uList=uDialogOpen("Key Frames",&Rect1,ItemCount);
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	ret=uDialogSet(uList,0L,
	              
	              uDialogSetMyIcon,myIcon,	              
	              uDialogSetAction,doKeyAction,	              
	              uDialogSetStart,doKeyStart,
	              uDialogSetDone,uDialogSetDone
	    );
	if(ret)goto ErrorOut;
	
	if(uDialog(uList))goto ErrorOut;
	
	reti=0;
ErrorOut:	

	if(reti){
	    if(k){
	        freeKeyFrames(&r->CKeyFrames,0);
	        r->CKeyFrames= *k;
	        cFree((char *)k);
	    }
	    if(o){
	       SetKeyScene(scene,o);
	       freeObjectList(o,1);
	   }
	}else{
	    if(k){
	        freeKeyFrames(k,1);
	    }
	    if(o){
	       	freeObjectList(o,1);
	    }
	}

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	RenderScene(r);
    
	return reti;
}



