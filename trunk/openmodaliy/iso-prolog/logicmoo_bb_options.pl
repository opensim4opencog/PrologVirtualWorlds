:-include('logicmoo_utils_header.pl').
/*
:-module(moo_globalisms,[
	 setMooOption/1,
	 setMooOption/2,
	 getMooOption/1,
	 getMooOption/2,
	 ensureMooOption/2,
	 ensureMooOption/3,
	 setMooOptionDefaults/0]).
*/
	       
% :-ensure_loaded(moo_threads).

% Agent Agent
:-dynamic('$MooOption'/4).
:-dynamic(act_mem/3).

% Database
:-dynamic(tq_attempted_request/0).
:-dynamic(title/1).

% TQ System
:-dynamic(tq_missed_one_answer/0).
:-dynamic(tq_least_one_answer/0).
:-dynamic(t_answer_found/1).


% ===================================================================
% OPERATION PREDICATES
% ===================================================================
% Defaults
:-dynamic(getDefaultKB/1).
:-dynamic(get_default_request_context/1).
:-dynamic(get_default_assertion_context/1).
:-dynamic(version_tag/1).

:-dynamic(answer_found/1).
:-dynamic(moo_K_scenario/2).
:-dynamic(telling_prolog/0).  % If set asserts clauses into prolog database
:-dynamic(telling_file/0).   % If set write assertions to file
:-dynamic(disp_debug/5).         %PREDICATE RESOLUTON
:-dynamic(contexts/2).            %CONTEXT STATES
:-dynamic(setting_table/2).
:-dynamic(tabling/1).
:-dynamic(tabled_t/1).
:-dynamic(tabled_f/1).
:-dynamic(answer_yes/0).
:-dynamic(already_asked/2).
:-dynamic(save_all/2).
:-dynamic(moo_K_scenario/6).         %We keep a cache of forall consultations
:-dynamic(consultation_mode_on/0).
:-dynamic(resource_cache/2).
:-dynamic(debuggerize/0).

:-dynamic( le_ele/1).


:-dynamic('surface-word'/2).
:-dynamic('surface-macro'/2).
:-dynamic('browser-only'/1).
:-dynamic('not-implemented'/1).
:-dynamic('surface-atom'/1).
:-dynamic('surface-single-arity'/1).
:-dynamic('surface-multiple-arity'/1).
:-dynamic('surface-instance'/2).
:-dynamic('surface-subclass'/2).
:-dynamic('surface-class'/1).
:-dynamic('surface-quantifier'/1).



indexPredicate(positive_fact_cache(1,1,1,1)).
indexPredicate(positive_rule_cache(1,1,1,1)).
indexPredicate(negative_fact_cache(1,1,1,1)).
indexPredicate(negative_rule_cache(1,1,1,1)).

:-dynamic(have_arity/4).

% these hold prototypes of calls
:-dynamic(positive_fact_cache/4).
:-dynamic(positive_rule_cache/4).
:-dynamic(negative_fact_cache/4).
:-dynamic(negative_rule_cache/4).

:-dynamic(make_positive_cache/2).


:-dynamic('in-active-memory'/2).

:-dynamic('should_be_loaded'/2).



setMooOption(Agent,[]):-!.
setMooOption(Agent,[H|T]):-!,
      setMooOption(Agent,H),!,
      setMooOption(Agent,T),!.
setMooOption(Agent,Var=_):-var(Var),!.
setMooOption(Agent,_=Var):-var(Var),!.
setMooOption(Agent,(N=V)):-nonvar(N),!,setMooOption_thread(Agent,N,V),!.
setMooOption(Agent,N):-atomic(N),!,setMooOption_thread(Agent,N,true).
	
setMooOption(Agent,Name,Value):-setMooOption_thread(Agent,Name,Value).
setMooOption_thread(Agent,Name,Value):-
	((thread_self(Process),
	retractall('$MooOption'(Agent,Process,Name,_)),
	asserta('$MooOption'(Agent,Process,Name,Value)),!)).


unsetMooOption(Agent,Name=Value):-nonvar(Name),
	unsetMooOption_thread(Agent,Name,Value).
unsetMooOption(Agent,Name):-nonvar(Name),
	unsetMooOption_thread(Agent,Name,_).
unsetMooOption(Agent,Name):-(retractall('$MooOption'(Agent,_Process,Name,_Value))).


unsetMooOption_thread(Agent,Name):-
	unsetMooOption_thread(Agent,Name,_Value).

unsetMooOption_thread(Agent,Name,Value):-
	thread_self(Process),
	retractall('$MooOption'(Agent,Process,Name,Value)).
	
getMooOption_nearest_thread(Name,Value):-
	getMooOption_thread(Agent,Name,Value),!.
getMooOption_nearest_thread(Name,Value):-
	'$MooOption'(Agent,_,Name,Value),!.
getMooOption_nearest_thread(_Name,_Value):-!.



isMooOption(Agent,Name=Value):-!,isMooOption(Agent,Name,Value).
isMooOption(Agent,Name):-!,isMooOption(Agent,Name,true).

isMooOption(Agent,Name,Value):-getMooOption_thread(Agent,Name,Value).

getMooOption_thread(Agent,Name,Value):-
	((thread_self(Process),
	'$MooOption'(Agent,Process,Name,Value))),!.


getMooOption(Agent,Name=Value):-nonvar(Name),!,ensureMooOption(Agent,Name,_,Value).
getMooOption(Agent,Name=Default,Value):-nonvar(Name),!,ensureMooOption(Agent,Name,Default,Value).
getMooOption(Agent,Name,Value):-nonvar(Name),!,ensureMooOption(Agent,Name,_,Value).


ensureMooOption(Agent,Name=Default,Value):-
	ensureMooOption(Agent,Name,Default,Value),!.
	
ensureMooOption(Agent,Name,_Default,Value):-
	getMooOption_thread(Agent,Name,Value),!.

ensureMooOption(Agent,Name,Default,Default):-
	setMooOption_thread(Agent,Name,Default),!.

ensureMooOption(Agent,Name,_Default,Value):-nonvar(Name),!,   
	setMooOption_thread(Agent,Name,Value),!.

ensureMooOption(Agent,_Name,Default,Default).



setMooOptionDefaults:-
             (unsetMooOption(Agent,_)),
             setMooOption(Agent,opt_callback='sendNote'),
             setMooOption(Agent,cb_consultation='off'),
             setMooOption(Agent,opt_debug='off'),
             setMooOption(Agent,cb_error='off'),
             setMooOption(Agent,cb_result_each='off'),

% Agent Agent Defaults for Blank Variables
             setMooOption(Agent,opt_cxt_request='GlobalContext'),
             setMooOption(Agent,opt_ctx_assert='GlobalContext'),
             setMooOption(Agent,opt_tracking_number='generate'),
             setMooOption(Agent,opt_agent='ua_parse'),
             setMooOption(Agent,opt_precompiled='off'),
             getMooOption(Agent,opt_theory,Context),setMooOption(Agent,opt_theory=Context),
             setMooOption(Agent,opt_notation='kif'),
             setMooOption(Agent,opt_timeout=2),
             setMooOption(Agent,opt_readonly='off'),
             setMooOption(Agent,opt_debug='off'),
             setMooOption(Agent,opt_compiler='Byrd'),
             setMooOption(Agent,opt_language = 'pnx_nf'),

%Request Limits
             setMooOption(Agent,opt_answers_min=1),
             setMooOption(Agent,opt_answers_max=999), %TODO Default
             setMooOption(Agent,opt_backchains_max=5),
             setMooOption(Agent,opt_deductions_max=100),
             setMooOption(Agent,opt_backchains_max_neg=5),
             setMooOption(Agent,opt_deductions_max_neg=20),
             setMooOption(Agent,opt_forwardchains_max=1000),
             setMooOption(Agent,opt_max_breath=1000), %TODO Default

%Request Contexts
             setMooOption(Agent,opt_explore_related_contexts='off'),
             setMooOption(Agent,opt_save_justifications='off'),
             setMooOption(Agent,opt_deductions_assert='on'),
             setMooOption(Agent,opt_truth_maintence='on'),
             setMooOption(Agent,opt_forward_assertions='on'),
             setMooOption(Agent,opt_deduce_domains='on'),
             setMooOption(Agent,opt_notice_not_say=off),


%Request Pobibility
             setMooOption(Agent,opt_certainty_max=1),
             setMooOption(Agent,opt_certainty_min=1),
             setMooOption(Agent,opt_certainty=1),
             setMooOption(Agent,opt_resource_commit='on').


%:-module(moo_database,[]).

:-include('logicmoo_utils_header.pl').

% =======================================================================================
% ACTIVATE CONTEXT DAG
% =======================================================================================

% on a dirty ctx/theory run remake_positive_cache(Context,Ctx)
% on a dirty ctx/theory run remake_negative_cache(Context,Ctx)




negative_fact_cache(Context,+X):-not(positive_fact_cache(Context,_,X)).
negative_fact_cache(Context,-X):-not(positive_fact_cache(Context,_,X)).

negative_rule_cache(Context,+X):-not(positive_rule_cache(Context,_,X)).
negative_rule_cache(Context,-X):-not(positive_rule_cache(Context,_,X)).

ambiguous_fact_cache(Context,+X):- (positive_fact_cache(Context,?,X)).
ambiguous_fact_cache(Context,-X):- (positive_fact_cache(Context,?,X)).

ambiguous_rule_cache(Context,+X):- (positive_rule_cache(Context,?,X)).
ambiguous_rule_cache(Context,-X):- (positive_rule_cache(Context,?,X)).


holds_wrapper(Context,R,X,Y):-nonvar(R),!,
        P=..[R,X,Y],
        all_predicate_cache(Context,+P).

holds_wrapper(Context,R,X,Y,Z):-nonvar(R),!,
        P=..[R,X,Y,Z],
        all_predicate_cache(Context,+P).

holds_wrapper(Context,R,X,Y,Z,Q):-nonvar(R),!,
        P=..[R,X,Y,Z,Q],
        all_predicate_cache(Context,+P).

holds_wrapper(Context,R,X,Y):-
        all_predicate_cache(Context,+P),
        P=..[R,X,Y].

holds_wrapper(Context,R,X,Y,Z):-
        all_predicate_cache(Context,+P),
        P=..[R,X,Y,Z].

holds_wrapper(Context,R,X,Y,Z,Q):-
        all_predicate_cache(Context,+P),
        P=..[R,X,Y,Z,Q].

not_holds_wrapper(Context,R,X,Y):-nonvar(R),!,
        P=..[R,X,Y],
        all_predicate_cache(Context,-P).

not_holds_wrapper(Context,R,X,Y,Z):-nonvar(R),!,
        P=..[R,X,Y,Z],
        all_predicate_cache(Context,-P).

not_holds_wrapper(Context,R,X,Y,Z,Q):-nonvar(R),!,
        P=..[R,X,Y,Z,Q],
        all_predicate_cache(Context,-P).

not_holds_wrapper(Context,R,X,Y):-
        all_predicate_cache(Context,-P),
        P=..[R,X,Y].

not_holds_wrapper(Context,R,X,Y,Z):-
        all_predicate_cache(Context,-P),
        P=..[R,X,Y,Z].

not_holds_wrapper(Context,R,X,Y,Z,Q):-
        all_predicate_cache(Context,-P),
        P=..[R,X,Y,Z,Q].

positive_rule_fact(Context,X):-
        positive_fact_cache(Context,X),
        positive_rule_cache(Context,X).

positive_rule_only(Context,X):-
        positive_rule_cache(Context,X),
        not(positive_fact_cache(Context,X)).

positive_fact_only(Context,X):-
        positive_fact_cache(Context,X),
        not(positive_rule_cache(Context,X)).


all_predicate_cache(Context,X):-
        positive_fact_cache(Context,X).
all_predicate_cache(Context,X):-
        positive_rule_cache(Context,X).

positive_fact_cache(Context,+X):-positive_fact_cache(Context,+,X).
positive_fact_cache(Context,-X):-positive_fact_cache(Context,-,X).
positive_fact_cache(Context,+X):-positive_fact_cache(Context,?,X).
positive_fact_cache(Context,-X):-positive_fact_cache(Context,?,X).
positive_rule_cache(Context,+X):-var(X),!,positive_rule_cache(Context,+,X).
 % Note that -X is missing from positive rule cache when its a X is a Var
positive_rule_cache(Context,+X):-positive_rule_cache(Context,+,X).
positive_rule_cache(Context,-X):-positive_rule_cache(Context,-,X).
positive_rule_cache(Context,+X):-positive_rule_cache(Context,?,X).
positive_rule_cache(Context,-X):-positive_rule_cache(Context,?,X).


all_predicate_fast_cache(Context,X):-positive_fact_cache(Context,X).
all_predicate_fast_cache(Context,X):-positive_rule_cache(Context,X).

all_predicate_slow_cache(Context,X):-positive_fact_cache(Context,X).
all_predicate_slow_cache(Context,X):-positive_rule_only(Context,X).




remake_positive_cache(Context,Ctx):-
        unmake_positive_cache(Context,Ctx),
        make_positive_cache(Context,Ctx),!.

unmake_positive_cache(Context,Ctx):-
        retractall((make_positive_cache(Context,Ctx):-!)).

make_positive_cache(Context,Ctx):-
        retractall(positive_fact_cache(_,_,_,_)),
        retractall(positive_rule_cache(_,_,_,_)),
        retractall(negative_fact_cache(_,_,_,_)),
        retractall(negative_rule_cache(_,_,_,_)),
        fail.

make_positive_cache(Context,Ctx):-
        mooCache(R,Cons, Ante, _, L, Context, Ctx, Explaination),
        add_positive_rule_cache(Context,L,R),
        fail.

make_positive_cache(Context,Ctx):-
        mooCache(R,Fact, _, L, Context, Ctx, Explaination),
        add_positive_fact_cache(Context,L,R),
        fail.

make_positive_cache(Context,Ctx):-
        asserta((make_positive_cache(Context,Ctx):-!)),!.


add_positive_fact_cache(Context,L,R):-
        get_arity_fast(Context,R,A),!,
        functor(P,R,A),
        not(positive_fact_cache(Context,?,P)),
                ((L=true  ->
                        ((
                        retract(positive_fact_cache(Context,-,P)) ->
                                asserta(positive_fact_cache(Context,?,P)) ;
                                assert_if_new(positive_fact_cache(Context,+,P))
                        ));
                        ((
                        retract(positive_fact_cache(Context,+,P)) ->
                                asserta(positive_fact_cache(Context,?,P)) ;
                                assert_if_new(positive_fact_cache(Context,-,P))
                        ))
                )),!.

add_positive_rule_cache(Context,L,R):-
        get_arity_fast(Context,R,A),!,
        functor(P,R,A),
        not(positive_rule_cache(Context,?,P)),
                ((L=true  ->
                        ((
                        retract(positive_rule_cache(Context,-,P)) ->
                                asserta(positive_rule_cache(Context,?,P)) ;
                                assert_if_new(positive_rule_cache(Context,+,P))
                        ));
                        ((
                        retract(positive_rule_cache(Context,+,P)) ->
                                asserta(positive_rule_cache(Context,?,P)) ;
                                assert_if_new(positive_rule_cache(Context,-,P))
                        ))
                )),!.


get_arity_fast(Context,R,A):-have_arity(R,Context,A),!.
get_arity_fast(Context,R,A):-
        mooCache(R,Cons, _, _, Context, Ctx, Explaination),
        functor(Cons,R,A),
        asserta(have_arity(R,Context,A)),!.
get_arity_fast(Context,R,A):-
        mooCache(R,Cons, _, _, _, Context, Ctx, Explaination),
        functor(Cons,R,A),
        asserta(have_arity(R,Context,A)),!.
get_arity_fast(Context,R,A):-
        mooCache(valence, _, valence(R,A), Vars, ContextName, Context, Tracking, Agent, Status),
        asserta(have_arity(R,Context,A)),!.
get_arity_fast(Context,R,2).


% ensureMooContext
% make_dead
% sync_memory

% should_be_loaded


%% :-include('logicmoo_utils_header.pl').

activate_context(KnowledgeBase:Context):-
                ensureMooContext(KnowledgeBase,Context).

getDefaultImageFilepath(X):-
        'LOGIC_ENGINE_RT'(Local),concat_atom([Local,'/','moo_image.data'],X).


save_ado_cache:-saveMooCache.
                                                                                                                         
                                                                                                                         
saveMooCache:-
                save_can_to_file(Context,Handle),close(Handle),
                retractall(save_can_to_file(Context,Handle)),
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
        say('
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
        retractall(mooCache(Literal,asserted,Context,surf(Context,TN,CLID,Vars))),  %Facts /5
        retractall(mooCache(Literal,deduced,Context,surf(Context,TN,CLID,Vars))),  %Facts /5
        retractall(mooCache(Literal,AnteLiteral,asserted,Context,surf(Context,TN,CLID,Vars))),         %Rules /6
        retractall(mooCache(Literal,AnteLiteral,deduced,Context,surf(Context,TN,CLID,Vars))),  %Rules /6
        retractall(mooCache(Surface,CLF,Flags,Vars,Context,TN,Maintainer,TMResult)),  %Surface /9
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

save_each_theory([]).
save_each_theory([H|T]):-
        save_the_theory(H),!,
        save_each_theory(T),!.

save_the_theory(H):-!.
save_the_theory(H):-
        concat_atom(['moo_theoryimage_',H,'.pl'],FileName),
        safe_file_open(FileName,'w',OUTPUT),
%       say(OUTPUT,':-include(\'logicmoo_utils_header.P\').\n',[]),
        say(OUTPUT,':-index(mooCache(1 ,0,1, 0,0,0, 0,0, 1)).\n',[]),
        say(OUTPUT,'% ~w for ~w.\n',[FileName,H]),
        get_time(Time),convert_time(Time,String),
        say(OUTPUT,'% Created at ~w.\n',[String]),
        ignore(write_the_theory_now(OUTPUT,H)),
        say(OUTPUT,':-catch([moo_eval],_,true).\n',[]),
        close(OUTPUT).


write_the_theory_now(OUTPUT,KnowledgeBase):-
        mooCache(PredR,wfs,WFS,Prolog,KnowledgeBase,Context,AssertionID,Creator,on),
        say(OUTPUT,'~q.\n',[Prolog]),
        fail.
write_the_theory_now(OUTPUT,KnowledgeBase):-
        get_time(Time),convert_time(Time,String),
        say(OUTPUT,'% Finished at ~w.\n',[String]).


remove_all_but_logOnFailure_chars(String,FileName):-
        atom_codes(String,Codes),
        subtract(Codes,[32,58],CCodes),
        atom_codes(FileName,CCodes).

:-dynamic(is_up_to_date/2).

ensureMooContext(KnowledgeBase,Context):-
        is_up_to_date(KnowledgeBase,Context),!.

ensureMooContext(KnowledgeBase,Context):-
        asserta(is_up_to_date(KnowledgeBase,Context)),!.

%       concat_atom(['moo_theoryimage_',KnowledgeBase,'.pl'],FileName),
 %       ensure_loaded(FileName).

show_active_memory:-!.
show_active_memory:-listing('in-active-memory').





save_each_clause_in_buffer(KnowledgeBase,Context,(PRO,LOG)):-!,
                save_each_clause_in_buffer(KnowledgeBase,Context,PRO),
                save_each_clause_in_buffer(KnowledgeBase,Context,LOG).

save_each_clause_in_buffer(KnowledgeBase,Context,PROLOG):-
                not(not((numbervars(((KnowledgeBase,Context,PROLOG)),'$VAR',0,_),assert_if_new(storage_buffer(KnowledgeBase,Context,PROLOG))))).

%assert_if_new(X):-X,!.
%assert_if_new(X):-!,assert(X).

% ===================================================================
%  RESETING WORKING MEMORY
% ===================================================================

clear_moo_memory:-
                  sendNote(extreme_debug,constentManger,'Rebuilding working memory',' '),
                  clear_tq_db,!.

clear_tq_db:-
        mooCache(PredR,Form,USurface,UVars,Context,TN,Maintainer,Result),
        TN>20000,retractall(mooCache(PredR,_,_,_,_,_,TN,_,_)),fail.
clear_tq_db:-
        mooCache(PredR,Form,_,_,Context,surf(_,TN)),
        TN>20000,retractall(mooCache(PredR,Form,_,_,Context,surf(_,TN))),fail.
clear_tq_db:-
        mooCache(PredR,Form,_,_,_,Context,(surf(_,TN)*_)),
        TN>20000,retractall(mooCache(PredR,Form,_,_,_,Context,(surf(_,TN)*_))),fail.

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
         sendNote(debug,contentManager,'Moo Context startup states being loaded',' '),
         %adoConnect,
         %sync_ado_cache,
         clear_moo_memory
         ),E,sendNote(debug,contentManager,'Moo Context startup states insufficient for full support',E));sendNote(debug,contentManager,'Moo Context startup states insufficient for full support',E).

:-dynamic('in-active-memory'/2).


set_default_theory(X):-retractall(getMooOption(Agent,opt_theory,_)),assert(getMooOption(Agent,opt_theory,X)).
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

Defining Contexts

The Moo LE must record forall information required to make fully persistent the side-effects specified for these calls. Refer to the description under "startup_status/1," above, for details of the requirements for persistence.

These calls are used to create, delete and manipulate the Context definitions known within a given Moo LE:

- define_theory(Context_Designator)
  Define a new Context based on the specified Context_Designator, the theory_name must not be the name of a Context already known to the receiving LE.

- rename_theory(Context_Designator)
  Change the theory_name associated with the theory_source in the Context_Designator, which must be known to the receiving LE. [ TBD: Multiple Contexts using equal source file? ]

- reconstitute_theory(Context_Designator)
  Change the theory_source associated with the theory_name in the Context_Designator, which must be known to the receiving LE.

- delete_theory(Context_Designator)
  Remove and forget everything about the specified Context (i.e., perform the "FG" action and then forget any internal and / or persistent descriptor for the specified Context). [ TBD: "If the specified Context is in the startup list, should it be removed from there as well" ?? Probably "yes." ]


Context_Designator
Where I use the term "Context_Designator" I mean a Prolog term with one of the following structures:

        theory(theory_name, theory_source)
        theory(theory_name, Context_SOURCE)
        theory(Context_NAME, theory_source)
        theory(theory_name)

        Other forms may be added as needed.


Examples:
        theory('LogicMOO', file('/MooL/SUO/LogicMOO.can')
        theory('LogicMOO', url('http://jordan/stheory/exported/merge.can')

        % Acceptable If the location is known:
        theory('LogicMOO', _)

        % Also acceptable if the location is known:
        theory('LogicMOO')

        Future forms may include:

        theory('LogicMOO', ftp('jordan', 'anonymous', 'rs@cris.com', '/theorys/LogicMOO.can'))
        theory('LogicMOO', https('jordan', 8080, 'mysession', '/theorys/LogicMOO.can'))


theory_source is one of:

        file(local_file_name)
        url(file_url)
        url(http_url)

        (Other forms may be added as needed.)

Note: file_url and http_url are TBD and their implementation is yet to be scheduled.


Startup

When a Moo LE starts up it first establishes or initializes is basic functions. Once that initialization completes successfully, it must next re-establish the client-specified initial state.

Current state initialization centers on Context pre-loading and is described under "startup_status/1," above.

Since startup actions occur apart from any end-user action and thus have no "client" to which to return feedback, status or other diagnostics, the Moo LE must write to its log any noteworthy or erroneous events that occur while it performs its startup actions.


*/
% Defining and renaming Contexts

writeXMLTagged(moo:message,Goal):-
      write_response_begin,
      ignore(Goal),
      write_response_end.

define_theory(Var):-var(Var),!,
      verify_status(X),!,
      member((Var=_),X).

define_theory(theory(Name,Location)):-
         writeXMLTagged(moo:message,theory_startup_start(theory(Name,Location)=void)).

define_theory(_):-
      write_response_begin,
      ignore(writeIfOption(disp_error,'syntax error'('say for define_theory/1 is define_theory(theory(''PrologMOOAddition'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')).'),_)),
      write_response_end.

rename_theory(Var):-var(Var),!,
      write_response_begin,
      ignore(writeIfOption(disp_error,'syntax error'('say for rename_theory/1 is rename_theory(theory(''NewName'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')).'),_)),
      write_response_end.

rename_theory(theory(Name,Location)):-
         writeXMLTagged(moo:message,(
                  sendNote(debug,logicEngine,'Debug Info',[rename_theory,theory(Name,Location)]),
                  retractall(theory_loaded(N)),
                  define_theory_proc(theory(Name,Location))
           )).

rename_theory(_):-
      write_response_begin,
      ignore(writeIfOption(disp_error,'syntax error'('say for rename_theory/1 is rename_theory(theory(''NewName'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')).'),_)),
      write_response_end.

reconstitute_theory(Var):-var(Var),!,
      write_response_begin,
      ignore(writeIfOption(disp_error,'syntax error'('say for reconstitute_theory/1 is reconstitute_theory(theory(''PrologMOOAddition'', ''/cygdrive/c/Moo/the/new/path_to.can'')).'),_)),
      write_response_end.

reconstitute_theory(theory(Name,Location)):-
         writeXMLTagged(moo:message,(
               sendNote(debug,logicEngine,'Debug Info',[reconstitute_theory,theory(Name,Location)]),
               retractall(theory_loaded(Name)),
               define_theory_proc(theory(Name,Location))
               )).

reconstitute_theory(_):-
      write_response_begin,
      ignore(writeIfOption(disp_error,'syntax error'('say for reconstitute_theory/1 is reconstitute_theory(theory(''PrologMOOAddition'', ''/cygdrive/c/Moo/the/new/path_to.can'')).'),_)),
      write_response_end.

delete_theory(Var):-var(Var),!,
      write_response_begin,
      ignore(writeIfOption(disp_error,'syntax error'('say for delete_theory/1 is delete_theory(theory(''PrologMOOAddition'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')).'),_)),
      write_response_end.

delete_theory(theory(Name,Location)) :-theory_startup_status(theory(Name,Location)=unknown).

delete_theory(_):-
      write_response_begin,
      ignore(writeIfOption(disp_error,'syntax error'('say for delete_theory/1 is delete_theory(theory(''PrologMOOAddition'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')).'),_)),
      write_response_end.

delete_theory_proc(theory(Name,Location)) :-
         sendNote(debug,contentManager,'theory deletions',[deleting_theory,theory(Name,Location)]),
         retractall(theory_make_status_start(theory(Name,_)=_)),
         save_theory_statuses.


startup_status(ContextS_List):-
      write_response_begin,
      ignore(writeIfOption(disp_error,'predicate not found'('server_startup_status(Context_List) will is establish startup statuses and    theory_status( theory(Name,_)=Status ) will change a single Context status.'),_)),
      write_response_end.



% ===================================================================================================
% Status API
% These calls allow clients to ascertain Context load and currency status and to effect the loading and unloading of Contexts within an LE.
% ===================================================================================================

% ===================================================================================================
%  verify_status(ContextS_List)  Succeeds if every Context_Status in ContextS_List unifies with the current status for the WFS whose name equal the theory_name in that Context_Status.
% ===================================================================================================
verify_status(ContextS_List):-var(ContextS_List),!,   write('<LE></LE>'),whole_status_set(ContextS_List).

verify_status([theory(Context, Path) = Context_STATUS]):-
            var(Context),var(Path),var(Context_STATUS),          !,
            write('<LE></LE>'),whole_status_set(ContextS_List),
            member((theory(Context, Path) = Context_STATUS),ContextS_List).


whole_status_set(ContextS_List):-findall( Context=Status,theory_status(Context,Status),ContextS_List),!.

verify_status(ContextS_List):- write('<LE></LE>'),
                     whole_status_set(ContextS_ListO),
                     member(X,ContextS_List),member(X,ContextS_ListO).

verify_status_proc(ContextS_List):-nonvar(ContextS_List),

                                                setof( (theory(Name,Location)=Status),
                                                                  ((
                                                                   (member(theory(Name,Location)=_,ContextS_List);member(theory(Name,Location),ContextS_List)),
                                                                   theory_status(theory(Name,Location),Status)
                                                                    )),
                                                   ContextS_List).

% ===================================================================================================
% verify_status(ContextS_List, Status_Return)  Performs the actions of verify_status/1 and bind Status_Return to a list of actual status designators corresponding to the Context_Designators in the request list.
%  A Context_Status bearing a theory_name that is not known to the responding Moo LE will equal only with the status code "unknown" (or a variable).
% ===================================================================================================

verify_status(ContextS_List,OUT):-nonvar(ContextS_List),write('<LE>'),
                                                ignore(setof( (theory(Name,Location)=Status),
                                                                  ((
                                                                   (member(theory(Name,Location)=_,ContextS_List);member(theory(Name,Location),ContextS_List)),
                                                                   theory_status(theory(Name,Location),Status)
                                                                    ))),
                                                   OUT),write('</LE>').

% ===================================================================================================
% establish_status(ContextS_List)  Succeed if for every Context_Status in ContextS_List, the Context with the specified theory_name either already has the specified status or can be made to have that state by successfully carrying out the state transition action given in the table above.
% ===================================================================================================

establish_status(ContextS_List):-!,
                  writeXMLTagged(moo:message,establish_status_0(ContextS_List)).

establish_status_0([]):-!.
establish_status_0([First|REST]):-!,
      establish_status_1(First),!,
      establish_status_0(REST),!.
establish_status_0(theory(Name,Location)=RequestStatus):- !,
            establish_status_1(theory(Name,Location)=RequestStatus),!.

establish_status_1(theory(Name,Location)=S):-nonvar(S),S=unknown,!,
                  delete_theory_proc(theory(Name,Location)).

establish_status_1(theory(Name,Location)=RequestStatus):-
                  verify_status_proc([theory(Name,Location)=CurrentStatus]),!,
                  sendNote(debug,contentManager,'theory status establish',[establish,RequestStatus,from,CurrentStatus,theory(Name,Location)]),
                  ignore(make_status(theory(Name,Location),CurrentStatus,RequestStatus)),!.

% ===================================================================================================
% elevate_status(ContextS_List)  Succeed if for every Context_Status in ContextS_List, the Context with the specified theory_name either already has the specified status or can be elevated to that state by successfully carrying out the state transition action given in the table above never reducing.
% ===================================================================================================

elevate_status(ContextS_List):-!,
                  write('<LE>'),
                  ignore(elevate_status_0(ContextS_List)),!,
                 write('</LE>'),!.

elevate_status_0([]):-!.
elevate_status_0([First|REST]):-!,
      elevate_status_1(First),!,
      elevate_status_0(REST),!.
elevate_status_0(theory(Name,Location)=RequestStatus):- !,
            elevate_status_1(theory(Name,Location)=RequestStatus),!.

elevate_status_1(theory(Name,Location)=RequestStatus):-
                  verify_status_proc([theory(Name,Location)=CurrentStatus]),!,
                  sendNote(debug,contentManager,'theory status elevate',[elevate,RequestStatus,from,CurrentStatus,theory(Name,Location)]),
                  elev_status(theory(Name,Location),CurrentStatus,RequestStatus),!.

/*

establish_status([theory('LogicMOO',_) = current]).

- establish_status(ContextS_List, Status_Return)
  Perform the actions of establish_status/1 and bind Status_Return to a list of actual status designators corresponding to the Context_Designators in the request list.

Status Transition

The following tables indicates the actions to take to effect a transition from the status in the left-hand column to the status shown in the column headers along the top. The "file" is whatever external forms the Moo LE happens to be using currently (i.e., it's an unspecified external form meant to be used to facilitate Context loading by comparison to recompilation).

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

theory_status(theory(Name,Location),Status):- theory_make_status_start(theory(Name,Location)=_), lookup_theory_status(theory(Name,Location),Status).

lookup_theory_status(X,Y):- lookup_theory_status_array(X,Y),!.


lookup_theory_status_array(theory(Name,Location),void):- not((moo_file_loaded(p(Name)))),not(theory_p_file_current(Name)).
lookup_theory_status_array(theory(Name,Location),available):- not(moo_file_loaded(p(Name))),theory_p_file_current(Name).
lookup_theory_status_array(theory(Name,Location),old):- (moo_file_loaded(p(Name))),theory_modified(Name),not(theory_p_file_current(Name)).
lookup_theory_status_array(theory(Name,Location),stale):- (moo_file_loaded(p(Name))),theory_modified(Name),(theory_p_file_current(Name)). %should be stale
lookup_theory_status_array(theory(Name,Location),volatile):- (moo_file_loaded(p(Name))),not(theory_modified(Name)),not(theory_p_file_current(Name)).
lookup_theory_status_array(theory(Name,Location),current):- (moo_file_loaded(p(Name))),not(theory_modified(Name)),(theory_p_file_current(Name)).
lookup_theory_status_array(theory(Name,Location),unknown):-!,nonvar(Name),nonvar(Location).

:-dynamic(theory_loaded/1).
:-dynamic(theory_is_modified/1).

theory_modified(Name):-theory_is_modified(Name).

theory_p_file_current(Name):-safe_theory_info_db(Name,Location,_WFS,PFILE),file_newer(PFILE,Location).


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

% theory_startup_status(theory('LogicMOO', '/cygdrive/c/Moo/SUO/LogicMOO.can')=available)

make_status(theory(Name,Location),_,unknown):-!,
                           sendNote(debug,contentManager,Name,[making,unknown,from,Location,'(=> deletion)']),
                           delete_theory_proc(theory(Name,Location)),!.

make_status(theory(Name,Location),_,available):-!,
                           sendNote(debug,contentManager,Name,[making,available,from,Location,'(compiled but not loaded)']),
                           source_to_p_file(Name),!.

make_status(theory(Name,Location),_,current):-!,
                           sendNote(debug,contentManager,Name,[making,current,from,Location]),
                           load_theory_file(Name),!.

elev_status(theory(Name,Location),_,available):-!,
                           sendNote(debug,contentManager,Name,[elevating,to,available,from,Location]),
                           source_to_p_file(Name),!.
elev_status(theory(Name,Location),_,current):-!,
                           sendNote(debug,contentManager,Name,[elevating,to,current,from,Location]),
                           load_theory_file(Name),!.

elev_status(theory(Name,Location),_,_):-!,
                           sendNote(debug,contentManager,Name,[elevating,has,no,effect]).

/*
  theory_startup_status(ContextS_List)
  The Moo LE that executes this call should record the specified Context status list as the status that the LE should attempt to establish upon startup. The specified ContextS_List must be fully ground and the status indicators should be permissible as target status codes. A Context_Designator with an unknown theory_name is only acceptable if its status indicator is "unknown." [ Is there any point in that special case? ]
  The information recorded to implement this call must be stored in a manner that is persistent in the face of forall known shutdown and restart mechanisms, whether controlled or uncontrolled (Prolog crash, system crash, power failure) and whether abortive (e.g. SIGTERM or SIGKILL) or cooperative (shutdown request).
  The means by which Moo LE achieves this persistence is unspecified (fully at its discretion).
  Succeeds if and only if the startup status designators are successfully recorded.

*/
theory_startup_status(Var):- var(Var),
            findall(Context,theory_make_status_start(Context),List),!,
            member(Var,List).

theory_startup_status(ContextDesignator=Status):-var(Status),!,theory_make_status_start(ContextDesignator=Status).
theory_startup_status(ContextDesignator=Status):-
      write_response_begin,
               theory_startup_status_0(ContextDesignator=Status),
               save_theory_statuses,
      write_response_end.

theory_startup_status(_):-
       writeXMLTagged(moo:message,writeIfOption(disp_error,'syntax error'('say for theory_startup_status/1 is theory_startup_status(theory(''PrologMOOAddition'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')=current).'),_)).

theory_startup_status_0(ContextDesignator=unknown):-nonvar(ContextDesignator),!,delete_theory_proc(ContextDesignator).
theory_startup_status_0(ContextDesignator=Status):-nonvar(Status),nonvar(ContextDesignator),!,
               sendNote(debug,logicEngine,'Debug Info','Content Management'(set_theory_startup_status(ContextDesignator=Status))),
               nonvar(Status),
               retractall(theory_make_status_start(ContextDesignator=_)),
               assert(theory_make_status_start(ContextDesignator=Status)),
               ignore(elev_status(ContextDesignator,_,Status)).



% ===================================================================================================
%  verify_startup_status(ContextS_List)  Succeeds if every Context_Status in ContextS_List unifies with the current status for the WFS whose name equal the theory_name in that Context_Status.
% ===================================================================================================

verify_startup_status(Var):-var(Var),!,whole_startup_status_set(ContextS_List).

verify_startup_status([theory(Context, Path) = Context_STATUS]):-
            var(Context),var(Path),var(Context_STATUS),          !,
            write('<LE></LE>'),whole_startup_status_set(ContextS_List),
            member((theory(Context, Path) = Context_STATUS),ContextS_List).

verify_startup_status(ContextS_List):-write('<LE></LE>'),nonvar(ContextS_List),

                                                setof( (theory(Name,Location)=Status),
                                                                  ((
                                                                   (member(theory(Name,Location)=_,ContextS_List);member(theory(Name,Location),ContextS_List)),
                                                                   theory_make_status_start(theory(Name,Location)=Status)
                                                                    )),
                                                   ContextS_List).



% ===================================================================================================
%  server_startup_status(ContextS_List)  Succeeds if every Context_Status in ContextS_List unifies with the current status for the WFS whose name equal the theory_name in that Context_Status.
%  The Moo LE that executes this call should record the specified Context status list as the status that the LE should attempt to establish upon startup. The specified ContextS_List must be fully ground and the status indicators should be permissible as target status codes. A Context_Designator with an unknown theory_name is only acceptable if its status indicator is "unknown." [ Is there any point in that special case? ]
%  The information recorded to implement this call must be stored in a manner that is persistent in the face of forall known shutdown and restart mechanisms, whether controlled or uncontrolled (Prolog crash, system crash, power failure) and whether abortive (e.g. SIGTERM or SIGKILL) or cooperative (shutdown request).
%  The means by which Moo LE achieves this persistence is unspecified (fully at its discretion).
%  Succeeds if and only if the startup status designators are successfully recorded.
% ===================================================================================================
server_startup_status(ContextS_List):-var(ContextS_List),!,whole_startup_status_set(ContextS_List).

server_startup_status([theory(Context, Path) = Context_STATUS]):-
            var(Context),var(Path),var(Context_STATUS),          !,
            write('<LE></LE>'),whole_startup_status_set(ContextS_List),
            member((theory(Context, Path) = Context_STATUS),ContextS_List).

whole_startup_status_set(ContextS_List):-findall(Context,theory_make_status_start(Context),ContextS_List).

server_startup_status(LIST):-
      findall((theory(X,Y)=Status),member((theory(X,Y)=Status),LIST),Look),
      Look=LIST,!,
      write_response_begin,
         server_startup_status_0(LIST),
         save_theory_statuses,
      write_response_end.

server_startup_status(_):-!,
      write_response_begin,
      ignore(writeIfOption(disp_error,'syntax error'('say for server_startup_status/1 is server_startup_status([theory(''PrologMOOAddition'', ''/cygdrive/c/Moo/SUO/PrologMOOAddition.can'')=current,theory(''LogicMOO'', ''/cygdrive/c/Moo/SUO/LogicMOO.can'')=unknown]).'),_)),
      write_response_end.


server_startup_status_0([]):-!.
server_startup_status_0([Head|Tail]):-!,
         theory_startup_status_0(Head),
         server_startup_status_0(Tail).

establish_each([]):-!.
establish_each([ContextDesignator=Status|REST]):-!,once(make_status(ContextDesignator,_,Status)),
              establish_each(REST).



invokeTheoryLoading(Context):-isTheoryLoading(Context,Status),!.
invokeTheoryLoading(Context):-
        servantProcessCreate(loadContextfromSource(Context,GlobalContext),loadContextfromSource(Context,GlobalContext),ID,[detatched(false)]),
        assert(isMooProcess(ID,loadContextfromSource(Context,GlobalContext))).

loadContextfromSource(Context,Ctx):-
                getFilenameOfContextSource(Context,Filename),!,
                once(invokeKifFileChecking(Context,Filename,Ctx,Agent)).

getFilenameOfContextSource(Context,Filename):-
                fmtString(FileChars,'../moo-rt/work/~w.can',[Context]),!,
                string_to_atom(FileChars,Filename),!.



% ===========================================================
% Test Knowledge Base File (Currently only Test until browser integration)
% ===========================================================
loadKnowledgebaseSourcefile(Name,Filename):-  make,
                catch(atom_codes(Name,Codes),_,Codes=[]),
                length(Codes,L),L<3,
                file_base_name(Filename,BaseName),
                file_name_extension(Context,Extension,BaseName),
                say('<B color=red>No name was given, so a Knowledge Base called <font color=green>~w</font> is being created.<p>',[Context]),
                load_kif_to_theory_ctx(Context,Filename,'GlobalContext','MooWeb').

loadKnowledgebaseSourcefile(Context,Filename):-!,
                (unsetMooOption(Agent,opt_surface_check=_)),
                (setMooOption(Agent,opt_surface_check=trusted)),
                idGen(TN1),
                idGen(TN2),
                idGen(TN3),
                retractall(mooCache(PredR,_,_,Context,_,_,_,_)),
                assertaClean(mooCache(PredR,surface,'instance'(Context,'KnowledgeBase'),'$VAR'(0),'MooKernel','GlobalContext',TN1,'WebUser',gaf)),
                assertaClean(mooCache(PredR,surface,'instance'('GlobalContext','Context'),'$VAR'(0),Context,'GlobalContext',TN2,'WebUser',gaf)),
                assertaClean(mooCache(PredR,surface,'sourcefile-of'(Context,Filename),'$VAR'(0),'MooKernel','GlobalContext',TN3,'WebUser',gaf)),
                invokeKifFileChecking(Context,Filename,'GlobalContext','MooWeb').



invokeKifFileChecking(Context,Filename,Ctx,Agent):-!, make,tell(user_error),
        ignore(Agent='MooWeb'),
         get_default_assertion_context(DCtx), ignore((Ctx=DCtx)),
         ensureMooOption(Agent,opt_theory,_,Context),
         retractall(mooCache(PredR,_,_,_,Context,_,_,_)),!,
         saveMooCache,
         say(user_error,'Reading In ~w to ~w with a default context of ~w \n',[Filename,Context,Ctx]),
         flag('Axioms Compiled',_,0),
         safe_file_open(Filename,'r',INPUT),!,
         say(user_error,'~q\n',[safe_file_open(Filename,'r',INPUT)]),
         repeat,
                  compile_each_line(INPUT),
        at_end_of_stream(INPUT),
        close(INPUT),
        flag('Axioms Compiled',AX,AX),
         say('\n% Compiled ~w axioms.\n',[AX]),
         %(test_syntax_save_theory_ctx(Context,Filename)),
         ignore(retract(findings(CPU,RESULT))) ,
         ignore(findall(T,retract(title(T)),Title)).

compile_each_line(Stream):-
                sleep(1),
                once(readKIF_priv(Stream,RRAW)),
                once(getCleanCharsWhitespaceProper(RRAW,Trimed)),
                once(ltrim(Trimed,L_trimmed)),
                        catch(say(user_error,'"~s" \n',[L_trimmed]),_,true).

/*
                        source_from_stream(INPUT,Trimed,SOURCEFORM,Vars),nonvar(Trimed),
                        catch(say(user_error,'~s \n',[Trimed]),_,true),
                        rememberAxioms(Context,SOURCEFORM,Vars,Agent),!.

*/






%ttheory:-loadContextfromSource('LogicMOO','GlobalContext').
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




rememberAxioms(Context,file_comment(_),Vars,Agent):-!.
rememberAxioms(Context,surf,Vars,Agent):-!.
rememberAxioms(Context,SOURCEFORM,Vars,Agent):-!,
                global_increment('Axioms Compiled'),
                flag('Axioms Compiled',AssertionID,AssertionID),
                idGen(INTID),
                assert(mooCache(PredR,surface,SOURCEFORM,Vars,Context,AssertionID,Agent,uncanonicalized)).

test_syntax_save_theory_ctx(KnowledgeBase,Context,Filename):-
         safe_file_open(Filename,'w',OUTPUT),
         test_syntax_write_theory_ctx(KnowledgeBase,Context,OUTPUT),
         close(OUTPUT).

test_syntax_save_theory_ctx:-test_syntax_save_theory_ctx('LogicMOO','GlobalContext','C:/mool/SUO/MFixed.txt').


test_syntax_write_theory_ctx(KnowledgeBase,Context,OUTPUT):-
                mooCache(PredR,surface,Source,Vars,KnowledgeBase,Context,AssertionID,Creator,Status),
                once(catch((toMarkup(kif,Source,Vars,OutChars),say(OUTPUT,'~w\n',[OutChars])),_,true)),
                fail.

test_syntax_write_theory_ctx(KnowledgeBase,Context,OUTPUT):-!,saveMooCache.

:-setMooOptionDefaults.


