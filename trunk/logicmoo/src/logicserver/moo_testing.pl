/*
The defaults are shown here
opt_timeout=5  The request system will try for 5 seconds before stopping (RealNumber in seconds)
opt_answers_min=1 The system will at minumum try to come up with 1 answer
opt_answers_max=99 The system will at maximun will try to come up with 99 answers
opt_backchains_max=50 will try 50 prolog backchains aprocimately 12 authorial backchains on each deduction
opt_deductions_max=100 The system will try to come up with 100 non unique answers before stooping
*/ 

:-include('moo_header.pl').
												     
:-'ROOT_RT'(X),concat_atom([X,'/belief_test/'],O),assert(tq_input_dir(O)).


% =========================================================
% Run A Single TQ
% =========================================================
invokeTestSystem(Options):-
	retractall(last_tq(_)),assert(last_tq('TES')),	
	memberchk(cmd='runtest',Options),!,make,
	setMooOption(opt_ctx_assert='GlobalContext'),
	setMooOption(opt_kb='PrologMOO'),
	getMooOption(asid=_,AID),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	getMooOption(filename='axioms.tq',FileName),
	testTQFileWriteHTML(FileName,User,TimeRequired,Result,Title,Mods).

	
% ===========================================================
% Run TQ Entire Directory "Sub" using "KB" and "Ctx" then Send the Output to Stream "Dest"
% ===========================================================
invokeTestSystem(Options):-memberchk(submit='Run TQs',Options),!,make,
	setMooOption(opt_ctx_assert='GlobalContext'),
	setMooOption(opt_kb='PrologMOO'),
	getMooOption(asid=_,AID),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	getMooOption(tqDir='soundness',TQDIR),
	mooProcessCreate(ProcessName,run_tq_thread(TQDIR),Id,[]),!,
	invokeTestSystem([client='tqsystem']).

% ===========================================================
% Show a TEST DIRECTORY
% ===========================================================

invokeTestSystem(Options):-memberchk(cmd='tqpage',Options),!,make,
	setMooOption(opt_ctx_assert='GlobalContext'),
	setMooOption(opt_kb='PrologMOO'),
	getMooOption(asid=_,AID),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	getMooOption(tqDir='soundness',TQDIR),
	logOnFailure(test_prep_dir(TQDIR,TQOutputDir)),
	logOnFailure(concat_atom([TQOutputDir,'*.tq'],MASK)),
        logOnFailure(expand_file_name(MASK,TQS)),!,
	writeFmt('<table border=0>',[]),
	writeFmt('<th><b>View Test Source</b></th><th><b>Run Test</b></th><th></th><th><b>View Test Source</b></th><th><b>Run Test</b></th>',[]),
	logOnFailure(write_file_list(TQDIR,TQS,TQOutputDir)),
	writeFmt('</table>',[]).

% ====================================================
% Show main TQ Front page (fallback)
% ====================================================

invokeTestSystem(Options):-
	setMooOption(opt_ctx_assert='GlobalContext'),
	setMooOption(opt_kb='PrologMOO'),
	getMooOption(asid=_,AID),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	getMooOption(tqDir='soundness',TQDIR),
	expand_file_name('../../belief_test/*', Absolute),
	writeFmt('<table border=0 cellpadding=4>',[]),
	show_available_tqs2(Absolute,Paths),
	links_to_available_tqs(Paths),
	writeFmt('</table>',[]).


% ===================================================================
%  TQ BATCH INTERFACE
% ===================================================================
% This file runs the automated test suite over the deducenece questions

run_batch(Sub):-
         (unsetMooOption(client=html)),
         global_set('Tests Failed',0),
         global_set('Tests Passed',0),
         global_set('Tests Run',0),
         global_set('Tests Skipped',0),
         global_set('Tests Incomplete',0),
	 test_prep_dir(Sub,OutputDir),	    
         concat_atom([OutputDir,'index.html'],TestSummary),!,
         atom_concat(OutputDir,'*.tq',MASK),
         expand_file_name(MASK,TQS),!,
         Version='Byrd',
         getCputime(Start), 
         safe_file_open(TestSummary,'w',Dest),
         writeFmt(Dest,'<HTML><BODY background=/pixmaps/back.gif>\n<UL><H2>Moo Basic Belief Engine Test Questions</H2><BR>\n<H3><SCRIPT>document.writeln("Tests on version 1.01 ran " + document.lastModified); </SCRIPT><P></H3><BR>',[]),
         writeFmt(Dest,'<p><font color=green>Passed_Likey</font> = Passed but was not completely verified<br><font color=purple>Incomplete</font> = Passed but answers did notmatch verbatim (like decimal precision)<br><font color=green>Passed_Soundness</font> = Passed a soundness test due to unproven result<br><font color="#FF9900">Failed_Incomplete</font> = Failed due to lack of Explaination<br><font color="#0000FF">Syntax_Errors</font> = Test could not be accurately ran</p><p>' ,[]),
         writeFmt(Dest,'<TABLE><TR><TD  width=250>Test Source</TD><TD width=100>Test Result</TD><TD width=192>Request Time</TD><TD width=192>Time Total</TD></TR>',[]),
           findall(TQ,((
                        member(TQ,TQS), 
                        once(batch(Start,TQ,TQI,Dest,TQO))
                        )),_),
         writeFmt(Dest,'</TABLE>\n</BODY></HTML>',_),
         getCputime(End),    
         Elapsed is End - Start,
         writeFmt(Dest,'<P>\n',_),  
         summerize_dest(Dest,Version,NEWDIR,Elapsed),
         file_close(Dest).



:-assert(oddeven).

:-assert(last_tq('TES')).

:-dynamic(tq_ignored/0).
:-dynamic(tq_skipped/0).
%:-dynamic(tq_attempted_request/0).

batch(Start,FileName,INDIRRELATE,Dest,OUTDIR):- !,
         file_base_name(FileName,FileBaseName),
         concat_atom([OUTDIR,'/',FileBaseName,'.result.html'],ResultFile),
         file_base_name(ResultFile,BaseResultFile),
         getCputime(TS), 
         (
          (last_tq(LTQ),atom_codes(LTQ,[A,B,C|_]),not(atom_codes(FileBaseName,[_,_,C|_]))) -> 
            ((
          writeFmt(Dest,'</TABLE>',[]),
          atom_codes(TQN,[A,B,C]),
         Elapsed is TS - Start,
          summerize_dest(Dest,TQN,TQN,Elapsed),
          writeFmt(Dest,'<BR><TABLE><TR><TD  width=250>Test Source</TD><TD width=100>Test Result</TD><TD width=192>Request Time</TD><TD width=192>Time Total</TD></TR>',[])

          ))
          ; true
         ),
         ignore(test_batch_operation(FileName,'byrd',ResultFile,CPU,Result,Title)),
   
         getCputime(TE),
   (tq_ignored -> true ; ((
         (retract(oddeven) -> writeFmt(Dest,'<tr bgcolor=#CCCCEE> ',[]) ; (writeFmt(Dest,'<tr bgcolor=#FFFFFF> ',[]),assert(oddeven))),
         TSE is TE - TS,
         (Result = 'Syntax_Errors' -> ((DISP=blue, global_increment('Tests Skipped'))) ;
         (Result = 'Incomplete' -> ((DISP=purple, global_increment('Tests Incomplete'),global_increment('Tests Run'),global_increment('Tests Passed'))) ;
         (Result = 'Skipped' -> ((DISP=blue, global_increment('Tests Skipped'))) ;
   
         (atom_codes(Result,[80|_]) ->  
            (( DISP=green,global_increment('Tests Passed'),global_increment('Tests Run') ));
            (( DISP='orange', global_increment('Tests Failed'),global_increment('Tests Run') )))
         
         ))),
         once(( (CPU is 0) ->  CPUDISP=' &lt; 2ms'  ; CPUDISP = CPU )),!,
         writeFmt(Dest,'<TD><a href="~w">~w&nbsp;</a></TD><TD><a href="~w"><font color=~w>~w</font></a></TD><TD>~w</TD><TD>~w secs </TD></TR> \n',[FileBaseName,Title,BaseResultFile,DISP,Result,CPUDISP,TSE])
        ))
   ),      retractall(last_tq(_)),
            assert(last_tq(FileBaseName)),
            flush_output(Dest).


summerize_dest(Dest,Comment,NEWDIR,Elapsed):- 
         catch((
                flag('Tests Failed',TotalFailed,TotalFailed),
                flag('Tests Passed',TotalPassed,TotalPassed),
                flag('Tests Run',TotalRan,TotalRan),
                flag('Tests Skipped',Skipped,Skipped),
                flag('Tests Incomplete',Incomplete,Incomplete),
         Percent is TotalPassed/TotalRan * 100,                                                     
         PercentComplete is ((TotalPassed - Incomplete )/TotalRan * 100),!,                                                     
         writeFmt(Dest,'<UL><Strong>Succesfully <Font color=green>Passed</Font> ~w and <Font color=orange>Failed</Font> ~w but <Font color=blue>Skipped</Font> ~w ',[TotalPassed,TotalFailed,Skipped]),
         writeFmt(Dest,'Test Questions (Total Counted ~w) giving us <font size="+1" color=purple>Passed ~w%</font> with <font size="+1" color=green>Absolutely ~w% </font> in ~w seconds.&nbsp;&nbsp;<BR>\n</Strong></UL>\n',[TotalRan,Percent,PercentComplete,Elapsed])
              ),_,true).


batch_answer(H):-var(H),!,batch_answer_proc(H).
batch_answer([]):-!.
batch_answer([H|T]):-
      once(batch_answer_proc(H)),
      batch_answer(T).

batch_answer_proc(H):-var(H),!,assert(tq_least_one_answer),sendNote(user,tqTester,'solution found','Found (tq_least_one_answer):'(var)),!,sendNote('(??)').
batch_answer_proc(H):-H=..[list|HL],!,batch_answer(HL).
batch_answer_proc(H):-H=..['List'|HL],!,batch_answer(HL).
batch_answer_proc(H):-number(H),number_found(H),!,assert(tq_least_one_answer),sendNote(user,tqTester,'solution found','Found:'(formula(H))),!.
batch_answer_proc(H):-nonvar(H),answer_found(H),!,assert(tq_least_one_answer),sendNote(user,tqTester,'solution found','Found:'(formula(H))),!.
batch_answer_proc(H):-number(H),assert(tq_missed_one_answer),sendNote(user,tqTester,'solution not found','Missing:'(formula(H))),sendNote('(numeric)').
batch_answer_proc(exists(V,F)):-assert(tq_missed_one_answer),sendNote(user,tqTester,'solution not found','Missing:'(exists(V,F))),!,sendNote('(exists/prop)').
batch_answer_proc(H):-compound(H),assert(tq_missed_one_answer),sendNote(user,tqTester,'solution not found','Missing:'(formula(H))),!,sendNote('(proposition)').
batch_answer_proc(H):-assert(tq_missed_one_answer),sendNote(user,tqTester,'solution not found','Missing:'(formula(H))),!.


number_found(H):-HN is H *1.0 , number_codes(HN,Codes),answer_found(Codes),sendNote('(numeric)').


write_file_list(TQDIR,[],TQOutputDir):- !.
write_file_list(TQDIR,[T1,T2|QDIR],TQOutputDir):-
	writeFmt('<tr>',[]),
	write_file_list_0(TQDIR,T1,TQOutputDir),
	writeFmt('<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>',[]),
	write_file_list_0(TQDIR,T2,TQOutputDir),
	writeFmt('</tr>',[]),
	write_file_list(TQDIR,QDIR,TQOutputDir).
write_file_list(TQDIR,[T|QDIR],TQOutputDir):-
	writeFmt('<tr>',[]),
	write_file_list_0(TQDIR,T,TQOutputDir),
	writeFmt('</tr>',[]),
	write_file_list(TQDIR,QDIR,TQOutputDir).
	
write_file_list_0(TQDIR,T,TQOutputDir):-
	file_base_name(T, BaseName),
	concat_atom(PathList,'\\', T), 
	concat_atom(PathList,'/', TO),
	writeFmt('<td><A href="belief_test/~w/~w">~w</A></td><td><A href="tests.jsp?cmd=runtest&filename=~w"><font color=green><b>Run Now</b></font></A></td>\n',[TQDIR,BaseName,BaseName,TO]).


run_tq_thread(TQDIR):-running_tq(_),!.
run_tq_thread(TQDIR):-
			assert(running_tq(TQDIR)),!,
			thread_at_exit(retractall(running_tq(TQDIR))),
			logOnFailure(test_prep_dir(TQDIR,TQOutputDir)),!,
			concat_atom([TQOutputDir,'index.html'],TestSummary),!,
			safe_file_open(TestSummary,'w',Dest),!,
			thread_at_exit(close(Dest)),
			writeFmt(Dest,'<HTML><BODY>',[]),
			run_batch_web(Dest,KB,Ctx,TQDIR),!,
			writeFmt(Dest,'</BODY></HTML>',[]),
			retractall(running_tq(TQDIR)),
			ignore(catch(close(Dest),_,true)),
			ignore(thread_exit(completed_tq(KB,Ctx,TQDIR))
			).
	

links_to_available_tqs([]).
links_to_available_tqs([P|ATHS]):-
	links_to_available_tqs0(P),
	links_to_available_tqs(ATHS),!.

links_to_available_tqs0(P):-
	writeFmt('<tr><td><font size=+1>~w<font></td>',[P]),
	concat_atom(['../../belief_test/',P],Dir),
	(running_tq(_) -> 
		(writeFmt('<td> waiting </td>',[]),
			(running_tq(P) -> 
				(
				writeFmt('<td><A href="belief_test/~w/"><font color=green><i>Currenty Running</i></font></A></td>',[P,P])
				)
				;
				writeFmt('<td> waiting </td>',[])
			) 
		)
		;
		(
		writeFmt('<td> <A href="tests.jsp?cmd=tqpage&tqDir=~w">Live</A></td>', [P]),
		writeFmt('<td><A href="tests.jsp?submit=Run TQs&tqDir=~w">Batch Process</a></td>',[P,P])
		)
	), 
	(exists_directory(Dir) -> 
		writeFmt('<td><P><A href="belief_test/~w/">View</a></td>',[P])
		;
		writeFmt('<td>&nbsp;</td>')
	),
	writeFmt('</tr>',[]).
	
	
show_available_tqs2([],[]).
show_available_tqs2([H|T],TT):-
	file_base_name(H,'CVS'),!,
	show_available_tqs2(T,TT).
show_available_tqs2([H|T],TT):-
	file_base_name(H,'index.html'),!,
	show_available_tqs2(T,TT).

/*

show_available_tqs2([H|T],TT):-
	once((file_base_name(H,Base),
	file_name_extension(Base,Ext,Name))),not((Ext='tq' ; Ext='TQ')),!,
	show_available_tqs2(T,TT).
*/
show_available_tqs2([H|T],[HH|TT]):-
	file_base_name(H,HH),
	show_available_tqs2(T,TT).

:-dynamic(running_tq/1).



test_prep_dir(Sub,ODIRSub):-
	 tq_input_dir(ODIR),
	 concat_atom([ODIR,Sub,'/'],ODIRSub),
	 prolog_to_os_filename(ODIRSub,TQOS),
	 writeFmt(user_error,'~q.\n',[prolog_to_os_filename(ODIRSub,TQOS)]),!.
	 	
run_batch_web(Dest,KB,Ctx,Sub):-
         logOnFailure((
	 flag('Tests Failed',_,0),
         flag('Tests Passed',_,0),
         flag('Tests Run',_,0),
         flag('Tests Skipped',_,0),
         flag('Tests Incomplete',_,0)
	 )),
	 logOnFailure(test_prep_dir(Sub,TQOutputDir)),
         logOnFailure(concat_atom([TQOutputDir,'*.tq'],MASK)),
         logOnFailure(expand_file_name(MASK,TQS)),!,
         getCputime(Start), 
	 logOnFailure(run_batch_file_list(Sub,Dest,TQS,TQOutputDir)),
         catch((getCputime(End),    
         Elapsed is End - Start,
	 flush_output(Dest),
         summerize_dest_web(Dest,'Byrd',NEWDIR,Elapsed)),_,true).

summerize_dest_web(Dest,Comment,NEWDIR,Elapsed):- 
         catch((
                flag('Tests Failed',TotalFailed,TotalFailed),
                flag('Tests Passed',TotalPassed,TotalPassed),
                flag('Tests Run',TotalRan,TotalRan),
                flag('Tests Skipped',Skipped,Skipped),
                flag('Tests Incomplete',Incomplete,Incomplete),
         Percent is TotalPassed/TotalRan * 100,                                                     
         PercentComplete is ((TotalPassed - Incomplete )/TotalRan * 100),!,                                                     
         writeFmt(Dest,'<UL><Strong>Succesfully <Font color=green>Passed</Font> ~w and <Font color=orange>Failed</Font> ~w but <Font color=blue>Skipped</Font> ~w ',[TotalPassed,TotalFailed,Skipped]),
         writeFmt(Dest,'Test Questions (Total Counted ~w) giving us <font size="+1" color=purple>Passed ~w%</font> with <font size="+1" color=green>Absolutely ~w% </font> in ~w seconds.&nbsp;&nbsp;<BR>\n</Strong></UL>\n',[TotalRan,Percent,PercentComplete,Elapsed])
              ),_,true).


run_batch_file_list(Type,Dest,[],TQOutputDir):-!,sendNote(user,tqTester,'Nothing to do','No *.tq files found in test dirrectory '(Type)),!.
run_batch_file_list(Type,Dest,TQS,TQOutputDir):-!,
	 getPrettyDateTime(DateTime),
	 writeFmt(Dest,'<UL><H2>Moo Belief Engine Test Question Results</H2><BR>\n<H3>Ran at: <B>~w</B>\n<P></H3><BR>',[DateTime]),
         writeFmt(Dest,'<p><font color=green>Passed</font> = Definately and Deterministicly Passed<br><font color=green>Passed_Likey</font> = Passed but was not completely verified<br><font color=purple>Incomplete</font> = Passed but answers did notmatch verbatim (like decimal precision)<br><font color=green>Passed_Soundness</font> = Passed a soundness test due to unproven result<br><font color="#FF9900">Failed_Incomplete</font> = Failed due to lack of Explaination<br><font color="#0000FF">Syntax_Errors</font> = Test could not be accurately ran</p><p>' ,[]),
        % writeFmt(Dest,'<TABLE><TR><TD  width=250>Test Source</TD><TD width=100>Test Result</TD><TD width=192>Request Time</TD><TD width=192>Time Total</TD></TR>',[]),
	 flush_output(Dest),
	 run_batch_file_list1(Type,Dest,TQS,TQOutputDir),!.
	 
run_batch_file_list1(Type,Dest,[],TQOutputDir):-!,writeFmt(Dest,'</TABLE>\n',_),!.

run_batch_file_list1(Type,Dest,[FileName|TQS],TQOutputDir):-  % Iterate over each file
	run_batch_file_list_item(Type,Dest,FileName,TQOutputDir),!,
	run_batch_file_list1(Type,Dest,TQS,TQOutputDir),!.

run_batch_file_list_item(Type,Dest,FileName,TQOutputDir):-
	writeFmt(user_error,'Testing ~w ~w.\n',[Type,FileName]),!,
	 file_base_name(FileName,FileBaseName),
	 getCputime(TimeStart), 
	 (   % If/Then/Else (Make block Of TQs)
	     (last_tq(LTQ),atom_codes(LTQ,[A,B,C|_]),not(atom_codes(FileBaseName,[_,_,C|_])))
	            -> 
			    ((
			  writeFmt(Dest,'</TABLE>',[]),
			  atom_codes(TQN,[A,B,C]),
			  getCputime(Now),
			  Elapsed is TimeStart - Now,
			  summerize_dest_web(Dest,TQN,TQN,Elapsed),
			  writeFmt(Dest,'<BR><TABLE><TR><TD  width=250>Test Source</TD><TD width=100>Test Result</TD><TD width=192>Request Time</TD><TD width=192>Predicates Used</TD></TR>',[])
			  ))
		  ; true
	  ),
	 concat_atom([TQOutputDir,'/',FileBaseName,'.result.html'],ResultFile),
	 file_base_name(ResultFile,BaseResultFile),
	 tell(ResultFile),
	 once(logOnFailure(testTQFileWriteHTML(FileName,'WebUser',CPU,Result,Title,Mods))),
	 once(memo_result(Result)),

	 told,
	 % Record Result
	 getCputime(TimeElapsed),
	 TotalTime is TimeElapsed - TimeStart,
	  (tq_ignored -> true ;  
		 ((
			 (retract(oddeven) -> writeFmt(Dest,'<tr bgcolor=#CCCCEE> ',[]) ; (writeFmt(Dest,'<tr bgcolor=#FFFFFF> ',[]),assert(oddeven))),
			 color_result(Result,ResultColor),!,
			 once(( catch((CPU is 0),_,fail) ->  ColorTime=' &lt;  2ms'  ; ColorTime = CPU )),!,
			 writeFmt(Dest,'<TD><a href="~w">~w&nbsp;</a></TD><TD><a href="~w"><font color=~w>~w</font></a></TD><TD>~w</TD><TD>~w</TD></TR> \n',[FileBaseName,Title,BaseResultFile,ResultColor,Result,ColorTime,Mods])
		  ))
	   ),
	retractall(last_tq(_)),
	assert(last_tq(FileBaseName)),
	flush_output(Dest).			

color_result('Skipped',blue):-!. 
color_result('Syntax_Errors',blue):-!. 
color_result('Incomplete',purple):-!.
color_result('Passed',green):-!.
color_result('Passed_Likely',green):-!.
color_result(_,orange):-!.
			
memo_result('Skipped') :-global_increment('Tests Skipped'),!.
memo_result('Syntax_Errors') :-global_increment('Tests Skipped'),!.
memo_result('Incomplete') :-global_increment('Tests Incomplete'),global_increment('Tests Run'),global_increment('Tests Failed'),!,write('<pre>\n'),dump_table,write('</pre>\n').
memo_result(Result) :-atom_codes(Result,[80|_]) ,!,global_increment('Tests Passed'),global_increment('Tests Run'),!.
memo_result(Result) :-atom_codes(Result,[70|_]) ,!,global_increment('Tests Failed'),global_increment('Tests Run'),!,write('<pre>\n'),dump_table,write('</pre>\n').
memo_result(Result) :-writeDebug(unscored(Result)),!.
memo_result(Result) .

dump_table:-
	listing_template(mooCache(tabling,_,_)),
	listing_template(mooCache(found,_,_)),
	listing_template(mooCache(failed,_,_)).

% ========================================================================================
% Run Batch File "FileName" to HTML Output "ResultFilename"  as "User" and return "TimeRequired","Result" and "Title"
% ========================================================================================
:-dynamic(request_time/1).

:-dynamic(debug_tests).

testTQFileWriteHTML(FileName,User,TimeRequired,Result,Title,Mods):-debug_tests,!,
	 file_base_name(FileName,FileBaseName),
	writeModeSet(html),!,
	 getPrettyDateTime(DateTime),
         writeFmt('<HTML><HEAD><TITLE>~w</TITLE></HEAD><BODY>Mode: Belief-201<BR>Version: SWI-Prolog 4.0.10<P>Filename:<A href="~w">~w</A>\n\nRan: <B>~w</B>\n<pre>',
	 [FileBaseName,FileBaseName,FileBaseName,DateTime]), 
	setMooOption((opt_timeout=120)), %2 minutes
	setMooOption((opt_debug=on)),
	testTQFileWriteHTML0(FileName,User,TimeRequired,Result,Title,Mods),
         writeFmt('</pre></BODY><HTML>\n',[]).

testTQFileWriteHTML(FileName,User,TimeRequired,Result,Title,Mods):-
	 file_base_name(FileName,FileBaseName),
	writeModeSet(html),!,
	 getPrettyDateTime(DateTime),
         writeFmt('<HTML><HEAD><TITLE>~w</TITLE></HEAD><BODY>Mode: Belief-201<BR>Version: SWI-Prolog 4.0.10<P>Filename:<A href="~w">~w</A>\n\nRan: <B>~w</B>\n<pre>',
	 [FileBaseName,FileBaseName,FileBaseName,DateTime]), 
	setMooOption((opt_timeout=600)), %10 minutes
	setMooOption((opt_debug=off)),

	testTQFileWriteHTML0(FileName,User,TimeRequired1,Result1,Title1,Mods1),
	once(((Result1='Passed',TimeRequired=TimeRequired1,Result=Result1,Title=Title1,Mods=Mods1);
		((
		writeFmt('\n<h3><font color=red>Retaking again with debug on and 45 seconds of search</font></h3>\n',[]),
	setMooOption((opt_timeout=45)),	% 45 seconds
	setMooOption((opt_debug=on)),
		testTQFileWriteHTML0(FileName,User,TimeRequired,Result,Title,Mods)
		)))),
         writeFmt('</pre></BODY><HTML>\n',[]).

		
	
testTQFileWriteHTML0(FileName,User,TimeRequired,Result,Title,Mods):- make,    
	file_directory_name(FileName,Dir),ignore(catch(cd(Dir),_,true)),
	 file_base_name(FileName,FileBaseName),
	%writeFmt(user_error,'~q.\n',[Dir + FileBaseName = FileName]),
         flag('Axioms Compiled',_,0),
	 logOnFailure(retractall(request_time(_))),
	 logOnFailure(retractall(answer_found(_))),
         logOnFailure(retractall(answer_found_t(_))),
         logOnFailure(retractall(tq_attempted_request)),	 
         retractall(tq_missed_one_answer),
         retractall(tq_least_one_answer),
         retractall(tq_ignored),
         retractall(tq_skipped),
         logOnFailure(safe_file_open(FileName,'r',INPUT)), 
         repeat,    		 
		writeModeSet(html),
	         once(source_from_stream(INPUT,Chars,Formula,Vars)),nonvar(Formula),
		  logOnFailure(isMooOption(opt_ctx_assert=Ctx)),
		  logOnFailure(isMooOption(opt_kb=KB)),
		  %logOnFailure(once(toMarkUp(html,formula(Formula),Vars,Out))),writeFmt('<hr>~w<br>',[Out]),
                   logOnFailure(once(invokeOperation(verbose,Formula,Ctx,Tracking,KB,User,Vars))),
         at_end_of_stream(INPUT),!,
         file_close(INPUT),
         evaluate_result(Result),!,
         create_title(FileName,Title),
	 create_mods(FileName,Mods),
	 logOnFailure((
	 ignore(retract(request_time(TimeRequired))),
         ignore((TimeRequired='&lt; 2ms')) ,
         ignore((Result='Passed')),
         ignore((Title=FileName)),
	 writeFmt('<hr><P><P>CPUTIME = <B>~w</b>',[TimeRequired]),
	 color_result(Result,Color),
	 writeFmt('<br>RESULT = <B><font color=~w>~w</font></b>',[Color,Result])
	 )).
	 	 
create_title(FileName,Title):-
	 sandbox((findall(T,retract(title(T)),TTitle),flatten(TTitle,FlatTitle),concat_atom(FlatTitle,' ',Title))).
create_title(FileName,FileName):-!.

create_mods(FileName,Title):-
	 sandbox((findall(T,retract(mods(T)),TTitle),flatten(TTitle,FlatTitle),concat_atom(FlatTitle,' ',Title))).
create_mods(FileName,'.full.'):-!.

evaluate_result('Skipped'):- tq_skipped,!.
evaluate_result('Syntax_Errors'):-not(tq_attempted_request),!.
evaluate_result('Incomplete'):- tq_missed_one_answer,tq_least_one_answer,!.
evaluate_result('Failed_Incomplete'):- once(tq_missed_one_answer),not(tq_least_one_answer),!.
evaluate_result('Passed'):- once(tq_least_one_answer), not(tq_missed_one_answer),!.
evaluate_result('Passed_Likely'):- once(tq_attempted_request), not(tq_missed_one_answer),!.
evaluate_result('Passed_Soundness'):- tq_least_one_answer,!.
evaluate_result('Syntax_Errors'):-!.


         
% TODO
%%retain_answer(X):-!.
retain_answer(Var):-var(Var),!.
retain_answer(:):-!.
retain_answer((_=X)):-!,retain_answer(X),!.
retain_answer((_:_:X^_G33048)):-!,retain_answer(X),!.
retain_answer(A * B):-!,retain_answer(A),retain_answer(B),!.
retain_answer(X):-is_list(X),!,retain_answer_list(X).
retain_answer(X):-compound(X),!,ra(X),X=..XX,retain_answer_list(XX).
retain_answer(X):-number(X),!,HX is X * 1.0,number_codes(HX,Codes),ra(Codes),!. 
retain_answer(X):-ra(X),!. %,sendNote(debug,tqTester,answer_found(X),' ').

ra(X):-%writeDebug(retained(X)),
 catch(asserta(answer_found(X)),_,true).

retain_answer_list(V):-isSlot(V),!.
retain_answer_list([]).
retain_answer_list([H|T]):-!,
         retain_answer(H),!,
         retain_answer_list(T).     
retain_answer_list(_).
	 
                   


parse_moo_lf(Options):-memberchk(submit='Negated',Options),!,make,
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_kb='PrologMOO',KB),
	getMooOption(sf=surf,Assertion),
	atom_codes(Assertion,Assertion_Chars),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	logOnFailure(getMooOption(tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
	getNegationForm(NEWFORM,Vars,Output,Paths),
	writeObject('$spacer',Vars),
	writeObject(Output,Vars),
	writeObject('$spacer',Vars),
	writeObject('Paths '(Paths),Vars),!.

parse_moo_lf(Options):-memberchk(submit='Conjunctive',Options),!,make,
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_kb='PrologMOO',KB),
	getMooOption(sf=surf,Assertion),
	atom_codes(Assertion,Assertion_Chars),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	logOnFailure(getMooOption(tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
	getNegationForm(NEWFORM,Vars,NNF,Paths),
	getConjNF(NNF,Output),
	writeObject('$spacer',Vars),
	writeObject(Output,Vars).

parse_moo_lf(Options):-memberchk(submit='Disjunctive',Options),!,make,
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_kb='PrologMOO',KB),
	getMooOption(sf=surf,Assertion),
	atom_codes(Assertion,Assertion_Chars),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	logOnFailure(getMooOption(tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
	getNegationForm(NEWFORM,Vars,NNF,Paths),
	getDisjForm(NNF,Output),
	writeObject('$spacer',Vars),
	writeObject(Output,Vars).
	

parse_moo_lf(Options):-
	(memberchk(submit='Entailment',Options);memberchk(submit='Given',Options)),
	!,make,
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_kb='PrologMOO',KB),
	getMooOption(sf=surf,Assertion),
	atom_codes(Assertion,Assertion_Chars),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	logOnFailure(getMooOption(tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
	getAssertionClauses(PreQ,KB,Ctx,NEWFORM,Output,Vars,T),
	writeObject_conj(Output,Vars),
	writeObject('$spacer',Vars),
	writeObject('Paths '(T),Vars),!.

parse_moo_lf(Options):-
	memberchk(submit='True',Options),!,make,
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_kb='PrologMOO',KB),
	getMooOption(sf=surf,Assertion),
	atom_codes(Assertion,Assertion_Chars),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	logOnFailure(getMooOption(tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
	getAssertionClauses(PreQ,KB,Ctx,NEWFORM,Output,Vars,_),
	writeObject('$spacer',Vars),
	writeObject(Output,Vars).

parse_moo_lf(Options):-
	memberchk(submit='False',Options),!,make,
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_kb='PrologMOO',KB),
	getMooOption(sf=surf,Assertion),
	atom_codes(Assertion,Assertion_Chars),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	logOnFailure(getMooOption(tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
	getAssertionClauses(PreQ,KB,Ctx,not(NEWFORM),Output,Vars,_),
	writeObject('$spacer',Vars),
	writeObject(Output,Vars).
	

parse_moo_lf(Options):-
	memberchk(submit='Possible',Options),!,make,
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_kb='PrologMOO',KB),
	getMooOption(sf=surf,Assertion),
	atom_codes(Assertion,Assertion_Chars),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	logOnFailure(getMooOption(tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
	getAssertionClauses(PreQ,KB,Ctx,possible(NEWFORM),Output,Vars,_),
	writeObject('$spacer',Vars),
	writeObject_conj(Output,Vars).

parse_moo_lf(Options):-
	memberchk(submit='Impossible',Options),!,make,
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_kb='PrologMOO',KB),
	getMooOption(sf=surf,Assertion),
	atom_codes(Assertion,Assertion_Chars),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	logOnFailure(getMooOption(tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
	getAssertionClauses(PreQ,KB,Ctx,not(possible(NEWFORM)),Output,Vars,_),
	writeObject('$spacer',Vars),
	writeObject_conj(Output,Vars).

parse_moo_lf(Options):-
	memberchk(submit='All Forms',Options),
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_kb='PrologMOO',_KB),
	getMooOption(data=instance,TN),!,
	writeKnownFormsTN(Ctx,KB:TN).

parse_moo_lf(Options):-
	memberchk(submit='mooCache',Options),
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_kb='PrologMOO',_KB),
	getMooOption(data=instance,TN),!,
	listing_template(mooCache(_,_,_,_,_,_,TN,_,_)),
	listing_template(mooCache(PredR,Fact,Type,_,KB,Agent,(surf(KB,TN) ))),
	listing_template(mooCache(PredR,Fact,Pre,Type,_,KB,Agent,(surf(KB,TN) * via(_,Vars)))),
	!.

	

/*
mkm:-
	tell(mkm),
	show_relations,
	told.

show_relations:-
	retractall(shown_rel(_,_)),fail.
	
show_relations:-
	mooCache(R, surface, Axiom, Vars, KBName, Context, Tracking, User, Status),
	show_relations(R),fail.

show_relations:-
	mooCache(R, Axiom, Vars, KBName, Context, Tracking, User, Status),
	show_relations(R),fail.

show_relations:-
	mooCache(R, Axiom, Vars, KBName, Context, Tracking, User),
	show_relations(R),fail.
	
show_relations:-!.

mk_length(R,A,P):-
	getArity(R,A),!,
	length(L,A),
	numbervars(L,'$VAR',0,_),
	P=..[R|L].

getArity(R,A):-
	mooCache(valence, surface, valence(R,A), Vars, KBName, Context, Tracking, User, Status).
getArity(R,A):-
	mooCache(R, surface, P, Vars, KBName, Context, Tracking, User, Status),
	functor(P,R,A).
getArity(R,A):-
	mooCache(R, GAF, Vars, KBName, Context, Tracking, User),
	functor(GAF,holds,HF),
	A is HF-1,!.
	
getArity(R,2):-!.

show_relations(R):-
	mk_length(R,A,P),!,
	show_relations(R,A,P).

show_relations(R,A,P):-	shown_rel(R,A),!.
show_relations(R,A,P):-	
	assert(shown_rel(R,A)),
	format(
'% ~q/~w
mapping_nt(~q,undefined).

',[R,A,P]),!.
	

*/


%%% Theorem proving examples from
%%%   Chang, C.L. and R.C.T. Lee.
%%%   Symbolic Logic and Mechanical Theorem Proving.
%%%   Academic Press, New York, 1973, pp. 298-305.

%%% Note that the search driver predicate
%%% can be invoked by search(request) as in
%%% chang_lee_example8 or can be explicitly
%%% included in the request as in chang_lee_example2.

chang_lee_example2 :-
        nl,
        write(chang_lee_example2),
        pttp((
                p(e,X,X),
                p(X,e,X),
                p(X,X,e),
                p(a,b,c),
                (p(U,Z,W) :- p(X,Y,U) , p(Y,Z,V) , p(X,V,W)),
                (p(X,V,W) :- p(X,Y,U) , p(Y,Z,V) , p(U,Z,W)),
                (request :- search(p(b,a,c)))
        )),
        time(request).

chang_lee_example8 :-
        nl,
        write(chang_lee_example8),
        pttp((
                l(1,a),
                d(X,X),
                (p(X) ; d(g(X),X)),
                (p(X) ; l(1,g(X))),
                (p(X) ; l(g(X),X)),
                (not_p(X) ; not_d(X,a)),
                (not_d(X,Y) ; not_d(Y,Z) ; d(X,Z)),
                (not_l(1,X) ; not_l(X,a) ; p(f(X))),
                (not_l(1,X) ; not_l(X,a) ; d(f(X),X)),
                (request :- (p(X) , d(X,a)))
        )),
        time(search(request)).


kifnnf_pnnf(PNNF,KRVars,PNNF):-var(PNNF).

kifnnf_pnnf('$VAR'(N),KRVars,'$VAR'(N)):-number(N),!.

kifnnf_pnnf('$VAR'(N),KRVars,PrologNNF):-!,
	kifnnf_pnnf(N,KRVars,PrologNNF),!.

kifnnf_pnnf(PNNF,KRVars,PNNF):-not(compound(PNNF)),!.

kifnnf_pnnf([A|B],KRVars,[AA|BB]):-!,
	kifnnf_pnnf(A,KRVars,AA),
	kifnnf_pnnf(B,KRVars,BB).

kifnnf_pnnf(and(A,B),KRVars,(AA,BB)):-!,
	kifnnf_pnnf(A,KRVars,AA),
	kifnnf_pnnf(B,KRVars,BB).

kifnnf_pnnf(or(A,B),KRVars,(AA;BB)):-!,
	kifnnf_pnnf(A,KRVars,AA),
	kifnnf_pnnf(B,KRVars,BB).

kifnnf_pnnf(not(A),KRVars,AA):-!,
	kifnnf_pnnf(A,KRVars,AM),!,
	AM=..[F|Args],
	atom_concat('not_',F,NF),
	AA=..[NF|Args],!.

kifnnf_pnnf(Term,KRVars,PTerm):-!,
	Term=..List,
	kifnnf_pnnf(List,KRVars,PList),!,
	PTerm=..PList.
	



ts1:- tsurf(equal(X30, Y30)=>forall(Z30, attribute(X30, Z30)<=>attribute(Y30, Z30)),['THING1'=X30, 'THING2'=Y30, 'ATTR'=Z30]).

ts2:- tsurf(equal('AbstractionFn'(T1), U1)
	<=>forall(V1, instance(V1, T1)
		<=>attribute(V1, U1)),['CLASS'=T1, 'ATTR'=U1, 'INST'=V1]).

ts3:- tsurf((equal('MaxFn'(T27, U27), V27)=>equal(V27, T27)and greaterThan(T27, U27)or equal(V27, U27)and greaterThan(U27, T27)or equal(V27, T27)and equal(V27, U27)),['NUMBER1'=T27, 'NUMBER2'=U27, 'NUMBER'=V27]).

ts4:- tsurf(identityElement(S9, T9)=>forall(U9, instance(U9, 'DomainFn'(S9))=>equal('AssignmentFn'(S9, T9, U9), U9)), ['FUNCTION'=S9, 'ID'=T9, 'INST'=U9]).

ts5:- tsurf(instance(Z12, 'Integer')=>equal(Z12, 'PredecessorFn'('SuccessorFn'(Z12))),['INT'=Z12]).

ts6:- tsurf(inverse(N24, O24)=>forall(P24, forall(Q24, holds(N24, Q24, P24)<=>holds(O24, P24, Q24))),['REL1'=N24, 'REL2'=O24, 'INST1'=Q24, 'INST2'=P24]).

ts7:- tsurf(instance(H34, 'RangeFn'(I34))<=>exists(J34, holds(I34, J34, H34)),['INST1'=H34, 'REL'=I34, 'INST2'=J34]).

ts8:- tsurf((instance(A, 'ClassOfFn'(A))=>instance('ClassOfFn'(A),'Class')),['INST'=A]).

ts9:- tsurf((instance(A, 'Person') and attribute(A,'Male') => member(A,'ElksClubOrg')),['INST'=A]).

ts10:- tsurf((instance(A, 'Male')  => member(A,'ElksClubOrg')),['INST'=A]).




tsurf10:-tsurf( equal('PlusFn'(A,B),C), [ 'PHYS' =A , 'PHYS1' = B, 'PHYS2' = C |_]).
tsurf10a:-tsurf( equal('PlusFn'(A,A),'MultFn'(A,2)), [ 'Number' =A|_]).

tsurf15:-tsurf(
	forall(X10, forall(Y10, forall(Z10, forall(A11, forall(B11, 
		not exists(X10, exists(Y10, instance(Y10, 'CutSetFn'(Z10))
			and instance(X10, 'MinimalCutSetFn'(Z10))and pathLength(Y10, A11)
			and pathLength(X10, B11)and lessThan(A11, B11)))))))),
		 
	['PATH1'=Y10, 'PATH2'=X10, 'GRAPH'=Z10, 'NUMBER1'=A11, 'NUMBER2'=B11|C11]). 


sct:-tsurf(=>(and(instance(A, C), subclass(C, B)), instance(A, B)),['PATH1'=B, 'PATH2'=C, 'GRAPH'=A|_]).


tsurf1:-tsurf(forall(A,p(A)),['ADVERT'=A|_]),!.
tsurf2:-tsurf(exists(A,p(A)),['ADVERT'=A|_]),!.
tsurf3:-tsurf(exists(A,p(A)),['ADVERT'=A|_]),!.
tsurf4:-tsurf(=>(instance(A, 'Advertisement'),  selling(A)),['ADVERT'=A|D]).

tsurf5:-tsurf(
	=>(
		instance(A, 'Advertisement'),
		 exists(B, selling(A,B))
		 ),['ADVERT'=A, 'OBJ'=B, 'SALE'=C|D]).


tsurf11:-tsurf(=>(attribute(A, 'NuclearSuperPower'), exists(B, exists(C, exists(D, exists(E, and(superficialPart(E, A), and(superficialPart(D, A), and(attribute(E, C), and(attribute(D, B), and(instance(C, 'ColorProperty'), and(instance(B, 'ColorProperty'), not(equal(C, B))))))))))))), ['OBJ'=A, 'PART1'=E, 'PART2'=D, 'COLOR1'=C, 'COLOR2'=B|F]),!.




tsurf6:-tsurf(

	(
	=>(
		instance(A, 'Advertisement'), 
		(
		exists(B, and(refers(A, B), 
		hasPurpose(A, exists(C, and(instance(C, 'Selling') ,patient(C, B))))))))),['ADVERT'=A, 'OBJ'=B, 'SALE'=C|D]).

tsurf6b:-tsurf(

	(
	=>(
		instance(A, 'Advertisement'), 
		(
		exists(B, and(refers(A, B), 
		hasPurpose(A, exists(C, or(instance(C, 'Selling') ,patient(C, B))))))))),['ADVERT'=A, 'OBJ'=B, 'SALE'=C|D]).

tsurf6a:-tsurf(

	(
	=>(
		instance(A, 'Advertisement'), 
		(
		exists(B, and(refers(A, B), 
		hasPurpose(A, exists(C, and(instance(C, 'Selling') ,patient(C, B, sueFn(C)))))))))),['ADVERT'=A, 'OBJ'=B, 'SALE'=C|D]).

tsurf6bi:-tsurf(

	(
	<=>(
		instance(A, 'Advertisement'), 
		(
		exists(B, and(refers(A, B), 
		hasPurpose(A, exists(C, and(instance(C, 'Selling') ,patient(C, B, sueFn(C)))))))))),['ADVERT'=A, 'OBJ'=B, 'SALE'=C|D]).

tsurf6c:-tsurf(

	(
	=>(
		instance(A, 'Advertisement'), 
		(
		exists(B, and(refers(A, B), 
		hasPurpose(A, exists(C, =>(instance(C, 'Selling') ,patient(C, B, sueFn(C)))))))))),['ADVERT'=A, 'OBJ'=B, 'SALE'=C|D]).

tsurf7:-tsurf(o,D).

tsurf8:-tsurf(A,['PHYS'=A|D]).                                

tsurf9:-tsurf(<=>(instance(A, 'Physical'), exists(B, exists(C, and(located(A, C), time(A, B))))), ['PHYS'=A, 'LOC'=C, 'TIME'=B|D]).

tsurf9a:-tsurf(=>(instance(A, 'Physical'), exists(B, exists(C, and(located(A, C), time(A, B))))), ['PHYS'=A, 'LOC'=C, 'TIME'=B|D]).


%tsurf(Surface,Vars):-tam(Surface,Vars),!.

    
tsurf(Clause,KRVars):-
	getAssertionClauses('KB','Ctx',Clause,Out,KRVars,FlagsL),!,
	subst(FlagsL,'.',*,Flags),!,
	writeq_conj(Out),nl,nl.
	%writeTranslation(Clause).
		       /*
	writeObject('$spacer',KRVars),
	writeObject(Clause,KRVars),
	writeObject('$spacer',KRVars),
	writeObject(PNNF,KRVars),
	writeObject('$spacer',KRVars),
	writeObject(Flags,KRVars),!,
	writeObject('$spacer',KRVars),
	writeObject_conj(Out,KRVars),!.
			 */

writeq_conj(and(Out1,Out2)):-
	writeq_conj(Out1),
	writeq_conj(Out2).
writeq_conj([Out1|Out2]):-
	writeq_conj(Out1),
	writeq_conj(Out2).
writeq_conj(Out):-writeq(Out),nl.
			 
			 

sb(TN):-
	mooCache(_,_,Surf, Vars, KB,Ctx, TN, _,_),
	tsurf(Surf,Vars),!.




/*----------------------------------------------------------------------
    Debugging kit.
	writeSentenceBlock(ListOfClauses)
	    displays a list of clauses, one per line.
	writeClauseBlock(Clause)
	    displays a single clause in "Kowalski notation"
	writeTranslation(Formula)
	    translates a formula and prints the result.

:- public
	t1/0,t9/0,writeTranslation/1.

     */

writeSentenceBlock([Clause]) :- !,
	writeClauseBlock(Clause),
	nl.
writeSentenceBlock([Clause|Clauses]) :-
	writeClauseBlock(Clause),
	write(' AND'), nl,
	writeSentenceBlock(Clauses).
writeSentenceBlock([]) :-
	write('TRUE'), nl.


writeClauseBlock(clause(PosAtoms, NegAtoms)) :-
	numbervars(PosAtoms, 0, N),
	numbervars(NegAtoms, N, _),
	writeClauseBlock(PosAtoms, ' v '),
	write(' <- '),
	writeClauseBlock(NegAtoms, ' & '),
	fail.
writeClauseBlock(_).


writeClauseBlock([Atom], _) :- !,
	print(Atom).
writeClauseBlock([Atom|Atoms], Separator) :-
	print(Atom), write(Separator),
	writeClauseBlock(Atoms, Separator).
writeClauseBlock([], _) :-
	write([]).



writeTranslation(X) :-
	clausal_form(X, Y),
	writeSentenceBlock(Y),
	clausal_form_of_negation(X,N),
	nl,nl,
	writeSentenceBlock(N),!.
	

t1 :- writeTranslation((a=>b) and (b=>c) and (c=>d) and (d=>a) => (a<=>d)).

t2 :- writeTranslation(continuous(F,X) <=> all(Epsilon, (Epsilon > 0) =>
	    exists(Delta, (Delta > 0) and all(Y,
		(abs(Y-X) < Delta) => (abs(val(F,Y)-val(F,X)) < Epsilon)
	)))).

t3n :- clausal_form_of_negation(
	( subset(S1,S2) <=> all(X, member(X,S1) => member(X,S2) )) =>
	( subset(T1,T2) and subset(T2,T3) => subset(T1,T3) )	,Y),
	writeSentenceBlock(Y).

t3 :- clausal_form(
	( subset(S1,S2) <=> all(X, member(X,S1) => member(X,S2) )) =>
	( subset(T1,T2) and subset(T2,T3) => subset(T1,T3) )	,Y),
	writeSentenceBlock(Y).

t4 :- writeTranslation(subset(T1,T2) and subset(T2,T3) => subset(T1,T3)).


t5 :- writeTranslation((a=>b) and (b=>c)).

t6g :- writeTranslation(not (a(X) and b(X))).

t6e :- writeTranslation((a(X) => (exists(Y, b(X,Y))))).

t7 :- writeTranslation((a and b) or c).

t8 :- writeTranslation((a and b) or (a and not b) or (not a and b) or (not a and not b)).

t9 :- writeTranslation(
	(true(P) <=> writeTranslation(w0,P)) and
	(writeTranslation(W1,P1 and P2) <=> writeTranslation(W1,P1) and writeTranslation(W1,P2)) and
	(writeTranslation(W1,P1 or P2) <=> writeTranslation(W1,P1) or writeTranslation(W1,P2)) and
	(writeTranslation(W1,P1 => P2) <=> (writeTranslation(W1,P1) => writeTranslation(W1,P2))) and
	(writeTranslation(W1,P1 <=> P2) <=> (writeTranslation(W1,P1) <=> writeTranslation(W1,P2))) and
	(writeTranslation(W1,not P1) <=> not writeTranslation(W1,P1)) and
	(writeTranslation(W1,know(A1,P1)) <=> all(W2,k(A1,W1,W2)=>writeTranslation(W2,P1))) and
	k(A1,W1,W1) and
	(k(A1,W1,W2) => (k(A1,W2,W3) => k(A1,W1,W3))) and
	(k(A1,W1,W2) => (k(A1,W1,W3) => k(A1,W2,W3))) and
	(writeTranslation(W1,know(A,P)) <=> all(W2,k(A,W1,W2) => writeTranslation(W2,P)))
	).

/*----------------------------------------------------------------------*/




/*


 error:  'Predicate Failed' getNegationForm(PIN,PNNF,forall(P20, forall(Q20, instance(P20, 'VariableArityRelation')=>not exists(Q20, valence(P20, Q20)))), toplevel, 0, 'BASE ONTOLOGY', ['REL'=P20, 'INT'=Q20|R20], _G4720, forall(P20, forall(Q20, instance(P20, 'VariableArityRelation')=>not exists(Q20, valence(P20, Q20)))), [], _G4723, _G4724)
*/

/*
% error:  'Predicate Failed' getNegationForm(PIN,PNNF,forall(X10, forall(Y10, forall(Z10, forall(A11, forall(B11, not exists(X10, exists(Y10, instance(Y10, 'CutSetFn'(Z10))and instance(X10, 'MinimalCutSetFn'(Z10))and pathLength(Y10, A11)and pathLength(X10, B11)and lessThan(A11, B11)))))))), toplevel, 0, 'GRAPH THEORY', ['PATH1'=Y10, 'PATH2'=X10, 'GRAPH'=Z10, 'NUMBER1'=A11, 'NUMBER2'=B11|C11], _G12470, forall(X10, forall(Y10, forall(Z10, forall(A11, forall(B11, not exists(X10, exists(Y10, instance(Y10, Z11803)and instance(X10, A11804)and pathLength(Y10, A11)and pathLength(X10, B11)and lessThan(A11, B11)))))))), [Z11803, A11804], _G12473, _G12474)

*/




write_cnf_lit(and(C1,C2),Vars):-!,
	write_cnf_lit(C1,Vars),nl,nl,
	write_cnf_lit(C2,Vars).
write_cnf_lit(C1,Vars):-!,nl,nl,
	write_cnf_c(C1,Vars).
		
write_cnf_c(CNF,Vars):-
	c_lit(CNF,Lit),
	term_to_atom(Lit,Lit1),
	writeObject(Lit1,Vars).
	
c_lit(or(C1,C2),L):-!,
	c_lit(C1,L1),
	c_lit(C2,L2),
	append(L1,L2,L).
	
c_lit((C1),[C1]):-!.

c_lit(not(C1),[C1]):-!.
c_lit(C1,[not(C1)]):-!.





:-include('moo_header.pl').

% ===================================================================
% EXPORTS
% ===================================================================


test_u_load_kif:-agentLoadKif('IsApplicationExecutable.txt','test','tester').

/*

Saves state of a Context (crypt is a less human readable form) 
u_save(+FileName,+GlobalContext) 
u_save_crypt(+FileName,+GlobalContext) + Nearly Completed/Tested (actully just a QLF version =)  
*/
test_u_save:-u_save('testfile.pl','test').

	   
/*

Loads the saved state (crypted or not)  (adds assertions) they were originally stored with a context
interfaceLoad(+FileName) 
Clears a context (Leave unbound to clear all contexts) 
u_clear(?Context) 
We still have some discussion on how contexts are linked together so right now they are all active.. 
Mainly it gives us a way to do bulk load/unloads and separation user data from general system's like STANDARD. 

*/

test_u_load:-interfaceLoad('testfile.pl').




/* Assertion 
interfaceAssert(+KIFCharsIn,+Ctx,+KB,+User). 
KIFCharsIn = "(instance virginia Computer)"  
Ctx = 'Computers and Things' 
KB = 'PrologMOO' currently must be set to 'PrologMOO' 
User = 'dmiles@users.sourceforge.net'  

*/
	% interfaceAssert(+KIFCharsIn,+Ctx,+KB,+User). 
test_u_assert:-interfaceAssert("(instance hardwareType Relation)",'test','PrologMOO','tester') .
test_u_assert1:-interfaceAssert("(instance hardwareType Object)",'test','PrologMOO','tester') .


/*
Now the predicates that you most want 
Invokes a request 
interfaceRequest(+KIFCharsIn,+Ctx,+KB,+User,-UResultsSoFar,-Result,-Explaination,-Status). 

Input variables: 
KIFCharsIn = "(instance ?What Computer)"  
Ctx = 'Computers and Things'  % Any legal Atom that helps you book keep 
KB = 'PrologMOO' currently must be set to 'PrologMOO' 
User = 'dmiles@users.sourceforge.net'  There are times when error messages about redundant assertions happen.. and for bookkeeping all assertions have an author atom.. perhaps you can even use the source file name it came from.

output variables: 
UResultsSoFar = 0 indexed value of answer number.. +1 would mean it is a repeated answer with a different explaination.. you could choose not to display answers with negative numbers 
Result = Is the Prolog Variable Vector like ['What'=virginia|_] (can be written with writeObject(getPrologVars(Result),Result))  
Explaination = A explaination object that can be printed with writeObject(explaination(Explaination),Result) 
Status = is either 'true' or '!' ... true = more results.. '!' means no more results 
(Status is optional.. if you completely remove this argument and it will fail at the next call after the first solution returning !)  
*/

		% interfaceRequest(+KIFCharsIn,+Ctx,+KB,+User,-UResultsSoFar,-Result,-Explaination,-Status). 
test_u_request:-interfaceRequest("(hardwareType ?W ?U)",'test','PrologMOO','tester',U,R,P),
	 writeObject(getPrologVars(R),R),writeObject(explaination(P),R),fail.

test_u_request2:-
	 interfaceRequest("(hardwareType ?W ?U)",'test','PrologMOO','tester',U,R,P),
	 not((U=(-1))),
	 writeObject(getPrologVars(R),R),
	 writeObject(explaination(P),R),
	 fail.





/*
% Lets you discover any errors that were created durring any operation 
popError(Message) 
+Will fail if no errors are in this buffer 
+When any of the above commands are first invoked the error buffer is cleared 
+Whenever there were errors and you read this buffer each one is deleted while it is read 
+Messages are stored as printable objects writeObject(Message)  we can discuss a more programmatic approach to formats later based on how we want to use them. 
*/

show_errors:-popError(Message),writeObject(Message,_).

/*
 
 Term Writing

writeObject(+Object).
writeObject(+Object,+Vars). 

Allows easy printing of variables and objects
*/


 
/*
Belief Variables
 
Set Belief Optional Variable
u_set_option(VariableName=NewValue).
*/

u_set_option_test:-u_set_option(opt_timeout=2).

 
/*
Retrive Belief Optional Variable
u_get_option(VariableName=CurrentValue).
*/

u_get_option_test:-u_get_option(O),
	write_ln(O),fail.
u_get_option_test.
 

