% ===========================================================
% SOCKET SERVER - Looks at first charicater of request and decides between:
%  Http, Native or Soap and replies accordingly
% ===========================================================
/*
:-module(moo_httpd,[
   createPrologServer/1,
   xmlPrologServer/1,
   read_line_with_nl/3,
   decodeRequest/2,
   invokePrologCommandRDF/6,
   serviceAcceptedClientSocketAtThread/1]).
*/

% :-include(moo_header).



% :-use_module(moo_threads).
%% :-ensure_loaded(system_dependant).

:-dynamic(isKeepAlive/1).

createPrologServer(Port) :-
        mooProcessCreate(nokill,'Moo XML/SOAP Server Socket',xmlPrologServer(Port),_,[]).

xmlPrologServer(Port):-
        tcp_socket(ServerSocket),
        catch(ignore(tcp_setopt(ServerSocket, reuseaddr)),_,true),
        at_halt(tcp_close_socket(ServerSocket)),
        attemptServerBind(ServerSocket, Port),
        tcp_listen(ServerSocket, 655),
        repeat,
	       acceptClientsAtServerSocket(ServerSocket),
        fail.


attemptServerBind(ServerSocket, Port):-
        catch((tcp_bind(ServerSocket, Port),
        flush_output,
        writeSTDERR('cs.\nMoo server started on port ~w. \n\nYes\n?- ',[Port]),flush_output),
        error(E,_),
        (writeSTDERR('\nWaiting for OS to release port ~w. \n(sleeping 4 secs becasue "~w")\n',[Port,E]),
        sleep(4),
        attemptServerBind(ServerSocket, Port))),!.


acceptClientsAtServerSocket(ServerSocket):-
		tcp_open_socket(ServerSocket, AcceptFd, _),
                cleanOldProcesses,!,
		tcp_accept(AcceptFd, ClientSocket, ip(A4,A3,A2,A1)),!,
                getPrettyDateTime(DateTime),
                sformat(Name,'Dispatcher for ~w.~w.~w.~w  started ~w ',[A4,A3,A2,A1,DateTime]),
                mooProcessCreate(killable,Name,serviceAcceptedClientSocketAtThread(ClientSocket),_,[detatch(true)]),!.

serviceAcceptedClientSocketAtThread(ClientSocket):-
	tcp_open_socket(ClientSocket, In, Out),!,
        setMooOption('$socket_in',In),
        setMooOption('$socket_out',Out),!,
        serviceIO(In,Out),
        flush_output,
	catch(tcp_close_socket(ClientSocket),_,true),
	prolog_thread_exit(complete).



getPrettyDateTime(String):-get_time(Time),convert_time(Time, String).

serviceIO(In,Out):-
        peek_char(In,Char),!,
	%writeSTDERR(serviceIOBasedOnChar(Char,In,Out)),
	serviceIOBasedOnChar(Char,In,Out),!.


serviceIOBasedOnChar('G',In,Out):-!,  
         serviceHttpRequest(In,Out).

serviceIOBasedOnChar('P',In,Out):-!,
         serviceHttpRequest(In,Out).

serviceIOBasedOnChar('<',In,Out):-!,
         serviceSoapRequest(In,Out).  % see moo_soap.pl

serviceIOBasedOnChar(C,In,Out):-
        serviceNativeRequest(C,In,Out).

% ===========================================================
% HTTPD SERVICE
% ===========================================================

serviceHttpRequest(In,Out):-
        readHTTP(In,Options),
        writeFmtFlushed(Out,'HTTP/1.1 200 OK\nServer: Moo-HTTPD\nContent-Type: text/html\n\n',[]),
        setMooOption(client,html),
        tell(Out),
        writeSTDERR('REQUEST: "~q" \n',[Options]), 
	processRequest(Options),
	flush_output.


readHTTP(In,Request):-
        read_line_with_nl(In, Codes, []),
        append("GET /",Stuff,Codes), %true,
        append(RequestCodes,[72,84,84,80|_],Stuff),
        atom_codes(RequestEncoded,RequestCodes),
        decodeRequest(RequestEncoded,Request).

readHTTP(In,Request):-
        read_line_with_nl(In, Codes, []),
        append("POST /",Stuff,Codes), %true,
        append(RequestCodes,[72,84,84,80|_],Stuff),
        atom_codes(RequestEncoded,RequestCodes),
        decodeRequest(RequestEncoded,Request).


read_line_with_nl(Fd, Codes, Tail) :-
        get_code(Fd, C0),
        read_line_with_nl(C0, Fd, Codes, Tail).
read_line_with_nl(end_of_file, _, Tail, Tail) :- !.
read_line_with_nl(-1, _, Tail, Tail) :- !.
read_line_with_nl(10, _, [10|Tail], Tail) :- !.
read_line_with_nl(C, Fd, [C|T], Tail) :-
        get_code(Fd, C2),
        read_line_with_nl(C2, Fd, T, Tail).



decodeRequest(RequestEncoded,[file=Request]):-
      concat_atom([X],'?',RequestEncoded),
      www_form_encode(Request,X),!.
decodeRequest(RequestEncoded,[file=Request|ENCARGS]):-
      concat_atom([X,ARGS],'?',RequestEncoded),
      www_form_encode(Request,X),
      concat_atom(ArgList,'&',ARGS),
      decodeRequestArguments(ArgList,ENCARGS).

decodeRequestArguments([],[]):-!.
decodeRequestArguments([ctx=Value|List],[ctx=CValue,theory=KValue|ARGS]):-
          concat_atom([KValue,CValue],':',Value),!,
          decodeRequestArguments(List,ARGS).
decodeRequestArguments([Arg|List],[DName=DValue|ARGS]):-
          split_nv(Arg,Name,Value),
          www_form_encode(AName,Name),
          www_form_encode(AValue,Value),!,
          decodeRequestAtom(AName,DName),
          decodeRequestAtom(AValue,DValue),
          decodeRequestArguments(List,ARGS).

%ctx=PrologMOO%3ASTRUCTURAL-ONTOLOGY&

split_nv(Arg,Name,Value):-concat_atom([Name,Value],'=',Arg),!.
split_nv(Arg,Arg,Arg).

decodeRequestAtom(A,A):-var(A),!.
decodeRequestAtom(tn,tn):-!.
decodeRequestAtom(N,N):-number(N),!.
decodeRequestAtom(A=B,AA=BB):-
                decodeRequestAtom(A,AA),
                decodeRequestAtom(B,BB),!.
decodeRequestAtom(A,T):-catch(atom_to_term(A,T,_),_,fail),number(T),!.
decodeRequestAtom(A,T):-catch(atom_to_term(A,T,_),_,fail),not(var(T)),not(compound(T)),!.
decodeRequestAtom(A,T):-atom(A),catch(atom_codes(A,[95|_]),_,fail),catch(atom_to_term(A,T,_),_,fail),!.
decodeRequestAtom(A,A):-!.

% ===========================================================
% NATIVE SERVICE
% ===========================================================

serviceNativeRequest(_,In,Out):-
        writeFmt(Out,'<?xml version="1.0" encoding="ISO-8859-1"?>\n',[]),
        getThread(Session),
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


keep_alive:-getThread(Me),retractall(isKeepAlive(Me)),assert(isKeepAlive(Me)),writeFmtFlushed('<keepalive/>\n',[]).
goodbye:-getThread(Me),retractall(isKeepAlive(Me)),writeFmt('<bye/>/n',[]).


invokePrologCommandRDF(Session,In,Out,PrologGoal,ToplevelVars,Returns):-var(PrologGoal),!.

invokePrologCommandRDF(Session,In,Out,PrologGoal,ToplevelVars,Returns):-
        term_to_atom(Session,Atom),concat_atom(['$answers_for_session',Atom],AnswersFlag),
        writeFmt(Out,'<prolog:solutions goal="~q">\n',[PrologGoal]),
        flag(AnswersFlag,_,0),
        set_output(Out),set_input(In),!,
        getCputime(Start),
        callNondeterministicPrologCommand(Session,AnswersFlag,In,Out,PrologGoal,ToplevelVars),
        xmlExitTags,
        getCputime(End),
        flag(AnswersFlag,Returns,Returns),
%       (Returns > 0 ->
%               writeFmt(Out,'<prolog:yes/>\n',[]) ;
%               writeFmt(Out,'<prolog:no/>\n',[])),!,
        Elapsed is End -Start,
        writeFmt(Out,'</prolog:solutions answers="~w" cputime="~g">\n',[Returns,Elapsed]),!.

callNondeterministicPrologCommand(Session,AnswersFlag,In,Out,PrologGoal,ToplevelVars):-
        ground(PrologGoal),!,
        catch(
                (PrologGoal,
                 flag(AnswersFlag,Answers,Answers+1),
                 writePrologToplevelVarsXML(Out,PrologGoal,AnswersFlag,ToplevelVars)
                 ),
           Err,writeErrMsg(Out,Err,PrologGoal)),!.





callNondeterministicPrologCommand(Session,AnswersFlag,In,Out,PrologGoal,ToplevelVars):-
        catch(
                (PrologGoal,
                 flag(AnswersFlag,Answers,Answers+1),
                 writePrologToplevelVarsXML(Out,PrologGoal,AnswersFlag,ToplevelVars),
                 fail),
           Err,writeErrMsg(Out,Err,PrologGoal)),!.
callNondeterministicPrologCommand(Session,AnswersFlag,In,Out,PrologGoal,ToplevelVars):-!.


writePrologToplevelVarsXML(Out,PrologGoal,AnswersFlag,ToplevelVars):-
         flag(AnswersFlag,Answers,Answers),
        writeFmt(Out,'<prolog:result solution="~w">\n',[Answers]),
        writePrologToplevelVarsXML2(Out,ToplevelVars),
        writeFmt(Out,'</prolog:result>\n',[]),!.

writePrologToplevelVarsXML2(Out,[]):-!.
writePrologToplevelVarsXML2(Out,[Term|REST]):-!,Term=..[_,N,V],
         writeFmtFlushed(Out,'       <prolog:p>~w = ~q</prolog:p>\n',[N,V]),
         writePrologToplevelVarsXML2(Out,REST),!.


writeFmt(A,B,C):-!.
writeFmt(A,B):-!.

writeFmt(A,B,C):-
        writeFmtFlushed(A,B,C).
writeFmt(A,B):-
        writeFmtFlushed(A,B).


throwMoo(Module,Type,Details):-
        current_prolog_flag(debug_on_error, DebugOnError),
        set_prolog_flag(debug_on_error, false),!,
        throw(mooException(Module,Type,Details,DebugOnError)),
        ifInteractive(writeDebug('Post throwMoo')),!.



