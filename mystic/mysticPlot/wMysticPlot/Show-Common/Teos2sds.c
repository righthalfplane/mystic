#define EXTERN22 extern
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "Xappl.h"
#include "CExpress.h"
#include "NewEdit.h"
#include "Linedata.h"
#include "Message.h"
#include "FileManager.h"

#define uint16 unsigned int
#define DFTAG_SDG ((uint16)700) /* Scientific Data Group*/



struct TEOS {
	float *prs,*tev;
	float **sie,**rho;
	int numm,numt,nump,*eostype,*matid,*numprs;
};

int DFSDrestart(void);
int DFSDclear(void); 
int DFANputlabel(char *file,unsigned short int type,unsigned short int tag,char *name);
int DFSDadddata(char *path,int rank,int *size,float *data);
int DFSDlastref(void);
int DFSDputdata(char *path,int rank,int *size,float *data);

void *cMalloc(unsigned long r,int add);
int cFree(char *ptr);
int mstrncat(char *out,char *in,long n);
int mstrncpy(char *out,char *in,long n);
int zerol(char *s,unsigned long n);

int Teos2sds(struct FileInfo2 *Files,long n);


IconPtr  NewNetWindow(IconPtr myIcon);

/* ================================================================== */
int Teos2sds(struct FileInfo2 *Files,long CurrentFrame)
{
	struct TEOS aTEOS,*pTEOS=NULL;
	double xmin,ymin,xmax,ymax,vmin,vmax;
	double *pix=NULL;
	int imax,jmax;
	int ic,i,j,idf,ret;
	char tag[9];
	int im,ip;
	struct SetFrameData sd;
	struct areadata ai;
	IconPtr myIcon;
	char named[256];
	long length;
	double *fdata;
	int nplot;
	
	
	ret = 1;
	
	nplot=0;
	
	if(!Files)return 1;
	
	myIcon=NewNetWindow(NULL);
	if(!myIcon)goto errorExit;

	pTEOS = &aTEOS;

	zerol((char*)pTEOS,sizeof(struct TEOS));
	zerol(tag,9);
	
	
	mstrncpy(named,"numm",255);	
	fdata=NULL;
	if(SageGetFileItem(Files,named,0,&fdata,&length,CurrentFrame)){
	    ret=17;goto errorExit;
	}
	if(!fdata)goto errorExit;
	pTEOS->numm = (int)fdata[0];	
	if(fdata)cFree((char *)fdata);
	fdata=NULL;
	
	
	mstrncpy(named,"numt",255);	
	fdata=NULL;
	if(SageGetFileItem(Files,named,0,&fdata,&length,CurrentFrame)){
	    ret=18;goto errorExit;
	}
	if(!fdata)goto errorExit;
	pTEOS->numt = (int)fdata[0];	
	if(fdata)cFree((char *)fdata);
	fdata=NULL;
	
	
	mstrncpy(named,"nump",255);	
	fdata=NULL;
	if(SageGetFileItem(Files,named,0,&fdata,&length,CurrentFrame)){
	    ret=19;goto errorExit;
	}
	if(!fdata)goto errorExit;
	pTEOS->nump = (int)fdata[0];	
	if(fdata)cFree((char *)fdata);
	fdata=NULL;
	
	
	mstrncpy(named,"idents",255);	
	fdata=NULL;
	if(SageGetFileItem(Files,named,0,&fdata,&length,CurrentFrame)){
	    ret=20;goto errorExit;
	}
	if(!fdata)goto errorExit;
	
	pTEOS->matid = (int*)cMalloc(pTEOS->numm*sizeof(int),22205);
	if (!pTEOS->matid) goto errorExit;
	for(i=0;i<pTEOS->numm;i++) {
		pTEOS->matid[i] = (int)fdata[i];
	}
	if(fdata)cFree((char *)fdata);
	fdata=NULL;


	mstrncpy(named,"numprs",255);	
	fdata=NULL;
	if(SageGetFileItem(Files,named,0,&fdata,&length,CurrentFrame)){
	    ret=21;goto errorExit;
	}
	if(!fdata)goto errorExit;

	pTEOS->numprs = (int*)cMalloc(pTEOS->numt*sizeof(int),22225);
	if (!pTEOS->numprs) goto errorExit;
	for(i=0;i<pTEOS->numt;i++) {
		pTEOS->numprs[i] = (int)fdata[i];
		if (pTEOS->numprs[0]!=pTEOS->numprs[i]) goto errorExit; /* must be regular array */
	}
	if(fdata)cFree((char *)fdata);
	fdata=NULL;


	mstrncpy(named,"teos_t",255);	
	fdata=NULL;
	if(SageGetFileItem(Files,named,0,&fdata,&length,CurrentFrame)){
	    ret=22;goto errorExit;
	}
	if(!fdata)goto errorExit;

	pTEOS->tev = (float*)cMalloc(pTEOS->numt*sizeof(float),22235);
	if (!pTEOS->tev) goto errorExit;
	for(i=0;i<pTEOS->numt;i++) {
		pTEOS->tev[i] = (float)fdata[i];
	}
	if(fdata)cFree((char *)fdata);
	fdata=NULL;


	mstrncpy(named,"teos_p",255);	
	fdata=NULL;
	if(SageGetFileItem(Files,named,0,&fdata,&length,CurrentFrame)){
	    ret=23;goto errorExit;
	}
	if(!fdata)goto errorExit;


	pTEOS->prs = (float*)cMalloc(pTEOS->nump*sizeof(float),22245);
	if (!pTEOS->prs) goto errorExit;
	for(i=0;i<pTEOS->nump;i++) {
		pTEOS->prs[i] = (float)fdata[i];
	}
	if(fdata)cFree((char *)fdata);
	fdata=NULL;

	pTEOS->sie = (float**)cMalloc(pTEOS->numm*sizeof(float*),22250);
	if (!pTEOS->sie) goto errorExit;
	zerol((char*)pTEOS->sie,pTEOS->numm*sizeof(float*));
	pTEOS->rho = (float**)cMalloc(pTEOS->numm*sizeof(float*),22255);
	if (!pTEOS->rho) goto errorExit;
	zerol((char*)pTEOS->rho,pTEOS->numm*sizeof(float*));

	for(im=0;im<pTEOS->numm;im++) {


		sprintf(named,"teos_e(%d)",im+1);	
		fdata=NULL;
		if(SageGetFileItem(Files,named,0,&fdata,&length,CurrentFrame)){
		    ret=24;goto errorExit;
		}
		if(!fdata)goto errorExit;

		pTEOS->sie[im] = (float*)cMalloc(pTEOS->nump*sizeof(float),22265);
		if (!pTEOS->sie[im]) goto errorExit;
		for(i=0;i<pTEOS->nump;i++) {
			pTEOS->sie[im][i] = (float)fdata[i];
		}
		if(fdata)cFree((char *)fdata);
		fdata=NULL;


		sprintf(named,"teos_r(%d)",im+1);	
		fdata=NULL;
		if(SageGetFileItem(Files,named,0,&fdata,&length,CurrentFrame)){
		    ret=25;goto errorExit;
		}
		if(!fdata)goto errorExit;


		pTEOS->rho[im] = (float*)cMalloc(pTEOS->nump*sizeof(float),22275);
		if (!pTEOS->rho[im]) goto errorExit;
		for(i=0;i<pTEOS->nump;i++) {
			pTEOS->rho[im][i] = (float)fdata[i];
		}
		if(fdata)cFree((char *)fdata);
		fdata=NULL;

	}
	
	pix = (double *)cMalloc(pTEOS->numprs[0]*pTEOS->numt*sizeof(double),22285);
	if (!pix) goto errorExit;

	if (pTEOS->prs[0]<=0.0) goto errorExit;
	if (pTEOS->prs[pTEOS->nump-1]<=0.0) goto errorExit;
	if (pTEOS->tev[0]<=0.0) goto errorExit;
	if (pTEOS->tev[pTEOS->numt-1]<=0.0) goto errorExit;
	xmin=log10(pTEOS->prs[0]);
	xmax=log10(pTEOS->prs[pTEOS->nump-1]);
	ymin=log10(pTEOS->tev[0]);
	ymax=log10(pTEOS->tev[pTEOS->numt-1]);

	vmin=1.0e20;
	vmax=-1.0e20;

	for (ip=0;ip<2;ip++) {
		for (idf=0;idf<pTEOS->numm;idf++) {

			if (ip==0) {
				imax=pTEOS->numprs[0];
				jmax=pTEOS->numt;
				ic=0;
				for (j=0;j<jmax;j++) {
					for (i=0;i<imax;i++) {
						pix[(jmax-1-j)*imax+i] = pTEOS->rho[idf][ic++];
						if (pix[(jmax-1-j)*imax+i]<vmin) vmin = pix[(jmax-1-j)*imax+i];
						if (pix[(jmax-1-j)*imax+i]>vmax) vmax = pix[(jmax-1-j)*imax+i];
					}
				}

			} else  {
				imax=pTEOS->numprs[0];
				jmax=pTEOS->numt;
				ic=0;
				for (j=0;j<jmax;j++) {
					for (i=0;i<imax;i++) {
						pix[(jmax-1-j)*imax+i] = pTEOS->sie[idf][ic++];
						if (pix[(jmax-1-j)*imax+i]<vmin) vmin = pix[(jmax-1-j)*imax+i];
						if (pix[(jmax-1-j)*imax+i]>vmax) vmax = pix[(jmax-1-j)*imax+i];
					}
				}
			}

			
			zerol((char *)&sd,sizeof(struct SetFrameData));
		    sd.type=FRAME_DATA_FLOAT;

			if (ip==0) {
				sprintf(named,"rho(%d)",idf+1);
			} else  {
				sprintf(named,"sie(%d)",idf+1);
			}
			mstrncpy(sd.pioName,named,sizeof(sd.pioName));
		
	        sd.xsize=imax;
	        sd.ysize=jmax;
	        sd.data=pix;
	        	        	        	        
	    	sd.CurrentFrame=nplot;	    			    	
			if(SendMessageByIcon(myIcon,MessageType_SET_REGION_DATA,&sd)){	        	        
				    sprintf(WarningBuff,"Teos2sds - Window Error Saving Data\n");
				    WarningBatch(WarningBuff);
				   goto errorExit;
		    }
		    
	    	zerol((char *)&ai,sizeof(struct areadata));


	        mstrncpy(ai.targetName,sd.pioName,sizeof(sd.pioName));
			ai.xminArea=xmin;
			ai.yminArea=ymin;
			ai.zminArea=0;
			ai.xmaxArea=xmax;
			ai.ymaxArea=ymax;
			ai.zmaxArea=0;
			ai.pioTime=(double)idf;
			ai.vminArea = vmin;
			ai.vmaxArea = vmax;
			
			ai.CurrentFrame=nplot;
			
			ai.type = FRAME_DATA_FLOAT;
			if(SendMessageByIcon(myIcon,MessageType_SET_AREA_RANGE,&ai)){
				    sprintf(WarningBuff,"Teos2sds - Window Error Setting Data range\n");
				    WarningBatch(WarningBuff);
				   goto errorExit;
			}
		    		    		    		    
		    ++nplot;

		} /* end loop on data files */
	} /* end loop on property count */

	ret = 0;

errorExit:
	if (pTEOS->matid) cFree((char*)pTEOS->matid);
	if (pTEOS->numprs) cFree((char*)pTEOS->numprs);
	if (pTEOS->tev) cFree((char*)pTEOS->tev);
	if (pTEOS->prs) cFree((char*)pTEOS->prs);
	for (im=0;im<pTEOS->numm;im++) {
		if (pTEOS->sie[im]) cFree((char*)pTEOS->sie[im]);
		if (pTEOS->rho[im]) cFree((char*)pTEOS->rho[im]);
	}
	if (pTEOS->sie) cFree((char*)pTEOS->sie);
	if (pTEOS->rho) cFree((char*)pTEOS->rho);
	if (pix) cFree((char*)pix);
	if(fdata)cFree((char *)fdata);
	return ret;
}
