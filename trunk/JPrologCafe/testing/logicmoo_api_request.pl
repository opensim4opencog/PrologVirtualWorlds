% ===================================================================
% File 'moo_api.pl'
% Maintainer: Douglas Miles
% Contact: dmiles@users.sourceforge.net ;
% Version: 'moo_api.pl' 1.0.0
% Revision:             $Revision: 1.1 $
% Revised At:   $Date: 2002/12/03 08:34:05 $
% ===================================================================
% PURPOSE
% ===================================================================
% This file meets the needs of an external agent working for the needs of eigther an automated or human user
% Interfacea with Java with XML to display explaination trees and variable bindings
% It defines the default settings most users will use and gives a starting expected state.
% Ask a Context, tell to a Context, retract from a Context, consult the user (this function is currently treated by ua_command/2, but it is planned to separate it),
% Report status to Moo, initialization of the LE, and file-handling routines
% ===================================================================
:-include('logicmoo_utils_header.pl'). %<?
% ===========================================================
% SOCKET SERVER - Looks at first charicater of request and decides between:
%  Http, Native or Soap and replies accordingly
% ===========================================================
% =================================================================================
/*
createCycServer(BasePort) :-isServerCreated(BasePort),!.
createCycServer(BasePort) :-
         asserta(isServerCreated(BasePort)),
         AsciiPort1 is BasePort+1,
         AsciiPort2 is BasePort+2,
         CFASLPORT is BasePort+14,
         COSRVER is BasePort+79,
        servantProcessCreate(nokill,'Logicmoo/CYC HTTPD/CycL/XML/SOAP Server Socket',adaptiveServer(AsciiPort1),_,[global(0),local(0),trail(0)]),
        servantProcessCreate(nokill,'Prolog HTTPD Server Socket',adaptiveServer(AsciiPort2),_,[global(0),local(0),trail(0)]),
        %servantProcessCreate(nokill,'CFASL Server Socket',cfaslServer(CFASLPORT),_,[]),
        %servantProcessCreate(nokill,'COPROCESSOR Server Socket',coServer(COSRVER),_,[]),
        ensureCycCallsProlog('10.10.10.198',AsciiPort1),!.


attemptServerBind(ServerSocket, Port):-
        catch((tcp_bind(ServerSocket, Port),
        debugFmt('% CYC Prolog API server started on port ~w. \n',[Port])),
        error(E,_),
        debugFmt('% CYC Prolog API server not started on port ~w becasue: "~w"\n',[Port,E])).

traceCall(X):-
   debugFmt('ENTER: ~q ',[X]),
      call(X),
   debugFmt('EXIT: ~q ',[X]).


:- use_module(library(streampool)).

adaptiveServer(Port):-
% streampool_server(Port) :-
        tcp_socket(ServerSocket),
        catch(ignore(tcp_setopt(ServerSocket, reuseaddr)),_,true),
        tcp_bind(ServerSocket, Port),
        tcp_listen(ServerSocket, 5),
        tcp_open_socket(ServerSocket, In, _Out),
        add_stream_to_pool(In, streampool_accept(ServerSocket)),
        stream_pool_main_loop.

streampool_accept(ServerSocket) :-
        tcp_accept(ServerSocket, AcceptFd, Peer),
        tcp_open_socket(AcceptFd, In, Out),
        add_stream_to_pool(In, streampool_servant(AcceptFd,In, Out, Peer)).

streampool_servant(ClientSocket,In, Out, Peer) :-
  %      read_line_to_codes(In, Command),
        ignore(ip(A4,A3,A2,A1)=Peer),
   %  cleanOldProcesses_p,!,
   setMooOption('$source_ip',ip(A4,A3,A2,A1)),
   getPrettyDateTime(DateTime),
   %setMooOption('$datetime',DateTime),
   sformat(Name,'Dispatcher for ~w.~w.~w.~w  started ~w ',[A4,A3,A2,A1,DateTime]),
   debugFmt('~s',[Name]),!,
  servantProcessCreate(killable,Name,serviceAcceptedClientSocketAtThread([ip(A4,A3,A2,A1)],ClientSocket,In,Out),_,[global(0),local(0),trail(0)]),!,
  delete_stream_from_pool(In).

serviceAcceptedClientSocketAtThread(OClientInfo,ClientSocket,In,Out):-
   %    tcp_open_socket(ClientSocket,In,Out),!,      
       ClientInfo = ['io'(ClientSocket,In,Out)|OClientInfo],
        setMooOption('socket',ClientSocket),
        setMooOption('$socket_in',In),
        setMooOption('$socket_out',Out),!,
        set_prolog_IO(In,Out,user_error),
        ignore(catch(serviceIO(ClientInfo,In,Out),E,debugFmt(E:serviceIO(ClientInfo,In,Out)))),
        flush_output,seen,told,
        ignore(catch(close(In,[force(true)]),_,true)),
	ignore(catch(close(Out,[force(true)]),_,true)),
	ignore(catch(tcp_close_socket(ClientSocket),_,true)),
	thread_exit(ClientInfo).      


:-dynamic_transparent(isKeepAlive/1).

:-dynamic_transparent(isServerCreated/1).
startCycAPIServer:-isServerCreated(_),!.
startCycAPIServer:- createCycServer(4600),!.


removalPredicateCodedInProlog(Pred,Prolog).

prologEval(X,Y):-Y is X + 111.
prologEval(X,Y):-Y is X + 211.

:-use_module(library(odbc)).  
:-odbc_debug(5).


:-dynamic(ensureCycCallsProlog/2).

ensureCycCallsProlog(Host,Port):-
   sformat(SFormat,'(progn (define callprologpred (outval) (clet ((*retval* nil)(*stream* (OPEN-TCP-STREAM *prolog-host* *prolog-port*)))
   (prin1 outval *stream*)(force-output *stream*)(terpri *stream*)(force-output *stream*)(csetq *retval* (read *stream*))
   (close *stream*)(ret *retval*))) (defvar *prolog-host* "~w")(csetq *prolog-host* "~w")(defvar *prolog-port* ~w)(csetq *prolog-port* ~w))',[Host,Host,Port,Port]),
   evalSubL(SFormat,_,_),

   asserta((ensureCycCallsProlog(_,Port):-!)),!.

evalSubL(Send,Surface,Vars):-
     converseRaw(Send,Receive),!,
     getSurfaceFromChars(Receive,Surface,Vars).

                                    */
% =================================================================================
% Server           
% =================================================================================
% =================================================================================
runServerThread(Port,Type) :-
	concat_atom([Type,' World Server'],Name),
        servantProcessCreate(kill,Name,networkServer(Port,Type),_,[local(0),global(0),trail(0),argument(0)]).


networkServer(Port,Type):-
	 prolog_system(swi),!,
        tcp_socket(ServerSocket),
        %catch(ignore(tcp_setopt(ServerSocket, reuseaddr)),_,true),
        at_halt(tcp_close_socket(ServerSocket)),
        attemptServerBind(ServerSocket, Port),
        tcp_listen(ServerSocket, 655),
        catch((
	 repeat,
	 acceptNetworkClient(ServerSocket,Type),
	 fail),E,
	 (writeSTDERROnDebug(E),
	 ignore(catch(tcp_close_socket(ServerSocket),_,true)),!,
	 networkServer(Port,Type))),!.

networkServer(Port,Type):-
	 prolog_system(yap),!,
         	socket('AF_INET', Sock),
		 socket_bind(Sock, 'AF_INET'('12.211.44.132', Port)),
		 socket_listen(Sock, 5),
        catch((
	 repeat,
	 acceptNetworkClient(Sock,Type),fail),E,
	 (writeSTDERROnDebug(E),
	 ignore(catch(socket_close(Sock),_,true)),!,
	 networkServer(Port,Type))),!.



:-dynamic_transparent(isKeepAlive/1).
/*
attemptServerBind(ServerSocket, Port):-
        catch((tcp_bind(ServerSocket, Port),
        writeSTDERR('% API server started on port ~w. \n',[Port])),
        error(E,_),
        writeSTDERR('% API server not started on port ~w becasue: "~w"\n',[Port,E])).
*/
writeSTDERROnDebug(X):-writeSTDERR(X).
writeSTDERROnDebug(X,Y):-writeSTDERR(X,Y).
% =================================================================================
% =================================================================================
% Client Accepter dispatches to client thread
% =================================================================================
% =================================================================================
acceptNetworkClient(Sock,Type):- prolog_system(yap),!,
	socket_accept(Sock, Loc, ClientSocket),
	 socket_buffering(Sock,read,_,unbuf),
	 socket_buffering(Sock,write,_,unbuf),
	       socket_buffering(ClientSocket,read,_,unbuf),
	       socket_buffering(ClientSocket,write,_,unbuf),
                getPrettyDateTime(DateTime),
                sformat(Name,'~w dispatcher for   started ~w ',[Type,Loc,DateTime]),
		writeSTDERROnDebug(Name),
		cleanOldProcesses,
		servantProcessCreate(killable,Name,serviceAcceptedClient(Loc,ClientSocket,Type),_,[detatch(true),local(0),global(0),trail(0),argument(0)]),!.

acceptNetworkClient(ServerSocket,Type):-
		tcp_open_socket(ServerSocket, AcceptFd, _),
		tcp_accept(AcceptFd, ClientSocket, Loc),
		ignore(Loc = ip(A4,A3,A2,A1)),
                getPrettyDateTime(DateTime),
                sformat(Name,'~w dispatcher for ~w.~w.~w.~w  started ~w ',[Type,A4,A3,A2,A1,DateTime]),
		writeSTDERROnDebug(Name),
		cleanOldProcesses,
		servantProcessCreate(killable,Name,serviceAcceptedClient(Loc,ClientSocket,Type),_,[detatch(true),local(0),global(0),trail(0),argument(0)]),!.

% =================================================================================
% =================================================================================
% Client Handler Thread
% =================================================================================
% =================================================================================
:-dynamic(hasLeft/1).
:-dynamic(wantsEvents/4).

serviceAcceptedClient(Agent,ClientSocket,Type):-prolog_system(yap),!,
	 Out = ClientSocket,In= ClientSocket,
	 set_output(Out),set_input(In),
	 setMooOption(Agent,'$socket_in',In),
	 setMooOption(Agent,'$socket_out',Out),!,
	 %thread_self(CC),
	 CC = io(In,Out),
	 retractall(wantsEvents(Channel,Agent,_,_,_)),
	 negotateType(Channel,Agent,Type,ClientSocket, In, Out,NewType),
	 asserta(wantsEvents(Channel,Agent,In,Out,NewType)),!,
	 set_default_channel(Out),
	 repeat,
	       catch((once(readRequest(Channel,Agent,CMD)),once(sendEvent(Channel,Agent,CMD)),!,flush_output(Out)),
	       E,(writeSTDERROnDebug('~w: signalled ~q~n',[Channel,Agent,E]),killClient(CC))),
	       hasLeft(CC),!,
	       catch(flush_output,_,true),
	       catch(tcp_close_socket(ClientSocket),_,true),
	       prolog_thread_exit(complete),!.

serviceAcceptedClient(Loc,ClientSocket,Type):-prolog_system(swi),!,
	 Agent = Loc,
	 Channel = NewType,
	 tcp_open_socket(ClientSocket, In, Out),!,
	 set_output(Out),set_input(In),
	 setMooOption(Agent,'$socket_in',In),
	 setMooOption(Agent,'$socket_out',Out),!,
	 set_default_channel(Out),
	 %thread_self(CC),
	 CC = io(In,Out),
	 retractall(wantsEvents(Channel,Agent,_,_,_)),
	 negotateType(Channel,Agent,Type,ClientSocket, In, Out,NewType),
	 asserta(wantsEvents(Channel,Agent,In,Out,NewType)),!,
	 repeat,
	       catch((once(readRequest(Channel,Agent,CMD)),!,once(sendEvent(Channel,Agent,CMD)),flush_output(Out)),
	       E,(writeSTDERROnDebug('~w: signalled ~q durriing ~w ~n',[Channel,Agent,E,CMD]),killClient(CC))),
	       hasLeft(CC),!,
	       catch(flush_output,_,true),
	       catch(tcp_close_socket(ClientSocket),_,true),
	       prolog_thread_exit(complete),!.

killClient(CC):-asserta(hasLeft(CC)),logoff(CC).

% =================================================================================
% =================================================================================
% Get Command Packaet from input stream
% =================================================================================
% =================================================================================

readRequest(Channel,Agent,PackedTok):-
   wantsEvents(Channel,Agent,In,Out,NewType),
   readRequest(Channel,Agent,In,NewType,PackedTok).


readRequest(Channel,Agent,In,'Packed',PackedTok):-!,read_to(In,Codes),!,tokenize_packed(Codes,Tokens),!,
	    tokensToProlog(Tokens,PackedTok),!.
readRequest(Channel,Agent,In,'Bach_beta2',PackedTok):-!,read_to_bach(In,Codes),!,tokenize_bach(Codes,Tokens),!,
	    tokensToProlog(Tokens,PackedTok),!.

readRequest(Channel,Agent,In,'TELNETDX',english(phrase(Lower,String),Packet)):-!,
	 readTelentLine(In,String),
	 codesToForms(String,[lowercase,words,idioms(doctor)],Lower),
	 codesToForms(String,[words,idioms(doctor)],Normal),
	 packetFromCC(Channel,Agent,Packet).

readRequest(Channel,Agent,In,'HTTPD',request(Options,Packet)):-!,
   wantsEvents(Channel,Agent,In,Out,_),
   asserta(hasLeft(CC)),
        readHTTP(In,Options),
        writeFmtFlushed(Out,'HTTP/1.0 200 OK\r\nServer: LogicMooCyc-HTTPD\r\nContent-Type: text/html\r\n\r\n',[]),
        setMooOption(Agent,client,html),
        tell(Out),
        writeSTDERR('REQUEST: "~q" \n',[Options]), 
	setMooOption(Agent,Options),
	packetFromCC(Channel,Agent,Packet),!.


readRequest(Channel,Agent,In,'SOAP',request(Options,Packet)):-!,
   wantsEvents(Channel,Agent,In,Out,_),
      writeSTDERR('SOAP Request'),
        catch(read_soap(In,Out,[client=soap|Options]),E,
        say(Out,'<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n<error>~w</error>\n',[E])),
        catch(flush_output(Out),_,true),!.


readRequest(Channel,Agent,In,'JAVA',call(PrologGoal,ToplevelVars,Packet)):-!,
   asserta(hasLeft(CC)),
   wantsEvents(Channel,Agent,In,Out,_),
      get0(In,Plus),
        thread_self(Session),
        retractall(isKeepAlive(Session)),
        xmlClearTags,
	catch(read_term(In,PrologGoal,[variable_names(ToplevelVars),character_escapes(true),syntax_errors(error)]),E,writeErrMsg(Out,E)),
	packetFromCC(Channel,Agent,Packet),!.


readRequest(Channel,Agent,In,'CycApiRequest',call(PrologGoal,ToplevelVars,Packet)):-!,
   wantsEvents(Channel,Agent,In,Out,_),
   thread_self(Session),
   retractall(isKeepAlive(Session)),
   asserta(isKeepAlive(Session)),
      readCycL(In,Trim), 
      getSurfaceFromChars(Trim,[Result],ToplevelVars),
	 set_output(Out),set_input(In),
      balanceBinding(Result,PrologGoal),
	packetFromCC(Channel,Agent,Packet),!.


readRequest(Channel,Agent,In,'PROLOG',call(PrologGoal,ToplevelVars,Packet)):-!,
   wantsEvents(Channel,Agent,In,Out,_),
        thread_self(Session),
        retractall(isKeepAlive(Session)),
        xmlClearTags,
	catch(read_term(In,PrologGoal,[variable_names(ToplevelVars),character_escapes(true),syntax_errors(error)]),E,writeErrMsg(Out,E)),
	packetFromCC(Channel,Agent,Packet),!.

readRequest(Channel,Agent,In,'UOX',message(CMD,Packet)):-!,
   wantsEvents(Channel,Agent,In,Out,_),
   uoReadPacket(In,CMD),
	packetFromCC(Channel,Agent,Packet),!.

readRequest(Channel,Agent,In,Transport,error(Codes)):-read_to(In,Codes),!.

readTelentLine(In,Line):-readUntil(10,In,LineIn),trim(LineIn,Line).
readTelentLineAtom(In,Atom):-readTelentLine(In,Line),atom_codes(Atom,Line).


packetFromCC(Channel,Agent,packet('world_0', 0, Agent, Serialno, S1970)):-get_time(S1970),getLocalSerial(Serialno),getActiveForCC(Channel,Agent,Agent).
% =================================================
% Front Page
% =================================================
clientEvent(Channel,Agent,[file='']):-
   writeHTMLStdHeader('Welcome Engineer'),
   writeHTMLStdFooter.

% =================================================
% Command is missing (404)
% =================================================
clientEventUnhandled(Channel,Agent,X):- once(channel_say('"#logicmoo"',clientEventUnhandled(Channel,Agent,X))),fail.
clientEventUnhandled(Channel,Agent,X):-
   memberchk(file=_,X),
   isMooOption(Agent,client,html),!,
   writeHTMLStdHeader('404 handler'),
   say('~q<br><p>Welcome to LogicMoo is the request page handler for non-understood requests',[X]),
   showMooProcessHTML,
   writeHTMLStdFooter.

% =================================================
% SubL
% =================================================
clientEvent(Channel,Agent,[file='subl.moo']):-
      writeHTMLStdHeader('SubL Interactor'),
      say('
      <form method="GET">
	<p><textarea rows="9" name="formula" cols="40"></textarea><br>
	<input type="submit" value="Call" name="submit">&nbsp;<input type="reset" value="Reset" name="resetButton"></p>
      </form>'),
      writeHTMLStdFooter.

clientEvent(Channel,Agent,[file='english.moo'|A]):-!,
      writeHTMLStdHeader('English Expert Chat'),
      getMooOption(Agent,utter='What is the capitol of Russia?',Utter),
      say('
      <form method="GET">
	<p><textarea rows="4" name="utter" cols="40">~w</textarea><br>
	<input type="submit" value="Say" name="call">&nbsp;<input type="submit" value="Test" name="call">&nbsp;<input type="reset" value="Reset" name="resetButton"></p>
      </form>',[Utter]),!,
      say('<pre>'),
      tgramS(Utter,Semantic1,Assertions),
      writeq(Semantic1),
      say('</pre>'),!,
      writeHTMLStdFooter.

clientEvent(Channel,Agent,[file='daml.moo'|A]):-!,
      writeHTMLStdHeader('Daml Tools'),
      writeHTMLStdFooter.

clientEvent(Channel,Agent,[file='wn.moo'|A]):-!,
      writeHTMLStdHeader('WordNet Explorer'),
      writeHTMLStdFooter.

clientEvent(Channel,Agent,[file='cycml.moo'|A]):-!,
      writeHTMLStdHeader('CycML Import/Export'),
      writeHTMLStdFooter.

