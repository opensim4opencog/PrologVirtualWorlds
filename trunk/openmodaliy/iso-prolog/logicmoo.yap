% ===================================================================
% PURPOSE
% This File is the bootstrap YAP-Prolog listener to handle API requests
% So first is loads the proper files and then starts up the system
% ===================================================================


:-use_module(library(atts)).  
:-use_module(library(chr)).  
:-use_module(library(assoc)).  
:-use_module(library(lists)).  
:-use_module(library(heaps)).  
:-use_module(library(system)).  
:-use_module(library(charsio)).



thread_self(main).
registerCycPred(_,_).

user:index(_).
user:style_check(_).
flag(Name,Before,After):-  get_value(Name,[]),!,Before = 0,Save is After,!,set_value(Name,Save).
flag(Name,Before,After):- get_value(Name,Before),Save is After,set_value(Name,Save).
% nondet_arg(N,erm(a,A,c),V).
nondet_arg(N,Term,V):- 
	 functor(Term,_,A),
	 member(N,[1,2,3,4,5,6,7,8,9,10,11,12]),
	 arg(N,Term,V),A >= N.

user:at_halt(_).
	
gensym(Atom,Out):-flag(Atom,Before,Before+1), number_atom(Before,N),atom_concat(Atom,N,Out).

concat_atom(L,Atom):-concat_atom(L,'',Atom).
concat_atom(L,Sep,Atom):-nonvar(L),!,concat_atom_lr(L,Sep,Atom).
concat_atom(L,Sep,Atom):-trace,concat_atom_rl(L,Sep,Atom).

concat_atom_lr([L],_,L):-!.
concat_atom_lr([L|List],Sep,Atom):-atom_concat(L,Sep,LS),concat_atom(List,Sep,Mid),atom_concat(LS,Mid,Atom).
		
%concat_atom_rl(L,Sep,Atom):-atom_concat(L,Sep,Atom)

get_time(Time):-datime(datime(Year,Month,DayOfTheMonth,Hour,Minute, Second)),
		Time is Second + Minute * 60 + Hour * 3600 + DayOfTheMonth * 86400 + Month * 2592000 + (Year-1970) * 31104000.


convert_time(Time, String):-number_atom(Time, String).

sformat(Result, Form, Args):- format_to_chars(Form, Args, Result).

ignore(Call):-call(Call).
ignore(Call).

tcp_setopt(_,_).

prolog_system(yap).

:-ensure_loaded('logicmoo.pl').


