:-include('logicmoo_utils_header.pl').
% ===========================================================
% HTTPD SERVICE
% ===========================================================
readHTTP(In,Options):-
	 read_line_with_nl(In, Codes, []),
	 (append("GET /",Stuff,Codes);append("POST /",Stuff,Codes)),!,
	 append(RequestCodes,[72,84,84,80|_],Stuff),
	 atom_codes(RequestEncoded,RequestCodes),
	 decodeRequest(RequestEncoded,OptionsIn),
	 suckIn(In),
	 ensureMooOption(Agent,client,html,_),
	 logOnFailure(fixOptionsFromForeign(OptionsIn,Options)),
	 logOnFailure(setMooOption(Agent,Options)).


suckInRest(In,Rest):-
        repeat,read_line_with_nl(In, Codes2, []),Codes2=[_|_].
suckIn(In):-
        read_line_with_nl(In, Codes2, []),
	 format(user_error,'sucked: "~s"~n',[Codes2]),!.

decodeRequest(RequestEncoded,ENCARGS):-%trace,
      concat_atom([Request,ARGS],'?',RequestEncoded),
      concat_atom(ArgList,'&',ARGS),
      decodeRequestArguments([file=Request|ArgList],ENCARGS),!.

decodeRequest(RequestEncoded,ENCARGS):-%trace,
      concat_atom([Request,ARGS],'?',RequestEncoded),
      decodeRequestArguments([file=Request,params=ARGS],ENCARGS),!.

decodeRequest(RequestEncoded,ENCARGS):-%trace,
      concat_atom([Request,ARGS],' ',RequestEncoded),
      decodeRequestArguments([file=Request],ENCARGS),!.

decodeRequest(RequestEncoded,ENCARGS):-
      decodeRequestArguments([file=RequestEncoded],ENCARGS),!.

decodeRequestArguments([],[]):-!.
decodeRequestArguments([Arg|List],[DName=DValue|ARGS]):-
          split_nv(Arg,Name,Value),
          www_form_encode(AName,Name),
          www_form_encode(AValue,Value),!,
          decodeRequestAtom(AName,DName),
          decodeRequestAtom(AValue,DValue),
	  writeSTDERR(DName=DValue),
          decodeRequestArguments(List,ARGS).

%ctx=LogicMOO%3ASTRUCTURAL-ONTOLOGY&

split_nv(N=V,N,V):-!.
split_nv(Arg,Name,Value):-concat_atom([Name,Value],'=',Arg),!.
split_nv(Arg,Arg,Arg).

decodeRequestAtom(A,A):-var(A),!.
decodeRequestAtom(A=B,AA=BB):-decodeRequestAtom(A,AA),decodeRequestAtom(B,BB),!.
decodeRequestAtom(A,A):-not(atom(A)),!.
decodeRequestAtom(A,T):-atom(A),atom_concat(Left,' ',A),!,decodeRequestAtom(Left,T).
decodeRequestAtom(A,T):-atom(A),atom_codes(A,[95|_]),catch(atom_to_term(A,T,_),_,fail),!.
%decodeRequestAtom(A,T):-atom(A),catch(atom_to_term(A,T,_),_,fail),nonvar(T),!.
decodeRequestAtom(A,A).
			     

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
fixOptionsFromForeign([Agent=AValue|List],[Channel=DValue|ARGS]):-
          transform_option(Agent,Channel),!,
          logOnFailure(decodeRequestAtom(AValue,DValue)),
          fixOptionsFromForeign(List,ARGS).
fixOptionsFromForeign([AName=AValue|List],[DName=DValue|ARGS]):-
          logOnFailure(decodeRequestAtom(AName,DName)),
          logOnFailure(decodeRequestAtom(AValue,DValue)),
          logOnFailure(fixOptionsFromForeign(List,ARGS)).
fixOptionsFromForeign([AName|List],[DName|ARGS]):-
          decodeRequestAtom(AName,DName),
          logOnFailure(fixOptionsFromForeign(List,ARGS)).

% transform_option(Agent,Channel)
transform_option(author,user).
transform_option(theory,opt_theory).
transform_option(language,interp).
transform_option(timeLimit,opt_timeout).
transform_option(bindingLimit,opt_answers_max).
transform_option(depthLimit,opt_backchains_max).



read_line_with_nl(Fd, Codes, Tail) :-
        get_code(Fd, C0),
        read_line_with_nl(C0, Fd, Codes, Tail).
read_line_with_nl(end_of_file, _, Tail, Tail) :- !.
read_line_with_nl(-1, _, Tail, Tail) :- !.
read_line_with_nl(10, _, [10|Tail], Tail) :- !.
read_line_with_nl(C, Fd, [C|T], Tail) :-
        get_code(Fd, C2),
        read_line_with_nl(C2, Fd, T, Tail).
% ===========================================================
% HTML
% ===========================================================

writeHTMLStdHeader(Title):-
   say('
   <html>
   <head>
   <meta http-equiv="Content-Language" content="en-us">
   <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
   <meta name="Keywords" content="PROLOG Artificial Intelligence Ontology AI MOO Linguistics Philosophy DARPA Doulgas Miles">
   <meta name="Description" content="PROLOG Artificial Intelligence Ontology AI MOO Linguistics Philosophy DARPA">
   <title>Inference Engine - ~w</title>
   </head>
   <body>
      <a href="browse.moo">Browse</a> 
	    <a href="english.moo">English</a>
	    <a href="cycl.moo">CycL</a>
	    <a href="subl.moo">SubL</a>
	    <a href="daml.moo">Daml</a>
	    <a href="wn.moo">WordNet</a>
	    <a href="microtheory.moo">Microtheory</a>
	    <a href="cycml.moo">CycML</a>
	    <a href="prolog.moo">Prolog</a>
	    <a href="settings.moo">Settings</a>
	    <a href="system.moo">System</a>
	    <a href="help.moo">Help</a>
	  <br><font size=+1>Create</font>
	   <a href="predicate.moo?create">Predicate</a>
	   <a href="function.moo?create">Function</a>
	   <a href="collection.moo?create">Collection</a>
	   <a href="microtheory.moo?create">Microtheory</a>
	  <br><font size=+1>Edit</font>
	   <a href="predicate.moo?edit">Predicate</a>
	   <a href="function.moo?edit">Function</a>
	   <a href="collection.moo?edit">Collection</a>
	   <a href="microtheory.moo?edit">Microtheory</a>
	  <br><font size=+1>Tests</font>
	   <a href="inference_tests.moo">Inference</a><br>
       <br><br><font size=+1 color=green><b>~w</b></font><br>
   ',[Title,Title]).

writeHTMLStdFooter:-
   say('
   </body>
   </html>',[X]).


 
clauseForVirtual(Predicate/Arity,Head):-functor(Head,Predicate,Arity),!.
clauseForVirtual(Predicate,Predicate).
clauseForVirtual(Predicate,Call):-not(atom(Predicate)),!.
clauseForVirtual(Term,saved(Data,Mt,Vars,List)):-saved(Data,Mt,Vars,List),once(memberchk(Term,List)).
clauseForVirtual(Predicate,Call):-Call=..[Predicate,_].
clauseForVirtual(Predicate,Call):-Call=..[Predicate,_,_].
clauseForVirtual(Predicate,Call):-Call=..[Predicate,_,_,_].
clauseForVirtual(Predicate,Call):-Call=..[Predicate,_,_,_,_].

getClauseDB(saved(Data,Mt,Vars,List),saved(Data,Mt,Vars,List)):-!.
getClauseDB(Head,imported_from(X,Head)):-predicate_property(Head,imported_from(X)).
getClauseDB(Head,implimentedInCode(Head)):-predicate_property(Head,built_in),!.
getClauseDB(Head,implimentedInCode(Head)):-predicate_property(Head,foreign),!.
getClauseDB(Head,entails(VirtualClauses,Head)):-clause(Head,VirtualClauses).


htmlListing(Predicate):-
      say('<pre>'),
      clauseForVirtual(Predicate,Head),
      getClauseDB(Head,VirtualClauses),
      writeHtml(VirtualClauses),fail.

htmlListing(Predicate):-
      make,
      say('~nEnd of Clauses with ~w</pre>',[Predicate]),!.

writeHtml(linkEach([])):-!.
writeHtml(linkEach([H|T])):-writeHtml(linkFor(H)),
      writeHtml(nl),writeHtml(linkEach(T)).
writeHtml(nl):-say('<br>').

writeHtml(linkFor(H)):-
	 my_www_form_encode(H,E),
	 format('<A href="browse.moo?find=~w">~w</A>',[E,H]).
%writeHtml((H:-T)):-!,writeHtml(prologEntails(T,H)).      
writeHtml(Clauses):-
        flag(indent,_,0),
      numbervars(Clauses),%trace,
      toMarkup(html,Clauses,[],O),!,
      format('~w~n',[O]).




writeHyperLink(NameFmt,NameArgs,UrlFmt,UlrArgs):-
      say('<a href="'),say(UrlFmt,UlrArgs),
      say('">'),say(NameFmt,NameArgs),say('</a>'),!.

writeCheckbox(Name,Text,Default):-
      getMooOption(Agent,Name=Default,Value),
      valueToCheckMark(Value,OnOff,More),
      say('<label for="~w"><input id="~w" type="checkbox" name="~w" value="~w" ~w>~w</label>',[Name,Name,Name,OnOff,More,Text]).

writeSpaces(N):-not(number(N)),!.
writeSpaces(N):-N<1,!.
writeSpaces(N):-say('&nbsp;'),NN is N-1,writeSpaces(NN),!.


valueToCheckMark(Value,'ON','CHECKED'):-memberchk(Value,['ON',yes,true,'T','True','Yes']).
valueToCheckMark(Value,'OFF',' ').

