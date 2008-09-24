:-include('logicmoo_utils_header.pl'). %<?
% ===========================================================
% Edit Assertion
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(t='ea',Options),!,
	getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(Agent,opt_theory='LogicMOO',Context),
	getMooOption(Agent,asid=_,AID),
	getMooOption(Agent,user='Web',Agent),
	getMooOption(Agent,interp='kif',Interp),
	mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,_),
	%retractall(mooCache(PredR,_,_,_,Context,AID,_,_)),
	toMarkup(kif,SURF,Vars,Formula),
	say('<textarea rows=6 cols=90 name="sf">~w</textarea><br>',[Formula]),
	say('<br>&nbsp;&nbsp;<INPUT type=submit name=submit value="Update Source">&nbsp;<input type=hidden name=editid value="~w">',[AID	]),
	say('&nbsp;&nbsp;&nbsp;&nbsp;<INPUT type=radio name="interp" value="kif" checked>KIF</INPUT><INPUT type=radio name="interp" value="ace" DISABLED>ACE</INPUT>&nbsp;&nbsp;<A href="askInsert.jsp?theory=~w">Cancel</a>',[Context]).


% ===========================================================
% Show Assertion Updater
% ===========================================================
draw_update_enable(Context,[]):- say('Search yielded no Results.\n',[]).
draw_update_enable(Context,As):-!,
	format_o(
		'<HR>~w&nbsp;<INPUT type=submit name=submit value="Change"/>',select(ue,['Enable','Disable','Delete'])),!,
       %show_available_contexts_in_combobox(destination,[],Out),!,say('<BR><input type=radio name=CopyOrTransfer checked value=Transfer><B>Transfer</B></input>&nbsp;<input type=radio name=CopyOrTransfer value=Copy><B>Copy</B></input> selected assertions to ~w <input type=submit name=move value=Submit>',[Out]),!.
       	say('&nbsp;&nbsp;&nbsp;&nbsp;<A href="askInsert.jsp?theory=~w">Return to Ask/Insert</A>',[Context]).
		

clientEvent(Channel,Agent,Options):-memberchk(submit=tn_sync,Options),!,
       %  say(user_error,'ua: ~w\n',[Options]),flush_output(user_error),
        ensureMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        ensureMooOption(Agent,tn=_,EXTID),
        ensureMooOption(Agent,user='Web',Agent),
        ensureMooOption(Agent,sf=surf,Assertion),
        logOnFailure(atom_codes(Assertion,Codes)),
        getSurfaceFromChars(Codes,STERM,Vars),
        getMooTermFromSurface(STERM,Surface),
        destroyTN(Context,EXTID,_),
        once(invokeInsert([trusted,canonicalize],surface,Surface,Ctx,EXTID,Context,Vars,Agent)),!.

clientEvent(Channel,Agent,Options):-memberchk(submit=tn_form_update,Options),!,
       %  say(user_error,'ua: ~w\n',[Options]),flush_output(user_error),
        ensureMooOption(Agent,tn=_,EXTID),
        ensureMooOption(Agent,user='Web',Agent),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        ensureMooOption(Agent,sf=surf,Assertion),
        logOnFailure(atom_codes(Assertion,Codes)),
        getSurfaceFromChars(Codes,STERM,Vars),
        getMooTermFromSurface(STERM,Surface),
        logOnFailure(retract(mooCache(PredR,surface,OldSurf,_,Context,EXTID,_,_))),
        destroyTN(Context,EXTID,_),
        once(invokeInsert([trusted,canonicalize],surface,Surface,Ctx,EXTID,Context,Vars,Agent)),!.

% ===========================================================
% Show Assertions (Surface)
% ===========================================================

writeAssertion(surface,SURF,Vars,Context,AID,Maintainer,OnOff):- !,
	assert(pkids(AID)),
	toMarkup(html,SURF,Vars,SAtom),
	%toMarkup(kif,SURF,Vars,KIF),
	%setMooOption(Agent,sf=KIF),
	on_to_check(on,OnOff,Checked),
	say('<hr><A HREF="askInsert.jsp?theory=~w&asid=~w&t=ea" title="Edit Assertion"><IMG border=0 src="pixmaps/cyan.gif" asrtid=~w></A><input type=chectheoryox class=assertionChecks  name=~w ~w/><nobr>',[Context,AID,AID,Checked]),
	say('<b>Surface</b> ID<font color=red>~w:~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font> Status: <font color=puple>~w</font>',[AID,Context,Maintainer,OnOff]),
	say('~w',[SAtom]),!.
	
% ===========================================================
% Show Assertions (HL)
% ===========================================================

writeAssertion(surface,WFS,Vars,Context,AID,Maintainer,OnOff):- %isMooOption(Agent,disp_notes_nonuser=on),
	assert(pkids(AID)),
	toMarkup(html,formula(WFS),Vars,SAtom),
	%toMarkup(kif,SURF,Vars,KIF),
	%setMooOption(Agent,sf=KIF),
	on_to_check(on,OnOff,Checked),
	say('<hr><A HREF="askInsert.jsp?theory=~w&submit=editaid&asid=~w&t=ea" title="Show Source"><IMG border=0 src="pixmaps/purple.gif" asrtid=~w></A><input type=chectheoryox class=assertionChecks  name=~w ~w/><nobr>',[Context,AID,AID,Checked]),
	say('<b>~w</b> ID<font color=red>~w:~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font> Status: <font color=puple>~w</font>',['Surface',AID,Context,Maintainer,OnOff]),
	say('~w',[SAtom]),!.
/*
writeAssertion(skolem,WFS,Vars,Context,AID,Maintainer,OnOff):- %isMooOption(Agent,disp_notes_nonuser=on),
	assert(pkids(INTID)),
	toMarkup(html,formula(WFS),Vars,SAtom),
	%toMarkup(kif,SURF,Vars,KIF),
	%setMooOption(Agent,sf=KIF),
	on_to_check(on,OnOff,Checked),
	say('<hr><A HREF="askInsert.jsp?theory=~w&submit=editaid&asid=~w&t=ea" title="Show Source"><IMG border=0 src="pixmaps/purple.gif" asrtid=~w></A><input type=chectheoryox class=assertionChecks  name=~w ~w/><nobr>',[Context,AID,AID,Checked]),
	say('<b>~w</b> ID<font color=red>~w:~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>   Status: <font color=puple>~w</font>',['Skolem',AID,Context,OnOff]),
	say('~w',[SAtom]),!.
*/
writeAssertion(Head,Tail,Vars,Context,AID,Maintainer,OnOff):- %isMooOption(Agent,disp_notes_nonuser=on),
	assert(pkids(AID)),
	toMarkup(html,formula((Head:-Tail)),Vars,SAtom),
	%toMarkup(kif,SURF,Vars,KIF),
	%setMooOption(Agent,sf=KIF),
	on_to_check(on,OnOff,Checked),
	say('<hr><A HREF="askInsert.jsp?theory=~w&submit=editaid&asid=~w&t=ea" title="Show Source"><IMG border=0 src="pixmaps/purple.gif" asrtid=~w></A><input type=chectheoryox class=assertionChecks  name=~w ~w/><nobr>',[Context,AID,AID,Checked]),
	say('<b>~w</b> ID<font color=red>~w:~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Status: <font color=puple>~w</font>',['Heuristic',AID,Context,OnOff]),
	say('~w',[SAtom]),!.


% ===========================================================
% EDITOR WINDOW
% ===========================================================

writeEditorLogicWindow:-!,
        ensureMooOption(Agent,sf='(isa ?X ?Y)',Formula),
        say('<textarea rows=6 cols=90 name="sf">~w</textarea><br>',[Formula]),
        say('<br><INPUT type=submit name=submit value="Update Source"></INPUT><hr>',[]),
        say('<INPUT type=radio name="interp" value="kif" checked>KIF</INPUT>',[]),
        say('<INPUT type=radio name="interp" value="ace">ACE</INPUT>',[]).

writeDebugWindow:-!,
        ensureMooOption(Agent,sf='(isa ?X ?Y)',Formula),
	say('<hr><FORM name="theForm" ID="theForm"  METHOD="GET">
		<textarea rows="5" cols="70" name="sf">~w</textarea><br>
		<INPUT type="hidden" name="interp" value="kif" checked>
		<INPUT type="submit" name="submit" value="command"></INPUT></form>',[Formula]).






