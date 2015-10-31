#ifndef _PARSER_H_
#define _PARSER_H_

#include "util.h"

#define MAXLINE 257
#define MAXIMAGENAME 129



// instructions:
// image,
// draw
// print
// translate
// child
// rotate
typedef enum{
	IMAGEDEF,
	DRAW,
	PRINT,
	TRANSLATE,
	CHILD,
	ROTATE,
}INSTR_TYPE;

typedef enum {
	COMMENT,
	INSTRUCTION,
	IDENTIFIER,
	END_OF_FILE,
	BLANK_LINE,
	UNKNOWN,
}TokenType;

typedef struct{
	TokenType type;
	char * lexeme;
}Token;

typedef struct{
	double x, y;
}Point;


struct PointList{
	Point pt;
	struct PointList * next;
};

typedef struct PointList PointList;

typedef struct{
	char name[128];
	PointList * lst;
}ImageInfo;


struct ImageList
{
	ImageInfo * img;
	struct ImageList * next;
};
typedef struct ImageList ImageList;

extern Token GetNextToken(void);
extern BOOL BeginParser(const char * filename);
extern void EndParser();
extern void parser_file(const char * filename);
#endif
