%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "conf.h"
#include "y.tab.h"
#include "lexer.h"
static void yyerror(config *cfg, void * scanner, const char *msg);
%}

%lex-param { void *scanner }
%parse-param { config *cfg }
%parse-param { void *scanner }

%token <text> TEXT
%token EQUALS
%union {
	int value;
	char *text;
}


%%
config:
| config expr
;

expr:
TEXT EQUALS TEXT {
	config_item *x = malloc(sizeof(config_item) + 1);
	memset(x, '\0', sizeof(config_item));
	strncpy(x->key, $1, CONF_KEY_SIZE);
	strncpy(x->value, $3, CONF_KEY_SIZE);
	append_config(cfg, x);
}
| TEXT EQUALS {
	config_item *x = malloc(sizeof(config_item) + 1);
	memset(x, '\0', sizeof(config_item));
	strncpy(x->key, $1, CONF_KEY_SIZE);
	strncpy(x->value, "", CONF_KEY_SIZE);
	append_config(cfg, x);
}
;

%%

static void yyerror(config *cfg, yyscan_t scanner, const char *msg)
{
	fprintf(stderr, "Parse error: %s\n", msg);
}
