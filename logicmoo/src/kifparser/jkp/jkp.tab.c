
/*  A Bison parser, made from jkp.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	INDVAR	258
#define	SEQVAR	259
#define	TRUE	260
#define	FALSE	261
#define	STRING	262
#define	INTEGER	263
#define	REAL	264
#define	EQ	265
#define	NOTEQ	266
#define	IMPLIES	267
#define	IMPLIED	268
#define	EQUIV	269
#define	EQLESS	270
#define	LESS	271
#define	EQGREATER	272
#define	GREATER	273
#define	QUOTE	274
#define	FORALL	275
#define	EXISTS	276
#define	AND	277
#define	OR	278
#define	NOT	279
#define	IF	280
#define	COND	281
#define	LISTOF	282
#define	DEFOBJECT	283
#define	DEFFUNCTION	284
#define	DEFRELATION	285
#define	DEFLOGICAL	286
#define	CONSTANT	287
#define	PLACEHOLDER	288


#line 9 "jkp.y"
typedef union {String; Integer; KifSent; KifObj; KifTerm; KifSeq; 
		KifLogConst; KifOperator; KifRelSent; KifSeqvar} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		82
#define	YYFLAG		-32768
#define	YYNTBASE	36

#define YYTRANSLATE(x) ((unsigned)(x) <= 288 ? yytranslate[x] : 67)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
     4,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     5,     6,     7,
     8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
    18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    34,    35
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     4,     6,     8,    10,    12,    17,    21,    23,
    25,    27,    29,    31,    33,    35,    37,    42,    48,    54,
    60,    66,    72,    74,    77,    79,    81,    83,    85,    87,
    89,    91,    94,    96,    99,   104,   108,   110,   112,   114,
   116,   118,   120,   122,   124,   126,   128,   130,   132,   134,
   136,   138,   140,   142,   144,   146,   148,   150,   152,   154,
   156,   158,   160,   162,   164,   166
};

static const short yyrhs[] = {    37,
     0,    38,     0,    39,     0,    42,     0,     7,     0,     8,
     0,    47,    40,    45,    48,     0,    47,    66,    48,     0,
    66,     0,    41,     0,    59,     0,    60,     0,    61,     0,
    62,     0,    54,     0,    55,     0,    47,    65,    37,    48,
     0,    47,    63,    37,    43,    48,     0,    47,    64,    37,
    43,    48,     0,    47,    56,    43,    37,    48,     0,    47,
    57,    37,    43,    48,     0,    47,    58,    37,    37,    48,
     0,    37,     0,    43,    37,     0,    51,     0,    49,     0,
    52,     0,    53,     0,    46,     0,    66,     0,    44,     0,
    45,    44,     0,    50,     0,    45,    50,     0,    47,    66,
    45,    48,     0,    47,    66,    48,     0,     3,     0,     4,
     0,     5,     0,     6,     0,     9,     0,    10,     0,    11,
     0,    12,     0,    13,     0,    14,     0,    15,     0,    16,
     0,    17,     0,    18,     0,    19,     0,    20,     0,    21,
     0,    22,     0,    23,     0,    24,     0,    25,     0,    26,
     0,    27,     0,    28,     0,    29,     0,    30,     0,    30,
     0,    32,     0,    33,     0,    34,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    46,    53,    58,    63,    69,    74,    80,    85,    91,    95,
   100,   104,   108,   112,   116,   120,   125,   130,   135,   140,
   145,   150,   156,   161,   167,   172,   177,   182,   187,   192,
   198,   203,   208,   213,   219,   224,   240,   241,   242,   243,
   244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
   254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
   264,   265,   266,   267,   268,   269
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","'('","')'",
"INDVAR","SEQVAR","TRUE","FALSE","STRING","INTEGER","REAL","EQ","NOTEQ","IMPLIES",
"IMPLIED","EQUIV","EQLESS","LESS","EQGREATER","GREATER","QUOTE","FORALL","EXISTS",
"AND","OR","NOT","IF","COND","LISTOF","DEFOBJECT","DEFFUNCTION","DEFRELATION",
"DEFLOGICAL","CONSTANT","PLACEHOLDER","form","sentence","logconst","relsent",
"relconst","relop","logsent","sentseq","term","termseq","funterm","tLPAREN",
"tRPAREN","tINDVAR","tSEQVAR","tSTRING","tINTEGER","tREAL","tEQ","tNOTEQ","tIMPLIES",
"tIMPLIED","tEQUIV","tEQLESS","tLESS","tEQGREATER","tGREATER","tAND","tOR","tNOT",
"tCONSTANT", NULL
};
#endif

static const short yyr1[] = {     0,
    36,    37,    37,    37,    38,    38,    39,    39,    40,    40,
    41,    41,    41,    41,    41,    41,    42,    42,    42,    42,
    42,    42,    43,    43,    44,    44,    44,    44,    44,    44,
    45,    45,    45,    45,    46,    46,    47,    48,    49,    50,
    51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    -1,    -1,    -1,    63,    64,    65,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    66
};

static const short yyr2[] = {     0,
     1,     1,     1,     1,     1,     1,     4,     3,     1,     1,
     1,     1,     1,     1,     1,     1,     4,     5,     5,     5,
     5,     5,     1,     2,     1,     1,     1,     1,     1,     1,
     1,     2,     1,     2,     4,     3,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1
};

static const short yydefact[] = {     0,
    37,     5,     6,     1,     2,     3,     4,     0,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    56,    57,    58,
    66,     0,    10,    15,    16,     0,     0,     0,    11,    12,
    13,    14,     0,     0,     0,     9,    39,    40,    41,    42,
    43,    31,     0,    29,     0,    26,    33,    25,    27,    28,
    30,    23,     0,     0,     0,     0,     0,     0,    38,     8,
    32,     7,    34,     0,    24,     0,     0,     0,     0,    17,
     0,    36,    20,    24,    21,    22,    18,    19,    35,     0,
     0,     0
};

static const short yydefgoto[] = {    80,
    52,     5,     6,    22,    23,     7,    53,    42,    43,    44,
     8,    60,    46,    47,    48,    49,    50,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    51
};

static const short yypact[] = {     7,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,    66,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,    14,-32768,-32768,-32768,     7,     7,     7,-32768,-32768,
-32768,-32768,     7,     7,     7,     5,-32768,-32768,-32768,-32768,
-32768,-32768,     2,-32768,   -18,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,     7,     7,     7,     7,     7,     5,-32768,-32768,
-32768,-32768,-32768,     2,     5,    34,     5,    34,    34,-32768,
     2,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    18,
    21,-32768
};

static const short yypgoto[] = {-32768,
     0,-32768,-32768,-32768,-32768,-32768,   -10,   -40,   -38,-32768,
   -21,     6,-32768,   -39,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    -6
};


#define	YYLAST		100


static const short yytable[] = {     4,
    45,    36,    61,    63,     1,    59,    37,    38,    59,     1,
    39,    40,    41,     2,     3,    21,     1,    81,    37,    38,
    82,    45,    39,    40,    41,    71,    54,    55,     0,     0,
    61,    63,    56,    57,    58,    21,     1,    59,    64,     0,
     2,     3,    45,    66,     0,    68,    69,    21,    62,    45,
     0,     0,    65,     0,    67,     0,     0,     0,     0,     0,
     0,     0,     0,    70,     0,    74,     0,    74,    74,    72,
    73,    75,    76,    77,    78,     0,    79,     9,    10,    11,
    12,    13,    14,    15,    16,    17,     0,     0,     0,    18,
    19,    20,     0,     0,     0,     0,     0,     0,     0,    21
};

static const short yycheck[] = {     0,
    22,     8,    43,    43,     3,     4,     5,     6,     4,     3,
     9,    10,    11,     7,     8,    34,     3,     0,     5,     6,
     0,    43,     9,    10,    11,    64,    27,    28,    -1,    -1,
    71,    71,    33,    34,    35,    34,     3,     4,    45,    -1,
     7,     8,    64,    54,    -1,    56,    57,    34,    43,    71,
    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    58,    -1,    66,    -1,    68,    69,    64,
    65,    66,    67,    68,    69,    -1,    71,    12,    13,    14,
    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    24,
    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/local/share/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/usr/local/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 47 "jkp.y"
{	
						// form: sentence
						KifParser._result = yyvsp[0].KifSent; 
						//System.out.println("Parsing result is: " + ($1).toString());
					;
    break;}
case 2:
#line 54 "jkp.y"
{ 
						// sentence :   logconst
						yyval.KifSent = yyvsp[0].KifSent;  
					;
    break;}
case 3:
#line 59 "jkp.y"
{ 
						// sentence: relsent
						yyval.KifSent = yyvsp[0].KifRelSent;  
					;
    break;}
case 4:
#line 64 "jkp.y"
{ 
						// sentence: logsent
						yyval.KifSent = yyvsp[0].KifSent;  
					;
    break;}
case 5:
#line 70 "jkp.y"
{ 
						// logconst: TRUE
						yyval.KifSent = new KifLogConst(true);
					;
    break;}
case 6:
#line 75 "jkp.y"
{ 
						// logconst: FALSE
						yyval.KifSent = new KifLogConst(false);
					;
    break;}
case 7:
#line 81 "jkp.y"
{ 
						// relsent: tLPAREN tCONSTANT termseq tRPAREN
						yyval.KifRelSent = new KifRelSent(yyvsp[-2].String, yyvsp[-1].KifSeq); 
					;
    break;}
case 8:
#line 86 "jkp.y"
{ 
						// relsent: tLPAREN tCONSTANT tRPAREN
						yyval.KifRelSent = new KifRelSent(yyvsp[-1].String); 
					;
    break;}
case 9:
#line 92 "jkp.y"
{
						yyval.String = yyvsp[0].String;
					;
    break;}
case 10:
#line 96 "jkp.y"
{
						yyval.String = yyvsp[0].String;
					;
    break;}
case 11:
#line 101 "jkp.y"
{
						yyval.String = yyvsp[0].String;
					;
    break;}
case 12:
#line 105 "jkp.y"
{
						yyval.String = yyvsp[0].String;
					;
    break;}
case 13:
#line 109 "jkp.y"
{
						yyval.String = yyvsp[0].String;
					;
    break;}
case 14:
#line 113 "jkp.y"
{
						yyval.String = yyvsp[0].String;
					;
    break;}
case 15:
#line 117 "jkp.y"
{
						yyval.String = yyvsp[0].String;
					;
    break;}
case 16:
#line 121 "jkp.y"
{
						yyval.String = yyvsp[0].String;
					;
    break;}
case 17:
#line 126 "jkp.y"
{ 
						// logsent :   tLPAREN tNOT sentence tRPAREN
						yyval.KifSent = new KifNegation (yyvsp[-1].KifSent);
					;
    break;}
case 18:
#line 131 "jkp.y"
{ 
						// logsent: tLPAREN tAND sentence sentseq tRPAREN
						yyval.KifSent = new KifConjunction(yyvsp[-2].KifSent, yyvsp[-1].KifSeq);
					;
    break;}
case 19:
#line 136 "jkp.y"
{ 
						// logsent: tLPAREN tOR sentence sentseq tRPAREN
						yyval.KifSent = new KifDisjunction(yyvsp[-2].KifSent, yyvsp[-1].KifSeq);
					;
    break;}
case 20:
#line 141 "jkp.y"
{ 
						// logsent: tLPAREN tIMPLIES sentseq sentence tRPAREN
						yyval.KifSent = new KifImplication(yyvsp[-1].KifSent, yyvsp[-2].KifSeq);
					;
    break;}
case 21:
#line 146 "jkp.y"
{ 
						// logsent: tLPAREN tIMPLIED sentence sentseq tRPAREN
						yyval.KifSent = new KifImplication(yyvsp[-2].KifSent, yyvsp[-1].KifSeq);
					;
    break;}
case 22:
#line 151 "jkp.y"
{ 
						// logsent: tLPAREN tEQUIV sentence sentence tRPAREN
						yyval.KifSent = new KifEquivalence(yyvsp[-2].KifSent, yyvsp[-1].KifSent);
					;
    break;}
case 23:
#line 157 "jkp.y"
{ 
						// sentseq:   sentence
						yyval.KifSeq = new KifSeq(yyvsp[0].KifSent); 
					;
    break;}
case 24:
#line 162 "jkp.y"
{ 
						// sentseq: sentseq sentence
						yyval.KifSeq = new KifSeq(yyvsp[-1].KifSeq, yyvsp[0].KifSent);
					;
    break;}
case 25:
#line 168 "jkp.y"
{ 
						// term :   tSTRING
						yyval.KifTerm = new KifString(yyvsp[0].String);
					;
    break;}
case 26:
#line 173 "jkp.y"
{ 
						// term :   tINDVAR
						yyval.KifTerm = new KifIndvar(yyvsp[0].String);
					;
    break;}
case 27:
#line 178 "jkp.y"
{ 
						// term :   tINTEGER
						yyval.KifTerm = new KifInt(yyvsp[0].String);
					;
    break;}
case 28:
#line 183 "jkp.y"
{ 
						// term :   tREAL
						yyval.KifTerm = new KifReal(yyvsp[0].String);
					;
    break;}
case 29:
#line 188 "jkp.y"
{ 
						// term : funterm
						yyval.KifTerm = yyvsp[0].KifTerm;
					;
    break;}
case 30:
#line 193 "jkp.y"
{ 
						// term : object constant
						yyval.KifTerm = new KifObjConst(yyvsp[0].String);
					;
    break;}
case 31:
#line 199 "jkp.y"
{ 
						// termseq:   term
						yyval.KifSeq = new KifSeq(yyvsp[0].KifTerm);  
					;
    break;}
case 32:
#line 204 "jkp.y"
{ 
						// termseq: termseq term
						yyval.KifSeq = new KifSeq(yyvsp[-1].KifSeq, yyvsp[0].KifTerm);  
					;
    break;}
case 33:
#line 209 "jkp.y"
{
						// termseq:   tSEQVAR
						yyval.KifSeq = new KifSeq(yyvsp[0].KifSeqvar);  
					;
    break;}
case 34:
#line 214 "jkp.y"
{ 
						// term termseq tSEQVAR
						yyval.KifSeq = new KifSeq(yyvsp[-1].KifSeq, yyvsp[0].KifSeqvar);  
					;
    break;}
case 35:
#line 220 "jkp.y"
{
						// funterm: tLPAREN tCONSTANT termseq tRPAREN
						yyval.KifTerm = new KifFunTerm(yyvsp[-2].String, yyvsp[-1].KifSeq);
					;
    break;}
case 36:
#line 225 "jkp.y"
{
						// funterm: tLPAREN tCONSTANT tRPAREN
						yyval.KifTerm = new KifFunTerm(yyvsp[-1].String);
					;
    break;}
case 37:
#line 240 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 38:
#line 241 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 39:
#line 242 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 40:
#line 243 "jkp.y"
{yyval.KifSeqvar = new KifSeqvar (new String(yytext)); ;
    break;}
case 41:
#line 244 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 42:
#line 245 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 43:
#line 246 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 44:
#line 247 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 45:
#line 248 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 46:
#line 249 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 47:
#line 250 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 48:
#line 251 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 49:
#line 252 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 50:
#line 253 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 51:
#line 254 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 52:
#line 255 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 53:
#line 256 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 54:
#line 257 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 55:
#line 258 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 56:
#line 259 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 57:
#line 260 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 58:
#line 261 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 59:
#line 262 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 60:
#line 263 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 61:
#line 264 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 62:
#line 265 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 63:
#line 266 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 64:
#line 267 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 65:
#line 268 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
case 66:
#line 269 "jkp.y"
{yyval.String = new String (yytext); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/usr/local/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 272 "jkp.y"


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

