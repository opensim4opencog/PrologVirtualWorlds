% ===================================================================
% OPERATOR PRECEDANCE
% ===================================================================
:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

:-ensure_loaded('logicmoo_utils.pl').

:-multifile(opHowto/2).
:-dynamic(opHowto/2).
:-multifile(invokeTodo/2).
:-dynamic(invokeTodo/2).
:-multifile(notifyChanges/1).
:-dynamic(notifyChanges/1).
:-dynamic(getRegistered/3).
:-multifile(getRegistered/3).

:-dynamic(chattingWith/2).
:-multifile(chattingWith/2).

:- dynamic valueOf/3.
:- dynamic typeOf/3.
:- index(valueOf(1,1,1)).
:- index(typeOf(1,1,1)).
:-dynamic(saved/4).
:-index(saved(1,1,0,0)).

:-dynamic(mooCache/9).
:-dynamic(mooCache/8).

:-multifile(clientEventPre/3).
:-dynamic(clientEventPre/3).
:-multifile(clientEvent/3).
:-dynamic(clientEvent/3).
:-multifile(clientEventUnhandled/3).
:-dynamic(clientEventUnhandled/3).

              
:- op(1200,fx,::-).         /* operator for integrity constraints */

%:- op(1200,xfx,<--).
%:- op(1150,fx,[(tabled),(prolog),(default)]).


:- op(500,xfx,#).
:- op(500,xfy,:).
%:- op(550,xfx,:).
%:- op(500,xfx,'@').

:- op(900,xfx,<-).
:- op(400,fx,'~').
:- op(400,fy,not).    % negation
:- op(500,xfy,and).   % conjunction
:- op(600,xfy,or).   % disjunction
:- op(0,xfx, 'equal' ).
:- op(900,xfx,'<=').
:- op(900,xfx,'if').
:- op(1000,xfy,'=>').
:- op(400,fy,known).  % Found in Model
:- op(400,fy,possible).  % Not In Model
:- op(400,fy,next).  % Next time
:- op(400,fy,after).  % Next time
:- op(400,fy,then).  % Next time
:- op(650,xfy,=>).  % implication
:- op(700,xfy,<=>). % equivalence
:- op(400,fy,always).  % Necessity, Always
:- op(400,fy,possible).  % Possibly, Eventually
:- op(400,fy,necessary).  % Necessity




