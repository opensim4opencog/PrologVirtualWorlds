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

getDefaultKB('PrologMOO').

%:-include('moo_header.pl').
:-dynamic(title/1).

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
once(((%		writeq(Code),nl,
		atom_codes(Code,Codes),
%		writeq(Codes),nl,
		getUnquotedCodes(Codes,Chars),
%		writeq(Chars),nl,
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
	getDefaultKB(QKB),!,ignore((KB=QKB)),!,
	get_default_request_context(QCTX),!,ignore((Ctx=QCTX)),!,
	logOnFailure(ensureMooKB(KB,Ctx)),!,
	flag('$Returned Answers',_,0),
	flag('$UAnswers',_,0))),
	TN = User, % Tracks request based on 'User'
	destroyTN(KB,TN,_Ctx),	% Removes previous Request
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
	assert(final_answer(How)),!.	%Finalize on done/1.
	
addToResultBuffer(UResultsSoFar,Result,Explaination,Status):-
	once((numbervars((UResultsSoFar,Result,Explaination,Status),'$VAR',0,_),
	assert(requestBuffer_db(UResultsSoFar,Result,Explaination,Status)))),
	fail.









