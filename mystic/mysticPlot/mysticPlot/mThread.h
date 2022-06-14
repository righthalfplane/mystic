#ifndef __MTHREAD__

#define __MTHREAD__

typedef struct mThreadStruct{
	void *data;
	long smin;
	long smax;
    int done;
    int nthread;
	double value1;
	double value2;
    void *save;     /* place to save windows thread handle for later release */
}mThread;

int runThreads(long ThreadCount,void *data,long Count,int (*sageThread)(mThread *Threads));

int launchThread(void *data,int (*sageThread)(void *data));


int runThreads2(long ThreadCount,void *data,long DataCount,int (*sageThread)(mThread *Threads),
				int (*sageThread2)(mThread *Threads,int count));

int runThreads3(long ThreadCount,void *data,long DataCount,long ZonesPerBlock,
                int (*sageThread)(mThread *Threads),
				int (*sageThread2)(mThread *Threads,int count));


#endif
