% ===================================================================
% File 'moo_api.pl'
% Maintainer: Douglas Miles
% Contact: dmiles@users.sourceforge.net ;
% Version: 'moo_api.pl' 1.0.0
% Revision:             $Revision: 1.1 $
% Revised At:   $Date: 2002-11-16 04:41:07 $

% ===================================================================
% PURPOSE
% ===================================================================
% This file meets the needs of an external agent working for the needs of eigther an automated or human user
% Interfacea with Java with XML to display explaination trees and variable bindings
% It defines the default settings most users will use and gives a starting expected state.
% Ask a Context, tell to a Context, retract from a Context, consult the user (this function is currently treated by ua_command/2, but it is planned to separate it),
% Report status to Moo, initialization of the LE, and file-handling routines
% ===================================================================

% ===================================================================
% EXPORTS   (Called  only by XSB Java server beans)
% ===================================================================
/*
:-module(moo_api,
            [
	    processRequest/1
/*
            ua_ask/3,        % handles user invoked request
            uaInsert/4,         % handles user invoked assertions
            ua_retract/4,    % handles user invoked retractions
            ua_command/2, %allows user to define new terms, and handles consultation with user

            define_theory/1,  %selects user's choice of a theory to load
            rename_theory/1,  %allows server to rename a theory
            reconstitute_theory/1,  %allows server to redefine the location of a theory
            delete_theory/1,  %allows server to delete a theory from the IE

            establish_status/1,
            establish_status/2,  %loads and unloads theorys, reports status to server,

            verify_status/1,
            verify_status/2,  %only reports status of theorys,

            server_startup_status/1,  %presets multiple theorys to be loaded at startup of IE,
            theory_startup_status/1,  %presets a single theory to be loaded at startup of IE,

            ua_read/2 % -- (to be dropped) loads a single theory
	    */
            ]).
     */

% :-include('moo_header.pl').

% ===================================================================
% Other major predicates:
% agentConsultation/3 (called by writeIfOption in moo_response.P, also by writeUAEvent, and by command_proc/3, by any predicate marked for consultation)
% ===================================================================

% ===================================================================
% DEBUG PREDICATES
% ===================================================================

fme2:-ua_ask("(?P2 ?A1 ?2)", 'GlobalContext', [opt_theory='PrologMOOAddition',disp_debug=true,disp_note_user=true,disp_notes_nonuser=true]).
fme3:-ua_ask("(?P3 ?A1 ?A2 ?A3)", 'GlobalContext', [opt_theory='PrologMOOAddition',disp_debug=true,disp_note_user=true,disp_notes_nonuser=true]).
gme2:-ua_ask("(genls ?X ?Y)").

% ===================================================================
% IMPORTS
% ===================================================================


% Stored persistantly as  theory_make_status_start(theory(Name,CanFileLocal)=StartupStatus)  in moo_persist.wfs


% ===================================================================
%  BOOTUP MOO SERVER
% ===================================================================

% This Function when called from outside does the Embeding the Logic Engine into the XSB Server
initializeMooServerData :-!,
     % findall(L,library_directory(L),Library_directory),
      sendNote(user,logicEngine,'Initializing and Embedding Moo Logic Engine',['architecture=',Architecture,Host_cpu,Version,Release_date,Scheduling_strategy]),
      establish_startup_state,
      sendNote(debug,logicEngine,'Contexts are now loaded by contentManager',' '),
      ensureMooContext('MooKernel','GlobalContext'),!.

%% :-include('moo_header.pl').

% ===========================================================
% CONVERSE WITH JAVA
% ===========================================================

processRequest(OptionsIn):-
                logOnFailure(fixOptionsFromForeign(OptionsIn,Options)),
                logOnFailure(setMooOption(Options)),
                ensureMooOption(client,html,ResponseType),
                logOnFailure(invokeRequest(Options)),!.


fixOptionsFromForeign([],[]):-!.

fixOptionsFromForeign([context=Value|List],[opt_ctx_assert=Value,opt_ctx_request=Value|ARGS]):-     %TODO Sepatate Context/Ctx
          fixOptionsFromForeign(List,ARGS).
fixOptionsFromForeign([ctx=Value|List],[opt_ctx_assert=Value,opt_ctx_request=Value|ARGS]):-     %TODO Sepatate Context/Ctx
          fixOptionsFromForeign(List,ARGS).
fixOptionsFromForeign([sf=AValue|List],[sf=AValue|ARGS]):-atom(AValue),!,
          logOnFailure(fixOptionsFromForeign(List,ARGS)).
fixOptionsFromForeign([sf=String|List],[sf=AValueClean|ARGS]):-is_list(String),
          logOnFailure(string_to_atom(String,AValueClean)),!,
          logOnFailure(fixOptionsFromForeign(List,ARGS)).
fixOptionsFromForeign([From=AValue|List],[To=DValue|ARGS]):-
          transform_option(From,To),!,
          logOnFailure(decodeRequestAtom(AValue,DValue)),
          fixOptionsFromForeign(List,ARGS).
fixOptionsFromForeign([AName=AValue|List],[DName=DValue|ARGS]):-
          logOnFailure(decodeRequestAtom(AName,DName)),
          logOnFailure(decodeRequestAtom(AValue,DValue)),
          logOnFailure(fixOptionsFromForeign(List,ARGS)).
fixOptionsFromForeign([AName|List],[DName|ARGS]):-
          decodeRequestAtom(AName,DName),
          logOnFailure(fixOptionsFromForeign(List,ARGS)).

% transform_option(From,To)
transform_option(author,user).
transform_option(theory,opt_theory).
transform_option(language,interp).
transform_option(timeLimit,opt_timeout).
transform_option(bindingLimit,opt_answers_max).
transform_option(depthLimit,opt_backchains_max).


invokeRequest(Options):-memberchk(cmd='Halt LE',Options),halt.
invokeRequest(Options):-memberchk(cmd='Rebuild',Options),write('Rebuilding Moo').


%invokeRequest(Options):-memberchk(command=html,Options),!,

invokeRequest(Options):-memberchk(client=soap,Options),!,
        parse_moo_soap(Options).

invokeRequest(Options):-memberchk(client=moo_xml,Options),!,
        parse_moo_soap(Options).

% ===========================================================
% Surface Returns External TN
% ===========================================================
invokeRequest(Options):-
   %attach_console,trace,
      memberchk(submit=tn_find,Options),!,
        ensureMooOption(opt_ctx_assert,'GlobalContext',Ctx),
        ensureMooOption(opt_theory,'PrologMOO',Context),
        ensureMooOption(client,'java',CLIENT),
        ensureMooOption(sf,surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        ensureMooOption(user,'Web',User),
         logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
         write_out_kif_tn(Assertion_Chars,NEWFORM,Vars,Ctx,Context,Maintainer),!.

write_out_kif_tn(Assertion_Chars,computed(comment(_)),Vars,Ctx,Context,Maintainer):-
         writeFmt('Syntax Error: Unmatched parentheses in "~s"\n',[Assertion_Chars]).
write_out_kif_tn(Assertion_Chars,comment(_),Vars,Ctx,Context,Maintainer):-
         writeFmt('Syntax Error: Unmatched parentheses in  "~s"\n',[Assertion_Chars]).
write_out_kif_tn(Assertion_Chars,NEWFORM,Vars,Ctx,Context,Maintainer):-
        copy_term((NEWFORM),(CNF)),
        numbervars(CNF,'$VAR',0,_),
        mooCache(PredR,surface,CNF, _, Context, TN, Maintainer, O),
        mooCache(PredR,surface,OF, _, Context, TN, Maintainer, O),
        numbervars(OF,'$VAR',0,_),
        OF == CNF,!,
        writeFmt('~w\n',[TN]),!.
write_out_kif_tn(Assertion_Chars,NEWFORM,Vars,Ctx,Context,Maintainer):-
         writeFmt('Not found in ~w of ~w "~s"\n',[Ctx,Context,Assertion_Chars]).

% ===========================================================
% Detroys an External TN
% ===========================================================
invokeRequest(Options):-memberchk(submit=tn_delete,Options),!,
        ensureMooOption(opt_ctx_assert,'GlobalContext',Ctx),
        ensureMooOption(opt_theory,'PrologMOO',Context),
        ensureMooOption(client,'java',CLIENT),
        ensureMooOption(tn,0,TN),
        ensureMooOption(sf,surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        ensureMooOption(user,'Web',User),
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
% Shows whats known about an External TN
% ===========================================================
invokeRequest(Options):-memberchk(submit=tn_show,Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_theory='PrologMOO',Context),
        ensureMooOption(client='java',CLIENT),
        ensureMooOption(tn=0,TN),
        ensureMooOption(sf=surf,Assertion),
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
        writeFmt('\n</pre><H3><br>Compiled Forms</H3><pre>\n',[]),fail.

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
        writeFmt('\n</pre><H3><br>Active Forms</H3><pre>\n',[]),fail.

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
        isMooOption(opt_debug=on),
        writeKnownFormsTN_used(Ctx,Context:TN).
*/
writeKnownFormsTN(Ctx,Context:TN):-writeFmt('</pre>').

writeKnownFormsTN_used(Ctx,Context:TN):-
        writeFmt('\n</pre><H3><br>Belief Forms</H3><pre>\n',[]),
        retractall(request_theory(X)),
        asserta(request_theory(Context)),
        mooCache(PredR,Fact,Ctx,Context:TN:_^Vars),
        t_ado_cache(PredR,_,Fact, Vars, Context, TN, Surf, on),
        writeFmt('\n<hr>'),fail.

writeKnownFormsTN_used(Ctx,Context:TN):-
        mooCache(PredR,Fact, Ctx, Pre, Context:TN:_^Vars),
        toMarkUp(html,formula(entails(Pre,Fact)),Vars,O),
        writeFmt('\n~w<hr>',[O]),fail.


% ===========================================================
% Ask
% ===========================================================
invokeRequest(Options):-memberchk(submit=ask,Options),!, make,
        ensureMooOption(opt_ctx_request='GlobalContext',Ctx),
        ensureMooOption(opt_theory='PrologMOO',Context),
        ensureMooOption(sf=surf,Askion),
        atom_codes(Askion,Askion_Chars),
        ensureMooOption(user='Web',User),
        ensureMooOption(interp='kif',Interp),
        logOnFailure(getCleanCharsWhitespaceProper(Askion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        logOnFailure(invokeOperation(quiet,request(NEWFORM),Ctx,TrackingAtom,Context,User,Vars)).


% ===========================================================
% Add
% ===========================================================
invokeRequest(Options):-memberchk(submit=tn_sync,Options),!,
       %  writeFmt(user_error,'ua: ~w\n',[Options]),flush_output(user_error),
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_theory='PrologMOO',Context),
        ensureMooOption(tn=_,EXTID),
        ensureMooOption(user='Web',User),
        ensureMooOption(sf=surf,Assertion),
        logOnFailure(atom_codes(Assertion,Codes)),
        getSurfaceFromChars(Codes,STERM,Vars),
        getMooTermFromSurface(STERM,Surface),
        destroyTN(Context,EXTID,_),
        once(invokeInsert([trusted,canonicalize],surface,Surface,Ctx,EXTID,Context,Vars,User)),!.

invokeRequest(Options):-memberchk(submit=tn_form_update,Options),!,
       %  writeFmt(user_error,'ua: ~w\n',[Options]),flush_output(user_error),
        ensureMooOption(tn=_,EXTID),
        ensureMooOption(user='Web',User),
        ensureMooOption(opt_theory='PrologMOO',Context),
        ensureMooOption(sf=surf,Assertion),
        logOnFailure(atom_codes(Assertion,Codes)),
        getSurfaceFromChars(Codes,STERM,Vars),
        getMooTermFromSurface(STERM,Surface),
        logOnFailure(retract(mooCache(PredR,surface,OldSurf,_,Context,EXTID,_,_))),
        destroyTN(Context,EXTID,_),
        once(invokeInsert([trusted,canonicalize],surface,Surface,Ctx,EXTID,Context,Vars,User)),!.

invokeRequest(Options):-memberchk(submit=canonicalize,Options),!,
        ensureMooOption(opt_theory='PrologMOO',Context),
        once(invokeContextCompilerProcess(Context)),write_ln('canonicalizing.\n').

invokeRequest(Options):-memberchk(submit=blank_theory,Options),!,
        ensureMooOption(opt_theory='PrologMOO',Context),
        destroyContext(Context),
        saveMooCache.


invokeRequest(Options):-memberchk(submit=removeTheoryNameSpace,Options),!,
        ensureMooOption(opt_theory='PrologMOO',Context),
        destroyContext(Context),
        retractall(mooCache(instance,surface,'instance'(Context,'KnowledgeBase'),'$VAR'(0),'MooKernel','GlobalContext',TN1,'WebUser',gaf)),
        retractall(mooCache(instance,surface,'instance'(Ctx,'Context'),'$VAR'(0),Context,'GlobalContext',TN2,'WebUser',gaf)),
        retractall(mooCache(instance,surface,'instance'('GlobalContext','Context'),'$VAR'(0),Context,'GlobalContext',TN2,'WebUser',gaf)),
        retractall(mooCache(instance,surface,'sourcefile-of'(Context,Filename),'$VAR'(0),'MooKernel','GlobalContext',TN3,'WebUser',gaf)),
        !.

% ===========================================================
% Verify Surface Returns SUO-KIF
% ===========================================================
invokeRequest(Options):-memberchk(submit=verify_assert,Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_theory='PrologMOO',Context),
        ensureMooOption(client='java',CLIENT),
        ensureMooOption(sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        ensureMooOption(user='Web',User),
         logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
         write_out_kif(Assertion_Chars,NEWFORM,Vars,Ctx,Context,Maintainer).

write_out_kif(Assertion_Chars,computed(comment(_)),Vars,Ctx,Context,Maintainer):-
         writeFmt('Syntax Error: Unmatched parentheses in "~s"',[Assertion_Chars]).
write_out_kif(Assertion_Chars,comment(_),Vars,Ctx,Context,Maintainer):-
         writeFmt('Syntax Error: Unmatched parentheses in "~s"',[Assertion_Chars]).
write_out_kif(Assertion_Chars,NEWFORM,Vars,Ctx,Context,Maintainer):-
        logOnFailure(getTruthCheckResults(tell,[untrusted],surface,NEWFORM,Ctx,STN,Context,Vars,Maintainer,Result)),
        (Result=accept(_) ->
                        (toMarkUp(kif,NEWFORM,Vars,Out),writeFmt('~w\n',[Out]),!)
                        ;
                        (
                        Result=notice(FormatStr,Args),
                        writeFmt('error:\n',[]),
                        writeFmt(FormatStr,Args)
                        )
        ),!.

% ===========================================================
% Draw Context DAG in HTML
% ===========================================================
invokeRequest(Options):-memberchk(submit=show_dag,Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_theory='PrologMOO',Context),
        ensureMooOption(user='Web',User),
        writeFmt('<pre>\n',[]),
        retractall(drawn(_)),
        draw_context_dag(Ctx,Context,0),!,
        writeFmt('</pre>\n',[]).

tcdag(C):-retractall(drawn(_)),draw_context_dag('PrologMOO',C,0).

:-dynamic(drawn/1).

draw_context_dag(Context,N):-
        assert(drawn(Ctx)),
        make_space(N,O),
        writeFmt('~w<strong>~w</strong>\n',[O,Ctx]),
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



% ===========================================================
% Insert
% ===========================================================
invokeRequest(Options):-memberchk(submit=assert,Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_theory='PrologMOO',Context),
        ensureMooOption(sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        ensureMooOption(user='Web',User),
        ensureMooOption(interp='kif',Interp),
        logOnFailure(ensureMooOption(tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        logOnFailure(invokeOperation(verbose,assert(NEWFORM),Ctx,TN,Context,User,Vars)).


% ===========================================================
% Start Canonicalizer
% ===========================================================
invokeRequest(Options):-memberchk(submit=canonicalize,Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_theory='PrologMOO',Context),
        logOnFailure(invokeContextCompilerProcess(Context)).


% ===========================================================
% Create Knowledge Base (New)
% ===========================================================
invokeRequest(Options):-memberchk(submit=register_theory,Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_theory='PrologMOO',Context),
        ensureMooOption(client='java',CLIENT),
        ensureMooOption(user='Web',User),
        saveMooCache,
        getDefaultImageFilepath(IF),
        open(IF,append,Handle,[close_on_abort(false),buffer(full)]),
        assert(save_can_to_file(Context,Handle)),
        !.

:-dynamic(save_can_to_file/2).

% ===========================================================
% Retract
% ===========================================================
invokeRequest(Options):-memberchk(submit=retract,Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_theory='PrologMOO',Context),
        ensureMooOption(sf=surf,Retraction),
        atom_codes(Retraction,Retraction_Chars),
        ensureMooOption(user='Web',User),
        ensureMooOption(interp='kif',Interp),
        writeDebugWindow,
         getCleanCharsWhitespaceProper(Retraction_Chars,Show),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
              once(( NEWFORM=comment(_) ->
                     (do_chars(Show),!,FORM=_) ;(!,
                     logOnFailure(invokeOperation(verbose,retract(NEWFORM),Ctx,TN,Context,User,Vars))
                     ))).


% ===========================================================
% Delete Assertion
% ===========================================================
invokeRequest(Options):-memberchk(submit='Delete Assertion',Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_theory='PrologMOO',Context),
        ensureMooOption(asid=_,AID),
        ensureMooOption(user='Web',User),
        ensureMooOption(interp='kif',Interp),
        writeFmt('<H3><Font Color=Red>Deleted....</Font></H3>',[]),
        delete_assertion(AID).

delete_assertion(AID):-
        retract(mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,_)),
        delete_assertion_disp(Form,SURF,Vars,Context,AID,Maintainer),
        fail.

delete_assertion(AID):-writeFmt('<H3><Font Color=Red>Done Deleting.</Font></H3>',[]).

delete_assertion_disp(Form,SURF,Vars,Context,AID,Maintainer):-
        toMarkUp(html,SURF,Vars,SAtom),
        writeFmt('<IMG src="pixmaps/bullet.gif" asrtid=~w><nobr>',[AID]),
        writeFmt('<b>~w</b> ID<font color=red>~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font>',[Form,AID,Context,Maintainer]),
        %format_o('&nbsp;&nbsp;~w&nbsp;Enabled&nbsp;&nbsp;<br>',chectheoryox(AID,OnOff)),
        writeFmt('~w<br>',[SAtom]),!.

invokeRequest(Options):-
        memberchk(cmd='Show Cache',Options),!,
        writeFmt('<H3><Font Color=Red>Listing Cache Assertions...</Font></H3><PRE>',[]),
        listing(mooCache),
        writeFmt('</PRE><BR><B>Done.</B>',[]),!.


listingt(NH):-catch((string_to_atom(NH,AA),listing(AA)),_,true).
%listingt(NH):-listing(NH),!.

% ===========================================================
% Invoke Prolog Command
% ===========================================================
invokeRequest(Options):- %memberchk(interp='prolog',Options),!,
        memberchk(submit=command,Options),!, make,
        ensureMooOption(sf=surf,Prolog),
        ignore(parse_prolog_cmd(Prolog)).

parse_prolog_cmd(Prolog):-
        writeDebugWindow,
	writeFmtFlushed('<pre>') ,
        catch(atom_to_term(Prolog,CMD,Vars),E,
        (message_to_string(E,S),writeFmt('\nCall "~w" could not be read.  \nError: ~s\n',[Prolog,S]))),!,
        callFromWeb(Prolog,CMD,Vars),!.
parse_prolog_cmd(Prolog):-writeFmt('\nCall "~w" failed',[Prolog]),!.
callFromWeb(Prolog,CMD,Vars):-var(CMD),!.
callFromWeb(Prolog,CMD,Vars):-
        getThread(Id),
        ensureMooOption('$socket_out',user_output,Out),
        ensureMooOption('$socket_in',user_input,In),
        invokePrologCommandRDF(Id,In,Out,CMD,Vars,Results),
        writeFmt('\n       Results: ~w\n',[Results]),!.




% ===========================================================
% Logical Form Display
% ===========================================================
invokeRequest(Options):-memberchk(logicforms=logicforms,Options),!,
       parse_moo_lf(Options).


% ===========================================================
% Un-Canonicalize on Context/Ctx
% ===========================================================

invokeRequest(Options):-
        memberchk(cmd='Un-Canonicalize',Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_theory='PrologMOO',Context),!,
        (unsetMooOption(opt_surface_check=_)),
        (setMooOption(opt_surface_check=untrusted)),
        (unsetMooOption(opt_tracking_number=_)),
        (setMooOption(opt_tracking_number=supplied)),!,
        (unsetMooOption(opt_canonicalizer=_)),
        (setMooOption(opt_canonicalizer=byrd)),!,
        writeFmt('<H2>Un-Canonicalizing  Context:~w Ctx:~w.  This process may take several minutes.. Do not navigate away.</H2>',[Context,Ctx]),
        mark_all_surface_to_uncanonicalized(Context,Ctx),!.

% ===========================================================
% Invoke Load SContext on Context/Ctx (POST)
% ===========================================================

invokeRequest(Options):-
                memberchk(context='Load SContext',Options),!,
                (unsetMooOption(opt_theory=_)),
                (setMooOption(opt_theory=Context)),
                ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
                ensureMooOption(opt_theory='PrologMOO',Context),!,
                fmtString(FileChars,'C:/mool/SUO/~w.can',[Context]),!,string_to_atom(FileChars,Filename),!,
                (unsetMooOption(opt_surface_check=_)),
                (setMooOption(opt_surface_check=trusted)),
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
invokeRequest(Options):-memberchk(cmd='Load SContext',Options),!,
                ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
                ensureMooOption(opt_theory='PrologMOO',Context),!,
                fmtString(FileChars,'C:/mool/SUO/~w.can',[Context]),!,string_to_atom(FileChars,Filename),!,
                (unsetMooOption(opt_surface_check=_)),
                (setMooOption(opt_surface_check=trusted)),
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
invokeRequest(Options):-
        memberchk(cmd='Canonicalize',Options),!,%true,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_theory='PrologMOO',Context),!,
        (unsetMooOption(opt_surface_check=_)),
        (setMooOption(opt_surface_check=untrusted)),
        (unsetMooOption(opt_tracking_number=_)),
        (setMooOption(opt_tracking_number=supplied)),!,
        (unsetMooOption(opt_canonicalizer=_)),
        (setMooOption(opt_canonicalizer=byrd)),!,
        writeFmt('<H2>Canonicalizing  Context:~w Ctx:~w.  This process may take several minutes.. <br>This process must be done once and should not be interupted<br>Wait until this page is completely loaded before clicking <br>any links and do not navigate away.</H2>',[Context,Ctx]),
        logOnFailure(canonicalizeMooContextHTML(Context,Ctx)),!.

% ===========================================================
% Invoke Canonicalizer on Context/Ctx (POST)
% ===========================================================
invokeRequest(Options):-
        member(Context='Canonicalize',Options),!,
        (unsetMooOption(opt_theory=_)),
        (setMooOption(opt_theory=Context)),
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        (unsetMooOption(opt_surface_check=_)),
        (setMooOption(opt_surface_check=untrusted)),
        (unsetMooOption(opt_tracking_number=_)),
        (setMooOption(opt_tracking_number=supplied)),!,
        (unsetMooOption(opt_canonicalizer=_)),
        (setMooOption(opt_canonicalizer=byrd)),!,
        writeFmt('<H2>Canonicalizing  Context:~w Ctx:~w.  This process may take several minutes.. <br>This process must be done once and should not be interupted<br>Wait until this page is completely loaded before clicking <br>any links and do not navigate away.</H2>',[Context,Ctx]),
        logOnFailure(canonicalizeMooContextHTML(Context,Ctx)),!.


% ===========================================================
% Invoke Surface Checker on Context/Ctx
% ===========================================================
invokeRequest(Options):-
        memberchk(cmd='Surface Check',Options),!,%true,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_theory='PrologMOO',Context),!,
        logOnFailure(surface_check_entire_theory_ctx(Context,Ctx)),!,
        writeFmt('<hr>Done Surface Check\n',[]).

% ===========================================================
% Invoke Prolog Command
% ===========================================================
invokeRequest(Options):- %memberchk(interp='prolog',Options),!,
        memberchk(submit='Clean Slate',Options),!,halt.

% ===========================================================
% Invoke Prolog Command
% ===========================================================
invokeRequest(Options):- %memberchk(interp='prolog',Options),!,
        memberchk(cmd='Compile Demo',Options),!,autoload,
        (unsetMooOption(client=html)),qsave_program('moo_server',[goal=(initializeMooServerData,cs)]),(setMooOption(client=html)).

build_exe_and_halt:- !,autoload,
        (unsetMooOption(client=html)),qsave_program('moo_server',[goal=(initializeMooServerData,cs)]).

invokeRequest(Options):-invokeBrowserRequest(Options),!.



% ===========================================================
% Backup Moo
% ===========================================================
invokeRequest(Options):-memberchk(cmd='Save Image',Options),!,saveMooCache.

% ===========================================================
% Define New context
% ===========================================================
invokeRequest(Options):-memberchk(client='command',Options),memberchk(submitbuttonUp='Create',Options),!,
                        ensureMooOption(newCtxName='GlobalContext',Ctx),
                        ensureMooOption(theory='PrologMOO',Context),
                        ensureMooOption(user='Web',User),
                        idGen(TN2),
                        assertaClean(mooCache(instance,surface,'instance'(Ctx,'Context'),'$VAR'(0),Context,'GlobalContext',TN2,User,on)).


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
        toMarkUp(html,select(ctx,List),_,Out),write(Out).

show_available_contexts_in_combobox(Context,Selected):-
        getCtxListForContext(Context,List),
        toMarkUp(html,select(ctx,[Selected|List]),_,Out),write(Out).


getListOfContext(ContextList):-
        findall(Context,mooCache(_,_,_, _, Context, _, _, _),UList),!,sort(UList,ContextList).

show_available_theorys_in_combobox(Out):-
                findall(Context,mooCache(PredR,surface,('instance'(Context,'KnowledgeBase'):_), 'MooKernel',_, _, _, _),List),
                toMarkUp(html,select(theory,List),_,Out).

% =================================================
% Useragent Control Panel
% =================================================
invokeRequest(Options):-memberchk(client='controlpanel',Options),
        memberchk(killable=_,Options),!,
        member(killable=ID,Options),
        catch(prolog_thread_signal(ID,prolog_thread_exit(user_killed(ID))),_,true),
        invokeRequest([client='controlpanel']).


invokeRequest(Options):-memberchk(client='controlpanel',Options), !,
        showMooProcessHTML.

% ===========================================================
% TQ HOOK
% ===========================================================
invokeRequest(Options):-memberchk(client='tqsystem',Options),!,ignore(invokeTestSystem(Options)).

% ===========================================================
% FALLBACKS
% ===========================================================

invokeRequest(Options):-memberchk(client=command,Options),!.

invokeRequest(Options):-%true,
   writeFmt('
<html>

<head>
<meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
<meta name="Keywords" content="PROLOG Artificial Intelligence Ontology AI MOO Linguistics Philosophy DARPA Doulgas Miles">
<meta name="Description" content="PROLOG Artificial Intelligence Ontology AI MOO Linguistics Philosophy DARPA">
<title>LogicMOO request ~q</title>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</head>

<body>
',[Options]),
%showMooProcessHTML,
write('
<pre>
                               _)\\.-.
              .-.__,___,_.-=-. )\\`  a`\\_
          .-.__\\__,__,__.-=-. `/  \\     `\\
          {~,-~-,-~.-~,-,;;;;\\ |   \'--;`)/
           \\-,~_-~_-,~-,(_(_(;\\/   ,;/
            &quot;,-.~_,-~,-~,)_)_)\'.  ;;(               W E L C O M E
              `~-,_-~,-~(_(_(_(_\\  `;\\                   t o
        ,          `&quot;~~--,)_)_)_)\\_   \\               J a M U D
        |\\              (_(_/_(_,   \\  ;
        \\ \'-.       _.--\'  /_/_/_)   | |
         \'--.\\    .\'          /_/    | |
             ))  /       \\      |   /.\'
            //  /,        | __.\'|  ||
           //   ||        /`    (  ||           the java-based mud in
          ||    ||      .\'       \\ \\\\           T E C H N I C O L O R
          ||    ||    .\'_         \\ \\\\	       (Web Interface)
           \\\\   //   / _ `\\        \\ \\\\__
     jgs    \'-\'/(   _  `\\,;        \\ \'--:,
             `&quot;`  `&quot;` `-,,;         `&quot;`&quot;,,;</pre>
<form method="GET" action="login">
Player Name:&nbsp;&nbsp;<input type="text" name="username" size="20">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Password:&nbsp;&nbsp;<input type="text" name="password" size="20">&nbsp;&nbsp;&nbsp;
<input type="submit" value="Login" name="loginbutton">
</form>
</body>

</html>
'),   showMooStatisticsHTML,writeDebugWindow.



% ===========================================================
% EDITOR WINDOW
% ===========================================================

writeEditorLogicWindow:-!,
        ensureMooOption(sf='(isa ?X ?Y)',Formula),
        writeFmt('<textarea rows=6 cols=90 name="sf">~w</textarea><br>',[Formula]),
        writeFmt('<br><INPUT type=submit name=submit value="Update Source"></INPUT><hr>',[]),
        writeFmt('<INPUT type=radio name="interp" value="kif" checked>KIF</INPUT>',[]),
        writeFmt('<INPUT type=radio name="interp" value="ace">ACE</INPUT>',[]).

writeDebugWindow:-!,
        ensureMooOption(sf='(isa ?X ?Y)',Formula),
	writeFmt('<hr><FORM name="theForm" ID="theForm"  METHOD="GET">
		<textarea rows="5" cols="70" name="sf">~w</textarea><br>
		<INPUT type="hidden" name="interp" value="kif" checked>
		<INPUT type="submit" name="submit" value="command"></INPUT></form>',[Formula]).


% ===================================================================================================
%  End of moo_useragent.P
% ===================================================================================================

getTheoryStatus('MooKernel'):-writeFmt('Browse Only\n',[]),!.
getTheoryStatus(Context):-
        isContextCompilerProcess(Context,Progress),
        writeFmt('<A href=controlpanel.jsp?theory=~w><font color=green>Canonicalizing</font></a>\n',[Context]),!.

getTheoryStatus(Context):-
        isUncanonicalized(Context),
        invokeContextCompilerProcess(Context),
        isContextCompilerProcess(Context,Progress),
        writeFmt('<A href=controlpanel.jsp?theory=~w><font color=green>Canonicalizing</font></a>\n',[Context]),!.

getTheoryStatus(Context):-
        isUncanonicalized(Context),
        isContextCurrentlyInUse(OtherContext,Status),!,
        writeFmt('<font color=orange>Waiting for ~w</font>\n',[OtherContext]).

getTheoryStatus(Context):-
        isContextCurrentlyInUse(Context,Status),
        writeFmt('<font color=orange>Busy ~w</font>\n',[Status]).

getTheoryStatus(Context):-
        isTheoryUntransfered(Context),
        writeFmt('<A href=stheory_to_prolog.jsp?theory=~w><font color=red>Update</font></a>\n',[Context]),!.

getTheoryStatus(Context):-
        isSourceNewerThenImage(Context),
        writeFmt('<A href=stheory_to_prolog.jsp?theory=~w><font color=red>Needs Update</font></a>\n',[Context]),!.

getTheoryStatus(Context):-isKnowledgeBaseLoaded(Context,_),writeFmt('<A href="askInsert.jsp?theory=~w">Ask/Insert</A>\n',[Context]),!.

getTheoryStatus(Context):-writeFmt('Unknown\n',[]),!.

% ===================================================================
%  INVOKE REQUEST INTERFACE USED BY REQUEST AGENTS
% ===================================================================

%% :-include('moo_header.pl').

% =======================================================================================

invokeRequestAndWriteUserAgent(Formula,Ctx,TrackingAtom,Context,User,Vars,CPU):-
         invokeRequestToBuffer(Formula,Ctx,TrackingAtom,Context,User,Vars,CPU),
         writeUserAgentBuffer.


% ===========================================================
% Cite Buffer
% ===========================================================
writeUserAgentBuffer:-%true,
        retract(requestBuffer_db(UResultsSoFar,Result,Explaination,Status)),
        once(writeAnswersUserAgent(UResultsSoFar,Result,Explaination,Status)),fail.

% Call to write Summary
writeUserAgentBuffer:-
        final_answer(Logic:How),
        writeDebug(final_answer(Logic:How)),
        writeAnswersUserAgent(How, ['Summary'=Logic|_G14093],final_answer(Logic:How),final_answer(Logic:How) ).

writeUserAgentBuffer:-!.


% ===========================================================
% Send to debugger
% ===========================================================
writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status):-
        ((once(writeDebug(writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status))),fail)).

% ===========================================================
% Hide certain returns
% ===========================================================
writeAnswersUserAgent(-1,Result,Explaination,Status).

writeAnswersUserAgent(0, ['Result'=none|A], 'Unproven', done(possible:searchfailed)).
writeAnswersUserAgent(_, ['Result'=true|A], found(_), done(true:_)).

% ===========================================================
% Write Summaries
% ===========================================================
writeAnswersUserAgent(_, ['Summary'=true|_G5892], _, final_answer(Logic:NA)):-
        writeObject('$spacer',Vars), retain_yes,
        request_total_time(Total),
        writeObject(nv([' Yes. (Found ',NA,Logic,' in ',Total,' seconds.)']),['Total'=Total,'Answers'=NA,'Logic'=Logic]),!.

writeAnswersUserAgent(Explaination, ['Summary'=false|_G5282], final_answer(Logic:Request:P), _):-
        writeObject('$spacer',Vars), retain_no,
        request_total_time(Total),
        writeObject(nv([' No. (Found ',Logic,' in ',Total,' seconds.)']),['Total'=Total,'Answers'=NA,'Logic'=Logic]),!.

writeAnswersUserAgent(searchfailed, ['Summary'=Logic|_G4695], final_answer(possible:searchfailed), final_answer(possible:searchfailed)):-
        writeObject('$spacer',Vars), retain_unproven,
        request_total_time(Total),
        writeObject(nv([' Unproven. (Found no answers in ',Total,' seconds.)']),['Total'=Total,'Answers'=NA,'Logic'=Logic]),!.

% ===========================================================
% Write Answers
% ===========================================================
:-dynamic(isMooOption(show_all_explainations,true)).


/*
writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status):-
        format('<b>~q.</b>',writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status)),!.
*/

writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status):-not(isMooOption(show_all_explainations,true)),
        writeObject('$spacer',Vars),
        %writeObject(writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status),Vars),nl,
        writeObject(getPrologVars(Result),Vars),
        length_explaination(InExplaination,InLength),
        findall(Length-Explaination,
                (retract(requestBuffer_db(_,Result,Explaination,_)),
                 length_explaination(Explaination,Length)
                 ),KeyList),
        keysort([(InLength-InExplaination)|KeyList],[(_-ChoiceExplaination)|_]),
        writeObject(explaination(ChoiceExplaination),Result).

writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status):-isMooOption(show_all_explainations,true),
        writeObject('$spacer',Vars),
        writeObject(getPrologVars(Result),Vars),!,
        writeObject(explaination(InExplaination),Vars),!.

retain_no:-retain_answer('No'),retain_answer('no'). %,retain_answer('Unproven').
retain_yes:-retain_answer('Yes'),retain_answer('yes'). % ,retain_answer('Unproven').
retain_unproven:-retain_answer('Unproven'),retain_answer('unproven'). % ,retain_answer('Unproven').



length_explaination(List,Len):-length(List,Len),!.
length_explaination(deduced,3):- !.
length_explaination(P * Explaination,Length):- !,
        length_explaination(P,PLength),
        length_explaination(Explaination,ExplainationLength),
        Length is PLength + ExplainationLength,!.
length_explaination(_,1):- !.


unset_promiscuous:-
                setMooOption(opt_deduce_domains='off').
set_promiscuous:-
                setMooOption(opt_deduce_domains='on').


% ===================================================================
% EXPORTS
% ===================================================================

register_belief_module(_ModuleName).


/*
Loads and compiles a Maintainerial KIF file
agentLoadKif(+Filename,+GlobalContext,+DefiningMaintainer).
*/

agentLoadKif(Filename):-
        set_automation,
        agent_load_kif(Filename,Filename,Filename).

agentLoadKif(Filename,GlobalContext,User):-
        set_automation,
        agent_load_kif_quiet(Filename,GlobalContext,User).


/*
Saves state of a Contet
agentSave(+FileName,+GlobalContext)
*/



/*

Loads the saved state (crypted or not)  (adds assertions) they were originally stored with a context
interfaceLoad(+FileName)
Clears a context (Leave unbound to clear all contexts)
u_clear(?Context)
We still have some discussion on how contexts are linked together so right now they are all active..
Mainly it gives us a way to do bulk load/unloads and separation user data from general system's like STANDARD.

*/



/* Assertion
agentInsert(+KIFCharsIn,+Ctx,+Context,+User).
KIFCharsIn = "(instance virginia Computer)"
Ctx = 'Computers and Things'
Context = 'PrologMOO' currently must be set to 'PrologMOO'
*/

agentInsert(KIFCharsIn,Ctx,Context,User):-
        set_automation,
        agentInsert(KIFCharsIn,Ctx,TN,Context,User).


/*
Now the predicates that you most want
Invokes a request
agentRequest(+KIFCharsIn,+Ctx,+Context,+User,-UResultsSoFar,-Result,-Explaination,-Status).

Input variables:
KIFCharsIn = "(instance ?What Computer)"
Ctx = 'Computers and Things'  % Any legal Atom that helps you book keep
Context = 'PrologMOO' currently must be set to 'PrologMOO'
User = ''  There are times when error messages about redundant assertions happen.. and for bookkeeping all assertions have an author atom.. perhaps you can even use the source file name it came from.

output variables:
UResultsSoFar = 0 indexed value of answer number.. +1 would mean it is a repeated answer with a different explaination.. you could choose not to display answers with negative numbers
Result = Is the Prolog Variable Vector like ['What'=virginia|_] (can be written with writeObject(Verbose,getPrologVars(Result),Result))
Explaination = A explaination object that can be printed with writeObject(Verbose,explaination(Explaination),Result)
Status = is either 'true' or '!' ... true = more results.. '!' means no more results
(Status is optional.. if you completely remove this argument and it will fail at the next call after the first solution returning !)
*/




/*
% Lets you discover any errors that were created durring any operation
popError(Message)
+Will fail if no errors are in this buffer
+When any of the above commands are first invoked the error buffer is cleared
+Whenever there were errors and you read this buffer each one is deleted while it is read
+Messages are stored as printable objects writeObject(Verbose,Message)  we can discuss a more programmatic approach to formats later based on how we want to use them.
*/
:-dynamic(mooErrorBuffer/1).
popError(Message):-retract(mooErrorBuffer(Message)).

/*
Term Writting
writeObject(Verbose,+Object).
writeObject(Verbose,+Object,+Vars).
Allows easy printing of variables and objects
*/


/*
Belief Variables

Set Belief Optional Variable
setMooOption(VariableName=NewValue).
*/

/*
Retrive Belief Optional Variable
isMooOption(VariableName=CurrentValue).
*/

/*
The defualts are shown here
opt_timeout=5  The request system will try for 5 seconds before stopping (RealNumber in seconds)
opt_answers_min=1 The system will at minumum try to come up with 1 answer
opt_answers_max=99 The system will at maximun will try to come up with 99 answers
opt_backchains_max=50 will try 50 prolog backchains aprocimately 12 authorial backchains on each deduction
opt_deductions_max=100 The system will try to come up with 100 non unique answers before stooping
*/

set_automation:-
                retractall(mooErrorBuffer(_)),
               !.
               % (unsetMooOption(client=_)),
                %(setMooOption(client=automata)).


% ===================================================================
%  BATCH INTERFACE
% ===================================================================

invokeOperation(Verbose,surf,Ctx,Tracking,Context,User,Vars):-!.
invokeOperation(Verbose,Fml,Ctx,TN,Context,User,Vars):-!,
                once(invokeOperationProc(Verbose,Fml,Ctx,TN,Context,User,Vars)),!.

invokeOperationProc(Verbose,TERM,Ctx,TN,Context,User,Vars):-TERM =..[note|T],!,
                assert(title(T)),
                WT=..['note '|T],
                writeObject(Verbose,WT,Vars),!,
                writeObject(Verbose,'\n\n;; Assertions \n\n',Vars),!,
                clear_moo_memory,set_promiscuous,!.

invokeOperationProc(Verbose,TERM,Ctx,TN,Context,User,Vars):-TERM =..[answer|T],!,batch_answer(T).

invokeOperationProc(Verbose,TERM,Ctx,TN,Context,User,Vars):-TERM =..[abort],!,assert(tq_ignored).
invokeOperationProc(Verbose,'deduce-domains'(on),Ctx,TN,Context,User,Vars):-!,set_promiscuous,!.
invokeOperationProc(Verbose,'deduce-domains'(true),Ctx,TN,Context,User,Vars):-!,set_promiscuous,!.
invokeOperationProc(Verbose,'deduce-domains'(false),Ctx,TN,Context,User,Vars):-!,unset_promiscuous,!.
invokeOperationProc(Verbose,'deduce-domains'(off),Ctx,TN,Context,User,Vars):-!,unset_promiscuous,!.

invokeOperationProc(Verbose,end_of_file,Ctx,TN,Context,User,Vars):-!.

invokeOperationProc(Verbose,'file_comment'(C),SourceCtx,SourceTN,Context,User,Vars):-!,nl,write(C),!.
invokeOperationProc(Verbose,'browser-only'(_),SourceCtx,SourceTN,Context,User,Vars):-!.
invokeOperationProc(Verbose,'execute-prolog'(Code),SourceCtx,SourceTN,Context,User,Vars):-!,
once(((%                writeq(Code),nl,
                atom_codes(Code,Codes),
%               writeq(Codes),nl,
                getUnquotedCodes(Codes,Chars),
%               writeq(Chars),nl,
                atom_codes(Atom,Chars),
                atom_to_term(Atom,Term,V)

                );writeFmt('could not parse ~w\n',[Code])),once(((once(catch(Term,_,fail)),writeFmt('Exec Suceeded',[]));writeFmt('Exec Failed',[])))).
invokeOperationProc(Verbose,'load-theory'(Filename),SourceCtx,SourceTN,Context,User,Vars):-!,fail,agent_load_kif_quiet(Filename,'GlobalContext','Anonymous').
invokeOperationProc(Verbose,'load-theory-show'(Filename),SourceCtx,SourceTN,Context,User,Vars):-!,
        agent_load_kif(Filename,'GlobalContext','Anonymous').

invokeOperationProc(Verbose,retract(nil),Ctx,TN,Context,User,Vars):-!.

invokeOperationProc(Verbose,request(nil),Ctx,TN,Context,User,Vars):-!.

invokeOperationProc(Verbose,(nil),Ctx,TN,Context,User,Vars):-!.

invokeOperationProc(Verbose,tell(nil),Ctx,TN,Context,User,Vars):-!.

invokeOperationProc(Verbose,assert(end_of_file),Ctx,TN,Context,User,Vars):- !.

invokeOperationProc(Verbose,assert(NEWFORM),Ctx,TN,Context,User,Vars):-  !,
        invokeOperationProc(Verbose,assert([trusted,canonicalize,to_mem],NEWFORM),Ctx,TN,Context,User,Vars).

invokeOperationProc(Verbose,assert(Flags,NEWFORM),Ctx,TN,Context,User,Vars):-  !,
        flag('Axioms Compiled',AC,AC+1),
        LN is AC + 1,
        flag(explaination_linenumber,_,LN),
        writeObject(Verbose,nl,Vars),
        writeObject(Verbose,NEWFORM,Vars),
        writeObject(Verbose,nl,Vars),
        logOnFailure(invokeInsert(Flags,surface,NEWFORM,Ctx,TN,Context,Vars,User)),
        ignore((getMooOption(opt_debug=off,on),Verbose=verbose,writeKnownFormsTN(Ctx,Context:TN))).


invokeOperationProc(Verbose,retract(Fml),Ctx,TN,Context,User,Vars):-
         flag('Axioms Compiled',AC,AC+1),
         writeObject(Verbose,nl,Vars),
         writeObject(Verbose,retract(Fml),Vars),
         writeObject(Verbose,nl,Vars),
         retract_pterm(Fml,Ctx,TN,Context,User,Vars).

invokeOperationProc(Verbose,canonicalize(Q),Ctx,TN,Context,User,Vars):-!,
         writeObject(Verbose,nl,Vars),
         writeObject(Verbose,canonicalize(Q),Vars),
         invokeRequestWithTime(canonicalize(Q),_,Ctx,TN,Context,User,Vars,CPU1), assert(findings(CPU1)).

invokeOperationProc(Verbose,request(Q),Ctx,TN,Context,User,Vars):-!,
         writeObject(Verbose,'\n\n\n;; Request \n\n',Vars),
         writeObject(Verbose,'request '(Q),Vars),
         writeObject(Verbose,nl,Vars),
         invokeRequestWithTime(request(Q),Ctx,TN,Context,User,Vars,CPU1), assert(findings(CPU1)).

invokeOperationProc(Verbose,request(N,Q),Ctx,TN,Context,User,Vars):-!,
         writeObject(Verbose,'\n\n\n;; Timed Request \n\n',Vars),
         writeObject(Verbose,'request '(Q),Vars),
         writeObject(Verbose,nl,Vars),
         invokeRequestWithTime(request(N,Q),Ctx,TN,Context,User,Vars,CPU1), assert(findings(CPU1)).

invokeOperationProc(Verbose,requestyn(Q),Ctx,TN,Context,User,Vars):-!,
         writeObject(Verbose,'\n\n\n;; Yes/No Request \n\n',Vars),
         writeObject(Verbose,'request '(Q),Vars),
         writeObject(Verbose,nl,Vars),
         invokeRequestWithTime(requestyn(Q),Ctx,TN,Context,User,Vars,CPU1), assert(findings(CPU1)).


invokeOperationProc(Verbose,Fml,Ctx,TN,Context,User,Vars):-               % Default Left-over
             toFSCK(Fml,Ctx,TN,Assertion,SourceCtx,SourceTN),
             invokeOperation(Verbose,assert(Assertion),SourceCtx,SourceTN,Context,User,Vars).

invokeRequestWithTime(Q,Ctx,TN,Context,User,Vars,CPU):-
                invokeRequestAndWriteUserAgent(Q,Ctx,TN,Context,User,Vars,CPU),
                assert(request_time(CPU)).


% ===============================================================
% AGENT REQUEST (Chars)
% ===============================================================

tkq1:-agentRequest("(isa Joe ?Class)",'GlobalContext','PrologMOO','Dmiles',U,R,P).
tkq2:-agentRequest("(isa on Relation)",'GlobalContext','PrologMOO','Dmiles',U,R,P).


agentRequest(KIFCharsIn,Ctx,Context,User,UResultsSoFar,Result,Explaination):-
        agentRequest(KIFCharsIn,Ctx,Context,User,UResultsSoFar,Result,Explaination,Status),
        (Status = done(_) -> ! ; true).


agentRequest(KIFCharsIn,Ctx,Context,User,UResultsSoFar,Result,Explaination,Status):-
        isCharCodelist(KIFCharsIn),!,
        string_clean(KIFCharsIn,KIFChars),
        logOnFailure(ask_parse_chars(KIFChars,FmlInOpen,Vars)),!,
        agentRequest(KIFCharsIn,FmlInOpen,Vars,Ctx,Context,User,UResultsSoFar,Result,Explaination,Status).


agentRequest(KIFCharsIn,FmlInOpen,Vars,Ctx,Context,User,UResultsSoFar,Result,Explaination,Status):-
        ((
        retractall(answer_found(_)),
        retractall(t_answer_found(_)),
        retractall(tabled_f(_)),
        retractall(mooCache(deduceInstanceTable(Context,Class,Set))),
        ignore(findall(_,expireOptimizationsInContext(_,_,_),_)),
        retractall(table_g(_)),
        retractall(cpuend(_)),
        retractall(tabled_t(_)),
        retractall(table_g(_)),
        retractall(proving(_)),
        getMooOption(opt_theory,QContext),!,ignore((Context=QContext)),!,
        get_default_request_context(QCTX),!,ignore((Ctx=QCTX)),!,
        logOnFailure(ensureMooContext(Context,Ctx)),!,
        flag('$Returned Answers',_,0),
        flag('$UAnswers',_,0))),
        TN = User, % Tracks request based on 'User'
        destroyTN(Context,TN,_Ctx),  % Removes previous Request
        getOpenVariablesWFF(FmlInOpen,Vars,ChaseVars),
        getPrologVars(ChaseVars,QVars,_,_),
        RequestPrototype=..[request|QVars],
        ((not(not((
                getAssertionClauses(Context,'<=>'(FmlInOpen,RequestPrototype),CAN,Vars,Flags),
                assert(mooCache(FmlInOpen,CAN,Flags,Vars,Context,TN,User,Result)),!,
                (recanonicalizeTN(Context,TN)),    % Compile Belief
                assert(tq_attempted_request),
                writeDebug(blue,'Stage 1 - Compilation ':CAN:Flags)))))),!,
        agentRequestEach(FmlInOpen,RequestPrototype,ChaseVars,Ctx,Context,User,UResultsSoFar,Result,Explaination,Status),
        commitCleanExplaination(Explaination,ExplainationOut).


% ======================================================
% REQUEST PROC POSITIVE/NEGATIVE
% ======================================================

% Check For Undefines
agentRequestEach(KIFCharsIn,Formula,Vars,Ctx,Context,User,UResultsSoFar,Result,Explaination,Status):-  fail,
                once(getConstants(atomic,Formula,UsedConstants,_,_)),
                logOnFailure(checkAllConstantsHaveTypes(Formula,UsedConstants,UnDefinedList)),
                UnDefinedList=[_|_],!,writeObject(undefined_constants(UnDefinedList),_),!,fail.

% Check For Theorem
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,User,1,['Result' = 'True'],formula(instance(FormulaIN,'Theorem')),done(true:thereom)):-
        resetTableFlags,
        writeDebug(purple,'Stage 2 - Theorem Check ':FmlInOpen),
        isTheorem(FmlInOpen,_),!,
        retain_yes,sendNote('(theorem)'),!.

% Call Belief
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,User,UA, UVars,Explaination,Result):-
        agentBelief(FmlInOpen,URequest,UVars,Ctx,Context,User,UA, UVars, Explaination,Result).

% Request Failed
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,User,0,['Result'='none'|_],'Unproven',done(possible:searchfailed)):-
        flag('$UAnswers',UA,UA),UA<1,!.

% Request Succeeded
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,User,UA,['Result'='true'|_],found(UA),done(true:UA)):-!,
        flag('$UAnswers',UA,UA).


% ======================================================
% ======================================================
commitCleanExplaination(deduced,deduced):-!.
commitCleanExplaination(Explaination * deduced,ExplainationOut):-!,commitCleanExplaination(Explaination,ExplainationOut).
commitCleanExplaination(deduced * Explaination,ExplainationOut):-!,commitCleanExplaination(Explaination,ExplainationOut).
commitCleanExplaination(Explaination * '$VAR'(_),ExplainationOut):-!,commitCleanExplaination(Explaination,ExplainationOut).
commitCleanExplaination('$VAR'(_) * Explaination,ExplainationOut):-!,commitCleanExplaination(Explaination,ExplainationOut).
commitCleanExplaination(Explaination1 * Explaination2,ExplainationOut1 * ExplainationOut2):-!,
        commitCleanExplaination(Explaination1,ExplainationOut1),!,
        commitCleanExplaination(Explaination2,ExplainationOut2),!.
commitCleanExplaination(Explaination,Explaination):-!.


getRequestDefaults(URequest,OAnswers,BackchainsMax,Deductions):-
        ((
        (getMooOption(opt_backchains_max=BackchainsMax)),
        (getMooOption(opt_deductions_max=Deductions)),!,
        ignore((ground(URequest) -> Answers=1 ; Answers=PAnswers)),
        (getMooOption(opt_answers_max=Answers)),!,
        ignore(BackchainsMax=30),ignore(Answers=60),OAnswers is Answers,!)).

set_quit_time(Num):-
        ((
        (getMooOption(opt_timeout=QuitTime)),!,ignore(QuitTime=5),!,
        retractall(cpuend(_)),
        getCputime(Now),
        Then is Now + QuitTime*Num,
        asserta(cpuend(Then)),!)).

:-dynamic(noexistencials/0).

%edify_vars(X,X):-!.
edify_vars(Var,Var):-var(Var),!.
edify_vars([],[]):-!.
edify_vars([N=V|T],[N=RV|NT]):-
            eval_lr(V,RV),!,retain_answer(RV),
            edify_vars(T,NT),!.
edify_vars([H|T],[H|NT]):-
            edify_vars(T,NT),!.


% ===================================================================
%  INVOKE REQUEST STANDARD
% ===================================================================


:-dynamic(final_answer/1).
:-dynamic(addToResultBuffer/4).
:-dynamic(requestBuffer_db/4).
:-dynamic(request_start_time/1).
:-dynamic(request_total_time/1).


invokeRequestToBuffer(findall(V,Formula),Ctx,TrackingAtom,Context,User,Vars,CPU):-!,
        invokeRequestToBuffer(Formula,Ctx,TrackingAtom,Context,User,Vars,CPU).
invokeRequestToBuffer(request(Formula),Ctx,TrackingAtom,Context,User,Vars,CPU):-!,
         invokeRequestToBuffer(Formula,Ctx,TrackingAtom,Context,User,Vars,CPU),!.
invokeRequestToBuffer(requestyn(Formula),Ctx,TrackingAtom,Context,User,Vars,CPU):-!,
         invokeRequestToBuffer(Formula,Ctx,TrackingAtom,Context,User,Vars,CPU),!.

invokeRequestToBuffer(canonicalize(Formula),Ctx,TrackingAtom,Context,User,Vars,CPU):-!,
        retractall(requestBuffer_db(_,_,_,_)),
        retractall(final_answer(_)),
        getAssertionClauses(PreQ,Context,Formula,Out,Vars,P),
        retain_answer(Out),
        writeObject('$spacer',Vars),
        asserta(tq_passed),
        asserta(tq_attempted_request),
        asserta(tq_least_one_answer),
        writeObject_conj(Out,Vars).


invokeRequestToBuffer(Formula,Ctx,TrackingAtom,Context,User,Vars,CPU):-
        getCputime(Start),!,
        logOnFailure(createRequestResponseBuffer_proc(Formula,Ctx,TrackingAtom,Context,User,Vars)),
        getCputime(End),ignore(Start=0),ignore(End=0),
        retractall(request_total_time(_)),
        CPU is End - Start,!,ignore(CPU=0),!,
        assert(request_total_time(CPU)),!.


createRequestResponseBuffer_proc(FmlInOpen,Ctx,TrackingAtom,Context,User,Vars):-
        assert(tq_attempted_request),
        retractall(requestBuffer_db(_,_,_,_)),
        retractall(final_answer((_))),
        agentRequest(KIFCharsIn,FmlInOpen,Vars,Ctx,Context,User,UResultsSoFar,Result,Explaination,Status),
        ignore(logOnFailure(retain_answer(Result))),
        writeDebug(addToResultBuffer(UResultsSoFar,Result,Explaination,Status)),
        addToResultBuffer(UResultsSoFar,Result,Explaination,Status),!. % Buffer Calls Itteration via failure

createRequestResponseBuffer_proc(FmlInOpen,Ctx,TrackingAtom,Context,User,Vars):-!.


% ===========================================================
% Buffer Results
% ===========================================================
addToResultBuffer(UResultsSoFar,Result,Explaination,Status):-var(Explaination),!,fail.

addToResultBuffer(UResultsSoFar,Result,Explaination,done(How)):-
        numbervars((UResultsSoFar,Result,Explaination,Status),'$VAR',0,_),
        assert(requestBuffer_db(UResultsSoFar,Result,Explaination,done(How))),
        assert(final_answer(How)),!.    %Finalize on done/1.

addToResultBuffer(UResultsSoFar,Result,Explaination,Status):-
        once((numbervars((UResultsSoFar,Result,Explaination,Status),'$VAR',0,_),
        assert(requestBuffer_db(UResultsSoFar,Result,Explaination,Status)))),
        fail.


