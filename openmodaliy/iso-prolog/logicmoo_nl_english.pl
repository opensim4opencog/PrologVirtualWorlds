:-include('logicmoo_utils_header.pl').
% =================================================================
% english2Kif
% =================================================================
clientEvent(Channel,Agent,english(phrase([learn|Input],Codes),_)):-!,
	    AS = exec_lf(and(equals('?TargetAgent','#$Self'),equals('?Speaker',Agent),['#$or'|Ors])),
	    findall(Kif,english2Kif(Input,Kif),Ors),say(AS).

clientEvent(Channel,Agent,english(phrase(Input,Codes),_)):-
	    AS = exec_lf(and(equals('?TargetAgent','#$Self'),equals('?Speaker',Agent),['#$or'|Ors])),
	    findall(Kif,english2Kif(Input,Kif),Ors),
	    sendEvent(Channel,Agent,(AS)).
% ===================================================================
% ===================================================================
codesToForms(Codes,[],Codes):-!.
codesToForms(Codes,[List|More],Out):-!,
      codesToForms(Codes,List,M),!,
      codesToForms(M,More,Out),!.

codesToForms(Codes,lowercase,Out):-!,toLowercase(Codes,Out).
codesToForms(Codes,uppercase,Out):-!,toUppercase(Codes,Out).
codesToForms(Codes,cyclist,Out):-!,getSurfaceFromChars(Codes,M,_).
codesToForms(Codes,cyclistvars,Out:V):-!,getSurfaceFromChars(Codes,Out,V).
codesToForms(Codes,cycl,Out):-!,getSurfaceFromChars(Codes,O,V),balanceBinding(O,Out).
codesToForms(Codes,cyclvars,Out:V):-!,getSurfaceFromChars(Codes,O,V),balanceBinding(O,Out).
codesToForms(Codes,words,Out):-!,getCycLTokens(Codes,Out).
codesToForms(Codes,idioms(D),Out):-!,idioms(D,Codes,Out).
codesToForms(Codes,Pred,Out):-atom(Pred),!,Call=..[Pred,Codes,Out],!,Call.

dirrect_order([start,tomcat]).
%dirrect_order([start,tomcat]):-shell('/opt/tomcat/bin/startup.sh'),say([ok,done]).


:- op(500,xfy,&). 
:- op(510,xfy,=>). 
:- op(100,fx,`).

	 



english2Kif(Sentence):-english2Kif(Sentence,Kif),say(Kif).
english2Obj(Sentence):-english2Obj(Sentence,Kif),say(Kif).

english2Kif(Sentence,Kif):-
      convertToWordage(Sentence,Words),
      wordageToKif(Words,Kif).

english2Obj(Sentence,noun_phrase(A,C)):-
      convertToWordage(Sentence,Words),
      phrase(noun_phrase(A,_,C),Words).

english2Kif:-english2Kif('i am happy').

convertToWordage([],['#$True']):-!.
convertToWordage(String,C):-is_string(String),string_to_atom(String,Atom),!,convertToWordage(Atom,C).
convertToWordage(Atom,C):-atom(Atom),!,
      atom_codes(Atom,Codes),!,getCycLTokens(Codes,List),!,
      idioms(chat,List,ListExpanded),!,
      convertToWordage(ListExpanded,C),!.
      %isDebug(say('~q<br>',[C])),!.

convertToWordage(Words,C):-is_list(Words),
      removeRepeats(Words,NoRepeats),!,
      convertToWordage(noRepeats(NoRepeats),C),!.

convertToWordage(noRepeats(NoRepeats),Next):-!,
      fdelete(NoRepeats,['Hm','hm','ah','uh','Uh','Um','um'],WordsNoIT),!,
      subst(WordsNoIT,i,'I',Next),!.
%e2c(English,CycLOut)   


% =================================================================
% wordageToKif
% =================================================================
      
wordageToKif(Words,ResultO):- reverse(Words,[Symbol|Rev]),reverse(Rev,Quest),!,
	 wordageToKif(Symbol,Words,Quest,ResultO). %,simplifyLF(Result,ResultO).


wordageToKif(('?'),Words,Quest,query(Kif)) :- phrase(questionmark_sent(Kif),Quest).
wordageToKif(('.'),Words,Quest,assert(Kif)) :- phrase(period_sent(Kif),Quest).
wordageToKif(Symbol,Words,Quest,Kif) :- not(memberchk(Symbol,[('.'),('?')])),phrase(sentence(Kif),Words).
wordageToKif(Symbol,Words,Quest,words(Words)).

% =======================================================
% sentence(CycL, [every,man,that,paints,likes,monet],[]) 
% =======================================================
%sentence(S) --> conjunct(_),!,syntact(S).
%sentence(S) --> interjections(_),!,syntact(S).
questionmark_sent(true(CycL)) --> assertion(CycL).
questionmark_sent(can(CycL)) --> interogative(CycL).
questionmark_sent(yn(CycL)) --> imparative(CycL).

simplifyLF(true(X),X).
simplifyLF(yn(X),X).

period_sent(CycL) --> assertion(CycL).
period_sent(interogative(CycL)) --> interogative(CycL).
period_sent(command(Act)) --> imparative(Act).

simplifyLF(interogative(X),X).
simplifyLF(command(X),X).

sentence(command(Act)) --> imparative(Act).
sentence(assert(CycL)) --> assertion(CycL).
%sentence(query(CycL)) --> interogative(CycL).
     
simplifyLF(interogative(X),X).
simplifyLF(assert(X),assert(X)).

literal([E|F], C, B):-!,append([E|F],B, C).
literal(E, [E|C], C).
literal([], C, C).

      
% =================================================================
% interjections  TODO
% =================================================================
interjections(interject(W)) --> interjection(W).
interjections(interjects(H,T)) --> interjection(H),interjections(T).

interjection(C) --> isPOS('#$Interjection-SpeechPart',C).

% =================================================================
% imparative  TODO
% =================================================================

% tell me
imparative(CycL) --> verb_phrase(TargetAgent,ImparitiveEvent,CycL),
	 {varnameIdea('?TargetAgent',TargetAgent),varnameIdea('ImparitiveEvent',ImparitiveEvent)}.

% =================================================================
% interogative  TODO
% =================================================================
% How are you
% What have you
% What do you have?
% What do you think?
% How will you
% Could the dog
% could he think of it? 
% are you happy
% * ?
interogative(CycL) --> verb_phrase(TargetAgent,ImparitiveEvent,CycL),
	 {varnameIdea('?TargetAgent',TargetAgent),varnameIdea('QuestionEvent',ImparitiveEvent)}.

% =================================================================
% assertion
% =================================================================
% Now lets say that the input values for the memory NN uses the pattern from the other nodes output
% our naming specialist, Linda Bergstedt
% it is good
% the fubar licks the bowl
% It should be a mix.

% the dog licks the bowl
assertion(CycL) --> noun_phrase(Subj,CycL1,CycL),verb_phrase_after_nouns(Subj,Event,CycL1).

% gen assertion 1
assertion(gen_assert(Call,Result)) --> [S],
	    { '#$genFormat'(Predicate,[S|Template],ArgsI,_,_),atom(Predicate),
	    (compound(ArgsI) -> trasfromArgs(ArgsI,Args) ; Args=[1,2]),
	    length(Args,Size),functor(Call,Predicate,Size),
	    placeVars(Blanks,Args,Call)},
	    do_dcg(Template,Blanks,Result).

assertion(gen_assert(Predicate)) --> [S],
	    { '#$genFormat'(Predicate,S,_,_,_) }.

% =================================================================
% WHORDS
% =================================================================

% which do
what_do(W,V) --> query_starter(W),isPOS('#$DoAux',V).

% where 
%query_starter(W)  --> isPOS('#$WHAdverb',W).
% could / which
query_starter(W)  --> isPOS('#$Modal',W);isPOS('#$WHWord',W).

% =======================================================
% Rel Clauses
% =======================================================

% Linda Bergstedt
human_name(([First,Last])) --> capitolWord(First),capitolWord(Last).
% Linda
human_name(Name) --> capitolWord(Name). 

capitolWord(A) --> [A],{atom(A),atom_codes(A,[C|_]),char_type(C,upper)}.

% =======================================================
% Nouns Phrases
% =======================================================
% =======================================================
% TODO
%'#$properNounSemTrans'('#$Egyptian-TheWord', 0, '#$RegularNounFrame', '#$citizens'('#$Egypt', ':NOUN'), '#$GeneralEnglishMt', v(v('#$Egypt', '#$Egyptian-TheWord', '#$RegularNounFrame', '#$citizens', ':NOUN'), A)).
% =======================================================


% TODO
% =======================================================
%'#$nlPhraseTypeForTemplateCategory'('#$PhraseFn-Bar1'('#$Verb'), '#$PerfectiveVBarTemplate', '#$AuxVerbTemplateMt', v(v('#$PerfectiveVBarTemplate', '#$PhraseFn-Bar1', '#$Verb'), A)).


% TODO
% =======================================================
%'#$nounSemTrans'('#$Aspect-TheWord', 0, '#$PPCompFrameFn'('#$TransitivePPCompFrame', '#$Of-TheWord'), '#$hasAttributes'(':OBLIQUE-OBJECT', A), '#$GeneralEnglishMt', v(v('#$Aspect-TheWord', '#$Of-TheWord', '#$PPCompFrameFn', '#$TransitivePPCompFrame', '#$hasAttributes', ':OBLIQUE-OBJECT'), ['?ATTR'=A|B])).

% TODO
% a man hapilly maried
% a man who knows
% a man of his word that walks
% a man of his word
% the cost of what the product is

% a man that walks
%noun_phrase(Subj,In,also(A,LL)) --> [A|LL],{cont([A|LL])}.
noun_phrase(List, In, Out, [M,N,O|More], F) :-
      (nth1(Loc,[M,N,O],',');nth1(Loc,[M,N,O],'and')),
      noun_phrase_list(Loc,List, In, Out, [M,N,O|More], F).

noun_phrase_list(Loc,[H],In,Out) --> subject(H,In,Out).
noun_phrase_list(Loc,[H|T],In,Out) --> ([and];[(',')];[]),
      subject(H,In,Mid),([and];[(',')];[]),
      noun_phrase_list(Loc,T,Mid,Out),{!}. 

noun_phrase(S,A,B)-->subject(S,A,B).

%subject(_,_,_) --> isPOS('#$Verb',_),{!,fail},_.

% a man that walks
subject(List, In, Out, [M,N,O|More], F) :-
      (nth1(Loc,[M,N,O],'who');nth1(Loc,[M,N,O],'that')),
      noun_phrase_rel_clause(Loc,List, In, Out, [M,N,O|More], F).

%rel_clause(Subj,HowDoes) -->isPOS('#$Complementizer',Modal,String),verb_phrase(Subj,Event,HowDoes),{varnameIdea(String,Event)}.
noun_phrase_rel_clause(Loc,Subj,In,rel_clause(In,Out)) --> 
	 subject(Subj,HowDoes,Out), 
	 (isPOS('#$Complementizer',Modal,String);[]),
	 verb_phrase(Subj,Event,HowDoes).


% =======================================================
subject_isa(SubjectIsa,Subj,Template,TemplateO) --> subject(Subj,Template,TemplateO).


% =======================================================
% '#$Determiner-Indefinite' , '#$Determiner-Definite'
%'#$determinerAgreement'('#$A-Half-Dozen-MWW', '#$plural-Generic',

% all dog
subject(Subj,In,'#$forAll'(Subj,AttribIsa)) --> 
    ([every];[all];[forall];[each];[for,all]),
    det_object(Subj,In,AttribIsa).


% the happy dog
%subject(X,In,referant(X,isa(X,Subj),AttribIsa)) --> [the],      det_object(Subj,In,AttribIsa),{varnameIdea('Thing',X),!}.
subject(Subj,In,AttribIsa) --> [the],det_object(Subj,In,AttribIsa).

% a dog
subject(Subj,In,'#$thereExists'(Subj,AttribIsa)) --> 
    ([a];[an];[some];[there,is];[there,are];[there,exists]),
    det_object(Subj,In,AttribIsa).

% your rainbow
subject(X,A,and(ownedBy(X,Agent),isa(X,Thing),B)) --> possessive(Agent),noun_phrase(Thing,A,B),{varnameIdea('Thing',X),!}.

% he
subject(PN,CycL,CycL) --> pronoun(PN),{!}.

% Joe blow
subject(named(Name),CycL,CycL) --> human_name(Name),{!}.

% a man

% dog
subject(Subj,In,AttribIsa) --> det_object(Subj,In,AttribIsa).

% =======================================================
%'#$multiWordSemTrans'([equilateral], '#$Shaped-TheWord', '#$Adjective', '#$RegularAdjFrame', '#$shapeOfObject'(':NOUN', '#$EquilateralShaped'), '#$EnglishMt', v(v('#$Adjective', '#$EquilateralShaped', '#$RegularAdjFrame', '#$Shaped-TheWord', '#$shapeOfObject', ':NOUN', equilateral), A)).
det_object(Subj,In,and(Extras,Out)) --> [S],
  {'#$multiWordSemTrans'([S|String],CycWord, '#$Adjective', NextFrame,Template,_,_)},
     String,isCycWord(CycWord), frame_template(NextFrame,Subj,Result,Extras),
    {apply_frame(Template,Subj,Event,Obj,Result,Out)}.

det_object(Subj,In,Out) --> 
	 isPOS('#$Adjective',CycAdj),
	 det_object_adj(CycAdj,Subj,In,Out).

% =======================================================
%'#$adjSemTrans-Restricted'('#$Wooden-TheWord', 0, '#$RegularAdjFrame', '#$Artifact', '#$isa'(':NOUN', '#$Wood'), '#$GeneralEnglishMt', v(v('#$Artifact', '#$RegularAdjFrame', '#$Wood', '#$Wooden-TheWord', '#$isa', ':NOUN'), A)).
det_object_adj(CycAdj,Subj,In,and(Extras,Out)) --> 
	 {'#$adjSemTrans-Restricted'(CycAdj, _, FrameType, NounIsa, Template,_,_)},
	subject_isa(NounIsa,Subj,Template,TemplateO),
      {apply_frame(TemplateO,Subj,Event,Obj,Result,Out)}.

% =======================================================
%'#$adjSemTrans'('#$Tame-TheWord', 0, '#$RegularAdjFrame', '#$isa'(':NOUN', '#$TameAnimal'), '#$GeneralEnglishMt', v(v('#$RegularAdjFrame', '#$Tame-TheWord', '#$TameAnimal', '#$isa', ':NOUN'), A)).
%'#$adjSemTransTemplate'('#$ColorTingeAttribute', '#$RegularAdjFrame', '#$objectHasColorTinge'(':NOUN', ':DENOT'), '#$GeneralEnglishMt', v(v('#$ColorTingeAttribute', '#$RegularAdjFrame', '#$objectHasColorTinge', ':DENOT', ':NOUN'), A)).
det_object_adj(CycAdj,Subj,In,and(Extras,Out)) --> 
	 {'#$adjSemTrans'(CycAdj, _, FrameType, Template,_,_);
	 ('#$adjSemTransTemplate'(AdjIsa, FrameType, Template,_,_),cycQueryIsa(CycAdj,AdjIsa))},
	frame_template(NextFrame,Subj,Result,Extras),
      {apply_frame(Template,Subj,Event,Obj,Result,Out)}.


det_object_adj(CycAdj,Subj,In,and(Isa,hasTrait(Subj,CycL))) -->
       det_object(Subj,In,Isa),{cvtWordPosCycL(CycAdj,'#$Adjective',CycL),!}.

% =======================================================
det_object(Subj,In,Isa) --> object(Subj,In,Isa).

% =======================================================
det_object(PN,CycL,CycL) --> proper_object(PN).

%'#$multiWordSemTrans'([intended, recipient, of], '#$Communicate-TheWord', '#$SimpleNoun', '#$RegularNounFrame', '#$communicationTarget'(A, ':NOUN'), '#$EnglishMt', v(v('#$Communicate-TheWord', '#$RegularNounFrame', '#$SimpleNoun', '#$communicationTarget', ':NOUN', intended, of, recipient), ['?X'=A|B])).
object(Subj,In,'#$and'(In,Out)) --> [S],
   {'#$multiWordSemTrans'([S|String],CycWord,POS, NextFrame,Template,_,_),POS \= '#$Adjective'},
     String,isCycWord(CycWord), frame_template(NextFrame,Subj,Result,Extras),
    {apply_frame(Template,Subj,Event,Obj,Result,Out)}.

% =======================================================
%'#$nounPrep'('#$Address-TheWord', '#$Of-TheWord', ['#$pointOfContactInfo', ':OBLIQUE-OBJECT', '#$ContactLocation', '#$addressText', ':NOUN']).
%'#$nounPrep'('#$Retail-TheWord', '#$Of-TheWord', ['#$sellsProductType', ':NOUN', ':OBLIQUE-OBJECT']).
%'#$nounPrep'('#$Market-TheWord', '#$Of-TheWord', ['#$sellsProductType', ':NOUN', ':OBLIQUE-OBJECT']).
%'#$nounPrep'('#$Start-TheWord', '#$Of-TheWord', ['#$startingPoint', ':OBLIQUE-OBJECT', ':NOUN']).
%'#$nounPrep'('#$City-TheWord', '#$Of-TheWord', '#$equals'(':OBLIQUE-OBJECT', ':NOUN'), '#$EnglishMt', v(v('#$City-TheWord', '#$Of-TheWord', '#$equals', ':NOUN', ':OBLIQUE-OBJECT'), A)).
%'#$nounPrep'('#$Victim-TheWord', '#$Of-TheWord', '#$victim'(':OBLIQUE-OBJECT', ':NOUN'), '#$EnglishMt', v(v('#$Of-TheWord', '#$Victim-TheWord', '#$victim', ':NOUN', ':OBLIQUE-OBJECT'), A)).
object(Subject,In,and(CycL,Out)) --> isPOS('#$Noun',CycWord), 
      {'#$nounPrep'(CycWord,CycWordPrep, Template,_,_)},
      isCycWord(CycWordPrep),subject(Result,In,CycL),
    {apply_frame(Template,Subject,Event,Object,Result,Out)}.

% the happy dog
object(Subj,CycL,and(CycL,Isa)) --> colection(Subj,Isa).

%% of what the product is
%'team-mate'
% happy dog
% kickoff call
colection(Subj,isaMember(Subj,W)) --> [W],{atom(W),atom_concat('#$',_,W),!}.
colection(Subj,isaMember(Subj,CycL)) --> isPOS('#$Noun',CycWord,String),
	    {cvtWordPosCycL(CycWord,'#$Noun',CycL),varnameIdea(String,Subj),!}.

wordPosCycL(CycWord,POS,CycL):-
      '#$denotation'(CycWord, POS, _, CycL,_,_);'#$denotationRelatedTo'(CycWord, POS, _, CycL,_,_).
wordPosCycL(CycWord,POS,CycL):-'#$genls'(Child,POS,_,_),wordPosCycL(CycWord,Child,CycL).
wordPosCycL(CycWord,_,CycL):-
      '#$denotation'(CycWord, POS, _, CycL,_,_);'#$denotationRelatedTo'(CycWord, POS, _, CycL,_,_).
      

cvtWordPosCycL(CycWord,POS,CycL):-wordPosCycL(CycWord,POS,CycL),!.
cvtWordPosCycL(CycWord,POS,CycL):-CycL=..[POS,CycWord],!.
% ==========================================================
% String to CYC - POS
% ==========================================================
proper_object(CycL) --> [S,S2],{poStr(CycL,[S,S2|String])},String.
proper_object(CycL) --> [String],{poStr(CycL,String)}.

poStr(CycL,String):-
      '#$initialismString'(CycL,String,_,_);
      '#$abbreviationString-PN'(CycL, String,_,_);
      '#$preferredNameString'(CycL, String,_,_);
      '#$countryName-LongForm'(CycL, String,_,_);
      '#$countryName-ShortForm'(CycL, String,_,_);
      '#$acronymString'(CycL,String, _,_);
      '#$scientificName'(CycL,String, _,_);
      '#$termStrings'(CycL,String, _,_);
      '#$termStrings-GuessedFromName'(CycL,String, _,_);
      '#$prettyName'(CycL,String, _,_);
      '#$nameString'(CycL,String, _,_);
      '#$nicknames'(CycL,String, _,_);
      '#$preferredTermStrings'(CycL,String, _,_).

proper_object(CycL) --> {'#$genPhrase'(CycL, POS,Form, String,_,_)},String.

proper_object(CycL) --> {'#$termPOS-Strings'(CycL,POS,String,_,_)},literal(String).

proper_object(CycL) -->
	 isPOS('#$NLWordForm',CycWord),
	 {'#$compoundString'(CycWord,String, POS,CycL,_,_)},String.

proper_object(CycL) --> [S],{'#$multiWordString'([S|String], CycWord,POS,CycL,_,_)},
	    String,isCycWord(CycWord).

proper_object(CycL) --> [S],{'#$headMedialString'([S|String], CycWord,POS,Right,CycL,_,_)},
	    String,isCycWord(CycWord),Right.


proper_object(multFn(Multiply,Collection)) --> [String],
	 {'#$unitOfMeasurePrefixString'(Multiply, Prefix,_,_),
	 atom_concat(Prefix,Rest,String),!,phrase(collection(Collection),[Rest])}.

proper_object(CycL) -->  [String],
      {concat_atom([Left,Right],'-',String),
      '#$hyphenString'([Left], RightWord, POS,CycL, _,_),
	 phrase(isCycWord(RightWord),[Rest])}.

%possessive(Agent)-->pronoun(Agent),isCycWord('#$Have-Contracted'),{!}.
possessive(Agent)-->pronoun(Agent),isCycWord('#$Be-Contracted').
possessive(Agent)-->isPOS('#$PossessivePronoun-Pre',Agent).
possessive(Agent)-->isPOS('#$PossessivePronoun-Post',Agent).
possessive(Inters)-->human_name(Inters),['\'',s].
%possessive(Agent)-->pronoun(Agent).

pronoun('?Speaker') --> isCycWord('#$I-TheWord');isCycWord('#$Me-TheWord').
pronoun('?TargetAgent') --> isCycWord('#$You-TheWord').
pronoun('?Where') --> isCycWord('#$Where-TheWord').
pronoun('?How') --> isCycWord('#$How-TheWord').
pronoun('?IT') --> isCycWord('#$It-TheWord').
pronoun('?He') --> isCycWord('#$He-TheWord').
pronoun('?She') --> isCycWord('#$She-TheWord').

pronoun(X) --> wh_pronoun(X).
pronoun(ref(CycWord)) --> isPOS('#$Pronoun', CycWord).

wh_pronoun('?Agent') --> [who].
wh_pronoun('?What') --> [what].

% ==========================================================
% POS DCG
% ==========================================================
isForm(POS,CycWord,Form) --> [String],{notrace(meetsForm(Form,String,POS,CycWord)),!}.
isForm(POS,CycWord,Form) --> [S,W|String],{notrace(meetsForm(Form,[S,W|String],POS,CycWord))},String.

meetsForm(Form,String,POS,CycWord):- (var(String);var(Form)),throw(meetsForm(String,POS,CycWord)).
meetsForm(Form,String,POS,CycWord):-stringToWordForm(String,CycWord,Form),cycWordPosForm(POS,CycWord,Form).

meetsForm(Form,String,POS,CycWord):-'#$genlPreds'(Child,Form,_,_),meetsForm(Child,String,POS,CycWord).


isPOS(POS,CycWord) --> isPOS(POS,CycWord,String).

isPOS(POS,CycWord,String) --> [String],{notrace(meetsPos(String,POS,CycWord)),!}.
isPOS(POS,CycWord,String) --> [S,W],{notrace(meetsPos([S,W|String],POS,CycWord))},String.

cycWordPosForm(POS,CycWord,Form):-
	 '#$preferredGenUnit'(CycL,POS, CycWord,_,_);
	 '#$posBaseForms'(CycWord,POS,_,_);
	 '#$posForms'(CycWord,POS,_,_);
	 '#$denotation'(CycWord,POS, Arg, CycL, _,_);
	 '#$speechPartPreds'(POS, Form, _,_).


meetsPos(String,POS,CycWord):- (var(String);var(POS)),throw(meetsPos(String,POS,CycWord)).
meetsPos([String],POS,CycWord):-!,meetsPos(String,POS,CycWord).
meetsPos(String,POS,CycWord):-'#$partOfSpeech'(CycWord,POS, String,_,_).
meetsPos(String,POS,CycWord):-stringToWordForm(String,CycWord,Form),cycWordPosForm(POS,CycWord,Form).
meetsPos(String,POS,CycWord):-'#$genls'(Child,POS,_,_),meetsPos(String,Child,CycWord).
meetsPos(String,'#$Verb',CycWord):-atom(String),meetsPosVerb(String,CycWord),!.

%meetsPos(String,'#$Noun',CycWord):-atom(String),meetsPosNoun(String,CycWord).
meetsPosVerb(String,CycWord):-atom_concat(S,'ed',String),meetsPos(S,'#$Verb',CycWord).
meetsPosVerb(String,CycWord):-atom_concat(S,'s',String),meetsPos(S,'#$Verb',CycWord).

meetsPos(String,POS,CycWord):-
	 memberchk(POS,['#$Noun','#$Adjective','#$Verb','#$Adverb']),
	 '#$wnS'(CycWord,_, String,POS, _,_,_,_),!.
meetsPos(String,'#$Adjective',CycWord):-'#$wnS'(CycWord,_, String, '#$AdjectiveSatellite', _,_,_,_).

meetsPos(String,POS,CycWord):- atom(String),
	    '#$prefixString'(CycWord, Prefix, _,_),
	    atom_concat(Prefix,_,String),
	    '#$derivationalAffixBasePOS'(CycWord,POS,_,_).


% Wordnet
wordToWNPOS(CycWord,WNWord,POS):-'#$denotationPlaceholder'(CycWord,POS, _, WNWord, _,_).
%'#$synonymousExternalConcept'('#$AbandoningSomething', '#$WordNet-1995Version', 'V01269572', '#$WordNetMappingMt', v(v('#$AbandoningSomething', '#$WordNet-1995Version', 'V01269572'), A)).


% ==========================================================
% String / Word
% ==========================================================


% cycWordForISA
cycWordForISA(CycWord,EventIsa):-fail.


% peace atal beh - 695-1297
%isCycWord(CycWord) --> {var(CycWord),!,trace}.
isCycWord(CycWord) --> {notrace(cycWordFromString(CycWord,String))},literal(String).

cycWordFromString(CycWord,String):-'#$baseForm'(CycWord,String,_,_).
cycWordFromString(CycWord,String):-'#$partOfSpeech'(CycWord,POS, String,_,_).
cycWordFromString(CycWord,String):- stringToWordForm(String,CycWord,Form).


% ==========================================================
% stringToWordForm(String,CycWord,Form)
% ==========================================================
%stringToWordForm([W],CycWord,Form):-atom(W),!,stringToWordForm(W,CycWord,Form).
stringToWordForm(String,CycWord,Form):-'#$abbreviationForLexicalWord'(CycWord,Form, String,_,_).

% Nouns
stringToWordForm(String,CycWord,'#$singular'):-'#$singular'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$plural'):-'#$plural'(CycWord, String, _,_).
%stringToWordForm(String,CycWord,'#$nonPlural-Generic'):-'#$nonPlural-Generic'(CycWord, String, _,_).

stringToWordForm(String,CycWord,'#$agentive-Mass'):-'#$agentive-Mass'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$agentive-Pl'):-'#$agentive-Pl'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$agentive-Sg'):-'#$agentive-Sg'(CycWord, String, _,_).
%stringToWordForm(String,CycWord,'#$singular-Feminine'):-'#$singular-Feminine'(CycWord, String, _,_).
%stringToWordForm(String,CycWord,'#$singular-Masculine'):-'#$singular-Masculine'(CycWord, String, _,_).
%stringToWordForm(String,CycWord,'#$singular-Neuter'):-'#$singular-Neuter'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$massNumber'):-'#$massNumber'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$pnSingular'):-'#$pnSingular'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$pnMassNumber'):-'#$pnMassNumber'(CycWord, String, _,_).

% Adjectives
stringToWordForm(String,CycWord,'#$regularDegree'):-'#$regularDegree'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$comparativeDegree'):-'#$comparativeDegree'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$superlativeDegree'):-'#$superlativeDegree'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$nonGradableAdjectiveForm'):-'#$nonGradableAdjectiveForm'(CycWord, String, _,_).

% Adverbs
stringToWordForm(String,CycWord,'#$regularAdverb'):-'#$regularAdverb'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$superlativeAdverb'):-'#$superlativeAdverb'(CycWord, String, _,_).

% Verbs
stringToWordForm(String,CycWord,'#$infinitive'):-'#$infinitive'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$perfect'):-'#$perfect'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$presentParticiple'):-'#$presentParticiple'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$pastTense-Universal'):-'#$pastTense-Universal'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$presentTense-Universal'):-'#$presentTense-Universal'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$firstPersonSg-Present'):-'#$firstPersonSg-Present'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$secondPersonSg-Present'):-'#$secondPersonSg-Present'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$nonThirdSg-Present'):-'#$nonThirdSg-Present'(CycWord, String, _,_).
stringToWordForm(String,CycWord,'#$thirdPersonSg-Present'):-'#$thirdPersonSg-Present'(CycWord, String, _,_).
stringToWordForm(String,CycWord,Form):-atom(String),!,stringAtomToWordForm(String,CycWord,Form).
%stringToWordForm(String,CycWord,Form):-nonvar(String),stringListToWordForm(String,CycWord,Form).
stringAtomToWordForm(String,CycWord,Form):-
	    '#$regularSuffix'(Before, Form, Suffix, _,_),
	    atom_concat(NewString,Suffix,String),
	    stringToWordForm(NewString,CycWord,Before).
%'#$suffixString'('#$Y_AdjectiveProducing-TheSuffix', y, '#$GeneralEnglishMt', v(v('#$Y_AdjectiveProducing-TheSuffix', y), A)).

%'#$variantOfSuffix'('#$Able-TheSuffix', ible, '#$GeneralEnglishMt', v(v('#$Able-TheSuffix', ible), A)).
%'#$variantOfSuffix'('#$Al_AdjectiveProducing-TheSuffix', ual, '#$GeneralEnglishMt', v(v('#$Al_AdjectiveProducing-TheSuffix', ual), A)).

%'#$compoundStringDenotesArgInReln'('#$Actor-TheWord', [remaining, afterwards], '#$CountNoun', '#$postActors', 2, '#$GeneralEnglishMt', v(v('#$Actor-TheWord', '#$CountNoun', '#$postActors', afterwards, remaining), A)).
%'#$multiWordStringDenotesArgInReln'([unchanged], '#$Actor-TheWord', '#$SimpleNoun', '#$unchangedActors', 2, '#$GeneralEnglishMt', v(v('#$Actor-TheWord', '#$SimpleNoun', '#$unchangedActors', unchanged), A)).
	 

% ==========================================================
% String to WordsList - Form / POS
% ==========================================================
stringToWordsForm(String,[CycWord|Words],Form):- 
         '#$abbreviationForCompoundString'(CycWord,WordList,Form,String,_,_),
	 stringToWords(WordList,Words,_).

stringToWordsPOS(String,CycWords,POS):-
      '#$abbreviationForMultiWordString'(List,Word,POS,String,_,_),
      stringToWordsForm(List,Words,_),
      append(Words,[Word],CycWords).


varnameIdea(X,Y):-varnameIdea2(X,Y),!.
varnameIdea2([String|_],Subj):-!,varnameIdea2(String,Subj).
varnameIdea2('?TargetAgent','?TargetAgent').
varnameIdea2('TargetAgent','?TargetAgent').
varnameIdea2('?Speaker','?Speaker').
varnameIdea2(String,Subj):-atom(String),var(Subj),atom_concat('?',String,Sym),gensym(Sym,Subj).
varnameIdea2(String,Subj).

% =======================================================
% Conjunctions
% =======================================================
% [that];[who]

conjunct --> conjunct(X).
conjunct(C) --> isPOS('#$CoordinatingConjunction',C).
conjunct(C)--> [and];[also].

disj_word --> [or];[not];[but].


modal_phrase(CycAuxVerb,Subj,Event,Out)-->aux_phrase(CycAuxVerb,Subj,Event,Out).
% =======================================================
% Aux Phrases
% =======================================================

% is good
aux_phrase('#$Be-TheWord',Subj,Event,and(CycL,hasTrait(Subj,AdjCycL))) -->
	 isPOS('#$Adjective',CycAdj),
      aux_phrase('#$Be-TheWord',Subj,Event,CycL),
      {cvtWordPosCycL(CycAdj,'#$Adjective',AdjCycL)}.


% can
aux_phrase('#$Can-TheModal',Subj,Event,'can'(Subj,CycL)) --> 
	  verb_phrase(Subj,Event,CycL).

% do/is/be/does
aux_phrase(CycAuxVerb,Subj,Event,aux_isa_for(Subj,Event,Action)) --> [],
	 {cvtWordPosCycL(CycAuxVerb,'#$Verb',Action)}.

% do <X>
aux_phrase('#$Do-TheWord',Subj,Event,(CycL)) --> 
	 verb_phrase(Subj,Event,CycL) .

% =======================================================
%'#$verbPrep-Passive'('#$Make-TheWord', '#$Of-TheWord', '#$mainConstituent'(':OBJECT', ':OBLIQUE-OBJECT'), '#$EnglishMt', v(v('#$Make-TheWord', '#$Of-TheWord', '#$mainConstituent', ':OBJECT', ':OBLIQUE-OBJECT'), A)).
aux_phrase(CycWord,Subj,Event,CycLO) --> 
      {'#$verbPrep-Passive'(CycWord, CycWord2, Template,_,_)},
       isCycWord(CycWord2),subject(Result,Out,CycLO),
      {apply_frame(Template,Subj,Event,Obj,Result,Out)}.

% =======================================================
%'#$prepSemTrans'('#$Above-TheWord', 0, '#$Post-NounPhraseModifyingFrame', '#$above-Generally'(':NOUN', ':OBJECT'), '#$GeneralEnglishMt', v(v('#$Above-TheWord', '#$Post-NounPhraseModifyingFrame', '#$above-Generally', ':NOUN', ':OBJECT'), A)).
aux_phrase(CycAuxWord,Subj,Event,Out) -->
      {'#$prepSemTrans'(CycWordPrep, _, NextFrame, Template,_,_)},
      isCycWord(CycWordPrep),subject(Obj,Out,CycLO),
    {apply_frame(Template,Subj,Event,Obj,Result,Out)}.

% =======================================================
% preopistional_phrase
% =======================================================
preopistional_phrase(Oblique,CycWord,CycL) -->
	 isPOS('#$Preposition',CycWord), 
	 noun_phrase(Oblique,Prep,CycL),{varnameIdea('Prep',Prep)}.
      
% =======================================================
% verb_phrase
% =======================================================

% no verb
verb_phrase(_,_,_) --> isPOS('#$Determiner',CycWord),{!,fail}.

verb_phrase_after_nouns(Subj,Event,exists(Subj)) --> [].

verb_phrase_after_nouns(Subj,Event,CycL) --> 
	 verb_phrase(Subj,Event,CycL).

% One verb
%verb_phrase(Subj,Event,do(Subj,Verb)) --> [Verb].

% known phrase
verb_phrase(Subj,Event,known_phrase(CycL)) --> 
	    isPOS('#$Verb',CycVerb),
	    verb_phrase_known(CycVerb,Subj,Event,CycL).

% gen phrase 2
verb_phrase(Subj,Event,gen_phrase2(Call,Result)) --> [S,N],
	    { '#$genFormat'(Predicate,['~a',S,N|Template],ArgsI,_,_),atom(Predicate),
	    (compound(ArgsI) -> trasfromArgs(ArgsI,Args) ; Args=[1,2]),
	    length(Args,Size),functor(Call,Predicate,Size),
	    placeVars([Subj|Blanks],Args,Call)},
	    do_dcg(Template,Blanks,Result).
	    
% modal phrase
verb_phrase(Subj,Event,modal(CycL)) --> 
	    isPOS('#$Modal',CycWord,String),
	    modal_phrase(CycWord,Subj,Event,CycL),{varnameIdea(String,Event)}.

% aux phrase
verb_phrase(Subj,Event,(CycL)) --> 
	    isPOS('#$AuxVerb',CycWord,String),
	    aux_phrase(CycWord,Subj,Event,CycL),{varnameIdea(String,Event)}.

% adverbal phrase
verb_phrase(Subj,Event,'and_adverbal'(Event,AdvCycL,CycL))  --> 
	    isPOS('#$Adverb',CycWord),
	    verb_phrase(Subj,Event,CycL),
	    {cvtWordPosCycL(CycWord,'#$Adverb',AdvCycL)}.

% unknown phrase has arity CycL + object %TODO rename subject/3 to noun_phrase/3
verb_phrase(Subj,Event,and_concat(CycL)) --> [Verb],
	 {atom(Verb),((atom_concat('#$',Verb,Predicate),'#$arity'(Predicate,N,_,_));('#$arity'(Verb,N,_,_),Predicate=Verb)),!},
	 verb_phrase_arity(N,Predicate,Subj,Event,CycL).

:-index(verb_phrase_arity(0,0,0,0,0,0,0)).
%TODO rename subject/3 to noun_phrase/3
verb_phrase_arity(2,Predicate,Subj,Event,CycL) --> 
	       best_subject(Obj,ACT,Mid),
	       colect_noun_list(List,Mid,CycL),
	       {apply_act(Predicate,Subj,[Obj|List],ACT)}.
%and
verb_phrase_arity(3,Predicate,Subj,Event,CycL) --> 
	 best_subject(Obj,Event,Mid),
	 best_subject_constituant(RES,Event,Mid,CycL).
	{ACT=..[Predicate,Subj,Obj,RES]}.

colect_noun_list([],In,In) --> [].
colect_noun_list([H|T],In,Out) --> ([(',')];[and];[]),
      best_subject(H,In,Mid),
      colect_noun_list(T,Mid,Out).

verb_phrase(Subj,Event,(CycL)) --> 
	 isPOS('#$Verb',CycVerb,String),
%	 best_subject(Obj,true,CycL),
 %        best_subject_constituant(Target,Event,CycL,CycLO),
	 {cvtWordPosCycL(CycVerb,'#$Verb',Verb),
	 (atom(Verb),(atom_concat('#$',Verb,Predicate),'#$arity'(Predicate,N,_,_));('#$arity'(Verb,N,_,_),Predicate=Verb)),!},
	  verb_phrase_arity(N,Predicate,Subj,Event,CycL),{varnameIdea(String,Event)}.
	 
% gen phrase 1
verb_phrase(Subj,Event,gen_phrase1(Call,Result)) --> [S],
	    {S\=is, '#$genFormat'(Predicate,['~a',S|Template],ArgsI,_,_),
	    (compound(ArgsI) -> trasfromArgs(ArgsI,Args) ; Args=[1,2]),
	    length(Args,Size),functor(Call,Predicate,Size),atom(Predicate),
	    placeVars([Subj|Blanks],Args,Call)},
	    do_dcg(Template,Blanks,Result).



% unkown phrase	+ object %TODO rename subject/3 to noun_phrase/3
verb_phrase(Subj,Event,and(isaAction(Event,Action),'#$doneBy'(Event,Subj),'#$constituentInSituation'(Event,Obj),CycLO)) --> 
	 isPOS('#$Verb',CycVerb,String),
	 best_subject(Obj,true,CycL),
	 best_subject_constituant(Target,Event,CycL,CycLO),
	 {cvtWordPosCycL(CycVerb,'#$Verb',Action),varnameIdea(String,Event)}.
														  
% unkown phrase + text
best_subject(Obj,Event,CycL) --> isPOS('#$Preposition',_),{!},best_subject(Obj,Event,CycL).
best_subject(Obj,Event,CycL) --> noun_phrase(Obj,Event,CycL),{!}.
best_subject(Obj,CycL,CycL) --> rest_of(Obj).

best_subject_constituant(RES,Event,CycL,CycL) --> [].
best_subject_constituant(Target,Event,CycL,and(CycL,CycLO,'#$eventOccursAt'(Event,Target))) --> 
	 best_subject(Target,Event,CycLO).
   
%rest_of(txt([A|C])) --> [A|C].
rest_of(thingFor(Rest), [A|Rest], []):-notrace(meetsPos(A,'#$Determiner',CycWord)),!.
rest_of(thingFor(Rest), Rest, []):-Rest=[_|_].

%%lowerCasePred(CycLPred)
lowerCasePred(Pred):-atom(Pred),atom_codes(Pred,[_,_,C|_]),char_type(C,lower).

apply_act(Predicate,Subj,Obj,ACT) :- \+ ';'(is_list(Subj),is_list(Obj)),!,ACT=..[Predicate,Subj,Obj].

apply_act(Predicate,Subj,[Obj],ACT):-!,ACT=..[Predicate,Subj,Obj].
apply_act(Predicate,Subj,[Obj|List],each(ACT,MORE)):-
      ACT=..[Predicate,Subj,Obj],apply_act(Predicate,Subj,List,MORE),!.

apply_act(Predicate,[Obj],Subj,ACT):-!,ACT=..[Predicate,Obj,Subj].
apply_act(Predicate,[Obj|List],Subj,each(ACT,MORE)):-
      ACT=..[Predicate,Obj,Subj],apply_act(Predicate,List,Subj,MORE),!.

% =======================================================
% GENFORMAT Verbs TODO
% =======================================================

do_dcg([],_,nil_true) --> {!},[].
do_dcg(['~a'|Template],[Subj|Blanks],(Result)) -->{!},
	    noun_phrase(Subj,More,Result),
      do_dcg(Template,Blanks,More).
do_dcg(Template,Blanks,end_true) --> Template,{!}.
do_dcg([Word|Template],[Subj|Blanks],(Result)) --> [Word],
      {append(Find,['~a'|More],Template),!},
      Find,noun_phrase(Subj,CycL,Result),
      do_dcg(More,Blanks,CycL).

/*
genFormatVerb2(Term,String,More,Subj,CycLO,noun_phrase(Object,[Predicate,Subj,Object],CycLO)):-
      append(String,['~a'],More),!.
genFormatVerb2([2,1],Predicate,String,More,Subj,CycLO,noun_phrase(Object,[Predicate,Object,Subj],CycLO)):-
      append(String,['~a'],More),!.

genFormatVerb2(Call,[Subj|Blanks],String,More,CycLO,ToDO):-!.
*/

%trasfromArgs(Args,List).
trasfromArgs('NIL',[1,2,3,4,5,6]):-!.
trasfromArgs([H],[HH]):-trasfromArg(H,HH),!.
trasfromArgs([H|T],[HH|TT]):-trasfromArg(H,HH),trasfromArgs(T,TT),!.

trasfromArg([[]|_],_).
trasfromArg([H|_],H).
trasfromArg(H,H).

placeVars([Subj],[N],Call):-integer(N),arg(N,Call,Subj),!.
placeVars([Subj|Blanks],[N|More],Call):-integer(N),arg(N,Call,Subj),placeVars(Blanks,More,Call),!.
      

      	 

%'#$genTemplate'('#$many-GenQuantRelnToType', '#$TermParaphraseFn'([':ARG1', '#$BestDetNbarFn'('#$TermParaphraseFn'('#$Many-NLAttr'), '#$TermParaphraseFn-Constrained'('#$plural-Generic', ':ARG2')), '#$ConditionalPhraseFn'('#$equals'(':ARG3', '#$Thing'), '#$BestNLPhraseOfStringFn'(something), '#$BestDetNbarFn'('#$TermParaphraseFn'('#$BareForm-NLAttr'), '#$TermParaphraseFn-Constrained'('#$nonSingular-Generic', ':ARG3')))]), '#$EnglishParaphraseMt', v(v('#$BareForm-NLAttr', '#$BestDetNbarFn', '#$BestNLPhraseOfStringFn', '#$ConditionalPhraseFn', '#$Many-NLAttr', '#$TermParaphraseFn', '#$TermParaphraseFn-Constrained', '#$Thing', '#$equals', '#$many-GenQuantRelnToType', '#$nonSingular-Generic', '#$plural-Generic', ':ARG1', ':ARG2', ':ARG3', something), A)).
%'#$genTemplate'('#$many-GenQuant', '#$TermParaphraseFn'('#$elementOf'('#$BestDetNbarFn'('#$TermParaphraseFn'('#$Many-NLAttr'), '#$TermParaphraseFn-Constrained'('#$nonSingular-Generic', ':ARG1')), ':ARG2')), '#$EnglishParaphraseMt', v(v('#$BestDetNbarFn', '#$Many-NLAttr', '#$TermParaphraseFn', '#$TermParaphraseFn-Constrained', '#$elementOf', '#$many-GenQuant', '#$nonSingular-Generic', ':ARG1', ':ARG2'), A)).
%'#$genTemplate'('#$markCreated', '#$ConcatenatePhrasesFn'('#$TermParaphraseFn-NP'(':ARG2'), '#$BestHeadVerbForInitialSubjectFn'('#$Be-TheWord'), '#$BestNLPhraseOfStringFn'([the, mark, created, by]), '#$TermParaphraseFn-NP'(':ARG2')), _,_)

% =======================================================
% Intrans phrase                                                                    
verb_phrase_known(CycWord,Subj,Event,CycLO) --> 
	 [],{cvtWordPosCycL(CycWord,'#$Verb',CycL),
	 (('#$arg2Isa'(CycL,Type,_,_),Rel=..[CycL,Subj,Obj],
	 CycLO = and_iv(isa(Obj,Type),Rel) );
	 CycLO=and_iv('#$bodilyDoer'(Subj,Event),event_isa(Event,CycL))),varnameIdea('Intrans',Event),varnameIdea('Thing',Obj)}.

% TODO
%'#$agentiveNounSemTrans'('#$Assist-TheWord', 0, '#$RegularNounFrame', '#$assistingAgent'(A, ':NOUN'), '#$GeneralEnglishMt', v(v('#$Assist-TheWord', '#$RegularNounFrame', '#$assistingAgent', ':NOUN'), ['?X'=A|B])).
%'#$agentiveNounSemTrans'('#$Emit-TheWord', 0, '#$RegularNounFrame', ['#$emitter', '?X', ':NOUN']).	    
% =======================================================
%'#$lightVerb-TransitiveSemTrans'('#$Take-TheWord', '#$DrugProduct', '#$and'('#$isa'(':ACTION', '#$Ingesting'), '#$performedBy'(':ACTION', ':SUBJECT'), '#$primaryObjectMoving'(':ACTION', ':OBJECT')), '#$EnglishMt', v(v('#$DrugProduct', '#$Ingesting', '#$Take-TheWord', '#$and', '#$isa', '#$performedBy', '#$primaryObjectMoving', ':ACTION', ':OBJECT', ':SUBJECT'), A)).
verb_phrase_known(CycWord,Subj,Event,'lightVerb-TransitiveSemTrans'(Out)) -->
	{'#$lightVerb-TransitiveSemTrans'(CycWord,ObjectIsa, Template,_,_)},
	subject_isa(ObjectIsa,Object,Template,TemplateO),
     {apply_frame(TemplateO,Subj,Event,Object,Result,Out)}.

% =======================================================
%'#$prepReln-Action'('#$LosingUserRights', '#$Agent', '#$From-TheWord', '#$fromPossessor'(':ACTION', ':OBLIQUE-OBJECT'), '#$EnglishMt', v(v('#$Agent', '#$From-TheWord', '#$LosingUserRights', '#$fromPossessor', ':ACTION', ':OBLIQUE-OBJECT'), A)).
%'#$prepReln-Action'('#$MovementEvent', '#$PartiallyTangible', '#$From-TheWord', '#$fromLocation'(':ACTION', ':OBLIQUE-OBJECT'), '#$EnglishMt', v(v('#$From-TheWord', '#$MovementEvent', '#$PartiallyTangible', '#$fromLocation', ':ACTION', ':OBLIQUE-OBJECT'), A)).
%'#$prepReln-Action'('#$Movement-TranslationEvent', '#$SomethingExisting', '#$On-TheWord', '#$toLocation'(':ACTION', ':OBLIQUE-OBJECT'), '#$EnglishMt', v(v('#$Movement-TranslationEvent', '#$On-TheWord', '#$SomethingExisting', '#$toLocation', ':ACTION', ':OBLIQUE-OBJECT'), A)).
%'#$prepReln-Action'('#$Stealing-Generic', '#$Agent', '#$From-TheWord', '#$victim'(':ACTION', ':OBLIQUE-OBJECT'), '#$EnglishMt', v(v('#$Agent', '#$From-TheWord', '#$Stealing-Generic', '#$victim', ':ACTION', ':OBLIQUE-OBJECT'), A)).
%'#$prepReln-Action'('#$TransportationEvent', '#$Conveyance', '#$By-TheWord', '#$transporter'(':ACTION', ':OBLIQUE-OBJECT'), '#$EnglishMt', v(v('#$By-TheWord', '#$Conveyance', '#$TransportationEvent', '#$transporter', ':ACTION', ':OBLIQUE-OBJECT'), A)).
verb_phrase_known(CycWord,Subj,Event,'prepReln-Action'(CycLO,Out)) -->
      {'#$prepReln-Action'(EventIsa, SubjIsa, CycWordPrep, Template,_,_),cycQueryIsa(Subj,SubjIsa)},
	verb_phrase_event_isa(CycWord,EventIsa,Subj,Object,Event,EventMid),
      isCycWord(CycWordPrep),subject(Result,EventMid,CycLO),
    {apply_frame(Template,Subject,Event,Object,Result,Out)}.

% =======================================================
%'#$prepReln-Object'('#$Action', '#$PartiallyTangible', '#$Of-TheWord', '#$objectActedOn'(':NOUN', ':OBLIQUE-OBJECT'), '#$EnglishMt', v(v('#$Action', '#$Of-TheWord', '#$PartiallyTangible', '#$objectActedOn', ':NOUN', ':OBLIQUE-OBJECT'), A)).
%'#$prepReln-Object'('#$AnimalBodyPartType', '#$Animal', '#$Of-TheWord', '#$anatomicalParts'(':OBLIQUE-OBJECT', ':NOUN'), '#$EnglishMt', v(v('#$Animal', '#$AnimalBodyPartType', '#$Of-TheWord', '#$anatomicalParts', ':NOUN', ':OBLIQUE-OBJECT'), A)).
%'#$prepReln-Object'('#$Area', '#$PartiallyTangible', '#$Of-TheWord', '#$areaOfObject'(':OBLIQUE-OBJECT', ':NOUN'), '#$EnglishMt', v(v('#$Area', '#$Of-TheWord', '#$PartiallyTangible', '#$areaOfObject', ':NOUN', ':OBLIQUE-OBJECT'), A)).
%'#$prepReln-Object'('#$CapitalCityOfRegion', '#$IndependentCountry', '#$Of-TheWord', '#$capitalCity'(':OBLIQUE-OBJECT', ':SUBJECT'), '#$EnglishMt', v(v('#$CapitalCityOfRegion', '#$IndependentCountry', '#$Of-TheWord', '#$capitalCity', ':OBLIQUE-OBJECT', ':SUBJECT'), A)).
%'#$prepReln-Object'('#$Communicating', '#$Agent', '#$By-TheWord', '#$senderOfInfo'(':NOUN', ':OBLIQUE-OBJECT'), '#$EnglishMt', v(v('#$Agent', '#$By-TheWord', '#$Communicating', '#$senderOfInfo', ':NOUN', ':OBLIQUE-OBJECT'), A)).
verb_phrase_known(CycWord,Subj,Event,'prepReln-Object'(Out)) -->
      {'#$prepReln-Object'(SubjIsa, ObjectIsa, CycWordPrep, Template,_,_),cycQueryIsa(Subj,SubjIsa)},
	subject_isa(ObjectIsa,Object,Template,TemplateO),
      isCycWord(CycWordPrep),subject(Result,TemplateO,CycLO),
    {apply_frame(CycLO,Subject,Event,Object,Result,Out)}.

% =======================================================
%'#$verbSemTrans'('#$Depart-TheWord', 0, '#$PPCompFrameFn'('#$TransitivePPCompFrame', '#$From-TheWord'), '#$and'('#$isa'(':ACTION', '#$LeavingAPlace'), '#$fromLocation'(':ACTION', ':OBLIQUE-OBJECT'), '#$doneBy'(':ACTION', ':SUBJECT')), '#$GeneralEnglishMt', v(v('#$Depart-TheWord', '#$From-TheWord', '#$LeavingAPlace', '#$PPCompFrameFn', '#$TransitivePPCompFrame', '#$and', '#$doneBy', '#$fromLocation', '#$isa', ':ACTION', ':OBLIQUE-OBJECT', ':SUBJECT'), A)).
verb_phrase_known(CycWord,Subj,Event,verbSemTrans(Out,Extras)) -->
	 {'#$verbSemTrans'(CycWord, _, NextFrame, Template,_,_)},
	    frame_template(NextFrame,Object,Result,Extras),
	 {apply_frame(Template,Subj,Event,Object,Result,Out)}.

% =======================================================
%'#$verbPrep-Transitive'('#$Ablate-TheWord', '#$From-TheWord', '#$and'('#$isa'(':ACTION', '#$Ablation'), '#$objectOfStateChange'(':ACTION', ':OBLIQUE-OBJECT'), '#$doneBy'(':ACTION', ':SUBJECT'), '#$objectRemoved'(':ACTION', ':OBJECT')), '#$EnglishMt', v(v('#$Ablate-TheWord', '#$Ablation', '#$From-TheWord', '#$and', '#$doneBy', '#$isa', '#$objectOfStateChange', '#$objectRemoved', ':ACTION', ':OBJECT', ':OBLIQUE-OBJECT', ':SUBJECT'), A)).
verb_phrase_known(CycWord,Subj,Event,'verbPrep-Transitive'(Out,Extras)) -->
   {'#$verbPrep-Transitive'(CycWord, CycWord2, Template,_,_)},
	 isCycWord(CycWord2),{!},subject(Result,Out,CycLO),
   {apply_frame(Template,Subj,Event,Obj,Result,Out)}.
   
% =======================================================
%'#$compoundVerbSemTrans'('#$Give-TheWord', [off], '#$TransitiveNPCompFrame', '#$and'('#$isa'(':ACTION', '#$EmittingAnObject'), '#$emitter'(':ACTION', ':SUBJECT'), '#$objectEmitted'(':ACTION', ':OBJECT')), '#$EnglishMt', v(v('#$EmittingAnObject', '#$Give-TheWord', '#$TransitiveNPCompFrame', '#$and', '#$emitter', '#$isa', '#$objectEmitted', ':ACTION', ':OBJECT', ':SUBJECT', off), A)).
verb_phrase_known(CycWord,Subj,Event,compoundVerbSemTrans(Out,Extras)) -->
   [S],{'#$compoundVerbSemTrans'(CycWord, [S|String],NextFrame, Template,_,_)},
   String,frame_template(NextFrame,Object,Result,Extras),
   {apply_frame(Template,Subj,Event,Object,Result,Out)}.

% =======================================================
%'#$compoundSemTrans'('#$End-TheWord', [during], '#$Verb', '#$TransitiveNPCompFrame', '#$endsDuring'(':SUBJECT', ':OBJECT'), '#$EnglishMt', v(v('#$End-TheWord', '#$TransitiveNPCompFrame', '#$Verb', '#$endsDuring', ':OBJECT', ':SUBJECT', during), A)).
verb_phrase_known(CycWord,Subj,Event,compoundSemTrans(Out,Extras)) -->
    [S],{'#$compoundSemTrans'(CycWord, [S|String], '#$Verb', NextFrame, Template,_,_)},
   String,frame_template(NextFrame,Obj,Result,Extras),
   {apply_frame(Template,Subj,Event,Obj,Result,Out)}.

% =======================================================
%'#$nonCompositionalVerbSemTrans'('#$Separate-TheWord', '#$Mixture', '#$and'('#$isa'(':ACTION', '#$SeparatingAMixture'), '#$doneBy'(':ACTION', ':SUBJECT'), '#$objectOfStateChange'(':ACTION', ':OBJECT')), '#$EnglishMt', v(v('#$Mixture', '#$Separate-TheWord', '#$SeparatingAMixture', '#$and', '#$doneBy', '#$isa', '#$objectOfStateChange', ':ACTION', ':OBJECT', ':SUBJECT'), A)).
verb_phrase_known(CycWord,Subj,Event,nonCompositionalVerbSemTrans(Out)) -->
	{'#$nonCompositionalVerbSemTrans'(CycWord,ObjectIsa, Template,_,_)},
	subject_isa(ObjectIsa,Object,Template,TemplateO),
     {apply_frame(TemplateO,Subj,Event,Object,Result,Out)}.


% =======================================================
%'#$verbPrep-TransitiveTemplate'('#$Constructing', '#$Out-Of-MWW', '#$and'('#$isa'(':ACTION', ':DENOT'), '#$inputs'(':ACTION', ':OBLIQUE-OBJECT'), '#$products'(':ACTION', ':OBJECT'), '#$doneBy'(':ACTION', ':SUBJECT')), '#$EnglishMt', v(v('#$Constructing', '#$Out-Of-MWW', '#$and', '#$doneBy', '#$inputs', '#$isa', '#$products', ':ACTION', ':DENOT', ':OBJECT', ':OBLIQUE-OBJECT', ':SUBJECT'), A)).
%'#$verbPrep-TransitiveTemplate'('#$DistributionEvent', '#$To-TheWord', '#$and'('#$isa'(':ACTION', ':DENOT'), '#$toLocation'(':ACTION', ':OBLIQUE-OBJECT'), '#$objectMoving'(':ACTION', ':OBJECT'), '#$doneBy'(':ACTION', ':SUBJECT')), '#$EnglishMt', v(v('#$DistributionEvent', '#$To-TheWord', '#$and', '#$doneBy', '#$isa', '#$objectMoving', '#$toLocation', ':ACTION', ':DENOT', ':OBJECT', ':OBLIQUE-OBJECT', ':SUBJECT'), A)).
%'#$verbPrep-TransitiveTemplate'('#$Evaluating', '#$For-TheWord', '#$and'('#$isa'(':ACTION', ':DENOT'), '#$performedBy'(':ACTION', ':SUBJECT'), '#$evaluee-Direct'(':ACTION', ':OBJECT'), '#$purposeInEvent'(':SUBJECT', ':ACTION', '#$knowsAbout'(':SUBJECT', ':OBLIQUE-OBJECT'))), '#$EnglishMt', v(v('#$Evaluating', '#$For-TheWord', '#$and', '#$evaluee-Direct', '#$isa', '#$knowsAbout', '#$performedBy', '#$purposeInEvent', ':ACTION', ':DENOT', ':OBJECT', ':OBLIQUE-OBJECT', ':SUBJECT'), A)).
%'#$verbPrep-TransitiveTemplate'('#$FusionEvent', '#$With-TheWord', '#$and'('#$isa'(':ACTION', ':DENOT'), '#$inputs'(':ACTION', ':OBJECT'), '#$inputs'(':ACTION', ':OBLIQUE-OBJECT'), '#$doneBy'(':ACTION', ':SUBJECT')), '#$EnglishMt', v(v('#$FusionEvent', '#$With-TheWord', '#$and', '#$doneBy', '#$inputs', '#$isa', ':ACTION', ':DENOT', ':OBJECT', ':OBLIQUE-OBJECT', ':SUBJECT'), A)).
%'#$verbPrep-TransitiveTemplate'('#$HoldingAnObject', '#$By-TheWord', '#$and'('#$isa'(':ACTION', ':DENOT'), '#$objectActedOn'(':ACTION', ':OBLIQUE-OBJECT'), '#$physicalParts'(':OBJECT', ':OBLIQUE-OBJECT'), '#$doneBy'(':ACTION', ':SUBJECT')), '#$EnglishMt', v(v('#$By-TheWord', '#$HoldingAnObject', '#$and', '#$doneBy', '#$isa', '#$objectActedOn', '#$physicalParts', ':ACTION', ':DENOT', ':OBJECT', ':OBLIQUE-OBJECT', ':SUBJECT'), A)).
%'#$verbPrep-TransitiveTemplate'('#$InformationRemoving', '#$From-TheWord', '#$and'('#$isa'(':ACTION', ':DENOT'), '#$informationOrigin'(':ACTION', ':OBLIQUE-OBJECT'), '#$infoRemoved'(':ACTION', ':OBJECT'), '#$doneBy'(':ACTION', ':SUBJECT')), '#$EnglishMt', v(v('#$From-TheWord', '#$InformationRemoving', '#$and', '#$doneBy', '#$infoRemoved', '#$informationOrigin', '#$isa', ':ACTION', ':DENOT', ':OBJECT', ':OBLIQUE-OBJECT', ':SUBJECT'), A)).
verb_phrase_known(CycWord,Subj,Event,'verbPrep-TransitiveTemplate'(Out,EventMidO)) -->
	 {'#$verbPrep-TransitiveTemplate'(EventIsa, CycWordPrep, Template,_,_)},
      verb_phrase_event_isa(CycWord,EventIsa,Subj,Object,Event,EventMid),
      isCycWord(CycWordPrep),subject(Result,EventMid,EventMidO),
   {apply_frame(Template,Subj,Event,Object,Result,OutD),subst(OutD,':DENOT',EventIsa,Out)}.
  
% =======================================================
%'#$verbSemTransTemplate'('#$InformationRemoving', '#$PPCompFrameFn'('#$DitransitivePPCompFrame', '#$From-TheWord'), '#$and'('#$isa'(':ACTION', ':DENOT'), '#$informationOrigin'(':ACTION', ':OBLIQUE-OBJECT'), '#$infoRemoved'(':ACTION', ':OBJECT'), '#$doneBy'(':ACTION', ':SUBJECT')), '#$GeneralEnglishMt', v(v('#$DitransitivePPCompFrame', '#$From-TheWord', '#$InformationRemoving', '#$PPCompFrameFn', '#$and', '#$doneBy', '#$infoRemoved', '#$informationOrigin', '#$isa', ':ACTION', ':DENOT', ':OBJECT', ':OBLIQUE-OBJECT', ':SUBJECT'), A)).
%'#$verbSemTransTemplate'('#$InformationRemoving', '#$TransitiveNPCompFrame', '#$and'('#$isa'(':ACTION', ':DENOT'), '#$infoRemoved'(':ACTION', ':OBJECT'), '#$doneBy'(':ACTION', ':SUBJECT')), '#$GeneralEnglishMt', v(v('#$InformationRemoving', '#$TransitiveNPCompFrame', '#$and', '#$doneBy', '#$infoRemoved', '#$isa', ':ACTION', ':DENOT', ':OBJECT', ':SUBJECT'), A)).
%'#$verbSemTransTemplate'('#$Killing-Biological', '#$TransitiveNPCompFrame', '#$and'('#$isa'(':ACTION', ':DENOT'), '#$inputsDestroyed'(':ACTION', ':OBJECT'), '#$doneBy'(':ACTION', ':SUBJECT')), '#$GeneralEnglishMt', v(v('#$Killing-Biological', '#$TransitiveNPCompFrame', '#$and', '#$doneBy', '#$inputsDestroyed', '#$isa', ':ACTION', ':DENOT', ':OBJECT', ':SUBJECT'), A)).
%'#$verbSemTransTemplate'('#$LeavingAPlace', '#$PPCompFrameFn'('#$TransitivePPCompFrame', '#$From-TheWord'), '#$and'('#$isa'(':ACTION', ':DENOT'), '#$fromLocation'(':ACTION', ':OBLIQUE-OBJECT'), '#$doneBy'(':ACTION', ':SUBJECT')), '#$GeneralEnglishMt', v(v('#$From-TheWord', '#$LeavingAPlace', '#$PPCompFrameFn', '#$TransitivePPCompFrame', '#$and', '#$doneBy', '#$fromLocation', '#$isa', ':ACTION', ':DENOT', ':OBLIQUE-OBJECT', ':SUBJECT'), A)).
%'#$verbSemTransTemplate'('#$Bartering', '#$PPCompFrameFn'('#$DitransitivePPCompFrame', '#$For-TheWord'), '#$thereExists'(A, '#$thereExists'(B, '#$and'('#$isa'(':ACTION', ':DENOT'), '#$exchangers'(':ACTION', ':SUBJECT'), '#$subEvents'(':ACTION', A), '#$subEvents'(':ACTION', B), '#$toPossessor'(B, ':SUBJECT'), '#$objectOfPossessionTransfer'(A, ':OBLIQUE-OBJECT'), '#$objectOfPossessionTransfer'(B, ':OBJECT'), '#$fromPossessor'(A, ':SUBJECT'), '#$reciprocalTransfers'(A, B)))), '#$GeneralEnglishMt', v(v('#$Bartering', '#$DitransitivePPCompFrame', '#$For-TheWord', '#$PPCompFrameFn', '#$and', '#$exchangers', '#$fromPossessor', '#$isa', '#$objectOfPossessionTransfer', '#$reciprocalTransfers', '#$subEvents', '#$thereExists', '#$toPossessor', ':ACTION', ':DENOT', ':OBJECT', ':OBLIQUE-OBJECT', ':SUBJECT'), ['?T1'=A, '?T2'=B|C])).
%'#$verbSemTransTemplate'('#$CarryingWhileLocomoting', '#$PPCompFrameFn'('#$DitransitivePPCompFrame', '#$By-TheWord'), '#$and'('#$isa'(':ACTION', ':DENOT'), '#$transportees'(':ACTION', ':OBJECT'), '#$physicalParts'(':OBJECT', ':OBLIQUE-OBJECT'), '#$doneBy'(':ACTION', ':SUBJECT'), '#$objectsInContact'(':ACTION', ':OBLIQUE-OBJECT', ':SUBJECT')), '#$GeneralEnglishMt', v(v('#$By-TheWord', '#$CarryingWhileLocomoting', '#$DitransitivePPCompFrame', '#$PPCompFrameFn', '#$and', '#$doneBy', '#$isa', '#$objectsInContact', '#$physicalParts', '#$transportees', ':ACTION', ':DENOT', ':OBJECT', ':OBLIQUE-OBJECT', ':SUBJECT'), A)).
verb_phrase_known(CycWord,Subj,Event,verbSemTransTemplate(Out,EventMidO)) -->
      {'#$verbSemTransTemplate'(EventIsa,NextFrame, Template,_,_)},
	 verb_phrase_event_isa(CycWord,EventIsa,Subj,Object,Event,EventMid),
      frame_template(NextFrame,Obj,Result,Extras),
   {apply_frame(Template,Subj,Event,Object,Result,OutD),subst(OutD,':DENOT',EventIsa,Out)}.

verb_phrase_known(CycWord,Subj,Event,auxV(Out)) --> isPOS('#$AuxVerb',_),
      verb_phrase_known(CycWord,Subj,Event,Out).


% =======================================================
% verb_phrase_event_isa
% =======================================================
verb_phrase_event_isa(CycWord,EventIsa,Subj,Object,Event,Out) -->
      {cycWordForISA(CycWord,EventIsa)},verb_phrase_known(CycWord,Subj,Event,Out).

% =======================================================
%'#$semTransPredForPOS'('#$Verb', '#$verbSemTrans', '#$EnglishMt', v(v('#$Verb', '#$verbSemTrans'), A)).
% =================================================================
apply_frame(Formula,Subj,Event,Obj,Target,CycL):-
      varnameIdea('ACTION',Event),
      varnameIdea('SUBJECT',Subj),
      varnameIdea('OBJECT',Obj),
      varnameIdea('OBLIQUE',Target),
      subst(Formula,':SUBJECT',Subj,Formula1),
      subst(Formula1,':NOUN',Subj,Formula2),
      subst(Formula2,':ACTION',Event,Formula3),
      subst(Formula3,':OBJECT',Obj,Formula4),
      subst(Formula4,':EVENT',Event,Formula5),
      subst(Formula5,':OBLIQUE-OBJECT',Target,Formula6),
      subst(Formula6,':ARG1',Subj,Formula7),
      subst(Formula7,':VERB',Event,Formula8),
      subst(Formula8,':ARG2',Obj,Formula9),
      subst(Formula9,':EVENT',Event,Formula10),
      subst(Formula10,':ARG3',Target,CycL).

contains_obliqe(Formula):-flatten(Formula,Flat),member(':OBLIQUE-OBJECT',Flat).


% =======================================================
% frame_template
% =======================================================
frame_template('#$TransitiveNPCompFrame',Obj,Result,Extras) --> noun_phrase(Obj,true,Extras). 
%$#$DitransitivePPCompFrame','#$TransitivePPCompFrame'
frame_template('#$PPCompFrameFn'(_,CycPrep),Obj,Result,Extras) --> isCycWord(CycPrep),{!},best_subject(Result,true,Extras).
frame_template('#$PPCompFrameFn'(_,CycPrep),Obj,Result,Extras) --> noun_phrase(Obj,true,Mid),isCycWord(CycPrep),{!},best_subject(Result,Mid,Extras).
frame_template('#$RegularAdjFrame',Subj,Result,Extras) -->noun_phrase(Subj,true,Extras).


% ==========================================================
% String to String
% ==========================================================



removeRepeats(WordsNoIT,WordsNoITO):-
      removeRepeats1(WordsNoIT,M),
      removeRepeats2(M,WordsNoITO),!.
     
removeRepeats1([],[]):-!.
removeRepeats1([H|T],[HH|TT]):- stringToString(H,HH),!,removeRepeats1(T,TT).
removeRepeats1([H,H1|Rest],Out):-toLowercase(H,H1),!,removeRepeats1([H|Rest],Out).
removeRepeats1([H|Rest],[H|Out]):-removeRepeats1(Rest,Out).
removeRepeats1(X,X).

removeRepeats2(X,X):-!.

removeRepeats2(X,O):-append(L,R,X),
	    append([S,O|Me],LL,L),
	    append([S,O|Me],RR,R),!,
	    flatten([[S,O|Me],LL,RR],O).

stringToString(Before,After):-'#$abbreviationForString'(After, Before, _,_).

simplifyLF(Result,Result):-!.

cycQueryIsa(X,Y):-fail,writeq(cycQueryIsa(X,Y)),nl.

end_of_file.

% ==========================================================
% ==========================================================
% ==========================================================
% ==========================================================
% ==========================================================
% ==========================================================
% ==========================================================
% ==========================================================
% ==========================================================

% String Based
'#$genTemplate'('#$abnormal', '#$ConcatenatePhrasesFn'('#$TermParaphraseFn'(':ARG1'), '#$BestNLPhraseOfStringFn'([-, with, the, exception, of]), '#$TermParaphraseFn'(':ARG2')), '#$EnglishParaphraseMt', v(v('#$BestNLPhraseOfStringFn', '#$ConcatenatePhrasesFn', '#$TermParaphraseFn', '#$abnormal', -, ':ARG1', ':ARG2', exception, of, the, with), A)).
'#$genTemplate-Constrained'('#$arg1Isa', '#$isa'(':ARG1', '#$BinaryPredicate'), '#$TermParaphraseFn'('#$implies'('#$thereExists'(A, [':ARG1', B, A]), '#$ConcatenatePhrasesFn'('#$TermParaphraseFn-NP'(B), '#$BestNLPhraseOfStringFn'([must, be]), '#$BestDetNbarFn-Indefinite'('#$TermParaphraseFn-Constrained'('#$nonPlural-Generic', ':ARG2'))))), '#$EnglishParaphraseMt', v(v('#$BestDetNbarFn-Indefinite', '#$BestNLPhraseOfStringFn', '#$BinaryPredicate', '#$ConcatenatePhrasesFn', '#$TermParaphraseFn', '#$TermParaphraseFn-Constrained', '#$TermParaphraseFn-NP', '#$arg1Isa', '#$implies', '#$isa', '#$nonPlural-Generic', '#$thereExists', ':ARG1', ':ARG2', be, must), ['?Y'=A, '?X'=B|C])).
'#$genQuestion'('#$genls', 1, [what, kinds, of, '~a', are, there], [[2, ':NON-SINGULAR-GENERIC']], '#$EnglishParaphraseMt', v(v('#$genls', ':NON-SINGULAR-GENERIC', are, kinds, of, there, what, '~a'), A)).
'#$genQuestion'('#$hasAttributes', 2, ['What', attributes, does, '~a', 'have?'], [1], '#$EnglishParaphraseMt', v(v('#$hasAttributes', 'What', attributes, does, 'have?', '~a'), A)).
%'#$assertTemplate-Reln'(TemplateType,Predicate, Match, Template, _,_).
	    %#$InfinitivalVPTemplate'
	    % '#$aspectPerfect'
	    %[have, not, '#$PerfectiveVPTemplate'(':VP-CONT')]
	    %'#$NLNegFn'('#$NotVP-NLAttr', '#$aspectPerfect'(':VP-CONT'))

/*
meetsParam(transverb,String,CycWord):-genFormatAccess(String,CycWord).

genFormat(Verb,X,'NIL',_,_):-'#$genFormat'(Verb,X,_,_,_).
*/

'#$genFormat'('#$abbreviationString-PN', abbreviation, 'NIL', '#$EnglishParaphraseMt', v(v('#$abbreviationString-PN', 'NIL', abbreviation), A)).
'#$genFormat'('#$abbreviationString-PN', ['~a', is, the, abbreviated, form, of, the, name, for, '~a'], [[2, ':QUOTE'], 1], '#$EnglishParaphraseMt', v(v('#$abbreviationString-PN', ':QUOTE', abbreviated, for, form, is, name, of, the, '~a'), A)).
'#$genFormat'('#$adjSemTrans', ['~a', is, the, semantic, translation, of, word, sense, number, '~a', of, '~a', with, the, subcategorization, frame, '~a'], [[4, ':QUOTE'], 2, [1, ':QUOTE'], [3, ':QUOTE']], '#$EnglishParaphraseMt', v(v('#$adjSemTrans', ':QUOTE', frame, is, number, of, semantic, sense, subcategorization, the, translation, with, word, '~a'), A)).
'#$genFormat'('#$adjSemTrans-Restricted', [the, semantic, translation, of, word, sense, number, '~a', of, '~a', (','), when, modifying, '~a', with, the, subcategorization, frame, '~a', (','), is, '~a'], [2, [1, ':QUOTE'], [4, ':A'], [3, ':QUOTE'], 5], '#$EnglishParaphraseMt', v(v('#$adjSemTrans-Restricted', (','), ':A', ':QUOTE', frame, is, modifying, number, of, semantic, sense, subcategorization, the, translation, when, with, word, '~a'), A)).
'#$genFormat'('#$Area1023', ['Troi', '\'', s, 'Quarters'], 'NIL', '#$EnglishParaphraseMt', v(v('#$Area1023', '\'', 'NIL', 'Quarters', 'Troi', s), A)).
'#$genFormat-ArgFixed'('#$SubcollectionOfWithRelationFromTypeFn', 2, '#$surfaceParts', ['~A', on, '~A'], [[1, ':MASS-NUMBER', ':PLURAL', ':GERUND'], [3, ':PLURAL', ':MASS-NUMBER']], '#$EnglishParaphraseMt', v(v('#$SubcollectionOfWithRelationFromTypeFn', '#$surfaceParts', ':GERUND', ':MASS-NUMBER', ':PLURAL', on, '~A'), A)).
'#$genFormat-ArgFixed'('#$SubcollectionOfWithRelationToFn', 2, '#$containsInformationAbout', ['~A', about, '~A'], [[1, ':MASS-NUMBER', ':PLURAL', ':GERUND'], 3], '#$EnglishParaphraseMt', v(v('#$SubcollectionOfWithRelationToFn', '#$containsInformationAbout', ':GERUND', ':MASS-NUMBER', ':PLURAL', about, '~A'), A)).
'#$genFormat-ArgFixed'('#$SubcollectionOfWithRelationToFn', 2, '#$eventOccursAt', ['~A', in, '~A'], [[1, ':MASS-NUMBER', ':PLURAL', ':GERUND'], 3], '#$EnglishParaphraseMt', v(v('#$SubcollectionOfWithRelationToFn', '#$eventOccursAt', ':GERUND', ':MASS-NUMBER', ':PLURAL', in, '~A'), A)).
'#$genFormat-Precise'('#$synonymousExternalConcept', [the, 'Cyc', concept, '~s', is, synonymous, with, the, concept, named, by, '~s', in, the, external, data, source, '~a'], [1, 3, 2], '#$EnglishParaphraseMt', v(v('#$synonymousExternalConcept', 'Cyc', by, concept, data, external, in, is, named, source, synonymous, the, with, '~a', '~s'), A)).
'#$genFormat-Precise'('#$tastes', [the, agent, '~a', can, taste, '~a'], 'NIL', '#$EnglishParaphraseMt', v(v('#$tastes', 'NIL', agent, can, taste, the, '~a'), A)).
'#$genFormat-Precise'('#$temporalBoundsIntersect', [the, temporal, interval, of, '~a', intersects, the, temporal, interval, of, '~a'], 'NIL', '#$EnglishParaphraseMt', v(v('#$temporalBoundsIntersect', 'NIL', intersects, interval, of, temporal, the, '~a'), A)).



'#$formalityOfWS'('#$Aussie-TheWord', '#$ProperCountNoun', 0, '#$InformalSpeech', '#$GeneralEnglishMt', v(v('#$Aussie-TheWord', '#$InformalSpeech', '#$ProperCountNoun'), A)).
'#$formalityOfWS'('#$Babe-TheWord', '#$SimpleNoun', 1, '#$InformalSpeech', '#$GeneralEnglishMt', v(v('#$Babe-TheWord', '#$InformalSpeech', '#$SimpleNoun'), A)).
'#$politenessOfWS'('#$Cock-TheWord', '#$SimpleNoun', 1, '#$VulgarSpeech', '#$GeneralEnglishMt', v(v('#$Cock-TheWord', '#$SimpleNoun', '#$VulgarSpeech'), A)).

% ==========================================================
% WordsList Heuristics
% ==========================================================
'#$determinerAgreement'('#$A-Dozen-MWW', '#$plural-Generic', '#$EnglishMt', v(v('#$A-Dozen-MWW', '#$plural-Generic'), A)).

'#$denotesArgInReln'('#$Acquaint-TheWord', '#$CountNoun', '#$acquaintedWith', 2, '#$GeneralEnglishMt', v(v('#$Acquaint-TheWord', '#$CountNoun', '#$acquaintedWith'), A)).
'#$generateArgWithOutsideScope'('#$several-GenQuantRelnToType', 2, '#$ParaphraseMt', v(v('#$several-GenQuantRelnToType'), A)).
'#$generateQuantOverArg'('#$few-GenQuantRelnFrom', '#$Few-NLAttr', 3, '#$ParaphraseMt', v(v('#$Few-NLAttr', '#$few-GenQuantRelnFrom'), A)).
'#$genNatTerm-ArgLast'('#$PureFn', [pure], '#$Noun', '#$EnglishMt', v(v('#$Noun', '#$PureFn', pure), A)).
'#$genNatTerm-compoundString'('#$AttemptingFn', '#$Try-TheWord', [to], '#$Verb', '#$infinitive', '#$EnglishMt', v(v('#$AttemptingFn', '#$Try-TheWord', '#$Verb', '#$infinitive', to), A)).
'#$genNatTerm-multiWordString'('#$TreatmentFn', 'NIL', '#$Treatment-TheWord', '#$MassNoun', '#$nonPlural-Generic', '#$EnglishMt', v(v('#$MassNoun', '#$Treatment-TheWord', '#$TreatmentFn', '#$nonPlural-Generic', 'NIL'), A)).
%'#$headsPhraseOfType'('#$Pronoun', '#$Noun', '#$GeneralLexiconMt', v(v('#$Noun', '#$Pronoun'), A)).
'#$ncRuleConstraint'('#$AttackingDogs-NCR', '#$NCGenlsConstraintFn'('#$TheNCModifier', '#$Event'), '#$GeneralLexiconMt', v(v('#$AttackingDogs-NCR', '#$Event', '#$NCGenlsConstraintFn', '#$TheNCModifier'), A)).
'#$ncRuleLabel'('#$WaterSolution-NCR', [water, solution], '#$GeneralLexiconMt', v(v('#$WaterSolution-NCR', solution, water), A)).
'#$ncRuleTemplate'('#$AnimalPopulations-NCR', '#$SubcollectionOfWithRelationToTypeFn'('#$TheNCHead', '#$groupMembers', '#$TheNCModifier'), '#$GeneralLexiconMt', v(v('#$AnimalPopulations-NCR', '#$SubcollectionOfWithRelationToTypeFn', '#$TheNCHead', '#$TheNCModifier', '#$groupMembers'), A)).
'#$posForTemplateCategory'('#$Verb', '#$ProgressiveVPTemplate', '#$EnglishTemplateMt', v(v('#$ProgressiveVPTemplate', '#$Verb'), A)).
'#$posOfPhraseType'('#$NounPhrase', '#$Noun', '#$GeneralLexiconMt', v(v('#$Noun', '#$NounPhrase'), A)).
'#$posOfPhraseType'('#$PhraseFn'(A), A, '#$GeneralLexiconMt', v(v('#$PhraseFn', '$VAR'), ['VAR1'=A|B])).
'#$posPredForTemplateCategory'('#$presentParticiple', '#$ProgressiveVPTemplate', '#$EnglishTemplateMt', v(v('#$ProgressiveVPTemplate', '#$presentParticiple'), A)).
%'#$prepCollocation'('#$Beset-TheWord', '#$Adjective', '#$By-TheWord').      
'#$prepCollocation'('#$Wrangle-TheWord', '#$Verb', '#$With-TheWord', '#$EnglishMt', v(v('#$Verb', '#$With-TheWord', '#$Wrangle-TheWord'), A)).
'#$relationIndicators'('#$ailmentConditionAffects', '#$Infect-TheWord', '#$SimpleNoun', '#$EnglishMt', v(v('#$Infect-TheWord', '#$SimpleNoun', '#$ailmentConditionAffects'), A)).
'#$requiredActorSlots'('#$MonetaryExchangeOfUserRights', '#$buyer', '#$HumanActivitiesMt', v(v('#$MonetaryExchangeOfUserRights', '#$buyer'), A)).
%'#$semTransArg'('#$adjSemTrans', 4, '#$GeneralLexiconMt', v(v('#$adjSemTrans'), A)).
%'#$semTransArg'('#$adjSemTrans-Restricted', 5, '#$GeneralLexiconMt', v(v('#$adjSemTrans-Restricted'), A)).
'#$subcatFrame'('#$Argue-TheWord', '#$Verb', 0, '#$TransitiveNPCompFrame', '#$GeneralEnglishMt', v(v('#$Argue-TheWord', '#$TransitiveNPCompFrame', '#$Verb'), A)).
'#$subcatFrameArity'('#$Post-NounPhraseModifyingFrame', 1, '#$GeneralLexiconMt', v(v('#$Post-NounPhraseModifyingFrame'), A)).
'#$subcatFrameDependentConstraint'('#$TransitiveNPCompFrame', 1, '#$PhraseFn'('#$Noun'), '#$GeneralLexiconMt', v(v('#$Noun', '#$PhraseFn', '#$TransitiveNPCompFrame'), A)).
'#$subcatFrameDependentKeyword'('#$Post-NounPhraseModifyingFrame', 1, ':OBJECT', '#$GeneralLexiconMt', v(v('#$Post-NounPhraseModifyingFrame', ':OBJECT'), A)).
'#$subcatFrameKeywords'('#$MiddleVoiceFrame', ':ACTION', '#$InferencePSC', v(v('#$MiddleVoiceFrame', ':ACTION'), A)).

'#$psRuleArity'('#$PSRule-AdjPFromAdj', 1, '#$EnglishLexiconMt', v(v('#$PSRule-AdjPFromAdj'), A)).
'#$psRuleArity'('#$PSRule-AdvP-AdvPAdvP', 2, '#$EnglishLexiconMt', v(v('#$PSRule-AdvP-AdvPAdvP'), A)).
'#$psRuleCategory'('#$PSRule-V-VAdvP', '#$Verb', '#$EnglishLexiconMt', v(v('#$PSRule-V-VAdvP', '#$Verb'), A)).
'#$psRuleConstraint'('#$PSRule-AdjPFromAdj', '#$ConstituentTypeConstraintFn'(1, '#$Adjective'), '#$EnglishLexiconMt', v(v('#$Adjective', '#$ConstituentTypeConstraintFn', '#$PSRule-AdjPFromAdj'), A)).
'#$psRuleExample'('#$PSRule-VbarVComps', [likes, emus], '#$EnglishLexiconMt', v(v('#$PSRule-VbarVComps', emus, likes), A)).
'#$psRuleSemanticsFromDtr'('#$PSRule-DbarFromDet', 1, '#$EnglishLexiconMt', v(v('#$PSRule-DbarFromDet'), A)).
'#$psRuleSemanticsHandler'('#$PSRule-NP-DetNbar', 'PSP-SEMX-FOR-DET-NBAR', '#$EnglishLexiconMt', v(v('#$PSRule-NP-DetNbar', 'PSP-SEMX-FOR-DET-NBAR'), A)).
'#$psRuleSyntacticHeadDtr'('#$PSRule-AdjPFromAdj', 1, '#$EnglishLexiconMt', v(v('#$PSRule-AdjPFromAdj'), A)).
'#$psRuleTemplateBindings'('#$PSRule-V-VAdvP', '#$PSBindingFn'(1, ':ACTION'), '#$EnglishLexiconMt', v(v('#$PSBindingFn', '#$PSRule-V-VAdvP', ':ACTION'), A)).
'#$psRuleTemplateDtr'('#$PSRule-AdjPFromAdj', 1, '#$EnglishLexiconMt', v(v('#$PSRule-AdjPFromAdj'), A)).

