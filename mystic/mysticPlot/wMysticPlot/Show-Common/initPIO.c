#define EXTERN22 extern
#include <ctype.h>
#include "Xappl.h"
#include "uFiles.h"
#include "CExpress.h"
#include "initPIOBlocked.h"
#include "doSage3DGet.h"

#define LEVEL_MAX	100

int CheckForWrite(struct file8 **in8);

int isDerived(struct PIO *sage,char *dataName,int *index);

int TestDouble(double *one,double *two);

struct PIO *sageHeader(char *name);
int sageClose(struct PIO *sage,int flag);

int SageGetData(struct PIO *sage,char *dataName,int index,
                double **data,long *dataLength,int *dataType);

int SageSetData(struct PIO *sage,char *dataName,int index,
                void *data,long dataLength);

static int SageTranslateDoubles(struct PIO *sage,double *d,long length);

int FilesOpenPIO2D(struct FileInfo2 *files);
int FilesOpenPIO3D(struct FileInfo2 *files);

int clip_names(char *names);

#define   SIO_VERSION  19970909
#define   SIO_NAME_LENGTH   32
#define   SIO_HEADER_LENGTH   16
#define   SIO_INDEX_LENGTH   8
#define   MXSIO   1000

#define   SIO_RESTART   0
#define   SIO_X3D   1

#define WORDSIZE   8

#define MXBYTE   8

int sio_get_string(char *out,int out_len,char *in,int *in_len);

static int intcmp(const void *xx, const void *yy);

int initPIO(struct FileInfo2 *Files);

int initPIO2(struct FileInfo2 *Files,int numdim);

static int CheckPIO2DSpace(struct FileInfo2 *Files);
static int FilesSetFramePIO(long itIs,struct FileInfo2 *Files);

int CheckNames(struct FileInfo2 *Files);

int SageGetDerived(struct PIO *sage,char *dataName,int index,
                   double **data,long *dataLength,int *dataType);

int IsPIO(char *namef,int *rank);

int SageCheckName(struct PIO *sage,char *dataName,int Record);

int doSage1DStart(struct FileInfo2 *Files);

int doSage2DStart(struct FileInfo2 *Files);

int doSage3DStart(struct FileInfo2 *Files);

int FilesOpenPIODUMP(struct FileInfo2 *Files);

static int SageGetItemBlock(struct PIO *sage,struct FilePIOInfo *pio,char *dataName,long plotItem,double *data,long start, long dataLength,int *dataType);

static int SageGetDataBlock(struct PIO *sage,char *dataName,int index,double *data,long start,long dataLength,int *dataType);

int SageGetDerivedBlock(struct PIO *sage,struct FilePIOInfo *pio,char *dataName,int plotItem,double *data,long start,long dataLength,int *dataType);

int SageSetBlockSize(struct FilePIOInfo *pio,long dataLength);

int SageSetBlockSize(struct FilePIOInfo *pio,long dataLength)
{
	int ret;
	
	if(!pio)return 1;

	ret=1;
	
    if((dataLength > pio->BlockSizeTemp) || !pio->bt1 || !pio->bt2 || !pio->bt3 || !pio->bt4){
    	if(pio->bt1)cFree((char *)pio->bt1);
    	pio->bt1=NULL;
    	if(pio->bt2)cFree((char *)pio->bt2);
    	pio->bt2=NULL; 	
    	if(pio->bt3)cFree((char *)pio->bt3);
    	pio->bt3=NULL; 	
    	if(pio->bt4)cFree((char *)pio->bt4);
    	pio->bt4=NULL; 	
		pio->bt1=(void *)cMalloc(sizeof(double)*dataLength,7018);
		if(!pio->bt1){ret=4;goto ErrorOut;};
		pio->bt2=(void *)cMalloc(sizeof(double)*dataLength,7019);
		if(!pio->bt2){ret=4;goto ErrorOut;};
		pio->bt3=(void *)cMalloc(sizeof(double)*dataLength,7018);
		if(!pio->bt3){ret=4;goto ErrorOut;};
		pio->bt4=(void *)cMalloc(sizeof(double)*dataLength,7019);
		if(!pio->bt4){ret=4;goto ErrorOut;};
		pio->BlockSizeTemp=dataLength;
    }


	ret=0;
ErrorOut:

	return ret;
}

int SageGetDerivedBlock(struct PIO *sage,struct FilePIOInfo *pio,char *dataName,int plotItem,double *data,long start,long dataLength,int *dataType)
{
	char name[256],*fp;	
	double *mass,*volume;
	long mass_length,volume_length;
	static long start_chunk;
	int mass_type,volume_type;
	double *mixed_cells,*num_frac_mat,*dataf;
	double *chunk_mat,*chunk_mass,*chunk_eng,*chunk_vol;
	long mixed_length,num_frac_mat_length;
	int mixed_type,num_frac_mat_type;
	int ret;
	int n;
	long l,m,k,item;
	int nv;

	if(!sage || !dataName || !data || !dataLength || !dataType || !pio)return 1;

	ret=1;

	mass=NULL;
	volume=NULL;
	chunk_mat=NULL;
	chunk_mass=NULL;
	chunk_eng=NULL;
	chunk_vol=NULL;
	mixed_cells=NULL;
	num_frac_mat=NULL;

    if(SageSetBlockSize(pio,dataLength)){
    	goto ErrorOut;
    }

	dataf=data;
	zerol((char *)dataf,sizeof(double)*dataLength);

	plotItem = - plotItem;
	
		
	if(plotItem < 20){
	    item=plotItem;
	    if(item == 1){
			if(SageGetItemBlock(sage,pio,"mass",0,pio->bt1,start,
		               dataLength,&mass_type))goto ErrorOut;
			if(SageGetItemBlock(sage,pio,"vcell",0,pio->bt2,start,
		               dataLength,&volume_type))goto ErrorOut;
			for(l=0;l<dataLength;++l){
	           if(pio->bt1[l] > 0 && pio->bt2[l] > 0){
	               dataf[l]=pio->bt1[l]/pio->bt2[l];
	           }else{
	               dataf[l]=0;
	           }
			}
	    }else if(item == 2){	    
	 	    if(SageCheckName(sage,"cell_velocity",0)){
				for(k=0;k<sage->numdim;++k){
					if(SageGetItemBlock(sage,pio,"cell_velocity",(int)k,pio->bt1,start,
					              dataLength,&mass_type))goto ErrorOut;
					for(l=0;l<dataLength;++l){
						dataf[l] += (pio->bt1[l]*pio->bt1[l]);
					}
				}
			}else if(SageCheckName(sage,"cell_momentum",1)){
						
				if(SageGetItemBlock(sage,pio,"mass",0,pio->bt1,start,
			               dataLength,&mass_type))goto ErrorOut;
			               			               
				for(k=0;k<sage->numdim;++k){		              			               				               
					if(SageGetItemBlock(sage,pio,"cell_momentum",(int)(k+1),pio->bt2,start,
					           dataLength,&volume_type))goto ErrorOut;
					for(l=0;l<dataLength;++l){
					    double vel;
						if(pio->bt1[l] > 0){
						   vel=pio->bt2[l]/pio->bt1[l];
						   dataf[l] += (vel*vel);
						}
					}
				}								
			}else{
			    goto ErrorOut;
			}
			for(l=0;l<dataLength;++l){
					dataf[l]=sqrt(dataf[l]);
			}
	    }else if(item == 3){
			m=0;
			for(l=0;l<sage->numcell;++l){
			    if(l >= start && l < start+dataLength){
			    	dataf[m]=(double)(l+1);
					++m;
				}
			}
	    }else if(item == 4){  /* xdt */	    	    
	 	    if(SageCheckName(sage,"cell_velocity(1)",0)){
				if(SageGetItemBlock(sage,pio,"cell_velocity(1)",(int)0,pio->bt1,start,
				           dataLength,&mass_type))goto ErrorOut;
				for(l=0;l<dataLength;++l){
					dataf[l] = pio->bt1[l];
				}
			}else if(SageCheckName(sage,"cell_momentum",1)){						
				if(SageGetItemBlock(sage,pio,"mass",0,pio->bt1,start,
				       dataLength,&mass_type))goto ErrorOut;				       			               
				if(SageGetItemBlock(sage,pio,"cell_momentum",(int)1,pio->bt2,start,
				           dataLength,&volume_type))goto ErrorOut;
				for(l=0;l<dataLength;++l){
				    double vel;
					if(pio->bt1[l] > 0){
					   vel=pio->bt2[l]/pio->bt1[l];
					   dataf[l] = vel;
					}
				}
			}else{
			    goto ErrorOut;
			}
	 	}else if(item == 5){  /* ydt */
	 	    if(SageCheckName(sage,"cell_velocity(2)",1)){
				if(SageGetItemBlock(sage,pio,"cell_velocity(2)",(int)1,pio->bt1,start,
				           dataLength,&mass_type))goto ErrorOut;
				for(l=0;l<dataLength;++l){
					dataf[l] = pio->bt1[l];
				}
			}else if(SageCheckName(sage,"cell_momentum",2)){						
				if(SageGetItemBlock(sage,pio,"mass",0,pio->bt1,start,
				       dataLength,&mass_type))goto ErrorOut;				       			               
				if(SageGetItemBlock(sage,pio,"cell_momentum",(int)2,pio->bt2,start,
				           dataLength,&volume_type))goto ErrorOut;
				for(l=0;l<dataLength;++l){
				    double vel;
					if(pio->bt1[l] > 0){
					   vel=pio->bt2[l]/pio->bt1[l];
					   dataf[l] = vel;
					}
				}
			}else{
			    goto ErrorOut;
			}
	    }else if(item == 6){  /* zdt */
	 	    if(SageCheckName(sage,"cell_velocity(3)",2)){
				if(SageGetItemBlock(sage,pio,"cell_velocity(3)",(int)2,pio->bt1,start,
				           dataLength,&mass_type))goto ErrorOut;
				for(l=0;l<dataLength;++l){
					dataf[l] = pio->bt1[l];
				}
			}else if(SageCheckName(sage,"cell_momentum",3)){						
				if(SageGetItemBlock(sage,pio,"mass",0,pio->bt1,start,
				       dataLength,&mass_type))goto ErrorOut;				       			               
				if(SageGetItemBlock(sage,pio,"cell_momentum",(int)3,pio->bt2,start,
				           dataLength,&volume_type))goto ErrorOut;
				for(l=0;l<dataLength;++l){
				    double vel;
					if(pio->bt1[l] > 0){
					   vel=pio->bt2[l]/pio->bt1[l];
					   dataf[l] = vel;
					}
				}
			}else{
			    goto ErrorOut;
			}
	    }else if(item == 7){  /* sj2 */
			if(SageGetItemBlock(sage,pio,"sxx",0,pio->bt1,start,
			       dataLength,&mass_type))goto ErrorOut;
			if(SageGetItemBlock(sage,pio,"syy",0,pio->bt2,start,
			       dataLength,&volume_type))goto ErrorOut;
			for(l=0;l<dataLength;++l){
			    dataf[l] += 3.0*(pio->bt2[l]*pio->bt2[l]+pio->bt1[l]*pio->bt1[l]+pio->bt2[l]*pio->bt1[l]);
			}
			if(sage->numdim > 1){			
				if(SageGetItemBlock(sage,pio,"sxy",0,pio->bt2,start,
				       dataLength,&volume_type))goto ErrorOut;
				for(l=0;l<dataLength;++l){
				    dataf[l] += 3.0*(pio->bt2[l]*pio->bt2[l]);
				}			
			}
			if(sage->numdim > 2){			
				if(SageGetItemBlock(sage,pio,"syz",0,pio->bt1,start,
				       dataLength,&mass_type))goto ErrorOut;
				if(SageGetItemBlock(sage,pio,"szx",0,pio->bt2,start,
				       dataLength,&volume_type))goto ErrorOut;
				for(l=0;l<dataLength;++l){
				    dataf[l] += 3.0*(pio->bt2[l]*pio->bt2[l]+pio->bt1[l]*pio->bt1[l]);
				}
			}		
			for(l=0;l<dataLength;++l){
				dataf[l]=sqrt(dataf[l]);
			}
	    }else if(item == 8){  /*txx */
			if(SageGetItemBlock(sage,pio,"sxx",0,pio->bt1,start,
			       dataLength,&mass_type))goto ErrorOut;
			if(SageGetItemBlock(sage,pio,"pres",0,pio->bt2,start,
			       dataLength,&volume_type))goto ErrorOut;
			for(l=0;l<dataLength;++l){
				dataf[l]=pio->bt1[l]-pio->bt2[l];
			}
	    }else if(item == 9){  /*tyy */
			if(SageGetItemBlock(sage,pio,"syy",0,pio->bt1,start,
			       dataLength,&mass_type))goto ErrorOut;
			if(SageGetItemBlock(sage,pio,"pres",0,pio->bt2,start,
			       dataLength,&volume_type))goto ErrorOut;
			for(l=0;l<dataLength;++l){
				dataf[l]=pio->bt1[l]-pio->bt2[l];
			}
	    }else if(item == 10){  /*tzz */
			if(SageGetItemBlock(sage,pio,"syy",0,pio->bt1,start,
			       dataLength,&mass_type))goto ErrorOut;
			if(SageGetItemBlock(sage,pio,"pres",0,pio->bt2,start,
			       dataLength,&volume_type))goto ErrorOut;
			for(l=0;l<dataLength;++l){
				dataf[l]= -pio->bt1[l] -pio->bt2[l];
			}
									
			if(SageGetItemBlock(sage,pio,"sxx",0,pio->bt2,start,
			       dataLength,&volume_type))goto ErrorOut;
			for(l=0;l<dataLength;++l){
			    dataf[l] -= pio->bt2[l];
			}
	    }else if(item == 11){  /*ste */
			if(SageGetItemBlock(sage,pio,"cell_energy",0,pio->bt1,start,
		               dataLength,&mass_type))goto ErrorOut;
			if(SageGetItemBlock(sage,pio,"mass",0,pio->bt2,start,
		               dataLength,&volume_type))goto ErrorOut;
			for(l=0;l<dataLength;++l){
	           if(pio->bt2[l] > 0){
	               dataf[l]=pio->bt1[l]/pio->bt2[l];
	           }else{
	               dataf[l]=0;
	           }
			}
	    }else if(item == 12){  /*szz */
			if(SageGetItemBlock(sage,pio,"syy",0,pio->bt1,start,
			       dataLength,&mass_type))goto ErrorOut;
			if(SageGetItemBlock(sage,pio,"sxx",0,pio->bt2,start,
			       dataLength,&volume_type))goto ErrorOut;
			for(l=0;l<dataLength;++l){
				dataf[l]= -pio->bt1[l] -pio->bt2[l];
			}
			
	    }else if(item == 14){  /* tracer_data */	   
	    
			if(SageGetItem(sage,"tracer_data",0,(&mass),
			       &mass_length,&mass_type))goto ErrorOut;
		
		    mstrncpy(name,dataName,256);
		    fp=strrchr((char *)name,'(');
		    if(fp){
		        ++fp;
		        nv=(int)atoi(fp);
			}else{
				goto ErrorOut;	    
			}
			n=0;  
			for(l=0;l<mass_length;l += sage->tracer_words_per_record){
				dataf[n++]= mass[nv+l];
			}
			*dataType = SAGE_DOUBLE;
			/* *dataLength=n; */
			ret=0;
			goto ErrorOut;	    
	    }else{
	        sprintf(WarningBuff,"SageGetDerived  Name %s plotItem %d Out of Range nummat %ld mixed_cell %d\n",
	                           dataName,plotItem,sage->nummat,sage->mixed_cell);
	        WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    goto okOut;
	}

	if(plotItem < 4000){
	
		plotItem -= 2000;

		if(sage->mixed_cell == 2){
		    int sw,index;

		    sw=plotItem % 5;
		    index=(plotItem/5);
		    
		    switch(sw){
		       case 0:
				if(SageGetItemBlock(sage,pio,"frac_mass",index+1,pio->bt1,start,
			               dataLength,&mass_type))goto ErrorOut;
				if(SageGetItemBlock(sage,pio,"frac_vol",index,pio->bt2,start,
			               dataLength,&volume_type))goto ErrorOut;
				for(l=0;l<dataLength;++l){
			           if(pio->bt1[l] > 0 && pio->bt2[l] > 0){
			               dataf[l]=index+1;
			           }
				}
		           break;
		       case 1:
				if(SageGetItemBlock(sage,pio,"frac_mass",index+1,pio->bt1,start,
					      dataLength,&mass_type))goto ErrorOut;
				for(l=0;l<dataLength;++l){
			    	dataf[l]=pio->bt1[l];
				}
		           break;
		       case 2:
					if(SageGetItemBlock(sage,pio,"frac_eng",index+1,pio->bt1,start,
						      dataLength,&mass_type))goto ErrorOut;
					for(l=0;l<dataLength;++l){
				    	dataf[l]=pio->bt1[l];
					}
		           break;
		       case 3:
					if(SageGetItemBlock(sage,pio,"frac_vol",index+1,pio->bt1,start,
					           dataLength,&mass_type))goto ErrorOut;
					if(SageGetItemBlock(sage,pio,"vcell",0,pio->bt2,start,
					           dataLength,&volume_type))goto ErrorOut;
					for(l=0;l<sage->numcell;++l){
						if(pio->bt1[l] > 0 && pio->bt2[l] > 0){
						   dataf[l]=pio->bt1[l]/pio->bt2[l];
						}
					}
		           break;
		           
		       case 4:
					if(SageGetItemBlock(sage,pio,"frac_mass",index+1,pio->bt1,start,
					           dataLength,&mass_type))goto ErrorOut;
					if(SageGetItemBlock(sage,pio,"vcell",0,pio->bt2,start,
					           dataLength,&volume_type))goto ErrorOut;
					for(l=0;l<sage->numcell;++l){
						if(pio->bt1[l] > 0 && pio->bt2[l] > 0){
						   dataf[l]=pio->bt1[l]/pio->bt2[l];
						}
					}
		           break;
		    }
		}else if(sage->mixed_cell == 3){
		    long size_chunk;
		    int sw,index;

		    sw=plotItem % 5;
		    index=(plotItem/5);

		    
			if(SageGetItemBlock(sage,pio,"chunk_nummat",0,pio->bt1,start,
			           dataLength,&volume_type))goto ErrorOut;
			           
		    if(start <= 0)start_chunk=0;

	        size_chunk=0;

		    for(l=0;l<dataLength;++l){
				size_chunk += (long)pio->bt1[l];
		     }
		     
		     if(size_chunk > pio->BlockSizeTemp){		     
			    if(SageSetBlockSize(pio,size_chunk)){
			    	goto ErrorOut;
			    }
				if(SageGetItemBlock(sage,pio,"chunk_nummat",0,pio->bt1,start,
				           dataLength,&volume_type))goto ErrorOut;
		     }

			if(SageGetItemBlock(sage,pio,"chunk_mat",0,pio->bt2,start_chunk,
			           size_chunk,&volume_type))goto ErrorOut;
			           
		    switch(sw){
		       case 0:
				if(SageGetItemBlock(sage,pio,"chunk_mass",0,pio->bt3,start_chunk,
				           size_chunk,&volume_type))goto ErrorOut;
				           
				if(SageGetItemBlock(sage,pio,"chunk_vol",0,pio->bt4,start_chunk,
				           size_chunk,&volume_type))goto ErrorOut;
				k=0;
				for(l=0;l<dataLength;++l){
				    for(m=0;m<(long)(pio->bt1[l]);++m,++k){
				        n=((int)pio->bt2[k])-1;
				        if(index == n){
				           if(pio->bt3[k] > 0 && pio->bt4[k] > 0){
				               dataf[l]=index+1;
				           }
			            }
				    }
				}
		        break;
		       case 1:
				if(SageGetItemBlock(sage,pio,"chunk_mass",0,pio->bt3,start_chunk,
				           size_chunk,&volume_type))goto ErrorOut;

				k=0;
				for(l=0;l<dataLength;++l){
				    for(m=0;m<(long)(pio->bt1[l]);++m,++k){
				        n=((int)pio->bt2[k])-1;
				        if(index == n){
				            dataf[l]=pio->bt3[k];
			            }
				    }
				}

		        break;
		       case 2:

				if(SageGetItemBlock(sage,pio,"chunk_eng",0,pio->bt3,start_chunk,
				           size_chunk,&volume_type))goto ErrorOut;
				k=0;
				for(l=0;l<dataLength;++l){
				    for(m=0;m<(long)(pio->bt1[l]);++m,++k){
				        n=((int)pio->bt2[k])-1;
				        if(index == n){
				            dataf[l]=pio->bt3[k];
			            }
				    }
				}

		        break;
		       case 3:
				if(SageGetItemBlock(sage,pio,"chunk_vol",0,pio->bt3,start_chunk,
				           size_chunk,&volume_type))goto ErrorOut;					      
					      
				if(SageGetItemBlock(sage,pio,"vcell",0,pio->bt4,start,
				           dataLength,&volume_type))goto ErrorOut;
				k=0;
				for(l=0;l<dataLength;++l){
				    for(m=0;m<(long)(pio->bt1[l]);++m,++k){
				        n=((int)pio->bt2[k])-1;
				        if(index == n){
				           if(pio->bt3[k] > 0 && pio->bt4[l] > 0){
				               dataf[l]=pio->bt3[k]/pio->bt4[l];
				           }
		                }
				    }
				}

		        break;
		           	           
		       case 4:
				if(SageGetItemBlock(sage,pio,"chunk_mass",0,pio->bt3,start_chunk,
				           size_chunk,&volume_type))goto ErrorOut;
			               
				if(SageGetItemBlock(sage,pio,"vcell",0,pio->bt4,start,
				           dataLength,&volume_type))goto ErrorOut;
				k=0;
				for(l=0;l<dataLength;++l){
				    for(m=0;m<(long)(pio->bt1[l]);++m,++k){
				        n=((int)pio->bt2[k])-1;
				        if(index == n){
				           if(pio->bt3[k] > 0 && pio->bt4[l] > 0){
				               dataf[l]=pio->bt3[k]/pio->bt4[l];
				           }
		                }
				    }
				}
		        break;
		    }
			start_chunk += size_chunk;
		}else if(sage->mixed_cell == 1){

		    if(SageGetItem(sage,"mixed_cell",0,(&mixed_cells),
		               &mixed_length,&mixed_type))goto ErrorOut;
		    if(SageGetItem(sage,"num_frac_mat",0,(&num_frac_mat),
		               &num_frac_mat_length,&num_frac_mat_type))goto ErrorOut;
		    if(SageGetItem(sage,"vcell",0,(&volume),
		               &volume_length,&volume_type))goto ErrorOut;

		    k=0;
		    for(l=0;l<sage->numcell;++l){
		        for(m=0;m<(long)(num_frac_mat[l]);++m){
		            int sw;
		            n=((int)mixed_cells[k])-1;
		            if(plotItem/5 == n){
		                sw= plotItem % 5;
		                if(sw == 0){
		                   if(l >= start && l < start+dataLength)dataf[l-start]=n+1;
		                }else if(sw == 3){
		                   if(mixed_cells[k+3] > 0){
		                       if(l >= start && l < start+dataLength)dataf[l-start]=mixed_cells[k+3]/volume[l];
		                   }
		                }else if(sw == 4){
		                   if(mixed_cells[k+1] > 0 && mixed_cells[k+3] > 0){
		                       if(l >= start && l < start+dataLength)dataf[l-start]=mixed_cells[k+1]/volume[l];
		                   }
		                }else{
		                   if(l >= start && l < start+dataLength)dataf[l-start]=mixed_cells[k+sw];
		                }
	                }
	                k=k+4;
		        }
		    }
		}
		goto okOut;
		
	}
	
okOut:
	*dataType = SAGE_DOUBLE;
	ret=0;
ErrorOut:
	if(mixed_cells)cFree((char *)mixed_cells);
	if(num_frac_mat)cFree((char *)num_frac_mat);
	
	if(mass)cFree((char *)mass);
	if(volume)cFree((char *)volume);
	
	if(chunk_mat)cFree((char *)chunk_mat);
	if(chunk_mass)cFree((char *)chunk_mass);
	if(chunk_eng)cFree((char *)chunk_eng);
	if(chunk_vol)cFree((char *)chunk_vol);
	return ret;
}
int SageGetFileItemBlock(struct FileInfo2 *Files,char *dataName,long plotItem,
                         double *data,long start,long dataLength,long CurrentFrame)
{
	struct FilePIOInfo *pio;
	struct SetFrameData *s;
	struct PIO *sage;
	int dataType;
	int ret;
	
	if(!Files || !dataName || !data || !dataLength)return 1;
	if(!Files->PIO2DList)return 1;
	s = &Files->SetData;
	pio=&Files->pioData;
	
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	sage = &Files->PIO2DList[CurrentFrame];
	
	if(!sage)return 1;
	
	s->pioTime=sage->time;

	if(SetFileToRead8(CurrentFrame,Files))return 1;
	
	sage->in=Files->in8;
	
	ret=SageGetItemBlock(sage,pio,dataName,plotItem,data,start,dataLength,&dataType);
	
	sage->in=NULL;
	
	return ret;
}
static int SageGetItemBlock(struct PIO *sage,struct FilePIOInfo *pio,char *dataName,long plotItem,double *data,long start, long dataLength,int *dataType)
{
	int index;
	
	if(!sage || !dataName)return 1;

	if(isDerived(sage,dataName,&index)){
	    return SageGetDerivedBlock(sage,pio,dataName,index,data,start,dataLength,dataType);
	}

	return SageGetDataBlock(sage,dataName,(int)plotItem,data,start,dataLength,dataType);

}
static int SageGetDataBlock(struct PIO *sage,char *dataName,int index,double *data,long start,long dataLength,int *dataType)
{
	char name[1024];
	char *fp,*np;
	int ret;
	int n;

	if(!sage || !dataName || !data || !dataLength || !dataType)return 1;
	if(!sage->in || !sage->data)return 1;	

	ret=2;

	np=strrchr((char *)dataName,'(');

    for(n=0;n<sage->sio_num;++n){
/*       
	    fprintf(stderr,"%s ",sage->data[n].sio_names);
	    fprintf(stderr,"%d %d %lld %d\n",sage->data[n].sio_indexs,sage->data[n].sio_lengths,
				    sage->data[n].sio_positions,sage->data[n].sio_types);
*/

	    if(np){
		    if(strcmp(sage->data[n].sio_names,dataName)){
		    	continue;
		    }
	    }else{
		    mstrncpy(name,sage->data[n].sio_names,1024);
		    fp=strrchr((char *)name,'(');
		    if(fp){
		        *fp = 0;
		    }

		    if(!strcmp(name,dataName)){
		    	if(index != sage->data[n].sio_indexs)continue;
		    }else{
		        continue;
		    }
	    }

		if(fseek8(sage->in,WORDSIZE*(sage->data[n].sio_positions+start),0)){ret=3;goto ErrorOut;};
		if(fget8(sage->in,(char *)data,WORDSIZE*dataLength)){ret=5;goto ErrorOut;};
		*dataType=SAGE_DOUBLE;
        SageTranslateDoubles(sage,(double *)data,dataLength);
		ret = 0;
		break;
	}
ErrorOut:
	return ret;
}


int filePIOList(struct FileInfo2 *Files,long CurrrentFrame,struct ScrollList *List,int flag)
{

    struct PIO *sage;
    char *names[20],*name;
    char buff[256],test[1024],*fp;
    int k,n;
	    
	if(!Files || !List)return 1;
	if(!Files->PIO2DList){
	    if(Files->PIOBlkList){
			return filePIOBlkList(Files,CurrrentFrame,List,flag);
		}
	     return 1;
	}

	if(CurrrentFrame < 0 || CurrrentFrame >= Files->ImageCount)CurrrentFrame = 0;

	sage = &Files->PIO2DList[CurrrentFrame];
	if(!sage)return 1;
	
	k=0;
	for(n=0;n<sage->sio_num;++n){
	    if(k >= 20){
	        ScrollListInsertOneType(names,3L,-1L,k,List);
	        k=0;
	    }
		if((sage->data[n].sio_lengths != sage->numcell) && flag)continue;
		name = sage->data[n].sio_names;
		
		mstrncpy(test,name,1024);
	    fp=strrchr((char *)test,'(');
	    if(fp){
	        *fp = 0;
	    }
		
	    if(!strcmp(test,"frac_mass"))continue;
	    if(!strcmp(test,"frac_vol"))continue;
	    if(!strcmp(test,"frac_eng"))continue;
	    
	    if(!strcmp(test,"chunk_nummat"))continue;
	    
	    names[k++]=name;
	}
	if(k > 0)ScrollListInsertOneType(names,3L,-1L,k,List);
	
	if(sage->numdim == -1)goto Done;

	if(sage->mixed_cell){
	    for(n=0;n<sage->nummat;++n){
	        char buff1[256],buff2[256],buff3[256],buff4[256],buff5[256];
	        sprintf(buff1,"mat_%ld",(long)n+1);
	        sprintf(buff2,"mass_%ld",(long)n+1);
	        sprintf(buff3,"eng_%ld",(long)n+1);
	        sprintf(buff4,"fvol_%ld",(long)n+1);
	        sprintf(buff5,"density_%ld",(long)n+1);
		    names[0]=buff1;
		    names[1]=buff2;
		    names[2]=buff3;
		    names[3]=buff4;
		    names[4]=buff5;
		    ScrollListInsertOneType(names,3L,-1L,5,List);
	    }
	}

	sprintf(buff,"density");
	names[0]=buff;
	ScrollListInsertOneType(names,3L,-1L,1,List);
		
	sprintf(buff,"speed");
	names[0]=buff;
	ScrollListInsertOneType(names,3L,-1L,1,List);
	
	sprintf(buff,"cell_number");
	names[0]=buff;
	ScrollListInsertOneType(names,3L,-1L,1,List);

	sprintf(buff,"xdt");
	names[0]=buff;
	ScrollListInsertOneType(names,3L,-1L,1,List);
	
	sprintf(buff,"ste");
	names[0]=buff;
	ScrollListInsertOneType(names,3L,-1L,1,List);
	
	if(sage->numdim > 1){
		sprintf(buff,"ydt");
		names[0]=buff;
		ScrollListInsertOneType(names,3L,-1L,1,List);
		
	}

	if(sage->numdim > 2){
		sprintf(buff,"zdt");
		names[0]=buff;
		ScrollListInsertOneType(names,3L,-1L,1,List);
	}
	
	if(sage->strength){
		sprintf(buff,"sj2");
		names[0]=buff;
		ScrollListInsertOneType(names,3L,-1L,1,List);
		
		sprintf(buff,"txx");
		names[0]=buff;
		ScrollListInsertOneType(names,3L,-1L,1,List);
	
		sprintf(buff,"tyy");
		names[0]=buff;
		ScrollListInsertOneType(names,3L,-1L,1,List);
		
		sprintf(buff,"tzz");
		names[0]=buff;
		ScrollListInsertOneType(names,3L,-1L,1,List);
		
		sprintf(buff,"szz");
		names[0]=buff;
		ScrollListInsertOneType(names,3L,-1L,1,List);		
		
	}

Done:
	
	ScrollListSort(LIST_SORT_ASCII,List);
		
	return 0;
}

int initPIO(struct FileInfo2 *Files)
{
	struct PIO *sage;
    char name[2048];
	int numdim;

	if(!Files)return 1;
	if(!Files->FilesNames || !Files->FilesNames[0])return 1;

	mstrncpy(name,Files->directory,2048);
	strncatToPath(name,Files->FilesNames[0],2048);
	    
	sage=sageHeader(name);
    if(!(sage)){
         sprintf(WarningBuff,"initPIO Could Not Open Sage File \"%s\" To Read\n",Files->FilesNames[0]);
         WarningBatch(WarningBuff);
         return 1;
    }

	numdim=(int)sage->numdim;

	sageClose(sage,1);

	if(numdim == -1 || numdim == 1 || numdim == 2 || numdim == 3){
	    return initPIO2(Files,numdim);
	}else{
	     sprintf(WarningBuff,"Sage File \"%s\" Has Unsupported numdim %d\n",
	                         Files->FilesNames[0],numdim);
	     WarningBatch(WarningBuff);
             return 1;
	}
}
int initPIO2(struct FileInfo2 *Files,int numdim)
{
	double rmin[3],rmax[3];
	double rminp[3],rmaxp[3];
    long n,ImageCount;
	struct PIO *sage;
    double *fdata,*ldata;
	int ret,ndim;
    char name[2048];
    int fileType;

	if(!Files)return 1;

	ret=1;
	
	if((numdim < 1 || numdim > 3) && (numdim != -1)){
	   sprintf(WarningBuff,"initPIO2D Bad numdim %d\n",numdim);
	   WarningBatch(WarningBuff);
	   return 1;
	}

	rmin[0]=rmin[1]=rmin[2]=     1e60;
	rmax[0]=rmax[1]=rmax[2]=    -1e60;

	ImageCount=0;

	fdata=NULL;
	ldata=NULL;

	for(n=0;n<Files->FileCount;++n){

	    sage = NULL;
	    ldata = NULL;

	    rminp[0]=rminp[1]=rminp[2]=     1e60;
	    rmaxp[0]=rmaxp[1]=rmaxp[2]=    -1e60;

		mstrncpy(name,Files->directory,2048);
		strncatToPath(name,Files->FilesNames[n],2048);
	
	    sage=sageHeader(name);
        if(!sage){
	         sprintf(WarningBuff,"initPIO2D Could Not Open Sage File \"%s\" To Read\n",name);
	         WarningBatch(WarningBuff);
             break;
        }
	    if(sage->numdim != numdim){
			sprintf(WarningBuff,"Sage File \"%s\" Has UnMatched sage->numdim %ld numdim %d\n",
		                         Files->FilesNames[n],sage->numdim,numdim);
			WarningBatch(WarningBuff);
			sageClose(sage,1);
			sage=NULL;
			continue;
	    }

		if(CheckPIO2DSpace(Files))goto ErrorOut;


		for(ndim=0;ndim<numdim;++ndim){
		    if(ndim == 0){
		        rminp[ndim]=sage->xzero;
		        rmaxp[ndim]=sage->xzero+sage->dxset*(double)sage->ixmax;
		        if(numdim == 1){
		            rmin[1]=0;
		            rmax[1]=sage->dxset;
		        }
		    }else if(ndim == 1){
		        rminp[ndim]=sage->yzero;
		        rmaxp[ndim]=sage->yzero+sage->dyset*(double)sage->iymax;
		    }else if(ndim == 2){
		        rminp[ndim]=sage->zzero;
		        rmaxp[ndim]=sage->zzero+sage->dzset*(double)sage->izmax;
		    }
		}

		for(ndim=0;ndim<numdim;++ndim){
            if(rminp[ndim] < rmin[ndim])rmin[ndim]=rminp[ndim];
            if(rmaxp[ndim] > rmax[ndim])rmax[ndim]=rmaxp[ndim];
		}

		if(ldata)cFree((char *)ldata);
		ldata=NULL;

		sage->xmin=rminp[0];
		sage->ymin=rminp[1];
		sage->zmin=rminp[2];

		sage->xmax=rmaxp[0];
		sage->ymax=rmaxp[1];
		sage->zmax=rmaxp[2];

		Files->PIO2DList[ImageCount]= *sage;

		Files->ImageNames[ImageCount]=strsave(Files->FilesNames[n],8812);
		
		Files->ImageFiles[ImageCount]=n;

		Files->ImageCount = ++ImageCount;


		cFree((char *)sage);
		
	}
	Files->xminr=rmin[0];
	Files->yminr=rmin[1];
	Files->zminr=rmin[2];
	Files->xmaxr=rmax[0];
	Files->ymaxr=rmax[1];
	Files->zmaxr=rmax[2];
	Files->CurrentFrame = -1;
	
	fileType=Files->fileType;
	if(numdim == 1){
	    Files->fileClass=PIO1D_FILES;
	    Files->fileType=SAGE1D_DISK;
	    Files->pdefault=FilesOpenPIO2D;
	    Files->pSetFrame=FilesSetFramePIO;
	    doSage1DStart(Files);
	}else if(numdim == 2){
	    Files->fileClass=PIO2D_FILES;
	    Files->pdefault=FilesOpenPIO2D;
	    Files->pSetFrame=FilesSetFramePIO;
	    Files->fileType=SAGE2D_DISK;
	    doSage2DStart(Files);
	}else if(numdim == 3){
	    Files->fileClass=PIO3D_FILES;
	    Files->fileType=SAGE3D_DISK;
	    Files->pdefault=FilesOpenPIO3D;
	    Files->pSetFrame=FilesSetFramePIO;
	    doSage3DStart(Files);
	}else if(numdim == -1){
	    Files->fileClass=PIOEOS_FILES;
	    Files->fileType=SAGEEOS_DISK;
	    Files->pdefault=FilesOpenPIODUMP;
	    Files->pSetFrame=FilesSetFramePIO;
	}else{
	   sprintf(WarningBuff,"initPIO2D Bad numdim %d\n",numdim);
	   WarningBatch(WarningBuff);
	   return 1;
	}
	
	if(fileType == SAGE_DUMP){
	    Files->pdefault=FilesOpenPIODUMP;
	}
	
		
	return 0;
ErrorOut:
	if(fdata)cFree((char *)fdata);
	if(ldata)cFree((char *)ldata);
	
	sprintf(WarningBuff,"initPIO2D Error ret %d\n",ret);
	WarningBatch(WarningBuff);
	
	if(Files->ImageCount > 0)return 0;
	freeFileList(Files);
	return 1;
}
static int FilesSetFramePIO(long CurrentFrame,struct FileInfo2 *Files)
{
	struct FilePIOInfo *pio;
	struct SetFrameData *s;

	if(!Files)return 1;
	pio=&Files->pioData;
	s = &Files->SetData;
	s->SetFrame= -1;

	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;

	s->name=Files->ImageNames[CurrentFrame];

	s->data= Files->dataSave;
	s->SetFrame= CurrentFrame;
	s->type= Files->fileClass;
	if(Files->PIO2DList){
	    s->pioTime=Files->PIO2DList[CurrentFrame].time;
	    Files->pioTime=s->pioTime;
	    s->numcell=Files->PIO2DList[CurrentFrame].numcell;
	    pio->numcell=Files->PIO2DList[CurrentFrame].numcell;
	    pio->sage=&Files->PIO2DList[CurrentFrame];
	}else{
	    s->pioTime=0;
	    s->numcell=0;
	    pio->numcell=0;
	    pio->sage=NULL;
	}
	return 0;
}
static int CheckPIO2DSpace(struct FileInfo2 *Files)
{
	struct PIO *PIO2DList;
        long ImageMax;
	
	if(!Files)return 1;

	if(Files->ImageCount+1 < Files->ImageMax)return 0;

	if(CheckNames(Files))return 1;

	ImageMax = Files->ImageMax+IMAGE_INCREMENT;

	PIO2DList=NULL;
	if(Files->PIO2DList){
	    PIO2DList=(struct PIO *)cRealloc((char *)Files->PIO2DList,ImageMax*sizeof(struct PIO),8761);
	    if(!PIO2DList){
	        sprintf(WarningBuff,"CheckPIO2DSpace out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)&PIO2DList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct PIO));
	}else{
	    PIO2DList=(struct PIO *)cMalloc(ImageMax*sizeof(struct PIO),8452);
	    if(!PIO2DList){
	        sprintf(WarningBuff,"CheckPIO2DSpace out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)PIO2DList,ImageMax*sizeof(struct PIO));
	}

	Files->ImageMax=ImageMax;
	Files->PIO2DList=PIO2DList;

	return 0;
ErrorOut:
	if(PIO2DList){
	    Files->PIO2DList=PIO2DList;
	}
	return 1;
}
int SageGetFileItem(struct FileInfo2 *Files,char *dataName,long plotItem,
                double **data,long *dataLength,long CurrentFrame)
{
	struct SetFrameData *s;
	struct PIO *sage;
	int dataType;
	int ret;
	
	if(!Files || !dataName || !data || !dataLength)return 1;
	if(!Files->PIO2DList){
	    if(Files->PIOBlkList){
			return SageGetFileItemFnt(Files,dataName,plotItem,
					                  data,dataLength,CurrentFrame);
		}
		return 1;
	}
	s = &Files->SetData;
	
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	sage = &Files->PIO2DList[CurrentFrame];
	
	if(!sage)return 1;
	
	s->pioTime=sage->time;

	if(SetFileToRead8(CurrentFrame,Files))return 1;
	
	sage->in=Files->in8;
	
	ret=SageGetItem(sage,dataName,plotItem,data,dataLength,&dataType);
	
	sage->in=NULL;
	
	return ret;
}
int CheckForWrite(struct file8 **in8)
{
	char name[2048];
	
	if(!in8 || !*in8)return 1;
	
	if(strcmp((*in8)->mode,"rb+")){
	    strncpy(name,(*in8)->name,sizeof(name));
	    fclose8(*in8);
	    *in8=fopen8(name,"rb+");
	    if(!*in8)return 1;
	}
	
	return 0;
}
int SageSetFileItem(struct FileInfo2 *Files,char *dataName,long plotItem,
                void *data,long dataLength,long CurrentFrame)
{
	struct SetFrameData *s;
	struct PIO *sage;
	int ret;
	
	if(!Files || !dataName || !data || !dataLength)return 1;
	if(!Files->PIO2DList)return 1;
	s = &Files->SetData;
	
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	sage = &Files->PIO2DList[CurrentFrame];
	
	if(!sage)return 1;
	
	if(CheckForWrite(&Files->in8))return 1;

	if(SetFileToRead8(CurrentFrame,Files))return 1;
	
	sage->in=Files->in8;

	ret=SageSetData(sage,dataName,plotItem,data,dataLength);

	sage->in=NULL;
	
	return ret;
}
int SageCheckName(struct PIO *sage,char *dataName,int plotItem)
{
	char name[1024];
	char *fp,*np;
	int n;

	if(!sage || !dataName)return 0;
	
	np=strrchr((char *)dataName,'(');

	for(n=0;n<sage->sio_num;++n){
	    if(np){
		    if(!strcmp(sage->data[n].sio_names,dataName)){
		    	return 1;
		    }
	    }else{
		    mstrncpy(name,sage->data[n].sio_names,1024);
		    fp=strrchr((char *)name,'(');
		    if(fp){
		        *fp = 0;
		    }

		    if(!strcmp(name,dataName)){
		        if(plotItem == sage->data[n].sio_indexs)return 1;
		    }
	    }
	}
	return 0;
}

int SageGetItem(struct PIO *sage,char *dataName,long plotItem,double **data,long *dataLength,int *dataType)
{
	int index;
	
	if(!sage || !dataName)return 1;

	if(isDerived(sage,dataName,&index)){
	    return SageGetDerived(sage,dataName,index,data,dataLength,dataType);
	}

	return SageGetData(sage,dataName,(int)plotItem,data,dataLength,dataType);

}
int isDerived(struct PIO *sage,char *dataName,int *index)
{
	char name[256],*fp;
	long int n;

	if(!sage || !dataName)return 0;

	*index = 0;


	if(!strcmp(dataName,"density")){
	    *index=(int)(-(1));
	}else if(!strcmp(dataName,"rho")){
	    *index=(int)(-(1));
	}else if(!strcmp(dataName,"speed")){
	    *index=(int)(-(2));
	}else if(!strcmp(dataName,"cell_number")){
	    *index=(int)(-(3));
	}else if(!strcmp(dataName,"xdt")){
	    *index=(int)(-(4));
	}else if(!strcmp(dataName,"ydt")){
	    *index=(int)(-(5));
	}else if(!strcmp(dataName,"zdt")){
	    *index=(int)(-(6));
	}else if(!strcmp(dataName,"sj2")){
	    *index=(int)(-(7));
	}else if(!strcmp(dataName,"txx")){
	    *index=(int)(-(8));
	}else if(!strcmp(dataName,"tyy")){
	    *index=(int)(-(9));
	}else if(!strcmp(dataName,"tzz")){
	    *index=(int)(-(10));
	}else if(!strcmp(dataName,"ste")){
	    *index=(int)(-(11));
	}else if(!strcmp(dataName,"szz")){
	    *index=(int)(-(12));
	}else{
	    mstrncpy(name,dataName,256);
	    fp=strrchr((char *)name,'(');
	    if(fp){
	        *fp = 0;
	        if(!strcmp(name,"tracer_data")){
	    		*index=(int)(-(14));
	    		return 1;
	        }
		}

	    if(sage->mixed_cell){
	        for(n=0;n<sage->nummat;++n){
	            char buff[256];
	            sprintf(buff,"mat_%ld",n+1);
	            if(!strcmp(buff,dataName)){
	                *index=(int)(-(5*n+2000));
	                break;
	            }
	            sprintf(buff,"mass_%ld",n+1);
	            if(!strcmp(buff,dataName)){
	                *index=(int)(-(5*n+2001));
	                break;
	            }
	            sprintf(buff,"eng_%ld",n+1);
	            if(!strcmp(buff,dataName)){
	                *index=(int)(-(5*n+2002));
	                break;
	            }
	            sprintf(buff,"fvol_%ld",n+1);
	            if(!strcmp(buff,dataName)){
	                *index=(int)(-(5*n+2003));
	                break;
	            }
	            sprintf(buff,"density_%ld",n+1);
	            if(!strcmp(buff,dataName)){
	                *index=(int)(-(5*n+2004));
	                break;
	            }
	        }
	    }
	}
	    
	if(*index){
	    return 1;
	}
	return 0;
}
int SageGetDerived(struct PIO *sage,char *dataName,int plotItem,double **data,long *dataLength,int *dataType)
{
	char name[256],*fp;
	double *mass,*volume;
	long mass_length,volume_length;
	int mass_type,volume_type;
	double *mixed_cells,*num_frac_mat,*dataf;
	double *chunk_mat,*chunk_mass,*chunk_eng,*chunk_vol;
	long mixed_length,num_frac_mat_length;
	int mixed_type,num_frac_mat_type;
	int ret;
	int n;
	long l,m,k,item;
	int nv;

	if(!sage || !dataName || !data || !dataLength || !dataType)return 1;

	ret=1;

	mixed_cells=NULL;
	num_frac_mat=NULL;
	mass=NULL;
	volume=NULL;
	chunk_mat=NULL;
	chunk_mass=NULL;
	chunk_eng=NULL;
	chunk_vol=NULL;

	*data=(double *)cMalloc(sizeof(double)*sage->numcell,7018);
	if(!*data){ret=4;goto ErrorOut;};

	dataf=(double *)*data;
	zerol((char *)dataf,sizeof(double)*sage->numcell);

	plotItem = - plotItem;
	
		
	if(plotItem < 20){
	    item=plotItem;
	    if(item == 1){
			if(SageGetItem(sage,"mass",0,(&mass),
		               &mass_length,&mass_type))goto ErrorOut;
			if(SageGetItem(sage,"vcell",0,(&volume),
		               &volume_length,&volume_type))goto ErrorOut;
			for(l=0;l<sage->numcell;++l){
	           if(mass[l] > 0 && volume[l] > 0){
	               dataf[l]=mass[l]/volume[l];
	           }else{
	               dataf[l]=0;
	           }
			}
	    }else if(item == 2){	    
	 	    if(SageCheckName(sage,"cell_velocity",0)){
				for(k=0;k<sage->numdim;++k){
					if(SageGetItem(sage,"cell_velocity",(int)k,(&mass),
					           &mass_length,&mass_type))goto ErrorOut;
					for(l=0;l<sage->numcell;++l){
						dataf[l] += (mass[l]*mass[l]);
					}
					if(mass)cFree((char *)mass);
					mass = NULL;
				}
			}else if(SageCheckName(sage,"cell_momentum",1)){
						
				if(SageGetItem(sage,"mass",0,(&mass),
			               &mass_length,&mass_type))goto ErrorOut;
			               			               
				for(k=0;k<sage->numdim;++k){		              			               				               
					if(SageGetItem(sage,"cell_momentum",(int)(k+1),(&volume),
					           &volume_length,&volume_type))goto ErrorOut;
					for(l=0;l<sage->numcell;++l){
					    double vel;
						if(mass[l] > 0){
						   vel=volume[l]/mass[l];
						   dataf[l] += (vel*vel);
						}
					}
					if(volume)cFree((char *)volume);
					volume = NULL;				
				}								
			}else{
			    goto ErrorOut;
			}
			for(l=0;l<sage->numcell;++l){
				dataf[l]=sqrt(dataf[l]);
			}
	    }else if(item == 3){
			for(l=0;l<sage->numcell;++l){
			    dataf[l]=(double)(l+1);
			}
	    }else if(item == 4){  /* xdt */	    	    
	 	    if(SageCheckName(sage,"cell_velocity(1)",0)){
				if(SageGetItem(sage,"cell_velocity(1)",(int)0,(&mass),
				           &mass_length,&mass_type))goto ErrorOut;
				for(l=0;l<sage->numcell;++l){
					dataf[l] = mass[l];
				}
			}else if(SageCheckName(sage,"cell_momentum",1)){						
				if(SageGetItem(sage,"mass",0,(&mass),
				       &mass_length,&mass_type))goto ErrorOut;				       			               
				if(SageGetItem(sage,"cell_momentum",(int)1,(&volume),
				           &volume_length,&volume_type))goto ErrorOut;
				for(l=0;l<sage->numcell;++l){
				    double vel;
					if(mass[l] > 0){
					   vel=volume[l]/mass[l];
					   dataf[l] = vel;
					}
				}
			}else{
			    goto ErrorOut;
			}
	 	}else if(item == 5){  /* ydt */
	 	    if(SageCheckName(sage,"cell_velocity(2)",1)){
				if(SageGetItem(sage,"cell_velocity(2)",(int)1,(&mass),
				           &mass_length,&mass_type))goto ErrorOut;
				for(l=0;l<sage->numcell;++l){
					dataf[l] = mass[l];
				}
			}else if(SageCheckName(sage,"cell_momentum",2)){						
				if(SageGetItem(sage,"mass",0,(&mass),
				       &mass_length,&mass_type))goto ErrorOut;				       			               
				if(SageGetItem(sage,"cell_momentum",(int)2,(&volume),
				           &volume_length,&volume_type))goto ErrorOut;
				for(l=0;l<sage->numcell;++l){
				    double vel;
					if(mass[l] > 0){
					   vel=volume[l]/mass[l];
					   dataf[l] = vel;
					}
				}
			}else{
			    goto ErrorOut;
			}
	    }else if(item == 6){  /* zdt */
	 	    if(SageCheckName(sage,"cell_velocity(3)",2)){
				if(SageGetItem(sage,"cell_velocity(3)",(int)2,(&mass),
				           &mass_length,&mass_type))goto ErrorOut;
				for(l=0;l<sage->numcell;++l){
					dataf[l] = mass[l];
				}
			}else if(SageCheckName(sage,"cell_momentum",3)){						
				if(SageGetItem(sage,"mass",0,(&mass),
				       &mass_length,&mass_type))goto ErrorOut;				       			               
				if(SageGetItem(sage,"cell_momentum",(int)3,(&volume),
				           &volume_length,&volume_type))goto ErrorOut;
				for(l=0;l<sage->numcell;++l){
				    double vel;
					if(mass[l] > 0){
					   vel=volume[l]/mass[l];
					   dataf[l] = vel;
					}
				}
			}else{
			    goto ErrorOut;
			}
	    }else if(item == 7){  /* sj2 */
			if(SageGetItem(sage,"sxx",0,(&mass),
			       &mass_length,&mass_type))goto ErrorOut;
			if(SageGetItem(sage,"syy",0,(&volume),
			       &volume_length,&volume_type))goto ErrorOut;
			for(l=0;l<sage->numcell;++l){
			    dataf[l] += 3.0*(volume[l]*volume[l]+mass[l]*mass[l]+volume[l]*mass[l]);
			}
			if(volume)cFree((char *)volume);
			volume = NULL;				
			if(sage->numdim > 1){			
				if(SageGetItem(sage,"sxy",0,(&volume),
				       &volume_length,&volume_type))goto ErrorOut;
				for(l=0;l<sage->numcell;++l){
				    dataf[l] += 3.0*(volume[l]*volume[l]);
				}			
			}
			if(volume)cFree((char *)volume);
			volume = NULL;				
			if(mass)cFree((char *)mass);
			mass = NULL;				
			if(sage->numdim > 2){			
				if(SageGetItem(sage,"syz",0,(&mass),
				       &mass_length,&mass_type))goto ErrorOut;
				if(SageGetItem(sage,"szx",0,(&volume),
				       &volume_length,&volume_type))goto ErrorOut;
				for(l=0;l<sage->numcell;++l){
				    dataf[l] += 3.0*(volume[l]*volume[l]+mass[l]*mass[l]);
				}
			}		
			for(l=0;l<sage->numcell;++l){
				dataf[l]=sqrt(dataf[l]);
			}
	    }else if(item == 8){  /*txx */
			if(SageGetItem(sage,"sxx",0,(&mass),
			       &mass_length,&mass_type))goto ErrorOut;
			if(SageGetItem(sage,"pres",0,(&volume),
			       &volume_length,&volume_type))goto ErrorOut;
			for(l=0;l<sage->numcell;++l){
				dataf[l]=mass[l]-volume[l];
			}
	    }else if(item == 9){  /*tyy */
			if(SageGetItem(sage,"syy",0,(&mass),
			       &mass_length,&mass_type))goto ErrorOut;
			if(SageGetItem(sage,"pres",0,(&volume),
			       &volume_length,&volume_type))goto ErrorOut;
			for(l=0;l<sage->numcell;++l){
				dataf[l]=mass[l]-volume[l];
			}
	    }else if(item == 10){  /*tzz */
			if(SageGetItem(sage,"syy",0,(&mass),
			       &mass_length,&mass_type))goto ErrorOut;
			if(SageGetItem(sage,"pres",0,(&volume),
			       &volume_length,&volume_type))goto ErrorOut;
			for(l=0;l<sage->numcell;++l){
				dataf[l]= -mass[l] -volume[l];
			}
			
			if(volume)cFree((char *)volume);
			volume = NULL;	
						
			if(SageGetItem(sage,"sxx",0,(&volume),
			       &volume_length,&volume_type))goto ErrorOut;
			for(l=0;l<sage->numcell;++l){
			    dataf[l] -= volume[l];
			}
	    }else if(item == 11){  /*ste */
			if(SageGetItem(sage,"cell_energy",0,(&mass),
		               &mass_length,&mass_type))goto ErrorOut;
			if(SageGetItem(sage,"mass",0,(&volume),
		               &volume_length,&volume_type))goto ErrorOut;
			for(l=0;l<sage->numcell;++l){
	           if(volume[l] > 0){
	               dataf[l]=mass[l]/volume[l];
	           }else{
	               dataf[l]=0;
	           }
			}
	    }else if(item == 12){  /*szz */
			if(SageGetItem(sage,"syy",0,(&mass),
			       &mass_length,&mass_type))goto ErrorOut;
			if(SageGetItem(sage,"sxx",0,(&volume),
			       &volume_length,&volume_type))goto ErrorOut;
			for(l=0;l<sage->numcell;++l){
				dataf[l]= -mass[l] -volume[l];
			}
			
	    }else if(item == 14){  /* tracer_data */	    
			if(SageGetItem(sage,"tracer_data",0,(&mass),
			       &mass_length,&mass_type))goto ErrorOut;
			       
		    mstrncpy(name,dataName,256);
		    fp=strrchr((char *)name,'(');
		    if(fp){
		        ++fp;
		        nv=(int)atoi(fp);
			}else{
				goto ErrorOut;	    
			}
			n=0;  
			for(l=0;l<mass_length;l += sage->tracer_words_per_record){
				dataf[n++]= mass[nv+l];
			}
			*dataType = SAGE_DOUBLE;
			*dataLength=n;
			ret=0;
			goto ErrorOut;	    
	    }else{
	        sprintf(WarningBuff,"SageGetDerived  Name %s plotItem %d Out of Range nummat %ld mixed_cell %d\n",
	                           dataName,plotItem,sage->nummat,sage->mixed_cell);
	        WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    goto okOut;
	}

	if(plotItem < 4000){
	
		plotItem -= 2000;

		if(sage->mixed_cell == 2){
		    int sw,index;

		    sw=plotItem % 5;
		    index=(plotItem/5);
		    
		    switch(sw){
		       case 0:
				if(SageGetItem(sage,"frac_mass",index+1,(&mass),
			               &mass_length,&mass_type))goto ErrorOut;
				if(SageGetItem(sage,"frac_vol",index,(&volume),
			               &volume_length,&volume_type))goto ErrorOut;
				for(l=0;l<sage->numcell;++l){
			           if(mass[l] > 0 && volume[l] > 0){
			               dataf[l]=index+1;
			           }else{
			               dataf[l]=0;
			           }
				}
		           break;
		       case 1:
				if(SageGetItem(sage,"frac_mass",index+1,(&mass),
					      &mass_length,&mass_type))goto ErrorOut;
				for(l=0;l<sage->numcell;++l){
			             dataf[l]=mass[l];
				}
		           break;
		       case 2:
					if(SageGetItem(sage,"frac_eng",index+1,(&mass),
						      &mass_length,&mass_type))goto ErrorOut;
					for(l=0;l<sage->numcell;++l){
				    	dataf[l]=mass[l];
					}
		           break;
		       case 3:
					if(SageGetItem(sage,"frac_vol",index+1,(&mass),
					           &mass_length,&mass_type))goto ErrorOut;
					if(SageGetItem(sage,"vcell",0,(&volume),
					           &volume_length,&volume_type))goto ErrorOut;
					for(l=0;l<sage->numcell;++l){
						if(mass[l] > 0 && volume[l] > 0){
						   dataf[l]=mass[l]/volume[l];
						}else{
						   dataf[l]=0;
						}
					}
		           break;
		           
		       case 4:
					if(SageGetItem(sage,"frac_mass",index+1,(&mass),
					           &mass_length,&mass_type))goto ErrorOut;
					if(SageGetItem(sage,"vcell",0,(&volume),
					           &volume_length,&volume_type))goto ErrorOut;
					for(l=0;l<sage->numcell;++l){
						if(mass[l] > 0 && volume[l] > 0){
						   dataf[l]=mass[l]/volume[l];
						}else{
						   dataf[l]=0;
						}
					}
		           break;
		    }
		}else if(sage->mixed_cell == 3){
		    long size_chunk;
		    int sw,index;

		    sw=plotItem % 5;
		    index=(plotItem/5);

		    if(SageGetItem(sage,"chunk_nummat",0,(&num_frac_mat),
		               &num_frac_mat_length,&num_frac_mat_type))goto ErrorOut;

	            size_chunk=0;

		    for(l=0;l<sage->numcell;++l){
				size_chunk += (long)num_frac_mat[l];
		     }

		    if(SageGetItem(sage,"chunk_mat",0,(&chunk_mat),
		               &num_frac_mat_length,&num_frac_mat_type))goto ErrorOut;

		    if(size_chunk != num_frac_mat_length){
				ret=5;
		        goto ErrorOut;
		    }

		    switch(sw){
		       case 0:
				if(SageGetItem(sage,"chunk_mass",0,(&chunk_mass),
			               &mass_length,&mass_type))goto ErrorOut;
				if(SageGetItem(sage,"chunk_vol",0,(&chunk_vol),
			               &volume_length,&volume_type))goto ErrorOut;
				k=0;
				for(l=0;l<sage->numcell;++l){
				    for(m=0;m<(long)(num_frac_mat[l]);++m){
				        n=((int)chunk_mat[k])-1;
				        if(index == n){
				           if(chunk_mass[k] > 0 && chunk_vol[k] > 0){
				               dataf[l]=index+1;
				           }else{
				               dataf[l]=0;
				           }
			            }
			            ++k;
				    }
				}
		        break;
		       case 1:
				if(SageGetItem(sage,"chunk_mass",0,(&chunk_mass),
					      &mass_length,&mass_type))goto ErrorOut;

				k=0;
				for(l=0;l<sage->numcell;++l){
				    for(m=0;m<(long)(num_frac_mat[l]);++m){
				        n=((int)chunk_mat[k])-1;
				        if(index == n){
				            dataf[l]=chunk_mass[k];
			            }
			            ++k;
				    }
				}

		        break;
		       case 2:
				if(SageGetItem(sage,"chunk_eng",0,(&chunk_eng),
					      &mass_length,&mass_type))goto ErrorOut;

				k=0;
				for(l=0;l<sage->numcell;++l){
				    for(m=0;m<(long)(num_frac_mat[l]);++m){
				        n=((int)chunk_mat[k])-1;
				        if(index == n){
				            dataf[l]=chunk_eng[k];
			            }
			            ++k;
				    }
				}

		        break;
		       case 3:
				if(SageGetItem(sage,"chunk_vol",0,(&chunk_vol),
					      &mass_length,&mass_type))goto ErrorOut;
				if(SageGetItem(sage,"vcell",0,(&volume),
			               &volume_length,&volume_type))goto ErrorOut;
				k=0;
				for(l=0;l<sage->numcell;++l){
				    for(m=0;m<(long)(num_frac_mat[l]);++m){
				        n=((int)chunk_mat[k])-1;
				        if(index == n){
				           if(chunk_vol[k] > 0 && volume[l] > 0){
				               dataf[l]=chunk_vol[k]/volume[l];
				           }else{
				               dataf[l]=0;
				           }
		                }
		                ++k;
				    }
				}

		        break;
		           	           
		       case 4:
				if(SageGetItem(sage,"chunk_mass",0,(&chunk_mass),
			               &mass_length,&mass_type))goto ErrorOut;
				if(SageGetItem(sage,"vcell",0,(&volume),
			               &volume_length,&volume_type))goto ErrorOut;
				k=0;
				for(l=0;l<sage->numcell;++l){
				    for(m=0;m<(long)(num_frac_mat[l]);++m){
				        n=((int)chunk_mat[k])-1;
				        if(index == n){
				           if(chunk_mass[k] > 0 && volume[l] > 0){
				               dataf[l]=chunk_mass[k]/volume[l];
				           }else{
				               dataf[l]=0;
				           }
		                }
		                ++k;
				    }
				}

		        break;
		    }
		
		}else if(sage->mixed_cell == 1){

		    if(SageGetItem(sage,"mixed_cell",0,(&mixed_cells),
		               &mixed_length,&mixed_type))goto ErrorOut;
		    if(SageGetItem(sage,"num_frac_mat",0,(&num_frac_mat),
		               &num_frac_mat_length,&num_frac_mat_type))goto ErrorOut;
		    if(SageGetItem(sage,"vcell",0,(&volume),
		               &volume_length,&volume_type))goto ErrorOut;

		    k=0;
		    for(l=0;l<sage->numcell;++l){
		        for(m=0;m<(long)(num_frac_mat[l]);++m){
		            int sw;
		            n=((int)mixed_cells[k])-1;
		            if(plotItem/5 == n){
		                sw= plotItem % 5;
		                if(sw == 0){
		                   dataf[l]=n+1;
		                }else if(sw == 3){
		                   if(mixed_cells[k+3] > 0){
		                       dataf[l]=mixed_cells[k+3]/volume[l];
		                   }else{
		                       dataf[l]=0;
		                   }
		                }else if(sw == 4){
		                   if(mixed_cells[k+1] > 0 && mixed_cells[k+3] > 0){
		                       dataf[l]=mixed_cells[k+1]/volume[l];
		                   }else{
		                       dataf[l]=0;
		                   }
		                }else{
		                   dataf[l]=mixed_cells[k+sw];
		                }
	                }
	                k=k+4;
		        }
		    }
		}
		goto okOut;
		
	}
	
okOut:
	*dataType = SAGE_DOUBLE;
	*dataLength=sage->numcell;
	ret=0;
ErrorOut:
	if(mixed_cells)cFree((char *)mixed_cells);
	if(num_frac_mat)cFree((char *)num_frac_mat);
	if(mass)cFree((char *)mass);
	if(volume)cFree((char *)volume);
	if(chunk_mat)cFree((char *)chunk_mat);
	if(chunk_mass)cFree((char *)chunk_mass);
	if(chunk_eng)cFree((char *)chunk_eng);
	if(chunk_vol)cFree((char *)chunk_vol);
	return ret;
}
int SageGetData(struct PIO *sage,char *dataName,int index,double **data,long *dataLength,int *dataType)
{
	char name[1024];
	char *fp,*np;
	int ret;
	int n;

	if(!sage || !dataName || !data || !dataLength || !dataType)return 1;
	if(!sage->in || !sage->data)return 1;	

	ret=2;

	np=strrchr((char *)dataName,'(');

    for(n=0;n<sage->sio_num;++n){
/*       
	    fprintf(stderr,"%s ",sage->data[n].sio_names);
	    fprintf(stderr,"%d %d %lld %d\n",sage->data[n].sio_indexs,sage->data[n].sio_lengths,
				    sage->data[n].sio_positions,sage->data[n].sio_types);
*/

	    if(np){
		    if(strcmp(sage->data[n].sio_names,dataName)){
		    	continue;
		    }
	    }else{
		    mstrncpy(name,sage->data[n].sio_names,1024);
		    fp=strrchr((char *)name,'(');
		    if(fp){
		        *fp = 0;
		    }

		    if(!strcmp(name,dataName)){
		    	if(index != sage->data[n].sio_indexs)continue;
		    }else{
		        continue;
		    }
	    }

		if(fseek8(sage->in,WORDSIZE*sage->data[n].sio_positions,0)){ret=3;goto ErrorOut;};
		*data=(double *)cMalloc(WORDSIZE*sage->data[n].sio_lengths,1018);
		if(!*data){ret=4;goto ErrorOut;};
		if(fget8(sage->in,(char *)*data,WORDSIZE*sage->data[n].sio_lengths)){ret=5;goto ErrorOut;};
		*dataLength=sage->data[n].sio_lengths;
		*dataType=SAGE_DOUBLE;
        SageTranslateDoubles(sage,(double *)*data,*dataLength);
		ret = 0;
		break;
	}
ErrorOut:
	return ret;
}
int SageSetData(struct PIO *sage,char *dataName,int index,void *data,long dataLength)
{
	char name[1024];
	char *fp,*np;
	int ret;
	int n;

	if(!sage || !dataName || !data || !dataLength)return 1;
	if(!sage->in || !sage->data)return 1;	

	ret=2;

	np=strrchr((char *)dataName,'(');

    for(n=0;n<sage->sio_num;++n){
/*       
	    fprintf(stderr,"%s ",sage->data[n].sio_names);
	    fprintf(stderr,"%d %d %lld %d\n",sage->data[n].sio_indexs,sage->data[n].sio_lengths,
				    sage->data[n].sio_positions,sage->data[n].sio_types);
*/

	    if(np){
		    if(strcmp(sage->data[n].sio_names,dataName)){
		    	continue;
		    }
	    }else{
		    mstrncpy(name,sage->data[n].sio_names,1024);
		    fp=strrchr((char *)name,'(');
		    if(fp){
		        *fp = 0;
		    }

		    if(!strcmp(name,dataName)){
		    	if(index != sage->data[n].sio_indexs)continue;
		    }else{
		        continue;
		    }
	    }

		if(fseek8(sage->in,WORDSIZE*sage->data[n].sio_positions,0)){ret=3;goto ErrorOut;};
		
        /* SageTranslateDoubles(sage,(double *)data,*dataLength); */
		
		if(fput8(sage->in,(char *)data,WORDSIZE*sage->data[n].sio_lengths)){ret=5;goto ErrorOut;};
		
        /* SageTranslateDoubles(sage,(double *)data,*dataLength); */
		ret = 0;
		break;
	}
ErrorOut:
	return ret;
}
int sageClose(struct PIO *sage,int flag)
{
	int n;

	if(!sage)return 1;

	if(sage->in)fclose8(sage->in);
	sage->in=NULL;
	if(sage->data){
		for(n=0;n<sage->sio_num;++n){
			if(sage->data[n].sio_names)cFree((char *)sage->data[n].sio_names);
			sage->data[n].sio_names=NULL;
	    }
	    if(sage->data)cFree((char *)sage->data);
	    sage->data=NULL;
	}
	
	if(sage->strength_nm){
		cFree((char *)sage->strength_nm);	
		sage->strength_nm=NULL;
	}
	
	if(sage && flag){
	    cFree((char *)sage);
        sage=NULL;
    }

	return 0;
}
int TestDouble(double *one,double *two)
{
	unsigned char *ione;
	unsigned char *itwo;
	int n;

	if(!one || !two)return 1;

	ione=(unsigned char *)one;
	itwo=(unsigned char *)two;

	for(n=0;n<8;++n){
	    if(ione[n] != itwo[n])return 1;
	}
	return 0;
}
struct PIO *sageHeader(char *filename)
{
	double sio_header[SIO_HEADER_LENGTH];
	double sio_scratch[SIO_INDEX_LENGTH];
	char sio_id[9],sio_dandt[17];
	double two = 2.0;
	FILE8 *in;
	int ret;
	int n;

	struct PIO *sage;

	struct PIOData *data;

	int name_length;
	int index_length;
	int sio_num;
	INT8_64 sio_position;
	int len_scratch;

	char *names,*name;
    double *amhc_i;
    double *amhc_r8;
	double *controller_r8;
	int itable;

	if(!filename)return NULL;

	ret=1;
	in=NULL;
	sio_num=0;
	sage=NULL;
	data=NULL;
	names=NULL;
	amhc_i=NULL;
	amhc_r8=NULL;
	itable=FALSE;
	
    controller_r8 = NULL;
    if(!(in=fopen8(filename,"rb"))){
         return NULL;
    }

	sage=(struct PIO *)cMalloc(sizeof(struct PIO),1999);

	if(!sage){ret=1;goto ErrorOut;};

	zerol((char *)sage,sizeof(struct PIO));

	sage->in=in;

	if(fget8(in,(char *)sio_header,WORDSIZE*SIO_HEADER_LENGTH)){ret=2;goto ErrorOut;};

	if(sio_get_string(sio_id,8,(char *)&sio_header[0],&len_scratch)){ret=3;goto ErrorOut;};

	if(strcmp("pio_file",sio_id)){ret=4;goto ErrorOut;};

	if(sio_get_string(sio_dandt,16,(char *)&sio_header[6],&len_scratch)){ret=5;goto ErrorOut;};


	SageTranslateDoubles(sage,&sio_header[1],1L);
	if(TestDouble(&sio_header[1],&two)){
		sage->SageFlip=TRUE;
		SageTranslateDoubles(sage,&sio_header[1],1L);
		if(TestDouble(&sio_header[1],&two)){
		  ret=6;goto ErrorOut;
		}
	}


	SageTranslateDoubles(sage,&sio_header[3],1L);
	SageTranslateDoubles(sage,&sio_header[5],1L);
	SageTranslateDoubles(sage,&sio_header[8],2L);

    name_length=(int)sio_header[3];
    index_length=(int)sio_header[5];
    sio_num=(int)sio_header[8];
    sio_position=(INT8_64)sio_header[9];

/*
        fprintf(stderr,"sio_num %d sio_position %lld\n",(int)sio_num,sio_position);
*/
	sage->sio_num=sio_num;

	if(index_length > SIO_INDEX_LENGTH){
	    ret=6;goto ErrorOut;
	}

	names=(char *)cMalloc(name_length+2,7878);

	if(!names){
	    ret=7;goto ErrorOut;
	}


	data=(struct PIOData *)cMalloc(sio_num*sizeof(struct PIOData),8900);
	if(!data){
	    ret=8;goto ErrorOut;
	}

	zerol((char *)data,sio_num*sizeof(struct PIOData));

	sage->data=data;

	for(n=0;n<sio_num;++n){

	    if(fseek8(in,WORDSIZE*sio_position,0)){ret=9;goto ErrorOut;};

	    if(fget8(in,(char *)sio_scratch,WORDSIZE*index_length)){ret=10;goto ErrorOut;};

	    sio_position=sio_position+index_length;

	    if(sio_get_string(names,name_length,(char *)sio_scratch,&len_scratch)){ret=11;goto ErrorOut;};

	    data[n].sio_names=strsave(names,6463);
	    if(!data[n].sio_names){
	        ret=12;goto ErrorOut;
	    }

	    clip_names(data[n].sio_names);
	    
	    name=data[n].sio_names;
	    
	    if(!strcmp(name,"mixed_cell")){
	        sage->mixed_cell=TRUE;
	    }else if(!strcmp(name,"frac_mass")){
	        sage->mixed_cell=2;
	    }else if(!strcmp(name,"chunk_nummat")){
	        sage->mixed_cell=3;
	    }else if(!strcmp(name,"sxx")){
	        sage->strength=TRUE;
	    }else if(!strcmp(name,"strength_option")){
	        sage->strength_option=TRUE;
	    }else if(!strcmp(name,"table_type")){
			itable=TRUE;
	    }else if(!strcmp(name,"tracer_num_pnts")){
	        sage->tracer_num_pnts=TRUE;
	    }


	    SageTranslateDoubles(sage,&sio_scratch[len_scratch],4L);
	    data[n].sio_indexs=(int)sio_scratch[len_scratch];
	    data[n].sio_lengths=(int)sio_scratch[len_scratch+1];
	    data[n].sio_positions=(INT8_64)sio_scratch[len_scratch+2];
	    /* data[n].sio_types=(int)sio_scratch[len_scratch+3]; Not used and bad in some files */
	    data[n].sio_types=0;
	    if(data[n].sio_indexs != 0){
	       char name[1024];
	       sprintf(name,"%s(%d)",data[n].sio_names,data[n].sio_indexs);
	       if(data[n].sio_names)cFree((char *)data[n].sio_names);
	       data[n].sio_names=strsave(name,6464);
		   if(!data[n].sio_names){
		        ret=14;goto ErrorOut;
		   }
	    }
/*
	    fprintf(stderr,"%s ",data[n].sio_names);
	    fprintf(stderr,"indexs %d lengths %d positions %lld types %d\n",data[n].sio_indexs,data[n].sio_lengths,
				    data[n].sio_positions,data[n].sio_types);
*/
	}
	
	
	if(itable){
	    sage->numdim = -1;
	}else {
		double *value;
	    long dataLength;
	    int dataType;


	    if(SageGetData(sage,"amhc_i",0,&amhc_i,&dataLength,&dataType)){
	        ret=17;goto ErrorOut;
	    }
	    if(dataType != SAGE_DOUBLE){ret=18;goto ErrorOut;}

	    sage->numcell = (long)amhc_i[55-1];
	    sage->nummat  = (long)amhc_i[6-1];
	    sage->numdim  = (long)amhc_i[43-1];

	    sage->ixmax = (long)amhc_i[17-1];
	    sage->iymax  = (long)amhc_i[18-1];
	    sage->izmax  = (long)amhc_i[30-1];

	    if(SageGetData(sage,"amhc_r8",0,&amhc_r8,&dataLength,&dataType)){
	        ret=19;goto ErrorOut;
	    }
	    if(dataType != SAGE_DOUBLE){ret=20;goto ErrorOut;}

	    sage->dxset = amhc_r8[22-1];
	    sage->dyset = amhc_r8[23-1];
	    sage->dzset = amhc_r8[39-1];

	    sage->xzero = amhc_r8[20-1];
	    sage->yzero = amhc_r8[21-1];
	    sage->zzero = amhc_r8[36-1];
	    
        if(SageGetData(sage,"controller_r8",0,&controller_r8,&dataLength,&dataType)){
            ret=18;goto ErrorOut;
        }
        if(dataType != SAGE_DOUBLE){ret=18;goto ErrorOut;}
        
	    sage->time=controller_r8[1-1];
	    
  
	    if(sage->strength_option){
	        value=NULL;
	        if(SageGetData(sage,"strength_option",0,&value,&dataLength,&dataType)){
	            ret=24;goto ErrorOut;
	        }
	        if(dataType != SAGE_DOUBLE){ret=25;goto ErrorOut;}
		    sage->strength_option=(int)value[0];
    		if(value)cFree((char *)value);
	        value=NULL;
	    }
	    
	    if(sage->tracer_num_pnts){
	        value=NULL;
	        if(SageGetData(sage,"tracer_num_pnts",0,&value,&dataLength,&dataType)){
	            ret=25;goto ErrorOut;
	        }
	        if(dataType != SAGE_DOUBLE){ret=25;goto ErrorOut;}
		    sage->tracer_num_pnts=(int)value[0];
    		if(value)cFree((char *)value);
	        value=NULL;

	        if(SageGetData(sage,"tracer_num_vars",0,&value,&dataLength,&dataType)){
	            ret=25;goto ErrorOut;
	        }
	        if(dataType != SAGE_DOUBLE){ret=26;goto ErrorOut;}
		    sage->tracer_num_vars=(int)value[0];
    		if(value)cFree((char *)value);
	        value=NULL;
			sage->tracer_words_per_point=(int)(1+sage->numdim+sage->tracer_num_vars);
		    sage->tracer_words_per_record=sage->tracer_num_pnts*sage->tracer_words_per_point;
	    }

	}
/*
	fprintf(stderr,"name %s numcell %ld nummat %ld numdim %ld sum_numcell %ld mixed_cell %d strength %d\n",
	       name,sage->numcell,sage->nummat,sage->numdim,(long)amhc_i[55-1],sage->mixed_cell,sage->strength);
*/
	qsort((char *)data,sio_num, sizeof(struct PIOData),intcmp);

	ret=0;

ErrorOut:

    if(names)cFree((char *)names);
    if(amhc_i)cFree((char *)amhc_i);
    if(amhc_r8)cFree((char *)amhc_r8);
    if(controller_r8)cFree((char *)controller_r8);

    if(ret){
     	sprintf(WarningBuff,"File \"%s\" Error return %d\n",filename,ret);
     	if(ret != 17)WarningBatch(WarningBuff);
         if(sage){
	          sageClose(sage,1);
         }else{
             if(in)fclose8(in);
         }
         sage=NULL;
    }else{
        if(sage && sage->in){
            fclose8(sage->in);
            sage->in=NULL;
        }
    }

	return sage;
}
static int intcmp(const void *xx, const void *yy)
{
	struct PIOData *x;
	struct PIOData *y;
	int n;

	x=(struct PIOData *)xx;
	y=(struct PIOData *)yy;
	n=strcmp(x->sio_names,y->sio_names);
	if(!n)return x->sio_indexs-y->sio_indexs;
	return n;
}
int clip_names(char *names)
{	
	int n;
	int k;

	if(!names)return 1;

	n=(int)strlen(names);
	for(k=n-1;k > 0;--k){
	    if(names[k] == ' '){
	        names[k]= '\0';
	    }else{
	        break;
	    }
	}

	return 0;

}
int sio_get_string(char *out,int out_len,char *in,int *array_length)
{
	int n;

	if(!out || !in)return 1;

	for(n=0;n<out_len;++n){
	    *out++ = *in++;
	}

	*out = '\0';
/*
	fprintf(stderr,"sio_get_string1 %d %s\n",out_len,&out[-(out_len)]);
*/
        *array_length=1+(out_len-1)/MXBYTE;
	return 0;
}
static int SageTranslateDoubles(struct PIO *sage,double *d,long length)
{
	unsigned char c,*cp;
	long np;


	if(!d || !sage)return 1;

	if(!sage->SageFlip)return 0;

	if(length <= 0)return 1;

	length=length*8;

        cp=(unsigned char *)d;
        for(np=0;np<length;np += 8){
            c=cp[np];
            cp[np]=cp[np+7];
            cp[np+7]=c;
            c=cp[np+1];
            cp[np+1]=cp[np+6];
            cp[np+6]=c;
            c=cp[np+2];
            cp[np+2]=cp[np+5];
            cp[np+5]=c;
            c=cp[np+3];
            cp[np+3]=cp[np+4];
            cp[np+4]=c;
        }
	return 0;
}
int IsPIO(char *namef,int *rank)
{
		struct PIO *sage;
        char name[256];
        FILE *in;
        int n;

        if(!namef || !rank)return 0;

        in=fopen(namef,"r");
        if(in == NULL){
           sprintf(WarningBuff,"FILE %s Could Not Be Opened to Read\n",namef);
           WarningBatch(WarningBuff);
           return 0;
        }
        for(n=0;n<8;++n){

            name[n]=fgetc(in);
        }
        name[8]=0;

        fclose(in);

        if(strcmp(name,"pio_file")){
            return 0;
        }

		sage=sageHeader(namef);
	    if(!(sage)){
	         return 0;
	    }

		*rank=(int)sage->numdim;

		sageClose(sage,1);

        return 1;
}
/*
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
	return (INT8_64)ftell(file->file);
}

int fseek8(FILE8 *file,INT8_64 offset,int from)
{
	if(!file)return -1;
	if(!file->file)return -1;
	return fseek(file->file,(long)offset,from);
}

int fput8(FILE8 *file,char *data,INT8_64 lengthin)
{
	long length;

	if(!data)return -1;
	if(!file)return -1;
	if(!file->file)return -1;
	length=(long)lengthin;
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
	if(fread((char *)data,1,length,file->file) != length){ 
	    return -1;
	}
	return 0;
}
*/
