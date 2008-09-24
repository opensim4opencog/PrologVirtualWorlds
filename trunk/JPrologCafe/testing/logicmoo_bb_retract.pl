:-include('logicmoo_utils_header.pl'). %<?
% ===========================================================
% Detroys an External TN
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(submit=tn_destroy,Options),!,
        ensureMooOption(Agent,opt_ctx_assert,'GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory,'LogicMOO',Context),
        ensureMooOption(Agent,client,'java',CLIENT),
        ensureMooOption(Agent,tn,0,TN),
        ensureMooOption(Agent,sf,surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        ensureMooOption(Agent,user,'Web',Agent),
        destroyTN(Context,TN,Ctx),
        saveMooCache.

destroyTN(Context,TN,Ctx):-
        retractall(mooCache(Literal,_,Context,TN)),  %Facts
        retractall(mooCache(Literal,AnteLiteral,_,Context,TN)),   %Rules
        retractall(mooCache(Surface,CLF,Flags,Vars,Context,TN,Maintainer,TMResult)).

destroyContext(Context):-
        retractall(mooCache(Context,_)),
        retractall(mooCache(Context,_,_)),
        retractall(mooCache(Context,_,_,_)),
        retractall(mooCache(Literal,_,Context,TN)),  %Facts
        retractall(mooCache(Literal,AnteLiteral,_,Context,TN)),   %Rules
        retractall(mooCache(Surface,CLF,Flags,Vars,Context,TN,Maintainer,TMResult)),
        saveMooCache.

% ===========================================================
% Retract
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(submit=retract,Options),!,
        ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        ensureMooOption(Agent,sf=surf,Retraction),
        atom_codes(Retraction,Retraction_Chars),
        ensureMooOption(Agent,user='Web',Agent),
        ensureMooOption(Agent,interp='kif',Interp),
        writeDebugWindow,
         getCleanCharsWhitespaceProper(Retraction_Chars,Show),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
              once(( NEWFORM=comment(_) ->
                     (do_chars(Show),!,FORM=_) ;(!,
                     logOnFailure(invokeOperation(verbose,retract(NEWFORM),Ctx,TN,Context,Agent,Vars))
                     ))).

% ===========================================================
% Delete Assertion
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(submit='Delete Assertion',Options),!,
        ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        ensureMooOption(Agent,asid=_,AID),
        ensureMooOption(Agent,user='Web',Agent),
        ensureMooOption(Agent,interp='kif',Interp),
        say('<H3><Font Color=Red>Deleted....</Font></H3>',[]),
        delete_assertion(AID).

delete_assertion(AID):-
        retract(mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,_)),
        delete_assertion_disp(Form,SURF,Vars,Context,AID,Maintainer),
        fail.

delete_assertion(AID):-say('<H3><Font Color=Red>Done Deleting.</Font></H3>',[]).

delete_assertion_disp(Form,SURF,Vars,Context,AID,Maintainer):-
        toMarkup(html,SURF,Vars,SAtom),
        say('<IMG src="pixmaps/bullet.gif" asrtid=~w><nobr>',[AID]),
        say('<b>~w</b> ID<font color=red>~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font>',[Form,AID,Context,Maintainer]),
        %format_o('&nbsp;&nbsp;~w&nbsp;Enabled&nbsp;&nbsp;<br>',chectheoryox(AID,OnOff)),
        say('~w<br>',[SAtom]),!.

