#include <stdlib.h>
static struct linker *decLink(dType,dStructureName,dName)
int dType;
char *dStructureName,*dName;
{
	struct Variables *v;
	struct dStack *dStack;
	struct linker *l;
	long Length;
	int k;
		
	Length=(long)sizeof(struct Variables);
	if(!(v=(struct Variables *)malloc(Length))){
	    printf("decLink Out Of Memory (Request #1 - %ld)\n",Length);
	    longjmp(env77,1);
	}
	zerol((char *)v,Length);
	
	
	if(!(l=firstLink(1L))){
	    longjmp(env77,1);
	}
	v->c_name=strsave(dName);
	v->Type=dType;
	if(*dStructureName){
	    v->StructureName=strsave(dStructureName);
	}else{
	    v->StructureName=NULL;
	}
	*(l->Link)=v;
	if(g.dStackCount){
	    Length=(long)sizeof(struct dStack)*(g.dStackCount+1);
	    if(!(dStack=(struct dStack  *)malloc(Length))){
	          printf("decLink Out Of Memory (Request #2 - %ld)\n",Length);
	          longjmp(env77,1);
	    }
	    zerol((char *)dStack,Length);
	    v->dStackCount=g.dStackCount;
	    v->dStack=dStack;
	    for(k=0;k<g.dStackCount;++k)dStack[k]=g.dStackData[k];
	    g.dStackCount=0;
	}	
	return l;
}
static struct linker *dataLink(p)
struct linker *p;
{
	struct Variables *v;
	struct linker *l;
	long Length;

	if(!p){
	    printf("dataLink Null Pointer %p\n",p);
	    longjmp(env77,1);
	}
	Length=(long)sizeof(struct Variables);
	if(!(v=(struct Variables *)malloc(Length))){
	    printf("dataLink Out Of Memory (Request #1 - %ld)\n",Length);
	    longjmp(env77,1);
	}
	zerol((char *)v,Length);
	if(!(l=firstLink(1L))){
	    longjmp(env77,1);
	}
	v->c_name=NULL;
	v->Type=L_DATA;
	v->dStack=(struct dStack *)p;
	*(l->Link)=v;
	return l;
}
static struct linker *structLink(identifier,p)
struct tree *identifier;
struct linker *p;
{
	struct Variables *s;
	struct linker *l;
	long Length;
	
	if(!identifier || !p){
	    printf("structLink Null Pointer %p %p",identifier,p);
	    longjmp(env77,1);
	}
	Length=(long)sizeof(struct Variables);
	if(!(s=(struct Variables *)malloc(Length))){
	    printf("structLink Out Of Memory (Request #1 - %ld)\n",Length);
	    longjmp(env77,1);
	}
	zerol((char *)s,Length);
	if(!(l=firstLink(1L))){
	    longjmp(env77,1);
	}
	s->c_name=identifier->sValue;
	identifier->sValue=NULL;
	s->Members=(struct Variables *)p;
	s->Type=L_STRUCT;
	*(l->Link)=s;
	if(identifier)freeTTree(identifier);
	return l;
}
static struct linker *newLink(l1,l2)
struct linker *l1,*l2;
{
	struct linker *l;
	long Count,k,kl;
	
	if(!l1 || !l1){
	    printf("newLink Null Pointer %p %p",l1,l2);
	    longjmp(env77,1);
	}
	Count=l1->Count+l2->Count;
	if(!(l=firstLink(Count))){
	    longjmp(env77,1);
	}
	
	/*  Merge Links */
	
	kl=0;
	for(k=0;k<l1->Count;++k)l->Link[kl++]=l1->Link[k];

	for(k=0;k<l2->Count;++k)l->Link[kl++]=l2->Link[k];
	
	/* Free Old Links */

	if(l1){
	    if(l1->Link)free((char *)l1->Link);
	    free((char *)l1);
	}
	if(l2){
	    if(l2->Link)free((char *)l2->Link);
	    free((char *)l2);
	}
	return l;
}
static struct linker *firstLink(Count)
long Count;
{
	struct linker *l;
	long Length;
	void **pl;
	
	Length=(long)sizeof(struct linker);
	if(!(l=(struct linker *)malloc(Length))){
	    printf("firstLink out of Memory (Request #1 - %ld)\n",Length);
	    longjmp(env77,1);
	}
	zerol((char *)l,Length);
	Length=(long)sizeof(void *)*(Count+1);
	if(!(pl=(void **)malloc(Length))){
	    printf("firstLink out of Memory (Request #2 - %ld)\n",Length);
	    longjmp(env77,1);
	}
	zerol((char *)pl,Length);
	l->Link=pl;
	l->Count=Count;
	return l;
}

static int pushdStack(int Type,long SizeD,long Count)
{
	
	if(g.dStackCount >= g.dStackMax){
	    g.dStackMax += 5;
	    if(g.dStackData){
	        if(!(g.dStackData=(struct dStack *)realloc(g.dStackData,(long)sizeof(struct dStack)*g.dStackMax))){
		    printf("pushdStack Out Of Memory  (%ld)\n",(long)g.dStackMax);
	    	    longjmp(env77,1);
	        }
	    }else{
	        if(!(g.dStackData=(struct dStack *)malloc((long)sizeof(struct dStack)*g.dStackMax))){
		    printf("pushdStack Out Of Memory  (%ld)\n",(long)g.dStackMax);
	    	    longjmp(env77,1);
	        }
	    }
	}
	g.dStackData[g.dStackCount].Type=Type;
	g.dStackData[g.dStackCount].Size=SizeD;
	g.dStackData[g.dStackCount++].Count=Count;
	return 0;
}
static struct tree *bTree(op,one,two)
int op;
struct tree *one,*two;
{
	struct tree *mine,**list;

	if(!(mine=(struct tree *)malloc(sizeof(struct tree)))){
	    printf("bTree out of Memory/n");
	    longjmp(env77,1);
	}
	zerol((char *)mine,(long)sizeof(struct tree));
	mine->Type=op;
	mine->Count=2;
	if(!(list=(struct tree **)malloc(2*sizeof(struct tree *)))){
	    printf("bTree out of Memory/n");
	    longjmp(env77,1);
	}
	zerol((char *)list,2*(long)sizeof(struct tree *));
	list[0]=one;
	list[1]=two;
	mine->treeList=list;
	return mine;
}
static struct tree *uTree(op,one)
int op;
struct tree *one;
{
	struct tree *mine,**list;

	if(!(mine=(struct tree *)malloc(sizeof(struct tree)))){
	    printf("bTree out of Memory/n");
	    longjmp(env77,1);
	}
	zerol((char *)mine,(long)sizeof(struct tree));
	mine->Type=op;
	mine->Count=1;
	if(!(list=(struct tree **)malloc(sizeof(struct tree *)))){
	    printf("bTree out of Memory/n");
	    longjmp(env77,1);
	}
	zerol((char *)list,(long)sizeof(struct tree *));
	list[0]=one;
	mine->treeList=list;
	return mine;
}
static struct tree *tTree(op,one,two,three)
int op;
struct tree *one,*two,*three;
{
	struct tree *mine,**list;

	if(!(mine=(struct tree *)malloc(sizeof(struct tree)))){
	    printf("bTree out of Memory/n");
	    longjmp(env77,1);
	}
	zerol((char *)mine,(long)sizeof(struct tree));
	mine->Type=op;
	mine->Count=3;
	if(!(list=(struct tree **)malloc(3*sizeof(struct tree *)))){
	    printf("bTree out of Memory/n");
	    longjmp(env77,1);
	}
	zerol((char *)list,3*(long)sizeof(struct tree *));
	list[0]=one;
	list[1]=two;
	list[2]=three;
	mine->treeList=list;
	return mine;
}
static struct tree *fTree(one,two,three,four)
struct tree *one,*two,*three,*four;
{
	struct tree *mine,**list;

	if(!(mine=(struct tree *)malloc(sizeof(struct tree)))){
	    printf("bTree out of Memory/n");
	    longjmp(env77,1);
	}
	zerol((char *)mine,(long)sizeof(struct tree));
	mine->Type=FOR;
	mine->Count=4;
	if(!(list=(struct tree **)malloc(4*sizeof(struct tree *)))){
	    printf("bTree out of Memory/n");
	    longjmp(env77,1);
	}
	zerol((char *)list,4*(long)sizeof(struct tree *));
	list[0]=one;
	list[1]=two;
	list[2]=three;
	list[3]=four;
	mine->treeList=list;
	return mine;
}
static int put_m(a)
char *a;
{
	extern struct c_list *pfind;

	g.treesize=sizeof(*g.m_root);
	g.m_root=tree(g.m_root,a);
	m_np=pfind;
	return 0;
}
