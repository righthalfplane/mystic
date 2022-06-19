#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define rlsMAX_CHAR_PER_LINE 1024
#define rlsMAX_STRING_CHAR 256

#define rlsWIN 1
#define rlsMAC 2
#define rlsUNIX 3

int rlsCharCount(FILE *fp,int *nChar,int *nEolChar);
int rlsWriteBytes(FILE *out,size_t nBytes,unsigned char *c);
int rlsUngetChar(FILE *in,int c);
int rlsGetChar(FILE *in);
char *strpathname(char *path,char *name);
void *cMalloc(unsigned long r,int add);
int cFree(char *p);
int zerol(char *p,unsigned long length);
int rlsStringAppend(char *string0,int len0,char *string1,int len1,long maxlen);

int intCR=0x0D,intLF=0x0A;
char stringCR[2]={0x0D,0x00},stringLF[2]={0x0A,0x00};

int WarningBatch(char *message);
int WarningBatchFast(char *message);

int TextFileConvert(char *filename,char *path,int outType);


/* ================================================================== */
int TextFileConvert(char *filename,char *path,int outType)
{
	FILE *fp=NULL,*fpout=NULL;
	int ch,charCount,ich,eol,eof,err,ccerr,nChar,nEolChar;
	char message[rlsMAX_STRING_CHAR];
	char *pathfilename;
	char stringLine[rlsMAX_CHAR_PER_LINE+1],*stringFile=NULL;
	int ret;
	
	
	if(!filename || !path)return 1;
	 	
	ret=0;

	err = 0;
	
	pathfilename=strpathname(path,filename);
	if(!pathfilename) return 0;

	/* Some error checking */
	if ((outType!=rlsWIN) && (outType!=rlsMAC) && (outType!=rlsUNIX)) {
		sprintf(message,"TextFileConvert: Unrecognized text file type = %d",outType);
		WarningBatchFast(message);
		goto ErrorOut;
	}
	
	if ((fp = fopen(pathfilename,"rb")) == 0) {
		sprintf(message,"TextFileConvert: Cannot open (for reading) file %s",pathfilename);
		WarningBatchFast(message);
		goto ErrorOut;
	}
	
	/* Count characters and EOL characters */
	ccerr = rlsCharCount(fp,&nChar,&nEolChar);
	fclose(fp);
	fp = NULL;

	if (ccerr==0) {
		sprintf(message,"TextFileConvert: Error counting characters in file %s",pathfilename);
		WarningBatchFast(message);
		goto ErrorOut;
	}
	if (ccerr==-1) {
		sprintf(message,"TextFileConvert: Non-text character found in file %s",pathfilename);
		WarningBatchFast(message);
		goto ErrorOut;
	}
	
	
	/* Allocate space to load entire file into memory including biggest EOL characters */
	stringFile = (char*)cMalloc((nChar+2*nEolChar)*sizeof(char)+1,8574);
	if (!stringFile) {
		sprintf(message,"TextFileConvert: Cannot allocate enough memory for %s",pathfilename);
		WarningBatchFast(message);
		goto ErrorOut;
	}
	if (zerol(stringFile,(nChar+2*nEolChar)*sizeof(char)+1)) {
		sprintf(message,"TextFileConvert: Cannot zero memory for %s",pathfilename);
		WarningBatchFast(message);
		goto ErrorOut;
	}
	
	
	if ((fp = fopen(pathfilename,"rb")) == 0) {
		sprintf(message,"TextFileConvert: Cannot open (for reading) file %s",pathfilename);
		WarningBatchFast(message);
		goto ErrorOut;
	}


	eof = 0;
	err = 0;
	charCount = 0;
	while (!eof) {
		
		eol = 0;
		ich = 0;
		
		/* Loop on characters in line until end-of-line OR end-of-file is reached
		Move file pointer just before next valid character after an EOL occured */
		while (!eol && !eof) {
			
			ch=rlsGetChar(fp);
			
			/* If End-of-file reached */
			if (ch==-2) {
				eof = 1;
				
			/* If a fatal error occured */
			} else if (ch==-1) {
				sprintf(message,"TextFileConvert: Error reading file %s",pathfilename);
				WarningBatchFast(message);
				eof = 1;
				eol = 1;
				err = 1;
				
				/* Else a valid character returned */
			} else {
				
				if (ch==intCR) {
					
					/* Get another character to check for a following LF for WIN standard */
					ch=rlsGetChar(fp);
					
					/* End-of-file reached */
					if (ch==-2) {
						eof = 1;
						eol = 1;
						
						/* Fatal error */
					} else if (ch==-1) {
						sprintf(message,"TextFileConvert: Error reading file %s\n",pathfilename);
						WarningBatch(message);
						eof = 1;
						eol = 1;
						err = 1;
						
						/* A second valid character returned */
					} else {
						eol = 1;

						/* Push anything but a LF (LF is part of CR-LF combo) back on the stream */
						if (ch!=intLF) {
							rlsUngetChar(fp,ch);
						}
					}
					
				} else if (ch==intLF) {
					eol = 1;
					
					/* It's a character */
				} else {
					stringLine[ich]=(char)ch;
					ich++;
					if (ich>rlsMAX_CHAR_PER_LINE) {
						sprintf(message,"TextFileConvert: Line too long. More than %d characters",rlsMAX_CHAR_PER_LINE);
						WarningBatchFast(message);
						ich--; /* reclaim space for null zero */
						eol = 1;
						eof = 1;
						err = 1;
					}
				}
			}
		} /* End loop on !eof && !eol */
	

		if (ich>0) {
			stringLine[ich]='\0';
			rlsStringAppend(stringFile,charCount,stringLine,ich,(long)((nChar+2*nEolChar)*sizeof(char)));
			charCount+=ich;
		}
		
		if (eol) {
			if (outType==rlsWIN) {
				rlsStringAppend(stringFile,charCount,stringCR,1,(long)((nChar+2*nEolChar)*sizeof(char)));
				charCount++;
				rlsStringAppend(stringFile,charCount,stringLF,1,(long)((nChar+2*nEolChar)*sizeof(char)));
				charCount++;
				
			} else if (outType==rlsMAC) {
				rlsStringAppend(stringFile,charCount,stringCR,1,(long)((nChar+2*nEolChar)*sizeof(char)));
				charCount++;
				
			} else {
				rlsStringAppend(stringFile,charCount,stringLF,1,(long)((nChar+2*nEolChar)*sizeof(char)));
				charCount++;
			}
		}
		
	} /* End loop on !eof */
	

	/* Doesn't hurt to check this again */
	if (charCount > nChar+2*nEolChar) {
		sprintf(message,"TextFileConvert: Not enough memory allocated for %s",pathfilename);
		WarningBatchFast(message);
		goto ErrorOut;
	}
	

	/* If there were no errors above, write the file back out */
	if (!err) {
		if ((fpout=fopen(pathfilename,"wb")) == 0) {
			sprintf(message,"TextFileConvert: Cannot open (for writing) file %s",pathfilename);
			WarningBatchFast(message);
			goto ErrorOut;
		}
		if (!rlsWriteBytes(fpout,strlen(stringFile),(unsigned char*)stringFile)) {
			sprintf(message,"TextFileConvert: Cannot write file %s",pathfilename);
			WarningBatchFast(message);
			goto ErrorOut;
		}
	}
	
	ret = 1;
	
ErrorOut:
	
	/* Clean up */
	
	if (stringFile) {
		cFree(stringFile);
		stringFile = NULL;
	}
	
	if (pathfilename) {
		cFree(pathfilename);
		pathfilename = NULL;
	}
	
	if(fp)fclose(fp);
	if(fpout)fclose(fpout);

	return ret;
}

/* ================================================================== */
int rlsStringAppend(char *string0,int len0,char *string1,int len1,long maxlen)
{
	int ic;

	if ((!string0) || (!string1)) return 0;
	if (len0+len1 > maxlen) return 0;

	for(ic=0;ic<len1;ic++) {
		string0[len0+ic] = string1[ic];
	}

	return 1;
}

/* ================================================================== */
int rlsCharCount(FILE *fp,int *nChar,int *nEolChar)
{
	int eof,eol,ch;
	
	if (!fp) return 0;

	*nChar = 0;
	*nEolChar = 0;
	eof = 0;
	while (!eof) {
		
		eol = 0;
		
		/* Loop on characters in line until end-of-line OR end-of-file is reached
		Move file pointer just before next valid character after an EOL occured */
		while (!eol && !eof) {
			
			ch=rlsGetChar(fp);
			
			/* End-of-file reached */
			if (ch==-2) {
				eof = 1;
				
				/* Fatal error */
			} else if (ch==-1) {
				eof = 1;
				eol = 1;
				return 0;
				
				/* Valid character returned */
			} else {
								
				if (ch==intCR) {
					
					(*nEolChar)++;
					
					/* Get another character to check for a following LF for WIN standard */
					ch=rlsGetChar(fp);
					
					/* End-of-file reached */
					if (ch==-2) {
						eof = 1;
						eol = 1;
						
						/* Fatal error */
					} else if (ch==-1) {
						eof = 1;
						eol = 1;
						return 0;
						
						/* A second valid character returned */
					} else {
						eol = 1;
						
						/* Push anything but a LF (LF is part of CR-LF combo) back on the stream */
						if (ch!=intLF) {
							rlsUngetChar(fp,ch);
						}
					}
					
				} else if (ch==intLF) {
					(*nEolChar)++;
					eol = 1;
					
					/* It's a character */
				} else {
					
					/* Check for non-textfile characters */
					if (strchr(" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",ch)==NULL) {
						if (strchr("`~!@#$%^&*()-_=+\t[{]}\\|;:\'\",<.>/\?",ch)==NULL) {
							eof = 1;
							eol = 1;
							return -1;
						}
					}
					(*nChar)++;
				}
			}
		} /* End loop on !eof && !eol */
	}
	return 1;
}
  
/* ================================================================== */
int rlsWriteBytes(FILE *fp,size_t nBytes,unsigned char *c)
{
	if (!fp) return 0;

	if (fwrite(c,sizeof(char),nBytes,fp)!=nBytes) {
		if(ferror(fp)) {
			return 0;
		}
	}
	return 1;
}

/* ================================================================== */
int rlsGetChar(FILE *fp)
{
	int c;

	if (!fp) return -2;
	
	c = getc(fp);
	if(ferror(fp)) {
		return -1;
	}
	if (feof(fp)) {
		return -2;
	}
	return c;
}

/* ================================================================== */
int rlsUngetChar(FILE *fp,int c)
{
	int cunget;
	
	if (!fp) return -1;

	/* catch an EOF first because it is used as an error flag later in ungetc */
	if (c==EOF) return EOF;
	
	cunget=ungetc(c,fp);
	if (cunget==EOF) {  /* error flag */
		return -1;
	}
	return cunget;
}
