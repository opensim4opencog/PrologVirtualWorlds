
:-include('moo_header.pl').

% ===========================================================
% Context From File
% ===========================================================
ctxFromFile(BadCtxName,Filename,KB):-
                catch(atom_codes(BadCtxName,Codes),_,Codes=[]),
                length(Codes,L),L<3,
                file_base_name(Filename,BaseCtxName),
                file_name_extension(CtxName,Extension,BaseCtxName),
                writeFmt('<B color=red>No name was given, so a Context called <font color=green>~w</font> is being created.<p>',[CtxName]),
                load_kif_to_kb_ctx(CtxName,Filename,'GlobalContext','MooWeb').

ctxFromFile(CtxName,Filename,KB):-!,
                idGen(TN1),
                idGen(TN3),
                assertaClean(mooCache(PredR,surface,'instance'(CtxName,'Context'),'$VAR'(0),KB,'GlobalContext',TN1,'WebUser',on)),
                assertaClean(mooCache(PredR,surface,'sourcefile'(CtxName,Filename),'$VAR'(0),KB,'GlobalContext',TN3,'WebUser',on)),
                load_kif_to_kb_ctx(KB,Filename,CtxName,'MooWeb').




load_kif_to_kb_ctx(KB,FileName,Ctx,User):-!,
        ignore(User='MooWeb'),
        assert(telling_file),
%%         atom_concat(FileName,'.compiled.pl',DBG),
%%         tell(DBG),
         get_default_assertion_context(DCtx), !,ignore((Ctx=DCtx)),!,
         getDefaultKB(DKB), !,  ignore((KB=DKB)),!,

         writeFmt('Reading In ~w to ~w with a default context of ~w <p>',[FileName,KB,Ctx]),nl,
         flag('Axioms Compiled',_,0),
         safe_file_open(FileName,'r',INPUT),
         repeat,   %trace,
                load_kif_to_kb_ctx_display(KB,Ctx,User,INPUT),
                close(INPUT),
                flag('Axioms Compiled',AX,AX),
         writeFmt('\n% Compiled ~w axioms.\n',[AX]),
         ignore(retract(findings(CPU,RESULT))) ,
         ignore(findall(T,retract(title(T)),Title)),saveMooCache.



ado_to_prolog(FileName):-
        tell(FileName),
        mooCache(Pred, Head, Type, Logic,KB, Ctx, Explaination),
        w_ado_cache(Pred, Head, Type, Logic,KB, Ctx, Explaination),
        fail.

ado_to_prolog(FileName):-
        mooCache(Pred, Head, Cond,Type, Logic,KB, Ctx, Explaination),
        w_ado_cache(Pred, Head, Cond,Type, Logic,KB, Ctx, Explaination),
        fail.
ado_to_prolog(FileName):-told.

atom_to_prolog(FileName):-
        tell(FileName),
        atom_to_prolog(FileName),
        told.


atom_to_prolog:-
        mooCache(Pred, Head, Type, Logic,KB, Ctx, Explaination),
        a_ado_cache(Pred, Head, Type, Logic,KB, Ctx, Explaination),
        fail.

atom_to_prolog:-
        mooCache(Pred, Head, Cond,Type, Logic,KB, Ctx, Explaination),
        a_ado_cache(Pred, entails(Head, Cond),Type, Logic,KB, Ctx, Explaination),
        fail.

atom_to_prolog:-
        mooCache(atom(A)),
        format(' ~q ; ',[A]),fail.

atom_to_prolog.

a_ado_cache(argOf, Head, Type, true,KB, Ctx, Explaination):-!.
a_ado_cache(documentation, Head, Type, true,KB, Ctx, Explaination):-!.
a_ado_cache(Pred, FOO,Type, Logic,KB, Ctx, Explaination):-
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





w_ado_cache(argOf, Head, Type, true,KB, Ctx, Explaination):-!.
w_ado_cache(documentation, Head, Type, true,KB, Ctx, Explaination):-!.

w_ado_cache(Pred, Head, Type, true,KB, Ctx, Explaination):-
        format('~q.~n',[Head]),!.
w_ado_cache(Pred, Head, Type, false,KB, Ctx, Explaination):-
        format('not_~q.~n',[Head]),!.
w_ado_cache(Pred, Head, Pre, Type, true,KB, Ctx, Explaination):-
        pre_to_b(Pre,B),
        format('~q:-~q.~n',[Head,B]),!.
w_ado_cache(Pred, Head, Pre, Type, false,KB, Ctx, Explaination):-
        pre_to_b(Pre,B),
        format('not_~q:-~q.~n',[Head,B]),!.

pre_to_b(B,var(B)):-isSlot(B),!.
pre_to_b(and(A,B),(AA,BB)):-!,pre_to_b(A,AA),pre_to_b(B,BB).
pre_to_b(not(B),BBO):-pre_to_b(B,BB),BB=..[F|A],atom_concat('not_',F,NF),BBO=..[NF|A].
pre_to_b(B,B).


load_kif_to_kb_ctx_display(KB,Ctx,User,Stream):-at_end_of_stream(Stream),!.

load_kif_to_kb_ctx_display(KB,Ctx,User,Stream):-
                      once((
                                source_from_stream(Stream,_,SOURCEFORM,Vars),
                                catch(
                                                (
                                                                        flag('Axioms Compiled',X,X+1),
                                                                        invokeInsert([trusted,nocanonicalize],surface,SOURCEFORM,Ctx,TN,KB,Vars,User)
                                                ),

                                        E,
                                        (line_count(Stream,Line),writeFmt('\nLine ~w  Uninterpretable "~q" (~q)\n',[Line,KIFSTERM,E]))
                                )
                      )),
                      fail.


% ===================================================================
%  Read Knowledge Representation File
% ===================================================================

% Example
lmerge:-tell_from_kif(forall,'c:/mooL/SUO/PrologMOO.can','PrologMOO','GlobalContext',Maintainer).

kif_file(File):-tell_from_kif(forall,File,File,'GlobalContext',Maintainer).

tell_from_kif(SourceFile):-!,tell_from_kif(forall,SourceFile,KB_Name,Ctx,SourceFile).

tell_from_kif(SourceFile,KB_Name):-!,tell_from_kif(forall,SourceFile,KB_Name,Ctx,SourceFile).

tell_from_kif(Driver,SourceFile,KB_Name,Ctx,Maintainer):-
            sendNote(debug,kifParser,['Loading Moo KIF/CAN file into ',Driver,' as',KB_Name,Ctx,from,SourceFile],' '),
            ignore(safe_kb_info_db(KB_Name,SourceFile,WFSFile,_)),
            ignore(Maintainer=SourceFile),
            safe_file_open(SourceFile,'r',INPUT),
            repeat,
               once((
                     once(readKIF(INPUT,CHARS)),
                     once(invokeInsert(Driver,chars,CHARS,Ctx,TN,KB_Name,Vars,Maintainer))
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
                    %catch(writeFmt(OUTPUT,'\n% ~s  ',[Chars]),_,true),
                    tell_retract_parse_chars(Chars,PROLOG,_),
                     once((
                           PROLOG=comment(_)
                            ;
                          writeFmt(OUTPUT,'\n~q.\n',[PROLOG])
                            )))),
                     at_end_of_stream(INPUT), !,
            sendNote(debug,kifParser,'File Loaded',PLocation),
            file_close(OUTPUT),
            file_close(INPUT),
            consult_as_dynamic(PLocation).

% ===================================================================
%  Export Surface Forms as KIF (From a KB and Context)
% ===================================================================


export_kif_from_source(KB,DestFile):-
         export_kif_from_source(KB,Ctx,DestFile,full_cmt).

export_kif_from_source(KB,Ctx,DestFile):-
         export_kif_from_source(KB,Ctx,DestFile,full_cmt).

export_kif_from_source(KB,Ctx,DestFile,Fmt):-
         add_file_extension(".kif",DestFile,PLocation),
         safe_file_open(PLocation,'w',OUTPUT),
         sendNote(debug,kifParser,'Saving kif from Prolog.',[saving,to,PLocation]),
         export_kif_from_source_0(Fmt,KB,Ctx,OUTPUT).

export_kif_from_source_0(Format,KB,Ctx,OUTPUT):-
         get_store(forall,surface,(Surface:Vars),KB,Ctx,TN,Maintainer),
         unnumbervars((Surface:Vars),(USurface:UVars)),
         toMarkUp(kif,USurface,UVars,Chars),
         ( Format=full_cmt -> writeFmt(OUTPUT,'\n; KB:~q  Ctx:~q  TN:~q  Auth:~q\n~s\n',[KB,Ctx,TN,Maintainer,Chars]);
         ( Format=terse -> writeFmt(OUTPUT,'\n~s\n',[Chars]);
         ( Format=pnx_nf -> writeFmt(OUTPUT,'\n( pnx_nf ~s ~w ''~w'' )\n',[Chars,Ctx,TN])))),
         fail.

export_kif_from_source_0(Format,KB,Ctx,OUTPUT):-!.

% ===================================================================


agent_load_kif_quiet(Filename,GlobalContext,User):-
        agent_load_kif_surface(Filename,KB,Ctx,User,AX,quiet).

agent_load_kif(Filename,Ctx,User):-
        agent_load_kif_surface(Filename,KB,Ctx,User,AX,loud).


agent_load_kif_surface(Filename,KB,Ctx,User,AX,Verbose):-
        ignore(User='Automation'),
        (unsetMooOption(opt_surface_check=_)),
        (setMooOption(opt_surface_check=trusted)),
         get_default_assertion_context(DCtx), !,ignore((Ctx=DCtx)),!,
         getDefaultKB(DKB), !,  ignore((KB=DKB)),!,
        idGen(TN1),
        idGen(TN2),
        idGen(TN3),
        assertaClean(mooCache(PredR,surface,'instance'(KB,'KnowledgeBase'),'$VAR'(0),'MooKernel','GlobalContext',TN1,User,gaf)),
        assertaClean(mooCache(PredR,surface,'instance'('GlobalContext','Context'),'$VAR'(0),KB,'GlobalContext',TN2,User,gaf)),
        assertaClean(mooCache(PredR,surface,'sourcefile-of'(KB,Filename),'$VAR'(0),'MooKernel','GlobalContext',TN3,User,gaf)),
        writeFmt(user_error,'% Reading In ~w to ~w with a default context of ~w \n',[Filename,KB,Ctx]),
        flag('Axioms Compiled',_,0),
        safe_file_open(Filename,'r',INPUT),!,
           repeat,
               once((once(readKIF(INPUT,Chars)),
                  % say(Chars),
                    %catch(writeFmt(OUTPUT,'\n% ~s  ',[Chars]),_,true),
                    tell_retract_parse_chars(Chars,PROLOG,Vars),
                     once((
                           PROLOG=comment(_)
                            ;

                          remember_ado(PROLOG,Vars,KB,Ctx,User,Verbose)
                            )))),
                     at_end_of_stream(INPUT), !,
        close(INPUT),
        flag('Axioms Compiled',AX,AX),
        writeFmt(user_error,'% Loaded ~w axioms.\n',[AX]),
        ignore(retract(findings(CPU,RESULT))),
        ignore(findall(T,retract(title(T)),Title)),!.

remember_ado(Surface,Vars,KB,Ctx,User,quiet):-!,
        flag('Axioms Compiled',AX,AX+1),
        once(invokeInsert([untrusted,canonicalize],surface,Surface,Ctx,TN,KB,Vars,User)).

remember_ado(Surface,Vars,KB,Ctx,User,_):-!,
        flag('Axioms Compiled',AX,AX+1),
        once(invokeInsert([untrusted,canonicalize],surface,Surface,Ctx,TN,KB,Vars,User)),
        ignore((writeObject(formula(Surface),Vars))),nl.

agentSave(FileName,Ctx):-
        tell(FileName),
%       writeFmt((':-include(\'moo_header.P\').\n'),[]),
        writeFmt(':-multifile(mooCache/6).\n',[]),
        writeFmt(':-multifile(mooCache/9).\n',[]),
        writeFmt(':-multifile(mooCache/5).\n',[]),
        writeFmt(':-retractall(mooCache(PredR,_,_,_,_,~q,_,_,_)).\n',[Ctx]),
        writeFmt(':-index(mooCache(1 ,0,1,0,0,1,0,0,1)).\n',[]),
        save_each_assertion_of(Ctx),
        told.


save_each_assertion_of(Ctx):-mooCache(PredR,Form,Surface,Vars,KB,Ctx,EXTID,User,Status),
                writeFmt('~q.\n',[mooCache(PredR,Form,Surface,Vars,KB,Ctx,EXTID,User,Status)]),fail.
save_each_assertion_of(Ctx):-!.

agentLoad(Filename):-ensure_loaded(Filename).


% fguard(Template,RH,NVars,Explaination,Functor)
fguard(RH,NVars,surf(KB,TN,CID,[]),Functor):-!.

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
        writeDebugS(bumping(KRVars)),!,
        catch(exit(Ref,cut),_,fail).
        %(catch(fail(Ref),_,(writeDebugS(missing(Copy)),fail)),fail),write(bollk),nl.
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
        %catch(erase(Ref),_,writeDebugS(missingRef(Vars))).
%       processResult(Result,TN,CID,F,KRVars,RB,Session,Ref),!,
%       ground(RealShared).

%processResult(Result,TN,CID,F,Vars,RB,Session,Ref):-var(Result),!, % normal completion

%processResult(previousCallMoreSpecific(Vars,Copy),TN,CID,F,KRVars,RB,Session,Ref):- % decendant aborted to here
        %catch(erase(Ref),_,writeDebugS(missingRefInpreviousCallMoreSpecific(KRVars))),
 %       writeDebugS(previousCallMoreSpecific(KRVars,Vars,Copy)). %,!,fail.



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
        writeDebugS(somethingKilled(Session)).


mooCall(X):-
        mooCache(X, Cost,KB, Ctx,surf(KB,TN,CID,[])).

mooCall(X):-
        mooCache(X, Cost,KB, Ctx,surf(KB,TN,CID,[])).


mooCall(Flags,KB):-
        mooCache(Cons, Ante,Vars,KB, Ctx,TN).

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


ca:-compile_show('PrologMOO',instance,2,Debug).

cf:- compile_to_file(instance,2,'PrologMOO').


va:- compile_show('PrologMOO',valence,2,Debug).

ensure_all_compiled:-!.

ensure_all_compiled:-
        getAllMooKB(X),
        compileKB(X),fail.


getAllMooKB(X):-fail.

compileKB(KB):-!.
compileKB(KB):-
        compileInstanceSubclass(KB).



make_kb(KB):-
        retractall(mooCache(KB,_,_)),
        atom_concat(KB,'.prolog',PrologFile),
        tell(PrologFile),
        format('

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

        '),
        image_to_prolog(KB),!,
        told,
        save_make_kb(PrologFile,KB,FeatureFile),
        atom_concat(KB,'.pl',OutputFile),
        concat_atom([cat,FeatureFile,PrologFile,'>',OutputFile],' ',Cmd),
        format('~n~w~n',[Cmd]).


save_make_kb(PrologFile,KB,FeatureFile):-
        atom_concat(KB,'.feature',FeatureFile),
        tell(FeatureFile),
        format('

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

        '),
        save_features_kb(KB),
        told.

save_features_kb(KB):-
        format('~n~n% Predicates~n~n'),
        mooCache(KB,type(dynamic),Data),
        format(':-~q.~n',[dynamic(Data)]),fail.

save_features_kb(KB):-
        format('~n~n% Predicates~n~n'),
        mooCache(KB,type(dynamic),Data),
        format(':-~q.~n',[tabled(Data)]),fail.

/*
save_features_kb(KB):-
        format('~n~n% Tables~n~n'),
        mooCache(KB,type(tabled),Data),
        format(':-~q.~n',[tabled(Data)]),fail.
  */

/*
save_features_kb(KB):-
        format('~n~n% Not Tabled ~n~n'),
        mooCache(KB,type(dynamic),Data),
        not(mooCache(KB,type(tabled),Data)),
        format(':-~q.~n',[prolog(Data)]),fail.
*/

save_features_kb(KB):-
        format('~n~n% Rules~n~n'),
        mooCache(KB,type(rule),Data),
        format('~q.~n',[rules_for(Data)]),fail.

save_features_kb(KB):-
        format('~n~n% No Rules For~n~n'),
        mooCache(KB,type(dynamic),Data),
        not(mooCache(KB,type(rule),Data)),
        format('~q.~n',[no_rules_for(Data)]),fail.

save_features_kb(KB):-
        format('~n~n% Facts~n~n'),
        mooCache(KB,type(fact),Data),
        format('~q.~n',[facts_for(Data)]),fail.

save_features_kb(KB):-
        format('~n~n% No Facts~n~n'),
        mooCache(KB,type(dynamic),Data),
        not(mooCache(KB,type(fact),Data)),
        format('~q.~n',[no_facts_for(Data)]),fail.

save_features_kb(KB):-
        format('~n~n% No Rules/Facts~n~n'),
        mooCache(KB,type(dynamic),Data),
        not(mooCache(KB,type(fact),Data)),
        not(mooCache(KB,type(rule),Data)),
        format('~q.~n',[no_assertions_for(Data)]),fail.

save_features_kb(KB):-
        format('~n~n% Lemma Reqs:~n~n'),
        findall((F-C),mooCache(KB,type(F/A),(C/N)),Edges),
        keysort(Edges,Sorted),
        format('lemma_edges(~q).~n~n',[Sorted]),!.


image_to_prolog:-
        tell('PrologMOO.pl'),
        image_to_prolog(KB),
        told.


image_to_prolog(KB):-
        hardcoded(HardCoded),
        image_to_prolog([holds,neg(lit)|HardCoded],KB).

image_to_prolog(Flags,KB):-format(
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





image_to_prolog(Flags,KB):-
        mooCache(Cons, Cost,KB, Ctx,Explaination),
                convertPrologWFS(Flags,KB,Activation,Explaination,Cons,Prolog),
        writeAsProlog(Prolog),fail.


image_to_prolog(Flags,KB):-
        mooCache(Cons, Ante,Cost,KB, Ctx,Explaination),
        convertPrologWFS(Flags,KB,Activation,Explaination,(Cons:-Ante),Prolog),
        writeAsProlog(Prolog),fail.



image_to_prolog(Flags,KB):-!.



tabled_consult(File):-
        tell('tabling.log'),
        assertz((term_expansion(X,Y) :- catch((!,getTermExpansionLogged(X,Y)),E,fail))),
        consult(File),
        abolish(term_expansion/2),
        dynamic(term_expansion/2),
        multifile(term_expansion/2),!,
        told.

/*
tkb(KB):-
        tell('tabling.log'),
        mooCache(Cons, Cost,KB, Ctx,Explaination),
        assertClauseTable(KB,Cons,Explaination),fail.

tkb(KB):-
        mooCache(Cons, Ante,Cost,KB, Ctx,Explaination),
        assertClauseTable(KB,(Cons:-Ante),Explaination),fail.

tkb(KB):-told.

*/

assertClauseTable(KB,WFS,Explaination):-
        convertPrologWFS(Flags,KB,assertClauseTable,Explaination,WFS,Prolog),
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

assertClauseTable(KB,WFS,Explaination):-
        convertPrologWFS(Flags,KB,assertClauseTable,Explaination,WFS,Prolog),
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


convertPrologWFS(Flags,KB,Activation,surf(KB,TN,CID,KRVars),
        (C :- A),
        ((
        RealHead :-
                guard(Functor,(RealHead:Head),(RFVH:FVH),Body,TN,CID,KRVars,
                RuleVars,UnivHead,BodyUniv,BodySelfConnected,Shared)))):-!,
        functor(C,F,_),convertNegations((not),F,Functor,_),
        convertRuleHeadWFS(Flags,KB,Activation,C,Head,RuleHead),
        convertRuleBodyWFS(Flags,KB,Activation,RuleHead,A,Body),!,
        getPrologVars(KRVars,RuleVars,_,_),!,
        getPrologVars(Head,FVH,_,_),!,set_partition(RuleVars,FVH,_,_,HeadVars),
        getPrologVars(Body,FVB,BSingles,_),!,set_partition(RuleVars,FVB,_,_,BodyVars),
        set_partition(HeadVars,BodyVars,PrivHead,PrivBody,Shared),!,
        set_partition(PrivBody,BSingles,BodySelfConnected,_,BodyUniv),!,
        copy_term((Head,FVH,PrivHead),(RealHead,RFVH,UnivHead)).

convertPrologWFS(Flags,KB,Activation,Explaination,(C),(RH:- fguard(RH,NVars,Explaination,Functor))):-!,
        functor(C,F,_),convertNegations((not),F,Functor,_),
        convertFactHeadWFS(Flags,KB,Activation,C,RH),
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

recordIfNew(Activation,KB,Cons,Type):-!.

recordIfNew(Activation,KB,Cons,Type):-
        atom(Type),!,
        functor(Cons,Pred,Arity),
        recordIfNewCache(KB,Type,Pred/Arity),!.
recordIfNew(Activation,KB,Cons,Type):-
        functor(Type,T,A),
        functor(Cons,Pred,Arity),
        recordIfNewCache(KB,(T/A),(Pred/Arity)).

recordIfNewCache(KB,Type,Data):-
        mooCache(KB,type(Type),Data),!.
recordIfNewCache(KB,Type,Data):-
        assertz(mooCache(KB,type(Type),Data)),!.



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
compile_to_file(STANDARDPred,Arity,KB):-
        make_relation_profile(KB,STANDARDPred,Logic,Arity,N,Module,SourceInfo,Functor),
        open(SourceInfo, write, Stream, [buffer(full),type(text),alias(STANDARDPred)]),
        compile_show(STANDARDPred,KB,STANDARDPred,Arity,Debug),
        close(STANDARDPred).
*/
compile_to_file(STANDARDPred,Arity,KB):-
        make_relation_profile(KB,STANDARDPred,Logic,Arity,N,Module,SourceInfo,Functor),!,
        open(SourceInfo, write, Stream, [buffer(full),type(text),alias(STANDARDPred)]),
        compile_show(Stream,KB,STANDARDPred,Arity,Debug),!,
        close(Stream),!.

compile_show(KB,STANDARDPred,Arity,Debug):-
        current_output(Stream),!,
        compile_show(Stream,KB,STANDARDPred,Arity,Debug),!.


compile_show(Stream,KB,STANDARDPred,Arity,Debug):-
        ignore(Debug=no_debug),!,
        make_relation_profile(KB,STANDARDPred,Logic,Arity,N,Module,SourceInfo,Functor),
        mkImported(full,Functor,Arity,Logic,Vect,Ctx,ExplainationIn,Explaination,Imported),
        mkArgsAtom(Arity,ArgsAtom),
        mkHolds(STANDARDPred,Arity,Vect,Cons),
        mkIndex(Functor,N,Index),
        Dash = (Functor/N),
        format(Stream,'
/* <PRE face="Arial,Helvetica"><font face="Arial,helvetica"><font color=green>
File: "~w"

Maintainer:      dmiles@users.sourceforge.net [Douglas R. Miles]

Purpose:  Individual loading of Moo KB Predicates.

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
KB:   ~q
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
% belief_module(KB,Ctx,STANDARDPred,Cons,Imported,ConnectionType,SourceInfo,HowOften).
%       KB = The KnowedgeBase
%       Ctx = The Context
%       STANDARDPred = The Functor\'s STANDARD Name
%       Goal = Functors STANDARD Prototype       "goal(Logic,ExplainationIn,holds(STANDARDPred,A,B),Ctx,KB,ExplainationOut)"
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
        Module,STANDARDPred,Cons,KB,Debug, % Comments
        Module,Dash,Dash,Dash,Dash,  % module/2
        KB,STANDARDPred,STANDARDPred,ArgsAtom,KB,Functor,ArgsAtom,SourceInfo, % belief_module/5
        Index,Index,Index,Index,    % index/1
        Dash,Dash,Dash,Dash      % dynamic/1
        ]),!,
        create_entry_points(Stream,non_singleValued,Functor,KB,STANDARDPred,Arity,N,Module,Debug,ArgsAtom),!,
        make_pred_data(Stream,Functor,KB,STANDARDPred,Arity,N,Module,Debug,ArgsAtom),!.

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


create_head(Tag,KB,STANDARDPred,Logic,Args,Head):-
        concat_atom([Tag,KB,'_',STANDARDPred],Functor),
        Head=..[Functor|Args],!.


% ===============================================================
% make_relation_profile(-KB,-STANDARDPred,-Logic,-Arity,+N,+Module,+SourceInfo,+Functor)
% ===============================================================
make_relation_profile(KB,STANDARDPred,Logic,Arity,N,Module,SourceInfo,Functor):-!,
        ignore(KB='PrologMOO'),
        ignore(STANDARDPred='attribute'),
        ignore(Arity=2),
        is(N,(Arity + 4)),
        concat_atom([KB,'_',STANDARDPred],Module),
        concat_atom([Module,Arity],Functor),
        concat_atom(['pred_', Module, '.pl' ],SourceInfo),!.


create_entry_points(Stream,non_singleValued,Functor,KB,STANDARDPred,Arity,N,Module,Debug,ArgsAtom):-
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

make_pred_data(Stream,Functor,KB,STANDARDPred,Arity,N,Module,Debug,ArgsAtom):-
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


getrule(STANDARDPred,Arity,Cons,Precond, KB,Ctx, surf(KB,TN,CID),Vars):-
        make_head_t(true,STANDARDPred,Arity,Cons),
        mooCache(Cons, A1,A2,A3, Cost,KB, Ctx,surf(KB,TN,CID,Vars)),
        once((append(A1,A2,AM),
        append(AM,A3,Precond))).
getrule(STANDARDPred,Arity,Cons,Precond, KB,Ctx, surf(KB,TN,CID),Vars):-
        make_head_t(false,STANDARDPred,Arity,Cons),
        mooCache(Cons, A1,A2,A3, Cost,KB, Ctx,surf(KB,TN,CID,Vars)),
        once((append(A1,A2,AM),
        append(AM,A3,Precond))).

% True GAFS
make_pred_data(Stream,Functor,KB,STANDARDPred,Arity,N,Module,Debug,ArgsAtom):-
        make_head_t(true,STANDARDPred,Arity,Cons),
        mooCache(Cons, Precon, KB,Ctx, TID),
        write_rulenum(Stream,TID),
        numbervars((Cons, Precon, KB,Ctx, TID,Vars),'$VAR',15,_),
        submit_ado_cache(Stream,STANDARDPred,Cons, Precon, KB,Ctx, TID,Vars),fail.

% False GAFS
make_pred_data(Stream,Functor,KB,STANDARDPred,Arity,N,Module,Debug,ArgsAtom):-
        make_head_t(false,STANDARDPred,Arity,Cons),
        mooCache(Cons, Precon, KB,Ctx, TID),
        write_rulenum(Stream,TID),
        numbervars((Cons, Precon, KB,Ctx, TID,Vars),'$VAR',15,_),
        submit_ado_cache(Stream,STANDARDPred,Cons, Precon, KB,Ctx, TID,Vars),fail.

% True then Fasle Rules
make_pred_data(Stream,Functor,KB,STANDARDPred,Arity,N,Module,Debug,ArgsAtom):- %trace,
        getrule(STANDARDPred,Arity,Cons,Precond, KB,Ctx, TID,Vars), %trace,
        write_rulenum(Stream,TID),
        close_list(Vars),
        numbervars((Stream,STANDARDPred,Cons,Precond, KB,Ctx, TID,Vars),'$VAR',15,_),
        submit_ado_cache(Stream,STANDARDPred,Cons,Precond, KB,Ctx, TID,Vars),fail.

       % toMarkUp(kif,DispExplaination,Vars,PrettyForm),
        %logOnFailure(format(Stream,'/*~n</B><font color=green>~n~nForms:~n~n~s~nFlags: ~w \n</font><B>*/',[PrettyForm,Flags])),
%       make_disp_explaination(Logic,TID,Vars,Cons,Ante,DispExplaination),
%       format(Stream,'~q. ~n~n',[explaination_line(TID,Vars,DispExplaination)])         %trace,


% Write footer
make_pred_data(Stream,Functor,KB,STANDARDPred,Arity,N,Module,Debug,ArgsAtom):-
        getPrettyDateTime(String),
        format(Stream,'\n/* \n Last Saved: ~s</B></font>\n</PRE>*/\n\n\n',[String]),!.


%submit_ado_cache(Stream,STANDARDPred,Cons,_,_,Logic, KB, Ctx, Explaination * _):-trace,fail.


% No antecedents flags
submit_ado_cache(Stream,STANDARDPred,Cons, [], KB,Ctx, TID,Vars):-
        Cons=..[_|Arguments],
        append(Arguments,[Ctx,TID],Args),  !,
        create_head('gaf_',KB,STANDARDPred,true,Args,PrologHead),
        format(Stream,'~n~q.~n',[PrologHead]),!.

% With antecedents flags
submit_ado_cache(Stream,STANDARDPred,Cons,Ante, KB,Ctx, TID,Vars):-
        Cons=..[_|Arguments],
        numbervars(SubCtx),
        append(Arguments,[Ctx,ExplainationIn,[explaination_line(TID,VarsRef)|ExplainationOut]],Args),
        create_head('bk_',KB,STANDARDPred,true,Args,PrologHead),
        format(Stream,'~n ~q :- \n',[PrologHead]),!,
        %format(Stream,'\t~q,~n',[not_near_member(TID,ExplainationIn)]), !,
      %  findall(Neg,member(-(Neg),Ante),NegS),
        % findall(Pos,member(+(Pos),Ante),PosS),
        %format(Stream,'\t~q,~n',[minor_interception(PosS,NegS,ExplainationIn,ExplainationMid)]),
        %write_std_flags(Stream,KB,Flags,(Cons:Ante)),!,
        format(Stream,'\t~q,~n',[subcontext(Ctx,SubCtx)]),
        write_prolog_body_clause(Stream,Ante,STANDARDPred,Cons,Ante,KB, SubCtx, TID,Vars,ExplainationIn,ExplainationOut),!,
        format(Stream,'\t~q.~n~n',[(ground(Vars),VarsRef=Vars)]),!.

write_std_flags(Stream,KB,Flags,Term):-
        write_lllist(Stream,KB,Flags),!.

write_lllist(Stream,KB,[]):-!.
write_lllist(Stream,KB,[Arg|List]):-
        format(Stream,'\tk~w_~q,~n',[KB,Arg]),
        write_lllist(Stream,KB,List),!.


/*

submit_ado_cache(Stream,STANDARDPred,Cons,Ante,Flags,Logic, KB, Ctx, TID * Vars):-
        write_prolog_rule(Stream,STANDARDPred,Cons,Ante,Flags,Logic, KB, Ctx, TID,Vars),!.

write_prolog_rule(Stream,STANDARDPred,Cons,Ante,Flags,Logic, KB, Ctx, TID,Vars):-
        format(Stream,' ~q. ~n',[explaination_line(TID,Vars,DispExplaination)]),
        Cons=..[_|Arguments],
        append(Arguments,[Ctx,ExplainationIn,(ExplainationOut * TID,Vars)],Args),
        create_head('bachchain_',KB,STANDARDPred,Logic,Args,PrologHead),
        format(Stream,' ~q :- ~n',[PrologHead]),
        write_prolog_body_start(Stream,Ante,STANDARDPred,Cons,Ante,Flags,Logic, KB, Ctx, TID,Vars,ExplainationIn,ExplainationMid),!,
        write_prolog_body_clause(Stream,Ante,STANDARDPred,Cons,Ante,Flags,Logic, KB, Ctx, TID,Vars,ExplainationMid,ExplainationOut),!.

*/
%       write_prolog_body_clause(Stream,Ante,STANDARDPred,Cons,Ante,Flags,Logic, KB, Ctx, TID,Vars,ExplainationMid,ExplainationOut),!.

write_prolog_body_clause(Stream,OrigAnte,STANDARDPred,Cons,[], KB, Ctx, TID,Vars,ExplainationIn,ExplainationOut):-!,
        format('\t~w,~n',[ExplainationIn=ExplainationOut]).

write_prolog_body_clause(Stream,OrigAnte,STANDARDPred,Cons,[Ante], KB, Ctx, TID,Vars,ExplainationIn,ExplainationOut):-!,%trace,
        write_prolog_term(Stream,OrigAnte,KB,Flags,Caller,FlagsSTANDARDPred,Cons,Ante,KB, Ctx, TID,Vars,ExplainationIn,ExplainationOut),!.

write_prolog_body_clause(Stream,OrigAnte,STANDARDPred,Cons,[Ante|More], KB, Ctx, TID,Vars,ExplainationIn,ExplainationOut):-!,%trace,
        write_prolog_term(Stream,OrigAnte,KB,Flags,Caller,FlagsSTANDARDPred,Cons,Ante,KB, Ctx, TID,Vars,ExplainationIn,ExplainationMid),
        write_prolog_body_clause(Stream,OrigAnte,STANDARDPred,Cons,More, KB, Ctx, TID,Vars,ExplainationMid,ExplainationOut),!.




check_end_flags(FlagsList,ExplainationIn,ExplainationOut):-ExplainationIn=ExplainationOut.
check_begin_flags(FlagsList,ExplainationIn,ExplainationOut):-ExplainationIn=ExplainationOut.



write_prolog_term(Stream,OrigAnte,KB,Flags,Caller,FlagsSTANDARDPred,Cons,Useless, KB, Ctx, TID,Vars,ExplainationIn,ExplainationIn):- useless(Useless),!.
write_prolog_term(Stream,OrigAnte,KB,Flags,Caller,FlagsSTANDARDPred,Cons,Ante, KB, Ctx, TID,Vars,ExplainationIn,ExplainationOut):-
        Ante=..[P|Arguments],
        append(Arguments,[Ctx,ExplainationIn,ExplainationOut],Args),
        create_head('fw_',KB,P,true,Args,PrologHead),
        format(Stream,'\t~q,~n',[PrologHead]),!.

useless(domainC(_,[])).
useless(domainA(_,[])).



make_disp_explaination(true,surf(KB,TID),Vars,Cons,Conds,via(entails(CondsO,Cons),Vars) * surf(KB,TID)):-fix_conds(Conds,CondsO),!.
make_disp_explaination(false,surf(KB,TID),Vars,Cons,Conds,via(entails(CondsO,not(Cons)),Vars) * surf(KB,TID)):-fix_conds(Conds,CondsO),!.
make_disp_explaination(true,surf(KB,TID,ID),Vars,Cons,Conds,via(entails(CondsO,Cons),Vars) * surf(KB,TID,ID)):-fix_conds(Conds,CondsO),!.
make_disp_explaination(false,surf(KB,TID,ID),Vars,Cons,Conds,via(entails(CondsO,not(Cons)),Vars) * surf(KB,TID,ID)):-fix_conds(Conds,CondsO),!.

fix_conds(Var,Var):-isSlot(Var),!.
fix_conds([],true):-!.
fix_conds([A],AA):-!,
        fix_conds(A,AA).
fix_conds(+Conds,Conds).
fix_conds(-Conds,not(Conds)).
fix_conds([A|B],and(AA,BB)):-!,
        fix_conds(A,AA),
        fix_conds(B,BB).



