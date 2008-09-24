:-include('logicmoo_utils_header.pl'). %<?

% ===========================================================
% Search For Constant (Atom)
% ===========================================================
clientEvent(Channel,Agent,[file='browse.moo']):-
      writeHTMLStdHeader('Contsant Browser'),
      say('<pre>'),
      ignore((getBrowseAtoms(X),once(writeHtml(linkFor(X))),nl,fail)),!,
      say('</pre>'),
      writeHTMLStdFooter.                

clientEvent(Channel,Agent,[file='browse.moo', find=Predicate]):-
      atom(Predicate),atom_to_term(Predicate,Term,_),!,
      writeHTMLStdHeader(Term),
      htmlListing(Term),
      writeHTMLStdFooter.

clientEvent(Channel,Agent,Options):-memberchk(show='find',Options),!,
	getMooOption(Agent,word='instance',Word),
	getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(Agent,opt_theory='LogicMOO',Context),
	getMooOption(Agent,asid=_,AID),
	getMooOption(Agent,user='Web',Agent),
	getMooOption(Agent,interp='kif',Interp),
	disp_word_to_surface(Word,Context,Out),
	draw_update_enable(Context,Out).

disp_word_to_surface(Word,Context,_Ctx,Out):-
	retractall(pkids(_)),
	mooCache(PredR,SurfaceTODO,SURF,PROLOG,Context,AID,Maintainer,OnOff),
	getConstants(atomic,SURF,Atoms,_,_),memberchk(Word,Atoms),  
	writeAssertion(SurfaceTODO,SURF,PROLOG,Context,AID,Maintainer,OnOff),
	fail.	

disp_word_to_surface(Word,Context,Out):-!,setof(retract(pkids(INTID)),Out).

on_to_check(OnOff,OnOff,'Checked').
on_to_check(_,_,' ').


% ===========================================================
% Search For Tracking Number
% ===========================================================
clientEvent(Channel,Agent,Options):-(memberchk(submit=editaid,Options);memberchk(dispasid=dispasid,Options)),!,  
	getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(Agent,opt_theory='LogicMOO',Context),
	getMooOption(Agent,asid=_,AID),
	getMooOption(Agent,user='Web',Agent),
	getMooOption(Agent,interp='kif',Interp),
	retractall(pkids(_)),!,
	disp_tn_to_surface(Context,AID,Out),
	draw_update_enable(Context,Out),!.
	
disp_tn_to_surface(Context,bogus,Out):-!.

disp_tn_to_surface(Context,TN,Out):-
	mooCache(PredR,Format,SURF,Prolog,Context,TN,Maintainer,OnOff), 
	writeAssertion(Format,SURF,Prolog,Context,TN,Maintainer,OnOff),
	fail.	

disp_tn_to_surface(Context,Out):- %isMooOption(Agent,disp_notes_nonuser=on),
	mooCache(PredR,Format,SURF,Prolog,Context,TN,Maintainer,OnOff),  
	writeAssertion(Format,SURF,Prolog,Context,TN,Maintainer,OnOff),
	fail.	

disp_tn_to_surface(Context,_,Out):-!,setof(retract(pkids(INTID)),Out).

aid_to_number(Number,Number):-number(AID),!.
aid_to_number(AID,Number):- atom_to_term(AID,Number,_),!.


% ===========================================================
% Surface Returns External TN
% ===========================================================
clientEvent(Channel,Agent,Options):-
   %attach_console,trace,
      memberchk(submit=tn_find,Options),!,
        ensureMooOption(Agent,opt_ctx_assert,'GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory,'LogicMOO',Context),
        ensureMooOption(Agent,client,'java',CLIENT),
        ensureMooOption(Agent,sf,surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        ensureMooOption(Agent,user,'Web',Agent),
         logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
         write_out_kif_tn(Assertion_Chars,NEWFORM,Vars,Ctx,Context,Maintainer),!.

write_out_kif_tn(Assertion_Chars,computed(comment(_)),Vars,Ctx,Context,Maintainer):-
         say('Syntax Error: Unmatched parentheses in "~s"\n',[Assertion_Chars]).
write_out_kif_tn(Assertion_Chars,comment(_),Vars,Ctx,Context,Maintainer):-
         say('Syntax Error: Unmatched parentheses in  "~s"\n',[Assertion_Chars]).
write_out_kif_tn(Assertion_Chars,NEWFORM,Vars,Ctx,Context,Maintainer):-
        copy_term((NEWFORM),(CNF)),
        numbervars(CNF,'$VAR',0,_),
        mooCache(PredR,surface,CNF, _, Context, TN, Maintainer, O),
        mooCache(PredR,surface,OF, _, Context, TN, Maintainer, O),
        numbervars(OF,'$VAR',0,_),
        OF == CNF,!,
        say('~w\n',[TN]),!.
write_out_kif_tn(Assertion_Chars,NEWFORM,Vars,Ctx,Context,Maintainer):-
         say('Not found in ~w of ~w "~s"\n',[Ctx,Context,Assertion_Chars]).

% ===========================================================
% Search For State (Atom) (rejected,on,gaf).. etc
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(show='state',Options),!,
	getMooOption(Agent,word='disabled',Word),
	getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(Agent,opt_theory='LogicMOO',Context),
	getMooOption(Agent,asid=_,AID),
	getMooOption(Agent,user='Web',Agent),
	getMooOption(Agent,interp='kif',Interp),
	disp_word_to_surface(Word,Context,Out),
	draw_update_enable(Context,Out).

disp_status_to_surface(Status,Context,Out):-
	retractall(pkids(_)),
	mooCache(PredR,SurfaceTODO,SURF,Prolog,Context,AID,Maintainer,Status),
	writeAssertion(SurfaceTODO,SURF,Prolog,Context,AID,Maintainer,Status),
	fail.	

disp_status_to_surface(Status,Context,Out):-!,setof(retract(pkids(INTID)),Out).

% ===========================================================
% Search Disabled Assertion
% ===========================================================
clientEvent(Channel,Agent,Options):-
	memberchk(cmd='Show Disabled',Options),!,
	getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(Agent,opt_theory='LogicMOO',Context),
	getMooOption(Agent,asid=_,AID),
	getMooOption(Agent,user='Web',Agent),
	getMooOption(Agent,interp='kif',Interp),
	say('<H3><Font Color=Red>Listing Disabled Assertions...</Font></H3>',[]),!,
	show_disabled_assertions(Context,Ctx),!.
	
show_disabled_assertions(Context,Ctx):-
	mooCache(PredR,Form,Formula,Prolog,Context,AID,Maintainer,OnOff),not(OnOff=on),not(OnOff=uncanonicalized),
	writeAssertion(Form,Formula,Prolog,Context,AID,Maintainer,OnOff),
	fail.
	
show_disabled_assertions(Context,Ctx):-say('<H3><Font Color=Red>Done.</Font></H3>',[]).





