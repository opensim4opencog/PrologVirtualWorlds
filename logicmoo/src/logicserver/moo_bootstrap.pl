
% Database
dynamicPredicate(mooCache/1).
dynamicPredicate(mooCache/2).
dynamicPredicate(mooCache/3).
dynamicPredicate(mooCache/4).
dynamicPredicate(mooCache/5).
dynamicPredicate(mooCache/6).
dynamicPredicate(mooCache/9).

% User Agent
dynamicPredicate('$MooOption'/3).
dynamicPredicate(saved_note/4).
dynamicPredicate(act_mem/3).

% Database
dynamicPredicate(tq_attempted_request/0).
dynamicPredicate(title/1).

% TQ System
dynamicPredicate(tq_missed_one_answer/0).
dynamicPredicate(tq_least_one_answer/0).
dynamicPredicate(t_answer_found/1).


% ===================================================================
% OPERATION PREDICATES
% ===================================================================
% Defaults
dynamicPredicate(getDefaultKB/1).
dynamicPredicate(get_default_request_context/1).
dynamicPredicate(get_default_assertion_context/1).
dynamicPredicate(version_tag/1).

dynamicPredicate(answer_found/1).
dynamicPredicate(moo_K_scenario/2).
dynamicPredicate(telling_prolog/0).  % If set asserts clauses into prolog database
dynamicPredicate(telling_file/0).   % If set write assertions to file
dynamicPredicate(disp_debug/5).         %PREDICATE RESOLUTON
dynamicPredicate(contexts/2).            %CONTEXT STATES
dynamicPredicate(setting_table/2).
dynamicPredicate(tabling/1).
dynamicPredicate(tabled_t/1).
dynamicPredicate(tabled_f/1).
dynamicPredicate(answer_yes/0).
dynamicPredicate(already_asked/2).
dynamicPredicate(save_all/2).
dynamicPredicate(moo_K_scenario/6).         %We keep a cache of forall consultations
dynamicPredicate(consultation_mode_on/0).
dynamicPredicate(resource_cache/2).
dynamicPredicate(debuggerize/0).

dynamicPredicate( le_ele/1).


dynamicPredicate('surface-word'/2).
dynamicPredicate('surface-macro'/2).
dynamicPredicate('browser-only'/1).
dynamicPredicate('not-implemented'/1).
dynamicPredicate('surface-atom'/1).
dynamicPredicate('surface-single-arity'/1).
dynamicPredicate('surface-multiple-arity'/1).
dynamicPredicate('surface-instance'/2).
dynamicPredicate('surface-subclass'/2).
dynamicPredicate('surface-class'/1).
dynamicPredicate('surface-quantifier'/1).



indexPredicate(positive_fact_cache(1,1,1,1)).
indexPredicate(positive_rule_cache(1,1,1,1)).
indexPredicate(negative_fact_cache(1,1,1,1)).
indexPredicate(negative_rule_cache(1,1,1,1)).

dynamicPredicate(have_arity/4).

% these hold prototypes of calls
dynamicPredicate(positive_fact_cache/4).
dynamicPredicate(positive_rule_cache/4).
dynamicPredicate(negative_fact_cache/4).
dynamicPredicate(negative_rule_cache/4).

dynamicPredicate(make_positive_cache/2).


dynamicPredicate('in-active-memory'/2).

dynamicPredicate('should_be_loaded'/2).



% utility modules
moduleFile(prolog([system]),'moo_utility.pl').
moduleFile(prolog([system]),'moo_fileutils.pl').
moduleFile(prolog([system]),'moo_uwdgraphs.pl').
moduleFile(prolog([system]),'moo_slg_tabling.pl').
moduleFile(prolog([system]),'moo_reader.pl').
moduleFile(prolog([system]),'moo_database.pl').

% server modules
moduleFile(prolog([system]),'moo_httpd.pl').
moduleFile(prolog([system]),'moo_soap.pl').
moduleFile(prolog([system]),'moo_browser.pl').
moduleFile(prolog([system]),'moo_markup.pl').
moduleFile(prolog([system]),'moo_generation.pl').
moduleFile(prolog([system]),'moo_cmdline.pl').
moduleFile(prolog([system]),'moo_api.pl').

% inference type modules
moduleFile(prolog([system,inference]),'moo_transitiveclosure.pl').
moduleFile(prolog([system,inference]),'moo_oprove.pl').
moduleFile(prolog([system,inference]),'moo_icl.pl').
moduleFile(prolog([system,inference]),'moo_tap.pl').
moduleFile(prolog([system,inference]),'moo_simple.pl').
moduleFile(prolog([system,inference]),'moo_pttp_xray.pl').
moduleFile(prolog([system,inference]),'moo_belief.pl').

% other
moduleFile(prolog([system]),'moo_sentence.pl').
moduleFile(prolog([system]),'moo_cycl.pl').
moduleFile(prolog([system]),'moo_nnf.pl').
moduleFile(prolog([system]),'moo_english.pl').
moduleFile(prolog([system]),'moo_chat.pl').
moduleFile(prolog([system]),'moo_compiler.pl').
moduleFile(prolog([system]),'moo_functionals.pl').
moduleFile(prolog([system]),'moo_insert.pl').
moduleFile(prolog([system]),'moo_testing.pl').

moduleFile(image('PrologMOO'),'moo_image.data').

