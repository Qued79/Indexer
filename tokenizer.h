
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include "hash.h"


typedef enum {NA,WORD,DECI,OCT,HEX,FLO,OP,LPAREN,RPAREN,LBRACK,RBRACK,POINTER,MINUS,NEGATE,COMP,INC,DEC,MULTIPLY,DIVIDE,MOD,ADD,RSHIFT,LSHIFT,LESS,GREATER,LESSEQ,GREATEREQ,EQUALS,NOTEQUALS,BITAND,BITXOR,BITOR,LOGAND,LOGOR,CONDTRUE,CONDFALSE,ASSIGN,PLUSEQUALS,MINUSEQUALS,TIMESEQUALS,DIVEQUALS,MODEQUALS,RSHIFTEQUALS,LSHIFTEQUALS,BITANDEQUALS,BITXOREQUALS,BITOREQUALS,COMMAOP,SEMICOLON} tokentype;


struct TokenizerT_ {
	char* data;
	tokentype type;
	struct TokenizerT_ *next;
};

struct TokenizerT_ *TKCreate( char *ts);
char *TypeToString(tokentype x);
void tokenizeAndStoreFile(FILE *fp, const char* filePath, struct hashtable *table);
char *TKGetNextToken(struct TokenizerT_ *tk);
void TKDestroy(struct TokenizerT_ *tk );
