 #include <stdio.h>
 //cc -o TestMacro TestMacro.c
 int sub(double *b);
 int main()
 {
 	double b[4];
 	
 	for(int n=0;n<4;++n){
 	    b[n]=-1;
 	}
 	sub(b);
 	
 	for(int n=0;n<4;++n){
 	    fprintf(stderr,"n %d b[n] %g\n",n,b[n]);
 	}
 	
 	return 0;
 }
int sub(double *b)
{

#define b(i,j) b[i-1+(j-1)*2]

	for(int j=1;j<=2;++j){
	    for(int i=1;i<=2;++i){
	       b(i,j)=i+j*10;
	    }
	}
	
#undef b

 	for(int n=0;n<4;++n){
 	    b[n]=-1;
 	}
	return 1;
}