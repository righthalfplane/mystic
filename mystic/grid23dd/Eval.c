#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Simple.h"
#include "Simple.tab.h"
#include "Simpleop.h"
 
#define DATASEGMENT	1
#define CODESEGMENT	2
 
extern int VariableStorage[];
extern char *TypeList[];
 
 
int dprintf(),dgetchar(),dputchar(),dsprintf();
int datan(),dsin(),dcos(),dfabs(),dsqrt(),dexit();
int dfprintf(),dfopen(),dfclose(),dmalloc(),dfree();
int dsecond(),datan2(),dfmod(),dbeam(),dbrick(),dpress();
int dshell();
 
static struct c_list *e_root,*u_root,*calls_root;
 
static int put_c();
 
struct forward{
	char *label;
	long code;
	int segment;
	int relative;
};
 
static struct c_list *c_np;
 
static struct c_list *put_ll();
 
static struct forward *reference;
 
static long codemax=10000,datamax=2000;
 
static long rCount,rCountMax=1500,codeRelative,stackLength;
 
static int relative = FALSE;
 
static int segment = CODESEGMENT;
 
static struct c_list *put_e();
 
static struct Register{
    int Class;
    int Type;
    int Mode;
    long Size;
    int pCommand;
    long pLength;
    struct dStack *dStack;
    char  *StructureName;
}R0,R2,Rzero;
 
static struct Register *rStack;
static long rStackCount,rStackMax=10;
 
 
 
static long c_long;
 
static int store();
static int push1();
static int pop2();
static int moreStack();
static int doSTRING();
static int doIdent();
static int doDEFAULT();
static int doCASE();
static int doSWITCH();
static int doRETURN();
static int doMEMBER();
static int doINDIRECTION();
static int doUNOT();
static int doExternalCalls();
static int doUBIT();
static int doUMINUS();
static int doUADDRESS();
static int PointerR0();
static int doARRAY();
static int storeR2();
static int loadR0();
static int doWHILE();
static int doDO();
static int doAND();
static int doOR();
static int doQUESTION();
static int doELSE();
static int doIF();
static int doPost();
static int doPre();
static int doFor();
static int doSubCall();
static int unMerge();
static int do_MTYPE();
static int do_ATYPE();
static int doBinaryOp();
static long PointerSize();
static int BinaryConversions();
static int writeLabel();
static int goForward();
static int moreForward();
static int more();
static int mored();
static int putDouble();
static int aline();
static int putString();
static int putByte();
static int jump();
static int jumptrue();
static int jumpfalse();
static int doR0();
 
int manageBC(int flag);
int zerol(char *value,long count);
int manageSW(int flag,long Value,long Address);
int new_line();
int setCalls(char *name);
int setCall(struct c_list *calls);
int setEx(char *name,int (*routine)(),int count);
int freeCTree(struct c_list *p);

int evalDone();
int endEval();

int freeTTree(struct tree *p);
int freeVSTree(struct Variables *p);
int putInt(int i);
int putLong(long l);
int doDS();
int doCS();
int getlab();

int Eval(struct tree *eRoot,int flag);

int putlab(int label);

int startEval(char *name)
{
	struct KeyWords{
	    char *name;
	    int type;
	};
	
	static struct KeyWords List[]={
	    "int",INT,
	    "char",CHAR,
	    "long",LONG,
	    "float",FLOAT,
	    "double",DOUBLE,
	    "if",IF,
 	    "else",ELSE,
	    "switch",SWITCH,
	    "while",WHILE,
	    "do",DO,
	    "for",FOR,
	    "continue",CONTINUE,
	    "break",BREAK,
	    "define",DEFINE,
	    "struct",STRUCT,
	    "return",RETURN,
	    "case",CASE,
	    "default",DEFAULT,
	    "sizeof",SIZEOF,
	    "end",END,
	};
	
	extern struct c_list *pfind;
	
	long Length;
	int n;
	
	
	g.code=0;
	
	g.data=0;
			
	g.error=0;
			
	g.linenum=0;
	
	e_root=NULL;
	
	g.l_root=NULL;
	
	g.c_root=NULL;
	
	g.m_root=NULL;
			
	g.ll_root=NULL;
	
	g.Address=0;
	
	g.dStackData=NULL;
	
	g.LocalVariables=NULL;
		
	g.GlobalVariables=NULL;
		
	g.LocalStructures=NULL;
		
	g.GlobalStructures=NULL;
		
	segment = CODESEGMENT;
	
	g.LargestStructure=0;
	
	rCount=0;
	
	rStackCount=0;
	
	rStack=NULL;
	
	g.codebuff=NULL;
	
	g.databuff=NULL;
	
	reference=NULL;
	
	if(!(g.infile=fopen(name,"r"))){
	    printf("Error Cannot Open (%s) To Read\n",name);
	    return 1;
	};
	
	
	
	for(n=0;n<sizeof(List)/sizeof(struct KeyWords);++n){
	    put_c(List[n].name);
	    c_np->c_type=List[n].type;
	}
	
	g.treesize=sizeof(*g.m_root);
	g.m_root=tree(g.m_root,"FILE");
	pfind->c_string=strsave("char");
	
	
	
	if(!(g.codebuff=(unsigned char *)malloc(codemax))){
	    printf("codebuff out of memory\n");
	    return 1;
	}
	zerol((char *)g.codebuff,codemax);
	
	if(!(g.databuff=(unsigned char *)malloc(datamax))){
	    printf("databuff out of memory\n");
	    return 1;
	}
	zerol((char *)g.databuff,datamax);
	
	Length=rCountMax*sizeof(struct forward);
	if(!(reference=(struct forward *)malloc(Length))){
	    printf("reference out of memory (%ld)\n",Length);
	    return 1;
	}
	
	zerol((char *)reference,Length);
	
	Length=rStackMax*sizeof(struct Register);
	if(!(rStack=(struct Register *)malloc(Length))){
	    printf("rStack out of memory\n");
	    return 1;
	}
	zerol((char *)rStack,Length);
	
	manageBC(BC_START);
	manageSW(SW_START,0L,0L);
	
	new_line();
	return 0;
}
int setCalls(char *name)
{
	extern struct c_list *pfind;
	
	g.treesize=sizeof(*calls_root);
	calls_root=tree(calls_root,name);
	return 0;
}
struct c_list *findCalls(char *name)
{
	struct c_list *calls;
	
	return (calls=find_sym(calls_root,name));
}
int saveCalls()
{
	setCall(calls_root);
	return 0;
}
int setCall(struct c_list *calls)
{
	struct c_list *m_root,*a_np;
	
	if(calls){
	    if(calls->c_left)setCall(calls->c_left);
	    if(calls->c_right)setCall(calls->c_right);
	    if((a_np=find_sym(g.l_root,calls->c_name))){
	        calls->c_long = a_np->c_long;
	    }else if((a_np=find_sym(g.ll_root,calls->c_name))){
	        calls->c_long = a_np->c_long;
	    }
	}
	return 0;
}
int setEx(char *name,int (*routine)(),int count)
{
	extern struct c_list *pfind;
	
	g.treesize=sizeof(*u_root);
	u_root=tree(u_root,name);
	c_np=pfind;
	c_np->c_type=count;
	c_np->routine=routine;
	return 0;
}
int clearEx()
{
	if(u_root){
	    freeCTree(u_root);
	    u_root=NULL;
	}
	if(calls_root){
	    freeCTree(calls_root);
	    calls_root=NULL;
	}
	return 0;
}
/*	code data	*/
static int put_c(a)
char *a;
{
	extern struct c_list *pfind;
 
	g.treesize=sizeof(*g.c_root);
	g.c_root=tree(g.c_root,a);
	c_np=pfind;
	return 0;
}
int endEval()
{
	int ret;
 
	ret=0;
	printf("code : %ld data : %ld Reference : %ld LargestStructure %ld\n",
	       g.code,g.data,rCount,g.LargestStructure);
	
	if(rCount){
	    ret=doExternalCalls();
	}
	evalDone();
	return (ret | g.error);

}
int evalDone()
{
	extern struct dStack *dStackData;
	long n;
	
	for(n=0;n<rCount;++n){	
	    if(reference[n].label)free((char *)reference[n].label);
	}
	rCount=0;
	manageBC(BC_FINISH);
	manageSW(SW_FINISH,0L,0L);
	if(rStack){
	    free((char *)rStack);
	    rStack=NULL;
	}
	if(reference){
	    free((char *)reference);
	    reference=NULL;
	}
	if(e_root){
	    freeCTree(e_root);
	    e_root=NULL;
	}
	if(g.c_root){
	    freeCTree(g.c_root);
	    g.c_root=NULL;
	}
	if(g.m_root){
	    freeCTree(g.m_root);
	    g.m_root=NULL;
	}
	if(g.l_root){
	    freeCTree(g.l_root);
	    g.l_root=NULL;
	}
	if(g.GlobalVariables){
	    freeVSTree(g.GlobalVariables);
	    g.GlobalVariables=NULL;
	}
	if(g.GlobalStructures){
	    freeVSTree(g.GlobalStructures);
	    g.GlobalStructures=NULL;
	}
	if(g.dStackData){
	    free((char *)g.dStackData);
	    g.dStackData=NULL;
	}
	if(g.infile){
	    FILE *out;
	    int c;

            if((out=fopen("input","w"))){
		while((c=getc(g.infile)) != EOF)putc(c,out);
		fclose(out);
	    }
	    fclose(g.infile);
	}
	g.infile=NULL;
	return 0;
}
static int doExternalCalls()
{
	struct Externals{
	    char *name;
	    int (*routine)();
	};
	
	
	static struct Externals ExList[]={
	    "malloc",dmalloc,
	    "free",dfree,
	    "fprintf",dfprintf,
	    "printf",dprintf,
	    "sprintf",dsprintf,
	    "fclose",dfclose,
	    "getchar",dgetchar,
	    "putchar",dputchar,
	    "fopen",dfopen,
	    "atan",datan,
	    "atan2",datan2,
	    "fmod",dfmod,
	    "sin",dsin,
	    "cos",dcos,
	    "fabs",dfabs,
	    "sqrt",dsqrt,
	    "exit",dexit,
	    "second",dsecond,
	    "beam",dbeam,
	    "brick",dbrick,
	    "press",dpress,
	    "shell",dshell,
	};
	
	struct c_list *l_np;
	
	unsigned char *pc;
	long *p,*d;
	int n,*i;
	int ierror;
	unsigned char **pu;
	
	e_root=NULL;
	
	for(n=0;n<sizeof(ExList)/sizeof(struct Externals);++n){
	    if(!(l_np=put_e(ExList[n].name))){
	          printf("put_e Out Of Memory\n");
	          return 1;
	    }
	    l_np->routine=ExList[n].routine;
	}
	
	
	ierror=0;
	for(n=0;n<rCount;++n){	
	    pc=g.codebuff+reference[n].code;
	    p=(long *)pc;
	    pc += sizeof(long);
	    i = (int *)pc;
	
	    pc=g.databuff+reference[n].code;
	    d=(long *)pc;
	
	    if((l_np=find_sym(u_root,reference[n].label)) && (reference[n].segment == CODESEGMENT)){
	        *p = (long)l_np->routine;
	        *i = -(l_np->c_type);
	        if(*i == 0)*i = -1;
	    }else if((l_np=find_sym(e_root,reference[n].label)) && (reference[n].segment == CODESEGMENT)){
	        *p = (long)l_np->routine;
	        *i = -(*i);
	        *i = 0;
	    }else{
	        if(reference[n].relative == CODE_RELATIVE){
	            pu=(unsigned char **)p;
	            *pu = *p+ g.codebuff;
	        }else if(reference[n].relative == DATA_RELATIVE){
	            pu=(unsigned char **)d;
	           *pu = *d + g.databuff;
	        }else{
	            printf("External Not Found (%s)\n",reference[n].label);
		    ierror=1;
		}
	    }
	}
	saveCalls();
	return ierror;
}
static struct c_list *put_e(a)
char *a;
{
	extern struct c_list *pfind;
	
	g.treesize=sizeof(*e_root);
	e_root=tree(e_root,a);
	return pfind;
}
static int store()
{
	static int Char[]={
			0,
			CVTCC4,
			CVTIC4,
			CVTLC4,
			CVTFC4,
			CVTDC4,
			   };
	static int Double[]={
			0,
			CVTCD4,
			CVTID4,
			CVTLD4,
			CVTFD4,
			CVTDD4,
			   };
	static int Float[]={
			0,
			CVTCF4,
			CVTIF4,
			CVTLF4,
			CVTFF4,
			CVTDF4,
			   };
	static int Long[]={
		 	0,
			CVTCL4,
			CVTIL4,
			CVTLL4,
			CVTFL4,
			CVTDL4,
			CVTPL4,
			   };
	int Type0,Type2;
	
	pop2();
	
	
	Type0 = (R0.dStack && R0.dStack->Type) ?  (R0.dStack->Type) : R0.Type;
	Type2 = (R2.dStack && R2.dStack->Type) ?  (R2.dStack->Type) : R2.Type;
	
	switch(Type2){
	    case T_CHAR:
	        if(Type0 >= 6){
	            printf("Cannot Cast %s To Char\n",TypeList[Type0]);
	            g.error=TRUE;
	            return 0;
	        }
	        putInt(Char[Type0]);
	        break;
	    case T_LONG:
	    case T_POINTER:
	        if(Type0 >= 6 || (R0.Mode == G_ADDRESS)){
	            Type0=3;
	        }
	        putInt(Long[Type0]);
	        break;
	    case T_FLOAT:
	        if(Type0 >= 6){
	            printf("Cannot Cast %s To Float\n",TypeList[Type0]);
	            g.error=TRUE;
	            return 0;
	        }
	        putInt(Float[Type0]);
	        break;
	    case T_DOUBLE:
	        if(Type0 >= 6){
	            printf("Cannot Cast %s To Double\n",TypeList[Type0]);
	            g.error=TRUE;
	            return 0;
	        }
	        putInt(Double[Type0]);
	        break;
	    case T_STRUCT:
	        if(Type0 != T_STRUCT){
	            printf("Cannot Cast %s To Structure\n",TypeList[Type0]);
	            g.error=TRUE;
	            return 0;
	        }
	        break;
	    default:
	        if(Type0 >= 7){
	            printf("Cannot Cast (%s) To (%s)\n",TypeList[Type0],TypeList[Type2]);
	            g.error=TRUE;
	            return 0;
	        }
	        putInt(Long[Type0]);
	        break;
	}
	storeR2();
	return 0;
}
static int push1()
{
	if(rStackCount >= rStackMax)moreStack();
	if(R0.dStack || (R0.Mode != G_VALUE)){
	    R0.pCommand=POPP;
	    R0.pLength=VariableStorage[T_POINTER];
	    putInt(PUSHP);
	}else{
	    switch(R0.Type){
	    case T_DOUBLE:
	        R0.pCommand=POPD;
	        R0.pLength=VariableStorage[T_DOUBLE];
	        putInt(PUSHD);
	        break;
	    case T_STRUCT:
	        R0.pCommand=POPS;
	        R0.pLength=R0.Size;
	        putInt(PUSHS);
	        putLong(R0.Size);
	        break;
	    default:
	        R0.pCommand=POPL;
	        R0.pLength=VariableStorage[T_LONG];
	        putInt(PUSHL);
	        break;
	    }
	}
	g.stackDepth += R0.pLength;
	rStack[rStackCount++]=R0;
	return 0;
}
static int pop2()
{
	if(rStackCount <= 0){
	    printf("Register Stack Underflow\n");
	    longjmp(env77,1);
	}
	R2=rStack[--rStackCount];
	g.stackDepth -= R2.pLength;
	putInt(R2.pCommand);
	if(R2.pCommand == PUSHS){
	    putLong(R2.Size);
	}
	return 0;
	
}
static int moreStack()
{
	long Length;
	
	rStackMax += 5;
	Length=sizeof(struct Register)*rStackMax;
	if(!(rStack=(struct Register *)realloc((char *)rStack,Length))){
	    printf("moreStack Out of Memory (Requested %ld)\n",Length);
	    g.error=TRUE;
	    longjmp(env77,1);
	}
	return 0;
}
static int doSTRING(sValue,flag)
char *sValue;
{
	extern struct c_list *pfind;
	long dAddress,pAddress;
	struct Variables *v;
	struct dStack *dStack;
	char buff[256];
	int label;
	long Length;
	
	dAddress=g.data;
	
	doDS();
	putString(sValue);
	aline();
	pAddress=g.data;
	
	label=getlab();
	sprintf(buff,".%d",label);
	relative = DATA_RELATIVE;
	c_long=dAddress;
	writeLabel(buff);
	relative = FALSE;
	
	
	doCS();
	
	label=getlab();
	sprintf(buff,".%d",label);
	g.treesize=sizeof(struct Variables);
	g.LocalVariables=(struct Variables *)tree(g.LocalVariables,buff);
	v=(struct Variables *)pfind;
	
	Length=(long)sizeof(struct dStack)*(1+1);
	if(!(dStack=(struct dStack  *)malloc(Length))){
	    printf("doSTRING (Request #2 - %ld)\n",Length);
	    g.error=TRUE;
	    longjmp(env77,1);
	}
	zerol((char *)dStack,Length);
	v->dStackCount=1;
	v->dStack=dStack;
	
	dStack[0].Type=T_POINTER;
	
	R0=Rzero;
	R0.Type=T_CHAR;
	R0.Class=V_GLOBAL;
	R0.dStack=dStack;
	R0.Mode = flag;
	
	if(flag == G_VALUE){
	    putInt(LEP0);
	}else{
	    putInt(LEA0);
	}
	
	putLong(pAddress);
	return 0;
	
}
static int doIdent(eRoot,flag)
struct tree *eRoot;
int flag;
{
	struct Variables *v;
	
	if(!(v=findSymbol(eRoot->sValue))){
	    printf("IDENTIFIER %s Not Found\n",eRoot->sValue);
	    g.error=TRUE;
	    return 1;
	}
	R0.Class=v->Class;
	R0.Type=v->Type;
	R0.Mode=flag;
	R0.dStack=v->dStack;	
	if((R0.StructureName=v->StructureName)){
	    struct Variables *vs;
	
	    if(!(vs=findStructures(R0.StructureName))){
	        printf("Variable (%s) : Structure (%s) Not Found\n",eRoot->sValue,R0.StructureName);
	        g.error=TRUE;
	        return 1;
	    }
	    R0.Size=vs->Size;
	}
	if(flag == G_VALUE){	/* Load Value	*/
	    if(v->dStack){
	        switch(v->dStack[0].Type){
		case T_POINTER:
		    (v->Class == V_GLOBAL) ? putInt(LEP0) : putInt(LSP0);
		    break;
		case T_FUNCTION:	
		    printf("Cannot Load Function\n");
		    return 1;
		case T_ARRAY:
		    (v->Class == V_GLOBAL) ? putInt(LEA0) : putInt(LSA0);
		    break;
		default:
		    printf("Error Loading %s Type %d",v->c_name,v->Type);
	            g.error=TRUE;
		    return 1;
	        }
	    }else{		/* Simple Variable */
	        R0.Class=V_REGISTER;
	            switch(v->Type){
			case T_CHAR:
			    (v->Class == V_GLOBAL) ? putInt(LEB0) : putInt(LSB0);
			    R0.Type=T_LONG;
			    break;
			case T_INT:	
			    (v->Class == V_GLOBAL) ? putInt(LEI0) : putInt(LSI0);
			    R0.Type=T_LONG;
			    break;
			case T_LONG:
			    (v->Class == V_GLOBAL) ? putInt(LEL0) : putInt(LSL0);
			     break;
			case T_FLOAT:
			    (v->Class == V_GLOBAL) ? putInt(LEF0) : putInt(LSF0);
			    R0.Type=T_DOUBLE;
			    break;
			case T_DOUBLE:
			    (v->Class == V_GLOBAL) ? putInt(LED0) : putInt(LSD0);
			    break;
			case T_STRUCT:
			    (v->Class == V_GLOBAL) ? putInt(LET0) : putInt(LST0);
			    putLong(v->Size);
			    break;
			default:
			printf("Error Loading %s Type %d",v->c_name,v->Type);
	                g.error=TRUE;
			return 1;
	            }
	    }
	}else{			/* Load Address  */
	    (v->Class == V_GLOBAL) ? putInt(LEA0) : putInt(LSA0);
	}
	(v->Class == V_GLOBAL) ? putLong(v->Address) : putLong(v->Address+g.stackDepth);
	return 0;
	
}
int Eval(struct tree *eRoot,int flag)
{
	long cAddress,dAddress,value;
	
	cAddress=g.code;
	dAddress=g.data;
	
	switch(eRoot->Type){
	case MERGE:
	    if(eRoot->treeList[0])Eval(eRoot->treeList[0],flag);
	    if(eRoot->treeList[1])Eval(eRoot->treeList[1],flag);
	    break;
	case ID1:
	    doIdent(eRoot,flag);
	    break;
	case A_TYPE:
	    do_ATYPE(eRoot);
	    break;
	case M_TYPE:
	    do_MTYPE(eRoot);
	    break;
	case U_INDIRECTION:
	    doINDIRECTION(eRoot,flag);
	    break;	
	case U_ADDRESS:
	    doUADDRESS(eRoot,flag);
	    break;
	case U_PLUS:
	    break;
	case U_MINUS:
	    doUMINUS(eRoot,flag);
	    break;
	case U_BIT:
	    doUBIT(eRoot,flag);
	    break;
	case U_NOT:
	    doUNOT(eRoot,flag);
	    break;
	case SIZEOF:
	    value=eRoot->lValue;
	    if(value == T_STRUCT){
		struct Variables *v;
	
		if(!(v=findStructures(eRoot->sValue))){
	    	    printf("Stucture definition  %s Not Found\n",eRoot->sValue);
	            g.error=TRUE;
	    	    break;
		}
	        value=v->Size;
	    }else{
	        value=VariableStorage[value];
	    }
	    doDS();
	    putLong((long)value);
	    doCS();
	    putInt(LEI0);
	    putLong(dAddress);
	    R0=Rzero;
	    R0.Type=T_LONG;
	    R0.Mode = G_VALUE;
	    break;
	case C_INT:
	    doDS();
	    putLong((long)eRoot->iValue);
	    doCS();
	    putInt(LEI0);
	    putLong(dAddress);
	    R0=Rzero;
	    R0.Type=T_LONG;
	    R0.Mode = G_VALUE;
	    break;
	case C_LONG:
	    doDS();
	    putLong(eRoot->lValue);
	    doCS();
	    putInt(LEL0);
	    putLong(dAddress);
	    R0=Rzero;
	    R0.Type=T_LONG;
	    R0.Mode = G_VALUE;
	    break;
	case C_STRING:
	    doSTRING(eRoot->sValue,flag);
	    break;
	case C_DOUBLE:
	    doDS();
	    putDouble(eRoot->dValue,&dAddress);
	    doCS();
	    putInt(LED0);
	    putLong(dAddress);
	    R0=Rzero;
	    R0.Type=T_DOUBLE;
	    R0.Mode = G_VALUE;
	    break;
	case '?':
	    doQUESTION(eRoot);
	    break;
	case AND_OP:
	    doAND(eRoot);
	    break;
	case OR_OP:
	    doOR(eRoot);
	    break;
	case POST_INC:
	     doPost(eRoot,1);
	     break;
	case POST_DEC:
	     doPost(eRoot,-1);
	     break;
	case PRE_INC:
	     doPre(eRoot,1);
	     break;
	case PRE_DEC:
	     doPre(eRoot,-1);
	    break;	
	case SUB:
	    doSubCall(eRoot);
	    break;	
	case SUB_LIST:
	    doSubCall(eRoot);
	    break;
	case FOR:
	    doFor(eRoot);
	    break;
	case '[':
	    doARRAY(eRoot,flag);
	    break;
	case IF:
	    doIF(eRoot);
	    break;
	case ELSE:
	    doELSE(eRoot);
	    break;
	case WHILE:
	    doWHILE(eRoot);
	    break;
	case DO:
	    doDO(eRoot);
	    break;
	case CONTINUE:
	    manageBC(BC_JUMP_CONTINUE);
	    break;
	case BREAK:
	    manageBC(BC_JUMP_BREAK);
	    break;
	case '.':
	    doMEMBER(eRoot,flag,G_ADDRESS);
	    break;
	case MEMBER:
	    doMEMBER(eRoot,flag,G_VALUE);
	    break;
	case RETURN:
	    doRETURN(eRoot,flag);
	    break;
	case SWITCH:
	    doSWITCH(eRoot,flag);
	    break;
	case CASE:
	    doCASE(eRoot,flag);
	    break;
	case DEFAULT:
	    doDEFAULT(eRoot,flag);
	    break;
	default:
	    printf("Eval Error unknown Type %d\n",eRoot->Type);
	    g.error=TRUE;
	    break;
	}
	return 0;
}
static int doDEFAULT(eRoot,flag)
struct tree *eRoot;
int flag;
{
	manageSW(SW_DEFAULT,0L,g.code);
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	return 0;
}
static int doCASE(eRoot,flag)
struct tree *eRoot;
int flag;
{
	struct tree *mRoot;
	long Value;
 
	Value=eRoot->lValue;
	manageSW(SW_CASE,Value,g.code);
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	return 0;
}
static int doSWITCH(eRoot,flag)
struct tree *eRoot;
int flag;
{
	int label1,label2;
	
	label1=getlab();
	label2=getlab();
	
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	jump(label1);
	
	manageSW(SW_PUSH,0L,0L);
	manageBC(BC_PUSH_SWITCH);
	if(eRoot->treeList[1])Eval(eRoot->treeList[1],G_VALUE);
	jump(label2);
	
	putlab(label1);
	manageSW(SW_POP,0L,0L);
	
	putlab(label2);
	manageBC(BC_WRITE_CONTINUE);
	manageBC(BC_WRITE_BREAK);
	manageBC(BC_POP);
	return 0;
}
static int doRETURN(eRoot,flag)
struct tree *eRoot;
int flag;
{
	struct Variables *vs,*vm;
	struct tree *mRoot;
	char *Name;
	
	if(eRoot && eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	jump(g.SubroutineReturn);
	return 0;
}
static int doMEMBER(eRoot,flag,call)
struct tree *eRoot;
int flag,call;
{
	struct Variables *vs,*vm;
	struct tree *mRoot;
	char *Name;
	int Type;
	
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],call);
	Name=((struct tree *)((eRoot->treeList[1])))->sValue;
	if(!(vs=findStructures(R0.StructureName))){
	    printf("Member (%s) : Structure (%s) Not Found\n",Name,R0.StructureName);
	    g.error=TRUE;
	    return 1;
	}
	
	Type =	(R0.dStack && R0.dStack->Type) ?  (R0.dStack->Type) : R0.Type;
	
	if(call == G_VALUE){
	    if(Type != T_POINTER){
	        printf("Member (%s) Requires Pointer To Structure (%s)\n",Name,R0.StructureName);
	        g.error=TRUE;
	        return 1;
	    }
	}else{
	    if(Type != T_STRUCT){
	        printf("Member (%s) Requires Structure (%s)\n",Name,R0.StructureName);
	        g.error=TRUE;
	        return 1;
	    }
	}
	
	if(!(vm=(struct Variables *)find_sym(vs->Members,Name))){
	    printf("Structure Member (%s) Not Found\n",Name);
	    g.error=TRUE;
	    return 1;
	}
	
	if(vm->Address){
	    putInt(ACP);
	    putLong(vm->Address);
	}
	
	R0.Class=vm->Class;
	R0.Type=vm->Type;
	R0.Mode=flag;
	R0.Size=vm->Size;
	R0.dStack=vm->dStack;
	R0.StructureName=vm->StructureName;
	if(flag == G_VALUE){
	    loadR0();
	}else{
	    R0.Mode=G_ADDRESS;
	}
	return 0;
}
static int doINDIRECTION(eRoot,flag)
struct tree *eRoot;
int flag;
{
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	if(R0.dStack){
	    if(R0.dStack->Type){
	        ++(R0.dStack);
	        if(!(R0.dStack->Type))R0.dStack=NULL;
	    }else{
	        R0.dStack=NULL;
	    }
	}
	if(flag == G_VALUE){
	    loadR0();
	}else{
	    R0.Mode=G_ADDRESS;
	}
	return 0;
}
static int doUNOT(eRoot,flag)
struct tree *eRoot;
int flag;
{
	int Type;
 
	
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	
	Type =	(R0.dStack && R0.dStack->Type) ?  (R0.dStack->Type) : R0.Type;
	
	switch(Type){
	    case T_CHAR:
	    case T_INT:	
	    case T_LONG:
	        putInt(NOT_L0);
		break;
	    case T_FLOAT:
	    case T_DOUBLE:
	        putInt(NOT_D0);
		break;
	    case T_POINTER:
	        putInt(NOT_P0);
		break;
	    default:
	        printf("Cannot ! %s",TypeList[Type]);
	        g.error=TRUE;
		break;
	}
	R0.dStack=NULL;
	R0.Type=T_LONG;
	return 0;
}
 
static int doUBIT(eRoot,flag)
struct tree *eRoot;
int flag;
{
	int Type;
 
	
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	
	Type =	(R0.dStack && R0.dStack->Type) ?  (R0.dStack->Type) : R0.Type;
	
	switch(Type){
	    case T_CHAR:
	    case T_INT:	
	    case T_LONG:
	        putInt(UCL0);
		break;	
	    default:	
	        printf("Cannot Complement %s",TypeList[Type]);
	        g.error=TRUE;
		break;
	}
	return 0;
}
static int doUMINUS(eRoot,flag)
struct tree *eRoot;
int flag;
{
	int Type;
 
	
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	
	Type =	(R0.dStack && R0.dStack->Type) ?  (R0.dStack->Type) : R0.Type;
	
	switch(Type){
	    case T_CHAR:
	    case T_INT:	
	    case T_LONG:
	        putInt(UML0);
		break;
	    case T_FLOAT:
	    case T_DOUBLE:
	        putInt(UMD0);
		break;
	    case T_POINTER:
	    case T_ARRAY:
	    case T_STRUCT:
	        printf("Cannot Negate %s",TypeList[Type]);
	        g.error=TRUE;
		break;
	}
	return 0;
}
static int doUADDRESS(eRoot,flag)
struct tree *eRoot;
int flag;
{
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_ADDRESS);
	R0.Mode=G_ADDRESS;
	PointerR0();
	return 0;
}
static int PointerR0()
{
	extern struct c_list *pfind;
	struct Variables *v;
	struct dStack *dStack;
	char buff[256];
	int Count,k;
	long Length;
	
	Count=0;
	if((dStack=R0.dStack)){
	    while(dStack->Type){
	        ++Count;
	        ++dStack;
	    }
	}
	Length=(2+Count)*sizeof(struct dStack);
	if(!(dStack=(struct dStack *)malloc(Length))){
	    printf("PointerR0 Out Of Memory Requested (%ld)\n",Length);
	    g.error=TRUE;
	    return 0;	
	}
	zerol((char *)dStack,Length);
	dStack[0].Type=T_POINTER;
	for(k=0;k<Count;++k)dStack[k+1]=R0.dStack[k];
	
	sprintf(buff,".%d",getlab());
	g.treesize=sizeof(struct Variables);
	g.LocalVariables=(struct Variables *)tree(g.LocalVariables,buff);
	v=(struct Variables *)pfind;
	v->dStack=dStack;
	R0.dStack=dStack;
	return 0;
}
 
static int doARRAY(eRoot,flag)
struct tree *eRoot;
int flag;
{
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	push1();
	if(eRoot->treeList[1])Eval(eRoot->treeList[1],G_VALUE);
	doBinaryOp(BOP_PLUS);
	if(R0.dStack){
	    if(R0.dStack->Type){
	        ++(R0.dStack);
	        if(!(R0.dStack->Type))R0.dStack=NULL;
	    }else{
	        R0.dStack=NULL;
	    }
	}
	if(flag == G_VALUE){
	    loadR0();
	}else{
	    R0.Mode=G_ADDRESS;
	}
	return 0;
}
static int storeR2()
{
	int Type;
 
	Type =	(R2.dStack && R2.dStack->Type) ?  (R2.dStack->Type) : R2.Type;
	
	switch(Type){
	    case T_CHAR:
	        putInt(SIC2);
		break;
	    case T_INT:	
	        putInt(SII2);
		break;
	    case T_LONG:
	        putInt(SIL2);
		break;
	    case T_FLOAT:
	        putInt(SIF2);
		break;
	    case T_DOUBLE:
	        putInt(SID2);
		break;
	    case T_POINTER:
	        putInt(SIP2);
		break;
	    case T_ARRAY:
	        putInt(SIP2);
		break;
	    case T_STRUCT:
	        putInt(SIT2);
	        putLong(R0.Size);
		break;
	}
	return 0;
}
static int loadR0()
{
	int Type;
 
	Type =	(R0.dStack && R0.dStack->Type) ?  (R0.dStack->Type) : R0.Type;
	
	switch(Type){
	    case T_CHAR:
	        putInt(LIC0);
	        R0.Type = T_LONG;
		break;
	    case T_INT:	
	        putInt(LII0);
	        R0.Type = T_LONG;
		break;
	    case T_LONG:
	        putInt(LIL0);
	        R0.Type = T_LONG;
		break;
	    case T_FLOAT:
	        putInt(LIF0);
	        R0.Type = T_DOUBLE;
		break;
	    case T_DOUBLE:
	        putInt(LID0);
	        R0.Type = T_DOUBLE;
		break;
	    case T_POINTER:
	        putInt(LIP0);
	        R0.Type = T_POINTER;
		break;
	    case T_ARRAY:
	        R0.Mode=G_ADDRESS;
		return 0;
	    case T_STRUCT:
	        putInt(LIT0);
	        putLong(R0.Size);
		break;
	}
	R0.Mode=G_VALUE;
	return 0;
}
static int doWHILE(eRoot)
struct tree *eRoot;
{
	int label;
	
	label=getlab();
	putlab(label);
	manageBC(BC_PUSH);
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	jumpfalse(manageBC(BC_GET_BREAK));	
	if(eRoot->treeList[1])Eval(eRoot->treeList[1],G_VALUE);
	manageBC(BC_WRITE_CONTINUE);
	jump(label);	
	manageBC(BC_WRITE_BREAK);
	manageBC(BC_POP);
	return 0;
}
static int doDO(eRoot)
struct tree *eRoot;
{
	int label;
	
	label=getlab();
	putlab(label);
	manageBC(BC_PUSH);
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	manageBC(BC_WRITE_CONTINUE);
	if(eRoot->treeList[1])Eval(eRoot->treeList[1],G_VALUE);
	jumptrue(label);	
	manageBC(BC_WRITE_BREAK);
	manageBC(BC_POP);
	return 0;
}
static int doAND(eRoot)
struct tree *eRoot;
{
	int label;
	
	label=getlab();
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	jumpfalse(label);	
	if(eRoot->treeList[1])Eval(eRoot->treeList[1],G_VALUE);
	putlab(label);
	return 0;
}
static int doOR(eRoot)
struct tree *eRoot;
{
	int label;
	
	label=getlab();
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	jumptrue(label);	
	if(eRoot->treeList[1])Eval(eRoot->treeList[1],G_VALUE);
	putlab(label);
	return 0;
}
static int doQUESTION(eRoot)
struct tree *eRoot;
{
	int label1,label2;
	
	label1=getlab();
	label2=getlab();
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	jumpfalse(label1);	
	if(eRoot->treeList[1])Eval(eRoot->treeList[1],G_VALUE);
	jump(label2);	
	putlab(label1);
	if(eRoot->treeList[1])Eval(eRoot->treeList[2],G_VALUE);
	putlab(label2);
	return 0;
}
static int doELSE(eRoot)
struct tree *eRoot;
{
	int label1,label2;
	
	label1=getlab();
	label2=getlab();
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	jumpfalse(label1);	
	if(eRoot->treeList[1])Eval(eRoot->treeList[1],G_VALUE);
	jump(label2);	
	putlab(label1);
	if(eRoot->treeList[1])Eval(eRoot->treeList[2],G_VALUE);
	putlab(label2);
	return 0;
}
static int doIF(eRoot)
struct tree *eRoot;
{
	int label;
	
	label=getlab();
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	jumpfalse(label);	
	if(eRoot->treeList[1])Eval(eRoot->treeList[1],G_VALUE);
	putlab(label);
	return 0;
}
static int doPost(eRoot,value)
struct tree *eRoot;
int value;
{
	struct tree one;
	
	doPre(eRoot,value);
	push1();
	one.Type=C_LONG;
	one.lValue=value;
	Eval(&one,G_VALUE);
	doBinaryOp(BOP_MINUS);
	return 0;
}
static int doPre(eRoot,value)
struct tree *eRoot;
int value;
{
	struct tree one;
	
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_ADDRESS);
	push1();
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	push1();
	one.Type=C_LONG;
	one.lValue=value;
	Eval(&one,G_VALUE);
	doBinaryOp(BOP_PLUS);
	store();
	
	return 0;
}
static int doFor(eRoot)
struct tree *eRoot;
{
	int start;
	
	start=getlab();
	manageBC(BC_PUSH);
	
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	
	putlab(start);
	
	if(eRoot->treeList[1]){
	    Eval(eRoot->treeList[1],G_VALUE);
	    jumpfalse(manageBC(BC_GET_BREAK));
	}
	
	if(eRoot->treeList[3]){
	    Eval(eRoot->treeList[3],G_VALUE);
	}
	
	manageBC(BC_WRITE_CONTINUE);
	
	if(eRoot->treeList[2]){
	    Eval(eRoot->treeList[2],G_VALUE);
	}
	
	jump(start);
	
	manageBC(BC_WRITE_BREAK);
	
	manageBC(BC_POP);
	
	return 0;
}
static int doSubCall(eRoot)
struct tree *eRoot;
{
	struct tree *list[500];
	struct Variables *v;
	char *name;
	long sLength,stackSave;
	int k,n;
	
	k=0;
	sLength=0;
	name=eRoot->treeList[0]->sValue;
	if(eRoot->Count > 1)unMerge(eRoot->treeList[1],&k,list);
	if(k > 0){
	    stackSave=rStackCount;
	    for(n=0;n<k;++n){
	        Eval(list[k-n-1],G_VALUE);
	        push1();
	        sLength += R0.pLength;
	    }
	    rStackCount=stackSave;
	}
	putInt(CF);
	writeLabel(name);
	putInt(1);
	putInt(FE);
	putLong(sLength);
	g.stackDepth -= sLength;
	
	if(!(v=findSymbol(name))){
	    R0=Rzero;
	    R0.Type=T_LONG;
	    R0.Mode = G_VALUE;
	    return 0;
	}
	
	R0.Class=v->Class;
	R0.Type=v->Type;
	R0.Mode=G_VALUE;
	R0.dStack=v->dStack;	
	if((R0.StructureName=v->StructureName)){
	    struct Variables *vs;
	
	    if(!(vs=findStructures(R0.StructureName))){
	        printf("Structure Name (%s) Not Found\n",R0.StructureName);
	        g.error=TRUE;
	        return 1;
	    }
	    R0.Size=vs->Size;
	}
	
	if(R0.dStack){
	    if(R0.dStack->Type){
	        ++(R0.dStack);
	        if(!(R0.dStack->Type))R0.dStack=NULL;
	    }else{
	        R0.dStack=NULL;
	    }
	}
	
	return 0;
}
static int unMerge(eRoot,k,list)
struct tree *eRoot,**list;
int *k;
{
	if(!eRoot)return 1;
	if(eRoot->Type == MERGE){
	    if(eRoot->treeList[0])unMerge(eRoot->treeList[0],k,list);
	    if(eRoot->treeList[1])unMerge(eRoot->treeList[1],k,list);
	
	}else{
	    list[(*k)++] = eRoot;
	}
	return 0;
}
static int do_MTYPE(eRoot)
struct tree *eRoot;
{
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	push1();
	if(eRoot->treeList[1])Eval(eRoot->treeList[1],G_VALUE);
	doBinaryOp(eRoot->SubType);
	return 0;
}
static int do_ATYPE(eRoot)
struct tree *eRoot;
{
	if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_ADDRESS);
	push1();
	if(eRoot->SubType != '='){
	    if(eRoot->treeList[0])Eval(eRoot->treeList[0],G_VALUE);
	    push1();
	    if(eRoot->treeList[1])Eval(eRoot->treeList[1],G_VALUE);
	    doBinaryOp(eRoot->SubType);
	}else{
	    if(eRoot->treeList[1])Eval(eRoot->treeList[1],G_VALUE);
	}
	store();
	return 0;
}
static int doBinaryOp(Type)
int Type;
{
	static int Long[]={
		LONG_MOD,
		LONG_OR,
		LONG_XOR,
		LONG_AND,
		LONG_LEFT,
		LONG_RIGHT,
		LONG_PLUS,
		LONG_MINUS,
		LONG_MULT,
		LONG_DIV,
		LONG_LESS,
		LONG_GREATER,
		LONG_NE,
		LONG_EQ,
		LONG_LE,
		LONG_GE
	   };
	
	static int Double[]={
		DOUBLE_MOD,
		DOUBLE_OR,
		DOUBLE_XOR,
		DOUBLE_AND,
		DOUBLE_LEFT,
		DOUBLE_RIGHT,
		DOUBLE_PLUS,
		DOUBLE_MINUS,
		DOUBLE_MULT,
		DOUBLE_DIV,
		DOUBLE_LESS,
		DOUBLE_GREATER,
		DOUBLE_NE,
		DOUBLE_EQ,
		DOUBLE_LE,
		DOUBLE_GE
	   };
	
	static int Pointer[]={
		POINTER_MOD,
		POINTER_OR,
		POINTER_XOR,
		POINTER_AND,
		POINTER_LEFT,
		POINTER_RIGHT,
		POINTER_PLUS,
		POINTER_MINUS,
		POINTER_MULT,
		POINTER_DIV,
		POINTER_LESS,
		POINTER_GREATER,
		POINTER_NE,
		POINTER_EQ,
		POINTER_LE,
		POINTER_GE
	   };
	
	
	int Type0,Type2;
	
	long PointerSize();
	
	
	pop2();
	BinaryConversions();
	
	Type0 = (R0.dStack && R0.dStack->Type) ?  (R0.dStack->Type) : R0.Type;
	Type2 = (R2.dStack && R2.dStack->Type) ?  (R2.dStack->Type) : R2.Type;
	
	switch(Type2){
	case T_LONG:
	    if((Type0 == T_POINTER) || (Type0 == T_ARRAY)){
	        putInt(PSCALE2);
	        putLong(PointerSize(&R0,R0.dStack+1));
	    } else if(Type0 != T_LONG) {
	        printf("Error Long Binary op With %s\n",TypeList[Type0]);
	        g.error=TRUE;
	        break;
	    }
	    putInt(Long[Type]);
	    break;
	case T_DOUBLE:
	    if(Type0 != T_DOUBLE){
	        printf("Error Double Binary op With %s\n",TypeList[Type0]);
	        g.error=TRUE;
	        break;
	    }
	    putInt(Double[Type]);
	    if(Type >= 10)R2.Type=T_LONG;
	    break;
	case T_ARRAY:
	case T_POINTER:
	    if(Type0 == T_LONG){
	        putInt(PSCALE0);
	        putLong(PointerSize(&R2,R2.dStack+1));
	    	putInt(Pointer[Type]);
	    }else if((Type0 == T_POINTER) || (Type0 == T_ARRAY)){
	    	putInt(Pointer[Type]);
	    	if(Type < 10){
	            putInt(PDIVIDE0);
	            putLong(PointerSize(&R2,R2.dStack+1));
		}
		R2.Type=T_LONG;
	        R2.dStack=NULL;
	    }else{
	        printf("Pointer Binary op With %s\n",TypeList[Type0]);
	        g.error=TRUE;
	    }
	    break;
	    default:
	        printf("Binary op Error %s And %s\n",TypeList[Type0],TypeList[Type2]);
	        g.error=TRUE;
	    break;
	}
	R0=R2;
	return 0;
}
static long PointerSize(r,dStack)
struct Register *r;
struct dStack *dStack;
{
	struct Variables *vn;
	struct dStack *Stack;
	long Length;
	int Type;
 
	Type =	(dStack && dStack->Type) ?  (dStack->Type) : r->Type;
	
	switch(Type){
	
	    case T_CHAR:
	    case T_INT:	
	    case T_LONG:
	    case T_FLOAT:
	    case T_DOUBLE:
	    case T_POINTER:
		Length=VariableStorage[Type];
		break;
	    case T_ARRAY:
	        Length=PointerSize(r,dStack+1)*dStack->Count;
		break;
	    case T_STRUCT:	
	        if(!(vn=findStructures(r->StructureName))){
		    printf("Structure (%s) Not Defined\n",r->StructureName);
	            g.error=TRUE;
		    return 0;
	        }
	        Length = vn->Size;
		break;
	}
	return Length;
}
static int BinaryConversions()
{
	int Type0,Type2;
	
	Type0 = (R0.dStack && R0.dStack->Type) ?  (R0.dStack->Type) : R0.Type;
	Type2 = (R2.dStack && R2.dStack->Type) ?  (R2.dStack->Type) : R2.Type;
	
	switch(Type2){
	case T_LONG:
	    if(Type0 == T_DOUBLE){
	        putInt(CVTLD2);
	        R2.Type=T_DOUBLE;
	        R2.dStack = NULL;
	    }
	    break;
	case T_DOUBLE:
	    if(Type0 == T_LONG){
	        putInt(CVTLD0);
	        R0.Type=T_DOUBLE;
	        R0.dStack = NULL;
	    }
	    break;
	}
	return 0;
}
static int writeLabel(name)
char *name;
{
	struct c_list *a_np;
 
 
	if((a_np=find_sym(g.l_root,name))){
	    if(relative){
	        putLong(a_np->c_long+c_long);
	    }else{
	        putLong(a_np->c_long);
	    }
	}else if((a_np=find_sym(g.ll_root,name))){
	    if(relative){
	        putLong(a_np->c_long+c_long);
	    }else{
	        putLong(a_np->c_long);
	    }
	}else{
	    goForward(name);
	    if(relative){
	        putLong((long)c_long);
	    }else{
	        putLong(0L);
	    }
	}
	return 0;
}
static int goForward(name)
char *name;
{
	char *strsave();
	
	if(rCount >= rCountMax){
	    moreForward();
	}
	reference[rCount].label=strsave(name);
	reference[rCount].segment=segment;
	reference[rCount].relative=relative;
	if(segment == CODESEGMENT){
	    reference[rCount++].code=g.code;
	}else{
	    reference[rCount++].code=g.data;
	}
	return 0;
}
static int moreForward()
{
	printf("moreForward\n");
	longjmp(env77,1);
	return 0;
}
static int more()
{
	
	codemax += 5000;
	if(!(g.codebuff=(unsigned char *)realloc((char *)g.codebuff,codemax))){
	    printf("MoreCode Out of Memory %ld\n",codemax);
	    g.error=TRUE;
	    longjmp(env77,1);
	}
	zerol((char *)(g.codebuff+codemax-5000),5000L);
	return 0;
}
static int mored()
{
	
	datamax += 5000;
	if(!(g.databuff=(unsigned char *)realloc((char *)g.databuff,datamax))){
	    printf("MoreData Out of Memory %ld\n",datamax);
	    g.error=TRUE;
	    longjmp(env77,1);
	}
	zerol((char *)(g.databuff+datamax-5000),5000L);
	return 0;
}
static int putDouble(d,dAddress)
double d;
long *dAddress;
{
	double *p;
        int n,k;
 
 
	if(segment == CODESEGMENT){
	    if((g.code += sizeof(double)) >= codemax){
	        more();
	    }
	    p=(double *)&g.codebuff[g.code - sizeof(double)];
	    memcpy((char *)p,(char *)&d,(long)sizeof(double));
	}else{

	    if((n =g.data % sizeof(double))){
		n = sizeof(int)-n;
                for(k=0;k<n;++k)putByte(0);
	    }


	    *dAddress=g.data;
	    
	    if((g.data += sizeof(double)) >= datamax){
	        mored();
	    }
	    
	    
	    p=(double *)&g.databuff[g.data - sizeof(double)];
	    
	    *p = d;
	    
/*
	    memcpy((char *)p,(char *)&d,(long)sizeof(double));
*/
	}
	return 0;
}
int putLong(long l)
{
	unsigned long *p;
 
 
	if(segment == CODESEGMENT){
	    if((g.code += sizeof(long)) >= codemax){
	        more();
	    }
	    p=(unsigned long *)&g.codebuff[g.code - sizeof(long)];
	    memcpy((char *)p,(char *)&l,(long)sizeof(long));
	}else{
	    if((g.data += sizeof(long)) >= datamax){
	        mored();
	    }
	    p=(unsigned long *)&g.databuff[g.data - sizeof(long)];
	    
	    *p = l;
	    
	    /*
	    memcpy((char *)p,(char *)&l,(long)sizeof(long));
	    */
	}
	return 0;
}
int putInt(int i)
{
	unsigned int *p,u;
 
        u=i;
	if(segment == CODESEGMENT){
	    if((g.code += sizeof(int)) >= codemax){
	        more();
	    }
	    p=(unsigned int *)&g.codebuff[g.code - sizeof(int)];
	    memcpy((char *)p,(char *)&u,(long)sizeof(int));
	}else{
	    if((g.data += sizeof(int)) >= datamax){
	        mored();
	    }
	    p=(unsigned int *)&g.databuff[g.data - sizeof(int)];
	    memcpy((char *)p,(char *)&u,(long)sizeof(int));
	}
	return 0;
}
static int aline()
{
      long int k,n;
 
	if(segment == CODESEGMENT){
	    if((n= g.code % sizeof(int)) ){
		n = sizeof(int)-n;
                for(k=0;k<n;++k)putByte(0);
	        printf("code Segment out of alinement\n");
	        g.error=TRUE;
	    }
	}else{
	    if((n =g.data % sizeof(int))){
		n = sizeof(int)-n;
                for(k=0;k<n;++k)putByte(0);
	    }
	}
	return 0;
}
static int putString(s)
char *s;
{
	while(*s)putByte(*s++);
	putByte(0);
	return 0;
}
static int putByte(b)
int b;
{
	unsigned char c;
 
	c=b;
	if(segment == CODESEGMENT){
	    if((g.code += sizeof(char)) >= codemax){
	        more();
	    }	
	    g.codebuff[g.code - 1] = c;
	}else{
	    if((g.data += sizeof(char)) >= datamax){
	        mored();
	    }
	    g.databuff[g.data - 1] = c;
	}
	return 0;
}
static int jump(label)
int label;
{
	char buff[256];
	
	if(label){
	    sprintf(buff,".%d",label);
	    putInt(JP);
	    writeLabel(buff);
	}
	return 0;
}
static int jumptrue(label)
int label;
{
	char buff[256];
	int Type0;
	
	Type0 = (R0.dStack && R0.dStack->Type) ?  (R0.dStack->Type) : R0.Type;
	
	if(label){
	    sprintf(buff,".%d",label);
	    if(Type0 == T_DOUBLE){
	       putInt(NZD);
	    }else if(Type0 == T_FLOAT){
	       putInt(NZF);
	    }
	    putInt(JT);
	    writeLabel(buff);
	}
	return 0;
}
static int jumpfalse(label)
int label;
{
	char buff[256];
	int Type0;
	
	Type0 = (R0.dStack && R0.dStack->Type) ?  (R0.dStack->Type) : R0.Type;
	
	if(label){
	    sprintf(buff,".%d",label);
	    if(Type0 == T_DOUBLE){
	       putInt(NZD);
	    }else if(Type0 == T_FLOAT){
	       putInt(NZF);
	    }
	    putInt(JF);
	    writeLabel(buff);
	}
	return 0;
}
 
int manageBC(int flag)
{
	static int *Break,*Continue;
	static int Count,MaxCount=10;
	long Length;
	
	switch(flag){
	    case BC_START:
	        Length=MaxCount*sizeof(int);
	        if(!(Break = (int *)malloc(Length))){
	            printf("manageBC Error Memory requested (%ld)\n",Length);
	            g.error=TRUE;
	            return -1;
	        }
	        if(!(Continue = (int *)malloc(Length))){
	            printf("manageBC Error Memory requested (%ld)\n",Length);
	            g.error=TRUE;
	            return -1;
	        }
	        Count=0;
	        break;
	    case BC_FINISH:
	        if(Break)free((char *)Break);
	        if(Continue)free((char *)Continue);
	        Break=NULL;
	        Continue=NULL;
	        break;
	    case BC_PUSH:
	    case BC_PUSH_SWITCH:
	        if(Count >= MaxCount){
	            MaxCount = Count+ 5;
		    Length=MaxCount*sizeof(int);
		    if(!(Break = (int *)realloc((char *)Break,Length))){
			printf("manageBC Error Memory requested (%ld)\n",Length);
	                g.error=TRUE;
			return -1;
		    }
		    if(!(Continue = (int *)realloc((char *)Continue,Length))){
			printf("manageBC Error Memory requested (%ld)\n",Length);
	                g.error=TRUE;
			return -1;
		    }	
	        }
	        Break[Count]=getlab();
	        Continue[Count++]=getlab();
	        if(flag == BC_PUSH_SWITCH){
	            if(Count >= 2)Continue[Count-1]=Continue[Count-2];
	        }
	        break;
	    case BC_POP:
	        if(--Count < 0){
		   printf("manageBC Error Stack UnderFlow\n");
	           g.error=TRUE;
		   Count=0;
		   return -1;
	        }
	        break;
	    case BC_CLEAR:
	    	Count=0;
	        break;
	    case BC_CHECK:
	         if(Count != 0){
	             printf("Error %d UnTerminated Loop(s)\n",Count);
	             g.error=TRUE;
	             return -1;
	         }
	        break;
	    case BC_WRITE_CONTINUE:
	        if(Count <= 0){
	             printf("manageBC Error Continue Not in Loop\n");
	             g.error=TRUE;
	             return -1;
	        }
	        putlab(Continue[Count-1]);
	        break;
	    case BC_WRITE_BREAK:
	        if(Count <= 0){
	             printf("manageBC Error Break Not in Loop\n");
	             g.error=TRUE;
	             return -1;
	        }
	        putlab(Break[Count-1]);
	        break;
	    case BC_GET_CONTINUE:
	        if(Count <= 0){
	             printf("manageBC Error Continue Not in Loop\n");
	             g.error=TRUE;
	             return -1;
	        }
	        return Continue[Count-1];
	        break;
	    case BC_GET_BREAK:
	        if(Count <= 0){
	             printf("manageBC Error Break Not in Loop\n");
	             g.error=TRUE;
	             return -1;
	        }
	        return Break[Count-1];
	        break;
	    case BC_JUMP_CONTINUE:
	        if(Count <= 0){
	             printf("manageBC Error Continue Not in Loop\n");
	             g.error=TRUE;
	             return -1;
	        }
	        jump(Continue[Count-1]);
	        break;
	    case BC_JUMP_BREAK:
	        if(Count <= 0){
	             printf("manageBC Error Break Not in Loop\n");
	             g.error=TRUE;
	             return -1;
	        }
	        jump(Break[Count-1]);
	        break;
	    default:
	        printf("manageBC Unknown Message %d\n",flag);
	        g.error=TRUE;
	        return -1;
	}
	return 0;
}
/*   Get Unique Label    */
int getlab()
{
	static int lcount=1;
	
	return(++lcount);
}
int putlab(int label)
{
	struct c_list *ll_np;
	char buff[256];
	
	if(label){
	    sprintf(buff,".%d",label);
	    if(!(ll_np=put_ll(buff))){
	        printf("putlab Error\n");
	        return 1;
	    }
	    ll_np->c_long=g.code;
	}
	return 0;
}
static struct c_list *put_ll(a)
char *a;
{
	extern struct c_list *pfind;
	
	g.treesize=sizeof(*g.ll_root);
	g.ll_root=tree(g.ll_root,a);
	return pfind;
}
int manageSW(flag,Value,Address)
int flag;
long Value,Address;
{
	static long **sValue,**sAddress,*sCount,*sMaxCount,*sDefault;
	static int Level,MaxLevel=10;
	long Length,Count,MaxCount,NewCount,k;
	
	switch(flag){
	    case SW_START:
	        Length=MaxLevel*sizeof(long *);
	        if(!(sValue = (long **)malloc(Length))){
	            printf("manageSW Error Memory requested (%ld)\n",Length);
	            g.error=TRUE;
	            return -1;
	        }
	        if(!(sAddress = (long **)malloc(Length))){
	            printf("manageSW Error Memory requested (%ld)\n",Length);
	            g.error=TRUE;
	            return -1;
	        }
	        zerol((char *)sValue,Length);
	        zerol((char *)sAddress,Length);
	        Length=MaxLevel*sizeof(long);
	        if(!(sCount = (long *)malloc(Length))){
	            printf("manageSW Error Memory requested (%ld)\n",Length);
	            g.error=TRUE;
	            return -1;
	        }
	        if(!(sMaxCount = (long *)malloc(Length))){
	            printf("manageSW Error Memory requested (%ld)\n",Length);
	            g.error=TRUE;
	            return -1;
	        }
	        if(!(sDefault = (long *)malloc(Length))){
	            printf("manageSW Error Memory requested (%ld)\n",Length);
	            g.error=TRUE;
	            return -1;
	        }
	        zerol((char *)sCount,Length);
	        zerol((char *)sMaxCount,Length);
	        zerol((char *)sDefault,Length);
	        Level=0;
	        break;
	    case SW_FINISH:
	        if(sValue)free((char *)sValue);
	        if(sAddress)free((char *)sAddress);
	        if(sCount)free((char *)sCount);
	        if(sMaxCount)free((char *)sMaxCount);
	        if(sDefault)free((char *)sDefault);
	        sValue=NULL;
	        sAddress=NULL;
	        sCount=NULL;
	        sMaxCount=NULL;
	        sDefault=NULL;
	        break;
	    case SW_PUSH:
	        if(Level >= MaxLevel){
	            MaxLevel = Level+ 5;
		    Length=MaxLevel*sizeof(long *);
		    if(!(sValue = (long **)realloc((char *)sValue,Length))){
			printf("manageSW Error Memory requested (%ld)\n",Length);
	                g.error=TRUE;
			return -1;
		    }
		    if(!(sAddress = (long **)realloc((char *)sAddress,Length))){
			printf("manageSW Error Memory requested (%ld)\n",Length);
	                g.error=TRUE;
			return -1;
		    }	
		    Length=MaxLevel*sizeof(long);
		    if(!(sCount = (long *)realloc((char *)sCount,Length))){
			printf("manageSW Error Memory requested (%ld)\n",Length);
	                g.error=TRUE;
			return -1;
		    }
		    if(!(sMaxCount = (long *)realloc((char *)sMaxCount,Length))){
			printf("manageSW Error Memory requested (%ld)\n",Length);
	                g.error=TRUE;
			return -1;
		    }	
	        }
	        sCount[Level]=0;
	        sMaxCount[Level]=0;
	        sDefault[Level]=0;
	        ++Level;
	        break;
	    case SW_POP:
	        if(Level <= 0){
		   printf("manageSW Error Stack UnderFlow\n");
	           g.error=TRUE;
		   Level=0;
		   return -1;
	        }
	        Count = sCount[Level-1];
	        MaxCount = sMaxCount[Level-1];
	        for(k=0;k<Count;++k){
	            Value=*(sValue[Level-1]+k);
	            Address=*(sAddress[Level-1]+k);
	            putInt(JUMP_CASE);
	            putLong(Value);
	            putLong(Address);
	        }
	
	        if(sDefault[Level-1]){
/*
	            printf("Default Address %ld\n",sDefault[Level-1]);
*/
	            putInt(JUMP_DEFAULT);
	            putLong(sDefault[Level-1]);
	        }
	
	        for(k=0;k<MaxCount;++k){
	            if(sValue[Level-1])free((char *)sValue[Level-1]);
	            sValue[Level-1]=NULL;
	            if(sAddress[Level-1])free((char *)sAddress[Level-1]);
	            sAddress[Level-1]=NULL;
	        }
	        --Level;
	        break;
	    case SW_CLEAR:
	    	Level=0;
	        break;
	    case SW_CHECK:
	         if(Level != 0){
	             printf("Error %d UnTerminated Switch Loop(s)\n",Level);
	             g.error=TRUE;
	             return -1;
	         }
	        break;
	    case SW_DEFAULT:
	        if(Level <= 0){
		   printf("manageSW Error Default Out Of Switch\n");
	           g.error=TRUE;
		   Level=0;
		   return -1;
	        }
	        sDefault[Level-1]=Address;
	        break;
	    case SW_CASE:
	        if(Level <= 0){
		   printf("manageSW Error Case Out Of Switch\n");
	           g.error=TRUE;
		   Level=0;
		   return -1;
	        }
	        Count = sCount[Level-1];
	        MaxCount = sMaxCount[Level-1];
	        if(Count >= MaxCount){
	            NewCount=MaxCount+5;
		    Length=NewCount*sizeof(long);
	            if(MaxCount){
		        if(!(sValue[Level-1] = (long *)realloc((char *)sValue[Level-1],Length))){
			    printf("manageSW Error Memory requested (%ld)\n",Length);
	                    g.error=TRUE;
			    return -1;
		        }
		        if(!(sAddress[Level-1] = (long *)realloc((char *)sAddress[Level-1],Length))){
			    printf("manageSW Error Memory requested (%ld)\n",Length);
	            	    g.error=TRUE;
			    return -1;
		        }	
	            }else{
		        if(!(sValue[Level-1] = (long *)malloc(Length))){
			    printf("manageSW Error Memory requested (%ld)\n",Length);
	            	    g.error=TRUE;
			    return -1;
		        }
		        if(!(sAddress[Level-1] = (long *)malloc(Length))){
			    printf("manageSW Error Memory requested (%ld)\n",Length);
	           	    g.error=TRUE;
			    return -1;
		        }	
	            }
	            MaxCount=NewCount;
	        }
	        *(sValue[Level-1]+Count)=Value;
	        *(sAddress[Level-1]+Count)=Address;
	        sCount[Level-1] = Count+1;
	        sMaxCount[Level-1] = MaxCount;
	        break;
	    default:
	        printf("manageSW Unknown Message %d\n",flag);
	        g.error=TRUE;
	        return -1;
	}
	return 0;
}
int doSpace(long l)
{
      long k;
 
      if((k = l % 4))l += k;
 
	if(segment == CODESEGMENT){
	    while((g.code + l) >= codemax){
	        more();
	    }
	    g.code += l;
	}else{
	    while((g.data + l) >= datamax){
	        mored();
	    }
	    g.data += l;
	}
	return 0;
}
int doBackPatch(int flag)
{
	struct c_list *m_root,*a_np;
	unsigned char *pc;
	long *p,l;
	int k,n,r;
	
	for(k=0,n=0;k<rCount;++k){
	    if((a_np=find_sym(g.l_root,reference[k].label))){
	        if(reference[k].segment != CODESEGMENT)continue;
	        pc=g.codebuff+reference[k].code;
	        p=(long *)pc;
	        *p = a_np->c_long;
	        free(reference[k].label);
	        continue;
	    }else if((a_np=find_sym(g.ll_root,reference[k].label))){
	        if(reference[k].segment != CODESEGMENT)continue;
	        pc=g.codebuff+reference[k].code;
	        p=(long *)pc;
	        *p = a_np->c_long;
	        free(reference[k].label);
	        continue;
	    }
	    reference[n].relative=reference[k].relative;
	    reference[n].segment=reference[k].segment;
	    reference[n].label=reference[k].label;
	    reference[n++].code=reference[k].code;
	
	}
	rCount=n;
	return 0;
}
int doCS()
{
	segment = CODESEGMENT;
	return 0;
}
int doDS()
{
	segment = DATASEGMENT;
	return 0;
}
 
