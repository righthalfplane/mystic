#include bdscio.h
char input[BUFSIZ];
char inopt[BUFSIZ];
#define CPMEOF 26
#define MAXVAL 72

char list[MAXVAL+1],name[MAXVAL+1];
char sub[20],deck[20],prog[20],fun[20];
int ncol,nword,c;
int decks,lines;

main(argc,argv)	
int argc;
char *argv[];
{

	if(argc == 1){
	    printf("NO INPUT FILE\n");
	    exit(1);
	}
	tail(argv[1],".FOR");
	printf("\n Filename:   %s \n",list);
	if(fopen(list,input) == ERROR  ){
	    printf("CANNOT OPEN FILE\n");
	    exit(1);
	}

	setup();

	tail(argv[1],".DEK");
	fcreat(list,inopt);

	add();

	putc(CPMEOF,inopt);
	fclose(inopt);

	printf("\n Number of Lines = %d ",lines);
	printf("\n Number of Decks = %d \n",decks);

	exit(0);

}		
add()
{

	decks=0;
	lines=0;
	while(getline(list,MAXVAL) != EOF){
	    ncol=0;
	    if(list[0] == 'C'){
		l_out();
	    } else {
		next();
		if(!strcmp(sub,name) || !strcmp(prog,name) ||
		   !strcmp(fun,name)){
		    next();
		    d_out();
		}
		l_out();
	    }
	}
}
d_out()
{
	ncol=0;
	while((c=deck[ncol++]) != '\0')putc(c,inopt);
	putc((c=' '),inopt);
	ncol=0;
	while((c=name[ncol++]) != '\0')putc(c,inopt);
	putc((c='\r'),inopt);
	putc((c='\n'),inopt);
	decks++;

}
l_out()
{
	for(nword--;nword > 0;nword--){
	    if(list[nword] != ' ')break;
	    list[nword]='\0';
	}

	for(ncol=0;(c=list[ncol]) != '\0';ncol++){
	    putc(c,inopt);
	}
	putc((c='\r'),inopt);
	putc((c='\n'),inopt);
	lines++;

}
skip()
{
	while((c=list[ncol]) == ' ' || c == '\t')ncol++;
}
next()
{
	int n;

	skip();

	n=0;
	while((c=list[ncol]) != ' ' && c != '\0' && c != '('  ){
	    name[n++]=c;
	    ncol++;
	}
	name[n]='\0';
}
tail(f_in,f_ext)
char f_in[],f_ext[];
{
	ncol=0;
	nword=0;
	while((c=f_in[ncol++]) != '\0' && c != '.')list[nword++]=c;
	ncol=0;
	while((c=f_ext[ncol++]) != '\0')list[nword++]=c;
	list[nword]='\0';
}
setup()
{
	strcpy(sub,"SUBROUTINE");
	strcpy(deck,"*DECK");
	strcpy(prog,"PROGRAM");
	strcpy(fun,"FUNCTION");
}
getline(w,lim)
char w[];
int lim;
{

	nword=0;
	while((c=getc(input))!=EOF && c != CPMEOF){
	    if(c == '\n'){
		w[nword]='\0';
		if(w[nword-1] == '\r')w[--nword]='\0';
		return(NULL);

	    }
	    if(nword < lim)w[nword++]=c;
	}
	return(EOF);
}
