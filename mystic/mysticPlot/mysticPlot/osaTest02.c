#include <stdio.h>
#include <string.h>
 #include <stdlib.h>

int zerol(char *s,unsigned long n);
int mstrncat(char *out,char *in,long n);

int makeCommand(char *command, char *file, char *Application, int length);

int main(int argc,char *argv[])
{
	FILE *out;
	char output[4096];
	char *file="/Users/dir/Desktop/Apple-Script/test04.exp";
	char *Application="/Applications/mShow.app";
	
	
	file="/Users/dir/xappl/out.exp";
	
	out=fopen(file,"w");
	if(!out){
	     printf("Could Not Open %s To Write\n",file);
	     return 1;
	}
	
	fprintf(out,"one\n");
	fprintf(out,"two\n");
	fprintf(out,"three\n");
	
	if(out)fclose(out);
		
	makeCommand(output,file,Application,sizeof(output));
	
	printf("%s\n",output);
	
	system(output);
	
	
	return 0;
}


int makeCommand(char *command, char *file, char *Application, int length)
{
	if(!command || !file || !Application)return 1;
	
	zerol((char *)command,length);
	
	mstrncat(command,"/usr/bin/osascript -e 'tell application \"System Events\"' -e '  set default application of file \"",length);
	
	mstrncat(command,file,length);
	
	mstrncat(command,"\" to \"",length);
	
	mstrncat(command,Application,length);
	
	mstrncat(command,"\" ' -e 'end tell'",length);
	
	return 0;
}


/*
int makeCommand(char *command, char *file, char *Application, int length)
{
	if(!command || !file || !Application)return 1;
	
	zerol((char *)command,length);
	
	mstrncat(command,"osascript -e 'tell application \"System Events\"' -e '  set default application of file \"",length);
	
	mstrncat(command,file,length);
	
	mstrncat(command,"\" to \"",length);
	
	mstrncat(command,Application,length);
	
	mstrncat(command,"\" ' -e 'end tell'",length);
	
	return 0;
}
*/
int mstrncat(char *out,char *in,long n)
{
	long len;
	
	if(!out || !in || (n <= 0))return 1;
		
	len=(long)strlen(out);
	
	n -= len;
	
	if(n <= 0)return 1;
	
	out += len;
	
	while(n-- > 0){
	    if(*in == 0){
	       *out = 0;
	       break;
	    }else{
	       *out++ = *in++;
	    }
	}
	
	return 0;
}
int zerol(char *s,unsigned long n)
{
    if(!s || (n <= 0))return 1;


	while(n-- > 0)*s++ = 0;
	
	return 0;
}
