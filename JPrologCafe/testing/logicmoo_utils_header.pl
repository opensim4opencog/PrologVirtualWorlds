% ===================================================================
% OPERATOR PRECEDANCE
% ===================================================================
:-style_check(-singleton).
:-style_check(-discontiguous).
:-style_check(-atom).
:-style_check(-string).

:-ensure_loaded('logicmoo_utils.pl').

%getWordTokens(X,Y):-getWordTokens(X,Y).
:-op(1200,fx,::-).         /* operator for integrity constraints */
:-op(1000,xfy,'=>').
:-op(900,xfx,<-).
:-op(900,xfx,'if').
:-op(900,xfx,'<=').
:-op(700,xfy,<=>). % equivalence
:-op(650,xfy,=>).  % implication
:-op(600,xfy,or).   % disjunction
:-op(500,xfy,and).   % conjunction
:-op(500,xfy,:).
:-op(500,xfx,#).
:-op(400,fy,then).  % Next time
:-op(400,fy,possible).  % Possibly, Eventually
:-op(400,fy,possible).  % Not In Model
:-op(400,fy,not).    % negation
:-op(400,fy,next).  % Next time
:-op(400,fy,necessary).  % Necessity
:-op(400,fy,known).  % Found in Model
:-op(400,fy,always).  % Necessity, Always
:-op(400,fy,after).  % Next time
:-op(400,fx,'~').
:-op(0,xfx, 'equal' ).
%:-op(550,xfx,:).
%:-op(500,xfx,'@').
%:-op(1200,xfx,<--).
%:-op(1150,fx,[(tabled),(prolog),(default)]).
              




