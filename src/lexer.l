%{
#include <stdlib.h>
#include <stdio.h>
#include "conf.h"
#include "parser.h"
%}

%option noyywrap reentrant
%option outfile="lexer.c" header-file="lexer.h"

%%

([[:alpha:]]|_|-|\.|\/)+ {
	 yylval.text = strdup(yytext);
	 return TEXT;
}

"=" { return EQUALS; }
[ \t] ;

%%

static void yyerror(const char *msg)
{
	fprintf(stderr, "Lexer error: %s\n", msg);
}
