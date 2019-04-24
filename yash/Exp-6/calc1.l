%{
	#include<stdio.h>
	#include<stdlib.h>
	#include"y.tab.h"
	void yyerror(char *s);
%}

%%
[0-9]+		{ 

			 	yylval = atoi(yytext);
			  	return INTEGER;
			}

[-+*/\n]	{ return *yytext; }

[\t]		;

.			yyerror("Unknown character");

%%

	int yywrap(void)
	{ return 1; }