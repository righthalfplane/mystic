
/*  A Bison parser, made from Simple.y  */

#include <string.h>

int yylex();


#define YYBISON 1  /* Identify Bison output.  */

#define	ID1	258
#define	ID2	259
#define	ID3	260
#define	ID4	261
#define	TYPELIST	262
#define	MERGE	263
#define	SUB	264
#define	SUB_LIST	265
#define	IDENTIFIER	266
#define	ERROR	267
#define	MEMBER	268
#define	DEFINE	269
#define	END	270
#define	PROGRAM	271
#define	RETURN	272
#define	INC_OP	273
#define	DEC_OP	274
#define	LEFT_OP	275
#define	RIGHT_OP	276
#define	LE_OP	277
#define	GE_OP	278
#define	EQ_OP	279
#define	NE_OP	280
#define	AND_OP	281
#define	OR_OP	282
#define	MUL_ASSIGN	283
#define	DIV_ASSIGN	284
#define	MOD_ASSIGN	285
#define	ADD_ASSIGN	286
#define	SUB_ASSIGN	287
#define	LEFT_ASSIGN	288
#define	RIGHT_ASSIGN	289
#define	AND_ASSIGN	290
#define	XOR_ASSIGN	291
#define	OR_ASSIGN	292
#define	STRUCT	293
#define	M_TYPE	294
#define	A_TYPE	295
#define	CHAR	296
#define	SHORT	297
#define	INT	298
#define	LONG	299
#define	SIGNED	300
#define	UNSIGNED	301
#define	FLOAT	302
#define	DOUBLE	303
#define	IF	304
#define	ELSE	305
#define	SWITCH	306
#define	WHILE	307
#define	DO	308
#define	FOR	309
#define	CONTINUE	310
#define	BREAK	311
#define	CASE	312
#define	DEFAULT	313
#define	C_INT	314
#define	C_LONG	315
#define	C_STRING	316
#define	C_DOUBLE	317
#define	POST_INC	318
#define	POST_DEC	319
#define	PRE_INC	320
#define	PRE_DEC	321
#define	U_ADDRESS	322
#define	U_PLUS	323
#define	U_MINUS	324
#define	U_NEGATE	325
#define	U_NOT	326
#define	U_BIT	327
#define	U_INDIRECTION	328
#define	SIZEOF	329

#line 1 "Simple.y"

#include <stdio.h>
#include "Simple.h"
//#define yyoverflow
static struct linker *decLink(),*dataLink(),*structLink(),*newLink(),*firstLink();
static struct tree *bTree(),*uTree(),*tTree(),*fTree();
static struct tree *zTree;
static struct c_list *m_np;
static char dStructureName[256],dName[256];
static struct Variables *vp;
static int dType,put_m();
static int pushdStack(int Type,long SizeD,long Count);

#line 15 "Simple.y"
typedef union 
{
	long LongType;
	int SimpleType;
	struct tree *treePtr;
	struct linker *linkPtr;
} YYSTYPE;

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __STDC__
#define const
#endif

int doLocals(struct tree *Name,struct tree *List,struct linker *ListDec,struct linker *LocalDec);
int doSubroutine(struct tree *statements);
int doExternals(struct linker *eRoot);
int yyerror(char *s);
int freeTTree(struct tree *p);
int zerol();

#define	YYFINAL		313
#define	YYFLAG		-32768
#define	YYNTBASE	100

#define YYTRANSLATE(x) ((unsigned)(x) <= 329 ? yytranslate[x] : 165)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    85,     2,    99,     2,    87,    81,     2,    75,
    76,    80,    82,    95,    83,    79,    86,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    93,    98,    88,
    94,    89,    92,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    77,     2,    78,    90,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    96,    91,    97,    84,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74
};

static const short yyprhs[] = {     0,
     0,     2,     4,     6,     8,    12,    14,    16,    21,    25,
    30,    34,    38,    43,    49,    51,    54,    57,    59,    62,
    65,    68,    71,    73,    75,    77,    79,    81,    83,    87,
    91,    95,    97,   101,   105,   107,   111,   115,   117,   121,
   125,   129,   133,   135,   139,   143,   145,   149,   151,   155,
   157,   161,   163,   167,   169,   173,   175,   181,   183,   187,
   189,   191,   193,   195,   197,   199,   201,   203,   205,   207,
   209,   211,   215,   216,   218,   220,   222,   224,   226,   228,
   231,   235,   237,   240,   242,   245,   251,   259,   265,   273,
   283,   289,   292,   295,   298,   302,   307,   311,   313,   315,
   319,   321,   324,   326,   329,   331,   333,   335,   337,   338,
   349,   350,   352,   353,   355,   356,   358,   360,   363,   365,
   367,   371,   378,   380,   384,   386,   390,   392,   396,   400,
   405,   407,   410,   411,   414,   416,   418,   420,   422,   424,
   426,   429,   434,   439,   441,   443,   449,   453,   455,   459,
   461,   465,   467,   471,   473,   477,   479,   483,   487,   489,
   493,   497,   501,   505,   507,   511,   515,   517,   521,   525,
   527,   531,   535,   539,   541,   544,   547,   550,   552,   556,
   558
};

static const short yyrhs[] = {    59,
     0,    60,     0,    61,     0,    62,     0,    75,   118,    76,
     0,    11,     0,   100,     0,   101,    77,   118,    78,     0,
   101,    75,    76,     0,   101,    75,   118,    76,     0,   101,
    79,   127,     0,   101,    13,   127,     0,    74,    75,   149,
    76,     0,    74,    75,   149,    80,    76,     0,   101,     0,
   102,    18,     0,   102,    19,     0,   102,     0,    18,   103,
     0,    19,   103,     0,    80,   103,     0,   104,   105,     0,
    81,     0,    82,     0,    83,     0,    84,     0,    85,     0,
   103,     0,   105,    80,   103,     0,   105,    86,   103,     0,
   105,    87,   103,     0,   105,     0,   106,    82,   105,     0,
   106,    83,   105,     0,   106,     0,   107,    20,   106,     0,
   107,    21,   106,     0,   107,     0,   108,    88,   107,     0,
   108,    89,   107,     0,   108,    22,   107,     0,   108,    23,
   107,     0,   108,     0,   109,    24,   108,     0,   109,    25,
   108,     0,   109,     0,   110,    81,   109,     0,   110,     0,
   111,    90,   110,     0,   111,     0,   112,    91,   111,     0,
   112,     0,   113,    26,   112,     0,   113,     0,   114,    27,
   113,     0,   114,     0,   114,    92,   114,    93,   115,     0,
   115,     0,   103,   117,   116,     0,    94,     0,    28,     0,
    29,     0,    30,     0,    31,     0,    32,     0,    33,     0,
    34,     0,    35,     0,    36,     0,    37,     0,   116,     0,
   118,    95,   116,     0,     0,   118,     0,   121,     0,   123,
     0,   124,     0,   125,     0,   126,     0,    96,    97,     0,
    96,   122,    97,     0,   120,     0,   122,   120,     0,    98,
     0,   118,    98,     0,    49,    75,   118,    76,   120,     0,
    49,    75,   118,    76,   120,    50,   120,     0,    52,    75,
   118,    76,   120,     0,    53,   120,    52,    75,   118,    76,
    98,     0,    54,    75,   119,    98,   119,    98,   119,    76,
   120,     0,    51,    75,   118,    76,   120,     0,    55,    98,
     0,    56,    98,     0,    17,    98,     0,    17,   118,    98,
     0,    57,   151,    93,   120,     0,    58,    93,   120,     0,
    11,     0,   127,     0,   128,    95,   127,     0,    15,     0,
   130,    15,     0,   131,     0,   130,   131,     0,   133,     0,
   132,     0,   150,     0,   138,     0,     0,   127,    75,   136,
    76,   137,    96,   137,   134,   135,    97,     0,     0,   122,
     0,     0,   128,     0,     0,   138,     0,   139,     0,   138,
   139,     0,   141,     0,   140,     0,   149,   142,    98,     0,
    38,   127,    96,   138,    97,    98,     0,   146,     0,   142,
    95,   146,     0,   127,     0,    75,   148,    76,     0,   143,
     0,   144,    75,    76,     0,   144,    77,    78,     0,   144,
    77,   151,    78,     0,   144,     0,    80,   145,     0,     0,
   147,   148,     0,   145,     0,    41,     0,    43,     0,    44,
     0,    47,     0,    48,     0,    38,   127,     0,    99,    14,
   127,   127,     0,    99,    14,   127,   151,     0,   152,     0,
   153,     0,   153,    92,   152,    93,   152,     0,   153,    27,
   154,     0,   154,     0,   154,    26,   155,     0,   155,     0,
   155,    91,   156,     0,   156,     0,   156,    90,   157,     0,
   157,     0,   157,    81,   158,     0,   158,     0,   158,    24,
   159,     0,   158,    25,   159,     0,   159,     0,   159,    22,
   160,     0,   159,    23,   160,     0,   159,    88,   160,     0,
   160,    89,   160,     0,   160,     0,   160,    21,   161,     0,
   160,    20,   161,     0,   161,     0,   161,    82,   162,     0,
   161,    83,   162,     0,   162,     0,   162,    80,   163,     0,
   162,    86,   163,     0,   162,    87,   163,     0,   163,     0,
    83,   163,     0,    85,   163,     0,    84,   163,     0,   164,
     0,    75,   152,    76,     0,    59,     0,    60,     0
};

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    61,    63,    65,    67,    69,    71,    83,    84,    85,    88,
    90,    92,    94,   101,   107,   108,   109,   113,   114,   115,
   116,   117,   121,   122,   123,   124,   125,   129,   130,   131,
   132,   136,   137,   138,   142,   143,   144,   148,   149,   150,
   151,   152,   156,   157,   158,   162,   163,   167,   168,   172,
   173,   177,   178,   182,   183,   187,   188,   193,   194,   199,
   200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
   213,   214,   218,   219,   224,   225,   226,   227,   228,   232,
   233,   237,   238,   242,   243,   247,   248,   253,   254,   255,
   257,   261,   262,   263,   264,   265,   271,   275,   281,   282,
   286,   287,   291,   292,   296,   297,   298,   302,   306,   310,
   319,   320,   324,   325,   329,   330,   334,   335,   339,   340,
   344,   348,   354,   355,   358,   359,   363,   364,   365,   366,
   370,   371,   375,   377,   382,   386,   387,   388,   389,   390,
   391,   399,   407,   415,   417,   418,   420,   421,   423,   424,
   426,   427,   429,   430,   432,   433,   435,   436,   437,   439,
   440,   441,   442,   443,   445,   446,   447,   449,   450,   451,
   453,   454,   455,   456,   458,   459,   460,   461,   464,   465,
   466
};

static const char * const yytname[] = {   "$","error","$illegal.","ID1","ID2",
"ID3","ID4","TYPELIST","MERGE","SUB","SUB_LIST","IDENTIFIER","ERROR","MEMBER",
"DEFINE","END","PROGRAM","RETURN","INC_OP","DEC_OP","LEFT_OP","RIGHT_OP","LE_OP",
"GE_OP","EQ_OP","NE_OP","AND_OP","OR_OP","MUL_ASSIGN","DIV_ASSIGN","MOD_ASSIGN",
"ADD_ASSIGN","SUB_ASSIGN","LEFT_ASSIGN","RIGHT_ASSIGN","AND_ASSIGN","XOR_ASSIGN",
"OR_ASSIGN","STRUCT","M_TYPE","A_TYPE","CHAR","SHORT","INT","LONG","SIGNED",
"UNSIGNED","FLOAT","DOUBLE","IF","ELSE","SWITCH","WHILE","DO","FOR","CONTINUE",
"BREAK","CASE","DEFAULT","C_INT","C_LONG","C_STRING","C_DOUBLE","POST_INC","POST_DEC",
"PRE_INC","PRE_DEC","U_ADDRESS","U_PLUS","U_MINUS","U_NEGATE","U_NOT","U_BIT",
"U_INDIRECTION","SIZEOF","'('","')'","'['","']'","'.'","'*'","'&'","'+'","'-'",
"'~'","'!'","'/'","'%'","'<'","'>'","'^'","'|'","'?'","':'","'='","','","'{'",
"'}'","';'","'#'","primary_expr","primary_expr2","postfix_expr","unary_expr",
"unary_operator","multiplicative_expr","additive_expr","shift_expr","relational_expr",
"equality_expr","and_expr","exclusive_or_expr","inclusive_or_expr","logical_and_expr",
"logical_or_expr","conditional_expr","assignment_expr","assignment_operator",
"expr","expr_or_none","statement","compound_statement","statement_list","expression_statement",
"selection_statement","iteration_statement","jump_statement","identifier","identifier_list",
"file","program","external_types","external_definitions","subroutine_done","@1",
"statement_sub","identifier_sub","declaration_sub","definition_list","definitions",
"data_definition","struct_definition","name_list","p1_dcltr","p2_dcltr","p3_dcltr",
"names","@2","dcltr","data_type","define_statements","exprc","expr1","expr2",
"expr3","expr4","expr5","expr6","expr7","expr8","expr9","expr10","expr11","expr12",
"expr14",""
};
#endif

static const short yyr1[] = {     0,
   100,   100,   100,   100,   100,   100,   101,   101,   101,   101,
   101,   101,   101,   101,   102,   102,   102,   103,   103,   103,
   103,   103,   104,   104,   104,   104,   104,   105,   105,   105,
   105,   106,   106,   106,   107,   107,   107,   108,   108,   108,
   108,   108,   109,   109,   109,   110,   110,   111,   111,   112,
   112,   113,   113,   114,   114,   115,   115,   116,   116,   117,
   117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
   118,   118,   119,   119,   120,   120,   120,   120,   120,   121,
   121,   122,   122,   123,   123,   124,   124,   125,   125,   125,
   125,   126,   126,   126,   126,   126,   126,   127,   128,   128,
   129,   129,   130,   130,   131,   131,   131,   132,   134,   133,
   135,   135,   136,   136,   137,   137,   138,   138,   139,   139,
   140,   141,   142,   142,   143,   143,   144,   144,   144,   144,
   145,   145,   147,   146,   148,   149,   149,   149,   149,   149,
   149,   150,   150,   151,   152,   152,   153,   153,   154,   154,
   155,   155,   156,   156,   157,   157,   158,   158,   158,   159,
   159,   159,   159,   159,   160,   160,   160,   161,   161,   161,
   162,   162,   162,   162,   163,   163,   163,   163,   164,   164,
   164
};

static const short yyr2[] = {     0,
     1,     1,     1,     1,     3,     1,     1,     4,     3,     4,
     3,     3,     4,     5,     1,     2,     2,     1,     2,     2,
     2,     2,     1,     1,     1,     1,     1,     1,     3,     3,
     3,     1,     3,     3,     1,     3,     3,     1,     3,     3,
     3,     3,     1,     3,     3,     1,     3,     1,     3,     1,
     3,     1,     3,     1,     3,     1,     5,     1,     3,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     3,     0,     1,     1,     1,     1,     1,     1,     2,
     3,     1,     2,     1,     2,     5,     7,     5,     7,     9,
     5,     2,     2,     2,     3,     4,     3,     1,     1,     3,
     1,     2,     1,     2,     1,     1,     1,     1,     0,    10,
     0,     1,     0,     1,     0,     1,     1,     2,     1,     1,
     3,     6,     1,     3,     1,     3,     1,     3,     3,     4,
     1,     2,     0,     2,     1,     1,     1,     1,     1,     1,
     2,     4,     4,     1,     1,     5,     3,     1,     3,     1,
     3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
     3,     3,     3,     1,     3,     3,     1,     3,     3,     1,
     3,     3,     3,     1,     2,     2,     2,     1,     3,     1,
     1
};

static const short yydefact[] = {     0,
    98,   101,     0,   136,   137,   138,   139,   140,     0,     0,
     0,   103,   106,   105,   108,   117,   120,   119,   133,   107,
   141,     0,   113,   102,   104,   118,     0,   123,     0,     0,
     0,    99,   114,     0,   133,   121,     0,     0,   125,   127,
   131,   135,   134,     0,   180,   181,     0,     0,     0,     0,
   142,   143,   144,   145,   148,   150,   152,   154,   156,   159,
   164,   167,   170,   174,   178,     0,   115,   124,     0,   132,
     0,     0,     0,     0,   175,   177,   176,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   100,     0,   116,   126,
   128,   129,     0,   122,   179,   147,     0,   149,   151,   153,
   155,   157,   158,   160,   161,   162,   166,   165,   163,   168,
   169,   171,   172,   173,   115,   130,     0,   109,   146,   111,
     6,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     1,     2,     3,     4,     0,     0,     0,
    23,    24,    25,    26,    27,     0,    84,     7,    15,    18,
    28,     0,    32,    35,    38,    43,    46,    48,    50,    52,
    54,    56,    58,    71,     0,    82,    75,   112,    76,    77,
    78,    79,     0,    94,     0,    19,    20,     0,     0,     0,
     0,    73,    92,    93,     0,     0,     0,     0,    21,    80,
     0,     0,     0,     0,     0,    16,    17,    61,    62,    63,
    64,    65,    66,    67,    68,    69,    70,    60,     0,    28,
    22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    85,    83,   110,    95,     0,     0,     0,     0,    74,
     0,     0,    97,     0,     0,     5,    81,    12,     9,     0,
     0,    11,    59,    29,    30,    31,    33,    34,    36,    37,
    41,    42,    39,    40,    44,    45,    47,    49,    51,    53,
    55,     0,    72,     0,     0,     0,     0,    73,    96,   141,
    13,     0,    10,     8,     0,    86,    91,    88,     0,     0,
    14,    57,     0,     0,    73,    87,    89,     0,     0,    90,
     0,     0,     0
};

static const short yydefgoto[] = {   158,
   159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
   169,   170,   171,   172,   173,   174,   219,   175,   251,   176,
   177,   178,   179,   180,   181,   182,    39,    33,   311,    11,
    12,    13,    14,   130,   183,    34,    98,    15,    16,    17,
    18,    27,    40,    41,    42,    28,    29,    43,    19,    20,
    52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
    62,    63,    64,    65
};

static const short yypact[] = {    33,
-32768,-32768,     8,-32768,-32768,-32768,-32768,-32768,    15,   -12,
    41,-32768,-32768,-32768,   378,-32768,-32768,-32768,-32768,-32768,
   -10,     8,     8,-32768,-32768,-32768,    43,-32768,    38,   378,
   306,-32768,    26,    54,-32768,-32768,    38,    38,-32768,-32768,
   144,-32768,-32768,   117,-32768,-32768,   -47,   -47,   -47,   -47,
-32768,-32768,-32768,    28,   133,    61,    86,   109,   101,    45,
    -6,    64,   -46,-32768,-32768,     8,   378,-32768,   152,-32768,
   170,   355,   120,   179,-32768,-32768,-32768,   -47,   -47,   -47,
   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,
   -47,   -47,   -47,   -47,   -47,   -47,-32768,   171,   378,-32768,
-32768,-32768,   188,-32768,-32768,   133,   175,    61,    86,   109,
   101,    45,    45,   142,   142,   142,    64,    64,   142,   -46,
   -46,-32768,-32768,-32768,   378,-32768,   -47,-32768,-32768,   230,
-32768,   259,   318,   318,   194,   196,   197,   230,   198,   182,
   195,   -47,   201,-32768,-32768,-32768,-32768,   220,   318,   318,
-32768,-32768,-32768,-32768,-32768,   126,-32768,-32768,    40,   153,
   376,   318,    49,   111,   183,     3,   174,   215,   207,   209,
   272,    35,-32768,-32768,    44,-32768,-32768,   230,-32768,-32768,
-32768,-32768,   204,-32768,    56,-32768,-32768,   318,   318,   318,
   250,   318,-32768,-32768,   210,   230,   403,   -52,-32768,-32768,
   178,     8,   288,   318,     8,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   318,-32768,
    49,   318,   318,   318,   318,   318,   318,   318,   318,   318,
   318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
   318,-32768,-32768,-32768,-32768,   -49,   -31,   -26,   233,   214,
   218,   230,-32768,     8,   -60,-32768,-32768,-32768,-32768,   -25,
   -48,-32768,-32768,-32768,-32768,-32768,    49,    49,   111,   111,
   183,   183,   183,   183,     3,     3,   174,   215,   207,   209,
   272,   -18,-32768,   230,   230,   230,   318,   318,-32768,-32768,
-32768,   246,-32768,-32768,   318,   273,-32768,-32768,    58,   226,
-32768,-32768,   230,   227,   318,-32768,-32768,   251,   230,-32768,
   330,   331,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,  -129,-32768,   -34,   -15,    13,    -8,    97,    99,
   108,   100,   107,   112,    59,  -209,-32768,  -131,  -174,  -136,
-32768,   199,-32768,-32768,-32768,-32768,     0,-32768,-32768,-32768,
   340,-32768,-32768,-32768,-32768,-32768,   228,   -13,    -9,-32768,
-32768,-32768,-32768,-32768,   320,   321,-32768,   322,   163,-32768,
   -64,   -40,-32768,   283,   287,   293,   294,   292,   166,   129,
   167,   172,    74,-32768
};


#define	YYLAST		470


static const short yytable[] = {    10,
   185,   191,    21,   186,   187,    26,    74,   103,   239,   263,
    10,    45,    46,    89,    90,   291,    44,   198,     1,   292,
   199,    31,    32,   256,   229,   230,   284,    47,    22,   294,
    51,   283,   220,    94,    26,    48,    49,    50,   107,    95,
    96,   243,   241,     1,   285,   241,   241,     2,     1,   286,
   293,     1,   202,    99,    78,    24,   246,   247,   248,   253,
   250,   239,    23,   241,   243,    97,    86,    87,   241,   241,
     3,   260,   261,     4,   295,     5,     6,   195,     3,     7,
     8,     4,    91,     5,     6,    30,   129,     7,     8,    26,
   231,   232,   264,   265,   266,   220,   220,   220,   220,   220,
   220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
   220,    99,    37,   300,   203,   289,   204,    38,   205,    79,
    66,    75,    76,    77,    84,    85,   240,   221,   222,    67,
   308,     9,    88,   304,   223,   224,   131,    35,   241,     9,
    36,   242,   132,   133,   134,    92,    93,   296,   297,   298,
   241,    81,   241,   245,     3,   299,   250,     4,    80,     5,
     6,    89,    90,     7,     8,   220,   306,   122,   123,   124,
   206,   207,   310,   250,   135,    82,   136,   137,   138,   139,
   140,   141,   142,   143,   144,   145,   146,   147,   131,    83,
   267,   268,   225,   226,   132,   133,   134,   233,   234,   148,
   149,   258,   227,   228,   262,   150,   151,   152,   153,   154,
   155,   269,   270,    73,   114,   115,   116,   104,    71,   119,
    72,   156,   200,   157,   275,   276,   135,   100,   136,   137,
   138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
   131,   271,   272,   273,   274,   101,   132,   133,   134,   112,
   113,   148,   149,   290,   105,   117,   118,   150,   151,   152,
   153,   154,   155,   120,   121,   126,   125,   127,   188,   131,
   189,   190,   192,   156,   257,   157,   133,   134,   135,   193,
   136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
   146,   147,   194,   196,   197,   235,   236,   238,   131,   237,
   244,   249,   252,   148,   149,   133,   134,   287,   241,   150,
   151,   152,   153,   154,   155,   288,     1,   144,   145,   146,
   147,   301,   303,   305,   307,   156,   309,   157,   131,   312,
   313,   277,   148,   149,   278,   133,   134,   280,   150,   151,
   152,   153,   154,   155,   279,   281,   144,   145,   146,   147,
    25,   282,   128,   302,   201,    68,   184,    70,    69,   255,
   106,   148,   149,   259,    45,    46,   108,   150,   151,   152,
   153,   154,   155,   109,   111,   110,   144,   145,   146,   147,
    47,     0,     0,     0,     0,     0,     0,     0,    48,    49,
    50,   148,   149,     0,     0,     0,     0,   150,   151,   152,
   153,   154,   155,   208,   209,   210,   211,   212,   213,   214,
   215,   216,   217,    45,    46,     3,     0,     0,     4,     0,
     5,     6,     0,     0,     7,     8,     0,     0,     0,    47,
     0,     0,   102,     0,     0,     0,     0,    48,    49,    50,
   254,     0,     0,     4,     0,     5,     6,     0,     0,     7,
     8,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   218
};


static const short yycheck[] = {     0,
   132,   138,     3,   133,   134,    15,    47,    72,    27,   219,
    11,    59,    60,    20,    21,    76,    30,   149,    11,    80,
   150,    22,    23,    76,    22,    23,    76,    75,    14,    78,
    31,   241,   162,    80,    44,    83,    84,    85,    79,    86,
    87,   178,    95,    11,    76,    95,    95,    15,    11,    76,
    76,    11,    13,    67,    27,    15,   188,   189,   190,   196,
   192,    27,    75,    95,   201,    66,    22,    23,    95,    95,
    38,   203,   204,    41,    93,    43,    44,   142,    38,    47,
    48,    41,    89,    43,    44,    96,   127,    47,    48,    99,
    88,    89,   222,   223,   224,   225,   226,   227,   228,   229,
   230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
   240,   125,    75,   288,    75,   252,    77,    80,    79,    92,
    95,    48,    49,    50,    24,    25,    92,   162,    80,    76,
   305,    99,    88,    76,    86,    87,    11,    95,    95,    99,
    98,    98,    17,    18,    19,    82,    83,   284,   285,   286,
    95,    91,    95,    98,    38,   287,   288,    41,    26,    43,
    44,    20,    21,    47,    48,   295,   303,    94,    95,    96,
    18,    19,   309,   305,    49,    90,    51,    52,    53,    54,
    55,    56,    57,    58,    59,    60,    61,    62,    11,    81,
   225,   226,    82,    83,    17,    18,    19,    24,    25,    74,
    75,   202,    20,    21,   205,    80,    81,    82,    83,    84,
    85,   227,   228,    97,    86,    87,    88,    98,    75,    91,
    77,    96,    97,    98,   233,   234,    49,    76,    51,    52,
    53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
    11,   229,   230,   231,   232,    76,    17,    18,    19,    84,
    85,    74,    75,   254,    76,    89,    90,    80,    81,    82,
    83,    84,    85,    92,    93,    78,    96,    93,    75,    11,
    75,    75,    75,    96,    97,    98,    18,    19,    49,    98,
    51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    98,    93,    75,    81,    90,    26,    11,    91,
    97,    52,    93,    74,    75,    18,    19,    75,    95,    80,
    81,    82,    83,    84,    85,    98,    11,    59,    60,    61,
    62,    76,    50,    98,    98,    96,    76,    98,    11,     0,
     0,   235,    74,    75,   236,    18,    19,   238,    80,    81,
    82,    83,    84,    85,   237,   239,    59,    60,    61,    62,
    11,   240,   125,   295,   156,    35,    98,    38,    37,   197,
    78,    74,    75,    76,    59,    60,    80,    80,    81,    82,
    83,    84,    85,    81,    83,    82,    59,    60,    61,    62,
    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
    85,    74,    75,    -1,    -1,    -1,    -1,    80,    81,    82,
    83,    84,    85,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    59,    60,    38,    -1,    -1,    41,    -1,
    43,    44,    -1,    -1,    47,    48,    -1,    -1,    -1,    75,
    -1,    -1,    78,    -1,    -1,    -1,    -1,    83,    84,    85,
    38,    -1,    -1,    41,    -1,    43,    44,    -1,    -1,    47,
    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Bob Corbett and Richard Stallman

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */


#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#define YYLEX		yylex(&yylval, &yylloc)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_bcopy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_bcopy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_bcopy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 169 "bison.simple"
int yyparse()
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yysp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yysp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
#ifdef YYLSP_NEEDED
		 &yyls1, size * sizeof (*yylsp),
#endif
		 &yystacksize);

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_bcopy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_bcopy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_bcopy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symboles being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 62 "Simple.y"
{zTree=uTree(C_INT,(struct tree *)NULL);yyval.treePtr=zTree;zTree->iValue=g.c_int;;
    break;}
case 2:
#line 64 "Simple.y"
{zTree=uTree(C_LONG,(struct tree *)NULL);yyval.treePtr=zTree;zTree->lValue=g.c_long;;
    break;}
case 3:
#line 66 "Simple.y"
{zTree=uTree(C_STRING,(struct tree *)NULL);yyval.treePtr=zTree;zTree->sValue=g.c_string;;
    break;}
case 4:
#line 68 "Simple.y"
{zTree=uTree(C_DOUBLE,(struct tree *)NULL);yyval.treePtr=zTree;zTree->dValue=g.c_double;;
    break;}
case 5:
#line 69 "Simple.y"
{yyval.treePtr = yyvsp[-1].treePtr;
    break;}
case 6:
#line 72 "Simple.y"
{yyval.treePtr=uTree(ID1,NULL);
	        yyval.treePtr->sValue=g.c_string;
	        if(yychar == YYEMPTY)yychar=YYLEX;
	        if(yychar != '('){
	            if(!(vp=findSymbol(g.c_string))){
	    	        printf("Error '%s' Not Defined \n",g.c_string);
		    }
		}
	   ;
    break;}
case 8:
#line 84 "Simple.y"
{yyval.treePtr = bTree('[',yyvsp[-3].treePtr,yyvsp[-1].treePtr);
    break;}
case 9:
#line 86 "Simple.y"
{yyval.treePtr = uTree(SUB,yyvsp[-2].treePtr);;
    break;}
case 10:
#line 89 "Simple.y"
{yyval.treePtr = bTree(SUB_LIST,yyvsp[-3].treePtr,yyvsp[-1].treePtr);
    break;}
case 11:
#line 91 "Simple.y"
{yyval.treePtr = bTree('.',yyvsp[-2].treePtr,yyvsp[0].treePtr);
    break;}
case 12:
#line 93 "Simple.y"
{yyval.treePtr = bTree(MEMBER,yyvsp[-2].treePtr,yyvsp[0].treePtr);
    break;}
case 13:
#line 94 "Simple.y"
{yyval.treePtr = uTree(SIZEOF,(struct tree *)NULL);
									    yyval.treePtr->lValue=dType;
									    if(dType == T_STRUCT){
									        yyval.treePtr->sValue=strsave(dStructureName);
									    }
									 ;
    break;}
case 14:
#line 101 "Simple.y"
{yyval.treePtr = uTree(SIZEOF,(struct tree *)NULL);
									    yyval.treePtr->lValue=T_POINTER;
									;
    break;}
case 16:
#line 108 "Simple.y"
{yyval.treePtr = uTree(POST_INC,yyvsp[-1].treePtr);
    break;}
case 17:
#line 109 "Simple.y"
{yyval.treePtr = uTree(POST_DEC,yyvsp[-1].treePtr);
    break;}
case 19:
#line 114 "Simple.y"
{yyval.treePtr = uTree(PRE_INC,yyvsp[0].treePtr);
    break;}
case 20:
#line 115 "Simple.y"
{yyval.treePtr = uTree(PRE_DEC,yyvsp[0].treePtr);
    break;}
case 21:
#line 116 "Simple.y"
{yyval.treePtr = uTree(U_INDIRECTION,yyvsp[0].treePtr);
    break;}
case 22:
#line 117 "Simple.y"
{yyval.treePtr = uTree(yyvsp[-1].SimpleType,yyvsp[0].treePtr)   ;
    break;}
case 23:
#line 121 "Simple.y"
{yyval.SimpleType = U_ADDRESS;
    break;}
case 24:
#line 122 "Simple.y"
{yyval.SimpleType = U_PLUS;
    break;}
case 25:
#line 123 "Simple.y"
{yyval.SimpleType = U_MINUS;
    break;}
case 26:
#line 124 "Simple.y"
{yyval.SimpleType = U_BIT;
    break;}
case 27:
#line 125 "Simple.y"
{yyval.SimpleType = U_NOT;
    break;}
case 29:
#line 130 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_MULT;;
    break;}
case 30:
#line 131 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_DIV;;
    break;}
case 31:
#line 132 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_MOD;;
    break;}
case 33:
#line 137 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_PLUS;;
    break;}
case 34:
#line 138 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_MINUS;;
    break;}
case 36:
#line 143 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_LEFT;;
    break;}
case 37:
#line 144 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_RIGHT;;
    break;}
case 39:
#line 149 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_LESS;;
    break;}
case 40:
#line 150 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_GREATER;;
    break;}
case 41:
#line 151 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_LE;;
    break;}
case 42:
#line 152 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_GE;;
    break;}
case 44:
#line 157 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_EQ;;
    break;}
case 45:
#line 158 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_NE;;
    break;}
case 47:
#line 163 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_AND;;
    break;}
case 49:
#line 168 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_XOR;;
    break;}
case 51:
#line 173 "Simple.y"
{yyval.treePtr = bTree(M_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=BOP_OR;;
    break;}
case 53:
#line 178 "Simple.y"
{yyval.treePtr = bTree(AND_OP,yyvsp[-2].treePtr,yyvsp[0].treePtr);;
    break;}
case 55:
#line 183 "Simple.y"
{yyval.treePtr = bTree(OR_OP,yyvsp[-2].treePtr,yyvsp[0].treePtr);;
    break;}
case 57:
#line 189 "Simple.y"
{yyval.treePtr = tTree('?',yyvsp[-4].treePtr,yyvsp[-2].treePtr,yyvsp[0].treePtr);
    break;}
case 59:
#line 195 "Simple.y"
{yyval.treePtr = bTree(A_TYPE,yyvsp[-2].treePtr,yyvsp[0].treePtr);yyval.treePtr->SubType=yyvsp[-1].SimpleType;;
    break;}
case 60:
#line 199 "Simple.y"
{yyval.SimpleType = '=';
    break;}
case 61:
#line 200 "Simple.y"
{yyval.SimpleType = BOP_MULT;
    break;}
case 62:
#line 201 "Simple.y"
{yyval.SimpleType = BOP_DIV;
    break;}
case 63:
#line 202 "Simple.y"
{yyval.SimpleType = BOP_MOD;
    break;}
case 64:
#line 203 "Simple.y"
{yyval.SimpleType = BOP_PLUS;
    break;}
case 65:
#line 204 "Simple.y"
{yyval.SimpleType = BOP_MINUS;
    break;}
case 66:
#line 205 "Simple.y"
{yyval.SimpleType = BOP_LEFT;
    break;}
case 67:
#line 206 "Simple.y"
{yyval.SimpleType = BOP_RIGHT;
    break;}
case 68:
#line 207 "Simple.y"
{yyval.SimpleType = BOP_AND;
    break;}
case 69:
#line 208 "Simple.y"
{yyval.SimpleType = BOP_XOR;
    break;}
case 70:
#line 209 "Simple.y"
{yyval.SimpleType = BOP_OR;
    break;}
case 72:
#line 214 "Simple.y"
{ yyval.treePtr = bTree(MERGE,yyvsp[-2].treePtr,yyvsp[0].treePtr) ;
    break;}
case 73:
#line 218 "Simple.y"
{yyval.treePtr=NULL;
    break;}
case 74:
#line 219 "Simple.y"
{yyval.treePtr=yyvsp[0].treePtr;
    break;}
case 80:
#line 232 "Simple.y"
{yyval.treePtr=NULL;
    break;}
case 81:
#line 233 "Simple.y"
{yyval.treePtr=yyvsp[-1].treePtr;
    break;}
case 83:
#line 238 "Simple.y"
{yyval.treePtr=bTree(MERGE,yyvsp[-1].treePtr,yyvsp[0].treePtr);
    break;}
case 84:
#line 242 "Simple.y"
{yyval.treePtr=NULL ;
    break;}
case 85:
#line 243 "Simple.y"
{yyval.treePtr=yyvsp[-1].treePtr  ;
    break;}
case 86:
#line 247 "Simple.y"
{yyval.treePtr = bTree(IF,yyvsp[-2].treePtr,yyvsp[0].treePtr);
    break;}
case 87:
#line 249 "Simple.y"
{yyval.treePtr = tTree(ELSE,yyvsp[-4].treePtr,yyvsp[-2].treePtr,yyvsp[0].treePtr);
    break;}
case 88:
#line 253 "Simple.y"
{yyval.treePtr = bTree(WHILE,yyvsp[-2].treePtr,yyvsp[0].treePtr);
    break;}
case 89:
#line 254 "Simple.y"
{yyval.treePtr = bTree(DO,yyvsp[-5].treePtr,yyvsp[-2].treePtr);
    break;}
case 90:
#line 256 "Simple.y"
{yyval.treePtr = fTree(yyvsp[-6].treePtr,yyvsp[-4].treePtr,yyvsp[-2].treePtr,yyvsp[0].treePtr);
    break;}
case 91:
#line 257 "Simple.y"
{yyval.treePtr = bTree(SWITCH,yyvsp[-2].treePtr,yyvsp[0].treePtr);
    break;}
case 92:
#line 261 "Simple.y"
{yyval.treePtr = uTree(CONTINUE,NULL);
    break;}
case 93:
#line 262 "Simple.y"
{yyval.treePtr = uTree(BREAK,NULL);
    break;}
case 94:
#line 263 "Simple.y"
{yyval.treePtr = uTree( RETURN,NULL);
    break;}
case 95:
#line 264 "Simple.y"
{yyval.treePtr = uTree(RETURN,yyvsp[-1].treePtr);
    break;}
case 96:
#line 265 "Simple.y"
{
	
								 yyval.treePtr = uTree(CASE,yyvsp[0].treePtr);
								 yyval.treePtr->lValue = yyvsp[-2].LongType;

								;
    break;}
case 97:
#line 271 "Simple.y"
{yyval.treePtr = uTree(DEFAULT,yyvsp[0].treePtr);
    break;}
case 98:
#line 276 "Simple.y"
{yyval.treePtr=uTree(IDENTIFIER,NULL);
	    yyval.treePtr->sValue=g.c_string;;
    break;}
case 100:
#line 282 "Simple.y"
{yyval.treePtr=bTree(MERGE,yyvsp[-2].treePtr,yyvsp[0].treePtr);
    break;}
case 101:
#line 286 "Simple.y"
{yychar = YYEOF;YYACCEPT;
    break;}
case 102:
#line 287 "Simple.y"
{yychar = YYEOF;YYACCEPT;
    break;}
case 105:
#line 296 "Simple.y"
{;
    break;}
case 106:
#line 297 "Simple.y"
{;
    break;}
case 107:
#line 298 "Simple.y"
{;
    break;}
case 108:
#line 302 "Simple.y"
{doExternals(yyvsp[0].linkPtr);;
    break;}
case 109:
#line 309 "Simple.y"
{doLocals(yyvsp[-6].treePtr,yyvsp[-4].treePtr,yyvsp[-2].linkPtr,yyvsp[0].linkPtr);
    break;}
case 110:
#line 312 "Simple.y"
{ 
		doSubroutine(yyvsp[-1].treePtr);
	   ;
    break;}
case 111:
#line 319 "Simple.y"
{yyval.treePtr=NULL;
    break;}
case 112:
#line 320 "Simple.y"
{yyval.treePtr=yyvsp[0].treePtr;
    break;}
case 113:
#line 324 "Simple.y"
{yyval.treePtr=NULL;
    break;}
case 114:
#line 325 "Simple.y"
{yyval.treePtr=yyvsp[0].treePtr;
    break;}
case 115:
#line 329 "Simple.y"
{yyval.linkPtr=NULL;
    break;}
case 116:
#line 330 "Simple.y"
{yyval.linkPtr=yyvsp[0].linkPtr;
    break;}
case 118:
#line 335 "Simple.y"
{ yyval.linkPtr=newLink(yyvsp[-1].linkPtr,yyvsp[0].linkPtr) ;
    break;}
case 121:
#line 344 "Simple.y"
{ yyval.linkPtr=dataLink(yyvsp[-1].linkPtr);
    break;}
case 122:
#line 350 "Simple.y"
{ yyval.linkPtr=structLink(yyvsp[-4].treePtr,yyvsp[-2].linkPtr);
    break;}
case 124:
#line 355 "Simple.y"
{ yyval.linkPtr=newLink(yyvsp[-2].linkPtr,yyvsp[0].linkPtr);
    break;}
case 125:
#line 358 "Simple.y"
{  strcpy(dName,yyvsp[0].treePtr->sValue);freeTTree(yyvsp[0].treePtr);;
    break;}
case 128:
#line 364 "Simple.y"
{pushdStack(T_FUNCTION,0L,0L); ;
    break;}
case 129:
#line 365 "Simple.y"
{pushdStack(T_ARRAY,0L,0L); ;
    break;}
case 130:
#line 366 "Simple.y"
{pushdStack(T_ARRAY,0L,yyvsp[-1].LongType);;
    break;}
case 132:
#line 371 "Simple.y"
{ pushdStack(T_POINTER,0L,0L); ;
    break;}
case 133:
#line 375 "Simple.y"
{g.dStackCount = 0;;
    break;}
case 134:
#line 377 "Simple.y"
{ 
		yyval.linkPtr=decLink(dType,dStructureName,dName);
	  ;
    break;}
case 136:
#line 386 "Simple.y"
{dType=T_CHAR;dStructureName[0]=0;;
    break;}
case 137:
#line 387 "Simple.y"
{dType=T_INT;dStructureName[0]=0;;
    break;}
case 138:
#line 388 "Simple.y"
{dType=T_LONG;dStructureName[0]=0;;
    break;}
case 139:
#line 389 "Simple.y"
{dType=T_FLOAT;dStructureName[0]=0;;
    break;}
case 140:
#line 390 "Simple.y"
{dType=T_DOUBLE;dStructureName[0]=0;;
    break;}
case 141:
#line 391 "Simple.y"
{
						    dType=T_STRUCT;
						    strcpy(dStructureName,yyvsp[0].treePtr->sValue);
		    				freeTTree(yyvsp[0].treePtr);
						;
    break;}
case 142:
#line 400 "Simple.y"
{
		    yyval.treePtr=NULL;put_m(yyvsp[-1].treePtr->sValue);
		    m_np->c_string=strsave(yyvsp[0].treePtr->sValue);
		    freeTTree(yyvsp[-1].treePtr);
		    freeTTree(yyvsp[0].treePtr);
		;
    break;}
case 143:
#line 408 "Simple.y"
{
			yyval.treePtr=NULL;put_m(yyvsp[-1].treePtr->sValue);
		    m_np->c_long=yyvsp[0].LongType;
		    freeTTree(yyvsp[-1].treePtr);
		;
    break;}
case 146:
#line 418 "Simple.y"
{yyval.LongType = (yyvsp[-4].LongType) ? (yyvsp[-2].LongType) : (yyvsp[0].LongType);
    break;}
case 147:
#line 420 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType || yyvsp[0].LongType;
    break;}
case 149:
#line 423 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType && yyvsp[0].LongType;
    break;}
case 151:
#line 426 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType | yyvsp[0].LongType;
    break;}
case 153:
#line 429 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType ^ yyvsp[0].LongType;
    break;}
case 155:
#line 432 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType & yyvsp[0].LongType;
    break;}
case 157:
#line 435 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType == yyvsp[0].LongType;
    break;}
case 158:
#line 436 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType != yyvsp[0].LongType;
    break;}
case 160:
#line 439 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType <= yyvsp[0].LongType;
    break;}
case 161:
#line 440 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType >= yyvsp[0].LongType;
    break;}
case 162:
#line 441 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType < yyvsp[0].LongType;
    break;}
case 163:
#line 442 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType > yyvsp[0].LongType;
    break;}
case 165:
#line 445 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType >> yyvsp[0].LongType;
    break;}
case 166:
#line 446 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType << yyvsp[0].LongType;
    break;}
case 168:
#line 449 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType + yyvsp[0].LongType;
    break;}
case 169:
#line 450 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType - yyvsp[0].LongType;
    break;}
case 171:
#line 453 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType * yyvsp[0].LongType;
    break;}
case 172:
#line 454 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType / yyvsp[0].LongType;
    break;}
case 173:
#line 455 "Simple.y"
{yyval.LongType = yyvsp[-2].LongType % yyvsp[0].LongType;
    break;}
case 175:
#line 458 "Simple.y"
{yyval.LongType= - yyvsp[0].LongType;
    break;}
case 176:
#line 459 "Simple.y"
{yyval.LongType= ! yyvsp[0].LongType;
    break;}
case 177:
#line 460 "Simple.y"
{yyval.LongType= ~ yyvsp[0].LongType;
    break;}
case 179:
#line 464 "Simple.y"
{ yyval.LongType=yyvsp[-1].LongType;
    break;}
case 180:
#line 465 "Simple.y"
{ yyval.LongType=g.c_int;
    break;}
case 181:
#line 466 "Simple.y"
{ yyval.LongType=g.c_long;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 440 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  for (x = 0; x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) xmalloc(size + 15);
	  strcpy(msg, "parse error");

	  if (count < 5)
	    {
	      count = 0;
	      for (x = 0; x < (sizeof(yytname) / sizeof(char *)); x++)
		if (yycheck[x + yyn] == x)
		  {
		    strcat(msg, count == 0 ? ", expecting `" : " or `");
		    strcat(msg, yytname[x]);
		    strcat(msg, "'");
		    count++;
		  }
	    }
	  yyerror(msg);
	  free(msg);
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 468 "Simple.y"


#include <stdio.h>

extern char *yytext;

int yyerror(char *s)
{
	printf("\n column %d %s\n",g.column, s);
/*
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", g.column, s);
*/
	return 0;
}
#include "tree.h"

