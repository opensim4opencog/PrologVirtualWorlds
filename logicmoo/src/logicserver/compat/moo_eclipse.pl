

%:-use_module(library(quintus)).
%:- use_module(library(fd)). 
 %use_module(library(cprolog)),consult('moo_eclipse.pl').
%:-use_module(library(cprolog)).
:-use_module(library(iso)).



%multifile(_).

%set_prolog_flag(_,_).


%:-op(600,xfy,':').
 

arithmetic_function(_).

flag(Name,Out,In):-
	flag_db(Name,Out),
	set_the_flag(Name,In).

:-dynamic(in_flag_db/2).

index(_).	

	
	

flag_db(Name,Out):-in_flag_db(Name,Out),!.
flag_db(Name,0).

set_the_flag(Name,In):-
	retractall(in_flag_db(Name,_)),
	set_the_flag_1(Name,In).
	
set_the_flag_1(Name,V):-var(V).

set_the_flag_1(Name,X + Y):-Z is X + Y,!,
	     set_the_flag_1(Name,Z).
set_the_flag_1(Name,X - Y):-Z is X - Y,!,
	     set_the_flag_1(Name,Z).
set_the_flag_1(Name,V):-asserta(in_flag_db(Name,V)).

style_check(_).


ignore(X):-(call(X);true).

absolute_file_name(X,X).

include_header:-!.

:-['moo_header.pl'].


writeModeSet(Mode):-
	(unsetMooOption(client=_)),
	(setMooOption(client=Mode)),!.


:-absolute_file_name(.,Local),assert('LOGIC_ENGINE_RT'(Local)),assert(library_directory(Local)).
:-absolute_file_name('../..',X),assert('ROOT_RT'(X)).

:-dynamic(ado_cnnstr/1).

:- ( not(ado_cnnstr(_)) -> ('ROOT_RT'(X),atom_concat(X,'/tmp/moo_prolog.mdb',O),assert(ado_cnnstr(O))) ; true ).


:- dynamic mooCache/1.
:- dynamic mooCache/6.
	
:-['moo_temporal.pl'].
:-['moo_reader.pl'].
:-['moo_notes.pl'].
:-['moo_soap.pl'].
:-['moo_transitivity.pl'].
:-['moo_request_buffer.pl'].
:-['moo_belief.pl'].
:-['moo_enable.pl'].
:-['moo_canonicalize.pl'].
:-['moo_api_test.pl'].
:-['moo_utility.pl'].
:-['moo_builtin.pl'].
:-['moo_suo.pl'].
:-['moo_ace.pl'].
:-['moo_kif_check.pl'].
:-['moo_file_functions.pl'].
:-['moo_wfs_safety.pl'].
:-['moo_theorems.pl'].
:-['moo_hashing.pl'].
:-['moo_assert.pl'].
:-['moo_retract.pl'].
:-['moo_arg_domains.pl'].
:-['moo_surf_to_can.pl'].
:-['moo_surface_belief.pl'].
:-['moo_reduce.pl'].
:-['moo_functions.pl'].
%:-['moo_can_to_wfs.pl'].
:-['moo_operation.pl'].
:-['moo_modules.pl'].
:-['moo_krlog.pl'].
:-['moo_database.pl'].
:-['agentConsultation.pl'].
:-['moo_useragent.pl'].
:-['moo_useragent.pl'].
:-['moo_response.pl'].
:-['moo_request.pl'].
:-['moo_truth_maintains.pl'].
:-['moo_tester.pl'].
:-['moo_translit.pl'].
:-['moo_markup.pl'].
:-['moo_httpd.pl'].
:-['moo_image.data'].
:-['moo_equal.pl'].
:-['moo_request.pl'].


get_time(6667).
convert_time(T,'Now is the time').
convert_time(T,1,2,3,4,5,6,7).

numbervars(X,_,Y,Z):-numbervars(X,Y,Z).


:-setMooOptionDefaults.
%:-set_prolog_flag(unknown,fail).

 
string([_|_]).

writeFmt(Format,Args):-!,current_output(Stream),writeFmt(Stream,Format,Args).

 


writeFmt(Loc,[],Args):-!.
writeFmt(user_error,F,Args):-!,
	writeFmt(user_output,F,Args).
writeFmt(Loc,Atom,Args):-atom(Atom),atom_codes(Atom,String),!,
	writeFmt(Loc,String,Args).
writeFmt(Loc,[126,110|Rest],Args):-!,nl(Loc),
	writeFmt(Loc,Rest,Args),!.
writeFmt(Loc,[126,N|Rest],[A|Rgs]):-!,
	wformat(Loc,N,A),!,
	writeFmt(Loc,Rest,Rgs),!.
writeFmt(Loc,[N|Rest],Rgs):-!,
	put_code(Loc,N),
	writeFmt(Loc,Rest,Rgs),!.
writeFmt(Loc,Rest,Rgs):-!,throw('fomat_error'(writeFmt(Loc,Rest,Rgs))).	


fmtString(Format,Args):-!,current_output(Stream),fmtString(Stream,Format,Args).

fmtString([],[],Args):-!.
fmtString(Loc,Atom,Args):-atom(Atom),atom_codes(Atom,String),!,
	fmtString(Loc,String,Args).
fmtString([10|Loc],[126,110|Rest],Args):-!,nl(Loc),
	fmtString(Loc,Rest,Args),!.
fmtString(OLOc,[126,N|Rest],[A|Rgs]):-!,
	swformat(LocN,N,A),!,
	fmtString(Loc,Rest,Rgs),!,
	append(LocN,Loc,OLOc).
fmtString([N|Loc],[N|Rest],Rgs):-!,
	fmtString(Loc,Rest,Rgs),!.
fmtString(Loc,Rest,Rgs):-!,throw('fomat_error'(fmtString(Loc,Rest,Rgs))).	

wformat(Loc,_,C):-string(C),!,atom_codes(A,C),write(Loc,A).
wformat(Loc,119,A):-!,write(Loc,A). % ~w
wformat(Loc,113,A):-!,writeq(Loc,A). % ~q
wformat(Loc,103,A):-!,write(Loc,A). % ~g
wformat(Loc,N,A):-!,put_code(Loc,N),put_code(Loc,A),!.

swformat(A,_,A):-string(A),!.
swformat(C,_,A):-atom(A),!,atom_codes(A,C),!.
swformat(String,119,A):-!,term2string(A,String). % ~w
swformat(String,113,A):-!,term2string(A,String). % ~q
swformat(String,103,A):-!,term2string(A,String). % ~g
swformat([N,A],N,A):-!.




%:-ensure_loaded(moo_pttp_original).
%:-ensure_loaded(moo_fol).
%:-ensure_loaded(moo_normal).


/*
recompile_all :- !,make,
         ['moo_dynamics.pl'],
         ['moo_language.pl'].
*/
recompile_all :- make.


%         get_storage_file("moo_dynamics.kif",Name),
%         load_kif_as_prolog(Name),
 %        get_storage_file("moo_language.kif",Name1),
 %        load_kif_as_prolog(Name1).

:-recompile_all.

%:-ensure_loaded(moo_console).

getCleanCharsWhitespaceProper([X],[]):-is_whitespace_code(X).
getCleanCharsWhitespaceProper([F|X],W):-is_whitespace_code(F),getCleanCharsWhitespaceProper(X,W).
getCleanCharsWhitespaceProper(X,X):-!.

list_to_set(List1,List2):-sort(List1,List2).

string_clean(X,X).



%['moo_bprolog.pl'],[tests_for_doug],load_ontologies.

ttta:-term_to_atom('T'(A),X),nl,writeq(X),nl.

term_to_atom(T,A):-break,
	open(term_to_atom,write,W,[type(binary)]),
	writeq(W,T),
	close(W,[force(true)]),!,
	open(term_to_atom,read,R),
	get_all_codes(R,Codes),
	atom_codes(A,Codes),
	close(R,[force(true)]).!.
	
get_all_codes(R,[]):-at_end_of_stream(R),!.
get_all_codes(R,[C|Odes]):-
	get_code(R,C),
	get_all_codes(R,Odes),!.
	
	
%atom_to_term(Atom,Term,Vars):-parse_atom(Atom,Term,Vars).

is_whitespace_code(32).
is_whitespace_code(X):-X<32.




unnumbervars(X,Y):-
	getPrologVars(X,List,_,_),
	recopy_each_var(X,List,Y).

recopy_each_var(X,[],X).
recopy_each_var(X,[V|List],Y):-
	subst(X,V,NewVar,O),
	recopy_each_var(O,List,Y),!.
	
	


