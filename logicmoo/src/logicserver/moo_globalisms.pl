:-module(moo_globalisms,[
	 setMooOption/1,
	 setMooOption/2,
	 getMooOption/1,
	 getMooOption/2,
	 ensureMooOption/2,
	 ensureMooOption/3,
	 setMooOptionDefaults/0]).

% Database
:-dynamic(mooCache/1).
:-dynamic(mooCache/2).
:-dynamic(mooCache/3).
:-dynamic(mooCache/4).
:-dynamic(mooCache/5).
:-dynamic(mooCache/6).
:-dynamic(mooCache/7).
:-dynamic(mooCache/8).
:-dynamic(mooCache/9).

% User Agent
:-dynamic('$MooOption'/3).
:-dynamic(saved_note/4).
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



setMooOption([]):-!.
setMooOption([H|T]):-!,
      setMooOption(H),!,
      setMooOption(T),!.
setMooOption(Var=_):-var(Var),!.
setMooOption(_=Var):-var(Var),!.
setMooOption((N=V)):-nonvar(N),!,setMooOption_thread(N,V),!.
setMooOption(N):-atomic(N),!,setMooOption_thread(N,true).
	
setMooOption(Name,Value):-setMooOption_thread(Name,Value).
setMooOption_thread(Name,Value):-
	system_dependant:prolog_notrace((getThread(Process),
	retractall('$MooOption'(Process,Name,_)),
	asserta('$MooOption'(Process,Name,Value)),!)).


unsetMooOption(Name=Value):-nonvar(Name),
	unsetMooOption_thread(Name,Value).
unsetMooOption(Name):-nonvar(Name),
	unsetMooOption_thread(Name,_).
unsetMooOption(Name):-system_dependant:prolog_notrace(retractall('$MooOption'(_Process,Name,_Value))).


unsetMooOption_thread(Name):-
	unsetMooOption_thread(Name,_Value).

unsetMooOption_thread(Name,Value):-
	getThread(Process),
	retractall('$MooOption'(Process,Name,Value)).
	
getMooOption_nearest_thread(Name,Value):-
	getMooOption_thread(Name,Value),!.
getMooOption_nearest_thread(Name,Value):-
	'$MooOption'(_,Name,Value),!.
getMooOption_nearest_thread(_Name,_Value):-!.



isMooOption(Name=Value):-!,isMooOption(Name,Value).
isMooOption(Name):-!,isMooOption(Name,true).

isMooOption(Name,Value):-getMooOption_thread(Name,Value).

getMooOption_thread(Name,Value):-
	system_dependant:prolog_notrace((getThread(Process),
	'$MooOption'(Process,Name,Value))),!.


getMooOption(Name=Value):-nonvar(Name),!,ensureMooOption(Name,_,Value).
getMooOption(Name=Default,Value):-nonvar(Name),!,ensureMooOption(Name,Default,Value).
getMooOption(Name,Value):-nonvar(Name),!,ensureMooOption(Name,_,Value).


ensureMooOption(Name=Default,Value):-
	ensureMooOption(Name,Default,Value),!.
	
ensureMooOption(Name,_Default,Value):-
	getMooOption_thread(Name,Value),!.

ensureMooOption(Name,Default,Default):-
	setMooOption_thread(Name,Default),!.

ensureMooOption(Name,_Default,Value):-nonvar(Name),!,   
	setMooOption_thread(Name,Value),!.

ensureMooOption(_Name,Default,Default).



setMooOptionDefaults:-
             (unsetMooOption(_)),
             setMooOption(opt_callback='sendNote'),
             setMooOption(cb_consultation='off'),
             setMooOption(opt_debug='off'),
             setMooOption(cb_error='off'),
             setMooOption(cb_result_each='off'),

% User Agent Defaults for Blank Variables
             setMooOption(opt_cxt_request='GlobalContext'),
             setMooOption(opt_ctx_assert='GlobalContext'),
             setMooOption(opt_tracking_number='generate'),
             setMooOption(opt_agent='ua_parse'),
             setMooOption(opt_precompiled='off'),
             getMooOption(opt_theory,Context),setMooOption(opt_theory=Context),
             setMooOption(opt_notation='kif'),
             setMooOption(opt_timeout=2),
             setMooOption(opt_readonly='off'),
             setMooOption(opt_debug='off'),
             setMooOption(opt_compiler='Byrd'),
             setMooOption(opt_language = 'pnx_nf'),

%Request Limits
             setMooOption(opt_answers_min=1),
             setMooOption(opt_answers_max=999), %TODO Default
             setMooOption(opt_backchains_max=5),
             setMooOption(opt_deductions_max=100),
             setMooOption(opt_backchains_max_neg=5),
             setMooOption(opt_deductions_max_neg=20),
             setMooOption(opt_forwardchains_max=1000),
             setMooOption(opt_max_breath=1000), %TODO Default

%Request Contexts
             setMooOption(opt_explore_related_contexts='off'),
             setMooOption(opt_save_justifications='off'),
             setMooOption(opt_deductions_assert='on'),
             setMooOption(opt_truth_maintence='on'),
             setMooOption(opt_forward_assertions='on'),
             setMooOption(opt_deduce_domains='on'),
             setMooOption(opt_notice_not_say=off),


%Request Pobibility
             setMooOption(opt_certainty_max=1),
             setMooOption(opt_certainty_min=1),
             setMooOption(opt_certainty=1),
             setMooOption(opt_resource_commit='on').

