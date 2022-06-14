#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "Simple.h"
#include "Simple.tab.h"
#define CHARMAX 1024	/*	character buffers	*/
#define CPMEOF 26

struct Global g;

static int ct,c;
static char *list,ch[256],*chp;
static char buff[CHARMAX];

static int skip();
static int ib();
static int ibc();
static int ishex();
static int do_string();

int new_line();

int yylex()
{
	struct c_list *m_np,*c_np;
	char *strsave();
	int n,cn,cnt;

	ct=0;
	skip();
	g.column = list - buff;
	c=*list++;
	if(c == CPMEOF ){
	    return(0);
	}
	chp=ch;
	cnt=0;
	ibc();
	if(isalpha(c) || c == '_'){	/*	if symbol	*/
	    while(isalpha(c=*list++) || isdigit(c) || c == '_')ibc();
	    ib('\0');
	    --list;
Start:
	    if((c_np=find_sym(g.c_root,ch))){
		c=c_np->c_type;			/* token  */
		return c;
	    }
	    if(cnt++ > 50){
	        printf("Recusive Macro Expansion (%s)\n",ch);
	    	return IDENTIFIER;	        
	    }
	    if((m_np=find_sym(g.m_root,ch))){
	        if(m_np->c_string){
	            strcpy(ch,m_np->c_string);
	            goto Start;
	        }else{
	            g.c_long=m_np->c_long;
		    return C_LONG;
	        }
	    }
	    g.c_string=strsave(ch);
	    return IDENTIFIER;
	} else if(isdigit(c) ){		/*	if number	*/
	    while(isdigit(c=*list++))ibc();
	    switch(c){
	    case 'l':
	    case 'L':
		ib('\0');
		if(ch[0] == '0'){
		    sscanf(&ch[1],"%lo",&g.c_long);
		 }else{
		    sscanf(ch,"%ld",&g.c_long);
		 }   
		return C_LONG;
	    case 'x':
	    case 'X':
		while(ishex(c=*list++))ibc();
		ib('\0');
		if(c == 'l' || c == 'L'){
		    sscanf(&ch[1],"%lx",&g.c_long);
		    return C_LONG;
		} else {
		    --list;
		    sscanf(ch,"%x",&g.c_int);
		    return C_INT;
		}
	    case '.':
		ib('.');
lfloat:
		while(isdigit(c=*list++))ibc();
		    if(c != 'e' && c != 'E'){
			--list;
			ib('\0');
		        sscanf(ch,"%lf",&g.c_double);
			return C_DOUBLE;
		    }
	    case 'e':
	    case 'E':
		   ib('e');
		   if(!isdigit(c=*list++) && c != '+' && c != '-') {
			printf(" bad exponent\n");
			return ERROR;
		   }
		   ibc();
		   while(isdigit(c=*list++))ibc();
		   --list;
		   ib('\0');
		   sscanf(ch,"%lf",&g.c_double);
		   return C_DOUBLE;
	    }
	    ib('\0');
	    if(ch[0] == '0'){
		sscanf(ch,"%o",&g.c_int);
	    }else{
		sscanf(ch,"%d",&g.c_int);
	    }
	    --list;
	    return C_INT;
	} else if(c == '"'){	/*	string		*/
	    chp=ch;
	    while((c=*list++) != '"' && c){
		if(c == '\\'){
		    if(!*list){
			new_line();
			continue;
		    } else{
			ibc();
			c=*list++;
		    }
		}
		ibc();
	    }
	    ib('\0');
	    do_string(ch);
	    g.c_string=strsave(ch);
	    return C_STRING;
	} else if(c == '\''){	/* character constant	*/
	    chp=ch;
	    while((c=*list++) != '\'' && c ){
		if(c == '\\'){
		    ibc();
		    c=*list++;
		}
		ibc();
	    }
	    ib('\0');
	    if(ch[0] == '\\'){
		do_string(ch);		 
	    }
	    g.c_int=ch[0];
	    return C_INT;
	}

	skip();
	ct=*list;
	if(ct == '='){
	    ct=c;
	    ++list;
	    switch(ct){
	    case '!':
		return NE_OP;
	    case '%':
		return MOD_ASSIGN;
	    case '^':
		return XOR_ASSIGN;
	    case '&':
		return AND_ASSIGN;
	    case '*':
		return MUL_ASSIGN;
	    case '-':
		return SUB_ASSIGN;
	    case '+':
		return ADD_ASSIGN;
	    case '=':
		return EQ_OP;
	    case '|':
		return OR_ASSIGN;
	    case '<':
		return LE_OP;
	    case '>':
		return GE_OP;
	    case '/':
		return DIV_ASSIGN;
	    default:
		--list;
		c=ct;
		ct=*list;
	        break;
	    }
	}
	switch(c){
	case '.':
	    if(isdigit(c=*list++)){
		ibc();
		goto lfloat;
	    }
	    --list;
	    return '.';
	case ']':
	case '[':
	case ',':
	case ':':
	case '?':
	case '~':
	case '!':
	case '%':
	case '^':
	case '*':
	case '=':
	case '/':
	case '#':
	    return c;
	case '&':
	    if(ct == '&'){
		++list;
		return AND_OP;
	    }
	    return c;
	case '-':
	    ++list;
	    if(ct == '-'){
		return DEC_OP;
	    } else if( ct == '>'){
	        return MEMBER;
	    } else {
		--list;
	    }
	    return c;
	case '+':
	    if(ct == '+'){
		++list;
		return INC_OP;
	    }
	    return c;
	case '|':
	    if(ct == '|'){
		++list;
		return OR_OP; 
	    }
	    return c;
	case '<':
	    if(ct != '<'){
		return c;
	    }
	    ++list;
	    skip();
	    if(*list == '='){
		++list;
		return LEFT_ASSIGN;
	    } else {
		return LEFT_OP;
	    }
	case '>':
	    if(ct != '>'){
		return c;
	    }
	    ++list;
	    skip();
	    if(*list == '='){
		++list;
		return RIGHT_ASSIGN;
	    } else {
		return RIGHT_OP;
	    }
	case '}':
	case '{':
	case ')':
	case '(':
	case ';':
	    return c;	
	default:
	    printf("Bad character : %d\n",c);
	    return ERROR;
	}
	return 0;
}

/*
static int isdigit(c)
int c;
{
	return (c >= '0' && c <= '9');
}
static int isalpha(c)
int c;
{
	return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z');
}
*/

/*	skip white space	*/
static int skip()
{
	int tr;
	
	while((tr=*list++) == '\t' || tr == ' ' || tr =='\f');
	--list;
	if(!tr){
	    new_line();
	    skip();
	}
	return 0;
}
static int ib(cc)
int cc;
{
	if(chp < ch+CHARMAX){
	    *chp++=cc;
	}else {
	    printf("ib Exceeded max char ib\n");
	    longjmp(env77,1);
	}
	return 0;
}
static int ibc()
{
	ib(c);
	return 0;
}
/*		find hex	*/
static int ishex(a)
int a;
{
	return(	(a >= '0' && a <= '9') ||
		(a >= 'a' && a <= 'f') ||
		(a >= 'A' && a <= 'F'));
}
/*	fix strings	*/
static int do_string(s)
char *s;
{
	char *so;
	int c,c1;

	so=s;
	while((c=*s++)){
	    if(c != '\\'){
		;
	    } else {
		if(!(c=*s++)){
		    *so='\0';
		    return 0;
		} else {
		    switch(c){
		    case 'b':
			c=8;
			break;
		    case 't':
			c=9;
			break;
		    case 'n':
			c=10;
			break;
		    case 'v':
			c=11;
			break;
		    case 'f':
			c=12;
			break;
		    case 'r':
			c=13;
			break;
		    default:
			break;
		    case '0':
		    case '1':
		    case '2':
		    case '3':
		    case '4':
		    case '5':
		    case '6':
		    case '7':
			c -='0';
			if((c1=*s-'0') >= 0 && c1 <= 7){
			    c=(c<<3)|c1;
			    ++s;
			    if((c1=*s-'0') >= 0 && c1 <= 7){
				c=(c<<3)|c1;
			    ++s;
			    }
			}
			break;
		    }
		}
	    }
	*so++=c;
	}
	*so='\0';
	return 0;
}
/*	load new line	*/
int new_line()
{
	static FILE *flist[15];
	static int Count;
	
	static char p[]={'#','i','n','c','l','u','d','e','\0'};
	static char *pp,name[256];
	int tr,cn;
	
Start:
	list=buff;
	while(1){
	    if((tr=getc(g.infile)) == CPMEOF || tr == EOF){
EndOfFile:
		if(Count > 0){
		    fclose(g.infile);
		    g.infile=flist[--Count];
		    goto Start;
		}
		*buff=CPMEOF;
		list=buff;
		buff[1]='\0';
	    	return 0;
	    } else if(tr == '\n'){
		if(((list-1) >= buff) && (*(list-1) == '\r'))--list;
		break;
	    } else if(list < buff+CHARMAX){
	        if(tr == '/'){
	             cn=getc(g.infile);
	             if(cn == '*'){
MoreContinue:
	                 while((tr=getc(g.infile)) != EOF && tr != '*');
	                 if(tr == EOF){
	                     printf("Error EOF Unterminated Comment\n");
	                     goto EndOfFile;
	                 }
	                 if((tr=getc(g.infile)) != EOF && tr != '/')goto MoreContinue;
	                 if(tr == EOF){
	                     printf("Error EOF Unterminated Comment\n");
	                     goto EndOfFile;
	                 }
	                 continue;
	             }
	             ungetc(cn,g.infile);
	        }
		*list++=tr;
	    } else {
		printf("Line length exceeded\n");
	        longjmp(env77,1);
	    }
	}

	*list='\0';
	list=buff;
	if(g.infile != g.inopt){
	    ++g.linenum;
	    if(g.writes)printf(" %d:%s\n",g.linenum,list);
	}
	
	if(*list == '#'){
	    pp=p;
	    while(*pp)if(*pp++ != *list++)goto Out;
	    while(*list && *list != '"')++list;
	    if(!*list++){
	        printf("Include File Name Not Found\n");
	        goto Out;
	    }
	    pp=name;
	    while(*list && *list != '"'){
	        *pp++ = *list++;
	    }
	    if(!*list){
	        printf("Include File Name Not Terminated\n");
	        goto Out;
	    }
	    *pp=0;
	    
	    flist[Count++]=g.infile;
	    	    
	    if(!(g.infile=fopen(name,"r"))){
	        printf("Could Not Open File (%s) To Read\n",name);
		g.infile=flist[--Count];
		goto Start;
	    }
	    goto Start;
	}
Out:
	list=buff;
	return 0;
}
