% ===================================================================
% File 'e2c'
% Purpose: Attempto Controlled English to CycL conversions from SWI-Prolog  
% This implementation is an incomplete proxy for CycNL and likely will not work as well
% Maintainer: Douglas Miles
% Contact: $Author: dmiles $@users.sourceforge.net ;
% Version: 'interface' 1.0.0
% Revision:  $Revision: 1.3 $
% Revised At:   $Date: 2005/06/06 15:43:15 $
% from Bratko chapter 17 page 455. This comes from Pereira and Warren paper, AI journal, 1980 
% ===================================================================
% ===================================================================
% PURPOSE
% This File is the bootstrap SWI-Prolog listener to handle API requests
% So first is loads the proper files and then starts up the system
% ===================================================================
%<?

%:-[el_holds].

%:-[owl_parser].
%:-[tasks_sd1].
%:-[sdom1].

% imports sentencePos/2,bposToPos/2
:-style_check(-string).
:-style_check(-singleton).
:-style_check(-discontiguous).
:-style_check(-atom).
:-set_prolog_flag(double_quotes,string).
:-set_prolog_flag(double_quotes,codes).
:-set_prolog_flag(float_format,'%.12g').
:-set_prolog_flag(gc,false).
%:-set_prolog_flag(optimise,true).
%:-set_prolog_flag(file_name_variables,false).
%:-set_prolog_flag(agc_margin,0).
%:-set_prolog_flag(trace_gc,false).
%:-set_prolog_flag(character_escapes,true).
%:-set_prolog_flag(double_quotes,string).
%:-set_prolog_flag(report_error,true).
%:-set_prolog_flag(verbose,normal).


:-redefine_system_predicate(system:shell(_)).
:-redefine_system_predicate(system:shell).
%:-redefine_system_predicate(system:prolog).
:-redefine_system_predicate(system:shell(_,_)).
:-redefine_system_predicate(system:halt).
:-redefine_system_predicate(system:exit(_)).
:-redefine_system_predicate(system:phrase(_,_,_)).

:-dynamic(system:phrase/3).
:-module_transparent(system:phrase/3).
:-multifile(system:phrase/3).
system:phrase(A, B, C) :-
    '$strip_module'(A, D, E),
        (   compound(E)
        ->  (   E=[F|G]
            ->  append(E, C, B)
            ;   catch(call(user:E, B, C),Er,catch(call(A,B,C),Er2,(format(user_error,'\nDCGError ~w/~w in ~q \n',[Er,Er2,call(E, B, C)]),fail)))
            )
        ;   E==[]
        ->  C=B
        ;   B=[E|C]
        ).

/*
system:phrase(A, B, C) :-
            '$strip_module'(A, D, E),
            (   E==[]
            ->  C=B
            ;   nonvar(E),
                E=[F|G]
            ->  append(E, C, B)
            ;   call(A, B, C)
            ).
*/
/*
system:phrase([], B,C ):-!,B=C.
system:phrase([F|G],B, C):-!,append([F|G],C,B).
system:phrase(A, B, C) :-compound(A),!,catch(call(A,B,C),_,fail).
system:phrase(A, [A|C], C).
*/


%:-use_module(library(rdf)).

%Load the TCP Library
:-use_module(library(socket)).
%:-use_module(library(unix)).
:-use_module(library(system)).
:-use_module(library(readutil)).
:-use_module(library(listing)).
:-use_module(library(shell)).
:-use_module(library(shlib)).
:-use_module(library(url)).
:-use_module(library(quintus)).
:-use_module(library(qsave)).
:-use_module(library(sgml)).
:-use_module(library(occurs)).
%:-use_module(library(rdf)).
:-use_module(library(system)).
:-use_module(library(shlib)).
:-use_module(library(listing)).
:-use_module(library(sgml)).
%:-use_module(library(rdf)).
:-use_module(library(socket)).
:-use_module(library(readutil)).

%Load the TCP Library
%:-use_module(library('http/http_open')).
:-use_module(library('http/http_client')).
%:-use_module(library('http/http_header')).
%:-use_module(library('http/thread_httpd')).

%:-use_module(library(unix)).
:-use_module(library(shell)).
:-use_module(library(shlib)).
:-use_module(library(url)).
:-use_module(library(quintus)).
:-use_module(library(qsave)).

%:-use_module((javart)).



:-include('logicmoo_utils_header.pl'). %<?

:-
ensure_loaded('logicmoo_api_reply'),
ensure_loaded('logicmoo_api_chat'),
ensure_loaded('logicmoo_api_system'),
ensure_loaded('logicmoo_api_cyc'),
ensure_loaded('logicmoo_api_batch'),
ensure_loaded('logicmoo_api_soap'),
ensure_loaded('logicmoo_api_markup'),
ensure_loaded('logicmoo_api_httpd'),
ensure_loaded('logicmoo_api_prolog'),
ensure_loaded('logicmoo_api_eggdrop'),
ensure_loaded('logicmoo_api_request'),
ensure_loaded('logicmoo_api_event'),
ensure_loaded('logicmoo_api_testing'),
ensure_loaded('logicmoo_api_java'),
ensure_loaded('logicmoo_atlas_combine'),
ensure_loaded('logicmoo_atlas_codec_packed'),
ensure_loaded('logicmoo_atlas_consume'),
ensure_loaded('logicmoo_atlas_create'),
ensure_loaded('logicmoo_atlas_delete'),
ensure_loaded('logicmoo_atlas_drop'),
ensure_loaded('logicmoo_atlas_dynamic_op'),
ensure_loaded('logicmoo_atlas_exec'),
ensure_loaded('logicmoo_atlas_from'),
ensure_loaded('logicmoo_atlas_get'),
ensure_loaded('logicmoo_atlas_grid'),
ensure_loaded('logicmoo_atlas_kif'),
ensure_loaded('logicmoo_atlas_logoff'),
ensure_loaded('logicmoo_atlas_logon'),
ensure_loaded('logicmoo_atlas_look'),
ensure_loaded('logicmoo_atlas_mode'),
ensure_loaded('logicmoo_atlas_move'),
ensure_loaded('logicmoo_atlas_persistance'),
ensure_loaded('logicmoo_atlas_reply'),
ensure_loaded('logicmoo_atlas_set'),
ensure_loaded('logicmoo_atlas_take'),
ensure_loaded('logicmoo_atlas_talk'),
ensure_loaded('logicmoo_atlas_touch'),
ensure_loaded('logicmoo_atlas_uo'),
ensure_loaded('logicmoo_atlas_worldforge'),
ensure_loaded('logicmoo_atlas_xml'),
ensure_loaded('logicmoo_bb_options'),
ensure_loaded('logicmoo_bb_assert'),
ensure_loaded('logicmoo_bb_el_access'),
ensure_loaded('logicmoo_bb_hl_access'),
ensure_loaded('logicmoo_bb_enable'),
ensure_loaded('logicmoo_bb_context'),
ensure_loaded('logicmoo_bb_edit'),
ensure_loaded('logicmoo_bb_search'),
ensure_loaded('logicmoo_bb_retract'),
ensure_loaded('logicmoo_cmp_compiler'),
ensure_loaded('logicmoo_cmp_functionals'),
ensure_loaded('logicmoo_cmp_sentence'),
ensure_loaded('logicmoo_cmp_simple'),
ensure_loaded('logicmoo_cmp_truth'),
ensure_loaded('logicmoo_cmp_cycl'),
ensure_loaded('logicmoo_module_aiml'),
ensure_loaded('logicmoo_module_eliza'),
ensure_loaded('logicmoo_nl_dictionary'),
ensure_loaded('logicmoo_nl_english'),
ensure_loaded('logicmoo_nl_englishy'),
ensure_loaded('logicmoo_nl_generation'),
ensure_loaded('logicmoo_nl_grammer_interface'),
ensure_loaded('logicmoo_nl_talk'),
ensure_loaded('logicmoo_nl_wn_utils'),
ensure_loaded('logicmoo_nl_pos'),
ensure_loaded('logicmoo_nl_best'),
ensure_loaded('logicmoo_pvw_actx_invokes'),
ensure_loaded('logicmoo_pvw_apldriver'),
ensure_loaded('logicmoo_pvw_espg'),
ensure_loaded('logicmoo_pvw_event_to_property'),
ensure_loaded('logicmoo_pvw_goal_threads'),
ensure_loaded('logicmoo_pvw_proquery'),
ensure_loaded('logicmoo_pvw_vb6support'),
ensure_loaded('logicmoo_pvw_vwevents'),
ensure_loaded('logicmoo_pvw_vwevent_to_method'),
ensure_loaded('logicmoo_pvw_vwmethods'),
ensure_loaded('logicmoo_pvw_vwtriggers'),
ensure_loaded('logicmoo_tap_ale'),
ensure_loaded('logicmoo_tap_algebra'),
ensure_loaded('logicmoo_tap_belief'),
ensure_loaded('logicmoo_tap_csat'),
ensure_loaded('logicmoo_tap_icl'),
ensure_loaded('logicmoo_tap_modlean'),
ensure_loaded('logicmoo_tap_motel'),
ensure_loaded('logicmoo_tap_byrd'),
ensure_loaded('logicmoo_tap_plan'),
ensure_loaded('logicmoo_tap_pttp_xray'),
ensure_loaded('logicmoo_tap_slg_tabling'),
ensure_loaded('logicmoo_tap_transitiveclosure'),
ensure_loaded('logicmoo_utils_file'),
ensure_loaded('logicmoo_utils_uwdgraphs').

:-set_prolog_flag(double_quotes,string).

            
help(_).
nondet_arg(N,Term,V):-arg(N,Term,V).


%:-cd('../dynamic_transparent/').
%:-ensure_loaded('../dynamic_transparent/world.persist').
%:-ensure_loaded('../world/mea.prolog').
%:-runServerThread(3677,'LOGICMOO').
%:-runServerThread(6767,'Atlas').
%:-runServerThread(4040,'TELNETDX').
%:-runServerThread(3679,'CycApiRequest').
%:-runServerThread(3681,'PROLOG').
%:-runServerThread(7777,'UOX').

%:-[moo_sentence].
:-ensure_loaded('../world/cyc.el').
%:-use_module(cyc).
:-eggdrop.
:-createCycServer(6700).

