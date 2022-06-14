%{
#include <stdio.h>
#include "Simple.h"
#define yyoverflow
static struct linker *decLink(),*dataLink(),*structLink(),*newLink(),*firstLink();
static struct tree *bTree(),*uTree(),*tTree(),*fTree();
static struct tree *zTree;
static struct c_list *m_np;
static char dStructureName[256],dName[256];
static struct Variables *vp;
static int dType,put_m();
%}


%union 
{
	long LongType;
	int SimpleType;
	struct tree *treePtr;
	struct linker *linkPtr;
}
%token  ID1 ID2 ID3 ID4 TYPELIST MERGE SUB SUB_LIST
%token  IDENTIFIER ERROR MEMBER DEFINE END PROGRAM RETURN
%token  INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token  AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token  SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token  XOR_ASSIGN OR_ASSIGN STRUCT M_TYPE A_TYPE

%token  CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE

%token  IF ELSE SWITCH WHILE DO FOR CONTINUE BREAK CASE DEFAULT
%token  C_INT C_LONG C_STRING C_DOUBLE
%token  POST_INC POST_DEC PRE_INC PRE_DEC
%token  U_ADDRESS U_PLUS U_MINUS U_NEGATE U_NOT U_BIT U_INDIRECTION
%token SIZEOF

%type <LongType> exprc expr1 expr2 expr3 expr4 expr5 expr6 expr7 expr8
%type <LongType> expr9 expr10 expr11 expr12 expr14
%type <SimpleType>  unary_operator assignment_operator
%type <SimpleType> INC_OP DEC_OP
%type <treePtr> unary_expr postfix_expr primary_expr external_types
%type <treePtr> multiplicative_expr additive_expr shift_expr relational_expr 
%type <treePtr> equality_expr and_expr exclusive_or_expr inclusive_or_expr
%type <treePtr> logical_and_expr logical_or_expr conditional_expr
%type <treePtr> assignment_expr expr identifier_list
%type <treePtr> statement compound_statement statement_list subroutine_done
%type <treePtr> statement_sub identifier_sub 
%type <treePtr> expression_statement selection_statement 
%type <treePtr> iteration_statement jump_statement 
%type <treePtr>   file
%type <treePtr> program identifier  expr_or_none
%type <treePtr> define_statements primary_expr2

%type <linkPtr>  definitions data_definition struct_definition external_definitions
%type <linkPtr> name_list names declaration_sub definition_list

%start file
%%

primary_expr
	: C_INT   
	   {zTree=uTree(C_INT,(struct tree *)NULL);$$=zTree;zTree->iValue=g.c_int;}
	| C_LONG   
	    {zTree=uTree(C_LONG,(struct tree *)NULL);$$=zTree;zTree->lValue=g.c_long;}
	| C_STRING  
	    {zTree=uTree(C_STRING,(struct tree *)NULL);$$=zTree;zTree->sValue=g.c_string;}
	| C_DOUBLE  
	    {zTree=uTree(C_DOUBLE,(struct tree *)NULL);$$=zTree;zTree->dValue=g.c_double;}
	| '(' expr ')'                   {$$ = $2}
	 
	| IDENTIFIER   
	   {$$=uTree(ID1,NULL);
	        $$->sValue=g.c_string;
	        if(yychar == YYEMPTY)yychar=YYLEX;
	        if(yychar != '('){
	            if(!(vp=findSymbol(g.c_string))){
	    	        printf("Error '%s' Not Defined \n",g.c_string);
		    }
		}
	   }
	;
primary_expr2
	: primary_expr
	| primary_expr2 '[' expr ']'	{$$ = bTree('[',$1,$3)}
	| primary_expr2   '('   ')'
					{$$ = uTree(SUB,$1);}
	 	 
	| primary_expr2   '(' expr  ')'
					{$$ = bTree(SUB_LIST,$1,$3)}
	| primary_expr2   '.' identifier
					{$$ = bTree('.',$1,$3)}
	| primary_expr2   MEMBER identifier
					{$$ = bTree(MEMBER,$1,$3)}
	| SIZEOF '(' data_type ')' 		{$$ = uTree(SIZEOF,(struct tree *)NULL);
									    $$->lValue=dType;
									    if(dType == T_STRUCT){
									        $$->sValue=strsave(dStructureName);
									    }
									 }
	
	| SIZEOF '(' data_type '*' ')'  {$$ = uTree(SIZEOF,(struct tree *)NULL);
									    $$->lValue=T_POINTER;
									}

	;
postfix_expr
	: primary_expr2
	| postfix_expr INC_OP		{$$ = uTree(POST_INC,$1)}
	| postfix_expr DEC_OP		{$$ = uTree(POST_DEC,$1)}
	;

unary_expr
	: postfix_expr
	| INC_OP unary_expr                  {$$ = uTree(PRE_INC,$2)}
	| DEC_OP unary_expr                  {$$ = uTree(PRE_DEC,$2)}
	| '*' unary_expr		     {$$ = uTree(U_INDIRECTION,$2)}
	| unary_operator multiplicative_expr {$$ = uTree($1,$2)   }
	;

unary_operator
	: '&'  {$$ = U_ADDRESS}
	| '+'  {$$ = U_PLUS}
	| '-'  {$$ = U_MINUS}
	| '~'  {$$ = U_BIT}
	| '!'  {$$ = U_NOT}
	;

multiplicative_expr
	: unary_expr
	| multiplicative_expr '*' unary_expr {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_MULT;}
	| multiplicative_expr '/' unary_expr {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_DIV;}
	| multiplicative_expr '%' unary_expr {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_MOD;}
	;

additive_expr
	: multiplicative_expr
	| additive_expr '+' multiplicative_expr {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_PLUS;}
	| additive_expr '-' multiplicative_expr {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_MINUS;}
	;

shift_expr
	: additive_expr
	| shift_expr LEFT_OP additive_expr  {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_LEFT;}
	| shift_expr RIGHT_OP additive_expr {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_RIGHT;}
	;

relational_expr
	: shift_expr
	| relational_expr '<' shift_expr   {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_LESS;}
	| relational_expr '>' shift_expr   {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_GREATER;}
	| relational_expr LE_OP shift_expr {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_LE;}
	| relational_expr GE_OP shift_expr {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_GE;}
	;

equality_expr
	: relational_expr
	| equality_expr EQ_OP relational_expr {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_EQ;}
	| equality_expr NE_OP relational_expr {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_NE;}
	;

and_expr
	: equality_expr
	| and_expr '&' equality_expr     {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_AND;}
	;

exclusive_or_expr
	: and_expr
	| exclusive_or_expr '^' and_expr {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_XOR;}
	;

inclusive_or_expr
	: exclusive_or_expr
	| inclusive_or_expr '|' exclusive_or_expr {$$ = bTree(M_TYPE,$1,$3);$$->SubType=BOP_OR;}
	;

logical_and_expr
	: inclusive_or_expr
	| logical_and_expr AND_OP inclusive_or_expr {$$ = bTree(AND_OP,$1,$3);}
	;

logical_or_expr
	: logical_and_expr
	| logical_or_expr OR_OP logical_and_expr {$$ = bTree(OR_OP,$1,$3);}
	;

conditional_expr
	: logical_or_expr
	| logical_or_expr '?' logical_or_expr ':' conditional_expr
			{$$ = tTree('?',$1,$3,$5)}
	;

assignment_expr
	: conditional_expr
	| unary_expr assignment_operator assignment_expr
			{$$ = bTree(A_TYPE,$1,$3);$$->SubType=$2;}
	;

assignment_operator
	: '='            {$$ = '='}
	| MUL_ASSIGN     {$$ = BOP_MULT}
	| DIV_ASSIGN     {$$ = BOP_DIV}
	| MOD_ASSIGN     {$$ = BOP_MOD}
	| ADD_ASSIGN     {$$ = BOP_PLUS}
	| SUB_ASSIGN     {$$ = BOP_MINUS}
	| LEFT_ASSIGN    {$$ = BOP_LEFT}
	| RIGHT_ASSIGN   {$$ = BOP_RIGHT}
	| AND_ASSIGN     {$$ = BOP_AND}
	| XOR_ASSIGN     {$$ = BOP_XOR}
	| OR_ASSIGN      {$$ = BOP_OR}
	;

expr
	: assignment_expr
	| expr ',' assignment_expr { $$ = bTree(MERGE,$1,$3) }
	;

expr_or_none 
	:		{$$=NULL}
	| expr		{$$=$1}
	;
	

statement
    : compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

compound_statement
	: '{' '}'                {$$=NULL}
	| '{' statement_list '}' {$$=$2}
	;

statement_list
	: statement
	| statement_list statement {$$=bTree(MERGE,$1,$2)}
	;

expression_statement
	: ';'       {$$=NULL }
	| expr ';'  {$$=$1  }
	;

selection_statement
	: IF '(' expr ')' statement   {$$ = bTree(IF,$3,$5)}
	| IF '(' expr ')' statement 
	             ELSE statement   {$$ = tTree(ELSE,$3,$5,$7)}
	;

iteration_statement
	: WHILE '(' expr ')' statement          {$$ = bTree(WHILE,$3,$5)}
	| DO statement WHILE '(' expr ')' ';'   {$$ = bTree(DO,$2,$5)}
	| FOR '(' expr_or_none ';' expr_or_none ';' expr_or_none ')' statement 
	                                          {$$ = fTree($3,$5,$7,$9)}
	| SWITCH '(' expr ')' statement          {$$ = bTree(SWITCH,$3,$5)}
	;

jump_statement
	: CONTINUE ';'   {$$ = uTree(CONTINUE,NULL)}
	| BREAK ';'      {$$ = uTree(BREAK,NULL)}
	| RETURN ';'     {$$ = uTree( RETURN,NULL)}
	| RETURN expr ';'		{$$ = uTree(RETURN,$2)}
	| CASE exprc ':' statement	{
	
								 $$ = uTree(CASE,$4);
								 $$->lValue = $2;

								}
	| DEFAULT ':' statement		{$$ = uTree(DEFAULT,$3)}
	;

identifier
	: IDENTIFIER 
	   {$$=uTree(IDENTIFIER,NULL);
	    $$->sValue=g.c_string;}
	;
	
identifier_list
	: identifier                       
	| identifier_list ',' identifier   {$$=bTree(MERGE,$1,$3)}
	;

file
	: END	        {yychar = YYEOF;YYACCEPT}
	| program END	{yychar = YYEOF;YYACCEPT}         
	;

program
	: external_types 
	| program  external_types
	;

external_types
	: subroutine_done      {}
	| external_definitions {}
	| define_statements    {}
	;

external_definitions
	: definition_list {doExternals($1);}
	;
	
subroutine_done
	: identifier   '(' identifier_sub  ')'
	                   declaration_sub
	               '{'
	                   declaration_sub {doLocals($1,$3,$5,$7)} statement_sub 
	               '}'

	   { 
		doSubroutine($9);
	   }
	;


statement_sub
	:				{$$=NULL}
	|	statement_list          {$$=$1}
	;

identifier_sub
	:				 {$$=NULL}
	|	identifier_list          {$$=$1}
	;

declaration_sub
	:				       {$$=NULL}
	|	definition_list	               {$$=$1}
	;

definition_list
	: definitions
	| definition_list definitions  { $$=newLink($1,$2) }
	;

definitions
	: struct_definition
	| data_definition
	;

data_definition
	: data_type name_list ';' { $$=dataLink($2)}
	;

struct_definition
	: STRUCT identifier '{'
	         definition_list
	                    '}' ';'  { $$=structLink($2,$4)}
	;

name_list
	:  names
	| name_list  ','   names { $$=newLink($1,$3)}
	;
p1_dcltr
	: identifier   {  strcpy(dName,$1->sValue);freeTTree($1);}
	| '(' dcltr ')'
	;
	
p2_dcltr
	: p1_dcltr
	| p2_dcltr '(' ')'  	 {pushdStack(T_FUNCTION,0L,0L); }
	| p2_dcltr '[' ']'  	 {pushdStack(T_ARRAY,0L,0L); }
	| p2_dcltr '[' exprc ']' {pushdStack(T_ARRAY,0L,$3);}
	;
	
p3_dcltr
	: p2_dcltr
	| '*' p3_dcltr { pushdStack(T_POINTER,0L,0L); }
	;

names	
	: {g.dStackCount = 0;} dcltr
	
	  { 
		$$=decLink(dType,dStructureName,dName);
	  }
	;
dcltr	
	: p3_dcltr
	;

data_type
	: CHAR   {dType=T_CHAR;dStructureName[0]=0;}
	| INT    {dType=T_INT;dStructureName[0]=0;}
	| LONG   {dType=T_LONG;dStructureName[0]=0;}
	| FLOAT  {dType=T_FLOAT;dStructureName[0]=0;}
	| DOUBLE {dType=T_DOUBLE;dStructureName[0]=0;}
	| STRUCT identifier {
						    dType=T_STRUCT;
						    strcpy(dStructureName,$2->sValue);
		    				freeTTree($2);
						}
	;
	
define_statements
	: '#' DEFINE identifier identifier 
		{
		    $$=NULL;put_m($3->sValue);
		    m_np->c_string=strsave($4->sValue);
		    freeTTree($3);
		    freeTTree($4);
		}
		
	| '#' DEFINE identifier exprc
		{
			$$=NULL;put_m($3->sValue);
		    m_np->c_long=$4;
		    freeTTree($3);
		}
	;

exprc	: expr1
	;
expr1	: expr2
	| expr2 '?' expr1 ':' expr1 {$$ = ($1) ? ($3) : ($5)}
	;
expr2	: expr2 OR_OP expr3   {$$ = $1 || $3}
	| expr3
	;
expr3	: expr3 AND_OP expr4  {$$ = $1 && $3}
	| expr4
	;
expr4	: expr4 '|' expr5  {$$ = $1 | $3}
	| expr5
	;
expr5	: expr5 '^' expr6  {$$ = $1 ^ $3}
	| expr6
	;
expr6	: expr6 '&' expr7  {$$ = $1 & $3}
	| expr7
	;
expr7	: expr7 EQ_OP expr8  {$$ = $1 == $3}
	| expr7 NE_OP expr8  {$$ = $1 != $3}
	| expr8
	;
expr8	: expr8 LE_OP expr9  {$$ = $1 <= $3}
	| expr8 GE_OP expr9  {$$ = $1 >= $3}
	| expr8 '<' expr9  {$$ = $1 < $3}
	| expr9 '>' expr9  {$$ = $1 > $3}
	| expr9
	;
expr9	: expr9 RIGHT_OP expr10  {$$ = $1 >> $3}
	| expr9 LEFT_OP  expr10  {$$ = $1 << $3}
	| expr10
	;
expr10	: expr10 '+' expr11  {$$ = $1 + $3}
	| expr10 '-' expr11  {$$ = $1 - $3}
	| expr11
	;
expr11	: expr11 '*' expr12  {$$ = $1 * $3}
	| expr11 '/' expr12  {$$ = $1 / $3}
	| expr11 '%' expr12  {$$ = $1 % $3}
	| expr12
	;
expr12	: '-' expr12 {$$= - $2}
	| '!' expr12 {$$= ! $2}
	| '~' expr12 {$$= ~ $2}
	| expr14
	;

expr14	: '(' expr1 ')' { $$=$2}
	|  C_INT	{ $$=g.c_int}
	| C_LONG	{ $$=g.c_long}
	;
%%

#include <stdio.h>

extern char *yytext;

yyerror(s)
char *s;
{
	printf("\n column %d %s\n",g.column, s);
/*
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", g.column, s);
*/
}
#include "tree.h"

