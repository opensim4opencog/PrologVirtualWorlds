:-include('logicmoo_utils_header.pl').
%:-module(moo_fileutils,[]).


% :-include('logicmoo_utils_header.pl').

% ===========================================================
% Context Agent File
% ===========================================================
ctxFromFile(BadCtxName,Filename,Context):-
                catch(atom_codes(BadCtxName,Codes),_,Codes=[]),
                length(Codes,L),L<3,
                file_base_name(Filename,BaseCtxName),
                file_name_extension(CtxName,Extension,BaseCtxName),
                say('<B color=red>No name was given, so a Context called <font color=green>~w</font> is being created.<p>',[CtxName]),
                load_kif_to_theory_ctx(CtxName,Filename,'GlobalContext','MooWeb').

ctxFromFile(CtxName,Filename,Context):-!,
                idGen(TN1),
                idGen(TN3),
                assertaClean(mooCache(PredR,surface,'instance'(CtxName,'Context'),'$VAR'(0),Context,'GlobalContext',TN1,'WebUser',on)),
                assertaClean(mooCache(PredR,surface,'sourcefile'(CtxName,Filename),'$VAR'(0),Context,'GlobalContext',TN3,'WebUser',on)),
                load_kif_to_theory_ctx(Context,Filename,CtxName,'MooWeb').




load_kif_to_theory_ctx(Context,FileName,Ctx,Agent):-!,
        ignore(Agent='MooWeb'),
        assert(telling_file),
%%         atom_concat(FileName,'.compiled.pl',DBG),
%%         tell(DBG),
         get_default_assertion_context(DCtx), !,ignore((Ctx=DCtx)),!,
         getMooOption(Agent,opt_theory,DContext), !,  ignore((Context=DContext)),!,

         say('Reading In ~w to ~w with a default context of ~w <p>',[FileName,Context,Ctx]),nl,
         flag('Axioms Compiled',_,0),
         safe_file_open(FileName,'r',INPUT),
         repeat,   %true,
                load_kif_to_theory_ctx_display(Context,Agent,INPUT),
                close(INPUT),
                flag('Axioms Compiled',AX,AX),
         say('\n% Compiled ~w axioms.\n',[AX]),
         ignore(retract(findings(CPU,RESULT))) ,
         ignore(findall(T,retract(title(T)),Title)),saveMooCache.



ado_to_prolog(FileName):-
        tell(FileName),
        mooCache(Pred, Head, Type, Logic,Context, Ctx, Explaination),
        w_ado_cache(Pred, Head, Type, Logic,Context, Ctx, Explaination),
        fail.

ado_to_prolog(FileName):-
        mooCache(Pred, Head, Cond,Type, Logic,Context, Ctx, Explaination),
        w_ado_cache(Pred, Head, Cond,Type, Logic,Context, Ctx, Explaination),
        fail.
ado_to_prolog(FileName):-told.

atom_to_prolog(FileName):-
        tell(FileName),
        atom_to_prolog(FileName),
        told.


atom_to_prolog:-
        mooCache(Pred, Head, Type, Logic,Context, Ctx, Explaination),
        a_ado_cache(Pred, Head, Type, Logic,Context, Ctx, Explaination),
        fail.

atom_to_prolog:-
        mooCache(Pred, Head, Cond,Type, Logic,Context, Ctx, Explaination),
        a_ado_cache(Pred, entails(Head, Cond),Type, Logic,Context, Ctx, Explaination),
        fail.

atom_to_prolog:-
        mooCache(atom(A)),
        format(' ~q ; ',[A]),fail.

atom_to_prolog.

a_ado_cache(argOf, Head, Type, true,Context, Ctx, Explaination):-!.
a_ado_cache(documentation, Head, Type, true,Context, Ctx, Explaination):-!.
a_ado_cache(Pred, FOO,Type, Logic,Context, Ctx, Explaination):-
        getConstants(atomic,FOO,List,_,_),
        assert_list(List).

assert_list([]):-!.
assert_list([A|L]):-!,
        assert_list_n(A),
        assert_list(L).


assert_list_n(A):-mooCache(atom(A)),!.
assert_list_n([]):-!.
assert_list_n('.'):-!.
assert_list_n(N):-number(N),!.
assert_list_n(A):-
        asserta(mooCache(atom(A))).





w_ado_cache(argOf, Head, Type, true,Context, Ctx, Explaination):-!.
w_ado_cache(documentation, Head, Type, true,Context, Ctx, Explaination):-!.

w_ado_cache(Pred, Head, Type, true,Context, Ctx, Explaination):-
        format('~q.~n',[Head]),!.
w_ado_cache(Pred, Head, Type, false,Context, Ctx, Explaination):-
        format('not_~q.~n',[Head]),!.
w_ado_cache(Pred, Head, Pre, Type, true,Context, Ctx, Explaination):-
        pre_to_b(Pre,B),
        format('~q:-~q.~n',[Head,B]),!.
w_ado_cache(Pred, Head, Pre, Type, false,Context, Ctx, Explaination):-
        pre_to_b(Pre,B),
        format('not_~q:-~q.~n',[Head,B]),!.

pre_to_b(B,var(B)):-isSlot(B),!.
pre_to_b(and(A,B),(AA,BB)):-!,pre_to_b(A,AA),pre_to_b(B,BB).
pre_to_b(not(B),BBO):-pre_to_b(B,BB),BB=..[F|A],atom_concat('not_',F,NF),BBO=..[NF|A].
pre_to_b(B,B).


load_kif_to_theory_ctx_display(Context,Agent,Stream):-at_end_of_stream(Stream),!.

load_kif_to_theory_ctx_display(Context,Agent,Stream):-
                      once((
                                source_from_stream(Stream,_,SOURCEFORM,Vars),
                                catch(
                                                (
                                                                        flag('Axioms Compiled',X,X+1),
                                                                        invokeInsert([trusted,nocanonicalize],surface,SOURCEFORM,Ctx,TN,Context,Vars,Agent)
                                                ),

                                        E,
                                        (line_count(Stream,Line),say('\nLine ~w  Uninterpretable "~q" (~q)\n',[Line,KIFSTERM,E]))
                                )
                      )),
                      fail.


% ===================================================================
%  Read Knowledge Representation File
% ===================================================================

% Example
lmerge:-tell_from_kif(forall,'c:/mooL/SUO/LogicMOO.can','LogicMOO','GlobalContext',Maintainer).

kif_file(File):-tell_from_kif(forall,File,File,'GlobalContext',Maintainer).

tell_from_kif(SourceFile):-!,tell_from_kif(forall,SourceFile,Context_Name,Ctx,SourceFile).

tell_from_kif(SourceFile,Context_Name):-!,tell_from_kif(forall,SourceFile,Context_Name,Ctx,SourceFile).

tell_from_kif(Driver,SourceFile,Context_Name,Ctx,Maintainer):-
            sendNote(debug,kifParser,['Loading Moo KIF/CAN file into ',Driver,' as',Context_Name,Ctx,from,SourceFile],' '),
            ignore(safe_theory_info_db(Context_Name,SourceFile,WFSFile,_)),
            ignore(Maintainer=SourceFile),
            safe_file_open(SourceFile,'r',INPUT),
            repeat,
               once((
                     once(readKIF(INPUT,CHARS)),
                     once(invokeInsert(Driver,chars,CHARS,Ctx,TN,Context_Name,Vars,Maintainer))
                     )),
            at_end_of_stream(INPUT),!,
            file_close(INPUT),
            garbage_collect,
            garbage_collect_atoms.

% ===================================================================
%  Read KIF File as Prolog
% ===================================================================


load_kif_as_prolog(SourceFile):-
         add_file_extension(".P",SourceFile,PLocation),
         file_newer(PLocation,SourceFile),!,
         sendNote(debug,kifParser,'Loading kif as if prolog.',[PLocation,from,SourceFile]),
         consult_as_dynamic(PLocation).


t6:-uaInsert("(pnx_nf (domain AssignmentFn 1 Function) GlobalContext T-536)").


load_kif_as_prolog(SourceFile):- !,
         add_file_extension(".P",SourceFile,PLocation),!,
         safe_file_open(SourceFile,'r',INPUT),
         safe_file_open(PLocation,'w',OUTPUT),
         sendNote(debug,kifParser,'Loading kif as if prolog.',[converting,SourceFile,to,PLocation]),
           repeat,
               once((once(readKIF(INPUT,Chars)),
                    %catch(say(OUTPUT,'\n% ~s  ',[Chars]),_,true),
                    tell_retract_parse_chars(Chars,PROLOG,_),
                     once((
                           PROLOG=comment(_)
                            ;
                          say(OUTPUT,'\n~q.\n',[PROLOG])
                            )))),
                     at_end_of_stream(INPUT), !,
            sendNote(debug,kifParser,'File Loaded',PLocation),
            file_close(OUTPUT),
            file_close(INPUT),
            consult_as_dynamic(PLocation).

% ===================================================================
%  Export Surface Forms as KIF (Agent a Context and Context)
% ===================================================================


export_kif_from_source(Context,DestFile):-
         export_kif_from_source(Context,DestFile,full_cmt).

export_kif_from_source(Context,DestFile):-
         export_kif_from_source(Context,DestFile,full_cmt).

export_kif_from_source(Context,DestFile,Fmt):-
         add_file_extension(".kif",DestFile,PLocation),
         safe_file_open(PLocation,'w',OUTPUT),
         sendNote(debug,kifParser,'Saving kif from Prolog.',[saving,to,PLocation]),
         export_kif_from_source_0(Fmt,Context,OUTPUT).

export_kif_from_source_0(Format,Context,OUTPUT):-
         get_store(forall,surface,(Surface:Vars),Context,TN,Maintainer),
         unnumbervars((Surface:Vars),(USurface:UVars)),
         toMarkup(kif,USurface,UVars,Chars),
         ( Format=full_cmt -> say(OUTPUT,'\n; Context:~q  Ctx:~q  TN:~q  Auth:~q\n~s\n',[Context,TN,Maintainer,Chars]);
         ( Format=terse -> say(OUTPUT,'\n~s\n',[Chars]);
         ( Format=pnx_nf -> say(OUTPUT,'\n( pnx_nf ~s ~w ''~w'' )\n',[Chars,Ctx,TN])))),
         fail.

export_kif_from_source_0(Format,Context,OUTPUT):-!.

% ===================================================================


agent_load_kif_quiet(Filename,GlobalContext,Agent):-
        agent_load_kif_surface(Filename,Context,Agent,AX,quiet).

agent_load_kif(Filename,Ctx,Agent):-
        agent_load_kif_surface(Filename,Context,Agent,AX,loud).


agent_load_kif_surface(Filename,Context,Agent,AX,Verbose):-
        ignore(Agent='Automation'),
        (unsetMooOption(Agent,opt_surface_check=_)),
        (setMooOption(Agent,opt_surface_check=trusted)),
         get_default_assertion_context(DCtx), !,ignore((Ctx=DCtx)),!,
         getMooOption(Agent,opt_theory,DContext), !,  ignore((Context=DContext)),!,
        idGen(TN1),
        idGen(TN2),
        idGen(TN3),
        assertaClean(mooCache(PredR,surface,'instance'(Context,'KnowledgeBase'),'$VAR'(0),'MooKernel','GlobalContext',TN1,Agent,gaf)),
        assertaClean(mooCache(PredR,surface,'instance'('GlobalContext','Context'),'$VAR'(0),Context,'GlobalContext',TN2,Agent,gaf)),
        assertaClean(mooCache(PredR,surface,'sourcefile-of'(Context,Filename),'$VAR'(0),'MooKernel','GlobalContext',TN3,Agent,gaf)),
        say(user_error,'% Reading In ~w to ~w with a default context of ~w \n',[Filename,Context,Ctx]),
        flag('Axioms Compiled',_,0),
        safe_file_open(Filename,'r',INPUT),!,
           repeat,
               once((once(readKIF(INPUT,Chars)),
                  % say(Chars),
                    %catch(say(OUTPUT,'\n% ~s  ',[Chars]),_,true),
                    tell_retract_parse_chars(Chars,PROLOG,Vars),
                     once((
                           PROLOG=comment(_)
                            ;

                          remember_ado(PROLOG,Vars,Context,Agent,Verbose)
                            )))),
                     at_end_of_stream(INPUT), !,
        close(INPUT),
        flag('Axioms Compiled',AX,AX),
        say(user_error,'% Loaded ~w axioms.\n',[AX]),
        ignore(retract(findings(CPU,RESULT))),
        ignore(findall(T,retract(title(T)),Title)),!.

remember_ado(Surface,Vars,Context,Agent,quiet):-!,
        flag('Axioms Compiled',AX,AX+1),
        once(invokeInsert([untrusted,canonicalize],surface,Surface,Ctx,TN,Context,Vars,Agent)).

remember_ado(Surface,Vars,Context,Agent,_):-!,
        flag('Axioms Compiled',AX,AX+1),
        once(invokeInsert([untrusted,canonicalize],surface,Surface,Ctx,TN,Context,Vars,Agent)),
        ignore((writeObject(formula(Surface),Vars))),nl.

agentSave(FileName,Ctx):-
        tell(FileName),
%       say((':-include(\'logicmoo_utils_header.P\').\n'),[]),
        say(':-multifile(mooCache/6).\n',[]),
        say(':-multifile(mooCache/9).\n',[]),
        say(':-multifile(mooCache/5).\n',[]),
        say(':-retractall(mooCache(PredR,_,_,_,_,~q,_,_,_)).\n',[Ctx]),
        say(':-index(mooCache(1 ,0,1,0,0,1,0,0,1)).\n',[]),
        save_each_assertion_of(Ctx),
        told.


save_each_assertion_of(Ctx):-mooCache(PredR,Form,Surface,Vars,Context,EXTID,Agent,Status),
                say('~q.\n',[mooCache(PredR,Form,Surface,Vars,Context,EXTID,Agent,Status)]),fail.
save_each_assertion_of(Ctx):-!.

agentLoad(Filename) :-ensure_loaded(Filename).


% fguard(Template,RH,NVars,Explaination,Functor)
fguard(RH,NVars,surf(Context,TN,CID,[]),Functor):-!.

      %  not(recorded(Functor,Template,FRef)),
        %unify_with_occurs_check(Template,RH).


%guard(instance,instance(v(_G238, _G238, _G240), v('Abstract', _G238, ['Class'|_G247])):instance(v(_G238, _G238, _G240), v('Abstract', _G238, ['Class'|_G247])), [_G238, _G238, _G240, _G238, _G247]:[_G238, _G238, _G240, _G238, _G247], (subclass(v('Abstract', _G238, ['Class'|_G327]), v('Abstract', _G238, ['Class'|_G247])), instance(v(_G238, _G238, _G240), v('Abstract', _G238, ['Class'|_G327]))): (subclass(v('Abstract', _G238, ['Class'|_G361]), v('Abstract', _G238, ['Class'|_G247])), instance(v(_G238, _G238, _G240), v('Abstract', _G238, ['C
/*
guard(instance,
instance(v(_G235, _G236, _G237), v('Abstract', _G240, ['Class'|_G244])):instance(v(_G266, _G267, _G268), v('Abstract', _G271, ['Class'|_G275])),
 [_G235, _G236, _G237, _G240, _G244]:[_G266, _G267, _G268, _G271, _G275],
 (subclass(v('Abstract', _G317, ['Class'|_G321]), v('Abstract', _G271, ['Class'|_G275])),instance(v(_G266, _G267, _G268), v('Abstract', _G317, ['Class'|_G321]))),
  188, 3, ['SUBCLASS'=_G317, 'CLASS'=_G271, 'INST'=_G267], [_G267, _G271, _G317], [], [], [_G317], [_G267, _G271])
*/

                                         /*
guard(Functor,RealHead:Head,RFVH:FVH,Body,TN,CID,KRVars,RuleVars,UnivHead,BodyUniv,BodySelfConnected,RealShared):-
        unify_with_occurs_check(RFVH,FVH), %unifies list of 'real' prolog variables
        recorded(TN,KRVars,Ref),!,
        writeDebugFast(bumping(KRVars)),!,
        catch(exit(Ref,cut),_,fail).
        %(catch(fail(Ref),_,(writeDebugFast(missing(Copy)),fail)),fail),write(bollk),nl.
%       catch(exit(Ref,previousCallMoreSpecific(KRVars,Copy)),_,fail),fail.
                                           */

'$existential'(v(_,V,_),A,F):-ignore(V=F).


guard(Functor,RealHead:Head,RFVH:FVH,Body,TN,CID,KRVars,RuleVars,UnivHead,BodyUniv,BodySelfConnected,RealShared):-!,


%        not(Functor=not(_)),
        not(recorded(TN,KRVars,Ref)),
        %ground(UnivHead), % Makes sure this is valid
        unify_with_occurs_check(RFVH,FVH), %unifies list of 'real' prolog variables
        copy_term(KRVars,Session),
        numbervars(Session,'$VAR',0,_),
        recorda(TN,Session,Ref),
     %   recorda(Functor,RealHead,FRef),
        %stepQualifier(Ref,Functor,RealHead:Head,RFVH:FVH,Body,TN,CID,KRVars,RuleVars,UnivHead,BodyUniv,BodySelfConnected,RealShared,Qualifier),
%findall(Body,call_with_depth_limit(block(Ref,callBody(Body,true),Result),47,_),Sols),!,
        findall(Body,call_with_depth_limit(Body,47,_),Sols),!,
        %findall(Body,Body,Sols),!,
        sort(Sols,SolsS),
        member(Body,SolsS).


        %(Result=cut -> !,),
        %catch(erase(Ref),_,writeDebugFast(missingRef(Vars))).
%       processResult(Result,TN,CID,F,KRVars,RB,Session,Ref),!,
%       ground(RealShared).

%processResult(Result,TN,CID,F,Vars,RB,Session,Ref):-var(Result),!, % normal completion

%processResult(previousCallMoreSpecific(Vars,Copy),TN,CID,F,KRVars,RB,Session,Ref):- % decendant aborted to here
        %catch(erase(Ref),_,writeDebugFast(missingRefInpreviousCallMoreSpecific(KRVars))),
 %       writeDebugFast(previousCallMoreSpecific(KRVars,Vars,Copy)). %,!,fail.



stepQualifier(Ref,Functor,RealHead:Head,RFVH:FVH,Body,TN,CID,
        KRVars,RuleVars,UnivHead,[BodY|Univ],BodySelfConnected,RealShared,(ground(BodY),exit(Ref,bodyUnivBound))).


stepQualifier(Ref,Functor,RealHead:Head,RFVH:FVH,Body,TN,CID,
        KRVars,RuleVars,UnivHead,BodYUniv,BodySelfConnected,RealShared,true).

%callBody(Body,true):-!,Body.
callBody((A,B),Qualifier):-
        callBody(A,Qualifier),Qualifier,
        callBody(B,Qualifier).
callBody(Body,Qualifier):-not(functor(Body,function,_)),Body.

unguard(TN,F,Vars,Session):-
        recorded(TN,Session,Ref),!,erase(Ref),!.

unguard(TN,F,Vars,Session):-
        writeDebugFast(somethingKilled(Session)).


mooCall(X):-
        mooCache(X, Cost,Context, Ctx,surf(Context,TN,CID,[])).

mooCall(X):-
        mooCache(X, Cost,Context, Ctx,surf(Context,TN,CID,[])).


mooCall(Flags,Context):-
        mooCache(Cons, Ante,Vars,Context, Ctx,TN).

guard(Functor,RealHead:Head,RFVH:FVH,Body,TN,CID,KRVars,RuleVars,UnivHead,BodyUniv,BodySelfConnected,RealShared):-!,
%        not(Functor=not(_)),
        not(recorded(TN,KRVars,Ref)),
        %ground(UnivHead), % Makes sure this is valid
        unify_with_occurs_check(RFVH,FVH), %unifies list of 'real' prolog variables
        copy_term(KRVars,Session),
        numbervars(Session,'$VAR',0,_),
        recorda(TN,Session,Ref),
     %   recorda(Functor,RealHead,FRef),
        %stepQualifier(Ref,Functor,RealHead:Head,RFVH:FVH,Body,TN,CID,KRVars,RuleVars,UnivHead,BodyUniv,BodySelfConnected,RealShared,Qualifier),
%findall(Body,call_with_depth_limit(block(Ref,callBody(Body,true),Result),47,_),Sols),!,
        findall(Body,call_with_depth_limit(Body,47,_),Sols),!,
        %findall(Body,Body,Sols),!,
        sort(Sols,SolsS),
        member(Body,SolsS).


/*

        assertz((term_expansion(X,Y) :- catch((!,getTermExpansionLogged(X,Y)),E,fail))),
        told.
*/




:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).


ca:-compile_show('LogicMOO',instance,2,Debug).

cf:- compile_to_file(instance,2,'LogicMOO').


va:- compile_show('LogicMOO',valence,2,Debug).

ensure_all_compiled:-!.

ensure_all_compiled:-
        getAllMooContext(X),
        compileContext(X),fail.


getAllMooContext(X):-fail.

compileContext(Context):-!.
compileContext(Context):-
        compileInstanceSubclass(Context).



make_theory(Context):-
        retractall(mooCache(Context,_,_)),
        atom_concat(Context,'.prolog',PrologFile),
        tell(PrologFile),
        format('

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

        '),
        image_to_prolog(Context),!,
        told,
        save_make_theory(PrologFile,Context,FeatureFile),
        atom_concat(Context,'.pl',OutputFile),
        concat_atom([cat,FeatureFile,PrologFile,'>',OutputFile],' ',Cmd),
        format('~n~w~n',[Cmd]).


save_make_theory(PrologFile,Context,FeatureFile):-
        atom_concat(Context,'.feature',FeatureFile),
        tell(FeatureFile),
        format('

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

        '),
        save_features_theory(Context),
        told.

save_features_theory(Context):-
        format('~n~n% Predicates~n~n'),
        mooCache(Context,type(dynamic),Data),
        format(':-~q.~n',[dynamic(Data)]),fail.

save_features_theory(Context):-
        format('~n~n% Predicates~n~n'),
        mooCache(Context,type(dynamic),Data),
        format(':-~q.~n',[tabled(Data)]),fail.

/*
save_features_theory(Context):-
        format('~n~n% Tables~n~n'),
        mooCache(Context,type(tabled),Data),
        format(':-~q.~n',[tabled(Data)]),fail.
  */

/*
save_features_theory(Context):-
        format('~n~n% Not Tabled ~n~n'),
        mooCache(Context,type(dynamic),Data),
        not(mooCache(Context,type(tabled),Data)),
        format(':-~q.~n',[prolog(Data)]),fail.
*/

save_features_theory(Context):-
        format('~n~n% Rules~n~n'),
        mooCache(Context,type(rule),Data),
        format('~q.~n',[rules_for(Data)]),fail.

save_features_theory(Context):-
        format('~n~n% No Rules For~n~n'),
        mooCache(Context,type(dynamic),Data),
        not(mooCache(Context,type(rule),Data)),
        format('~q.~n',[no_rules_for(Data)]),fail.

save_features_theory(Context):-
        format('~n~n% Facts~n~n'),
        mooCache(Context,type(fact),Data),
        format('~q.~n',[facts_for(Data)]),fail.

save_features_theory(Context):-
        format('~n~n% No Facts~n~n'),
        mooCache(Context,type(dynamic),Data),
        not(mooCache(Context,type(fact),Data)),
        format('~q.~n',[no_facts_for(Data)]),fail.

save_features_theory(Context):-
        format('~n~n% No Rules/Facts~n~n'),
        mooCache(Context,type(dynamic),Data),
        not(mooCache(Context,type(fact),Data)),
        not(mooCache(Context,type(rule),Data)),
        format('~q.~n',[no_assertions_for(Data)]),fail.

save_features_theory(Context):-
        format('~n~n% Lemma Reqs:~n~n'),
        findall((F-C),mooCache(Context,type(F/A),(C/N)),Edges),
        keysort(Edges,Sorted),
        format('lemma_edges(~q).~n~n',[Sorted]),!.


image_to_prolog:-
        tell('LogicMOO.pl'),
        image_to_prolog(Context),
        told.


image_to_prolog(Context):-
        hardcoded(HardCoded),
        image_to_prolog([holds,neg(lit)|HardCoded],Context).

image_to_prolog(Flags,Context):-format(
'
:- op(400,fy,~~).
%:- op(500,xfy,:).
:- op(500,xfx,#).
%:- op(500,xfx,@).

:- op(400,fy,not).    % negation
:- op(500,xfy,and).   % conjunction
:- op(600,xfy,or).   % disjunction
%:- op(500,xfy,:).
:- op(0,xfx, equal ).
:- op(900,xfx,''<='').
:- op(900,xfx,if).
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

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

:-set_prolog_flag(unknown,fail).

'),fail.





/*
delayBody(TN,(B1,B2)):-
        delayBody(TN,B1),
        delayBody(TN,B2).

delayBody(TN,B2):-
        functor(B2,F,_),
        recorda(F,delayed).
*/


mcl1:-mcl(instance(X,Y)).

mcc(X):-
        resetTableFlags,X.


mcl2:-
        mcl(X,instance(v(_,X,_),v('Abstract', 'BinaryRelation',_))).



mcl(X):-
        resetTableFlags,
       findall(X,(X,writeq(X),nl),L),
       writeq(L),nl,
       length(L,N),
       write(num:N),nl.

mcl(X,Y):-
        resetTableFlags,
       findall(X,(Y,writeq(Y),nl),L),
       writeq(L),nl,
       length(L,N),
       write(num:N),nl.





image_to_prolog(Flags,Context):-
        mooCache(Cons, Cost,Context, Ctx,Explaination),
                convertPrologWFS(Flags,Context,Activation,Explaination,Cons,Prolog),
        writeAsProlog(Prolog),fail.


image_to_prolog(Flags,Context):-
        mooCache(Cons, Ante,Cost,Context, Ctx,Explaination),
        convertPrologWFS(Flags,Context,Activation,Explaination,(Cons:-Ante),Prolog),
        writeAsProlog(Prolog),fail.



image_to_prolog(Flags,Context):-!.



tabled_consult(File):-
        tell('tabling.log'),
        assertz((term_expansion(X,Y) :- catch((!,getTermExpansionLogged(X,Y)),E,fail))),
        consult(File),
        abolish(term_expansion/2),
        dynamic(term_expansion/2),
        multifile(term_expansion/2),!,
        told.

/*
ttheory(Context):-
        tell('tabling.log'),
        mooCache(Cons, Cost,Context, Ctx,Explaination),
        assertClauseTable(Context,Cons,Explaination),fail.

ttheory(Context):-
        mooCache(Cons, Ante,Cost,Context, Ctx,Explaination),
        assertClauseTable(Context,(Cons:-Ante),Explaination),fail.

ttheory(Context):-told.

*/

assertClauseTable(Context,WFS,Explaination):-
        convertPrologWFS(Flags,Context,assertClauseTable,Explaination,WFS,Prolog),
        getTermExpansionLogged(Prolog,Tabled),
        assertAll(Tabled),!.



tconsult(File):-
        tell('tabling.log'),
        see(File),
        repeat,
                read(X),
                catch((!,getTermExpansionLogged(X,Y)),E,(writeq(E),nl,fail)),
                once(assertAll(Y)),
           X==end_of_file,
        seen,
        told.

assertClauseTable(Context,WFS,Explaination):-
        convertPrologWFS(Flags,Context,assertClauseTable,Explaination,WFS,Prolog),
        getTermExpansionLogged(Prolog,Tabled),
        assertAll(Tabled).


getTermExpansionLogged(end_of_file,end_of_file):-!.
getTermExpansionLogged(X,Y):-
        slg_term_expansion(X,Y),
        log_term_expansion(X,Y).

log_term_expansion(X,Y):-X==Y,!,
        format('\n%no_expansion\n'),write_te_list(Y).
log_term_expansion(X,Y):-
        format('\n%~q~n',[X]),write_te_list(Y).

write_te_list([]):-nl,!.
write_te_list([X|T]):-!,write_te_list(X),write_te_list(Y).
write_te_list(X):-format('~q.\n',X).


convertPrologWFS(Flags,Context,Activation,surf(Context,TN,CID,KRVars),
        (C :- A),
        ((
        RealHead :-
                guard(Functor,(RealHead:Head),(RFVH:FVH),Body,TN,CID,KRVars,
                RuleVars,UnivHead,BodyUniv,BodySelfConnected,Shared)))):-!,
        functor(C,F,_),convertNegations((not),F,Functor,_),
        convertRuleHeadWFS(Flags,Context,Activation,C,Head,RuleHead),
        convertRuleBodyWFS(Flags,Context,Activation,RuleHead,A,Body),!,
        getPrologVars(KRVars,RuleVars,_,_),!,
        getPrologVars(Head,FVH,_,_),!,set_partition(RuleVars,FVH,_,_,HeadVars),
        getPrologVars(Body,FVB,BSingles,_),!,set_partition(RuleVars,FVB,_,_,BodyVars),
        set_partition(HeadVars,BodyVars,PrivHead,PrivBody,Shared),!,
        set_partition(PrivBody,BSingles,BodySelfConnected,_,BodyUniv),!,
        copy_term((Head,FVH,PrivHead),(RealHead,RFVH,UnivHead)).

convertPrologWFS(Flags,Context,Activation,Explaination,(C),(RH:- fguard(RH,NVars,Explaination,Functor))):-!,
        functor(C,F,_),convertNegations((not),F,Functor,_),
        convertFactHeadWFS(Flags,Context,Activation,C,RH),
        functor(RH,F,A),functor(Template,F,A),Template=..[F|NVars].



writeAsProlog([]):-format('\n\n'),!.
writeAsProlog([H]):-format('\n\t~q.~n',[C]),!.
writeAsProlog(':-'(Cons, Ante)):-format('\n ~q :-',[Cons]),writeAsProlog(Ante),!.
writeAsProlog('<-'(Cons,Ante)):-format('\n ~q <-',[Cons]),writeAsProlog(Ante),!.
writeAsProlog('<--'(Cons,Ante)):-format('\n ~q <--',[Cons]),writeAsProlog(Ante),!.
writeAsProlog('::-'(Cons,Ante)):-format('\n ~q ::-',[Cons]),writeAsProlog(Ante),!.
writeAsProlog((H,T)):-format('\n\t~q,',[H]),writeAsProlog(T),!.
writeAsProlog([H|T]):-format('\n\t~q,',[H]),writeAsProlog(T),!.
writeAsProlog(C):-format('\n\t ~q.~n',[C]),!.

recordIfNew(Activation,Context,Cons,Type):-!.

recordIfNew(Activation,Context,Cons,Type):-
        atom(Type),!,
        functor(Cons,Pred,Arity),
        recordIfNewCache(Context,Type,Pred/Arity),!.
recordIfNew(Activation,Context,Cons,Type):-
        functor(Type,T,A),
        functor(Cons,Pred,Arity),
        recordIfNewCache(Context,(T/A),(Pred/Arity)).

recordIfNewCache(Context,Type,Data):-
        mooCache(Context,type(Type),Data),!.
recordIfNewCache(Context,Type,Data):-
        assertz(mooCache(Context,type(Type),Data)),!.



/*

Subclasses of Physical

        Object
                SelfConnectedObject
                Collection
                ContentBearingObject
        Process

Subclasses of Abstract

         Quantity
                Number (how many)
                PhysicalQuantity (how much)
         Attribute
         Proposition
         Class
                Set
         Relation
                Predicate
                Function
               VariableArityRelation

   Other Disjoint CLasses like the new 'Graph'



*/





/*
compile_to_file(STANDARDPred,Arity,Context):-
        make_relation_profile(Context,STANDARDPred,Logic,Arity,N,Module,SourceInfo,Functor),
        open(SourceInfo, write, Stream, [buffer(full),type(text),alias(STANDARDPred)]),
        compile_show(STANDARDPred,Context,STANDARDPred,Arity,Debug),
        close(STANDARDPred).
*/
compile_to_file(STANDARDPred,Arity,Context):-
        make_relation_profile(Context,STANDARDPred,Logic,Arity,N,Module,SourceInfo,Functor),!,
        open(SourceInfo, write, Stream, [buffer(full),type(text),alias(STANDARDPred)]),
        compile_show(Stream,Context,STANDARDPred,Arity,Debug),!,
        close(Stream),!.

compile_show(Context,STANDARDPred,Arity,Debug):-
        current_output(Stream),!,
        compile_show(Stream,Context,STANDARDPred,Arity,Debug),!.


compile_show(Stream,Context,STANDARDPred,Arity,Debug):-
        ignore(Debug=no_debug),!,
        make_relation_profile(Context,STANDARDPred,Logic,Arity,N,Module,SourceInfo,Functor),
        mkImported(full,Functor,Arity,Logic,Vect,Ctx,ExplainationIn,Explaination,Imported),
        mkArgsAtom(Arity,ArgsAtom),
        mkHolds(STANDARDPred,Arity,Vect,Cons),
        mkIndex(Functor,N,Index),
        Dash = (Functor/N),
        format(Stream,'
/* <PRE face="Arial,Helvetica"><font face="Arial,helvetica"><font color=green>
File: "~w"

Maintainer:      dmiles@users.sourceforge.net [Douglas R. Miles]

Purpose:  Individual loading of Moo Context Predicates.

Exported:
        explaination_line/2,                                                                    % ExplainationIn of Explaination Formats
        full_~w(Logic, ~w, Ctx, ExplainationIn, Explaination),                                   % Basic Access
        bk_~w(Logic, ~w, Ctx,ExplainationIn, Explaination),                    % Backward Chain
        fw_~w(Logic, ~w, Ctx,ExplainationIn,Explaination),                % Forward Chain
        gaf_~w(Logic, ~w, Ctx,ExplainationIn,Explaination),                % Forward Chain

Uses: deduceTransitiveClosure_PartialOrderingRelation/6, fw_instance/5, fw/6.

Type of SourceInfo: predicate_module (generated runtime)
Module:   ~q
STANDARD:   ~q
Cons:   ~q
Context:   ~q
Debug:  ~q
</font>
*/

:-module(~q,
       [
        full_~w,
        bk_~w,
        gaf_~w,
        fw_~w
        ]).

:-include(\'moo_multifile.P\').

% =====================================================
% belief_module(Context,STANDARDPred,Cons,Imported,ConnectionType,SourceInfo,HowOften).
%       Context = The KnowedgeBase
%       Ctx = The Context
%       STANDARDPred = The Functor\'s STANDARD Name
%       Goal = Functors STANDARD Prototype       "goal(Logic,ExplainationIn,holds(STANDARDPred,A,B),Ctx,Context,ExplainationOut)"
%       Imported = Functor\'s Prolog Prototype
%       ConnectionType = \'prolog\'  meaning its compiled by consulting this file to memory.
%       SourceInfo = The connection parameters
%       HowOften = always,never,once
% =====================================================

:-index(belief_module(1,0,1,1,1,0,0,0)).


belief_module(
        ~q,_AllContexts,
        ~q,goal(Logic,ExplainationIn,holds(~q,~w),Ctx,~q,ExplainationOut),
        full_~w(Logic, ~w, Ctx, ExplainationIn, ExplainationOut),
        prolog, ~q,
        always).


% =====================================================
% index/dynamic All exported predicates
% =====================================================

:-index(full_~w).
:-index(fw_~w).
:-index(bk_~w).
:-index(gaf_~w).

:-dynamic(full_~w).
:-dynamic(fw_~w).
:-dynamic(bk_~w).
:-dynamic(gaf_~w).

',     [SourceInfo, % Comment SourceInfoname
        Functor,ArgsAtom,  % Comment full_Functor
        Functor,ArgsAtom,  % Comment bk_Functor
        Functor,ArgsAtom,  % Comment fw_Functor
        Functor,ArgsAtom,  % Comment gaf_Functor
        Module,STANDARDPred,Cons,Context,Debug, % Comments
        Module,Dash,Dash,Dash,Dash,  % module/2
        Context,STANDARDPred,STANDARDPred,ArgsAtom,Context,Functor,ArgsAtom,SourceInfo, % belief_module/5
        Index,Index,Index,Index,    % index/1
        Dash,Dash,Dash,Dash      % dynamic/1
        ]),!,
        create_entry_points(Stream,non_singleValued,Functor,Context,STANDARDPred,Arity,N,Module,Debug,ArgsAtom),!,
        make_pred_data(Stream,Functor,Context,STANDARDPred,Arity,N,Module,Debug,ArgsAtom),!.

mkArgsAtom(Arity,ArgsAtom):-
        length(Arglist,Arity),
        numbervars(Arglist,'$VAR',0,_),
        term_to_atom(Arglist,ArgListAtom),
        atom_codes(ArgListAtom,[_|ArgListAtomCodesRight]),
        append(ArgListAtomCodes,[93],ArgListAtomCodesRight),
        atom_codes(ArgsAtom,ArgListAtomCodes).

mkHolds(STANDARDPred,Arity,Vect,Cons):-
        Cons=..[holds,STANDARDPred|Vect],!.

mkImported(full,Module,Arity,Logic,Vect,Ctx,ExplainationIn,Explaination,Imported):-
        length(Vect,Arity),
        append([Module,Logic|Vect],[Ctx,ExplainationIn,ExplainationIn * Explaination],STANDARDPredList),
        Imported =.. STANDARDPredList,!.

mkIndex(STANDARDPred,Arity,Index):-
        interate_copy(Arity,IndexArgs),
        Index=..[STANDARDPred|IndexArgs],!.

interate_copy(N,[1,1,1,1|IndexArgs]):-
        NN is N - 4,
        length(IndexArgs,NN),
        put_in_n(NN,0,IndexArgs),!.

put_in_n(NN,Value,[]).
put_in_n(NN,Value,[Value|L]):-put_in_n(NN,Value,L).


create_head(Tag,Context,STANDARDPred,Logic,Args,Head):-
        concat_atom([Tag,Context,'_',STANDARDPred],Functor),
        Head=..[Functor|Args],!.


% ===============================================================
% make_relation_profile(-Context,-STANDARDPred,-Logic,-Arity,+N,+Module,+SourceInfo,+Functor)
% ===============================================================
make_relation_profile(Context,STANDARDPred,Logic,Arity,N,Module,SourceInfo,Functor):-!,
        ignore(Context='LogicMOO'),
        ignore(STANDARDPred='attribute'),
        ignore(Arity=2),
        is(N,(Arity + 4)),
        concat_atom([Context,'_',STANDARDPred],Module),
        concat_atom([Module,Arity],Functor),
        concat_atom(['pred_', Module, '.pl' ],SourceInfo),!.


create_entry_points(Stream,non_singleValued,Functor,Context,STANDARDPred,Arity,N,Module,Debug,ArgsAtom):-
format(Stream,'
% ==================================================================================
% <B>Entry Points</B> for <font color=red>non_singleValued</font>   <font size=+1 color=green>~w/~w</font>
% ==================================================================================

% gaf hook
full_~w(Logic,~w, Ctx,ExplainationIn,(Explaination * ExplainationIn)):-
        gaf_~w(Logic,~w ,Ctx,ExplainationIn,Explaination),not_in(Explaination,ExplainationIn).

% fw hook
full_~w(Logic,~w, Ctx,ExplainationIn,(Explaination * ExplainationIn)):-
        fw_~w(Logic,~w ,Ctx,ExplainationIn,Explaination),not_in(Explaination,ExplainationIn).

% bk hook
full_~w(Logic,~w, Ctx,ExplainationIn,Explaination):-
        bk_~w(Logic,~w ,Ctx,ExplainationIn,Explaination).

',     [STANDARDPred,Arity, %Comment
        Functor,ArgsAtom,Functor,ArgsAtom,
        Functor,ArgsAtom,Functor,ArgsAtom,
        Functor,ArgsAtom,Functor,ArgsAtom
        ]),!. % never fails

make_pred_data(Stream,Functor,Context,STANDARDPred,Arity,N,Module,Debug,ArgsAtom):-
format(Stream,'

% ================================================================================================================
% <B>Rules/Facts</B> for <font size=+1 color=green>~w/~w</font>
% ================================================================================================================<B>

',[STANDARDPred,Arity]),once((flag(rule_num,_,1),flag(explaination_linenumber,_,1))),fail.

write_rulenum(Stream,Explaination):-!,
        flag(rule_num,RN,RN+1),flag(explaination_linenumber,_,1),!,
        format(Stream,'~n/*Id: ~q */~n',[Explaination]),!.

make_head_t(true,STANDARDPred,N,Cons):-
                length(Args,N),
                Cons=..[STANDARDPred|Args],!.

make_head_t(false,STANDARDPred,N,Cons):-
                length(Args,N),
                atom_concat('~',STANDARDPred,STANDARDPredN),
                Cons=..[STANDARDPredN|Args],!.


getrule(STANDARDPred,Arity,Cons,Precond, Context, surf(Context,TN,CID),Vars):-
        make_head_t(true,STANDARDPred,Arity,Cons),
        mooCache(Cons, A1,A2,A3, Cost,Context, Ctx,surf(Context,TN,CID,Vars)),
        once((append(A1,A2,AM),
        append(AM,A3,Precond))).
getrule(STANDARDPred,Arity,Cons,Precond, Context, surf(Context,TN,CID),Vars):-
        make_head_t(false,STANDARDPred,Arity,Cons),
        mooCache(Cons, A1,A2,A3, Cost,Context, Ctx,surf(Context,TN,CID,Vars)),
        once((append(A1,A2,AM),
        append(AM,A3,Precond))).

% True GAFS
make_pred_data(Stream,Functor,Context,STANDARDPred,Arity,N,Module,Debug,ArgsAtom):-
        make_head_t(true,STANDARDPred,Arity,Cons),
        mooCache(Cons, Precon, Context, TID),
        write_rulenum(Stream,TID),
        numbervars((Cons, Precon, Context, TID,Vars),'$VAR',15,_),
        submit_ado_cache(Stream,STANDARDPred,Cons, Precon, Context, TID,Vars),fail.

% False GAFS
make_pred_data(Stream,Functor,Context,STANDARDPred,Arity,N,Module,Debug,ArgsAtom):-
        make_head_t(false,STANDARDPred,Arity,Cons),
        mooCache(Cons, Precon, Context, TID),
        write_rulenum(Stream,TID),
        numbervars((Cons, Precon, Context, TID,Vars),'$VAR',15,_),
        submit_ado_cache(Stream,STANDARDPred,Cons, Precon, Context, TID,Vars),fail.

% True then Fasle Rules
make_pred_data(Stream,Functor,Context,STANDARDPred,Arity,N,Module,Debug,ArgsAtom):- %true,
        getrule(STANDARDPred,Arity,Cons,Precond, Context, TID,Vars), %true,
        write_rulenum(Stream,TID),
        close_list(Vars),
        numbervars((Stream,STANDARDPred,Cons,Precond, Context, TID,Vars),'$VAR',15,_),
        submit_ado_cache(Stream,STANDARDPred,Cons,Precond, Context, TID,Vars),fail.

       % toMarkup(kif,DispExplaination,Vars,PrettyForm),
        %logOnFailure(format(Stream,'/*~n</B><font color=green>~n~nForms:~n~n~s~nFlags: ~w \n</font><B>*/',[PrettyForm,Flags])),
%       make_disp_explaination(Logic,TID,Vars,Cons,Ante,DispExplaination),
%       format(Stream,'~q. ~n~n',[explaination_line(TID,Vars,DispExplaination)])         %true,


% Write footer
make_pred_data(Stream,Functor,Context,STANDARDPred,Arity,N,Module,Debug,ArgsAtom):-
        getPrettyDateTime(String),
        format(Stream,'\n/* \n Last Saved: ~s</B></font>\n</PRE>*/\n\n\n',[String]),!.


%submit_ado_cache(Stream,STANDARDPred,Cons,_,_,Logic, Context, Ctx, Explaination * _):-true,fail.


% No antecedents flags
submit_ado_cache(Stream,STANDARDPred,Cons, [], Context, TID,Vars):-
        Cons=..[_|Arguments],
        append(Arguments,[Ctx,TID],Args),  !,
        create_head('gaf_',Context,STANDARDPred,true,Args,PrologHead),
        format(Stream,'~n~q.~n',[PrologHead]),!.

% With antecedents flags
submit_ado_cache(Stream,STANDARDPred,Cons,Ante, Context, TID,Vars):-
        Cons=..[_|Arguments],
        numbervars(SubCtx),
        append(Arguments,[Ctx,ExplainationIn,[explaination_line(TID,VarsRef)|ExplainationOut]],Args),
        create_head('bk_',Context,STANDARDPred,true,Args,PrologHead),
        format(Stream,'~n ~q :- \n',[PrologHead]),!,
        %format(Stream,'\t~q,~n',[not_near_member(TID,ExplainationIn)]), !,
      %  findall(Neg,member(-(Neg),Ante),NegS),
        % findall(Pos,member(+(Pos),Ante),PosS),
        %format(Stream,'\t~q,~n',[minor_interception(PosS,NegS,ExplainationIn,ExplainationMid)]),
        %write_std_flags(Stream,Context,Flags,(Cons:Ante)),!,
        format(Stream,'\t~q,~n',[subcontext(Ctx,SubCtx)]),
        write_prolog_body_clause(Stream,Ante,STANDARDPred,Cons,Ante,Context, SubCtx, TID,Vars,ExplainationIn,ExplainationOut),!,
        format(Stream,'\t~q.~n~n',[(ground(Vars),VarsRef=Vars)]),!.

write_std_flags(Stream,Context,Flags,Term):-
        write_lllist(Stream,Context,Flags),!.

write_lllist(Stream,Context,[]):-!.
write_lllist(Stream,Context,[Arg|List]):-
        format(Stream,'\tk~w_~q,~n',[Context,Arg]),
        write_lllist(Stream,Context,List),!.


/*

submit_ado_cache(Stream,STANDARDPred,Cons,Ante,Flags,Logic, Context, Ctx, TID * Vars):-
        write_prolog_rule(Stream,STANDARDPred,Cons,Ante,Flags,Logic, Context, Ctx, TID,Vars),!.

write_prolog_rule(Stream,STANDARDPred,Cons,Ante,Flags,Logic, Context, Ctx, TID,Vars):-
        format(Stream,' ~q. ~n',[explaination_line(TID,Vars,DispExplaination)]),
        Cons=..[_|Arguments],
        append(Arguments,[Ctx,ExplainationIn,(ExplainationOut * TID,Vars)],Args),
        create_head('bachchain_',Context,STANDARDPred,Logic,Args,PrologHead),
        format(Stream,' ~q :- ~n',[PrologHead]),
        write_prolog_body_start(Stream,Ante,STANDARDPred,Cons,Ante,Flags,Logic, Context, Ctx, TID,Vars,ExplainationIn,ExplainationMid),!,
        write_prolog_body_clause(Stream,Ante,STANDARDPred,Cons,Ante,Flags,Logic, Context, Ctx, TID,Vars,ExplainationMid,ExplainationOut),!.

*/
%       write_prolog_body_clause(Stream,Ante,STANDARDPred,Cons,Ante,Flags,Logic, Context, Ctx, TID,Vars,ExplainationMid,ExplainationOut),!.

write_prolog_body_clause(Stream,OrigAnte,STANDARDPred,Cons,[], Context, Ctx, TID,Vars,ExplainationIn,ExplainationOut):-!,
        format('\t~w,~n',[ExplainationIn=ExplainationOut]).

write_prolog_body_clause(Stream,OrigAnte,STANDARDPred,Cons,[Ante], Context, Ctx, TID,Vars,ExplainationIn,ExplainationOut):-!,%true,
        write_prolog_term(Stream,OrigAnte,Context,Flags,Caller,FlagsSTANDARDPred,Cons,Ante,Context, Ctx, TID,Vars,ExplainationIn,ExplainationOut),!.

write_prolog_body_clause(Stream,OrigAnte,STANDARDPred,Cons,[Ante|More], Context, Ctx, TID,Vars,ExplainationIn,ExplainationOut):-!,%true,
        write_prolog_term(Stream,OrigAnte,Context,Flags,Caller,FlagsSTANDARDPred,Cons,Ante,Context, Ctx, TID,Vars,ExplainationIn,ExplainationMid),
        write_prolog_body_clause(Stream,OrigAnte,STANDARDPred,Cons,More, Context, Ctx, TID,Vars,ExplainationMid,ExplainationOut),!.




check_end_flags(FlagsList,ExplainationIn,ExplainationOut):-ExplainationIn=ExplainationOut.
check_begin_flags(FlagsList,ExplainationIn,ExplainationOut):-ExplainationIn=ExplainationOut.



write_prolog_term(Stream,OrigAnte,Context,Flags,Caller,FlagsSTANDARDPred,Cons,Useless, Context, Ctx, TID,Vars,ExplainationIn,ExplainationIn):- useless(Useless),!.
write_prolog_term(Stream,OrigAnte,Context,Flags,Caller,FlagsSTANDARDPred,Cons,Ante, Context, Ctx, TID,Vars,ExplainationIn,ExplainationOut):-
        Ante=..[P|Arguments],
        append(Arguments,[Ctx,ExplainationIn,ExplainationOut],Args),
        create_head('fw_',Context,P,true,Args,PrologHead),
        format(Stream,'\t~q,~n',[PrologHead]),!.

useless(domainC(_,[])).
useless(domainA(_,[])).



make_disp_explaination(true,surf(Context,TID),Vars,Cons,Conds,via(entails(CondsO,Cons),Vars) * surf(Context,TID)):-fix_conds(Conds,CondsO),!.
make_disp_explaination(false,surf(Context,TID),Vars,Cons,Conds,via(entails(CondsO,not(Cons)),Vars) * surf(Context,TID)):-fix_conds(Conds,CondsO),!.
make_disp_explaination(true,surf(Context,TID,ID),Vars,Cons,Conds,via(entails(CondsO,Cons),Vars) * surf(Context,TID,ID)):-fix_conds(Conds,CondsO),!.
make_disp_explaination(false,surf(Context,TID,ID),Vars,Cons,Conds,via(entails(CondsO,not(Cons)),Vars) * surf(Context,TID,ID)):-fix_conds(Conds,CondsO),!.

fix_conds(Var,Var):-isSlot(Var),!.
fix_conds([],true):-!.
fix_conds([A],AA):-!,
        fix_conds(A,AA).
fix_conds(+Conds,Conds).
fix_conds(-Conds,not(Conds)).
fix_conds([A|B],and(AA,BB)):-!,
        fix_conds(A,AA),
        fix_conds(B,BB).



