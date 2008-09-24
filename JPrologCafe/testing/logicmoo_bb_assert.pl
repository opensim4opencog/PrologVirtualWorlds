:-include('logicmoo_utils_header.pl'). %<?

%clientEvent(Channel,Agent,exec_lf(AS))

% ===========================================================
% Insert
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(submit=assert,Options),!,
        ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        ensureMooOption(Agent,sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        ensureMooOption(Agent,user='Web',Agent),
        ensureMooOption(Agent,interp='kif',Interp),
        logOnFailure(ensureMooOption(Agent,tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        logOnFailure(invokeOperation(verbose,assert(NEWFORM),Ctx,TN,Context,Agent,Vars)).

% ===========================================================
% Verify Surface Returns SUO-KIF
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(submit=verify_assert,Options),!,
        ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        ensureMooOption(Agent,client='java',CLIENT),
        ensureMooOption(Agent,sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        ensureMooOption(Agent,user='Web',Agent),
         logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
         write_out_kif(Assertion_Chars,NEWFORM,Vars,Ctx,Context,Maintainer).

write_out_kif(Assertion_Chars,computed(comment(_)),Vars,Ctx,Context,Maintainer):-
         say('Syntax Error: Unmatched parentheses in "~s"',[Assertion_Chars]).
write_out_kif(Assertion_Chars,comment(_),Vars,Ctx,Context,Maintainer):-
         say('Syntax Error: Unmatched parentheses in "~s"',[Assertion_Chars]).
write_out_kif(Assertion_Chars,NEWFORM,Vars,Ctx,Context,Maintainer):-
        logOnFailure(getTruthCheckResults(tell,[untrusted],surface,NEWFORM,Ctx,STN,Context,Vars,Maintainer,Result)),
        (Result=accept(_) ->
                        (toMarkup(kif,NEWFORM,Vars,Out),say('~w\n',[Out]),!)
                        ;
                        (
                        Result=notice(FormatStr,Args),
                        say('error:\n',[]),
                        say(FormatStr,Args)
                        )
        ),!.


