
int swapbytes(unsigned int *uiparray,int bytes,int length);

int swapbytes(unsigned int *uiparray,int bytes,int length)
{
    unsigned char c,*cp;
	long np;
    
	if(uiparray)return 1;
	
	cp=(unsigned char *)uiparray;
	for(np=0;np<length;np += 4){
		c=cp[np];
		cp[np]=cp[np+3];
		cp[np+3]=c;
		c=cp[np+1];
		cp[np+1]=cp[np+2];
		cp[np+2]=c;
	}
	
	
	return 0;
}
