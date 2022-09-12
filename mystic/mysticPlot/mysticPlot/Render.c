#define EXTERN22 extern
#include "Xappl.h"
#include "Render.h"
#include "uFiles.h"
#include "uDialog.h"
#include "Message.h"
#include "Vrml.h"
#include "Universal.h"
#include "uGraph.h"
#include "FileManager.h"
#include "Defs.h"
#include "VoiceCommand.h"

#define SUM(n)   (((n) & 1)  ? (((n)+2)*((n)/2)+1):(((n)+1)*((n)/2)))

int DoTracingExport(IconPtr myIcon);

int CleanTracing2(struct Scene *scene);

int CObjectReadFileList(IconPtr myIcon);

static int SetWindowView(struct uDialogStruct *uList,int type);       
static int doequal(struct Scene *scene);

static int doInFly(IconPtr myIcon);
int changePalette(struct Scene *scene,int mat);
static int setSceneMaterial(IconPtr myIcon,int mat);
static int doRaysButton(struct uDialogStruct *uList,long no);
static int n2SaveScrap(IconPtr myIcon);
static int drawTek2(IconPtr myIcon,long CurrentFrame,uRect *DRect);
static int RenderReadWrite(IconPtr myIcon,FILE8 *out,int flag);

static int doSetTarget(IconPtr myIcon,uPoint p1);

static int doWalk(IconPtr myIcon,int c);

static int AdjustEye(struct Scene *scene,double Length);

static int nMouseMoved(IconPtr myIcon,uPoint pt);

static int rSpeechCommand(IconPtr myIcon,char *command);

int setDefaultMaterial(struct Material *material);

int showPaletteLevels(struct Scene *scene);

static int pushStack(renderPtr r);
int CleanTracing(void);
int doRenderKeys(char *line,IconPtr myIcon);
static int doTime(IconPtr myIcon);
int MergeBlock(struct Scene *scene,struct Block *block);
int doSetImageInfo(char *Name,char *Directory,int flag);
int AddKeyFrame(IconPtr myIcon);
int ReplaceKeyFrame(IconPtr myIcon);
int freeKeyFrames(CKeyFramePtr k,int flag);
int KeyFrameAnimation(IconPtr myIcon);
int ListCountObjects(CObjectList *d,long *nodes,long *cells);
static int DrawAnimate(IconPtr myIcon,long CurrentFrame);

int centerScene(struct Scene *scene);
int doTestSine(void);
int doTestSpeed(IconPtr myIcon);
static int DoRayTracingLights(IconPtr myIcon);

int rangeE(struct Scene *scene,double *xmini,double *xmaxi,double *ymini,double *ymaxi,
          double *zmini,double *zmaxi,double *vmini,double *vmaxi);
int elementrange(struct Block *block);

int fixScene(renderPtr r);
int GetPlotOptions(IconPtr myIcon);
int KeyFrames(IconPtr myIcon);

static int doTimeAnimation(IconPtr myIcon);
static int Time1Action(struct uDialogStruct *uList);

static int doRotate(IconPtr myIcon);
static int Rotate1Action(struct uDialogStruct *uList);

static int doView(IconPtr myIcon);
static int doView1Action(struct uDialogStruct *uList);

static int doMaterials(IconPtr myIcon);
static int doMaterialStart(struct uDialogStruct *uList);
static int doMaterialAction(struct uDialogStruct *uList);
static int doLightsStart(struct uDialogStruct *uList);
static int doLightsAction(struct uDialogStruct *uList);
static int ClickLightList0(struct uDialogStruct *uList);
static int ClickMaterialList0(struct uDialogStruct *uList);
static int SaveLightData(struct uDialogStruct *uList,long n);
static int SetLightData(struct uDialogStruct *uList,long n);
static int SetMaterialData(struct uDialogStruct *uList,long n);
static int SaveMaterialData(struct uDialogStruct *uList,long n);

static int doScale(IconPtr myIcon,int mat,struct CObjectStruct *o);
static int doScale1Action(struct uDialogStruct *uList);

static int doMove(IconPtr myIcon);
static int doOptions(IconPtr myIcon);

int FilesOpenDefault(struct FileInfo2 *Files);

int TrueWriteMerge(char *Name,unsigned char *bline,
			  unsigned char *bout,long xsize,
			  long ysize,FILE *out);
int Dump(struct Scene *scene,unsigned char *bline,
           unsigned char *bout,long xsize,long ysize,void *DumpData);
           
int TrueMergedDone(FILE *out);

int FilesSaveTRUEToMemory(char *name,unsigned char *bline,
			  unsigned char *bout,long xsize,
			  long ysize,struct FileInfo2 *Files);
			  
static int doOrbit(IconPtr myIcon);
struct FileInfo2 *FilesTRUE(void);

static int CheckMenu(IconPtr myIcon,int flag);
static int doNULL(IconPtr myIcon);
int doEraseDown(IconPtr myIcon);

int Sleep2(int ticks);

int doRotateLocalDown(IconPtr myIcon);
int doZoomDown(IconPtr myIcon);
int doRotateGlobalDown(IconPtr myIcon);
static int doSetTargetDown(IconPtr myIcon);
int doDragDown(IconPtr myIcon);
int doDragDownGlobal(IconPtr myIcon);
int doSelect(IconPtr myIcon);
int findAngles(struct System *Local);
int Transform(struct P *p,struct Matrix *m);
void DrawCircle(uPoint p1,struct Scene *scene,int xshift,int yshift,
                IconPtr myIcon,struct System *World);

int ScrollListSelect(long itis,struct ScrollList *scroll);
int freeblocks(struct Scene *scene);
int freeScene(struct Scene *scene);
int ListObjectKill(CObjectList *d);
int uGetPaletteSize(long *xsizep,long *ysizep,struct paletteDraw *pd,IconPtr myIcon);
static void tMoveControls(IconPtr myIcon);
static void tMoveControls2(IconPtr myIcon);
static void doFrameImage(IconPtr myIcon);
static int drawTek(IconPtr myIcon,long CurrentFrame,uRect *DRect);

int RenderScene(renderPtr openData);
int saveRenderImage(renderPtr r,unsigned char *p1,long xsize,long ysize);
int ListObjectMessage(CObjectListPtr o,struct ObjectMessage *Message);

static int doObjects(IconPtr myIcon);
static int doObjectAction(struct uDialogStruct *uList);
static int SetObjectData(struct uDialogStruct *uList,long no);
static int SaveObjectData(struct uDialogStruct *uList,long no);
static int doObjectStart(struct uDialogStruct *uList);
static int ClickObjectList(struct uDialogStruct *uList);
static int upDateObjectList(struct Scene *scene,struct uDialogStruct *uList);
int PackObjectList(CObjectListPtr o);
static int groupObjects(struct Scene *scene,struct uDialogStruct *uList);
static int ungroupObjects(struct Scene *scene,struct uDialogStruct *uList);
static int moveObjects(struct Scene *scene,struct uDialogStruct *uList,
		  double xmove,double ymove,double zmove);
static int reset(struct Scene *scene,struct uDialogStruct *uList);
static int deleteo(struct Scene *scene,struct uDialogStruct *uList);
static int duplicate(struct Scene *scene,struct uDialogStruct *uList,int flag);
int rotate3d(struct System *Local);
static int doRangeObjects(struct uDialogStruct *uList,CObjectPtr o);
int ListAddObjectList(CObjectList *d,CObject *Object);
void CObjectCreate(struct Scene *scene,CObject *b);
CGroupPtr CGroupCreate(struct Scene *scene,long ObjectCount);
int FilesGetData(double **data,long *length,
                 long CurrentFrame,struct FileInfo2 *Files);
int oDumpit(renderPtr r);

int OpenViewLib(struct FileList *Files);

int OpenTek(struct FileList *Files);

int DefaultScene(struct Scene **scenei);

int getPaletteReal(struct Color *ct,int n);

int checkWorld(struct Scene *scene,CObjectListPtr o,int flag);
int ObjectListAddBlock(struct Scene *scene,long k,long mat);
int adjustPalette(struct Scene *scene,int mat);
int fillct(struct Color *ct,unsigned char *palette);

int ObjectListAddFiles(struct FileInfo2 *files,struct Scene *scene);
static void drawIt(IconPtr myIcon);

static void renderDoActivate(IconPtr myIcon,int flag);

#define File_Menu  				7000
#define About_Item 				1
#define Open_Item 				2
#define Close_Item 				3
#define savePreferencesCommand	4
#define PreferencesCommand 		5
#define Quit_Item 				6
#define New_Item 				7
#define Load_Item 				8
#define Save_Item 				9


#define EDIT_MENU				7001
#define copyTekCommand			1
#define FRAME_IMAGE_ITEM		2
#define popItem					3
#define popAllItems				4
#define zoomIn					5
#define zoomOut					6
#define WINDOW_OPTIONS_ITEM		7
#define OPTIONS_ITEM			8
#define VIEW_ITEM				9
#define OBJECT_ITEM				10
/*
#define SCALE_ITEM				11
*/
#define MATERIAL_ITEM			12
#define KEYFRAME_ITEM			13
#define ADD_KEYFRAME_ITEM		14
#define REPLACE_KEYFRAME_ITEM	15
#define RAY_TRACING_LIGHTS	    16
#define EXPORT_ITEM	    		17


#define OPERATIONS_MENU		7002
#define DRAG_ALL_ITEM		1
#define DRAG_ONE_ITEM		2
#define ROTATE_ALL_ITEM		3
#define ROTATE_ONE_ITEM		4
#define SET_TARGET_ITEM		5
#define SPIN_ALL_ITEM		6
#define ZOOM_ITEM			7
#define SELECT_ITEM			8
#define WALK_ITEM			9

#define ANIMATIONS_MENU				7003
#define ORBIT_ITEM					1
#define MOVE_TARGET_ITEM			2
#define ROTATE_OBJECT_ITEM			3
#define TIME_AMIMATE_ITEM			4
#define KEY_FRAME_AMIMATE_ITEM		5
#define FLIGHT__AMIMATE_ITEM		6
#define loopTek	 					8
#define loopBackTek	 				9
#define optionsTek	 				10
#define synchronizeTek	 			11

#define NEW_Menu  					48
#define NetWindow_Item				1
#define SendWindow_Item				2
#define DrawWindow_Item				3
#define ExpressionWindow_Item		4
#define TableWindow_Item			5
#define UtilityWindow_Item			6
#define RenderWindow_Item			7

#define PalMenu4	1908



static int doClose(IconPtr myIcon);
static int doMenus(IconPtr myIcon,int menu,int item);
static int doLoadImages(IconPtr myIcon);
static int UpdateWrite(IconPtr myIcon);

static int RenderGetMessage(IconPtr myIcon,long MessageType,void *MessageData);
static int UpdateWrite(IconPtr myIcon);

IconPtr  NewRenderWindow(IconPtr myIcon);

static int tMoveDy(IconPtr myIcon);
static int tgoDown(controlPtr control,short part,IconPtr myIcon);
static int tgoUp(controlPtr control,short part,IconPtr myIcon);
static int tdoPageDown(controlPtr control,short part,IconPtr myIcon);
static int tdoPageUp(controlPtr control,short part,IconPtr myIcon);
static int tdothumb(controlPtr control,IconPtr myIcon);

static int tInContent(IconPtr myIcon);

static int RenderKeyBoard(IconPtr myIcon,int c);


IconPtr openRender(struct Scene *scene);

int FilesOpenVRML(struct FileInfo2 *Files);

static int doWalkCommands(IconPtr myIcon,int c);


static struct VoiceCommands Commands[]=
{
	{"Animate Forward",c_Animate_Forward},
	{"Animate Backward",c_Animate_Backward},
	{"Animate Faster",c_Animate_Faster},
	{"Animate Slower",c_Animate_Slower},
	
	{"Zoom In",c_Zoom_In},
	{"Zoom Out",c_Zoom_Out},
	
	{"Move Up",c_Move_Up},
	{"Move Down",c_Move_Down},
	{"Move Left",c_Move_Left},
	{"Move Right",c_Move_Right},
	{"Move Faster",c_Move_Faster},
	{"Move Slower",c_Move_Slower},

	{"Move Forward",c_Move_Forward},
	{"Move Backward",c_Move_Backward},
		
	{"Step Forward",c_Step_Forward},
	{"Step Backward",c_Step_Backward},
	
	{"Stop",c_Stop},
	
	{"Continue",c_Start},
	
	{"Halt",c_Stop},
	{"Help",c_Help},
};
static long CommandLength=sizeof(Commands)/sizeof(struct VoiceCommands);

static int doCommands(IconPtr myIcon,int c);


static struct VoiceCommands CommandsW[]=

{
	{"Continue",c_Start},
	{"Do Again",c_Do_Again},
	{"Rotate Faster",c_Rotate_Faster},
	{"Rotate Slower",c_Rotate_Slower},
	{"Rotate Left",c_Rotate_Left},
	{"Rotate Right",c_Rotate_Right},
	{"Rotate Up",c_Rotate_Up},
	{"Rotate Down",c_Rotate_Down},
	{"Move Up",c_Move_Up},
	{"Move Faster",c_Move_Faster},
	{"Move Slower",c_Move_Slower},
	{"Move Forward",c_Move_Forward},
	{"Move Backward",c_Move_Backward},
	{"Move Right",c_Move_Right},
	{"Move Left",c_Move_Left},
	{"Move Down",c_Move_Down},
	{"Stop",c_Stop},
	{"Halt",c_Stop},
	{"Start Automatic Motion",c_Start},
	{"Cursor Turn On",c_Cursor_On},
	{"Cursor Turn Off",c_Cursor_Off},
	{"Cursor Move Up",c_Cursor_Up},
	{"Cursor Move Right",c_Cursor_Right},
	{"Cursor Move Left",c_Cursor_Left},
	{"Cursor Move Down",c_Cursor_Down},
	{"Cursor Move Faster",c_Cursor_Faster},
	{"Cursor Move Slower",c_Cursor_Slower},
	{"Cursor Select",c_Cursor_Select},
	{"Cursor Center",c_Cursor_Center},
	{"Cursor Quadrant One",c_Cursor_Quadrant_One},
	{"Cursor Quadrant Two",c_Cursor_Quadrant_Two},
	{"Cursor Quadrant Three",c_Cursor_Quadrant_Three},
	{"Cursor Quadrant Four",c_Cursor_Quadrant_Four},
	{"Help",c_Help},
};

static long CommandLengthW=sizeof(CommandsW)/sizeof(struct VoiceCommands);



static int drawWalkCursor(IconPtr myIcon);

static int iMoveTo(int x,int y,IconPtr myIcon);

static int iLineTo(int x,int y,IconPtr myIcon);

static int doCommands(IconPtr myIcon,int c)
{
	renderPtr r;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;	
		
	switch(c){	
	    case c_Animate_Forward:
	    	r->WalkMotion = 0;
			RenderKeyBoard(myIcon,rightArrow);
	    break;
	    case c_Animate_Backward:
	    	r->WalkMotion = 0;
			RenderKeyBoard(myIcon,leftArrow);
	    break;
	    case c_Animate_Faster:
	    	myIcon->framesPerSecond *= 2;
	    break;
	    case c_Animate_Slower:
	    	myIcon->framesPerSecond /= 2;
	    break;
	    case c_Step_Forward:
			RenderKeyBoard(myIcon,upArrow);
	    break;
	    case c_Step_Backward:
			RenderKeyBoard(myIcon,downArrow);
	    break;
	    case c_Zoom_In:
	        doMenus(myIcon,EDIT_MENU,zoomIn);
	    break;
	    case c_Zoom_Out:
	        doMenus(myIcon,EDIT_MENU,zoomOut);
	    break;
	    case c_Move_Left:
	    	r->WalkMotion = c_Move_Left;
	    	doWalkCommands(myIcon,c_Move_Left);
	    break;
	    case c_Move_Right:
	    	r->WalkMotion = c_Move_Right;
	    	doWalkCommands(myIcon,c_Move_Right);
	    break;
	    case c_Move_Up:
	    	r->WalkMotion = c_Move_Up;
	    	doWalkCommands(myIcon,c_Move_Up);
	    break;
	    case c_Move_Down:
	    	r->WalkMotion = c_Move_Down;
	    	doWalkCommands(myIcon,c_Move_Down);
	    break;
	    
	    case c_Move_Forward:
	    	r->WalkMotion = c_Move_Forward;
	    	doWalkCommands(myIcon,c_Move_Forward);
	    break;
	    
	    case c_Move_Backward:
	    	r->WalkMotion = c_Move_Backward;
	    	doWalkCommands(myIcon,c_Move_Backward);
	    break;
	    
	    case c_Move_Faster:
	    	doWalkCommands(myIcon,c_Move_Faster);
	    break;
	    
	    case c_Move_Slower:
	    	doWalkCommands(myIcon,c_Move_Slower);
	    break;
	    
	    case c_Start:
	    	r->WalkMotionContinuous = 1;	 
	   	break;   	
	    case c_Stop:
	    	r->WalkMotionContinuous = 0;	    	
	        r->AnimationOn = 0;
	    break;
	}
	return 0;
	
}
static int iMoveTo(int x,int y,IconPtr myIcon)
{
	renderPtr r;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;	

	uMoveTo(x,y);
	
	return 0;
}
static int iLineTo(int x,int y,IconPtr myIcon)
{
	renderPtr r;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;	

	uLineTo(x,y,myIcon);
	
	return 0;
}
static int drawWalkCursor(IconPtr myIcon)
{
	renderPtr r;
	int x,y;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;	
	
	if(!r->WalkCursorOn)return 1;
	
	x=r->WalkCursorX;
	y=r->WalkCursorY;
	
	iMoveTo(x,y-10,myIcon);
	iLineTo(x,y+10,myIcon);
	
	iMoveTo(x-10,y,myIcon);
	iLineTo(x+10,y,myIcon);
	
	iMoveTo(x-10,y-10,myIcon);
	iLineTo(x+10,y-10,myIcon);
	iLineTo(x+10,y+10,myIcon);
	iLineTo(x-10,y+10,myIcon);
	iLineTo(x-10,y-10,myIcon);
	
	
	
	return 0;
}

static int rSpeechCommand(IconPtr myIcon,char *command)
{
	renderPtr r;
	long n;
	
	if(!myIcon || !command)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;	
	
	sprintf(WarningBuff,"Voice Command = %s\n",command);
	WarningBatch(WarningBuff);
	
	if(!mstrcmp(command,"help")){
		if(r->Tool == ControlModeFly){
			for(n=0;n<CommandLengthW;++n){
				sprintf(WarningBuff,"Voice Commands[%ld] = %s\n",n,CommandsW[n].CommandText);
				WarningBatch(WarningBuff);
			}
		}else{
			for(n=0;n<CommandLength;++n){
				sprintf(WarningBuff,"Voice Commands[%ld] = %s\n",n,Commands[n].CommandText);
				WarningBatch(WarningBuff);
			}
		}
		return 0;
	}
	
	if(r->Tool == ControlModeFly){
		for(n=0;n<CommandLengthW;++n){
			if(!strcmp(command,CommandsW[n].CommandText)){
				doWalkCommands(myIcon,CommandsW[n].CommandIndex);	    
			}
		}
	}else{
		for(n=0;n<CommandLength;++n){
			if(!strcmp(command,Commands[n].CommandText)){
				doCommands(myIcon,Commands[n].CommandIndex);	    
			}
		}
	}
	
	return 0;
}

static int doWalkCommands(IconPtr myIcon,int c)
{
    struct P *flightPath;
    CFlightPathPtr s;
	CObjectListPtr o;
   	struct Scene *scene;
   	struct P diff,diff2,Up,Vx,Vy,Vz;
	double Length,WalkScale,WalkAngle,pi;
	struct P EyeToTarget;
	renderPtr r;
	long n;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;	
	scene=r->scene;
	if(!scene)return 1;	
			
	pushStack(r);
	
	diff=Sub(&scene->Target,&scene->Eye);
	Length=Len(&diff);
	
	WalkScale=scene->WalkScale;
	
	if(WalkScale <= 0){
	    WalkScale=Length;
		scene->WalkScale=WalkScale;
	}
	
	WalkAngle=scene->WalkAngle;
	
	if(WalkAngle <= 0){
	    WalkAngle=5;
		scene->WalkAngle=WalkAngle;
	}


	if(r->WalkCursorStep <= 0){
		r->WalkCursorStep=10;
	}
	
	pi=4.0*atan(1.0);
	WalkAngle *= pi/180;
	
	if(scene->WalkStepSize <= 0)scene->WalkStepSize=0.1;
		
	WalkScale *= scene->WalkStepSize;
	
	Up=Norm(&scene->Up);
	Vx=Norm(&scene->g.V1);
	Vy=Norm(&scene->g.V2);
	Vz=Norm(&scene->g.V3);
	
	
	s=NULL;
	o=scene->ObjectList;
	if(o){
	    for(n=0;n<o->oCount;++n){
			CObjectPtr Current;

	        if((Current=o->oCurrent[n]) == NULL)continue;
	        
	     	if(Current->type == G_FLIGHT_PATH){	     	    
	     	    if(r->WalkObject == (CFlightPathPtr)Current){
					s=r->WalkObject;
	     	        if(!s->flightPath || (r->WalkObjectCount < 0) || 
	     	           (r->WalkObjectCount >= s->flightCount)){
						s=NULL;	     	            
	     	        }
	     	    }	     	    
	     	} 
		}  
	
	}
	
	
	switch(c){	
	    case c_Delete_Object:
            if(s){	      
                long nn;
                flightPath=s->flightPath;
	   			if(flightPath){
	   			   nn=0;
				   for(n=0;n<s->flightCount;++n){
				       if(r->WalkObjectCount == n)continue;
				       s->flightPath[nn++]=flightPath[n];
				   }
				   s->flightCount=nn;
				   r->WalkObjectCount = -1;
				   
	   			}
            }
			pushpopPop(&r->stack,r->popStack);
	    break;
	    case c_Add_Object:
	        if(!s){
	            double len;
	        	s=CFlightPathCreate(scene,0,0,0);
	        	if(!s)goto ErrorOut;
			    if(ListAddObjectList(scene->ObjectList,(CObjectPtr)s)){
			        (*s->killObject)((CObjectPtr)s);
			        goto ErrorOut;
			    }    	
	   			s->flightPath=(struct P *)cMalloc(sizeof(struct P)*(s->flightCount+1),8568);
	   			if(s->flightPath){
	   				s->flightPath[0].x=scene->Target.x;
	   				s->flightPath[0].y=scene->Target.y;
	   				s->flightPath[0].z=scene->Target.z;
	   			}
	   			EyeToTarget=Sub(&scene->Target,&scene->Eye);
	   			len=Len(&EyeToTarget);
	   			if(len > 0){
	   			   s->radius=0.05*len;
	   			}
	   			s->flightCount=1;
	        }else if(s){	      
                long nn;
                flightPath=s->flightPath;
	   			s->flightPath=(struct P *)cMalloc(sizeof(struct P)*(s->flightCount+1),8568);
	   			if(s->flightPath){
	   			   nn=0;
				   for(n=0;n<s->flightCount;++n){
				       s->flightPath[nn++]=flightPath[n];
				       if(r->WalkObjectCount == n){
				       		s->flightPath[nn++]=flightPath[n];
				       }
				   }
				   
				   if(nn > s->flightCount){
				       r->WalkObjectCount++;
				   }else{
				   		s->flightPath[s->flightCount]=flightPath[s->flightCount-1];
				   		r->WalkObjectCount=s->flightCount;
				   }
				   cFree((char *)flightPath);
				   s->flightCount++;
				   
	   			}else{
	   			    s->flightPath=flightPath;
	   			}
            }
			pushpopPop(&r->stack,r->popStack);
	    break;
	    case c_Move_Object_Up:
            if(s){	         
		        s->flightPath[r->WalkObjectCount].z += WalkScale;
            }
			pushpopPop(&r->stack,r->popStack);
	    break;
	    case c_Move_Object_Down:
            if(s){	            
		        s->flightPath[r->WalkObjectCount].z -= WalkScale;
            }
			pushpopPop(&r->stack,r->popStack);
	    break;	    
	    case c_Move_Object_Forward:
            if(s){	            	
		        s->flightPath[r->WalkObjectCount].y += WalkScale;
            }
			pushpopPop(&r->stack,r->popStack);
	    break;	    
	    case c_Move_Object_Backward:
            if(s){	            
		        s->flightPath[r->WalkObjectCount].y -= WalkScale;
            }
			pushpopPop(&r->stack,r->popStack);
	    break;	    
	    case c_Move_Object_Left:
            if(s){	       
		        s->flightPath[r->WalkObjectCount].x += WalkScale;
           }
			pushpopPop(&r->stack,r->popStack);
	    break;
	    case c_Move_Object_Right:
            if(s){	            	
 		        s->flightPath[r->WalkObjectCount].x -= WalkScale;
            }
			pushpopPop(&r->stack,r->popStack);
	    break;
	    case c_Rotate_Faster:
			scene->WalkAngle *= 2;
			pushpopPop(&r->stack,r->popStack);
	    	return 0;
	    case c_Rotate_Slower:
			scene->WalkAngle /= 2;
			pushpopPop(&r->stack,r->popStack);
	    	return 0;
	    case c_Rotate_Left:
	    	r->WalkMotion = c_Rotate_Left;
        	scene->Target.x = scene->Eye.x-Length*(cos(WalkAngle)*Vz.x+sin(WalkAngle)*Vx.x);
        	scene->Target.y = scene->Eye.y-Length*(cos(WalkAngle)*Vz.y+sin(WalkAngle)*Vx.y);
        	scene->Target.z = scene->Eye.z-Length*(cos(WalkAngle)*Vz.z+sin(WalkAngle)*Vx.z);
	    break;
	    case c_Rotate_Right:
	    	r->WalkMotion = c_Rotate_Right;
        	scene->Target.x = scene->Eye.x-Length*(cos(-WalkAngle)*Vz.x+sin(-WalkAngle)*Vx.x);
        	scene->Target.y = scene->Eye.y-Length*(cos(-WalkAngle)*Vz.y+sin(-WalkAngle)*Vx.y);
        	scene->Target.z = scene->Eye.z-Length*(cos(-WalkAngle)*Vz.z+sin(-WalkAngle)*Vx.z);
	    break;
	    case c_Rotate_Up:
	    	r->WalkMotion = c_Rotate_Up;
        	scene->Target.x = scene->Eye.x-Length*(cos(-WalkAngle)*Vz.x+sin(-WalkAngle)*Vy.x);
        	scene->Target.y = scene->Eye.y-Length*(cos(-WalkAngle)*Vz.y+sin(-WalkAngle)*Vy.y);
        	scene->Target.z = scene->Eye.z-Length*(cos(-WalkAngle)*Vz.z+sin(-WalkAngle)*Vy.z);
	    break;
	    case c_Rotate_Down:
	    	r->WalkMotion = c_Rotate_Down;
        	scene->Target.x = scene->Eye.x-Length*(cos(WalkAngle)*Vz.x+sin(WalkAngle)*Vy.x);
        	scene->Target.y = scene->Eye.y-Length*(cos(WalkAngle)*Vz.y+sin(WalkAngle)*Vy.y);
        	scene->Target.z = scene->Eye.z-Length*(cos(WalkAngle)*Vz.z+sin(WalkAngle)*Vy.z);
	    break;
	    case c_Move_Faster:
			scene->WalkStepSize *= 2;
			pushpopPop(&r->stack,r->popStack);
	    	return 0;
	    case c_Move_Slower:
			scene->WalkStepSize /= 2;
			pushpopPop(&r->stack,r->popStack);
	    	return 0;
	    case c_Move_Forward:
	    	r->WalkMotion = c_Move_Forward;
        	scene->Eye.x -= Vz.x*WalkScale;
        	scene->Eye.y -= Vz.y*WalkScale;
        	scene->Eye.z -= Vz.z*WalkScale;
	        diff2=Sub(&scene->Target,&scene->Eye);
	        if((diff.x*diff2.x+diff.y*diff2.y+diff.z*diff2.z) < 0){	        
        		scene->Target.x = scene->Eye.x+diff.x;
        		scene->Target.y = scene->Eye.y+diff.y;
        		scene->Target.z = scene->Eye.z+diff.z;        	
	        }
	    break;
	    case c_Move_Backward:
	    	r->WalkMotion = c_Move_Backward;
	        scene->Eye.x += Vz.x*WalkScale;
	        scene->Eye.y += Vz.y*WalkScale;
	        scene->Eye.z += Vz.z*WalkScale;
	    break;
	    case c_Move_Right:
	    	r->WalkMotion = c_Move_Right;
	        scene->Eye.x += Vx.x*WalkScale;
	        scene->Eye.y += Vx.y*WalkScale;
	        scene->Eye.z += Vx.z*WalkScale;
	        AdjustEye(scene,Length);
	    break;
	    case c_Move_Left:
	    	r->WalkMotion = c_Move_Left;
	        scene->Eye.x -= Vx.x*WalkScale;
	        scene->Eye.y -= Vx.y*WalkScale;
	        scene->Eye.z -= Vx.z*WalkScale;
	        AdjustEye(scene,Length);
	    break;
	    case c_Move_Up:
	    	r->WalkMotion = c_Move_Up;
	        scene->Eye.x += Up.x*WalkScale;
	        scene->Eye.y += Up.y*WalkScale;
	        scene->Eye.z += Up.z*WalkScale;
        	AdjustEye(scene,Length);
	    break;
	    case c_Move_Down:
	    	r->WalkMotion = c_Move_Down;
	        scene->Eye.x -= Up.x*WalkScale;
	        scene->Eye.y -= Up.y*WalkScale;
	        scene->Eye.z -= Up.z*WalkScale;
	        AdjustEye(scene,Length);
	    break;
	    case c_Stop:
	    	r->WalkMotionContinuous = 0;	    	
			pushpopPop(&r->stack,r->popStack);
	    break;
	    case c_Start:
	    	r->WalkMotionContinuous = 1;	    	
			pushpopPop(&r->stack,r->popStack);
	    break;
	    
	    case c_Cursor_On:
	    	r->WalkCursorOn = 1;	    	
			pushpopPop(&r->stack,r->popStack);
	    break;
	    
	    case c_Cursor_Off:
	    	r->WalkCursorOn = 0;	    	
			pushpopPop(&r->stack,r->popStack);
	    break;
	    	    
	    case c_Cursor_Right:
	    	r->WalkMotion = c_Cursor_Right;
	    	r->WalkCursorOn = 1;	    	
	    	r->WalkCursorX += r->WalkCursorStep;	    	
			pushpopPop(&r->stack,r->popStack);
	    break;
	    	    
	    case c_Cursor_Left:
	    	r->WalkMotion = c_Cursor_Left;
	    	r->WalkCursorOn = 1;	    	
	    	r->WalkCursorX -= r->WalkCursorStep;	    	
			pushpopPop(&r->stack,r->popStack);
	    break;
	    	    
	    case c_Cursor_Up:
	    	r->WalkMotion = c_Cursor_Up;
	    	r->WalkCursorOn = 1;	    	
	    	r->WalkCursorY -= r->WalkCursorStep;	    	
			pushpopPop(&r->stack,r->popStack);
	    break;
	    case c_Cursor_Down:
	    	r->WalkMotion = c_Cursor_Down;
	    	r->WalkCursorOn = 1;	    	
	    	r->WalkCursorY += r->WalkCursorStep;	    	
			pushpopPop(&r->stack,r->popStack);
	    break;
	    
	    case c_Cursor_Center:
	    	r->WalkCursorOn = 1;	    	
	    	r->WalkCursorX = myIcon->uviewRect.x+myIcon->uviewRect.xsize/2;	    	
	    	r->WalkCursorY = myIcon->uviewRect.y+myIcon->uviewRect.ysize/2;	        	
			pushpopPop(&r->stack,r->popStack);
	    break;
	    
	    case c_Cursor_Quadrant_One:
	    	r->WalkCursorOn = 1;	    	
	    	r->WalkCursorX = myIcon->uviewRect.x+myIcon->uviewRect.xsize/4;	    	
	    	r->WalkCursorY = myIcon->uviewRect.y+(3*myIcon->uviewRect.ysize)/4;	        	
			pushpopPop(&r->stack,r->popStack);
	    break;
	    
	    case c_Cursor_Quadrant_Two:
	    	r->WalkCursorOn = 1;	    	
	    	r->WalkCursorX = myIcon->uviewRect.x+(3*myIcon->uviewRect.xsize)/4;	    	
	    	r->WalkCursorY = myIcon->uviewRect.y+(3*myIcon->uviewRect.ysize)/4;	        	
			pushpopPop(&r->stack,r->popStack);
	    break;
	    
	    case c_Cursor_Quadrant_Three:
	    	r->WalkCursorOn = 1;	    	
	    	r->WalkCursorX = myIcon->uviewRect.x+myIcon->uviewRect.xsize/4;	    	
	    	r->WalkCursorY = myIcon->uviewRect.y+myIcon->uviewRect.ysize/4;	        	
			pushpopPop(&r->stack,r->popStack);
	    break;
	    
	    case c_Cursor_Quadrant_Four:
	    	r->WalkCursorOn = 1;	    	
	    	r->WalkCursorX = myIcon->uviewRect.x+(3*myIcon->uviewRect.xsize)/4;	    	
	    	r->WalkCursorY = myIcon->uviewRect.y+myIcon->uviewRect.ysize/4;	        	
			pushpopPop(&r->stack,r->popStack);
	    break;
	    
	    case c_Cursor_Faster:
			r->WalkCursorStep *= 2;
			pushpopPop(&r->stack,r->popStack);
	    	return 0;
	    case c_Do_Again:
			pushpopPop(&r->stack,r->popStack);
			if(r->WalkMotion > 0)doWalkCommands(myIcon,r->WalkMotion);
	    	return 0;
	    		    
	    case c_Cursor_Slower:
			r->WalkCursorStep /= 2;
			if(r->WalkCursorStep <= 0)r->WalkCursorStep=1;
			pushpopPop(&r->stack,r->popStack);
	    	return 0;
	    
	    case c_Cursor_Select:
	        {
	        	uPoint p1;
	    		p1.x=r->WalkCursorX;
	    		p1.y=r->WalkCursorY;
				doSetTarget(myIcon,p1);
			}
	    	return 0;
	    	
	   default:
			pushpopPop(&r->stack,r->popStack);
	    	return 0;
	}
	
	scene->lamp.origin=scene->Eye;
	
	RenderScene(r);
	
ErrorOut:

	return 0;
}


static int AdjustEye(struct Scene *scene,double Length)
{
	struct P diff;
	
	if(!scene || Length <= 0)return 1;

	diff=Sub(&scene->Eye,&scene->Target);	
	diff=Norm(&diff);
	diff=Mult(&diff,Length);

	scene->Eye=Add(&scene->Target,&diff);
	
	return 0;
}

static int doInFly(IconPtr myIcon)
{
   	struct Scene *scene;
	CObjectListPtr o;
	renderPtr r;
	
	if(!myIcon || !(r=(renderPtr)myIcon->DWindow))return 1;
	scene=r->scene;
	if(!scene)return 1;	
	o=scene->ObjectList;
	if(!o)return 1;
	
	if(scene->WalkMode == Walk_Move){
		doSetTargetDown(myIcon);
	}else if(scene->WalkMode == Walk_Move_Object){
		doSelect(myIcon);
	}
	return 0;
}

static int doequal(struct Scene *scene)
{
	CObjectListPtr o;
	CFlightPathPtr s;
	double len,dl,loc;
	double *x,*y,*z;
	long np,k,n;
	    			
	if(!scene)return 1;
	
	x=NULL;
	y=NULL;
	z=NULL;
	o=scene->ObjectList;				
	s=NULL;
	if(o){
	    len=0;
	    for(n=0;n<o->oCount;++n){
			CObjectPtr Current;

	        if((Current=o->oCurrent[n]) == NULL)continue;
	        
	     	if(Current->type == G_FLIGHT_PATH){	     	    
	     	    s = (CFlightPathPtr)Current;
	     	    break;
	     	} 
		}  

	}

	if(!s || (s->flightCount < 3))return 0;
	
	len=0;
	for(k=0;k<s->flightCount-1;++k){
	    double dx,dy,dz;
	    dx=s->flightPath[k].x-s->flightPath[k+1].x;
	    dy=s->flightPath[k].y-s->flightPath[k+1].y;
	    dz=s->flightPath[k].z-s->flightPath[k+1].z;
	    len += sqrt(dx*dx+dy*dy+dz*dz);
	}		
	
	if(len <= 0)return 0;
	
	x=(double *)cMalloc(sizeof(double)*s->flightCount,9991);
	y=(double *)cMalloc(sizeof(double)*s->flightCount,9992);
	z=(double *)cMalloc(sizeof(double)*s->flightCount,9993);
	if(!x || !y || !z)goto ErrorOut;
	
	x[0]=s->flightPath[0].x;
	y[0]=s->flightPath[0].y;
	z[0]=s->flightPath[0].z;
	

	dl = len/(double)(s->flightCount-1);

	len=0;
	np=1;
	for(k=0;k<s->flightCount-1;++k){
	    double dx,dy,dz,ds,ss;
	    dx=s->flightPath[k+1].x-s->flightPath[k].x;
	    dy=s->flightPath[k+1].y-s->flightPath[k].y;
	    dz=s->flightPath[k+1].z-s->flightPath[k].z;
	    ds=sqrt(dx*dx+dy*dy+dz*dz);
Start:
	    if(ds <= 0)continue;
	    if((len+ds) >= dl){
	       loc=len+ds-dl;
	       ss=(ds-loc)/ds;
	       x[np]=s->flightPath[k].x+ss*dx;
	       y[np]=s->flightPath[k].y+ss*dy;
	       z[np]=s->flightPath[k].z+ss*dz;
	       ++np;
	       len=0;
	       ds=loc;
	       goto Start;
	    }else{
	       len += ds;	   
	    }
	}	
	
	for(k=1;k<s->flightCount-1;++k){
	    s->flightPath[k].x=x[k];
	    s->flightPath[k].y=y[k];
	    s->flightPath[k].z=z[k];
	}	
	
		
ErrorOut:
	if(x)cFree((char *)x);   
	if(y)cFree((char *)y);   
	if(z)cFree((char *)z);   
	return 0;
}

static int doWalk(IconPtr myIcon,int c)
{
   	struct Scene *scene;
	renderPtr r;
	long n,k;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;	
		
	switch(c){
	    case upArrow:
	    	if(scene->WalkMode == Walk_Move){
	    		doWalkCommands(myIcon,c_Move_Up);
	        }else if(scene->WalkMode == Walk_Rotate){
	    		doWalkCommands(myIcon,c_Rotate_Up);
	        }else if(scene->WalkMode == Walk_Move_Object){
	    		doWalkCommands(myIcon,c_Move_Object_Up);
	        }
	    	break;
	    case downArrow:
	    	if(scene->WalkMode == Walk_Move){
	    		doWalkCommands(myIcon,c_Move_Down);
	        }else if(scene->WalkMode == Walk_Rotate){
	    		doWalkCommands(myIcon,c_Rotate_Down);
	        }else if(scene->WalkMode == Walk_Move_Object){
	    		doWalkCommands(myIcon,c_Move_Object_Down);
	        }
	    	break;
	    case leftArrow:
	    	if(scene->WalkMode == Walk_Move){
	    		doWalkCommands(myIcon,c_Move_Left);
	        }else if(scene->WalkMode == Walk_Rotate){
	    		doWalkCommands(myIcon,c_Rotate_Left);
	        }else if(scene->WalkMode == Walk_Move_Object){
	    		doWalkCommands(myIcon,c_Move_Object_Left);
	        }
	    	break;
	    case rightArrow:
	    	if(scene->WalkMode == Walk_Move){
	    		doWalkCommands(myIcon,c_Move_Right);
	        }else if(scene->WalkMode == Walk_Rotate){
	    		doWalkCommands(myIcon,c_Rotate_Right);
	        }else if(scene->WalkMode == Walk_Move_Object){
	    		doWalkCommands(myIcon,c_Move_Object_Right);
	        }
	    	break;
	    case 'f':
	        if(scene->WalkMode == Walk_Move_Object){
	    		doWalkCommands(myIcon,c_Move_Object_Forward);
	        }else{
			    scene->WalkMode = Walk_Move;
	    	    doWalkCommands(myIcon,c_Move_Forward);
	    	}
	    	break;
	    case 'b':
	        if(scene->WalkMode == Walk_Move_Object){
	    		doWalkCommands(myIcon,c_Move_Object_Backward);
	        }else{
			    scene->WalkMode = Walk_Move;
	    	    doWalkCommands(myIcon,c_Move_Backward);
	    	}
	    	break;
	    case 'a':
	    	doWalkCommands(myIcon,c_Add_Object);
	    	break;
	    case 'd':
	        if(scene->WalkMode == Walk_Move_Object){
	    		doWalkCommands(myIcon,c_Delete_Object);
	    	}
	    	break;
	    case 's':
	    	doWalkCommands(myIcon,c_Stop);
	    	break;
	    case 'c':
	    	doWalkCommands(myIcon,c_Start);
	    	break;
	    case 'h':
			WarningBatchFast("Walk Mode Key Board");
			WarningBatchFast("   h: help");
			WarningBatchFast("   s: Stop");
			WarningBatchFast("   a: Add Object");
			WarningBatchFast("   c: Continue - Auto Repeat Last Move Or Rotate");
			WarningBatchFast("   e: Uniformly Space the Control Points");
			WarningBatchFast("   .: Write Eye and Target Locations");
			WarningBatchFast("          ");
			WarningBatchFast("   r: Rotate Mode");
			WarningBatchFast("               +: Rotate Faster");
			WarningBatchFast("               -: Rotate Slower");
			WarningBatchFast("               f: Move Forward");
			WarningBatchFast("               b: Move Backward");
			WarningBatchFast("        Up Arrow: Rotate Up");
			WarningBatchFast("      Down Arrow: Rotate Down");
			WarningBatchFast("      Left Arrow: Rotate Left");
			WarningBatchFast("     Right Arrow: Rotate Right");
			WarningBatchFast("          ");
			WarningBatchFast("   w: Walk Mode");
			WarningBatchFast("               +: Move Faster");
			WarningBatchFast("               -: Move Slower");
			WarningBatchFast("               f: Move Forward");
			WarningBatchFast("               b: Move Backward");
			WarningBatchFast("               ?: Print Walk Information");
			WarningBatchFast("        Up Arrow: Move Up");
			WarningBatchFast("      Down Arrow: Move Down");
			WarningBatchFast("      Left Arrow: Move Left");
			WarningBatchFast("     Right Arrow: Move Right");
			WarningBatchFast("      Mouse Down: Select Target");
			WarningBatchFast("          ");
			WarningBatchFast("   m: Move Control Points Mode");
			WarningBatchFast("               +: Move Object Faster");
			WarningBatchFast("               -: Move Object Slower");
			WarningBatchFast("               f: Move Object Forward");
			WarningBatchFast("               b: Move Object Backward");
			WarningBatchFast("               d: Delete Object");
			WarningBatchFast("               ?: Print Control Points");
			WarningBatchFast("        Up Arrow: Move Object Up");
			WarningBatchFast("      Down Arrow: Move Object Down");
			WarningBatchFast("      Left Arrow: Move Object Left");
			WarningBatchFast("     Right Arrow: Move Object Right");
			WarningBatchFast("      Mouse Down: Select Object");
			WarningBatchAtEnd(WarningBuff);        
	    	break;
	    case '+':
	    	if(scene->WalkMode == Walk_Move || scene->WalkMode == Walk_Move_Object){
	    		doWalkCommands(myIcon,c_Move_Faster);
	        }else{
	    		doWalkCommands(myIcon,c_Rotate_Faster);
	        }
	    	return 0;
	    case '-':
	    	if(scene->WalkMode == Walk_Move || scene->WalkMode == Walk_Move_Object){
	    		doWalkCommands(myIcon,c_Move_Slower);
	        }else{
	    		doWalkCommands(myIcon,c_Rotate_Slower);
	        }
	    	return 0;
	    case 'r':
			scene->WalkMode = Walk_Rotate;
	    	return 0;
	    case 'w':
			scene->WalkMode = Walk_Move;
	    	return 0;
	    case 'm':
			scene->WalkMode = Walk_Move_Object;
	    	return 0;
	    case '.':
			
				
				sprintf(WarningBuff,"Step %04ld Eye %g %g %g Target %g %g %g Degrees %g",
				             0L,scene->Eye.x,scene->Eye.y,scene->Eye.z,
				             scene->Target.x,scene->Target.y,scene->Target.z,
				             scene->Degrees);
			    WarningBatchFast(WarningBuff);
				     
				WarningBatchAtEnd(WarningBuff);        
			
	    	return 0;
	    case 'e':
	        doequal(scene);
			RenderScene(r);
	    	return 0;
	    case '?':
			{
				long count;
				
				if(scene->WalkMode != Walk_Move_Object){
					count=pushpopDepth(r->popStack);
					for(n=0;n<count;++n){
						pushpopPeek(&r->stack,n,r->popStack);
						sprintf(WarningBuff,"Step %04ld Eye %g %g %g Target %g %g %g Degrees %g",
					             0L,r->stack.Eye.x,r->stack.Eye.y,r->stack.Eye.z,
					             r->stack.Target.x,r->stack.Target.y,r->stack.Target.z,
					             r->stack.Degrees);
					    WarningBatchFast(WarningBuff);
					     
					}
				}else{
					CObjectListPtr o;
	    			CFlightPathPtr s;
				
					o=scene->ObjectList;				
					if(o){
					    for(n=0;n<o->oCount;++n){
							CObjectPtr Current;

					        if((Current=o->oCurrent[n]) == NULL)continue;
					        
					     	if(Current->type == G_FLIGHT_PATH){	     	    
					     	    s = (CFlightPathPtr)Current;
								sprintf(WarningBuff,"SplineSteps %ld",s->SplineSteps);
							    WarningBatchFast(WarningBuff);
								sprintf(WarningBuff,"PassSmooth %ld",s->PassSmooth);
	    						WarningBatchFast(WarningBuff);
								sprintf(WarningBuff,"CyclicSmooth %ld",s->CyclicSmooth);
	    						WarningBatchFast(WarningBuff);
								sprintf(WarningBuff,"tilt %g",scene->tilt);
	    						WarningBatchFast(WarningBuff);
							    for(k=0;k<s->flightCount;++k){
									sprintf(WarningBuff,"Step %04ld eye %g %g %g",
							             0L,s->flightPath[k].x,s->flightPath[k].y,s->flightPath[k].z);
							    	WarningBatchFast(WarningBuff);
							    }
					     	} 
						}  
					
					}
				
				}   
				WarningBatchAtEnd(WarningBuff);     
			}
	    	return 0;
	   default:
	    	return 0;
	}
	
	return 0;
}

int FilesOpenVRML(struct FileInfo2 *Files)
{
	IconPtr myIcon;
	renderPtr r;
	int ret,iret;
		
	if(!Files)return 1;
	
	ret=1;
	
	myIcon=NewRenderWindow(NULL);
	if(!myIcon)goto ErrorOut;

	r=(renderPtr)myIcon->DWindow;
	if(!r)goto ErrorOut;

    if(Files->fileClass == SDS2D_FILES ||
       Files->fileClass == HDF_FILES   ||
       Files->fileClass == VRML_FILES  ||
       Files->fileClass == SDS3D_FILES ||
       Files->fileClass == SHOW_FILES  ||
       Files->fileClass == STL_FILES
       ){       
       
        iret=ObjectListAddFiles(Files,r->scene);
	    if(iret){
	        if(iret != 7)goto ErrorOut;
	    }else{
	        doObjects(myIcon);
	    }
	    		
	}

	if(r && r->scene){
		if(r->scene->AutoCenterScreen){
			RenderSceneSetTime(r->scene);
			r->scene->AutoCenterScreenDone=FALSE;
		}
	}
	
	if(r)RenderScene(r);

	ret=0;
ErrorOut:
	if(ret){
	    doClose(myIcon);
	}
	return ret;
}

IconPtr openRender(struct Scene *scene)
{
	IconPtr myIcon;
	renderPtr r;
	int ret;
	
	ret=1;
	
	myIcon=NewRenderWindow(NULL);
	if(!myIcon)goto ErrorOut;
	
	
	r=(renderPtr)myIcon->DWindow;
	if(!r)goto ErrorOut;
	
	if(r->scene){
	    freeScene(r->scene);
		r->scene=NULL;
	}
	if(DefaultScene(&scene))goto ErrorOut;
	if(ScreenSetSize(scene->xResolution,scene->yResolution,myIcon))goto ErrorOut;
	r->scene=scene;

	ret=0;
ErrorOut:
	if(ret){
	    doClose(myIcon);
	    myIcon=NULL;
	}
	
	return myIcon;
	
}

IconPtr  NewRenderWindow(IconPtr myIconIn)
{
	static struct uMenuItems File[]={
	    {"File",  		  File_Menu,uWindowStandardMenu,0,},
	    {"New             /N",New_Item,0,0,},
	    {"Close           /W",Close_Item,0,0,},
	    {"-----------------",About_Item,0,0,},
	    {"Save Preferences ",savePreferencesCommand,0,0,},
	    {"Preferences...   ",PreferencesCommand,0,0,},
	    {"-----------------",About_Item,0,0,},
	    {"Save Images...   ",Save_Item,0,0,},
	    {"Load Images... /L",Open_Item,0,0,},
	    {"Load Objects...  ",Load_Item,0,0,},
	    {"-----------------",About_Item,0,0,},
	    {"Quit           /Q",Quit_Item,0,0,},
	    {NULL,0,0,0,},
	};
	

	static struct uMenuItems Edit[]={
	    {"Edit",  					7001,uWindowStandardMenu,0,},
	    {"Copy               /C",   copyTekCommand,0,0,},
	    {"---------------------",About_Item,0,0,},
	    {"Frame Image        /F",FRAME_IMAGE_ITEM,0,0,},
	    {"Pop One              ",popItem,0,0,},
	    {"Pop All              ",popAllItems,0,0,},
	    {"Zoom In              ",zoomIn,0,0,},
	    {"Zoom Out             ",zoomOut,0,0,},
	    {"---------------------",About_Item,0,0,},
	    {"Information...     /I",	WINDOW_OPTIONS_ITEM,0,0,},
	    {"Options...",				OPTIONS_ITEM,0,0,},
	    {"View...",					VIEW_ITEM,0,0,},
	    {"Objects...",				OBJECT_ITEM,0,0,},
/*
	    {"Data Range...",				SCALE_ITEM,0,0,},
*/
	    {"Materials...",			MATERIAL_ITEM,0,0,},
	    {"Key Frames...",			KEYFRAME_ITEM,0,0,},
	    {"Add KeyFrame       /A",	ADD_KEYFRAME_ITEM,0,0,},
	    {"Replace KeyFrame   /R",	REPLACE_KEYFRAME_ITEM,0,0,},
	    {"Ray Tracing Lights...",	RAY_TRACING_LIGHTS,0,0,},	    
	    {"Export...",				EXPORT_ITEM,0,0,},	    
	    {NULL,0,0,0,},
	};
	
	static struct uMenuItems Operations[]={
	    {"Tools",  			OPERATIONS_MENU,uWindowStandardMenu,0,},
	    {"Drag All", 		DRAG_ALL_ITEM,0,0,},
	    {"Drag One",		DRAG_ONE_ITEM,0,0,},
	    {"Rotate All",		ROTATE_ALL_ITEM,0,0,},
	    {"Rotate One",		ROTATE_ONE_ITEM,0,0,},
	    
	    {"Set Target", 		SET_TARGET_ITEM,0,0,},
	    {"Spin All",		SPIN_ALL_ITEM,0,0,},
	    {"Zoom",			ZOOM_ITEM,0,0,},
	    {"Find Point",		SELECT_ITEM,0,0,},
	    {"Walk",			WALK_ITEM,0,0,},
	    {NULL,0,0,0,},
	};

	static struct uMenuItems Animation[]={
	    {"Animation",  			    ANIMATIONS_MENU,uWindowStandardMenu,0,},
	    {"Orbit Target...", 		ORBIT_ITEM,0,0,},
	    {"Move Target...",			MOVE_TARGET_ITEM,0,0,},
	    {"Rotate Objects...",		ROTATE_OBJECT_ITEM,0,0,},
	    {"Time Animation...",		TIME_AMIMATE_ITEM,0,0,},	    
	    {"KeyFrame Animation...", 	KEY_FRAME_AMIMATE_ITEM,0,0,},
	    {"Flight Animation...", 	FLIGHT__AMIMATE_ITEM,0,0,},
	    {"----------------",		7,0,0,},
	    {"Loop",					loopTek,0,0,},
	    {"Loop Back and Forth",		loopBackTek,0,0,},
	    {"Animation Options...",	optionsTek,0,0,},
	    {"Synchronize",				synchronizeTek,0,0,},
	    {NULL,0,0,0,},
	};

	static struct uMenuItems Palette[]={
	    {"Palette",  				    PalMenu4,uWindowPaletteMenu,0,},
	    {"Load Palette...", 			1,0,0,},
	    {"Save Palette...", 			2,0,0,},
	    {"Add Palette...", 				3,0,0,},
	    {"Restore Palette   /R", 		4,0,0,},
	    {"Load Menu Palette", 			5,0,0,},
	    {"Synchronize Palettes", 		6,0,0,},
	    {"Palette To Table", 		    7,0,0,},
	    {"--------------------", 		8,0,0,},
	    { NULL,0,0,0},

	};
	
	static struct uMenuList menuList[]={
	    {File},
	    {Edit},
	    {Operations},
	    {Animation},
	    {Palette},
	    {NULL}
	};
	
	IconPtr myIcon;
	uRect Rect={40,40,400,200};
	char buff[256];
	static int Count;
	struct Scene *scene;
	renderPtr r;
	int ret;
		
    ret=1;
    r=NULL;
    
	if(!(myIcon=uCreateIcon(xRenderType,&Rect))){
		WarningBatch("doNewRenderWindow uCreateIcon Failed\n");
		goto ErrorOut;
	}
	uSetCursor(myIcon,uWATCH_CURSOR);
	
	
	sprintf(buff,"RenderWindow.%03d",Count++);
	
	SetWindowName(myIcon,buff);			
	if(uSetWindow(myIcon,	       
	              uWindowSetWindowName,buff,
				  uWindowSetMenuList,menuList,
	              uWindowSetDone,uWindowSetDone)
				  )goto ErrorOut;
				  
				  
	if(!(r=(renderPtr)cMalloc((long)sizeof(struct renderInfo),8202))){
	    WarningBatch("doNewRenderWindow Out of Memory\n");
	    goto ErrorOut;
	}
	zerol((char *)r,(long)sizeof(struct renderInfo));
	
	myIcon->DWindow=(struct DumWindow *)r;
	
	myIcon->pInContent=tInContent;
	myIcon->pCheckMenu=CheckMenu;
	myIcon->pClose=doClose;
	myIcon->pMenu=doMenus;
	myIcon->pMessage=RenderGetMessage;
	myIcon->pMoveControls=tMoveControls2;
	myIcon->pUpdateWrite=UpdateWrite;
	myIcon->pNullEvent=doNULL;
	myIcon->pKeyBoard=RenderKeyBoard;
	myIcon->uDraw=drawTek2;
	myIcon->uDrawAnimate=DrawAnimate;
	myIcon->pMouseMoved=nMouseMoved;
	myIcon->uSpeech=rSpeechCommand;	
	myIcon->pDoActivate=renderDoActivate;
	
	myIcon->pPageUp=tdoPageUp;
	myIcon->pPageDown=tdoPageDown;
	myIcon->pLineUp=tgoUp;
	myIcon->pLineDown=tgoDown;
	myIcon->pThumb=tdothumb;
	
	

	
	myIcon->paletteMenuNumber=PalMenu4;
	
	myIcon->mMenuOffSet=MenuOffSet;
		
	myIcon->eraseRectSize=120;
	
	myIcon->framesPerSecond=30;
	r->milliSecondsPerFrame=(long int)(1000./myIcon->framesPerSecond);

	r->TimeToFlip = TickCount();
	r->WalkCursorX = 50;
	r->WalkCursorY = 50;

	r->popStack=pushpopStart(&r->stack);
	if(!r->popStack)goto ErrorOut;		
				  
	scene=NULL;
	
	if(DefaultScene(&scene))goto ErrorOut;
	if(ScreenSetSize(scene->xResolution,scene->yResolution,myIcon))goto ErrorOut;
	r->scene=scene;
	
	r->myIcon=myIcon;
	r->Tool=ControlModeSpinGlobal;
	
	r->mat=1;
	
	SetWindowPath(myIconIn,myIcon);
	
    if(!IsShift()){
    	doUniversalRender(myIcon,universal_GetDefaults,NULL);
	    FlushCurrentPort();
    }
	
	
	doFrameImage(myIcon);

    StandardDoActive(myIcon,TRUE);

	CheckMenu(myIcon,TRUE);
	    
	GetPaletteByName("Ps",myIcon->red,myIcon->green,myIcon->blue);
	SetDefaultPaletteName(myIcon,"Ps");
		
    ret=0;
ErrorOut:
    if(ret){
        if(myIcon)doClose(myIcon);
	}else{
		if(myIcon)myIcon->isOpen=TRUE;
		if(r->Tool == ControlModeFly)doMenus(myIcon,OPERATIONS_MENU,WALK_ITEM);
	}

	uSetCursor(myIcon,uARROW_CURSOR);
	return myIcon;
}
static void renderDoActivate(IconPtr myIcon,int flag)
{
	renderPtr t;
	
	if(!myIcon)return;
	t=(renderPtr)myIcon->DWindow;
	if(!t)return;

	StandardDoActive(myIcon,flag);
	
	if(flag){
	    if(t->Tool == ControlModeFly){
			stopLanguage();
			startLanguage(CommandsW,CommandLengthW,myIcon);
		}else{
			stopLanguage();
			startLanguage(Commands,CommandLength,myIcon);
		}
	}else{
		stopLanguage();
	}

}
static int nMouseMoved(IconPtr myIcon,uPoint pt)
{
	renderPtr t;

	if(!myIcon)return 1;
	t=(renderPtr)myIcon->DWindow;
	if(!t)return 1;
	
	if(uPtInRect(pt,&myIcon->uviewRect)) {		
	    uSetCursor(myIcon,uARROW_CURSOR);
	}else{
	    uSetCursor(myIcon,uARROW_CURSOR);
	}		
	
	return 0;	
}
static int RenderGetFrameCount(IconPtr myIcon)
{
	struct Message1000 m;
   	struct Scene *scene;
	renderPtr r;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	
	m.FrameCount=0;
	
	r->Files.ImageCount=1;
		
	SendMessageByIcon(myIcon,MessageType_GET_FRAME_COUNT,&m);
	
	if(!m.FrameCount){
		return 0;
	}
	
	r->ImageCount=m.FrameCount;
	
	r->Files.ImageCount=m.FrameCount;
	
	if(r->CurrentFrame >= r->ImageCount)r->CurrentFrame=r->ImageCount-1;
	
	if(r->CurrentFrame < 0)r->CurrentFrame=0;
	
	if(scene)scene->time=r->CurrentFrame;	        

	return 0;
}



static int RenderKeyBoard(IconPtr myIcon,int c)
{
	static char line[256],buff[256];
	static int lineCount=0;
   	struct Scene *scene;
	renderPtr r;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	
	if(r->Tool == ControlModeFly){
	    return doWalk(myIcon,c);
	}

	scene=r->scene;

	if(c == upArrow     || c == downArrow ||
	   c == leftArrow   ||  c == rightArrow ){
	    if(!scene)return 1;
		RenderGetFrameCount(myIcon);
	}
	
	switch(c){
	    case upArrow:
	        if(++(r->CurrentFrame) >= r->ImageCount)r->CurrentFrame = 0;
	        r->AnimationOn = 0;
	   		scene->time=r->CurrentFrame;	        
	    	if(scene->AutoCenterScreen){
	    	   	RenderSceneSetTime(scene);
	    		scene->AutoCenterScreenDone=FALSE;
	    	}
	        RenderScene(r);
       		if(myIcon->Synchronize)Synchronize(myIcon,r->CurrentFrame);
	    	return 0;
	    case downArrow:
	        if(--(r->CurrentFrame) < 0)r->CurrentFrame = r->ImageCount-1;
	        r->AnimationOn = 0;
	   		scene->time=r->CurrentFrame;	        
	    	if(scene->AutoCenterScreen){
	    	   	RenderSceneSetTime(scene);
	    		scene->AutoCenterScreenDone=FALSE;
	    	}
	        RenderScene(r);
       		if(myIcon->Synchronize)Synchronize(myIcon,r->CurrentFrame);
	    	return 0;
	    case leftArrow:
	        r->AnimationOn = -1;
	    	return 0;
	    case rightArrow:
	        r->AnimationOn = 1;
	    	return 0;
	    case 't':
	        if(lineCount == 1 && line[0] == ' '){
	            lineCount = 0;
	            break;
	        }else if(lineCount > 0){
	            break;
	        }
	        
 			RenderGetFrameCount(myIcon);
			myIcon->Files=&r->Files;
	 		doTestSpeed(myIcon);	
			myIcon->Files=NULL;
			lineCount=0;
		    return 0;
	   default:
	        break;
	}
	
	
	if(c != '\n' && c != '\r'){
	    if(c == 0)return 0;
	    if(lineCount < 255)line[lineCount++]=c;
	    return 0;
	}
	line[lineCount]=0;
	if(lineCount > 1){
	    doRenderKeys(line,myIcon);
		lineCount=0;
	    return 0;
	}
	lineCount=0;
	c=line[lineCount];
	if(c == 't' || c == 'T'){
	    doTime(myIcon);
	} else if(c == '?'){
		long nodes,cells;
		
		nodes=0;
		cells=0;
		ListCountObjects((r->scene->ObjectList),&nodes,&cells);
		sprintf(buff,"Elements %ld Nodes %ld\n",cells,nodes);
		WarningBatch(buff);    		
	} else if(c == 's'){
	    doTestSine();
	} else if(c == 'w' || c == 'W' ){
		struct ObjectMessage Message;
		double rc;

		    Message.MessageType=OBJECT_MESSAGE_RANGE_DATA;
		    ListObjectMessage(r->scene->ObjectList,&Message);
			sprintf(buff,"xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g\n",
			        Message.xmin,Message.xmax,
			        Message.ymin,Message.ymax,
			        Message.zmin,Message.zmax);
			WarningBatch(buff);    		
		r->scene->Target.x=.5*(Message.xmin+Message.xmax);
		r->scene->Target.y=.5*(Message.ymin+Message.ymax);
		r->scene->Target.z=.5*(Message.zmin+Message.zmax);
		rc=sqrt((r->scene->Target.x-Message.xmax)*(r->scene->Target.x-Message.xmax)+
		        (r->scene->Target.y-Message.ymax)*(r->scene->Target.y-Message.ymax)+
		        (r->scene->Target.z-Message.zmax)*(r->scene->Target.z-Message.zmax));
		sprintf(buff,"xc %g yc %g zc %g r %g Surface Count %lld\n",r->scene->Target.x,
		             r->scene->Target.y,r->scene->Target.z,rc,Message.SurfaceCount);
			WarningBatch(buff);    		
	}
	
	

	return 0;
}
static int doTime(IconPtr myIcon)
{
 	renderPtr openData;
   	struct Scene *scene;
	int n;
	long count;
	char buff[256];
	double time,start,end;
	long cells;
	long nodes;
        int ControlMode;

  	  	    		
	if(!myIcon|| (openData=(renderPtr)myIcon->DWindow) == NULL)return 1;
	scene=openData->scene;
	if(!scene)return 1;
	ControlMode=openData->Tool;
	openData->Tool=ControlModeSpinGlobal;


	start=rtime();
	
	count=0;

	for(n=0;n<10;++n){
	    doNULL(myIcon);
		FlushCurrentPort();
	    count++;
	}
	openData->Tool=ControlMode;
	
	end=rtime();

	time=(end-start);
	if(time < 0)time = 1;
	{
	    	
	    	nodes=0;
	    	cells=0;
	    	ListCountObjects((scene->ObjectList),&nodes,&cells);
	}
	sprintf(buff,"%f Seconds %f (Frames/sec) %.0f (Cells/sec)\n",
		time,((double)count)/(time),((double)(count*cells))/(time));
	WarningBatch(buff);
	return 0;

}
static int CheckMenu(IconPtr myIcon,int flag)
{
	renderPtr t;


	if(!myIcon)return 1;
	t=(renderPtr)myIcon->DWindow;
		
	if(flag){
	    
		if(!t)return 1;
	    
		     	    
		uCheckMenuItem(myIcon,OPERATIONS_MENU,DRAG_ALL_ITEM, 		(t->Tool == ControlModeDragGlobal));
		uCheckMenuItem(myIcon,OPERATIONS_MENU,DRAG_ONE_ITEM,       	(t->Tool == ControlModeDrag));
		uCheckMenuItem(myIcon,OPERATIONS_MENU,ROTATE_ALL_ITEM, 		(t->Tool == ControlModeRotateGlobal));
		uCheckMenuItem(myIcon,OPERATIONS_MENU,ROTATE_ONE_ITEM,  	(t->Tool == ControlModeRotateLocal));
					
		uCheckMenuItem(myIcon,OPERATIONS_MENU,SET_TARGET_ITEM, 		(t->Tool == ControlModeSetTarget));
		uCheckMenuItem(myIcon,OPERATIONS_MENU,SPIN_ALL_ITEM,       	(t->Tool == ControlModeSpinGlobal));
		uCheckMenuItem(myIcon,OPERATIONS_MENU,ZOOM_ITEM, 			(t->Tool == ControlModeZoom));
		uCheckMenuItem(myIcon,OPERATIONS_MENU,SELECT_ITEM,  		(t->Tool == ControlModeSelect));		
		uCheckMenuItem(myIcon,OPERATIONS_MENU,WALK_ITEM,  			(t->Tool == ControlModeFly));		
		
		
		
		uCheckMenuItem(myIcon,ANIMATIONS_MENU,loopTek,t->LoopFlag);
		uCheckMenuItem(myIcon,ANIMATIONS_MENU,loopBackTek,t->LoopBackAndForth);
		uCheckMenuItem(myIcon,ANIMATIONS_MENU,synchronizeTek,myIcon->Synchronize);
	    	    							
	    CWindowSetPalMenu(myIcon);	    
	    doWindowMenu();
	}
	return 0;
}
static int pushStack(renderPtr r)
{
   	struct Scene *scene;
   	
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	r->stack.Target=scene->Target;
	r->stack.Degrees=scene->Degrees;
	r->stack.Eye=scene->Eye;
	r->stack.lamp=scene->lamp;
	r->stack.WalkMode=scene->WalkMode;
	
	return pushpopPush(&r->stack,r->popStack);
}
static int doOptionsAnimate(IconPtr myIcon)
{
 	
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok",      {40,125,65,25},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {240,125,65,25},uDialogButton,uDialogNone},
      /*  2 */      {"Frames/sec data", {180,45,120,25},uDialogText,uDialogGetDouble},
      /*  3 */      {"Frames/sec :",     {35,45,105,25},uDialogString,uDialogNone},
      /*  4 */      {"Update Pause Time", {180,75,120,25},uDialogText,uDialogGetDouble},
      /*  3 */      {"Update Pause Time",     {35,75,135,25},uDialogString,uDialogNone},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rectl   = {100,50,339,170};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
	renderPtr t;
	int reti;
    int ret;
		
	if(!myIcon)return 1;
	t=(renderPtr)myIcon->DWindow;
	if(!t)return 1;
		
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Animation Options",&Rectl,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	

	reti=uDialogSet(uList,2L,
	              	              
	              uDialogSetDouble,(double)myIcon->framesPerSecond,
	              
	          	  uDialogSetItem,4L,
	              uDialogSetDouble,(double)t->UpdatePauseTime,
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
Start:
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
		
    if(uR[2].dreturn <= 0.0){
		Warning("Frames/sec must be greater than Zero");
		goto Start;
	}
		
	
	myIcon->framesPerSecond=uR[2].dreturn;
	
	t->UpdatePauseTime=uR[4].dreturn;
	
	if(t->UpdatePauseTime < 0)t->UpdatePauseTime=0;
	if(t->UpdatePauseTime > 10)t->UpdatePauseTime=10;
	
	t->milliSecondsPerFrame=(long int)(1000./myIcon->framesPerSecond);
     
    ret = 0;
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int doMenus(IconPtr myIcon,int menu,int item)
{
	struct Scene *scene;
	renderPtr r;
	uRect Box;

	if(!myIcon || !(r=(renderPtr)myIcon->DWindow))return 1;
	scene=r->scene;
	if(!scene)return 1;
	 
	 RenderGetFrameCount(myIcon);
	 
	Box.x = 0;
	Box.y = 0;
  	Box.xsize=(int)(myIcon->uviewRect.xsize);
  	Box.ysize=(int)(myIcon->uviewRect.ysize);
	 
	 
	uSetCursor(myIcon,-uWATCH_CURSOR);
	
		switch(menu){
		case AppleMenu:
			if(item == 1){
			    doAbout(myIcon);
			    break;
			}
		break;
		case File_Menu:
		    if(item == Open_Item){
		        doLoadImages(myIcon);
		    }else if(item == Close_Item){
		        doClose(myIcon);
		  		myIcon=NULL;
			} else if(item == Save_Item){
				doSavePlots(myIcon,&r->Files,r->CurrentFrame,&Box);			
			} else if(item == New_Item){
		    	NewNewWindow(myIcon);
			} else if(item == savePreferencesCommand){		
		    	writeUniversal(myIcon,0);
			} else if(item == PreferencesCommand){		
				saveUniversal(myIcon);
		    }else if(item == Load_Item){
				renderPtr r;
				CObjectReadFileList(myIcon);
				if(scene->BlockCount <= 0)
				{
					CenterScreen(scene,0);
				}
				r=(renderPtr)myIcon->DWindow;
				if(r)RenderScene(r);
		    }else if(item == Quit_Item){
		        doQuit();
		  		myIcon=NULL;
		    }
		    
		break;
		
		case EDIT_MENU:
			switch(item){
 			case FRAME_IMAGE_ITEM:
				doFrameImage(myIcon);
				break;
 			case WINDOW_OPTIONS_ITEM:
		  		doWindowInformation(myIcon);
				break;
 			case OPTIONS_ITEM:
		  		doOptions(myIcon);
				break;
 			case VIEW_ITEM:
		  		doView(myIcon);
				break;
 			case OBJECT_ITEM:
		  		doObjects(myIcon);
				break;
/*
 			case SCALE_ITEM:
		  		doScale(myIcon,r->mat,NULL);
				break;
*/
 			case MATERIAL_ITEM:
		  		doMaterials(myIcon);
				break;
 			case KEYFRAME_ITEM:
				KeyFrames(myIcon);
				break;
 			case ADD_KEYFRAME_ITEM:
		  		AddKeyFrame(myIcon);
				break;
 			case REPLACE_KEYFRAME_ITEM:
		  		ReplaceKeyFrame(myIcon);
				break;
 			case RAY_TRACING_LIGHTS:
		  		DoRayTracingLights(myIcon);
				break;				
 			case EXPORT_ITEM:
		  		DoTracingExport(myIcon);
				break;				
 			case copyTekCommand:
 				n2SaveScrap(myIcon);
				break;
				
 			case popItem:
 		   		if(pushpopPop(&r->stack,r->popStack))goto OutOfHere;
POP:		  
		  		scene->Target=r->stack.Target;
		  		scene->Degrees=r->stack.Degrees;
		  		scene->Eye=r->stack.Eye;
		  		scene->lamp=r->stack.lamp;
				scene->WalkMode=r->stack.WalkMode;
				if(r)RenderScene(r);
 			 	break;
								
 			case popAllItems:
 		   		if(pushpopPopAll(&r->stack,r->popStack))goto OutOfHere;
		   		goto POP;
		   				   		
 			case zoomOut:	
		    	if(pushStack(r)){
					break;
		    	}
				scene->Degrees *= 2;
				if(r)RenderScene(r);
				break;		   		
		   		
 			case zoomIn:	
		    	if(pushStack(r)){
					break;
		    	}
				scene->Degrees /= 2;
				if(r)RenderScene(r);
				break;		   		
		   		
		    }
		break;
		
		case OPERATIONS_MENU:
		    if(item == DRAG_ALL_ITEM){
		        r->Tool=ControlModeDragGlobal;
		    }else if(item == DRAG_ONE_ITEM){
		        r->Tool=ControlModeDrag;
		    }else if(item == ROTATE_ALL_ITEM){
		        r->Tool=ControlModeRotateGlobal;
		    }else if(item == ROTATE_ONE_ITEM){
		        r->Tool=ControlModeRotateLocal;
		    }else if(item == SET_TARGET_ITEM){
		        r->Tool=ControlModeSetTarget;
		    }else if(item == SPIN_ALL_ITEM){
		        r->Tool=ControlModeSpinGlobal;
		    }else if(item == ZOOM_ITEM){
		        r->Tool=ControlModeZoom;
		    }else if(item == SELECT_ITEM){
		        r->Tool=ControlModeSelect;
		    }else if(item == WALK_ITEM){
				struct ObjectMessage Message;
								
		        r->Tool=ControlModeFly;
		        
		        zerol((char *)&Message,sizeof(Message));
				Message.MessageType=OBJECT_MESSAGE_RANGE_DATA;
				ListObjectMessage(scene->ObjectList,&Message);
				if(Message.xmin < 1e29){
					scene->WalkScale=sqrt((Message.xmax-Message.xmin)*(Message.xmax-Message.xmin)+
										  (Message.ymax-Message.ymin)*(Message.ymax-Message.ymin)+
										  (Message.zmax-Message.zmin)*(Message.zmax-Message.zmin));

				}
				stopLanguage();
				startLanguage(CommandsW,CommandLengthW,myIcon);
				break;
		    }
			stopLanguage();
			startLanguage(Commands,CommandLength,myIcon);
		break;
		
		case ANIMATIONS_MENU:
		    if(item == ORBIT_ITEM){
		  		doOrbit(myIcon);
		    }else if(item == MOVE_TARGET_ITEM){
		  		doMove(myIcon);
		    }else if(item == ROTATE_OBJECT_ITEM){
		  		doRotate(myIcon);
		    }else if(item == TIME_AMIMATE_ITEM){
		  		doTimeAnimation(myIcon);
		    }else if(item == KEY_FRAME_AMIMATE_ITEM){
				KeyFrameAnimation(myIcon);
		    }else if(item == FLIGHT__AMIMATE_ITEM){
				RenderFlightAnimation(myIcon);												
		    }else if(item == loopTek){
	        	r->LoopFlag = !r->LoopFlag;
		    }else if(item == loopBackTek){
	        	r->LoopBackAndForth = !r->LoopBackAndForth;
		    }else if(item == optionsTek){
 		 		doOptionsAnimate(myIcon);
		    }else if(item == synchronizeTek){
 		 		myIcon->Synchronize = !myIcon->Synchronize;
       			if(myIcon->Synchronize)Synchronize(myIcon,r->CurrentFrame);
		    }
		break;	
		
		case NEW_Menu:
		  pNewMenu(myIcon,item);
		break;
		
	case PalMenu4:
		CWindowdoPalMenu(myIcon,menu,item);
	    {
	    	int n;
	    	if(r->scene){
		        for(n=0;n<256;++n){
			    	r->scene->pd.palette[3*n]=myIcon->red[n];
			     	r->scene->pd.palette[3*n+1]=myIcon->green[n];
			     	r->scene->pd.palette[3*n+2]=myIcon->blue[n];
		        }
		        
		        changePalette(r->scene,r->mat);
		        
				if(r)RenderScene(r);
	        }
	    }
		break;
		
		case WindowMenu:
			pWindowMenu(myIcon,item);
			break;
					
			}  
OutOfHere:		   
	uSetCursor(myIcon,-uARROW_CURSOR);
	HiliteMenu(0);
	return 0;
}
static int doOptions(IconPtr myIcon)
{
	return GetPlotOptions(myIcon);
}
static int doNULL(IconPtr myIcon)
{
	renderPtr r;
   	struct Scene *scene;
   	double theta1,theta2;

	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	
	
	scene=r->scene;
	if(!scene)return 1;
	
	if((r->WalkMotion > 0) && r->WalkMotionContinuous){
		if(TickCount() > r->TimeToFlip){
			if(r->Tool == ControlModeFly){
				doWalkCommands(myIcon,r->WalkMotion);
			}else{
				doCommands(myIcon,r->WalkMotion);
			}
       		r->TimeToFlip = TickCount() + r->milliSecondsPerFrame*60/1000;
		}
		return 0;
	}
		
	if(!(r->AnimationOn) || (r->ImageCount <= 1))goto RotateSpin;
	    
	if(TickCount() > r->TimeToFlip){
       r->CurrentFrame += r->AnimationOn;
       if(r->CurrentFrame >= r->ImageCount){
           if(r->LoopBackAndForth){
               r->CurrentFrame=r->ImageCount-1;
               r->AnimationOn = -1;
           }else{
               r->CurrentFrame=0;
           }
       }else if(r->CurrentFrame < 0){
           if(r->LoopBackAndForth){
               r->CurrentFrame=0;
               r->AnimationOn = 1;
           }else{
               r->CurrentFrame=r->ImageCount-1;
           }
       }
       r->TimeToFlip = TickCount() + r->milliSecondsPerFrame*60/1000;

	   scene->time=r->CurrentFrame;
	   
    	if(scene->AutoCenterScreen){
	   		RenderSceneSetTime(scene);
			scene->AutoCenterScreenDone=FALSE;
		}

	        
	   if(r->Tool != ControlModeSpinGlobal || !(r->theta1 || r->theta2))RenderScene(r);
		
       if(myIcon->Synchronize)Synchronize(myIcon,r->CurrentFrame);
	       
		if(!r->LoopFlag){
		    if(r->AnimationOn > 0){	        
			    if(r->CurrentFrame+1 == r->ImageCount){
	            	r->AnimationOn=FALSE;
					InvalRectMyWindow(myIcon);
			    }
		    }else if(!r->CurrentFrame){ 
			    r->AnimationOn=FALSE;
				InvalRectMyWindow(myIcon);
		    }
		}
	}

	
	
	
	
RotateSpin:	
	   	
	if(r->Tool != ControlModeSpinGlobal)return 1;
	
	theta1=r->theta1*3.1415926/180.;
	theta2=r->theta2*3.1415926/180.;
	if(theta1 || theta2){
		struct Trans1 t;
		struct P V1,V2;
		struct P V1n,V2n;
		
		zerol((char *)&t,sizeof(struct Trans1));
		
		t.Type=T_LOCALSYSTEM;
		t.Groups=1;
		t.P=(char *)&scene->World;
	    if(theta1 != 0){
			V1=scene->g.V3;
			V2=scene->g.V1;
			t.Xa=V1;
			t.Ya=V2;
			t.Oa=scene->Target;
		
			V1n=Mult(&V1,cos(theta1));
			V2n=Mult(&V2,sin(theta1));
			t.Xn=Add(&V1n,&V2n);
			V1n=Mult(&V1,sin(theta1));
			V2n=Mult(&V2,cos(theta1));
			t.Yn=Add(&V2n,&V1n);
			t.On=scene->Target;
			Transf1(&t);
		}
	    if(theta2 != 0){
			V1=scene->g.V2;
			V2=scene->g.V3;
			t.Xa=V1;
			t.Ya=V2;
			t.Oa=scene->Target;
		
			V1n=Mult(&V1,cos(theta2));
			V2n=Mult(&V2,sin(theta2));
			t.Xn=Add(&V1n,&V2n);
			V1n=Mult(&V1,sin(theta2));
			V2n=Mult(&V2,cos(theta2));
			t.Yn=Add(&V2n,&V1n);
			t.On=scene->Target;
			Transf1(&t);
		}
		findAngles(&scene->World); 
		RenderScene(r);
	} 

	return 0;
}
static int tInContent(IconPtr myIcon)
{
	renderPtr r;
	uRect cBox;
	
	
	if(!myIcon || !(r=(renderPtr)myIcon->DWindow))return 1;

	cBox=myIcon->eraseRect;
	cBox.x = cBox.x+cBox.xsize-4;
	cBox.xsize = 4;
	cBox.ysize = 4;

	uPointer(&r->p1,myIcon);
		
	if(uPtInRect(r->p1,&myIcon->uviewRect)){
		
		if(r->Tool == ControlModeRotateLocal){
			doRotateLocalDown(myIcon);
		} else if(r->Tool == ControlModeZoom){
			doZoomDown(myIcon);
		} else if(r->Tool == ControlModeSetTarget){
			doSetTargetDown(myIcon);
		} else if(r->Tool == ControlModeRotateGlobal){
			doRotateGlobalDown(myIcon);
		} else if(r->Tool == ControlModeDrag){
			doDragDown(myIcon);
		} else if(r->Tool == ControlModeDragGlobal){
			doDragDownGlobal(myIcon);
		} else if(r->Tool == ControlModeSelect){
			doSelect(myIcon);
		} else if(r->Tool == ControlModeFly){
			doInFly(myIcon);
		} else if(r->Tool == ControlModeSpinGlobal){
	   	    double theta1,theta2,dt1,dt2;

		    uPointer(&r->p1,myIcon);
		    theta1= 0.;
		    theta2= 0.;
		    while(uStillDown(myIcon)){
		        char buff[256];
		        uPointer(&r->p2,myIcon);
				dt1=r->p2.x - r->p1.x;
				dt2=r->p2.y - r->p1.y;
				theta1= (.25*dt1);
				theta2= (.25*dt2);
				sprintf(buff,"theta-x %g theta-y %g",theta1,theta2);
				nWriteBox(myIcon,buff);
		    }
		    r->theta1=theta1;
		    r->theta2=theta2;
		    return 0;
		}				
	} else if(uPtInRect(r->p1,&cBox)){
	    doEraseDown(myIcon);
	} else {
	   uDoControls(myIcon);
	}
	return 0;
}
int doSelect(IconPtr myIcon)
{
   	struct Scene *scene;
   	char buff[256];
	uPoint p1,p2;
	struct P p1s,p1so;
	long ncell,ix,iy,n;
	double xmin,ymin,delx,dely,odx,ody;
	int rastorY,xshift,yshift;
	renderPtr r;
	CObjectListPtr o;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;	
	scene=r->scene;
	if(!scene)return 1;
	o=scene->ObjectList;
	if(!o)return 1;
	
	/* pushStack(r); */
	
	r->WalkObjectCount = -1;
	r->WalkObject = NULL;
	
	p1so.x=-123;
	p1so.y=-13;
	p1so.z=-1.3;
	
	p2.x = -123;
	p2.y = -121;
	xshift = -myIcon->uviewRect.x;
	yshift = -myIcon->uviewRect.y;
	do {
	    uPointer(&p1,myIcon);
	    if((p1.x == p2.x) && (p1.y == p2.y)){
	    	continue;
	    }
		nWriteBox(myIcon," ");
		ix=p1.x+xshift;
		if(ix < 0 || ix >= scene->xResolution)continue;
		iy=p1.y+yshift;
		if(iy < 0 || iy >= scene->yResolution)continue;

		ncell=scene->CellBuff[ix+iy*scene->xResolution];
		if(ncell < 0)continue;
		xmin=scene->xmin;
		ymin=scene->ymin;
		odx=scene->odx;
		ody=scene->ody;
		delx=1.0/odx;
		dely=1.0/ody;
		rastorY=(int)(scene->yResolution-1);
		p1s.z=scene->zbuff[ix+iy*scene->xResolution];
		p1s.x=((double)ix*delx+xmin)*(scene->Height-p1s.z)/scene->Height;
		p1s.y=((double)(rastorY-iy)*dely+ymin)*(scene->Height-p1s.z)/scene->Height;

		Transform(&p1s,&scene->ScreenToGlobal);
		
		sprintf(buff,"x %g y %g z %g",p1s.x,p1s.y,p1s.z);
		nWriteBox(myIcon,buff);
		
		if(p1s.x == p1so.x && p1s.y == p1so.y && p1s.z == p1so.z)continue;
		p1so=p1s;		
		sprintf(buff,"Point %g %g %g %ld",p1s.x,p1s.y,p1s.z,ncell);
	    WarningBatchFast(buff);
/*	    	 
		sprintf(buff,"xmin %g ymin %g odx %g ody %g",xmin,ymin,odx,ody);
	    WarningBatchFast(buff);
*/	    	 
		scene->FindPoint=p1s;
		p1=p2;		
		
	    for(n=o->oCount-1;n >= 0;--n){
			CObjectPtr Current;

	        if((Current=o->oCurrent[n]) == NULL)continue;

	        if(ncell < Current->FirstCell || ncell >= Current->LastCell)continue;
	        
	     	if(Current->type == G_FLIGHT_PATH){
	     	    CFlightPathPtr f;
	     	    
	     	    r->WalkObject=f=(CFlightPathPtr)Current;
	     	    
	    		r->WalkObjectCount = (ncell-Current->FirstCell)/((f->nx-1)*(f->ny-1));
	     	}   
	        
/*	        
	        sprintf(WarningBuff,"WalkObjectCount %ld %ld %ld\n",ncell,Current->FirstCell,(long)r->WalkObjectCount);
	        WarningBatch(WarningBuff);
	        sprintf(WarningBuff,"Material Selected %ld\n",(long)Current->Material);
	        WarningBatch(WarningBuff);
*/	        
			if(Current->type == G_BLOCK){
			   // sprintf(buff,"Material %ld type %ld block %ld",Current->Material,(long)Current->type,(long)G_BLOCK);
	    	   // WarningBatchFast(buff);
	    	    CBlockPtr bp=(CBlockPtr)Current;
	    	    struct Block *b;
	    	    b = &(scene->block[bp->BlockNumber]);
	    	    if(b->stress){
			    	sprintf(buff,"BlockNumber %ld ncell %ld stress %g",
			          (long)bp->BlockNumber,ncell-Current->FirstCell,b->stress[ncell-Current->FirstCell]);
	    	    	WarningBatchFast(buff);
	    	    }
	    	
	    	}
	        
	        setSceneMaterial(myIcon,Current->Material);
	        
	        break;      
	        
	    }
		
			
		
	}while(uStillDown(myIcon));
	
	WarningBatchAtEnd(WarningBuff);        
		return 0;

}
static int setSceneMaterial(IconPtr myIcon,int mat)
{
   	struct Scene *scene;
	renderPtr r;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;	
	scene=r->scene;
	if(!scene)return 1;
	if(mat < 0 || mat >= scene->MaterialCount)return 1;

	r->mat=mat;	  
	
	scene->pd.sType=scene->material[mat].pd.sType;
	scene->pd.sPmin=scene->material[mat].pd.sPmin;
	scene->pd.sPmax=scene->material[mat].pd.sPmax;
	scene->pd.dmin=scene->material[mat].pd.dmin;
	scene->pd.dmax=scene->material[mat].pd.dmax;
	        
	return 0;
}
int doDragDownGlobal(IconPtr myIcon)
{
   	struct Scene *scene;
	uPoint p1,p2;
	struct P p1s,p2s;
	long ncell,ix,iy;
	double xmin,ymin,delx,dely,odx,ody,dx,dy;
	int rastorY,xshift,yshift;
	renderPtr r;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	
	scene=r->scene;
	if(!scene)return 1;

	pushStack(r);
		
	p1=r->p1;
	xshift = -myIcon->uviewRect.x;
	yshift = -myIcon->uviewRect.y;
	while(uStillDown(myIcon)){
	    uPointer(&p2,myIcon);
	    if((p1.x == p2.x) && (p1.y == p2.y)){
	    	continue;
	    }
	    ix=p1.x+xshift;
	    if(ix < 0 || ix >= scene->xResolution)continue;
	    iy=p1.y+yshift;
	    if(iy < 0 || iy >= scene->yResolution)continue;
	    
		ncell=scene->CellBuff[ix+iy*scene->xResolution];
		if(ncell < 0)continue;
	        xmin=scene->xmin;
	        ymin=scene->ymin;
	        odx=scene->odx;
	        ody=scene->ody;
	        delx=1.0/odx;
	        dely=1.0/ody;
			rastorY=(int)(scene->yResolution-1);
			p1s.z=scene->zbuff[ix+iy*scene->xResolution];
			p1s.x=((double)ix*delx+xmin)*(scene->Height-p1s.z)/scene->Height;
			p1s.y=((double)(rastorY-iy)*dely+ymin)*(scene->Height-p1s.z)/scene->Height;
	        ix=p2.x+xshift;
	        iy=p2.y+yshift;
			p2s.z=p1s.z;
			p2s.x=((double)ix*delx+xmin)*(scene->Height-p2s.z)/scene->Height;
			p2s.y=((double)(rastorY-iy)*dely+ymin)*(scene->Height-p2s.z)/scene->Height;
			dx=p2s.x-p1s.x;
			dy=p2s.y-p1s.y;
		{
			struct System Local;
	        
	        Local=scene->World;
	                
	        Transform(&Local.p,&scene->GlobalToScreen);
	        
	        Local.p.x += dx;
	        Local.p.y += dy;
	        
	        Transform(&Local.p,&scene->ScreenToGlobal);

	        scene->World=Local;
	    }
		RenderScene(r);
		p1=p2;
	    if(r->UpdatePauseTime > 0)Sleep2((int)(r->UpdatePauseTime*60.));
	}
	    uPointer(&r->p2,myIcon);
		return 0;

}
int doDragDown(IconPtr myIcon)
{
   	struct Scene *scene;
	uPoint p1,p2;
	struct P p1s,p2s;
	long ncell,ix,iy,n;
	double xmin,ymin,delx,dely,odx,ody,dx,dy;
	int rastorY,xshift,yshift;
	renderPtr r;
	CObjectListPtr o;
	struct System Local;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	
	scene=r->scene;
	if(!scene)return 1;
	o=scene->ObjectList;
	if(!o)return 1;

	pushStack(r);

	p1=r->p1;
	xshift = -myIcon->uviewRect.x;
	yshift = -myIcon->uviewRect.y;
	while(uStillDown(myIcon)){
	    uPointer(&p2,myIcon);
	    if((p1.x == p2.x) && (p1.y == p2.y)){
	    	continue;
	    }
	    ix=p1.x+xshift;
	    if(ix < 0 || ix >= scene->xResolution)continue;
	    iy=p1.y+yshift;
	    if(iy < 0 || iy >= scene->yResolution)continue;
	    
		ncell=scene->CellBuff[ix+iy*scene->xResolution];
		if(ncell < 0)continue;
	        xmin=scene->xmin;
	        ymin=scene->ymin;
	        odx=scene->odx;
	        ody=scene->ody;
	        delx=1.0/odx;
	        dely=1.0/ody;
			rastorY=(int)(scene->yResolution-1);
			p1s.z=scene->zbuff[ix+iy*scene->xResolution];
			p1s.x=((double)ix*delx+xmin)*(scene->Height-p1s.z)/scene->Height;
			p1s.y=((double)(rastorY-iy)*dely+ymin)*(scene->Height-p1s.z)/scene->Height;
	        ix=p2.x+xshift;
	        iy=p2.y+yshift;
			p2s.z=p1s.z;
			p2s.x=((double)ix*delx+xmin)*(scene->Height-p2s.z)/scene->Height;
			p2s.y=((double)(rastorY-iy)*dely+ymin)*(scene->Height-p2s.z)/scene->Height;
			dx=p2s.x-p1s.x;
			dy=p2s.y-p1s.y;


	    for(n=o->oCount-1;n >= 0;--n){
			CObjectPtr Current;

	        if((Current=o->oCurrent[n]) == NULL)continue;
	        
	        if(ncell < Current->FirstCell || ncell >= Current->LastCell)continue;
	        
	        Local=Current->Local;
	        
	        
	        Transform(&Local.p,&scene->GlobalToScreen);
	        
	        Local.p.x += dx;
	        Local.p.y += dy;
	        
	        Transform(&Local.p,&scene->ScreenToGlobal);
	        Current->Local=Local;
	        break;	        
	    }
		RenderScene(r);
		p1=p2;
	    if(r->UpdatePauseTime > 0)Sleep2((int)(r->UpdatePauseTime*60.));
	}
    uPointer(&r->p2,myIcon);
	return 0;

}
static int doSetTarget(IconPtr myIcon,uPoint p1)
{
   	struct Scene *scene;
   	char buff[256];
	
	long ncell,ix,iy;
	int xshift,yshift;
	struct P p1s;
	renderPtr openData;
	
	if(!myIcon)return 1;
	openData=(renderPtr)myIcon->DWindow;
	if(!openData)return 1;
	scene=openData->scene;
	if(!scene)return 1;

	xshift = -myIcon->uviewRect.x;
	yshift = -myIcon->uviewRect.y;

	ix=p1.x+xshift;
	if(ix < 0 || ix >= scene->xResolution)return 1;
	iy=p1.y+yshift;
	if(iy < 0 || iy >= scene->yResolution)return 1;

	ncell=scene->CellBuff[ix+iy*scene->xResolution];
	if(ncell < 0)return 1;

	p1s.z=scene->zbuff[ix+iy*scene->xResolution];
	p1s.x=((double)ix/scene->odx+scene->xmin)*(scene->Height-p1s.z)/scene->Height;
	p1s.y=((double)(scene->yResolution-1-iy)/scene->ody+scene->ymin)*(scene->Height-p1s.z)/scene->Height;

	Transform(&p1s,&scene->ScreenToGlobal);

	scene->Target=p1s;

	RenderScene(openData);
	
	sprintf(buff,"x %g y %g z %g",p1s.x,p1s.y,p1s.z);
	nWriteBox(myIcon,buff);

	
	return 0;
}
static int doSetTargetDown(IconPtr myIcon)
{
   	struct Scene *scene;
	uPoint p1;
	renderPtr openData;
	
	if(!myIcon)return 1;
	openData=(renderPtr)myIcon->DWindow;
	if(!openData)return 1;
	
	scene=openData->scene;
	if(!scene)return 1;

	pushStack(openData);
	
	p1=openData->p1;
	while(uStillDown(myIcon)){
		if(doSetTarget(myIcon,p1)){
			uPointer(&p1,myIcon);
			continue;
		}		
		break;
	}
    uPointer(&openData->p2,myIcon);
	return 0;

}
int doRotateGlobalDown(IconPtr myIcon)
{
   	struct Scene *scene;
	renderPtr r;
	double theta1,theta2,dt1,dt2;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;

	scene=r->scene;
	if(!scene)return 1;


    uPointer(&r->p1,myIcon);
    theta1= 0.;
    theta2= 0.;
    while(uStillDown(myIcon)){
        uPointer(&r->p2,myIcon);
		dt1=r->p2.x - r->p1.x;
		dt2=r->p2.y - r->p1.y;
		theta1= (.25*dt1);
		theta2= (.25*dt2);
		
		theta1=theta1*3.1415926/180.;
		theta2=theta2*3.1415926/180.;
		
		if(theta1 || theta2){
			struct Trans1 t;
			struct P V1,V2;
			struct P V1n,V2n;
			
			zerol((char *)&t,sizeof(struct Trans1));
			
			t.Type=T_LOCALSYSTEM;
			t.Groups=1;
			t.P=(char *)&scene->World;
		    if(theta1 != 0){
				V1=scene->g.V3;
				V2=scene->g.V1;
				t.Xa=V1;
				t.Ya=V2;
				t.Oa=scene->Target;
			
				V1n=Mult(&V1,cos(theta1));
				V2n=Mult(&V2,sin(theta1));
				t.Xn=Add(&V1n,&V2n);
				V1n=Mult(&V1,sin(theta1));
				V2n=Mult(&V2,cos(theta1));
				t.Yn=Add(&V2n,&V1n);
				t.On=scene->Target;
				Transf1(&t);
			}
		    if(theta2 != 0){
				V1=scene->g.V2;
				V2=scene->g.V3;
				t.Xa=V1;
				t.Ya=V2;
				t.Oa=scene->Target;
			
				V1n=Mult(&V1,cos(theta2));
				V2n=Mult(&V2,sin(theta2));
				t.Xn=Add(&V1n,&V2n);
				V1n=Mult(&V1,sin(theta2));
				V2n=Mult(&V2,cos(theta2));
				t.Yn=Add(&V2n,&V1n);
				t.On=scene->Target;
				Transf1(&t);
			}
			findAngles(&scene->World); 
			RenderScene(r);
			r->p1=r->p2;
	        if(r->UpdatePauseTime > 0)Sleep2((int)(r->UpdatePauseTime*60.));
		} 		

    }

	
	return 0;
	
}
int doRotateLocalDown(IconPtr myIcon)
{
   	struct Scene *scene;
	uPoint p1,p2;
	struct P p1s;
	long ncell,ix,iy,n;
	double xmin,ymin,delx,dely,odx,ody;
	double a1,b1;
	int rastorY,xshift,yshift;
	renderPtr r;
	CObjectListPtr o;
	CObjectPtr Current;
	double theta1,theta2,dt1,dt2;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;

	scene=r->scene;
	if(!scene)return 1;
	o=scene->ObjectList;
	if(!o)return 1;

	pushStack(r);
		
	xshift = -myIcon->uviewRect.x;
	yshift = -myIcon->uviewRect.y;
	
	p1=r->p1;
	Current=NULL;
	while(uStillDown(myIcon)){


	    uPointer(&p2,myIcon);
	    if((p1.x == p2.x) && (p1.y == p2.y)){
	    	continue;
	    }
	    ix=p1.x+xshift;
	    if(ix < 0 || ix >= scene->xResolution)continue;
	    iy=p1.y+yshift;
	    if(iy < 0 || iy >= scene->yResolution)continue;
		ncell=scene->CellBuff[ix+iy*scene->xResolution];
		if(ncell < 0)continue;
	        xmin=scene->xmin;
	        ymin=scene->ymin;
	        odx=scene->odx;
	        ody=scene->ody;
	        delx=1.0/odx;
	        dely=1.0/ody;
			rastorY=(int)(scene->yResolution-1);
			p1s.z=scene->zbuff[ix+iy*scene->xResolution];
			p1s.x=((double)ix*delx+xmin)*(scene->Height-p1s.z)/scene->Height;
			p1s.y=((double)(rastorY-iy)*dely+ymin)*(scene->Height-p1s.z/scene->Height);
	        ix=p2.x+xshift;
	        iy=p2.y+yshift;
			a1 = -((double)ix*delx+xmin);
			b1 = -((double)(rastorY-iy)*dely+ymin);
	        for(n=o->oCount-1;n >= 0;--n){

		        if((Current=o->oCurrent[n]) == NULL)continue;
		        
	        	if(ncell < Current->FirstCell || ncell >= Current->LastCell)continue;
				break;
	    }
	    break;
	}
	
	if(!Current)return 0;
	
	
    uPointer(&r->p1,myIcon);
    theta1= 0.;
    theta2= 0.;
    while(uStillDown(myIcon)){
        uPointer(&r->p2,myIcon);
		dt1=r->p2.x - r->p1.x;
		dt2=r->p2.y - r->p1.y;
		theta1= (.25*dt1);
		theta2= (.25*dt2);
		
		theta1=theta1*3.1415926/180.;
		theta2=theta2*3.1415926/180.;
		
		if(theta1 || theta2){
			struct Trans1 t;
			struct P V1,V2;
			struct P V1n,V2n;
			
			zerol((char *)&t,sizeof(struct Trans1));
			
			t.Type=T_LOCALSYSTEM;
			t.Groups=1;
			t.P=(char *)&Current->Local;
		    if(theta1 != 0){
				V1=scene->g.V3;
				V2=scene->g.V1;
				t.Xa=V1;
				t.Ya=V2;
				t.Oa=Current->Local.p;
			
				V1n=Mult(&V1,cos(theta1));
				V2n=Mult(&V2,sin(theta1));
				t.Xn=Add(&V1n,&V2n);
				V1n=Mult(&V1,sin(theta1));
				V2n=Mult(&V2,cos(theta1));
				t.Yn=Add(&V2n,&V1n);
				t.On=Current->Local.p;
				Transf1(&t);
			}
		    if(theta2 != 0){
				V1=scene->g.V2;
				V2=scene->g.V3;
				t.Xa=V1;
				t.Ya=V2;
				t.Oa=Current->Local.p;
			
				V1n=Mult(&V1,cos(theta2));
				V2n=Mult(&V2,sin(theta2));
				t.Xn=Add(&V1n,&V2n);
				V1n=Mult(&V1,sin(theta2));
				V2n=Mult(&V2,cos(theta2));
				t.Yn=Add(&V2n,&V1n);
				t.On=Current->Local.p;
				Transf1(&t);
			}
			findAngles(&Current->Local); 
			RenderScene(r);
			r->p1=r->p2;
	    	if(r->UpdatePauseTime > 0)Sleep2((int)(r->UpdatePauseTime*60.));
		} 		

    }

	
	
	
	
	return 0;
	
}
int doZoomDown(IconPtr myIcon)
{
   	struct Scene *scene;
   	struct P diff;
	struct P p1s;
	uPoint where;
	uRect r;
	renderPtr openData;
	int rastorY,xshift,yshift;
	long ix,iy;
	double Degrees,Length;
	
	if(!myIcon)return 1;
	openData=(renderPtr)myIcon->DWindow;
	if(!openData)return 1;
	
	scene=openData->scene;
	if(!scene)return 1;
	
	
	xshift = -myIcon->uviewRect.x;
	yshift = -myIcon->uviewRect.y;
	rastorY=(int)(scene->yResolution-1);
	
	uPointer(&where,myIcon);
	
	if(drawBoxOnScreen(myIcon,&r,NULL))return 1;
	
	if(r.xsize < 2 || r.ysize < 2)return 1;
	
	ix=r.x+r.xsize/2+xshift;
	if(ix < 0 || ix >= scene->xResolution)return 1;
	iy=r.y+r.ysize/2+yshift;
	if(iy < 0 || iy >= scene->yResolution)return 1;

	pushStack(openData);
	
	diff=Sub(&scene->Target,&scene->g.pScreen);
	Length=Len(&diff);

	p1s.z = -Length;
	p1s.x=((double)ix/scene->odx+scene->xmin)*(scene->Height-p1s.z)/scene->Height;
	p1s.y=((double)(rastorY-iy)/scene->ody+scene->ymin)*(scene->Height-p1s.z)/scene->Height;

	Transform(&p1s,&scene->ScreenToGlobal);
		
	scene->Target=p1s;
	
	Degrees=scene->Degrees;
	
	if((double)scene->xResolution/(double)r.xsize <  (double)scene->yResolution/(double)r.ysize){
	    Degrees *= ((double)r.xsize/(double)scene->xResolution);
	}else{
	    Degrees *= ((double)r.ysize/(double)scene->yResolution);
	}
	
	scene->Degrees=Degrees;
	    
	RenderScene(openData);

	return 0;

}
static int n2SaveScrap(IconPtr myIcon)
{
	struct Scene *scene;
	renderPtr t;
	uRect Box;


	if(!myIcon)return 1;
	t=(renderPtr)myIcon->DWindow;
	if(!t)return 1;
	scene=t->scene;
	if(!scene)return 1;
		  	
  	Box.x = 0;
  	Box.y = 0;
  	Box.xsize=(int)(myIcon->uviewRect.xsize);
  	Box.ysize=(int)(myIcon->uviewRect.ysize);
  	  	
	return drawToScrap(myIcon,&Box,t->CurrentFrame,1);
}
static int UpdateWrite(IconPtr myIcon)
{
	    uRect portRect;
		uPort save;

		if(!myIcon || !myIcon->isOpen)return 1;
		
		uGetPort(&save);
		SetPortMyWindow(myIcon);
	
	    uGetPortBounds(myIcon,&portRect);
	    uClipRect(&portRect,myIcon);
	    /* uEraseRect(&portRect,myIcon); */
	
		drawIt(myIcon);
		
	    uDrawControls(myIcon);
	    uDrawGrowIcon(myIcon);
	    
	    drawWalkCursor(myIcon);

	    
		uSetPort(&save);
	    return 0;
}
static void drawIt(IconPtr myIcon)
{
		struct Scene *scene;
        uRect portRect;
	    renderPtr t;
		uRect Box;
	    
		if(!myIcon)return;
		t=(renderPtr)myIcon->DWindow;
		if(!t)return;
		scene=t->scene;
		if(!scene)return;
		
	    uClipRect(&myIcon->uviewRect,myIcon);
	    
  		Box.x = 0;
  		Box.y = myIcon->uviewRect.y;
  		Box.xsize=(int)(myIcon->uviewRect.xsize);
  		Box.ysize=(int)(myIcon->uviewRect.ysize);
  		
  		
		drawTek(myIcon,t->CurrentFrame,&Box);
		
	    uGetPortBounds(myIcon,&portRect);
	    uClipRect(&portRect,myIcon);
}
static int DrawAnimate(IconPtr myIcon,long CurrentFrame)
{
	struct Scene *scene;
	renderPtr s;
	uPort save;
	
	if(!myIcon)return 1;
	s=(renderPtr)myIcon->DWindow;
	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;

	uGetPort(&save);
	SetPortMyWindow(myIcon);
	
	if(s->CurrentFrame != CurrentFrame){
	    s->CurrentFrame=CurrentFrame;
		scene->time=s->CurrentFrame;	        
		RenderScene(s);
	}
		
	drawIt(myIcon);	
		
	uSetPort(&save);
	return 0;
}
static int drawTek2(IconPtr myIcon,long CurrentFrame,uRect *DRect)
{
	struct Scene *scene;
	renderPtr t;

	if(!myIcon || !DRect)return 1;
	t=(renderPtr)myIcon->DWindow;
	if(!t)return 1;
	scene=t->scene;
	if(!scene)return 1;
	
	if(t->CurrentFrame != CurrentFrame){
	    t->CurrentFrame=CurrentFrame;
		scene->time=t->CurrentFrame;	        
		RenderScene(t);
	}
			
	drawTek(myIcon,CurrentFrame,DRect);
	
	return 0;
}
static int drawTek(IconPtr myIcon,long CurrentFrame,uRect *DRect)
{
		struct Scene *scene;
	    uRect SRect,LRect;
	    int top,left,dx,dy;
	    renderPtr s;
	    long xsize,ysize;
	    
		if(!myIcon)return 1;
		s=(renderPtr)myIcon->DWindow;
		if(!s)return 1;
		scene=s->scene;
		if(!scene)return 1;
		if(!DRect)return 1;
		
		s->CurrentFrame=CurrentFrame;	
		
		xsize=DRect->x+DRect->xsize;
		if(xsize < scene->xResolution)xsize = scene->xResolution;
		
		ysize=DRect->y+DRect->ysize;
		if(ysize < scene->yResolution)ysize = scene->yResolution;
							
	    if(SetBuffers(xsize*ysize*3+512L))return 1;
	    
	   /* ScreenSetSize(scene->xResolution,scene->xResolution,myIcon); */
	   
	    ScreenSetSize(xsize,ysize,myIcon);
	    
	    LRect= *DRect;
	    
	    dx=LRect.xsize;
	    dy=LRect.ysize;
	    
	    top=0;
	    left=0;

		if(myIcon->VControl)top=(int)uGetControlValue(myIcon->VControl);
		if(myIcon->HControl)left=(int)uGetControlValue(myIcon->HControl);

        SRect.y=top;
        if(SRect.y < 0)SRect.y=0;
        if(SRect.y > s->ysize)SRect.y=(int)s->ysize;
        
        SRect.ysize = dy;
        if(SRect.ysize < 0)SRect.ysize=0;
        if(SRect.y+SRect.ysize > s->ysize)SRect.ysize=(int)(s->ysize-SRect.y);
        
        
        SRect.x=left;
        if(SRect.x < 0)SRect.x=0;
        if(SRect.x > s->xsize)SRect.x=(int)s->xsize;
        
        SRect.xsize = dx;
        if(SRect.xsize < 0)SRect.xsize=0;
        if(SRect.x+SRect.xsize > s->xsize)SRect.xsize=(int)(s->xsize-SRect.x);
        
	     LRect.xsize=SRect.xsize;
	    
	    LRect.ysize=SRect.ysize;	    

		ScreenSetPalette(myIcon,myIcon->red,myIcon->green,myIcon->blue);
		    	    
		uScreenWrite(s->xsize,s->ysize,(unsigned char *)s->buffer,(unsigned char *)xg.buffout,
		            ScreenDataType24,&SRect,&LRect,myIcon);
	    	    			    
		return 0;
}


static int RenderGetMessage(IconPtr myIcon,long MessageType,void *MessageData)
{
    struct ObjectMessage Message;
	struct Message1000 *m;
	char windowName[256];
	/* struct linedata *li;
	struct areadata *ai; */
	uPort save;
	struct Scene *scene;
	CObjectListPtr o;
	renderPtr s;
	struct FileInfo2 *Files;
	int ret;
	int n;
	
    struct Material *mp;
    struct Stripes *sdata;
    struct Image *idata;
    long k;
	int isFound;

	if(!myIcon)return 1;
	s=(renderPtr)myIcon->DWindow;
	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;
	o=scene->ObjectList;
	if(!o)return 1;
	
	
	
	m=(struct Message1000 *)MessageData;

	ret = 1;
	
	uGetPort(&save);
	
	SetPortMyWindow(myIcon);

    Message.MessageType=OBJECT_MESSAGE_MESSAGE1000_TYPE;
    Message.m=m;
    Message.MessageType1000=MessageType;
	
	switch(MessageType){
	case MessageType_LIMITS:
	    break;
	case MessageType_SET_FRAME_NUMBER:
	    if(!myIcon->Synchronize)goto OK;
	    if(!m)goto ErrorOut;
	    
	    if(s->CurrentFrame != m->CurrentFrame){
	    	s->CurrentFrame=m->CurrentFrame;
	    	scene->time=m->CurrentFrame;
			if(s)RenderScene(s);
	    }
	   	break;
	case MessageType_SET_PALETTE:
	/*
		SetDefaultPaletteName(myIcon,myIcon->DefaultPaletteName);
     */
     	break;
	case MessageType_GET_FRAME_COUNT:
	    ListObjectMessage(o,&Message);
		goto OK;
	case MessageType_GET_LINE_DATA:
	    /* li=MessageData;
	     ret = doLineGetData(li); */
	    goto ErrorOut;
	case MessageType_GET_AREA_DATA:
	    /* ai=MessageData;
	     ret = doAreaGetData(ai); */
	    goto ErrorOut;
	case MessageType_SET_AREA_RANGE:
	   /* ai=MessageData;
	    ret = doSetAreaRange(ai); */
	    goto ErrorOut;
	case MessageType_GET_FILE_LIST:
	    
		for(k=0;k<scene->MaterialCount;++k){
		    mp = &scene->material[k];
		    
		    Files=NULL;
		    
		    if(mp->PatternType==PATTERN_STRIPES){
		        sdata=(struct Stripes *)mp->PatternData;
		        if(sdata)Files=sdata->Files;
		    }else if(mp->PatternType==PATTERN_IMAGE){
		        idata=&mp->ImageData;
		        Files=idata->Files;
		    }
		    		    
			if(!Files)continue;
			
		    isFound=FALSE;
			for(n=0;n<m->FileCount;++n){
			    if(Files == m->FilesList[n]){isFound=TRUE;break;}
			}
			if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
			    m->FilesList[m->FileCount++]=Files;
			}
		}
	    ListObjectMessage(o,&Message);
		goto OK;
	case MessageType_GET_FILE_OWNER:
	
		for(k=0;k<scene->MaterialCount;++k){
		    mp = &scene->material[k];
		    
		    Files=NULL;
		    
		    if(mp->PatternType==PATTERN_STRIPES){
		        sdata=(struct Stripes *)mp->PatternData;
		        if(sdata)Files=sdata->Files;
		    }else if(mp->PatternType==PATTERN_IMAGE){
		        idata=&mp->ImageData;
		        Files=idata->Files;
		    }
		    		    
			if(!Files)continue;
		    
			for(n=0;n<m->FileCount;++n){
			    if(Files == m->FilesList[n])m->FileOwner[n] = TRUE;
			}
		
		}
	    ListObjectMessage(o,&Message);
		goto OK;
	case MessageType_PRINT_FILE_INFORMATION:
		GetWindowName(myIcon,windowName,0);
	    sprintf(WarningBuff,"Window \'%s\'\n%c",windowName,0);
	    WarningBatch(WarningBuff);
	    ListObjectMessage(o,&Message);
	    
		for(k=0;k<scene->MaterialCount;++k){
		    mp = &scene->material[k];
		    
		    Files=NULL;
		    
		    if(mp->PatternType==PATTERN_STRIPES){
		        sdata=(struct Stripes *)mp->PatternData;
		        if(sdata)Files=sdata->Files;
		    }else if(mp->PatternType==PATTERN_IMAGE){
		        idata=&mp->ImageData;
		        Files=idata->Files;
		    }
		    		    
			if(!Files)continue;
			
	    	sprintf(WarningBuff,"  Material %ld \n%c",(long)k,0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(Files);
			
		}
		goto OK;
	}
	InvalRectMyWindow(myIcon);
	/* nDumpIt(myIcon); */
OK:
	ret = 0;
ErrorOut:
	uSetPort(&save);
	return ret;
}
int DefaultScene(struct Scene **scenei)
{
	double xmin,xmax,ymin,ymax,zmin,zmax,vmin,vmax;
	long MaterialCount;
	struct CObjectListStruct *ObjectList;
	struct Sphere lamp;
	struct Material *material;
	struct Block *block;
	long *CellBuff,xsize,ysize,length;
	struct Scene *scene;
	double *zbuff;
	int k,flag,n;
	
	if(*scenei){
	    flag=1;
	    scene = *scenei;	    
	    rangeE(scene,&xmin,&xmax,&ymin,&ymax,&zmin,&zmax,&vmin,&vmax);
	}else{
	    flag=0;
	    scene=(struct Scene *)cMalloc(sizeof(struct Scene),8622);
	    if(scene == NULL)return 1;
	    zerol((char *)scene,sizeof(struct Scene));
	    *scenei=scene;
	    xmin=ymin=zmin=vmin=-400;
	    xmax=ymax=zmax=vmax=400;
	    {
	        unsigned char pal[256*3];
	        getPaletteByName("ps",pal);
	        for(n=0;n<3*256;++n){
	            scene->pd.palette[n]=pal[n];
	        }
	    }
	    scene->pd.sType=0;
	    scene->pd.sPmin=-2;
	    scene->pd.sPmax=10;
	    scene->pd.LabeledPalette=FALSE;	
	    scene->pd.paletteFont=2;
	}

	scene->xResolution=300;
	scene->yResolution=300;

	scene->BackGround=c(1.0,1.,1.);

	scene->Degrees=20.;
	scene->AspectRatio=1.;
	scene->Height=1.;
	scene->MaxLevel=5;
	scene->MacRays=25;

	scene->ClipPlaneDistance=2.;

	scene->ClipPlaneNormal.z=1.;

	scene->TransparencyPasses=4;

	scene->BitsCompression=8;

	scene->AutoFullScreen = FALSE;
	
	scene->Up=p(0.,0.,1.);
	
	scene->Eye=p(xmax+2.*(xmax-xmin),
	                 ymax+2.*(ymax-ymin),
	                 zmax+2.*(zmax-zmin));
	                 
	scene->Target=p(.5*(xmax+xmin),
	                 .5*(ymax+ymin),
	                 .5*(zmax+zmin));


	mstrncpy(scene->sequenceName,"true",255);
	
	scene->sequenceSave=TRUE;

	material=NULL;
	CellBuff = NULL;
	zbuff 	= NULL;
    ObjectList = NULL;
	
	xsize=scene->xResolution;
	ysize=scene->yResolution;
	length=xsize*ysize;
	if(!(CellBuff=(long  *)cMalloc(length*sizeof(long),8623))){
	    sprintf(WarningBuff,"DefaultScene  CellBuff Out of memory Requested : (%ld)\n",length*sizeof(long));
	    WarningBatch(WarningBuff);
	    goto OutOfHere;
	}

	for(n=0;n<length;++n){
	    CellBuff[n] = -1;
	}
	
	if((zbuff=(double *)cMalloc(length*sizeof(double),8624)) == NULL){
	    sprintf(WarningBuff,"DefaultScene  zbuff Out of memory Requested : (%ld)\n",length*sizeof(double));
	    WarningBatch(WarningBuff);
		goto OutOfHere;
	}
		
	
	
	MaterialCount=7+scene->BlockCount;
	
	if(!(material=(struct Material *)cMalloc(MaterialCount*sizeof(struct Material),8628))){
		WarningBatch("DefaultScene out of memory\n");
		goto OutOfHere;
	}
	
	zerol((char *)material,MaterialCount*sizeof(struct Material));
	
	ObjectList=(CObjectListPtr)cMalloc(sizeof(CObjectList),8629);
	if(!ObjectList){
	    WarningBatch("DefaultScene out of memory\n");
	    goto OutOfHere;
	}
	zerol((char *)ObjectList,sizeof(CObjectList));

	
	lamp.radius=.75;
	lamp.material=0;
	lamp.origin=p(scene->Eye.x,scene->Eye.y,scene->Eye.z+.5*(zmax+zmin));
	
	if(!scene->Lights || scene->nLights < 1){
	    scene->nLights=1;
	    scene->Lights=(struct t_light *)cMalloc(scene->nLights*sizeof(struct t_light),8607);
	    if(!scene->Lights){
	    	WarningBatch("DefaultScene out of memory\n");
	        goto OutOfHere;
	    }
	    zerol((char *)scene->Lights,scene->nLights*sizeof(struct t_light));
	    scene->Lights[0].red=1.0;
	    scene->Lights[0].green=1.0;
	    scene->Lights[0].blue=1.0;
	    scene->Lights[0].light_bright=1.0;
	}
	    
	scene->material=material;
	scene->lamp=lamp;
	scene->MaterialCount=MaterialCount;
	scene->ObjectList=ObjectList;
	scene->MaterialUsed=5;
	
	for(k=0;k<MaterialCount;++k){
	   setDefaultMaterial(&material[k]);
	}

	material[0].diffuse=c(1.0,1.0,1.0);

	material[1].diffuse=c(.61,.2,.61);	
	material[1].ColorCount=254;

	material[2].diffuse=c(.61,.2,.61);	
	material[2].ColorCount=0;

	adjustPalette(scene,1);
	adjustPalette(scene,2);
	
   	scene->World.x=scene->Global.x=p(1.0,0.0,0.0);
   	scene->World.y=scene->Global.y=p(0.0,1.0,0.0);
   	scene->World.z=scene->Global.z=p(0.0,0.0,1.0);
   	scene->World.p=scene->Global.p=p(0.0,0.0,0.0);
   	scene->World.Scale_x=scene->Global.Scale_x=1.0;
   	scene->World.Scale_y=scene->Global.Scale_y=1.0;
   	scene->World.Scale_z=scene->Global.Scale_z=1.0;

	scene->ObjectList->scene = scene;

	checkWorld(scene,scene->ObjectList,0);

	if(flag){
 	    for(k=0;k<scene->BlockCount;++k){
 	        block= &scene->block[k];
 	        block->Local=scene->Global;
 	        block->Local.Scale_x=1.0;
 	        block->Local.Scale_y=1.0;
 	        block->Local.Scale_z=1.0;
 	        ObjectListAddBlock(scene,k,1L);
 	    }
	}
	

	scene->pd.ScreenBits=RASTER_24;

	scene->pd.dmin=vmin;
	scene->pd.dmax=vmax;
	
	scene->SmoothNormals=2;


	scene->CellBuff=CellBuff;
	scene->zbuff=zbuff;
	
	return 0;
OutOfHere:

	if(material)cFree((char *)material);
	if(CellBuff)cFree((char *)CellBuff);
	if(zbuff)cFree((char *)zbuff);
	if(ObjectList)cFree((char *)ObjectList);
	return 1;
}
#define Material_Increment 1
int SetMaterialProperty(struct Scene *scene,struct Material **Mat,int mat)
{
	if(!scene || !Mat || !scene->material)return 1;
	if((mat < 0) || (mat >= scene->MaterialCount))return 1;
	*Mat = &scene->material[mat];
	return 0;
}
long getNextMaterial(struct Scene *scene)
{
	struct Material *material;
	long MaterialCount;
	long mat,n;
	
	if(!scene)return 1;
	
	mat=scene->MaterialUsed;
	if(scene->MaterialUsed >= scene->MaterialCount){
	    MaterialCount=scene->MaterialCount;
	    material=scene->material;
	    
	    material=(struct Material *)cRealloc((char *)scene->material,(MaterialCount+Material_Increment)*sizeof(struct Material),1158);
	    if(!material){
	        WarningBatch("getNextMaterial out of Memory\n");
	        return 1;
	    }
	    zerol((char *)&material[scene->MaterialCount],Material_Increment*sizeof(struct Material));
	    for(n=scene->MaterialCount;n<scene->MaterialCount+Material_Increment;++n){
	         setDefaultMaterial(&material[n]);
	    }
	    
	    scene->material=material;
	    
	    scene->MaterialCount += Material_Increment;	    
	    	     
	}
	
	scene->material[mat].ColorCount=254;
	
	++scene->MaterialUsed;
	
	return mat;
}
int setDefaultMaterial(struct Material *material)
{
    if(!material)return 1;
    
	material->diffuse=c(1.0,0.0,0.0);
	material->opacity=1.0;
	material->ambient=c(0.,0.,0.);
	material->ImageData.rmin=0;
	material->ImageData.smin=0;
	material->ImageData.rmax=1;
	material->ImageData.smax=1;
	material->ImageData.ItWas= -1;
	material->ImageData.repeatr=0;
	material->ImageData.repeats=0;
	material->ImageData.cycleFlag=1;
		    
	material->pd.sType=0;
	material->pd.sPmin=-2;
	material->pd.sPmax=10;
	material->pd.LabeledPalette=FALSE;	
	material->pd.paletteFont=2;

	material->pa.ZoneLevel=1;

	getPaletteReal(&material->ColorValue[0],12);

	getPaletteByName("ps",(unsigned char *)&material->pd.palette);
		    
    return 0;
}
int changePalette(struct Scene *scene,int mat)
{
	if(!scene || !scene->material)return 1;
	
	if(mat < 0 || mat >= scene->MaterialCount)return 1;
	
	uMemcpy(scene->material[mat].pd.palette,scene->pd.palette,768);

	fillct(scene->material[mat].ColorValue,scene->pd.palette);
	
	return 0;
}
int adjustPalette(struct Scene *scene,int mat)
{
	double vmax,vmin,cd;
	struct Color *ct;
	double *cv;
	int n;

	if(!scene || !scene->material)return 1;
	
	if(mat < 0 || mat >= scene->MaterialCount)return 1;
	
	
	ct=scene->material[mat].ColorValue;
	cv=scene->material[mat].ColorLevel;
	
	vmin=scene->pd.dmin;
	vmax=scene->pd.dmax;

	if((cd=vmax-vmin) <= 0.)cd = 1.0;

	fillct(ct,scene->pd.palette);
	
	uMemcpy(scene->material[mat].pd.palette,scene->pd.palette,768);

	if(scene->pd.sType == 1){
	    vmin=scene->pd.sPmin;
	    vmax=scene->pd.sPmax;
	    for(n=2;n<255;++n){
	        cv[n]=(double)(vmin*pow(vmax/vmin,(double)(n-2)/252.));
	    }
	   cv[1]=(double)(cv[2]-1.0e33);
	   cv[0]=(double)(cv[2]-1.1e33);
	}else if(scene->pd.sType == 2){
	    vmin=scene->pd.sPmin;
	    vmax=scene->pd.sPmax;
	    for(n=2;n<255;++n){
	        cv[n]=(double)(vmin+(vmax-vmin)*(n-2)/252.);
	    }
	   cv[1]=(double)(cv[2]-1.0e33);
	   cv[0]=(double)(cv[2]-1.1e33);
	}else{	
		double xmin,xmax;
		double ymin,ymax;
		double zmin,zmax;

		rangeE(scene,&xmin,&xmax,&ymin,&ymax,&zmin,&zmax,&vmin,&vmax);
		if(vmin >= vmax){	
		    for(n=0;n<256;++n)cv[n]=(double)((cd*n/255.)+vmin);	
		    cv[1]=(double)(vmin-5.);
		    cv[0]=(double)(vmin-10.);
	    }else{
			for(n=2;n<255;++n){
	        	cv[n]=(double)(vmin+(vmax-vmin)*(n-2)/252.);
			}
			cv[1]=(double)(cv[2]-1.0e33);
	   		cv[0]=(double)(cv[2]-1.1e33);
	    }
	}
	
	return 0;
}
int startPaletteLevels(struct Scene *scene)
{
	long n;	
	
	if(!scene || !scene->material)return 1;
	
	for(n=0;n<scene->MaterialCount;++n){
	    scene->material[n].pd.dmin =  1e60;
	    scene->material[n].pd.dmax = -1e60;
	}
	return 0;
}
int checkPaletteLevels(struct Scene *scene,int mat,double vmin,double vmax)
{
	if(!scene || !scene->material)return 1;
	
	if((mat >= 0) && (mat < scene->MaterialCount)){
	    if(vmin < scene->material[mat].pd.dmin)scene->material[mat].pd.dmin=vmin;
	    if(vmax > scene->material[mat].pd.dmax)scene->material[mat].pd.dmax=vmax;
	}
		
	return 0;
}
int showPaletteLevels(struct Scene *scene)
{
	long n;
	
	if(!scene || !scene->material)return 1;
	sprintf(WarningBuff,"Materials %ld - Auto Level List\n",scene->MaterialCount);
	WarningBatch(WarningBuff);
	for(n=0;n<scene->MaterialCount;++n){
	    if(scene->material[n].pd.dmin > 5e29)continue;
		sprintf(WarningBuff,"    Material %ld - min %g max %g\n",n,
		        scene->material[n].pd.dmin,scene->material[n].pd.dmax);
		WarningBatch(WarningBuff);
	}
		
	return 0;
}
int setPaletteLevels(struct Scene *scene)
{
	long n;
	
	if(!scene || !scene->material)return 1;
	
	for(n=0;n<scene->MaterialCount;++n){
	    if(scene->material[n].pd.dmin > 5e29)continue;
	    adjustPalette2(scene,n);
	    if(scene->material[n].pd.LabeledPalette){
	        scene->pd=scene->material[n].pd;
	    }
	}
		
	return 0;
}
int adjustPalette2(struct Scene *scene,int mat)
{
	struct paletteDraw *pd;
	double vmax,vmin,cd;
	double *cv;
	int n;

	if(!scene || !scene->material)return 1;
	
	if(mat < 0 || mat >= scene->MaterialCount)return 1;

	cv=scene->material[mat].ColorLevel;
	
	pd = &scene->material[mat].pd;
	
	vmin=pd->dmin;
	vmax=pd->dmax;

	if((cd=vmax-vmin) <= 0.)cd = 1.0;
		
	if(pd->sType == 1){
	    vmin=pd->sPmin;
	    vmax=pd->sPmax;
	    for(n=2;n<255;++n){
	        cv[n]=(double)(vmin*pow(vmax/vmin,(double)(n-2)/252.));
	    }
	   cv[1]=(double)(cv[2]-1.0e33);
	   cv[0]=(double)(cv[2]-1.1e33);
	}else if(pd->sType == 2){
	    vmin=pd->sPmin;
	    vmax=pd->sPmax;
	    for(n=2;n<255;++n){
	        cv[n]=(double)(vmin+(vmax-vmin)*(n-2)/252.);
	    }
	   cv[1]=(double)(cv[2]-1.0e33);
	   cv[0]=(double)(cv[2]-1.1e33);
	}else{	
		if(vmin >= vmax){	
		    for(n=0;n<256;++n)cv[n]=(double)((cd*n/255.)+vmin);	
		    cv[1]=(double)(vmin-5.);
		    cv[0]=(double)(vmin-10.);
	    }else{
			for(n=2;n<255;++n){
	        	cv[n]=(double)(vmin+(vmax-vmin)*(n-2)/252.);
			}
			cv[1]=(double)(cv[2]-1.0e33);
	   		cv[0]=(double)(cv[2]-1.1e33);
	    }
	}
	
	return 0;
}
static int doLoadImages(IconPtr myIcon)
{
	struct FileInfo2 *Files2;
	struct FileList *Files;
	int TypeList[]={HDF_DISK,HDF_MEMORY,SDS2D_DISK,SDS2D_MEMORY,
	                SDS3D_MEMORY,SDS3D_DISK,
	                VRML_MEMORY,VRML_DISK,SHOW_MEMORY,SHOW_DISK,
					SAGE2D_DISK,SAGE3D_DISK,SAGE1D_DISK,
					SAGE1D_BLOCKED,SAGE2D_BLOCKED,SAGE3D_BLOCKED,
					STL_DISK,0};
	renderPtr r;
	int iret;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	
	Files=uOpenFileDialog(myIcon,TypeList);		
	if(!Files)return 1;
	
	Files2=FilesOpenList(Files);
	if(!Files2)return 1;

    if(Files2->fileClass == SDS2D_FILES ||
       Files2->fileClass == HDF_FILES   ||
       Files2->fileClass == VRML_FILES  ||
       Files2->fileClass == SDS3D_FILES ||
       Files2->fileClass == PIO1D_FILES ||
       Files2->fileClass == PIO2D_FILES ||
       Files2->fileClass == PIO3D_FILES ||
       Files2->fileClass == PIO1D_BLK_FILES ||
       Files2->fileClass == PIO2D_BLK_FILES ||
       Files2->fileClass == PIO3D_BLK_FILES ||
       Files2->fileClass == SHOW_FILES  ||
       Files2->fileClass == STL_FILES
       ){       
        iret=ObjectListAddFiles(Files2,r->scene);
	    if(iret){
	        if(iret != 7)return 1;
	    }else{
	        doObjects(myIcon);
	    }
	}
	
	if(r && r->scene){
		if(r->scene->AutoCenterScreen){
			RenderSceneSetTime(r->scene);
			r->scene->AutoCenterScreenDone=FALSE;
		}
	}
	
	if(r)RenderScene(r);
	    
	return 0;
}
static int doClose(IconPtr myIcon)
{
	renderPtr r;
	
	if(!myIcon)return 1;
	
	stopLanguage();	
	
	r=(renderPtr)myIcon->DWindow;
	
	
	if(r){
	    struct Scene *scene;
	    
	    CleanTracing();
	    
	    if(r->buffer)cFree((char *)r->buffer);
	    r->buffer=NULL;

	    myIcon->Closing=TRUE;

		freeKeyFrames(&r->CKeyFrames,0);
		
		pushpopEND(r->popStack);
		
	    scene=r->scene;
	    if(scene){
	        scene->Closing=TRUE;
	   		CleanTracing2(scene);
	        freeScene(scene);
	    }
	    cFree((char *)r);
	    myIcon->DWindow=NULL;
	}
	
	myIcon=RemoveIcon(myIcon);
	    
	return 0;
}
static int doObjects(IconPtr myIcon)
{
	static uDialogGroup Items[] =

        {
      /*  0 */      {"Objects", {29,2,55,15},uDialogString,uDialogNone},
      /*  1 */      {"Objects List",{2,18,115,143},uDialogScrollList,uDialogNone},
      /*  2 */      {"group", {1,175,70,20},uDialogButton,uDialogNone},
      /*  3 */      {"ungroup", {78,175,72,20},uDialogButton,uDialogNone},
      /*  4 */      {"move", {157,175,72,20},uDialogButton,uDialogNone},
      /*  5 */      {"render", {1,200,70,20},uDialogButton,uDialogNone},
      /*  6 */      {"reset", {78,200,72,20},uDialogButton,uDialogNone},
      /*  7 */      {"reflect x", {157,200,72,20},uDialogButton,uDialogNone},
      /*  8 */      {"dup", {1,225,70,20},uDialogButton,uDialogNone},
      /*  9 */      {"delete", {78,225,72,20},uDialogButton,uDialogNone},
      /* 10 */      {"reflect y", {157,225,72,20},uDialogButton,uDialogNone},
      /* 11 */      {"Ok", {1,250,70,20},uDialogButton,uDialogNone},
      /* 12 */      {"Cancel", {78,250,72,20},uDialogButton,uDialogNone},
      /* 13 */      {"reflect z", {157,250,72,20},uDialogButton,uDialogNone},
      
      /* 14 */      {"origin", {317,9,77,20},uDialogString,uDialogNone},
      /* 15 */      {"0", {397,10,55,20},uDialogText,uDialogGetDouble},
      /* 16 */      {"0", {457,10,55,20},uDialogText,uDialogGetDouble},
      /* 17 */      {"0", {517,10,55,20},uDialogText,uDialogGetDouble},
      /* 18 */      {"angles", {317,34,77,20},uDialogString,uDialogNone},
      /* 19 */      {"0", {397,35,55,20},uDialogText,uDialogGetDouble},
      /* 20 */      {"0", {457,35,55,20},uDialogText,uDialogGetDouble},
      /* 21 */      {"0", {517,35,55,20},uDialogText,uDialogGetDouble},
      /* 22 */      {"scales", {317,59,77,20},uDialogString,uDialogNone},
      /* 23 */      {"0", {397,60,55,20},uDialogText,uDialogGetDouble},
      /* 24 */      {"0", {457,60,55,20},uDialogText,uDialogGetDouble},
      /* 25 */      {"0", {517,60,55,20},uDialogText,uDialogGetDouble},
      /* 26 */      {"move", {317,209,77,20},uDialogString,uDialogNone},
      /* 27 */      {"0", {397,210,55,20},uDialogText,uDialogGetDouble},
      /* 28 */      {"0", {457,210,55,20},uDialogText,uDialogGetDouble},
      /* 29 */      {"0", {517,210,55,20},uDialogText,uDialogGetDouble},
      
      /* 30 */      {"mat", {317,234,77,20},uDialogString,uDialogNone},
      /* 31 */      {"0", {397,235,55,20},uDialogText,uDialogGetLong},
      
      
      /* cone */
      
      /* 32 */      {"mat", {317,84,77,20},uDialogString,uDialogNone},
      /* 33 */      {"0", {397,85,55,20},uDialogText,uDialogGetLong},
      /* 34 */      {"o", {317,134,77,20},uDialogString,uDialogNone},
      /* 35 */      {"0", {397,135,55,20},uDialogText,uDialogGetDouble},
      /* 36 */      {"0", {457,135,55,20},uDialogText,uDialogGetDouble},
      /* 37 */      {"0", {517,135,55,20},uDialogText,uDialogGetDouble},
      /* 38 */      {"rtop", {317,159,77,20},uDialogString,uDialogNone},
      /* 39 */      {"0", {397,160,55,20},uDialogText,uDialogGetDouble},
      /* 40 */      {"rbottom", {457,160,55,20},uDialogString,uDialogNone},
      /* 41 */      {"0", {517,160,55,20},uDialogText,uDialogGetDouble},
      /* 42 */      {"length", {317,184,77,20},uDialogString,uDialogNone},
      /* 43 */      {"0", {397,185,55,20},uDialogText,uDialogGetDouble},
      
      /* 44 */      {"nr", {317,110,77,20},uDialogString,uDialogNone},
      /* 45 */      {"0", {397,110,55,20},uDialogText,uDialogGetLong},
      /* 46 */      {"nl", {472,110,27,16},uDialogString,uDialogNone},
      /* 47 */      {"0", {517,110,55,20},uDialogText,uDialogGetLong},
      
      
      /* torus */
      
      /* 48 */      {"mat", {317,84,77,20},uDialogString,uDialogNone},
      /* 49 */      {"0", {397,85,55,20},uDialogText,uDialogGetLong},
      /* 50 */      {"o", {317,134,77,20},uDialogString,uDialogNone},
      /* 51 */      {"0", {397,135,55,20},uDialogText,uDialogGetDouble},
      /* 52 */      {"0", {457,135,55,20},uDialogText,uDialogGetDouble},
      /* 53 */      {"0", {517,135,55,20},uDialogText,uDialogGetDouble},
      /* 54 */      {"rbig", {317,159,77,20},uDialogString,uDialogNone},
      /* 55 */      {"0", {397,160,55,20},uDialogText,uDialogGetDouble},
      /* 56 */      {"rsmall", {457,160,55,20},uDialogString,uDialogNone},
      /* 57 */      {"0", {517,160,55,20},uDialogText,uDialogGetDouble},
      
      /* 58 */      {"nrr", {317,110,77,20},uDialogString,uDialogNone},
      /* 59 */      {"0", {397,110,55,20},uDialogText,uDialogGetLong},
      /* 60 */      {"nrl", {472,110,27,16},uDialogString,uDialogNone},
      /* 61 */      {"0", {517,110,55,20},uDialogText,uDialogGetLong},
      
      
      /* disk */
      
      /* 62 */      {"mat", {317,84,77,20},uDialogString,uDialogNone},
      /* 63 */      {"0", {397,85,55,20},uDialogText,uDialogGetLong},
      /* 64 */      {"o", {317,134,77,20},uDialogString,uDialogNone},
      /* 65 */      {"0", {397,135,55,20},uDialogText,uDialogGetDouble},
      /* 66 */      {"0", {457,135,55,20},uDialogText,uDialogGetDouble},
      /* 67 */      {"0", {517,135,55,20},uDialogText,uDialogGetDouble},
      /* 68 */      {"rin", {317,159,77,20},uDialogString,uDialogNone},
      /* 69 */      {"0", {397,160,55,20},uDialogText,uDialogGetDouble},
      /* 70 */      {"rout", {457,160,55,20},uDialogString,uDialogNone},
      /* 71 */      {"0", {517,160,55,20},uDialogText,uDialogGetDouble},
      
      /* 72 */      {"nr", {317,110,77,20},uDialogString,uDialogNone},
      /* 73 */      {"0", {397,110,55,20},uDialogText,uDialogGetLong},
      /* 74 */      {"nt", {472,110,27,16},uDialogString,uDialogNone},
      /* 75 */      {"0", {517,110,55,20},uDialogText,uDialogGetLong},
      
      
      /* sphere */
      
      /* 76 */      {"mat", {317,84,77,20},uDialogString,uDialogNone},
      /* 77 */      {"0", {397,85,55,20},uDialogText,uDialogGetLong},
      /* 78 */      {"o", {317,134,77,20},uDialogString,uDialogNone},
      /* 79 */      {"0", {397,135,55,20},uDialogText,uDialogGetDouble},
      /* 80 */      {"0", {457,135,55,20},uDialogText,uDialogGetDouble},
      /* 81 */      {"0", {517,135,55,20},uDialogText,uDialogGetDouble},
      /* 82 */      {"r", {317,159,77,20},uDialogString,uDialogNone},
      /* 83 */      {"0", {397,160,55,20},uDialogText,uDialogGetDouble},
      
      /* 84 */      {"nx", {317,110,77,20},uDialogString,uDialogNone},
      /* 85 */      {"0", {397,110,55,20},uDialogText,uDialogGetLong},
      /* 86 */      {"ny", {472,110,27,16},uDialogString,uDialogNone},
      /* 87 */      {"0", {517,110,55,20},uDialogText,uDialogGetLong},
      
      /* block */
      
      /* 88 */      {"nodes", {317,85,77,20},uDialogString,uDialogNone},
      /* 89 */      {"0", {397,85,55,20},uDialogText,uDialogGetLong},
      
      /* 90 */      {"cells", {463,85,42,18},uDialogString,uDialogNone},
      /* 91 */      {"0", {517,85,55,20},uDialogText,uDialogGetLong},

      /* 92 */      {"x", {317,110,77,20},uDialogString,uDialogNone},
      /* 93 */      {"0", {397,110,55,20},uDialogText,uDialogNone},
      /* 94 */      {"0", {457,110,55,20},uDialogText,uDialogNone},
      
      /* 95 */      {"y", {317,134,77,20},uDialogString,uDialogNone},
      /* 96 */      {"0", {397,135,55,20},uDialogText,uDialogNone},
      /* 97 */      {"0", {457,135,55,20},uDialogText,uDialogNone},
      
      /* 98 */      {"z", {317,160,77,20},uDialogString,uDialogNone},
      /* 99 */      {"0", {397,160,55,20},uDialogText,uDialogNone},
      /*100 */      {"0", {457,160,55,20},uDialogText,uDialogNone},
      
      /*101 */      {"v", {317,185,77,20},uDialogString,uDialogNone},
      /*102 */      {"0", {397,185,55,20},uDialogText,uDialogNone},
      /*103 */      {"0", {457,185,55,20},uDialogText,uDialogNone},
      
      /*104 */      {"mat", {317,235,77,20},uDialogString,uDialogNone},
      /*105 */      {"0", {397,235,55,20},uDialogText,uDialogGetLong},
      
      /* hdfext */
      
      /*106 */      {"Levels", {265,2,51,14},uDialogString,uDialogNone},
      /*107 */      {"Levels",{232,18,115,143},uDialogScrollList,uDialogNone},
       
      /*108 */     {"range data", {336,220,90,20},uDialogButton,uDialogNone},     
      /*109 */     {"Files",{153,1,44,16},uDialogString,uDialogNone},
      /*110 */     {"File",{117,18,115,143},uDialogScrollList,uDialogNone},
     
      /*111 */      {"mat", {317,10,43,17},uDialogString,uDialogNone},
      /*112 */      {"0", {374,10,55,20},uDialogText,uDialogGetLong},

      /*113 */      {"vscale", {317,35,47,17},uDialogString,uDialogNone},
      /*114 */      {"0", {374,35,55,20},uDialogText,uDialogGetDouble},
      
      /*115 */      {"xmin", {317,60,45,19},uDialogString,uDialogNone},
      /*116 */      {"0", {374,60,55,20},uDialogText,uDialogGetLong},

      /*117 */      {"xmax", {317,85,45,19},uDialogString,uDialogNone},
      /*118 */      {"0", {374,85,55,20},uDialogText,uDialogGetLong},

      /*119 */      {"ymin", {317,110,45,19},uDialogString,uDialogNone},
      /*120 */      {"0", {374,110,55,20},uDialogText,uDialogGetLong},
      
      /*121 */      {"ymax", {317,135,45,19},uDialogString,uDialogNone},
      /*122 */      {"0", {374,135,55,20},uDialogText,uDialogGetLong},
      
      /*123 */      {"dmin", {317,160,45,19},uDialogString,uDialogNone},
      /*124 */      {"0", {374,160,55,20},uDialogText,uDialogGetDouble},
      
      /*125 */      {"dmax", {317,185,45,19},uDialogString,uDialogNone},
      /*126 */      {"0", {374,185,55,20},uDialogText,uDialogGetDouble},
      
      /*127 */      {"tzero", {451,10,43,17},uDialogString,uDialogNone},
      /*128 */      {"0", {508,10,55,20},uDialogText,uDialogGetLong},
      
      /*129 */      {"thold", {451,35,47,17},uDialogString,uDialogNone},
      /*130 */      {"0", {508,35,55,20},uDialogText,uDialogGetLong},
      
      /*131 */      {"repeat", {451,60,45,19},uDialogString,uDialogNone},
      /*132 */      {"0", {508,60,55,20},uDialogText,uDialogGetLong},
      
      /*133 */      {"hold", {459,99,106,18},uDialogButtonRadio,uDialogGetLong},
      /*134 */      {"cycle", {459,122,106,18},uDialogButtonRadio,uDialogGetLong},
      /*135 */      {"hide", {459,146,106,18},uDialogButtonRadio,uDialogGetLong},
      
      /*136 */      {"black zero", {463,173,106,18},uDialogCheck,uDialogGetLong},
      /*137 */      {"white 255", {463,193,106,18},uDialogCheck,uDialogGetLong},
      
      /* VrmlList use the hdfext stuff */
      
      /* hdfrot */
      
      /*138 */      {"mat", {359,29,43,17},uDialogString,uDialogNone},
      /*139 */      {"0", {416,30,55,20},uDialogText,uDialogGetLong},
      /*140 */      {"smooth", {359,54,54,19},uDialogString,uDialogNone},
      /*141 */      {"0", {416,55,55,20},uDialogText,uDialogGetLong},
      /*142 */      {"angle", {359,79,47,17},uDialogString,uDialogNone},
      /*143 */      {"0", {416,80,55,20},uDialogText,uDialogGetDouble},
      /*144 */      {"xmin", {359,104,45,19},uDialogString,uDialogNone},
      /*145 */      {"0", {416,105,55,20},uDialogText,uDialogGetLong},
      /*146 */      {"xmax", {359,129,45,19},uDialogString,uDialogNone},
      /*147 */      {"0", {416,130,55,20},uDialogText,uDialogGetLong},
      /*148 */      {"ymin", {359,154,45,19},uDialogString,uDialogNone},
      /*149 */      {"0", {416,155,55,20},uDialogText,uDialogGetLong},
      /*150 */      {"ymax", {359,179,45,19},uDialogString,uDialogNone},
      /*151 */      {"0", {416,180,55,20},uDialogText,uDialogGetLong},
      /*152 */      {"dmin", {359,204,45,19},uDialogString,uDialogNone},
      /*153 */      {"0", {416,205,80,20},uDialogText,uDialogGetDouble},
      /*154 */      {"dmax", {359,229,45,19},uDialogString,uDialogNone},
      /*155 */      {"0", {416,230,80,21},uDialogText,uDialogGetDouble},

      /*156 */      {"slices", {476,32,43,17},uDialogString,uDialogNone},
      /*157 */      {"0", {533,31,55,20},uDialogText,uDialogGetLong},
      /*158 */      {"tzero", {476,57,43,17},uDialogString,uDialogNone},
      /*159 */      {"0", {533,56,55,20},uDialogText,uDialogGetLong},
      /*160 */      {"thold", {476,82,47,17},uDialogString,uDialogNone},
      /*161 */      {"0", {533,81,55,20},uDialogText,uDialogGetLong},
      /*162 */      {"repeat", {476,107,45,19},uDialogString,uDialogNone},
      /*163 */      {"0", {533,106,55,20},uDialogText,uDialogGetLong},
      
      /*164 */      {"hold", {499,132,106,18},uDialogButtonRadio,uDialogGetLong},
      /*165 */      {"cycle", {499,155,106,18},uDialogButtonRadio,uDialogGetLong},
      /*166 */      {"hide", {499,179,106,18},uDialogButtonRadio,uDialogGetLong},
      /*167 */      {"black zero", {499,206,106,18},uDialogCheck,uDialogGetLong},
      /*168 */      {"white 255", {499,226,106,18},uDialogCheck,uDialogGetLong},
      /*169 */      {"step levels", {499,249,106,18},uDialogCheck,uDialogGetLong},
      /*170 */      {"no shift", {499,269,106,18},uDialogCheck,uDialogGetLong},
      
      /*171 */      {"Edit Text", {239,173,106,15},uDialogText,uDialogGetDouble},
      /*172 */      {"enter", {236,198,53,20},uDialogButton,uDialogNone},
      /*173 */      {"delete", {295,198,59,20},uDialogButton,uDialogNone}, 
      /*174 */      {"range data", {378,263,90,20},uDialogButton,uDialogNone},
      /*175 */      {"rotate x", {236,226,106,18},uDialogButtonRadio,uDialogGetLong},
      /*176 */      {"rotate y", {236,248,106,18},uDialogButtonRadio,uDialogGetLong},
      /*177 */      {"Rays", {236,228,53,20},uDialogButton,uDialogNone},
      
      /*178 */      {"mat", {370,20,75,20},uDialogString,uDialogNone},
      /*179 */      {"0", {430,20,105,20},uDialogText,uDialogGetLong},
      /*180 */      {"radius", {370,45,75,20},uDialogString,uDialogNone},
      /*181 */      {"0", {430,45,105,20},uDialogText,uDialogGetDouble},
      /*182 */      {"nx", {370,95,75,20},uDialogString,uDialogNone},
      /*183 */      {"0", {430,95,105,20},uDialogText,uDialogGetLong},
      /*184 */      {"ny", {370,120,75,20},uDialogString,uDialogNone},
      /*185 */      {"0", {430,120,105,20},uDialogText,uDialogGetLong},
      /*186 */      {"step", {370,145,75,20},uDialogString,uDialogNone},
      /*187 */      {"0", {430,145,105,20},uDialogText,uDialogGetDouble},
      /*188 */      {"r-def", {370,70,75,20},uDialogString,uDialogNone},
      /*189 */      {"0", {430,70,105,20},uDialogText,uDialogGetDouble},
      /*190 */      {"lines", {370,225,105,20},uDialogButtonRadio,uDialogGetLong},
      /*191 */      {"spheres", {370,245,105,20},uDialogButtonRadio,uDialogGetLong},
      
      /*192 */      {"mat", {360,20,75,20},uDialogString,uDialogNone},
      /*193 */      {"0", {420,20,120,20},uDialogText,uDialogGetLong},
      /*194 */      {"vscale", {360,45,75,20},uDialogString,uDialogNone},
      /*195 */      {"0", {420,45,120,20},uDialogText,uDialogGetDouble},
      
      /*196 */      {"dmin",{359,204,45,19},uDialogString,uDialogNone},
      /*197 */      {"0",  {416,205,120,20},uDialogText,uDialogGetDouble},
      
      /*198 */      {"dmax", {359,229,45,19},uDialogString,uDialogNone},
      /*199 */      {"0", {416,230,120,21},uDialogText,uDialogGetDouble},
      
      /*200 */     {"select data", {356,170,90,20},uDialogButton,uDialogNone},     
      /*201 */     {"select area", {456,170,90,20},uDialogButton,uDialogNone},  
      
      /*202 */      {"smooth", {360,45,75,20},uDialogString,uDialogNone},
      /*203 */      {"0", {420,45,105,20},uDialogText,uDialogGetLong},
      
      /*204 */      {"Angle", {360,70,75,20},uDialogString,uDialogNone},
      /*205 */      {"0", {420,70,105,20},uDialogText,uDialogGetDouble},
      
      /*206 */      {"Slices", {360,95,75,20},uDialogString,uDialogNone},
      /*207 */      {"0",{420,95,105,20},uDialogText,uDialogGetLong},
      
      /*208 */     {"select data", {356,170,90,20},uDialogButton,uDialogNone},     
      /*209 */     {"select area", {456,170,90,20},uDialogButton,uDialogNone},  
      
         
      /*210 */     {"select data", {356,170,90,20},uDialogButton,uDialogNone},     
      /*211 */     {"select volume", {456,170,110,20},uDialogButton,uDialogNone},  
      /*212 */     {"select paint", {456,140,110,20},uDialogButton,uDialogNone},  
      /*213 */     {"use paint", {356,140,90,20},uDialogCheck,uDialogGetLong},  
      

      /*214 */      {"cut min",{360,70,75,20},uDialogString,uDialogNone},
      /*215 */      {"0",      {420,70,120,20},uDialogText,uDialogGetDouble},
      
      /*216 */      {"cut max", {360,95,75,20},uDialogString,uDialogNone},
      /*217 */      {"0",       {420,95,120,21},uDialogText,uDialogGetDouble},

      /*218 */      {"scale data", {478,263,90,20},uDialogButton,uDialogNone},
         
         
      /*219 */       {"item", {370,170,75,20},uDialogString,uDialogNone},
      /*220 */      {"2", {430,170,105,20},uDialogText,uDialogGetLong},
        
      /*221 */       {"name", {370,195,75,20},uDialogString,uDialogNone},
      /*222 */      {"2", {430,195,105,20},uDialogText,uDialogNone},
        
      /*223 */      {"mat", {370,20,75,20},uDialogString,uDialogNone},
      /*224 */      {"0", {430,20,105,20},uDialogText,uDialogGetLong},
      /*225 */      {"radius", {370,45,75,20},uDialogString,uDialogNone},
      /*226 */      {"0", {430,45,105,20},uDialogText,uDialogGetDouble},
      /*227 */      {"nx", {370,95,75,20},uDialogString,uDialogNone},
      /*228 */      {"0", {430,95,105,20},uDialogText,uDialogGetLong},
      /*229 */      {"ny", {370,120,75,20},uDialogString,uDialogNone},
      /*230 */      {"0", {430,120,105,20},uDialogText,uDialogGetLong},
      /*231 */      {"steps", {370,70,75,20},uDialogString,uDialogNone},
      /*232 */      {"0", {430,70,105,20},uDialogText,uDialogGetLong},
      /*233 */      {"write path", {360,245,90,20},uDialogButton,uDialogNone},     
      /*234 */      {"smooth", {360,145,75,20},uDialogString,uDialogNone},
      /*235 */      {"0", {430,145,105,20},uDialogText,uDialogGetLong},
      /*236 */      {"hide", {360,220,106,18},uDialogCheck,uDialogGetLong},
      /*237 */      {"cyclic smooth", {360,195,120,18},uDialogCheck,uDialogGetLong},
      /*238 */      {"tilt", {360,170,75,20},uDialogString,uDialogNone},
      /*239 */      {"0", {430,170,105,20},uDialogText,uDialogGetDouble},
      /*240 */      {"write", {356,120,90,20},uDialogCheck,uDialogGetLong},
      
      /*241 */      {"lines", {250,205,105,20},uDialogButtonRadio,uDialogGetLong},
      /*242 */      {"spheres", {250,225,105,20},uDialogButtonRadio,uDialogGetLong},
      /*243 */      {"cones", {250,245,105,20},uDialogButtonRadio,uDialogGetLong},
      /*244 */      {"stream grad", {250,185,105,20},uDialogCheck,uDialogGetLong},  

      /*245 */      {"steps", {250,145,75,20},uDialogString,uDialogNone},
      /*246 */      {"0",     {310,145,55,20},uDialogText,uDialogGetLong},
      /*247 */      {"show normals", {457,234,115,20},uDialogCheck,uDialogGetLong},  
      
      /*248 */      {"Mult", {250,145,75,20},uDialogString,uDialogNone},
      /*249 */      {"0",    {310,145,55,20},uDialogText,uDialogGetDouble},
      
      /*250 */      {"Stress", {250,115,75,20},uDialogString,uDialogNone},
      /*251 */      {"0",      {310,115,55,20},uDialogText,uDialogGetDouble},
      
	  /*252 */      {"passes", {250,115,75,20},uDialogString,uDialogNone},
      /*253 */      {"0",      {310,115,55,20},uDialogText,uDialogGetLong},
        
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,594,305};
 	
        
	struct Scene *scene;
	CObjectListPtr o;
	renderPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
    int reti;
    int ret;
    long n;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	o=scene->ObjectList;
	if(!o)return 1;
	
	scene->itIs = -1;
		
	reti=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Objects",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	ret=uDialogSet(uList,0L,
	              
	          	  uDialogSetItem,175L,
	          	  uDialogSetRadioGroup,1,
	          	  
	          	  uDialogSetItem,176L,
	          	  uDialogSetRadioGroup,1,
                    
	              uDialogSetMyIcon,myIcon,	              
	              uDialogSetAction,doObjectAction,	              
	              uDialogSetStart,doObjectStart,
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(ret)goto ErrorOut;	

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
    
	SaveObjectData(uList,scene->itIs);	
    
	for(n=0;n<o->oSave;++n){
		CObjectPtr Current;
		if((Current=o->oOld[n]) == NULL)continue;
		(*Current->killObject)(Current);
		o->oOld[n]=NULL;
	}
	o->oSave=0;
        		       
	reti=0;
	
ErrorOut:

    if(reti){    
		for(n=0;n<o->oCount;++n){
			CObjectPtr Current;
			if((Current=o->oCurrent[n]) == NULL)continue;
			(*Current->killObject)(Current);
			o->oCurrent[n]=NULL;
		}
		for(n=0;n<o->oSave;++n){
			o->oCurrent[n]=o->oOld[n];
			if(o->oCurrent[n])o->oCurrent[n]->ItWas = -1;
			o->oOld[n]=NULL;
		}
		o->oCount=o->oSave;
		o->oSave=0;
		checkWorld(scene,o,1);
    }
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
    
	if(r){
		if(scene->AutoCenterScreen){
			RenderSceneSetTime(scene);
			scene->AutoCenterScreenDone=FALSE;
		}
	    RenderScene(r);
	}
	
	return reti;
}
int RenderAddPaint(CSds2dExtPtr s,struct FileList *Files)
 {
 	char buff[256];
	struct FileInfo2 *Files2;
	int TypeList1[]={SDS2D_DISK,SDS2D_MEMORY,0};
	int TypeList2[]={HDF_DISK,HDF_MEMORY,0};
	int TypeList3[]={SDS3D_DISK,SDS3D_MEMORY,0};
 	static int Count;
 	
 	if(!s)return 1;
 	

 	if(s->type == G_EXTRUDE_SDS2D || s->type == G_ROTATE_SDS2D){
		if(!Files)Files=uOpenFileDialog(NULL,TypeList1);		
		if(!Files)return 1;
	    sprintf(buff,"CSDSExtPaint_%03d",Count++);	
		
    	mstrncat(buff,".b",255);
 	}else if(s->type == G_EXTRUDE_HDF || s->type == G_ROTATE_HDF){
		if(!Files)Files=uOpenFileDialog(NULL,TypeList2);		
		if(!Files)return 1;
	    sprintf(buff,"CSDSExtPaint_%03d",Count++);	
 	}else if(s->type == G_LEVEL_SDS3D){
		if(!Files)Files=uOpenFileDialog(NULL,TypeList3);		
		if(!Files)return 1;
	    sprintf(buff,"CSD3SExtPaint_%03d",Count++);	
 	}else{
 		return 1;
 	}
 	
	Files2=FilesOpenList(Files);
	if(!Files2)return 1;
 	
 	s->Files2=Files2;
 	
    mstrncpy(s->PaintName,buff,255);
    if(FileManagerAddToList(Files2,s->PaintName))return 1;
 	
 	return 0;
 }

static int doObjectAction(struct uDialogStruct *uList)
{
	struct ScrollList *scroll,*List2;
	IconPtr myIcon;	
	struct Scene *scene;
    CObjectListPtr o;
	renderPtr r;
	uDialogReturn *uR;
	long itWas;
	int reti;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	o=scene->ObjectList;
	if(!o)return 1;
	
	ret = 1;
	uR=NULL;

	reti=uDialogSet(uList,1L,
          uDialogGetScrollList,&scroll,
          
          uDialogSetItem,107L,
          uDialogGetScrollList,&List2,
          
          uDialogSetDone,uDialogSetDone          
    );
    
	if(reti) goto ErrorOut;
	
    uR=uDialogGetReturn(uList);
    if(!uR)goto ErrorOut;
	
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 1){
	   ClickObjectList(uList);	   
	}else if(uList->actionItem == 2){
	   groupObjects(scene,uList);	   
	}else if(uList->actionItem == 3){
	   ungroupObjects(scene,uList);	   
	}else if(uList->actionItem == 4){
		SaveObjectData(uList,scene->itIs);	
	    checkWorld(scene,o,1);
		moveObjects(scene,uList,scene->xmove,scene->ymove,scene->zmove);	   
	}else if(uList->actionItem == 5){
		SaveObjectData(uList,scene->itIs);	
	    checkWorld(scene,o,1);
	    RenderScene(r);	
	}else if(uList->actionItem == 6){
		SaveObjectData(uList,scene->itIs);	
	    checkWorld(scene,o,1);
	   	reset(scene,uList);	   
	}else if(uList->actionItem == 7){
	    duplicate(scene,uList,1);
	}else if(uList->actionItem == 8){
	    duplicate(scene,uList,0);
	}else if(uList->actionItem == 9){
	    deleteo(scene,uList);
	}else if(uList->actionItem == 10){
	    duplicate(scene,uList,2);
	}else if(uList->actionItem == 13){
	    duplicate(scene,uList,3);
	}else if(uList->actionItem == 233){
	    CFlightPathPtr s;
	    long n;

		itWas=ScrollListFirstSelect(scroll);
		if(itWas < 0)goto ErrorOut;
		SaveObjectData(uList,scene->itIs);	
		s=(CFlightPathPtr)o->oCurrent[itWas];
		sprintf(WarningBuff,"SplineSteps %ld",s->SplineSteps);
	    WarningBatchFast(WarningBuff);
		sprintf(WarningBuff,"PassSmooth %ld",s->PassSmooth);
	    WarningBatchFast(WarningBuff);
		sprintf(WarningBuff,"CyclicSmooth %ld",s->CyclicSmooth);
	    WarningBatchFast(WarningBuff);
		sprintf(WarningBuff,"tilt %g",scene->tilt);
		WarningBatchFast(WarningBuff);
	    for(n=0;n<s->flightCount;++n){
			sprintf(WarningBuff,"Step %04ld eye %g %g %g",
	             0L,s->flightPath[n].x,s->flightPath[n].y,s->flightPath[n].z
	             );
	    	WarningBatchFast(WarningBuff);
	    }
		WarningBatchAtEnd(WarningBuff);        
	}else if(uList->actionItem == 200 || uList->actionItem == 208 ||
	         uList->actionItem == 210 || uList->actionItem == 212){	
	    CSds2dExtPtr s;
		int type;
		

		itWas=ScrollListFirstSelect(scroll);
		if(itWas < 0)goto ErrorOut;
		s=(CSds2dExtPtr)o->oCurrent[itWas];
		
		
		if(uList->actionItem == 212 && (s->type == G_EXTRUDE_SDS2D ||
		   s->type == G_EXTRUDE_HDF ||  s->type == G_ROTATE_SDS2D  ||
		   s->type == G_ROTATE_HDF  ||  s->type == G_LEVEL_SDS3D)){
		     if(RenderAddPaint(s,NULL))goto ErrorOut;
		     goto Ok;
		}

		
		
	    if(s->type != G_EXTRUDE_PIO2D && s->type != G_ROTATE_PIO2D &&
	       s->type != G_LEVEL_PIO3D   && s->type != G_EXTRUDE_PIO3D &&
	       s->type != G_ROTATE_SDS2D  && s->type != G_ROTATE_HDF  &&
	       s->type != G_LEVEL_SDS3D   && s->type != G_EXTRUDE_HDF &&
	       s->type != G_EXTRUDE_SDS2D && s->type != G_STREAMLINE_PIO3D
	       )goto ErrorOut;
	    
	    type = MessageType_DO_SELECTION_DIALOG;
		if(SendMessageByName(s->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
		
		if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
			goto ErrorOut;
		}
		
		if(uList->actionItem == 212){
			if(s->sd.pioName[0]){
	    		mstrncpy(s->pioNamePaint,s->sd.pioName,255);
	    		s->pioIndexPaint=s->sd.pioIndex;			    
			}		
		}else{
			if(s->sd.pioName[0]){
	    		mstrncpy(s->pioName,s->sd.pioName,255);
	    		s->pioIndex=s->sd.pioIndex;			    
			}		
		}				
	}else if(uList->actionItem == 201 || uList->actionItem == 209  || 
	         uList->actionItem == 211){	
	    CPio2dExtPtr s;
		int type;

		itWas=ScrollListFirstSelect(scroll);
		if(itWas < 0)goto ErrorOut;
		
		s=(CPio2dExtPtr)o->oCurrent[itWas];
		
	    if(s->type != G_EXTRUDE_PIO2D && s->type != G_ROTATE_PIO2D &&
	       s->type != G_LEVEL_PIO3D   && s->type != G_EXTRUDE_PIO3D &&
	       s->type != G_EXTRUDE_SDS2D && s->type != G_EXTRUDE_HDF &&
	       s->type != G_ROTATE_SDS2D  && s->type != G_ROTATE_HDF  &&
	       s->type != G_LEVEL_SDS3D   && s->type !=G_STREAMLINE_PIO3D)goto ErrorOut;
	    
	    if(s->type == G_EXTRUDE_PIO3D){
	       type = MessageType_DO_GENERAL_DIALOG;
	    }else{
	       type = MessageType_DO_AREA_DIALOG;
	    }
		if(SendMessageByName(s->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
		if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
			goto ErrorOut;
		}
			
	}else if(uList->actionItem == 108 || uList->actionItem == 174){	
	
	    itWas=ScrollListFirstSelect(scroll);
	    if(itWas < 0)goto Ok;

	    doRangeObjects(uList,o->oCurrent[itWas]);
	}else if(uList->actionItem == 172){	
	    ScrollListInsertSort(uR[171].sreturn,3,LIST_SORT_DOUBLE,List2);
	    ScrollListUpdate(List2);
	}else if(uList->actionItem == 173){	
	    ScrollListDeleteSelected(List2);
	    ScrollListUpdate(List2);
	}else if(uList->actionItem == 177){	
	     doRaysButton(uList,scene->itIs);
	}else if(uList->actionItem == 31){	
	    if(scene->itIs < 0){
	        Warning("Must Select Material First");
	        goto ErrorOut;
	    }
	}else if(uList->actionItem == 218){	
	    
	    itWas=ScrollListFirstSelect(scroll);
	    if(itWas < 0)goto Ok;
	    doScale(myIcon,(int)o->oCurrent[itWas]->Material,o->oCurrent[itWas]);
	}
	
Ok:
	ret = 0;
	
ErrorOut:	
	if(uList)uDialogUpdate(uList);
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}
static int doRaysButton(struct uDialogStruct *uList,long no)
{
	struct ScrollList *List2;
	struct Scene *scene;
	struct RayList *Rays;
	char number[256],name[256];
	char *fp;
	long list[2000];
	renderPtr r;
	IconPtr myIcon;
	CObjectListPtr ol;
	CObjectPtr o;
	long *Levels;
	long count,n;
	double dd;
	int reti;
    int ret;
	int nc,nn,iskip,k;
    
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	ol=scene->ObjectList;
	if(!ol)return 1;
	if((no < 0) || (no >= ol->oCount))return 0;
	o=ol->oCurrent[no];
	if(!o)return 1;

	ret = 1;
	
	Levels=NULL;
	
	reti=uDialogSet(uList,1L,
          
          uDialogSetItem,107L,
          uDialogGetScrollList,&List2,
          
          uDialogSetDone,uDialogSetDone          
    );
    
	if(reti)goto ErrorOut;
	
	if(o->type == G_RAY_PIO2D || o->type == G_TRACER_PIO2D ){
	    CPio2dRayPtr r=(CPio2dRayPtr)o;
	    if(!r->Files)goto ErrorOut;
	    Rays=GetRayList(r->Files);
	    if(!Rays)goto ErrorOut;
	    
	    
		nc=0;
		for(n=0;n<Rays->nameCount;++n){
		    if(!Rays->nameList[n])continue;
			mstrncpy(name,Rays->nameList[n],255);
			fp=strrchr((char *)name,'(');
			if(fp){
			    *fp=0;
			    if(!strcmp(name,"laser_plot_e") || !strcmp(name,"laser_plot_x") ||
			       !strcmp(name,"laser_plot_y") || !strcmp(name,"laser_plot_z") ||
			       !strcmp(name,"tracer_data")){
					if(nc < 2000){
						++fp;
						list[nc++]=atoi(fp);
			        }
					cFree((char *)Rays->nameList[n]);
					Rays->nameList[n]=NULL;
					continue;
			    }
			}  
			sprintf(WarningBuff,"doRaysButton  %s - cannot plot\n",Rays->nameList[n]);
			WarningBatch(WarningBuff);
			cFree((char *)Rays->nameList[n]);
			Rays->nameList[n]=NULL;
			
		}
		
		cFree((char *)Rays->nameList);
		Rays->nameList=NULL;
	    
	    if(nc == 0)goto ErrorOut;
		    
	    count=ScrollListCount(List2);
	    if(count){
	        Levels=(long *)cMalloc((count+nc)*sizeof(long),8608);
	        if(!Levels)return 1;
	        for(n=0;n<count;++n){
	            ScrollListGet(number,n,List2);
	            sscanf(number,"%lf",&dd);
	            Levels[n]=(long)dd;
	        }
	        
		    nn=0;
			for(n=0;n<nc;++n){
				iskip = FALSE;
		    	for(k=0;k<count;++k){
				    if(Levels[k] == list[n]){
				       iskip = TRUE;
				       break;
				    }
				}			
				if(!iskip){
				    list[nn++]=list[n];
				}
		    }
	        
		    for(k=0;k<count;++k){
		        list[nn++]=Levels[k];
		    }
	        
	        nc=nn;
	        ScrollListDelete(0L,count,List2);
	    }
	    
	    for(n=0;n<nc;++n){
	        sprintf(number,"%ld%c",list[n],0);
	        ScrollListInsertSort(number,3,LIST_SORT_DOUBLE,List2);
	    }	    
	    
	    
	    
	}	
	ret = 0;
ErrorOut:
	if(Levels)cFree((char *)Levels);
	return ret;
}
static int SaveObjectData(struct uDialogStruct *uList,long no)
{
	struct ScrollList *List2;
	struct Scene *scene;
	uDialogReturn *uR;
	char number[256];
	renderPtr r;
	IconPtr myIcon;
	CObjectListPtr ol;
	CObjectPtr o;
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
	ol=scene->ObjectList;
	if(!ol)return 1;
	if((no < 0) || (no >= ol->oCount))return 0;
	o=ol->oCurrent[no];
	if(!o)return 1;
	
	
	ret = 1;
	uR=NULL;
	
    uR=uDialogGetReturn(uList);
    if(!uR)goto ErrorOut;
            
	if(o->type == G_GROUP || o->type == G_WORLD  || o->type == G_VRML2 
	|| o->type == G_BLOCK || o->type == G_VRML ){
StardardGet:
	    o->Local.p.x=uR[15].dreturn;
	    o->Local.p.y=uR[16].dreturn;
	    o->Local.p.z=uR[17].dreturn;
	    o->Local.Theta_x=uR[19].dreturn;
	    o->Local.Theta_y=uR[20].dreturn;
	    o->Local.Theta_z=uR[21].dreturn;
	    o->Local.Scale_x=uR[23].dreturn;
	    o->Local.Scale_y=uR[24].dreturn;
	    o->Local.Scale_z=uR[25].dreturn;
	    scene->xmove=uR[27].dreturn;
	    scene->ymove=uR[28].dreturn;
	    scene->zmove=uR[29].dreturn;
	    if(o->type == G_VRML2)o->Material=uR[31].lreturn;
	    if(o->type == G_VRML)o->Material=uR[31].lreturn;
	    if(o->type == G_BLOCK){
	        o->Material=uR[105].lreturn;
	    	o->ShowNormals=uR[247].lreturn;
	   }

	}else if(o->type == G_CONE){
	
	    CConePtr c=(CConePtr)o;
	
		c->Material=uR[33].lreturn;
		
		c->origin.x=uR[35].dreturn;
		c->origin.y=uR[36].dreturn;
		c->origin.z=uR[37].dreturn;
	
		c->rtop=uR[39].dreturn;
		c->rbottom=uR[41].dreturn;
		c->length=uR[43].dreturn;
	
		c->nx=uR[45].lreturn;
		c->ny=uR[47].lreturn;
		
	    goto StardardGet;
	}else if(o->type == G_TORUS){
	
	    CTorusPtr t=(CTorusPtr)o;
	    	
		t->Material=uR[49].lreturn;
		
		t->origin.x=uR[51].dreturn;
		t->origin.y=uR[52].dreturn;
		t->origin.z=uR[53].dreturn;
	
		t->rbig=uR[55].dreturn;
		t->rsmall=uR[57].dreturn;
	
		t->nx=uR[59].lreturn;
		t->ny=uR[61].lreturn;
		
	    goto StardardGet;
	}else if(o->type == G_DISK){
	
	    CDiskPtr disk=(CDiskPtr)o;	    
	    
	    	
		disk->Material=uR[63].lreturn;
		
		disk->origin.x=uR[65].dreturn;
		disk->origin.y=uR[66].dreturn;
		disk->origin.z=uR[66].dreturn;
	
		disk->rin=uR[69].dreturn;
		disk->rout=uR[71].dreturn;
	
		disk->nx=uR[73].lreturn;
		disk->ny=uR[75].lreturn;
		
	    goto StardardGet;
	}else if(o->type == G_SPHERE){
	
	    CSpherePtr s=(CSpherePtr)o;	    
	    	    	
		s->Material=uR[77].lreturn;
		
		s->origin.x=uR[79].dreturn;
		s->origin.y=uR[80].dreturn;
		s->origin.z=uR[81].dreturn;
	
		s->radius=uR[83].dreturn;
	
		s->nx=uR[85].lreturn;
		s->ny=uR[87].lreturn;
		
	    goto StardardGet;
	}else if(o->type == G_VRML_LIST2){
	    CVrmlListPtr s=(CVrmlListPtr)o;
	    
		s->tzero=uR[128].lreturn;
		s->thold=uR[130].lreturn;
		s->repeat=uR[132].lreturn;
		
		if(s->cycleFlag == 1){
		   s->cycleFlag = 1;
		}else if(s->cycleFlag == 2){
		   s->cycleFlag = 2;
		}else{
		   s->cycleFlag = 0;
		}
		 				
	    s->ItWas = -1;
	}else if(o->type == G_EXTRUDE_PIO2D || o->type == G_EXTRUDE_PIO3D ||
	         o->type == G_EXTRUDE_SDS2D || o->type == G_EXTRUDE_HDF){
	    CPio2dExtPtr s=(CPio2dExtPtr)o;
	    
		s->Material=uR[193].lreturn;
		s->vscale=uR[195].dreturn;
	    s->usePaint=(int)uR[213].lreturn;
	    
	    s->CutMin=uR[215].dreturn;
	    s->CutMax=uR[217].dreturn;
	    s->ItWas = -1;
	    
	}else if(o->type == G_FLIGHT_PATH){
	    CFlightPathPtr r=(CFlightPathPtr)o;
	    
	    r->Material=uR[224].lreturn;
	    
	    r->radius=uR[226].dreturn;
	    	    
	    r->nx=uR[228].lreturn;
	    
	    r->ny=uR[230].lreturn;
	    
	    r->SplineSteps=uR[232].lreturn;
	    
	    r->PassSmooth=uR[235].lreturn;
	    
	    r->Hide=uR[236].lreturn;
	    
	    r->CyclicSmooth=uR[237].lreturn;
	    
	    scene->tilt=uR[239].dreturn;
	    
	}else if(o->type == G_STREAMLINE_PIO3D){
	    CPio3dStreamLinesPtr r=(CPio3dStreamLinesPtr)o;
	    
	    r->Material=uR[179].lreturn;
	    
	    r->radius=uR[181].dreturn;
	    	    
	    r->nx=uR[183].lreturn;
	    
	    r->ny=uR[185].lreturn;
	    
	    r->radiusStepFactor=uR[187].dreturn;
	    
	    r->lineType=uR[242].lreturn;
	    if(uR[243].lreturn)r->lineType=2;
	    
	    r->streamGradients=uR[244].lreturn;

	    r->streamSteps=uR[246].lreturn;
	    
	    r->streamPasses=uR[253].lreturn;
	    
	    r->ItWas = -1;
	    
	}else if(o->type == G_RAY_PIO2D || o->type == G_TRACER_PIO2D){
	    CPio2dRayPtr r=(CPio2dRayPtr)o;
	    
				 				
		r->LastEntry=uR[171].dreturn;

	    r->Material=uR[179].lreturn;
	    
	    r->radius=uR[181].dreturn;
	    	    
	    r->nx=uR[183].lreturn;
	    
	    r->ny=uR[185].lreturn;
	    
	    r->radiusStepFactor=uR[187].dreturn;
	    
	    r->lineType=uR[191].lreturn;
	    
	    r->itemToPlot=uR[220].lreturn;

	    r->ItWas = -1;
				
		
		reti=uDialogSet(uList,0L,
                
                uDialogSetItem,107L,
                uDialogGetScrollList,&List2,
														
				uDialogSetDone,uDialogSetDone
	    );		
		if(reti) goto ErrorOut;		
		
	    if(r->Levels)cFree((char *)r->Levels);
	    r->Levels=NULL;
	    r->LevelCount=count=ScrollListCount(List2);
	    if(!count)goto Ok;

	    r->Levels=(double *)cMalloc(count*sizeof(double),8608);
	    if(!r->Levels)return 1;
	    for(n=0;n<count;++n){
	        ScrollListGet(number,n,List2);
	        sscanf(number,"%lf",&r->Levels[n]);
	    }
	    	
	}else if(o->type == G_ROTATE_PIO2D || o->type == G_ROTATE_SDS2D ||
	         o->type == G_ROTATE_HDF){
	    CPio2dRotPtr r=(CPio2dRotPtr)o;				 				

	    r->Material=uR[179].lreturn;
	    
	    r->usePaint=(int)uR[213].lreturn;
	    
	    r->iWrite=(int)uR[240].lreturn;
	    
		r->PassSmooth=uR[203].lreturn;

	    r->Angle=uR[205].dreturn;
	    	    
	    r->Angles=uR[207].lreturn;
	    
		r->Axis=uR[176].lreturn;

	    r->ItWas = -1;
						
		reti=uDialogSet(uList,0L,
                
                uDialogSetItem,107L,
                uDialogGetScrollList,&List2,
														
				uDialogSetDone,uDialogSetDone
	    );		
		if(reti) goto ErrorOut;		
		
	    if(r->Levels)cFree((char *)r->Levels);
	    r->Levels=NULL;
	    r->LevelCount=count=ScrollListCount(List2);
	    if(!count)goto Ok;

	    r->Levels=(double *)cMalloc(count*sizeof(double),8608);
	    if(!r->Levels)return 1;
	    for(n=0;n<count;++n){
	        ScrollListGet(number,n,List2);
	        sscanf(number,"%lf",&r->Levels[n]);
	    }
	    	
	}else if(o->type == G_LEVEL_PIO3D || o->type == G_LEVEL_SDS3D){
	    CPio3dLevelPtr r=(CPio3dLevelPtr)o;				 				

	    r->Material=uR[179].lreturn;
	    
	    r->usePaint=(int)uR[213].lreturn;
	    
	    r->ItWas = -1;
						
		reti=uDialogSet(uList,0L,
                
                uDialogSetItem,107L,
                uDialogGetScrollList,&List2,
														
				uDialogSetDone,uDialogSetDone
	    );		
		if(reti) goto ErrorOut;		
		
	    if(r->Levels)cFree((char *)r->Levels);
	    r->Levels=NULL;
	    r->LevelCount=count=ScrollListCount(List2);
	    if(!count)goto Ok;

	    r->Levels=(double *)cMalloc(count*sizeof(double),8608);
	    if(!r->Levels)return 1;
	    for(n=0;n<count;++n){
	        ScrollListGet(number,n,List2);
	        sscanf(number,"%lf",&r->Levels[n]);
	    }
	    	
	}else if(o->type == G_SHOW){
	    CShowPtr r=(CShowPtr)o;
	    
	    r->vscale=uR[249].dreturn;
	    
	    r->nstress=uR[251].dreturn;
	    
	    r->ItWas = -1;
	    
	}
		
Ok:	

	if(o)rotate3d(&(o->Local));	
	
	ret = 0;           
ErrorOut:	

    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int SetObjectData(struct uDialogStruct *uList,long no)
{
	struct ScrollList *List1,*List2;
	struct Scene *scene;
	char number[256];
	CObjectListPtr ol;
	IconPtr myIcon;
	CObjectPtr o;
	renderPtr r;
	long count;
	int reti;
    int ret;
    int n;
    
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	ol=scene->ObjectList;
	if(!ol)return 1;
	if((no < 0) || (no >= ol->oCount))return 0;
	o=ol->oCurrent[no];
	if(!o)return 1;


	reti = 1;


   for(n=14;n<=253;++n){   
		ret=uDialogSet(uList,0L,
				uDialogSetItem,(long)n,
				uDialogSetHidden,(int)TRUE,
																
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;   
   }

	if(o->type == G_GROUP || o->type == G_WORLD || o->type == G_VRML2 || o->type == G_VRML){
	
StandardFill:
	   for(n=14;n<=29;++n){
	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;
	  	   
	   
	   }
		ret=uDialogSet(uList,0L,
				uDialogSetItem,15L,
				uDialogSetDouble,(double)o->Local.p.x,

				uDialogSetItem,16L,
				uDialogSetDouble,(double)o->Local.p.y,
				              	              	              
				uDialogSetItem,17L,
				uDialogSetDouble,(double)o->Local.p.z,
				              	              	              
				uDialogSetItem,19L,
				uDialogSetDouble,(double)o->Local.Theta_x,
				
				uDialogSetItem,20L,
				uDialogSetDouble,(double)o->Local.Theta_y,
				              	              	              
				uDialogSetItem,21L,
				uDialogSetDouble,(double)o->Local.Theta_z,
				
				uDialogSetItem,23L,
				uDialogSetDouble,(double)o->Local.Scale_x,
				
				uDialogSetItem,24L,
				uDialogSetDouble,(double)o->Local.Scale_y,
				
				uDialogSetItem,25L,
				uDialogSetDouble,(double)o->Local.Scale_z,
											
				uDialogSetItem,27L,
				uDialogSetDouble,(double)scene->xmove,
				
				uDialogSetItem,28L,
				uDialogSetDouble,(double)scene->ymove,
				
				uDialogSetItem,29L,
				uDialogSetDouble,(double)scene->zmove,
				
				uDialogSetItem,30L,
				uDialogSetHidden,(int)(o->type != G_VRML2 && o->type != G_VRML),
				
				uDialogSetItem,31L,
				uDialogSetHidden,(int)(o->type != G_VRML2 && o->type != G_VRML),
				uDialogSetDouble,(double)o->Material,

				
												
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;
	}else if(o->type == G_CONE){
	
	    CConePtr c=(CConePtr)o;
	
	   for(n=32;n<=47;++n){
	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;
	  	   
	   
	   }
	
		ret=uDialogSet(uList,0L,
				uDialogSetItem,33L,
				uDialogSetDouble,(double)c->Material,

				uDialogSetItem,35L,
				uDialogSetDouble,(double)c->origin.x,
				
				uDialogSetItem,36L,
				uDialogSetDouble,(double)c->origin.y,
				              	              	              
				uDialogSetItem,37L,
				uDialogSetDouble,(double)c->origin.z,
				
				uDialogSetItem,39L,
				uDialogSetDouble,(double)c->rtop,
				
				uDialogSetItem,41L,
				uDialogSetDouble,(double)c->rbottom,
				
				uDialogSetItem,43L,
				uDialogSetDouble,(double)c->length,
											
				uDialogSetItem,45L,
				uDialogSetDouble,(double)c->nx,
				
				uDialogSetItem,47L,
				uDialogSetDouble,(double)c->ny,
				
														
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;
	
		goto StandardFill;
	}else if(o->type == G_TORUS){
	    CTorusPtr t=(CTorusPtr)o;
	    
	   for(n=48;n<=61;++n){
	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;
	  	   
	   
	   }
	      
		ret=uDialogSet(uList,0L,
				uDialogSetItem,49L,
				uDialogSetDouble,(double)t->Material,

				uDialogSetItem,51L,
				uDialogSetDouble,(double)t->origin.x,
				
				uDialogSetItem,52L,
				uDialogSetDouble,(double)t->origin.y,
				              	              	              
				uDialogSetItem,53L,
				uDialogSetDouble,(double)t->origin.z,
				
				uDialogSetItem,55L,
				uDialogSetDouble,(double)t->rbig,
				
				uDialogSetItem,57L,
				uDialogSetDouble,(double)t->rsmall,
				
				uDialogSetItem,59L,
				uDialogSetDouble,(double)t->nx,
				
				uDialogSetItem,61L,
				uDialogSetDouble,(double)t->ny,
				
														
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;
	
		goto StandardFill;
	}else if(o->type == G_DISK){
	    CDiskPtr disk=(CDiskPtr)o;
	    
	   for(n=62;n<=75;++n){
	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;
	  	   
	   
	   }
	      
		ret=uDialogSet(uList,0L,
				uDialogSetItem,63L,
				uDialogSetDouble,(double)disk->Material,

				uDialogSetItem,65L,
				uDialogSetDouble,(double)disk->origin.x,
				
				uDialogSetItem,66L,
				uDialogSetDouble,(double)disk->origin.y,
				              	              	              
				uDialogSetItem,67L,
				uDialogSetDouble,(double)disk->origin.z,
				
				uDialogSetItem,69L,
				uDialogSetDouble,(double)disk->rin,
				
				uDialogSetItem,71L,
				uDialogSetDouble,(double)disk->rout,
				
				uDialogSetItem,73L,
				uDialogSetDouble,(double)disk->nx,
				
				uDialogSetItem,75L,
				uDialogSetDouble,(double)disk->ny,
				
														
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;
	
		goto StandardFill;
	}else if(o->type == G_SPHERE){
	    CSpherePtr s=(CSpherePtr)o;

	   for(n=76;n<=87;++n){
	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;
	  	   
	   
	   }
	      
		ret=uDialogSet(uList,0L,
				uDialogSetItem,77L,
				uDialogSetDouble,(double)s->Material,

				uDialogSetItem,79L,
				uDialogSetDouble,(double)s->origin.x,
				
				uDialogSetItem,80L,
				uDialogSetDouble,(double)s->origin.y,
				              	              	              
				uDialogSetItem,81L,
				uDialogSetDouble,(double)s->origin.z,
				
				uDialogSetItem,83L,
				uDialogSetDouble,(double)s->radius,
				
				uDialogSetItem,85L,
				uDialogSetDouble,(double)s->nx,
				
				uDialogSetItem,87L,
				uDialogSetDouble,(double)s->ny,
				
														
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;
	
		goto StandardFill;
	}else if(o->type == G_BLOCK){
	    CBlockPtr b=(CBlockPtr)o;
	    struct Block *block;
	    
	    block = &(scene->block[b->BlockNumber]);


	   for(n=88;n<=105;++n){
	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;
	  	   
	   
	   }
	   
	      
		ret=uDialogSet(uList,0L,
				uDialogSetItem,89L,
				uDialogSetDouble,(double)block->NodeCount,

				uDialogSetItem,91L,
				uDialogSetDouble,(double)block->ElementCount,
				
				uDialogSetItem,93L,
				uDialogSetDouble,(double)block->xmin,
				              	              	              
				uDialogSetItem,94L,
				uDialogSetDouble,(double)block->xmax,
				
				uDialogSetItem,96L,
				uDialogSetDouble,(double)block->ymin,
				              	              	              
				uDialogSetItem,97L,
				uDialogSetDouble,(double)block->ymax,
				
				uDialogSetItem,99L,
				uDialogSetDouble,(double)block->zmin,
				              	              	              
				uDialogSetItem,100L,
				uDialogSetDouble,(double)block->zmax,
				
				uDialogSetItem,102L,
				uDialogSetDouble,(double)block->vmin,
				              	              	              
				uDialogSetItem,103L,
				uDialogSetDouble,(double)block->vmax,
				
				uDialogSetItem,105L,
				uDialogSetDouble,(double)b->Material,
				
				uDialogSetItem,247L,
				uDialogSetHidden,(int)FALSE,
                uDialogSetLogical,(b->ShowNormals == 1),
														
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;
		goto StandardFill;
	}else if(o->type == G_LEVEL_PIO3D || o->type == G_LEVEL_SDS3D){
	    CPio2dRotPtr r=(CPio2dRotPtr)o;
	    
	   for(n=171;n<=174;++n){	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;	  	   
	   
	   }


		ret=uDialogSet(uList,0L,

				uDialogSetItem,171L,
				uDialogSetDouble,(double)r->LastEntry,
				
                uDialogSetItem,109L,
    			uDialogSetHidden,(int)FALSE,
                            
                uDialogSetItem,110L,
                uDialogGetScrollList,&List1,
				uDialogSetHidden,(int)(0),
														
                uDialogSetItem,106L,
    			uDialogSetHidden,(int)FALSE,
                            
                uDialogSetItem,107L,
                uDialogGetScrollList,&List2,
				uDialogSetHidden,(int)(0),

				uDialogSetItem,178L,
				uDialogSetHidden,(int)(0),
				
				uDialogSetItem,179L,
				uDialogSetHidden,(int)(0),
				uDialogSetDouble,(double)r->Material,
				
				uDialogSetItem,196L,
				uDialogSetHidden,(int)(0),
				
				uDialogSetItem,197L,
				uDialogSetHidden,(int)(0),
				uDialogSetDouble,(double)r->dmin,
				
				uDialogSetItem,198L,
				uDialogSetHidden,(int)(0),
				
				uDialogSetItem,199L,
				uDialogSetHidden,(int)(0),
				uDialogSetDouble,(double)r->dmax,
														

                uDialogSetItem,210L,
    			uDialogSetHidden,(int)FALSE,

                uDialogSetItem,211L,
    			uDialogSetHidden,(int)FALSE,

                uDialogSetItem,212L,
    			uDialogSetHidden,(int)FALSE,

                uDialogSetItem,213L,
    			uDialogSetHidden,(int)FALSE,
                uDialogSetLogical,(r->usePaint == 1),

                uDialogSetItem,218L,
    			uDialogSetHidden,(int)FALSE,
    			
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;
		
	    count=(int)ScrollListCount(List1);

	    ScrollListDelete(0L,count,List1);
	    

	    ScrollListInsertOneType(r->Files->ImageNames,0,0L,
	        r->Files->ImageCount,List1);


	    count=ScrollListCount(List2);

	    ScrollListDelete(0L,count,List2);

	    for(n=0;n<r->LevelCount;++n){
	        double ll;

	        ll=(double)((long)r->Levels[n]);
	        if(ll == r->Levels[n]){
	            sprintf(number,"%ld%c",(long)(r->Levels[n]),0);
	        }else{
	            sprintf(number,"%g%c",r->Levels[n],0);
	        }
	        ScrollListInsertSort(number,3,LIST_SORT_DOUBLE,List2);
	    }	    
	    
	
	}else if(o->type == G_EXTRUDE_PIO2D || o->type == G_EXTRUDE_PIO3D ||
	         o->type == G_EXTRUDE_SDS2D || o->type == G_EXTRUDE_HDF){
	    CPio2dExtPtr sx=(CPio2dExtPtr)o;
	    
	   for(n=192;n<=201;++n){
	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;
	  	   
	   
	   }
      
		ret=uDialogSet(uList,0L,
				uDialogSetItem,193,
				uDialogSetDouble,(double)sx->Material,

				uDialogSetItem,195L,
				uDialogSetDouble,(double)sx->vscale,
								
				uDialogSetItem,174L,
				uDialogSetHidden,(int)(0),
				
                uDialogSetItem,218L,
    			uDialogSetHidden,(int)FALSE,
    			
				uDialogSetItem,196L,
				uDialogSetHidden,(int)(0),
				
				uDialogSetItem,197L,
				uDialogSetDouble,(double)sx->dmin,
				
				uDialogSetItem,198L,
				uDialogSetHidden,(int)(0),
				
				uDialogSetItem,199L,
				uDialogSetDouble,(double)sx->dmax,
												
                uDialogSetItem,110L,
				uDialogSetHidden,(int)(0),
                uDialogGetScrollList,&List1,

                uDialogSetItem,212L,
    			uDialogSetHidden,(int)FALSE,

                uDialogSetItem,213L,
    			uDialogSetHidden,(int)FALSE,
                uDialogSetLogical,(sx->usePaint == 1),
    																	
                uDialogSetItem,214L,
    			uDialogSetHidden,(int)FALSE,
    			
                uDialogSetItem,215L,
    			uDialogSetHidden,(int)FALSE,
				uDialogSetDouble,(double)sx->CutMin,
    			
                uDialogSetItem,216L,
    			uDialogSetHidden,(int)FALSE,
    			
                uDialogSetItem,217L,
    			uDialogSetHidden,(int)FALSE,
				uDialogSetDouble,(double)sx->CutMax,


				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;
		
	    count=(int)ScrollListCount(List1);

	    ScrollListDelete(0L,count,List1);
	    
	    ScrollListInsertOneType(sx->Files->ImageNames,0,0L,
	            sx->Files->ImageCount,List1);
	}else if(o->type == G_FLIGHT_PATH){
	    CFlightPathPtr r=(CFlightPathPtr)o;
	   for(n=223;n<=239;++n){
	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;
	  	   
	   
	   }
		ret=uDialogSet(uList,0L,
	    
  				uDialogSetItem,224L,
				uDialogSetDouble,(double)r->Material,
								
				uDialogSetItem,226L,
				uDialogSetDouble,(double)r->radius,

				uDialogSetItem,228L,
 				uDialogSetDouble,(double)r->nx,
				
				uDialogSetItem,230L,
 				uDialogSetDouble,(double)r->ny,
 				
				uDialogSetItem,232L,
 				uDialogSetDouble,(double)r->SplineSteps,
 				
				uDialogSetItem,235L,
 				uDialogSetDouble,(double)r->PassSmooth,
 				
 				uDialogSetItem,236L,
                uDialogSetLogical,(r->Hide == 1),
														
 				uDialogSetItem,237L,
                uDialogSetLogical,(r->CyclicSmooth == 1),
														
				uDialogSetItem,239L,
 				uDialogSetDouble,scene->tilt,
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;
		
                          
	}else if(o->type == G_STREAMLINE_PIO3D){
	    CPio3dStreamLinesPtr r=(CPio3dStreamLinesPtr)o;
	    
	   for(n=178;n<=189;++n){
	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;
	  	   
	   
	   }
	    
		ret=uDialogSet(uList,0L,

                uDialogSetItem,109L,
    			uDialogSetHidden,(int)FALSE,
                            
                uDialogSetItem,110L,
                uDialogGetScrollList,&List1,
				uDialogSetHidden,(int)(0),
														
                uDialogSetItem,174L,
				uDialogSetHidden,(int)(0),
    			                                         
  				uDialogSetItem,179L,
				uDialogSetDouble,(double)r->Material,
								
				uDialogSetItem,181L,
				uDialogSetDouble,(double)r->radius,

				uDialogSetItem,183L,
 				uDialogSetDouble,(double)r->nx,
				
				uDialogSetItem,185L,
 				uDialogSetDouble,(double)r->ny,
                          
				uDialogSetItem,187L,
				uDialogSetDouble,(double)r->radiusStepFactor,
                          
                                                  
				uDialogSetItem,189L,
				uDialogSetDouble,(double)(0.2*r->LengthGreatest),
                          

                uDialogSetItem,241L,
				uDialogSetHidden,(int)(0),
                uDialogSetLogical,(r->lineType == 0),
				
                uDialogSetItem,242L,
				uDialogSetHidden,(int)(0),
                uDialogSetLogical,(r->lineType == 1),
 
                uDialogSetItem,243L,
				uDialogSetHidden,(int)(0),
                uDialogSetLogical,(r->lineType == 2),
 
                uDialogSetItem,244L,
				uDialogSetHidden,(int)(0),
                uDialogSetLogical,(r->streamGradients == 1),
 

				uDialogSetItem,196L,
				uDialogSetHidden,(int)(0),
				
				uDialogSetItem,197L,
				uDialogSetHidden,(int)(0),
				uDialogSetDouble,(double)r->dmin,
				
				uDialogSetItem,198L,
				uDialogSetHidden,(int)(0),
				
				uDialogSetItem,199L,
				uDialogSetHidden,(int)(0),
				uDialogSetDouble,(double)r->dmax,
														

                uDialogSetItem,210L,
    			uDialogSetHidden,(int)FALSE,

                uDialogSetItem,211L,
    			uDialogSetHidden,(int)FALSE,
                                                 
                uDialogSetItem,218L,
				uDialogSetHidden,(int)(0),
    			                                         
				uDialogSetItem,245L,
				uDialogSetHidden,(int)(0),
				
				uDialogSetItem,246L,
				uDialogSetHidden,(int)(0),
				uDialogSetDouble,(double)r->streamSteps,
				
				uDialogSetItem,252L,
				uDialogSetHidden,(int)(0),
				
				uDialogSetItem,253L,
				uDialogSetHidden,(int)(0),
				uDialogSetDouble,(double)r->streamPasses,
				
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;
		
	    count=(int)ScrollListCount(List1);

	    ScrollListDelete(0L,count,List1);
	    

	    ScrollListInsertOneType(r->Files->ImageNames,0,0L,
	        r->Files->ImageCount,List1);

	}else if(o->type == G_RAY_PIO2D || o->type == G_TRACER_PIO2D){
	    CPio2dRayPtr r=(CPio2dRayPtr)o;
	    
	   for(n=171;n<=191;++n){
	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;
	  	   
	   
	   }
	    
		ret=uDialogSet(uList,0L,

				uDialogSetItem,171L,
				uDialogSetDouble,(double)r->LastEntry,
				
                uDialogSetItem,109L,
    			uDialogSetHidden,(int)FALSE,
                            
                uDialogSetItem,110L,
                uDialogGetScrollList,&List1,
				uDialogSetHidden,(int)(0),
														
                uDialogSetItem,106L,
    			uDialogSetHidden,(int)FALSE,
                            
                uDialogSetItem,107L,
                uDialogGetScrollList,&List2,
				uDialogSetHidden,(int)(0),
				
                uDialogSetItem,174L,
    			uDialogSetHidden,(int)TRUE,                            

                uDialogSetItem,218L,
    			uDialogSetHidden,(int)FALSE,
    			                            				
                uDialogSetItem,175L,
    			uDialogSetHidden,(int)TRUE,                            
                            
                uDialogSetItem,176L,
    			uDialogSetHidden,(int)TRUE,                            
                            
  				uDialogSetItem,179L,
				uDialogSetDouble,(double)r->Material,
								
				uDialogSetItem,181L,
				uDialogSetDouble,(double)r->radius,

				uDialogSetItem,183L,
 				uDialogSetDouble,(double)r->nx,
				
				uDialogSetItem,185L,
 				uDialogSetDouble,(double)r->ny,
                          
				uDialogSetItem,187L,
				uDialogSetDouble,(double)r->radiusStepFactor,
                          
                                                  
				uDialogSetItem,189L,
				uDialogSetDouble,(double)(0.2*r->LengthGreatest),
                          

                uDialogSetItem,190L,
                uDialogSetLogical,(r->lineType == 0),
				
                uDialogSetItem,191L,
                uDialogSetLogical,(r->lineType == 1),

                uDialogSetItem,219L,
				uDialogSetHidden,(int)(o->type != G_TRACER_PIO2D),
                                                  
                uDialogSetItem,220L,
				uDialogSetHidden,(int)(o->type != G_TRACER_PIO2D),
    			uDialogSetDouble,(double)r->itemToPlot,
                                               
                uDialogSetItem,221L,
				uDialogSetHidden,(int)(o->type != G_TRACER_PIO2D),
                                                  
                uDialogSetItem,222L,
				uDialogSetHidden,(int)(o->type != G_TRACER_PIO2D),
     			uDialogSetText,r->itemName,
                                                 
														
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;
		
	    count=(int)ScrollListCount(List1);

	    ScrollListDelete(0L,count,List1);
	    

	    ScrollListInsertOneType(r->Files->ImageNames,0,0L,
	        r->Files->ImageCount,List1);


	    count=ScrollListCount(List2);

	    ScrollListDelete(0L,count,List2);

	    for(n=0;n<r->LevelCount;++n){
	        double ll;

	        ll=(double)((long)r->Levels[n]);
	        if(ll == r->Levels[n]){
	            sprintf(number,"%ld%c",(long)(r->Levels[n]),0);
	        }else{
	            sprintf(number,"%g%c",r->Levels[n],0);
	        }
	        ScrollListInsertSort(number,3,LIST_SORT_DOUBLE,List2);
	    }	    
	    
	
	}else if(o->type == G_ROTATE_PIO2D || o->type == G_ROTATE_SDS2D ||
	         o->type == G_ROTATE_HDF){
	    CPio2dRotPtr r=(CPio2dRotPtr)o;
	    
	   for(n=171;n<=179;++n){	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;	  	   
	   
	   }

	   for(n=202;n<=209;++n){	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;	  	   
	   
	   }

	    
		ret=uDialogSet(uList,0L,

				uDialogSetItem,171L,
				uDialogSetDouble,(double)r->LastEntry,
				
                uDialogSetItem,109L,
    			uDialogSetHidden,(int)FALSE,
                            
                uDialogSetItem,110L,
                uDialogGetScrollList,&List1,
				uDialogSetHidden,(int)(0),
														
                uDialogSetItem,106L,
    			uDialogSetHidden,(int)FALSE,
                            
                uDialogSetItem,107L,
                uDialogGetScrollList,&List2,
				uDialogSetHidden,(int)(0),
				
                uDialogSetItem,174L,
    			uDialogSetHidden,(int)0,             
    			               
                uDialogSetItem,218L,
    			uDialogSetHidden,(int)FALSE,
    			                            				
                uDialogSetItem,175L,
                uDialogSetLogical,(r->Axis == 0),
                            
                uDialogSetItem,176L,
                uDialogSetLogical,(r->Axis == 1),
                            
                uDialogSetItem,177L,
    			uDialogSetHidden,(int)TRUE,                            
                            
  				uDialogSetItem,179L,
				uDialogSetDouble,(double)r->Material,
								                                                  
				uDialogSetItem,196L,
				uDialogSetHidden,(int)(0),
				
				uDialogSetItem,197L,
				uDialogSetHidden,(int)(0),
				uDialogSetDouble,(double)r->dmin,
				
				uDialogSetItem,198L,
				uDialogSetHidden,(int)(0),
				
				uDialogSetItem,199L,
				uDialogSetHidden,(int)(0),
				uDialogSetDouble,(double)r->dmax,
														
				uDialogSetItem,203L,
				uDialogSetDouble,(double)r->PassSmooth,
				
				
				uDialogSetItem,205L,
				uDialogSetDouble,(double)r->Angle,
				
				uDialogSetItem,207L,
				uDialogSetDouble,(double)r->Angles,														

                uDialogSetItem,212L,
    			uDialogSetHidden,(int)FALSE,

                uDialogSetItem,213L,
    			uDialogSetHidden,(int)FALSE,
                uDialogSetLogical,(r->usePaint == 1),
                
        		uDialogSetItem,240L,
				uDialogSetHidden,(int)FALSE,
                uDialogSetLogical,(r->iWrite == 1),   			
														
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;
		
	    count=(int)ScrollListCount(List1);

	    ScrollListDelete(0L,count,List1);
	    

	    ScrollListInsertOneType(r->Files->ImageNames,0,0L,
	        r->Files->ImageCount,List1);


	    count=ScrollListCount(List2);

	    ScrollListDelete(0L,count,List2);

	    for(n=0;n<r->LevelCount;++n){
	        double ll;

	        ll=(double)((long)r->Levels[n]);
	        if(ll == r->Levels[n]){
	            sprintf(number,"%ld%c",(long)(r->Levels[n]),0);
	        }else{
	            sprintf(number,"%g%c",r->Levels[n],0);
	        }
	        ScrollListInsertSort(number,3,LIST_SORT_DOUBLE,List2);
	    }	    
	    
	
	}else if(o->type == G_VRML_LIST2){
	    CVrmlListPtr s=(CVrmlListPtr)o;

	   for(n=127;n<=135;++n){
	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;
	  	   
	   
	   }
		ret=uDialogSet(uList,0L,
				
				uDialogSetItem,128L,
				uDialogSetDouble,(double)s->tzero,
				
				uDialogSetItem,130L,
				uDialogSetDouble,(double)s->thold,
				
				uDialogSetItem,132L,
				uDialogSetDouble,(double)s->repeat,
				
                uDialogSetItem,133L,
                uDialogSetLogical,(s->cycleFlag == 0),
				
                uDialogSetItem,134L,
                uDialogSetLogical,(s->cycleFlag == 1),
				
                uDialogSetItem,135L,
                uDialogSetLogical,(s->cycleFlag == 2),
				
                uDialogSetItem,109L,
				uDialogSetHidden,(int)(0),
				
                uDialogSetItem,110L,
                uDialogGetScrollList,&List1,
				uDialogSetHidden,(int)(0),
														
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;

	    count=(int)ScrollListCount(List1);

	    ScrollListDelete(0L,count,List1);
	    
	    ScrollListInsertOneType(s->Files->FilesNames,0,0L,
	            s->Files->FileCount,List1);

	}else if(o->type == G_SHOW){
	    CShowPtr s=(CShowPtr)o;

	   for(n=248;n<=251;++n){
	   
			ret=uDialogSet(uList,0L,
					uDialogSetItem,(long)n,
					uDialogSetHidden,(int)FALSE,
																	
					uDialogSetDone,uDialogSetDone
		    );		
			if(ret) goto ErrorOut;
	  	   
	   
	   }
	      
		ret=uDialogSet(uList,0L,
				uDialogSetItem,249L,
				uDialogSetDouble,(double)s->vscale,

				uDialogSetItem,251L,
				uDialogSetDouble,(double)s->nstress,
				
														
				uDialogSetDone,uDialogSetDone
	    );		
		if(ret) goto ErrorOut;
		
	
	}
	
	reti = 0;
ErrorOut:
	return reti;
}
static int doObjectStart(struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	IconPtr myIcon;
	struct Scene *scene;
	CObjectListPtr o;
	renderPtr r;
	int itis;
	int ret;
	long n;
	
		
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	o=scene->ObjectList;
	if(!o)return 1;
	
	checkWorld(scene,o,0);

	for(n=0;n<o->oCount;++n){
		CObjectPtr Current;
		o->oOld[n]=NULL;
		if((Current=o->oCurrent[n]) == NULL)continue;
		o->oOld[n]=(*Current->duplicateObject)(Current);
	}
	o->oSave=o->oCount;
	
	upDateObjectList(scene,uList);
		
	itis = -1;
	for(n=o->oCount-1;n>=0;--n){
	    if(o->oCurrent[n]){
	        itis=(int)n;
	        break;
	    }
	}
	if(itis < 0)return 0;
	
	ret=uDialogSet(uList,1L,
              uDialogGetScrollList,&scroll,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	ScrollListDeSelect(scroll);
	ScrollListSelect(itis,scroll);
	
	SetObjectData(uList,itis);		

	scene->itIs=itis;
		
	return 0;
}
static int ClickObjectList(struct uDialogStruct *uList)
{
	struct ScrollList *List0;
	struct Scene *scene;
	renderPtr r;
	IconPtr myIcon;
    int ret;
    long n;

	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;


	ret=uDialogSet(uList,1L,
              uDialogGetScrollList,&List0,
                            	              	              
              uDialogSetDone,uDialogSetDone
    );		
	if(ret)goto ErrorOut;
			
	n=ScrollListFirstSelect(List0);
	if(n >= 0){
	    if((scene->itIs) >= 0 && (scene->itIs != n)){
	         SaveObjectData(uList,scene->itIs);
	    }
		scene->itIs=n;
	    SetObjectData(uList,scene->itIs);		
	}
	
ErrorOut:
			
	if(uList)uDialogUpdate(uList);
	
	return 0;
}
static int ungroupObjects(struct Scene *scene,struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	long itWas,count;
	long oCount;
	long n,k;
	CObjectListPtr o;
	int ret;

	if(!scene || !uList)return 1;
	o=scene->ObjectList;
	if(!o)return 1;

	ret=uDialogSet(uList,1L,
          uDialogGetScrollList,&scroll,
                    
          uDialogSetDone,uDialogSetDone          
    );
    
	if(ret)return 1;
	itWas=ScrollListFirstSelect(scroll);
	if(itWas < 0)return 0;

	count=ScrollListCount(scroll);

	oCount=o->oCount;
	if(oCount != count){
	    sprintf(WarningBuff,"ungroupObjects oCount %ld != count %ld\n",oCount,count);
		WarningBatch(WarningBuff);
	    return 1;
	}

	for(n=0;n<count;++n){
	    if((o->oCurrent[n]) == NULL)continue;
	    if(ScrollListIsSelected(n,scroll)){
	        o->oCurrent[n]->Selected = TRUE;
	    }else{
	        o->oCurrent[n]->Selected = FALSE;
	    }
	}

	for(n=o->oCount-1;n >= 0 ;--n){
	    CObjectPtr Current;
	    CGroupPtr Return;
	    if((Current = o->oCurrent[n]) != NULL){
	        if(Current->Selected && (Current->type == G_GROUP)){
	            Return=(CGroupPtr)Current;
	            for(k=0;k<Return->ObjectCount;++k){
					if(ListAddObjectList(o,(CObjectPtr)(Return->Objects[k]))){
					    break;
					}else{
					    Return->Objects[k]=NULL;
					}       
	            }
	            (*Return->killObject)((CObjectPtr)Return);
	            o->oCurrent[n]=NULL;
	            break;
	        }
	    }
	}

	PackObjectList(o);

	upDateObjectList(scene,uList);
	
	scene->itIs = -1;
	
	return 0;
}
static int groupObjects(struct Scene *scene,struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	long itWas,count;
	long oCount;
	long n;
	CObjectListPtr o;
	CGroupPtr g;
	long ObjectCount;
	int ret;

	if(!scene || !uList)return 1;

	o=scene->ObjectList;
	if(!o)return 1;
	
	
	ret=uDialogSet(uList,1L,
          uDialogGetScrollList,&scroll,
                    
          uDialogSetDone,uDialogSetDone          
    );
    
	if(ret)return 1;
	

	itWas=ScrollListFirstSelect(scroll);
	if(itWas < 0)return 0;

	count=ScrollListCount(scroll);

	oCount=o->oCount;
	if(oCount != count){
	    sprintf(WarningBuff,"groupObjects oCount %ld != count %ld\n",oCount,count);
		WarningBatch(WarningBuff);
	    return 1;
	}

	ObjectCount=0;
	for(n=0;n<count;++n){
	    if((o->oCurrent[n]) == NULL)continue;
	    if(ScrollListIsSelected(n,scroll)){
	        o->oCurrent[n]->Selected = TRUE;
	        ObjectCount++;
	    }else{
	        o->oCurrent[n]->Selected = FALSE;
	    }
	}

	g=CGroupCreate(scene,ObjectCount);
	if(!g)return 1;

	ObjectCount=0;
	for(n=0;n<count;++n){
	    CObjectPtr Current;
	    if((Current=o->oCurrent[n]) == NULL)continue;
	    if(Current->Selected){
	        g->Objects[ObjectCount++]=Current;
			o->oCurrent[n]=NULL;
	    }
	}

	g->ObjectCount=ObjectCount;

	PackObjectList(o);

	if(ListAddObjectList(o,(CObjectPtr)g)){
		(*g->killObject)((CObjectPtr)g);
	}

	upDateObjectList(scene,uList);
	
	scene->itIs = -1;
		
	return 0;
}
int PackObjectList(CObjectListPtr o)
{
	long n,k;

	if(!o)return 1;

	k=0;
	for(n=0;n<o->oCount;++n){
	    CObjectPtr Current;
	    Current=o->oCurrent[n];
	    o->oCurrent[n]=NULL;
	    if(Current != NULL){
	        o->oCurrent[k++]=Current;
	    }
	    /*
	    if(o->oOld && ((Current=o->oOld[n]) != NULL)){
	        (*Current->killObject)(Current);
		o->oOld[n]=NULL;
	    }
	    */
	}
	o->oCount=k;
	return 0;
}
static int upDateObjectList(struct Scene *scene,struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	CObjectListPtr o;
	long count,n;
	int ret;

	if(!scene || !uList)return 1;
	o=scene->ObjectList;
	if(!o)return 1;

	ret=uDialogSet(uList,1L,
              uDialogGetScrollList,&scroll,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;

	count=ScrollListCount(scroll);
	for(n=0;n<count;++n){
		ScrollListSelect(n,scroll);
	}
	ScrollListDeleteSelected(scroll);
	
	for(n=0;n<o->oCount;++n){
	    char buff[256];
		char *data;
		static char *names[]={"GroupError","Sphere","Cone",
		               "Disk","Torus","Block","Group",
		               "World","Sds2dExt","Sds2dRot",
		               "HdfExt","HdfRot","Sds3d","VrmlList",
		               "Vrml2","Show","Pio2dExt","Pio2dRot","PioRay",
		               "Pio3dLevel","Pio3dExt","PioTracer",
		               "FlightPath","Stl","Pio3dStreamLines",
		               "Vrml"};
		CObjectPtr Current;
		int nn;

		data = &buff[0];

		if((Current=o->oCurrent[n]) == NULL)continue;
		
		nn=Current->type;
		
		if(nn < 0 || nn >= sizeof(names)/sizeof(char *)){
		   nn=0;
		}
		sprintf(buff,"%s<%ld>",names[nn],n);
		if(ScrollListInsertOneType(&data,3,n,
		    1L,scroll))return 1;
	}
	
	ScrollListSetTop(0L,scroll);
	
	return 0;
}
static int deleteo(struct Scene *scene,struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	long itWas,count;
	long oCount;
	CObjectListPtr o;
	long n,add;
	int ret;

	if(!scene || !uList)return 1;
	o=scene->ObjectList;
	if(!o)return 1;

	ret=uDialogSet(uList,1L,
          uDialogGetScrollList,&scroll,
          uDialogSetDone,uDialogSetDone          
    );
    
	if(ret)return 1;
	itWas=ScrollListFirstSelect(scroll);
	if(itWas <0)return 0;

	count=ScrollListCount(scroll);

	oCount=o->oCount;
	if(oCount != count){
	    sprintf(WarningBuff,"deleteObject oCount %ld != count %ld\n",oCount,count);
		WarningBatch(WarningBuff);
	    return 1;
	}

	add=0;
	for(n=0;n<count;++n){
	    CObjectPtr Current;
	    if(ScrollListIsSelected(n,scroll)){
	        Current=o->oCurrent[n];
			if(Current){
			   (*Current->killObject)(Current);
		       o->oCurrent[n]=NULL;
			}
	    }else{
	       o->oCurrent[add++]=o->oCurrent[n];
	    }
	}

	o->oCount=add;

	upDateObjectList(scene,uList);

	return 0;
}
static int reset(struct Scene *scene,struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	long itWas,count;
	long oCount;
	long n;
	CObjectListPtr o;
	int ret;

	if(!scene || !uList)return 1;
	o=scene->ObjectList;
	if(!o)return 1;

	ret=uDialogSet(uList,1L,
          uDialogGetScrollList,&scroll,
                    
          uDialogSetDone,uDialogSetDone          
    );
    
	if(ret)return 1;
	itWas=ScrollListFirstSelect(scroll);
	if(itWas <0)return 0;

	count=ScrollListCount(scroll);

	oCount=o->oCount;
	if(oCount != count){
	    sprintf(WarningBuff,"reset oCount %ld != count %ld\n",oCount,count);
		WarningBatch(WarningBuff);
	    return 1;
	}

	for(n=0;n<count;++n){
	    CObjectPtr Current;
	    if((Current=o->oCurrent[n]) == NULL)continue;
	    if(ScrollListIsSelected(n,scroll)){
		    Current->Local.p.x = 0.;
		    Current->Local.p.y = 0.;
		    Current->Local.p.z = 0.;
		    Current->Local.Theta_x = 0.;
		    Current->Local.Theta_y = 0.;
		    Current->Local.Theta_z = 0.;
		    if(Current->Local.Scale_x >= 0){
		       Current->Local.Scale_x = 1.0;
		    }else{
		       Current->Local.Scale_x = -1.0;
		    }
		    if(Current->Local.Scale_y >= 0){
		       Current->Local.Scale_y = 1.0;
		    }else{
		       Current->Local.Scale_y = -1.0;
		    }
		    if(Current->Local.Scale_z >= 0){
		       Current->Local.Scale_z = 1.0;
		    }else{
		       Current->Local.Scale_z = -1.0;
		    }
	    }
	    rotate3d(&Current->Local);
	}
	scene->itIs=itWas;
	SetObjectData(uList,scene->itIs);		
	return 0;
}
static int moveObjects(struct Scene *scene,struct uDialogStruct *uList,
		  double xmove,double ymove,double zmove)
{
	struct ScrollList *scroll;
	long itWas,count;
	long oCount;
	long n;
	CObjectListPtr o;
	int ret;

	if(!scene || !uList)return 1;
	o=scene->ObjectList;
	if(!o)return 1;

	ret=uDialogSet(uList,1L,
          uDialogGetScrollList,&scroll,
                    
          uDialogSetDone,uDialogSetDone          
    );
    
	if(ret)return 1;
	itWas=ScrollListFirstSelect(scroll);
	if(itWas <0)return 0;

	count=ScrollListCount(scroll);

	oCount=o->oCount;
	if(oCount != count){
	    sprintf(WarningBuff,"moveObjects oCount %ld != count %ld\n",oCount,count);
		WarningBatch(WarningBuff);
	    return 1;
	}

	for(n=0;n<count;++n){
	    CObjectPtr Current;
	    if((Current=o->oCurrent[n]) == NULL)continue;
	    if(ScrollListIsSelected(n,scroll)){
			Current->Local.p.x     += xmove;
			Current->Local.p.y     += ymove;
			Current->Local.p.z     += zmove;
	    }
	    rotate3d(&(Current->Local));
	}

	scene->itIs=itWas;
	SetObjectData(uList,scene->itIs);		

	return 0;
}
static int duplicate(struct Scene *scene,struct uDialogStruct *uList,int flag)
{
	struct ScrollList *scroll;
	long itWas,count;
	long oCount;
	long n;
	CObjectListPtr o;
	int ret;

	if(!scene || !uList)return 1;
	o=scene->ObjectList;
	if(!o)return 1;

	ret=uDialogSet(uList,1L,
          uDialogGetScrollList,&scroll,
          uDialogSetDone,uDialogSetDone          
    );
    
	if(ret)return 1;
	itWas=ScrollListFirstSelect(scroll);
	if(itWas <0)return 0;

	count=ScrollListCount(scroll);

	oCount=o->oCount;
	if(oCount != count){
	    sprintf(WarningBuff,"duplicateObject oCount %ld != count %ld\n",oCount,count);
		WarningBatch(WarningBuff);
	    return 1;
	}
	
	for(n=0;n<count;++n){
	    CObjectPtr Current;
	    if((Current=o->oCurrent[n]) == NULL)continue;
	    if(ScrollListIsSelected(n,scroll)){
	        Current=(*Current->duplicateObject)(Current);
			if(flag == 1){
			    Current->Local.Scale_x *= -1.0;
			    Current->Local.p.x     *= -1.0;
			}else if(flag == 2){
			    Current->Local.Scale_y *= -1.0;
			    Current->Local.p.y     *= -1.0;
			}else if(flag == 3){
			    Current->Local.Scale_z *= -1.0;
			    Current->Local.p.z     *= -1.0;
			}

	        rotate3d(&(Current->Local));

	        if(ListAddObjectList(o,(CObjectPtr)Current)){
	            (*Current->killObject)((CObjectPtr)Current);
	            return 1;
	        }
	    }
	}

	upDateObjectList(scene,uList);

	return 0;
}
int rotate3d(struct System *Local)
{
	static struct P v1={1.,0.,0.};
	static struct P v2={0.,1.,0.};
	static struct P v3={0.,0.,1.};
	double xx,xy,xz,yx,yy,yz,zx,zy,zz;
	double c1,c2,c3,s1,s2,s3;
	double tx,ty,tz,dtor;
	double x,y,z;
	double xo[3],yo[3],zo[3],xn[3],yn[3],zn[3];
	double rx,ry,rz;
	long n;

	rx=Local->Theta_x;
	ry=Local->Theta_y;
	rz=Local->Theta_z;

	xo[0]=v1.x;
	yo[0]=v1.y;
	zo[0]=v1.z;

	xo[1]=v2.x;
	yo[1]=v2.y;
	zo[1]=v2.z;

	xo[2]=v3.x;
	yo[2]=v3.y;
	zo[2]=v3.z;

	dtor=atan(1.0)/45.;

	tx=dtor*rx;
	ty=dtor*ry;
	tz=dtor*rz;

	c1=cos(tz);
	s1=sin(tz);
	c2=cos(ty);
	s2=sin(ty);
	c3=cos(tx);
	s3=sin(tx);

	xx=c1*c2;
	xy=s3*c1*s2-s1*c3;
	xz=s1*s3+c1*s2*c3;

	yx=s1*c2;
	yy=c1*c3+s1*s2*s3;
	yz = -s3*c1+s1*s2*c3;

	zx = -s2;
	zy = s3*c2;
	zz = c2*c3;

	for(n=0;n<3;++n){
	    x=xo[n];
	    y=yo[n];
	    z=zo[n];
	    xn[n] = xx*x+xy*y+xz*z;
	    yn[n] = yx*x+yy*y+yz*z;
	    zn[n] = zx*x+zy*y+zz*z;
	}

	Local->x=p(xn[0],yn[0],zn[0]);
	Local->y=p(xn[1],yn[1],zn[1]);
	Local->z=p(xn[2],yn[2],zn[2]);
	return 0;

}
static int doRangeObjects(struct uDialogStruct *uList,CObjectPtr o)
{
	struct ScrollList *scroll;
	int ret;
	long n;

	if(!uList || !o)return 1;

		ret=uDialogSet(uList,110L,
	              uDialogGetScrollList,&scroll,
	              uDialogSetDone,uDialogSetDone
	    );
	    
		if(ret)return 1;
		
	if(o->type == G_ROTATE_SDS2D || o->type == G_ROTATE_HDF ||
	   o->type == G_LEVEL_SDS3D  || o->type == G_EXTRUDE_SDS2D ||
	   o->type == G_EXTRUDE_HDF  || o->type == G_ROTATE_PIO2D  ||
	   o->type == G_EXTRUDE_PIO2D|| o->type == G_LEVEL_PIO3D   ||
	   o->type == G_EXTRUDE_PIO3D|| o->type == G_STREAMLINE_PIO3D){
	    CSds2dRotPtr s=(CSds2dRotPtr)o;
	    CPio2dRotPtr ss=(CPio2dRotPtr)o;
	   long first,count;
	   double dmin,dmax;
	   double *data;
	   long length,k;
	   double *fdata,*daughter;

	   first = ScrollListFirstSelect(scroll);
	   if(first < 0){
	       Warning("No Images Selected To Range");
	       return 0;
	   }

/*
	   s->Files->d.pd=s->scene->pd;
	   s->Files->d.pd.blackZero=s->blackZero;
	   s->Files->d.pd.white255=s->white255;
*/
	   dmin =  1e60;
	   dmax = -1e60;
	   count=ScrollListCount(scroll);

		fdata=NULL;
		daughter=NULL;
	    
	   for(n=0;n<count;++n){
			if(ScrollListIsSelected(n,scroll)){
				if(o->type == G_ROTATE_PIO2D || o->type == G_EXTRUDE_PIO2D ||
				   o->type == G_LEVEL_PIO3D  || o->type == G_EXTRUDE_PIO3D ||
				   o->type == G_STREAMLINE_PIO3D){
							
					if(SageGetFileItem(s->Files,ss->pioName,ss->pioIndex,&fdata,&length,n)){
					    ret=17;goto OutOfHere;
					}

					if(SageGetFileItem(s->Files,"cell_daughter",0,&daughter,&length,n)){
					    ret=18;goto OutOfHere;
					}

					for(k=0;k<length;++k){
						if(daughter[k])continue;
						if(fdata[k] > dmax)dmax=fdata[k];
						if(fdata[k] < dmin)dmin=fdata[k];
					}

					if(fdata)cFree((char *)fdata);
					fdata=NULL;
					if(daughter)cFree((char *)daughter);
					daughter=NULL;
				
				}else{
					struct areadata ai;					
					
					ai.pioIndex=s->pioIndex;
				    mstrncpy(ai.pioName,s->pioName,64);
					
					if(o->type == G_LEVEL_SDS3D){
						ai.xminArea=s->sd.plane.Plane_xmin;
						ai.yminArea=s->sd.plane.Plane_ymin;
						ai.zminArea=s->sd.plane.Plane_zmin;
						
						ai.xmaxArea=s->sd.plane.Plane_xmax;
						ai.ymaxArea=s->sd.plane.Plane_ymax;
						ai.zmaxArea=s->sd.plane.Plane_zmax;
						
						ai.xsize=s->sd.plane.Plane_xcount;
						ai.ysize=s->sd.plane.Plane_ycount;
						ai.zsize=s->sd.plane.Plane_zcount;
					}else{
						ai.xminArea=s->sd.range.xminData;
						ai.yminArea=s->sd.range.yminData;
						ai.zminArea=0;
						ai.xmaxArea=s->sd.range.xmaxData;
						ai.ymaxArea=s->sd.range.ymaxData;
						ai.zmaxArea=0;
						ai.xsize=s->sd.range.ixmax;
						ai.ysize=s->sd.range.iymax;
						ai.zsize=1;
					}
					ai.CurrentFrame=n;
					ai.type = AREADATA_AREA_DATA;
					if(SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,&ai)){
						sprintf(WarningBuff,"doRangeObjects - file (%s)  Error Getting Area Data\n",s->BatchName);
						WarningBatch(WarningBuff);
						ret=117;
						goto OutOfHere;
					}
					
					data=ai.data;
					if(data){				
						length=ai.xsize*ai.ysize*ai.zsize;
					
					    for(k=0;k<length;++k){
					       if(data[k] > dmax)dmax=data[k];
					       if(data[k] < dmin)dmin=data[k];
					    }
				    }
					if(ai.data)cFree((char *)ai.data);
					ai.data=NULL;
			    }
			}
	   }
	   
		ret=uDialogSet(uList,0L,
			uDialogSetItem,124L,
			uDialogSetDouble,(double)dmin,
			
			uDialogSetItem,153L,
			uDialogSetDouble,(double)dmin,
			
			uDialogSetItem,197L,
			uDialogSetDouble,(double)dmin,
			
			uDialogSetItem,126L,
			uDialogSetDouble,(double)dmax,
			
			uDialogSetItem,155L,
			uDialogSetDouble,(double)dmax,
			
			uDialogSetItem,199L,
			uDialogSetDouble,(double)dmax,
			
		    uDialogSetDone,uDialogSetDone
		);
	   	   
	   s->dmin=dmin;
	   s->dmax=dmax;

	}
OutOfHere:
	return 0;
}
int oDumpit(renderPtr r)
{
	IconPtr myIcon;

	if(!r)return 1;
	myIcon=r->myIcon;
	if(!myIcon)return 1;
	UpdateWrite(myIcon);
	return 0;
}
int saveRenderImage(renderPtr r,unsigned char *p1,long xsize,long ysize)
{
	IconPtr myIcon;
	long n,length;

	if(!r || !p1)return 1;
	myIcon=r->myIcon;
	if(!myIcon)return 1;

	if(!r->buffer || (xsize != r->xsize) || (ysize != r->ysize)){
	    if(r->buffer)cFree((char *)r->buffer);
	    r->buffer=NULL;

	     r->buffer=(unsigned char *)cMalloc(xsize*ysize*4,8621);
	     if(!r->buffer){
			sprintf(WarningBuff,"saveRenderImage out of memory\n");
			WarningBatch(WarningBuff);
			return 1;
	     }
	     r->xsize=xsize;
	     r->ysize=ysize;
	}
	
	length=3*xsize*ysize;
	
	for(n=0;n<length;++n){
	   r->buffer[n]= *p1++;
	}


    return 0;
}
int freeScene(struct Scene *scene)
{
	long n;
	
	if(!scene)return 1;

	freeblocks(scene);
	if(scene->material){
	    if(scene->Closing){
		struct Material *m;
		long n;

		for(n=0;n<scene->MaterialCount;++n){
		    m = &scene->material[n];
		    if(m->ImageData.Files){
		        freeFileList(m->ImageData.Files);
		        m->ImageData.Files=NULL;
		    }
		}
	        
	    }
	    cFree((char *)scene->material);
	}	
	if(scene->CellBuff)cFree((char *)scene->CellBuff);
	if(scene->zbuff)cFree((char *)scene->zbuff);
	ListObjectKill(scene->ObjectList);
	scene->ObjectList=NULL;
	
	if(scene->VrmlCount){
	    for(n=0;n<scene->VrmlCount;++n){
	        if(scene->vrml){
	            freeVrmlList(&scene->vrml[n]);
	        }
	    }
	    if(scene->vrml)cFree((char *)scene->vrml);
	    scene->vrml=NULL;
	}

	if(scene->Lights)cFree((char *)scene->Lights);
	scene->nLights=0;

	cFree((char *)scene);
	return 0;
}
int freeblocks(struct Scene *scene)
{
	struct Block *block;
	long BlockCount,n;

	if(scene->block){
	    BlockCount=scene->BlockCount;
	}else{
	    scene->BlockCount=0;
	    return 0;
	}

	for(n=0;n<BlockCount;++n){

	    block=&(scene->block[n]);

	    if(block->xdata){
	        cFree((char *)block->xdata);
		    block->xdata=NULL;
	    }
	    if(block->ydata){
	        cFree((char *)block->ydata);
		    block->ydata=NULL;
	    }
	    if(block->zdata){
	        cFree((char *)block->zdata);
		    block->zdata=NULL;
	    }

	    if(block->dxdata){
	        cFree((char *)block->dxdata);
		    block->dxdata=NULL;
	    }
	    if(block->dydata){
	        cFree((char *)block->dydata);
		    block->dydata=NULL;
	    }
	    if(block->dzdata){
	        cFree((char *)block->dzdata);
		    block->dzdata=NULL;
	    }

	    if(block->oldx){
	        cFree((char *)block->oldx);
		    block->oldx=NULL;
	    }
	    if(block->oldy){
	        cFree((char *)block->oldy);
		    block->oldy=NULL;
	    }
	    if(block->oldz){
	        cFree((char *)block->oldz);
		    block->oldz=NULL;
	    }
	    if(block->vdata){
	        cFree((char *)block->vdata);
		    block->vdata=NULL;
	    }
	    if(block->Elements){
	        cFree((char *)block->Elements);
		    block->Elements=NULL;
	    }
	    if(block->nx){
	        cFree((char *)block->nx);
		    block->nx=NULL;
	    }
	    if(block->ny){
	        cFree((char *)block->ny);
		    block->ny=NULL;
	    }
	    if(block->nz){
	        cFree((char *)block->nz);
		    block->nz=NULL;
	    }
	    
	    if(block->rdata){
	        cFree((char *)block->rdata);
		    block->rdata=NULL;
	    }
	    
	    if(block->sdata){
	        cFree((char *)block->sdata);
		    block->sdata=NULL;
	    }
	    
        if(block->stress){
           cFree((char *)block->stress);
           block->stress = NULL;
        }

	}

	scene->BlockCount=0;

	if(scene->block)cFree((char *)scene->block);
    scene->block=NULL;

	return 0;
}

static void doFrameImage(IconPtr myIcon)
{
	struct Scene *scene;
	int xsize,ysize;
	long xsizep,ysizep;
	renderPtr t;


	if(!myIcon)return;
	t=(renderPtr)myIcon->DWindow;
	if(!t)return;
	scene=t->scene;
	if(!scene)return;
	

	xsize=(int)scene->xResolution;
	ysize=(int)(scene->yResolution);
	
	
	uGetPaletteSize(&xsizep,&ysizep,&scene->pd,myIcon);
	xsize += xsizep;
	if(ysizep > ysize)ysize=(int)ysizep;
	
		
	uSizeWindow(myIcon,(int)(xsize+ScrollOffSet),(int)(ysize+ScrollOffSet+myIcon->mMenuOffSet));	
	
	tMoveControls2(myIcon);
	
	uSetControlValue(myIcon->VControl,0L);
	uSetControlValue(myIcon->HControl,0L);
}
static void tMoveControls2(IconPtr myIcon)
{
	struct Scene *scene;
	uRect uviewRect;
	renderPtr r;
	
	if(!myIcon)return;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return;
	scene=r->scene;
	if(!scene)return;
	
	uviewRect=myIcon->uviewRect;
	
	tMoveControls(myIcon);
	
	if(uviewRect.xsize == 0)return;

	if(uviewRect.xsize != myIcon->uviewRect.xsize || uviewRect.ysize != myIcon->uviewRect.ysize){	
		long xsizep,ysizep;
		uGetPaletteSize(&xsizep,&ysizep,&scene->pd,myIcon);
		scene->xResolution=myIcon->uviewRect.xsize-xsizep;
		scene->yResolution=myIcon->uviewRect.ysize;
		tMoveControls(myIcon);
    	fixScene(r);
        RenderScene(r);
	}

}
static void tMoveControls(IconPtr myIcon)
{
	
	controlPtr control;
	struct Scene *scene;
	long xsizep,ysizep;
	renderPtr t;
	uRect eraseRect;
	int MaxValue;
	uRect r;
	int bottom;
	int right;	
	int top;
	int left;
	
	if(!myIcon)return;
	t=(renderPtr)myIcon->DWindow;
	if(!t)return;
	scene=t->scene;
	if(!scene)return;
	
	uGetPortBounds(myIcon,&r);
	
	top=0;
	left=0;
	bottom=r.ysize;
	right=r.xsize;
	

	if(uGetPaletteSize(&xsizep,&ysizep,&scene->pd,myIcon))return;

	myIcon->uviewRect = r;
	myIcon->uviewRect.y = myIcon->mMenuOffSet;
	myIcon->uviewRect.x = 0;
	myIcon->uviewRect.ysize -= (myIcon->mMenuOffSet+ScrollOffSet+myIcon->CursorSpace);
	myIcon->uviewRect.xsize -= (ScrollOffSet);


	scene->pd.left=myIcon->uviewRect.xsize-xsizep;
	scene->pd.top=myIcon->uviewRect.ysize/2-128-10;


	ScreenSetSize(myIcon->uviewRect.xsize,myIcon->uviewRect.ysize,myIcon);
	
	eraseRect.y=bottom-ScrollOffSet;
	eraseRect.x=left;
	eraseRect.ysize=ScrollOffSet;
	eraseRect.xsize=myIcon->eraseRectSize;	
	myIcon->eraseRect=eraseRect;
	
	control=myIcon->VControl;
	if(control){
		MoveControl(control,right-ScrollOffSet,top+myIcon->mMenuOffSet);
		SizeControl(control,ScrollOffSet,bottom-top-ScrollOffSet-myIcon->mMenuOffSet);
		MaxValue=(int)(scene->yResolution-(r.ysize-ScrollOffSet-myIcon->mMenuOffSet-myIcon->CursorSpace));
		if(MaxValue < 0)MaxValue = 0;
		uSetControlMaximum(control,MaxValue);
	}
		
	control=myIcon->HControl;	
	if(control){
		MoveControl(control,eraseRect.x+eraseRect.xsize,bottom-ScrollOffSet);
		SizeControl(control,right-(eraseRect.xsize+ScrollOffSet),ScrollOffSet);
		MaxValue=(int)(scene->xResolution-(right-ScrollOffSet));
		if(MaxValue < 0)MaxValue = 0;
		uSetControlMaximum(control,MaxValue);
	}
	
	InvalRectMyWindow(myIcon);
}
static int tdothumb(controlPtr control,IconPtr myIcon)
{
	//control=control;
	return tMoveDy(myIcon);
}
static int tdoPageUp(controlPtr control,short part,IconPtr myIcon)
{
		int dy,newp;
		
		if(!myIcon || !control)return 0;
		
		//part=part;
		dy=0;
		if(control == myIcon->VControl){
		    dy= myIcon->uviewRect.ysize;
		} else if(control == myIcon->HControl){
		    dy=myIcon->uviewRect.xsize;
		}
		newp=(int)uGetControlValue(control);
		uSetControlValue(control,newp-dy);
		if(uGetControlValue(control) == newp)return 0;
		return tMoveDy(myIcon);

}
static int tdoPageDown(controlPtr control,short part,IconPtr myIcon)
{
		int dy,newp;

		if(!myIcon || !control)return 0;
		
		//part=part;
		dy=0;
		if(control == myIcon->VControl){
		    dy= myIcon->uviewRect.ysize;
		} else if(control == myIcon->HControl){
		    dy=myIcon->uviewRect.xsize;
		}
		newp=(int)uGetControlValue(control);
		uSetControlValue(control,newp+dy);
		if(uGetControlValue(control) == newp)return 0;
		return tMoveDy(myIcon);
}
static int tgoUp(controlPtr control,short part,IconPtr myIcon)
{	
		int old;
	
		if(!myIcon || !control)return 0;
		//part=part;
		old=(int)uGetControlValue(control);
		uSetControlValue(control,old-8);
		if(uGetControlValue(control) == old)return 0;
		return tMoveDy(myIcon);

}
static int tgoDown(controlPtr control,short part,IconPtr myIcon)
{			
		int old;
		
		if(!myIcon || !control)return 0;
		//part=part;
		old=(int)uGetControlValue(control);
		uSetControlValue(control,old+8);
		if(uGetControlValue(control) == old)return 0;
		return tMoveDy(myIcon);

}
static int tMoveDy(IconPtr myIcon)
{	
	drawIt(myIcon);
	return 0;
}

void DrawCircle(uPoint p1,struct Scene *scene,int xshift,int yshift,
                IconPtr myIcon,struct System *World)
{
	struct P p1s;
	long ncell,ix,iy,n;
	double xmin,ymin,delx,dely,odx,ody;
	int rastorY;
	renderPtr rr;
	CObjectListPtr o;
	
	if(!myIcon)return;
	rr=(renderPtr)myIcon->DWindow;
	if(!rr)return;
	if(!scene)return;
 	o=scene->ObjectList;
 	if(!o)return;
		
	    ix=p1.x+xshift;
	    if(ix < 0 || ix >= scene->xResolution)return;
	    iy=p1.y+yshift;
	    if(iy < 0 || iy >= scene->yResolution)return;
		ncell=scene->CellBuff[ix+iy*scene->xResolution];
		if(ncell < 0)return;
		if(!World){
	            for(n=o->oCount-1;n >= 0;--n){
 			CObjectPtr Current;

	                if((Current=o->oCurrent[n]) == NULL)continue;
	        
	                if(ncell < Current->FirstCell)continue;	 

		        World = &(Current->Local);
		        break;
		    }

		}

	
	    xmin=scene->xmin;
	    ymin=scene->ymin;
	    odx=scene->odx;
	    ody=scene->ody;
	    delx=1.0/odx;
	    dely=1.0/ody;
		rastorY=(int)(scene->yResolution-1);
		p1s.z=scene->zbuff[ix+iy*scene->xResolution];
		p1s.x=((double)ix*delx+xmin)*(1-p1s.z);
		p1s.y=((double)(rastorY-iy)*dely+ymin)*(1-p1s.z);
		{
			struct System Local;
			double r,r2,x,y,z,theta,ztran;
			

	        Local = *World;
	             
	        Transform(&Local.p,&scene->GlobalToScreen);
	        
	        r2=(p1s.x-Local.p.x)*(p1s.x-Local.p.x)+(p1s.y-Local.p.y)*(p1s.y-Local.p.y)+
	            (p1s.z-Local.p.z)*(p1s.z-Local.p.z);
	        r=sqrt(r2);
	        
	        theta=3.1415926*2.0/35.0;
	        
	        z=Local.p.z;
	        ztran=1./(1-z);	        
	        for(n=0;n<36;++n){
	        	x=r*cos((double)n*theta)+Local.p.x;
	        	y=r*sin((double)n*theta)+Local.p.y;
	        	ix=(long)((ztran*x-xmin)*odx-xshift);
	        	iy=(long)((ztran*y-ymin)*ody+yshift);
	        	if(n == 0){
	        	    uMoveTo((int)ix,(int)(rastorY-iy));
	        	}else{
	        	    uLineTo((int)ix,(int)(rastorY-iy),myIcon);
	        	}
	        }
	}

	
}
static int doOrbit(IconPtr myIcon)
{
	static uDialogGroup Items[] =
        {
      /*  0 */      {"0", {163,25,80,20},uDialogText,uDialogGetLong},
      /*  1 */      {"0", {73,94,80,20},uDialogText,uDialogGetLong},
      /*  2 */      {"0", {73,119,80,20},uDialogText,uDialogGetLong},
      /*  3 */      {"0", {73,144,80,20},uDialogText,uDialogGetLong},
      /*  4 */      {"0", {195,95,110,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"0", {195,120,110,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"0", {195,145,110,20},uDialogText,uDialogGetDouble},
      /*  7 */      {"0", {427,95,110,20},uDialogText,uDialogGetDouble},
      /*  8 */      {"0", {427,120,110,20},uDialogText,uDialogGetDouble},
      /*  9 */      {"0", {427,145,110,20},uDialogText,uDialogGetLong},
      /* 10 */      {"0", {397,8,80,20},uDialogText,uDialogNone},
      /* 11 */      {"0", {397,33,80,20},uDialogText,uDialogGetLong},
      /* 12 */      {"Save Images", {329,66,125,20},uDialogCheck,uDialogGetLong},
      /* 13 */      {"sequence", {317,34,65,20},uDialogString,uDialogNone},
      /* 14 */      {"in", {5,94,24,18},uDialogString,uDialogNone},
      /* 15 */      {"around", {5,119,52,19},uDialogString,uDialogNone},
      /* 16 */      {"root name", {317,9,77,20},uDialogString,uDialogNone},
      /* 17 */      {"Ok", {315,184,80,20},uDialogButton,uDialogNone},
      /* 18 */      {"Cancel", {401,184,80,20},uDialogButton,uDialogNone},
      /* 19 */      {"Orbits", {59,25,65,20},uDialogString,uDialogNone},
      /* 20 */      {"out", {5,144,41,18},uDialogString,uDialogNone},
      /* 21 */      {"x", {173,95,15,20},uDialogString,uDialogNone},
      /* 22 */      {"y", {173,120,13,17},uDialogString,uDialogNone},
      /* 23 */      {"z", {173,145,19,20},uDialogString,uDialogNone},
      /* 24 */      {"Target Radius", {317,95,100,17},uDialogString,uDialogNone},
      /* 25 */      {"Target Degrees", {317,120,106,20},uDialogString,uDialogNone},
      /* 26 */      {"Target Loops", {317,145,100,16},uDialogString,uDialogNone},
        };

    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,555,221};
 		
	static char saveFileName[128]={'o','r','b','i','t','.','t','r','u',};
	char fileName[256],Directory[512],FileOut[1280];
	char buff[256];
	IconPtr StatusWindow;
	
	static long orbits=1;
	static double targetradius=0;
	static double targetdegrees=0;
	static long targetloops=1;
	static long in=20;
	static long around=10;
	static long out=20;
	static struct P offset={0.,0.,0.};
	struct P EyeSave,LampSave;
	struct P EyeToTarget,TargetLoop,V1,V2;
	struct P OutToEye,EyeToIn;
	double DegreesSave;
	long o,l;
	double pi;
	double r1,r2,a1,a2;
	double dsin,dsout,epsin,epsout;
	double stime,etime;
	long ntime;
	
	
	
	
	struct Scene *scene;
	renderPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
	int saveWhere;
    int reti;
    int ret;
    long n;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	EyeSave=scene->Eye;

	LampSave=scene->lamp.origin;

	DegreesSave=scene->Degrees;

	if(targetradius <= 0.){
	    struct P diff;
	    diff=Sub(&scene->Target,&scene->Eye);
	    targetradius=.3*Len(&diff);
	}

	if(targetdegrees <= 0.){
	   targetdegrees=scene->Degrees*3.0;
	   if(targetdegrees > 170.)targetdegrees=170.;
	}

	ret=1;
	
	uR=NULL;
	
	r->saveWhere=0;
	
	StatusWindow=NULL;
	
	uList=uDialogOpen("Orbit Target",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
			
	reti=uDialogSet(uList,0L,
		          uDialogSetDouble,(double)orbits,
	              
	              uDialogSetItem,1L,
	              uDialogSetDouble,(double)in,
	              
	              uDialogSetItem,2L,
	              uDialogSetDouble,(double)around,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)out,
	              	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)offset.x,
	              	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)offset.y,
	              	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,(double)offset.z,
	              	              	              	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,(double)targetradius,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)targetdegrees,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,(double)targetloops,
	              	              
	              uDialogSetItem,10L,
	              uDialogSetText,scene->sequenceName,
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,(double)scene->sequence,
	                            	          	              
	              uDialogSetItem,12L,
	              uDialogSetLogical,(scene->sequenceSave == 1),
              
	              uDialogSetMyIcon,myIcon,	    
	              
	              /*          
	              uDialogSetAction,Time1Action,	              
	              */
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	

	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
		
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	
	orbits=uR[0].lreturn;
	
	in=uR[1].lreturn;
	    	
	around=uR[2].lreturn;
	    	
	out=uR[3].lreturn;
	    	
	offset.x=uR[4].dreturn;
	
	offset.y=uR[5].dreturn;
	
	offset.z=uR[6].dreturn;
	
	targetradius=uR[7].dreturn;
	
	targetdegrees=uR[8].dreturn;
	
	targetloops=uR[9].lreturn;
	
	mstrncpy(scene->sequenceName,uR[10].sreturn,255);
	
	scene->sequence=uR[11].lreturn;
	
	scene->sequenceSave=uR[12].lreturn;
	
    if(uList){
        uDialogClose(uList);
        uList=NULL;
    }
    
    if(uR){
        uDialogFreeReturn(uR);
		uR=NULL;
	}
	
		
	stime=rtime();

	ntime=scene->sequence;

	pi=4.0*atan(1.0);

	if(targetradius < 0.)targetradius=0.;

	TargetLoop=Add(&offset,&scene->Target);

	EyeToTarget=Sub(&TargetLoop,&scene->Eye);
	EyeToTarget=Norm(&EyeToTarget);

	V1=CrossN(&EyeToTarget,&scene->Up);

	V2=EyeToTarget;

	V2=CrossN(&scene->Up,&V1);

	{
	    struct P VinTarget;
	    VinTarget=Mult(&V1,targetradius);
	    VinTarget=Add(&VinTarget,&TargetLoop);
	    EyeToIn=Sub(&VinTarget,&scene->Eye);
	}
	{
	    struct P VoutTarget;
	    VoutTarget=Mult(&V1,-targetradius);
	    VoutTarget=Add(&VoutTarget,&TargetLoop);
	    OutToEye=Sub(&scene->Eye,&VoutTarget);
	}

	{
	    struct P Range;
	    Range=Sub(&scene->Target,&scene->Eye);
	    r1=Len(&Range);  
	    a1=r1*tan(.5*DegreesSave*pi/180.);
	    Range=Mult(&V1,targetradius);
	    Range=Add(&Range,&TargetLoop);
	    Range=Sub(&scene->Target,&Range);
	    r2=Len(&Range);
	    a2=r2*tan(.5*targetdegrees*pi/180.);
	}
	{
		double length;

		if(around > 0 && in > 3){
		    dsin=(targetradius*pi/((double)around))/(r1-r2);
		    length=dsin*(double)in;
		    if(length > 1.0){
			dsin=1.0/((double)in);
		        epsin=0.;
		    }else{
		        epsin=(1.0 - (double)length)/(double)SUM(in-1);
		    }
		}else{
		    dsin=0.0;
		    if(in > 0)dsin=1.0/((double)in);
		    epsin=0.;
		}
	}
	{
		double length;

		if(around > 0 && out > 3){
		    dsout=(targetradius*pi/((double)around))/(r1-r2);
		    length=dsout*(double)out;
		    if(length > 1.0){
			dsout=1.0/((double)out);
			epsout=0.;
		    }else {
		        epsout=(1.0 - (double)length)/(double)SUM(out-1);
		    }
		}else{
		    dsout=0.0;
		    if(out > 0)dsout=1.0/((double)out);
		    epsout=0.;
		}
	}

	if(orbits < 0)orbits = 1;

	if(scene->sequenceSave > 0){
	    saveWhere=doSetImageInfo(saveFileName,Directory,1);
	    if(saveWhere == 1)return 1;
	}else{
	    saveWhere = 1;
	}


	StatusWindow=uStartStatusWindow(myIcon,"Saving Contours");
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

	for(o=0;o<orbits;++o){
	    struct P Move,Move2;
	    double dm,s,rp,sp;

	    dm=0.0;
	    if(in)dm = 1.0/((double)(in));
	    for(n=0;n<in;++n){
	        s=((double)(in-n)*dsin);
	        if(epsin > 0.0){
	            if((in-n-1) >= 1)s += epsin*(double)SUM((in-n-1));
	        }
	        s = 1.0 - s;
	        Move=Mult(&EyeToIn,s);
	        scene->Eye=Add(&EyeSave,&Move);
			scene->lamp.origin=scene->Eye;
			rp=(1.-s)*r1+s*r2;
			sp=(1.-s)*a1+s*a2;
	        scene->Degrees=2.0*atan2(sp,rp)*180./pi;
	        sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
	        sprintf(buff,"Creating: %s",fileName);
	        mstrncpy(FileOut,Directory,1280);
	        strncatToPath(FileOut,fileName,1280);
	        uWriteStatusWindow(StatusWindow,buff);
	        if(uCheckStatusWindow(StatusWindow,"Stop Oribits"))goto ErrorOut;
			RenderScene(r);
	        ++scene->sequence;	    
	    }
	    scene->Degrees=targetdegrees;
	    for(l=0;l<targetloops;++l){
	        dm=0.0;
	        if(around)dm = 1.0/((double)(around));
	        for(n=0;n<around;++n){
	            Move=Mult(&V1,targetradius*cos((((double)n*dm+(double)l)*pi)));
		    	Move2=Mult(&V2,targetradius*sin((((double)n*dm+(double)l)*pi)));
	            Move=Add(&Move,&Move2);
	            scene->Eye=Add(&TargetLoop,&Move);
		    	scene->lamp.origin=scene->Eye;
		    	sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
	            sprintf(buff,"Creating: %s",fileName);
	            mstrncpy(FileOut,Directory,1280);
	            strncatToPath(FileOut,fileName,1280);
	            uWriteStatusWindow(StatusWindow,buff);
	            if(uCheckStatusWindow(StatusWindow,"Stop Orbits ?"))goto ErrorOut;
		    	RenderScene(r);
	            ++scene->sequence;	  
	        }
	    }
	    dm=0.0;
	    if(out)dm = 1.0/((double)(out));
	    for(n=0;n<out;++n){
	        s=(double)n*dsout;
	        if(epsout > 0.0){
	            if((n-1) >= 1)s += epsout*(double)SUM((n-1));
	        }
	        s = 1.0 - s;
	        Move=Mult(&OutToEye,-s);
	        scene->Eye=Add(&EyeSave,&Move);
			scene->lamp.origin=scene->Eye;
			rp=(1.-s)*r1+s*r2;
			sp=(1.-s)*a1+s*a2;
	        scene->Degrees=2.0*atan2(sp,rp)*180./pi;
	        sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
	        sprintf(buff,"Creating: %s",fileName);
	        mstrncpy(FileOut,Directory,1280);
	        strncatToPath(FileOut,fileName,1280);
	        uWriteStatusWindow(StatusWindow,buff);
	        if(uCheckStatusWindow(StatusWindow,"Stop Orbits ?"))goto ErrorOut;
			RenderScene(r);
	        ++scene->sequence;	    
	    }
	}

	etime=rtime()-stime;

	ntime=scene->sequence-ntime;

	sprintf(WarningBuff,"Time To Orbit %.2f Seconds  %ld Frames  %.2f Frames/sec\n",
	    etime,ntime,((double)ntime)/etime);
	WarningBatch(WarningBuff);


	ret = 0;	
	
ErrorOut:

	uStopStatusWindow(StatusWindow);
	
	scene->Eye=EyeSave;

	scene->lamp.origin=LampSave;

	scene->Degrees=DegreesSave;
	
	
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
	
	return ret;
}
static int doMove(IconPtr myIcon)
{
	static uDialogGroup Items[] =
        {
      /*  0 */      {"0", {163,25,80,20},uDialogText,uDialogGetLong},
      /*  1 */      {"0", {73,94,80,20},uDialogText,uDialogGetDouble},
      /*  2 */      {"0", {73,119,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"0", {73,144,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"0", {196,95,80,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"0", {196,120,80,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"0", {196,145,80,20},uDialogText,uDialogGetDouble},
      /*  7 */      {"0", {397,8,80,20},uDialogText,uDialogNone},
      /*  8 */      {"0", {397,33,80,20},uDialogText,uDialogGetLong},
      /*  9 */      {"0", {397,124,80,20},uDialogText,uDialogGetDouble},
      /* 10 */      {"Save Images", {329,66,125,20},uDialogCheck,uDialogGetLong},
      /* 11 */      {"Reset Target", {328,93,106,18},uDialogCheck,uDialogGetLong},
      /* 12 */      {"Steps", {59,25,65,20},uDialogString,uDialogNone},
      /* 13 */      {"x", {173,95,15,20},uDialogString,uDialogNone},
      /* 14 */      {"y", {173,120,13,17},uDialogString,uDialogNone},
      /* 15 */      {"z", {173,145,19,20},uDialogString,uDialogNone},
      /* 16 */      {"Find Degrees", {297,124,92,17},uDialogString,uDialogNone},
      /* 17 */      {"x", {49,92,15,20},uDialogString,uDialogNone},
      /* 18 */      {"y", {49,117,13,17},uDialogString,uDialogNone},
      /* 19 */      {"z", {49,142,19,20},uDialogString,uDialogNone},
      /* 20 */      {"Target", {87,70,55,16},uDialogString,uDialogNone},
      /* 21 */      {"Find Point", {201,72,75,16},uDialogString,uDialogNone},
      /* 22 */      {"sequence", {317,34,65,20},uDialogString,uDialogNone},
      /* 23 */      {"root name", {317,9,77,20},uDialogString,uDialogNone},
      /* 24 */      {"Ok", {315,163,80,20},uDialogButton,uDialogNone},
      /* 25 */      {"Cancel", {401,163,80,20},uDialogButton,uDialogNone},
        };

    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,498,202};
 	
	
	static char saveFileName[128]={'m','o','v','e','.','t','r','u',};
	char fileName[256],Directory[512],FileOut[1280];
	char buff[256];
	IconPtr StatusWindow;
	
	struct Scene *scene;
	renderPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
	static long orbits=10;
	static double targetradius=0;
	static double targetdegrees=0;
	static long resettarget=1;
	struct P EyeToTarget,EyeToFind;
	struct P TargetSave,TargetToFind;
	double DegreesSave;
	long o;
	int saveWhere;
	double pi;
	double r1,r2,a1,a2;
    int reti;
    int ret;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret=1;
	
	uR=NULL;
	
	r->saveWhere=0;
	
	StatusWindow=NULL;
	
	if(targetradius <= 0.){
	    struct P diff;
	    diff=Sub(&scene->Target,&scene->Eye);
	    targetradius=.3*Len(&diff);
	}

	if(targetdegrees <= 0.){
	   targetdegrees=scene->Degrees;
	   if(targetdegrees > 170.)targetdegrees=170.;
	}
	
	TargetSave=scene->Target;

	DegreesSave=scene->Degrees;	
	
	
	uList=uDialogOpen("Move Target",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
			
		reti=uDialogSet(uList,0L,
		          uDialogSetDouble,(double)orbits,
	              
	              uDialogSetItem,1L,
	              uDialogSetDouble,(double)scene->Target.x,
	              	              	              
	              uDialogSetItem,2L,
	              uDialogSetDouble,(double)scene->Target.y,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)scene->Target.z,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)scene->FindPoint.x,
	              	              	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)scene->FindPoint.y,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,(double)scene->FindPoint.z,
	              
	              uDialogSetItem,7L,
	              uDialogSetText,scene->sequenceName,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)scene->sequence,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,(double)targetdegrees,
	              
	              uDialogSetItem,10L,
	              uDialogSetLogical,(scene->sequenceSave == 1),
	              
	              uDialogSetItem,11L,
	              uDialogSetLogical,(resettarget == 1),
	              
	              uDialogSetMyIcon,myIcon,	              	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
		
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
		
	orbits=uR[0].lreturn;
	    	
	scene->Target.x=uR[1].dreturn;
	    		
	scene->Target.y=uR[2].dreturn;
	    		
	scene->Target.z=uR[3].dreturn;
	    		
	scene->FindPoint.x=uR[4].dreturn;
	
	scene->FindPoint.y=uR[5].dreturn;
	
	scene->FindPoint.z=uR[6].dreturn;
	
	mstrncpy(scene->sequenceName,uR[7].sreturn,255);
	
	scene->sequence=uR[8].lreturn;
	
	targetdegrees=uR[9].dreturn;
	
	scene->sequenceSave=uR[10].lreturn;
		
	resettarget=uR[11].lreturn;
	
    if(uList){
        uDialogClose(uList);
        uList=NULL;
    }
    
    if(uR){
        uDialogFreeReturn(uR);
		uR=NULL;
	}
	
	pi=4.0*atan(1.0);


	EyeToTarget=Sub(&scene->Target,&scene->Eye);

	EyeToFind=Sub(&scene->FindPoint,&scene->Eye);

	TargetToFind=Sub(&scene->FindPoint,&scene->Target);


	{
	    r1=Len(&EyeToTarget);  
	    a1=r1*tan(.5*DegreesSave*pi/180.);
	    r2=Len(&EyeToFind);
	    a2=r2*tan(.5*targetdegrees*pi/180.);
	}

	if(orbits < 0)orbits = 1;

	if(scene->sequenceSave > 0){
	    saveWhere=doSetImageInfo(saveFileName,Directory,1);
	    if(saveWhere == 1)return 1;
	}else{
	    saveWhere = 1;
	}


	StatusWindow=uStartStatusWindow(myIcon,"Saving Contours");
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

	for(o=0;o<orbits;++o){
	    struct P Move;
	    double dm,s,rp,sp;

	    dm=0.0;
	    if(orbits > 1)dm = 1.0/((double)(orbits-1));

	    s=((double)(o)*dm);
	    Move=Mult(&TargetToFind,s);
	    scene->Target=Add(&TargetSave,&Move);
	    rp=(1.-s)*r1+s*r2;
	    sp=(1.-s)*a1+s*a2;
	    scene->Degrees=2.0*atan2(sp,rp)*180./pi;
	    sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
	    sprintf(buff,"Creating: %s",fileName);
	    mstrncpy(FileOut,Directory,1280);
	    strncatToPath(FileOut,fileName,1280);
	    uWriteStatusWindow(StatusWindow,buff);
	    if(uCheckStatusWindow(StatusWindow,"Stop Move"))goto ErrorOut;
	    RenderScene(r);
	    ++scene->sequence;	    
	}

	ret = 0;	
	
ErrorOut:

	if(resettarget)scene->Target=TargetSave;
	scene->Degrees=DegreesSave;

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
	
	return ret;
}
int Dump(struct Scene *scene,unsigned char *bline,
           unsigned char *bout,long xsize,long ysize,void *DumpData)
{
	IconPtr myIcon;
	renderPtr r;

	myIcon=(IconPtr)DumpData;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	if(!scene)return 1;

	if(r->saveWhere == 1){
	    return 0;
	}else if(r->saveWhere == 2){
	    if(r->saveFiles == NULL){
	        r->saveFiles=FilesTRUE();
	        if(!r->saveFiles)return 1;
	    }
	    return FilesSaveTRUEToMemory(r->saveName,bline,bout,
	                                 xsize,ysize,r->saveFiles);
	}else if(r->saveWhere == 4){
	    char path[1024];
	    if(r->saveFILE == NULL){
	        mstrncpy(path,r->saveDirectory,1024);
	        strncatToPath(path,r->saveFileName,1024);
	        r->saveFILE=fopen(path,"wb");
	        if(!r->saveFILE)return 1;
	    }
	    return TrueWriteMerge(r->saveName,bline,bout,
	                          xsize,ysize,r->saveFILE);
	}
	return 0;
}
int CenterScreen(struct Scene *scene,int flag)
{
	struct ObjectMessage Message;
	struct P EyeToTarget,pScreen;
	double rc;
	double at=0.26794919;
	int n,ifind;
	
	if(!scene)return 1;
	
	if(flag){
		CObjectListPtr o;
		o=scene->ObjectList;
		
		if(!o)return 1;
	    if(!scene->AutoCenterScreen)return 1;
	    
	    ifind=FALSE;	    
		for(n=0;n<o->oCount;++n){
			CObjectPtr Current;
			if((Current=o->oCurrent[n]) == NULL)continue;
			if(Current->type != G_BLOCK && Current->type != G_VRML2 &&
			   Current->type != G_TRACER_PIO2D && Current->type != G_VRML &&
			   Current->type != G_RAY_PIO2D && Current->type != G_SPHERE &&
			   Current->type != G_EXTRUDE_HDF && Current->type != G_ROTATE_HDF &&
			   Current->type != G_ROTATE_SDS2D && Current->type != G_EXTRUDE_SDS2D &&
			   Current->type != G_LEVEL_SDS3D && Current->type != G_TORUS &&
			   Current->type != G_CONE && Current->type != G_DISK
			   )continue;
	        ifind=TRUE;
	        break;
		}
		if(!ifind)return 1;
	    
	}
	
	zerol((char *)&Message,sizeof(Message));
	Message.MessageType=OBJECT_MESSAGE_RANGE_DATA;
	ListObjectMessage(scene->ObjectList,&Message);
	if(Message.xmin > 1e59){
	    return 1;
	}
	scene->Target.x=.5*(Message.xmin+Message.xmax);
	scene->Target.y=.5*(Message.ymin+Message.ymax);
	scene->Target.z=.5*(Message.zmin+Message.zmax);
	rc=sqrt((scene->Target.x-Message.xmax)*(scene->Target.x-Message.xmax)+
	        (scene->Target.y-Message.ymax)*(scene->Target.y-Message.ymax)+
	        (scene->Target.z-Message.zmax)*(scene->Target.z-Message.zmax));
	        
	        
	WarningBatch("Bounds\n");
	        	        
	sprintf(WarningBuff,"xmin %g xmax %g\n",Message.xmin,Message.xmax);
	WarningBatch(WarningBuff);
	        
	sprintf(WarningBuff,"ymin %g ymax %g\n",Message.ymin,Message.ymax);
	WarningBatch(WarningBuff);
	        
	sprintf(WarningBuff,"zmin %g zmax %g\n",Message.zmin,Message.zmax);
	WarningBatch(WarningBuff);
	
	sprintf(WarningBuff,"Surface Count %lld\n",Message.SurfaceCount);
	WarningBatch(WarningBuff);
	
	scene->Eye.x=scene->Target.x+1.001;
	scene->Eye.y=scene->Target.y+1.002;
	scene->Eye.z=scene->Target.z+1.003;

	EyeToTarget=Sub(&scene->Target,&scene->Eye);
	EyeToTarget=Norm(&EyeToTarget);

	pScreen=Mult(&EyeToTarget,-(rc/at));
	scene->Eye=Add(&pScreen,&scene->Target);

	EyeToTarget=Sub(&scene->Target,&scene->Eye);
	
	if(scene->EyeView == Scene_Anaglyph){
		scene->Height=Len(&EyeToTarget);
	}else{
		scene->Height=0.001*Len(&EyeToTarget);
	}
	
	scene->EyeSeparation=0.02*scene->Height;

	scene->Degrees=30;

	scene->lamp.origin=scene->Eye;
	
	scene->AutoCenterScreenDone=TRUE;
	
	if(scene->ClipPlane){
	    WarningBatch("AutoCenterScreen Has Turned off The Clip Plane\n");
	}
	
	scene->ClipPlane=ClipPlane_Off;
	
	return 0;
}
static int doView1Action(struct uDialogStruct *uList)
{
	IconPtr myIcon;	
	struct Scene *scene;
	renderPtr r;
	uDialogReturn *uR;
	int Tool;
	int reti;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret = 1;
	uR=NULL;
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
	Tool=r->Tool;
	r->Tool=ControlModeSelect;
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	scene->Eye.x=uR[5].dreturn;
	scene->Eye.y=uR[6].dreturn;
	scene->Eye.z=uR[7].dreturn;
	
	scene->Target.x=uR[8].dreturn;
	scene->Target.y=uR[9].dreturn;
	scene->Target.z=uR[10].dreturn;
	
    scene->lamp.origin.x=uR[11].dreturn;          
    scene->lamp.origin.y=uR[12].dreturn;          
    scene->lamp.origin.z=uR[13].dreturn;          
              
	scene->Up.x=uR[14].dreturn; 
	scene->Up.y=uR[15].dreturn; 
	scene->Up.z=uR[16].dreturn; 
	
	scene->Degrees=uR[17].dreturn;
	
	scene->AutoCenterScreen=uR[18].lreturn;
	
	scene->Height=uR[24].dreturn;

	scene->EyeSeparation=uR[25].dreturn;

	scene->EyeView=Scene_Eye;
	if(uR[27].lreturn)scene->EyeView=Scene_EyeLeft;
	if(uR[28].lreturn)scene->EyeView=Scene_EyeRight;	
	if(uR[29].lreturn)scene->EyeView=Scene_Anaglyph;	

	scene->ClipPlane=ClipPlane_Off;
	if(uR[46].lreturn)scene->ClipPlane=ClipPlane_Fixed;
	if(uR[47].lreturn)scene->ClipPlane=ClipPlane_Eye;	

	scene->ClipPlanePoint.x=uR[37].dreturn;
	scene->ClipPlanePoint.y=uR[38].dreturn;
	scene->ClipPlanePoint.z=uR[39].dreturn;


	scene->ClipPlaneNormal.x=uR[40].dreturn;
	scene->ClipPlaneNormal.y=uR[41].dreturn;
	scene->ClipPlaneNormal.z=uR[42].dreturn;

	scene->ClipPlaneDistance=uR[44].dreturn;
	
	scene->ClipPlaneAngle=uR[49].dreturn;
	
	if(uList->actionItem == 19 || uList->actionItem == 21 || uList->actionItem == 30){
			
				
		if(uList->actionItem == 19){
			reti=uDialogSet(uList,0L,
              uDialogSetExitOk,
              uDialogSetDone,uDialogSetDone
    		);
    
			if(reti)goto ErrorOut;
	
		}else if(uList->actionItem == 30){               
	       
	       CenterScreen(scene,0);

			reti=uDialogSet(uList,5L,
					uDialogSetDouble,scene->Eye.x,

					uDialogSetItem,6L,
					uDialogSetDouble,scene->Eye.y,

					uDialogSetItem,7L,
					uDialogSetDouble,scene->Eye.z,

					uDialogSetItem,8L,
					uDialogSetDouble,scene->Target.x,

					uDialogSetItem,9L,
					uDialogSetDouble,scene->Target.y,

					uDialogSetItem,10L,
					uDialogSetDouble,scene->Target.z,

					uDialogSetItem,11L,
					uDialogSetDouble,scene->Eye.x,

					uDialogSetItem,12L,
					uDialogSetDouble,scene->Eye.y,

					uDialogSetItem,13L,
					uDialogSetDouble,scene->Eye.z,

					uDialogSetItem,17L,
					uDialogSetDouble,scene->Degrees,
							              
	              	uDialogSetItem,18L,
	              	uDialogSetLogical,(scene->AutoCenterScreen  == 1),
	              	
					uDialogSetItem,24L,
					uDialogSetDouble,scene->Height,

					uDialogSetItem,25L,
					uDialogSetDouble,scene->EyeSeparation,

					uDialogSetItem,26L,
					uDialogSetLogical,(scene->EyeView == Scene_Eye),

					uDialogSetItem,27L,
					uDialogSetLogical,(scene->EyeView == Scene_EyeLeft),

					uDialogSetItem,28L,
					uDialogSetLogical,(scene->EyeView == Scene_EyeRight),

					uDialogSetItem,29L,
					uDialogSetLogical,(scene->EyeView == Scene_Anaglyph),
							          	  
					uDialogSetItem,37L,
					uDialogSetDouble,scene->ClipPlanePoint.x,

					uDialogSetItem,38L,
					uDialogSetDouble,scene->ClipPlanePoint.y,

					uDialogSetItem,39L,
					uDialogSetDouble,scene->ClipPlanePoint.z,

					uDialogSetItem,40L,
					uDialogSetDouble,scene->ClipPlaneNormal.x,

					uDialogSetItem,41L,
					uDialogSetDouble,scene->ClipPlaneNormal.y,

					uDialogSetItem,42L,
					uDialogSetDouble,scene->ClipPlaneNormal.z,

					uDialogSetItem,44L,
					uDialogSetDouble,scene->ClipPlaneDistance,

					uDialogSetItem,45L,
					uDialogSetLogical,(scene->ClipPlane == ClipPlane_Off),

					uDialogSetItem,46L,
					uDialogSetLogical,(scene->ClipPlane == ClipPlane_Fixed),

					uDialogSetItem,47L,
					uDialogSetLogical,(scene->ClipPlane == ClipPlane_Eye),
	              
					uDialogSetItem,49L,
					uDialogSetDouble,scene->ClipPlaneAngle,


          	  		uDialogSetDone,uDialogSetDone
			    );
			if(reti)goto ErrorOut;
						
	     
	    	if(r)RenderScene(r);
	    	
			if(uList)uDialogUpdate(uList);
		}else{
	                  	
	       if(r)RenderScene(r);
	    
	    }
	}else if(uList->actionItem == 33){     
	    SetWindowView(uList,1);      
	}else if(uList->actionItem == 34){               
		SetWindowView(uList,0);      
	}else if(uList->actionItem == 31){   /* Next */   
		RenderKeyBoard(myIcon,upArrow);		         
	}else if(uList->actionItem == 32){   /* Previous */    
		RenderKeyBoard(myIcon,downArrow);		      
	}
	ret = 0;
ErrorOut:	
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	r->Tool=Tool;
	return ret;
}

static int SetWindowView(struct uDialogStruct *uList,int flag)
{
	IconPtr myIcon;	
	struct Scene *scene;
	renderPtr r;
	int n;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;

	for(n=0;n<=18;++n){
		if(uDialogSet(uList,(long)n,
		              uDialogSetHidden,(flag == 0),	              
		              
		              uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
	}
    
	for(n=22;n<=29;++n){
		if(uDialogSet(uList,(long)n,
		              uDialogSetHidden,(flag == 0),	              
		              
		              uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
	}
    
	for(n=35;n<=49;++n){
		if(uDialogSet(uList,(long)n,
		              uDialogSetHidden,(flag == 1),	              
		              
		              uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
	}
    
ErrorOut:
	if(uList)uDialogUpdate(uList);
	return 0;
}

static int doView(IconPtr myIcon)
{
		static uDialogGroup Items[] =
    {
       /*  0 */      {"Eye",     {20,20,65,20},uDialogString,uDialogNone},
       /*  1 */      {"Target",  {20,50,65,20},uDialogString,uDialogNone},
       /*  2 */      {"Lamp",    {20,80,65,20},uDialogString,uDialogNone},
       /*  3 */      {"Up",      {20,110,65,20},uDialogString,uDialogNone},
       /*  4 */      {"Degrees", {20,140,65,20},uDialogString,uDialogNone},
       
       /*  5 */      {"0", {110,20,110,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"0", {230,20,110,20},uDialogText,uDialogGetDouble},
       /*  7 */      {"0", {350,20,110,20},uDialogText,uDialogGetDouble},
       
       /*  8 */      {"0", {110,50,110,20},uDialogText,uDialogGetDouble},
       /*  9 */      {"0", {230,50,110,20},uDialogText,uDialogGetDouble},
       /* 10 */      {"0", {350,50,110,20},uDialogText,uDialogGetDouble},
       
       /* 11 */      {"0", {110,80,110,20},uDialogText,uDialogGetDouble},
       /* 12 */      {"0", {230,80,110,20},uDialogText,uDialogGetDouble},
       /* 13 */      {"0", {350,80,110,20},uDialogText,uDialogGetDouble},
       
       /* 14 */      {"0", {110,110,110,20},uDialogText,uDialogGetDouble},
       /* 15 */      {"0", {230,110,110,20},uDialogText,uDialogGetDouble},
       /* 16 */      {"0", {350,110,110,20},uDialogText,uDialogGetDouble},
       
       /* 17 */      {"0", {110,140,110,20},uDialogText,uDialogGetDouble},
       
       /* 18 */      {"Center Screen", {20,230,120,20},uDialogCheck,uDialogGetLong},
       
       /* 19 */      {"Ok",     {290,280,70,20},uDialogButton,uDialogNone},
       /* 20 */      {"Cancel", {380,280,70,20},uDialogButton,uDialogNone},
       /* 21 */      {"Render", {20 ,280,70,20},uDialogButton,uDialogNone},
       /* 22 */      {"Height", {20,170,65,20},uDialogString,uDialogNone},
       /* 23 */      {"Separation", {20,200,85,20},uDialogString,uDialogNone},
       /* 24 */      {"0", {110,170,110,20},uDialogText,uDialogGetDouble},
       /* 25 */      {"0", {110,200,110,20},uDialogText,uDialogGetDouble},
       /* 26 */      {"Eye",        {240,140,100,18},uDialogButtonRadio,uDialogGetLong},
       /* 27 */      {"Left Eye",   {240,170,100,18},uDialogButtonRadio,uDialogGetLong},
       /* 28 */      {"Right Eye",  {240,200,100,18},uDialogButtonRadio,uDialogGetLong},
       /* 29 */      {"Anaglyph",   {240,230,100,18},uDialogButtonRadio,uDialogGetLong},
       /* 30 */      {"Center",     {100 ,280,70,20},uDialogButton,uDialogNone},
       /* 31 */      {"Next", {380 ,220,70,20},uDialogButton,uDialogNone},
       /* 32 */      {"Previous", {380 ,250,70,20},uDialogButton,uDialogNone},       
       /* 33 */       {"View", {380,140,70,20},uDialogButtonRadio,uDialogGetLong},
       /* 34 */       {"Clip", {380,170,70,20},uDialogButtonRadio,uDialogGetLong},
       
       /* 35 */      {"Point",     {20,20,65,20},uDialogString,uDialogNone},
       /* 36 */      {"Normal",  {20,50,65,20},uDialogString,uDialogNone},
       /* 37 */      {"0", {110,20,110,20},uDialogText,uDialogGetDouble},
       /* 38 */      {"0", {230,20,110,20},uDialogText,uDialogGetDouble},
       /* 39 */      {"0", {350,20,110,20},uDialogText,uDialogGetDouble},
       
       /* 40 */      {"0", {110,50,110,20},uDialogText,uDialogGetDouble},
       /* 41 */      {"0", {230,50,110,20},uDialogText,uDialogGetDouble},
       /* 42 */      {"0", {350,50,110,20},uDialogText,uDialogGetDouble},

       /* 43 */      {"Distance",    {20,80,65,20},uDialogString,uDialogNone},
       /* 44 */      {"0", {110,80,110,20},uDialogText,uDialogGetDouble},


       /* 45 */      {"Clip Off",        {240,140,100,18},uDialogButtonRadio,uDialogGetLong},
       /* 46 */      {"Clip Fixed",   {240,170,100,18},uDialogButtonRadio,uDialogGetLong},
       /* 47 */      {"Clip Eye",  {240,200,100,18},uDialogButtonRadio,uDialogGetLong},
       
       /* 48 */      {"Clip Angle",    {20,110,75,20},uDialogString,uDialogNone},
       /* 49 */      {"0", {110,110,110,20},uDialogText,uDialogGetDouble},
       
       
    };


    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,490,340};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
	int reti;
    int ret;

	struct Scene *scene;
	renderPtr r;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;

	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("View",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	reti=uDialogSet(uList,5L,
		          uDialogSetDouble,scene->Eye.x,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,scene->Eye.y,
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,scene->Eye.z,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,scene->Target.x,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,scene->Target.y,
	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,scene->Target.z,
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,scene->lamp.origin.x,
	              
	              uDialogSetItem,12L,
	              uDialogSetDouble,scene->lamp.origin.y,
	              
	              uDialogSetItem,13L,
	              uDialogSetDouble,scene->lamp.origin.z,

	              uDialogSetItem,14L,
	              uDialogSetDouble,scene->Up.x,
	              
	              uDialogSetItem,15L,
	              uDialogSetDouble,scene->Up.y,
	              
	              uDialogSetItem,16L,
	              uDialogSetDouble,scene->Up.z,
	              
	              uDialogSetItem,17L,
	              uDialogSetDouble,scene->Degrees,
	              
	              uDialogSetItem,18L,
	              uDialogSetLogical,(scene->AutoCenterScreen  == 1),
	              
	              uDialogSetItem,24L,
	              uDialogSetDouble,scene->Height,
	              
	              uDialogSetItem,25L,
	              uDialogSetDouble,scene->EyeSeparation,
	              
	              uDialogSetItem,26L,
	              uDialogSetLogical,(scene->EyeView == Scene_Eye),
	              
	              uDialogSetItem,27L,
	              uDialogSetLogical,(scene->EyeView == Scene_EyeLeft),
	              
	              uDialogSetItem,28L,
	              uDialogSetLogical,(scene->EyeView == Scene_EyeRight),
	              
	              uDialogSetItem,29L,
	              uDialogSetLogical,(scene->EyeView == Scene_Anaglyph),
	              	              
	              uDialogSetItem,33L,
	              uDialogSetLogical,1,
	          	  uDialogSetRadioGroup,2,
	              	              
	              uDialogSetItem,34L,
	              uDialogSetLogical,0,
	          	  uDialogSetRadioGroup,2,
	          	  
	              uDialogSetItem,37L,
	              uDialogSetDouble,scene->ClipPlanePoint.x,
	              
	              uDialogSetItem,38L,
	              uDialogSetDouble,scene->ClipPlanePoint.y,
	              
	              uDialogSetItem,39L,
	              uDialogSetDouble,scene->ClipPlanePoint.z,
	              
	              uDialogSetItem,40L,
	              uDialogSetDouble,scene->ClipPlaneNormal.x,
	              
	              uDialogSetItem,41L,
	              uDialogSetDouble,scene->ClipPlaneNormal.y,
	              
	              uDialogSetItem,42L,
	              uDialogSetDouble,scene->ClipPlaneNormal.z,
	              
	              uDialogSetItem,44L,
	              uDialogSetDouble,scene->ClipPlaneDistance,
	              	          	  
		          uDialogSetItem,45L,
	              uDialogSetLogical,(scene->ClipPlane == ClipPlane_Off),
	          	  uDialogSetRadioGroup,3,
	              
	              uDialogSetItem,46L,
	              uDialogSetLogical,(scene->ClipPlane == ClipPlane_Fixed),
	          	  uDialogSetRadioGroup,3,
	              
	              uDialogSetItem,47L,
	              uDialogSetLogical,(scene->ClipPlane == ClipPlane_Eye),
	          	  uDialogSetRadioGroup,3,

	              uDialogSetItem,49L,
	              uDialogSetDouble,scene->ClipPlaneAngle,
	              	          	  	              
	              uDialogSetMyIcon,myIcon,	              
	              uDialogSetAction,doView1Action,	              
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
/*	
Start:
*/
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
    SetWindowView(uList,1);

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
/*    
    if((uR[27].lreturn || uR[28].lreturn) &&
        (uR[25].dreturn <= 0.0)){
        Warning("Eye Separation Must be greater than zero");
        goto Start;
    }
*/	
	scene->Eye.x=uR[5].dreturn;
	scene->Eye.y=uR[6].dreturn;
	scene->Eye.z=uR[7].dreturn;
	
	scene->Target.x=uR[8].dreturn;
	scene->Target.y=uR[9].dreturn;
	scene->Target.z=uR[10].dreturn;
	
    scene->lamp.origin.x=uR[11].dreturn;          
    scene->lamp.origin.y=uR[12].dreturn;          
    scene->lamp.origin.z=uR[13].dreturn;          
    
   	scene->Up.x=uR[14].dreturn; 
	scene->Up.y=uR[15].dreturn; 
	scene->Up.z=uR[16].dreturn; 
	
	scene->Degrees=uR[17].dreturn;
	
	scene->AutoCenterScreen=uR[18].lreturn;

	scene->Height=uR[24].dreturn;
	
	scene->EyeSeparation=uR[25].dreturn;

	scene->EyeView=Scene_Eye;
	if(uR[27].lreturn)scene->EyeView=Scene_EyeLeft;
	if(uR[28].lreturn)scene->EyeView=Scene_EyeRight;	
	if(uR[29].lreturn)scene->EyeView=Scene_Anaglyph;	

	scene->ClipPlane=ClipPlane_Off;
	if(uR[46].lreturn)scene->ClipPlane=ClipPlane_Fixed;
	if(uR[47].lreturn)scene->ClipPlane=ClipPlane_Eye;	

	scene->ClipPlanePoint.x=uR[37].dreturn;
	scene->ClipPlanePoint.y=uR[38].dreturn;
	scene->ClipPlanePoint.z=uR[39].dreturn;
	
	
	scene->ClipPlaneNormal.x=uR[40].dreturn;
	scene->ClipPlaneNormal.y=uR[41].dreturn;
	scene->ClipPlaneNormal.z=uR[42].dreturn;
	
	scene->ClipPlaneDistance=uR[44].dreturn;
	
	scene->ClipPlaneAngle=uR[49].dreturn;
	
	
	ret=0;
ErrorOut:	
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	if(scene->AutoCenterScreen){
	   	RenderSceneSetTime(scene);
		scene->AutoCenterScreenDone=FALSE;
	}
	if(r)RenderScene(r);
	return ret;
}
static int doScale1Action(struct uDialogStruct *uList)
{
	IconPtr myIcon;	
	struct Scene *scene;
	renderPtr r;
	uDialogReturn *uR;
	int dType;
	int imat;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret = 1;
	uR=NULL;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 10){
    	
    	showPaletteLevels(scene);
    	
    	goto Ok;
    	    	
	}else if(uList->actionItem == 11){
		uR=uDialogGetReturn(uList);

	    if(!uR)goto ErrorOut;
	    
		if(uR[2].dreturn >= uR[3].dreturn){
		    Warning("Pmin Must be less than Pmax");
		    goto ErrorOut;
		}
		
		if(uR[5].lreturn){
		   scene->pd.sType = 0;
		}else if(uR[6].lreturn){
		   if(uR[2].dreturn <= 0.0){
		      Warning("Pmin Must be Greater than Zero for Log Plots");
		      goto ErrorOut;
		   }
		   scene->pd.sType = 1;
		}else if(uR[7].lreturn){
		   scene->pd.sType = 2;
		}


		dType=0;
		if(uR[15].lreturn){
		    dType=1;
		}else if(uR[16].lreturn){
		    dType=2;
		}


		scene->pa.DrawZones=(int)dType;
		scene->pa.DrawZonesColor=(int)uR[18].lreturn;
		
		scene->pa.ZoneLevel=(int)uR[17].dreturn;
		scene->pa.flagGradients=(int)uR[19].lreturn;
		
		if(scene->CurrectObject){
			scene->CurrectObject->pa=scene->pa;
		}

		
		scene->pd.sPmin=(double)uR[2].dreturn;
		scene->pd.sPmax=(double)uR[3].dreturn;
				
		scene->pd.LabeledPalette =	(int)uR[4].lreturn;	
		
		imat=(int)uR[13].lreturn;
			
		if((imat >= 0) && (imat < scene->MaterialCount)){
		    scene->material[imat].pd=scene->pd;
		    scene->material[imat].pa=scene->pa;
		}
		
		adjustPalette2(scene,imat);
				
		doFrameImage(myIcon);

		{
	        struct ObjectMessage Message;

		    Message.MessageType=OBJECT_MESSAGE_FORCE_LOAD;
		    ListObjectMessage(scene->ObjectList,&Message);
		}
		
		if(r)RenderScene(r);
		
		if(uList)uDialogUpdate(uList);
	}
Ok:
	ret = 0;
	
ErrorOut:	
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}
static int doScale(IconPtr myIcon,int mat,struct CObjectStruct *o)
{
	static uDialogGroup Items[] =

        {
       /*  0 */      {"Range Min", {190,40,85,20},uDialogString,uDialogNone},
       /*  1 */      {"Range Max", {190,60,85,20},uDialogString,uDialogNone},
       /*  2 */      {"0", {280,40,110,20},uDialogText,uDialogGetDouble},
       /*  3 */      {"0", {280,65,110,20},uDialogText,uDialogGetDouble},
       /*  4 */      {"Labeled Palette", {20,95,130,20},uDialogCheck,uDialogGetLong},
       /*  5 */      {"Auto Scale - Linear", {20,10,160,18},uDialogButtonRadio,uDialogGetLong},
       /*  6 */      {"Manual Scale - Log", {20,40,160,16},uDialogButtonRadio,uDialogGetLong},
       /*  7 */      {"Manual Scale - Linear", {20,65,160,20},uDialogButtonRadio,uDialogGetLong},
       /*  8 */      {"Ok", {160,225,95,20},uDialogButton,uDialogNone},
       /*  9 */      {"Cancel", {260,225,95,20},uDialogButton,uDialogNone},
       /* 10*/       {"Show Range", {20,145,100,20},uDialogButton,uDialogNone},
       /* 11*/       {"Render", {20,225,100,20},uDialogButton,uDialogNone},
       /* 12 */      {"Material",{190,15,85,20},uDialogString,uDialogNone},
       /* 13 */      {"0", {280,15,110,20},uDialogText,uDialogGetLong},
       /* 14 */      {"Hide Mesh", {170,105,100,20},uDialogButtonRadio,uDialogGetLong},
       /* 15 */      {"Show Mesh", {170,125,100,20},uDialogButtonRadio,uDialogGetLong},
       /* 16 */      {"Show To Level:", {170,145,120,20},uDialogButtonRadio,uDialogGetLong},
       /* 17 */      {"",{295,145,85,20},uDialogText,uDialogGetDouble},
       /* 18 */      {"Show Mesh Color",{170,170,160,20},uDialogCheck,uDialogGetLong},
       /* 19 */      {"Use Gradients",{20,120,120,20},uDialogCheck,uDialogGetLong},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,420,265};
 	
	struct Scene *scene;
	renderPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
	int dType;
    int reti;
    int ret;
    int n;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	
	if((mat >= 0) && (mat < scene->MaterialCount)){
	    scene->pd=scene->material[mat].pd;
	    scene->pa=scene->material[mat].pa;
	}
	
	scene->CurrectObject=o;
	
	if(o){
	    scene->pa=o->pa;
	}
	
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Range",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	reti=uDialogSet(uList,4L,
	              uDialogSetItem,2L,
	              uDialogSetDouble,scene->pd.sPmin,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,scene->pd.sPmax,
	              	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,(scene->pd.LabeledPalette == 1),
	              
	              uDialogSetItem,5L,
	              uDialogSetLogical,(scene->pd.sType == 0),
	              
	              uDialogSetItem,6L,
	              uDialogSetLogical,(scene->pd.sType == 1),

	              uDialogSetItem,7L,
	              uDialogSetLogical,(scene->pd.sType == 2),

	              uDialogSetItem,13L,
	              uDialogSetDouble,(double)mat,
	              
	              uDialogSetItem,14L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(scene->pa.DrawZones  == 0),
	              
	              uDialogSetItem,15L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(scene->pa.DrawZones  == 1),
	              
	              uDialogSetItem,16L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(scene->pa.DrawZones  == 2),

	              uDialogSetItem,17L,
	              uDialogSetDouble,(double)scene->pa.ZoneLevel,

	              uDialogSetItem,18L,
	              uDialogSetLogical,(scene->pa.DrawZonesColor  == 1),

	              uDialogSetItem,19L,
	              uDialogSetLogical,(scene->pa.flagGradients  == 1),

	              uDialogSetMyIcon,myIcon,	              
	              uDialogSetAction,doScale1Action,	              
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
Start:
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
    
	if(uR[2].dreturn >= uR[3].dreturn){
	    Warning("Min Must be less than Max");
	    goto Start;
	}
	
	if(uR[5].lreturn){
	   scene->pd.sType = 0;
	}else if(uR[6].lreturn){
	   if(uR[2].dreturn <= 0.0){
	      Warning("Min Must be Greater than Zero for Log Plots");
	      goto Start;
	   }
	   scene->pd.sType = 1;
	}else if(uR[7].lreturn){
	   scene->pd.sType = 2;
	}
	
	dType=0;
	if(uR[15].lreturn){
	    dType=1;
	}else if(uR[16].lreturn){
	    dType=2;
	}


	scene->pa.DrawZones=(int)dType;
	scene->pa.DrawZonesColor=(int)uR[18].lreturn;
	
	scene->pa.ZoneLevel=(int)uR[17].dreturn;
	scene->pa.flagGradients=(int)uR[19].lreturn;

	
	scene->pd.sPmin=(double)uR[2].dreturn;
	scene->pd.sPmax=(double)uR[3].dreturn;
			
	scene->pd.LabeledPalette =	(int)uR[4].lreturn;				
	
	r->mat=(int)uR[13].lreturn;	
       
	mat=r->mat;
		
	if((mat >= 0) && (mat < scene->MaterialCount)){
	    if(scene->pd.LabeledPalette){
	       for(n=0;n<scene->MaterialCount;++n){
	           scene->material[n].pd.LabeledPalette=FALSE;
	       }
	    }
	    	    
	    scene->material[mat].pd=scene->pd;
	    scene->material[mat].pa=scene->pa;
	}

	if(o){
	    o->pa=scene->pa;
	}
	
	adjustPalette2(scene,r->mat);

	doFrameImage(myIcon);

	ret=0;

ErrorOut:

	scene->CurrectObject=NULL;
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	{
        struct ObjectMessage Message;

	    Message.MessageType=OBJECT_MESSAGE_FORCE_LOAD;
	    ListObjectMessage(scene->ObjectList,&Message);
	}

	if(r)RenderScene(r);
	
	return ret;
}
int rangeE(struct Scene *scene,double *xmini,double *xmaxi,double *ymini,double *ymaxi,
           double *zmini,double *zmaxi,double *vmini,double *vmaxi)
{
	double xmin,xmax,ymin,ymax,zmin,zmax,vmin,vmax;
	struct Block *block;
	long BlockCount,n;

	BlockCount=scene->BlockCount;
	block=scene->block;

	xmin = 1e60;
	ymin = 1e60;
	zmin = 1e60;
	vmin = 1e60;

	xmax = -1e60;
	ymax = -1e60;
	zmax = -1e60;
	vmax = -1e60;

	for(n=0;n<BlockCount;++n){
	    elementrange(&block[n]);
	    if(block[n].xmin < xmin)xmin = block[n].xmin;
	    if(block[n].ymin < ymin)ymin = block[n].ymin;
	    if(block[n].zmin < zmin)zmin = block[n].zmin;
	    if(block[n].vmin < vmin)vmin = block[n].vmin;
	    if(block[n].xmax > xmax)xmax = block[n].xmax;
	    if(block[n].ymax > ymax)ymax = block[n].ymax;
	    if(block[n].zmax > zmax)zmax = block[n].zmax;
	    if(block[n].vmax > vmax)vmax = block[n].vmax;
	}

	*xmini = xmin;
	*ymini = ymin;
	*zmini = zmin;
	*vmini = vmin;

	*xmaxi = xmax;
	*ymaxi = ymax;
	*zmaxi = zmax;
	*vmaxi = vmax;
	return 0;
}
static int SaveMaterialData(struct uDialogStruct *uList,long n)
{
	struct Scene *scene;
	struct Material *m;
	uDialogReturn *uR;
	renderPtr r;
	IconPtr myIcon;
    int ret;
    
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	if((n < 0) || (n >= scene->MaterialCount))return 1;
	
	ret = 1;
	uR=NULL;
	
    m = &scene->material[n];
    uR=uDialogGetReturn(uList);
    if(!uR)goto ErrorOut;
    
    m->ImageData.rmin=uR[2].dreturn;
    m->ImageData.rmax=uR[3].dreturn;
    m->ImageData.repeatr=uR[4].lreturn;

    m->ImageData.smin=uR[5].dreturn;
    m->ImageData.smax=uR[6].dreturn;
    m->ImageData.repeats=uR[7].lreturn;
    
    m->ImageData.xmin=uR[8].lreturn;
    m->ImageData.xmax=uR[9].lreturn;
    m->ImageData.ymin=uR[10].lreturn;
    m->ImageData.ymax=uR[11].lreturn;
    
    m->diffuse.red=uR[12].dreturn;
    m->diffuse.green=uR[13].dreturn;
    m->diffuse.blue=uR[14].dreturn;
    
    m->opacity=uR[15].dreturn;
    m->specular=uR[16].dreturn;
    m->phong=uR[17].dreturn;
    
    m->ambient.red=uR[18].dreturn;
    m->ambient.green=uR[19].dreturn;
    m->ambient.blue=uR[20].dreturn;
    
    m->ImageData.tzero=uR[21].lreturn;
    m->ImageData.thold=uR[22].lreturn;
    m->ImageData.repeat=uR[23].lreturn;
    
    if(uR[25].lreturn){
		m->ImageData.cycleFlag = 1;
    }else if(uR[26].lreturn){
		m->ImageData.cycleFlag = 2;
   }else{
		m->ImageData.cycleFlag = 0;
    }
            
 		ret = 0;           
ErrorOut:	

    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int SetMaterialData(struct uDialogStruct *uList,long n)
{
	struct ScrollList *List0,*List1;
	struct Scene *scene;
	struct Material *m;
	renderPtr r;
	IconPtr myIcon;
	long countD;
    int reti;
    int ret;
    
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;

	
	if((n < 0) || (n >= scene->MaterialCount))return 1;
	
	reti=uDialogSet(uList,0L,
              uDialogGetScrollList,&List0,
              
              uDialogSetItem,1L,
              uDialogGetScrollList,&List1,
              	              	              
              uDialogSetDone,uDialogSetDone
    );		
	if(reti) return 1;
	
	ret = 1;
	
    m = &scene->material[n];
	reti=uDialogSet(uList,0L,
			uDialogSetItem,2L,
			uDialogSetDouble,(double)m->ImageData.rmin,

			uDialogSetItem,3L,
			uDialogSetDouble,(double)m->ImageData.rmax,
			              	              	              
			uDialogSetItem,4L,
			uDialogSetDouble,(double)m->ImageData.repeatr,
			              	              	              
			uDialogSetItem,5L,
			uDialogSetDouble,(double)m->ImageData.smin,
			
			uDialogSetItem,6L,
			uDialogSetDouble,(double)m->ImageData.smax,
			              	              	              
			uDialogSetItem,7L,
			uDialogSetDouble,(double)m->ImageData.repeats,
			
			uDialogSetItem,8L,
			uDialogSetDouble,(double)m->ImageData.xmin,
			
			uDialogSetItem,9L,
			uDialogSetDouble,(double)m->ImageData.xmax,
			
			uDialogSetItem,10L,
			uDialogSetDouble,(double)m->ImageData.ymin,
			
			uDialogSetItem,11L,
			uDialogSetDouble,(double)m->ImageData.ymax,
			
			uDialogSetItem,12L,
			uDialogSetDouble,(double)m->diffuse.red,
							
			uDialogSetItem,13L,
			uDialogSetDouble,(double)m->diffuse.green,
							
			uDialogSetItem,14L,
			uDialogSetDouble,(double)m->diffuse.blue,
							
			uDialogSetItem,15L,
			uDialogSetDouble,(double)m->opacity,
							
			uDialogSetItem,16L,
			uDialogSetDouble,(double)m->specular,
							
			uDialogSetItem,17L,
			uDialogSetDouble,(double)m->phong,
							
			uDialogSetItem,18L,
			uDialogSetDouble,(double)m->ambient.red,
							
			uDialogSetItem,19L,
			uDialogSetDouble,(double)m->ambient.green,
							
			uDialogSetItem,20L,
			uDialogSetDouble,(double)m->ambient.blue,
			
			uDialogSetItem,21L,
			uDialogSetDouble,(double)m->ImageData.tzero,
							
			uDialogSetItem,22L,
			uDialogSetDouble,(double)m->ImageData.thold,
							
			uDialogSetItem,23L,
			uDialogSetDouble,(double)m->ImageData.repeat,
			
			uDialogSetItem,24L,
            uDialogSetLogical,(m->ImageData.cycleFlag == 0),
			
			uDialogSetItem,25L,
            uDialogSetLogical,(m->ImageData.cycleFlag == 1),
			
			uDialogSetItem,26L,
            uDialogSetLogical,(m->ImageData.cycleFlag == 2),
            
			uDialogSetItem,55L,
			uDialogSetDouble,(double)m->ImageData.vmin,
            
			uDialogSetItem,56L,
			uDialogSetDouble,(double)m->ImageData.vmax,
            
            
							
			uDialogSetDone,uDialogSetDone
    );		
	if(reti) goto ErrorOut;
	
	
	countD=ScrollListCount(List1);
	ScrollListDelete(0L,countD,List1);

	if(m->ImageData.Files){
		ScrollListInsertOneType(m->ImageData.Files->ImageNames,0,0L,
			m->ImageData.Files->ImageCount,List1);
	}

	ret = 0;
ErrorOut:
	return ret;
}
static int ClickMaterialList0(struct uDialogStruct *uList)
{
	struct ScrollList *List0,*List1;
	struct Scene *scene;
	renderPtr r;
	IconPtr myIcon;
    int ret;
    long n;

	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;


	ret=uDialogSet(uList,0L,
              uDialogGetScrollList,&List0,
              
              uDialogSetItem,1L,
              uDialogGetScrollList,&List1,
              	              	              
              uDialogSetDone,uDialogSetDone
    );		
	if(ret)goto ErrorOut;
			
	n=ScrollListFirstSelect(List0);
	if(n >= 0){
	    if((scene->itIs) >= 0 && (scene->itIs != n)){
	         SaveMaterialData(uList,scene->itIs);
	    }
		scene->itIs=n;
	    SetMaterialData(uList,scene->itIs);		
	}
	
ErrorOut:
			
	if(uList){
		uDialogUpdate(uList);
		if(uList->myIcon)InvalRectMyWindow(uList->myIcon);
	}
	return 0;
}
static int doMaterialAction(struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	struct Scene *scene;
	struct Material *m;
	IconPtr myIcon;	
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
	
	ret = 1;
	uR=NULL;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 0){
	   ClickMaterialList0(uList);
	}else if(uList->actionItem == 27){
		SaveMaterialData(uList,scene->itIs);
	}else if(uList->actionItem == 29){
		SaveMaterialData(uList,scene->itIs);
	    {
			struct ObjectMessage Message;

			Message.MessageType=OBJECT_MESSAGE_FORCE_LOAD;
			ListObjectMessage(scene->ObjectList,&Message);
	    }
		if(r)RenderScene(r);
	}else if(uList->actionItem == 30){	
    	struct FileList *ret;

	    if(scene->itIs < 0){
	        Warning("Must Select Material First");
	        return 0;
	    }

	    ret=FileDialog(NULL,"File Mapping",FileDialog_GetFiles,&myIcon->infoLoad);		

	    if(ret){
			struct FileInfo2 *files;

			files=FilesOpenList(ret);

			if(!files)return 1;

			if(files->fileClass == SDS2D_FILES ||
			   files->fileClass == HDF_FILES   ||
			   files->fileClass == TRUE_FILES  ||
			   files->fileClass == SDS3D_FILES){
				if(scene->itIs >= 0){
				    struct Material *m;

				    m = &scene->material[scene->itIs];
				    m->ImageData.Files=files;
				    m->PatternData=(char *)&m->ImageData;
				    m->PatternType = PATTERN_IMAGE;
				    m->ImageData.tmax=files->ImageCount;				    
				    m->ImageData.xmax=files->xmax;
				    m->ImageData.ymax=files->ymax;
				    
	    			SetMaterialData(uList,scene->itIs);		
				}
			}else{
		        sprintf(WarningBuff,"Cannot map fileclass %d",files->fileClass);
		        Warning(WarningBuff);
			}
	    }
	    
	}else if(uList->actionItem == 31){	
	    if(scene->itIs < 0){
	        Warning("Must Select Material First");
	        goto ErrorOut;
	    }
	    
		m = &scene->material[scene->itIs];
	    m->PatternData=NULL;
	    m->PatternType = 0;
	    m->ImageData.tmax=0;
		if(m->ImageData.Files){
		    freeFileList(m->ImageData.Files);
		    m->ImageData.Files=NULL;
		}
	    SetMaterialData(uList,scene->itIs);		
	}else if(uList->actionItem == 52){	
	    double dmin;
	    double dmax;
	    double *data;
	    long length;
	    long count;
	    long first;
	    long n;
	    long k;
	
	    if(scene->itIs < 0){
	        Warning("Must Select Material First");
	        return 0;
	    }
	    
		m = &scene->material[scene->itIs];
	    
		reti=uDialogSet(uList,1L,
	              uDialogGetScrollList,&scroll,
	              uDialogSetDone,uDialogSetDone
	    );
	    
		if(reti)goto ErrorOut;
	    
		first = ScrollListFirstSelect(scroll);
		if(first < 0){
		   Warning("No Images Selected To Range");
		   return 0;
		}
		
		dmin =  1e60;
		dmax = -1e60;
		count=ScrollListCount(scroll);

		for(n=0;n<count;++n){
			if(ScrollListIsSelected(n,scroll)){
			    if(FilesGetData(&data,&length,n,m->ImageData.Files))break;
			    for(k=0;k<length;++k){
			       if(data[k] > dmax)dmax=data[k];
			       if(data[k] < dmin)dmin=data[k];
			    }
			}
		}
		
		reti=uDialogSet(uList,0L,
			uDialogSetItem,55L,
			uDialogSetDouble,(double)dmin,
			
			uDialogSetItem,56L,
			uDialogSetDouble,(double)dmax,
			
		      uDialogSetDone,uDialogSetDone
		);
		
		if(reti)goto ErrorOut;
		
	}else if(uList->actionItem == 57){	
		getNextMaterial(scene);
		doMaterialStart(uList);
		ret=uDialogSet(uList,0L,
	              uDialogGetScrollList,&scroll,
	              uDialogSetDone,uDialogSetDone
	    );
	    
		if(ret)goto ErrorOut;
		ScrollListTop(scene->MaterialCount-1,scroll);
		ScrollListSetItemSelected(scene->MaterialCount-1,scroll);
		ClickMaterialList0(uList);
	}

	ret = 0;
	
ErrorOut:	
	if(uList)uDialogUpdate(uList);
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}
static int doMaterialStart(struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	IconPtr myIcon;
	struct Scene *scene;
	renderPtr r;
	long count;
	int ret;
	long n;
	
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
		
	ret=uDialogSet(uList,0L,
              uDialogGetScrollList,&scroll,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	count=ScrollListCount(scroll);
	if(count){
		ScrollListDelete(0L,count,scroll);	
	}	
	
	
	for(n=0;n<scene->MaterialCount;++n){
	    char buff[256];
		char *data;
		char *names[]={"Material",};
		data = &buff[0];
	    sprintf(buff,"%s<%ld>",names[0],n);
		if(ScrollListInsertOneType(&data,3,n,
	    	1L,scroll))return 1;
	}
	ScrollListTop(0L,scroll);


	return 0;
}
static int doMaterials(IconPtr myIcon)
{
	static uDialogGroup Items[] =

        {
      /*  0 */      {"materials",{20,20,150,135},uDialogScrollList,uDialogNone},
      /*  1 */      {"images",{170,20,150,135},uDialogScrollList,uDialogNone},
      
      /*  2 */      {"0", {83,178,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"0", {83,203,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"0", {83,228,80,20},uDialogText,uDialogGetLong},
      /*  5 */      {"0", {83,253,80,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"0", {83,278,80,20},uDialogText,uDialogGetDouble},
      /*  7 */      {"0", {83,303,80,20},uDialogText,uDialogGetLong},
      
      /*  8 */      {"0", {215,179,80,20},uDialogText,uDialogGetLong},
      /*  9 */      {"0", {215,204,80,20},uDialogText,uDialogGetLong},
      /* 10 */      {"0", {215,229,80,20},uDialogText,uDialogGetLong},
      /* 11 */      {"0", {215,254,80,20},uDialogText,uDialogGetLong},
      
      /* 12 */      {"0", {397,10,55,20},uDialogText,uDialogGetDouble},
      /* 13 */      {"0", {457,10,55,20},uDialogText,uDialogGetDouble},
      /* 14 */      {"0", {517,10,55,20},uDialogText,uDialogGetDouble},
      
      /* 15 */      {"0", {397,35,55,20},uDialogText,uDialogGetDouble},
      /* 16 */      {"0", {397,60,55,20},uDialogText,uDialogGetDouble},
      /* 17 */      {"0", {397,85,55,20},uDialogText,uDialogGetDouble},
      
      /* 18 */      {"0", {397,110,55,20},uDialogText,uDialogGetDouble},
      /* 19 */      {"0", {457,110,55,20},uDialogText,uDialogGetDouble},
      /* 20 */      {"0", {517,110,55,20},uDialogText,uDialogGetDouble},
      
      /* 21 */      {"0", {395,178,55,20},uDialogText,uDialogGetLong},
      /* 22 */      {"0", {395,203,55,20},uDialogText,uDialogGetLong},
      /* 23 */      {"0", {395,228,55,20},uDialogText,uDialogGetLong},
      
      /* 24 */      {"hold", {469,176,58,19},uDialogButtonRadio,uDialogGetLong},
      /* 25 */      {"cycle", {469,205,61,17},uDialogButtonRadio,uDialogGetLong},
      /* 26 */      {"hide", {469,233,67,18},uDialogButtonRadio,uDialogGetLong},
      
      /* 27 */      {"Ok", {15,332,80,20},uDialogButton,uDialogNone},
      /* 28 */      {"Cancel", {105,332,80,20},uDialogButton,uDialogNone},
      /* 29 */      {"Render", {195,332,80,20},uDialogButton,uDialogNone},
      /* 30 */      {"Images", {285,332,80,20},uDialogButton,uDialogNone},
      /* 31 */      {"Delete Images", {375,332,111,21},uDialogButton,uDialogNone},
      
      /* 32 */      {"opacity", {325,34,65,20},uDialogString,uDialogNone},
      /* 33 */      {"diffuse", {325,9,65,20},uDialogString,uDialogNone},
      /* 34 */      {"xmin", {170,179,41,20},uDialogString,uDialogNone},
      /* 35 */      {"xmax", {170,204,41,17},uDialogString,uDialogNone},
      /* 36 */      {"ymin", {170,229,42,20},uDialogString,uDialogNone},
      /* 37 */      {"rmin", {8,176,53,19},uDialogString,uDialogNone},
      /* 38 */      {"rmax", {8,202,53,20},uDialogString,uDialogNone},
      /* 39 */      {"repeat-r", {8,226,70,19},uDialogString,uDialogNone},
      /* 40 */      {"ymax", {170,254,42,21},uDialogString,uDialogNone},
      /* 41 */      {"smin", {8,251,53,22},uDialogString,uDialogNone},
      /* 42 */      {"smax", {8,276,53,21},uDialogString,uDialogNone},
      /* 43 */      {"repeat-s", {8,301,70,20},uDialogString,uDialogNone},
      /* 44 */      {"phong i", {325,59,65,20},uDialogString,uDialogNone},
      /* 45 */      {"phong c", {325,84,65,20},uDialogString,uDialogNone},
      /* 46 */      {"ambient", {325,109,65,20},uDialogString,uDialogNone},
      /* 47 */      {"Materials", {70,2,75,16},uDialogString,uDialogNone},
      /* 48 */      {"Images", {204,3,75,16},uDialogString,uDialogNone},
      /* 49 */      {"thold", {315,202,65,20},uDialogString,uDialogNone},
      /* 50 */      {"tzero", {315,177,77,20},uDialogString,uDialogNone},
      /* 51 */      {"repeat", {315,227,65,20},uDialogString,uDialogNone},
      
      /* 52 */      {"Range",  {495,332,80,21},uDialogButton,uDialogNone},
      
      /* 53 */      {"vmin", {315,252,65,20},uDialogString,uDialogNone},
      /* 54 */      {"vmax", {315,277,65,20},uDialogString,uDialogNone},

      /* 55 */      {"0", {395,252,115,20},uDialogText,uDialogGetLong},
      /* 56 */      {"0", {395,277,115,20},uDialogText,uDialogGetLong},
      /* 57 */      {"New Material", {195,302,110,20},uDialogButton,uDialogNone},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,593,370};
 	
    struct Material *material;
    long MaterialCount;
    long MaterialUsed;
        
	struct Scene *scene;
	renderPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
    int reti;
    int ret;
    int n;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	scene->itIs = -1;
	
	MaterialCount=scene->MaterialCount;
	MaterialUsed=scene->MaterialUsed;
	material=(struct Material *)cMalloc(scene->MaterialCount*sizeof(struct Material),8607);
	if(material){
	    for(n=0;n<MaterialCount;++n){
			material[n]=scene->material[n];
	    }
	}
	
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Materials",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	reti=uDialogSet(uList,0L,
	              
	              uDialogSetMyIcon,myIcon,	              
	              uDialogSetAction,doMaterialAction,	              
	              uDialogSetStart,doMaterialStart,
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
    
		       
	ret=0;
	

ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
    
    if(ret){    
	    if(material){
			for(n=0;n<MaterialCount;++n){
			    scene->material[n]=material[n];
			}
		    scene->MaterialCount=MaterialCount;
			scene->MaterialUsed=MaterialUsed;
	    }    
    }
    
	if(material)cFree((char *)material);
	
	{
        struct ObjectMessage Message;

	    Message.MessageType=OBJECT_MESSAGE_FORCE_LOAD;
	    ListObjectMessage(scene->ObjectList,&Message);
	}


	if(r)RenderScene(r);
	
	return ret;
}
static int Rotate1Action(struct uDialogStruct *uList)
{
	IconPtr myIcon;	
	struct Scene *scene;
	renderPtr r;
	int reti;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret = 1;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 6){
			reti=uDialogSet(uList,0L,
              uDialogSetExitOk,
	          uDialogSetItem,12L,
 	          uDialogSetFloatAux,(double)(uList->actionItem),
              uDialogSetDone,uDialogSetDone
    		);
    
			if(reti)goto ErrorOut;
	}
	
	ret = 0;
	
ErrorOut:	
	/* if(uList)uDialogUpdate(uList); */
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}
static int doRotate(IconPtr myIcon)
{
	static uDialogGroup Items[] =
        {
       /*  0 */      {"0", {100,50,80,20},uDialogText,uDialogGetLong},
       /*  1 */      {"0", {100,80,80,20},uDialogText,uDialogGetLong},
       /*  2 */      {"0", {300,50,80,20},uDialogText,uDialogNone},
       /*  3 */      {"0", {300,80,80,20},uDialogText,uDialogGetLong},
       /*  4 */      {"Save Images", {220,20,100,20},uDialogCheck,uDialogGetLong},
       /*  5 */      {"Object View Angles", {40,20,150,20},uDialogString,uDialogNone},
       /*  6 */      {"Ok", {220,120,70,20},uDialogButton,uDialogNone},
       /*  7 */      {"Cancel", {310,120,70,20},uDialogButton,uDialogNone},
       /*  8 */      {"Sequence", {220,80,70,20},uDialogString,uDialogNone},
       /*  9 */      {"Tilt Steps", {20,50,70,20},uDialogString,uDialogNone},
       /* 10 */      {"Rotate Steps", {20,80,70,20},uDialogString,uDialogNone},
       /* 11 */      {"Root Name", {220,50,70,20},uDialogString,uDialogNone},
       /* 12 */      {"Exit", {69,70,70,20},uDialogFloatAux,uDialogFloatAux},
        };

    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,400,160};
 	
	static char saveFileName[128]={'r','o','t','a','t','e','.','t','r','u',};
	char fileName[256],Directory[512],FileOut[1280];
	char buff[256];
	IconPtr StatusWindow;
	
	struct Scene *scene;
	renderPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
	static long tiltsteps=9;
	static long rotatesteps=36;
	double DegreesToRadians;
	struct P EyeSave,originSave;
	double vstep,vinc,vert,vertDegree;
	double hstep,hinc,hort,hortDegree;
	struct P /*EyeToLamp, */EyeToTarget,Up,Target;
	struct P x,y,z;
	struct P xEye,yEye,zEye;
	double radius,rOut;
	struct Sphere lamp;
	int saveWhere;
    int reti;
    int ret;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret=1;
	
	uR=NULL;
	
	r->saveWhere=0;
	
	StatusWindow=NULL;
	
	lamp=scene->lamp;
	originSave=lamp.origin;
	EyeSave=scene->Eye;
	
	
	uList=uDialogOpen("Rotate Objects",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	
		reti=uDialogSet(uList,0L,
		          uDialogSetDouble,(double)tiltsteps,
	              
	              uDialogSetItem,1L,
	              uDialogSetDouble,(double)rotatesteps,
	              	              	              
	              uDialogSetItem,2L,
	              uDialogSetText,scene->sequenceName,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)scene->sequence,
	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,(scene->sequenceSave == 1),
	              
	              uDialogSetMyIcon,myIcon,	              
	              uDialogSetAction,Rotate1Action,	              
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
Start:
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
		
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	if(uR[0].lreturn <= 2){
	    Warning("Tilt Steps Must Be Greater Than 2");
	    goto Start;
	}
	
	if(uR[1].lreturn <= 2){
	    Warning("Rotate Steps Must Be Greater Than 2");
	    goto Start;
	}
	
	tiltsteps=uR[0].lreturn;
	    	
	rotatesteps=uR[1].lreturn;
	    		
	mstrncpy(scene->sequenceName,uR[2].sreturn,255);
	
	scene->sequence=uR[3].lreturn;
	
	scene->sequenceSave=uR[4].lreturn;
		
    if(uList)uDialogClose(uList);
    uList=NULL;
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
		
  	DegreesToRadians=atan(1.0)/45.0;
	
        
	vstep = (int)tiltsteps;
	vinc= 180./(vstep-1.0);
	hstep= (int)rotatesteps;
	hinc=360./hstep;
	radius=sqrt((scene->Eye.x-scene->Target.x)*(scene->Eye.x-scene->Target.x)+
                (scene->Eye.y-scene->Target.y)*(scene->Eye.y-scene->Target.y)+
                (scene->Eye.z-scene->Target.z)*(scene->Eye.z-scene->Target.z));
/*
	EyeToLamp.x=lamp.origin.x-scene->Eye.x;
	EyeToLamp.y=lamp.origin.y-scene->Eye.y;
	EyeToLamp.z=lamp.origin.z-scene->Eye.z;
*/    
	EyeToTarget=Sub(&scene->Target,&scene->Eye);
	EyeToTarget=Norm(&EyeToTarget);
	
	Up=Norm(&scene->Up);
	
	z=Up;
	x=CrossN(&EyeToTarget,&Up);
	y=CrossN(&z,&x);
	
	Target=scene->Target;
	
	if(scene->sequenceSave > 0){	
	    saveWhere=doSetImageInfo(saveFileName,Directory,1);
	    if(saveWhere == 1)return 1;
	}else{
	    saveWhere = 1;
	}


	StatusWindow=uStartStatusWindow(myIcon,"Saving Ojbect");
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

    for(vert=90.0;vert>= -90.0; vert -= vinc){
		vertDegree = vert;
		if(vertDegree == 90){
			vertDegree -= .0001;
		}else if(vertDegree == -90){
			vertDegree += .0001;
		}
		rOut=radius*cos(vertDegree*DegreesToRadians);
		zEye=Mult(&z,radius*sin(vertDegree*DegreesToRadians));
		for(hort=0;hort<hstep;++hort){
			hortDegree=hort*hinc;

			xEye=Mult(&x,rOut*cos(hortDegree*DegreesToRadians));
			yEye=Mult(&y,rOut*sin(hortDegree*DegreesToRadians));


			scene->Eye=p(xEye.x+yEye.x+zEye.x+Target.x,
				 xEye.y+yEye.y+zEye.y+Target.y,
				 xEye.z+yEye.z+zEye.z+Target.z);
/*
			lamp.origin.x=scene->Eye.x+EyeToLamp.x;
			lamp.origin.y=scene->Eye.y+EyeToLamp.y;
			lamp.origin.z=scene->Eye.z+EyeToLamp.z;
*/
			scene->lamp.origin=scene->Eye;

			sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
			sprintf(buff,"Creating: %s",fileName);
			mstrncpy(FileOut,Directory,1280);
			strncatToPath(FileOut,fileName,1280);
			uWriteStatusWindow(StatusWindow,buff);
			if(uCheckStatusWindow(StatusWindow,"Stop Contours"))goto ErrorOut;
			RenderScene(r);
			++scene->sequence;	    
    	}
	}
	
	ret = 0;	
	
ErrorOut:

	scene->Eye=EyeSave;
	scene->lamp.origin=originSave;
	
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
	
	return ret;
}
static int Time1Action(struct uDialogStruct *uList)
{
	IconPtr myIcon;	
	struct Scene *scene;
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
	
	ret = 1;
	uR=NULL;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 13 || uList->actionItem == 14){
			reti=uDialogSet(uList,0L,
              uDialogSetExitOk,
	          uDialogSetItem,17L,
 	          uDialogSetFloatAux,(double)(uList->actionItem),
              uDialogSetDone,uDialogSetDone
    		);    
			if(reti)goto ErrorOut;
	}else if(uList->actionItem == 15){
		uR=uDialogGetReturn(uList);

    	if(!uR)goto ErrorOut;
    	    	
		scene->time=uR[3].lreturn;
		
		mstrncpy(scene->sequenceName,uR[4].sreturn,255);
		
		scene->sequence=uR[5].lreturn;
		
		scene->sequenceSave=uR[6].lreturn;
		
		    	    	
	    if(r){
	        RenderScene(r);
	    }
	}

	ret = 0;
	
ErrorOut:	
	/* if(uList)uDialogUpdate(uList); */
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}
static int doTimeAnimation(IconPtr myIcon)
{
	static uDialogGroup Items[] =
        {
       /*  0 */      {"0", {120,20,40,20},uDialogText,uDialogGetLong},
       /*  1 */      {"0", {120,50,40,20},uDialogText,uDialogGetLong},
       /*  2 */      {"0", {120,80,40,20},uDialogText,uDialogGetLong},
       /*  3 */      {"0", {120,140,40,20},uDialogText,uDialogGetLong},
       /*  4 */      {"0", {280,50,80,20},uDialogText,uDialogNone},
       /*  5 */      {"0", {280,80,80,20},uDialogText,uDialogGetLong},
       /*  6 */      {"Save Images", {200,20,100,20},uDialogCheck,uDialogGetLong},
       /*  7 */      {"Sequence", {200,80,70,20},uDialogString,uDialogNone},
       /*  8 */      {"First Time", {20,20,80,20},uDialogString,uDialogNone},
       /*  9 */      {"Last Time", {20,50,80,20},uDialogString,uDialogNone},
       /* 10 */      {"Root Name", {200,50,60,20},uDialogString,uDialogNone},
       /* 11 */      {"Increment", {20,80,80,20},uDialogString,uDialogNone},
       /* 12 */      {"Current Time", {20,140,90,20},uDialogString,uDialogNone},
       /* 13 */      {"Ok", {190,200,70,20},uDialogButton,uDialogNone},
       /* 14 */      {"Set", {170,140,70,20},uDialogButton,uDialogNone},
       /* 15 */      {"Render", {250,140,70,20},uDialogButton,uDialogNone},
       /* 16 */      {"Cancel", {280,200,70,20},uDialogButton,uDialogNone},
       /* 17 */      {"Exit", {69,70,70,20},uDialogFloatAux,uDialogFloatAux},
       /* 18 */      {"Last Time", {20,170,90,20},uDialogString,uDialogNone},
       /* 19 */      {"0", {120,170,40,20},uDialogText,uDialogGetLong},
       };

    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,370,240};
 	
	static long first=0,last=1,inc=1;
	
	static char saveFileName[256]={'t','i','m','e','.','t','r','u',};
	char fileName[256],Directory[512],FileOut[1280];
	char buff[256];
	IconPtr StatusWindow;
	struct Message1000 m;
	
	struct Scene *scene;
	renderPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
	int saveWhere;
    int reti;
    int ret;
    long n;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	m.FrameCount=0;
		
	SendMessageByIcon(myIcon,MessageType_GET_FRAME_COUNT,&m);
	
	if(--m.FrameCount < 0)m.FrameCount=0;
	
	ret=1;
	
	uR=NULL;
	
	r->saveWhere=0;
	
	StatusWindow=NULL;
	
	uList=uDialogOpen("Time Animation",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	
	reti=uDialogSet(uList,0L,
		          uDialogSetDouble,(double)first,
	              
	              uDialogSetItem,1L,
	              uDialogSetDouble,(double)last,
	              
	              uDialogSetItem,2L,
	              uDialogSetDouble,(double)inc,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)scene->time,
	              	              
	              uDialogSetItem,4L,
	              uDialogSetText,scene->sequenceName,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)scene->sequence,
	              
	              uDialogSetItem,6L,
	              uDialogSetLogical,(scene->sequenceSave == 1),
	              
	              uDialogSetItem,19L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)m.FrameCount,
	              	              
	              uDialogSetMyIcon,myIcon,	              
	              uDialogSetAction,Time1Action,	              
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
Start:
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
		
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	if(uR[0].lreturn >= uR[1].lreturn){
	    Warning("First Must Be Less Than Last");
	    goto Start;
	}
	
	if(uR[2].lreturn <= 0){
	    Warning("Increment Must Be Greater Than Zero");
	    goto Start;
	}
	first=uR[0].lreturn;
	    	
	last=uR[1].lreturn;
	    	
	inc=uR[2].lreturn;
	    	
	scene->time=uR[3].lreturn;
	
	mstrncpy(scene->sequenceName,uR[4].sreturn,255);
	
	scene->sequence=uR[5].lreturn;
	
	scene->sequenceSave=uR[6].lreturn;
	
	if(uR[17].dreturn == 14){
	    goto Ok;
	}
	
    if(uList)uDialogClose(uList);
    uList=NULL;
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
		
	if(scene->sequenceSave > 0){	
	    saveWhere=doSetImageInfo(saveFileName,Directory,1);
	    if(saveWhere == 1)return 1;
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

    for(n=first;n<=last; n += inc){
		scene->time=n;
        sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
        sprintf(buff,"Creating: %s",fileName);
        mstrncpy(FileOut,Directory,1280);
        strncatToPath(FileOut,fileName,1280);
        uWriteStatusWindow(StatusWindow,buff);
        if(uCheckStatusWindow(StatusWindow,"Stop Aninamtion"))goto ErrorOut;
        RenderScene(r);
        ++scene->sequence;
	}
	

Ok:

	r->CurrentFrame=scene->time;
	if(r->CurrentFrame >= r->ImageCount)r->CurrentFrame=r->ImageCount-1;	
	if(r->CurrentFrame < 0)r->CurrentFrame=0;
	
	ret = 0;	
	
ErrorOut:

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
	
	return ret;
}
static int Options1Action(struct uDialogStruct *uList)
{
	IconPtr myIcon;	
	struct Scene *scene;
	renderPtr r;
	uDialogReturn *uR;
	int threads;
	int reti;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret = 1;
	uR=NULL;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 5){
			reti=uDialogSet(uList,0L,
              uDialogSetExitOk,
              uDialogSetDone,uDialogSetDone
    		);
    
			if(reti)goto ErrorOut;
	}else if(uList->actionItem == 7){
		uR=uDialogGetReturn(uList);

    	if(!uR)goto ErrorOut;
    	    	
		scene->xResolution=uR[0].lreturn;
		scene->yResolution=uR[1].lreturn;
		
		scene->BackGround.red=uR[2].dreturn;		
    	scene->BackGround.green=uR[3].dreturn;
    	scene->BackGround.blue=uR[4].dreturn;
    	
		scene->Transparency=uR[8].lreturn;
		
		scene->TransparencyPasses=uR[15].lreturn;

		scene->Jitter=uR[18].lreturn;

		scene->JitterCount=uR[19].lreturn;

		scene->FixRayObjects=uR[20].lreturn;
		
		scene->AntiAlias=uR[22].lreturn;
		
		scene->MotionBlur=uR[24].lreturn;
		
		scene->threads=uR[27].lreturn;

		scene->debug=uR[29].lreturn;
		
		if(uR[9].lreturn){
		   scene->SmoothNormals=0;
		}else if(uR[10].lreturn){
		   scene->SmoothNormals=1;
		}else if(uR[11].lreturn){
		   scene->SmoothNormals=2;
		}else if(uR[25].lreturn){
		   scene->SmoothNormals=3;
		}else{
		   scene->SmoothNormals=0;
		}
		
		if(uR[17].lreturn){
		   scene->RayTracing=1;
		}else if(uR[30].lreturn){
		   scene->RayTracing=2;
		}else{
		   scene->RayTracing=0;
		}	
		
		threads=scene->threads;
		if(scene->RayTracing == 0){
	         scene->threads=0;
		}

	    if(r){
	    	fixScene(r);
			doFrameImage(myIcon);
	        RenderScene(r);
	    }
	    
	    scene->threads=threads;
	}

	ret = 0;
	
ErrorOut:	
	/* if(uList)uDialogUpdate(uList); */
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}
int GetPlotOptions(IconPtr myIcon)
{
	static uDialogGroup Items[] =
        {
      /*  0 */      {"0", {116,14,80,20},uDialogText,uDialogGetLong},
      /*  1 */      {"0", {116,39,80,20},uDialogText,uDialogGetLong},
      /*  2 */      {"0", {116,64,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"0", {201,64,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"0", {286,64,80,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"Ok", {220,325,70,20},uDialogButton,uDialogNone},
      /*  6 */      {"Cancel", {310,325,70,20},uDialogButton,uDialogNone},
      /*  7 */      {"Render", {20,325,70,20},uDialogButton,uDialogNone},
      /*  8 */      {"Transparency", {20,185,125,20},uDialogCheck,uDialogGetLong},
      /*  9 */      {"Flat Shading", {20,100,130,20},uDialogButtonRadio,uDialogGetLong},
      /* 10 */      {"Gouraud Shading", {20,120,130,20},uDialogButtonRadio,uDialogGetLong},
      /* 11 */      {"Phong Shading (fast)", {20,140,160,20},uDialogButtonRadio,uDialogGetLong},
      /* 12 */      {"Background", {20,64,93,20},uDialogString,uDialogNone},
      /* 13 */      {"xsize", {20,14,65,20},uDialogString,uDialogNone},
      /* 14 */      {"ysize", {20,39,65,20},uDialogString,uDialogNone},
      /* 15 */      {"0", {85,210,80,20},uDialogText,uDialogGetLong},
      /* 16 */      {"Passes", {20,210,55,20},uDialogString,uDialogNone},
      /* 17 */      {"BSP Ray Tracing", {200,140,125,20},uDialogButtonRadio,uDialogGetLong},
      /* 18 */      {"Jitter", {200,185,55,20},uDialogCheck,uDialogGetLong},
      /* 19 */      {"0", {285,185,80,20},uDialogText,uDialogGetLong},
      /* 20 */      {"Fix Ray Objects", {200,160,130,20},uDialogCheck,uDialogGetLong},
      /* 21 */      {"Anti Alias", {200,210,65,20},uDialogString,uDialogNone},
      /* 22 */      {"0", {285,210,80,20},uDialogText,uDialogGetLong},
      /* 23 */      {"Motion Blur", {200,235,95,20},uDialogString,uDialogNone},
      /* 24 */      {"0", {285,235,80,20},uDialogText,uDialogGetLong},
      /* 25 */      {"Phong Shading (best)", {20,160,160,20},uDialogButtonRadio,uDialogGetLong},
      /* 26 */      {"threads", {200,260,95,20},uDialogString,uDialogNone},
      /* 27 */      {"0", {285,260,80,20},uDialogText,uDialogGetLong},
      /* 28 */      {"debug", {200,285,95,20},uDialogString,uDialogNone},
      /* 29 */      {"0", {285,285,80,20},uDialogText,uDialogGetLong},
      /* 30 */      {"KdTree Ray Tracing", {200,120,145,20},uDialogButtonRadio,uDialogGetLong},
      /* 31 */      {"Ray Tracing Off", {200,100,125,20},uDialogButtonRadio,uDialogGetLong},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,400,380};
 	
	struct Scene *scene;
	renderPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
    int reti;
    int ret;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Options",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	reti=uDialogSet(uList,0L,
		          uDialogSetDouble,(double)scene->xResolution,
	              
	              uDialogSetItem,1L,
	              uDialogSetDouble,(double)scene->yResolution,
	              
	              uDialogSetItem,2L,
	              uDialogSetDouble,scene->BackGround.red,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,scene->BackGround.green,
	              	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,scene->BackGround.blue,
	              
	              uDialogSetItem,8L,
	              uDialogSetLogical,(scene->Transparency == 1),
	              
	              uDialogSetItem,9L,
	              uDialogSetLogical,(scene->SmoothNormals == 0),
	          	  uDialogSetRadioGroup,1,
	              
	              uDialogSetItem,10L,
	              uDialogSetLogical,(scene->SmoothNormals == 1),
	          	  uDialogSetRadioGroup,1,

	              uDialogSetItem,11L,
	              uDialogSetLogical,(scene->SmoothNormals == 2),
	          	  uDialogSetRadioGroup,1,
	              
	              uDialogSetItem,15L,
	              uDialogSetDouble,(double)scene->TransparencyPasses,
	              
	              uDialogSetItem,17L,
	              uDialogSetLogical,(scene->RayTracing == 1),
	          	  uDialogSetRadioGroup,2,
	              
	              uDialogSetItem,18L,
	              uDialogSetLogical,(scene->Jitter == 1),
	              
	              uDialogSetItem,19L,
	              uDialogSetDouble,(double)scene->JitterCount,
	              
	              uDialogSetItem,20L,
	              uDialogSetLogical,(scene->FixRayObjects == 1),
	              
	              uDialogSetItem,22L,
	              uDialogSetDouble,(double)scene->AntiAlias,
	              
	              uDialogSetItem,24L,
	              uDialogSetDouble,(double)scene->MotionBlur,
	              
	              uDialogSetItem,25L,
	              uDialogSetLogical,(scene->SmoothNormals == 3),
	          	  uDialogSetRadioGroup,1,
	              
	              uDialogSetItem,27L,
	              uDialogSetDouble,(double)scene->threads,
	              
	              uDialogSetItem,29L,
	              uDialogSetDouble,(double)scene->debug,
	              
	              uDialogSetItem,30L,
	              uDialogSetLogical,(scene->RayTracing == 2),
	          	  uDialogSetRadioGroup,2,
	              
	              uDialogSetItem,31L,
	              uDialogSetLogical,(scene->RayTracing == 0),
	          	  uDialogSetRadioGroup,2,
	              
	              
	              uDialogSetMyIcon,myIcon,	              
	              uDialogSetAction,Options1Action,	              
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
Start:
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	    	
	if(uR[0].lreturn < 2){
	    Warning("xResolution Must be at least 2");
	    goto Start;
	}
	
	if(uR[1].lreturn < 2){
	    Warning("yResolution Must be at least 2");
	    goto Start;
	}
	
	if(uR[19].lreturn > 1024){
	    Warning("Pixel Samples Must less then 1024");
	    goto Start;
	}
	
	scene->xResolution=uR[0].lreturn;
	
	scene->yResolution=uR[1].lreturn;
	
	scene->BackGround.red=uR[2].dreturn;		
	scene->BackGround.green=uR[3].dreturn;
	scene->BackGround.blue=uR[4].dreturn;
	
	scene->Transparency=uR[8].lreturn;
	
	scene->TransparencyPasses=uR[15].lreturn;

	
	scene->Jitter=uR[18].lreturn;
	
	scene->JitterCount=uR[19].lreturn;
	
	scene->FixRayObjects=uR[20].lreturn;
	
	scene->AntiAlias=uR[22].lreturn;

	scene->MotionBlur=uR[24].lreturn;

	scene->threads=uR[27].lreturn;

	scene->debug=uR[29].lreturn;

	if(uR[9].lreturn){
	   scene->SmoothNormals=0;
	}else if(uR[10].lreturn){
	   scene->SmoothNormals=1;
	}else if(uR[11].lreturn){
	   scene->SmoothNormals=2;
	}else if(uR[25].lreturn){
	   scene->SmoothNormals=3;
	}else{
	   scene->SmoothNormals=0;
	}
	
	
	if(uR[17].lreturn){
	   scene->RayTracing=1;
	}else if(uR[30].lreturn){
	   scene->RayTracing=2;
	}else{
	   scene->RayTracing=0;
	}	
	
	if(scene->RayTracing == 0){
	    if(scene->threads > 0){
	         WarningBatch("Warning Threads Set To Zero - Not Ray Tracing\n");
	         scene->threads=0;
	    }
	}

	fixScene(r);
	doFrameImage(myIcon);

	ret = 0;	
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	if(r)RenderScene(r);
	
	return ret;
}
int fixScene(renderPtr r)
{
    struct Scene *scene;
	long length;

	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	if(!r->myIcon)return 1;

	if(scene->CellBuff)cFree((char *)scene->CellBuff);
	scene->CellBuff=NULL;
		
	if(scene->zbuff)cFree((char *)scene->zbuff);
	scene->zbuff=NULL;
		
	length=scene->xResolution*scene->yResolution;
		
	if(!(scene->CellBuff=(long  *)cMalloc(length*sizeof(long),8391))){
	    sprintf(WarningBuff,"fixScene  CellBuff Out of memory Requested : (%ld)\n",length*sizeof(long));
		WarningBatch(WarningBuff);
	    goto OutOfHere;
	}
		
	if((scene->zbuff=(double *)cMalloc(length*sizeof(double),8392)) == NULL){
	    sprintf(WarningBuff,"fixScene  zbuff Out of memory Requested : (%ld)\n",length*sizeof(double));
		WarningBatch(WarningBuff);
	    goto OutOfHere;
	}
	
	if(ScreenSetSize(scene->xResolution,scene->yResolution,r->myIcon))goto OutOfHere;

	return 0;
OutOfHere:
	return 1;
}
int doTestSine(void)
{
	double r,s,ir,jr,xc,yc,range,theta,dr,dt,sintheta,costheta;
	int i,j,n;
	unsigned long length;
	long nn;
	double *xdata,*ydata,*zdata,*vdata;
	long *Elements;
	struct Scene *scene;
	struct Block block;
	IconPtr myIcon2;
	long NXY;
	int ret;
	

	ret=1;
	
	scene=NULL;
	xdata=NULL;
	ydata=NULL;
	zdata=NULL;
	vdata=NULL;
	Elements=NULL;
	
	NXY=400;
			
	scene=(struct Scene *)cMalloc(sizeof(struct Scene),8615);
	if(scene == NULL)goto ErrorOut;

	zerol((char *)scene,sizeof(struct Scene));
	zerol((char *)&block,sizeof(block));
 
	length=NXY*NXY*sizeof(double);
	if(!(xdata=(double *)cMalloc(length,8616))){
		WarningBatch("doTest out of memory\n");
		goto ErrorOut;
	}
	if(!(ydata=(double *)cMalloc(length,8617))){
		WarningBatch("doTest out of memory\n");
		goto ErrorOut;
	}
	if(!(zdata=(double *)cMalloc(length,8618))){
		WarningBatch("doTest out of memory\n");
		goto ErrorOut;
	}
	if(!(vdata=(double *)cMalloc(length,8610))){
		WarningBatch("doTest out of memory\n");
		goto ErrorOut;
	}
	length=NXY*NXY*sizeof(long)*5;
	if(!(Elements=(long *)cMalloc(length,8620))){
		WarningBatch("doTest out of memory\n");
		goto ErrorOut;
	}
	
	
	block.xdata=xdata;
	block.ydata=ydata;
	block.zdata=zdata;
	block.vdata=vdata;
	block.Elements=Elements;
	xc=10.;
	yc=10.;	
	dr=10./((double)NXY-1.0);
	theta=0;
	dt=360./((double)NXY-1.0);
	dt *= (3.1415926/180.);
	nn=0;
	for(j=0;j<NXY;++j){
	    sintheta=sin(theta);
	    costheta=cos(theta);
	    theta += dt;
	    range=0;
	    for(i=0;i<NXY;++i){
	        ir=xc+range*costheta;
	        jr=yc+range*sintheta;
	        range += dr;
	        r=sqrt((xc-ir)*(xc-ir)+(yc-jr)*(yc-jr));
      		r=r*2.;
      		if(r == 0.){
      		    s=1.;
      		}else{
		    if(r >= 1.0)
 			s=sin(r)/(r);
		    else
      		   	s=sin(r)/(r);
      		}
      	
      		zdata[nn]=(double)(10.*s);
      		vdata[nn]=(double)(10.*s);
      		xdata[nn]=(double)ir;
      		ydata[nn]=(double)jr;
      		++nn;
      	
	    }
	}
 
	block.NodeCount=nn;
	
	
		
	n=0;
	for(j=0;j<NXY-1;++j){
	    for(i=0;i<NXY-1;++i){
			Elements[n]=1;
			++n;
			Elements[n]=i+j*NXY;
			++n;
			Elements[n]=i+1+j*NXY;
			++n;
			Elements[n]=i+1+(j+1)*NXY;
			++n;
			Elements[n]=i+(j+1)*NXY;
			++n;
	    }
	}
	
	block.ElementCount=n/5;
	block.NodesMax=4;
	
	
	if(MergeBlock(scene,&block)){
		WarningBatch("MergeBlock Failed\n");
		return 1;
	}
	
	centerScene(scene);

	{
	    unsigned char pal[256*3];
	    getPaletteByName("rainbow",pal);
	    for(n=0;n<3*256;++n){
	        scene->pd.palette[n]=pal[n];
	    }
	}
	scene->pd.sPmin=-2;
	scene->pd.sPmax=10;


	{
	    renderPtr r;
	     myIcon2=openRender(scene);
	    r=(renderPtr)myIcon2->DWindow;
	    if(r)RenderScene(r); 
	}
	ret=0;
	
ErrorOut:

	if(ret){
	
		if(scene)cFree((char *)scene);
		if(xdata)cFree((char *)xdata);
		if(ydata)cFree((char *)ydata);
		if(zdata)cFree((char *)zdata);
		if(vdata)cFree((char *)vdata);
		if(Elements)cFree((char *)Elements);
	}

	return ret;
}
int centerScene(struct Scene *scene)
{
	double xmin,xmax,ymin,ymax,zmin,zmax,vmin,vmax;
	double cx,cy,cz;
	struct Trans1 t;

	if(!scene)return 1;

	rangeE(scene,&xmin,&xmax,&ymin,&ymax,&zmin,&zmax,&vmin,&vmax);
		
	cx = .5*(xmin+xmax);
	cy = .5*(ymin+ymax);
	cz = .5*(zmin+zmax);
		
	zerol((char *)&t,sizeof(struct Trans1));
		
	t.Type=T_BLOCK;
	t.Oa=p(0.,0.,0.);
	t.Xa=p(1.,0.,0.);
	t.Ya=p(0.,1.,0.);

	t.On=p(-cx,-cy,-cz);
	t.Xn=p(1.,0.,0.);
	t.Yn=p(0.,1.,0.);
	t.Groups=scene->BlockCount;
	t.P=(char *)scene->block;
	Transf1(&t);
	scene->CenterCz = -cz;
	return 0;

}
int doUniversalRender(IconPtr myIcon,int flag,FILE8 *inOut)
{

	static struct renderInfo ss;
	static struct Icon mm;
	static struct Scene se;


	static struct universalTableListStruct Local4[] = {
		{"FUNT",3500,universal_TypeFunctionPtr,(void *)RenderReadWrite},			
	};

	
	static struct universalTableStruct Global4 = {
		"Render Window Global parameters",universal_WindowRender,
		(void *)&mm,sizeof(mm),
		NULL,0L,							
		Local4,sizeof(Local4)/sizeof(struct universalTableListStruct),							
	};


	static struct universalTableListStruct Local3[] = {
		{"REAL",4301,universal_TypeLong,(void *)&se.xResolution},
		{"REAL",4302,universal_TypeLong,(void *)&se.yResolution},
		{"REAL",4303,universal_TypeLong,(void *)&se.SmoothNormals},
		{"DOUB",4304,universal_TypeDouble,(void *)&se.Degrees},

		{"DOUB",4305,universal_TypeDouble,(void *)&se.Up.x},
		{"DOUB",4306,universal_TypeDouble,(void *)&se.Up.y},
		{"DOUB",4307,universal_TypeDouble,(void *)&se.Up.z},
		
		{"DOUB",4308,universal_TypeDouble,(void *)&se.Eye.x},
		{"DOUB",4309,universal_TypeDouble,(void *)&se.Eye.y},
		{"DOUB",4310,universal_TypeDouble,(void *)&se.Eye.z},
		
		{"DOUB",4311,universal_TypeDouble,(void *)&se.Target.x},
		{"DOUB",4312,universal_TypeDouble,(void *)&se.Target.y},
		{"DOUB",4313,universal_TypeDouble,(void *)&se.Target.z},
		
		{"DOUB",4314,universal_TypeDouble,(void *)&se.lamp0.origin.x},
		{"DOUB",4315,universal_TypeDouble,(void *)&se.lamp0.origin.y},
		{"DOUB",4316,universal_TypeDouble,(void *)&se.lamp0.origin.z},
		{"REAL",4317,universal_TypeLong,(void *)&se.TransparencyPasses},
		{"REAL",4318,universal_TypeLong,(void *)&se.RayTracing},
		{"REAL",4319,universal_TypeLong,(void *)&se.EyeView},
		{"DOUB",4320,universal_TypeDouble,(void *)&se.Height},
		{"DOUB",4321,universal_TypeDouble,(void *)&se.EyeSeparation},
		{"REAL",4322,universal_TypeLong,(void *)&se.AutoCenterScreen},						
		
		{"DOUB",4323,universal_TypeDouble,(void *)&se.ClipPlanePoint.x},
		{"DOUB",4324,universal_TypeDouble,(void *)&se.ClipPlanePoint.y},
		{"DOUB",4325,universal_TypeDouble,(void *)&se.ClipPlanePoint.z},
		
		
		{"DOUB",4326,universal_TypeDouble,(void *)&se.ClipPlaneNormal.x},
		{"DOUB",4327,universal_TypeDouble,(void *)&se.ClipPlaneNormal.y},
		{"DOUB",4328,universal_TypeDouble,(void *)&se.ClipPlaneNormal.z},
		
		{"DOUB",4329,universal_TypeDouble,(void *)&se.ClipPlaneDistance},
		
		{"REAL",4330,universal_TypeLong,(void *)&se.ClipPlane},
		
		{"REAL",4331,universal_TypeLong,(void *)&se.AntiAlias},
		{"REAL",4332,universal_TypeLong,(void *)&se.MotionBlur},
		{"REAL",4333,universal_TypeLong,(void *)&se.threads},
		{"REAL",4334,universal_TypeLong,(void *)&se.debug},
		
		{"DOUB",4335,universal_TypeDouble,(void *)&se.ClipPlaneAngle},
		
		{"REAL",4336,universal_TypeLong,(void *)&se.FixRayObjects},
		
		{"REAL",4337,universal_TypeLong,(void *)&se.Transparency},

	};
	
	

	static struct universalTableStruct Global3 = {
		"Render Window Global parameters",universal_WindowRender,
		(void *)&se,sizeof(se),
		NULL,0L,							
		Local3,sizeof(Local3)/sizeof(struct universalTableListStruct),							
	};

	static struct universalTableListStruct Local2[] = {
		{"STRI",4202,universal_TypeStringPtr,(void *)&mm.infoSave.directory},
		{"STRI",4203,universal_TypeStringPtr,(void *)&mm.infoLoad.directory},
		{"REAL",4204,universal_TypeInt,(void *)&mm.eraseRectSize},
		{"STRI",4222,universal_TypeString,(void *)&mm.WindowName[0],sizeof(mm.WindowName)},
		{"REAL",4223,universal_TypeInt,(void *)&mm.WindowNameSet},		
		{"DOUB",4224,universal_TypeDouble,(void *)&mm.framesPerSecond},	
	};

	
	static struct universalTableStruct Global2 = {
		"Render Window Global parameters",universal_WindowRender,
		(void *)&mm,sizeof(mm),
		NULL,0L,							
		Local2,sizeof(Local2)/sizeof(struct universalTableListStruct),							
	};

	static struct universalTableListStruct Local[] = {
		{"RECT",4004,universal_TypeRect,(void *)&ss.windowRect},	
		{"REAL",4005,universal_TypeInt,(void *)&ss.Tool},	
		{"REAL",4006,universal_TypeLong,(void *)&ss.CurrentFrame},						
		{"DOUB",4007,universal_TypeDouble,(void *)&ss.UpdatePauseTime},
		{"TABL",4200,universal_TypeTable,(void *)&Global2},	
		{"TABL",4201,universal_TypeTable,(void *)&Global3},			
		{"TABL",3600,universal_TypeTable,(void *)&Global4},					
	};
	

	static struct universalTableStruct Global= {
		"Render Window parameters",universal_WindowRender,
		(void *)&ss,sizeof(ss),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};


	struct Scene *scene;
	renderPtr s;
	int ret;
	
	s=NULL;
	scene=NULL;
	if(myIcon && (myIcon->WindowType == xRenderType)){
		s=(renderPtr)myIcon->DWindow;
		if(!s)return 1;
		scene=s->scene;
		if(!scene)return 1;
		scene->lamp0=scene->lamp;
	}else{
	    myIcon=NULL;
	}
	Global.tableDynamic=s;
	Global.tableDynamicSpace=sizeof(struct renderInfo);
	
	Global2.tableDynamic=myIcon;
	Global2.tableDynamicSpace=sizeof(struct Icon);
	
	Global3.tableDynamic=scene;
	Global3.tableDynamicSpace=sizeof(struct Scene);

	Global4.tableDynamic=myIcon;
	Global4.tableDynamicSpace=sizeof(struct Icon);
	
	ret=1;
	if(flag == universal_SetDefaults){
	
	    if(s && myIcon && scene){
			uGetPortBoundsGlobal(myIcon,&s->windowRect);			
			if(moveUniversalTable(&Global,universal_MoveToStatic))goto ErrorOut;	
		}else{

			if(mm.infoSave.directory)cFree((char *)mm.infoSave.directory);
			mm.infoSave.directory=NULL;
			if(mm.infoLoad.directory)cFree((char *)mm.infoLoad.directory);
			mm.infoLoad.directory=NULL;
			mm.eraseRectSize=140;
			mm.framesPerSecond=30;
						
			uSetRect(&ss.windowRect,117,45,300,300);
			ss.Tool=ControlModeSpinGlobal;
			se.xResolution=300;
			se.yResolution=300;
			se.TransparencyPasses=4;
			se.Transparency=0;
			
			se.FixRayObjects=0;
			
			se.RayTracing=0;
			
			se.Degrees=20;
			
			se.AntiAlias=0;
			
			se.MotionBlur=0;
			
			se.threads=0;
			
			se.debug=0;
			
			se.BackGround=c(1.0,1.,1.);
			se.SmoothNormals=2;
			se.Up=p(0.,0.,1.);
			se.Eye=p(400.,400.,400.);
			se.Target=p(0.,0.,0.);
			se.lamp0.origin=se.Eye;
			se.EyeView=0;
			se.Height=1;
			se.EyeSeparation=0;
			se.AutoCenterScreen=TRUE;
			ss.UpdatePauseTime=0;
		}
		ss.defaultsSet=TRUE;
		
	}else if(flag == universal_SaveData){		
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	}else if(flag == universal_ReadData){		
		if(getUniversalTable(&Global,inOut))goto ErrorOut;		
	    if(s && myIcon && scene){
			RenderGetFrameCount(myIcon);
	   		scene->time=s->CurrentFrame;	        
	    	if(scene->AutoCenterScreen){
	    	   	RenderSceneSetTime(scene);
	    		scene->AutoCenterScreenDone=FALSE;
	    	}
	        RenderScene(s);
	    }
	}else if(flag == universal_GetDefaults){		
	    if(s && myIcon && scene){
			if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
			uMoveWindow(myIcon,s->windowRect.x,s->windowRect.y);
			uSizeWindowGlobal(myIcon,s->windowRect.xsize,s->windowRect.ysize);
			if(myIcon->WindowNameSet && uPref.config){
			    myIcon->WindowNameSet=FALSE;
				SetWindowName(myIcon,myIcon->WindowName);	
				myIcon->WindowNameSet=TRUE;
			}else{
			    GetWindowName(myIcon,myIcon->WindowName,0);
				myIcon->WindowNameSet=FALSE;
			}
			if(myIcon->pMoveControls){
			    (myIcon->pMoveControls)(myIcon);
			}
			scene->lamp=scene->lamp0;
		    fixScene(s);
	   		scene->time=s->CurrentFrame;	
	   		if(myIcon->Files){    
				RenderGetFrameCount(myIcon);
	        	RenderScene(s);
	        }
	    }
	}else if(flag == universal_Stop){	
		if(freeUniversalTable(&Global))goto ErrorOut;				
	}else{
		sprintf(WarningBuff,"doUniversalRender unknown flag %d\n",flag );
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
	ret=0;
ErrorOut:
	return ret;
}
static int RenderReadWrite(IconPtr myIcon,FILE8 *out,int flag)
{
	struct Scene *scene;
	renderPtr r;
	int ret;
	
	if(!myIcon || !FindWindowInList(myIcon) || !out)return 1;
	
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	
	ret=ListReadWriteObjectList(scene,scene->ObjectList,out,flag);
	
	return ret;
}
static int DoRayTracingLights(IconPtr myIcon)
{
	static uDialogGroup Items[] =

        {
      /* 00 */      {"materials",{20,20,110,135},uDialogScrollList,uDialogNone},
      
      /* 01 */      {"0", {217,10,110,20},uDialogText,uDialogGetDouble},
      /* 02 */      {"0", {337,10,110,20},uDialogText,uDialogGetDouble},
      /* 03 */      {"0", {457,10,110,20},uDialogText,uDialogGetDouble},
      
      /* 04 */      {"0", {217,40,110,20},uDialogText,uDialogGetDouble},
      /* 05 */      {"0", {337,40,110,20},uDialogText,uDialogGetDouble},
      /* 06 */      {"0", {457,40,110,20},uDialogText,uDialogGetDouble},
      
      /* 07 */      {"0", {217,70,110,20},uDialogText,uDialogGetDouble},
      
      
      /* 08 */      {"Ok", {15,232,80,20},uDialogButton,uDialogNone},
      /* 09 */      {"Cancel", {105,232,80,20},uDialogButton,uDialogNone},
      /* 10 */      {"Render", {195,232,80,20},uDialogButton,uDialogNone},
      /* 11 */      {"New", {285,232,80,20},uDialogButton,uDialogNone},
      /* 12 */      {"Delete", {375,232,80,20},uDialogButton,uDialogNone},
      
      /* 13 */      {"location", {135,10,65,20},uDialogString,uDialogNone},
      /* 14 */      {"color", {135,40,65,20},uDialogString,uDialogNone},
      /* 15 */      {"brightness", {135,70,75,20},uDialogString,uDialogNone},
	  /* 16 */      {"0", {217,100,110,20},uDialogText,uDialogGetLong},
	  /* 17 */      {"type", {135,100,75,20},uDialogString,uDialogNone},
		
			
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,593,270};
 	
    struct t_light *Lights;
    long nLights;
        
	struct Scene *scene;
	renderPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
    int reti;
    int ret;
    int n;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	scene->itIs = -1;

	if(scene->Lights){
	    scene->Lights[0].light_pos[0]=scene->lamp.origin.x;
	    scene->Lights[0].light_pos[1]=scene->lamp.origin.y;
	    scene->Lights[0].light_pos[2]=scene->lamp.origin.z;
	}
	
	nLights=scene->nLights;
	Lights=NULL;
	if(nLights){
	    Lights=(struct t_light *)cMalloc(scene->nLights*sizeof(struct t_light),8607);
	}
	if(Lights){
	    for(n=0;n<nLights;++n){
			Lights[n]=scene->Lights[n];
	    }
	}
	
	
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Ray Tracing Lights",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	reti=uDialogSet(uList,0L,
	              
	              uDialogSetMyIcon,myIcon,	              
	              uDialogSetAction,doLightsAction,	              
	              uDialogSetStart,doLightsStart,
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
    
		       
	ret=0;
	

ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
    
    if(ret){    
	    if(Lights){
        	if(scene->Lights)cFree((char *)scene->Lights);
		    scene->Lights=Lights;
		    scene->nLights=(int)nLights;
		    Lights=NULL;
	    }    
    }else{
    
		if(scene->Lights){
		    scene->lamp.origin.x=scene->Lights[0].light_pos[0];
		    scene->lamp.origin.y=scene->Lights[0].light_pos[1];
		    scene->lamp.origin.z=scene->Lights[0].light_pos[2];
			scene->Lights[0].type=0;
		}
    
    }
    
	if(Lights)cFree((char *)Lights);
	
	if(r)RenderScene(r);
	
	return ret;
}
static int doLightsAction(struct uDialogStruct *uList)
{
	struct Scene *scene;
	IconPtr myIcon;	
	renderPtr r;
	uDialogReturn *uR;
	struct t_light *Lights;
	long itis;
	int n,nn;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene || !scene->Lights)return 1;
	
	ret = 1;
	uR=NULL;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 0){
	   ClickLightList0(uList);
	}else if(uList->actionItem == 8){
		SaveLightData(uList,scene->itIs);
	}else if(uList->actionItem == 10){
		SaveLightData(uList,scene->itIs);
		if(r)RenderScene(r);	    
	}else if(uList->actionItem == 11){
		SaveLightData(uList,scene->itIs);
	    Lights=(struct t_light *)cMalloc((scene->nLights+1)*sizeof(struct t_light),8607);
	    if(!Lights)goto ErrorOut;
	    zerol((char *)Lights,(scene->nLights+1)*sizeof(struct t_light));
	    for(n=0;n<scene->nLights;++n){
			if(scene->Lights)Lights[n]=scene->Lights[n];
	    }
	    Lights[scene->nLights].light_bright = 1.;
	    Lights[scene->nLights].red = 1.;
	    Lights[scene->nLights].green = 1.;
	    Lights[scene->nLights].blue = 1.;
	    Lights[scene->nLights].type =0;
	    
	    if(scene->Lights)cFree((char *)scene->Lights);
	    scene->Lights=Lights;
	    scene->itIs=scene->nLights;
	    scene->nLights += 1;
		doLightsStart(uList);
		SetLightData(uList,scene->itIs);
	}else if(uList->actionItem == 12){
	    itis=scene->itIs;
		SaveLightData(uList,scene->itIs);
		scene->itIs=0;
		if(itis == 0 && scene->nLights == 1){
		   Warning("Must Have At Least One Light");
		   goto ErrorOut;
		}
		nn=0;
		for(n=0;n<scene->nLights;++n){
			if(itis == n)continue;
			scene->Lights[nn++]=scene->Lights[n];
		}
		scene->nLights=nn;
		doLightsStart(uList);
		SetLightData(uList,0);
	}

	ret = 0;
	
ErrorOut:	
	if(uList)uDialogUpdate(uList);
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}
static int doLightsStart(struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	IconPtr myIcon;
	struct Scene *scene;
	renderPtr r;
	int ret;
	long count;
	long n;
	
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
		
	ret=uDialogSet(uList,0L,
              uDialogGetScrollList,&scroll,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
		
	count=ScrollListCount(scroll);
	if(count){
		ScrollListDelete(0L,count,scroll);	
	}	
	
	for(n=0;n<scene->nLights;++n){
	    char buff[256];
		char *data;
		char *names[]={"Light",};
		data = &buff[0];
	    sprintf(buff,"%s<%ld>",names[0],n);
		if(ScrollListInsertOneType(&data,3,n,
	    	1L,scroll))return 1;
	}
	ScrollListTop(0L,scroll);

	SetLightData(uList,0L);

	return 0;
}
static int ClickLightList0(struct uDialogStruct *uList)
{
	struct ScrollList *List0;
	struct Scene *scene;
	renderPtr r;
	IconPtr myIcon;
    int ret;
    long n;

	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;


	ret=uDialogSet(uList,0L,
              uDialogGetScrollList,&List0,
                            	              	              
              uDialogSetDone,uDialogSetDone
    );		
	if(ret)goto ErrorOut;
			
	n=ScrollListFirstSelect(List0);
	if(n >= 0){
	    if((scene->itIs) >= 0 && (scene->itIs != n)){
	         SaveLightData(uList,scene->itIs);
	    }
		scene->itIs=n;
	    SetLightData(uList,scene->itIs);		
	}
	
ErrorOut:
			
	if(uList){
		uDialogUpdate(uList);
		if(uList->myIcon)InvalRectMyWindow(uList->myIcon);
	}
	return 0;
}
static int SaveLightData(struct uDialogStruct *uList,long n)
{
	struct Scene *scene;
	uDialogReturn *uR;
	renderPtr r;
	IconPtr myIcon;
    int ret;
    
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	if((n < 0) || (n >= scene->nLights))return 1;
	if(!scene->Lights)return 1;
	
	ret = 1;
	uR=NULL;
	
    uR=uDialogGetReturn(uList);
    if(!uR)goto ErrorOut;
    
    scene->Lights[n].light_pos[0]=uR[1].dreturn;
    scene->Lights[n].light_pos[1]=uR[2].dreturn;
    scene->Lights[n].light_pos[2]=uR[3].dreturn;
    
    scene->Lights[n].red=uR[4].dreturn;
    scene->Lights[n].green=uR[5].dreturn;
    scene->Lights[n].blue=uR[6].dreturn;
    
    scene->Lights[n].light_bright=uR[7].dreturn;
    scene->Lights[n].type=uR[16].lreturn;
        
                
 	ret = 0;           
ErrorOut:	

    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int SetLightData(struct uDialogStruct *uList,long n)
{
	struct ScrollList *List0;
	struct Scene *scene;
	renderPtr r;
	IconPtr myIcon;
    int reti;
    int ret;
    
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;

	
	if((n < 0) || (n >= scene->nLights))return 1;
	if(!scene->Lights)return 1;
	
	reti=uDialogSet(uList,0L,
              uDialogGetScrollList,&List0,
              
              uDialogSetDone,uDialogSetDone
    );		
	if(reti) return 1;
	
	ret = 1;
	
	reti=uDialogSet(uList,0L,
			
			uDialogSetItem,1L,
			uDialogSetDouble,(double)scene->Lights[n].light_pos[0],
							
			uDialogSetItem,2L,
			uDialogSetDouble,(double)scene->Lights[n].light_pos[1],
							
			uDialogSetItem,3L,
			uDialogSetDouble,(double)scene->Lights[n].light_pos[2],
							
							
			uDialogSetItem,4L,
			uDialogSetDouble,(double)scene->Lights[n].red,
							
			uDialogSetItem,5L,
			uDialogSetDouble,(double)scene->Lights[n].green,
							
			uDialogSetItem,6L,
			uDialogSetDouble,(double)scene->Lights[n].blue,
			            
			uDialogSetItem,7L,
			uDialogSetDouble,(double)scene->Lights[n].light_bright,
					
			uDialogSetItem,16L,
			uDialogSetLong,scene->Lights[n].type,
					
						
			uDialogSetDone,uDialogSetDone
    );		
	if(reti) goto ErrorOut;
	
	ScrollListDeSelect(List0);
	ScrollListSelect(n,List0);
	
	ret = 0;
ErrorOut:
	return ret;
}
