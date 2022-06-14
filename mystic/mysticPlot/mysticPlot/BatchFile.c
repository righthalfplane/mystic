#define EXTERN22 extern
#include <errno.h>
#include "Xappl.h"
#include "uLib.h"
#include "uDialog.h"
#include "ScrollList.h"
#include "uFiles.h"

#include "BatchFile.h"
#include "FileManager.h"
#include "Universal.h"

int BatchFile(void);

char *DefaultPathString(void);

int doBatch(BatchPtr Batch,CommandPtr cp);



int BatchOpenFileList(BatchPtr Batch);

int BatchFile(void)
{
	struct FileList *Files;
	int TypeList[]={HDF_DISK,HDF_MEMORY,
					SDS2D_DISK,SDS2D_MEMORY,
					TRUE_DISK,TRUE_MEMORY,
					RGB_DISK,RGB_MEMORY,
					BMP_DISK,BMP_MEMORY,
					JPG_DISK,JPG_MEMORY,
					GIF_DISK,GIF_MEMORY,
					TEK_DISK,TEK_MEMORY,
					QUICKTIME_DISK,QUICKTIME_MEMORY,
					SAGE1D_DISK,SAGE2D_DISK,
					TEXT_MEMORY,
					0};
	char name[2048];
	int ret;
	int n;
		
	ret=1;
	
	Files=uOpenFileDialog(NULL,TypeList);		
	if(!Files)return 1;
	
	for(n=0;n<Files->count;++n){
	    mstrncpy(name,Files->directory,2048);
		strncatToPath(name,Files->name[n],2048);
		sprintf(WarningBuff,"Process BatchFile: %s \n",name);
		WarningBatch(WarningBuff);
		if(processFile(name))goto ErrorOut;	    
	}	
	
	ret=0;
ErrorOut:
	if(Files)FileListFree(Files);
	return ret;
}
int BatchNextLine(BatchPtr Batch,char *line,long len)
{
	if(!Batch || !line || (len <= 0))return 1;
	if(!Batch->input){
	    if(Batch->getLine){
	    	return (*Batch->getLine)(Batch->myIcon2,line,len);
	    }else{
	        return 1;
	    }
	}
	
	return NextLine(Batch->input,line,(int)len);
}
int processFile(char *pathname)
{
	struct BatchInfo Batch;
	struct Icon myIcon;
	char line[4096];
	double start,end;
	FILE *input;

	if(!pathname)return 1;
	
	zerol((char *)&myIcon,sizeof(struct Icon));
	
	zerol((char *)&Batch,sizeof(struct BatchInfo));

	Batch.myIcon=&myIcon;
	
	input=NULL;
	
	start=rtime();

	input=fopen(pathname,"r");
	if(input == NULL){
	    sprintf(WarningBuff,"Could Not Open %s To Read Errno %d\n",pathname,errno);
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
	Batch.input=input;

	while(1){
	    if(BatchNextLine(&Batch,line,sizeof(line)))break;
	    if(ProcessLine(line,&Batch))break;
	}

	end=rtime();
	
	sprintf(WarningBuff,"Total Time in processFile %.2f Seconds\n",end-start);
	WarningBatch(WarningBuff);	
		
	if(input)fclose(input);
	
	return 0;
}
int ProcessLine(char *line,BatchPtr Batch)
{
	struct CommandInfo cp;
	int ret;

	if(!line || !Batch)return 1;
	
	ret=1;

	zerol((char *)&cp,sizeof(struct CommandInfo));

	if(getCommand(line,&cp))goto ErrorOut;
	
	for(cp.n=0;cp.n<cp.nword;++cp.n){
	    if(doBatch(Batch,&cp))goto ErrorOut;
	}
	
	ret=0;
	
ErrorOut:	

	getCommand(NULL,&cp);
	
	return ret;
}
int doBatch(BatchPtr Batch,CommandPtr cp)
{
	struct CommandInfo cpl;
	char *command;
 	double value;
	int ret;
	
	if(!Batch || !cp)return 1;
	
	zerol((char *)&cpl,sizeof(struct CommandInfo));
	
	ret = 1;
	command=stringCommand(cp);
	if(!command)goto ErrorOut;
	
	if(!mstrcmp("sage3d",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
    	doSage3DBatch(Batch,command);
    }else if(!mstrcmp("sage2d",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
    	doSage2DBatch(Batch,command);
    }else if(!mstrcmp("sage1d",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
    	doSage1DBatch(Batch,command);
    }else if(!mstrcmp("sagevol",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
    	doSageVolBatch(Batch,command);
    }else if(!mstrcmp("sds3d",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
    	doSDS3DBatch(Batch,command);
    }else if(!mstrcmp("sds2d",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
    	doSDS2DBatch(Batch,command);
    }else if(!mstrcmp("true24",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
    	do24BitBatch(Batch,command);
    }else if(!mstrcmp("expression",command)){
        doBatchExpression(Batch);
    }else if(!mstrcmp("draw",command)){
        doBatchDraw(Batch);
	}else if(!mstrcmp("cd",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    goCD(command);
	}else if(!mstrcmp("directoryInput",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	}else if(!mstrcmp("zonesperblock",command)){
	    ++(cp->n);
	     if(doubleCommand(&value,cp))goto ErrorOut;
		uPref.BlockSizeGlobal=(long)value;
		if(uPref.BlockSizeGlobal < 0)uPref.BlockSizeGlobal=0;
	}else if(!mstrcmp("filelist",command)){
	    if(FileManagerBatch(Batch,cp))goto ErrorOut;	
	}else if(!mstrcmp("exit",command)){
	    goto ErrorOut;
	}else if(!mstrcmp("clear",command)){
        FileManagerExit(0);
    }else if(!mstrcmp("sagewrite",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
    	/* doSageRewrite(Batch,command); */
	}else{
	    sprintf(WarningBuff,"doBatch Unknown Command %s\n",command);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	ret = 0;
	
ErrorOut:
	getCommand(NULL,&cpl);
	return ret;
}
int getCommand(char *line,CommandPtr cp)
{
	static char number[]={
			'0','1','2','3','4','5','6','7','8','9',
			'+','-','.'};
	char buff[256];
	int c,n,nw,iret,k,inum;
	
	if(!cp)return 1;

	for(n=0;n<256;++n){
	    cp->type[n]=BATCH_DOUBLE;
	    cp->value[n]=0;
	    if(cp->command[n])cFree((char *)cp->command[n]);
	    cp->command[n]=NULL;
	}

	cp->line=line;
	cp->nword=0;
	cp->n=0;
	
    if(!line)return 1;
    
	nw=0;
	do{

		while((c = *line) != 0 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')){
			line++;
		}
		
		n=0;
		if(c == 0){
			break;
		}else if(c == '"'){	
		    line++;	
			while((c = *line++) != 0 && c != '"' && n < 255 ){
				buff[n++]=c;
			}
			buff[n]=0;
			iret = 0;
	    	cp->type[nw]=BATCH_STRING;
		    cp->command[nw]=strsave(buff,9153);
		    if(!cp->command[nw]){
				return 1;
		    }
			continue;
		}else{
			while((c = *line++) != 0 && c != ' ' && c != '\n' && c != '\r' && c != '\t'
							 && n < 255 ){
				buff[n++]=c;
			}
		}
		
		if(c == ' ' || c == '"' || c == '\t'){
			iret = 0;
		}else{
			iret = 1;
		}
	
		buff[n]=0;
	
		if(!mstrcmp(buff,".") || !mstrcmp(buff,"..")){
	    	cp->type[nw]=BATCH_STRING;
		    cp->command[nw]=strsave(buff,9154);
		    if(!cp->command[nw])return 1;
			continue;
		}
	
		inum = 0;
		for(k=0;k<sizeof(number);++k){
			if(*buff == number[k]){
				inum = 1;
				break;
			}
		}
	
		if(inum && (cp->getKind != BATCH_STRING)){
			 cp->value[nw] = atof(buff);
		}else{
		    cp->command[nw]=strsave(buff,9155);
		    if(!cp->command[nw])return 1;
	    	cp->type[nw]=BATCH_STRING;
		}
	
    }while((++nw < 256) &&  !iret);

    cp->nword = nw;

	cp->getKind=0;
	    	
    return 0;
}
int NextLine(FILE *input,char *line,int linelength)
{
	int nc;
	int c;

	if(!input || !line || linelength <= 0)return 1;

	--linelength;
	
doWhite:

	while(1){
	    c=fgetc(input);
	    if(c != ' ' || c != '\t' || c != ','){
	        break;
	    }
	}

	if(c == EOF){
	    return 1;
	}else if(c == '!'){
	    while(1){
	        c=fgetc(input);
	        if(c == '\n' || c == '\r' || c == EOF){
	            break;
	        }
	    }
	    if(c == EOF){
	        return 1;
	    }else{
	       goto doWhite;
	    }
	}

	ungetc(c,input);

	nc=0;
	while(1){
	    c=fgetc(input);
	    if(c == '\n' || c == '\r' || c == EOF){
	        break;
	    }
	    if(nc >= linelength)break;
	    line[nc++] = c;
	}

	if(c == EOF){
	    return 1;
	}
	
	if((nc == 0) && (linelength > 0))goto doWhite;
	
	line[nc]=0;
	
	return 0;
}
int doubleCommand(double *value,CommandPtr cp)
{
	if(!cp || !value)return 1;
	if(cp->n >= cp->nword)return 1;
	if(cp->type[cp->n] != BATCH_DOUBLE)return 1;
	*value=cp->value[cp->n];
	return 0;
}

char *stringCommand(CommandPtr cp)
{
	if(!cp)return NULL;
	if(cp->n >= cp->nword)return NULL;
	if(cp->type[cp->n] != BATCH_STRING)return NULL;
	return cp->command[cp->n];
}
