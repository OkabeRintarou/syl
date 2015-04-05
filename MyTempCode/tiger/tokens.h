#ifndef _TIGER_TOKEN_H_
#define _TIGER_TOKEN_H_


enum TokenType
{
	ID          = 257, /* identifer:foo,bar etc	*/
	STRING      = 258, /* "hello,world" etc     */
	INT         = 259, /* 123,456,789 etc       */
	COMMA		= 260, /*      ,                */
	COLON       = 261, /*      :                */
	SEMICOLON   = 262, /*      ;                */
	LPAREN      = 263, /*      (                */
	RPAREN      = 264, /*      )                */
	LBRACK      = 265, /*      [                */
	RBRACK      = 266, /*      ]                */
	LBRACE      = 267, /*      {                */
	RBRACE      = 268, /*      }                */
	DOT         = 269, /*      .                */
	PLUS        = 270, /*      +                */
	MINUS       = 271, /*      -                */
	TIMES       = 272, /*      *                */
	DIVIDE      = 273, /*      /                */
	EQ          = 274, /*      =                */
	NEQ         = 275, /*      < >              */
	LT          = 276, /*      <                */
	LE          = 277, /*      < =              */
	GT          = 278, /*      >                */
	GE          = 279, /*      > =              */
	AND         = 280, /*      &                */
	OR          = 281, /*      |                */
	ASSIGN      = 282, /*      :=               */
	ARRAY       = 283, /*      array            */
	IF          = 284, /*      if               */
	ELSE        = 285, /*      else             */
	THEN        = 286, /*      then             */
	WHILE       = 287, /*      while            */
	FOR         = 288, /*      for              */
	TO          = 289, /*      to               */
	DO          = 290, /*      do               */
	LET         = 291, /*      let              */
	IN          = 292, /*      in               */
	END         = 293, /*      end              */
	OF          = 294, /*      of               */
	BREAK       = 295, /*      break            */
	NIL         = 296, /*      nil              */
	FUNCTION    = 297, /*      function         */
	VAR         = 298, /*      var              */
	TYPE        = 299  /*      type             */
};

typedef union
{
	int pos;
	int ival;
	char * sval;
}YYSTYPE;
extern YYSTYPE yylval;
#endif
