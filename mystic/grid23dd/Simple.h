#include <stdio.h>
 
#include <setjmp.h>

#include <stdlib.h>
 
#include "NewNames.h"
 
/*
#define malloc malloc2
#define free free2
#define realloc realloc2
*/
 
#define CODE_RELATIVE		1
#define DATA_RELATIVE		2
 
#define SW_START		1
#define SW_FINISH		2
#define SW_POP			3
#define SW_PUSH			4
#define SW_CLEAR		5
#define SW_CHECK		6
#define SW_CASE			7
#define SW_DEFAULT		8
 
 
#define BC_START		1
#define BC_FINISH		2
#define BC_POP			3
#define BC_PUSH			4
#define BC_PUSH_SWITCH		5
#define BC_CLEAR		6
#define BC_CHECK		7
#define BC_WRITE_CONTINUE	8
#define BC_WRITE_BREAK		9
#define BC_GET_CONTINUE		10
#define BC_GET_BREAK		11
#define BC_JUMP_CONTINUE	12
#define BC_JUMP_BREAK		13
 
 
#define BOP_MOD		0
#define BOP_OR		1
#define BOP_XOR		2
#define BOP_AND		3
#define BOP_LEFT	4
#define BOP_RIGHT	5
#define BOP_PLUS	6
#define BOP_MINUS	7
#define BOP_MULT	8
#define BOP_DIV		9
#define BOP_LESS	10
#define BOP_GREATER	11
#define BOP_NE		12
#define BOP_EQ		13
#define BOP_LE		14
#define BOP_GE		15
 
 
#define G_VALUE  1
#define G_ADDRESS 2
 
 
#define V_REGISTER 0
#define V_GLOBAL   1
#define V_LOCAL    2
#define V_CALL     3
#define V_MEMBERS  4
 
#define L_DATA	  1
#define L_STRUCT  2
 
 
#define T_CHAR 			1
#define T_INT 			2		
#define T_LONG 			3
#define T_FLOAT 		4
#define T_DOUBLE 		5
#define T_POINTER  		6
#define T_FUNCTION 		7
#define T_ARRAY    		8
#define T_STRUCT 		9
#define T_STRUCT_DEFINITION	10
 
 
struct linker{
	long Count;
	void **Link;
};
 
 
struct dStack{
    int Type;
    long Size;
    long Count;
};
 
 
struct Variables{
	char *c_name;
	struct Variables *c_left;
	struct Variables *c_right;
	int Type;
	int Class;
	long Size;
	long Address;
	long Offset;
	int dStackCount;
	struct dStack *dStack;
	char  *StructureName;
	struct Variables *Members;
};
 
 
 
 
 
struct tree{
    int Type;
    int Count;
    char  *sValue;
    int    iValue;
    long   lValue;
    double dValue;
    long Size;
    int SubType;
    struct tree **treeList;
};
 
	struct c_list{		/*	tokens		*/
	    char *c_name;
	    struct c_list *c_left;
	    struct c_list *c_right;
	    int c_type;
	    long c_long;
	    char *c_string;
	    int (*routine)();
	};
 
#ifdef THINK_C
struct c_list *find_var(),*find_sym(void *,char *),*tree(void *,char *);
#else
#define TRUE  1
#define FALSE 0
struct c_list *find_var(),*find_sym(),*tree();
#endif
struct Variables *findSymbol(),*findStructures();
 
char *strsave();
	 
struct Global{
    FILE *infile,*inopt;
    int linenum;
    int error;
    int writes;
    int column;
    int c_int;
    long c_long;
    long Address;
    char *c_string;
    double c_double;
    struct Variables *LocalVariables,*GlobalVariables,*LocalStructures,*GlobalStructures;
    struct dStack *dStackData;
    int dStackCount,dStackMax;
    long treesize;
    int SubroutineReturn;
    long LargestStructure;
    struct c_list *ll_root;
    long stackDepth;	/* Current Stack Offset */
    unsigned char *codebuff,*databuff;
    long code;
    long data;
    struct c_list *c_root;
    struct c_list *m_root;
    struct c_list *l_root;
};
 
extern struct Global g;
 
extern jmp_buf env77;
