#define EXTERN22 extern
#include "Xappl.h"
#include "uLib.h"
#include "uDialog.h"
#include "ScrollList.h"
#include "FileManager.h"
#include "Message.h"
#include "Linedata.h"

static struct FileManagerStruct FileManagerList;

static int doFileManagerBatch(FileManagerPtr List,CommandPtr cp,BatchPtr Batch);

static int doSetAreaRange(FilePtr Files,struct areadata *ai);

int FileManagerSaveName(char *name);

int MakeNewDirectory(char *name);

struct FileInfo2 *FilesSDS3D(void);

int FileManagerAddToList(FilePtr Files,char *fileListName)
{
	if(!Files || !fileListName)return 1;
	
	Files->doNotAutoRemove=TRUE;
	
	return FileManagerAdd(fileListName,Files,&FileManagerList);
}

int FileManagerSendMessage(FilePtr Files,long MessageType,void *MessageData)
{
	CommandPtr Commands;
	struct SetFrameData  *sd;
	struct Message1000 *m;
	struct linedata *li;
	struct areadata *ai;
	struct LIMITS *limits;
	struct LIMITS *limitl;
	int *type;
	int ret;
	
	if(!Files)return 1;
	
	ret=1;
	m=(struct Message1000 *)MessageData;
	
	switch(MessageType){
	case MessageType_GET_REGION_DATA:
	    sd=(struct SetFrameData  *)MessageData;
	    if(!sd)goto ErrorOut;
	    if(Files->pGetData2){
	    	ret = (*Files->pGetData2)(Files,sd->CurrentFrame,sd);
	    }
	    goto ErrorOut;
	case MessageType_GET_SCREEN_DATA:
	    sd=(struct SetFrameData  *)MessageData;
	    if(!sd)goto ErrorOut;
	    if(Files->pGetData3){
	    	ret = (*Files->pGetData3)(Files,sd->CurrentFrame,sd);
	    }
	    goto ErrorOut;
	case MessageType_SET_REGION_DATA:
	    sd=(struct SetFrameData  *)MessageData;
	    if(Files->pPutData2){
	    	ret = (*Files->pPutData2)(Files,sd->CurrentFrame,sd);
	    }
	    goto ErrorOut;	    	    
	case MessageType_GET_CELL_DATA:
	    sd=(struct SetFrameData  *)MessageData;
	    if(Files->pGetCellData){
	    	ret = (*Files->pGetCellData)(Files,sd->CurrentFrame,sd);
	    }
	    goto ErrorOut;	    	    
	case MessageType_SET_CELL_DATA:
	    sd=(struct SetFrameData  *)MessageData;
	    if(Files->pSetCellData){
	    	ret = (*Files->pSetCellData)(Files,sd->CurrentFrame,sd);
	    }
	    goto ErrorOut;	    	    
	case MessageType_GET_INFORMATION:
	    sd=(struct SetFrameData  *)MessageData;
	    if(Files->pInformation){
	    	ret = (*Files->pInformation)(Files,sd);
	    }
	    goto ErrorOut;	    	    
	case MessageType_DO_DIALOG:
	    type=(int *)MessageData;
	    if(Files->pDoDialog){
	    	ret = (*Files->pDoDialog)(Files,type);
	    }
	    goto ErrorOut;	    
	case MessageType_SET_AREA_RANGE:
	    ai=(struct areadata *)MessageData;
	    ret = doSetAreaRange(Files,ai);
	    goto ErrorOut;
	case MessageType_GET_AREA_DATA:
	    ai=(struct areadata *)MessageData;
	    if(!ai)goto ErrorOut;
	    if(Files->pGetAreaData){
	    	ret = (*Files->pGetAreaData)(Files,ai);
	    }
	    goto ErrorOut;
	case MessageType_GET_LINE_DATA:
	    li=(struct linedata *)MessageData;
	    if(!li)goto ErrorOut;
	    if(Files->pGetLineData){
	    	ret = (*Files->pGetLineData)(Files,li);
	    }
	    goto ErrorOut;
	case MessageType_SEND_COMMAND_DATA:
	    Commands=(CommandPtr)MessageData;
	    if(!Commands)goto ErrorOut;
	    if(Files->pCommandSend){
	    	ret = (*Files->pCommandSend)(Files,Commands);
	    }
	    goto ErrorOut;
	case MessageType_GET_LIMITS_DATA:
	    limitl=(struct LIMITS *)MessageData;
	    if(!limitl)goto ErrorOut;
	    limits=(struct LIMITS *)NULL;
		FileGetLimits(&limits,limitl->CurrentFrame,Files);
		if(limits && limits->iGot){
		    limits->CurrentFrame=limitl->CurrentFrame;
		    *limitl = *limits;
		}
		break;
	case MessageType_GET_FRAME_COUNT:
		if(Files->ImageCount > m->FrameCount)m->FrameCount=Files->ImageCount;
		goto OK;
	case MessageType_PLOT_LINE_DATA:
	    if(Files->pPlotLineData){
	    	ret = (*Files->pPlotLineData)(Files,m);
	    }
		break;
	case MessageType_PRINT_FILE_INFORMATION:
	    printFileInformation(Files);
		goto OK;
	}
OK:
	ret = 0;
ErrorOut:
	return ret;
}
static int doSetAreaRange(FilePtr Files,struct areadata *ai)
{
	struct LIMITS limits;
	int ret;
	
	if(!ai)return 1;
	if(!Files)return 1;

	if(ai->type != FRAME_DATA_FLOAT){
	    sprintf(WarningBuff,"doSetAreaRange - Only Returns FRAME_DATA_FLOAT");
	    Warning(WarningBuff);
	    return 1;
	}
	
	zerol((char *)&limits,sizeof(struct LIMITS));
	
	limits.xmin=ai->xminArea;
	limits.ymin=ai->yminArea;
	limits.zmin=ai->zminArea;
	limits.vmin=ai->vminArea;
	
	limits.xmax=ai->xmaxArea;
	limits.ymax=ai->ymaxArea;
	limits.zmax=ai->zmaxArea;
	limits.vmax=ai->vmaxArea;
	limits.time=ai->pioTime;
	limits.iGotTime=TRUE;
	
	limits.pioName=(unsigned char *)strsave(ai->pioName,9845);
	
	
	ret=FilePutLimits(&limits,ai->CurrentFrame,Files);
	
	if(ret){
	    if(limits.pioName)cFree((char *)limits.pioName);
	}
	
	if(ai->pioName[0]){
		mstrncpy(Files->pioName,ai->pioName,255);
		Files->pioIndex=ai->pioIndex;			    
	}					

	
	return ret;

}

int FileManagerBatch(BatchPtr Batch,CommandPtr cp)
{
	int reti;
	int ret;
	
   if(!Batch || !cp)return 1;
   
   ret = 1;
	
   reti=doFileManagerBatch(&FileManagerList,cp,Batch);
   if(reti == 2){
       goto OK;
   }else if(reti){
       goto ErrorOut;
   }


OK:
    ret=0;
ErrorOut:	
   return ret;
}
static int doFileManagerBatch(FileManagerPtr List,CommandPtr cp,BatchPtr Batch)
{
	struct FilePIOInfo *pio;
	struct CommandInfo cpl;
	struct FileList Files;
    char fileListName[256];
    char working[2028];
    char working2[2028];
	FilePtr Files2;
    long increment;
    long count;
	char line[4096];
	char *command;
 	double value;
	int ret;
	
	if(!List || !cp || !Batch)return 1;
	
	ret = 1;
	zerol((char *)&cpl,sizeof(struct CommandInfo));
	
    zerol((char *)&Files,sizeof(struct FileList));
    Files.donotfree=TRUE;    
    
	++(cp->n);
	command=stringCommand(cp);
	if(!command)goto ErrorOut;
	if(!mstrcmp("open",command)){
		long FileMax;		
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
        mstrncpy(fileListName,command,255);
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command){
			Files.directory=DefaultPathString();
		}else{
            Files.directory=strsave(command,7469);
		}
        if(!Files.directory)goto ErrorOut;
        
        getcwd(working,sizeof(working));
        if(goCD(Files.directory)){
	      sprintf(WarningBuff,"Directory %s Could Not Be Found\n",Files.directory);
	      WarningBatch(WarningBuff);
	      goto ErrorOut;
        }
        getcwd(working2,sizeof(working));
        if(Files.directory)cFree((char *)Files.directory);
        Files.directory=strsave(working2,7469);
        if(!Files.directory)goto ErrorOut;
        if(goCD(working)){
	      sprintf(WarningBuff,"Directory %s Could Not Be Found\n",working);
	      WarningBatch(WarningBuff);
	      goto ErrorOut;
        }
        
        
	    ++(cp->n);
	    if(doubleCommand(&value,cp)){
	        increment=1;
	    }else{
	        increment=(long)value;
	    }
	    
	    Files.count=0;
		FileMax = 2;
		
		count=0;			
	    Files.name=(char **)cMalloc(FileMax*sizeof(char *),8750);
	    if(!Files.name)goto ErrorOut;
		while(1){
	    	if(BatchNextLine(Batch,line,sizeof(line)))goto ErrorOut;
	    	cpl.getKind = BATCH_STRING;
			if(getCommand(line,&cpl))goto ErrorOut;	
			for(cpl.n=0;cpl.n<cpl.nword;++cpl.n){
				command=stringCommand(&cpl);
				if(!command)goto ErrorOut;
				if(!mstrcmp("end",command))goto EndList;
				if(++count < increment)continue;
				count = 0;
			    if(Files.count >= FileMax){		    
		    		Files.name=(char **)cRealloc((char *)Files.name,(FileMax+2000)*sizeof(char *),8750);
		    		if(!Files.name)goto ErrorOut;		
		    		zerol((char *)&Files.name[FileMax],2000*sizeof(char *));
		    		FileMax += 2000;    
			    }
		        Files.name[Files.count]=strsave(command,7776);
		        if(!Files.name[Files.count])goto ErrorOut;
		        Files.count++;
	        }
		    
		}
EndList:		
		Files2=FilesOpenList(&Files);
		if(!Files2)goto ErrorOut;
AddList:
		Files2->doNotAutoRemove=TRUE;
		Files2->openedByFileManager=TRUE;		
		if(FileManagerAdd(fileListName,Files2,List)){
			freeFileList(Files2);
			goto ErrorOut;
		}
		ret = 2;
		goto OK;
	
	}else if(!mstrcmp("create",command)){
	
        Files2=FilesSDS2D();
        if(!Files2)goto ErrorOut;
                
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
        mstrncpy(fileListName,command,255);
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command){
			Files.directory=DefaultPathString();
		}else{
            Files.directory=strsave(command,7469);
		}
        if(!Files.directory)goto ErrorOut;
        
        if(goCD(Files.directory)){
            if(MakeNewDirectory(Files.directory)){
                  sprintf(WarningBuff,"Directory %s Could Not Be Found Or Created\n",Files.directory);
                  WarningBatch(WarningBuff);
                  goto ErrorOut;
            }
        }
        
        mstrncpy(Files2->outputDirectory,Files.directory,1024);
        
        if(Files.directory)cFree((char *)Files.directory);
        Files.directory=NULL;
        goto AddList;                
	}else if(!mstrcmp("create3d",command)){
	
        Files2=FilesSDS3D();
        if(!Files2)goto ErrorOut;
  		pio=&Files2->pioData;
		pio->GetDataFlag = 1;
              
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
        mstrncpy(fileListName,command,255);
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command){
			Files.directory=DefaultPathString();
		}else{
            Files.directory=strsave(command,7469);
		}
        if(!Files.directory)goto ErrorOut;
        
        if(goCD(Files.directory)){
            if(MakeNewDirectory(Files.directory)){
                  sprintf(WarningBuff,"Directory %s Could Not Be Found Or Created\n",Files.directory);
                  WarningBatch(WarningBuff);
                  goto ErrorOut;
            }
        }
        
        mstrncpy(Files2->outputDirectory,Files.directory,1024);
        
        if(Files.directory)cFree((char *)Files.directory);
        Files.directory=NULL;
        goto AddList;                
	}else if(!mstrcmp("close",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
        mstrncpy(fileListName,command,255);
	    ++(cp->n);
		command=stringCommand(cp);
		if(command && !mstrcmp("save",command)){
			if(FileManagerSaveName(fileListName))goto ErrorOut;
		}        
		if(FileManagerCloseName(fileListName))goto ErrorOut;
		ret = 2;
		goto OK;
	}else if(!mstrcmp("set",command)){
	    FilePtr File;
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		File=FileManagerGet(command);
		if(!File)goto ErrorOut;
  		pio=&File->pioData;
		while(1){
	    	if(BatchNextLine(Batch,line,sizeof(line)))goto ErrorOut;
			if(getCommand(line,&cpl))goto ErrorOut;	
			command=stringCommand(&cpl);
			if(!command)goto ErrorOut;
			if(!mstrcmp("end",command)){
				ret = 2;
				goto OK;
			}else if(!mstrcmp("outputName",command)){
			    ++(cpl.n);
				command=stringCommand(&cpl);
				if(!command)goto ErrorOut;
		        mstrncpy(File->outputName,command,255);
			}else if(!mstrcmp("outputSequenceName",command)){
			    ++(cpl.n);
				command=stringCommand(&cpl);
				if(!command)goto ErrorOut;
		        mstrncpy(File->outputSequenceName,command,255);
			}else if(!mstrcmp("outputSequenceNumber",command)){
			    ++(cpl.n);
			    if(doubleCommand(&value,&cpl))goto ErrorOut;
			    File->outputSequenceNumber=(long)value;
			}else if(!mstrcmp("outputDirectory",command)){
			    ++(cpl.n);
				command=stringCommand(&cpl);
				if(!command)goto ErrorOut;
		        if(goCD(command)){
		            if(MakeNewDirectory(command)){
		                  sprintf(WarningBuff,"Directory %s Could Not Be Found Or Created\n",command);
		                  WarningBatch(WarningBuff);
		                  goto ErrorOut;
		            }
		        }
        		mstrncpy(File->outputDirectory,command,1024);
			}else if(!mstrcmp("outputSize",command)){
			    ++(cpl.n);
			    if(doubleCommand(&value,&cpl))goto ErrorOut;
			    File->xSize=(long)value;
			    ++(cpl.n);
			    if(!doubleCommand(&value,&cpl)){			      
			        File->ySize=(long)value;
			    }else{
			        File->ySize=0;
			    }
			    ++(cpl.n);
			    if(!doubleCommand(&value,&cpl)){
			        File->zSize=(long)value;
			    }else{
			        File->zSize=0;
			    }
			}else if(!mstrcmp("outputType",command)){
			    ++(cpl.n);
				command=stringCommand(&cpl);
				if(!command)goto ErrorOut;
				File->outputType = 0;
				if(!mstrcmp(command,"hdf")){
					File->outputType = HDF_FILES;
				}else if(!mstrcmp(command,"true")){
					File->outputType = TRUE_FILES;
				}else if(!mstrcmp(command,"sds2d")){
					File->outputType = SDS2D_FILES;
				}else if(!mstrcmp(command,"sds3d")){
					File->outputType = SDS3D_FILES;
					pio->GetDataFlag = 1;
				}else if(!mstrcmp(command,"jpg")){
					File->outputType = JPG_FILES;
				}
			}
		}
		
	}
	
	
	ret = 0;
OK:
ErrorOut:
 	getCommand(NULL,&cpl);
    FileListFree(&Files);
	return ret;
}
int FileManagerAdd(char *name,FilePtr Files,FileManagerPtr List)
{

	long FileMax;
	int ret;
	
	if(!name || !name[0] || !List || !Files)return 1;
	
	ret=1;
	
	if(FileManagerGet(name)){
	    sprintf(WarningBuff,"FileManagerAdd %s already open\n",name);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	FileMax=List->FileMax;
	if(List->FileCount >= FileMax){
	    if(List->Files){
			List->Files=(FilePtr *)cRealloc((char *)List->Files,(FileMax+2000)*sizeof(FilePtr),8751);
		}else{
			List->Files=(FilePtr *)cMalloc((FileMax+2000)*sizeof(FilePtr),8752);
		}
		if(!List->Files)goto ErrorOut;		
		zerol((char *)&List->Files[FileMax],2000*sizeof(FilePtr));
		List->FileMax += 2000;    
	}
	
	List->Files[List->FileCount++]=Files;
	
    mstrncpy(Files->fileListName,name,255);
	
	ret=0;
ErrorOut:	
	return ret;
}
FilePtr FileManagerGet(char *name)
{
	FileManagerPtr List;
	FilePtr Files;
	int n;
	
	if(!name)return NULL;
	
	List=&FileManagerList;
	for(n=0;n<List->FileCount;++n){
	    Files = List->Files[n];
	    if(Files){
	        if(!strcmp(name,Files->fileListName)){
	            return Files;
	        }
	    }
	}
	return NULL;
}
int FileManagerSaveName(char *name)
{
	FilePtr Files;
	
	if(!name)return 1;
	
	Files=FileManagerGet(name);
	
	if(!Files)return 1;
	
	return FilesSaveFiles(Files);
	
}

int FileManagerNotOwned()
{
	FileManagerPtr List;
	FilePtr Files;
	int ifound;
	long n,nn;
	
	
	List=&FileManagerList;
	
	ifound=FALSE;
	
	nn=0;
	for(n=0;n<List->FileCount;++n){
	    Files = List->Files[n];
	    if(FileOpenIsOpen(Files)){
	        List->Files[nn++]=Files;
	    }else{
			ifound=TRUE;
	    }
	}
	if(ifound){
	    List->FileCount = nn;
	    return 0;
	}else{
	    return 1;
	}

}

int FileManagerCloseName(char *name)
{
	FileManagerPtr List;
	FilePtr Files;
	int ifound;
	long n,nn;
	
	if(!name || !name[0])return 1;
	
	List=&FileManagerList;
	
	ifound=FALSE;
	
	nn=0;
	for(n=0;n<List->FileCount;++n){
	    Files = List->Files[n];
	    if(Files && Files->fileListName[0] && !strcmp(name,Files->fileListName)){
			ifound=TRUE;
			freeFileList(Files);
	    }else{
	        List->Files[nn++]=List->Files[n];
	    }
	}
	if(ifound){
	    List->FileCount = nn;
	    return 0;
	}else{
	    return 1;
	}
}
int FileManagerClosePtr(FilePtr Files)
{
	FileManagerPtr List;
	int ifound;
	long n,nn;
	
	if(!Files)return 1;
	
	List=&FileManagerList;
	
	ifound=FALSE;
	
	nn=0;
	for(n=0;n<List->FileCount;++n){
	    if(Files == List->Files[n]){
			ifound=TRUE;
			freeFileList(Files);
			List->Files[n]=NULL;
	    }else{
	        List->Files[nn++]=List->Files[n];
	    }
	}
	if(ifound){
	    List->FileCount = nn;
	    return 0;
	}else{
	    return 1;
	}
}
int FileManagerExit(int flag)
{
	FileManagerPtr List;
	FilePtr Files;
	long n,nn;
	
	List=&FileManagerList;
	
	if(flag){
		for(n=0;n<List->FileCount;++n){
		    Files = List->Files[n];
		    if(Files){
		        if(Files->fileListName[0]){
			    	sprintf(WarningBuff,"FileManagerExit - List %s Not Closed\n",Files->fileListName);
			    }else{
			    	sprintf(WarningBuff,"FileManagerExit - List 'Bad Name' Not Closed\n");
			    }
			    Warning(WarningBuff);
				freeFileList(Files);
		    }
		}
		
		if(List->Files)cFree((char *)List->Files);
		List->Files=NULL;
		List->FileCount=0;
		List->FileMax=0;
	}else{
		nn=0;
		for(n=0;n<List->FileCount;++n){
		    Files = List->Files[n];
		    if(!Files)continue;
		    if(Files->openedByFileManager){
				freeFileList(Files);
				List->Files[n]=NULL;
		    }else{
	        	List->Files[nn++]=Files;
		    }
		}
	    List->FileCount = nn;	
	}	
	return 0;
	
}
