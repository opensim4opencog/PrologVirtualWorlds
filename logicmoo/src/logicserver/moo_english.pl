
%:-module(moo_english,[]).

% :-include('moo_header.pl').


ace_to_surface(CHARS,Formula):-
         sentenceToList(CHARS,S),
         sentence(G,S,[]),!,tologic(G,F), 
         fixExistentials(F,SKIF),!,
         getMooTermFromSurface(SKIF,Formula).


:-dynamic(known_property_value(Property,Value)). % like known_property_value(size,big)
:-dynamic(known_proper_noun(ProperNoun,LogicalThing)). %like known_proper_noun('Superman','TheManOfSteel')
:-dynamic(known_noun(Word,PlurSing)). %like known_noun(apple,snglr)
:-dynamic(known_verb_to_predicate(Verb,Tense,SingPlur,LogicalPredicate)). %like known_verb_to_predicate(kissed,past,snglr,kiss)
:-dynamic(known_request_word(WhWhat,LogicalWhWhat)). %known_request_word(how,methodUsed)
:-dynamic(known_copula(IS_BE,PlurSing)).
:-dynamic(known_class(EnglishCol,Tense,LogicalClass)).
:-dynamic(known_determiner(Tense,LogicalQuant,Quantifier)).
:-dynamic(known_verb_frame(Verb,Tense,SingPlur,Frame_with_Subj_and_Obj,Subj,Obj)).


known_verb_to_predicate(LogicalPredicate,present,SingPlur,LogicalPredicate):-nonvar(LogicalPredicate),quick_search('Q'(LogicalPredicate,_,_)).
known_class(LogicalClass,sngular,LogicalClass):-nonvar(LogicalClass),quick_search('instance'(LogicalClass,'Class')).

quick_search(Goal):-!,fail.

quick_search(Goal):-
%         add_explaination_recording_args(Goal,ExplainationIn,ExplainationO,Goal2),
         add_complete_search_args(Goal,2,DepthOut,Goal3),
         add_explaination_recording_args(Goal3,ExplainationIn,ExplainationO,Goal4),
        !, Goal4,!.

% ==========================================
% BELIEF
% ==========================================

:-dynamic(complete_goal/1).

:-multifile(expireOptimizationsInContext/3).

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).


%end_of_file.




