% ===================================================================
% File 'moo_api.pl'
% Maintainer: Douglas Miles
% Contact: dmiles@users.sourceforge.net ;
% Version: 'moo_api.pl' 1.0.0
% Revision:             $Revision: 1.3 $
% Revised At:   $Date: 2002-03-08 14:39:50 $

% ===================================================================
% PURPOSE
% ===================================================================
% This file meets the needs of an external agent working for the needs of eigther an automated or human user
% Interfacea with Java with XML to display explaination trees and variable bindings
% It defines the default settings most users will use and gives a starting expected state.
% Ask a KB, tell to a KB, retract from a KB, consult the user (this function is currently treated by ua_command/2, but it is planned to separate it),
% Report status to Moo, initialization of the LE, and file-handling routines
% ===================================================================

% ===================================================================
% EXPORTS   (Called  only by XSB Java server beans)
% ===================================================================
:-include('moo_header.pl').
               /*
:-module(moo_useragent,
            [
            initializeMooServerData/0,   % Loads appropriate KBs

            ua_ask/3,        % handles user invoked request
            uaInsert/4,         % handles user invoked assertions
            ua_retract/4,    % handles user invoked retractions
            ua_command/2, %allows user to define new terms, and handles consultation with user

            define_kb/1,  %selects user's choice of a kb to load
            rename_kb/1,  %allows server to rename a kb
            reconstitute_kb/1,  %allows server to redefine the location of a kb
            delete_kb/1,  %allows server to delete a kb from the IE

            establish_status/1,
            establish_status/2,  %loads and unloads kbs, reports status to server,

            verify_status/1,
            verify_status/2,  %only reports status of kbs,

            server_startup_status/1,  %presets multiple kbs to be loaded at startup of IE,
            kb_startup_status/1,  %presets a single kb to be loaded at startup of IE,

            ua_read/2 % -- (to be dropped) loads a single kb
            ]).
  */


% ===================================================================
% Other major predicates:
% agentConsultation/3 (called by ua_out in moo_response.P, also by writeUAEvent, and by command_proc/3, by any predicate marked for consultation)
% ===================================================================

% ===================================================================
% DEBUG PREDICATES
% ===================================================================

fme2:-ua_ask("(?P2 ?A1 ?2)", 'GlobalContext', [opt_kb='PrologMOOAddition',disp_debug=true,disp_note_user=true,disp_notes_nonuser=true]).
fme3:-ua_ask("(?P3 ?A1 ?A2 ?A3)", 'GlobalContext', [opt_kb='PrologMOOAddition',disp_debug=true,disp_note_user=true,disp_notes_nonuser=true]).
gme2:-ua_ask("(genls ?X ?Y)").

% ===================================================================
% IMPORTS
% ===================================================================


% Stored persistantly as  kb_make_status_start(kb(Name,CanFileLocal)=StartupStatus)  in moo_persist.wfs


% ===================================================================
%  BOOTUP MOO SERVER
% ===================================================================

% This Function when called from outside does the Embeding the Logic Engine into the XSB Server
initializeMooServerData :-!,
     % findall(L,library_directory(L),Library_directory),
      sendNote(user,logicEngine,'Initializing and Embedding Moo Logic Engine',['architecture=',Architecture,Host_cpu,Version,Release_date,Scheduling_strategy]),
      establish_startup_state,
      sendNote(debug,logicEngine,'KBs are now loaded by contentManager',' '),
      ensureMooKB('MooKernel','GlobalContext'),!.

%:-include('moo_header.pl').

% ===========================================================
% CONVERSE WITH JAVA
% ===========================================================

processRequest(OptionsIn):-
                logOnFailure(fixOptionsFromForeign(OptionsIn,Options)),
                logOnFailure(setMooOption(Options)),
                ensureMooOption(client,html,ResponseType),
                logOnFailure(invokeRequest(Options)),!.


fixOptionsFromForeign([],[]):-!.

fixOptionsFromForeign([context=Value|List],[opt_ctx_assert=Value,opt_ctx_request=Value|ARGS]):-     %TODO Sepatate KB/Ctx
          fixOptionsFromForeign(List,ARGS).
fixOptionsFromForeign([ctx=Value|List],[opt_ctx_assert=Value,opt_ctx_request=Value|ARGS]):-     %TODO Sepatate KB/Ctx
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
transform_option(kb,opt_kb).
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
invokeRequest(Options):-memberchk(submit=tn_find,Options),!,
        ensureMooOption(opt_ctx_assert,'GlobalContext',Ctx),
        ensureMooOption(opt_kb,'PrologMOO',KB),
        ensureMooOption(client,'java',CLIENT),
        ensureMooOption(sf,surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        ensureMooOption(user,'Web',User),
         logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
         write_out_kif_tn(Assertion_Chars,NEWFORM,Vars,Ctx,KB,Maintainer),!.

write_out_kif_tn(Assertion_Chars,computed(comment(_)),Vars,Ctx,KB,Maintainer):-
         writeFmt('Syntax Error: Unmatched parentheses in "~s"\n',[Assertion_Chars]).
write_out_kif_tn(Assertion_Chars,comment(_),Vars,Ctx,KB,Maintainer):-
         writeFmt('Syntax Error: Unmatched parentheses in  "~s"\n',[Assertion_Chars]).
write_out_kif_tn(Assertion_Chars,NEWFORM,Vars,Ctx,KB,Maintainer):-
        copy_term((NEWFORM),(CNF)),
        numbervars(CNF,'$VAR',0,_),
        mooCache(PredR,surface,CNF, _, KB,Ctx, TN, Maintainer, O),
        mooCache(PredR,surface,OF, _, KB,Ctx, TN, Maintainer, O),
        numbervars(OF,'$VAR',0,_),
        OF == CNF,!,
        writeFmt('~w\n',[TN]),!.
write_out_kif_tn(Assertion_Chars,NEWFORM,Vars,Ctx,KB,Maintainer):-
         writeFmt('Not found in ~w of ~w "~s"\n',[Ctx,KB,Assertion_Chars]).

% ===========================================================
% Detroys an External TN
% ===========================================================
invokeRequest(Options):-memberchk(submit=tn_delete,Options),!,
        ensureMooOption(opt_ctx_assert,'GlobalContext',Ctx),
        ensureMooOption(opt_kb,'PrologMOO',KB),
        ensureMooOption(client,'java',CLIENT),
        ensureMooOption(tn,0,TN),
        ensureMooOption(sf,surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        ensureMooOption(user,'Web',User),
        destroyTN(KB,TN,Ctx),
        saveMooCache.

destroyTN(KB,TN,Ctx):-
        retractall(mooCache(Literal,_,KB,Ctx,TN)),  %Facts
        retractall(mooCache(Literal,AnteLiteral,_,KB,Ctx,TN)),   %Rules
        retractall(mooCache(Surface,CLF,Flags,Vars,KB,Ctx,TN,Maintainer,TMResult)).

destroyKB(KB):-
        retractall(mooCache(KB,_)),
        retractall(mooCache(KB,_,_)),
        retractall(mooCache(KB,_,_,_)),
        retractall(mooCache(Literal,_,KB,Ctx,TN)),  %Facts
        retractall(mooCache(Literal,AnteLiteral,_,KB,Ctx,TN)),   %Rules
        retractall(mooCache(Surface,CLF,Flags,Vars,KB,Ctx,TN,Maintainer,TMResult)),
        saveMooCache.

% ===========================================================
% Shows whats known about an External TN
% ===========================================================
invokeRequest(Options):-memberchk(submit=tn_show,Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_kb='PrologMOO',KB),
        ensureMooOption(client='java',CLIENT),
        ensureMooOption(tn=0,TN),
        ensureMooOption(sf=surf,Assertion),
        writeKnownFormsTN(_,KB:TN).

writeKnownFormsTN(Ctx,KB:Word):-(atom(Word)),!,
        writeKnownFormsAboutTerm(Ctx,KB:Word).

writeKnownFormsAboutTerm(Ctx,KB:Word):-
        mooCache(PredR,Fact,Pre,Type,true,KB,Agent,P),
        contains_const((Fact,Pre),Word),
        flag(explaination_linenumber,LN,1),
        once(writeObject(P,_)),
        write('<hr>'),
        fail.

writeKnownFormsAboutTerm(Ctx,KB:Word):-
        mooCache(PredR,Fact,Type,true,KB,Agent,P),
        contains_const(Fact,Word),
        flag(explaination_linenumber,LN,1),
        once(writeObject(P,_)),
        write('<hr>'),
        fail.

writeKnownFormsAboutTerm(Ctx,KB:Word).

contains_const(Fact,Word):-
        getConstants(atomic,Fact,List,_,_),!,
        memberchk(Word,List).

writeKnownFormsTN(Ctx,KB:TN):-
        writeFmt('\n</pre><H3><br>Compiled Forms</H3><pre>\n',[]),fail.

writeKnownFormsTN(Ctx,KB:TN):-
        flag(indent,_,0),
        mooCache(Surface,CLF,Flags,Vars,KB,Ctx,TN,Maintainer,TMResult),
        once(tam(Surface,Vars)),fail.
/*
writeKnownFormsTN(Ctx,KB:TN):-
        flag(indent,_,0),
        mooCache(Surface,CLF,Flags,Vars,KB,Ctx,TN,Maintainer,TMResult),
        once(writeObject(writeq(mooCache(Flags,Vars,KB,Ctx,TN,Maintainer,TMResult)),Vars)),fail.

writeKnownFormsTN(Ctx,KB:TN):-
        flag(indent,_,0),
        mooCache(Surface,CAN,Flags,Vars,KB,Ctx,TN,Maintainer,TMResult),
        flag(clause_id,_,0),
        tam(Surface,CAN,Flags,Vars,KB,Ctx,TN,Maintainer,Result),fail.
*/
writeKnownFormsTN(Ctx,KB:TN):-
        writeFmt('\n</pre><H3><br>Active Forms</H3><pre>\n',[]),fail.

writeKnownFormsTN(Ctx,KB:TN):-
        flag(clause_id,CLID,CLID),
        format('<hr>Clauses: ~w',[CLID]),fail.

writeKnownFormsTN(Ctx,KB:TN):-
        mooCache(Fact,Pre,Cost,KB,Agent,TN),
        list_to_and(Pre,Ands),
        flag(indent,_,0),
        once(writeObject('<hr>',Vars)),
        once(writeObject(writeq(Fact:Pre),Vars)),
        once(writeObject(nl,Vars)),
        once(writeObject('KB |= '(Ands,Fact),Vars)),fail.

writeKnownFormsTN(Ctx,KB:TN):-
        mooCache(Fact,Cost,KB,Agent,TN),
        flag(indent,_,0),
        once(writeObject('<hr>',Vars)),
        once(writeObject(Fact,Vars)),fail.

/*
writeKnownFormsTN(Ctx,KB:TN):-
        isMooOption(opt_debug=on),
        writeKnownFormsTN_used(Ctx,KB:TN).
*/
writeKnownFormsTN(Ctx,KB:TN):-writeFmt('</pre>').

writeKnownFormsTN_used(Ctx,KB:TN):-
        writeFmt('\n</pre><H3><br>Belief Forms</H3><pre>\n',[]),
        retractall(request_kb(X)),
        asserta(request_kb(KB)),
        mooCache(PredR,Fact,Ctx,KB:TN:_^Vars),
        t_ado_cache(PredR,_,Fact, Vars, KB,Ctx, TN, Surf, on),
        writeFmt('\n<hr>'),fail.

writeKnownFormsTN_used(Ctx,KB:TN):-
        mooCache(PredR,Fact, Ctx, Pre, KB:TN:_^Vars),
        toMarkUp(html,formula(entails(Pre,Fact)),Vars,O),
        writeFmt('\n~w<hr>',[O]),fail.


% ===========================================================
% Ask
% ===========================================================
invokeRequest(Options):-memberchk(submit=ask,Options),!, make,
        ensureMooOption(opt_ctx_request='GlobalContext',Ctx),
        ensureMooOption(opt_kb='PrologMOO',KB),
        ensureMooOption(sf=surf,Askion),
        atom_codes(Askion,Askion_Chars),
        ensureMooOption(user='Web',User),
        ensureMooOption(interp='kif',Interp),
        logOnFailure(getCleanCharsWhitespaceProper(Askion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        logOnFailure(invokeOperation(quiet,request(NEWFORM),Ctx,TrackingAtom,KB,User,Vars)).


% ===========================================================
% Add
% ===========================================================
invokeRequest(Options):-memberchk(submit=tn_sync,Options),!,
       %  writeFmt(user_error,'ua: ~w\n',[Options]),flush_output(user_error),
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_kb='PrologMOO',KB),
        ensureMooOption(tn=_,EXTID),
        ensureMooOption(user='Web',User),
        ensureMooOption(sf=surf,Assertion),
        logOnFailure(atom_codes(Assertion,Codes)),
        getSurfaceFromChars(Codes,STERM,Vars),
        getMooTermFromSurface(STERM,Surface),
        destroyTN(KB,EXTID,_),
        once(invokeInsert([trusted,canonicalize],surface,Surface,Ctx,EXTID,KB,Vars,User)),!.

invokeRequest(Options):-memberchk(submit=tn_form_update,Options),!,
       %  writeFmt(user_error,'ua: ~w\n',[Options]),flush_output(user_error),
        ensureMooOption(tn=_,EXTID),
        ensureMooOption(user='Web',User),
        ensureMooOption(opt_kb='PrologMOO',KB),
        ensureMooOption(sf=surf,Assertion),
        logOnFailure(atom_codes(Assertion,Codes)),
        getSurfaceFromChars(Codes,STERM,Vars),
        getMooTermFromSurface(STERM,Surface),
        logOnFailure(retract(mooCache(PredR,surface,OldSurf,_,KB,Ctx,EXTID,_,_))),
        destroyTN(KB,EXTID,_),
        once(invokeInsert([trusted,canonicalize],surface,Surface,Ctx,EXTID,KB,Vars,User)),!.

invokeRequest(Options):-memberchk(submit=canonicalize,Options),!,
        ensureMooOption(opt_kb='PrologMOO',KB),
        once(invokeKBCompilerProcess(KB)),write_ln('canonicalizing.\n').

invokeRequest(Options):-memberchk(submit=blank_kb,Options),!,
        ensureMooOption(opt_kb='PrologMOO',KB),
        destroyKB(KB),
        saveMooCache.


invokeRequest(Options):-memberchk(submit=removeKbNameSpace,Options),!,
        ensureMooOption(opt_kb='PrologMOO',KB),
        destroyKB(KB),
        retractall(mooCache(instance,surface,'instance'(KB,'KnowledgeBase'),'$VAR'(0),'MooKernel','GlobalContext',TN1,'WebUser',gaf)),
        retractall(mooCache(instance,surface,'instance'(Ctx,'Context'),'$VAR'(0),KB,'GlobalContext',TN2,'WebUser',gaf)),
        retractall(mooCache(instance,surface,'instance'('GlobalContext','Context'),'$VAR'(0),KB,'GlobalContext',TN2,'WebUser',gaf)),
        retractall(mooCache(instance,surface,'sourcefile-of'(KB,Filename),'$VAR'(0),'MooKernel','GlobalContext',TN3,'WebUser',gaf)),
        !.

% ===========================================================
% Verify Surface Returns SUO-KIF
% ===========================================================
invokeRequest(Options):-memberchk(submit=verify_assert,Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_kb='PrologMOO',KB),
        ensureMooOption(client='java',CLIENT),
        ensureMooOption(sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        ensureMooOption(user='Web',User),
         logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
         write_out_kif(Assertion_Chars,NEWFORM,Vars,Ctx,KB,Maintainer).

write_out_kif(Assertion_Chars,computed(comment(_)),Vars,Ctx,KB,Maintainer):-
         writeFmt('Syntax Error: Unmatched parentheses in "~s"',[Assertion_Chars]).
write_out_kif(Assertion_Chars,comment(_),Vars,Ctx,KB,Maintainer):-
         writeFmt('Syntax Error: Unmatched parentheses in "~s"',[Assertion_Chars]).
write_out_kif(Assertion_Chars,NEWFORM,Vars,Ctx,KB,Maintainer):-
        logOnFailure(getTruthCheckResults(tell,[untrusted],surface,NEWFORM,Ctx,STN,KB,Vars,Maintainer,Result)),
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
        ensureMooOption(opt_kb='PrologMOO',KB),
        ensureMooOption(user='Web',User),
        writeFmt('<pre>\n',[]),
        retractall(drawn(_)),
        draw_context_dag(Ctx,KB,Ctx,0),!,
        writeFmt('</pre>\n',[]).

tcdag(C):-retractall(drawn(_)),draw_context_dag('PrologMOO',C,0).

:-dynamic(drawn/1).

draw_context_dag(KB,Ctx,N):-
        assert(drawn(Ctx)),
        make_space(N,O),
        writeFmt('~w<strong>~w</strong>\n',[O,Ctx]),
        NN is N + 1,!,
        show_subs(KB,Ctx,NN).

show_subs(KB,Ctx,8):-!.
%show_subs(Top,KB,Top,_):-!.

show_subs(KB,Ctx,NN):-
        context_dag(KB,Ctx,Sub),
        not(drawn(Sub)),
        draw_context_dag(KB,Sub,NN),
        fail.
show_subs(KB,Ctx,NN):-!.


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
        ensureMooOption(opt_kb='PrologMOO',KB),
        ensureMooOption(sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        ensureMooOption(user='Web',User),
        ensureMooOption(interp='kif',Interp),
        logOnFailure(ensureMooOption(tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        logOnFailure(invokeOperation(verbose,assert(NEWFORM),Ctx,TN,KB,User,Vars)).


% ===========================================================
% Start Canonicalizer
% ===========================================================
invokeRequest(Options):-memberchk(submit=canonicalize,Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_kb='PrologMOO',KB),
        logOnFailure(invokeKBCompilerProcess(KB)).


% ===========================================================
% Create Knowledge Base (New)
% ===========================================================
invokeRequest(Options):-memberchk(submit=register_kb,Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_kb='PrologMOO',KB),
        ensureMooOption(client='java',CLIENT),
        ensureMooOption(user='Web',User),
        saveMooCache,
        getDefaultImageFilepath(IF),
        open(IF,append,Handle,[close_on_abort(false),buffer(full)]),
        assert(save_can_to_file(KB,Handle)),
        !.

:-dynamic(save_can_to_file/2).

% ===========================================================
% Retract
% ===========================================================
invokeRequest(Options):-memberchk(submit=retract,Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_kb='PrologMOO',KB),
        ensureMooOption(sf=surf,Retraction),
        atom_codes(Retraction,Retraction_Chars),
        ensureMooOption(user='Web',User),
        ensureMooOption(interp='kif',Interp),
        give_kif_window,
         getCleanCharsWhitespaceProper(Retraction_Chars,Show),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
              once(( NEWFORM=comment(_) ->
                     (do_chars(Show),!,FORM=_) ;(!,
                     logOnFailure(invokeOperation(verbose,retract(NEWFORM),Ctx,TN,KB,User,Vars))
                     ))).


% ===========================================================
% Delete Assertion
% ===========================================================
invokeRequest(Options):-memberchk(submit='Delete Assertion',Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_kb='PrologMOO',KB),
        ensureMooOption(asid=_,AID),
        ensureMooOption(user='Web',User),
        ensureMooOption(interp='kif',Interp),
        writeFmt('<H3><Font Color=Red>Deleted....</Font></H3>',[]),
        delete_assertion(AID).

delete_assertion(AID):-
        retract(mooCache(PredR,Form,SURF,Vars,KB,Ctx,AID,Maintainer,_)),
        delete_assertion_disp(Form,SURF,Vars,KB,Ctx,AID,Maintainer),
        fail.

delete_assertion(AID):-writeFmt('<H3><Font Color=Red>Done Deleting.</Font></H3>',[]).

delete_assertion_disp(Form,SURF,Vars,KB,Ctx,AID,Maintainer):-
        toMarkUp(html,SURF,Vars,SAtom),
        writeFmt('<IMG src="pixmaps/bullet.gif" asrtid=~w><nobr>',[AID]),
        writeFmt('<b>~w</b> ID<font color=red>~w</font> in KB: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font>',[Form,AID,KB,Ctx,Maintainer]),
        %format_o('&nbsp;&nbsp;~w&nbsp;Enabled&nbsp;&nbsp;<br>',checkbox(AID,OnOff)),
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
        give_kif_window,
        catch(atom_to_term(Prolog,CMD,Vars),E,
        (message_to_string(E,S),writeFmt('\nCall "~w" could not be read.  \nError: ~s\n',[Prolog,S]))),!,
        callFromWeb(Prolog,CMD,Vars),!.
parse_prolog_cmd(Prolog):-writeFmt('\nCall "~w" failed',[Prolog]),!.
callFromWeb(Prolog,CMD,Vars):-var(CMD),!.
callFromWeb(Prolog,CMD,Vars):-
        thread_self(Id),
        ensureMooOption('$socket_out',user_output,Out),
        ensureMooOption('$socket_in',user_input,In),
        invokePrologCommand(Id,In,Out,CMD,Vars,Results),
        writeFmt('\n       Results: ~w\n',[Results]),!.




% ===========================================================
% Logical Form Display
% ===========================================================
invokeRequest(Options):-memberchk(logicforms=logicforms,Options),!,
       parse_moo_lf(Options).


% ===========================================================
% Un-Canonicalize on KB/Ctx
% ===========================================================

invokeRequest(Options):-
        memberchk(cmd='Un-Canonicalize',Options),!,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_kb='PrologMOO',KB),!,
        (unsetMooOption(opt_surface_check=_)),
        (setMooOption(opt_surface_check=untrusted)),
        (unsetMooOption(opt_tracking_number=_)),
        (setMooOption(opt_tracking_number=supplied)),!,
        (unsetMooOption(opt_canonicalizer=_)),
        (setMooOption(opt_canonicalizer=byrd)),!,
        writeFmt('<H2>Un-Canonicalizing  KB:~w Ctx:~w.  This process may take several minutes.. Do not navigate away.</H2>',[KB,Ctx]),
        mark_all_surface_to_uncanonicalized(KB,Ctx),!.

% ===========================================================
% Invoke Load SKB on KB/Ctx (POST)
% ===========================================================

invokeRequest(Options):-
                memberchk(KB='Load SKB',Options),!,
                (unsetMooOption(opt_kb=_)),
                (setMooOption(opt_kb=KB)),
                ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
                ensureMooOption(opt_kb='PrologMOO',KB),!,
                fmtString(FileChars,'C:/mool/SUO/~w.can',[KB]),!,string_to_atom(FileChars,Filename),!,
                (unsetMooOption(opt_surface_check=_)),
                (setMooOption(opt_surface_check=trusted)),
                idGen(TN1),
                idGen(TN2),
                idGen(TN3),
                idGen(TN4),
                retractall(mooCache(PredR,_,_,_,KB,Ctx,_,_,_)),
                assertaClean(mooCache(instance,surface,'instance'(KB,'KnowledgeBase'),'$VAR'(0),'MooKernel','GlobalContext',TN1,'WebUser',gaf)),
                assertaClean(mooCache(instance,surface,'instance'(Ctx,'Context'),'$VAR'(0),KB,'GlobalContext',TN4,'WebUser',gaf)),
                assertaClean(mooCache(instance,surface,'instance'('GlobalContext','Context'),'$VAR'(0),KB,'GlobalContext',TN2,'WebUser',gaf)),
                assertaClean(mooCache('sourcefile-of',surface,'sourcefile-of'(KB,Filename),'$VAR'(0),'MooKernel','GlobalContext',TN3,'WebUser',gaf)),
                load_kif_to_kb_ctx(KB,Filename,'GlobalContext','MooWeb').

% ===========================================================
% load_from_can_file (Knowledge Base File)
% ===========================================================
invokeRequest(Options):-memberchk(cmd='Load SKB',Options),!,
                ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
                ensureMooOption(opt_kb='PrologMOO',KB),!,
                fmtString(FileChars,'C:/mool/SUO/~w.can',[KB]),!,string_to_atom(FileChars,Filename),!,
                (unsetMooOption(opt_surface_check=_)),
                (setMooOption(opt_surface_check=trusted)),
                idGen(TN1),
                idGen(TN2),
                idGen(TN3),
                idGen(TN4),
                retractall(mooCache(PredR,_,_,KB,Ctx,_,_,_)),
                assertaClean(mooCache(instance,surface,'instance'(KB,'KnowledgeBase'),'$VAR'(0),'MooKernel','GlobalContext',TN1,'WebUser',gaf)),
                assertaClean(mooCache(instance,surface,'instance'(Ctx,'Context'),'$VAR'(0),KB,'GlobalContext',TN4,'WebUser',gaf)),
                assertaClean(mooCache(instance,surface,'instance'('GlobalContext','Context'),'$VAR'(0),KB,'GlobalContext',TN2,'WebUser',gaf)),
                assertaClean(mooCache('sourcefile-of',surface,'sourcefile-of'(KB,Filename),'$VAR'(0),'MooKernel','GlobalContext',TN3,'WebUser',gaf)),
                load_kif_to_kb_ctx(KB,Filename,'GlobalContext','MooWeb').


% ===========================================================
% Invoke Canonicalizer on KB/Ctx (GET)
% ===========================================================
invokeRequest(Options):-
        memberchk(cmd='Canonicalize',Options),!,%trace,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_kb='PrologMOO',KB),!,
        (unsetMooOption(opt_surface_check=_)),
        (setMooOption(opt_surface_check=untrusted)),
        (unsetMooOption(opt_tracking_number=_)),
        (setMooOption(opt_tracking_number=supplied)),!,
        (unsetMooOption(opt_canonicalizer=_)),
        (setMooOption(opt_canonicalizer=byrd)),!,
        writeFmt('<H2>Canonicalizing  KB:~w Ctx:~w.  This process may take several minutes.. <br>This process must be done once and should not be interupted<br>Wait until this page is completely loaded before clicking <br>any links and do not navigate away.</H2>',[KB,Ctx]),
        logOnFailure(canonicalizeMooKBHTML(KB,Ctx)),!.

% ===========================================================
% Invoke Canonicalizer on KB/Ctx (POST)
% ===========================================================
invokeRequest(Options):-
        member(KB='Canonicalize',Options),!,
        (unsetMooOption(opt_kb=_)),
        (setMooOption(opt_kb=KB)),
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        (unsetMooOption(opt_surface_check=_)),
        (setMooOption(opt_surface_check=untrusted)),
        (unsetMooOption(opt_tracking_number=_)),
        (setMooOption(opt_tracking_number=supplied)),!,
        (unsetMooOption(opt_canonicalizer=_)),
        (setMooOption(opt_canonicalizer=byrd)),!,
        writeFmt('<H2>Canonicalizing  KB:~w Ctx:~w.  This process may take several minutes.. <br>This process must be done once and should not be interupted<br>Wait until this page is completely loaded before clicking <br>any links and do not navigate away.</H2>',[KB,Ctx]),
        logOnFailure(canonicalizeMooKBHTML(KB,Ctx)),!.


% ===========================================================
% Invoke Surface Checker on KB/Ctx
% ===========================================================
invokeRequest(Options):-
        memberchk(cmd='Surface Check',Options),!,%trace,
        ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(opt_kb='PrologMOO',KB),!,
        logOnFailure(surface_check_entire_kb_ctx(KB,Ctx)),!,
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
                        ensureMooOption(kb='PrologMOO',KB),
                        ensureMooOption(user='Web',User),
                        idGen(TN2),
                        assertaClean(mooCache(instance,surface,'instance'(Ctx,'Context'),'$VAR'(0),KB,'GlobalContext',TN2,User,on)).


% ===========================================================
% KB/Ctx ComboBoxs
% ===========================================================
show_available_contexts:-
        findall(Atom,
                (mooCache(PredR,surface,('instance'(Ctx,'Context'):_), KB,_, _, _, _),concat_atom([KB,':',Ctx],Atom)),
                List),
        writeq(List),nl.


getCtxListForKB(KB,['GlobalContext'|Sorted]):-
        findall(Ctx,mooCache(_,_,_, _, KB,Ctx, _, _, _),UList),!,sort(UList,Sorted).
getCtxListForKB(KB,['GlobalContext']):-!.

show_available_contexts_in_combobox(KB,null):-
        getCtxListForKB(KB,List),
        toMarkUp(html,select(ctx,List),_,Out),write(Out).

show_available_contexts_in_combobox(KB,Selected):-
        getCtxListForKB(KB,List),
        toMarkUp(html,select(ctx,[Selected|List]),_,Out),write(Out).


getListOfKB(KBList):-
        findall(KB,mooCache(_,_,_, _, KB,Ctx, _, _, _),UList),!,sort(UList,KBList).

show_available_kbs_in_combobox(Out):-
                findall(KB,mooCache(PredR,surface,('instance'(KB,'KnowledgeBase'):_), 'MooKernel',_, _, _, _),List),
                toMarkUp(html,select(kb,List),_,Out).

% =================================================
% Useragent Control Panel
% =================================================
invokeRequest(Options):-memberchk(client='controlpanel',Options),
        memberchk(kill=_,Options),!,
        member(kill=ID,Options),
        catch(thread_signal(ID,thread_exit(user_killed(ID))),_,true),
        invokeRequest([client='controlpanel']).


invokeRequest(Options):-memberchk(client='controlpanel',Options), !,
        showMooProcesses.

% ===========================================================
% TQ HOOK
% ===========================================================
invokeRequest(Options):-memberchk(client='tqsystem',Options),!,ignore(invokeTestSystem(Options)).

% ===========================================================
% FALLBACKS
% ===========================================================

invokeRequest(Options):-memberchk(client=command,Options),!.

invokeRequest(Options):-writeFmt('
<html>

<head>
<meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
<meta name="Keywords" content="PROLOG Artificial Intelligence Ontology AI MOO Linguistics Philosophy DARPA Doulgas Miles">
<meta name="Description" content="PROLOG Artificial Intelligence Ontology AI MOO Linguistics Philosophy DARPA">
<title>LogicMOO</title>
</head>

<body>

<p><b><span style="font-size: 10.0pt; font-family: Verdana"><font color="#008080">Welcome
to the </font><a href="http://logicmoo.sourceforge.net"><font color="#008080">PrologMOO</font></a></span><span style="font-size:10.0pt;font-family:Verdana;color:teal">!</span></b></p>
<p><span style="font-size: 10.0pt; font-family: Verdana">Your request was: "~q"</span></p>
<p><span style="font-size: 10.0pt; font-family: Verdana"><a href="login">You
would like</a></span><span style="font-size:10.0pt;font-family:Verdana"><a href="login">
log in and try it out</a>.</span></p>',[Options]),
showMooProcesses,
writeFmt('
</body>

</html>
').


give_kif_window:-!. %Given

% ===========================================================
% EDITOR WINDOW
% ===========================================================

give_editor_window:-!,
        ensureMooOption(sf='(isa ?X ?Y)',Formula),
        writeFmt('<textarea rows=6 cols=90 name="sf">~w</textarea><br>',[Formula]),
        writeFmt('<br><INPUT type=submit name=submit value="Update Source"></INPUT><hr>',[]),
        writeFmt('<INPUT type=radio name="interp" value="kif" checked>KIF</INPUT>',[]),
        writeFmt('<INPUT type=radio name="interp" value="ace">ACE</INPUT>',[]).


% ===================================================================================================
%  End of moo_useragent.P
% ===================================================================================================

getKbStatus('MooKernel'):-writeFmt('Browse Only\n',[]),!.
getKbStatus(KB):-
        isKBCompilerProcess(KB,Progress),
        writeFmt('<A href=controlpanel.jsp?kb=~w><font color=green>Canonicalizing</font></a>\n',[KB]),!.

getKbStatus(KB):-
        isUncanonicalized(KB),
        invokeKBCompilerProcess(KB),
        isKBCompilerProcess(KB,Progress),
        writeFmt('<A href=controlpanel.jsp?kb=~w><font color=green>Canonicalizing</font></a>\n',[KB]),!.

getKbStatus(KB):-
        isUncanonicalized(KB),
        isKBCurrentlyInUse(OtherKB,Status),!,
        writeFmt('<font color=orange>Waiting for ~w</font>\n',[OtherKB]).

getKbStatus(KB):-
        isKBCurrentlyInUse(KB,Status),
        writeFmt('<font color=orange>Busy ~w</font>\n',[Status]).

getKbStatus(KB):-
        isKbUntransfered(KB),
        writeFmt('<A href=skb_to_prolog.jsp?kb=~w><font color=red>Update</font></a>\n',[KB]),!.

getKbStatus(KB):-
        isSourceNewerThenImage(KB),
        writeFmt('<A href=skb_to_prolog.jsp?kb=~w><font color=red>Needs Update</font></a>\n',[KB]),!.

getKbStatus(KB):-isKnowledgeBaseLoaded(KB,_),writeFmt('<A href="askInsert.jsp?kb=~w">Ask/Insert</A>\n',[KB]),!.

getKbStatus(KB):-writeFmt('Unknown\n',[]),!.

% ===================================================================
%  INVOKE REQUEST INTERFACE USED BY REQUEST AGENTS
% ===================================================================

%:-include('moo_header.pl').

% =======================================================================================

invokeRequestAndWriteUserAgent(Formula,Ctx,TrackingAtom,KB,User,Vars,CPU):-
         invokeRequestToBuffer(Formula,Ctx,TrackingAtom,KB,User,Vars,CPU),
         writeUserAgentBuffer.


% ===========================================================
% Cite Buffer
% ===========================================================
writeUserAgentBuffer:-%trace,
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
        notrace((once(writeDebug(writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status))),fail)).

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


setMooOptionDefaults:-
             (unsetMooOption(_)),
             setMooOption(opt_callback='sendNote'),
             setMooOption(cb_consultation='off'),
             setMooOption(opt_debug='off'),
             setMooOption(cb_error='off'),
             setMooOption(cb_result_each='off'),

% User Agent Defaults for Blank Variables
             setMooOption(opt_cxt_request='GlobalContext'),
             setMooOption(opt_ctx_assert='GlobalContext'),
             setMooOption(opt_tracking_number='generate'),
             setMooOption(opt_agent='ua_parse'),
             setMooOption(opt_precompiled='off'),
             getMooOption(opt_kb,KB),setMooOption(opt_kb=KB),
             setMooOption(opt_notation='kif'),
             setMooOption(opt_timeout=2),
             setMooOption(opt_readonly='off'),
             setMooOption(opt_debug='off'),
             setMooOption(opt_compiler='Byrd'),
             setMooOption(opt_language = 'pnx_nf'),

%Request Limits
             setMooOption(opt_answers_min=1),
             setMooOption(opt_answers_max=999), %TODO Default
             setMooOption(opt_backchains_max=5),
             setMooOption(opt_deductions_max=100),
             setMooOption(opt_backchains_max_neg=5),
             setMooOption(opt_deductions_max_neg=20),
             setMooOption(opt_forwardchains_max=1000),
             setMooOption(opt_max_breath=1000), %TODO Default

%Request Contexts
             setMooOption(opt_explore_related_contexts='off'),
             setMooOption(opt_save_justifications='off'),
             setMooOption(opt_deductions_assert='on'),
             setMooOption(opt_truth_maintence='on'),
             setMooOption(opt_forward_assertions='on'),
             setMooOption(opt_deduce_domains='on'),
             setMooOption(opt_notice_not_say=off),


%Request Pobibility
             setMooOption(opt_certainty_max=1),
             setMooOption(opt_certainty_min=1),
             setMooOption(opt_certainty=1),
             setMooOption(opt_resource_commit='on').

unset_promiscuous:-
                setMooOption(opt_deduce_domains='off').
set_promiscuous:-
                setMooOption(opt_deduce_domains='on').



% ===================================================================
% File 'Moo_operation.pl'
% Maintainers: Douglas Miles
% Contact: dmiles@users.sourceforge.net
% Version: 'Moo_operation.pl' 1.0.0
% ===================================================================
% ===================================================================
% PURPOSE
% This File is the bootstrap for the Moo Infence engine one first calls "[moo_operation]"
% So first is loads the proper files and then starts up the system
% ===================================================================

:-include('moo_header.pl').

% ===================================================================
% EXPORTS
% ===================================================================

register_belief_module(_ModuleName).


canonicalizer_module('moo_normal').
compiler_module('moo_byrd').
version_tag(kbl).

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
agentInsert(+KIFCharsIn,+Ctx,+KB,+User).
KIFCharsIn = "(instance virginia Computer)"
Ctx = 'Computers and Things'
KB = 'PrologMOO' currently must be set to 'PrologMOO'
*/

agentInsert(KIFCharsIn,Ctx,KB,User):-
        set_automation,
        agentInsert(KIFCharsIn,Ctx,TN,KB,User).


/*
Now the predicates that you most want
Invokes a request
agentRequest(+KIFCharsIn,+Ctx,+KB,+User,-UResultsSoFar,-Result,-Explaination,-Status).

Input variables:
KIFCharsIn = "(instance ?What Computer)"
Ctx = 'Computers and Things'  % Any legal Atom that helps you book keep
KB = 'PrologMOO' currently must be set to 'PrologMOO'
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

invokeOperation(Verbose,surf,Ctx,Tracking,KB,User,Vars):-!.
invokeOperation(Verbose,Fml,Ctx,TN,KB,User,Vars):-!,
                once(invokeOperationProc(Verbose,Fml,Ctx,TN,KB,User,Vars)),!.

invokeOperationProc(Verbose,TERM,Ctx,TN,KB,User,Vars):-TERM =..[note|T],!,
                assert(title(T)),
                WT=..['note '|T],
                writeObject(Verbose,WT,Vars),!,
                writeObject(Verbose,'\n\n;; Assertions \n\n',Vars),!,
                clear_moo_memory,set_promiscuous,!.

invokeOperationProc(Verbose,TERM,Ctx,TN,KB,User,Vars):-TERM =..[answer|T],!,batch_answer(T).

invokeOperationProc(Verbose,TERM,Ctx,TN,KB,User,Vars):-TERM =..[abort],!,assert(tq_ignored).
invokeOperationProc(Verbose,'deduce-domains'(on),Ctx,TN,KB,User,Vars):-!,set_promiscuous,!.
invokeOperationProc(Verbose,'deduce-domains'(true),Ctx,TN,KB,User,Vars):-!,set_promiscuous,!.
invokeOperationProc(Verbose,'deduce-domains'(false),Ctx,TN,KB,User,Vars):-!,unset_promiscuous,!.
invokeOperationProc(Verbose,'deduce-domains'(off),Ctx,TN,KB,User,Vars):-!,unset_promiscuous,!.

invokeOperationProc(Verbose,end_of_file,Ctx,TN,KB,User,Vars):-!.

invokeOperationProc(Verbose,'file_comment'(C),SourceCtx,SourceTN,KB,User,Vars):-!,nl,write(C),!.
invokeOperationProc(Verbose,'browser-only'(_),SourceCtx,SourceTN,KB,User,Vars):-!.
invokeOperationProc(Verbose,'execute-prolog'(Code),SourceCtx,SourceTN,KB,User,Vars):-!,
once(((%                writeq(Code),nl,
                atom_codes(Code,Codes),
%               writeq(Codes),nl,
                getUnquotedCodes(Codes,Chars),
%               writeq(Chars),nl,
                atom_codes(Atom,Chars),
                atom_to_term(Atom,Term,V)

                );writeFmt('could not parse ~w\n',[Code])),once(((once(catch(Term,_,fail)),writeFmt('Exec Suceeded',[]));writeFmt('Exec Failed',[])))).
invokeOperationProc(Verbose,'load-kb'(Filename),SourceCtx,SourceTN,KB,User,Vars):-!,fail,agent_load_kif_quiet(Filename,'GlobalContext','Anonymous').
invokeOperationProc(Verbose,'load-kb-show'(Filename),SourceCtx,SourceTN,KB,User,Vars):-!,
        agent_load_kif(Filename,'GlobalContext','Anonymous').

invokeOperationProc(Verbose,retract(nil),Ctx,TN,KB,User,Vars):-!.

invokeOperationProc(Verbose,request(nil),Ctx,TN,KB,User,Vars):-!.

invokeOperationProc(Verbose,(nil),Ctx,TN,KB,User,Vars):-!.

invokeOperationProc(Verbose,tell(nil),Ctx,TN,KB,User,Vars):-!.

invokeOperationProc(Verbose,assert(end_of_file),Ctx,TN,KB,User,Vars):- !.

invokeOperationProc(Verbose,assert(NEWFORM),Ctx,TN,KB,User,Vars):-  !,
        invokeOperationProc(Verbose,assert([trusted,canonicalize,to_mem],NEWFORM),Ctx,TN,KB,User,Vars).

invokeOperationProc(Verbose,assert(Flags,NEWFORM),Ctx,TN,KB,User,Vars):-  !,
        flag('Axioms Compiled',AC,AC+1),
        LN is AC + 1,
        flag(explaination_linenumber,_,LN),
        writeObject(Verbose,nl,Vars),
        writeObject(Verbose,NEWFORM,Vars),
        writeObject(Verbose,nl,Vars),
        logOnFailure(invokeInsert(Flags,surface,NEWFORM,Ctx,TN,KB,Vars,User)),
        ignore((getMooOption(opt_debug=off,on),Verbose=verbose,writeKnownFormsTN(Ctx,KB:TN))).


invokeOperationProc(Verbose,retract(Fml),Ctx,TN,KB,User,Vars):-
         flag('Axioms Compiled',AC,AC+1),
         writeObject(Verbose,nl,Vars),
         writeObject(Verbose,retract(Fml),Vars),
         writeObject(Verbose,nl,Vars),
         retract_pterm(Fml,Ctx,TN,KB,User,Vars).

invokeOperationProc(Verbose,canonicalize(Q),Ctx,TN,KB,User,Vars):-!,
         writeObject(Verbose,nl,Vars),
         writeObject(Verbose,canonicalize(Q),Vars),
         invokeRequestWithTime(canonicalize(Q),_,Ctx,TN,KB,User,Vars,CPU1), assert(findings(CPU1)).

invokeOperationProc(Verbose,request(Q),Ctx,TN,KB,User,Vars):-!,
         writeObject(Verbose,'\n\n\n;; Request \n\n',Vars),
         writeObject(Verbose,'request '(Q),Vars),
         writeObject(Verbose,nl,Vars),
         invokeRequestWithTime(request(Q),Ctx,TN,KB,User,Vars,CPU1), assert(findings(CPU1)).

invokeOperationProc(Verbose,request(N,Q),Ctx,TN,KB,User,Vars):-!,
         writeObject(Verbose,'\n\n\n;; Timed Request \n\n',Vars),
         writeObject(Verbose,'request '(Q),Vars),
         writeObject(Verbose,nl,Vars),
         invokeRequestWithTime(request(N,Q),Ctx,TN,KB,User,Vars,CPU1), assert(findings(CPU1)).

invokeOperationProc(Verbose,requestyn(Q),Ctx,TN,KB,User,Vars):-!,
         writeObject(Verbose,'\n\n\n;; Yes/No Request \n\n',Vars),
         writeObject(Verbose,'request '(Q),Vars),
         writeObject(Verbose,nl,Vars),
         invokeRequestWithTime(requestyn(Q),Ctx,TN,KB,User,Vars,CPU1), assert(findings(CPU1)).


invokeOperationProc(Verbose,Fml,Ctx,TN,KB,User,Vars):-               % Default Left-over
             toFSCK(Fml,Ctx,TN,Assertion,SourceCtx,SourceTN),
             invokeOperation(Verbose,assert(Assertion),SourceCtx,SourceTN,KB,User,Vars).

invokeRequestWithTime(Q,Ctx,TN,KB,User,Vars,CPU):-
                invokeRequestAndWriteUserAgent(Q,Ctx,TN,KB,User,Vars,CPU),
                assert(request_time(CPU)).


% ===============================================================
% AGENT REQUEST (Chars)
% ===============================================================
:-include('moo_header.pl').

tkq1:-agentRequest("(isa Joe ?Class)",'GlobalContext','PrologMOO','Dmiles',U,R,P).
tkq2:-agentRequest("(isa on Relation)",'GlobalContext','PrologMOO','Dmiles',U,R,P).


agentRequest(KIFCharsIn,Ctx,KB,User,UResultsSoFar,Result,Explaination):-
        agentRequest(KIFCharsIn,Ctx,KB,User,UResultsSoFar,Result,Explaination,Status),
        (Status = done(_) -> ! ; true).


agentRequest(KIFCharsIn,Ctx,KB,User,UResultsSoFar,Result,Explaination,Status):-
        isCharCodelist(KIFCharsIn),!,
        string_clean(KIFCharsIn,KIFChars),
        logOnFailure(ask_parse_chars(KIFChars,FmlInOpen,Vars)),!,
        agentRequest(KIFCharsIn,FmlInOpen,Vars,Ctx,KB,User,UResultsSoFar,Result,Explaination,Status).


agentRequest(KIFCharsIn,FmlInOpen,Vars,Ctx,KB,User,UResultsSoFar,Result,Explaination,Status):-
        notrace((
        retractall(answer_found(_)),
        retractall(t_answer_found(_)),
        retractall(tabled_f(_)),
        retractall(mooCache(deduceInstanceTable(KB,Class,Set))),
        ignore(findall(_,expireOptimizationsInKB(_,_,_),_)),
        retractall(table_g(_)),
        retractall(cpuend(_)),
        retractall(tabled_t(_)),
        retractall(table_g(_)),
        retractall(proving(_)),
        getMooOption(opt_kb,QKB),!,ignore((KB=QKB)),!,
        get_default_request_context(QCTX),!,ignore((Ctx=QCTX)),!,
        logOnFailure(ensureMooKB(KB,Ctx)),!,
        flag('$Returned Answers',_,0),
        flag('$UAnswers',_,0))),
        TN = User, % Tracks request based on 'User'
        destroyTN(KB,TN,_Ctx),  % Removes previous Request
        getOpenVariablesWFF(FmlInOpen,Vars,ChaseVars),
        getPrologVars(ChaseVars,QVars,_,_),
        RequestPrototype=..[request|QVars],
        notrace((not(not((
                getAssertionClauses(KB,Ctx,'<=>'(FmlInOpen,RequestPrototype),CAN,Vars,Flags),
                assert(mooCache(FmlInOpen,CAN,Flags,Vars,KB,Ctx,TN,User,Result)),!,
                (recanonicalizeTN(KB,TN)),    % Compile Belief
                assert(tq_attempted_request),
                writeDebug(blue,'Stage 1 - Compilation ':CAN:Flags)))))),!,
        agentRequestEach(FmlInOpen,RequestPrototype,ChaseVars,Ctx,KB,User,UResultsSoFar,Result,Explaination,Status),
        commitCleanExplaination(Explaination,ExplainationOut).


% ======================================================
% REQUEST PROC POSITIVE/NEGATIVE
% ======================================================

% Check For Undefines
agentRequestEach(KIFCharsIn,Formula,Vars,Ctx,KB,User,UResultsSoFar,Result,Explaination,Status):-  fail,
                once(getConstants(atomic,Formula,UsedConstants,_,_)),
                logOnFailure(checkAllConstantsHaveTypes(Formula,UsedConstants,UnDefinedList)),
                UnDefinedList=[_|_],!,writeObject(undefined_constants(UnDefinedList),_),!,fail.

% Check For Theorem
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,KB,User,1,['Result' = 'True'],formula(instance(FormulaIN,'Theorem')),done(true:thereom)):-
        resetTableFlags,
        writeDebug(purple,'Stage 2 - Theorem Check ':FmlInOpen),
        isTheorem(FmlInOpen,_),!,
        retain_yes,sendNote('(theorem)'),!.

% Call Belief
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,KB,User,UA, UVars,Explaination,Result):-
        agentBelief(FmlInOpen,URequest,UVars,Ctx,KB,User,UA, UVars, Explaination,Result).

% Request Failed
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,KB,User,0,['Result'='none'|_],'Unproven',done(possible:searchfailed)):-
        flag('$UAnswers',UA,UA),UA<1,!.

% Request Succeeded
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,KB,User,UA,['Result'='true'|_],found(UA),done(true:UA)):-!,
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
        notrace((
        (getMooOption(opt_backchains_max=BackchainsMax)),
        (getMooOption(opt_deductions_max=Deductions)),!,
        ignore((ground(URequest) -> Answers=1 ; Answers=PAnswers)),
        (getMooOption(opt_answers_max=Answers)),!,
        ignore(BackchainsMax=30),ignore(Answers=60),OAnswers is Answers,!)).

set_quit_time(Num):-
        notrace((
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


invokeRequestToBuffer(findall(V,Formula),Ctx,TrackingAtom,KB,User,Vars,CPU):-!,
        invokeRequestToBuffer(Formula,Ctx,TrackingAtom,KB,User,Vars,CPU).
invokeRequestToBuffer(request(Formula),Ctx,TrackingAtom,KB,User,Vars,CPU):-!,
         invokeRequestToBuffer(Formula,Ctx,TrackingAtom,KB,User,Vars,CPU),!.
invokeRequestToBuffer(requestyn(Formula),Ctx,TrackingAtom,KB,User,Vars,CPU):-!,
         invokeRequestToBuffer(Formula,Ctx,TrackingAtom,KB,User,Vars,CPU),!.

invokeRequestToBuffer(canonicalize(Formula),Ctx,TrackingAtom,KB,User,Vars,CPU):-!,
        retractall(requestBuffer_db(_,_,_,_)),
        retractall(final_answer(_)),
        getAssertionClauses(PreQ,KB,Ctx,Formula,Out,Vars,P),
        retain_answer(Out),
        writeObject('$spacer',Vars),
        asserta(tq_passed),
        asserta(tq_attempted_request),
        asserta(tq_least_one_answer),
        writeObject_conj(Out,Vars).


invokeRequestToBuffer(Formula,Ctx,TrackingAtom,KB,User,Vars,CPU):-
        getCputime(Start),!,
        logOnFailure(createRequestResponseBuffer_proc(Formula,Ctx,TrackingAtom,KB,User,Vars)),
        getCputime(End),ignore(Start=0),ignore(End=0),
        retractall(request_total_time(_)),
        CPU is End - Start,!,ignore(CPU=0),!,
        assert(request_total_time(CPU)),!.


createRequestResponseBuffer_proc(FmlInOpen,Ctx,TrackingAtom,KB,User,Vars):-
        assert(tq_attempted_request),
        retractall(requestBuffer_db(_,_,_,_)),
        retractall(final_answer((_))),
        agentRequest(KIFCharsIn,FmlInOpen,Vars,Ctx,KB,User,UResultsSoFar,Result,Explaination,Status),
        ignore(logOnFailure(retain_answer(Result))),
        writeDebug(addToResultBuffer(UResultsSoFar,Result,Explaination,Status)),
        addToResultBuffer(UResultsSoFar,Result,Explaination,Status),!. % Buffer Calls Itteration via failure

createRequestResponseBuffer_proc(FmlInOpen,Ctx,TrackingAtom,KB,User,Vars):-!.


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


