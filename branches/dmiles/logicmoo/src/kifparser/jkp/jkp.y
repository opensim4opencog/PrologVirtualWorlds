
/*	Modifications from version 1.0:
	1. discard the horn clause.
	2. require the implication(logsent) must have both premise
	   and conclusion.
	 Modified date: Dec. 5, 1996
*/

%union {String; Integer; KifSent; KifObj; KifTerm; KifSeq; 
		KifLogConst; KifOperator; KifRelSent; KifSeqvar}

%token <Integer> '('   ')'   INDVAR  SEQVAR TRUE FALSE
%token <Integer> STRING  INTEGER REAL  EQ  NOTEQ  IMPLIES
%token <Integer> IMPLIED  EQUIV  EQLESS  LESS  EQGREATER
%token <Integer> GREATER  QUOTE  FORALL  EXISTS  AND
%token <Integer> OR  NOT  IF  COND  LISTOF  DEFOBJECT
%token <Integer> DEFFUNCTION  DEFRELATION  DEFLOGICAL
%token <Integer> CONSTANT PLACEHOLDER

%type  <String> tLPAREN   tRPAREN
%type  <String> tEQ  tNOTEQ
%type  <String> tQUOTE  tFORALL  tEXISTS
%type  <String> tIF  tCOND  tLISTOF  tDEFOBJECT
%type  <String> tDEFRELATION  tDEFLOGICAL  tCONSTANT

%type  <String>	tINDVAR  tSTRING tINTEGER tREAL
%type  <String>	tIMPLIES tIMPLIED tEQUIV tEQLESS tLESS 
%type  <String>	tEQGREATER tGREATER tAND tOR tNOT
%type  <String> relconst relop

%type  <KifSeqvar>	tSEQVAR



%type  <KifObj>		form  
%type  <KifSent>	sentence logconst logsent
%type  <KifRelSent>	relsent  
%type  <KifTerm>	term  funterm
%type  <KifSeq>		termseq sentseq 

%start form

%%


form		: 	sentence 	
					{	
						// form: sentence
						KifParser._result = $1; 
						//System.out.println("Parsing result is: " + ($1).toString());
					}
			;
sentence	:	logconst	
					{ 
						// sentence :   logconst
						$$ = $1;  
					}
			|	relsent		
					{ 
						// sentence: relsent
						$$ = $1;  
					}
			|	logsent		
					{ 
						// sentence: logsent
						$$ = $1;  
					}
			;
logconst	:	TRUE		
					{ 
						// logconst: TRUE
						$$ = new KifLogConst(true);
					}	
			|	FALSE		
					{ 
						// logconst: FALSE
						$$ = new KifLogConst(false);
					}
			;
relsent		:	tLPAREN relconst termseq tRPAREN 
					{ 
						// relsent: tLPAREN tCONSTANT termseq tRPAREN
						$$ = new KifRelSent($2, $3); 
					}
			|	tLPAREN tCONSTANT tRPAREN 
					{ 
						// relsent: tLPAREN tCONSTANT tRPAREN
						$$ = new KifRelSent($2); 
					}
			;
relconst	:	tCONSTANT
					{
						$$ = $1;
					}
			|	relop
					{
						$$ = $1;
					}
			;
relop		:	tEQLESS
					{
						$$ = $1;
					}
			|	tLESS
					{
						$$ = $1;
					}
			|	tEQGREATER
					{
						$$ = $1;
					}
			|	tGREATER
					{
						$$ = $1;
					}
			|	tEQ
					{
						$$ = $1;
					}
			|	tNOTEQ
					{
						$$ = $1;
					}
			;
logsent		:	tLPAREN tNOT sentence tRPAREN 
					{ 
						// logsent :   tLPAREN tNOT sentence tRPAREN
						$$ = new KifNegation ($3);
					}
			|	tLPAREN tAND sentence sentseq tRPAREN 
					{ 
						// logsent: tLPAREN tAND sentence sentseq tRPAREN
						$$ = new KifConjunction($3, $4);
					}
			|	tLPAREN tOR sentence sentseq tRPAREN 
					{ 
						// logsent: tLPAREN tOR sentence sentseq tRPAREN
						$$ = new KifDisjunction($3, $4);
					}
			|	tLPAREN tIMPLIES sentseq sentence tRPAREN 
					{ 
						// logsent: tLPAREN tIMPLIES sentseq sentence tRPAREN
						$$ = new KifImplication($4, $3);
					}
			|	tLPAREN tIMPLIED sentence sentseq tRPAREN 
					{ 
						// logsent: tLPAREN tIMPLIED sentence sentseq tRPAREN
						$$ = new KifImplication($3, $4);
					}
			|	tLPAREN tEQUIV sentence sentence tRPAREN 
					{ 
						// logsent: tLPAREN tEQUIV sentence sentence tRPAREN
						$$ = new KifEquivalence($3, $4);
					}
			;
sentseq		:	sentence			
					{ 
						// sentseq:   sentence
						$$ = new KifSeq($1); 
					}
			|	sentseq sentence	
					{ 
						// sentseq: sentseq sentence
						$$ = new KifSeq($1, $2);
					}
			;
term		:	tSTRING				
					{ 
						// term :   tSTRING
						$$ = new KifString($1);
					}
			|	tINDVAR				
					{ 
						// term :   tINDVAR
						$$ = new KifIndvar($1);
					}
			|	tINTEGER				
					{ 
						// term :   tINTEGER
						$$ = new KifInt($1);
					}
			|	tREAL				
					{ 
						// term :   tREAL
						$$ = new KifReal($1);
					}
			|	funterm				
					{ 
						// term : funterm
						$$ = $1;
					}
			|	tCONSTANT				
					{ 
						// term : object constant
						$$ = new KifObjConst($1);
					}
			;
termseq		:	term			
					{ 
						// termseq:   term
						$$ = new KifSeq($1);  
					}
			|	termseq term	
					{ 
						// termseq: termseq term
						$$ = new KifSeq($1, $2);  
					}
			|	tSEQVAR
					{
						// termseq:   tSEQVAR
						$$ = new KifSeq($1);  
					}
			|	termseq tSEQVAR	
					{ 
						// term termseq tSEQVAR
						$$ = new KifSeq($1, $2);  
					}
			;
funterm		:	tLPAREN tCONSTANT termseq tRPAREN
					{
						// funterm: tLPAREN tCONSTANT termseq tRPAREN
						$$ = new KifFunTerm($2, $3);
					}
			|	tLPAREN tCONSTANT tRPAREN
					{
						// funterm: tLPAREN tCONSTANT tRPAREN
						$$ = new KifFunTerm($2);
					}
			;


/* about object constant:
   create(new) upon reduce. at this time, we know that it
   is an objconst, not other constant.
   the same with tEQ, tNOTEQ
*/

/*	Copy out the text corresponding to each token
*/
tLPAREN		:	'('			{$$ = new String (yytext); };
tRPAREN		:	')'			{$$ = new String (yytext); };
tINDVAR		:	INDVAR		{$$ = new String (yytext); };
tSEQVAR		:	SEQVAR		{$$ = new KifSeqvar (new String(yytext)); } ;
tSTRING		:	STRING		{$$ = new String (yytext); };
tINTEGER	:	INTEGER		{$$ = new String (yytext); };
tREAL		:	REAL		{$$ = new String (yytext); };
tEQ			:	EQ			{$$ = new String (yytext); };
tNOTEQ		:	NOTEQ		{$$ = new String (yytext); };
tIMPLIES	:	IMPLIES		{$$ = new String (yytext); };
tIMPLIED	:	IMPLIED		{$$ = new String (yytext); };
tEQUIV		:	EQUIV		{$$ = new String (yytext); };
tEQLESS		:	EQLESS		{$$ = new String (yytext); };
tLESS		:	LESS		{$$ = new String (yytext); };
tEQGREATER	:	EQGREATER	{$$ = new String (yytext); };
tGREATER	:	GREATER		{$$ = new String (yytext); };
tQUOTE		:	QUOTE		{$$ = new String (yytext); };
tFORALL		:	FORALL		{$$ = new String (yytext); };
tEXISTS		:	EXISTS		{$$ = new String (yytext); };
tAND		:	AND			{$$ = new String (yytext); };
tOR			:	OR			{$$ = new String (yytext); };
tNOT		:	NOT			{$$ = new String (yytext); };
tIF			:	IF			{$$ = new String (yytext); };
tCOND		:	COND		{$$ = new String (yytext); };
tLISTOF		:	LISTOF		{$$ = new String (yytext); };
tDEFOBJECT	:	DEFOBJECT	{$$ = new String (yytext); };
tDEFOBJECT	:	DEFOBJECT	{$$ = new String (yytext); };
tDEFRELATION:	DEFRELATION	{$$ = new String (yytext); };
tDEFLOGICAL	:	DEFLOGICAL	{$$ = new String (yytext); };
tCONSTANT	:	CONSTANT	{$$ = new String (yytext); };


%%

public
void
yyverror(String s) throws ParseException
{
	String msg = yyerror_verbose(s);
	System.out.print("yyverror: line ");
	System.out.print(yyloc.lineno()+1);
	System.out.println(" ; " + msg);
	throw new ParseException(s);
}

