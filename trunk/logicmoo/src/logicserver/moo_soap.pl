% ===========================================================
% NATIVE SOAPD SERVER FOR SWI-PROLOG 
% ===========================================================

:-include('moo_header.pl').

service_soapd_request(In,Out):-
	catch(read_do_soap(stream(In),Out),E,writeFmt(Out,'<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n<error>~w</error>\n',[E])),
	catch(flush_output(Out),_,true).


read_do_soap(Source):-
	open(Source,read,Stream),
	read_do_soap(Stream,user_output).

read_do_soap(Source,Out):-	 
%	thread_self(Self),
        write(Out,'<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n'),
%        writeFmt(Out,'<?xml version="1.0" encoding="ISO-8859-1"?>\n<answer thread="~w">\n',[Self]),
	catch(flush_output(Out),_,true),
	load_structure(Source,RDF,[]),
	structure_to_options(RDF,Options),
%	writeFmt(user_error,'structure="~q"\noptions="~q"\n',[RDF,Options]),
	catch(flush_output(user_error),_,true),
	moo_ua([client=soap|Options]).
	%writeFmt(Out,'</answer>\n',[]).


% request
structure_to_options([element(request, Options, [Atom])],[submit=ask,sf=Atom|Options]):-!.

% assert
structure_to_options([element(assert, Options, [Atom])],[submit=assert,sf=Atom|Options]):-!.
structure_to_options([element(asssertion, Options, [Atom])],[submit=assert,sf=Atom|Options]):-!.
structure_to_options([element(assertion, Options, [Atom])],[submit=assert,sf=Atom|Options]):-!.

% get inner
structure_to_options([element(Ptag, ['xmlns:moo'=Server], Inner)],[opt_server=Server,opt_outter=Ptag|Out]):-!,
	structure_to_options(Inner,Out).



% ===========================================================
% Insert
% ===========================================================
parse_moo_soap(Options):-memberchk(submit=assert,Options),!,
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_kb='PrologMOO',KB),
	getMooOption(sf=surf,Assertion),
	atom_codes(Assertion,Assertion_Chars),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	logOnFailure(getMooOption(tn=_,EXTID)),
	%sendNote(user,'Assert',formula(NEWFORM),'Ok.'). %,logOnFailure(saveMooCache)
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
	xml_assert(Show,Ctx,KB,User).

xml_assert(Show,Ctx,KB,User):-
        getSurfaceFromChars(Show,STERM,Vars),
        getMooTermFromSurface(STERM,NEWFORM),
	xml_assert(Show,NEWFORM,Vars,Ctx,KB,User).

xml_assert(Show,Ctx,KB,User):-!,
	writeFmt('<assertionResponse accepted="false">\nUnable to parse: "~s"\n</assertionResponse>\n',[Show]).
	
xml_assert(Show,NEWFORM,Vars,Ctx,KB,User):-
	logOnFailure(getTruthCheckResults(tell,[untrusted],surface,NEWFORM,Ctx,STN,KB,Vars,Maintainer,Result)),
	(Result=accept(_) -> 
			(
			once(invokeInsert([trusted,canonicalize,to_mem],surface,NEWFORM,Ctx,EXTID,KB,Vars,User)),
			write('<assertionResponse accepted="true">\nOk.\n</assertionResponse>\n')
			)
			;
			(
			Result=notice(FormatStr,Args),
			write('<assertionResponse accepted="false">\n'),
			writeFmt(FormatStr,Args),
			write('\n</assertionResponse>\n')
			)
	),!.

xml_assert(Show,NEWFORM,Vars,Ctx,KB,User):-!.
	

% ===========================================================
% Ask a Request
% ===========================================================
parse_moo_soap(Options):-memberchk(submit=ask,Options),!,make,
	%write('<!DOCTYPE moo:ask SYSTEM "/opt/tomcat-4.0/webapps/moo-1.4b1/dtd/java_prolog.dtd">\n'),
	write('<moo:ask xmlns:moo="http://localhost">\n'),
	getMooOption(opt_ctx_request='GlobalContext',Ctx),
	getMooOption(opt_kb='PrologMOO',KB),
	getMooOption(sf=surf,Askion),
	atom_codes(Askion,Askion_Chars),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
         logOnFailure(getCleanCharsWhitespaceProper(Askion_Chars,Show)),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
              logOnFailure(once(( NEWFORM=comment(_) -> 
                     (writeFmt('<error>Syntax Error: Unmatched parentheses in "~s"</error>\n',[Show]),!,FORM=_) ;(!,
		     logOnFailure(invokeRequest_xml(NEWFORM,ChaseVars,Ctx,TrackingAtom,KB,User,Vars,CPU))
		     )))),
	write('</moo:ask>\n').
	
invokeRequest_xml(NEWFORM,ChaseVars,Ctx,TrackingAtom,KB,User,Vars,CPU):-
	invokeRequestToBuffer(NEWFORM,ChaseVars,Ctx,TrackingAtom,KB,User,Vars,CPU),
	final_answer(Logic:How),
	invoke_final_answer(Logic,How,CPU).
	
invoke_final_answer(possible,How,CPU):-!,
	writeFmt('<requestResponse yesno="~w" numBindings="0" seconds="~w"/>\n',[How,CPU]).

invoke_final_answer(Logic,How,CPU):-
	writeFmt('<requestResponse yesno="~w" numBindings="~w" seconds="~w">\n<bindings>\n',[Logic,How,CPU]),
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
	writeFmt('<binding>\n',[]),
	inform_xml_vars(Result,Vars),
	length_explaination(InExplaination,InLength),
	findall(Length-Explaination,
		(retract(inform_xml_agent_buffer_db(_,Result,Explaination,_)),
		 length_explaination(Explaination,Length)
		 ),KeyList),
		
	keysort([(InLength-InExplaination)|KeyList],[(_-ChoiceExplaination)|_]),
	inform_xml_explaination(InLength,ChoiceExplaination,Result),
	writeFmt('</binding>\n',[]).
	
inform_xml_vars(Result,Vars):-
	length_var(Result,NumVar),
	writeFmt('<variables numVars="~w">\n',[NumVar]),
	inform_each_variable(Result,Vars),
	writeFmt('</variables>\n',[]).
	
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
	toMarkUp(kif,Name,Vars,OName),
	toMarkUp(kif,Value,Vars,OValue),
	writeFmt('<var varName="~w" value="~w"/>\n',[OName,OValue]).


inform_xml_explaination(InLength,ChoiceExplaination,Result):-
	writeFmt('<explaination numSteps="~w">',[InLength]),
	flag(explaination_linenumber,_,0),
	writeObject_explaination(ChoiceExplaination,Result),
	writeFmt('</explaination>\n').

writeObject_explaination(deduced,_).
writeObject_explaination('$VAR'(_),_).
writeObject_explaination(explaination(Choice1) ,Result):-!,
	writeObject_explaination(Choice1,Result),!.	
writeObject_explaination(Choice1 * Choice2 ,Result):-!,
	writeObject_explaination(Choice1,Result), !,
	writeObject_explaination(Choice2,Result),!.
writeObject_explaination(Choice1,Result):-!,
             write('<explainationStep isRule="true">\n<originalRule>\n'),
	     toMarkUp(html,Choice1,Result,Out),!,
	     ignore(write_escaped(Out)),
             write('\n</originalRule>\n</explainationStep>\n').

write_escaped([O|T]):-!,
	write_e_codes([O|T]),!.
write_escaped(Out):-atom(Out),!,
	atom_codes(Out,Codes),!,write_escaped(Codes),!.
write_escaped(String):-	!,
	string_to_atom(String,Atom),
	 atom_codes(Atom,Codes),!,
	write_e_codes(Codes),!.

write_e_codes([]):-!.
write_e_codes([E|Cs]):-!,
	write_e(E),!,
	write_e_codes(Cs),!.
write_e(34):-write('&qt;'),!.
write_e(60):-write('&lt;'),!.
write_e(62):-write('&gt;'),!.
write_e(C):-put_code(C),!.

