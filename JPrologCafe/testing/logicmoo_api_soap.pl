:-include('logicmoo_utils_header.pl'). %<?
% ===========================================================
% NATIVE SOAPD SERVER FOR SWI-PROLOG
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(client=soap,Options),!,
        parse_moo_soap(Options).

clientEvent(Channel,Agent,Options):-memberchk(client=moo_xml,Options),!,
        parse_moo_soap(Options).


clientEvent(Channel,Agent, call(PrologGoal,ToplevelVars,Packet)):-!,
   wantsEvents(Channel,Agent,In,Out,_),
   callNondeterministicPrologCommandRDF(Session,AnswersFlag,In,Out,PrologGoal,ToplevelVars),!.

end_of_file.


isCycAPIQuit('API-QUIT').
isCycAPIQuit('api-quit').

/*
'PRINT'(X):-writel(X).

'TEST':-format('"hi"').

callCycApi(Out,PrologGoal,ToplevelVars):-write(Out,'200 ('),
	       PrologGoal,!,
	       writel(varslist(ToplevelVars,ToplevelVars)),write(Out,')\n'),flush_output(Out),!.
callCycApi(Out,PrologGoal,ToplevelVars):-write(Out,'NIL\n').

'API-QUIT':-'api-quit'.
'api-quit':-thread_self(Session),retractall(isKeepAlive(Session)).
*/


% ===========================================================
% NATIVE SERVICE
% ===========================================================

			    
%:-module(moo_soap,[]).

% :-include('logicmoo_utils_header.pl'). %<?

:-dynamic(xmlCurrentOpenTags/2).

serviceSoapRequest(In,Out):-
      writeSTDERR('SOAP Request'),
        catch(read_do_soap(stream(In),Out),E,
        say(Out,'<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n<error>~w</error>\n',[E])),
        catch(flush_output(Out),_,true).


read_do_soap(Source):-
        open(Source,read,Stream),
        read_do_soap(Stream,user_output).

read_do_soap(Source,Out):-
       thread_self(Self),
        write(Out,'<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n'),
       % say(Out,'<?xml version="1.0" encoding="ISO-8859-1"?>\n<answer thread="~w">\n',[Self]),
        catch(flush_output(Out),_,true),
        load_structure(Source,RDF,[]),
        structure_to_options(RDF,Options),
%       say(user_error,'structure="~q"\noptions="~q"\n',[RDF,Options]),
        catch(flush_output(user_error),_,true),
        clientEvent(Channel,Agent,[client=soap|Options]).
        %say(Out,'</answer>\n',[]).


% request
structure_to_options([element(request, Options, [Atom])],[submit=ask,sf=Atom|Options]):-!.

% assert
structure_to_options([element(assert, Options, [Atom])],[submit=assert,sf=Atom|Options]):-!.
structure_to_options([element(asssertion, Options, [Atom])],[submit=assert,sf=Atom|Options]):-!.
structure_to_options([element(assertion, Options, [Atom])],[submit=assert,sf=Atom|Options]):-!.

% get inner
structure_to_options([element(Ptag, ['xmlns:moo'=Server], Inner)],[opt_server=Server,opt_outter=Ptag|Out]):-!,
        structure_to_options(Inner,Out).


/*
xmlOpenTag(Name):-thread_self(Self),asserta(xmlCurrentOpenTags(Self,A)),sayServer('<~w>',[Name]),!.
xmlOpenTagW(Out,Name,Text):-thread_self(Self),asserta(xmlCurrentOpenTags(Self,A)),sayServer(Out,'~w',[Text]),!.

xmlCloseTag(Name):-thread_self(Self),ignore(retract(xmlCurrentOpenTags(Self,A))),sayServer('</~w>',[Name]),!.
xmlCloseTagW(Name,Text):-thread_self(Self),ignore(retract(xmlCurrentOpenTags(Self,A))),sayServer('~w',[Text]),!.
xmlCloseTagW(Out,Name,Text):-thread_self(Self),ignore(retract(xmlCurrentOpenTags(Self,A))),sayServer(Out,'~w',[Text]),!.

xmlClearTags:-thread_self(Self),retractall(xmlCurrentOpenTags(Self,A)).

xmlExitTags:-thread_self(Self),retract(xmlCurrentOpenTags(Self,A)),sayServer('</~w>',[Name]),fail.
xmlExitTags.
*/

/*

% ===========================================================
% PROLOGD for Java SERVICE
% ===========================================================

		writeSTDERR(invokePrologCommand(Session,In,Out,PrologGoal,ToplevelVars,Returns)),
                invokePrologCommand(Session,In,Out,PrologGoal,ToplevelVars,Returns),
                notKeepAlive(Out,Session),!.
invokePrologCommand(Session,In,Out,PrologGoal,ToplevelVars,Returns):-var(PrologGoal),!.

invokePrologCommand(Session,In,Out,PrologGoal,ToplevelVars,Returns):-
      %%  say(Out,'<prolog:solutions goal="~q">\n',[PrologGoal]),
        set_output(Out),set_input(In),!,
	ignore(catch(PrologGoal,_,true)),
        xmlExitTags,!.

*/

serviceNativeRequestAsRDF(_,In,Out):-
        say(Out,'<?xml version="1.0" encoding="ISO-8859-1"?>\n',[]),
        thread_self(Session),
        retractall(isKeepAlive(Session)),
        xmlClearTags,
        repeat,
                catch(
                        read_term(In,PrologGoal,[variable_names(ToplevelVars),character_escapes(true),syntax_errors(error)]),
                        E,
                        writeErrMsg(Out,E)),
                %writeSTDERR(PrologGoal:ToplevelVars),
                invokePrologCommandRDF(Session,In,Out,PrologGoal,ToplevelVars,Returns),
                notKeepAlive(Out,Session),!.

notKeepAlive(Out,Session):-isKeepAlive(Session),
        write(Out,
                'complete.\n'
                %'<prolog:keepalive/>\n'
                                ),catch(flush_output(Out),_,true),!,fail.
notKeepAlive(Out,Session):-catch(flush_output(Out),_,true).


keep_alive:-thread_self(Me),retractall(isKeepAlive(Me)),assert(isKeepAlive(Me)),writeFmtFlushed('<keepalive/>\n',[]).
goodbye:-thread_self(Me),retractall(isKeepAlive(Me)),say('<bye/>/n',[]).


invokePrologCommandRDF(Session,In,Out,PrologGoal,ToplevelVars,Returns):-var(PrologGoal),!.

invokePrologCommandRDF(Session,In,Out,PrologGoal,ToplevelVars,Returns):-
        term_to_atom(Session,Atom),concat_atom(['$answers_for_session',Atom],AnswersFlag),
        say(Out,'<prolog:solutions goal="~q">\n',[PrologGoal]),
        flag(AnswersFlag,_,0),
        set_output(Out),set_input(In),!,
        getCputime(Start),
        callNondeterministicPrologCommandRDF(Session,AnswersFlag,In,Out,PrologGoal,ToplevelVars),
        xmlExitTags,
        getCputime(End),
        flag(AnswersFlag,Returns,Returns),
%       (Returns > 0 ->
%               say(Out,'<prolog:yes/>\n',[]) ;
%               say(Out,'<prolog:no/>\n',[])),!,
        Elapsed is End -Start,
        say(Out,'</prolog:solutions answers="~w" cputime="~g">\n',[Returns,Elapsed]),!.

callNondeterministicPrologCommandRDF(Session,AnswersFlag,In,Out,PrologGoal,ToplevelVars):-
        ground(PrologGoal),!,
        catch(
                (PrologGoal,
                 flag(AnswersFlag,Answers,Answers+1),
                 writePrologToplevelVarsXML(Out,PrologGoal,AnswersFlag,ToplevelVars)
                 ),
           Err,writeErrMsg(Out,Err,PrologGoal)),!.

callNondeterministicPrologCommandRDF(Session,AnswersFlag,In,Out,PrologGoal,ToplevelVars):-
        catch(
                (PrologGoal,
                 flag(AnswersFlag,Answers,Answers+1),
                 writePrologToplevelVarsXML(Out,PrologGoal,AnswersFlag,ToplevelVars),
                 fail),
           Err,writeErrMsg(Out,Err,PrologGoal)),!.
callNondeterministicPrologCommandRDF(Session,AnswersFlag,In,Out,PrologGoal,ToplevelVars):-!.


writePrologToplevelVarsXML(Out,PrologGoal,AnswersFlag,ToplevelVars):-
         flag(AnswersFlag,Answers,Answers),
        say(Out,'<prolog:result solution="~w">\n',[Answers]),
        writePrologToplevelVarsXML2(Out,ToplevelVars),
        say(Out,'</prolog:result>\n',[]),!.

writePrologToplevelVarsXML2(Out,[]):-!.
writePrologToplevelVarsXML2(Out,[Term|REST]):-!,Term=..[_,N,V],
         writeFmtFlushed(Out,'       <prolog:p>~w = ~q</prolog:p>\n',[N,V]),
         writePrologToplevelVarsXML2(Out,REST),!.


% ===========================================================
% Insert
% ===========================================================
parse_moo_soap(Options):-memberchk(submit=assert,Options),!,
        getMooOption(Agent,opt_ctx_assert='#$BaseKB',Ctx),
        getMooOption(Agent,opt_theory='#$PrologDataMt',Context),
        getMooOption(Agent,sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        getMooOption(Agent,user='Web',Agent),
        getMooOption(Agent,interp='cycl',Interp),
        logOnFailure(getMooOption(Agent,tn=_,EXTID)),
        %sendNote(user,'Assert',formula(NEWFORM),'Ok.'). %,logOnFailure(saveCycCache)
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        xml_assert(Show,Ctx,Context,Agent).

xml_assert(Show,Ctx,Context,Agent):-
        getSurfaceFromChars(Show,STERM,Vars),
        getCycTermFromSurface(STERM,NEWFORM),
        xml_assert(Show,NEWFORM,Vars,Ctx,Context,Agent).

xml_assert(Show,Ctx,Context,Agent):-!,
        say('<assertionResponse accepted="false">\nUnable to parse: "~s"\n</assertionResponse>\n',[Show]).

xml_assert(Show,NEWFORM,Vars,Ctx,Context,Agent):-
        logOnFailure(getTruthCheckResults(tell,[untrusted],surface,NEWFORM,Ctx,STN,Context,Vars,Maintainer,Result)),
        (Result=accept(_) ->
                        (
                        once(invokeInsert([trusted,canonicalize,to_mem],surface,NEWFORM,Ctx,EXTID,Context,Vars,Agent)),
                        write('<assertionResponse accepted="true">\nOk.\n</assertionResponse>\n')
                        )
                        ;
                        (
                        Result=notice(FormatStr,Args),
                        write('<assertionResponse accepted="false">\n'),
                        say(FormatStr,Args),
                        write('\n</assertionResponse>\n')
                        )
        ),!.

xml_assert(Show,NEWFORM,Vars,Ctx,Context,Agent):-!.


% ===========================================================
% Ask a Request
% ===========================================================
parse_moo_soap(Options):-memberchk(submit=ask,Options),!,make,
        %write('<!DOCTYPE cyc:ask SYSTEM "/opt/tomcat-4.0/webapps/cyc-1.4b1/dtd/java_prolog.dtd">\n'),
        write('<cyc:ask xmlns:cyc="http://localhost">\n'),
        getMooOption(Agent,opt_ctx_request='#$BaseKB',Ctx),
        getMooOption(Agent,opt_theory='#$PrologDataMt',Context),
        getMooOption(Agent,sf=surf,Askion),
        atom_codes(Askion,Askion_Chars),
        getMooOption(Agent,user='Web',Agent),
        getMooOption(Agent,interp='cycl',Interp),
         logOnFailure(getCleanCharsWhitespaceProper(Askion_Chars,Show)),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getCycTermFromSurface(STERM,NEWFORM)),!,
              logOnFailure(once(( NEWFORM=comment(_) ->
                     (say('<error>Syntax Error: Unmatched parentheses in "~s"</error>\n',[Show]),!,FORM=_) ;(!,
                     logOnFailure(invokeRequest_xml(NEWFORM,ChaseVars,Ctx,TrackingAtom,Context,Agent,Vars,CPU))
                     )))),
        write('</cyc:ask>\n').

invokeRequest_xml(NEWFORM,ChaseVars,Ctx,TrackingAtom,Context,Agent,Vars,CPU):-
        invokeRequestToBuffer(NEWFORM,ChaseVars,Ctx,TrackingAtom,Context,Agent,Vars,CPU),
        final_answer(Logic:How),
        invoke_final_answer(Logic,How,CPU).

invoke_final_answer(possible,How,CPU):-!,
        say('<requestResponse yesno="~w" numBindings="0" seconds="~w"/>\n',[How,CPU]).

invoke_final_answer(Logic,How,CPU):-
        say('<requestResponse yesno="~w" numBindings="~w" seconds="~w">\n<bindings>\n',[Logic,How,CPU]),
        cite_xml_buffered_answers,
        write('</bindings>\n</requestResponse>\n').


cite_xml_buffered_answers:-
        retract(requestBuffer_db(UResultsSoFar,Result,Explaination,Status)),
        once(inform_xml_agent(UResultsSoFar,Result,Explaination,Status)),fail.

% Call to write Summary
/*
cite_xml_buffered_answers:-
        final_answer(Logic:How),
        writeDebug(final_answer(Logic:How)),
        inform_xml_agent(How, ['Summary'=Logic|_G14093],final_answer(Logic:How),final_answer(Logic:How) ).
*/
cite_xml_buffered_answers:-!.

% ===========================================================
% Send to debugger
% ===========================================================
inform_xml_agent(UResultsSoFar,Result,InExplaination,Status):-
        writeDebug(inform_xml_agent(UResultsSoFar,Result,InExplaination,Status)),fail.

% ===========================================================
% Hide certain returns
% ===========================================================
inform_xml_agent(-1,Result,Explaination,Status).

inform_xml_agent(0, ['Result'=none|A], 'Unproven', done(possible:searchfailed)).
inform_xml_agent(_, ['Result'=true|A], found(_), done(true:_)).
inform_xml_agent(_, ['Summary'=_|_G5892], _, _).

% ===========================================================
% Write Answers
% ===========================================================
inform_xml_agent(UResultsSoFar,Result,InExplaination,Status):-
        say('<binding>\n',[]),
        inform_xml_vars(Result,Vars),
        length_explaination(InExplaination,InLength),
        findall(Length-Explaination,
                (retract(inform_xml_agent_buffer_db(_,Result,Explaination,_)),
                 length_explaination(Explaination,Length)
                 ),KeyList),

        keysort([(InLength-InExplaination)|KeyList],[(_-ChoiceExplaination)|_]),
        inform_xml_explaination(InLength,ChoiceExplaination,Result),
        say('</binding>\n',[]).

inform_xml_vars(Result,Vars):-
        length_var(Result,NumVar),
        say('<variables numVars="~w">\n',[NumVar]),
        inform_each_variable(Result,Vars),
        say('</variables>\n',[]).

length_var([],0).
length_var([A|'$VAR'(_)],1).
length_var([A|L],N):-
          length_var(L,NN),
          N is NN +1.

inform_each_variable([],Vars).
inform_each_variable('$VAR'(_),Vars).
inform_each_variable([NV|Rest],Vars):-
        inform_nv(NV,Vars),
        inform_each_variable(Rest,Vars).


inform_nv('$VAR'(_),Vars).
inform_nv(Name=Value,Vars):-
        toMarkup(cycl,Name,Vars,OName),
        toMarkup(cycl,Value,Vars,OValue),
        say('<var varName="~w" value="~w"/>\n',[OName,OValue]).


inform_xml_explaination(InLength,ChoiceExplaination,Result):-
        say('<explaination numSteps="~w">',[InLength]),
        flag(explaination_linenumber,_,0),
        writeObject_explaination(ChoiceExplaination,Result),
        say('</explaination>\n').





