:-include('logicmoo_utils_header.pl').
% ===================================================================
% ===================================================================
% ===================================================================
clientEvent(Channel,Agent,[file='cycl.moo'|A]):-!,
	 writeHTMLStdHeader('CycL Assert/Query'),
	 getMooOption(Agent,formula='(#$genls ?X #$Omnivore)',Formula),!,
	 say('<form method="GET">'),
	 showContextHtml(Ctx),
	 say('<p><textarea rows="9" name="formula" cols="40">~w</textarea><br>
	 <input type="submit" value="Assert" name="submit">&nbsp;
	 <input type="submit" value="Query" name="submit">&nbsp;
	 <input type="reset" value="Reset" name="resetButton"></p>  Inference module: ',[Formula]),!,
	 %writeObject(select('inferenceType',['Full','Facts','Asserted','Extreme'])),
	 say('</form>'),!,
	 getMooOption(Agent,ctx,Mt),!,
	 getMooOption(Agent,submit,Submit),!,
	 getMooOption(Agent,inferenceType,IT),!,
	 once((atom_codes(Formula,Chars),getSurfaceFromChars(Chars,Goal,Vars))),
	 balanceBinding(Goal,Assert),
	 getCputime(Before),
	 processRequestCycL(Submit,Formula,Chars,Goal,Assert,Vars,Mt,IT),
	 getCputime(After),Total is After - Before,
	 say('~n<br><b>Total time was ~w secs</b>',[Total]),!,
	 writeHTMLStdFooter,!.

processRequestCycL(Submit,Formula,Chars,Goal,Assert,Vars,Mt,IT):-var(Formula),!.
processRequestCycL('Query',Formula,Chars,Goal,Assert,Vars,Mt,IT):-
      ignore(catch((andCall(Goal),writeCycLVarBindings(Vars),fail),E,say('<br>~n~q',[E]))),!.
processRequestCycL('Assert',Formula,Chars,Goal,Assert,Vars,Mt,IT):-
      say('<pre>'),
      say('<br>asserting...<br>'),
      ignore(writeObject(Assert,Vars)),
      ignore(assertCycLWithTMS(Assert,Vars,Mt,IT)),
      say('</pre>').

      
% 
assertCycLWithTMS(Assert,Vars,Mt,IT):- 
	    getPrologVars(Assert, _, Singles, Multiples),Singles=[X|L],
	    say('~n<font color=red>Assertion veto by Truth Compiler~nReason: Contains variables that are not quantified~nTry using quantifiers like #$forAll or #$thereExists</font>~n'),
	    writeObject([X|L],Vars).
assertCycLWithTMS(Assert,Vars,Mt,IT):-
      say('~ninto ~w',[Mt]),
      invokeOperationProc(verbose,canonicalize(Assert),Mt,TN,Context,Agent,Vars).

processRequestCycL(Submit,Formula,Mt,IT).

writeCycLVarBindings(Vars):-writeObject('<br>'),writeObject(getPrologVars(Vars)),!.

fixSvade:-
   saved(A,B,C,D),
   fixSaved(A,B,C,D,After),
   format('~q.~n',[After]),
   fail.

fixSaved(A,B,C,_,After):-
      A=..[P|List],
      transoutString(List,ListO),
      getConstants(atom,ListO,Items,_,_),
      Vector=..[v|Items],
      append([P|ListO],[B,v(Vector,C)],NewL),
      After=..NewL,!.
   
fixDb:-see('../world/cyc.el'),
   repeat,
      read(Term),
      convertTermNL(Term,Out),
      format('~q.~n',Out),
      Term==end_of_file.


convertTermNL(':-'(Term),':-'(Term)):-nonvar(Term),!.
convertTermNL(Term,Term):-not(compound(Term)),!.
%convertTermNL([T|Erm],Term):-ground([T|Erm]),Term=..['[]'|[T|Erm]],!.
convertTermNL([T|Erm],[T|Erm]):-!.
convertTermNL(Term,List):-functor(Term,[],_),Term=..[_|List],!.
convertTermNL(Term,O):-Term=..List,convertTermNLS(List,ListO),O=..ListO,!.

convertTermNL(Term,Term).
convertTermNLS([],[]).
convertTermNLS([H|T],[HH|TT]):-convertTermNL(H,HH),convertTermNLS(T,TT).

% ===================================================================
% File 'opencyc.pl'
% Purpose: Lower-level connection based utilities for interfacing to OpenCyc from SWI-Prolog
% Maintainer: Douglas Miles
% ===================================================================

/*
:-module(opencyc,[
	 cycInit/0,
	 getCycConnection/3,
	 finishCycConnection/3,
	 invokeSubL/1,
	 invokeSubL/2,
	 invokeSubLRaw/2,
	 cycInfo/0,
	 printSubL/2,
	 getMooOption/2,
	 setMooOption/2,
	 unsetMooOption/1,
	 formatCyc/3,
	 toCycApiExpression/2,
	 toCycApiExpression/3,
	 cycQuery/1,
	 cycQuery/2,
	 cycQuery/6,
	 cycQuery/8,
	 toMarkup/4,
	 cycAssert/1,
	 cycAssert/2,
	 cycRetract/1,
	 is_string/1,
	 balanceBinding/2,
	 cycRetract/2,
	 cycRetractAll/1,
	 cycRetractAll/2,
	 isDebug/0,
	 makeConstant/1,
	 ensureMt/1,
	 makeConstant/1,
	 readCycL/2,
	 cyclify/2,
	 idGen/1,
	 subst/4,
	 isMooOption/2,
	 cyclifyNew/2,
	 unNVar/2,
	 defaultAssertMt/1,
	 mtAssertPred/2,
	 isRegisterCycPred/3,
	 registerCycPred/1,
	 setMooOption/2,
	 getCycLTokens/2,
	 registerCycPred/2,
	 registerCycPred/3,
         getSurfaceFromChars/3,
	 assertThrough/1,
	 assertThrough/2,
	 assertThrough/2,
	 assertThrough/2,
	 writel/1,
	 writel/2,
	 atomSplit/2,
	 list_to_term/2,
	 testOpenCyc/0,
	 
	 createPrologServerThread/1,
	 xmlPrologServer/1,
	 read_line_with_nl/3,
	 decodeRequest/2,
	 invokePrologCommandRDF/6,
	 serviceAcceptedClientSocketAtThread/1,

	 servantProcessCreate/1,
	 servantProcessCreate/3,
	 servantProcessCreate/4,
	 servantProcessCreate/5,
	 isMooProcess/2,
	 isMooProcess/5,
	 createProcessedGoal/1,
	 servantProcessSelfClean/0,
	 showMooStatisticsHTML/0,
	 cleanOldProcesses/0,
	 showMooStatisticsHTML/0,

	 readUntil/3,

	 writeDebug/1,
	 writeDebug/2,
	 writeDebugFast/1,
	 logOnFailureIgnore/1,
	 sendNote/1,
	 sendNote/4,
	 writeFailureLog/2,
	 debugOnFailure/2,
	 writeObject/2,
	 writeObject/3,
	 writeObject_conj/2,
	 
	 getCputime/1,
	       unNVar/2,
	       writeSTDERR/1,
	       writeSTDERR/2,
	       say/1,
	       say/2,
	       say/3,
	       fmtString/2,
	       fmtString/3,
	       writeFmtFlushed/1,
	       writeFmtFlushed/2,
	       writeFmtFlushed/3,
	       saveUserInput/0,
	       writeSavedPrompt/0,
	       if_prolog/2,
	       callIfPlatformWin32/1,
	       callIfPlatformUnix/1,
	       prologAtInitalization/1,
	       prolog_thread_create/3,
	       prolog_current_thread/2,
	       prolog_thread_exit/1,
	       prolog_thread_self/1,
	       prolog_thread_at_exit/1,
	       prolog_thread_signal/2,
	       prolog_thread_join/2,
	       prolog_notrace/1,
	       prolog_statistics/0

	 ]).

*/

%:- set_prolog_flag(optimise,true).
%:- set_prolog_flag(file_name_variables,false).
%:- set_prolog_flag(agc_margin,0).
%:- set_prolog_flag(trace_gc,false).
%:- set_prolog_flag(gc,false).
%:-set_prolog_flag(character_escapes,true).
%:-set_prolog_flag(double_quotes,codes).
%:-set_prolog_flag(double_quotes,string).
:-set_prolog_flag(report_error,true).
%:-set_prolog_flag(verbose,normal).
:-dynamic(cycConnection/3).
:-dynamic(cycConnectionUsed/3).
:-dynamic(cycMutex/2).
:-dynamic(cycChatMode/1).


cycBaseJavaClass('org.opencyc.prolog.JavaRt').

:-ensure_loaded('logicmoo_bb_options.pl').

%:- use_module((javart)).


% ===================================================================
% Cyc initialization - call cycInit. once and this fiule will be loaded if not already
% ===================================================================
cycInit.


:-setMooOptionDefaults.

:-setMooOption(Agent,cycServer,'127.0.0.1':3601).
:-setMooOption(Agent,query(backchains),1).
:-setMooOption(Agent,query(number),nil).
:-setMooOption(Agent,query(time),nil). %max ten seconds maybe?
:-setMooOption(Agent,query(depth),nil).

:-setMooOption(Agent,defaultAssertOptions,[':DIRECTION', ':FORWARD', ':STRENGTH', ':MONOTONIC']).
:-setMooOption(Agent,':DIRECTION', ':FORWARD').
:-setMooOption(Agent,':STRENGTH', ':MONOTONIC').
:-setMooOption(Agent,hookCycPredicates,true).


writel(Lisp):-
   toCycApiExpression(Lisp,Chars),!,
   format('~w',[Chars]).

writel(Stream,Lisp):-
   toCycApiExpression(Lisp,Chars),!,
   format(Stream,'~w',[Chars]).
% ===================================================================
% Connecter to Cyc TCP Server
% ===================================================================

getCycConnection(ConnectionParams,SocketId,OutStream,InStream):-
      setMooOption(Agent,cycServer,ConnectionParams),!,
      disconnectAllCycs,
      getCycConnection(SocketId,OutStream,InStream).

disconnectAllCycs:-
      retract(opencyc:cycConnection(SocketId,OutStream,InStream)),
      tcp_close_socket(SocketId),
      fail.
disconnectAllCycs.

getCycConnection(SocketId,OutStream,InStream):-
      retract(opencyc:cycConnection(SocketId,OutStream,InStream)),
      assertz(opencyc:cycConnectionUsed(SocketId,OutStream,InStream)),!.

getCycConnection(SocketId,OutStream,InStream):-
      tcp_socket(SocketId),
      isMooOption(Agent,cycServer,Server),
      tcp_connect(SocketId,Server),
      tcp_open_socket(SocketId, InStream, OutStream),!,
      isDebug((format(user_error,'Connected to Cyc TCP Server {~w,~w}\n',[InStream,OutStream]),flush_output(user_error))),
      assertz(opencyc:cycConnectionUsed(SocketId,OutStream,InStream)),!.

finishCycConnection(SocketId,OutStream,InStream):-
      ignore(system:retractall(opencyc:cycConnectionUsed(SocketId,OutStream,InStream))),
      asserta(opencyc:cycConnection(SocketId,OutStream,InStream)),!.
      
% ===================================================================
% cycInfo. - Prints Cyc Usage info to current output 
% ===================================================================
cycInfo:- % will add more 
   listing(cycConnection),
   listing(cycConnectionUsed),
   listing(cycConstantMade),
   listing(isMooOption).


% ===================================================================
% Invoke SubL
% invokeSubLRaw(-Send[,+Receive]).
% 
% ?- invokeSubLRaw('(find-constant "Dog")').
% #$Dog
%
% ===================================================================

invokeSubL(Send):-
      invokeSubLRaw(Send,Receive),
      isDebug(format('~s',[Receive])).

invokeSubL(Send,Receive):-
      invokeSubLRaw(Send,ReceiveCodes),
      atom_codes(Receive,ReceiveCodes).

invokeSubLRaw(Send,Receive):-
      getCycConnection(SocketId,OutStream,InStream),
      printSubL(InStream,OutStream,Send),
      readSubL(InStream,Get),!,
      finishCycConnection(SocketId,OutStream,InStream),!,
      checkSubLError(Send,Get,Receive),!.

checkSubLError(Send,[53,48,48,_|Info],Info):-!, %Error "500 "
      atom_codes(ErrorMsg,Info),
      throw(cyc_error(ErrorMsg,Send)).
checkSubLError(_,[_,_,_,_|Info],Info):-!.
checkSubLError(Send,Info,Info).

% ===================================================================
% Lowlevel printng
% ===================================================================

printSubL(InStream,OutStream,Send):-
      popRead(InStream),
      printSubL(OutStream,Send).

printSubL(OutStream,Send):-     
      (var(Send) ->
	 throw(cyc_error('Unbound SubL message',Send));
         is_list(Send) ->
	    formatCyc(OutStream,'~s~n',[Send]);
	       atom(Send) -> formatCyc(OutStream,'~w~n',[Send]);
	       compound(Send) ->
      	       (toCycApiExpression(Send,[],STerm),formatCyc(OutStream,'~w~n',[STerm]));
%	       throw(cyc_error('SubL message type not supported',Send)),
	       	       formatCyc(OutStream,'~w~n',[Send])),!.


formatCyc(OutStream,Format,Args):-
      format(OutStream,Format,Args),
      isDebug(format(user_error,Format,Args)),
      flush_output(OutStream),!.

readSubL(InStream,[G,E,T,Space|Response]):-
      get_code(InStream,G),
      get_code(InStream,E),
      get_code(InStream,T),
      get_code(InStream,Space),
      readCycLTermChars(InStream,Response),!.

% ===================================================================
% Lowlevel readCycLTermChars
% ===================================================================
readCycLTermChars(InStream,Response):-
   notrace(readCycL(InStream,Response)).
   

readCycLTermChars(InStream,[Start|Response],Type):-
   peek_code(InStream,Start),
   readCycLTermCharsUntil(Start,InStream,Response,Type),
   isDebug(format('cyc>~s (~w)~n',[Response,Type])).

readCycLTermCharsUntil(34,InStream,Response,string):-!,
   get_code(InStream,_),
   readUntil(34,InStream,Response),
   popRead(InStream).

readCycLTermCharsUntil(35,InStream,[35|Response],term):-current_prolog_flag(arch,'i386-win32'),!,
   get_code(InStream,_),
   readUntil(13,InStream,Response),
   popRead(InStream).
readCycLTermCharsUntil(35,InStream,[35|Response],term):-
   get_code(InStream,_),
   readUntil(10,InStream,Response),
   popRead(InStream).

readCycLTermCharsUntil(84,InStream,"T",true):-!,
   popRead(InStream).

readCycLTermCharsUntil(78,InStream,"N",nil):-!,
   popRead(InStream).

readCycLTermCharsUntil(40,InStream,Trim,cons):-!,
   readCycL(InStream,Trim),
   popRead(InStream).

% needs better solution!  .01 seconds works but .001 seconds don't :(  meaning even .01 might in some circumstances be unreliable
popRead(InStream) :- once(wait_for_input([InStream], Inputs,0.01)),Inputs=[],!.
popRead(InStream) :-get_code(InStream, _),popRead(InStream).

readUntil(Char,InStream,Response):-
      get_code(InStream,C),
      readUntil(Char,C,InStream,Response).
      
readUntil(Char,Char,InStream,[]):-!.
readUntil(Char,C,InStream,[C|Out]):-get_code(InStream,Next),
   readUntil(Char,Next,InStream,Out).


      
% ===================================================================
%  conversion toCycApiExpression
% ===================================================================

toCycApiExpression(Prolog,CycLStr):-toCycApiExpression(Prolog,[],CycLStr).

toCycApiExpression(Prolog,Vars,Chars):-var(Prolog),!,toCycVar(Prolog,Vars,Chars).
toCycApiExpression('$VAR'(VAR),Vars,Chars):-!,sformat(Chars,'?~w',['$VAR'(VAR)]).
toCycApiExpression([],Vars,'()').
toCycApiExpression(Prolog,Vars,Prolog):- (atom(Prolog);number(Prolog)),!.
toCycApiExpression(Prolog,Vars,Chars):-is_string(Prolog),!,sformat(Chars,'"~s"',[Prolog]).
toCycApiExpression(nv(List),Vars,Chars):-toCycApiExpression_l(List,Vars,Chars),!.
toCycApiExpression(varslist(List),Vars,Chars):-!,toCycApiExpression_vars(List,Vars,Chars).
toCycApiExpression(varslist(List,Vars),_,Chars):-!,toCycApiExpression_vars(List,Vars,Chars).

toCycApiExpression(string(List),Vars,Chars):-
			toCycApiExpression_l(List,Vars,Term),
			sformat(Chars,'"~w"',[Term]).
toCycApiExpression(quote(List),Vars,Chars):-!,toCycApiExpression('QUOTE'(List),Vars,Chars).

toCycApiExpression('QUOTE'(List),Vars,Chars):-
			toCycApiExpression(List,Vars,Term),
			sformat(Chars,'\'~w',[Term]).

toCycApiExpression(List,Vars,Chars):-is_list(List),!,
			toCycApiExpression_l(List,Vars,Term),
			sformat(Chars,'\'(~w)',[Term]).

toCycApiExpression([nv|List],Vars,Chars):-toCycApiExpression_l(List,Vars,Chars),!.


toCycApiExpression(Prolog,Vars,Chars):-compound(Prolog),!,
		  Prolog=..[P|List],
		  toCycApiExpression(P,List,Vars,Chars).

toCycApiExpression((holds),List,Vars,Chars):-
	       toCycApiExpression_l(List,Vars,Term),
	       sformat(Chars,'(~w)',[Term]).
toCycApiExpression((';'),List,Vars,Chars):-
	       toCycApiExpression_l(List,Vars,Term),
	       sformat(Chars,'(#$or ~w)',[Term]).
toCycApiExpression((','),List,Vars,Chars):-
	       toCycApiExpression_l(List,Vars,Term),
	       sformat(Chars,'(#$and ~w)',[Term]).
toCycApiExpression((':-'),[A,B],Vars,Chars):-
	       toCycApiExpression(A,Vars,TA),
	       toCycApiExpression(B,Vars,TB),
	       sformat(Chars,'(#$enables-ThingProp ~w ~w)',[TB,TA]). % ? enables-Generic ?
toCycApiExpression(('=>'),[A,B],Vars,Chars):-
	       toCycApiExpression(A,Vars,TA),
	       toCycApiExpression(B,Vars,TB),
	       sformat(Chars,'(#$implies ~w ~w)',[TA,TB]). 
toCycApiExpression(('<=>'),[A,B],Vars,Chars):-
	       toCycApiExpression(A,Vars,TA),
	       toCycApiExpression(B,Vars,TB),
	       sformat(Chars,'(#$equiv ~w ~w)',[TA,TB]). 
toCycApiExpression(P,List,Vars,Chars):-
	       toCycApiExpression_l(List,Vars,Term),
	       sformat(Chars,'(~w ~w)',[P,Term]).


toCycApiExpression_vars(List,Vars,''):-var(List),!.
toCycApiExpression_vars([Var],Vars,Chars):-!,
		  toCycApiExpression_var(Var,Vars,Chars).
toCycApiExpression_vars([Var|Rest],Vars,Chars):-
		  toCycApiExpression_var(Var,Vars,C1),
	       toCycApiExpression_vars(Rest,Vars,C2),
	       sformat(Chars,'~w , ~w',[C1,C2]).

toCycApiExpression_var(Var,Vars,Chars):-
	    Var=..[_,Name,Value],
            %toCycVar(Name,Vars,C1),	 
	    toCycApiExpression(Value,Vars,C2),!,
	    sformat(Chars,'?~w = ~w',[Name,C2]).
toCycApiExpression_var(Value,Vars,Chars):-
	       toCycApiExpression(Value,Vars,Chars).

	       
toCycApiExpression_l([],Vars,''):-!.
toCycApiExpression_l([A],Vars,Chars):-toCycApiExpression(A,Vars,Chars),!.
toCycApiExpression_l([A|Rest],Vars,Chars):-
      toCycApiExpression(A,Vars,Chars1),
      toCycApiExpression_l(Rest,Vars,Chars2),
      sformat(Chars,'~w ~w',[Chars1,Chars2]),!.

toCycVar(Var,[VV|_],NameQ):-nonvar(VV),
   VV=..[_,Name,VarRef],
   Var==VarRef,!,sformat(NameQ,'?~w',[Name]).
toCycVar(Var,[_|Rest],Name):-
   nonvar(Rest),toCycVar(Var,Rest,Name).
toCycVar(VAR,_,VarName):-
      term_to_atom(VAR,AVAR),
      atom_codes(AVAR,[95|CODES]),!,
      catch(sformat(VarName,'?HYP-~s',[CODES]),_,VarName='?HYP-VAR').

is_string(X):-string(X),!.
is_string([A,B|_]):-integer(A),A>12,A<129,integer(B),B>12,B<129.


% ===================================================================
%  Debugging Cyc 
% ===================================================================
     
:-dynamic(isDebug/0).

% Uncomment this next line to see Cyc debug messages

isDebug.

isDebug(Call):- isDebug -> call(Call) ; true.


% ===================================================================
%  Cyc Query Cache Control
% ===================================================================


:-dynamic(cachable_query/1).
:-dynamic(cached_query/2).

cachable_query(isa(_,_)).

% ===================================================================
%  Cyc Assert
% ===================================================================

cycAssert(Mt:CycL):-!,
   cycAssert(CycL,Mt).
cycAssert(CycL):-
   mtAssertPred(CycL,Mt),
   cycAssert(CycL,Mt).

cycAssert(CycL,Mt):-
      system:retractall(opencyc:cached_query(_,_)),
      cyclifyNew(CycL,CycLified),
      cyclify(Mt,Mtified),
      defaultAssertOptions(DefaultOptions), 
      toCycApiExpression('CYC-ASSERT'(quote(CycLified),Mtified,(DefaultOptions)),API),
      invokeSubL(API),!.

defaultAssertOptions(Opts):-isMooOption(Agent,defaultAssertOptions,Opts).


% ===================================================================
%  Cyc Unassert/Retract
% ===================================================================
cycRetract(CycL,Mt):-cycRetractAll(CycL,Mt).
cycRetract(CycL):-cycRetractAll(CycL).

cycRetractAll(CycL):-
      mtAssertPred(CycL,Mt),
      cycUnassert(CycL,Mt).

cycRetractAll(CycL,Mt):-cycUnassert(CycL,Mt).
cycUnassert(CycL,Mt):-
      system:retractall(opencyc:cached_query(_,_)),
      cyclifyNew(CycL,CycLified),
      cyclify(Mt,Mtified),
      invokeSubL('CYC-UNASSERT'(quote(CycLified),Mtified)).


% ===================================================================
%  Cyc Query
% ===================================================================
cycQuery(CycL):-cycQuery(CycL,'#$EverythingPSC').
cycQuery(CycL,Mt):-
	 queryParams(Backchain,Number,Time,Depth),!,
	 cycQuery(CycL,Mt,Backchain,Number,Time,Depth).

cycQuery(CycL,Mt,Backchain,Number,Time,Depth):-
      copy_term(CycL,Copy),
      numbervars(Copy,'$VAR',0,_),!,
      cycQuery(Copy,CycL,Mt,Result,Backchain,Number,Time,Depth).

/*
cycQuery(Copy,CycL,Mt,Result,Backchain,Number,Time,Depth):-
      cached_query(Copy,Results),!,
      member(CycL,Results).

cycQuery(Copy,CycL,Mt,Result,Backchain,Number,Time,Depth):-cachable_query(Copy),!,
      findall(CycL,cycQueryReal(CycL,Mt,Result,Backchain,Number,Time,Depth),Save),
      (Save=[] -> true ; asserta(cached_query(CycL,Save))),!,
      member(CycL,Save).
*/
      cycQuery(Copy,CycL,Mt,Result,Backchain,Number,Time,Depth):-
      /*notrace*/(cycQueryReal(CycL,Mt,Result,Backchain,Number,Time,Depth)).

/*
	  (clet ((*cache-inference-results* t)
	    (*allow-forward-skolemization*t)  
	    (*compute-inference-results* nil)  
	    (*unique-inference-result-bindings* t) 
	    (*generate-readable-fi-results* t))
	    (without-wff-semantics
	       (ask-template '(?SEL1 ?SEL2)  '?Formula #$BaseKB 0 nil nil nll )) )
	       
*/
%queryParams(Backchain,Number,Time,Depth).
%queryParams(0,	nil,	nil,	nil). % default
%queryParams(1,	nil,	nil,	nil). % used here

queryParams(Backchain,Number,Time,Depth):-
   isMooOption(Agent,query(backchains),Backchain),
   isMooOption(Agent,query(number),Number),
   isMooOption(Agent,query(time),Time),
   isMooOption(Agent,query(depth),Depth),!.

cycQueryReal(CycL,Mt,Result,Backchain,Number,Time,Depth):-
	 getCycConnection(SocketId,OutStream,InStream),
      popRead(InStream),
      cyclify(CycL,CycLified),
      cyclify(Mt,Mtified),
      free_variables(CycLified,Vars),
      %  backchain number time depth
      printSubL(OutStream,
	       clet('((*cache-inference-results* t)(*compute-inference-results* nil)(*unique-inference-result-bindings* t)(*generate-readable-fi-results* t))',
		     'without-wff-semantics'('ask-template'(Vars,quote(CycLified),Mtified,Backchain,Number,Time,Depth)))),
  getToFirstTerm(InStream),
  get_code(InStream,FirstParen),!,% Takes the first paren
      call_cleanup((cycQueryResults(SocketId,InStream,OutStream,CycL,Mt,CycLified,Mtified,Vars,Result),syncCycLVars(Result,Vars)),
	    Exit,queryCleanup(Exit,SocketId,OutStream,InStream)).


getToFirstTerm(InStream):-getToFirstTerm(InStream,_).
getToFirstTerm(InStream,[G,E,T,Space]):-
	 get_code(InStream,G),
	 get_code(InStream,E),
	 get_code(InStream,T),
	 get_code(InStream,Space),!.


fiAsk(CycL,Mt):-
   free_variables(CycL,Vars),
   fiAsk2(CycL,Mt,Result),
   syncCycLVars(Result,Vars).

doResult([X, '.', Y]):-!,balanceBinding(Y,X).
doResult([H|T]):-doResultS([H|T]),!.
doResult(H).

doResultS([]).
doResultS([H|T]):
   doResult(H).
   doResultS(T).


fiAsk2(CycL,Mt,Result):-
      getCycConnection(SocketId,OutStream,InStream),
      popRead(InStream),
      cyclify(CycL,CycLified),
      cyclify(Mt,Mtified),
      free_variables(CycLified,Vars),
      %  backchain number time depth
      printSubL(OutStream,
	       ('FI-ASK'(quote(CycLified),Mtified))),
      getToFirstTerm(InStream),
      get_code(InStream,FirstParen),!,% Takes the first paren
      call_cleanup((cycQueryResults(SocketId,InStream,OutStream,CycL,Mt,CycLified,Mtified,Vars,Result)),
	    Exit,queryCleanup(Exit,SocketId,OutStream,InStream)).


queryCleanup(Exit,SocketId,OutStream,InStream):-
      finishCycConnection(SocketId,OutStream,InStream),!.

cycQueryResults(SocketId,InStream,OutStream,CycL,Mt,CycLified,Mtified,Vars,Result):-
      repeat,
      peek_code(InStream,PCode), % isDebug(format('PCODE (~q)~n',[PCode])),
      cycQueryEachResult(PCode,InStream,Vars,Result,Cut),
      ((Cut==cut,!);(Cut==fail,!,fail);true).

cycQueryEachResult(35,InStream,Vars,Result,fail). % No solutions at all
cycQueryEachResult(73,InStream,Vars,Result,fail). % Depth limit exceeded
cycQueryEachResult(41,InStream,Vars,Result,fail).  % The previous solution was the last
cycQueryEachResult(78,InStream,Vars,Result,cut). % True
cycQueryEachResult(__,InStream,Vars,Result,more):-
      readCycL(InStream,Trim), %isDebug(format('"~s"~n',[Trim])),
      getSurfaceFromChars(Trim,Result,_),!.%isDebug(format('~q.~n',[Result])),
%      syncCycLVars(Result,Vars),!.

syncCycLVars(_,[]).
syncCycLVars([[_,(.),Binding]|T],[PBinding|VV]):-!,
      balanceBinding(Binding,PBinding),syncCycLVars(T,VV),!.
syncCycLVars([Binding|T],[PBinding|VV]):-
      balanceBinding(Binding,PBinding),syncCycLVars(T,VV),!.

%list_to_term(X,Y):- balanceBinding(X,Y).
list_to_term(X,Y):-nonvar(X),var(Y),!,list_to_terms_lr(X,Y).
list_to_term(X,Y):-list_to_terms_rl(X,Y).
list_to_terms_rl(List,(A,B)):-list_to_terms_rl(A,AL),list_to_terms_rl(B,BL),append(AL,BL,List).
list_to_terms_rl(List,(A;B)):-list_to_terms_rl(A,AL),list_to_terms_rl(B,BL),append(AL,[or|BL],List).
list_to_terms_lr([],true):-!.
list_to_terms_lr([T],T):-!.
list_to_terms_lr([H|T],(H,TT)):-!,list_to_terms_lr(T,TT).
   



balanceBinding(Binding,Binding):- (var(Binding);atom(Binding);number(Binding)),!.
balanceBinding('$jCycAssertion'(T),Out):-balanceBinding(T,Out).
balanceBinding('$jCycList'([H|T]),Out):-atom(H),atom_concat('#$',_,H),!,
	   balanceBinding(H,P),balanceBindingS(T,O),Out=..[P|O].
balanceBinding('$jCycList'(T),Out):-balanceBindingS(T,Out).
balanceBinding('$jCycVariable'(String),'$jCycVariable'(String)):-!.

balanceBinding('$jCycVariable'(N,Num),Out):-!,balanceBindingV(N,Num,Out).
balanceBinding('$jCycNart'(Trim),O):-!,
	 getSurfaceFromChars(Trim,Result,ToplevelVars),
	 balanceBindingN(Result,ToplevelVars,O).


balanceBinding('$jString'(Codes),Words):-!,balanceBinding('$stringCodes'(Codes),Words).
balanceBinding('$stringCodes'(Codes),'$stringSplit'(List)):-atom_codes(Atom,Codes),concat_atom(List,' ',Atom),!.
balanceBinding('$nart'([List]),'$nartFixed'(ListO)):-!,balanceBinding(List,ListO).
balanceBinding('$nart'(List),'$nartFixed'(ListO)):-!,balanceBinding(List,ListO).
balanceBinding('$nartFixed'(A),'$nartFixed'(A)).
balanceBinding(['\'',[B]],BO):-!,balanceBindingS(B,BO).
balanceBinding(['QUOTE',B],BO):-!,balanceBindingS(B,BO).
balanceBinding([A|L],Binding):-
      (atom(A) -> balanceBindingCons(A,L,Binding) ; balanceBindingS([A|L],Binding)),!.
balanceBinding(Binding,Binding):-!.
 
balanceBindingV('?null',Atom,'$VAR'(Num)):-!,atom_codes(Atom,Codes),number_codes(Num,Codes),!.
balanceBindingV(null,Num,O):-!,balanceBindingV('?null',Num,O).
balanceBindingV(Name,_,'$NamedVarFn'(Name)).

balanceBindingN(Result,[],'$nartFixed'(PrologGoal)):-balanceBinding(Result,PrologGoal).
balanceBindingN(Result,ToplevelVars,'$pCycNartVars'(PrologGoal,ToplevelVars)):-balanceBinding(Result,PrologGoal).

balanceBindingCons(A,L,[A|L]):- (var(A);A=string(_);number(A)),!.
balanceBindingCons('and-also',L,Binding):-balanceBindingS(L,LO), list_to_term(LO,Binding),!.
balanceBindingCons('eval',L,Binding):-balanceBindingS(L,LO), list_to_term(LO,Binding),!.
balanceBindingCons('#$and-also',L,Binding):-balanceBindingS(L,LO), list_to_term(LO,Binding),!.

balanceBindingCons(A,L,Binding):-
	 balanceBinding(A,AO),
	 balanceBindingS(L,LO),!,
	 balanceBindingConses(A,L,AO,LO,Binding).

balanceBindingConses(A,L,[AO|AL],LO,[A|L]):-!.
balanceBindingConses(A,L,AO,LO,Binding):-
	 Binding=..[AO|LO],!.

balanceBindingS(Binding,Binding):- (var(Binding);atom(Binding);number(Binding)),!.
balanceBindingS([],[]).
balanceBindingS([V,[L]|M],[LL|ML]):-V=='\'',balanceBindingS(L,LL),balanceBindingS(M,ML).
balanceBindingS([A|L],[AA|LL]):-balanceBinding(A,AA),balanceBindingS(L,LL).
   
% ===================================================================
%  Cyclification
%
%    cyclify(Statement,Cyclified)
%     Makes sure that atoms in Statement are prefixed witbh '#$' when comunicationg with Cyc
%
%    cyclifyNew(Statement,Cyclified)
%     same as cyclify/2 but adds the constant names with (CREATE-CONSTANT "~w")
%
% ===================================================================

noncyclified(Same):- (var(Same);number(Same);string(Same)),!.
noncyclified('$VAR'(_)).
noncyclified(string(_)).
noncyclified([]).

cyclify(Same,Same):- noncyclified(Same),!.
cyclify([H|T],Term):-integer(H) -> Term=[H|T]; cyclify_l([H|T],Term).
cyclify(Before,After):-atom(Before),
      sub_atom(Before,0,1,_,F),!,
      cyclify(F,Before,After).
cyclify(Before,After):- Before=..[B|BL], cyclify(B,A), cyclify_l(BL,AL), After=..[A|AL].

cyclify(Char,Before,After):-cyclifyAtom(Char,Before,After).
cyclify(_,Before,After):-atom_concat('#$',Before,After).
      
cyclify_l([B],[A]):-cyclify(B,A),!.
cyclify_l([],[]).
cyclify_l([B|BL],[A|AL]):-cyclify(B,A),cyclify_l(BL,AL).


cyclifyNew(Same,Same):- noncyclified(Same),!.
cyclifyNew([H|T],Term):-integer(H) -> Term=[H|T]; cyclifyNew_l([H|T],Term).
cyclifyNew(Before,After):-atom(Before),
      sub_atom(Before,0,1,_,F),!,
      cyclifyNew(F,Before,After).
cyclifyNew(Before,After):- Before=..[B|BL], cyclifyNew(B,A), cyclifyNew_l(BL,AL), After=..[A|AL].

cyclifyNew(Char,Before,After):-cyclifyAtom(Char,Before,After).
cyclifyNew(_,Before,After):-atom_concat('#$',Before,After),makeConstant(Before).
      
cyclifyNew_l([B],[A]):-cyclifyNew(B,A),!.
cyclifyNew_l([],[]).
cyclifyNew_l([B|BL],[A|AL]):-cyclifyNew(B,A),cyclifyNew_l(BL,AL).

cyclifyAtom('#',Before,Before).
cyclifyAtom('?',Before,Before).
cyclifyAtom(':',Before,Before).
cyclifyAtom('(',Before,Before).
cyclifyAtom('!',Before,After):-atom_concat('!',After,Before).
cyclifyAtom('"',Before,Before).

% ============================================
% Make new CycConstant
% ============================================

:-dynamic(cycConstantMade/1).

makeConstant(Const):-
   atom_concat('#$',New,Const),!,
   makeConstant(New).

makeConstant(Const):-
   (cycConstantMade(Const)->true;
   (sformat(String,'(CREATE-CONSTANT "~w")',[Const]),
   catch(invokeSubL(String),_,true),
   asserta(cycConstantMade(Const)))),!.

% ============================================
% Make new Microtheory
% ============================================

ensureMt(Const):-
   cycAssert('#$BaseKB':'#$isa'(Const,'#$Microtheory')).

% ============================================
% dynamic Default Microtheory
% ============================================

:-dynamic(defaultAssertMt/1).
:-dynamic(everythingMt/1).

defaultAssertMt('PrologDataMt').
everythingMt('#$EverythingPSC').

%:-defaultAssertMt(Mt),!,ensureMt(Mt),cycAssert('#$BaseKB':'#$genlMt'(Mt,'#$InferencePSC')). % Puts the defaultAssertMt/1 into Cyc 
/*
:-defaultAssertMt(Mt),!,
   ensureMt(Mt),
   everythingMt(EverythingPSC),
   cycAssert('#$BaseKB':'#$genlMt'(Mt,EverythingPSC)). % Puts the defaultAssertMt/1 into Cyc 
*/
% ===================================================================
%  Predicates need and Assertion Mt
% ===================================================================

mtAssertPred(CycL,Mt):-nonvar(CycL),
   functor(CycL,Pred,_),
   isRegisterCycPred(Mt,Pred,_),!.

mtAssertPred(CycL,Mt):-defaultAssertMt(Mt).

% ============================================
% Prolog to Cyc Predicate Mapping
%
%  the following will all do the same things:
%
% ?- registerCycPred('BaseKB':isa/2). 
% ?- registerCycPred('BaseKB':isa(_,_)). 
% ?- registerCycPred(isa(_,_),'BaseKB'). 
% ?- registerCycPred('BaseKB',isa,2). 
%
%  Will make calls 
% ?- isa(X,Y)
%  Query into #$BaseKB for (#$isa ?X ?Y) 
%
% ============================================
:-dynamic(isRegisterCycPred/3).

% ?- registerCycPred('BaseKB':isa/2). 
registerCycPred(Mt:Pred/Arity):-!,
   registerCycPred(Mt,Pred,Arity).
% ?- registerCycPred('BaseKB':isa(_,_)). 
registerCycPred(Mt:Term):-
   functor(Term,Pred,Arity),
   registerCycPred(Mt,Pred,Arity).
registerCycPred(Term):-
   functor(Term,Pred,Arity),
   registerCycPred(Mt,Pred,Arity).
   


% ?- registerCycPred(isa(_,_),'BaseKB'). 
registerCycPred(Term,Mt):-
   functor(Term,Pred,Arity),
   registerCycPred(Mt,Pred,Arity).
   
% ?- registerCycPred('BaseKB',isa,2). 
registerCycPred(Mt,Pred,0):-!,registerCycPred(Mt,Pred,2).
registerCycPred(Mt,Pred,Arity):-isRegisterCycPred(Mt,Pred,Arity),!.
registerCycPred(Mt,Pred,Arity):-
      functor(Term,Pred,Arity),
      ignore(defaultAssertMt(Mt)),
      asserta(( user:Term :- cycQuery(Term,Mt))),
      %asserta(( Mt:Term :- cycQuery(Term,Mt))),
      assertz(isRegisterCycPred(Mt,Pred,Arity)),!.


% ============================================
% Assert Side Effect Prolog to Cyc Predicate Mapping
%
% ============================================

%user:exception(undefined_predicate, Pred ,retry):-
%      isMooOption(Agent,hookCycPredicates,true),cycDefineOrFail(Pred).

cycDefineOrFail(Mod:Pred/Arity):-atom_concat('#$',_,Pred),
      cycDefineOrFail(Mod,Pred,Arity).
cycDefineOrFail(Pred/Arity):-atom_concat('#$',_,Pred),registerCycPred(Mod,Pred,Arity).

cycDefineOrFail(Mod,Pred,Arity):-
      atom_concat('#$',_,Mod),
      registerCycPred(Mod,Pred,Arity).
cycDefineOrFail(_,Pred,Arity):-
      registerCycPred(_,Pred,Arity).

% ============================================
% Assert Side Effect Prolog to Cyc Predicate Mapping
%
% ?- assert(isa('Fido','Dog')).
% Will assert (#$isa #$Fido #$Dog) into #$BaseKB
%
% ?- assert('DogsMt':isa('Fido','Dog')).
% Will assert (#$isa #$Fido #$Dog) into #$DogsMt
% ============================================
% :-redefine_system_predicate(system:assert(_)).
% system:assert(Term):-nonvar(Term),assertThrough(Term).

assertThrough(Mt:CycL):-
      assertThrough(Mt,CycL).

assertThrough(CycL):-
      assertThrough(Mt,CycL).

assertThrough(ToMt,CycL):-
      functor(CycL,Pred,Arity),
      (isRegisterCycPred(Mt,Pred,Arity);atom_concat('#$',_,Pred)),!,
      ignore(ToMt=Mt),
      cycAssert(CycL,ToMt),!.

assertThrough(ToMt,CycL):-
      ignore(ToMt=user),
      assertz(ToMt:CycL),!.

% ============================================
% Retract (All) Side Effect Prolog to Cyc Predicate Mapping
%
% ?- retractall(isa('Fido','Dog')).
% Will retract (#$isa #$Fido #$Dog) from #$BaseKB
%
% ?- retractall('DogsMt':isa('Fido','Dog')).
% Will retract (#$isa #$Fido #$Dog) from #$DogsMt
% ============================================
% :-redefine_system_predicate(retractall(_)).
% retractall(Term):-retractAllThrough(Term).

retractAllThrough(Mt:CycL):-
      retractAllThrough(Mt,CycL).

retractAllThrough(CycL):-
      retractAllThrough(Mt,CycL).

retractAllThrough(ToMt,CycL):-
      functor(CycL,Pred,Arity),
      isRegisterCycPred(Mt,Pred,Arity),!,
      ignore(ToMt=Mt),
      cycRetract(CycL,ToMt),!.

retractAllThrough(ToMt,CycL):-
      ignore(ToMt=user),
      system:retractall(ToMt:CycL),!.
            
% ============================================
% Register isa/genls (more for testing :)
% ============================================

% examples
%:-registerCycPred('#$BaseKB',isa,2).
%:-registerCycPred('#$BaseKB',genls,2).
%:-registerCycPred('#$BaseKB',genlMt,2).


% ============================================
% Testing 
% ============================================
      
testOpenCyc:-halt.

% ===================================================================

%isSlot(Var):-var(Var).
%isSlot('$VAR'(Var)):-number(Var).


% ===================================================================
% CycL Term Reader
% ===================================================================
:-dynamic(reading_in_comment/0).
:-dynamic(reading_in_string/0).
:-dynamic(read_in_atom/0).


% write("#<:>").
% [35, 60, 58, 62]

readCycL(CHARS):-readCycL(user_input,CHARS).

%readCycL(Stream,[])  :-at_end_of_stream(Stream).     
readCycL(Stream,Trim)  :-
		flag('$bracket_depth',_,0),
		flag('$pipe_depth',_,0),
		flag('$first_nonwhitespace',_,0),
		retractall(reading_in_comment),
		retractall(reading_in_string),!,
		catch(readCycLChars_start(Stream,CHARS),E,(CHARS=[] /*,trace*/)),!,trim(CHARS,Trim).

readCycLChars_start(Stream,CHARS):-
      peek_code(Stream,C),
	    (char_type(C,white) ->
	       (get_code(Stream,C),readCycLChars_start(Stream,CHARS));
	       (flag('$first_nonwhitespace',_,C),readCycLChars_p0(Stream,CHARS))),!.

%readCycLChars_p0(Stream,[]):-at_end_of_stream(Stream),!.
readCycLChars_p0(Stream,[Char|Chars]):-
        get_code(Stream,C),
	cyclReadStateChange(C),
	readCycLChars_p1(C,Char,Stream,Chars),!.
	
readCycLChars_p1(C,Char,Stream,[]):-isCycLTerminationStateChar(Stream,C,Char),!.
readCycLChars_p1(C,Char,Stream,Chars):-
      cyclAsciiRemap(C,Char),
      flag('$prev_char',_,Char),
      readCycLChars_p0(Stream,Chars),!.

isCycLTerminationStateChar(Stream,13,32):-reading_in_comment,!.
isCycLTerminationStateChar(Stream,10,32):-reading_in_comment,!.
isCycLTerminationStateChar(Stream,C,CC):-not(reading_in_string),
	 isCycLTerminationStateCharNoString(Stream,C,CC).


isCycLTerminationStateCharNoString(Stream,10,32):-not(flag('$first_nonwhitespace',40,40)).
isCycLTerminationStateCharNoString(Stream,13,32):-not(flag('$first_nonwhitespace',40,40)).
isCycLTerminationStateCharNoString(Stream,41,32):-not(flag('$first_nonwhitespace',40,40)).
isCycLTerminationStateCharNoString(Stream,C,32):-char_type(C,white),not(flag('$first_nonwhitespace',40,40)).
isCycLTerminationStateCharNoString(Stream,13,32):-not(flag('$first_nonwhitespace',40,40)).
isCycLTerminationStateCharNoString(Stream,41,41):-no_depth_agenda.
isCycLTerminationStateCharNoString(Stream,62,62):-no_depth_agenda.

no_depth_agenda:-flag('$bracket_depth',X,X),(X<1),!,flag('$pipe_depth',Y,Y),(Y<1),!.

cyclReadStateChange(_):- reading_in_comment,!.
cyclReadStateChange(34):-flag('$prev_char',Char,Char),   % char 92 is "\" and will escape a quote mark
      (Char=92 -> true;(retract(reading_in_string) ; assert(reading_in_string))),!.
cyclReadStateChange(_):- reading_in_string,!.
cyclReadStateChange(59):- assert(reading_in_comment),!.
cyclReadStateChange(40):-!,flag('$bracket_depth',N,N + 1).
cyclReadStateChange(41):-!,flag('$bracket_depth',N,N - 1).
cyclReadStateChange(60):-flag('$prev_char',35,35),!,flag('$pipe_depth',N,N + 1).
cyclReadStateChange(62):-!,flag('$pipe_depth',N,N - 1).
cyclReadStateChange(_).

cyclAsciiRemap(X,32):- (not(number(X));X>128;X<32),!.
cyclAsciiRemap(X,X):-!.


% ===================================================================
% CycL Term Parser
% ===================================================================
/*===================================================================
% getSurfaceFromChars/3 does less consistancy checking then conv_to_sterm

Always a S-Expression: 'WFFOut' placing variables in 'VARSOut'

|?-getSurfaceFromChars("(isa a b)",Clause,Vars).
Clause = [isa,a,b]
Vars = _h70

| ?- getSurfaceFromChars("(isa a (b))",Clause,Vars).
Clause = [isa,a,[b]]
Vars = _h70

|?-getSurfaceFromChars("(list a b )",Clause,Vars)
Clause = [list,a,b]
Vars = _h70

| ?- getSurfaceFromChars("(genlMt A ?B)",Clause,Vars).
Clause = [genlMt,'A',_h998]
Vars = [=('B',_h998)|_h1101]

| ?- getSurfaceFromChars("(goals Iran  (not   (exists   (?CITIZEN)   (and    (citizens Iran ?CITIZEN)    (relationExistsInstance maleficiary ViolentAction ?CITIZEN
)))))",Clause,Vars).

Clause = [goals,Iran,[not,[exists,[_h2866],[and,[citizens,Iran,_h2866],[relationExistsInstance,maleficiary,ViolentAction,_h2866]]]]]
Vars = [=(CITIZEN,_h2866)|_h3347]

====================================================================*/
getSurfaceFromChars(Chars,TERM,VARS):-trim(Chars,CharsClean),
      getSurfaceFromCleanChars(Chars,TERM,VARS).
getSurfaceFromChars(C,TERM,VARS):-atom(C),atom_codes(C,Chars),!,
      getSurfaceFromChars(Chars,TERM,VARS).
getSurfaceFromChars(C,TERM,VARS):-
      string_to_list(C,List),!,
      getSurfaceFromChars(List,TERM,VARS),!.

getSurfaceFromCleanChars([],[end_of_file],_):-!.
getSurfaceFromCleanChars([41|_],[end_of_file],_):-!.
getSurfaceFromCleanChars([59|Comment],[file_comment,Atom],VARS):-atom_codes(Atom,Comment),!.

getSurfaceFromCleanChars(Chars,WFFOut,VARSOut):- 
	       once((retractall(numbered_var(_,_)), 
               getCycLTokens(Chars,WFFClean))),
	       getSurfaceFromCleanTokens(WFFClean,WFFOut,VARSOut).

getSurfaceFromCleanTokens(WFFClean,WFFOut,VARSOut):-phrase(expr(WFF),WFFClean),!,
               collect_temp_vars(VARS),!,
               ((VARS=[],VARSOut=_,WFFOut=WFF);
                    (unNVar(VARS,LIST),
                     cyclVarNums(LIST,WFF,WFFOut,VARSOut2) ,
                     list_to_set(VARSOut2,VARSOut1),
                     open_list(VARSOut1,VARSOut))),!.

getSurfaceFromCleanTokens(WFFClean,'$mulit'(WFFOut),VARSOut):-phrase(exprS(WFF),WFFClean),!,
               collect_temp_vars(VARS),!,
               ((VARS=[],VARSOut=_,WFFOut=WFF);
                    (unNVar(VARS,LIST),
                     cyclVarNums(LIST,WFF,WFFOut,VARSOut2) ,
                     list_to_set(VARSOut2,VARSOut1),
                     open_list(VARSOut1,VARSOut))),!.

getSurfaceFromCleanChars(Comment,[unk_comment,Atom],VARS):-atom_codes(Atom,Comment),!.

%===================================================================
% Removes Leading and Trailing whitespaces and non ANSI charsets.
%====================================================================
trim(X,Y):-ltrim(X,R),reverse(R,Rv),ltrim(Rv,RY),reverse(RY,Y),!.

ltrim([],[]):-!.
ltrim([32,32,32,32,32,32,32|String],Out) :-trim(String,Out),!.
ltrim([32,32,32,32,32|String],Out) :-trim(String,Out),!.
ltrim([32,32,32|String],Out) :- trim(String,Out),!.
ltrim([32,32|String],Out) :- trim(String,Out),!.
ltrim([P|X],Y):- (not(number(P));P<33;P>128),trim(X,Y),!.
ltrim(T,T).

% ===================================================================
%  CycL String to DCG Converter
% Converts up to 13 forms
%     13 Terms long
%  
% =169 Parens Pairs at the First 2 levels  
% 
% ===================================================================


%?- getSurfaceFromChars("'(ls dfg)",S,V).
%S = ['QUOTE', [[ls, dfg]]]
expr('$object'(Type,Sexp)) --> ['#<',Type,':'],exprS(Sexp),['>'].
%expr('$object'(Type,Sexp)) --> ['#<',Type],exprS(Sexp),['>'].
expr('$nart'(Sexp)) --> ['#<'],expr(Sexp),['>'].
%expr('$nart'(Sexp)) --> ['#<'],exprS(Sexp),['>'].
expr('$info'(Sexp)) --> [':<'],exprS(Sexp),['>'].
expr('$literal'(Sexp)) --> ['<'],exprS(Sexp),['>'].
expr('{}'(Sexp)) -->  ['{'],expr(Sexp),['}'].
expr('QUOTE'(Sexp)) --> ['\''],expr(Sexp).
expr(Sexp) -->  simple(Sexp).
expr(Sexp) -->  ['('],exprS(Sexp),[')'].
%expr('$m'([A|L])) --> expr(A),exprS(L),{nonvar(A)}.

exprS([A]) --> expr(A).
exprS([A|L]) --> expr(A),exprS(L).


simple('NIL') -->  ['(',')'].
simple(Sexp) -->  quantity(Sexp).
simple(Sexp) -->  variable(Sexp).
simple(Sexp) -->  constant(Sexp).

quantity(Number) --> number(Number).

number(Number) -->  [Number],{number(Number),! } .

variable(VN)-->  ['?',A], { var_number(A,VN)   } . 
variable(VN)-->  ['??'], { var_gen(A),var_number(A,VN)   } .     %Anonymous
%variable(VN)-->  ['?'], { var_gen(A),var_number(A,VN)   } . 

constant(Constant) -->  ['#$'],atomish(Unquoted),{atom_concat('#$',Unquoted,Constant) } .
constant(Constant) -->  [':'],atomish(Unquoted), {atom_concat(':',Unquoted,Constant) } .
constant(Constant) --> atomish(Constant).

atomish(Unquoted) -->  [Unquoted] , {not((Unquoted='?';Unquoted='(';Unquoted=')')),! } .

/*
variables_list('$variables'(A)) --> qual_var(A).
variables_list('$variables'([A])) -->  ['('],qual_var(A),[')'],!.
variables_list('$variables'([A,B])) -->  ['('],qual_var(A),qual_var(B),[')'],! .
variables_list('$variables'([A|QV])) -->  ['('],qual_var(A),many_qual_var(QV),[')'],!.
many_qual_var([A]) -->  qual_var(A).
many_qual_var([A|T]) -->  qual_var(A),many_qual_var(T).

% Var/Quality pairs that Sowa's ACE examples use
qual_var(VN) --> ['('],variable(VN),[')'].
qual_var(VN) --> variable(VN).
qual_var(VN) --> ['('],variable(VN),qual(_Quality),[')'].

qual(Q) --> constant(Q).



% Construct arbitrary list of args
arbitrary([]) -->  [].
arbitrary(VN)-->  ['?',A], { var_number(A,VN)   } . 
arbitrary([Head]) -->  expr(Head).
arbitrary([A|L]) --> expr(A) , exprS(L).
*/
     
var_number(A,'$VAR'(VN)):-numbered_var(A,'$VAR'(VN)),!.
var_number(A,'$VAR'(VN)):-flag(get_next_num,VN,VN+1),asserta(numbered_var(A,'$VAR'(VN))),!.

:-dynamic(numbered_var/2).


% Makes up sequencial Variable names for anonymous cycl getPrologVars
var_gen(Atom):-idGen(Number),number_codes(Number,Codes),atom_codes(Atom,[86,65,82|Codes]). % "VAR"

%======================================================================
% CLSID Generation
% idGen(+Var)
% Creates a new unique number   TODO
%
% Example:
% | ?- idGen(X).
% X = 2234
%======================================================================
idGen(X):-flag(idGen,X,X+1).

constant(Number) --> number(Number) .
   

% This creates ISO Prolog getPrologVars w/in a CycL/STANDARD expression to be reconstrated as after parsing is complete 

cyclVarNums([],WFF,WFF,_):-!.

cyclVarNums(LIST,'$VAR'(NUM),VAR,[=(SYM,VAR)]):-numbered_var(SYM,'$VAR'(NUM)),
               member(=(SYM,VAR),LIST).

cyclVarNums(_,Atom,Atom,[]):-atomic(Atom).
cyclVarNums(LIST,Term,NewTerm,VARLIST):-Term=..[F|ARGS],cyclVarNums_list(LIST,ARGS,VARARGS,VARLIST),NewTerm=..[F|VARARGS].

cyclVarNums_list(_LIST,[],[],[]).
cyclVarNums_list(LIST,[A|RGS],[V|ARARGS],VARLIST):-
            cyclVarNums(LIST,A,V,VARS1),
            cyclVarNums_list(LIST,RGS,ARARGS,VARS2),
            append(VARS1,VARS2,VARLIST).


unNVar(X,Y):-term_to_atom(X,A),atom_to_term(A,Y,_).

open_list(V,V):-var(V).
open_list(A,B):-append(A,_,B).

unnumbervars_nil(X,Y):-!,unNVar(X,Y).

collect_temp_vars(VARS):-!,(setof(=(Name,Number),numbered_var(Name,Number),VARS);VARS=[]).

%================================================================
% STRING TOKENIZATION                            
%================================================================

getCycLTokens([],[]):-!.
getCycLTokens([C|T],O):- C<33,!,getCycLTokens(T,O).
getCycLTokens([C|List],[Token|TList])  :- 
  get_token(C,List,Token,Rest),!,
  getCycLTokens(Rest,TList).

%  cyc-> "#$"
get_token(35,[36|List],Token,Rest)  :-not(List=[32|_]),
  get_chars_type(List,Lchars,Rest,Type),!,
  atom_codes(Token,[35, 36|Lchars]).

%  cyc-> "#<"
get_token(35,[60|List],'#<',List).

%  cyc-> ":("
get_token(58,[L|IST],':',[L|IST]):-not(char_type(L,alpha)),!.

%  cyc-> ":"
get_token(58,List,Token,Rest)  :-not(List=[32|_]),
  get_chars_type(List,Lchars,Rest,Type),!,
  atom_codes(Token,[58|Lchars]).

get_token(A,List,Token,Rest)  :- 
  get_chars_type([A|List],Lchars,Rest,Type),!,
  type_codes(Type,Lchars,Token),!.

type_codes(num,CODES,Num):-catch(number_codes(Num,CODES),_,fail),!.
type_codes(_,[34|Lchars],'$stringCodes'(LcharsNoQuotes)):-!,
      reverse(Lchars,[_|Rchars]),
      reverse(Rchars,LcharsNoQuotes).

type_codes(_,Lchars,Token):-!,atom_codes(Token,Lchars).

get_chars_type(L,S,L1,sep)  :-  separator(L,S,L1),!.
get_chars_type([C|L],[C|Lc],L1,S)  :- 
  check_start(S,C),
  get_word_chars(S,L,Lc,L1).

get_word_chars(S,L,Lc,L1)  :- 
  check_end(S,L,Lc,L1).
get_word_chars(S,[C|L],[C|Lc],L1)  :- 
  legal_char(S,C),
  get_word_chars(S,L,Lc,L1).

legal_char(num,C)    :-  digit(C).
legal_char(quote,C)  :-  not(bracket(_,C,_)).
legal_char(symb,C)   :-  valid_char(C).

check_start(Name,S):-bracket(Name,S,_E).
check_start(num, C)   :- start_digit(C).
check_start(symb,C)   :- valid_char(C). %, 'not'(digit(C)).

check_end(_,[],[],[])  :-  !.
check_end(num, [C|L],[],[C|L])  :-  'not'(digit(C)),!.
check_end(Name,[E|L],[E],L)  :-  bracket(Name,S,E),!.
%check_end(symb,[C1,C2|L],[],[C1,C2|L])  :-  member([C1,C2],["Fn"]),!.
check_end(symb,[C|L],[],[C|L])  :-  'not'(valid_char(C)).

separator([C,D,E|L],[C,D,E],L)  :-member([C,D,E],["<=>","=:=","=\\=","\\==","@=<","@>=","=..","-->"]),!.
separator([C,D|L],[C,D],L)  :-member([C,D],["=>",":-","\\+","->","\\=","==","@<","@>","=<",">=","#$","//","??"]),!. %,"Fn"
separator([C|L],[C],L)  :- member(C,"*,.():[];= < >^{}?%$'#/"),!.

valid_char(46):-!,fail.
valid_char(C)  :-  letter(C); digit(C); C = 95 ; C=45. % ; C=39.
letter(C)  :-   (C=45 ; (97 =< C, C =< 122) ; (65 =< C, C =< 90) ; C = 95;C=126),! .
start_digit(C)   :- member(C,"-01234567890").
digit(C)   :- member(C,"-_.01234567890+eE").

%get_word([C|T],C,T)  :-  member(C,":,.?&%"),!. % ( : , . ?)
get_word([C|T],[C],T)  :- member(C,"=&"),!. % (=)
get_word([C,C1|T],[C,C1],T)  :- member([C,C1],["??"]),!. %"Fn",
get_word([C|T],[C|W],T2)  :-  bracket(_,C,C1),!,get_chars(0,C1,T,W,T2).
get_word([C|T],[C|W],T2)  :-  valid_start(C),!, get_chars(1,32,T,W,T2).

get_chars(K,C1,[C|T],[C|W],T2)  :-  valid_char(K,C,C1),!,get_chars(K,C1,T,W,T2).
get_chars(0,C,[C|T],[],T)  :- bracket(_,C,_), !.
get_chars(0,C,[C|T],[C],T)  :-  (C = 41; C = 93),!. % ) or ]
get_chars(1,_C1,[C|T],[],[C|T])  :-  member(C, [10,13|"=:,'?"]).
%get_chars(2,_C1,[C,C2|T],[],[C,C2|T])  :-  member([C,C2], ["Fn"]).

valid_start(C)  :-  valid(C). %; C = 37.  % (%)
valid_start(35).
valid_char(K,C,C1)  :-  K = 0,!, C \= C1; K = 1, valid(C).

%bracket(quote,39,39).  % single quotes
bracket(quote,34,34).  % double quotes
%bracket(list,91,93).  % square brackets []
%bracket(quote,37,37).  % Literal Percent %%
%bracket(quote,35,35).  % Literal Percent ##

quote_found(0,B,B)  :-  member(B,[34]),!.
quote_found(Q,Q,0).

var_found(0,B,C)  :-  'not'(valid(B)),var_start(C).

var_start(C)  :-  ((65 =< C,C =< 90);C = 95;C=126),!. %;C = 39.

valid(C)  :-   ((65 =< C, C =< 90);    % A - Z
             (97 =< C, C =< 122);   % a - z
             (48 =< C, C =< 57);    % 0 - 9
             (C = 95; %C = 39;
	     C = 45; C=126)),!.  % underscore; hyphen



/*===================================================================
Convert S-Expression originating from user to a Prolog Clause representing the surface level

Recursively creates a Prolog term based on the S-Expression to be done after compiler
                                                 
Examples:

| ?- sterm_to_pterm([a,b],Pterm).
Pterm = a(b)

| ?- sterm_to_pterm([a,[b]],Pterm).    %Note:  This is a special Case
Pterm = a(b)

| ?- sterm_to_pterm([holds,X,Y,Z],Pterm).    %This allows Hilog terms to be Converted
Pterm = _h76(_h90,_h104)                    

| ?- sterm_to_pterm([X,Y,Z],Pterm).   %But still works in normal places
Pterm = _h76(_h90,_h104)                    

| ?- sterm_to_pterm(['AssignmentFn',X,[Y,Z]],Pterm).                                
Pterm = 'AssignmentFn'(_h84,[_h102,_h116])
====================================================================*/

sterm_to_pterm(VAR,VAR):-isSlot(VAR),!.
sterm_to_pterm([VAR],VAR):-isSlot(VAR),!.
sterm_to_pterm([X],Y):-!,nonvar(X),sterm_to_pterm(X,Y).

sterm_to_pterm([S|TERM],PTERM):-isSlot(S),
            sterm_to_pterm_list(TERM,PLIST),            
            PTERM=..[holds,S|PLIST].

sterm_to_pterm([S|TERM],PTERM):-number(S),!,
            sterm_to_pterm_list([S|TERM],PTERM).            
	    
sterm_to_pterm([S|TERM],PTERM):-nonvar(S),atomic(S),!,
            sterm_to_pterm_list(TERM,PLIST),            
            PTERM=..[S|PLIST].

sterm_to_pterm([S|TERM],PTERM):-!,  atomic(S),
            sterm_to_pterm_list(TERM,PLIST),            
            PTERM=..[holds,S|PLIST].

sterm_to_pterm(VAR,VAR):-!.

sterm_to_pterm_list(VAR,VAR):-isSlot(VAR),!.
sterm_to_pterm_list([],[]):-!.
sterm_to_pterm_list([S|STERM],[P|PTERM]):-!,
              sterm_to_pterm(S,P),
              sterm_to_pterm_list(STERM,PTERM).
sterm_to_pterm_list(VAR,[VAR]).


importCycFrom:-
      %importCycFrom('127.0.0.1',5601),!.
      %importCycFrom('127.0.0.1',4601),
      importCycFrom('127.0.0.1',3601),!.

importCycFrom(Host,Port):-
   once((findall(X,fiAsk(isa(X,'Microtheory'),'BaseKB'),L),list_to_set(L,Set))),
   member(Mt,Set),
   sformat(S,'~w~w.pcache',[Host,Port]),
   string_to_atom(S,File),
   tell(File),
   importCycFrom(Host,Port,Set),
   told.


										      
cyclishMt:-importCycFrom('127.0.0.1',3601,'#$CyclistsMt').


importCycFrom(Host,Port,[]):-!.
importCycFrom(Host,Port,[Mt|T]):-!,
	 importCycFrom(Host,Port,Mt),sleep(1),
	 importCycFrom(Host,Port,T).

importCycFrom(Host,Port,'#$EverythingPSC'):-!.
importCycFrom(Host,Port,'#$BaseKB'):-!.

importCycFrom(Host,Port,Mt):-
      getCycConnection(Host:Port,SocketId,OutStream,InStream),
      %getCycConnection(Host:Port,SocketId2,OutStream2,InStream2),
      popRead(InStream),
      printSubL(OutStream,'mapcar'('ASSERTION-EL-FORMULA','GATHER-MT-INDEX'(Mt))),
      getToFirstTerm(InStream,Term),!,
      (Term="500 " -> 
		  (readUntil(10,InStream,_),
			printSubL(OutStream,'mapcar'('\'ASSERTION-EL-FORMULA','GATHER-MT-INDEX'(Mt))),
			getToFirstTerm(InStream));true),!,
      get_code(InStream,Paren),
   %   importCycFrom(Mt,SocketId,OutStream,InStream,SocketId2,OutStream2,InStream2),
      ignore(importCycFrom(Mt,SocketId,OutStream,InStream)),
      finishCycConnection(SocketId,OutStream,InStream).
     % finishCycConnection(SocketId2,OutStream2,InStream2).

importCycFrom(Mt,SocketId,OutStream,InStream):-
	 repeat,
	 readCycL(InStream,TrimOfList),
	 recordAssertion(TrimOfList,Mt),
	 TrimOfList=[].

recordAssertion([],Mt):-!.
recordAssertion(TrimOfList,Mt):-
         getSurfaceFromChars(TrimOfList,FORMULA,Vars),
	 numbervars(Vars),
	 balanceBinding(FORMULA,Save),
	 transoutString(Save,SaveO),
	 SaveO=..[Pred|List],
	 getConstants(atom,List, TOKS, Singletons, Multiples),!,
	 CV=..[v|TOKS],
	 append([Pred|List],[Mt,v(CV,Vars)],NewL),
	 RS=..NewL,
	 format('~q.~n',[RS]),!.

%	 rememberFact(saved(Save,Mt,Vars,TOKS)).

convertJavaExport(In,Out):-
   see(In),
   tell(Out),
   repeat,
   read(Term),
   rememverJavaExport(Term),
   Term=end_of_file,
   seen,
   told.

go2:-convertJavaExport2('oc.data','oc.pdata').


convertJavaExport2(In,Out):-
   see(In),
   tell(Out),
   repeat,
   read(Term),
   rememverJavaExport2(Term),
   Term=end_of_file,
   seen,
   told.

rememverJavaExport(end_of_file).
rememverJavaExport('$jCycAssertion'(Form, Mt, Str,Dir,_)):-
	    balanceBinding(Form,Binding),!,
	    rememberFact(save(Mt,Binding)).

rememverJavaExport2(end_of_file).
rememverJavaExport2(save(Mt,Save)):- 
	       breakOutVars(Save,Term,Vars),
	    getConstants(atom,Term, TOKS, Singletons, Multiples),
	    numbervars(Vars),!,
	    rememberFact(saved(Term,Mt,Vars,TOKS)).

breakOutVars(Save,Term,Vars):-
	 getVarishs(Save,Varishes),
	 list_to_set(Varishes,NVarishes),
	 breakOutVars2(Save,NVarishes,Term,Vars).


breakOutVars2(Save,[],Save,Vars):-!.
breakOutVars2(Save,[Name|Varishes],Term,[Name=NewVar|Vars]):-
	 subst(Save,'$NamedVarFn'(Name),NewVar,Mid),
	 breakOutVars2(Mid,Varishes,Term,Vars).
      
   


getVarishs(Save,[]):-not(compound(Save)),!.
getVarishs('$NamedVarFn'(Name),[Name]):-!.
getVarishs([H|T],Vars):-getVarishsS([H|T],Vars).
getVarishs(Save,Vars):-Save=..[P|Args],!,getVarishsS(Args,Vars).

getVarishsS([],[]):-!.
getVarishsS([H|T],Outs):-
       getVarishs(H,Vs),
       getVarishsS(T,TVs),!,
       append(Vs,TVs,Outs).



go4:-kifFileToPCache('BaseKBExtras.kif','BaseKBExtras.pcahce').
go6:-kifFileToPCache('EnglishMt.kif','EnglishMt.pcahce').
go8:-kifFileToPCache('EnglishParaphraseMt.kif','EnglishParaphraseMt.pcahce').

kifFileToPCache(In,Out):-
   open(In,read,Handle),
   tell(Out),
   repeat,
   readCycL(Handle,Term),
   rememberkifFileTerm(Term),
   at_end_of_stream(Handle),
   seen,
   told.

rememberkifFileTerm(TrimOfList):-
    getSurfaceFromChars(TrimOfList,Assertion,Vars),
    rememberkifFileTerm(Assertion,Vars).

%
rememberkifFileTerm('$mulit'(['[', ']']),Vars):-!.
rememberkifFileTerm([end_of_file],Vars):-!.
rememberkifFileTerm([file_comment, (;)],Vars):-!.

rememberkifFileTerm([unk_comment, _],Vars):-!.
rememberkifFileTerm(A,Vars):-atomic(A),!.
rememberkifFileTerm(Assertion,Vars):-
      rememberFact(Assertion:Vars),!.

go5:-kifListFileToPCache('BaseKBExtras.pcahce','BaseKBExtras.data').
go7:-kifListFileToPCache('EnglishParaphraseMt.pcahce','EnglishParaphraseMt.data').

kifListFileToPCache(In,Out):-
   see(In),
   tell(Out),
   repeat,
   read(Term),
   rememberkifListFileToPCache(Term),
   end_of_file=Term,
   seen,
   told.

rememberkifListFileToPCache(end_of_file):-!.
rememberkifListFileToPCache(Term:Vars):-
	 kifToCycL(Term,CycL),!,
	 balanceBinding(CycL,Out),!,
	    getConstants(atom,Out, TOKS, Singletons, Multiples),!,
	 rememberFact(save(Out,'#$EnglishParaphraseMt',Vars,TOKS)).
      
kifToCycL(Term,Term):-(var(Term);number(Term)),!.
kifToCycL([],[]).
kifToCycL('=>','#$implies').
kifToCycL('<=>','#$equiv').
kifToCycL('=','#$equals').
kifToCycL('NIL','NIL').
kifToCycL([termOfUnit,X,Y],['#$termOfUnit','$nart'(XX),YY]):-!,kifToCycL(X,XX),kifToCycL(Y,YY).
kifToCycL([H|T],[HH|TT]):-!,kifToCycL(H,HH),kifToCycL(T,TT),!.
kifToCycL(Term,Term):-compound(Term),functor(Term,F,1),atom_concat('$',_,F),!.
kifToCycL(Atom,Cyc):-atom(Atom),!,fixAtom(Atom,Cyc).
kifToCycL(Pred,Cyc):-compound(Pred),
	 Pred=..List,kifToCycL(List,CycList),Cyc=..CycList.
kifToCycL(Pred,Pred).

fixAtom(Atom,Atom):-atom_concat('#$',_,Atom),!.
fixAtom(Atom,Cyc):-atom_concat('#$',Atom,Cyc),!.

mbkb:-preferMtProcess('#$BaseKB','#$UniversalVocabularyMt'),fail.

mbkb:-saved(P,'#$BaseKB',A,C),functor(P,F,Arity),functor(PP,F,Arity),
      saved(PP,Mt,A,C),not(Mt='#$BaseKB'),PP==P,
      format('~q.~n',[saved(P,Mt,A,C)]),retract(saved(P,'#$BaseKB',A,C)),fail.

removeUniversalDuplicates:-saved(P,'#$UniversalVocabularyMt',A,C),functor(P,F,Arity),functor(PP,F,Arity),
      saved(PP,Mt,A,C),not(Mt='#$UniversalVocabularyMt'),PP==P,
      format('~q.~n',[saved(P,Mt,A,C)]),retract(saved(P,'#$UniversalVocabularyMt',A,C)),fail.

savedToSaved:-
   tell('best.sorted'),
   say('
	    :-dynamic(saved/4).
	    :-dynamic(saved(1,1,0,0)).
'),
   saved(A,B,C,D),
   oo(saved(A,B,C,D),O),
   format('~q.~n',[O]),
   fail.

savedToSaved:-told.

oo(A,A):-not(compound(A)),!.
oo([],[]):-!.
oo(saved('#$ist'(Mt,A),B,C,D),O):-!,oo(saved(A,Mt,C,D),O),!.
oo(saved(A,Mt,C,D),saved(AA,Mt,C,[Mt|TT])):-!,oo(A,AA),!,getConstants(atom,AA,TT,_,_),!.
oo([H|T],[HH|TT]):-!,oo(H,HH),!,oo(T,TT),!.
oo('$stringSplit'(List),'$stringSplit'(ListO)):-garbage_collect_atoms,!,strFix(List,ListO),!.
oo(P,O):-P=..L,!,oo(L,LL),O=..LL,!.
oo(O,O):-!.

strFix([],[]):-!.
strFix([H|HH],O):-!,strFix(H,ListO),strFix(HH,ListO2),!,flatten([ListO,ListO2],O),!.
strFix(O,ListO):-atom_codes(O,C),!,getCycLTokens(C,ListO),!.

	 %saved('#$arity', '#$intervalEndedBy', 2, '#$UniversalVocabularyMt', A, ['#$arity', '#$intervalEndedBy'], '#$arity'('#$intervalEndedBy', 2)).

preferMtProcess(Mt,PreferedMt):-
	 saved(P,Mt,A,C),saved(P,PreferedMt,A,C),
	 retract(saved(P,Mt,A,C)),
	 format('~q.~n',[deleted(P,A,Mt,C)]),fail.

preferMtProcess(Mt,PreferedMt).
	 
/*

      

importCycFrom(Mt,SocketId,OutStream,InStream,SocketId2,OutStream2,InStream2):-
      repeat,
	 readCycL(InStream,TrimOfList),
	 importCycFrom(Mt,SocketId,OutStream,InStream,SocketId2,OutStream2,InStream2,TrimOfList),
	 TrimOfList=[].

importCycFrom(Mt,SocketId,OutStream,InStream,SocketId2,OutStream2,InStream2,[]):-!.

importCycFrom(Mt,SocketId,OutStream,InStream,SocketId2,OutStream2,InStream2,TrimOfList):-
	 getSurfaceFromChars(TrimOfList,AssertionID,_),
	 gatherSaveID(AssertionID,Mt,InStream2,OutStream2),!.

gatherSaveID(Assertion,Mt,InStream,OutStream):-
	 printSubL(OutStream,'ASSERTION-FORMULA'('FIND-ASSERTION-BY-ID'(Assertion))),
	 getCycLTermAPI(InStream,FORMULA,Vars),
        printSubL(OutStream,'ASSERTION-DIRECTION'('FIND-ASSERTION-BY-ID'(Assertion))),
	 getCycLTermAPI(InStream,DIRECTION,_),
     %   printSubL(OutStream,'ASSERTION-STRENGTH'('FIND-ASSERTION-BY-ID'(Assertion))),
      %  getCycLTermAPI(InStream,STRENGTH,_),
	 saveAssertion(Mt,Assertion,FORMULA,Vars,DIRECTION,STRENGTH).
*/


getCycLTermAPI(InStream,Term,Vars):-
      getToFirstTerm(InStream),
      readCycLTermChars(InStream,TrimOfList),
     % format('"~s"~n',[TrimOfList]),
      readUntil(10,InStream,_),
      getSurfaceFromChars(TrimOfList,Term,Vars).

saveAssertion(Mt,Assertion,FORMULA,Vars,DIRECTION,STRENGTH):-
	    numbervars(Vars),
      balanceBinding(FORMULA,Save),
      getConstants(atom,Save, TOKS, Singletons, Multiples),
      rememberFact(s(Save,Mt,Vars,Assertion,DIRECTION,TOKS)).

chopvars([A],[A]):-!.
chopvars([Var|Rest],Out):-
      ((var(Var);number(Var);compound(Var)) -> chopvars(Rest,Out) ; ([Var|Rest]=Out)),!.
      
      



/*

(#$implies 
  (#$and 
    (#$natFunction ?PARTITION #$ThePartition) 
    (#$partitionedInto ?COL ?PARTITION) 
    (#$elementOf ?SPEC  (#$FormulaArgSetFn ?PARTITION))
    (#$elementOf ?SPEC2 (#$FormulaArgSetFn ?PARTITION))

  (#$different ?SPEC ?SPEC2)) 
  (#$disjointWith ?SPEC ?COL))  
*/



