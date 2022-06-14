
typedef union 
{
	long LongType;
	int SimpleType;
	struct tree *treePtr;
	struct linker *linkPtr;
} YYSTYPE;
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


extern YYSTYPE yylval;
