:- ensure_loaded('moo_swiprolog.pl').
:- include('moo_header.pl').

% ===================================================================
% IMPORTS
% ===================================================================
%:-include('moo_header.pl').

%S=(instance(A, 'BodyJunction')=>exists(B, exists(C, connected(A, C)and connected(A, B)and instance(C, 'AnatomicalStructure')and instance(B, 'AnatomicalStructure')and not equal(C, B)))),

cons:- !,
      %catch(cd('../../knowledge_bases/'),_,true),
      pwd,
      repeat, %told,seen,
      (unsetMooOption(client=html)),
      once(console_loop(C)),
      fail.

writeMooMenu:-
         writeFmt('ask - switches to ask mode \ntell - switches back to tell mode\n '),
         writeFmt('other cmds: statistics, ls, halt, contexts \n'),!.



:-dynamic(console_mode/1).
:-assert_n(console_mode(ask)).

console_loop(C):- console_mode(M),!,
      console_loop(M ,C).

console_loop(tell,SOURCEFORM):- 
            once(console_read('Insert> ',SOURCEFORM,Vars)),    
            invokeOperation(verbose,assert(SOURCEFORM),'GlobalContext',TN,'PrologMOO',CM,Vars).

console_loop(ask,SOURCEFORM):- 
            once(console_read('Ask> ',SOURCEFORM,Vars)),
            invokeOperation(verbose,request(SOURCEFORM),Ctx,TN,KB,CM,Vars).
      
console_loop(cmd,SOURCEFORM):- 
            once(console_read('Command> ',SOURCEFORM,Vars)),
            invokeOperation(verbose,cmd(SOURCEFORM),Ctx,TN,KB,CM,Vars).

console_read(P,FORM,Vars):-
         nl,write(P),cons_read(Askion_Chars),
         logOnFailure(getCleanCharsWhitespaceProper(Askion_Chars,Show)),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
              catch(once(( NEWFORM=comment(_) -> 
                     (ignore(do_chars(Show)),!,FORM=_) ;(!,
		     FORM=NEWFORM      ))),_,fail).

cons_read(Chars):-repeat,told,seen,readKIF(Chars),Chars=[_,_|_].

lpInsert(Insert_chars):-
      tell(Insert_chars,Ctx,TN,KB,CM).

lpInsert_file(File):-lp_file(File).

lp_list:-
      writeFmt('Listing of the current assertions:\n',[]),
      listing(mooCache),
      writeFmt('Done.\n',[]).

lp_clear:-do_clear.

lp_save(FileName):-!.

lp_ask(Chars):-ask(Chars,_Cxt,KB).

   
cons_help:-!.

do_chars([59|Chars]):-!,do_chars(Chars).
do_chars(Chars):-tokenize3(Chars,Tokens),!,do_tokens(Tokens).

 

do_tokens([(;)|Toks]):-!,do_tokens(Toks).
do_tokens([tell]):-writeFmt('% Entering tell mode.\n',[]),retractall(console_mode(_)),assert(console_mode(tell)).
do_tokens([ask]):- writeFmt('% Entering ask mode.\n',[]),retractall(console_mode(_)),assert(console_mode(ask)).
do_tokens([cmd]):- writeFmt('% Entering command mode.\n',[]),retractall(console_mode(_)),assert(console_mode(cmd)).
do_tokens([clear]):- clear_moo_memory.
do_tokens([list]):- lp_list.
do_tokens([load,Filename]):- do_tokens([load,Filename,'GlobalContext']).
do_tokens([load,Filename,Ctx]):- ctxFromFile(Ctx,Filename,'PrologMOO').

%do_chars(X):-string_concat(";find ",Word,X),find_word(Word).
do_tokens([make]):- make.
do_tokens([help]):- writeMooMenu.
do_tokens([halt]):- halt.
do_tokens([bye]):- abort.
%do_chars([116,109|Number]):-catch((number_codes(Value,Number),set_tm_level(Value)),_,show_tm_level).
do_tokens([prolog]):- writeFmt('Type ""cons"" to return to Logic Engine.\n ',[]),abort.
do_tokens([can,X]):-canonicalizeMooKBHTML('PrologMOO',X).
do_tokens([can,X]):-canonicalizeMooKBHTML('PrologMOO',X).
do_tokens([can]):-do_tokens([can,'GlobalContext']).
do_tokens(L):-P=..L,once(P),!.
do_tokens(X):-writeFmt('could not parse: ~q.\n',[X]).
%do_chars(PCHARS):- atom_codes(Atom,PCHARS),catch(atom_to_term(Atom,Term,Vars),_,fail),!,catch(my_call(Term,Vars),_,fail).



kb_define(KB):-invokeInsert(forall,surface,'instance'(KB,'KnowledgeBase'),'GlobalContext',TN,KB,Vars,Maintainer),
                     invokeInsert(forall,surface,'instance'('GlobalContext','Context'),'GlobalContext',TN2,KB,Vars,Maintainer),
                     ensureMooKB(KB,'GlobalContext').

ctx_define(Ctx):-invokeInsert(forall,surface,'instance'(Ctx,'Context'),'GlobalContext',TN,SKB,Vars,Maintainer),ensureMooKB(KB,Ctx).

set_ctx(Context):-ensureMooKB(KnowledgeBase,Context).
set_kb(KnowledgeBase):-ensureMooKB(KnowledgeBase,'GlobalContext').

my_call(fol,_):-retractall(version_tag(_)),assert(version_tag(fol)),writeFmt('% Entering FOL mode.\n',[]),!.
my_call(kbl,_) :-retractall(version_tag(_)),assert(version_tag(kbl)),writeFmt('% Entering KBL mode.\n',[]),!.

my_call(cd(Term),_Vars):-cd(Term),pwd.

my_call(file(Term),_Vars):-lp_file(Term),writeFmt('% Entering ask mode.\n',[]),retractall(console_mode(_)),assert(console_mode(ask)).
my_call(file(Term),_Vars):- writeFmt('% Entering ask mode.\n',[]),retractall(console_mode(_)),assert(console_mode(ask)).

my_call(Term,_Vars):-once(Term).


do_chars(Show):-%,catch(fmtString(Atom,'"~s"',[Show]),_,ignore(Atom=surf)),
	sendNote(user,'kifParser','KIF Unreadable (Syntax error: Unbalanced parentheses  )',Atom).


call_nth_times(Number,Prolog):-
                     retractall(call_count(_)),assert(call_count(Number)),repeat,
                     (Prolog),
                     ignore((retract(call_count(Q)),QQ is Q -1,
                     assert(call_count(QQ)))),
                     call_count(0).

test_batch_stdout(FileName):- test_batch_stdout(FileName,CM,CPU,RESULT,Title).

test_batch_stdout(FileName,CM):- test_batch_stdout(FileName,CM,CPU,RESULT,Title).


