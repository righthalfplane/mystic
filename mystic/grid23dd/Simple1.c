#include <stdio.h>
#include <string.h>
#include "Simple.h"
#include "Simple.tab.h"
#include "Simpleop.h"
/* variable tree functions */
struct c_list *l_np;
struct c_list *pfind;
 
char *TypeList[]={
	NULL,
	"T_CHAR",
	"T_INT",		
	"T_LONG",
	"T_FLOAT",
	"T_DOUBLE",
	"T_POINTER",
	"T_FUNCTION",
	"T_ARRAY",
	"T_STRUCT",
	"T_STRUCT_DEFINITION",
	};
 
 
int VariableStorage[]=	{
	0,
	(int)sizeof(char),	/* T_CHAR */
	(int)sizeof(long),	/* T_INT  */	
	(int)sizeof(long),	/* T_LONG */
	(int)sizeof(float),	/* T_FLOAT  */
	(int)sizeof(double),	/* T_DOUBLE */
	(int)sizeof(char *),	/* T_POINTER */
	
			};
 

jmp_buf env77;
 
static int doVariables();
static int eStructures();
static int eVariables();
 
static long stackSubroutine;	/* Subroutine Stack Requirements */
static char subroutineName[256];
static long VariableSize();
static int CloseAll();
static int NextAddress();
static int doAdjust();
static int doList();
static struct c_list *find_sy2();
static int doSubEnd();
static struct c_list *put_l(char *a);

int startEval(char *name);

int yyparse();
int endEval();
int freeTTree(struct tree *p);

int runCode(int argc,char *argv[]);
int evalDone();
int doBackPatch(int flag);
int RunEnd();
int doDS();
int doSpace(long l);
int doCS();
int alined(long *a);
int putInt(int i);
int putLong(long l);
int freeVSTree(struct Variables *p);
int freeCTree(struct c_list *p);
int putlab(int label);
int Eval(struct tree *eRoot,int flag);
int getlab();
int manageSW(int flag,long Value,long Address);
int manageBC(int flag);

int doFile(Name,argc,argv)
char *Name;
int argc;
char *argv[];
{
	if(!strcmp(Name,"-w")){
	    g.writes=1;
	    return 0;
	}
	
	if(!setjmp(env77)){	
	    if(startEval(Name))goto EndAll;	
	    if(yyparse())goto EndAll;
	    if(endEval())goto EndAll;
	    if(runCode(argc,argv))goto EndAll;
	    exit(0);
	}
EndAll:
	CloseAll();
	exit(1);
	return 0;
 
}
static int CloseAll()
{
	doSubEnd();
	evalDone();
	RunEnd();
	return 0;
}
int doExternals(struct linker *eRoot);
int doExternals(struct linker *eRoot)
{
	long Count,k,Address;
	
	if(!eRoot){
	    printf("doExternals NULL List\n");
	    longjmp(env77,1);
	}
	
	Address=g.Address;
	doVariables(V_GLOBAL,eRoot,&g.Address,&g.GlobalVariables,&g.GlobalStructures);
	doDS();
	doSpace(g.Address-Address);
	doCS();
	return 0;
	
}
static int doVariables(class,eRoot,Address,Variables,Structures)
int class;
struct linker *eRoot;
long *Address;
struct Variables **Variables,**Structures;
{
	struct Variables *v;
	long Count,k;
	int type;
	
	if(!eRoot){
	    return 1;
	}
	
	Count=eRoot->Count;
	for(k=0;k<Count;++k){ /* Data or Struct Links */
	    v=(struct Variables *)eRoot->Link[k];
	    switch((type=v->Type)){
	        case L_DATA:
	            eVariables(v,Address,Variables,class);
	            break;
	        case L_STRUCT:
	            eStructures(v,Address,Variables,Structures);
	            break;
	        default:
	            printf("doVariables Unknown Type %d\n",type);
	    	    longjmp(env77,1);
	    }
	}
	if(eRoot->Link)free((char *)eRoot->Link);
	if(eRoot)free((char *)eRoot);
	return 0;
}
static int eStructures(vii,Addr,Variables,Structures)
struct Variables *vii,**Variables,**Structures;
long *Addr;
{
	struct Variables *vm,*vs,*ve,*vi,*v,*vp,vl;
	struct linker *eRoot,*mRoot;
	long Count,k,mCount,km;
	long Address;
	
	if(!(eRoot=(struct linker *)vii->Members)){
	    printf("eVariables NULL List\n");
	    longjmp(env77,1);
	}
/*
	printf("Structure Name %s\n",vii->c_name);
*/
	
	if((vs=(struct Variables *)find_sym(*Structures,vii->c_name))){
	    printf("%s already defined\n",vii->c_name);
	    return 1;
	}else{
	    g.treesize=sizeof(*vs);
	    *Structures=(struct Variables *)tree(*Structures,vii->c_name);
	    vs=(struct Variables *)pfind;
	}
	
	Address=0;
	vs->Members=NULL;
	Count=eRoot->Count;
	for(k=0;k<Count;++k){
	    vi=(struct Variables *)eRoot->Link[k];
	    eVariables(vi,&Address,&vs->Members,V_MEMBERS);
	}
#ifndef THINK_C
    	alined(&Address);
#endif
	vs->Size=Address;
	if(Address > g.LargestStructure)g.LargestStructure=Address;
	*Addr += Address;
	
	if(vii->c_name)free((char *)vii->c_name);
	if(vii)free((char *)vii);
	if(eRoot->Link)free((char *)eRoot->Link);
	if(eRoot)free((char *)eRoot);
	return 0;
}
int alined(long *a)
{
        int n;
 
        n = *a % sizeof(double);
        if(n){
            n = sizeof(double)-n;
            *a += n;
        }
	return 0;
}
static int eVariables(vi,Address,Variables,class)
struct Variables *vi;
long *Address;
struct Variables **Variables;
int class;
{
	struct Variables *v,*vp,vl;
	struct linker *eRoot;
	long Count,k;
	
	if(!(eRoot=(struct linker *)vi->dStack)){
	    printf("eVariables NULL linker List\n");
	    longjmp(env77,1);
	}
	Count=eRoot->Count;
	for(k=0;k<Count;++k){
	    v=(struct Variables *)eRoot->Link[k];
	    vl = *v;
	    if(v){
	        free((char *)v);
	    }else{
	        printf("eVariables NULL Variable list\n");
	        continue;
	    }
	
	    if((vp=(struct Variables *)find_sym(*Variables,vl.c_name))){
	        printf("%s already defined\n",vl.c_name);
	        continue;
	    }else{
	        g.treesize=sizeof(vl);
	        *Variables=(struct Variables *)tree(*Variables,vl.c_name);
	        v=(struct Variables *)pfind;
	    }
	    v->Type=vl.Type;
	    v->dStack=vl.dStack;
	    v->dStackCount=vl.dStackCount;
	    v->Class=class;
	    v->Address=*Address;
	    v->StructureName=vl.StructureName;
	    v->Size=VariableSize(v);
	    NextAddress(Address,v,class);
	    if(vl.c_name)free((char *)vl.c_name);
/* 
	    printf("Variable Name %s Type %s Size %ld Address %ld\n",v->c_name,
	           TypeList[v->Type],v->Size,v->Address);
*/ 
	}
	if(eRoot->Link)free((char *)eRoot->Link);
	if(eRoot)free((char *)eRoot);
	if(vi)free((char *)vi);
	return 0;
}
static long VariableSize(v)
struct Variables *v;
{
	struct dStack *dStack;
	struct Variables *vn;
	long dStackCount,k,Size;
	int Type;
	
	if(v->Type == T_STRUCT_DEFINITION){
	     return (v->Size);
	}
	Size=1;
	if((dStack=v->dStack)){
	    dStackCount=v->dStackCount;
	    for(k=0;k<dStackCount;++k){
		switch(dStack[k].Type){
		    case T_ARRAY:
		        Size *= dStack[k].Count;
		        break;
		    case T_POINTER:
		        Size *= VariableStorage[T_POINTER];
		        return Size;
		    case T_FUNCTION:
		        break;
		    default :
		        printf("Unknown Stack Type (%d)\n",dStack[k].Type);
		        break;
		}
	    }
	}
	if(v->Type < T_POINTER){
	    Size *= VariableStorage[v->Type];	
	}else if(v->Type == T_STRUCT){
	    if(!(vn=findStructures(v->StructureName))){
		printf("Structure (%s) Not Defined\n",v->StructureName);
		return Size;
	    }
	    Size *= vn->Size;
	}else{
	    printf("VariableSize Error Type (%d)\n",v->Type);
	}
	return Size;
}
static int NextAddress(Address,v,class)
long *Address;
struct Variables *v;
int class;
{
	long Size,dSize,lSize;
	
	Size=VariableSize(v);
	lSize=VariableStorage[T_INT]; /* Force int Alinement */
	dSize=Size/lSize;
	if(dSize*lSize != Size){
	    Size = (dSize+1)*lSize;
	}
	*Address += Size;
	return 0;
}
int doLocals(struct tree *Name,struct tree *List,struct linker *ListDec,struct linker *LocalDec)
{
	char *c_string,*names[1000];
	long LocalStack,Count,k,pCount,pBefore;
	struct Variables *vp;
	
	if(Name){
	    strcpy(subroutineName,Name->sValue);
	    freeTTree(Name);
	    if((l_np=find_sym(g.l_root,subroutineName))){
	    	    printf("Subroutine '%s' Already Defined\n",subroutineName);
	    }else{
	    	if(!(l_np=put_l(subroutineName))){
	    	    printf("Subroutine '%s' Out of Memory\n",subroutineName);
	    	    longjmp(env77,1);
	   	}
	   	l_np->c_long=g.code;
	   	l_np->c_type=0;
	    }
	}else{
	    printf("Error No Subroutine Name\n");
	}
	
	Count=0;
	if(List){
	    doList(List,names,&Count);
	}
	
	g.LocalVariables=NULL;
	g.LocalStructures=NULL;
	if(ListDec){
	    LocalStack=0;
	    doVariables(V_CALL,ListDec,&LocalStack,&g.LocalVariables,&g.LocalStructures);
	}
	
	LocalStack=0;
	if(LocalDec){
	    doVariables(V_LOCAL,LocalDec,&LocalStack,&g.LocalVariables,&g.LocalStructures);
	}
	
	
	if(Count){
	    pCount=0;
	    for(k=0;k<Count;++k){
		if((vp=(struct Variables *)find_sym(g.LocalVariables,names[k]))){
		    if(pCount != vp->Address){
			printf("%s Declared Out Of Order\n",names[k]);
		    }
		    pBefore=pCount;
		    NextAddress(&pCount,vp,V_CALL);
		}else{
		    printf("Parameter %s not  declared\n",names[k]);
		}
		if(names[k])free((char *)names[k]);
	    }
	}
	
	doAdjust(g.LocalVariables,LocalStack);
	
	
	
	stackSubroutine=LocalStack;
	if(stackSubroutine){
	     putInt(MSP);
	     putLong(stackSubroutine);
	}
	
	g.stackDepth=0;
	return 0;
}
static int doAdjust(p,Length)
struct Variables *p;
long Length;
{
	if(p){
	    if(p->Class == V_CALL){
	       p->Address += (Length+VariableStorage[T_POINTER]);
	    }
/*
	    printf("Variable %s Address %ld\n",p->c_name,p->Address);
*/
	    if(p->c_left)doAdjust(p->c_left,Length);
	    if(p->c_right)doAdjust(p->c_right,Length);
	}
	return 0;
}
static int doList(List,names,Count)
struct tree *List;
char **names;
long *Count;
{
	
	if(List){
	    if(List->Type == IDENTIFIER){
	       /* printf("name %s\n",List->sValue); */
	       if(*Count >= 1000){
	           printf("Subroutine Parameter List Too Long\n");
	           return 1;
	       }
	       names[(*Count)++]=List->sValue;
	    }else if(List->Type == MERGE){
	        if(List->treeList[0])doList(List->treeList[0],names,Count);
	        if(List->treeList[1])doList(List->treeList[1],names,Count);
	    }else{
	        printf("Subroutine Parameter Error Type (%d)\n",List->Type);
	    }
	    if(List->treeList)free((char *)List->treeList);
	    free((char *)List);
	}
	return 0;
}
 
int doSubroutine(struct tree *statements);
 
int doSubroutine(struct tree *statements)
{
	char *c_string;
	
	
	g.ll_root=NULL;
	
	g.SubroutineReturn=getlab();
	
	manageBC(BC_CLEAR);
	manageSW(SW_CLEAR,0L,0L);
	
	if(statements){
	   Eval(statements,G_VALUE);
	   freeTTree(statements);
	}
	
	putlab(g.SubroutineReturn);
	
	if(stackSubroutine){
	     putInt(FE);
	     putLong(stackSubroutine);
	}
	
	putInt(RET);
	
	manageBC(BC_CHECK);
	manageSW(SW_CHECK,0L,0L);
	
	doBackPatch(0);
	
	doSubEnd();
	return 0;
	
}
static int doSubEnd()
{
	if(g.ll_root){
	    freeCTree(g.ll_root);
	    g.ll_root=NULL;
	}
	if(g.LocalVariables){
	    freeVSTree(g.LocalVariables);
	    g.LocalVariables=NULL;
	}
	if(g.LocalStructures){
	    freeVSTree(g.LocalStructures);
	    g.LocalStructures=NULL;
	}
	return 0;
	
}
int freeVSTree(struct Variables *p)
{
	int k;
	
	if(p){
	    if(p->c_left)freeVSTree(p->c_left);
	    if(p->c_right)freeVSTree(p->c_right);
	    if(p->Members)freeVSTree(p->Members);
	    if(p->dStack)free((char *)p->dStack);
	    if(p->c_name)free((char *)p->c_name);
	    if(p->StructureName)free((char *)p->StructureName);
	    free((char *)p);
	}
	return 0;
}
int freeTTree(struct tree *p)
{
	int k;
	
	if(p){
	    if(p->treeList){
	        for(k=0;k<p->Count;++k){
	            freeTTree(p->treeList[k]);
		}		
		free((char *)p->treeList);
	    }
	    if(p->sValue)free((char *)p->sValue);
	    free((char *)p);
	}
	return 0;
}
int freeCTree(struct c_list *p)
{
	if(p){
	    if(p->c_left)freeCTree(p->c_left);
	    if(p->c_right)freeCTree(p->c_right);
	    if(p->c_name)free(p->c_name);
	    if(p->c_string)free(p->c_string);
	    free((char *)p);
	}
	return 0;
}
int zerol(a,n)
char *a;
long n;
{
	while(n-- > 0)*a++ = 0;
	return 0;
}
static struct c_list *put_l(char *a)
{
	g.treesize=sizeof(*g.l_root);
	g.l_root=tree(g.l_root,a);
	l_np=pfind;
	return l_np;
}
/*	search for symbol	*/
struct Variables *findStructures(w)
char *w;
{
	if(g.LocalStructures && (find_sym(g.LocalStructures,w))){
	        return (struct Variables *)pfind;
	}
	
	if(g.GlobalStructures && (find_sym(g.GlobalStructures,w))){
	    return (struct Variables *)pfind;
	}else{
	    return (struct Variables *)NULL;
	}
}
struct Variables *findSymbol(w)
char *w;
{
	if(g.LocalVariables && (find_sym(g.LocalVariables,w))){
	        return (struct Variables *)pfind;
	}
	
	if(g.GlobalVariables && (find_sym(g.GlobalVariables,w))){
	    return (struct Variables *)pfind;
	}else{
	    return (struct Variables *)NULL;
	}
}
/*	search for symbol	*/
struct c_list *find_sym(p,w)
void *p;
char *w;
{
	find_sy2((struct c_list *)p,w);
	return(pfind);
}
/*	search for symbol	*/
static struct c_list *find_sy2(p,w)
struct c_list *p;
char *w;
{
	int cond;
 
	if(!p){
	    pfind=NULL;
	} else if(!(cond=strcmp(w,p->c_name))){
	    pfind=p;
	} else if (cond<0)
	    p->c_left=find_sy2(p->c_left,w);
	  else
	    p->c_right=find_sy2(p->c_right,w);
 
	return(p);
 
}
/*	put name into tree	*/
struct c_list *tree(pi,ch)
void *pi;
char *ch;
{
	char *strsave();
	struct c_list *p;
	int cond;
 
	p=(struct c_list *)pi;
	if(!p){
	    if(!(pfind=(struct c_list *)malloc(g.treesize))){
	        return NULL;
	    }
	    zerol((char *)pfind,(long)g.treesize);
	    pfind->c_name=strsave(ch);
	    p=pfind;
	    pfind->c_left=NULL;
	    pfind->c_right=NULL;
	} else if(!(cond=strcmp(ch,p->c_name))){
	    pfind=p;
	    }
	else if (cond<0)
	    p->c_left=tree(p->c_left,ch);
	else
	    p->c_right=tree(p->c_right,ch);
	return(p);
}
/*	save string	*/
char *strsave(s)
char *s;
{
	char *p;
 
	if((p=(char *)malloc((long)strlen(s)+1))){
	    strcpy(p,s);
	}
	return(p);
}
