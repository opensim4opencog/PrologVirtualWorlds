:-include('logicmoo_utils_header.pl'). %<?
% ===========================================================
% Shows whats known about an External TN
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(submit=tn_show,Options),!,
        ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        ensureMooOption(Agent,client='java',CLIENT),
        ensureMooOption(Agent,tn=0,TN),
        ensureMooOption(Agent,sf=surf,Assertion),
        writeKnownFormsTN(_,Context:TN).

writeKnownFormsTN(Ctx,Context:Word):-(atom(Word)),!,
        writeKnownFormsAboutTerm(Ctx,Context:Word).

writeKnownFormsAboutTerm(Ctx,Context:Word):-
        mooCache(PredR,Fact,Pre,Type,true,Context,P),
        contains_const((Fact,Pre),Word),
        flag(explaination_linenumber,LN,1),
        once(writeObject(P,_)),
        write('<hr>'),
        fail.

writeKnownFormsAboutTerm(Ctx,Context:Word):-
        mooCache(PredR,Fact,Type,true,Context,P),
        contains_const(Fact,Word),
        flag(explaination_linenumber,LN,1),
        once(writeObject(P,_)),
        write('<hr>'),
        fail.

writeKnownFormsAboutTerm(Ctx,Context:Word).

contains_const(Fact,Word):-
        getConstants(atomic,Fact,List,_,_),!,
        memberchk(Word,List).

writeKnownFormsTN(Ctx,Context:TN):-
        say('\n</pre><H3><br>Compiled Forms</H3><pre>\n',[]),fail.

writeKnownFormsTN(Ctx,Context:TN):-
        flag(indent,_,0),
        mooCache(Surface,CLF,Flags,Vars,Context,TN,Maintainer,TMResult),
        once(tam(Surface,Vars)),fail.
/*
writeKnownFormsTN(Ctx,Context:TN):-
        flag(indent,_,0),
        mooCache(Surface,CLF,Flags,Vars,Context,TN,Maintainer,TMResult),
        once(writeObject(writeq(mooCache(Flags,Vars,Context,TN,Maintainer,TMResult)),Vars)),fail.

writeKnownFormsTN(Ctx,Context:TN):-
        flag(indent,_,0),
        mooCache(Surface,CAN,Flags,Vars,Context,TN,Maintainer,TMResult),
        flag(clause_id,_,0),
        tam(Surface,CAN,Flags,Vars,Context,TN,Maintainer,Result),fail.
*/
writeKnownFormsTN(Ctx,Context:TN):-
        say('\n</pre><H3><br>Active Forms</H3><pre>\n',[]),fail.

writeKnownFormsTN(Ctx,Context:TN):-
        flag(clause_id,CLID,CLID),
        format('<hr>Clauses: ~w',[CLID]),fail.

writeKnownFormsTN(Ctx,Context:TN):-
        mooCache(Fact,Pre,Cost,Context,TN),
        list_to_and(Pre,Ands),
        flag(indent,_,0),
        once(writeObject('<hr>',Vars)),
        once(writeObject(writeq(Fact:Pre),Vars)),
        once(writeObject(nl,Vars)),
        once(writeObject('Context |= '(Ands,Fact),Vars)),fail.

writeKnownFormsTN(Ctx,Context:TN):-
        mooCache(Fact,Cost,Context,TN),
        flag(indent,_,0),
        once(writeObject('<hr>',Vars)),
        once(writeObject(Fact,Vars)),fail.

/*
writeKnownFormsTN(Ctx,Context:TN):-
        isMooOption(Agent,opt_debug=on),
        writeKnownFormsTN_used(Ctx,Context:TN).
*/
writeKnownFormsTN(Ctx,Context:TN):-say('</pre>').

writeKnownFormsTN_used(Ctx,Context:TN):-
        say('\n</pre><H3><br>Belief Forms</H3><pre>\n',[]),
        retractall(request_theory(X)),
        asserta(request_theory(Context)),
        mooCache(PredR,Fact,Ctx,Context:TN:_^Vars),
        t_ado_cache(PredR,_,Fact, Vars, Context, TN, Surf, on),
        say('\n<hr>'),fail.

writeKnownFormsTN_used(Ctx,Context:TN):-
        mooCache(PredR,Fact, Ctx, Pre, Context:TN:_^Vars),
        toMarkup(html,formula(entails(Pre,Fact)),Vars,O),
        say('\n~w<hr>',[O]),fail.



