#define EXTERN22 extern
#include "Xappl.h"

static struct OctTree{
        double RedAverage;
        double GreenAverage;
        double BlueAverage;
        long Count;
        int Red;
        int Green;
        int Blue;
        int Level;
} *OctTree;

int treeClean(void);

static unsigned char *red,*green,*blue,*tarrgb;
static long *number;

static long NewColors,TreeReductions,LargestCount;
static int DeepLevel;

static void StartOctTree(void);

static void AddOctTree(int r,int g,int b,long n,int k);

int reduceIt(unsigned char *bp,long length);
int buildPalette(unsigned char *pal);

int Build24to8(unsigned char *pal);
int Crush24to8(unsigned char *pp,long xsize,long ysize);

static void SearchOctTree(int r,int g,int b,long n);

int Start24to8(void);

int Add24to8(unsigned char *pp,long xsize,long ysize);

int do24to8(unsigned char *pal,unsigned char *p1,unsigned char *pp,long xsize,long ysize);

int Start24to8(void)
{
	long ni;

	treeClean();

	    OctTree=(struct OctTree  *)cMalloc(256*sizeof(struct OctTree),8088);
	    if(!OctTree){
	        sprintf(WarningBuff,"Start24to8 OctTree out of Memory\n");
			WarningBatch(WarningBuff);
	        return 1;
	    }
	    red=NULL;
	    green=NULL;
	    blue=NULL;
	    tarrgb=NULL;
	    number=NULL;
	    red=(unsigned char  *)cMalloc(4096*sizeof(unsigned char),8089);
	    if(!red){
	        sprintf(WarningBuff,"Start24to8 OctTree out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    green=(unsigned char  *)cMalloc(4096*sizeof(unsigned char),8090);
	    if(!green){
	        sprintf(WarningBuff,"Start24to8 OctTree out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    blue=(unsigned char  *)cMalloc(4096*sizeof(unsigned char),8091);
	    if(!blue){
	        sprintf(WarningBuff,"Start24to8 OctTree out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    tarrgb=(unsigned char  *)cMalloc(4096*sizeof(unsigned char),8092);
	    if(!tarrgb){
	        sprintf(WarningBuff,"Start24to8 OctTree out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    number=(long  *)cMalloc(4096*sizeof(long),8093);
	    if(!number){
	        sprintf(WarningBuff,"Start24to8 OctTree out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }

 	zerol((char *)OctTree,256L*sizeof(struct OctTree));

        StartOctTree();
 
        for(ni=0;ni<4096;++ni){
            number[ni]=0;
	    red[ni]=0;
	    green[ni]=0;
	    blue[ni]=0;
	}
	return 0;
ErrorOut:
    return 1;
}
int treeClean(void)
{
	if(OctTree)cFree((char *)OctTree);
	OctTree=NULL;
	if(red)cFree((char *)red);
	red=NULL;
	if(green)cFree((char *)green);
	green=NULL;
	if(blue)cFree((char *)blue);
	blue=NULL;
	if(tarrgb)cFree((char *)tarrgb);
	tarrgb=NULL;
	if(number)cFree((char *)number);
	number=NULL;
	return 0;
}
int Add24to8(unsigned char *pp,long xsize,long ysize)
{
	long length;

	if(!pp)return 1;

	length=ysize*xsize;

	reduceIt(pp,length);

	return 0;
}
int Build24to8(unsigned char *pal)
{
	if(!pal)return 1;

	buildPalette(pal);

	return 0;
}
int Crush24to8(unsigned char *pp,long xsize,long ysize)
{
	long length;
	unsigned char *bp,*bt;
	long n;

	if(!pp)return 1;

	length=ysize*xsize;

	bp=pp;
	bt=pp;
        for(n=0;n<length;++n){
            unsigned int r,g,b;
            int index;

            r=*bp++;
            g=*bp++;
            b=*bp++;
            index = (r >> 4) + ((g >> 4) << 4) + ((b >> 4) << 8);
            *bt++ = tarrgb[index];
        }

	return 0;
}
int buildPalette(unsigned char *pal)
{
       int rmin,rmax,gmin,gmax,bmin,bmax;
       int index,range;
       int i,j,k,ni;
       long div,sum;

       if(!pal)return 1;

        sum = 0;
        for(ni=0;ni<4096;++ni){
            if((div=number[ni]) != 0){
                SearchOctTree(red[ni],green[ni],blue[ni],div);
                ++sum;
            }
        }
        for(ni=0;ni<256;++ni){
            unsigned int r,g,b;
 
            r=(unsigned int)(OctTree[ni].RedAverage+.5);
            g=(unsigned int)(OctTree[ni].GreenAverage+.5);
            b=(unsigned int)(OctTree[ni].BlueAverage+.5);
            pal[3*ni]=r;
            pal[3*ni+1]=g;
            pal[3*ni+2]=b;
            if(!OctTree[ni].Count)continue;
            if(r > 255 || g > 255 || b > 255){
                sprintf(WarningBuff,"Bad Color ni %d r %d g %d b %d\n",ni,r,g,b);
				WarningBatch(WarningBuff);
            }
            if(OctTree[ni].Count > LargestCount)LargestCount=OctTree[ni].Count;
            r=OctTree[ni].Red;
            g=OctTree[ni].Green;
            b=OctTree[ni].Blue;
            range=OctTree[ni].Level;
            rmin=(r >> 4);
            rmax=((r+range-1) >> 4);
            gmin=(g >> 4);
            gmax=((g+range-1) >> 4);
            bmin=(b >> 4);
            bmax=((b+range-1) >> 4);
            for(i=rmin;i<=rmax;++i){
                for(j=gmin;j<=gmax;++j){
                    for(k=bmin;k<=bmax;++k){
                 	    index = (i) + (j << 4) + (k << 8);
                 	    if(index < 0 || index > 4096){
                 	        sprintf(WarningBuff,"index Error %d\n",index);
 							WarningBatch(WarningBuff);
                	        continue;
                 	    }
                 	    tarrgb[index]=ni;
                    }
                }
            }
        }
	return 0;
}
int reduceIt(unsigned char *bp,long length)
{
        long rastors,div,mult;
        int r,g,b,ro,go,bo;
		int index,n;

		if(!bp)return 1;

        ro=-1;
        go=-1;
        bo=-1;
        rastors=0;
        for(n=0;n<length;++n){
            r=*bp++;
            g=*bp++;
            b=*bp++;
            ++rastors;
            if(r != ro || g != go || b != bo){
                 ro=r;
                 go=g;
                 bo=b;
                 index = (r >> 4) + ((g >> 4) << 4) + ((b >> 4) << 8);
                 if(index < 0 || index > 4096){
                    sprintf(WarningBuff,"index error %d\n",index);
                    continue;
                 }
                 mult = number[index];
                 div =  mult +  rastors;
                 red[index]     = (unsigned char)((red[index]*mult   + r*rastors)/div);
                 green[index]   = (unsigned char)((green[index]*mult + g*rastors)/div);
                 blue[index]    = (unsigned char)((blue[index]*mult  + b*rastors)/div);
                 number[index]  = div;
                 rastors=0;
            }
        }
	return 0;
}
static void StartOctTree(void)
{
        int k;
 
        NewColors=0;
        TreeReductions=0;
        DeepLevel=0;
        LargestCount=0;
        for(k=0;k<256;++k){
            OctTree[k].Count=0;
            OctTree[k].Level=0;
        }
        OctTree[0].Count=93200000;
        OctTree[0].Level=1;
        OctTree[0].Red    =  255;
        OctTree[0].Green  =  255;
        OctTree[0].Blue   =  255;
        OctTree[0].RedAverage    = 255;
        OctTree[0].GreenAverage  =  255;
        OctTree[0].BlueAverage   =  255;
        OctTree[1].Count=1320000;
        OctTree[1].Level=1;
        OctTree[255].Count=93200000;
        OctTree[255].Level=1;
}
static void SearchOctTree(int r,int g,int b,long n)
{
        int k,ks,First,Level;
        long div,Count;
 
        while(1){
            ks=-1;
            for(k=0;k<256;++k){
                  if(OctTree[k].Count){
                      Level=OctTree[k].Level;
                      if(OctTree[k].Red/Level == r/Level   &&
                         OctTree[k].Green/Level == g/Level &&
                         OctTree[k].Blue/Level == b/Level  ){
                         AddOctTree(r,g,b,n,k);
                         return;
                      }
                  }else{
                      if(ks<0)ks=k;          /* Save Open Slot Number */
                  }
            }
            if(ks >= 0){                             /* Put in Open Slot      */
FillSlot:
                OctTree[ks].Red    =  r;
                OctTree[ks].Green  =  g;
                OctTree[ks].Blue   =  b;
                OctTree[ks].RedAverage    =  r;
                OctTree[ks].GreenAverage  =  g;
                OctTree[ks].BlueAverage   =  b;
                OctTree[ks].Count=n;
                OctTree[ks].Level=1;
                ++NewColors;
                return;
            }
 
            First=512;
            Count=10000000;
            for(k=2;k<255;++k){
                  if(OctTree[k].Level <= First){        /* Look For Lower Level */
                        if(OctTree[k].Level == First){
                           if(OctTree[k].Count >= Count){/* Look For Smaller Count */
                                continue;
                           }
                        }
                        First=OctTree[k].Level;
                        Count=OctTree[k].Count;
                        ks=k;
                  }
            }
            if(First == 512){
				sprintf(WarningBuff,"OctTreeError\n");
				WarningBatch(WarningBuff);
				return;
            }
 
 
 
            OctTree[ks].Level *= 2;
            Level=OctTree[ks].Level;
            OctTree[ks].Red   /= Level;
            OctTree[ks].Green /= Level;
            OctTree[ks].Blue  /= Level;
 
            OctTree[ks].Red   *= Level;
            OctTree[ks].Green *= Level;
            OctTree[ks].Blue  *= Level;
            if(Level > DeepLevel)DeepLevel=Level;
 
            First=-1;
            for(k=2;k<255;++k){
                double rdiv,rks,rk;
                if(k == ks)continue;
                if(OctTree[ks].Red/Level ==   OctTree[k].Red/Level   &&
                   OctTree[ks].Green/Level == OctTree[k].Green/Level &&
                   OctTree[ks].Blue/Level ==  OctTree[k].Blue/Level  ){
                   if(First < 0)First=k;
                   div=OctTree[k].Count+OctTree[ks].Count;
                   rdiv=1.0/(double)div;
                   rks=(double)OctTree[ks].Count;
                   rk=(double)OctTree[k].Count;
                   OctTree[ks].RedAverage =   (OctTree[ks].RedAverage*rks
                                              +OctTree[k].RedAverage*rk)*rdiv;
                   OctTree[ks].GreenAverage = (OctTree[ks].GreenAverage*rks
                                              +OctTree[k].GreenAverage*rk)*rdiv;
                   OctTree[ks].BlueAverage =  (OctTree[ks].BlueAverage*rks
                                              +OctTree[k].BlueAverage*rk)*rdiv;
                   OctTree[ks].Count=div;
                   OctTree[k].Count=0;
                   ++TreeReductions;
                }
            }
 
 
            if(OctTree[ks].Red/Level ==   r/Level   &&
               OctTree[ks].Green/Level == g/Level &&
               OctTree[ks].Blue/Level ==  b/Level  ){
                     AddOctTree(r,g,b,n,ks);
                     return;
            }
            if(First < 0)continue;
            ks=First;
            goto FillSlot;
 
        }
}
static void AddOctTree(int r,int g,int b,long n,int k)
{
        long div=OctTree[k].Count+n;
        double rdiv=1.0/(double)(div);
        double rk=(double)OctTree[k].Count;
 
        OctTree[k].RedAverage   = (OctTree[k].RedAverage*rk+(double)(r*n))*rdiv;
        OctTree[k].GreenAverage = (OctTree[k].GreenAverage*rk+(double)(g*n))*rdiv;
        OctTree[k].BlueAverage  = (OctTree[k].BlueAverage*rk+(double)(b*n))*rdiv;
        OctTree[k].Count=div;
}
int do24to8(unsigned char *pal,unsigned char *p1,unsigned char *pp,long xsize,long ysize)
{
	unsigned char *bp,*bt;
	unsigned char *d;
	long ni,n,length;

	if(!pal || !pp)return 1;

	if(!OctTree){
	    OctTree=(struct OctTree  *)cMalloc(256*sizeof(struct OctTree),8094);
	    if(!OctTree){
	        sprintf(WarningBuff,"do24to8 OctTree out of Memory\n");
	        Warning(WarningBuff);
	        return 1;
	    }
	    red=NULL;
	    green=NULL;
	    blue=NULL;
	    tarrgb=NULL;
	    number=NULL;
	    red=(unsigned char  *)cMalloc(4096*sizeof(unsigned char),8095);
	    if(!red){
	        sprintf(WarningBuff,"do24to8 OctTree out of Memory\n");
	        Warning(WarningBuff);
	        goto ErrorOut;
	    }
	    green=(unsigned char  *)cMalloc(4096*sizeof(unsigned char),8096);
	    if(!green){
	        sprintf(WarningBuff,"do24to8 OctTree out of Memory\n");
	        Warning(WarningBuff);
	        goto ErrorOut;
	    }
	    blue=(unsigned char  *)cMalloc(4096*sizeof(unsigned char),8097);
	    if(!blue){
	        sprintf(WarningBuff,"do24to8 OctTree out of Memory\n");
	        Warning(WarningBuff);
	        goto ErrorOut;
	    }
	    tarrgb=(unsigned char  *)cMalloc(4096*sizeof(unsigned char),8098);
	    if(!tarrgb){
	        sprintf(WarningBuff,"do24to8 OctTree out of Memory\n");
	        Warning(WarningBuff);
	        goto ErrorOut;
	    }
	    number=(long  *)cMalloc(4096*sizeof(long),8099);
	    if(!number){
	        sprintf(WarningBuff,"do24to8 OctTree out of Memory\n");
	        Warning(WarningBuff);
	        goto ErrorOut;
	    }
	}

 	zerol((char *)OctTree,256L*sizeof(struct OctTree));

        StartOctTree();
 
        for(ni=0;ni<4096;++ni){
            number[ni]=0;
	    red[ni]=0;
	    green[ni]=0;
	    blue[ni]=0;
	}

	length=ysize*xsize;

	if(p1){
	    d=p1;
	    for(n=0;n<length*3;++n){
	       pp[n]=(unsigned char)(255*d[n]+.5);
	    }
	}

	reduceIt(pp,length);

	buildPalette(pal);

	bp=pp;
	bt=pp;
        for(n=0;n<length;++n){
            unsigned int r,g,b;
            int index;

            r=*bp++;
            g=*bp++;
            b=*bp++;
            index = (r >> 4) + ((g >> 4) << 4) + ((b >> 4) << 8);
            *bt++ = tarrgb[index];
        }

	return 0;
ErrorOut:
	if(OctTree)cFree((char *)OctTree);
	if(red)cFree((char *)red);
	if(green)cFree((char *)green);
	if(blue)cFree((char *)blue);
	if(tarrgb)cFree((char *)tarrgb);
	if(number)cFree((char *)number);
	OctTree=NULL;
	return 1;
}
