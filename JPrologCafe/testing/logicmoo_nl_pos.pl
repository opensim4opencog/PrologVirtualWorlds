:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

% assumes loaded in  this order: ['freq.pdat.txt'],['colloc.pdat.txt'],['BRN_WSJ_LEXICON.txt'].
% TODO use %:- ['pt-grammar.prul2.txt'].
% exports sentencePos/2,bposToCPos/2
% imports learnText(Words),learnTexts(Words) from cyc_nl

:-[cyc_nl_rosetta].

% =======================================================
% utterance breaker
% =======================================================
sentenceTagger(String):-sentenceTagger(String,X),dumpList(X).

sentenceBreaker(String,BrokenA):-
      getWordTokens(String,Words),
      sentenceBreakerRespace(Words,Broken000),
      sentenceBreakerUncontract(Broken000,Broken00),
      sentenceBreakerDenoter(Broken00,Broken0),
      sentenceBreakerReMWS(Broken00,Broken0,BrokenA),!.
      
sentenceTagger(String,Tagged):-
      sentenceBreaker(String,BrokenA),!,
      sentencePos(BrokenA,Tagged).

sentenceBreakerRespace([L,'\'',R|BrokenA],BrokenB):- concat_atom([L,'\'',R],W),!,sentenceBreakerRespace([W|BrokenA],BrokenB).
sentenceBreakerRespace([W|BrokenA],[W|BrokenB]):-!,sentenceBreakerRespace(BrokenA,BrokenB).
sentenceBreakerRespace(Broken0,Broken0).


%commonMispelling(Mispell,Correction).
commonMispelling('teh',the).
commonMispelling('yo',you).
commonMispelling('A',a).
commonMispelling('An',an).
commonMispelling('i','I').
commonMispelling('The',the).

sentenceBreakerUncontract([Mispell|BrokenA],BrokenB):- commonMispelling(Mispell,Correction),sentenceBreakerUncontract([Correction|BrokenA],BrokenB).

sentenceBreakerUncontract(['aint'|BrokenA],BrokenB):- sentenceBreakerUncontract([are,not|BrokenA],BrokenB).
sentenceBreakerUncontract(['ain\'t'|BrokenA],BrokenB):- sentenceBreakerUncontract([are,not|BrokenA],BrokenB).
sentenceBreakerUncontract(['can\'t'|BrokenA],BrokenB):- sentenceBreakerUncontract([can,not|BrokenA],BrokenB).
sentenceBreakerUncontract(['won\'t'|BrokenA],BrokenB):- sentenceBreakerUncontract([will,not|BrokenA],BrokenB).
sentenceBreakerUncontract(['wont'|BrokenA],BrokenB):- sentenceBreakerUncontract([will,not|BrokenA],BrokenB).
sentenceBreakerUncontract(['she\'s'|BrokenA],BrokenB):- sentenceBreakerUncontract([she,is|BrokenA],BrokenB).
sentenceBreakerUncontract(['he\'s'|BrokenA],BrokenB):- sentenceBreakerUncontract([he,is|BrokenA],BrokenB).
sentenceBreakerUncontract(['it\'s'|BrokenA],BrokenB):- sentenceBreakerUncontract([it,is|BrokenA],BrokenB).
sentenceBreakerUncontract(['they\'s'|BrokenA],BrokenB):- sentenceBreakerUncontract([they,is|BrokenA],BrokenB).
sentenceBreakerUncontract([SHOULDNT|BrokenA],BrokenB):-atom_concat(SHOULD,'n\'t',SHOULDNT),sentenceBreakerUncontract([SHOULD,not|BrokenA],BrokenB).
sentenceBreakerUncontract([YOURE|BrokenA],BrokenB):-atom_concat(YOU,'\'re',YOURE),sentenceBreakerUncontract([YOU,are|BrokenA],BrokenB).
sentenceBreakerUncontract([IAM|BrokenA],BrokenB):-atom_concat(I,'\'m',IAM),sentenceBreakerUncontract([I,am|BrokenA],BrokenB).
sentenceBreakerUncontract([W|BrokenA],[W|BrokenB]):-!,sentenceBreakerUncontract(BrokenA,BrokenB).
sentenceBreakerUncontract(Broken0,Broken0).

sentenceBreakerDenoter(Words,Tagged):-evalSubL(mapcar('#\'car','denotation-mapper'(string(Words),
    quote(['#$middleName','#$middleNameInitial','#$nicknames','#$givenNames','#$firstName','#$abbreviationString-PN','#$alias','#$initialismString']),':greedy')),List,_),reformatStrings(List,Tagged),!.
%sentenceBreaker0(Words,Tagged):-evalSubL('denotation-mapper'(string(Words),'NIL',':greedy'),List:_),reformatStrings(List,Tagged),!.

%sentenceBreaker1(['\'',[A]|BrokenA],[[W]|BrokenB]):- atom(A),atom_concat('\'',A,W),!,sentenceBreaker1(BrokenA,BrokenB).
%sentenceBreaker1(['\'',A|BrokenA],[[W]|BrokenB]):- atom(A),atom_concat('\'',A,W),!,sentenceBreaker1(BrokenA,BrokenB).

noBreak(a).
noBreak(the).
noBreak(an).
noBreak(_). 


sentenceBreakerReMWS([NoBreak|Rest],Breaks,[[[txt,NoBreak]]|Results]):-noBreak(NoBreak),!,sentenceBreakerReMWS(Rest,Breaks,Results).
sentenceBreakerReMWS(Words,Breaks,[[[txt|Ws]]|Results]):- member(string(Ws),Breaks),append(Ws,Rest,Words),!,sentenceBreakerReMWS(Rest,Breaks,Results).
sentenceBreakerReMWS([Ws|Rest],Breaks,[[[txt,W],[txt,Ws],0.7-'\'s']|Results]):-atom_concat(W,'\'s',Ws),sentenceBreakerReMWS(Rest,Breaks,Results).
sentenceBreakerReMWS([Ws|Rest],Breaks,[[[txt,Ws]]|Results]):-sentenceBreakerReMWS(Rest,Breaks,Results).
sentenceBreakerReMWS([],_,[]):-!.



% ================================================================================
% sentencePos/2
% ================================================================================
sentencePos(In,Out):-
         sentencePosAdd(In,PosAdded),!,  
         sentenceViterbi(PosAdded,Viterbi),!,
         sentenceApplyBrillContextRules(Viterbi,Out).


% ================================================================================
% wordForString/3
% ================================================================================
wordForString(['right-wall'],'WordFn'(string('laast')),'NLWordForm'):-!.
wordForString(['left-wall'],'WordFn'(string('staart')),'NLWordForm'):-!.
wordForString(['.'],'Period-TheSymbol','Punctuation-SP'):-!.
wordForString(String,Word,Pos):-atom(String),!,wordForString([String],Word,Pos).
wordForString(String,Word,Pos):- once(learnText(String)),%ignore(member(Pos,['CountNoun',''])
         findall(Word:Pos,textCached(String,[lex,Word,Pos|_]),Words),leastOne(Words),sort(Words,Words9),!,member(Word:Pos,Words9).
wordForString(String,'WordFn'(string(String)),Pos).

% ================================================================================
% getPos/3
% ================================================================================
%getPos(S,Pos,Word):-holds(partOfSpeech,Word,Pos,string(S)).
getPos(S,Pos,Word):-number(S),!,fail.
getPos(S,'ProperNoun',Word):-catch(cycQuery(or(nameSpelling(Word,string(S)),commonNickname(Word,string(S))),'NameLexicalMt'),_,fail).
getPos(S,Pos,Word):-catch(cycQuery(partOfSpeech(Word,Pos,string(S)),'GeneralEnglishMt'),_,fail).
getPos(S,Pos,Word):-cycQuery(and(wordStrings(Word,string(S)),partOfSpeech(Word,Pos,string(S)))).
getPos(S,Pos,Word):-cycQuery(and(speechPartPreds(Pos,Pred),[Pred,Word,string(S)])).
getPos(S,Pred,Word):- catch(cycQuery(and(speechPartPreds(Pos,Pred),[Pred,Word,string(S)])),_,fail).
getPos(S,POSP,Word):-cycQuery(and(wordForms(Word,Pred,string(S)),speechPartPreds(Pos,Pred))),member(POSP,[Pred,Pos]).
%getPos(W,Pred,Word):-cycQueryV([Pred,Word],and(speechPartPreds(Pos,Pred),[Pred,Word,string(W)])).
%      sformat(S,'(remove-duplicates (ask-template \'(?Pos  ?Word) \'(#$and (#$speechPartPreds ?Pos ?Pred)(?Pred ?Word "~w")) #$EverythingPSC) #\'TREE-EQUAL)',[W]),
%      evalSubL(S,R:_).


%sentenceApplyBrillContextRules(Mid,Mid):-!

% ================================================================================
% sentenceViterbi/2
% ================================================================================
sentenceViterbi(In,Out):-!,
     append(In,[  [[txt,('.')], _:_ , 0.8 - ('.'), 'BEST'|'NIL'] ], Do),
      most_probable_hmm_path(In,_,Best),!,
      replacePOSFixer(In,Best,Out).

replacePOSFixer([],_,[]).
replacePOSFixer([S|In],[B|Best],[O|Out]):-
      notrace(subst(S,  'BEST',1.0 - B, O)),
      replacePOSFixer(In,Best,Out),!.
replacePOSFixer(S,B,S):-nl,writeq(replacePOSFixer(S,B,O)),nl,nl. %true.


e2p(S,O):- sentenceBreaker(S,B),most_probable_hmm_path(B,_,O).

testPos :- e2p([can,he,can,can,a,can],Path), write(Path),nl.
%          Path = [pron,aux,v,dt,n]


% cyc adjusted probs
% put output pos into dcg area

outprob([],_,_):-!,fail.
outprob(Atom,Pos,Prob):-atom(Atom),!,outprob([Atom],Pos,Prob).
outprob([a],dt,0.300):-!.
outprob(Data,Pos,Prob):-member([txt|Text],Data),!,isTagSimply(Data,Pos,Prob).
outprob(Data,Pos,Prob):-member([txt|Text],Data),!,outprob(Text,Pos,Prob).
%outprob([can],aux,0.010):-!.
%outprob([can],v,0.005):-!.
outprob([can],nn,0.007).
%outprob([he],pron,0.070):-!.
outprob([Atom],Pos,Prob):-atom(Atom),cyc:toLowercase(Atom,Lower),Atom\==Lower,!,outprob([Lower],Pos,Prob).
outprob([_|W],Pos,Prob):-W\==[],!,outprob(W,Pos,Prob).
outprob(W,Pos,Prob):-findall(Pos:Prob,getTextBPosR(W,Pos,Prob),List),List=[_|_],!,member(Pos:Prob,List).

outprob(_,'nn',0.85).
outprob(_,'jj',0.195).
outprob(_,'ns',0.02).
outprob(_,'vbz',0.01).


verterbiDcg(Prob,[]) --> wrd(Word1,Pos1,Prob1),wrd(Word2,Pos2,Prob2),{transprob(Pos1,Pos2,TProb),Prob is Prob1*Prob2*TProb}.



getTextBPosR([(',')],(cc),0.1).
getTextBPosR([('.')],('.'),0.8).
getTextBPosR([('?')],('.'),0.3).
getTextBPosR([W],W,1.0):-atom(W),atom_codes(W,[C]),char_type(C,M),member(M,[punct,white]),!. %text_lit(W).
getTextBPosR([N],cd,1.0):-number(N).
getTextBPosR(MWS,P,0.56):-mws(MWS,P),!.
getTextBPosR([W],'\'s',0.58):-atom_concat(N,'\'s',W).
getTextBPosR([W],'n\'t',0.59):-atom_concat(N,'n\'t',W).
getTextBPosR([W],P,N):-text_bpos(N,W,P).
getTextBPosR([W],P,0.25):-text_bpos(W,P),not((text_bpos(N,W,P))).
%getTextBPosR(P,L):-dcg_rewrite(P,L).

%transprob(X,Y,0.10000):-true,not(transprob(X,Y,_)),!.

transprob(X,Y,ZO):-tagEquiv(X,XO,Xr),tagEquiv(Y,YO,Yr),transprob2(XO,YO,Z),ZO is Z*(Xr+Yr)/2,!.
%transprob(X,Y,Z):-transprob(X,Y,Z,_).

                       /*
          transprob(staart,dt,0.30).          transprob(v,dt,0.36).
          transprob(staart,aux,0.20).          transprob(v,aux,0.01).
          transprob(staart,v,0.10).            transprob(v,v,0.01).
          transprob(staart,n,0.10).            transprob(v,n,0.26).
          transprob(staart,pron,0.30).         transprob(v,pron,0.36).
          transprob(dt,dt,0.20).            transprob(n,dt,0.01).
          transprob(dt,aux,0.01).            transprob(n,aux,0.25).
          transprob(dt,v,0.01).              transprob(n,v,0.39).
          transprob(dt,n,0.77).              transprob(n,n,0.34).
          transprob(dt,pron,0.01).           transprob(n,pron,0.01).
          transprob(aux,dt,0.18).            transprob(pron,dt,0.01).
          transprob(aux,aux,0.10).            transprob(pron,aux,0.45).
          transprob(aux,v,0.50).              transprob(pron,v,0.52).
          transprob(aux,n,0.01).              transprob(pron,n,0.01).
          transprob(aux,pron,0.21).           transprob(pron,pron,0.01).       */
    

          most_probable_hmm_path(Words,P,Path) :-
                probable_paths(Words,[1-[staart]],PPaths),
                keymax(PPaths,P-Path1),
                reverse(Path1,[staart|Path]).

          probable_paths([],PPaths,PPaths).
          probable_paths([Word|Words],PPaths0,PPaths) :-
                findall(PPath,
                    (outprob(Word,Tag2,PL),
                    findall(P2-[Tag2,Tag1|Tags],
                        (member(P1-[Tag1|Tags],PPaths0),
                        transprob(Tag1,Tag2,PT), 
                        P2 is PL*PT*P1),
                    AllPaths),
                    keymax(AllPaths,PPath)),
                PPaths1),
         %writeq(PPaths1),nl,nl,
                probable_paths(Words,PPaths1,PPaths).


 
keymax(AllPaths, MaxProb-PPath) :-
        keymax(AllPaths, -1, [staart], MaxProb, PPath).
 
%----------------------------------------------------+
% keymax(+Elements,+AkkuNum,+AkkuPath,-Max,-MaxPath)
%----------------------------------------------------+
% stop: keine elemente mehr
keymax([], Max, MaxPath, Max, MaxPath).
% nimmt das bisher groesste mit
keymax([Num1-Val1|Tail], Num2, Val2, Max, MaxPath) :-
        (    Num1 > Num2
        ->   keymax(Tail, Num1, Val1, Max, MaxPath)
        ;    keymax(Tail, Num2, Val2, Max, MaxPath)
        ).             

% ================================================================================
% sentenceS/2
% ================================================================================
sentenceS(Broken,Types):- sentenceSS(Broken,Types).
sentenceS(Broken,Broken).

sentenceSS(Broken,Types):-sentenceSS('S',Broken,Types).

sentenceSS(Type,Line,[S:Limit,Type,[sub|SubLine], [txt|Text] ]):-
      dcg_rewrite(Simple,Type,[M|Match]),
      sentenceSS(Simple,[M|Match],Line,SubLine).
      

expandM(How,[M|Match],Expanded,HowMS):-
      expandS(How,M,MExpanded,HowM),
      expandM(HowM,Match,MMExpanded,HowMS),
      append(MExpanded,MMExpanded,Expanded).

expandS(How,M,Expanded,Hows):-not(member(M,How)),dcg_rewrite(_,M,Expand),intersection(How,Expand,[]),expandM([M|How],Expand,Expanded,Hows).
expandS(H,M,[M],H):-not(dcg_rewrite(_,M,Expand)).
      
      
% ================================================================================
% sentenceSegs/2
% ================================================================================
%sentenceSegs(Broken,Type):- findall(Types,sentenceSegment(Broken,Types),Sort),list_to_set(Sort,Sorted),!,member(Type,Sorted).
sentenceSegs(Broken,Types):- sentenceSegment(Broken,Types).
sentenceSegs(Broken,Broken).

e2s(Eng,Out):-sentenceBreaker(Eng,Broken),writeConstit(Broken),!,sentenceSegs(Broken,Out).

getLoc(L,S:E):-member(S:E,L),!.
getLoc([L],S:E):-getLoc(L,S:E),!.
getLoc([L|List],S:E):-getLoc(L,S:_),getLoc(List,_:E),!.

getText([],[]).
getText(L,Txt):-member([txt|Txt],L),!.
getText([L|List],Text):-getText(L,Text1),getText(List,Text2),append(Text1,Text2,Text),!.
getText(F,S):-functor(F,_,3),arg(2,F,S),!.
getText(S,S).


sentenceSegment([],[]).
sentenceSegment([L],[L]):-isTagSimply(L,'S'),!.
sentenceSegment(X,Y):-sentenceSegment2(X,Y).
sentenceSegment2([L|Line],Types):- %
      once((getLoc(L,S:End),getText(L,Text1),length(Line,Len))),
      dcg_rewrite(Simple,Type,[M|Match]),length(Match,ML),%Len>=ML,
      once((isTagSimply(L,M))),
      once((tryRestMatch(End,Line,Match,Text2,Limit,Rest,SubLine),append(Text1,Text2,Text),
      THIS1=[S:Limit,Type,rule([M|Match]), [sub,[M,L]|SubLine], [txt|Text] ])),
            ( (sentenceSegment2(Rest,RestTypes),sentenceSegment([THIS1|RestTypes],Types));
              (sentenceSegment([THIS1|Rest],Types))).
%sentenceSegment([S,L|Line],[S|Types]):-sentenceSegment2([L|Line],Types).
%sentenceSegment([L],[L]):-member([tag,_,'S'|_],L),!.
%sentenceSegment([X],[X]).


tryRestMatch(S,Rest,[],[],S,Rest,[]):-!.
tryRestMatch(S,[L|Rest],[M],Text,Limit,Rest,[[M,L]]):-!,
     isTagSimply(L,M),getLoc(L,_:Limit),getText(L,Text).

tryRestMatch(S,[L|Line],[M|Match],Text,Limit,Rest,[[M,L]|Subline]):-
     isTagSimply(L,M),!,tryRestMatch(S,Line,Match,Text2,Limit,Rest,Subline),!,
      getText(L,Text1),append(Text1,Text2,Text).

%getSubLine(L,Subline).
%getSubLine(L,[L]).
         

cleanPOSr:-text_bpos(F,W,P),F>0.3,retract(text_bpos(W,_)),fail.
cleanPOSr.

%:-cleanPOSr.

% ================================================================================
% posAdder/2
% ================================================================================
sentencePosAdd(Before,After):-
         sentenceBreakerPOSAdder2(1,Before,After),!.
sentenceBreakerPOSAdder2(N,[],[]).
sentenceBreakerPOSAdder2(N,[W|Before],[D|After]):-
         NN is N+1,posAdder(N:N,W,D),
         sentenceBreakerPOSAdder2(NN,Before,After).

posAdder(N,W,D):-member([txt|Words],W),learnText(Words),learnTexts(Words),!,posAdder(N,Words,W,D).
posAdder(N,W,W):-!. %true.

posAdder(N,W,D,Out):-
      posKeys(W,Poses),
      append(D,[N,'BEST'|Poses],Out).
      %append(More,'NIL',Out).
posAdder(Words,D,D).


posKeys(W,RAPos):-         
         findAllBPos(W,Poses),
         findAllCPos(Poses,W,CPos),         
         append(Poses,CPos,LAPos),list_to_set(LAPos,APos),keysort(APos,SAPos),reverse(SAPos,RAPos).

findAllBPos(W,Poses):-findall(PosVal-BPos,outprob(W,BPos,PosVal),Poses).
findAllCPos(C,W,Poses):-findall(0.2-BPos,getTextCPos(C,W,BPos),Poses).


acceptedPOS(CPos):-atom(CPos),name(CPos,[A1|_]),is_lower(A1),!,fail.
acceptedPOS(CPos):-member(CPos,['CountNoun-Generic','CountNoun','NLWordForm','Noun']),!,fail.
acceptedPOS(CPos).


notLowercase(H1):-atom(H1),name(H1,[A1|_]), is_upper(A1).

getTextCPos([_|_],W,CPos):-textCached(W,[lex,_,CPos]),acceptedPOS(CPos).
getTextCPos([],W,CPos):-textCached(W,[lex,_,CPos]),notLowercase(CPos).
%getTextCPos(_,W,CPos):-textCached(W,[frame, SemTrans, RegularFrame, CPos , Frame]).

% ================================================================================
% posCleaner/4
% ================================================================================

bcrAll(BCRs):-findall(BCR,bcr(BCR),BCRs).

%sentenceApplyBrillContextRules(Out,Out):-!.
sentenceApplyBrillContextRules(Mid,Out):-bcrAll(BCRs),applyBCRs(BCRs,Mid,Out),!.

applyBCRs([],Out,Out).
applyBCRs([B|BCRs],In,Out):-applyBCR(B,B,In,Mid),applyBCRs(BCRs,Mid,Out).


isWrd(A,WORD):-memberchk([txt,WORD],A).

isTag(Data,TAG):-nth1(N,Data, _-TAGGED ), N<5, once(tagEquiv(TAG,TAGGED,_)).

isTagAtAll(Data,TAG):-nth1(N,Data, _-TAGGED ),once(tagEquiv(TAG,TAGGED,_)),!.

tagEquiv(X,X,1.0).
tagEquiv(X,Y,0.6):-bposToCPos(X,Y).
tagEquiv(X,Y,0.3):-bposToCPos(Y,X).
tagEquiv(X,Y,0.2):-bposToCPos(X,M),bposToCPos(Y,M).

isTagSimply(Data,Pos):-isTagSimply(Data,Pos,Num),Num>0.4.
%isTagSimply(Data,'staart',1.0):-memberchk(1:_,Data).
isTagSimply(Data,Pos,Prob):-member(Prob-Pos,Data).

substPos(BCR,A,OTAG,NTAG,X):-
         findall(MPOS,bposToCPos(OTAG,MPOS),SusbtList),         
         substAll(A,[OTAG|SusbtList],NTAG,X),
         cyc:writeFmtFlushed(substPos(BCR,A,OTAG,NTAG,X)),!.

applyBCR(BCR,R,[],[]):-!.

applyBCR(BCR,[TAG1,TAG2,'curwd',WORD],[A|In],[X|Out]):-
      isWrd(A,WORD),isTagSimply(A,TAG1),!,
      substPos(BCR,A,TAG1,TAG2,X),applyBCR(BCR,[Tag1,TAG2,'curwd',WORD],In,Out).

applyBCR(BCR,[TAG1,TAG2,'lbigram',WORD1,WORD2],[A,B|In],Out):-
      isWrd(A,WORD1),isWrd(B,WORD2),isTagSimply(B,TAG1),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'lbigram',WORD1,WORD2],[A,X|In],Out).

applyBCR(BCR,[TAG1,TAG2,'nextbigram',TAG3,TAG4],[A,B,C|In],Out):-
      isTagSimply(A,TAG1),isTagSimply(B,TAG3),isTagSimply(C,TAG4),!,
      substPos(BCR,A,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'nextbigram',TAG3,TAG4],[X,B,C|In],Out).

applyBCR(BCR,[TAG1,TAG2,'nexttag',TAG3],[B,A|In],Out):-
       isTagSimply(A,TAG3),isTagSimply(B,TAG1),!,
       substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'nexttag',TAG3],[X,A|In],Out).

applyBCR(BCR,[TAG1,TAG2,'next1or2tag',TAG3],[B,H1,A|In],Out):- 
      (isTagSimply(A,TAG3);isTagSimply(H1,TAG3)),isTagSimply(B,TAG1),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'next1or2tag',TAG3],[X,H1,A|In],Out).

applyBCR(BCR,[TAG1,TAG2,'next1or2or3tag',TAG3],[B,H1,H2,A|In],Out):- 
      (isTagSimply(A,TAG3);isTagSimply(H1,TAG3);isTagSimply(H2,TAG3)),isTagSimply(B,TAG1),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'next1or2or3tag',TAG3],[X,H1,H2,A|In],Out).

applyBCR(BCR,[TAG1,TAG2,'next2tag',TAG3],[B,H1,A|In],Out):-
      isTagSimply(A,TAG3),isTagSimply(B,TAG1),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'next2tag',TAG3],[X,H1,A|In],Out).

applyBCR(BCR,[TAG1,TAG2,'nextwd',WORD1],[B,A|In],Out):-
      isWrd(A,WORD1),isTagSimply(B,TAG1),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'nextwd',TAG3],[X,A|In],Out).

applyBCR(BCR,[TAG1,TAG2,'prevbigram',TAG3,TAG4],[A,B,C|In],Out):-
      isTagSimply(A,TAG3),isTagSimply(B,TAG4),isTagSimply(C,TAG1),!,
      substPos(BCR,C,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'prevbigram',TAG3,TAG4],[A,B,X|In],Out).

applyBCR(BCR,[TAG1,TAG2,'prevtag',TAG3],[A,B|In],Out):-
      isTagSimply(A,TAG3),isTagSimply(B,TAG1),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'prevtag',TAG3],[A,X|In],Out).

applyBCR(BCR,[TAG1,TAG2,'prev1or2tag',TAG3],[A,B,C|In],Out):- 
      (isTagSimply(A,TAG3);isTagSimply(B,TAG3)),isTagSimply(C,TAG1),!,
      substPos(BCR,C,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'prev1or2tag',TAG3],[A,B,X|In],Out).

applyBCR(BCR,[TAG1,TAG2,'prev1or2or3tag',TAG3],[A,H1,H2,B|In],Out):- 
      (isTagSimply(A,TAG3);isTagSimply(H1,TAG3);isTagSimply(H2,TAG3)),isTagSimply(B,TAG1),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'prev1or2or3tag',TAG3],[A,H1,H2,X|In],Out).

applyBCR(BCR,[TAG1,TAG2,'prev2tag',TAG3],[A,H1,B|In],Out):-
      isTagSimply(H1,TAG3),isTagSimply(B,TAG1),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'prev2tag',TAG3],[A,H1,X|In],Out).

applyBCR(BCR,[TAG1,TAG2,'prevwd',WORD1],[A,B|In],Out):-
      isWrd(A,WORD1),isTagSimply(B,TAG1),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'prevwd',TAG3],[A,X|In],Out).

applyBCR(BCR,[TAG1,TAG2,'prev1or2wd',TAG3],[A,H1,B|In],Out):- 
      (isWrd(A,TAG3);isWrd(H1,TAG3)),isTagSimply(B,TAG1),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'prev1or2wd',TAG3],[A,H1,X|In],Out).

applyBCR(BCR,[TAG1,TAG2,'prev1or2or3wd',TAG3],[A,H1,H2,B|In],Out):- 
      (isWrd(A,TAG3);isWrd(H1,TAG3);isWrd(H2,TAG3)),isTagSimply(B,TAG1),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'prev1or2or3wd',TAG3],[A,H1,H2,X|In],Out).

applyBCR(BCR,[TAG1,TAG2,'prev2wd',TAG3],[A,H1,B|In],Out):-
      isWrd(A,TAG3),isTagSimply(B,TAG1),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'prev2wd',TAG3],[A,H1,X|In],Out).

applyBCR(BCR,[TAG1,TAG2,'rbigram',WORD1,WORD2],[A,B|In],Out):-
      isWrd(A,WORD1),isWrd(B,WORD2),isTagSimply(A,TAG1),!,
      substPos(BCR,A,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'rbigram',WORD1,WORD2],[X,B|In],Out).

applyBCR(BCR,[TAG1,TAG2,'surroundtag',TAG3,TAG4],[A,B,C|In],Out):-
      isTagSimply(A,TAG3),isTagSimply(B,TAG1),isTagSimply(C,TAG4),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'surroundtag',TAG3,TAG4],[A,X,C|In],Out).

applyBCR(BCR,[TAG1,TAG2,'surroundwd',WORD1,WORD2],[A,B,C|In],Out):-
      isWrd(A,WORD1),isTagSimply(B,TAG1),isWrd(C,WORD2),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'surroundwd',TAG3,TAG4],[A,X,C|In],Out).

applyBCR(BCR,[TAG1,TAG2,'wdand2bfr',WORD1,WORD2],[A,B,C|In],Out):-
      isWrd(A,WORD1),isWrd(C,WORD2),isTagSimply(C,TAG1),!,
      substPos(BCR,C,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'wdand2bfr',WORD1,WORD2],[A,B,X|In],Out).

applyBCR(BCR,[TAG1,TAG2,'wdand2tagaft',WORD,TAG3],[A,B,C|In],Out):-
      isWrd(A,WORD),isTagSimply(C,TAG3),isTagSimply(A,TAG1),!,
      substPos(BCR,A,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'wdand2tagaft',WORD,TAG3],[X,B,C|In],Out).

applyBCR(BCR,[TAG1,TAG2,'wdprevtag',TAG3,WORD],[A,B|In],Out):-
      isTagSimply(A,TAG3),isWrd(B,WORD),isTagSimply(B,TAG1),!,
      substPos(BCR,B,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'wdprevtag',TAG3,WORD],[A,X|In],Out).

applyBCR(BCR,[TAG1,TAG2,'wdnexttag',WORD,TAG3],[A,B|In],Out):-
      isWrd(A,WORD),isTagSimply(B,TAG3),isTagSimply(A,TAG1),!,
      substPos(BCR,A,TAG1,TAG2,X),applyBCR(BCR,[TAG1,TAG2,'wdnexttag',WORD,TAG3],[X,B|In],Out).

applyBCR(BCR,R,[I|In],[I|Mid]):-applyBCR(BCR,R,In,Mid).


dcg_rewrite(Type,SPos):-dcg_rewrite_simple(Type,SPos).
dcg_rewrite(Type,SPos):-dcg_rewrite_complex(Type,SPos).



%bposToCPos(X,X):-not(bposToCPos(X,Y)),true.
bposToCPos(Atom,_):-not(atom(Atom)),!,fail.
bposToCPos(n,'Noun').
bposToCPos(v,'Verb').
bposToCPos(dt,'Determiner').

bposToCPos(pron,'Pronoun').
bposToCPos(aux,'Modal').
bposToCPos(aux,'AuxVerb').
bposToCPos('AuxVerb','Aux-Negated').

%ConjunctAdverb

bposToCPos(bedz,'AuxVerb').
bposToCPos(bedz,'BeAux').
bposToCPos(('?'),('.')).
%NN are common nouns, NP proper nouns, JJ adjectives, VBG gerunds, VBD past participles and CD numeral determiners.
%RP are particles, PREP prepositions, ART articles, and V verbs.
bposToCPos(W,W):-atom(W),atom_codes(W,[C|_]),char_type(C,M),member(M,[punct,white]),!. %text_lit(W).
%1.   CC     Coordinating conjunction
bposToCPos(cc,'CoordinatingConjunction').
%2.   CD     Cardinal number
bposToCPos(cd,'Number-SP').
bposToCPos(cd,'Determiner').
%3.   DT     Determiner
bposToCPos(dt,'Determiner').
%4.   EXAMPLE     Existential there
bposToCPos(example,'Existential').
bposToCPos(ex,'Existential').
%EX existential there EX0

%5.   FW     Foreign word
bposToCPos(fw,'ForeignWord').
%6.   IN     Preposition or subordinating conjunction
bposToCPos(in,'SubordinatingConjunction').
bposToCPos(in,'Preposition').
%7.   JJ     Adjective
bposToCPos(a,'Adjective').
bposToCPos(jj,'Adjective').
%8.   JJR    Adjective, comparative
bposToCPos(jjr,'Adjective','ComparativeAdjective').
%9.   JJS    Adjective, superlative
bposToCPos(jjs,'Adjective','SuperlativeAdjective').
%10.  LS     List item marker
bposToCPos('ls','OrdinalAdjective').
bposToCPos('ls','Pronoun').
%11.  MD     Modal
bposToCPos(md,'Modal').
%12.  NN     Noun, singular or mass
bposToCPos(nn,'Noun').
bposToCPos(nn,'MassNoun').
bposToCPos(nn,'CountNoun').
bposToCPosForm(nn,'singular').
bposToCPosForm(nn,'mass').
%13.  NNS    Noun, plural
bposToCPos(nns,'Noun').
bposToCPosForm(nns,'plural').
%14.  NP     Proper noun, singular
bposToCPos(np,'ProperNoun').
bposToCPosForm(np,'singular').
%15.  NPS    Proper noun, plural
bposToCPos(nps,'ProperNoun').
bposToCPosForm(nps,'plural').



%NNPS proper noun, plural NP0..
bposToCPos(nnps,'ProperNoun').
bposToCPosForm(nnps,'plural').

bposToCPos('nn$','Possessive').
bposToCPos('nn$','Noun').


%16.  PDT    Predeterminer
bposToCPos(pdt,'Determiner').
bposToCPos(pdt,'Determiner-Pre').
%bposToCPos(pdt,'Preposition').
%17.  POS    Possessive ending
bposToCPos(pos,'Possessive').
%18.  PP     Personal pronoun
bposToCPos(pp,'Personal').
bposToCPos(pp,'Pronoun').
%19.  PP$    Possessive pronoun
bposToCPos('pp$','Possessive').
bposToCPos('pp$','Pronoun').

%PRP$ possessive pronoun PNP..
bposToCPos('prp$','Possessive').
bposToCPos('prp$','Pronoun').


%20.  RB     Adverb
bposToCPos(rb,'Adverb').
%21.  RBR    Adverb, comparative
bposToCPos(rbr,'Adverb').
bposToCPosForm(rbr,'comparative').
%22.  RBS    Adverb, superlative
bposToCPos(rbs,'Adverb').
bposToCPosForm(rbs,'superlative').
%23.  RP     Particle
bposToCPos(rp,'Particle').
%24.  SYM    Symbol
bposToCPos(sym,'Symbol').
%25.  TO     to
bposToCPosForm(to,'to').
bposToCPos(to,'Preposition').
%26.  UH     Interjection
bposToCPos(uh,'Interjection').
%27.  VERBATIM     Verb, base form
bposToCPos(verbatim,'Verb').
bposToCPosForm(verbatim,'baseForms').
bposToCPos(vb,'Verb').
bposToCPosForm(vb,'baseForms').
%28.  VBD    Verb, past tense
bposToCPos(vbd,'Verb').
bposToCPosForm(vbd,'past').
%29.  VBG    Verb, gerund or present participle
bposToCPos(vbg,'Verb').
bposToCPosForm(vbg,'gerund').
bposToCPosForm(vbg,'present').
%30.  VBN    Verb, past participle
bposToCPos(vbn,'Verb').
bposToCPosForm(vbn,'gerund').
bposToCPosForm(vbn,'past').
%31.  VBP    Verb, non-3rd person singular present
bposToCPos(vbp,'Verb').
bposToCPosForm(vbp,'singular').
bposToCPosForm(vbp,'present').
bposToCPosForm(vbp,'no3rd').
%32.  VBZ    Verb, 3rd person singular present
bposToCPos(vbz,'Verb').
bposToCPosForm(vbz,'singular').
bposToCPosForm(vbz,'present').
bposToCPosForm(vbz,'Third').
%33.  WDT    Wh-determiner
bposToCPos(wdt,'Determiner').
bposToCPos(wdt,'WHDeterminer').
%34.  WP     Wh-pronoun
bposToCPos(wp,'Pronoun').
bposToCPos(wp,'WHPronoun').
%35.  WP$    Possessive wh-pronoun
bposToCPos('wp$','Pronoun').
bposToCPos('wp$','WHPronoun').
bposToCPos('wp$','Possessive').
%36.  WRB    Wh-adverb
bposToCPos(r,'Adverb').
bposToCPos(wrb,'WHAdverb').

/*
%37.  "      Simple double quote
%38.  $      Dollar sign
%39.  #      Pound sign
%40.  `      Left single quote
%41.  '      Right single quote
%42.  ``     Left double quote
%43.  ''     Right double quote
%44.  (      Left parenthesis (round, square, curly or angle bracket)
%45.  )      Right parenthesis (round, square, curly or angle bracket)
%46.  ,      Comma
%47.  .      SentencE-final punctuation
%48.  :      Mid-sentence punctuation
*/
bposToCPos(dt,'Determiner').
bposToCPos(n,'Noun').
bposToCPos(nnp,'ProperNoun').
bposToCPos(nnp,'ProperNoun').
bposToCPos(nnp,'Noun').
bposToCPos(np,'ProperNoun').
bposToCPos(np,'Noun').
bposToCPos(prep,'Preposition').
bposToCPos(uh,'Interjection-SpeechPart').
bposToCPos(v,'Verb').
bposToCPos(vbd,'Verb').
bposToCPos(vbg,'gerund').
bposToCPos(vbp,'Verb').
bposToCPos(ppl,'Pronoun').



bposToCPos(at,'Article').
bposToCPos(at,'Determiner').

% ppss  pronoun, personal, nominative, not 3rd person singular  they we I you ye thou you uns  
bposToCPos(ppss,'PersonalPronoun').
bposToCPos(ppss,'Pronoun').
bposToCPos(ppss,'no3rd').

%PPO objective personal pro
bposToCPos(ppo,'PersonalPronoun').
bposToCPos(ppo,'Pronoun').

bposToCPos(prp,'PersonalPronoun').
bposToCPos(prp,'Pronoun').

/*

                         
MAPPING Penn Treebank Tag Description to BNC Tag
cc coordinating conjunction cjc
cd cardinal number crd
dt determiner dt0, at0
ex existential there ex0
fw foreign word unc
in preposition/subordinating conjunction prf, prp
jj adjective aj0
jjr adjective, comparative greener ajc
jjs adjective, superlative ajs
ls list marker crd / -
md modal vm0
nn noun, singular or mass nn0, nn1
nns noun plural nn2
nnp proper noun, singular np0..
pdt predeterminer dt0
pos possessive ending pos
prp personal pronoun pnp..
prp$ possessive pronoun pnp..
rb adverb av0..
rbr adverb, comparative av0.. , ajc..
rbs adverb, superlative av0.. , ajs..
rp particle avp
to to to0
uh interjection unc
vb verb, base form vvi
vbd verb, past tense vvd
vbg verb, gerund/present participle vvg
vbn verb, past participle vvn
vbp verb, sing. present, non-3d vvi..  
vbz verb, 3rd person sing. present vvz
wdt wh-determiner dtq
wp wh-pronoun pnq
wp$ possessive wh-pronoun dtq
wrb wh-abverb avq


. sentence closer . ; ? !
( left paren
) right paren
-- dash , comma
: colon

ap  determiner/pronoun, Determiner-Post  many other next more last former little several enough most least only very few fewer past same  
at  article  the an no a every th' ever' ye  
cc  conjunction, coordinating  and or but plus & either neither nor yet 'n' and/or minus an'  
cs  conjunction, subordinating  that as after whether before while like because if since for than until so unless though providing once lest till whereas whereupon supposing albeit then  
in  preposition  of in for by considering to on among at through with under into regarding than since despite ...  
md  modal auxiliary  should may might will would must can could shall ought need wilt  
pn  pronoun, nominal  none something everything one anyone nothing nobody everybody everyone anybody anything someone no-one nothin'  
ppl  pronoun, singular, reflexive  itself himself myself yourself herself oneself ownself  
pp$  determiner, possessive  our its his their my your her out thy mine thine  
pps  pronoun, personal, nominative, 3rd person singular  it he she thee  
ppss  pronoun, personal, nominative, not 3rd person singular  they we I you ye thou you'uns  
wdt  WH-determiner  which what whatever whichever 
wps  WH-pronoun, nominative  that who whoever whosoever what whatsoever  
*/



bposToCPos('pos','Possessive').
bposToCPos('pp$','Possessive').
bposToCPos('wps','WHPronoun').
bposToCPos('pp','Punctuation').
bposToCPos('\'s','Possessive').

bposToCPos(HASDOLLAR,'Possessive'):-concat_atom([_,_|_],'$',HASDOLLAR).
bposToCPos(BEDZ,BeAux):-atom_concat('be',_,BEDZ),member(BeAux,['BeAux','AuxVerb']).
bposToCPos(X,Y):-concat_atom([P,L],'-',X),!,(bposToCPos(P,Y);X=Y).
bposToCPos(X,Y):-concat_atom([P,L],'|',X),!,(bposToCPos(P,Y);bposToCPos(P,L);X=Y).
bposToCPos(H,M):-brillPos([H|L]),member(M,L),atom_codes(M,[C|_]),is_upper(C).

:- ['cyc_pos_data.pl'].
