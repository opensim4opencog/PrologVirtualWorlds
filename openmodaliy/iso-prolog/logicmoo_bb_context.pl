:-include('logicmoo_utils_header.pl').
% ===========================================================
% microtheory.moo
% ===========================================================
clientEvent(Channel,Agent,[file='microtheory.moo'|A]):-member(request=advanced,A),!,
      getMooOption(Agent,ctx,Ctx),
      sformat(Title,'Advanced microtheory work -  ~w',[Ctx]),
      writeHTMLStdHeader(Title),
      say('<p><table border=0><tr><td width="20%"><font size=+1>Microtheory</font></td><td width="8%"><font size=+1>Status</font></td><td width="20%"><font size=+1>Options</font></td></tr>'),
      listDoMt(Ctx),
      say('</table><p>'),
      www_form_encode(Ctx,Mt),
      say('<pre>'),
      htmlListing(Ctx),
      say('</pre>').
    %  dumpMtContents(Ctx).

dumpMtContents(Ctx):-
   saved(Surface,Ctx,Vars,Lits),
   writeHtml(saved(Surface,Ctx,Vars,Lits)),
   say('<br>'),
   fail.
dumpMtContents(Ctx).	      



clientEvent(Channel,Agent,[file='microtheory.moo'|A]):-!,
      writeHTMLStdHeader('Work with Microtheory Content'),
      findall(X,saved('#$genlMt'(X,_),'#$BaseKB',_,_),L),sort(L,OptionList),
      ignore(getMooOption(Agent,ctx,Ctx)),
      ignore(writeObject(select('ctx',[Ctx|OptionList]))),
      say('<p>Status Filters:'),writeSpaces(4),
      writeCheckbox('MtShowActive','Show Active','true'),writeSpaces(4),
      writeCheckbox('MtShowUnloaded','Show Unloaded','true'),writeSpaces(4),
      writeCheckbox('MtShowRemoved','Show Removed','false'),writeSpaces(4),
      say('<p><table border=0><tr><td width="20%"><font size=+1>Microtheory</font></td><td width="8%"><font size=+1>Status</font></td><td width="20%"><font size=+1>Options</font></td></tr>'),
      listDoMt(OptionList),
      say('</table>'),
      writeHTMLStdFooter.

listDoMt([]).
listDoMt([H|T]):-!,listDoMt(H),listDoMt(T).
listDoMt(Mt):-say('<tr>'),say('<td>'),
	 www_form_encode(Mt,MtE),
	 writeHyperLink(Mt,[],'microtheory.moo?request=advanced&ctx=~w',[MtE,MtE]),writeSpaces(4),
	 getMtStatus(Mt,Status),
	 say('<td><a>~w</a><td>',[Status]),!,
	 writeHyperLink('Deactivate',[],'microtheory.moo?request=deactivate&ctx=~w',[MtE,MtE]),writeSpaces(4),
	 writeHyperLink('Revert',[],'microtheory.moo?request=revert&ctx=~w',[MtE,MtE]),writeSpaces(4),
	 writeHyperLink('Remove',[],'microtheory.moo?request=remove&ctx=~w',[MtE,MtE]),writeSpaces(4),
	 say('</td></tr>').
% ===========================================================
% Create Knowledge Base (New)
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(submit=register_theory,Options),!,
        ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        ensureMooOption(Agent,client='java',CLIENT),
        ensureMooOption(Agent,user='Web',Agent),
        saveMooCache,
        getDefaultImageFilepath(IF),
        open(IF,append,Handle,[close_on_abort(false),buffer(full)]),
        assert(save_can_to_file(Context,Handle)),
        !.
% ===========================================================
% Invoke Surface Checker on Context/Ctx
% ===========================================================
clientEvent(Channel,Agent,Options):-
        memberchk(cmd='Surface Check',Options),!,%true,
        ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),!,
        logOnFailure(surface_check_entire_theory_ctx(Context,Ctx)),!,
        say('<hr>Done Surface Check\n',[]).


showContextHtml(Ctx):-
      getMooOption(Agent,ctx,Ctx),
      findall(X,saved('#$genlMt'(X,_),'#$BaseKB',_,_),L),sort(L,OptionList),
      writeObject(select('ctx',[Ctx|OptionList])),!.
%showContextHtml(Ctx):-!.

:-dynamic(save_can_to_file/2).

getMtStatus(Mt,'<font color=green>Active</font>').



getTheoryStatus('MooKernel'):-say('Browse Only\n',[]),!.
getTheoryStatus(Context):-
        isContextCompilerProcess(Context,Progress),
        say('<A href=controlpanel.jsp?theory=~w><font color=green>Canonicalizing</font></a>\n',[Context]),!.

getTheoryStatus(Context):-
        isUncanonicalized(Context),
        invokeContextCompilerProcess(Context),
        isContextCompilerProcess(Context,Progress),
        say('<A href=controlpanel.jsp?theory=~w><font color=green>Canonicalizing</font></a>\n',[Context]),!.

getTheoryStatus(Context):-
        isUncanonicalized(Context),
        isContextCurrentlyInUse(OtherContext,Status),!,
        say('<font color=orange>Waiting for ~w</font>\n',[OtherContext]).

getTheoryStatus(Context):-
        isContextCurrentlyInUse(Context,Status),
        say('<font color=orange>Busy ~w</font>\n',[Status]).

getTheoryStatus(Context):-
        isTheoryUntransfered(Context),
        say('<A href=stheory_to_prolog.jsp?theory=~w><font color=red>Update</font></a>\n',[Context]),!.

getTheoryStatus(Context):-
        isSourceNewerThenImage(Context),
        say('<A href=stheory_to_prolog.jsp?theory=~w><font color=red>Needs Update</font></a>\n',[Context]),!.

getTheoryStatus(Context):-isKnowledgeBaseLoaded(Context,_),say('<A href="askInsert.jsp?theory=~w">Ask/Insert</A>\n',[Context]),!.

getTheoryStatus(Context):-say('Unknown\n',[]),!.

% ===========================================================
% Define New context
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(client='command',Options),memberchk(submitbuttonUp='Create',Options),!,
                        ensureMooOption(Agent,newCtxName='GlobalContext',Ctx),
                        ensureMooOption(Agent,theory='LogicMOO',Context),
                        ensureMooOption(Agent,user='Web',Agent),
                        idGen(TN2),
                        assertaClean(mooCache(instance,surface,'instance'(Ctx,'Context'),'$VAR'(0),Context,'GlobalContext',TN2,Agent,on)).


% ===========================================================
% Un-Canonicalize on Context/Ctx
% ===========================================================

clientEvent(Channel,Agent,Options):-
        memberchk(cmd='Un-Canonicalize',Options),!,
        ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),!,
        (unsetMooOption(Agent,opt_surface_check=_)),
        (setMooOption(Agent,opt_surface_check=untrusted)),
        (unsetMooOption(Agent,opt_tracking_number=_)),
        (setMooOption(Agent,opt_tracking_number=supplied)),!,
        (unsetMooOption(Agent,opt_canonicalizer=_)),
        (setMooOption(Agent,opt_canonicalizer=byrd)),!,
        say('<H2>Un-Canonicalizing  Context:~w Ctx:~w.  This process may take several minutes.. Do not navigate away.</H2>',[Context,Ctx]),
        mark_all_surface_to_uncanonicalized(Context,Ctx),!.

% ===========================================================
% Invoke Load SContext on Context/Ctx (POST)
% ===========================================================

clientEvent(Channel,Agent,Options):-
                memberchk(context='Load SContext',Options),!,
                (unsetMooOption(Agent,opt_theory=_)),
                (setMooOption(Agent,opt_theory=Context)),
                ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
                ensureMooOption(Agent,opt_theory='LogicMOO',Context),!,
                fmtString(FileChars,'C:/mool/SUO/~w.can',[Context]),!,string_to_atom(FileChars,Filename),!,
                (unsetMooOption(Agent,opt_surface_check=_)),
                (setMooOption(Agent,opt_surface_check=trusted)),
                idGen(TN1),
                idGen(TN2),
                idGen(TN3),
                idGen(TN4),
                retractall(mooCache(PredR,_,_,_,Context,_,_,_)),
                assertaClean(mooCache(instance,surface,'instance'(Context,'KnowledgeBase'),'$VAR'(0),'MooKernel','GlobalContext',TN1,'WebUser',gaf)),
                assertaClean(mooCache(instance,surface,'instance'(Ctx,'Context'),'$VAR'(0),Context,'GlobalContext',TN4,'WebUser',gaf)),
                assertaClean(mooCache(instance,surface,'instance'('GlobalContext','Context'),'$VAR'(0),Context,'GlobalContext',TN2,'WebUser',gaf)),
                assertaClean(mooCache('sourcefile-of',surface,'sourcefile-of'(Context,Filename),'$VAR'(0),'MooKernel','GlobalContext',TN3,'WebUser',gaf)),
                load_kif_to_theory_ctx(Context,Filename,'GlobalContext','MooWeb').

% ===========================================================
% load_from_can_file (Knowledge Base File)
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(cmd='Load SContext',Options),!,
                ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
                ensureMooOption(Agent,opt_theory='LogicMOO',Context),!,
                fmtString(FileChars,'C:/mool/SUO/~w.can',[Context]),!,string_to_atom(FileChars,Filename),!,
                (unsetMooOption(Agent,opt_surface_check=_)),
                (setMooOption(Agent,opt_surface_check=trusted)),
                idGen(TN1),
                idGen(TN2),
                idGen(TN3),
                idGen(TN4),
                retractall(mooCache(PredR,_,_,Context,_,_,_)),
                assertaClean(mooCache(instance,surface,'instance'(Context,'KnowledgeBase'),'$VAR'(0),'MooKernel','GlobalContext',TN1,'WebUser',gaf)),
                assertaClean(mooCache(instance,surface,'instance'(Ctx,'Context'),'$VAR'(0),Context,'GlobalContext',TN4,'WebUser',gaf)),
                assertaClean(mooCache(instance,surface,'instance'('GlobalContext','Context'),'$VAR'(0),Context,'GlobalContext',TN2,'WebUser',gaf)),
                assertaClean(mooCache('sourcefile-of',surface,'sourcefile-of'(Context,Filename),'$VAR'(0),'MooKernel','GlobalContext',TN3,'WebUser',gaf)),
                load_kif_to_theory_ctx(Context,Filename,'GlobalContext','MooWeb').

% ===========================================================
% Invoke Canonicalizer on Context/Ctx (GET)
% ===========================================================
clientEvent(Channel,Agent,Options):-
        memberchk(cmd='Canonicalize',Options),!,%true,
        ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),!,
        (unsetMooOption(Agent,opt_surface_check=_)),
        (setMooOption(Agent,opt_surface_check=untrusted)),
        (unsetMooOption(Agent,opt_tracking_number=_)),
        (setMooOption(Agent,opt_tracking_number=supplied)),!,
        (unsetMooOption(Agent,opt_canonicalizer=_)),
        (setMooOption(Agent,opt_canonicalizer=byrd)),!,
        say('<H2>Canonicalizing  Context:~w Ctx:~w.  This process may take several minutes.. <br>This process must be done once and should not be interupted<br>Wait until this page is completely loaded before clicking <br>any links and do not navigate away.</H2>',[Context,Ctx]),
        logOnFailure(canonicalizeMooContextHTML(Context,Ctx)),!.

% ===========================================================
% Invoke Canonicalizer on Context/Ctx (POST)
% ===========================================================
clientEvent(Channel,Agent,Options):-
        member(Context='Canonicalize',Options),!,
        (unsetMooOption(Agent,opt_theory=_)),
        (setMooOption(Agent,opt_theory=Context)),
        ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        (unsetMooOption(Agent,opt_surface_check=_)),
        (setMooOption(Agent,opt_surface_check=untrusted)),
        (unsetMooOption(Agent,opt_tracking_number=_)),
        (setMooOption(Agent,opt_tracking_number=supplied)),!,
        (unsetMooOption(Agent,opt_canonicalizer=_)),
        (setMooOption(Agent,opt_canonicalizer=byrd)),!,
        say('<H2>Canonicalizing  Context:~w Ctx:~w.  This process may take several minutes.. <br>This process must be done once and should not be interupted<br>Wait until this page is completely loaded before clicking <br>any links and do not navigate away.</H2>',[Context,Ctx]),
        logOnFailure(canonicalizeMooContextHTML(Context,Ctx)),!.


% ===========================================================
% Start Canonicalizer
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(submit=canonicalize,Options),!,
        ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        logOnFailure(invokeContextCompilerProcess(Context)).

clientEvent(Channel,Agent,Options):-memberchk(submit=canonicalize,Options),!,
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        once(invokeContextCompilerProcess(Context)),write_ln('canonicalizing.\n').

clientEvent(Channel,Agent,Options):-memberchk(submit=blank_theory,Options),!,
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        destroyContext(Context),
        saveMooCache.


clientEvent(Channel,Agent,Options):-memberchk(submit=removeTheoryNameSpace,Options),!,
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        destroyContext(Context),
        retractall(mooCache(instance,surface,'instance'(Context,'KnowledgeBase'),'$VAR'(0),'MooKernel','GlobalContext',TN1,'WebUser',gaf)),
        retractall(mooCache(instance,surface,'instance'(Ctx,'Context'),'$VAR'(0),Context,'GlobalContext',TN2,'WebUser',gaf)),
        retractall(mooCache(instance,surface,'instance'('GlobalContext','Context'),'$VAR'(0),Context,'GlobalContext',TN2,'WebUser',gaf)),
        retractall(mooCache(instance,surface,'sourcefile-of'(Context,Filename),'$VAR'(0),'MooKernel','GlobalContext',TN3,'WebUser',gaf)),
        !.


% ===========================================================
% Context/Ctx ComboBoxs
% ===========================================================
show_available_contexts:-
        findall(Atom,
                (mooCache(PredR,surface,('instance'(Ctx,'Context'):_), Context,_, _, _, _),concat_atom([Context,':',Ctx],Atom)),
                List),
        writeq(List),nl.


getCtxListForContext(Context,['GlobalContext'|Sorted]):-
        findall(Ctx,mooCache(_,_,_, _, Context, _, _, _),UList),!,sort(UList,Sorted).
getCtxListForContext(Context,['GlobalContext']):-!.

show_available_contexts_in_combobox(Context,null):-
        getCtxListForContext(Context,List),
        toMarkup(html,select(ctx,List),_,Out),write(Out).

show_available_contexts_in_combobox(Context,Selected):-
        getCtxListForContext(Context,List),
        toMarkup(html,select(ctx,[Selected|List]),_,Out),write(Out).


getListOfContext(ContextList):-
        findall(Context,mooCache(_,_,_, _, Context, _, _, _),UList),!,sort(UList,ContextList).

show_available_theorys_in_combobox(Out):-
                findall(Context,mooCache(PredR,surface,('instance'(Context,'KnowledgeBase'):_), 'MooKernel',_, _, _, _),List),
                toMarkup(html,select(theory,List),_,Out).


% ===========================================================
% Draw Context DAG in HTML
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(submit=show_dag,Options),!,
        ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        ensureMooOption(Agent,user='Web',Agent),
        say('<pre>\n',[]),
        retractall(drawn(_)),
        draw_context_dag(Ctx,Context,0),!,
        say('</pre>\n',[]).

tcdag(C):-retractall(drawn(_)),draw_context_dag('LogicMOO',C,0).

:-dynamic(drawn/1).

draw_context_dag(Context,N):-
        assert(drawn(Ctx)),
        make_space(N,O),
        say('~w<strong>~w</strong>\n',[O,Ctx]),
        NN is N + 1,!,
        show_subs(Context,NN).

show_subs(Context,8):-!.
%show_subs(Top,Context,Top,_):-!.

show_subs(Context,NN):-
        context_dag(Context,Sub),
        not(drawn(Sub)),
        draw_context_dag(Context,Sub,NN),
        fail.
show_subs(Context,NN):-!.


make_space(1,' ->'):-!.
make_space(0,''):-!.
make_space(N,O):-
        NN is N -1,
        make_space(NN,M),
        atom_concat('   ',M,O),!.



