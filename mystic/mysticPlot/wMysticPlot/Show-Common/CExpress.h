#ifndef __CEXPRESS__
#define __CEXPRESS__
#include "Linedata.h"

#define STACK_INCREMENT 50


struct stack{
	IconPtr myIcon;
	FILE *file;
	double value;
	double *data;
	int  logical;
	short int  *ldata;
	char name[256];
	char pioName[64];
	double pioTime;
	int pioIndex;
	
	int lineIndex;
	
	int n1,n2,n3;
	int PointLineCount;
	int PointLineType;
	double x1,x2,x3;
	double y1,y2,y3;
	double z1,z2,z3;

    double integral;
    int integrate;
    
    double *auxData;
    long auxLength;
	
	long xsize;
	long ysize;
	long zsize;
	int type;
	
    int multigroup;
	
	double bhangMeterHeight;
	double bhangMeterRadius;
    int bhangMeterSteps;
};

struct stackHolder{
	struct stack *stackList;
	long stackMax;
	long stackCount;
};


typedef struct stackHolder *stackHolderPtr;

#define T_LOR					1
#define T_LAND					2
#define T_OR					3
#define T_AND					4
#define T_EQUALEQUAL			5
#define T_NOT					6
#define T_NOTEQUAL				7
#define T_LESSEQUAL				8
#define T_LESS					9
#define T_LEFTSHIFT				10
#define T_GREAT					11
#define T_GREATEQUAL			12
#define T_RIGHTSHIFT			13
#define T_PLUS					14
#define T_MINUS			    	15
#define T_MINUSUNARY			16
#define T_XOR					17
#define T_MULT					18
#define T_DIV					19
#define T_MOD					20
#define T_NEGATE				21
#define T_PLUSPLUS				22
#define T_MINUSMINUS			23
#define T_UNARYINCREMENT		24




#define T_IF_START				25
#define T_IF_ENDSHORT			26
#define T_IF_ENDLONG			27
#define T_IF_ELSE				28

#define T_ANDEQUAL				50
#define T_LEFTSHIFTEQUAL		51
#define T_RIGHTSHIFTEQUAL		52
#define T_PLUSEQUAL				53
#define T_MINUSEQUAL			54
#define T_MULTEQUAL				55
#define T_XOREQUAL				56
#define T_DIVEQUAL				57
#define T_MODEQUAL				58
#define T_OREQUAL				59

#define T_IF					101
#define T_IDENTIFIER			102
#define T_EOF					104
#define T_FUNCTION				103
#define T_ELSE					105
#define T_CONSTANT				106
#define T_FUNCTION_NAME			107
#define T_COMMA					108
#define T_FOR					109

#define T_CURLY_LEFT			200
#define T_CURLY_RIGHT			201
#define T_PAREN_LEFT			202
#define T_PAREN_RIGHT			203
#define T_SEMICOLON				204
#define T_EQUAL					205
#define T_COLON					206
#define T_SQUARE_LEFT			207
#define T_SQUARE_RIGHT			208

#define T_STACK_CONSTANT	300
#define T_STACK_VARIABLE	301
#define T_STACK_ARRAY		302
#define T_STACK_FILE		303
#define T_STACK_TABLE		304
#define T_STACK_NAME		305
#define T_STACK_R1			306
#define T_STACK_R2			307
#define T_STACK_RR			308
#define T_STACK_LCONSTANT	309
#define T_STACK_LARRAY		310
#define T_STACK_FIND		311
#define T_STACK_CREATE		312
#define T_STACK_NAME_STACK	314
#define T_STACK_STRING		315
#define T_STACK_DIMENSIONS	316
#define T_STACK_OUT_STACK	317



struct CExpress{
	unsigned char *dat;
	struct stackHolder stackData;
	struct stackHolder logicalData;
	struct stackHolder nameData;
	struct stackHolder outData;
    pushpopPtr lineStack;
	struct linedata li;
	char path[2048];
	char ch[256];
	int Mode;
	int tok;
	int tok2;
	int function;
	struct stack R1;
	struct stack R2;
	struct stack *RR;
	
	long CurrentFrame;
	long LastFrame;

	int EqualToStack;
	int KeepStack;
	struct areadata ai;
	struct LIMITS limits;
	int lineplotgather;
	long lineNumber;
};

typedef struct CExpress *CExpressPtr;


CExpressPtr CExpressStart(int Mode);

int CExpressEvaluate(CExpressPtr e,unsigned char *dat);

int CExpressClose(CExpressPtr e);

int CExpressNext(CExpressPtr e);

int CExpressFreeStacks(CExpressPtr e);

int CExpressPopFrom(CExpressPtr e,stackHolderPtr s,int popWhat,int load);

int CExpressPush(CExpressPtr e,int pushWhat);
int CExpressPop(CExpressPtr e,int popWhat,int load);
struct stack *CExpressGetNameStack(CExpressPtr e,char *name);

long CExpressGetFrameCount(CExpressPtr e);

#define CExpressInteractive		0
#define CExpressBatch			1
#endif

