% ===================================================================
% File 'moo_useragetn.pl'
% Maintainer: Douglas Miles
% Contact: dmiles@users.sourceforge.net ; 
% Version: 'moo_useragent.pl' 1.0.0
% Revision:		$Revision: 1.1.1.1 $
% Revised At:	$Date: 2002-03-07 06:07:39 $

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

/*:-module(moo_useragent,
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
      setPrologFlag(architecture,Architecture),
      setPrologFlag(version,Version),
      setPrologFlag(host_cpu,Host_cpu),
      setPrologFlag(install_dir,Install_dir),
      setPrologFlag(release_date,Release_date),
      setPrologFlag(scheduling_strategy,Scheduling_strategy),
     % findall(L,library_directory(L),Library_directory),
      sendNote(user,logicEngine,'Initializing and Embedding Moo Logic Engine',['architecture=',Architecture,Host_cpu,Version,Release_date,Scheduling_strategy]),
      establish_startup_state,
      sendNote(debug,logicEngine,'KBs are now loaded by contentManager',' '),
      ensureMooKB('MooKernel','GlobalContext'),!.

%:-include('moo_header.pl').

% ===========================================================
% CONVERSE WITH JAVA
% ===========================================================

moo_ua(OptionsIn):-
		ignore(reset_wd),
		logOnFailure(fixOptionsFromForeign(OptionsIn,Options)),
		logOnFailure(setMooOption(Options)),
		ensureMooOption(client,html,ResponseType),
		writeModeSet(ResponseType),!,
		logOnFailure(parse_moo_ua(Options)),!.

reset_wd:-'LOGIC_ENGINE_RT'(Local),cd(Local).
reset_wd.

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
	  logOnFailure(safe_atom_to_term(AValue,DValue)),
	  fixOptionsFromForeign(List,ARGS).
fixOptionsFromForeign([AName=AValue|List],[DName=DValue|ARGS]):-  
	  logOnFailure(safe_atom_to_term(AName,DName)),
	  logOnFailure(safe_atom_to_term(AValue,DValue)),
	  logOnFailure(fixOptionsFromForeign(List,ARGS)).
fixOptionsFromForeign([AName|List],[DName|ARGS]):-  
	  safe_atom_to_term(AName,DName),
	  logOnFailure(fixOptionsFromForeign(List,ARGS)).

% transform_option(From,To)
transform_option(author,user).
transform_option(kb,opt_kb).
transform_option(language,interp).
transform_option(timeLimit,opt_timeout).
transform_option(bindingLimit,opt_answers_max).
transform_option(depthLimit,opt_backchains_max).


parse_moo_ua(Options):-memberchk(cmd='Halt LE',Options),halt.
parse_moo_ua(Options):-memberchk(cmd='Rebuild',Options),write('Rebuilding Moo').


%parse_moo_ua(Options):-memberchk(command=html,Options),!,
	
parse_moo_ua(Options):-memberchk(client=soap,Options),!,
	parse_moo_soap(Options).

parse_moo_ua(Options):-memberchk(client=moo_xml,Options),!,
	parse_moo_soap(Options).

% ===========================================================
% Surface Returns External TN
% ===========================================================
parse_moo_ua(Options):-memberchk(submit=tn_find,Options),!,
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
parse_moo_ua(Options):-memberchk(submit=tn_delete,Options),!,
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
	retractall(mooCache(Literal,AnteLiteral,_,KB,Ctx,TN)),	 %Rules 
	retractall(mooCache(Surface,CLF,Flags,Vars,KB,Ctx,TN,Maintainer,TMResult)).
	
destroyKB(KB):-
	retractall(mooCache(KB,_)),
	retractall(mooCache(KB,_,_)),
	retractall(mooCache(KB,_,_,_)),
	retractall(mooCache(Literal,_,KB,Ctx,TN)),  %Facts
	retractall(mooCache(Literal,AnteLiteral,_,KB,Ctx,TN)),	 %Rules 
	retractall(mooCache(Surface,CLF,Flags,Vars,KB,Ctx,TN,Maintainer,TMResult)),
	saveMooCache.
	
% ===========================================================
% Shows whats known about an External TN
% ===========================================================
parse_moo_ua(Options):-memberchk(submit=tn_show,Options),!,
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
parse_moo_ua(Options):-memberchk(submit=ask,Options),!, make,
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
parse_moo_ua(Options):-memberchk(submit=tn_sync,Options),!,
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
	
parse_moo_ua(Options):-memberchk(submit=tn_form_update,Options),!,
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

parse_moo_ua(Options):-memberchk(submit=canonicalize,Options),!,
	ensureMooOption(opt_kb='PrologMOO',KB),
	once(invokeKBCompilerProcess(KB)),write_ln('canonicalizing.\n').	

parse_moo_ua(Options):-memberchk(submit=blank_kb,Options),!,
	ensureMooOption(opt_kb='PrologMOO',KB),	
	destroyKB(KB),
	saveMooCache.


parse_moo_ua(Options):-memberchk(submit=removeKbNameSpace,Options),!,
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
parse_moo_ua(Options):-memberchk(submit=verify_assert,Options),!,
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
parse_moo_ua(Options):-memberchk(submit=show_dag,Options),!,
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
parse_moo_ua(Options):-memberchk(submit=assert,Options),!,
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
parse_moo_ua(Options):-memberchk(submit=canonicalize,Options),!,
	ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
	ensureMooOption(opt_kb='PrologMOO',KB),
	logOnFailure(invokeKBCompilerProcess(KB)).
	

% ===========================================================
% Create Knowledge Base (New)
% ===========================================================
parse_moo_ua(Options):-memberchk(submit=register_kb,Options),!,
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
parse_moo_ua(Options):-memberchk(submit=retract,Options),!,
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
parse_moo_ua(Options):-memberchk(submit='Delete Assertion',Options),!,
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

parse_moo_ua(Options):-
	memberchk(cmd='Show Cache',Options),!,
	writeFmt('<H3><Font Color=Red>Listing Cache Assertions...</Font></H3><PRE>',[]),	
	listing(mooCache),
	writeFmt('</PRE><BR><B>Done.</B>',[]),!.


listingt(NH):-catch((string_to_atom(NH,AA),listing(AA)),_,true).
%listingt(NH):-listing(NH),!.

% ===========================================================
% Invoke Prolog Command
% ===========================================================
parse_moo_ua(Options):- %memberchk(interp='prolog',Options),!,
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
	socket_out(Id,Out),
	socket_in(Id,In),
	invokePrologCommand(Id,In,Out,CMD,Vars,Results),
	writeFmt('\n       Results: ~w\n',[Results]),!.

	


% ===========================================================
% Logical Form Display
% ===========================================================
parse_moo_ua(Options):-memberchk(logicforms=logicforms,Options),!,
       parse_moo_lf(Options).
 

% ===========================================================
% Un-Canonicalize on KB/Ctx
% ===========================================================

parse_moo_ua(Options):-
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

parse_moo_ua(Options):-
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
parse_moo_ua(Options):-memberchk(cmd='Load SKB',Options),!,
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
parse_moo_ua(Options):-
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
parse_moo_ua(Options):-
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
parse_moo_ua(Options):-
	memberchk(cmd='Surface Check',Options),!,%trace,
	ensureMooOption(opt_ctx_assert='GlobalContext',Ctx),
	ensureMooOption(opt_kb='PrologMOO',KB),!,
	logOnFailure(surface_check_entire_kb_ctx(KB,Ctx)),!,
	writeFmt('<hr>Done Surface Check\n',[]).

% ===========================================================
% Invoke Prolog Command
% ===========================================================
parse_moo_ua(Options):- %memberchk(interp='prolog',Options),!,
	memberchk(submit='Clean Slate',Options),!,halt.
	    
% ===========================================================
% Invoke Prolog Command
% ===========================================================
parse_moo_ua(Options):- %memberchk(interp='prolog',Options),!,
	memberchk(cmd='Compile Demo',Options),!,autoload,
	(unsetMooOption(client=html)),qsave_program('moo_server',[goal=(initializeMooServerData,cs)]),(setMooOption(client=html)).

build_exe_and_halt:- !,autoload,
	(unsetMooOption(client=html)),qsave_program('moo_server',[goal=(initializeMooServerData,cs)]).
	
parse_moo_ua(Options):-parse_moo_enable(Options),!.



% ===========================================================
% Backup Moo
% ===========================================================
parse_moo_ua(Options):-memberchk(cmd='Save Image',Options),!,saveMooCache.
		                              
% ===========================================================
% Define New context
% ===========================================================
parse_moo_ua(Options):-memberchk(client='command',Options),memberchk(submitbuttonUp='Create',Options),!, 
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
parse_moo_ua(Options):-memberchk(client='controlpanel',Options), 
	memberchk(kill=_,Options),!,
	member(kill=ID,Options),
	catch(thread_signal(ID,thread_exit(user_killed(ID))),_,true),
	parse_moo_ua([client='controlpanel']).


parse_moo_ua(Options):-memberchk(client='controlpanel',Options), !,
	statistics,
	writeFmt('<hr><table border=1 width=80%><th>Id</th><th>Name</th><th>Status</th><th>Actions</th><th>Options</th><th>Goals</th>',[]),
	showMooProcesses,
	writeFmt('</table>',[]).

% ===========================================================
% TQ HOOK
% ===========================================================
parse_moo_ua(Options):-memberchk(client='tqsystem',Options),!,ignore(parse_moo_ua_tq(Options)).

% ===========================================================
% FALLBACKS
% ===========================================================

parse_moo_ua(Options):-memberchk(client=command,Options),!.

parse_moo_ua(Options):-%writeq(Options),write('<P>'),
	give_kif_window.

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


%=================================================================
%  LEGACY BELOW
%=================================================================



%=================================================================
%  REQUEST PROCESSING
%=================================================================

% Arity 1 (unused by Java)
ua_ask(RequestStringI):-!,ua_ask(RequestStringI,'GlobalContext',[]).

% Arity 2 (unused by Java)
ua_ask(RequestStringI,Options):-!,ua_ask(RequestStringI,'GlobalContext',Options).

% Arity 3 (used by Java)
ua_ask(RequestStringI,_Ctx,OptionList) :-
      setMooOption(OptionList),
      write_response_begin,!, %%trace,
     % elevate_status(kb(KB,_)=current),
      isMooOption(opt_kb=KB),
      once(ask(RequestStringI,_Ctx,KB)),
      write_response_end.

%=================================================================
%  ASSERT/RETRACT  PROCESSING
%=================================================================

% Make assertions  
% uaInsert(formulaObject(i),contextAtom(i),trackingAtom(i),OptionList(i))
% uaInsert("(mother Bill Jane)",sample_context,moo-44-545,[disp_modification] ).
% uaInsert("(assertion (=> (and (mother ?CH ?M) (siblings ?CH ?CH2)) (mother ?CH2 ?M ))",sample_context,moo-44-546,[disp_modification]).

% Make Retractions  
% ua_retract(assertionObject(i),OptionList(i))               TODO
% ua_retract("(assertion sample_context moo-44-545 (mother Bill Jane))",[disp_modification] ).
% ua_retract("(assertion sample_context moo-44-546 (=> (and (mother ?CH ?M) (siblings ?CH ?CH2)) (mother ?M ?CH2)))",[disp_modification] ). 


%=================================================================
%  ASSERT  PROCESSING
%=================================================================

% Arity 1 (unused by Java)
uaInsert(Assertion_Chars):-!,uaInsert(Assertion_Chars,'GlobalContext',_,[]).

% uaInsert(formulaObject(i),contextAtom(i),trackingAtom(i),OptionList(i))
uaInsert(Assertion_Chars,_Ctx,TrackingAtom,OptionList):-
      (((idGen(TN),TrackingAtom=(TN)) ; true)),
      setMooOption(OptionList),
      ignore(member(opt_kb=KB,OptionList)),
      write_response_begin,!, %%trace,
      ignore(once(tell(Assertion_Chars,_Ctx,TrackingAtom,KB,CMP))),
      write_response_end.

%=================================================================
% RETRACT  PROCESSING
%=================================================================

% Arity 1 (unused by Java)
ua_retract(Retraction_Chars):-!,ua_retract(Retraction_Chars,'GlobalContext',_,[]).

% ua_retract(formulaObject(i),contextAtom(i),trackingAtom(i),OptionList(i))
ua_retract(Retraction_Chars,_Ctx,TrackingAtom,OptionList):-
      (((idGen(TN),TrackingAtom=(TN)) ; true)),
      setMooOption(OptionList),
      ignore(member(opt_kb=KB,OptionList)),
      write_response_begin,!, %%trace,
      ignore(once(retract(Retraction_Chars,_Ctx,TrackingAtom,KB,CMP))),
      write_response_end.

%=================================================================
%  COMMAND PROCESSING
%=================================================================

% Arity 1 (unused by Java)
ua_command(CommandStringI):-ua_command(CommandStringI,[]).

% ua_command(CommandString(i),OptionList(i))
ua_command(CommandStringI,OptionList):-
      write_response_begin,
         setMooOption(OptionList),
         ask_parse_chars(CommandStringI,CMD,Vars),
      write_response_end.
         
ua_command_invoke(nil,_):-!.
ua_command_invoke(CMD,Vars):-
   sendNote(debug,kernel,'Making Prolog Call',writeq(CMD)),
            ua_out(request_start,ask_prolog(PrologKR),Vars),!,
            getCputime(Start),
            moo_belief(CMD,Vars),
            getCputime(Now),
            Elapsed is Now - Start,
            ua_out(request_end,('true',CMD,Elapsed,0,0),Vars).


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
:-dynamic(show_all_explainations).


/*
writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status):-
	format('<b>~q.</b>',writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status)),!.
*/
	
writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status):-not(show_all_explainations),
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

writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status):-show_all_explainations,
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
             getDefaultKB(KB),setMooOption(opt_kb=KB),
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


