%<?

:- include('logicmoo_utils_header.pl').

% ===================================================================
% IMPORTS
% ===================================================================
%% :-include('logicmoo_utils_header.pl'). %<?

%S=(instance(A, 'BodyJunction')=>exists(B, exists(C, connected(A, C)and connected(A, B)and instance(C, 'AnatomicalStructure')and instance(B, 'AnatomicalStructure')and not equal(C, B)))).

cons:- !,
      %catch(cd('../../knowledge_bases/'),_,true),
      pwd,
      repeat, %told,seen,
      (unsetMooOption(Agent,client=html)),
      once(console_loop(C)),
      fail.

writeMooMenu:-
         say('ask - switches to ask mode \ntell - switches back to tell mode\n '),
         say('other cmds: prolog_statistics, ls, halt, contexts \n'),!.



:-dynamic(console_mode/1).
:-assert_new(console_mode(ask)).

console_loop(C):- console_mode(M),!,
      console_loop(M ,C).


console_loop(tell,SOURCEFORM):- 
            once(console_read('Insert> ',SOURCEFORM,Vars)),    
            invokeOperation(verbose,assert(SOURCEFORM),'GlobalContext',TN,'LogicMOO',CM,Vars).

console_loop(ask,SOURCEFORM):- 
            once(console_read('Ask> ',SOURCEFORM,Vars)),
            invokeOperation(verbose,request(SOURCEFORM),Ctx,TN,Context,CM,Vars).
      
console_loop(cmd,SOURCEFORM):- 
            once(console_read('Command> ',SOURCEFORM,Vars)),
            invokeOperation(verbose,cmd(SOURCEFORM),Ctx,TN,Context,CM,Vars).

console_read(P,FORM,Vars):-
         nl,write(P),cons_read(Askion_Chars),
         logOnFailure(getCleanCharsWhitespaceProper(Askion_Chars,Show)),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
              catch(once(( NEWFORM=comment(_) -> 
                     (ignore(do_chars(Show)),!,FORM=_) ;(!,
		     FORM=NEWFORM      ))),_,fail).

cons_read(Chars):-repeat,told,seen,readKIF(Chars),Chars=[_,_|_].

lpInsert(Insert_chars):-
      tell(Insert_chars,Ctx,TN,Context,CM).

lpInsert_file(File):-lp_file(File).

lp_list:-
      say('Listing of the current assertions:\n',[]),
      listing(mooCache),
      say('Done.\n',[]).

lp_clear:-do_clear.

lp_save(FileName):-!.

lp_ask(Chars):-ask(Chars,_Cxt,Context).

   
cons_help:-!.

do_chars([59|Chars]):-!,do_chars(Chars).
do_chars(Chars):-tokenize3(Chars,Tokens),!,do_tokens(Tokens).

 

do_tokens([(;)|Toks]):-!,do_tokens(Toks).
do_tokens([tell]):-say('% Entering tell mode.\n',[]),retractall(console_mode(_)),assert(console_mode(tell)).
do_tokens([ask]):- say('% Entering ask mode.\n',[]),retractall(console_mode(_)),assert(console_mode(ask)).
do_tokens([cmd]):- say('% Entering command mode.\n',[]),retractall(console_mode(_)),assert(console_mode(cmd)).
do_tokens([clear]):- clear_moo_memory.
do_tokens([list]):- lp_list.
do_tokens([load,Filename]):- do_tokens([load,Filename,'GlobalContext']).
do_tokens([load,Filename,Ctx]):- ctxFromFile(Ctx,Filename,'LogicMOO').

%do_chars(X):-string_concat(";find ",Word,X),find_word(Word).
do_tokens([make]):- make.
do_tokens([help]):- writeMooMenu.
do_tokens([halt]):- halt.
do_tokens([bye]):- abort.
%do_chars([116,109|Number]):-catch((number_codes(Value,Number),set_tm_level(Value)),_,show_tm_level).
do_tokens([prolog]):- say('Type ""cons"" to return to Logic Engine.\n ',[]),abort.
do_tokens([can,X]):-canonicalizeMooContextHTML('LogicMOO',X).
do_tokens([can,X]):-canonicalizeMooContextHTML('LogicMOO',X).
do_tokens([can]):-do_tokens([can,'GlobalContext']).
do_tokens(L):-P=..L,once(P),!.
do_tokens(X):-say('could not parse: ~q.\n',[X]).
%do_chars(PCHARS):- atom_codes(Atom,PCHARS),catch(atom_to_term(Atom,Term,Vars),_,fail),!,catch(my_call(Term,Vars),_,fail).



theory_define(Context):-invokeInsert(forall,surface,'instance'(Context,'KnowledgeBase'),'GlobalContext',TN,Context,Vars,Maintainer),
                     invokeInsert(forall,surface,'instance'('GlobalContext','Context'),'GlobalContext',TN2,Context,Vars,Maintainer),
                     ensureMooContext(Context,'GlobalContext').

ctx_define(Ctx):-invokeInsert(forall,surface,'instance'(Ctx,'Context'),'GlobalContext',TN,SContext,Vars,Maintainer),ensureMooContext(Context,Ctx).

set_ctx(Context):-ensureMooContext(KnowledgeBase,Context).
set_theory(KnowledgeBase):-ensureMooContext(KnowledgeBase,'GlobalContext').

my_call(fol,_):-retractall(version_tag(_)),assert(version_tag(fol)),say('% Entering FOL mode.\n',[]),!.
my_call(theoryl,_) :-retractall(version_tag(_)),assert(version_tag(theoryl)),say('% Entering ContextL mode.\n',[]),!.

my_call(cd(Term),_Vars):-cd(Term),pwd.

my_call(file(Term),_Vars):-lp_file(Term),say('% Entering ask mode.\n',[]),retractall(console_mode(_)),assert(console_mode(ask)).
my_call(file(Term),_Vars):- say('% Entering ask mode.\n',[]),retractall(console_mode(_)),assert(console_mode(ask)).

my_call(Term,_Vars):-once(Term).


do_chars(Show):-%,catch(fmtString(Atom,'"~s"',[Show]),_,ignore(Atom=surf)),
	sendNote(user,'kifParser','KIF Unreadable (Syntax error: Unbalanced parentheses  )',Atom).


call_nth_times(Number,Prolog):-
                     retractall(call_count(_)),assert(call_count(Number)),repeat,
                     (Prolog),
                     ignore((retract(call_count(Q)),QQ is Q -1,
                     assert(call_count(QQ)))),
                     call_count(0).

test_batch_stdout(FileName):- test_batch_stdout(FileName,CM,CPU,RESULT,Title).

test_batch_stdout(FileName,CM):- test_batch_stdout(FileName,CM,CPU,RESULT,Title).


% ===================================================================
%  BATCH INTERFACE
% ===================================================================

invokeOperation(Verbose,surf,Ctx,Tracking,Context,Agent,Vars):-!.
invokeOperation(Verbose,Fml,Ctx,TN,Context,Agent,Vars):-!,
                once(invokeOperationProc(Verbose,Fml,Ctx,TN,Context,Agent,Vars)),!.

invokeOperationProc(Verbose,TERM,Ctx,TN,Context,Agent,Vars):-TERM =..[note|T],!,
                assert(title(T)),
                WT=..['note '|T],
                writeObject(Verbose,WT,Vars),!,
                writeObject(Verbose,'\n\n;; Assertions \n\n',Vars),!,
                clear_moo_memory,set_promiscuous,!.

invokeOperationProc(Verbose,TERM,Ctx,TN,Context,Agent,Vars):-TERM =..[answer|T],!,batch_answer(T).

invokeOperationProc(Verbose,TERM,Ctx,TN,Context,Agent,Vars):-TERM =..[abort],!,assert(tq_ignored).
invokeOperationProc(Verbose,'deduce-domains'(on),Ctx,TN,Context,Agent,Vars):-!,set_promiscuous,!.
invokeOperationProc(Verbose,'deduce-domains'(true),Ctx,TN,Context,Agent,Vars):-!,set_promiscuous,!.
invokeOperationProc(Verbose,'deduce-domains'(false),Ctx,TN,Context,Agent,Vars):-!,unset_promiscuous,!.
invokeOperationProc(Verbose,'deduce-domains'(off),Ctx,TN,Context,Agent,Vars):-!,unset_promiscuous,!.

invokeOperationProc(Verbose,end_of_file,Ctx,TN,Context,Agent,Vars):-!.

invokeOperationProc(Verbose,'file_comment'(C),SourceCtx,SourceTN,Context,Agent,Vars):-!,nl,write(C),!.
invokeOperationProc(Verbose,'browser-only'(_),SourceCtx,SourceTN,Context,Agent,Vars):-!.
invokeOperationProc(Verbose,'execute-prolog'(Code),SourceCtx,SourceTN,Context,Agent,Vars):-!,
once(((%                writeq(Code),nl,
                atom_codes(Code,Codes),
%               writeq(Codes),nl,
                getUnquotedCodes(Codes,Chars),
%               writeq(Chars),nl,
                atom_codes(Atom,Chars),
                atom_to_term(Atom,Term,V)

                );say('could not parse ~w\n',[Code])),once(((once(catch(Term,_,fail)),say('Exec Suceeded',[]));say('Exec Failed',[])))).
invokeOperationProc(Verbose,'load-theory'(Filename),SourceCtx,SourceTN,Context,Agent,Vars):-!,fail,agent_load_kif_quiet(Filename,'GlobalContext','Anonymous').
invokeOperationProc(Verbose,'load-theory-show'(Filename),SourceCtx,SourceTN,Context,Agent,Vars):-!,
        agent_load_kif(Filename,'GlobalContext','Anonymous').

invokeOperationProc(Verbose,retract(nil),Ctx,TN,Context,Agent,Vars):-!.

invokeOperationProc(Verbose,request(nil),Ctx,TN,Context,Agent,Vars):-!.

invokeOperationProc(Verbose,(nil),Ctx,TN,Context,Agent,Vars):-!.

invokeOperationProc(Verbose,tell(nil),Ctx,TN,Context,Agent,Vars):-!.

invokeOperationProc(Verbose,assert(end_of_file),Ctx,TN,Context,Agent,Vars):- !.

invokeOperationProc(Verbose,assert(NEWFORM),Ctx,TN,Context,Agent,Vars):-  !,
        invokeOperationProc(Verbose,assert([trusted,canonicalize,to_mem],NEWFORM),Ctx,TN,Context,Agent,Vars).

invokeOperationProc(Verbose,assert(Flags,NEWFORM),Ctx,TN,Context,Agent,Vars):-  !,
        flag('Axioms Compiled',AC,AC+1),
        LN is AC + 1,
        flag(explaination_linenumber,_,LN),
        writeObject(Verbose,nl,Vars),
        writeObject(Verbose,NEWFORM,Vars),
        writeObject(Verbose,nl,Vars),
        logOnFailure(invokeInsert(Flags,surface,NEWFORM,Ctx,TN,Context,Vars,Agent)),
        ignore((getMooOption(Agent,opt_debug=off,on),Verbose=verbose,writeKnownFormsTN(Ctx,Context:TN))).


invokeOperationProc(Verbose,retract(Fml),Ctx,TN,Context,Agent,Vars):-
         flag('Axioms Compiled',AC,AC+1),
         writeObject(Verbose,nl,Vars),
         writeObject(Verbose,retract(Fml),Vars),
         writeObject(Verbose,nl,Vars),
         retract_pterm(Fml,Ctx,TN,Context,Agent,Vars).

invokeOperationProc(Verbose,canonicalize(Q),Ctx,TN,Context,Agent,Vars):-!,
         writeObject(Verbose,nl,Vars),
         writeObject(Verbose,canonicalize(Q),Vars),
         invokeRequestWithTime(canonicalize(Q),Ctx,TN,Context,Agent,Vars,CPU1), assert(findings(CPU1)).

invokeOperationProc(Verbose,request(Q),Ctx,TN,Context,Agent,Vars):-!,
         writeObject(Verbose,'\n\n\n;; Request \n\n',Vars),
         writeObject(Verbose,'request '(Q),Vars),
         writeObject(Verbose,nl,Vars),
         invokeRequestWithTime(request(Q),Ctx,TN,Context,Agent,Vars,CPU1), assert(findings(CPU1)).

invokeOperationProc(Verbose,request(N,Q),Ctx,TN,Context,Agent,Vars):-!,
         writeObject(Verbose,'\n\n\n;; Timed Request \n\n',Vars),
         writeObject(Verbose,'request '(Q),Vars),
         writeObject(Verbose,nl,Vars),
         invokeRequestWithTime(request(N,Q),Ctx,TN,Context,Agent,Vars,CPU1), assert(findings(CPU1)).

invokeOperationProc(Verbose,requestyn(Q),Ctx,TN,Context,Agent,Vars):-!,
         writeObject(Verbose,'\n\n\n;; Yes/No Request \n\n',Vars),
         writeObject(Verbose,'request '(Q),Vars),
         writeObject(Verbose,nl,Vars),
         invokeRequestWithTime(requestyn(Q),Ctx,TN,Context,Agent,Vars,CPU1), assert(findings(CPU1)).


invokeOperationProc(Verbose,Fml,Ctx,TN,Context,Agent,Vars):-               % Default Left-over
             toFSCK(Fml,Ctx,TN,Assertion,SourceCtx,SourceTN),
             invokeOperation(Verbose,assert(Assertion),SourceCtx,SourceTN,Context,Agent,Vars).

invokeRequestWithTime(Q,Ctx,TN,Context,Agent,Vars,CPU):-
                invokeRequestAndWriteUserAgent(Q,Ctx,TN,Context,Agent,Vars,CPU),
                assert(request_time(CPU)).


% ===============================================================
% AGENT REQUEST (Chars)
% ===============================================================

tkq1:-agentRequest("(isa Joe ?Class)",'GlobalContext','LogicMOO','Dmiles',U,R,P).
tkq2:-agentRequest("(isa on Relation)",'GlobalContext','LogicMOO','Dmiles',U,R,P).


agentRequest(KIFCharsIn,Ctx,Context,Agent,UResultsSoFar,Result,Explaination):-
        agentRequest(KIFCharsIn,Ctx,Context,Agent,UResultsSoFar,Result,Explaination,Status),
        (Status = done(_) -> ! ; true).


agentRequest(KIFCharsIn,Ctx,Context,Agent,UResultsSoFar,Result,Explaination,Status):-
        isCharCodelist(KIFCharsIn),!,
        string_clean(KIFCharsIn,KIFChars),
        logOnFailure(ask_parse_chars(KIFChars,FmlInOpen,Vars)),!,
        agentRequest(KIFCharsIn,FmlInOpen,Vars,Ctx,Context,Agent,UResultsSoFar,Result,Explaination,Status).


agentRequest(KIFCharsIn,FmlInOpen,Vars,Ctx,Context,Agent,UResultsSoFar,Result,Explaination,Status):-
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
        getMooOption(Agent,opt_theory,QContext),!,ignore((Context=QContext)),!,
        get_default_request_context(QCTX),!,ignore((Ctx=QCTX)),!,
        logOnFailure(ensureMooContext(Context,Ctx)),!,
        flag('$Returned Answers',_,0),
        flag('$UAnswers',_,0))),
        TN = Agent, % Tracks request based on 'Agent'
        destroyTN(Context,TN,_Ctx),  % Removes previous Request
        getOpenVariablesWFF(FmlInOpen,Vars,ChaseVars),
        getPrologVars(ChaseVars,QVars,_,_),
        RequestPrototype=..[request|QVars],
        ((not(not((
                getAssertionClauses(Context,'<=>'(FmlInOpen,RequestPrototype),CAN,Vars,Flags),
                assert(mooCache(FmlInOpen,CAN,Flags,Vars,Context,TN,Agent,Result)),!,
                (recanonicalizeTN(Context,TN)),    % Compile Belief
                assert(tq_attempted_request),
                writeDebug(blue,'Stage 1 - Compilation ':CAN:Flags)))))),!,
        agentRequestEach(FmlInOpen,RequestPrototype,ChaseVars,Ctx,Context,Agent,UResultsSoFar,Result,Explaination,Status),
        commitCleanExplaination(Explaination,ExplainationOut).


% ======================================================
% REQUEST PROC POSITIVE/NEGATIVE
% ======================================================

% Check For Undefines
agentRequestEach(KIFCharsIn,Formula,Vars,Ctx,Context,Agent,UResultsSoFar,Result,Explaination,Status):-  fail,
                once(getConstants(atomic,Formula,UsedConstants,_,_)),
                logOnFailure(checkAllConstantsHaveTypes(Formula,UsedConstants,UnDefinedList)),
                UnDefinedList=[_|_],!,writeObject(undefined_constants(UnDefinedList),_),!,fail.

% Check For Theorem
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,Agent,1,['Result' = 'True'],formula(instance(FormulaIN,'Theorem')),done(true:thereom)):-
        resetTableFlags,
        writeDebug(purple,'Stage 2 - Theorem Check ':FmlInOpen),
        isTheorem(FmlInOpen,_),!,
        retain_yes,sendNote('(theorem)'),!.

% Call Belief
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,Agent,UA, UVars,Explaination,Result):-
        agentBelief(FmlInOpen,URequest,UVars,Ctx,Context,Agent,UA, UVars, Explaination,Result).

% Request Failed
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,Agent,0,['Result'='none'|_],'Unproven',done(possible:searchfailed)):-
        flag('$UAnswers',UA,UA),UA<1,!.

% Request Succeeded
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,Agent,UA,['Result'='true'|_],found(UA),done(true:UA)):-!,
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
        (getMooOption(Agent,opt_backchains_max=BackchainsMax)),
        (getMooOption(Agent,opt_deductions_max=Deductions)),!,
        ignore((ground(URequest) -> Answers=1 ; Answers=PAnswers)),
        (getMooOption(Agent,opt_answers_max=Answers)),!,
        ignore(BackchainsMax=30),ignore(Answers=60),OAnswers is Answers,!)).

set_quit_time(Num):-
        ((
        (getMooOption(Agent,opt_timeout=QuitTime)),!,ignore(QuitTime=5),!,
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


invokeRequestToBuffer(findall(V,Formula),Ctx,TrackingAtom,Context,Agent,Vars,CPU):-!,
        invokeRequestToBuffer(Formula,Ctx,TrackingAtom,Context,Agent,Vars,CPU).
invokeRequestToBuffer(request(Formula),Ctx,TrackingAtom,Context,Agent,Vars,CPU):-!,
         invokeRequestToBuffer(Formula,Ctx,TrackingAtom,Context,Agent,Vars,CPU),!.
invokeRequestToBuffer(requestyn(Formula),Ctx,TrackingAtom,Context,Agent,Vars,CPU):-!,
         invokeRequestToBuffer(Formula,Ctx,TrackingAtom,Context,Agent,Vars,CPU),!.

invokeRequestToBuffer(canonicalize(Formula),Ctx,TrackingAtom,Context,Agent,Vars,CPU):-!,
        retractall(requestBuffer_db(_,_,_,_)),
        retractall(final_answer(_)),
        getAssertionClauses(Context,Formula,Out,Vars,P),
        retain_answer(Out),
        writeObject('$spacer',Vars),
        asserta(tq_passed),
        asserta(tq_attempted_request),
        asserta(tq_least_one_answer),
        writeObject_conj(Out,Vars).


invokeRequestToBuffer(Formula,Ctx,TrackingAtom,Context,Agent,Vars,CPU):-
        getCputime(Start),!,
        logOnFailure(createRequestResponseBuffer_proc(Formula,Ctx,TrackingAtom,Context,Agent,Vars)),
        getCputime(End),ignore(Start=0),ignore(End=0),
        retractall(request_total_time(_)),
        CPU is End - Start,!,ignore(CPU=0),!,
        assert(request_total_time(CPU)),!.


createRequestResponseBuffer_proc(FmlInOpen,Ctx,TrackingAtom,Context,Agent,Vars):-
        assert(tq_attempted_request),
        retractall(requestBuffer_db(_,_,_,_)),
        retractall(final_answer((_))),
        agentRequest(KIFCharsIn,FmlInOpen,Vars,Ctx,Context,Agent,UResultsSoFar,Result,Explaination,Status),
        ignore(logOnFailure(retain_answer(Result))),
        writeDebug(addToResultBuffer(UResultsSoFar,Result,Explaination,Status)),
        addToResultBuffer(UResultsSoFar,Result,Explaination,Status),!. % Buffer Calls Itteration via failure

createRequestResponseBuffer_proc(FmlInOpen,Ctx,TrackingAtom,Context,Agent,Vars):-!.


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


/*
Loads and compiles a Maintainerial KIF file
agentLoadKif(+Filename,+GlobalContext,+DefiningMaintainer).
*/

agentLoadKif(Filename):-
        set_automation,
        agent_load_kif(Filename,Filename,Filename).

agentLoadKif(Filename,GlobalContext,Agent):-
        set_automation,
        agent_load_kif_quiet(Filename,GlobalContext,Agent).


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
agentInsert(+KIFCharsIn,+Ctx,+Context,+Agent).
KIFCharsIn = "(instance virginia Computer)"
Ctx = 'Computers and Things'
Context = 'LogicMOO' currently must be set to 'LogicMOO'
*/

agentInsert(KIFCharsIn,Ctx,Context,Agent):-
        set_automation,
        agentInsert(KIFCharsIn,Ctx,TN,Context,Agent).


/*
Now the predicates that you most want
Invokes a request
agentRequest(+KIFCharsIn,+Ctx,+Context,+Agent,-UResultsSoFar,-Result,-Explaination,-Status).

Input variables:
KIFCharsIn = "(instance ?What Computer)"
Ctx = 'Computers and Things'  % Any legal Atom that helps you book keep
Context = 'LogicMOO' currently must be set to 'LogicMOO'
Agent = ''  There are times when error messages about redundant assertions happen.. and for bookkeeping all assertions have an author atom.. perhaps you can even use the source file name it came from.

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
setMooOption(Agent,VariableName=NewValue).
*/

/*
Retrive Belief Optional Variable
isMooOption(Agent,VariableName=CurrentValue).
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
               % (unsetMooOption(Agent,client=_)),
                %(setMooOption(Agent,client=automata)).


% ===================================================================
%  INVOKE REQUEST INTERFACE USED BY REQUEST AGENTS
% ===================================================================

%% :-include('logicmoo_utils_header.pl'). %<?

% =======================================================================================

invokeRequestAndWriteUserAgent(Formula,Ctx,TrackingAtom,Context,Agent,Vars,CPU):-
         invokeRequestToBuffer(Formula,Ctx,TrackingAtom,Context,Agent,Vars,CPU),
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

/*
writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status):-
        format('<b>~q.</b>',writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status)),!.
*/

writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status):-not(isMooOption(Agent,show_all_explainations,true)),
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

writeAnswersUserAgent(UResultsSoFar,Result,InExplaination,Status):-isMooOption(Agent,show_all_explainations,true),
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


