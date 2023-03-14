/*****************************************************************************
 * json/lexicon.l: JSON tokeniser
 *****************************************************************************
 * Copyright © 2020 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

%option 8bit
%option bison-bridge
%option nodefault
%option noinput
%option reentrant
%option nostdinit
%option nounput
%option noyywrap
%option prefix="json" outfile="lex.yy.c"

%{

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdbool.h>
#include <stdlib.h>
#include "json.h"
#include "grammar.h"

#define YY_INPUT(buf,result,size) \
{ \
    size_t len = json_read(yyextra, buf, size); \
    result = (len > 0) ? len : YY_NULL; \
}

%}

%%

[\t\r\n ]		;

"null" {		return VALUE_NULL; }

"false"	{		yylval->boolean = false;
			return BOOLEAN; }

"true" {		yylval->boolean = true;
			return BOOLEAN; }

-?(0|([1-9][0-9]*))(\.[0-9]+)?(e[+-]?[0-9]+)? {
			yylval->number = atof(yytext);
			return NUMBER; }

\"([^"\\]|\\[\"\\/bfnrt]|\\u[0-9A-Fa-f]{4})*\" {
			yylval->string = json_unescape(yytext + 1, yyleng - 2);
			return STRING; }

[{}:,[\]] {		return *yytext; }

. {			return *yytext; }

<<EOF>> {		return 0; }

%%
