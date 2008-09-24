% ===================================================================
% File 'e2c.pl'
% Purpose: Attempto Controlled English to CycL conversions from SWI-Prolog  
% This implementation is an incomplete proxy for CycNL and likely will not work as well
% Maintainer: Douglas Miles
% Contact: $Author: dmiles $@users.sourceforge.net ;
% Version: 'interface.pl' 1.0.0
% Revision:  $Revision: 1.3 $
% Revised At:   $Date: 2005/06/06 15:43:15 $
% from Bratko chapter 17 page 455. This comes from Pereira and Warren paper, AI journal, 1980 
% ===================================================================
/*
:-module(e2c,[
	e2c/1,
	e2c/2,
	testE2C/0]).
*/
% imports sentencePos/2,bposToPos/2
:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).
:-set_prolog_flag(double_quotes,string).
:-multifile(user:currentParsingPhrase/1).
:-multifile(user:recentParses/1).
:-dynamic(user:recentParses/1).
:-dynamic(user:currentParsingPhrase/1).
:-(consult('freq.pdat.txt')).
:-(consult('colloc.pdat.txt')).
:-(consult('pt-grammar.prul2.txt')).
:-(consult('BRN_WSJ_LEXICON.txt')).
:-(consult(textCached)).
:-(consult(recentParses)).
                                                   
% ===================================================================
% Caches
% ===================================================================


%:-dynamic(textCached/2).
:-dynamic(currentParsingMt(Profile)).
:-dynamic(discouragedRule(Profile,Rule)).
saveCaches:-!.
saveCaches:-tell(textCached),
      listing(textCached),
      listing(currentParsingMt),
      listing(discouragedRule),
      told,tell(recentParses),listing(recentParses),told.
:-at_halt(saveCaches).

:-use_module(library(make)),redefine_system_predicate(make:list_undefined).
make:list_undefined.

:-module_transparent(processRequestHook/1).
:-multifile(processRequestHook/1).
:-dynamic(processRequestHook/1).

user:processRequestHook(ARGS):- member(file='english.moo',ARGS),!, 
      ignore(member(english=English,ARGS)),
      ignore(English=''),
      writeHTMLStdHeader('English Parser'),
      handleCheckedRules(ARGS),
      format('
      <form method="GET">
	<p><textarea rows="2" name="english" cols="80">~w</textarea><br>
       <input type="submit" value="Parse Normally" name="submit">&nbsp;<input type="submit" value="Parse with idiomatics" name="submit">
       <input type="checkbox" value="CHECKED" name="findall">&nbspShow All<br>
        </p>
       </form>
      Please wait.. this is <font color="red" size=+1>very</font> slow!<br>
      <textarea rows="5" name="debug" cols="80" wrap=off>',[English]),
      flush_output,
      notraceTry(e2c(English,CycL)),
      format('</textarea><br>'),flush_output,
     % format('<textarea rows="11" name="cycl" cols="80" wrap=off>',[]),flush_output,once(writeCycL(CycL)),format('</textarea> ..Done</pre>'),flush_output,
      notraceTry(showToCreateForm(English,CycL)),
      flush_output,
      notraceTry(listRecentParses),
      %format('</pre>'),flush_output,
      flush_output,
      writeHTMLStdFooter, flush_output,!.

diagnoseMainFormula(CycL,'T'):-!.

showToCreateForm(English,CycL):-
         format('<form action="assert.moo" method="GET">\n'),
         format('<table border=1 ><TH>Assertion</TR><tr><td nowrap>\n'),!,
         format('<textarea rows="20" name="cycl" cols="80" wrap=off>',[]),
         diagnoseMainFormula(CycL,Bad),!,
      flush_output,
         (not(is_string(Bad))-> 
               once(writeCycL(CycL)) ; once(writeCycL(Bad))),!,
      flush_output,
         format('</textarea>\n'),
         format('</td></tr></table><br>\n'),
         format('<input type="submit"  name="assert" value="Assert Now"/><br>'),
         format('</form>\n\n'),
         format('<form action="english.moo" method="POST">\n'),
         format('<input type="hidden" name="english" value="~w"/>',[English]),
         getFormulaVars(Sorted),!,
         format('<table border=1 ><caption>~q</caption>\n<TR><TH>Variable Quantification<br>Or Constant Name<TH>Refactoring To Customize The Assertion (Persistent)</TR>\n',[Sorted]),!,
      notraceTry(showToCreateVars(Sorted)),
         format('</table><br>\n'),
         currentParsingProfile(Profile),
         format('<input type=text name=profile value="~w"/>&nbsp<input type="submit" name="review" value="Save Profile"/>',[Profile]),
         format('</form>\n\n'),!.

getFormulaVars(Sorted):-findall(X,toCreate(_,hypotheticDenotation(X,_,_)),List),list_to_set(List,RSorted),reverse(RSorted,Sorted).


notraceTry(Goal):-notrace(ignore(catch(Goal,E,writeCycL(Goal-E)))).

unsaveCycL(CycLO):-
         getScope(CycLOS),         
%         findall(format('<br><input name="clause~w" type=checkbox checked>~w',[ID,CycL]),clause(toCreate(Rule,CycL),true,ID),CycLLS),         
         findall(CycL,(toCreate(Rule,CycL),discouragedRule(Rule)),CycLLS),         
         list_to_set(CycLLS,RCycLLS),joinCols(and,RCycLLS,Inner),
         subst(CycLOS,':SCOPE',Inner,CycLO),!.
         
getScope(O):-getScope1(CycLOS1),getScope2(CycLOS1,O).
getScope1(CycLOS):-
              findall(':SCOPE'-thereExistExactly(N,Subj,':SCOPE'),(retract(toCreate(_,thereExistExactly(N,Subj,':SCOPE'))),retractall(toCreate(_,thereExists(Subj,_)))),CycLL), 
              substEach(':SCOPE',CycLL,CycLOS).
getScope2(CycLOS,O):-
              findall(':SCOPE'-thereExists(Subj,':SCOPE'),retract(toCreate(_,thereExists(Subj,':SCOPE'))),CycLL), 
              substEach(CycLOS,[],O).

%                                                            %(ParseMtForSourceFn (StringInDocumentFn E2C-Parses-CCW "A picture of a dog is in the yard")) 
   %E2C-Parses-CCW
   %A picture of a dog is in the yard


cyclNumberVars(X):-
         flag('$cyclNumberVars',S,S),
         numbervars(X,S,E),
         flag('$cyclNumberVars',_,E),!.

saveCycL(Subj,[]):-!.
saveCycL(Subj,Subj):-!.
saveCycL(Subj,':SCOPE'):-!.
saveCycL(_,Subj):-atom(Subj),!.
saveCycL(_,'$VAR'(_)):-!.
saveCycL(Var,Subj):-var(Var),!,saveCycL(Subj,Subj).
saveCycL(_,rule(Rule,CycL)):-!,saveCycL(Rule,CycL).
saveCycL(X,Y):-not(ground(X:Y)),cyclNumberVars(X:Y),!,saveCycL(X,Y).
saveCycL(Subj,[C|CycL]):-!,saveCycL(Subj,C),!,saveCycL(Subj,CycL).
saveCycL(Subj,thereExistExactly(S,N,CycL)):-CycL\=':SCOPE',!,saveCycL(Subj,thereExistExactly(S,N,':SCOPE')),saveCycL(Subj,CycL),!.
saveCycL(Subj,thereExists(S,CycL)):-CycL\=':SCOPE',!,saveCycL(Subj,thereExists(S,':SCOPE')),saveCycL(Subj,CycL),!.
saveCycL(Subj,CycL):-compound(CycL),CycL=..[and|Ls],!,saveCycL(Subj,Ls).
saveCycL(Subj,CycLNV):-unnumbervars(CycLNV,CycL),retract(toCreate(_Subj,CycL)),!,asserta(toCreate(Subj,CycL)).
saveCycL(Subj,CycL):-asserta(toCreate(Subj,CycL)),!.

%unsaveCycL([Subj|S],CycLO):-

toCycApiExpressionCheckBox(CycL,CycL9):-toCycApiExpression(CycL,CycL9).

fCycL(N,CycL,CycLOutS):-
      once((toCycApiExpressionCheckBox(CycL,CycL9),
      indentSNls(N,CycL9,CycLOutS))).
writeCycL(CycL):-fCycL(0,CycL,CycLOutS),format('~w~n',[CycLOutS]),flush_output,!.
                                                   
nSpaces(N,[]):-N<1,!.
nSpaces(N,[32|More]):-NN is N-1,nSpaces(NN,More),!.

indentSNls(N,X,S):-string_to_list(X,M),indentNls(N,M,Y),trim(Y,YY),string_to_list(S,YY).
indentNls(N,[],[]).
indentNls(N,[40|More],CycLOutS):-NN is N+2,indentNls(NN,More,CycL9),nSpaces(N,Spaces),append([10|Spaces],[40|CycL9],CycLOutS).
indentNls(N,[41|More],[41|CycLOutS]):-NN is N-2,indentNls(NN,More,CycLOutS).
indentNls(N,[X|More],[X|CycLOutS]):-indentNls(N,More,CycLOutS).


offerVarQuantifiers(Item):-
   notraceTry(makeName(Item,Name)),!,
   format('<p>&nbsp;<INPUT TYPE="radio" NAME="skolem(~q)" VALUE="skolemize" CHECKED>~w</input><br>&nbsp;<INPUT TYPE="radio" NAME="skolem(~q)" VALUE="find-or-create-constant"><input type="text" name="constant(~q)" value="~w"></input>',
                                                                           [Item,                                         Item,                                                                  Item,                                                                                           Item,               Name]),!.

showToCreateVars([]):- %show rest
         format('\n<TR><TD NOWRAP>'),
%         offerVarQuantifiers(X),
         format('&nbsp;\n<TD NOWRAP><pre>'),!,
         retract(toCreate(Rule,CycL)),
         offerAssertion(Rule,CycL),fail.
showToCreateVars([]):-!,format('</pre></TR>\n').  
showToCreateVars([S|Sorted]):-
         notraceTry(showVarRow(S)),!,
         showToCreateVars(Sorted).

showVarRow(Item):-
         format('\n<TR><TD NOWRAP>'),
        notraceTry(offerVarQuantifiers(Item)),
         format('&nbsp;\n<TD NOWRAP><pre>'),!,
         toCreate(Rule,CycL),
         once((occurs:contains_var(Item,CycL),retractall(toCreate(Rule,CycL)),
         notraceTry(offerAssertion(Rule,CycL)))),fail.
showVarRow(_):-format('</pre></TR>\n').


currentParsingMt('ParseMtForSourceFn'('StringInDocumentFn'(Profile,string(Sent)))):-currentParsingPhrase(Sent),currentParsingProfile(Profile).
currentParsingPhrase(Sent):-recentParses(Sent),!.
currentParsingProfile('E2C-Parses-CCW').

discouragedRule(Rule):- currentParsingProfile(Profile),not(discouragedRule(Profile,Rule)).

handleCheckedRules(ARGS):-
      currentParsingProfile(Profile),
      currentParsingMt(Mt),
      findall(Rule:CycL,(member(accepted(CycL)=Rule,ARGS),retractall(discouragedRule(Profile,Rule))),POS),
      findall(Rule:CycL,((member(offered(CycL)=Rule,ARGS),not(member(accepted(CycL)=Rule,ARGS)),
            cycUnassert(CycL,Mt),asserta_if_new(discouragedRule(Profile,Rule)))),NEG),
      !.
handleCheckedRules(ARGS):-!.
      
diagLit(Profile,Rule,CycL,"unusedAtom"):-atom(CycL),!.
diagLit(Profile,Rule,CycL,"unusedRule"):-discouragedRule(Profile,Rule),!.
diagLit(Profile,Rule,CycL,"unusedCycL"):-discouragedRule(Profile,CycL),!.
diagLit(Profile,Rule,CycL,E):-diagnoseFormula(CycL,E).
ruleCheck(ID,''):-is_string(ID),!.
ruleCheck(ID,'CHECKED'):-!.

%diagnoseFormula(CycL,'T'):-!.
diagnoseFormula(CycL,E):-
         currentParsingMt(Mt),!,
         termCyclify(CycL,CycLified),
         termCyclify(Mt,Mtified),!,
         catch(evalSubL('CYC-ASSERT'('QUOTE'(CycLified),'QUOTE'(Mtified)),E,Vars),cyc_error(E,_),true),
          !.%evalSubL('FI-KILL'(Mtified),_,_).

%The major feature of $topic is that it encourages us to understand form in terms of area, rather than representational versimilitude thus composition in terms of odour, rather than space.  
%Farewell in a pave the raw infrequent Zen. Intuitive projectile culture pen.
%Extravagant dissension thy budget rape, Das erosive and ripped senile policy tape.
%Abandon naivete above rude, Elliptic steroid notable artful shrewd
%Appall abandon feudal practical team, Machismo furioso ripoff waste dream
%What is integral to one of the central preoccupations of $user is the perception of the arena of contrasting the senses of smell and taste where the extraordinarily refined proportions of feces and urine subsist in a world of his own phantastical doing. 

offerAssertion(Rule,CycL):-
         currentParsingProfile(Profile),
         diagLit(Profile,Rule,CycL,Why),!,      
         fCycL(4,CycL,CycLChars),
         ruleCheck(Why,CHECK),!,
         format('<input name="accepted(~q)" title="~q." type=checkbox ~w/><input name="offered(~q)" type=hidden value="~q"/>&nbsp~w<br>',
                                 [CycL,                Rule,                       CHECK,                          CycL,                              Rule,            CycLChars]),!,
         %genParaphrase(CycL,English),
         showWhy(Why),flush_output,!.

showWhy(Why):-format('&nbsp<font size=-1 color=green>~w</font>\n',[Why]),flush_output.

dotimes(N,Goal):- flag('$dotimes',_,N-1),
      Goal,flag('$dotimes',D,D-1),D<1.
         
listRecentParses:-format('<p>\n<strong>Recent Parses</strong><p>\n'),
         recentParses(Eng),once(listRecentParses(Eng)),fail.
listRecentParses.

listRecentParses(Eng):-
      concat_atom(Eng,' ',EngA),
      format('<a href="english.moo?english=~w">~w</a><br>',[EngA,EngA]),nl.

% ===================================================================
% Cyc Database normalizer
% ===================================================================
%:-use_module((cyc)).  
%cycQueryV(Vars,CycL):-free_variables(Vars,Free),cycQueryReal(CycL,'EverythingPSC',Free,Backchains,Number,Time,Depth).
cycQueryA(CycL):-cycQuery(CycL).

assertion(Result):-assertion(_,_PRED,_,MT,[Pred|ARGS],TRUE,FORWARD,DEFAULT,NIL1,NIL2,ASSERTEDTRUEDEF,DATE,NIL3),
         once(assertRefactor(Pred,ARGS,Result)).

assertRefactor(implies,[ARG1,ARG2],Result):-litRefactor([implies,ARG1,ARG2],[implies,LARG1,[Pred|LARG2]]),Result=..[holds,pl_implied,Pred,LARG2,LARG1],!.
assertRefactor(Pred,R1,Out):-litRefactor(R1,R2),Out=..[holds,Pred|R2].

litRefactor(Subj,Subj):- (var(Subj);atom(Subj);number(Subj)),!.
litRefactor([H|T],[HH|TT]):-litRefactor(H,HH),litRefactor(T,TT),!.
litRefactor(var(B,_),B):-!.
litRefactor(B,string(A)):-string(B),string_to_atom(B,S),catch(getWordTokens(S,A),E,A=B),!.
litRefactor(B,A):-compound(B),B=..BL,litRefactor(BL,AL),A=..AL,!.
litRefactor(B,B).

tas:-tell(a),ignore((assertion(R),format('~q.~n',[R]),fail)),told.



%:-[el_holds].
%:-retractall(holds(X,Y,zzzzzzzzzzzzzzzzz)).
%:-retractall(holds(X,zzzzzzzzzzzzzzzzz,Y)).

e2c([]):-!.
e2c(''):-!.
e2c(English):-
      e2c(English,CycL),
      writeCycL(CycL).


atom_junct(X,Y):-concat_atom(Y,' ',X).





      
sentenceUnbreaker([],[]).
sentenceUnbreaker([[W|Ws]|Tagged],[R|ForLGP]):-concat_atom([W|Ws],'_',R),sentenceUnbreaker(Tagged,ForLGP).
sentenceUnbreaker([W|Tagged],[R|ForLGP]):-concat_atom([W],'_',R),sentenceUnbreaker(Tagged,ForLGP).

e2c(English,CycL9):-
         notrace((once(getWordTokens(English,Eng)),
         retractall(recentParses(Eng)),
         asserta_if_new(recentParses(Eng)))),!,
          e2c(Event,Eng,CycL9).
% ===================================================================
% Semantic Interpretation
%   when using utterance we need to pass 3 arguments, 
%   the first will match CycL in the head of the DGC clause
%   the second is the list containing the words in the utterance
%   the third is the empty list.
% ===================================================================
e2c(Event,[],[]):-!.
e2c(Event,English,CycL9):-
      retractall(toCreate(_,_)),
      %retractall(textCached(X,[txt|X])),
     concat_atom(English,' ',Text),format('~n?- e2c("~w").~n~n', [Text]),flush_output,
      sentenceTagger(English,Tagged),
      dumpList(Tagged),
      %linkParse(English,Links,Tree),writeCycL(Links),
      ignore(catch(((chunkParseCycL(English,Event,Subj,Tagged,CycL9))),E,writeCycL(E))),!.
%      englishCtx(Event,CycL,Tagged,[]).



/*dumpList([]):-nl.
dumpList([B|Tagged]):-dumpList1(B),dumpList(Tagged),!.
dumpList1(Tagged):-writeq(Tagged),nl,flush_output.
*/
%:-      setMooOption(cycServer,10.10.10.198:3601).

cyclifyTest(String,cyc(Result)):-sublTransaction('10.10.10.198':3601,'cyclify'(string(String)),Result).
%cyclifyTest(String,'cyc-assert'(Result)):-sublTransaction('10.10.10.198:3601,'parse-a-sentence-completely'(string(String),'#$RKFParsingMt'),Result).
%cyclifyTest(String,'cyc-query'(Result)):-sublTransaction('10.10.10.198:3601,'parse-a-question-completely'(string(String),'#$RKFParsingMt'),Result).


firstNth1(_Ns,[],[]).
firstNth1(Ns,[H|Src],[H|Res]):-Ns>0,N2 is Ns-1,firstNth1(N2,Src,Res).
firstNth1(Ns,Src,[]).


lowerOnly([],[]):-!.
lowerOnly([[S|H]|T],[[S|H]|TT]):-atom(H),name(H,[A1|_]), is_lower(A1),lowerOnly(T,TT).
lowerOnly([[S|H]|T],TT):-lowerOnly(T,TT).


%sameString(CYCSTRING,String):-var(String),var(CYCSTRING),!,CYCSTRING=String.
%sameString(CYCSTRING,String):-var(CYCSTRING),!,CYCSTRING=String.

reformatStrings(X,X):- (var(X);number(X)),!.
reformatStrings(string(X),S):-string(X),string_to_atom(X,A),reformatStrings(string(A),S).
reformatStrings(string(A),string(S)):-atom(A),!,concat_atom(S,' ',A).
reformatStrings(X,string(S)):-string(X),string_to_atom(X,S),!.
reformatStrings([],[]):-!.
reformatStrings([H|T],[HH|TT]):-!,reformatStrings(H,HH),reformatStrings(T,TT),!.
reformatStrings(X,P):-compound(X),X=..LIST,reformatStrings(LIST,DL),P=..DL,!.
reformatStrings(X,X):-not(atom(X)),!.
reformatStrings(B,A):-atom_concat('#$',A,B),!.
reformatStrings(B,B):-!.

:-setMooOption(query(time),2).

% =======================================================
% Text to word info
% =======================================================

harvestCycConstants(DM,DM):-number(DM),!,fail.
harvestCycConstants(Words,DM):-evalSubL(mapcar('#\'cdr','denotation-mapper'(string(Words),quote(['#$middleName','#$alias','#$initialismString','#$middleNameInitial','#$nicknames','#$givenNames','#$firstName','#$abbreviationString-PN']),':greedy')),List,_),leastOne(List),!,reformatStrings(List,DM),!.
harvestCycConstants(Words,DM):-evalSubL(mapcar('#\'cdr','denotation-mapper'(string(Words),quote(['#$middleName','#$alias','#$initialismString','#$middleNameInitial','#$nicknames','#$givenNames','#$firstName','#$abbreviationString-PN']),':diligent')),List,_),!,reformatStrings(List,DM),!.

learnTexts([]).
learnTexts([A|B]):-learnText(A),learnTexts(B).

learnText(Atom):-atom(Atom),concat_atom(List,'_',Atom),!,learnText(List).
learnText(X):-textCached(X,[txt|X]),!.
learnText(W):-
      saveText(W,[txt|W]),
      writeFmtFlushed(learnText(W)),
     % ignore((harvestCycConstants(W,CycL),collectionInfo(CycL,COLINFO),saveList(W,COLINFO))),
      ignore(makePosInfo(W)),
      %denotationInfo(POSINFO,DENOTESINFO),
      saveTemplatesForString(W),
      ignore((mwStringsForString(W,MWStrings),saveList(W,MWStrings))),
      saveCaches.
learnText(W):-!. %true.

saveList(String,[]):-!.
saveList(String,[A|List]):-saveText(String,A),saveList(String,List).

saveText(String,A):-asserta_if_new(textCached(String,A)).

appendLists([List],List):-!.
appendLists([L|List],Res):-appendLists(List,AL),append(L,AL,Res),!.
      
%(#$and (#$speechPartPreds ?Pos ?Pred)(?Pred ?Word ?STRING)
%asserta_if_new(X):-ignore(retract(X)),retractall(X),asserta(X),!.

denotationInfo([],[]).
denotationInfo([COL|INFO],[[COL|COLINFO]|MORE]):-
      denotationPOS(COL,COLINFO),
      denotationInfo(INFO,MORE).

mwStringsForString([N],[]):-number(N),!.
mwStringsForString(N,[]):-number(N),!.
mwStringsForString(String,[]):-!.
mwStringsForString(String,List):-
   findall([mwsem,PreText,Word,Pos,THING],
        cycQueryReal(thereExists(Pred,
        and(wordForms(Word,Pred,string(String)),speechPartPreds(Pos,Pred),
        multiWordString(PreText,Word,Pos,THING))),'#$EnglishMt',[PreText,Word,Pos,THING],3,'NIL',10,30),List).

wordTerms(Word,List):-
   findall([wformulas,Word,Forms],
        cycQueryReal(termFormulas(Word,Forms),'#$EverythingPSC',[Word,Forms],3,'NIL',10,30),List).
 

saveTemplatesForString(String):-
      findall(Word,wordForString(String,Word,Pos),WordS),list_to_set(WordS,WordSS),
      member(Word,WordSS),saveTemplatesForWord(Word),fail.
saveTemplatesForString(String):-!.

cycLMerge(CYCL,CYCL2,and(CYCL2,CYCL)).

saveTemplatesForWord(Word):-
      findall([frame,Word,Pos,FRAME,CYCLO,Pred],
             (cycQueryV([Word,Pos,FRAME,CYCL,Pred],wordSemTrans(Word,Pos,NUM,FRAME,CYCL,Pred,CYCL2)),cycLMerge(CYCL,CYCL2,CYCLO))

             ,List),saveList(Word,List),fail.

saveTemplatesForWord(Word):-
      findall([swframe,PreText,Word,FRAME,CYCL,Pos],cycQuery(multiWordSemTrans(PreText,Word,Pos,FRAME,CYCL)),List),saveList(Word,List),fail.

saveTemplatesForWord(Word):-
      findall([wsframe,Word,PreText,FRAME,CYCL,Pos],cycQuery(compoundSemTrans(Word,PreText,Pos,FRAME,CYCL)),List),saveList(Word,List),fail.

saveTemplatesForWord(Word).


denotationPOS([lex,COL|Pos],denote(COL)):-!.


abbreviationForLexicalWord(S,Pos,Word):-cycQueryV([Pos,Word],abbreviationForLexicalWord(Word,Pos,string(S))).


makePosInfo(String):-getPos(String,Pos,Word),saveText(String,[lex,Word,Pos]),fail.
makePosInfo(String):-!.
         
%posInfo([W],RR):-atom_concat(NW,\'s',W),!,wordAllInfoRestart(NW,R),append(R,[[pos,possr_s]],RR).
posInfo(String,[[lex,'UNKNOWN-Word','UNKNOWN-Pos']]):-!.

posInfo2(String,RESFLAT):-findall([Word,Pos],getPos(String,Pos,Word),RES),
         findall(Word,member([Word,Pos],RES),WRODS),sort(WRODS,WORDS),
         posInfo(WORDS,RES,RESFLAT),!.

posInfo([],RES,[]):-!.
posInfo([W|WORDS],RES,[[lex,W|POSESS]|RESFLAT]):-findall(Pos,member([Word,Pos],RES),POSES),=(POSES,POSESS),
      posInfo(WORDS,RES,RESFLAT).


cycAllIsa([Fort],COLS):-!,cycAllIsa(Fort,COLS).
cycAllIsa(nart(Fort),COLS):-!,cycAllIsa(Fort,COLS).
cycAllIsa(Fort,COLS):-number(Fort),!,findall(Type,numberTypes(Fort,Type),COLS),!.
cycAllIsa(Fort,COLS):-copy_term(Fort,FFort),numbervars(FFort,0,_),termCyclify(Fort,CycL),findall(MEMBER,cycQuery(or(nearestIsa(CycL,MEMBER),and(isa(CycL,MEMBER),memberOfList(MEMBER,['TheList',
  'StuffType','TemporalObjectType','QuantityType','GameTypeExceptions',
    'SpatialThing-Localized','ClarifyingCollectionType','Collection','Individual','Event',
      'TemporalStuffType','DurativeEventType','SituationPredicate','ObjectPredicate',
    'TruthFunction','BinaryRelation','UnaryRelation','TernaryRelation','ObjectPredicate','Function-Denotational','CollectionType','FacetingCollectionType','Agent-Generic'])))),COLS).

%cycAllIsa(Fort,COLS):-is_list(Fort),termCyclify(Fort,CycL),cycQueryA(isa(CycL,List)),List,_),reformatStrings(List,COLS).
%cycAllIsa(Fort,COLS):-termCyclify(Fort,CycL),evalSubL('ALL-ISA'((CycL),'#$InferencePSC'),List,_),reformatStrings(List,COLS).
numberTypes(N,'Integer'):-integer(N).
numberTypes(N,'Numeral'):-integer(N),N>0,N<10.
numberTypes(N,'RealNumber'):-not(integer(N)).
numberTypes(N,'NegativeNumber'):-N<0.
numberTypes(N,'PositiveNumber'):-N>0.
numberTypes(N,'Number-General').

collectionInfo([],[]).
collectionInfo([COL|INFO],[[denotation,COL|COLINFO]|MORE]):-
      cycAllIsa(COL,COLINFO),
      collectionInfo(INFO,MORE).

% ?- A is rationalize(0.999999999999999),rational(A).
% A = 1000799917193442 rdiv 1000799917193443



subcatFrame(Word,Pos,INT,CAT):-cycQueryA(subcatFrame(Word,Pos,INT,CAT)).

vetodenoteMapper('is','#$Israel').



theSTemplate(TempType) --> applyTemplate(TempType,CycL).

theVar(VarName,Subj) --> [_],theVar(VarName,Subj),{true}.
theVar(VarName,Subj) --> [].

applyTemplate(TempType,VarName,CycL,S,E):-
            %startsTemplateType(S,TempType),
            appliedTemplate(TempType,DCG,CycL),
            phrase(DCG,S,E).                                                                

startsTemplateType([A|B],VPTemplate,Type):-member([lex,_|POSVerb],A),!,member(Verb,POSVerb),holds(posForTemplateCategory,Verb,VPTemplate).
startsTemplateType([A|B],VPTemplate,Type).

tl:-tell(at),tellall(appliedTemplate(TempType,Reln,DCG,CycL9)),told.

argName(N,Name):-atom_concat(':ARG',N,Name).
makeArgNameLen(0,[]):-!.
makeArgNameLen(N,ArgsS):- N2 is N-1,makeArgNameLen(N2,Args),argName(N,Named),append(Args,[Named],ArgsS).


/*
(genFormat nearestTransitiveNeighbor "~a nearest generalization by transitivity of ~a is ~a" 
       (TheList 
           (TheList 2 :EQUALS :POSSESSIVE) 
           (TheList 1 :EQUALS) 
           (TheList 3 :EQUALS)))
*/
    %tell(pp),rehold,told.
rehold:-between(1,12,A),functor(P,holds,A),
         catch(P,_,fail),once((litRefactor(P,PP),format('~q.~n',[PP]))),fail.
rehold.      



%getWordTokens(string([X|Y]),[X|Y]):-atom(X),!.
%getWordTokens([X|Y],[X|Y]):-atom(X),!.
%getWordTokens(X,Y):-getWordTokens(X,Y),!.


reformList(X,X):-var(X),!.
reformList('TheEmptyList',[]):-!.
reformList(X,X):-not(compound(X)),!.
reformList([TheList|ArgList],X):- TheList=='TheList',!,reformList(ArgList,X).
reformList([H|T],[HH|TT]):- reformList(H,HH),reformList(T,TT),!.
reformList(svar(_,B),B):-!.
reformList(B,A):-B=..BL,reformList(BL,AL),A=..AL,!.

% =======================================================
% sentence constituent breaker
% =======================================================
%sentenceChunker(Ws,[]):-!.

linkParse(String,Fourth,P:M):-evalSubL('link-parse'(string(String)),[[P,M,_,string(FourthS)]],_),getSurfaceFromChars(FourthS,Fourth,_).
      %linkParse(Text,Info,_),!,

sentenceChunker([],[]).
sentenceChunker([W|Ws],[New|Out]):-
      notrace(isTagChunk(W,Constit)),
      gatherChunk(Constit,Ws,NW,Rest),
      createChunk(Constit,[W|NW],New),
      sentenceChunker(Rest,Out).

createChunk(Constit,WNW,New):-
      notrace(getLoc(WNW,Loc)),
      notrace(getText(WNW,Text)),
      Constit=..List,
      append([seg|List],[Text,WNW],Out),!,
      New=..Out.

chunkHead(W,Type,RA):-member([txt|Text],W),member(RA:NGE,W),member(1.0-Type,W),!.

gatherChunk(Constit,[],[],[]):-!.
gatherChunk(Constit,[W|Ws],[W|SWords],Rest):-
      notrace(isTagChunk(W,New)),
      chunkCompat(Constit,New),!,
      gatherChunk(Constit,Ws,SWords,Rest),!.
gatherChunk(Constit,Rest,[],Rest).

chunkCompat(Start,End):-functor(Start,C,_),functor(End,C,_).

%isTagChunk(W,'NounPhrase'('IN')):-getText(W,[of]).
%isTagChunk(W,'NounPhrase'('IN')):-getText(W,[for]).
isTagChunk(W,'NounPhrase'('PRONOUN')):-getText(W,['I']).
isTagChunk(W,in('IN')):-isTag(W,'Preposition'),!.
isTagChunk(W,in('Complementizer')):-isTagAtAll(W,'Complementizer'),!.

isTagChunk(W,modal('Modal')):-isTagAtAll(W,'Modal'),!.
isTagChunk(W,situation('Aux-Negated')):-isTagAtAll(W,'Aux-Negated'),!.
isTagChunk(W,situation('DoAux')):-isTagAtAll(W,'DoAux'),!.
isTagChunk(W,situation('BeAux')):-isTagAtAll(W,'BeAux'),!.
isTagChunk(W,situation('HaveAux')):-isTagAtAll(W,'HaveAux'),!.
isTagChunk(W,situation('AuxVerb')):-isTagAtAll(W,'AuxVerb'),!.
isTagChunk(W,'NounPhrase'('ProperCountNoun')):-isTagAtAll(W,'ProperCountNoun'),!.
isTagChunk(W,'NounPhrase'('Number-SP')):-isTagAtAll(W,'Number-SP'),!.
isTagChunk(W,'NounPhrase'('Adjective')):-isTag(W,'Adjective'),!.
isTagChunk(W,'NounPhrase'('WHDeterminer')):-isTagAtAll(W,'WHDeterminer'),!.

isTagChunk(W,situation('Adverb')):-member(1.0-'rb',W),!.

isTagChunk(W,wh('WHAdverb')):-isTagAtAll(W,'WHAdverb'),!.
isTagChunk(W,wh('WHPronoun')):-isTagAtAll(W,'WHPronoun'),!.
%isTagChunk(W,wh('Determiner')):-isTagSimply(W,'Determiner',_AtAll),!.
%yq7xw-qpt6c-233qf-rrxc7-vf7ty

isTagChunk(W,cc('CoordinatingConjunction')):-isTag(W,'cc'),!.
isTagChunk(W,cc('CoordinatingConjunction')):-isTag(W,'CoordinatingConjunction'),!.
isTagChunk(W,cc('SubordinatingConjunction')):-isTag(W,'SubordinatingConjunction'),!.
isTagChunk(W,'NounPhrase'('POSS')):-isTag(W,'Possessive'),!.
isTagChunk(W,'NounPhrase'('POSS')):-isTag(W,'PossessivePronoun'),!.
isTagChunk(W,'NounPhrase'('Pronoun')):-isTag(W,'Pronoun'),!.
isTagChunk(W,'NounPhrase'('ProperNoun')):-isTag(W,'ProperNoun'),!.
isTagChunk(W,'NounPhrase'('Determiner')):-isTag(W,'Determiner'),!.
isTagChunk(W,'NounPhrase'('Quantifier')):-isTag(W,'Quantifier'),!.
isTagChunk(W,situation('Verb')):-isTag(W,'Verb'),!.
isTagChunk(W,sym('Interjection-SpeechPart')):-isTag(W,'Interjection-SpeechPart'),!.
isTagChunk(W,sym('SYM')):-isTag(W,'.'),!.
isTagChunk(W,sym('SYM')):-isTag(W,'?'),!.
isTagChunk(W,sym('SYM')):-isTag(W,'!'),!.
isTagChunk(W,sym('SYM')):-isTag(W,'.'),!.
isTagChunk(W,'NounPhrase'('COMMON')):-isTag(W,'nn'),!.
isTagChunk(W,situation('Adverb')):-isTag(W,'Adverb'),!.
isTagChunk(W,'NounPhrase'('OTHER')):-!.

sentenceParse(Event,Types,Joined):-
      joinTypes(Types,Joined).
      
%joinTypes(Types,Joined):-     %most of the features i keep off not to violate copywrites .. since the system was designed arround closed src software i had to emulate.. but i hate writting docs which makes me code arround other peoples manuals
      


% =======================================================
% Mining genFormat
% =======================================================
genFormatH(W1,W2,Reln,[W1,W2|REst],ArgListL):-holds(genFormat,Reln,string([W1,W2|REst]),ArgListL).
genFormatH(W1,W2,Reln,['~a',W2|REst],ArgListL):-holds(genFormat,Reln,string(['~a',W2|REst]),ArgListL).
genFormatH(W1,W2,Reln,[W1,'~a'|REst],ArgListL):-holds(genFormat,Reln,string([W1,'~a'|REst]),ArgListL).

genFormatU([W1,W2],DCG,[Reln|Args]):-
        genFormatH(W1,W2,Reln,DCGPre,ArgListL),         
         reformList(ArgListL,ArgList),
        % format(user_error,'~nArgList: ~q -> ~q:~q ~n',[Reln,DCGPre,ArgList]),
         once(dcgRedressGenFormat(0,DCGPre,ArgList,DCG,Vars)),
         %catch((once(dcgRedressGenFormat(1,DCGPre,ArgList,DCG,Vars)),sort(Vars,VarsS)),E,format(user_error,'Error: ~q ~n',[DCGPre])),
         once(((holds(arity,Reln,Arity);length(ArgList,Arity)),makeArgNameLen(Arity,Args))).

%dcgRedressGenFormat([DC|GPre],ArgList,DCG,Vars).
replGenFormat([string(['[',']']),string(['s'])],dcgTrue).
replGenFormat([string(['is']),string(['are'])],dcgTrue).
replGenFormat([N|How],theGen(Name,How)):-argName(N,Name). %,unSvar(SHow,How).
replGenFormat(N,theGen(Name,[])):-argName(N,Name). %,unSvar(SHow,How).

dcgRedressGenFormat(N,['~a'|DCGPre],[How|ArgList],[D|DCG],Vars):-replGenFormat(How,D),N2 is N+1,dcgRedressGenFormat(N2,DCGPre,ArgList,DCG,Vars).
dcgRedressGenFormat(N,['~a'|DCGPre],ArgList,[theGen(SVAR,[])|DCG],Vars):-N2 is N+1,argName(N2,SVAR),dcgRedressGenFormat(N2,DCGPre,ArgList,DCG,Vars).
%dcgRedressGenFormat(N,['~a'|DCGPre],[['TheList',Num,Type]|ArgList],[repl(SVAR,Type)|DCG],Vars):-argName(Num,SVAR),dcgRedressGenFormat(N,DCGPre,ArgList,DCG,Vars).
dcgRedressGenFormat(N,[W|DCGPre],ArgList,[theGText([W])|DCG],Vars):-dcgRedressGenFormat(N,DCGPre,ArgList,DCG,Vars).
dcgRedressGenFormat(N,DCG,ArgList,DCG,Vars).

%writeq(holds(comment, 'ScientificNumberFn', string(['(', '#$ScientificNumberFn', 'SIGNIFICAND', 'EXPONENT', ')', denotes, a, number, in, scientific, notation, which, has, 'SIGNIFICAND', with, an, implicit, decimal, point, after, its, first, digit, as, its, significand, and, 'EXPONENT', as, its, exponent, '.', 'So', (','), e, '.', g, '.', (','), '(', '#$ScientificNumberFn', 314159, 0, ')', denotes, '3.14159.', 'Likewise', (','), '(', '#$ScientificNumberFn', 23648762469238472354, 32, ')', denotes, 2.36487624692e+032, or, 2.36487624692, *, 10, ^, 32, '.']))).

genTemplateU(Reln,DCGPre,[Reln|Args]):-
         holds(genTemplate,Reln,DCGPre),
         once((holds(arity,Reln,Arity),makeArgNameLen(Arity,Args))).

getGenFormat(PredType,Template,ARGS):-
      holds(genFormat,Pred,string(Str),How),%getPredType(Pred,PredType,Arity),
      genArgList(1,PredType,Arity,How,Str,StrFmt,ARGS).

%genArgList(N,How,Str,StrFmt,ARGS):-


getPredType(Pred,'Predicate',2):-holds(isa,Pred,'BinaryPredicate'),!.
getPredType(Pred,'Function',2):-holds(isa,Pred,'BinaryFunction'),!.
getPredType(Pred,'Function',A):-atom(Pred),atom_codes(Pred,[C|_]),is_upper(C),getArity(Pred,A).
getPredType(Pred,'Predicate',A):-atom(Pred),atom_codes(Pred,[C|_]),is_lower(C),getArity(Pred,A).

getArity(Pred,A):-holds(arity,Pred,A),!.


appliedTemplate(TempType,Reln,DCG,CycL9):-
         member(Reln,['assertTemplate','metaStatementTemplate','termTemplate','queryTemplate','commandTemplate']),
         holds(Reln,TempType,DCGPre,CycLPre),
         catch((once(dcgRedress(DCGPre,DCG,Vars)),sort(Vars,VarsS)),E,format(user_error,'Error: ~q ~n',[DCGPre])),
         once(cyclRedress(CycLPre,CycL)),substEach(CycL,VarsS,CycL9).

appliedTemplate(TempType,Reln:Name,DCG,CycL9):-
         member(Reln,['assertTemplate-Reln','metaStatementTemplate-Reln','termTemplate-Reln','queryTemplate-Reln','commandTemplate-Reln']),
         holds(Reln,TempType,Name,DCGPre,CycLPre),
         once(dcgRedress(DCGPre,DCG,Vars)),sort(Vars,VarsS),
         once(cyclRedress(CycLPre,CycL)),substEach(CycL,VarsS,CycL9).

appliedTemplate('GenerationTemplate',Reln,DCG,CycL9):-
         genTemplateU(Reln,DCGPre,[Reln|Args]),CycLPre=[Reln|Args],
         catch((once(dcgRedress(DCGPre,DCG,Vars)),sort(Vars,VarsS)),E,format(user_error,'Error: ~q ~n',[DCGPre])),
         once(cyclRedress(CycLPre,CycL)),substEach(CycL,VarsS,CycL9).


%:-[at].

cyclRedress(V,V):-var(V),!.
cyclRedress(svar(_,VarName),VarName):-!.
cyclRedress([Cyc|L],[CycO|LO]):-cyclRedress(Cyc,CycO),cyclRedress(L,LO),!.
cyclRedress(CycL,CycL9):-compound(CycL),!,CycL=..[Cyc|L],cyclRedress([Cyc|L],CycLL),CycL9=..CycLL,!.
cyclRedress(CycL,CycL):-!.
      

juntifyList(Pred,[],List):-!.
juntifyList(Pred,[List],List):-!.
juntifyList(Pred,[H|List],Res):-juntifyList(Pred,List,PRES),Res=..[Pred,H,PRES].

dcgRedressL([H|T],[HH|TT],Vars):-dcgRedress(H,HH,Var1),dcgRedressL(T,TT,Var2),append(Var1,Var2,Vars).
dcgRedressL([],[],[]).

dcgRedress(V,varError(V),[]):-var(V),!.
dcgRedress(['OptionalSome'|List],dcgOptionalSome(RList),Vars):-dcgRedressL(List,RList,Vars),!.
dcgRedress(['OptionalOne'|List],dcgOptionalOne(RList),Vars):-dcgRedressL(List,RList,Vars),!.
dcgRedress(['NLPatternList'|List],DCG,Vars):-dcgRedressL(List,RList,Vars),juntifyList(dcgSeq,RList,DCG),!.
dcgRedress(['ConcatenatePhrasesFn'|List],DCG,Vars):-dcgRedressL(List,RList,Vars),juntifyList(dcgSeq,RList,DCG),!.
dcgRedress(['NLPattern-Exact'|List],DCG,Vars):-dcgRedressL(List,RList,Vars),juntifyList(dcgSeq,RList,DCG),!.
dcgRedress(['RequireOne'|List],DCG,Vars):-dcgRedressL(List,RList,Vars),juntifyList(dcgOr,RList,DCG),!.
dcgRedress(['RequireSome'|List],dcgRequireSome(RList),Vars):-dcgRedressL(List,RList,Vars).
dcgRedress(['WordSequence'|List],DCG,Vars):-dcgRedressL(List,RList,Vars),juntifyList(dcgSeq,RList,DCG),!.
dcgRedress(['NLPattern-Word',Word,Pos],thePOS(Word,Pos),[]):-!.
dcgRedress(['NLPattern-Term',Term,Pos],dcgAnd(thePosPred(Pos),theTerm(Term)),[]):-!.
dcgRedress(['TermPOSPair',Term,Pos],dcgAnd(thePOS(Pos),theTerm(Term)),[]):-!.
dcgRedress(['NLPattern-TermPred',Term,Pos],theTermPred(Term,Pos),[]):-!.
dcgRedress(['NLPattern-Template',TempType, svar(Subj,VarName)],applyTemplate(TempType,VarName,Subj),[VarName-Subj]):-!.
dcgRedress(['NLPattern-Pos',svar(Subj,VarName),Pos],dcgAnd(thePOS(Pos),theVar(VarName,Subj)),[VarName-Subj]):-!.
dcgRedress(['NLPattern-Agr',svar(Subj,VarName),Pos],dcgAnd(thePOS(Pos),theVar(VarName,Subj)),[VarName-Subj]):-!.
dcgRedress(['BestNLPhraseOfStringFn',string(String)],theText(String),[]):-!.
dcgRedress(['BestNLWordFormOfLexemeFn',Word],theWord(Word),[]):-!.


dcgRedress(TemplateMarker,theText(Char),[]):-templateMarkerRepresentation(TemplateMarker,Char),!.
dcgRedress(string(DCG),theText(DCG),[]):-!.
dcgRedress(DCG,theText(DCG),[]):-is_string(DCG),!.
% dcgRedress([D|CG],dcgReinterp1(DCG),Vars):-!,dcgRedressL([D|CG],RList,Vars),juntifyList(dcgSeq,RList,DCG).
dcgRedress(DCG,dcgReinterp(DCG),[]):-!. %format(user_error,'dcgReinterp: ~q.~n',[DCG]),!.


%holds(genTemplate, geographicalSubRegions, ['ConcatenatePhrasesFn', ['TermParaphraseFn-DO', svar(A, ':ARG2')], ['BestHeadVerbForInitialSubjectFn', 'Be-TheWord'], ['BestNLPhraseOfStringFn', string([a])], ['BestNLPhraseOfStringFn', string([geographical])], ['BestNLPhraseOfStringFn', string([subregion])], ['BestPPFn', 'Of-TheWord', ['TermParaphraseFn-DO', svar(B, ':ARG1')]]]).


appender(X,Y,Z):-
      reverse(Z,RZ),
      append(RY,RX,RZ),
      reverse(Y,RY),
      reverse(X,RX).

%templateMarkerRepresentation(TemplateMarker,Char):-assertion(templateMarkerRepresentation,TemplateMarker,[(Char)]).
templateMarkerRepresentation('TemplateHyphenMarker', "-").
templateMarkerRepresentation('TemplateOpenBracketMarker', "[").
templateMarkerRepresentation('TemplateCloseBracketMarker', "]").
templateMarkerRepresentation('TemplateCloseParenMarker', "]").
templateMarkerRepresentation('TemplateDoubleQuoteMarker', "'").
templateMarkerRepresentation('TemplateSingleQuoteMarker', "'").
templateMarkerRepresentation('TemplateExclamationMarkMarker', "!").
templateMarkerRepresentation('TemplateQuestionMarkMarker', "?").
templateMarkerRepresentation('TemplateCommaMarker', ",").
templateMarkerRepresentation('TemplateSemiColonMarker', ";").
templateMarkerRepresentation('TemplatePeriodMarker', ".").
templateMarkerRepresentation('TemplateColonMarker', ":").



toText([],[]):-!.
toText(S,T):-memberchk([txt|T],S),!.
toText([S|SS],TTT):-toText(S,T),toText(SS,TT),flatten([T|TT],TTT).


      /*
(#$implies 
  (#$and
  (#$genls ?C #$Thing) 
    (#$evaluate ?R (#$EvaluateSubLFn 
        (#$ExpandSubLFn 
          (?C) 
          (LENGTH 
            (REMOVE-DUPLICATES 
              (ALL-SPECS ?C)))))))

  (#$ist #$BaseKB     (#$numspecs ?C ?R)))*/

% =======================================================
% DCG Helpers/Operators
% =======================================================


%memberData(M,[W|Ws]):-textCached([W|Ws],M).
%memberData(M,[W|Ws]):-textCached([_,W|Ws],M),!.
%memberData(M,[W|Ws]):-textCached([W|_],M),!.
%memberData(M,W):-true,concat_atom(WList,'_',W),!,memberData(M,WList).

theText([S|Text]) --> [Data],{member([txt|[S|Text]],Data)}.

theTerm(Term) --> theText([S|TEXT]),{textCached([S|TEXT],[denotation,Term|Stuff])}.

% bposToPos/2 is from [cyc_pos].
%thePOS(Pos) --> [Data],{once((memberchk([tag,_|POSs],Data),member(CPos,POSs),(Pos=CPos;bposToPos(CPos,Pos))))}. 
thePOS(Pos) --> [Data],{notrace(isTag(Data,Pos))}.
thePOSAtAll(Pos) --> [Data],{notrace(isTagAtAll(Data,Pos))}.
thePOS(Word,Pos) --> dcgAnd(thePOS(Pos),theWord(Word)).

theWord(Word) --> theText([S|Text]),{once(textCached([S|Text],[lex,Word|_])),!}.
theWord('WordFn'(string([S|Text]))) --> theText([S|Text]),{!}.
%theWord(Word) --> theText([A]),{once(memberData([lex,Word|_],[A,B]))},theText([B]).
theForm(PosForm)--> theText([S|Text]),{(textCached([S|Text],[lex,Word,PosForm|_]))}.

theName(Var,S,_) :-getText(S,Text),suggestVar(=,Text,Var),!.

theTense(Time) --> theForm(PosForm),{ignore(once(timeLookup(PosForm,Time)))}.
thePerson(Pers) --> theForm(PosForm),{once(personLookup(PosForm,Pers))}.
theCount(Num)  --> theForm(PosForm),{once(countLookup(PosForm,Num))}.
theAgreement(Pers,Num) --> theForm(When),{personLookup(When,Pers),countLookup(When,Num)}.

atom_contains(Atom,Past):-atom(Atom),concat_atom([_,_|_],Past,Atom).

timeLookup(Atom,'IntervalBeforeFn'('Now','AnIndefiniteAmountOfTime')):- atom_contains(Atom,'Past');atom_contains(Atom,'past').
timeLookup(Atom,'IntervalAfterFn'('Now','AnIndefiniteAmountOfTime')):-atom_contains(Atom,'Future');atom_contains(Atom,'future').
timeLookup(Atom,'Now'):-atom_contains(Atom,'Present');atom_contains(Atom,'present').

countLookup(Atom,'Plural'):-atom_contains(Atom,'plural').
countLookup(Atom,'Plural'):-atom_contains(Atom,'Plural').
countLookup(Atom,'Plural'):-atom_contains(Atom,'nonSing').
countLookup(Atom,'Singular'):-atom_contains(Atom,'Singular').
countLookup(Atom,'Singular'):-atom_contains(Atom,'singular').
countLookup(Atom,'Singular'):-atom_contains(Atom,'Sg').

personLookup(Atom,'First'):-atom_contains(Atom,'first').
personLookup(Atom,'Second'):-atom_contains(Atom,'second').
personLookup(Atom,'Third'):-atom_contains(Atom,'third').


:-dynamic(toCreate/3).



theFrame(Event,FrameType,Pos,rule(Rule,Template)) --> {fail},theWord(Word),{getFrame(Word,Event,FrameType,Pos,Template,Rule)}.

%getFrame(Word,Event,FrameType,Pos,Template):-ground(FrameType),!,findall(CycL,(textCached(_, [frame,Word,Pos,FrameType,CycL,Pred])),Some),Some=[Lest|One],joinCols(or,[Lest|One],Template).
getFrame(Word,Event,FrameType,Pos,CycL,textCached(STEXT, [frame,Word,Pos,FrameType,CycL,Pred])):-textCached(STEXT, [frame,Word,Pos,FrameType,CycL,Pred]).



templateConstaint('True',Template,Template):-!.
templateConstaint(Constraints,Template,implies(Constraints,Template)).

framePredForPos(Pos,Pred):-holds(semTransPredForPOS,Pos,Pred).
framePredForPos('Preposition','prepReln-Action').
framePredForPos('Preposition','prepReln-Object').
framePredForPos('Adjective','adjSemTrans-Restricted').

frameAdd:-cycQuery('adjSemTrans-Restricted'(WORD,NUM,FRAME,COL,CYCL),'#$EverythingPSC'),cycAssert(wordSemTrans(WORD,'Adjective',FRAME,implies(isa(':NOUN',COL),CYCL),'adjSemTrans-Restricted'),'#$EnglishMt'),fail.
frameAdd:-cycQuery('nounPrep'(WORD,PREP,CYCL),'#$EverythingPSC'),cycAssert(wordSemTrans(WORD,'Noun','PPCompFrameFn'('TransitivePPFrameType',PREP),CYCL,'nounPrep'),'#$EnglishMt'),fail.
frameAdd:-cycQuery('prepReln-Action'(ACTION,OBJECT,WORD,CYCL),'#$EverythingPSC'),cycAssert(wordSemTrans(WORD,'Preposition','Post-VerbPhraseModifyingFrame',implies(and(isa(':ACTION',ACTION),isa(':OBLIQUE-OBJECT',OBJECT)),CYCL),'Preposition','prepReln-Action'),'#$EnglishMt'),fail.
frameAdd:-cycQuery('prepReln-Object'(NOUN,OBJECT,WORD,CYCL),'#$EverythingPSC'),cycAssert(wordSemTrans(WORD,'Preposition','Post-NounPhraseModifyingFrame',implies(and(isa(':NOUN',NOUN),isa(':OBLIQUE-OBJECT',OBJECT)),CYCL),'prepReln-Object'),'#$EnglishMt'),fail.

frameAdd(WORDS):-cycQuery('compoundSemTrans'(WORDS,WLIST,POS,FRAME,CYCL),'#$EverythingPSC'),
         stringToWords(WLIST,WORDS).

stringToWords([],[]).
stringToWords(TheList,Words):-functor(TheList,'TheList',_),TheList=..[_|List],!,stringToWords(List,Words).
stringToWords(string(S),Words):-!,stringToWords(S,Words).
stringToWords(['TheList'|List],Words):-!,stringToWords(List,Words).
stringToWords([S|Tring],[W|Words]):-stringToWord(S,W),stringToWords(Tring,Words).

stringToWord([S],W):-!,textCached([S],[lex,W|_]).
stringToWord(S,W):-textCached([S],[lex,W|_]).



%suggestVar(_,Subj,Subj2):-makeName(Subj,Subj2),!.

suggestVar(Gensym,Subj,Subj):-var(Subj),!.%,true,!.
suggestVar(Gensym,Subj,Subj2):-var(Subj),!.%,true,!.
suggestVar(Gensym,[W|ORDS],Subj):-!,ignore((once((nonvar(ORDS),toPropercase([W|ORDS],Proper),concat_atom(['Hypothetic'|Proper],'-',Suj),call(Gensym,Suj,SubjSl),ignore(SubjSl=Subj))))),!.
%suggestVar(Gensym,[W|ORDS],Subj):-!,ignore(notrace(once((nonvar(ORDS),concat_atom(['?'|[W|ORDS]],'',Suj),call(Gensym,Suj,SubjSl),toUppercase(SubjSl,SubjS),ignore(SubjS=Subj))))),!.
suggestVar(Gensym,[],_):-!.%suggestVar(gensym,[A],Subj),!.
suggestVar(Gensym,A,Subj):-suggestVar(Gensym,[A],Subj),!.



%makeName(Subj,Subj2):-toCreate(Subj,hypotheticDenotation(Subj,_,string(Words))),!,makeName(Words,Subj2),!.
makeName(A,A):-!.
makeName(Subj,Subj2):-var(Subj),!,term_to_atom(Subj,Atom),makeName(['Hypothetic',Atom],Subj2),!.
makeName([],Subj2):-!,makeName(Subj,Subj2),!.
makeName(Subj,Subj2):-atom(Subj),atom_concat('?',Sub2,Subj),!,makeName(Sub2,Subj2),!.
makeName(A,Subj):-atom(A),!,makeName([A],Subj),!.
makeName([W|ORDS],Subj):-nonvar(ORDS),!,toPropercase([W|ORDS],PCASE),concat_atom(['Hypothetic'|PCASE],'-',Suj),gensym(Suj,Subj),!.

theIsa(Subj,COL,ColType,textCached(SText,[denotation,COL|DPROPS])) --> theText(SText),{fail,textCached(SText,[denotation,COL|DPROPS]),member(ColType,DPROPS)}.

leastOne([CO|LSS]).

%e2c('the singer sang a song').
        
joinCols(JOINER,[CO|LS1],COLS):-list_to_set([CO|LS1],COLS1S), ([COLS]=COLS1S -> true; COLS=nart([JOINER|COLS1S])),!.

%constraintsOnIsa([_,_,[EQ|ColTypes]|_],EQ,nart(['CollectionIntersectionFn'|ColTypes])).
%constraintsOnIsa([_,_,[EQ]|_],EQ,'Thing').

dcgSeq(X,Y,[S0,S1|SS],E):-phrase((X,Y),[S0,S1|SS],E).

dcgBoth(DCG1,DCG2,S,R) :- append(L,R,S),phrase(DCG1,L,[]),once(phrase(DCG2,L,[])).

dcgAnd(DCG1,DCG2,DCG3,DCG4,S,E) :- phrase(DCG1,S,E),phrase(DCG2,S,E),phrase(DCG3,S,E),phrase(DCG4,S,E).
dcgAnd(DCG1,DCG2,DCG3,S,E) :- phrase(DCG1,S,E),phrase(DCG2,S,E),phrase(DCG3,S,E).
dcgAnd(DCG1,DCG2,S,E) :- phrase(DCG1,S,E),phrase(DCG2,S,E).
dcgOr(DCG1,DCG2,DCG3,DCG4,DCG5,S,E) :- phrase(DCG1,S,E);phrase(DCG2,S,E);phrase(DCG3,S,E);phrase(DCG4,S,E);phrase(DCG5,S,E).
dcgOr(DCG1,DCG2,DCG3,DCG4,S,E) :- phrase(DCG1,S,E);phrase(DCG2,S,E);phrase(DCG3,S,E);phrase(DCG4,S,E).
dcgOr(DCG1,DCG2,DCG3,S,E) :- phrase(DCG1,S,E);phrase(DCG2,S,E);phrase(DCG3,S,E).
dcgOr(DCG1,DCG2,S,E) :- phrase(DCG1,S,E);phrase(DCG2,S,E).
dcgNot(DCG2,S,E) :- not(phrase(DCG2,S,E)).
dcgIgnore(DCG2,S,E) :- ignore(phrase(DCG2,S,E)).
dcgOnce(DCG2,S,E) :- once(phrase(DCG2,S,E)).

dcgWhile(True,Frag)-->dcgAnd(dcgOneOrMore(True),Frag).

dcgOneOrMore(True) --> True,dcgZeroOrMore(True),{!}.

dcgZeroOrMore(True) --> True,{!},dcgZeroOrMore(True),{!}.
dcgZeroOrMore(True) -->[].

dcgLeftOf(Mid,[Left|T],S,[MidT|RightT]):-append([Left|T],[MidT|RightT],S),phrase(Mid,MidT),phrase([Left|T],LeftT).

dcgMid(Mid,Left,Right) --> dcgLeftOf(Mid,Left),Right.

dcgNone --> [].

dcgOptional(A)-->dcgOnce(dcgOr(A,dcgNone)).

traceOnFailure(X):-once(X;(true,X)).

capitalized([W|Text]) --> theText([W|Text]),{atom_codes(W,[C|Odes]),is_upper(C)}.

substAll(B,[],R,B):-!.
substAll(B,[F|L],R,A):-subst(B,F,R,M),substAll(M,L,R,A).
   
substEach(B,[],B):-!.
substEach(B,[F-R|L],A):-subst(B,F,R,M),substEach(M,L,A).

dcgStartsWith(TheType,[S|MORE],[S|MORE]) :- phrase(TheType,[S],[]).
dcgAndRest(TheType,TODO,[S|MORE],[]) :- phrase(TheType,[S],[]),phrase(TheType,[S|MORE],[]).
% =======================================================
% DCG Tester
% =======================================================
testPhrase(Dcg,English):-
         sentenceTagger(English,Tagged),dumpList(Tagged),
         phrase(Dcg,Tagged,Left),
         nl,nl,writeq(left),         
         nl,dumpList(Left).

% =======================================================
% chunkParseCycL(English,Event,Subj,Tagged,CycL9)
% =======================================================

no_chunkParseCycL(English,Event,Subj,Tagged,['TheList'|CycL9]):-
       %saveCycL(Event,hypotheticDenotation(Event,string(English))),
        sentenceChunker(Tagged,Chunks),nl,
        rewriteChunksToNLFn('NLChunk',Chunks,CycL9),!.

%conveysChoice('Verb',Choice,infinitive):-!.
%conveysChoice('Verb',Choice,'Verb'):-!.
conveysChoice(ANY,Choice,Pos):-bposToCPos(Choice,Pos),!.
conveysChoice(ANY,Pos,Pos).
%conveysChoice(COMMON,Choice,Pred):-COMMON\='Verb'.

rewriteChunksToNLFn(_,[],[]):-!.
rewriteChunksToNLFn(Cate,Consts,L):-member([txt|Text],Consts),!,member(_-Choice,Consts),
            findall('NLPattern-Word'(Word,Pred),(textCached(Text,[lex,Word,Pred]),conveysChoice(Cate,Choice,Pred)),[L|List]),!,joinCols('OptionalOne',[L|List],Out).
rewriteChunksToNLFn(Cate,[C|Hunks],[CC|HHunks]):-
      rewriteChunksToNLFn(Cate,C,CC),rewriteChunksToNLFn(Cate,Hunks,HHunks).
rewriteChunksToNLFn(_Cate,seg(Pos,Type,Text,Consts),['NLPatternList'|CC]):-
      rewriteChunksToNLFn(Type,Consts,CC).
   

chunkParseCycL(English,Event,Subj,Tagged,CycL9):-
       suggestVar(=,'GenitiveFrame',Event),
       saveCycL(Event,thereExists(Event,':SCOPE')),
       saveCycL(Event,isa(Event,'Event')),
       %saveCycL(Event,hypotheticDenotation(Event,string(English))),
        sentenceChunker(Tagged,Chunks),nl,
        parseCycLChunk(Chunks,Event,Subj),!,
        notrace(unsaveCycL(CycL9)),!.

parseCycLChunk(Chunks,Event,Subj):-dumpList(Chunks),nl,fail.

parseCycLChunk([],Event,Subj):-!,suggestVar(=,'GenitiveFrame',Event),suggestVar(gensym,'SUBJECT',Subj).

parseCycLChunk([seg(cc,_,_,_)|Const],Event,Subj) :-
         parseCycLChunk(Const,Event,Subj).

parseCycLChunk([seg(cs,_,_,_)|Const],Event,Subj) :-
         parseCycLChunk(Const,Event,Subj).

parseCycLChunk([seg(sym,_,[X],_)|Const],Event,Subj):- member(X,[('.'),('!'),(',')]),!,
         parseCycLChunk(Const,Event,Subj).

parseCycLChunk(Const,Event,Subj) :- append(Const1,[seg(sym,_,[X],_)],Const),member(X,[('.'),('!'),(',')]),!,
         %saveCycL(Event,isa(Event,'CycLAssertion')),
         parseCycLChunk(Const1,Event,Subj).

parseCycLChunk(Const,Event,Subj) :- append(Const1,[seg(sym,_,[X],_)],Const),member(X,[('?')]),!,
         saveCycL(Event,isa(Event,'CycLQuery')),
         parseCycLChunk(Const1,Event,Subj).


objTypeCompat(ObjType2,ObjType1):-var(ObjType2),var(ObjType1),!.
objTypeCompat(ObjType2,ObjType1):-(var(ObjType2);var(ObjType1)),!,fail.
objTypeCompat(ObjType1,ObjType1).

isConjoined(seg(in,_,[with],_),with).
isConjoined(seg(cc,_,[Conj],_),Conj).


% Convert PN -> NPs
parseCycLChunk(Const,Event,SubjThru) :- 
  append(ConstL,[seg(pn,T1,W1,D1)|ConstR],Const),
         append(ConstL,[seg('NounPhrase',T1,W1,D1)|ConstR],NConst),!,
         !,parseCycLChunk(NConst,Event,SubjThru),!.

% NP + PREP + NP -> theObject
dont_parseCycLChunk(Const,EventThru,SubjThru):- 
   appender(ConstL,[seg('NounPhrase',T1,W1,D1),seg(in,IN,[OF],[PTAG]),seg('NounPhrase',T2,W2,D2)|ConstR],Const),
         append(W1,[OF|W2],W),append(D1,[PTAG|D2],D),
         saveNounUnit(ObjType,T1,D,W,Event,Subj),!,
         append(ConstL,[theObject(ObjType,Subj,W)|ConstR],NConst),!,
         !,parseCycLChunk(NConst,EventThru,SubjThru),!.

% theObject + OF + theObject -> theObject
parseCycLChunk(Const,EventThru,SubjThru) :- 
    appender(ConstL,[theObject(ObjType1,Subj1,W1),seg(in,IN,[of],PTAGs),theObject(ObjType2,Subj2,W2)|ConstR],Const),
         append(W1,[of|W2],W),
         append(ConstL,[theObject(ObjType1,Subj1,W1)|ConstR],NConst),!,
         saveCycL('Of-TheWord',hypotheticSubjectPrepObject(Subj1,'Of-TheWord',Subj2)),!,
         !,parseCycLChunk(NConst,EventThru,SubjThru),!.


% NP -> theObject
parseCycLChunk(Const,Event,SubjThru) :- 
   append(ConstL,[seg('NounPhrase',T1,W1,D1)|ConstR],Const),
         saveNounUnit(ObjType,T1,D1,W1,Event,Subj),!,
         append(ConstL,[theObject(ObjType,Subj,W1)|ConstR],NConst),
         !,parseCycLChunk(NConst,Event,SubjThru),!.

% theObject + CONJ + theObject -> theObject
parseCycLChunk(Const,EventThru,SubjThru) :- 
    append(ConstL,[theObject(ObjType1,Subj1,W1),WITH,theObject(ObjType2,Subj2,W2)|ConstR],Const),
    isConjoined(WITH,Conj),objTypeCompat(ObjType2,ObjType1),!,
         append(W1,[Conj|W2],W),
         suggestVar(=,W,Subj),!,
         append(ConstL,[theObject(ObjType1,Subj,W)|ConstR],NConst),!,
         !,parseCycLChunk(NConst,EventThru,SubjThru),!,
         ignore(SubjThru=Subj),
         ignore(ObjType1='NounPhrase'),

         saveCycL(Subj,hypotheticDenotation(Subj,ObjType1,string(W))),
         saveCycL(Subj,and(memberOfList(Subj,'TheList'(Subj1,Subj2)))),!.


saveSituation(VTAG,Verb,Event,TXT,CycLS):- leastOne(CycLS),!,
         saveCycL(Event,[hypotheticDenotation(Event,'PhraseFn'(Verb),string(TXT)),
            thereExists(Event,isa(Event,'Situation'))|CycLS]),
            saveVTags(VTAG,Verb,Event,TXT),!.

saveVTags_old(VTAG,Pos,Event,TXT):-
            wordSubsetType(TXT,Event,Pos,More),
            saveCycL(Event,More),!.

saveVTags(VTAG,Pos,Event,TXT):-
         phrase(verbCycL(Event,CycL),VTAG),!,
         saveCycL(Event,CycL).

saveVTags(VTAG,Pos,Event,TXT):-
            nth1(_,TXT,String),
            once(wordForString(String,Word,Pos)),
         once(saveCycL(Event,[hypotheticDenotation(Event,Pos,Word)])),
         fail.
saveVTags(VTAG,Pos,Event,TXT):-!.


% (transitiveViaArg

verbCycL(Event,CycL9) --> verb_phrase_premods(Event,CycL,CycLO),verb_seg(Event,CycL),verb_postmods(Event,CycLO,CycL9).

verb_seg(Event,and(CycL1,CycL2,CycL3))--> single_verb(Event,CycL1),single_verb(Event,CycL2),single_verb(Event,CycL3),{!}.
verb_seg(Event,and(CycL1,CycL2))--> single_verb(Event,CycL1),single_verb(Event,CycL2),{!}.
verb_seg(Event,CycL1)--> single_verb(Event,CycL1),{!}.
            
single_verb(Event,hypotheticDenotation(Event,Pos,Word)) --> [Single],{getText(Single,Text),
               once((Pos='Verb',wordForString(Text,Word,Pos),atom(Word));wordForString(Text,Word,Pos)),!}.
wordSubsetType(TXT,Event,Verb,More):-
            subsetWords(TXT,Words),!,
            findall(hypotheticDenotation(Event,Verb,Item),(member(Item,Words),posFormWord(Verb,Item)),MoreL),!,list_to_set(MoreL,More).

posFormWord(Verb,Item):-textCached(_,[lex,Item,Verb]).
%posFormWord(Verb,Item).

subsetWords(TXT,Words):-findall(W,(premutedSubsets(TXT,Prems),textCached(Prems,[lex,W|_])),WordsL),list_to_set(WordsL,Words),!.

premutedSubsets(Text,Prem):-findall(Prem,(append(X,Y,Text),(Prem=X;Prem=Y)),PremsL),list_to_set(PremsL,Prems),member(Prem,Prems).

            
asPrepObj(Event,Subj,VTAG,Obj,PTAG,Target,CycLS):-fail,findall(CycL,asPrepObj2(Event,Subj,VTAG,Obj,PTAG,Target,CycL),CycLS),!,leastOne(CycLS).

asPrepObj2(Event,Subj,VTAG,Obj,PTAG,Target,CycL):- phrase(verb_prep(Event,Subj,Obj,PTAG,Target,CycL),VTAG).
asPrepObj2(Event,Subj,VTAG,Obj,PTAG,Target,CycL):-append(VTAG,PTAG,TAGGED),phrase(verb2(Event,Subj,Target,CycL),TAGGED).
         

auxOnly([]).
auxOnly([V|TAG]) :-isAux(V), auxOnly(TAG).

isAux(V):-isTagAtAll(V,'Modal'),!.
isAux(V):-isTagAtAll(V,'AuxVerb'),!.
isAux(V):-isTagAtAll(V,'Aux-Negated'),!.
isAux(V):-isTagAtAll(V,aux),!.

% he gave a gift to her
% theObject + VP + theObject + IN + theObject -> theEvent
parseCycLChunk(Const,EventThru,SubjThru):- 
   append(ConstL,[theObject(ObjType1,Subj,SWords),seg(situation,Type,TXT,VTAG),theObject(ObjType2,Obj,OWords),seg(in,PType,PTXT,PTAG),theObject(ObjType3,Target,TWords)|ConstR],Const),
      append(TXT,PTXT,Words),suggestVar(=,Words,Event),
      saveVerbType(Event,hypotheticSubjectVerbObjectPrepOblique,[Subj,Event,Obj,PTAG,Target],CycLS),!,
      %   asPrepObj(Event,Subj,VTAG,Obj,PTAG,Target,CycLS),!,                  
         saveSituation(VTAG,'Verb',Event,TXT,CycLS),!,
         append(ConstL,[theObject(ObjType1,Subj,SWords),theObject('Action',Event,Words),theObject(ObjType1,Subj,SWords)|ConstR],NConst),
         !,parseCycLChunk(NConst,EventThru,SubjThru),!.


% he gave to her a gift 
% theObject + VP + IN + theObject + theObject -> theEvent
parseCycLChunk(Const,EventThru,SubjThru):- 
   append(ConstL,[theObject(ObjType1,Subj,SWords),seg(situation,Type,TXT,VTAG),seg(in,PType,PTXT,PTAG),theObject(ObjType3,Target,TWords),theObject(ObjType2,Obj,OWords)|ConstR],Const),
      not(auxOnly(VTAG)),
      append(TXT,PTXT,Words),suggestVar(=,Words,Event),
      saveVerbType(Event,hypotheticSubjectVerbPrepObjectOblique,[Subj,Event,PTAG,Target,Obj],CycLS),!,
%         asPrepObj(Event,Subj,VTAG,Obj,PTAG,Target,CycLS),!,                  
         saveSituation(VTAG,'Verb',Event,TXT,CycLS),!,
         append(ConstL,[theObject(ObjType1,Subj,SWords),theObject('Action',Event,Words),theObject(ObjType1,Subj,SWords)|ConstR],NConst),
         !,parseCycLChunk(NConst,EventThru,SubjThru),!.


% theObject + IN + theObject -> theEvent
parseCycLChunk(Const,EventThru,SubjThru):- 
   appender(ConstL,[theObject(ObjType1,Subj,SWords),seg(in,Type,TXT,VTAG),theObject(ObjType2,Obj,OWords)|ConstR],Const),
         suggestVar(=,TXT,Event),
      saveVerbType(Event,hypotheticSubjectPrepObject,[Subj,Event,Obj],CycLS),!,
%         findall(CycL,phrase(verb2(Event,Subj,Obj,CycL),VTAG),CycLS),
         saveSituation(VTAG,'Preposition',Event,TXT,CycLS),!,
         append(ConstL,[theObject(ObjType1,Subj,SWords)|ConstR],NConst),
         !,parseCycLChunk(NConst,EventThru,SubjThru),!.


% he threw a gift up
% theObject + VP + theObject + IN -> theEvent
parseCycLChunk(Const,EventThru,SubjThru):- 
   append(ConstL,[theObject(ObjType1,Subj,SWords),seg(situation,Type,TXT,VTAG),theObject(ObjType2,Target,TWords),seg(in,PType,PTXT,PTAG)|ConstR],Const),
      append(TXT,PTXT,Words),suggestVar(=,Words,Event),
      saveVerbType(Event,hypotheticSubjectVerbObjectPrep,[Subj,Event,Target,PTAG],CycLS),!,
%         asPrepObj(Event,Subj,VTAG,Obj,PTAG,Target,CycLS),!,                  
         saveSituation(VTAG,'Verb',Event,TXT,CycLS),!,
         append(ConstL,[theObject(ObjType1,Subj,SWords),theObject('Action',Event,Words),theObject(ObjType1,Subj,SWords)|ConstR],NConst),
         !,parseCycLChunk(NConst,EventThru,SubjThru),!.

% he gave to her
% theObject + VP + IN + theObject -> theEvent
parseCycLChunk(Const,EventThru,SubjThru):- 
   append(ConstL,[theObject(ObjType1,Subj,SWords),seg(situation,Type,TXT,VTAG),seg(in,PType,PTXT,PTAG),theObject(ObjType2,Target,TWords)|ConstR],Const),
      append(TXT,PTXT,Words),suggestVar(=,Words,Event),
      saveVerbType(Event,hypotheticSubjectVerbPrepObject,[Subj,Event,PTAG,Target],CycLS),!,
%         asPrepObj(Event,Subj,VTAG,SubjThru,PTAG,Target,CycLS),!,                  
         saveSituation(VTAG,'Verb',Event,TXT,CycLS),!,
         append(ConstL,[theObject(ObjType1,Subj,SWords),theObject('Action',Event,Words),theObject(ObjType1,Subj,SWords)|ConstR],NConst),
         !,parseCycLChunk(NConst,EventThru,SubjThru),!.

% theObject + VP + theObject -> theEvent
parseCycLChunk(Const,EventThru,SubjThru):- 
   append(ConstL,[theObject(ObjType1,Subj,SWords),seg(situation,Type,TXT,VTAG),theObject(ObjType2,Obj,OWords)|ConstR],Const),
         suggestVar(=,TXT,Event),
      saveVerbType(Event,hypotheticSubjectVerbObject,[Subj,Event,Obj],CycLS),!,
%         findall(CycL,phrase(verb2(Event,Subj,Obj,CycL),VTAG),CycLS),
         saveSituation(VTAG,'Verb',Event,TXT,CycLS),!,
         append(ConstL,[theObject(ObjType1,Subj,SWords),theObject('Action',Event,TXT),theObject(ObjType1,Subj,SWords)|ConstR],NConst),
         !,parseCycLChunk(NConst,EventThru,SubjThru),!.

         /*
         default mt: BotanyMt  (fruitOfType BeanPlant (FruitFn BeanPlant))
         */

% theObject + VP  -> theEvent
parseCycLChunk(Const,EventThru,SubjThru):- 
   append(ConstL,[theObject(ObjType1,Subj,SWords),seg(situation,Type,TXT,VTAG)|ConstR],Const),
         suggestVar(=,TXT,Event),
      saveVerbType(Event,hypotheticSubjectVerb,[Subj,Event],CycLS),!,
%         findall(CycL,phrase(verb1(Event,Subj,CycL),VTAG),CycLS),
         saveSituation(VTAG,'Verb',Event,TXT,CycLS),!,
         append(ConstL,[theObject(ObjType1,Subj,SWords),theObject('Action',Event,TXT),theObject(ObjType1,Subj,SWords)|ConstR],NConst),
         !,parseCycLChunk(NConst,EventThru,SubjThru),!.

% Convert IN -> VP
parseCycLChunk(Const,EventThru,SubjThru) :- 
    append(ConstL,[seg(in,T1,W1,D1)|ConstR],Const),
         append(ConstL,[seg(situation,T1,W1,D1)|ConstR],NConst), !,
         !,parseCycLChunk(NConst,EventThru,SubjThru),!.

% remove conjunctions and try again
parseCycLChunk(Const,Event,Subj) :-
      append(ConstL,[seg(cc,T1,W1,D1)|ConstR],Const),
      append(ConstL,ConstR,NConst),!,
         !,parseCycLChunk(NConst,Event,SubjThru),!.

once(X,Y):-once(X),once(Y).
once(X,Y,Z):-once(X),once(Y),once(Z).
% Skipovers
parseCycLChunk([theObject(Action,Var,Words)|Rest],EventThru,SubjThru) :- Action == 'Action',!,
         once(suggestVar(=,Words,Var)),%saveCycL(Var,'subSituations'(EventThru,Var))),
         parseCycLChunk(Rest,EventThru,SubjThru),!.

parseCycLChunk([theObject(ObjType,Var,Words)|Rest],EventThru,SubjThru) :-
         suggestVar(=,Words,Var),      
         once(saveCycL(Var,'situationConstituents'(EventThru,Var))),
         ignore(Var=SubjThru),parseCycLChunk(Rest,EventThru,SubjThru),!.         

parseCycLChunk([seg(PC,Type,Words,Tagged)|Rest],EventThru,SubjThru) :- !,
         suggestVar(=,Words,Var),      
         once(saveCycL(Var,'doom:descriptionLazyStrings'(Var,string(Words)))),
         parseCycLChunk(Rest,EventThru,SubjThru),!.

parseCycLChunk([Err|Rest],Event,Subj) :-!,parseCycLChunk(Rest,Event,SubjNext),!.

verb1(Event,Subj,and(actors(Event,Subj),CycL))-->verb_unit(Type,Time,Subj,Obj,Target,Event,CycL).
verb2(Event,Subj,Obj,CycL)-->verb_unit(Type,Time,Subj,Obj,Target,Event,CycL).
skip(string(C))-->[X],{getText(X,C)}.






/*
(and
(isa hypotheticSubjectVerbObjectPrepOblique QuintaryPredicate)
(isa hypotheticSubjectVerbPrepObjectOblique QuintaryPredicate)
(isa hypotheticSubjectVerbObjectPrep QuaternaryPredicate)
(isa hypotheticSubjectVerbPrepObject QuaternaryPredicate)
(isa hypotheticSubjectVerbObject TernaryPredicate)
(isa hypotheticSubjectVerbPrep TernaryPredicate)
(isa hypotheticSubjectVerb BinaryPredicate))

       
       
cycTask        
                             
-seg('NounPhrase', 'Determiner', [a, single, high, window], [[[txt, a], 1:1, 1.0-dt, 0.3-dt, 0.2-'MassNoun-Generic', 0.2-'MassNoun', 0.2-'Determiner-ClassA', 0.2-'Determiner-ClassB', 0.2-'Determiner-ClassC', 0.2-'Postdeterminer', 0.2-'Determiner-Central', 0.2-'Determiner', 0.2-'WHDeterminer'], [[txt, single], 2:2, 1.0-jj, 0.548611-ap, 0.40625-jj, 0.25-rb, 0.2-'Verb', 0.2-'Adjective', 0.027778-nn, 0.006944-'ap-hl', 0.006944-vb, 0.003472-vbp], [[txt, high], 3:3, 1.0-jj, 0.829851-jj, 0.25-rp, 0.2-'Adjective', 0.2-'Verb', 0.054726-rb, 0.036816-nn, 0.000995-'jj-hl'], [[txt, window], 4:4, 1.0-nn, 1.0-nn]])
seg(in, 'IN', [beside], [[[txt, beside], 5:5, 1.0-in, 0.963855-in, 0.2-'Preposition', 0.012048-rb]])
-seg('NounPhrase', 'Number-SP', [the, desk], [[[txt, the], 6:6, 1.0-dt, 0.459398-at, 0.415235-dt, 0.25-vbd, 0.25-pdt, 0.25-'nn|dt', 0.25-in, 0.2-'Determiner-ClassA', 0.2-'Determiner-ClassB', 0.2-'Determiner-ClassC', 0.2-'Postdeterminer', 0.2-'Determiner-Central', 0.2-'Determiner', 0.2-'WHDeterminer', 0.001645-'at-tl', 0.001269-'at-hl', 0.000192-'at-nc', 5.9e-005-jj, 4.4e-005-nnp, 2.2e-005-nil, 7e-006-'at-tl-hl', 7e-006-cd, 7e-006-nn, 7e-006-vbp], [[txt, desk], 7:7, 1.0-jj, 1.0-jj]])
seg(situation, 'Verb', [looks], [[[txt, looks], 8:8, 1.0-vbz, 0.884058-vbz, 0.25-vbz, 0.2-'Verb', 0.108696-nns]])
seg(in, 'IN', [into], [[[txt, into], 9:9, 1.0-in, 0.99369-in, 0.25-rp, 0.2-'Verb', 0.2-'Preposition', 0.001328-'in-hl', 0.000996-nil]])
-seg('NounPhrase', 'COMMON', [space], [[[txt, space], 10:10, 1.0-nn, 0.896875-nn, 0.2-'MassNoun-Generic', 0.2-'MassNoun', 0.2-'Verb', 0.003125-'nn-hl', 0.003125-vb]])
seg(cc, 'CoordinatingConjunction', [ (','), and], [[[txt, (',')], 11:11, 1.0-cc, 1.0- (','), 0.1-cc], [[txt, and], 12:12, 1.0-cc, 1.0-cc, 0.96242-cc, 0.25-rb, 0.002616-'cc-tl', 0.001952-'cc-hl', 5.9e-005-jj, 5.9e-005-nil, 3.9e-005-nn, 3.9e-005-nnp, 2e-005-'cc-nc', 2e-005-'cc-tl-hl', 2e-005-in]])
-seg('NounPhrase', 'Determiner', [a, fish, tank], [[[txt, a], 13:13, 1.0-dt, 0.3-dt, 0.2-'MassNoun-Generic', 0.2-'MassNoun', 0.2-'Determiner-ClassA', 0.2-'Determiner-ClassB', 0.2-'Determiner-ClassC', 0.2-'Postdeterminer', 0.2-'Determiner-Central', 0.2-'Determiner', 0.2-'WHDeterminer'], [[txt, fish], 14:14, 1.0-jj, 0.847826-jj, 0.25-vb, 0.2-'MassNoun-Generic', 0.2-'Verb', 0.2-jj, 0.021739-nns, 0.021739-rb], [[txt, tank], 15:15, 1.0-nn, 0.884615-nn, 0.2-'Verb', 0.076923-vb]])
seg(be, 'AuxVerb', [is], [[[txt, is], 16:16, 1.0-'AuxVerb', 0.513169-bez, 0.477418-'AuxVerb', 0.25-vbp, 0.25-rb, 0.2-'AuxVerb', 0.00144-'bez-hl', 0.000257-'bez-nc', 0.000103-nns, 5.1e-005-'bez-tl', 5.1e-005-nil]])
seg(situation, 'Verb', [located], [[[txt, located], 17:17, 1.0-vbn, 0.923913-vbn, 0.2-'Verb', 0.2-'Adjective', 0.043478-vbd, 0.01087-jj]])
seg(in, 'IN', [in], [[[txt, in], 18:18, 1.0-in, 0.888257-in, 0.25-vbd, 0.25-'rp|in', 0.25-nnp, 0.25-'in|rp', 0.2-'Adjective', 0.2-'Adverb', 0.2-'WHAdverb', 0.2-'Preposition', 0.2-'Verb', 0.013142-rp, 0.001618-rb, 0.001505-'in-hl', 0.000494-'in-tl', 6.7e-005-fw, 6.7e-005-nil, 4.5e-005-'in-nc', 4.5e-005-nn, 4.5e-005-'rp-hl', 4.5e-005-'rp-nc', 2.2e-005-'fw-in', 2.2e-005-rbr]])
-seg('NounPhrase', 'Number-SP', [the, northwestern, corner], [[[txt, the], 19:19, 1.0-dt, 0.459398-at, 0.415235-dt, 0.25-vbd, 0.25-pdt, 0.25-'nn|dt', 0.25-in, 0.2-'Determiner-ClassA', 0.2-'Determiner-ClassB', 0.2-'Determiner-ClassC', 0.2-'Postdeterminer', 0.2-'Determiner-Central', 0.2-'Determiner', 0.2-'WHDeterminer', 0.001645-'at-tl', 0.001269-'at-hl', 0.000192-'at-nc', 5.9e-005-jj, 4.4e-005-nnp, 2.2e-005-nil, 7e-006-'at-tl-hl', 7e-006-cd, 7e-006-nn, 7e-006-vbp], [[txt, northwestern], 20:20, 1.0-jj, 0.25-jj, 0.2-'Adjective'], [[txt, corner], 21:21, 1.0-nn, 0.962121-nn, 0.2-'Verb', 0.015152-vb, 0.007576-jj]])
seg(in, 'IN', [of], [[[txt, of], 22:22, 1.0-in, 0.975358-in, 0.25-nnp, 0.2-'Preposition', 0.2-'Verb', 0.014853-'in-tl', 0.003051-'in-hl', 4.4e-005-'in-nc', 3e-005-'in-tl-hl', 3e-005-nil, 3e-005-rb, 3e-005-rp]])
-seg('NounPhrase', 'Number-SP', [the, room], [[[txt, the], 23:23, 1.0-dt, 0.459398-at, 0.415235-dt, 0.25-vbd, 0.25-pdt, 0.25-'nn|dt', 0.25-in, 0.2-'Determiner-ClassA', 0.2-'Determiner-ClassB', 0.2-'Determiner-ClassC', 0.2-'Postdeterminer', 0.2-'Determiner-Central', 0.2-'Determiner', 0.2-'WHDeterminer', 0.001645-'at-tl', 0.001269-'at-hl', 0.000192-'at-nc', 5.9e-005-jj, 4.4e-005-nnp, 2.2e-005-nil, 7e-006-'at-tl-hl', 7e-006-cd, 7e-006-nn, 7e-006-vbp], [[txt, room], 24:24, 1.0-nn, 0.958159-nn, 0.25-nnp, 0.2-'MassNoun-Generic', 0.2-'MassNoun', 0.2-'Verb', 0.002092-'nn-hl', 0.002092-vb]])
seg(sym, 'SYM', ['.'], [[[txt, '.'], 25:25, 1.0-'.', 1.0-'.', 0.8-'.']])

                             
%hypotheticSubjectVerb
(find-or-create-constant "hypotheticSubjectVerbObjectPrepOblique")
(find-or-create-constant "hypotheticSubjectVerbPrepObjectOblique")
(find-or-create-constant "hypotheticSubjectVerbObjectPrep")
(find-or-create-constant "hypotheticSubjectVerbPrepObject")
(find-or-create-constant "hypotheticSubjectVerbObject")
(find-or-create-constant "hypotheticSubjectVerbPrep")
(find-or-create-constant "hypotheticSubjectVerb")

(isa E2C-Parser SoftwareAgent)

E2C-Parses-CCW



 (ist 
    (TextStringPropositionalExtractionMtFn #$E2CParser #$English "Bill Clinton lives in New York State") 
    ?FACT)
    
    (isa ?FACT CycLPropositionalSentence)
    
 
 (#$DocumentSentenceFn DOCUMENT SENTENCE-LOCATOR)." 
*/

saveVerbType(Event,FrameType,ArgList,[CycLO]):-reframeConsts(ArgList,NewArgList),!,CycLO=..[FrameType|NewArgList].
reframeConsts(NewArg,NewArg):-atom(NewArg),!.
reframeConsts(Var,_):-var(Var),trace,fail.
reframeConsts([Consts],WordO):-member([txt|Text],Consts),ground(Text),!,textCached(Text,[lex,Word|_]),!,unlist(Word,WordO).
reframeConsts(Consts,WordO):-member([txt|Text],Consts),ground(Text),!,textCached(Text,[lex,Word|_]),!,unlist(Word,WordO).
reframeConsts([],[]):-!.
reframeConsts([Arg|List],[NewArg|SList]):-reframeConsts(Arg,NewArg),reframeConsts(List,SList).

unlist([Word],Word):-!.
unlist(Word,Word).


  /* saveVerbType(Event,hypotheticSubjectVerbObjectPrepOblique,[Subj,Event,Obj,PTAG,Target],CycLS),!,
   saveVerbType(Event,hypotheticSubjectVerbPrepObjectOblique,[Subj,Event,PTAG,Obj,Target],CycLS),!,
   saveVerbType(Event,hypotheticSubjectVerbObjectPrep,[Subj,Event,Obj,PTAG],CycLS),!,
   saveVerbType(Event,hypotheticSubjectVerbPrepObject,[Subj,Event,PTAG,Obj],CycLS),!,
   saveVerbType(Event,hypotheticSubjectVerbObject,[Subj,Event,Obj],CycLS),!,
   saveVerbType(Event,hypotheticSubjectVerb,[Subj,Event],CycLS),!,
    */
% =======================================================
% Nouns Units Each
% =======================================================
%textCached([fond], [frame, adjSemTrans, 'PPCompFrameFn'('TransitivePPFrameType', 'Of-TheWord'), 'Adjective', feelsTowardsObject(':NOUN', ':OBLIQUE-OBJECT', 'Affection', positiveAmountOf)]).
saveNounUnit(ObjType,NType,D,Ws,Event,Subj):-
      suggestVar(=,Ws,Subj),
      findall(CycL,phrase(noun_unit_start(NType,Event,Subj,':SCOPE',CycL),D),List),!,
      leastOne(List),saveCycL(Subj,['hypotheticDenotation'(Subj,'NounPhrase',string(Ws)),thereExists(Subj,':SCOPE'),isa(Subj,'TemporalThing')|List]),!.

% Quantifiers
noun_unit_start(Type,Event,Subj,Scope,CycL9)-->quant_phrase(Subj,Restr,Scope,CycL9),{!},subject_noun(Event,Subj,Restr).
          
% picture of X
subject_noun(Event,Subj,VerbCycL,[P1,P2,P3|S],[]) :- 
         phrase(theFrame(Event, 'PPCompFrameFn'('TransitivePPFrameType',Prep),Adjective,Template),[P1,P2,P3|S],More1),phrase(theWord(Prep),More1,More2),
         getText(More2,Ws2),saveNounUnit(ObjType,Type,More2,Ws2,Event,Obj),
         substEach(Template,[':SUBJECT'-Subj,':NOUN'-Subj,':ACTION'-Event,':OBJECT'-Obj,':INF-COMP'-Obj,':OBLIQUE-OBJECT'-Obj],VerbCycL).

% his BillClinton's joe's her + something 
subject_noun(Event,Subj,possessiveRelation(Obj,Subj),[P1,P2|S],[]):-
         phrase(thePOS('Possessive'),[P1]),!,
         getText([P1],Ws1),
         saveNounUnit(ObjType,Type,[P1],Ws1,Event,Obj),
         saveCycL(possessiveRelation,[possessiveRelation(Obj,Subj)]),
         getText([P2|S],Ws2),saveNounUnit(ObjType,Type,[P2|S],Ws2,Event,Subj),!.

% $100
subject_noun(Event,Subj,equals(Subj,'DollarFn'(Num))) --> dollarSign,decimalNumber(Num).

subject_noun(Event,Subj,CycL) --> dcgStartsWith(thePOS('Preposition')),{!,fail}.

% non number pronoun
subject_noun(Event,Subj,CycL) --> dcgStartsWith(thePOS('Pronoun')),{!},pronoun_unit(Type,Event,Subj,CycL).
   
% President Clinton
subject_noun(Event,Subj,rule(Rule1,and(CycL1,CycL2))) --> person_title(Subj,CycL1,Rule1),proper_name(Subj,CycL2).

% Clinton
subject_noun(Event,Subj,CycL) --> proper_name(Subj,CycL).

subject_noun(Event,Subj,CycL,[P1|S],[]):-
         phrase(thePOS('Adjective'),[P1]),findall(CycLOwn,phrase(adjective_word(Subj,CycLOwn),[P1]),CycLOwnS),saveCycL(Subj,CycLOwnS),!,
         phrase(subject_noun(Event,Subj,CycL),S).

subject_noun(Event,Subj,CycL,[P|S],[]):-
         findall(CycLOwn,phrase(collection_noun(Event,Subj,CycLOwn),[P]),CycLOwnS),saveCycL(Subj,CycLOwnS),!,
         phrase(subject_noun(Event,Subj,CycL),S).

subject_noun(Event,Subj,isa(Subj,'ProperNounFn'(DET,PN))) -->thePOS(DET,'ProperNoun'),dcgAnd(thePOS(PN,'Noun'),theName(Subj)).

subject_noun(Event,Subj,CycL)-->collection_noun(Event,Subj,CycL).

subject_noun(Event,Subj,isa(Subj,'TemporalThing')) -->[].

% =======================================================
% Proper Noun Phrases as nondet_noun_unit
% =======================================================      

% Bill Clinton
proper_name(Subj,equals(Subj,'BillClinton')) --> theTerm('BillClinton').

% Texas
proper_name(Subj,rule(Rule,equals(Subj,CycLTerm))) -->theIsa(Subj,CycLTerm,'Agent-Generic',Rule),{notPred(CycLTerm)}.

% Adjectives that are capitalized
proper_name(Subj,_)-->thePOS('Adjective'),{!,fail}.

% Van Morison Jr
proper_name(Subj,rule(captialized3,properNameStrings(Subj,string(Name)))) --> dcgSeq(capitalized(Text1),dcgSeq(capitalized(Text2),capitalized(Text3))),{flatten([Text1,Text2,Text3],Name)}.
% Van Morison
proper_name(Subj,rule(captialized2,properNameStrings(Subj,string(Name)))) --> dcgSeq(capitalized(Text1),capitalized(Text2)),{flatten([Text1,Text2],Name)}.
% Fido
proper_name(Subj,rule(captialized1,properNameStrings(Subj,string(Name)))) --> capitalized(Name).

% president
person_title(Subj,Term,Rule)--> theIsa(Subj,Term,'PersonTypeByActivity',Rule).

% =======================================================
% Pronoun Phrases as subject_noun
% =======================================================

% Her His Your Our Their

% My 
%pronoun_unit(Type,Event,Subj,CycL,and(controls(HIS,Subj),CycL9)) -->       dcgSeq(dcgAnd(theWord('My-TheWord'),pronoun(HIS,CycLI,CycL9)),subject_noun(Event,Subj,CycL,CycLI)).
%pronoun_unit(Type,Event,Subj,CycLIn,CycL9,[S|Data],E) :- not(phrase(thePOS('Postdeterminer'),[S])),!,nonpostdet_pronoun_unit(Type,Event,Subj,CycLIn,CycL9,[S|Data],E).
% Him She They
%nonpostdet_pronoun_unit(Type,Event,Subj,CycL,thereExists(Subj,CycL9)) --> pronoun(Subj,CycL,CycL9).


pronoun_unit(Type,Event,Subj,CycL) --> pronoun(Subj,CycL).

pronoun(Subj,hypotheticDenotation(Subj,Pos,Word)) --> theWordPos('Pronoun',Word,Pos),{!}.

theWordPos(Pos,Word,Pos)-->theWord(Word),{!}.

pronoun(Subj,isa(Subj,'Person'))  --> theWord('I-TheWord'),{suggestVar(=,'Speaker',Subj)}.
pronoun(Subj,isa(Subj,'Person'))  --> theWord('You-TheWord'),{suggestVar(=,'TargetAgent',Subj)}.
%pronoun(Subj,CycL,and(isa(Subj,'Person'),CycL))  --> theWord('They-TheWord'),{suggestVar(=,'TargetAgent',Subj)}.
pronoun(Subj,isa(Subj,'Male'))  --> theWord('He-TheWord'),{suggestVar(=,'Male',Subj)}.
pronoun(Subj,isa(Subj,'Female'))  --> theWord('She-TheWord'),{suggestVar(=,'Female',Subj)}.
pronoun(Subj,isa(Subj,'Artifact-NonAgentive'))  --> theWord('It-TheWord'),{suggestVar(=,'TargetThing',Subj)}.
%pronoun(Subj,CycL,and(Constraints,CycL)) --> dcgAnd(thePOS(Word,'IndefinitePronoun'),theConstraints(Subj,Constraints)).
pronoun(Subj,Constraints) -->  theText([Text]),{pronounConstraints([Text],Subj,Constraints)}.
                                                                    
%pronounConstraints(Text,Subj,equals(Subj,nart(Eq))):-textCached(Text,[denotation,nart(Eq)|Types]),suggestVar(=,Text,Subj),!.
pronounConstraints(Text,Subj,CycL):-constraintsOnIsa(Text,Subj,CycL),!.

theConstraints(Subj,isa(Subj,ColType)) --> theText([Text]),{constraintsOnIsa([Text],Subj,ColType)}.

constraintsOnIsa(Text,Subj,CycL):- 
      findall(rule(textCached(Text,[denotation,Eq|Types]),and(equals(Subj,Eq),isa(Subj,'CollectionUnionFn'(SET)))),(textCached(Text,[denotation,Eq|Types]),SET=..['TheSet'|Types],notPred(Eq)),ISAS),
      CycL=..[and|ISAS].


% =======================================================
% Quantities (DET Phrases)     
% =======================================================
decimalNumber(Num) --> wholeNumber(Subj,Num1),dotSign,wholeNumber(Subj,Num2),{concat_atom([Num1,'.',Num2],Atom),atom_number(Atom,Num)}.
decimalNumber(Num) --> wholeNumber(Subj,Num).
wholeNumber(Subj,Num,[S1,S2|S],S) :- phrase(wholeNumber(Subj,N1),S1),phrase(wholeNumber(Subj,N2),S2),Num is N1 * N2.
wholeNumber(Subj,Num) --> theText([Num]),{number(Num),!}.
wholeNumber(Subj,2) --> theText([two]).
wholeNumber(Subj,Num) --> dcgOr(theIsa(Subj,Num,'Numeral',_),theIsa(Subj,Num,'Number-General',_)).
dollarSign --> thePOS('$').
dotSign --> thePOS('.').

% =======================================================
% Quantification (DET Phrases)     
%
% TODO Negations (no fewer than)
% =======================================================

%quant_phrase(Subj,Pre,Post,CycL) --> quant_phrase1(Subj,Pre,Mid,CycL),quant_phrase(Subj,Mid,Post,CycL).

%quant_phrase(Subj,Restr,Scope,CycL9) --> theFrame(_,'QuantifierFrame',_,Template),
%      {substEach(Template,[':SCOPE'-Scope,':NOUN'-Subj,':RESTR'-Restr],CycL9)}.

quant_phrase(Subj,Restr,Scope,CycL9)--> dcgOr(dcgOr(existential_words,universal_words(_)),dcgNone), quant_phraseN(Subj,Restr,Scope,CycL9).
quant_phrase(Subj,Restr,Scope,'thereExists'(Subj,'and'(Restr , Scope))) --> existential_words.
quant_phrase(Subj,Restr,Scope,'forAll'(Subj,'implies'(Restr , Scope))) --> universal_words(_).
quant_phrase(Subj,Restr,Scope,and(Restr,Scope)) --> [].

quant_phraseN(Subj,Restr,Scope,'thereExistExactly'(Num,Subj,'and'(Restr , Scope))) -->  wholeNumber(Subj,Num3),wholeNumber(Subj,Num2),wholeNumber(Subj,Num1),{Num is Num1*Num2*Num3}.
quant_phraseN(Subj,Restr,Scope,'thereExistExactly'(Num,Subj,'and'(Restr , Scope))) -->  wholeNumber(Subj,Num2),wholeNumber(Subj,Num1),{Num is Num1*Num2}.
quant_phraseN(Subj,Restr,Scope,'thereExistExactly'(Num,Subj,'and'(Restr , Scope))) -->  wholeNumber(Subj,Num).


quant_phraseN(Subj,Restr,Scope,'thereExistAtLeast'(Num,Subj,'and'(Restr , Scope))) --> 
         dcgSeq(at_least_words(N),wholeNumber(Subj,Num1)),{Num is N+Num1}.

at_least_words(0) --> theWord('At-TheWord'),theWord('Little-TheWord').
at_least_words(1) --> theWord('More-Than-MWW').
at_least_words(1) --> theWord('Greater-Than-MWW').

quant_phraseN(Subj,Restr,CycL1,'thereExistAtMost'(Num,Subj,'and'(Restr , CycL1))) --> 
         dcgSeq(at_most_words(N),wholeNumber(Subj,Num1)),{Num is N+Num1}.

at_most_words(0) --> dcgSeq(theWord('At-TheWord'),dcgOr(theWord('Most-TheWord'),theTerm('Most-NLAttr'))).
at_most_words(-1) --> theWord('Less-Than-MWW').
at_most_words(-1) --> theWord('Few-TheWord'),theWord('Than-TheWord').


existential_words --> existential_word,existential_word.
existential_words --> existential_word.
existential_word --> theWord('A-TheWord');theWord('An-TheWord');theWord('The-TheWord');theWord('Some-TheWord').
% there is,there are
existential_word --> theWord('There-TheWord'),theWord('Be-TheWord'). 
existential_word --> theWord('There-TheWord'),theWord('Exist-TheWord'). % there exists

universal_words(S) --> universal_word(_),universal_word(S).
universal_words(S) --> universal_word(S).

universal_word(plural) --> theWord('All-TheWord'),dcgOptional(existential_word).
% every
universal_word(singular) --> theTerm('Every-NLAttr').
universal_word(singular) --> theWord('Every-TheWord');theWord('Each-TheWord').
universal_word(plural) --> theWord('For-TheWord'),theWord('All-TheWord').
universal_word(plural) --> theText([forAll]).
universal_word(plural) --> theText([forall]).
          
% =======================================================
% Adjective Phrases
% =======================================================

%adjective_word(Subj,CycL,and(CycL,equals(':POSSESSOR',PronounIsa),controls(PronounIsa,Subj))) --> dcgAnd(dcgAnd(thePOS('PossessivePronoun'),thePOS('Pronoun')),theIsa(Pro,PronounIsa,'Individual')).
adjective_word(Subj,hypotheticDenotation(Subj,'Adjective',Word)) --> theWord(Word),{!}.
adjective_word(Subj,NPTemplate) --> theFrame(_,_,'Adjective',Template),{substEach(Template,[':NOUN'-Subj,':REPLACE'-'IdentityFn'(Subj),':SUBJECT'-Subj,':OBLIQUE-OBJECT'-'IdentityFn'(Subj)],NPTemplate)}.
adjective_word(Subj,rule(Rule,isa(Subj,AttribProp))) --> theIsa(Subj,AttribProp,'ChromaticColor',Rule).
adjective_word(Subj,rule(Rule,isa(Subj,AttribProp))) --> theIsa(Subj,AttribProp,'FirstOrderCollection',Rule).
adjective_word(Subj,controls('?Speaker',Subj)) --> theWord('My-TheWord').
adjective_word(Subj,rule(Rule,isa(Subj,AttribProp))) --> theIsa(Subj,AttribProp,'CollectionType',Rule).
adjective_word(Subj,CycL)-->collection_noun(Event,Subj,CycL).
%adjective_word(Subj,isa(Subj,'AdjectiveFn'(Word))) --> theWord(Word).


% =======================================================
% Qualified Noun
% =======================================================

%':POSSESSOR'
% the eaters of the dead
collection_noun(Event,Subj,CycL) --> thePOS('Preposition'),{!,fail}.
collection_noun(Event,Subj,hypotheticDenotation(Subj,Pos,Word)) --> thePOS(Word,BPos),{bposToCPos(BPos,Pos),!}.

collection_noun(Event,Subj,and(HowDoes,occursBefore(PreEvent,Event))) --> dcgAnd(theFrame(Subj,'GenitiveFrame','AgentiveNoun',Template),theName(Subj)),
   {substEach(Template,[':SUBJECT'-Subj,':NOUN'-Subj,':ACTION'-Event,':POSSESSOR'-Obj,':OBLIQUE-OBJECT'-Obj,':OBJECT'-Obj],HowDoes),
   suggestVar(=,['GenitiveFrame'],PreEvent),!,
     ignore(sub_term(performedBy(PreEvent,Subj),HowDoes))}.

% the jugglers
collection_noun(Event,Subj,NPTemplate) --> theFrame(Subj,'RegularNounFrame',WHPronoun,Template),
    {substEach(Template,[':NOUN'-Subj,':OBLIQUE-OBJECT'-Subj2],NPTemplate)}. %numbervars(NPTemplate,_,_)

%collection_noun(Event,Subj,Event,'isa'(Subj,CycLCollection)) --> dcgAnd(collection_type(Subj,CycLCollection),scanNounUnit).
collection_noun(Event,Subj,CycLO) --> theFrame(Subj,'GenitiveFrame','CountNoun',Template),
   {substEach(Template,[':SUBJECT'-Subj,':NOUN'-Subj,':ACTION'-Event,':POSSESSOR'-Obj,':OBLIQUE-OBJECT'-Obj,':OBJECT'-Obj],HowDoes)},
   theWord('Of-TheWord'),!,noun_unit_start(Type,Event,Obj,HowDoes,CycLO).

% the men
collection_noun(Event,Subj,and(isa(Subj,CycLCollection1),isa(Subj,CycLCollection2))) --> 
%      dcgSeq(dcgAnd(collection_type(Subj,CycLCollection1),theName(Subj)),collection_type(Subj,CycLCollection2)).
      dcgSeq(collection_type(Subj,CycLCollection1),collection_type(Subj,CycLCollection2)).

collection_noun(Event,Subj,hypotheticDenotation(Subj,Pos,Word)) --> thePOS(Word,BPos),{bposToCPos(BPos,Pos),!}.
collection_noun(Event,Subj,rule(Rule,isa(Subj,CycLCollection))) --> dcgAnd(collection_type(Subj,CycLCollection,Rule),theName(Subj)).
collection_noun(Event,Subj,rule(Rule,equals(Subj,Type))) --> theIsa(Subj,Type,'Individual',Rule),{notPred(Type)}.

collection_type(Subj,Type,Rule)--> theIsa(Subj,Type,'StuffType',Rule),{notPred(Type)}.
collection_type(Subj,'Person',theText([person])) --> theText([person]).
collection_type(Subj,Type,Rule)--> theIsa(Subj,Type,'ClarifyingCollectionType',Rule),{notPred(Type)}.
collection_type(Subj,Type,Rule)--> theIsa(Subj,Type,'SentenceSubjectIndexical',Rule),{notPred(Type)}.
collection_type(Subj,Type,Rule)--> theIsa(Subj,Type,'Collection',Rule),{notPred(Type)}.
%collection_type(Subj,'NounPhraseModifierFn'(Word)) --> theWord(Word).
%collection_type(Subj,'Thing') --> [].

notPred(A):-atom(A),!,not((name(A,[C|_]),is_lower(C))).
notPred(_A).
% =======================================================
% Rel Clauses
% =======================================================

% of stuff
% on a shelf with a ball
rel_clause(Event,Subj,SubjObjectCycL) --> 
    theFrame(Event,'Post-NounPhraseModifyingFrame',_,Template),
      subject_noun(Event,Obj,HowDoes,SubjObjectCycL),
   {substEach(Template,[':SUBJECT'-Subj,':NOUN'-Subj,':ACTION'-Event,':OBJECT'-Obj,':OBLIQUE-OBJECT'-Obj],HowDoes)}.

% that Verb Phrase
rel_clause(Event,Subj,CycL9) --> theTerm('Backreference-ClassB-NLAttr'),{!},verb_phrase(Time,Subj,Obj,Event,CycL9).
% who Verb Phrase
rel_clause(Event,Subj,More) --> dcgAnd(thePOS('wps'),pronoun(Subj,CycL9,More)),verb_phrase(Time,Subj,Obj,Event,CycL9).
% named Benji
rel_clause(Event,Subj,'and'(IsaCycL, ProperNameCycL)) --> theWord('Name-TheWord'),proper_name(Subj,ProperNameCycL).


%rel_clause(Event,Subj,SubjIsaCycL,'rpand'(SubjIsaCycL, preactors(Event,Obj))) --> prepositional_noun_phrase(Event,Subj,Obj,SubjIsaCycL,Prep).

%rel_clause(Event,Subj,CycL9)-->theWord('Of-TheWord'),subject_noun(Event2,Subj,thereExists(Event2,and(isa(Event2,'Situation'),actorIn(Event2,Subj))),CycL9),{!}.
rel_clause(Event,Subj,'QueryFn'(That,CycL2))-->thePOS(That,'WHAdverb'),{!},sent(Event,Subj,CycL2),{!}.
%rel_clause(Event,Subj,CycL0,whp(DE),V) -->thePOS(DET,'WHPronoun'),{!},verb_unit(Type,V).
%rel_clause(Event,Subj,'WhDeterminerFn'(Subj,Obj,CycL9))-->thePOS(That,'Determiner'),vp1(Event,Subj,Obj,CycL9),{!}.
%rel_clause(Event,Subj,and(isa(Event2,That),'ThatFn'(That,Subj,Obj,CycL9)))-->thePrep(Event2,That),vp1(Event2,Subj,Obj,CycL9),{!}.


%verb_intransitive(Subj,Obj,Event,'and'('bodilyDoer'(Subj,Event),'isa'(Event,actOf(paint)))) --> [paints].

%rel_clause(Event,Subj,CycL0,'and'(CycL0, HowDoes)) --> theWord('That-TheWord'),verb_phrase_premods(Event,Does,HowDoes),verb_phrase(Time,Subj,Obj,Event,Does).


%   verb_phrase(Time,Subj,Obj,Event1,CycL1),theText([(,)]),verb_phrase(Time,Subj,Obj,Event,CycL2).
%verb_phrase(Time,Subj,Obj,Event,adv(Mod,CycL9)) -->[['ADVP'|Mod]],verb_phrase(Time,Subj,Obj,Event,mod(Mod,CycL9)).
%is good


% =======================================================
% Verb PRE Modifiers
% =======================================================
      
verb_phrase_premods(Event,Scope,Scope) --> dcgStartsWith(thePOS('Preposition')),{!}.
verb_phrase_premods(Event,ScopeIn,VerbCycL) --> theFrame(Pred,'VerbPhraseModifyingFrame',Adverb,Template),
   {substEach(Template,[':SUBJECT'-Subj,':SCOPE'-Scope,':ACTION'-Event,':NOUN'-Subj,':OBJECT'-Obj],VerbCycL)},
   verb_phrase_premods(Event,ScopeIn,Scope).

verb_phrase_premods(Event,ScopeIn,not(Scope)) --> theWord('Not-TheWord'),{!},
   verb_phrase_premods(Event,ScopeIn,Scope).

verb_phrase_premods(Event,ScopeIn,and(VerbCycL,Scope)) --> aux_phrase(Event,VerbCycL),{!},
   verb_phrase_premods(Event,ScopeIn,Scope).

verb_phrase_premods(Event,Scope,Scope)-->[].


% =======================================================
% AuxVerbs & Adverbs
% =======================================================
aux_phrase(Event,isa(Event,'Situation')) --> dcgAnd(theWord('Have-TheWord'),theTense(Time)),{saveTime(Event,Time)}. 
aux_phrase(Event,isa(Event,'Situation')) --> dcgAnd(thePOS('BeAux'),theTense(Time)),{saveTime(Event,Time)}. 
aux_phrase(Event,isa(Event,'Situation')) --> dcgAnd(theWord('Be-TheWord'),theTense(Time)),{saveTime(Event,Time)}. 
aux_phrase(Event,bodilyDoer(Subj,Event)) --> dcgAnd(theWord('Do-TheWord'),theTense(Time)),{saveTime(Event,Time)}. 
aux_phrase(Event,behavourCapable(Subj,Event)) -->  dcgAnd(theWord('Can-TheModal'),theTense(Time)),{saveTime(Event,Time)}. 
aux_phrase(Event,behavourCapable(Subj,Event)) -->  dcgAnd(theWord('Can-TheWord'),theTense(Time)),{saveTime(Event,Time)}. 
aux_phrase(Event,behavourCapable(Subj,Event)) -->  theWord('Could-TheWord'),{saveTime(Event,'IntervalAfterFn'('Now','AnIndefiniteAmountOfTime'))}. 
aux_phrase(Event,behavourCapable(Subj,Event)) -->  dcgAnd(thePOS('Modal'),theTense(Time)),{saveTime(Event,Time)}. 
%textCached([hardly], [denotation, 'AlmostNever', 'NonNumericQuantity', 'Frequency', 'Individual']).
aux_phrase(Event,rule(Rule,isa(Event,Term))) -->dcgAnd(thePOS('Adverb'),theIsa(Event,Term,'Individual',Rule)).
aux_phrase(Event,isa(Event,'AdverbFn'(Word))) --> dcgAnd(thePOS('Adverb'),theWord(Word)).

verb_phrase_postmods(Event,CycL,and(Truth,CycL)) -->aux_phrase(Event,Truth).
verb_phrase_postmods(Event,CycL,rule(Rule,implies(occursDuring(Event,Mod),holdsIn(Event,CycL)))) --> time_phrase(Event,Mod,Rule).
verb_phrase_postmods(Event,CycL,CycL) --> [].

verb_postmods(Event,CycL,CycLO)-->verb_phrase_postmods(Event,CycL,CycLO).
%verb_postmods(Event,CycL,CycLO)-->dcgAnd(dcgNot(thePOS('Determiner')),verb_phrase_postmods(Event,CycL,CycLO)).

% Today
time_phrase(Event,Mod,Rule) --> theIsa(Event,Mod,'TimePoint',Rule).
% Monday
time_phrase(Event,Mod,Rule) --> theIsa(Event,Mod,'CalendarDay',Rule).
time_phrase(Event,Mod,Rule) --> theIsa(Event,Mod,'TemporalObjectType',Rule).
time_phrase(Event,Mod,Rule) --> theIsa(Event,Mod,'TimeInterval',Rule).


% =======================================================
% Verbs/Verb Phrases
% =======================================================
verb_prep(Event,Subj,Obj,PTAG,Target,VerbObjectCycLO)-->
      verb_phrase_premods(Event,ScopeIn,Scope),{!},
      verb_prep1(Event,Subj,Obj,PTAG,Target,ScopeIn),
        verb_postmods(Event,VerbObjectCycL,VerbObjectCycLO).

% Two
verb_prep1(Event,Subj,Obj,PTAG,Target,VerbCycL)-->
        theFrame(Event,'PPCompFrameFn'('TransitivePPFrameType',Prep),Verb,Template),
     {phrase(theWord(Prep),PTAG)},
     {substEach(Template,[':SUBJECT'-Subj,':NOUN'-Subj,':ACTION'-Event,':OBJECT'-Obj,':INF-COMP'-Target,':OBLIQUE-OBJECT'-Target],VerbCycL)}.

% Three
verb_prep1(Event,Subj,Obj,PTAG,Target,VerbCycL)-->
     theFrame(Event,'PPCompFrameFn'('DitransitivePPFrameType',Prep),Verb,Template),%{true},
     {phrase(theWord(Prep),PTAG)},
     {substEach(Template,[':SUBJECT'-Subj,':NOUN'-Subj,':ACTION'-Event,':OBJECT'-Obj,':INF-COMP'-Target,':OBLIQUE-OBJECT'-Target],VerbCycL)}.


% Three
verb_prep1(Event,Subj,Obj,PTAG,Target,VerbCycL)-->
        theFrame(Event,'DitransitiveNP-InfinitivePhraseFrame','Verb',Template),%{true},
     {phrase(theWord(Prep),PTAG)},
        {substEach(Template,[':SUBJECT'-Subj,':NOUN'-Subj,':ACTION'-Event,':OBJECT'-Obj,':INF-COMP'-Target,':OBLIQUE-OBJECT'-Target],VerbCycL)}.

% =======================================================
% Verbs Resorts
% =======================================================

%verb_unit(Type,Time,Subj,Obj,Target,Event,and(hasVAttributes(Event,AVP),CycL5)) --> aux(AVP),verb_unit(Type,Time,Subj,Obj,Target,Event,CycL5).
%verb_unit(Type,Time,Subj,Obj,Target,Event,and(hasVAttributes(Event,AVP),CycL5)) --> adv(AVP),verb_unit(Type,Time,Subj,Obj,Target,Event,CycL5).

%verb_unit(Type,Time,Subj,Obj,Target,Event,Scope) --> 
%       verb_unit1(Type,Time,Subj,Obj,Target,Event,Scope).

verb_unit(Type,Time,Subj,Obj,Target,Event,VerbObjectCycLO) --> 
      dcgOnce(verb_phrase_premods(Event,ScopeIn,Scope)),
       verb_unit1(Type,Time,Subj,Obj,Target,Event,ScopeIn),
            dcgOnce(verb_postmods(Event,Scope,VerbObjectCycLO)).

% eaten by
verb_unit1(Type,Time,Subj,Obj,Target,Event,CycLO)-->
            verb_resort(Obj,Subj,Target,Event,CycL),theWord('By-TheWord'),{!}.

verb_unit1(Type,Time,Subj,Obj,Target,Event,CycL1)--> 
           verb_resort(Subj,Obj,Target,Event,CycL1).

verb_unit1(Type,Time,Subj,Obj,Target,Event,and(CycL1,CycL2))-->
            verb_resort(Subj,Obj,Target,Event,CycL1),
            verb_unit(Type,Time,Subj,Obj,Target,Event,CycL2).

verb_unit1_detr(Type,Time,Subj,Obj,Target,Event,VerbCycL) --> theFrame(Pred,VerbPhraseModifyingFrame,Adverb,Template),
   {substEach(Template,[':SUBJECT'-Subj,':SCOPE'-Scope,':ACTION'-Event,':NOUN'-Subj,':OBJECT'-Obj,':OBLIQUE-OBJECT'-Obj],VerbCycL)}.



%verb_unit1(Type,Time,Subj,Obj,Target,Event,isa(Event,Isa)) --> dcgBoth((event_verb(Event,Isa),thePOS('BeAux')),theName(Event)).
%verb_unit1(Type,Time,Subj,Obj,Target,Event,isa(Event,Isa)) --> dcgBoth(event_verb(Event,Isa),theName(Event)).

%textCached('South-TheWord', [frame, 'South-TheWord', 'Noun', 'PPCompFrameFn'('TransitivePPFrameType', 'Of-TheWord'), equals(':NOUN', 'SouthernRegionFn'(':OBLIQUE-OBJECT')), nounSemTrans]).
event_verb(Event,'VerbFn'(V))-->{!,fail}.
event_verb(Event,'VerbAPFn'(V,P))-->thePOS(V,'Adverb'),thePOS(P,'Preposition'),{!}.
event_verb(Event,'VerbVPFn'(V,P))--> thePOS(V,'Verb'),thePOS(P,'Preposition').
event_verb(Event,'VerbFn'(V))-->thePOS(V,'Verb'),{!}.

verb_resort(Subj,Obj,Target,Event,VerbCycL) --> theWord('To-TheWord'),{!},
      verb_resort(Subj,Obj,Target,Event,VerbCycL).

% Two
verb_resort(Subj,Obj,Target,Event,VerbCycL) --> 
           theFrame(Event,'ParticleCompFrameFn'('TransitiveParticleNPFrameType',Prep),'Verb',Template),theWord(Prep),
     {substEach(Template,[':SUBJECT'-Subj,':ACTION'-Event,':OBJECT'-Obj,':INF-COMP'-Target,':OBLIQUE-OBJECT'-Target],VerbCycL)}.

% Three
verb_resort(Subj,Obj,Target,Event,VerbCycL) --> 
        theFrame(Event,'DitransitiveNP-NPFrame','Verb',Template),%{true},
        {substEach(Template,[':SUBJECT'-Subj,':ACTION'-Event,':OBJECT'-Obj,':INF-COMP'-Target,':OBLIQUE-OBJECT'-Target],VerbCycL)}. 

% Two
verb_resort(Subj,Obj,Target,Event,and(isa(Event,'Event'),VerbCycL)) --> 
     theFrame(Event,'TransitiveNPFrame','Verb',Template),
     {substEach(Template,[':SUBJECT'-Subj,':ACTION'-Event,':OBJECT'-Obj,':INF-COMP'-Target,':OBLIQUE-OBJECT'-Target],VerbCycL)}.
                                     
% Three
verb_resort(Subj,Obj,Target,Event,VerbCycL) --> 
        theFrame(Event,_,'Verb',Template),%{true},
        {substEach(Template,[':SUBJECT'-Subj,':ACTION'-Event,':OBJECT'-Obj,':INF-COMP'-Target,':OBLIQUE-OBJECT'-Target],VerbCycL)}. 

verb_resort(Subj,Obj,Target,Event,CycL) --> verb_intransitive(Subj,Obj,Event,CycL).


verb_resort(Subj,Obj,Target,Event,'is-Underspecified'(Subj,Obj)) --> dcgOr(thePOS('BeAux'),theWord('Am-TheWord'),theWord('Be-TheWord')).

verb_resort(Subj,Obj,Target,Event,rule(Rule,and(isa(Event,EventType),preActors(Event,Subj),actors(Event,Obj)))) -->  
      dcgAnd(theIsa(Event,EventType,'DurativeEventType',Rule),theTense(Time)),{saveTime(Event,Time)}. 

verb_resort(Subj,Obj,Target,Event,rule(Rule,holdsIn(Event,[Pred,Subj,Obj]))) --> 
      dcgAnd(theIsa(Event,Pred,'TruthFunction',Rule),theTense(Time)),{saveTime(Event,Time)}. 


saveTime(Event,Time):-ignore((nonvar(Time),saveCycL(Event,occursDuring(Event,Time)))).

%verb_resort(Subj,Obj,Target,Event,holdsIn(Event,PrepCycL)) --> prepositional(Event,Subj,Obj,PrepCycL,Prep).

%verb_resort(Subj,Obj,Target,Event,implies(isa(Event,'VerbFn'(Word)),eventSOT(Event,Subj,Obj,Time))) --> dcgAnd(thePOS(Word,'Verb'),theName(Event),theTense(Time)).

optionalVerbGlue(and) --> theText([and]).
optionalVerbGlue(or) --> theText([or]).
optionalVerbGlue(and) --> theText([,]).
%optionalVerbGlue(and) --> [].
% Three
verb_expression_resorted(Time,Subj,Obj,Event,CycLO) --> 
        theFrame(Event,'DitransitiveNP-NPFrame','Verb',Template),%{true},
        verb_postmods(Event,CycL,CycLO),
        subject_noun(Event,Obj,VerbObjectCycL,CycL),
        dcgOptional(thePOS('Preposition')),
        subject_noun(Event,Target,VerbCycL,VerbObjectCycL),
        {substEach(Template,[':SUBJECT'-Subj,':NOUN'-Subj,':ACTION'-Event,':OBJECT'-Obj,':INF-COMP'-Target,':OBLIQUE-OBJECT'-Target],VerbCycL)}. 

% more frames UnderstoodReciprocalObjectFrame MiddleVoiceFrame 
% =======================================================
% Intransitive Verbs + verb_unit
% =======================================================
verb_intransitive(Subj,Obj,Event,rule(Rule,thereExists(Event,and(isa(Event,EventType),preActors(Event,Subj))))) --> 
     theIsa(Event,EventType,'DurativeEventType',Rule).

verb_intransitive(Subj,Obj,Event,and(preActors(Event,Subj),CycL)) --> 
      theFrame(Event,'MiddleVoiceFrame','Verb',Template),
      {substEach(Template,[':SUBJECT'-Subj,':ACTION'-Event],CycL)}.

verb_intransitive(Subj,Obj,Event,and(preActors(Event,Subj),CycL)) --> 
      theFrame(Event,'ParticleCompFrameFn'('IntransitiveParticleFrameType',Prep),Verb,Template),
      theWord(Prep),{!,substEach(Template,[':SUBJECT'-Subj,':ACTION'-Event],CycL)}.

verb_intransitive(Subj,Obj,Event,and(preActors(Event,Subj),CycL)) --> 
      theFrame(Event,'IntransitiveVerbFrame','Verb',Template),
      {!,substEach(Template,[':SUBJECT'-Subj,':ACTION'-Event],CycL)}.

% =======================================================
% Verbs Expression
% =======================================================

% Verb resort Two
verb_expression(Time,Subj,Obj,Event,PREPL) --> 
   verb_unit(Type,Time,Subj,Obj,Target,Event,PrepCycL),
   dcgOptional(thePOS(Prep,'Preposition')),
   subject_noun(Event,Obj,PrepCycL,VerbObjectCycL),
   {prepl(Event,VerbObjectCycL,Prep,PREPL)}.


% Verb resort Three
verb_expression(Time,Subj,Obj,Event,PREPL) --> 
   verb_unit(Type,Time,Subj,Obj,Target,Event,PrepCycL),
   subject_noun(Event,Obj,VerbTargetCycL,VerbObjectCycL),
   dcgOptional(thePOS(Prep,'Preposition')),
   subject_noun(Event,Target,PrepCycL,VerbTargetCycL),
   {prepl(Event,VerbObjectCycL,Prep,PREPL)}.

% Verb resort One
verb_expression(Time,Subj,Obj,Event,PREPL) --> 
   verb_unit(Type,Time,Subj,Obj,Target,Event,PrepCycL),
   dcgOptional(thePOS(Prep,'Preposition')),
   {prepl(Event,PrepCycL,Prep,PREPL)}.
     
prepl(Event,VerbObjectCycL,Prep,and(isa(Event,'PrepositionFn'(Prep,Event)),VerbObjectCycL)):-nonvar(Prep),!.
prepl(Event,VerbObjectCycL,Prep,VerbObjectCycL).



:-set_prolog_flag(double_quotes,string).
:-include(logicmoo_nl_testing).
            
%:-[el_holds].            

