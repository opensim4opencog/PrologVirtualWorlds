
:-include('moo_header.pl').

% =======================================================================================
% ACTIVATE CONTEXT DAG
% =======================================================================================

% on a dirty ctx/kb run remake_positive_cache(KB,Ctx)
% on a dirty ctx/kb run remake_negative_cache(KB,Ctx)




negative_fact_cache(KB,Ctx,+X):-not(positive_fact_cache(KB,Ctx,_,X)).
negative_fact_cache(KB,Ctx,-X):-not(positive_fact_cache(KB,Ctx,_,X)).

negative_rule_cache(KB,Ctx,+X):-not(positive_rule_cache(KB,Ctx,_,X)).
negative_rule_cache(KB,Ctx,-X):-not(positive_rule_cache(KB,Ctx,_,X)).

ambiguous_fact_cache(KB,Ctx,+X):- (positive_fact_cache(KB,Ctx,?,X)).
ambiguous_fact_cache(KB,Ctx,-X):- (positive_fact_cache(KB,Ctx,?,X)).

ambiguous_rule_cache(KB,Ctx,+X):- (positive_rule_cache(KB,Ctx,?,X)).
ambiguous_rule_cache(KB,Ctx,-X):- (positive_rule_cache(KB,Ctx,?,X)).


holds_wrapper(KB,Ctx,R,X,Y):-nonvar(R),!,
        P=..[R,X,Y],
        all_predicate_cache(KB,Ctx,+P).

holds_wrapper(KB,Ctx,R,X,Y,Z):-nonvar(R),!,
        P=..[R,X,Y,Z],
        all_predicate_cache(KB,Ctx,+P).

holds_wrapper(KB,Ctx,R,X,Y,Z,Q):-nonvar(R),!,
        P=..[R,X,Y,Z,Q],
        all_predicate_cache(KB,Ctx,+P).

holds_wrapper(KB,Ctx,R,X,Y):-
        all_predicate_cache(KB,Ctx,+P),
        P=..[R,X,Y].

holds_wrapper(KB,Ctx,R,X,Y,Z):-
        all_predicate_cache(KB,Ctx,+P),
        P=..[R,X,Y,Z].

holds_wrapper(KB,Ctx,R,X,Y,Z,Q):-
        all_predicate_cache(KB,Ctx,+P),
        P=..[R,X,Y,Z,Q].

not_holds_wrapper(KB,Ctx,R,X,Y):-nonvar(R),!,
        P=..[R,X,Y],
        all_predicate_cache(KB,Ctx,-P).

not_holds_wrapper(KB,Ctx,R,X,Y,Z):-nonvar(R),!,
        P=..[R,X,Y,Z],
        all_predicate_cache(KB,Ctx,-P).

not_holds_wrapper(KB,Ctx,R,X,Y,Z,Q):-nonvar(R),!,
        P=..[R,X,Y,Z,Q],
        all_predicate_cache(KB,Ctx,-P).

not_holds_wrapper(KB,Ctx,R,X,Y):-
        all_predicate_cache(KB,Ctx,-P),
        P=..[R,X,Y].

not_holds_wrapper(KB,Ctx,R,X,Y,Z):-
        all_predicate_cache(KB,Ctx,-P),
        P=..[R,X,Y,Z].

not_holds_wrapper(KB,Ctx,R,X,Y,Z,Q):-
        all_predicate_cache(KB,Ctx,-P),
        P=..[R,X,Y,Z,Q].

positive_rule_fact(KB,Ctx,X):-
        positive_fact_cache(KB,Ctx,X),
        positive_rule_cache(KB,Ctx,X).

positive_rule_only(KB,Ctx,X):-
        positive_rule_cache(KB,Ctx,X),
        not(positive_fact_cache(KB,Ctx,X)).

positive_fact_only(KB,Ctx,X):-
        positive_fact_cache(KB,Ctx,X),
        not(positive_rule_cache(KB,Ctx,X)).


all_predicate_cache(KB,Ctx,X):-
        positive_fact_cache(KB,Ctx,X).
all_predicate_cache(KB,Ctx,X):-
        positive_rule_cache(KB,Ctx,X).

positive_fact_cache(KB,Ctx,+X):-positive_fact_cache(KB,Ctx,+,X).
positive_fact_cache(KB,Ctx,-X):-positive_fact_cache(KB,Ctx,-,X).
positive_fact_cache(KB,Ctx,+X):-positive_fact_cache(KB,Ctx,?,X).
positive_fact_cache(KB,Ctx,-X):-positive_fact_cache(KB,Ctx,?,X).
positive_rule_cache(KB,Ctx,+X):-var(X),!,positive_rule_cache(KB,Ctx,+,X).
 % Note that -X is missing from positive rule cache when its a X is a Var
positive_rule_cache(KB,Ctx,+X):-positive_rule_cache(KB,Ctx,+,X).
positive_rule_cache(KB,Ctx,-X):-positive_rule_cache(KB,Ctx,-,X).
positive_rule_cache(KB,Ctx,+X):-positive_rule_cache(KB,Ctx,?,X).
positive_rule_cache(KB,Ctx,-X):-positive_rule_cache(KB,Ctx,?,X).


all_predicate_fast_cache(KB,Ctx,X):-positive_fact_cache(KB,Ctx,X).
all_predicate_fast_cache(KB,Ctx,X):-positive_rule_cache(KB,Ctx,X).

all_predicate_slow_cache(KB,Ctx,X):-positive_fact_cache(KB,Ctx,X).
all_predicate_slow_cache(KB,Ctx,X):-positive_rule_only(KB,Ctx,X).




remake_positive_cache(KB,Ctx):-
        unmake_positive_cache(KB,Ctx),
        make_positive_cache(KB,Ctx),!.

unmake_positive_cache(KB,Ctx):-
        retractall((make_positive_cache(KB,Ctx):-!)).

make_positive_cache(KB,Ctx):-
        retractall(positive_fact_cache(_,_,_,_)),
        retractall(positive_rule_cache(_,_,_,_)),
        retractall(negative_fact_cache(_,_,_,_)),
        retractall(negative_rule_cache(_,_,_,_)),
        fail.

make_positive_cache(KB,Ctx):-
        mooCache(R,Cons, Ante, _, L, KB, Ctx, Explaination),
        add_positive_rule_cache(KB,Ctx,L,R),
        fail.

make_positive_cache(KB,Ctx):-
        mooCache(R,Fact, _, L, KB, Ctx, Explaination),
        add_positive_fact_cache(KB,Ctx,L,R),
        fail.

make_positive_cache(KB,Ctx):-
        asserta((make_positive_cache(KB,Ctx):-!)),!.


add_positive_fact_cache(KB,Ctx,L,R):-
        get_arity_fast(KB,Ctx,R,A),!,
        functor(P,R,A),
        not(positive_fact_cache(KB,Ctx,?,P)),
                ((L=true  ->
                        ((
                        retract(positive_fact_cache(KB,Ctx,-,P)) ->
                                asserta(positive_fact_cache(KB,Ctx,?,P)) ;
                                assert_if_new(positive_fact_cache(KB,Ctx,+,P))
                        ));
                        ((
                        retract(positive_fact_cache(KB,Ctx,+,P)) ->
                                asserta(positive_fact_cache(KB,Ctx,?,P)) ;
                                assert_if_new(positive_fact_cache(KB,Ctx,-,P))
                        ))
                )),!.

add_positive_rule_cache(KB,Ctx,L,R):-
        get_arity_fast(KB,Ctx,R,A),!,
        functor(P,R,A),
        not(positive_rule_cache(KB,Ctx,?,P)),
                ((L=true  ->
                        ((
                        retract(positive_rule_cache(KB,Ctx,-,P)) ->
                                asserta(positive_rule_cache(KB,Ctx,?,P)) ;
                                assert_if_new(positive_rule_cache(KB,Ctx,+,P))
                        ));
                        ((
                        retract(positive_rule_cache(KB,Ctx,+,P)) ->
                                asserta(positive_rule_cache(KB,Ctx,?,P)) ;
                                assert_if_new(positive_rule_cache(KB,Ctx,-,P))
                        ))
                )),!.


get_arity_fast(KB,Ctx,R,A):-have_arity(R,KB,Ctx,A),!.
get_arity_fast(KB,Ctx,R,A):-
        mooCache(R,Cons, _, _, KB, Ctx, Explaination),
        functor(Cons,R,A),
        asserta(have_arity(R,KB,Ctx,A)),!.
get_arity_fast(KB,Ctx,R,A):-
        mooCache(R,Cons, _, _, _, KB, Ctx, Explaination),
        functor(Cons,R,A),
        asserta(have_arity(R,KB,Ctx,A)),!.
get_arity_fast(KB,Ctx,R,A):-
        mooCache(valence, _, valence(R,A), Vars, KBName, Context, Tracking, User, Status),
        asserta(have_arity(R,KB,Ctx,A)),!.
get_arity_fast(KB,Ctx,R,2).


% ensureMooKB
% make_dead
% sync_memory

% should_be_loaded


%:-include('moo_header.pl').

activate_context(KnowledgeBase:Context):-
                ensureMooKB(KnowledgeBase,Context).

getDefaultImageFilepath(X):-
        'LOGIC_ENGINE_RT'(Local),concat_atom([Local,'/','moo_image.data'],X).


save_ado_cache:-saveMooCache.
                                                                                                                         
                                                                                                                         
saveMooCache:-
                save_can_to_file(KB,Handle),close(Handle),
                retractall(save_can_to_file(KB,Handle)),
                getDefaultImageFilepath(IF),
                [IF],!.

saveMooCache:-
        ensure_all_compiled,
        get_time(T),convert_time(T,String),
        remove_all_but_logOnFailure_chars(String,FileName),
        %  concat_atom(['cp -f moo_image.P "',FileName,'.persist.P" '],ShellCmd),
        %ignore(shell(ShellCmd)),
        %sendNote(debug,contentManager,'Saving ADO Cache Was Succesfull',ShellCmd),
        getDefaultImageFilepath(IF),
        tell(IF),
        op(0,xfy,'=>'),
        %op(0,xfx,#),
        %op(0,xfx,'#'),
        %op(0,xfx,'@'),
        op(0,fy,not),    % negation
        op(0,xfy,and),   % conjunction
        op(0,xfy,or),   % disjunction
        %op(0,xfy,:),
        op(0,xfx, 'equal' ),
        op(0,xfx,'<='),
        op(0,xfx,'if'),
        op(0,xfy,'=>'),
        op(0,fy,known),  % Found in Model
        op(0,fy,consistent),  % Not In Model
        op(0,fy,after),  % Next time
        op(0,fy,then),  % Next time
        op(0,xfy,=>),  % implication
        op(0,xfy,<=>), % equivalence
        op(0,fy,always),  % Necessity, Always
        op(0,fy,possible),  % Possibly, Eventually
        op(0,fy,necessary),  % Necessity
        writeFmt('
:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).
%:-set_prolog_flag(double_quotes,string).
:-index(mooCache(1)).
:-index(mooCache(1,1)).
:-index(mooCache(1,1,1)).
:-index(mooCache(1,1,1,1)).
:-index(mooCache(1,1,1,1,0)).
:-index(mooCache(1,0,1,1,1,0)).
:-index(mooCache(1,1,0,0,1,1,0,0,0)).
:-dynamic(mooCache/1).
:-dynamic(mooCache/2).
:-dynamic(mooCache/3).
:-dynamic(mooCache/4).
:-dynamic(mooCache/5).
:-dynamic(mooCache/6).
:-dynamic(mooCache/9).

',[]),
        listing(mooCache),
        told,
/*
        retractall(mooCache(_)),
        retractall(mooCache(_,_)),
        retractall(mooCache(_,_,_)),
        retractall(mooCache(_,_,_,_)),
        retractall(mooCache(Literal,asserted,KB,Ctx,surf(KB,TN,CLID,Vars))),  %Facts /5
        retractall(mooCache(Literal,deduced,KB,Ctx,surf(KB,TN,CLID,Vars))),  %Facts /5
        retractall(mooCache(Literal,AnteLiteral,asserted,KB,Ctx,surf(KB,TN,CLID,Vars))),         %Rules /6
        retractall(mooCache(Literal,AnteLiteral,deduced,KB,Ctx,surf(KB,TN,CLID,Vars))),  %Rules /6
        retractall(mooCache(Surface,CLF,Flags,Vars,KB,Ctx,TN,Maintainer,TMResult)),  %Surface /9
*/
        [IF],!,
        op(1000,xfy,'=>'),
        %op(500,xfx,'#'),
        %op(500,xfx,'@'),
        op(400,fy,not),    % negation
        op(500,xfy,and),   % conjunction
        op(600,xfy,or),   % disjunction
        %op(500,xfy,':'),
        op(0,xfx, 'equal' ),
        op(900,xfx,'<='),
        op(900,xfx,'if'),
        op(1000,xfy,'=>'),
        op(400,fy,known),  % Found in Model
        op(400,fy,consistent),  % Not In Model
        op(400,fy,after),  % Next time
        op(400,fy,then),  % Next time
        op(650,xfy,=>),  % implication
        op(700,xfy,<=>), % equivalence
        op(400,fy,always),  % Necessity, Always
        op(400,fy,possible),  % Possibly, Eventually
        op(400,fy,necessary).  % Necessity

save_each_kb([]).
save_each_kb([H|T]):-
        save_the_kb(H),!,
        save_each_kb(T),!.

save_the_kb(H):-!.
save_the_kb(H):-
        concat_atom(['moo_kbimage_',H,'.pl'],FileName),
        safe_file_open(FileName,'w',OUTPUT),
%       writeFmt(OUTPUT,':-include(\'moo_header.P\').\n',[]),
        writeFmt(OUTPUT,':-index(mooCache(1 ,0,1, 0,0,0, 0,0, 1)).\n',[]),
        writeFmt(OUTPUT,'% ~w for ~w.\n',[FileName,H]),
        get_time(Time),convert_time(Time,String),
        writeFmt(OUTPUT,'% Created at ~w.\n',[String]),
        ignore(write_the_kb_now(OUTPUT,H)),
        writeFmt(OUTPUT,':-catch([moo_eval],_,true).\n',[]),
        close(OUTPUT).


write_the_kb_now(OUTPUT,KnowledgeBase):-
        mooCache(PredR,wfs,WFS,Prolog,KnowledgeBase,Context,AssertionID,Creator,on),
        writeFmt(OUTPUT,'~q.\n',[Prolog]),
        fail.
write_the_kb_now(OUTPUT,KnowledgeBase):-
        get_time(Time),convert_time(Time,String),
        writeFmt(OUTPUT,'% Finished at ~w.\n',[String]).


remove_all_but_logOnFailure_chars(String,FileName):-
        atom_codes(String,Codes),
        subtract(Codes,[32,58],CCodes),
        atom_codes(FileName,CCodes).

:-dynamic(is_up_to_date/2).

ensureMooKB(KnowledgeBase,Context):-
        is_up_to_date(KnowledgeBase,Context),!.

ensureMooKB(KnowledgeBase,Context):-
        asserta(is_up_to_date(KnowledgeBase,Context)),!.

%       concat_atom(['moo_kbimage_',KnowledgeBase,'.pl'],FileName),
 %       ensure_loaded(FileName).

show_active_memory:-!.
show_active_memory:-listing('in-active-memory').





save_each_clause_in_buffer(KnowledgeBase,Context,(PRO,LOG)):-!,
                save_each_clause_in_buffer(KnowledgeBase,Context,PRO),
                save_each_clause_in_buffer(KnowledgeBase,Context,LOG).

save_each_clause_in_buffer(KnowledgeBase,Context,PROLOG):-
                not(not((numbervars(((KnowledgeBase,Context,PROLOG)),'$VAR',0,_),assert_if_new(storage_buffer(KnowledgeBase,Context,PROLOG))))).

assert_if_new(X):-X,!.
assert_if_new(X):-!,assert(X).

% ===================================================================
%  RESETING WORKING MEMORY
% ===================================================================

clear_moo_memory:-
                  sendNote(extreme_debug,constentManger,'Rebuilding working memory',' '),
                  clear_tq_db,!.

clear_tq_db:-
        mooCache(PredR,Form,USurface,UVars,KB,Ctx,TN,Maintainer,Result),
        TN>20000,retractall(mooCache(PredR,_,_,_,_,_,TN,_,_)),fail.
clear_tq_db:-
        mooCache(PredR,Form,_,_,KB,Ctx,surf(_,TN)),
        TN>20000,retractall(mooCache(PredR,Form,_,_,KB,Ctx,surf(_,TN))),fail.
clear_tq_db:-
        mooCache(PredR,Form,_,_,_,KB,Ctx,(surf(_,TN)*_)),
        TN>20000,retractall(mooCache(PredR,Form,_,_,_,KB,Ctx,(surf(_,TN)*_))),fail.

clear_tq_db:-!.


/*
clear_tq_db:-
                        clear_tq_db(['K','L','M','N','O','P','Q','R','S']),
                  retractall(mooCache(PredR,_,_,_,_,_,_)),
                  retractall('in-active-memory'(_,_)),
                  clear_tq_db('instance',12),
                  clear_tq_db('subclass',12),
                  clear_tq_db('subrelation',12),
                  clear_tq_db('range',12),
                  clear_tq_db('domain',12),
                  clear_tq_db('domainSubclass',12),
                  clear_tq_db('RefexivePredicateTo',12),
                  clear_tq_db('equal',12).
*/

clear_tq_db([]).
clear_tq_db([A|As]):-clear_tq_db(A,22),!,clear_tq_db(As).
clear_tq_db(F,N):-atom_concat('int_',F,IRF),clear_tq_db1(IRF,N),!,atom_concat('int_~',F,NIRF),clear_tq_db1(NIRF,N),!,atom_concat('~',F,NRF),clear_tq_db1(NRF,N),!,clear_tq_db1(F,N).

clear_tq_db1(_F,0):-!.
clear_tq_db1(F,A):-functor(Term,F,A),catch(retractall(Term),_,true),AA is A -1,clear_tq_db1(F,AA).


% ===================================================================
%  SETTING WORKING MEMORY
% ===================================================================

establish_startup_state:-
         catch(
         (
         sendNote(debug,contentManager,'Moo KB startup states being loaded',' '),
         %adoConnect,
         %sync_ado_cache,
         clear_moo_memory
         ),E,sendNote(debug,contentManager,'Moo KB startup states insufficient for full support',E));sendNote(debug,contentManager,'Moo KB startup states insufficient for full support',E).

:-dynamic('in-active-memory'/2).


set_default_kb(X):-retractall(getMooOption(opt_kb,_)),assert(getMooOption(opt_kb,X)).
set_default_assertion_context(X):-retractall(get_default_assertion_context(_)),assert(get_default_assertion_context(X)).
set_default_request_context(X):-retractall(get_default_request_context(_)),assert(get_default_request_context(X)).


get_default_request_context('GlobalContext').
get_default_assertion_context('GlobalContext').

get_axioms_path(Path):-get_storage_file("moo_builtins.kif",Path).


%               actx_invoke_object(IPRset,'MoveNext',[],_).


%=================================================================
%  CONTEXT SYNC  PROCESSING
%=================================================================
/*

Defining KBs

The Moo LE must record forall information required to make fully persistent the side-effects specified for these calls. Refer to the description under "startup_status/1," above, for details of the requirements for persistence.

These calls are used to create, delete and manipulate the KB definitions known within a given Moo LE:

- define_kb(KB_Designator)
  Define a new KB based on the specified KB_Designator, the kb_name must not be the name of a KB already known to the receiving LE.

- rename_kb(KB_Designator)
  Change the kb_name associated with the kb_source in the KB_Designator, which must be known to the receiving LE. [ TBD: Multiple KBs using equal source file? ]

- reconstitute_kb(KB_Designator)
  Change the kb_source associated with the kb_name in the KB_Designator, which must be known to the receiving LE.

- delete_kb(KB_Designator)
  Remove and forget everything about the specified KB (i.e., perform the "FG" action and then forget any internal and / or persistent descriptor for the specified KB). [ TBD: "If the specified KB is in the startup list, should it be removed from there as well" ?? Probably "yes." ]


KB_Designator
Where I use the term "KB_Designator" I mean a Prolog term with one of the following structures:

        kb(kb_name, kb_source)
        kb(kb_name, KB_SOURCE)
        kb(KB_NAME, kb_source)
        kb(kb_name)

        Other forms may be added as needed.


Examples:
        kb('PrologMOO', file('/MooL/SUO/PrologMOO.can')
        kb('PrologMOO', url('http://jordan/skb/exported/merge.can')

        % Acceptable If the location is known:
        kb('PrologMOO', _)

        % Also acceptable if the location is known:
        kb('PrologMOO')

        Future forms may include:

        kb('PrologMOO', ftp('jordan', 'anonymous', 'rs@cris.com', '/kbs/PrologMOO.can'))
        kb('PrologMOO', https('jordan', 8080, 'mysession', '/kbs/PrologMOO.can'))


kb_source is one of:

        file(local_file_name)
        url(file_url)
        url(http_url)

        (Other forms may be added as needed.)

Note: file_url and http_url are TBD and their implementation is yet to be scheduled.


Startup

When a Moo LE starts up it first establishes or initializes is basic functions. Once that initialization completes successfully, it must next re-establish the client-specified initial state.

Current state initialization centers on KB pre-loading and is described under "startup_status/1," above.

Since startup actions occur apart from any end-user action and thus have no "client" to which to return feedback, status or other diagnostics, the Moo LE must write to its log any noteworthy or erroneous events that occur while it performs its startup actions.


*/
% Defining and renaming KBs

wrap_le(Goal):-
      write_response_begin,
      ignore(Goal),
      write_response_end.

define_kb(Var):-var(Var),!,
      verify_status(X),!,
      member((Var=_),X).

define_kb(kb(Name,Location)):-
         wrap_le(kb_startup_start(kb(Name,Location)=void)).

define_kb(_):-
      write_response_begin,
      ignore(ua_out(disp_error,'syntax error'('writeFmt for define_kb/1 is define_kb(kb(''PrologMOOAddition'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')).'),_)),
      write_response_end.

rename_kb(Var):-var(Var),!,
      write_response_begin,
      ignore(ua_out(disp_error,'syntax error'('writeFmt for rename_kb/1 is rename_kb(kb(''NewName'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')).'),_)),
      write_response_end.

rename_kb(kb(Name,Location)):-
         wrap_le((
                  sendNote(debug,logicEngine,'Debug Info',[rename_kb,kb(Name,Location)]),
                  retractall(kb_loaded(N)),
                  define_kb_proc(kb(Name,Location))
           )).

rename_kb(_):-
      write_response_begin,
      ignore(ua_out(disp_error,'syntax error'('writeFmt for rename_kb/1 is rename_kb(kb(''NewName'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')).'),_)),
      write_response_end.

reconstitute_kb(Var):-var(Var),!,
      write_response_begin,
      ignore(ua_out(disp_error,'syntax error'('writeFmt for reconstitute_kb/1 is reconstitute_kb(kb(''PrologMOOAddition'', ''/cygdrive/c/Moo/the/new/path_to.can'')).'),_)),
      write_response_end.

reconstitute_kb(kb(Name,Location)):-
         wrap_le((
               sendNote(debug,logicEngine,'Debug Info',[reconstitute_kb,kb(Name,Location)]),
               retractall(kb_loaded(Name)),
               define_kb_proc(kb(Name,Location))
               )).

reconstitute_kb(_):-
      write_response_begin,
      ignore(ua_out(disp_error,'syntax error'('writeFmt for reconstitute_kb/1 is reconstitute_kb(kb(''PrologMOOAddition'', ''/cygdrive/c/Moo/the/new/path_to.can'')).'),_)),
      write_response_end.

delete_kb(Var):-var(Var),!,
      write_response_begin,
      ignore(ua_out(disp_error,'syntax error'('writeFmt for delete_kb/1 is delete_kb(kb(''PrologMOOAddition'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')).'),_)),
      write_response_end.

delete_kb(kb(Name,Location)) :-kb_startup_status(kb(Name,Location)=unknown).

delete_kb(_):-
      write_response_begin,
      ignore(ua_out(disp_error,'syntax error'('writeFmt for delete_kb/1 is delete_kb(kb(''PrologMOOAddition'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')).'),_)),
      write_response_end.

delete_kb_proc(kb(Name,Location)) :-
         sendNote(debug,contentManager,'kb deletions',[deleting_kb,kb(Name,Location)]),
         retractall(kb_make_status_start(kb(Name,_)=_)),
         save_kb_statuses.


startup_status(KBS_List):-
      write_response_begin,
      ignore(ua_out(disp_error,'predicate not found'('server_startup_status(KB_List) will is establish startup statuses and    kb_status( kb(Name,_)=Status ) will change a single KB status.'),_)),
      write_response_end.



% ===================================================================================================
% Status API
% These calls allow clients to ascertain KB load and currency status and to effect the loading and unloading of KBs within an LE.
% ===================================================================================================

% ===================================================================================================
%  verify_status(KBS_List)  Succeeds if every KB_Status in KBS_List unifies with the current status for the WFS whose name equal the kb_name in that KB_Status.
% ===================================================================================================
verify_status(KBS_List):-var(KBS_List),!,   write('<LE></LE>'),whole_status_set(KBS_List).

verify_status([kb(KB, Path) = KB_STATUS]):-
            var(KB),var(Path),var(KB_STATUS),          !,
            write('<LE></LE>'),whole_status_set(KBS_List),
            member((kb(KB, Path) = KB_STATUS),KBS_List).


whole_status_set(KBS_List):-findall( KB=Status,kb_status(KB,Status),KBS_List),!.

verify_status(KBS_List):- write('<LE></LE>'),
                     whole_status_set(KBS_ListO),
                     member(X,KBS_List),member(X,KBS_ListO).

verify_status_proc(KBS_List):-nonvar(KBS_List),

                                                setof( (kb(Name,Location)=Status),
                                                                  ((
                                                                   (member(kb(Name,Location)=_,KBS_List);member(kb(Name,Location),KBS_List)),
                                                                   kb_status(kb(Name,Location),Status)
                                                                    )),
                                                   KBS_List).

% ===================================================================================================
% verify_status(KBS_List, Status_Return)  Performs the actions of verify_status/1 and bind Status_Return to a list of actual status designators corresponding to the KB_Designators in the request list.
%  A KB_Status bearing a kb_name that is not known to the responding Moo LE will equal only with the status code "unknown" (or a variable).
% ===================================================================================================

verify_status(KBS_List,OUT):-nonvar(KBS_List),write('<LE>'),
                                                ignore(setof( (kb(Name,Location)=Status),
                                                                  ((
                                                                   (member(kb(Name,Location)=_,KBS_List);member(kb(Name,Location),KBS_List)),
                                                                   kb_status(kb(Name,Location),Status)
                                                                    ))),
                                                   OUT),write('</LE>').

% ===================================================================================================
% establish_status(KBS_List)  Succeed if for every KB_Status in KBS_List, the KB with the specified kb_name either already has the specified status or can be made to have that state by successfully carrying out the state transition action given in the table above.
% ===================================================================================================

establish_status(KBS_List):-!,
                  wrap_le(establish_status_0(KBS_List)).

establish_status_0([]):-!.
establish_status_0([First|REST]):-!,
      establish_status_1(First),!,
      establish_status_0(REST),!.
establish_status_0(kb(Name,Location)=RequestStatus):- !,
            establish_status_1(kb(Name,Location)=RequestStatus),!.

establish_status_1(kb(Name,Location)=S):-nonvar(S),S=unknown,!,
                  delete_kb_proc(kb(Name,Location)).

establish_status_1(kb(Name,Location)=RequestStatus):-
                  verify_status_proc([kb(Name,Location)=CurrentStatus]),!,
                  sendNote(debug,contentManager,'kb status establish',[establish,RequestStatus,from,CurrentStatus,kb(Name,Location)]),
                  ignore(make_status(kb(Name,Location),CurrentStatus,RequestStatus)),!.

% ===================================================================================================
% elevate_status(KBS_List)  Succeed if for every KB_Status in KBS_List, the KB with the specified kb_name either already has the specified status or can be elevated to that state by successfully carrying out the state transition action given in the table above never reducing.
% ===================================================================================================

elevate_status(KBS_List):-!,
                  write('<LE>'),
                  ignore(elevate_status_0(KBS_List)),!,
                 write('</LE>'),!.

elevate_status_0([]):-!.
elevate_status_0([First|REST]):-!,
      elevate_status_1(First),!,
      elevate_status_0(REST),!.
elevate_status_0(kb(Name,Location)=RequestStatus):- !,
            elevate_status_1(kb(Name,Location)=RequestStatus),!.

elevate_status_1(kb(Name,Location)=RequestStatus):-
                  verify_status_proc([kb(Name,Location)=CurrentStatus]),!,
                  sendNote(debug,contentManager,'kb status elevate',[elevate,RequestStatus,from,CurrentStatus,kb(Name,Location)]),
                  elev_status(kb(Name,Location),CurrentStatus,RequestStatus),!.

/*

establish_status([kb('PrologMOO',_) = current]).

- establish_status(KBS_List, Status_Return)
  Perform the actions of establish_status/1 and bind Status_Return to a list of actual status designators corresponding to the KB_Designators in the request list.

Status Transition

The following tables indicates the actions to take to effect a transition from the status in the left-hand column to the status shown in the column headers along the top. The "file" is whatever external forms the Moo LE happens to be using currently (i.e., it's an unspecified external form meant to be used to facilitate KB loading by comparison to recompilation).

Current Desired Status:
Status          void    avail   old     stale   vol     current
_____           _____   _____   _____   _____   _____   _____
void             | --    CF      XX      XX      LF      CC
avail            | DS    --      XX      XX      DF      LF
old              | DS    SF      --      CF ?    CM      CC
stale           | DS    UL      DF      --      CM+DF   RF
volatile        | UL    SF+UL CM    SF+UL   --      SF
current        | FG    UL      XX      XX      DF      --

Actions:
        CF      Compile to File
        CM      Compile to Memory
        CC      Compile to Current (file and memory)
        LF      Load File into Memory
        RF      Reload File
        SF      Save Memory to File
        UL      Unload from Memory
        DF      Remove File
        DS      Dispose(Unload from Memory and Remove File)
        XX      Reject / fail
        --      Do Nothing (succeed w/o side-effects)


*/


:-dynamic(moo_file_loaded/1).

kb_status(kb(Name,Location),Status):- kb_make_status_start(kb(Name,Location)=_), lookup_kb_status(kb(Name,Location),Status).

lookup_kb_status(X,Y):- lookup_kb_status_array(X,Y),!.


lookup_kb_status_array(kb(Name,Location),void):- not((moo_file_loaded(p(Name)))),not(kb_p_file_current(Name)).
lookup_kb_status_array(kb(Name,Location),available):- not(moo_file_loaded(p(Name))),kb_p_file_current(Name).
lookup_kb_status_array(kb(Name,Location),old):- (moo_file_loaded(p(Name))),kb_modified(Name),not(kb_p_file_current(Name)).
lookup_kb_status_array(kb(Name,Location),stale):- (moo_file_loaded(p(Name))),kb_modified(Name),(kb_p_file_current(Name)). %should be stale
lookup_kb_status_array(kb(Name,Location),volatile):- (moo_file_loaded(p(Name))),not(kb_modified(Name)),not(kb_p_file_current(Name)).
lookup_kb_status_array(kb(Name,Location),current):- (moo_file_loaded(p(Name))),not(kb_modified(Name)),(kb_p_file_current(Name)).
lookup_kb_status_array(kb(Name,Location),unknown):-!,nonvar(Name),nonvar(Location).

:-dynamic(kb_loaded/1).
:-dynamic(kb_is_modified/1).

kb_modified(Name):-kb_is_modified(Name).

kb_p_file_current(Name):-safe_kb_info_db(Name,Location,_WFS,PFILE),file_newer(PFILE,Location).


/*
Status Codes For Moo LE-hosted WFSs

Where I use the term "StatusCode" I mean one of the atoms in the right-most column of this table:

Loaded  Memory    File *                 State
           Current    Current                Name

0               0               0               void
0               0               1               available
0               1               0               n/a
0               1               1               n/a
1               0               0               old
1               0               1               stale
1               1               0               volatile
1               1               1               current
-               -               -               unknown

* A missing file is deemed not current

*/

% kb_startup_status(kb('PrologMOO', '/cygdrive/c/Moo/SUO/PrologMOO.can')=available)

make_status(kb(Name,Location),_,unknown):-!,
                           sendNote(debug,contentManager,Name,[making,unknown,from,Location,'(=> deletion)']),
                           delete_kb_proc(kb(Name,Location)),!.

make_status(kb(Name,Location),_,available):-!,
                           sendNote(debug,contentManager,Name,[making,available,from,Location,'(compiled but not loaded)']),
                           source_to_p_file(Name),!.

make_status(kb(Name,Location),_,current):-!,
                           sendNote(debug,contentManager,Name,[making,current,from,Location]),
                           load_kb_file(Name),!.

elev_status(kb(Name,Location),_,available):-!,
                           sendNote(debug,contentManager,Name,[elevating,to,available,from,Location]),
                           source_to_p_file(Name),!.
elev_status(kb(Name,Location),_,current):-!,
                           sendNote(debug,contentManager,Name,[elevating,to,current,from,Location]),
                           load_kb_file(Name),!.

elev_status(kb(Name,Location),_,_):-!,
                           sendNote(debug,contentManager,Name,[elevating,has,no,effect]).

/*
  kb_startup_status(KBS_List)
  The Moo LE that executes this call should record the specified KB status list as the status that the LE should attempt to establish upon startup. The specified KBS_List must be fully ground and the status indicators should be permissible as target status codes. A KB_Designator with an unknown kb_name is only acceptable if its status indicator is "unknown." [ Is there any point in that special case? ]
  The information recorded to implement this call must be stored in a manner that is persistent in the face of forall known shutdown and restart mechanisms, whether controlled or uncontrolled (Prolog crash, system crash, power failure) and whether abortive (e.g. SIGTERM or SIGKILL) or cooperative (shutdown request).
  The means by which Moo LE achieves this persistence is unspecified (fully at its discretion).
  Succeeds if and only if the startup status designators are successfully recorded.

*/
kb_startup_status(Var):- var(Var),
            findall(KB,kb_make_status_start(KB),List),!,
            member(Var,List).

kb_startup_status(KBDesignator=Status):-var(Status),!,kb_make_status_start(KBDesignator=Status).
kb_startup_status(KBDesignator=Status):-
      write_response_begin,
               kb_startup_status_0(KBDesignator=Status),
               save_kb_statuses,
      write_response_end.

kb_startup_status(_):-
       wrap_le(ua_out(disp_error,'syntax error'('writeFmt for kb_startup_status/1 is kb_startup_status(kb(''PrologMOOAddition'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')=current).'),_)).

kb_startup_status_0(KBDesignator=unknown):-nonvar(KBDesignator),!,delete_kb_proc(KBDesignator).
kb_startup_status_0(KBDesignator=Status):-nonvar(Status),nonvar(KBDesignator),!,
               sendNote(debug,logicEngine,'Debug Info','Content Management'(set_kb_startup_status(KBDesignator=Status))),
               nonvar(Status),
               retractall(kb_make_status_start(KBDesignator=_)),
               assert(kb_make_status_start(KBDesignator=Status)),
               ignore(elev_status(KBDesignator,_,Status)).



% ===================================================================================================
%  verify_startup_status(KBS_List)  Succeeds if every KB_Status in KBS_List unifies with the current status for the WFS whose name equal the kb_name in that KB_Status.
% ===================================================================================================

verify_startup_status(Var):-var(Var),!,whole_startup_status_set(KBS_List).

verify_startup_status([kb(KB, Path) = KB_STATUS]):-
            var(KB),var(Path),var(KB_STATUS),          !,
            write('<LE></LE>'),whole_startup_status_set(KBS_List),
            member((kb(KB, Path) = KB_STATUS),KBS_List).

verify_startup_status(KBS_List):-write('<LE></LE>'),nonvar(KBS_List),

                                                setof( (kb(Name,Location)=Status),
                                                                  ((
                                                                   (member(kb(Name,Location)=_,KBS_List);member(kb(Name,Location),KBS_List)),
                                                                   kb_make_status_start(kb(Name,Location)=Status)
                                                                    )),
                                                   KBS_List).



% ===================================================================================================
%  server_startup_status(KBS_List)  Succeeds if every KB_Status in KBS_List unifies with the current status for the WFS whose name equal the kb_name in that KB_Status.
%  The Moo LE that executes this call should record the specified KB status list as the status that the LE should attempt to establish upon startup. The specified KBS_List must be fully ground and the status indicators should be permissible as target status codes. A KB_Designator with an unknown kb_name is only acceptable if its status indicator is "unknown." [ Is there any point in that special case? ]
%  The information recorded to implement this call must be stored in a manner that is persistent in the face of forall known shutdown and restart mechanisms, whether controlled or uncontrolled (Prolog crash, system crash, power failure) and whether abortive (e.g. SIGTERM or SIGKILL) or cooperative (shutdown request).
%  The means by which Moo LE achieves this persistence is unspecified (fully at its discretion).
%  Succeeds if and only if the startup status designators are successfully recorded.
% ===================================================================================================
server_startup_status(KBS_List):-var(KBS_List),!,whole_startup_status_set(KBS_List).

server_startup_status([kb(KB, Path) = KB_STATUS]):-
            var(KB),var(Path),var(KB_STATUS),          !,
            write('<LE></LE>'),whole_startup_status_set(KBS_List),
            member((kb(KB, Path) = KB_STATUS),KBS_List).

whole_startup_status_set(KBS_List):-findall(KB,kb_make_status_start(KB),KBS_List).

server_startup_status(LIST):-
      findall((kb(X,Y)=Status),member((kb(X,Y)=Status),LIST),Look),
      Look=LIST,!,
      write_response_begin,
         server_startup_status_0(LIST),
         save_kb_statuses,
      write_response_end.

server_startup_status(_):-!,
      write_response_begin,
      ignore(ua_out(disp_error,'syntax error'('writeFmt for server_startup_status/1 is server_startup_status([kb(''PrologMOOAddition'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')=current,kb(''PrologMOO'', ''/cygdrive/c/Moo/SUO/PrologMOO.can'')=unknown]).'),_)),
      write_response_end.


server_startup_status_0([]):-!.
server_startup_status_0([Head|Tail]):-!,
         kb_startup_status_0(Head),
         server_startup_status_0(Tail).

establish_each([]):-!.
establish_each([KBDesignator=Status|REST]):-!,once(make_status(KBDesignator,_,Status)),
              establish_each(REST).



invokeKbLoading(KB):-isKbLoading(KB,Status),!.
invokeKbLoading(KB):-
        mooProcessCreate(loadKBfromSource(KB,GlobalContext),loadKBfromSource(KB,GlobalContext),ID,[detatched(false)]),
        assert(isMooProcess(ID,loadKBfromSource(KB,GlobalContext))).

loadKBfromSource(KB,Ctx):-
                getFilenameOfKBSource(KB,Filename),!,
                once(invokeKifFileChecking(KB,Filename,Ctx,User)).

getFilenameOfKBSource(KB,Filename):-
                fmtString(FileChars,'../moo-rt/work/~w.can',[KB]),!,
                string_to_atom(FileChars,Filename),!.



% ===========================================================
% Test Knowledge Base File (Currently only Test until browser integration)
% ===========================================================
loadKnowledgebaseSourcefile(Name,Filename):-  make,
                catch(atom_codes(Name,Codes),_,Codes=[]),
                length(Codes,L),L<3,
                file_base_name(Filename,BaseName),
                file_name_extension(KB,Extension,BaseName),
                writeFmt('<B color=red>No name was given, so a Knowledge Base called <font color=green>~w</font> is being created.<p>',[KB]),
                load_kif_to_kb_ctx(KB,Filename,'GlobalContext','MooWeb').

loadKnowledgebaseSourcefile(KB,Filename):-!,
                (unsetMooOption(opt_surface_check=_)),
                (setMooOption(opt_surface_check=trusted)),
                idGen(TN1),
                idGen(TN2),
                idGen(TN3),
                retractall(mooCache(PredR,_,_,KB,_,_,_,_)),
                assertaClean(mooCache(PredR,surface,'instance'(KB,'KnowledgeBase'),'$VAR'(0),'MooKernel','GlobalContext',TN1,'WebUser',gaf)),
                assertaClean(mooCache(PredR,surface,'instance'('GlobalContext','Context'),'$VAR'(0),KB,'GlobalContext',TN2,'WebUser',gaf)),
                assertaClean(mooCache(PredR,surface,'sourcefile-of'(KB,Filename),'$VAR'(0),'MooKernel','GlobalContext',TN3,'WebUser',gaf)),
                invokeKifFileChecking(KB,Filename,'GlobalContext','MooWeb').



invokeKifFileChecking(KB,Filename,Ctx,User):-!, make,tell(user_error),
        ignore(User='MooWeb'),
         get_default_assertion_context(DCtx), ignore((Ctx=DCtx)),
         ensureMooOption(opt_kb,_,KB),
         retractall(mooCache(PredR,_,_,_,KB,Ctx,_,_,_)),!,
         saveMooCache,
         writeFmt(user_error,'Reading In ~w to ~w with a default context of ~w \n',[Filename,KB,Ctx]),
         flag('Axioms Compiled',_,0),
         safe_file_open(Filename,'r',INPUT),!,
         writeFmt(user_error,'~q\n',[safe_file_open(Filename,'r',INPUT)]),
         repeat,
                  compile_each_line(INPUT),
        at_end_of_stream(INPUT),
        close(INPUT),
        flag('Axioms Compiled',AX,AX),
         writeFmt('\n% Compiled ~w axioms.\n',[AX]),
         %(test_syntax_save_kb_ctx(KB,Ctx,Filename)),
         ignore(retract(findings(CPU,RESULT))) ,
         ignore(findall(T,retract(title(T)),Title)).

compile_each_line(Stream):-
                sleep(1),
                once(readKIF_priv(Stream,RRAW)),
                once(getCleanCharsWhitespaceProper(RRAW,Trimed)),
                once(ltrim(Trimed,L_trimmed)),
                        catch(writeFmt(user_error,'"~s" \n',[L_trimmed]),_,true).

/*
                        source_from_stream(INPUT,Trimed,SOURCEFORM,Vars),nonvar(Trimed),
                        catch(writeFmt(user_error,'~s \n',[Trimed]),_,true),
                        rememberAxioms(KB,Ctx,SOURCEFORM,Vars,User),!.

*/






%tkb:-loadKBfromSource('PrologMOO','GlobalContext').
/*
skipKIFChar(Stream):-stream_property(Stream,position('$stream_position'(CharIndex, LineNo, LinePos)),
        NewI is CharIndex+1,
*/
/*
(pnf (documentation instance "An object is an &%instance a &%Class if

it is a member of that &%Class.  An individual may be an instance of many

classes, some of which may be subclasses of others.  Thus, there is no

assumption in the meaning of &%instance about specificity or uniqueness.")
 GlobalContext T-4)
(pnf (instance subclass PartialOrderingRelation)
 GlobalContext T-5)

*/




rememberAxioms(KB,Ctx,file_comment(_),Vars,User):-!.
rememberAxioms(KB,Ctx,surf,Vars,User):-!.
rememberAxioms(KB,Ctx,SOURCEFORM,Vars,User):-!,
                global_increment('Axioms Compiled'),
                flag('Axioms Compiled',AssertionID,AssertionID),
                idGen(INTID),
                assert(mooCache(PredR,surface,SOURCEFORM,Vars,KB,Ctx,AssertionID,User,uncanonicalized)).

test_syntax_save_kb_ctx(KnowledgeBase,Context,Filename):-
         safe_file_open(Filename,'w',OUTPUT),
         test_syntax_write_kb_ctx(KnowledgeBase,Context,OUTPUT),
         close(OUTPUT).

test_syntax_save_kb_ctx:-test_syntax_save_kb_ctx('PrologMOO','GlobalContext','C:/mool/SUO/MFixed.txt').


test_syntax_write_kb_ctx(KnowledgeBase,Context,OUTPUT):-
                mooCache(PredR,surface,Source,Vars,KnowledgeBase,Context,AssertionID,Creator,Status),
                once(catch((toMarkUp(kif,Source,Vars,OutChars),writeFmt(OUTPUT,'~w\n',[OutChars])),_,true)),
                fail.

test_syntax_write_kb_ctx(KnowledgeBase,Context,OUTPUT):-!,saveMooCache.



