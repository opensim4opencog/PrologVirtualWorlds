package jkp;


public
abstract
class YYtokentypes {

public static final int EOF = 0;

public static final int tokenmin = 258;
public static final int tokenmax = 288;

public static Integer[] Tokentype;

static
{
  Tokentype = new Integer[tokenmax+1];
  for(int i=0;i<=tokenmax;i++) {Tokentype[i] = new Integer(i);}
}

public static final Integer YYEOF = Tokentype[EOF];

public static final Integer INDVAR = YYtokentypes.Tokentype[258];
public static final Integer SEQVAR = YYtokentypes.Tokentype[259];
public static final Integer TRUE = YYtokentypes.Tokentype[260];
public static final Integer FALSE = YYtokentypes.Tokentype[261];
public static final Integer STRING = YYtokentypes.Tokentype[262];
public static final Integer INTEGER = YYtokentypes.Tokentype[263];
public static final Integer REAL = YYtokentypes.Tokentype[264];
public static final Integer EQ = YYtokentypes.Tokentype[265];
public static final Integer NOTEQ = YYtokentypes.Tokentype[266];
public static final Integer IMPLIES = YYtokentypes.Tokentype[267];
public static final Integer IMPLIED = YYtokentypes.Tokentype[268];
public static final Integer EQUIV = YYtokentypes.Tokentype[269];
public static final Integer EQLESS = YYtokentypes.Tokentype[270];
public static final Integer LESS = YYtokentypes.Tokentype[271];
public static final Integer EQGREATER = YYtokentypes.Tokentype[272];
public static final Integer GREATER = YYtokentypes.Tokentype[273];
public static final Integer QUOTE = YYtokentypes.Tokentype[274];
public static final Integer FORALL = YYtokentypes.Tokentype[275];
public static final Integer EXISTS = YYtokentypes.Tokentype[276];
public static final Integer AND = YYtokentypes.Tokentype[277];
public static final Integer OR = YYtokentypes.Tokentype[278];
public static final Integer NOT = YYtokentypes.Tokentype[279];
public static final Integer IF = YYtokentypes.Tokentype[280];
public static final Integer COND = YYtokentypes.Tokentype[281];
public static final Integer LISTOF = YYtokentypes.Tokentype[282];
public static final Integer DEFOBJECT = YYtokentypes.Tokentype[283];
public static final Integer DEFFUNCTION = YYtokentypes.Tokentype[284];
public static final Integer DEFRELATION = YYtokentypes.Tokentype[285];
public static final Integer DEFLOGICAL = YYtokentypes.Tokentype[286];
public static final Integer CONSTANT = YYtokentypes.Tokentype[287];
public static final Integer PLACEHOLDER = YYtokentypes.Tokentype[288];


public static final String tokennames[] = {
"INDVAR", "SEQVAR", "TRUE", "FALSE", "STRING", "INTEGER", "REAL", "EQ", "NOTEQ", "IMPLIES", "IMPLIED", "EQUIV", "EQLESS", "LESS", "EQGREATER", "GREATER", "QUOTE", "FORALL", "EXISTS", "AND", "OR", "NOT", "IF", "COND", "LISTOF", "DEFOBJECT", "DEFFUNCTION", "DEFRELATION", "DEFLOGICAL", "CONSTANT", "PLACEHOLDER", null
};

public static String tokenname(Integer t) {return tokenname(t.intValue());}

public static String tokenname(int t)
{
    if(t >= tokenmin && t <= tokenmax) {
	return tokennames[t - tokenmin];
    } else if(t > 0 && t <= 255) {
	return "'" + String.valueOf((char)t) + "'";
    } else if(t == EOF) {
	return "EOF";
    } else {
	return "$undefined$";
    }
}

//////////////////////////////////////////////////

public static final int nontermmin = 289;
public static final int nontermmax = 319;

public static final int nt_form = 289;
public static final int nt_sentence = 290;
public static final int nt_logconst = 291;
public static final int nt_relsent = 292;
public static final int nt_relconst = 293;
public static final int nt_relop = 294;
public static final int nt_logsent = 295;
public static final int nt_sentseq = 296;
public static final int nt_term = 297;
public static final int nt_termseq = 298;
public static final int nt_funterm = 299;
public static final int nt_tLPAREN = 300;
public static final int nt_tRPAREN = 301;
public static final int nt_tINDVAR = 302;
public static final int nt_tSEQVAR = 303;
public static final int nt_tSTRING = 304;
public static final int nt_tINTEGER = 305;
public static final int nt_tREAL = 306;
public static final int nt_tEQ = 307;
public static final int nt_tNOTEQ = 308;
public static final int nt_tIMPLIES = 309;
public static final int nt_tIMPLIED = 310;
public static final int nt_tEQUIV = 311;
public static final int nt_tEQLESS = 312;
public static final int nt_tLESS = 313;
public static final int nt_tEQGREATER = 314;
public static final int nt_tGREATER = 315;
public static final int nt_tAND = 316;
public static final int nt_tOR = 317;
public static final int nt_tNOT = 318;
public static final int nt_tCONSTANT = 319;


public static final String nontermnames[] = {
"form", "sentence", "logconst", "relsent", "relconst", "relop", "logsent", "sentseq", "term", "termseq", "funterm", "tLPAREN", "tRPAREN", "tINDVAR", "tSEQVAR", "tSTRING", "tINTEGER", "tREAL", "tEQ", "tNOTEQ", "tIMPLIES", "tIMPLIED", "tEQUIV", "tEQLESS", "tLESS", "tEQGREATER", "tGREATER", "tAND", "tOR", "tNOT", "tCONSTANT", null
};

public static String nontermname(int t)
{
    if(t >= nontermmin && t <= nontermmax) {
	return nontermnames[t - nontermmin];
    } else {
	return "$undefined$";
    }
}

//////////////////////////////////////////////////
};
