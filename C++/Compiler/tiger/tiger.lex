%option noyywrap 

%x COMMENT

%{
#include <string.h>
#include "tokens.h"

int curColumn   = 1;
int curLine     = 1;
int nextColumn  = 1;

void advance()
{
	curColumn = nextColumn;
	nextColumn += yyleng;
}

%}
digit		[0-9]
whitespace  [ \t]+

%%
  /* ==================== deal with comment ==================== */
"/*"  	            {BEGIN(COMMENT);}	
<COMMENT>"*/"       {BEGIN(INITIAL);}
<COMMENT>([^*]|\n)+|.  /* do nothing */
<COMMENT><<EOF>>    {printf("unterminate comment\n");yyterminate();exit(1);}
  /* =========================================================== */
{whitespace}			{advance();continue;}
"\n"					{nextColumn = 1;++curLine;continue;}

[0-9]+					{advance();yylval.ival = atoi(yytext)	;return INT;}

  /*  keyword and punctuation */
","						{advance();return COMMA;}
":"						{advance();return COLON;}
";"						{advance();return SEMICOLON;}
"("						{advance();return LPAREN;}
")"						{advance();return RPAREN;}
"["						{advance();return LBRACK;}
"]"						{advance();return RBRACK;}
"{"						{advance();return LBRACE;}
"}"						{advance();return RBRACE;}
"."						{advance();return DOT;}
"+"						{advance();return PLUS;}
"-"						{advance();return MINUS;}
"*"						{advance();return TIMES;}
"/"						{advance();return DIVIDE;}
"="						{advance();return EQ;}
"<>"					{advance();return NEQ;}
"<"						{advance();return LT;}
"<="					{advance();return LE;}
">"						{advance();return GT;}
">="					{advance();return GE;}
"&"						{advance();return AND;}
"|"						{advance();return OR;}
":="					{advance();return ASSIGN;}
"array"					{advance();return ARRAY;}
"if"					{advance();return IF;}
"else"					{advance();return ELSE;}
"then"					{advance();return THEN;}
"while"					{advance();return WHILE;}
"for"					{advance();return FOR;}
"to"					{advance();return TO;}
"do"					{advance();return DO;}
"let"					{advance();return LET;}
"in"					{advance();return IN;}
"end"					{advance();return END;}
"of"					{advance();return OF;}
"break"					{advance();return BREAK;}
"nil"					{advance();return NIL;}
"function"				{advance();return FUNCTION;}
"var"					{advance();return VAR;}
"type"					{advance();return TYPE;}

[a-zA-Z][_a-zA-Z0-9]*   {return ID;}
.
<<EOF>>        			{return ENDOFFILE;}
%%

char * toknames[] = 
{
	"ID","STRING","INT","COMMA","COLON","SEMICOLON",
	"LPAREN","RPAREN","LBRACK","RBRACK","LBRACE","RBRACE",
	"DOT","PLUS","MINUS","TIMES","DIVIDE","EQ","NEQ",
	"LT","LE","GT","GE","AND","OR","ASSIGN","ARRAY","IF",
	"ELSE","THEN","WHILE","FOR","TO","DO","LET","IN","END",
	"OF","BREAK","NIL","FUNCTION","VAR","TYPE","EOF"
};

char * tokname(int n)
{
	return n < 257 || n > 300 ? "BAD TOKEN":toknames[n - 257];
}

YYSTYPE yylval;
int main(int argc,char * argv[])
{
	if(argc < 2){
		printf("uage:a.out filename\n");
		exit(1);
	}
	FILE * f = fopen(argv[1],"r");
	if(!f){
		printf("no such file:%s\n",argv[1]);
		exit(1);
	}
	/*yyrestart(f);*/
	yyin = fopen(argv[1],"r");
	int tok = 128;
	for(;;){
		tok = yylex();
		switch(tok){
		case 0:break;
		default:
			printf("%s,%d:%d\n",tokname(tok),curLine,curColumn);break;
		}
		
		
		if(tok == ENDOFFILE)break;
	}

	return 0;
}
