
%{
//import YYtokentypes;
%}



/* regular definitions 
*/

delim		[ \t\n]
ws		{delim}+
digit		[0-9]
/*
number		\-?{digit}+(\.{digit}+)?((E|e)[+\-]?{digit}+)?
*/
integer		[+\-]?{digit}+
purefloat	([+\-]?{digit}*(\.{digit}+))|([+\-]?{digit}+(\.{digit}*))
real		({purefloat}((E|e)[+\-]?{digit}+)?)|({integer}((E|e)[+\-]?{digit}+))
string		\"([^"\\]|(\\.))*\"
constant	[^ )(\t\n]+
indvar		\?{constant}+
seqvar		\@{constant}+


%%

{ws}			/* no action */
"("			{yyreturn('('); }
")"			{yyreturn(')'); }
{indvar}		{yyreturn(INDVAR); }
{seqvar}		{yyreturn(SEQVAR); }
("true"|"TRUE")  	{yyreturn(TRUE);  }
("false"|"FALSE")  	{yyreturn(FALSE);  }
{string}		{yyreturn(STRING);  }
{integer}		{yyreturn(INTEGER);  }
{real}			{yyreturn(REAL);  }
"="			{yyreturn(EQ);  }
"/="			{yyreturn(NOTEQ);  }
"=>"			{yyreturn(IMPLIES);  }
"<="			{yyreturn(IMPLIED);  }
"<=>"			{yyreturn(EQUIV);  }
"=<"			{yyreturn(EQLESS);  }
"<"			{yyreturn(LESS);  }
">="			{yyreturn(EQGREATER);  }
">"			{yyreturn(GREATER);  }
("quote"|"QUOTE")	{yyreturn(QUOTE);  }
("forall"|"FORALL")	{yyreturn(FORALL);  }
("exists"|"EXISTS")	{yyreturn(EXISTS);  }
("and"|"AND")		{yyreturn(AND);  }
("or"|"OR")		{yyreturn(OR);  }
("not"|"NOT")		{yyreturn(NOT);  }
("if"|"IF")		{yyreturn(IF);  }
("cond"|"COND")		{yyreturn(COND);  }
("listof"|"LISTOF")	{yyreturn(LISTOF);} 
("defobject"|"DEFOBJECT")	{yyreturn(DEFOBJECT);} 
("deffunction"|"DEFFUNCTION")	{yyreturn(DEFFUNCTION);} 
("defrelation"|"DEFRELATION")	{yyreturn(DEFRELATION);}  
("deflogical"|"DEFLOGICAL")	{yyreturn(DEFLOGICAL);}  
{constant}		{yyreturn(CONSTANT); }

%%


@TOKENTYPES@
