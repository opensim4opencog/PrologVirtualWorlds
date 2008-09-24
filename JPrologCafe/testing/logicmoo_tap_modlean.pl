%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% modlean_dec_k.pl
% Sicstus Prolog
% Copyright (C) 1998: Bernhard Beckert (University of Karlsruhe) and 
%                     Rajeev Gore (Australian National University)
%                     Email: beckert@ira.uka.de, rajeev.gore@arp.anu.edu.au
%
% Purpose:            Tableau-based theorem prover for NNF formulae
%                     of the propositional modal logic K
%                     Version 2.0, Decision procedure
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
:-include('logicmoo_utils_header.pl'). %<?

% :- module(modlean_dec_k,[decprove_k/1]).

:- op(400,fy,box), 
   op(400,fy,dia).

% :- use_module(library(lists),[reverse/2,append/3,member/2]).
	
% -----------------------------------------------------------------
% decprove_k(+Fml)
%
% Succeeds if there is a closed tableau for Fml and fails otherwise.
%
% Fml is a formula of propositional modal logic, i.e., the 
% logical operators `-' (negation), `,' (conjunction), 
% `;' (disjunction), box (box operator), and dia (diamond operator)
% may be used, where `-' only occurs on the atomic level.

decprove_k(Fml) :-
	modlean_tap_prove(Fml,[1],[],[],[],[],[],[],[],[],[],[]).


% modlean_tap_prove(Fml,Label,Univ,Inst,Lits,AllLits,UnExp,Sleep,AllLab,NonGrd,Free,Forbid)
%
% Fml: If it is _not_ a Prolog list, it is the current formula. If it is
%      a Prolog list, it is a list of closures that could not be made ground
%      before, but should be checked again at this point. A closure is of the
%      form ((Label1:Lit1),(Label2,Neg2),Rev), where Neg2 is the negation of 
%      the  second literal Lit2 and Rev is the label Label1 reversed.
% Label: The label of the current formula.
% Univ: The list of universal variables in the curren formula.
% Inst: Instantiations of previous copies of the formula
% Lits: The set of literals that have not been used yet to be unified
%       with the current formula to close the current branch.
% AllLits: The set of all literals on the branch.
% UnExp: The set of formulae that have not been considered yet.
% Sleep: Formulae that have been put asleep.
% AllLab: A list of all the labels on the current branch that have
%         been introduced by a diamond rule application; a label [1,2,3]
%         is stored as [3,2,1|_] (with an open tail).
% NonGrd: These are closures of the branch that up to now could not
%         be shown to be justified.
% Free: A list of the variables introduced in labels (can be instantiated).
% Forbid: A list of triples; the first element is a variable, the second is a 
%         list of terms; the third is a flag used in the evaluation. 
%         These are constraints: the first element must not be instaniated 
%         with an element of the list.
%
% In both Lits and AllLits, literals are stored in the form
% (Label:Negation), where Negation is the complement of the literal.
%
% Inst is a list of lists. It contains for each of the variables in 
% Univ a list of previous instantiations the variable.
%
% In UnExp formulae are stored in the form (Univ:Inst:Label:Fml).
%
% In Sleep formulae are stored in the form (Univ:Inst:Label:Fml)-Vars, where
% Vars is a list that has to contain at least one ground atom, for the 
% formula to be waken up.


% The conjunctive rule

modlean_tap_prove((A,B),Label,Univ,Inst,_,AllLits,UnExp,Sleep,AllLab,
      NonGrd,Free,Forbid) :- !,
        copy_term((Label,Univ,Free),(LabelB,UnivB,Free)),
        modlean_tap_prove(A,Label,Univ,Inst,AllLits,AllLits,[(UnivB:Inst:LabelB:B)|UnExp],
              Sleep,AllLab,NonGrd,Free,Forbid).


% The disjunctive rule

modlean_tap_prove((A;B),Label,Univ,Inst,_,AllLits,UnExp,Sleep,AllLab,
      NonGrd,Free,Forbid) :- !,
	copy_term((Label,Univ,Free),(LabelA,UnivA,Free)),
        copy_term((Label,Univ,Free),(LabelB,UnivB,Free)),
        extend_inst(Univ,Inst,NewInst),
        make_forbid(Univ,Inst,AddForbid),
        append(AddForbid,Forbid,NewForbid),
        append(Sleep,[(UnivA:NewInst:LabelA:(A;B))-Univ],SleepA),
        append(Sleep,[(UnivB:NewInst:LabelB:(A;B))-Univ],SleepB),
        modlean_tap_prove(A,Label,[],[],AllLits,AllLits,UnExp,SleepA,AllLab,NonGrd,
              (Univ+Free),NewForbid),
        modlean_tap_prove(B,Label,[],[],AllLits,AllLits,UnExp,SleepB,AllLab,NonGrd,
              (Univ+Free),NewForbid).


% The box rule

modlean_tap_prove(box Fml,Label,Univ,Inst,_,AllLits,UnExp,Sleep,AllLab,
      NonGrd,Free,Forbid) :- !,
	modlean_tap_prove(Fml,[(X,_)|Label],[X|Univ],[[]|Inst],AllLits,AllLits,
              UnExp,Sleep,AllLab,NonGrd,Free,Forbid).


% The diamond rule

modlean_tap_prove(dia Fml,Label,Univ,Inst,_,AllLits,UnExp,Sleep,AllLab,
      NonGrd,Free,Forbid) :- !,
	reverse([(Fml,Fml)|Label],TmpNewLabel),
        append(TmpNewLabel,_,NewLabel),
        modlean_tap_prove(NonGrd,_,_,_,AllLits,AllLits,
              [(Univ:Inst:[(Fml,Fml)|Label]:Fml)|UnExp],
              Sleep,[NewLabel|AllLab],[],Free,Forbid).


% Check old closures

modlean_tap_prove([(L1,L2,RevLabel)|RestNonG],_,_,_,_,AllLits,UnExp,Sleep,
      [NewLabel|AllLab],NonGrd,Free,Forbid) :- !,
	( L1 = L2,
          RevLabel = NewLabel,
          justified(RevLabel,AllLab),
	  check_forbid(Forbid),
          !
	; modlean_tap_prove(RestNonG,_,_,_,AllLits,AllLits,UnExp,Sleep,[NewLabel|AllLab],
                NonGrd,Free,Forbid)
        ).


% Switch back to normal mode from mode for checking old closures
	
modlean_tap_prove([],_,_,_,_,AllLits,[(Univ:Inst:Label:Fml)|UnExp],Sleep,AllLab,
      NonGrd,Free,Forbid) :- !,
        modlean_tap_prove(Fml,Label,Univ,Inst,AllLits,AllLits,UnExp,Sleep,AllLab,
              NonGrd,Free,Forbid).


% Do not use the current literal for closure, continue expanding
% (only if list Lits is empty)

modlean_tap_prove(Lit,LitLabel,_,_,[],AllLits,UnExp,Sleep,AllLab,NonGrd,Free,Forbid) :- !,
        ( (Lit = -Neg; -Lit = Neg) ->
          ( UnExp = [(Univ:Inst:Label:Fml)|UnExpR] ->
	    modlean_tap_prove(Fml,Label,Univ,Inst,
                  [(LitLabel:Neg)|AllLits],[(LitLabel:Neg)|AllLits],
                  UnExpR,Sleep,AllLab,NonGrd,Free,Forbid)
          ; wake_up(Sleep,(Univ:Inst:Label:Fml),SleepR),
	    modlean_tap_prove(Fml,Label,Univ,Inst,
                  [(LitLabel:Neg)|AllLits],[(LitLabel:Neg)|AllLits],
                  [],SleepR,AllLab,NonGrd,Free,Forbid)
          )
        ).


% Try to use the current literal for closure

modlean_tap_prove(Lit1,Label1,_,_,[(Label2:Lit2)|Lits],AllLits,UnExp,Sleep,
      AllLab,NonGrd,Free,Forbid) :-
	( \+(Label1:Lit1 = Label2:Lit2) ->
 	  modlean_tap_prove(Lit1,Label1,_,_,Lits,AllLits,UnExp,Sleep,AllLab,NonGrd,
                Free,Forbid)
        ; copy_term((Label1,Free),(NewLabel1,Free)),
          copy_term((Label2,Free),(NewLabel2,Free)),
          reverse(NewLabel1,RevLabel),
          ( NewLabel1 = NewLabel2,
            justified(RevLabel,AllLab),
            check_forbid(Forbid),
            !
          ; modlean_tap_prove(Lit1,Label1,_,_,Lits,AllLits,UnExp,Sleep,AllLab,
                  [(NewLabel1,NewLabel2,RevLabel)|NonGrd],Free,Forbid)
          )
        ).
    


% ADDITIONAL PREDICATES

% wake_up(+Sleep,-Fml,-RestSleep)
%
% Looks for the first formula in Sleep than can be woken up. 
% It is returned in Fml, all other formulae in Slepp are returned in 
% the list RestSleep.

/*
wake_up([(Fml-Vars)|Sleep],Fml,Sleep) :- 
	contains_ground(Vars),
	!.

wake_up([First|Sleep],Fml,[First|RestSleep]) :- 
	wake_up(Sleep,Fml,RestSleep).
*/
%contains_ground([H|_]) :- ground(H),!.
%contains_ground([_|T]) :- contains_ground(T).


% justified(+Label,+AllLab)
%
% Checks whether Label is justified by the list AllLab of labels.

justified(Label,_) :- ground(Label).

justified(Label,[Label|T]) :- justified(Label,T).
	
justified(Label,[_|T]) :- justified(Label,T).


% extend_inst(+Vars,+Inst,-NewInst)
%
% Vars and Inst are lists of the same length. Each element of 
% Vars are added at front to the corresponding element of Inst.

/*
extend_inst([FirstV|RestV],[FirstI|RestI],[[FirstV|FirstI]|Rest]) :-
    	extend_inst(RestV,RestI,Rest).

extend_inst([],[],[]).
*/

% make_forbid(+Vars,+Inst,-Forbid)
%
% Vars and Inst are lists of the same length. A list of forbid-triples
% of the same length is constructed
 /*
make_forbid([FirstU|RestU],[FirstI|RestI],[(FirstU,FirstI,_)|Rest]) :-
    	make_forbid(RestU,RestI,Rest).

make_forbid([],[],[]).
   */


% check_forbid(+Forbid)
%
% Checks a list of forbid-triples. Fails if in one of the triples 
% (Var,Inst,Label) the variable Var 
% is instantiated with an element of the list Inst. If Label is non-ground,
% then the triple is ok and does not have to be tested (again).


/*
check_forbid([(Var,Inst,Flag)|Rest]) :-
	(  (ground(Var), var(Flag)) ->
           \+(member(Var,Inst)),
           Flag = true
        ;  true
        ),
        check_forbid(Rest).

check_forbid([]).
*/
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% modlean_dec_kd.pl
% 
% Sicstus Prolog
% Copyright (C) 1998: Bernhard Beckert (University of Karlsruhe) and 
%                     Rajeev Gore (Australian National University)
%                     Email: beckert@ira.uka.de, rajeev.gore@arp.anu.edu.au
%
% Purpose:            Tableau-based theorem prover for NNF formulae
%                     of the propositional modal logic KD
%                     Version 2.0, Decision procedure
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% :- module(modlean_dec_kd,[decprove_kd/1]).

:- op(400,fy,box), 
   op(400,fy,dia).

% :- use_module(library(lists),[member/2,reverse/2,append/3]).

% -----------------------------------------------------------------
% decprove_kd(+Fml)
%
% Succeeds if there is a closed tableau for Fml and fails otherwise.
%
% Fml is a formula of propositional modal logic, i.e., the 
% logical operators `-' (negation), `,' (conjunction), 
% `;' (disjunction), box (box operator), and dia (diamond operator)
% may be used, where `-' only occurs on the atomic level.

decprove_kd(Fml) :-
	modlean_tap_prove(Fml,[1],[],[],[],[],[],[],[]).


% modlean_tap_prove(Fml,Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid)
%
% Fml: The current formula.
% Label: The label of the current formula
% Univ: The list of universal variables in the curren formula
% Inst: Instantiations of previous copies of the formula
% Lits: The set of literals that have not been used yet to be unified
%       with the current formula to close the current branch
% UnExp: The set of formulae that have not been considered yet
% Sleep: Formulae that have been put asleep
% Free: A list of the variables introduced in labels (can be instantiated)
% Forbid: A list of triples; the first element is a variable, the second is a 
%         list of terms; the third is a flag used in the evaluation. 
%         These are constraints: the first element must not be instaniated 
%         with an element of the list.
%
% In Lits literals are stored in the form
% (Label:Negation), where Negation is the complement of the literal
%
% Inst is a list of lists. It contains for each of the variables in 
% Univ a list of previous instantiations the variable.
%
% In UnExp formulae are stored in the form (Univ:Inst:Label:Fml)
%
% In Sleep formulae are stored in the form (Univ:Inst:Label:Fml)-Vars, where
% at least one of the variables in Vars must be instaniated (i.e., Vars is 
% a list that has to contain at least one ground atom for the 
% formula to be waken up.


% The conjunctive rule

modlean_tap_prove((A,B),Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid) :- !,
        copy_term((Label,Univ,Free),(LabelB,UnivB,Free)),
        modlean_tap_prove(A,Label,Univ,Inst,Lits,
              [(UnivB:Inst:LabelB:B)|UnExp],Sleep,Free,Forbid).


% The disjunctive rule

modlean_tap_prove((A;B),Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid) :- !,
        copy_term((Label,Univ,Free),(LabelA,UnivA,Free)),
        copy_term((Label,Univ,Free),(LabelB,UnivB,Free)),
        extend_inst(Univ,Inst,NewInst),
        make_forbid(Univ,Inst,AddForbid),
        append(AddForbid,Forbid,NewForbid),
        append(Sleep,[(UnivA:NewInst:LabelA:(A;B))-Univ],SleepA),
        append(Sleep,[(UnivB:NewInst:LabelB:(A;B))-Univ],SleepB),
        modlean_tap_prove(A,Label,[],[],Lits,UnExp,SleepA,(Univ+Free),NewForbid),
        modlean_tap_prove(B,Label,[],[],Lits,UnExp,SleepB,(Univ+Free),NewForbid).


% The box rule

modlean_tap_prove(box Fml,Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid) :- !,
	modlean_tap_prove(Fml,[X|Label],[X|Univ],[[]|Inst],Lits,UnExp,Sleep,Free,Forbid).

% The diamond rule

modlean_tap_prove(dia Fml,Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid) :- !,
        modlean_tap_prove(Fml,[Fml|Label],Univ,Inst,Lits,UnExp,Sleep,Free,Forbid).


% Try to use the current literal for closure

modlean_tap_prove(Lit,Label,_,_,[L|Lits],_,_,Free,Forbid) :-
        copy_term(((Label:Lit),Free),(New,Free)),
        (    copy_term((L,Free),(New,Free)),
             check_forbid(Forbid)
        ;    modlean_tap_prove(Lit,Label,_,_,Lits,[],[],Free,Forbid)
        ).


% Do not use the current literal for closure, continue expanding

modlean_tap_prove(Lit,LitLabel,_,_,Lits,UnExp,Sleep,Free,Forbid) :- !,
        ( (Lit = -Neg; -Lit = Neg) ->
          ( UnExp = [(Univ:Inst:Label:Fml)|UnExpR] ->
	    modlean_tap_prove(Fml,Label,Univ,Inst,
                  [(LitLabel:Neg)|Lits],UnExpR,Sleep,Free,Forbid)
          ; wake_up(Sleep,(Univ:Inst:Label:Fml),SleepR),
	    modlean_tap_prove(Fml,Label,Univ,Inst,
                  [(LitLabel:Neg)|Lits],[],SleepR,Free,Forbid)
          )
        ).


% ADDITIONAL PREDICATES

% wake_up(+Sleep,-Fml,-RestSleep)
%
% Looks for the first formula in Sleep than can be waken up. 
% It is returned in Fml, all other formulae in Sleep are returned in 
% the list RestSleep.

wake_up([(Fml-Vars)|Sleep],Fml,Sleep) :- 
	contains_ground(Vars),
	!.

wake_up([First|Sleep],Fml,[First|RestSleep]) :- 
	wake_up(Sleep,Fml,RestSleep).

%contains_ground([H|_]) :- ground(H),!.
%contains_ground([_|T]) :- contains_ground(T).



% extend_inst(+Vars,+Inst,-NewInst)
%
% Vars and Inst are lists of the same length. Each element of 
% Vars are added at front to the corresponding element of Inst.

extend_inst([FirstV|RestV],[FirstI|RestI],[[FirstV|FirstI]|Rest]) :-
    	extend_inst(RestV,RestI,Rest).

extend_inst([],[],[]).


% make_forbid(+Vars,+Inst,-Forbid)
%
% Vars and Inst are lists of the same length. A list of forbid-triples
% of the same length is constructed
/*
make_forbid([FirstU|RestU],[FirstI|RestI],[(FirstU,FirstI,_)|Rest]) :-
    	make_forbid(RestU,RestI,Rest).

make_forbid([],[],[]).

  */

% check_forbid(+Forbid)
%
% Checks a list of forbid-triples. Fails if in one of the triples 
% (Var,Inst,Label) the variable Var 
% is instantiated with an element of the list Inst. If Label is non-ground,
% then the triple is ok and does not have to be tested (again).


check_forbid([(Var,Inst,Flag)|Rest]) :-
	(  (ground(Var), var(Flag)) ->
           \+(member(Var,Inst)),
           Flag = true
        ;  true
        ),
        check_forbid(Rest).

check_forbid([]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% modlean_dec_kt.pl
% 
% Sicstus Prolog
% Copyright (C) 1998: Bernhard Beckert (University of Karlsruhe) and 
%                     Rajeev Gore (Australian National University)
%                     Email: beckert@ira.uka.de, rajeev.gore@arp.anu.edu.au
%
% Purpose:            Tableau-based theorem prover for NNF formulae
%                     of the propositional modal logic KT
%                     Version 2.0, Decision procedure
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% :- module(modlean_dec_kt,[decprove_kt/1]).

:- op(400,fy,box), 
   op(400,fy,dia).

% :- use_module(library(lists),[member/2,reverse/2,append/3]).

% -----------------------------------------------------------------
% decprove_kt(+Fml)
%
% Succeeds if there is a closed tableau for Fml and fails otherwise.
%
% Fml is a formula of propositional modal logic, i.e., the 
% logical operators `-' (negation), `,' (conjunction), 
% `;' (disjunction), box (box operator), and dia (diamond operator)
% may be used, where `-' only occurs on the atomic level.

decprove_kt(Fml) :-
	modlean_tap_prove(Fml,[1],[],[],[],[],[],[],[]).


% modlean_tap_prove(Fml,Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid)
%
% Fml: The current formula.
% Label: The label of the current formula
% Univ: The list of universal variables in the curren formula
% Inst: Instantiations of previous copies of the formula
% Lits: The set of literals that have not been used yet to be unified
%       with the current formula to close the current branch
% UnExp: The set of formulae that have not been considered yet
% Sleep: Formulae that have been put asleep
% Free: A list of the variables introduced in labels (can be instantiated)
% Forbid: A list of triples; the first element is a variable, the second is a 
%         list of terms; the third is a flag used in the evaluation. 
%         These are constraints: the first element must not be instaniated 
%         with an element of the list.
%
% In Lits literals are stored in the form
% (Label:Negation), where Negation is the complement of the literal
%
% Inst is a list of lists. It contains for each of the variables in 
% Univ a list of previous instantiations the variable.
%
% In UnExp formulae are stored in the form (Univ:Inst:Label:Fml)
%
% In Sleep formulae are stored in the form (Univ:Inst:Label:Fml)-Vars, where
% at least one of the variables in Vars must be instaniated (i.e., Vars is 
% a list that has to contain at least one ground atom for the 
% formula to be waken up.


% The conjunctive rule

modlean_tap_prove((A,B),Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid) :- !,
        copy_term((Label,Univ,Free),(LabelB,UnivB,Free)),
        modlean_tap_prove(A,Label,Univ,Inst,Lits,
              [(UnivB:Inst:LabelB:B)|UnExp],Sleep,Free,Forbid).


% The disjunctive rule

modlean_tap_prove((A;B),Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid) :- !,
        copy_term((Label,Univ,Free),(LabelA,UnivA,Free)),
        copy_term((Label,Univ,Free),(LabelB,UnivB,Free)),
        extend_inst(Univ,Inst,NewInst),
        make_forbid(Univ,Inst,AddForbid),
        append(AddForbid,Forbid,NewForbid),
        append(Sleep,[(UnivA:NewInst:LabelA:(A;B))-Univ],SleepA),
        append(Sleep,[(UnivB:NewInst:LabelB:(A;B))-Univ],SleepB),
        modlean_tap_prove(A,Label,[],[],Lits,UnExp,SleepA,(Univ+Free),NewForbid),
        modlean_tap_prove(B,Label,[],[],Lits,UnExp,SleepB,(Univ+Free),NewForbid).


% The box rule

modlean_tap_prove(box Fml,Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid) :- !,
	copy_term((Label,Univ,Free),(LabelT,UnivT,Free)),
        modlean_tap_prove(Fml,[X|Label],[X|Univ],[[]|Inst],Lits,
              [(UnivT:Inst:LabelT:Fml)|UnExp],Sleep,Free,Forbid).

% The diamond rule

modlean_tap_prove(dia Fml,Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid) :- !,
        modlean_tap_prove(Fml,[Fml|Label],Univ,Inst,Lits,UnExp,Sleep,Free,Forbid).


% Try to use the current literal for closure

modlean_tap_prove(Lit,Label,_,_,[L|Lits],_,_,Free,Forbid) :-
        copy_term(((Label:Lit),Free),(New,Free)),
        (    copy_term((L,Free),(New,Free)),
             check_forbid(Forbid)
        ;    modlean_tap_prove(Lit,Label,_,_,Lits,[],[],Free,Forbid)
        ).


% Do not use the current literal for closure, continue expanding

modlean_tap_prove(Lit,LitLabel,_,_,Lits,UnExp,Sleep,Free,Forbid) :- !,
        ( (Lit = -Neg; -Lit = Neg) ->
          ( UnExp = [(Univ:Inst:Label:Fml)|UnExpR] ->
	    modlean_tap_prove(Fml,Label,Univ,Inst,
                  [(LitLabel:Neg)|Lits],UnExpR,Sleep,Free,Forbid)
          ; wake_up(Sleep,(Univ:Inst:Label:Fml),SleepR),
	    modlean_tap_prove(Fml,Label,Univ,Inst,
                  [(LitLabel:Neg)|Lits],[],SleepR,Free,Forbid)
          )
        ).


% ADDITIONAL PREDICATES

% wake_up(+Sleep,-Fml,-RestSleep)
%
% Looks for the first formula in Sleep than can be waken up. 
% It is returned in Fml, all other formulae in Sleep are returned in 
% the list RestSleep.

/*
wake_up([(Fml-Vars)|Sleep],Fml,Sleep) :- 
	contains_ground(Vars),
	!.

wake_up([First|Sleep],Fml,[First|RestSleep]) :- 
	wake_up(Sleep,Fml,RestSleep).
*/
%contains_ground([H|_]) :- ground(H),!.
%contains_ground([_|T]) :- contains_ground(T).



% extend_inst(+Vars,+Inst,-NewInst)
%
% Vars and Inst are lists of the same length. Each element of 
% Vars are added at front to the corresponding element of Inst.

extend_inst([FirstV|RestV],[FirstI|RestI],[[FirstV|FirstI]|Rest]) :-
    	extend_inst(RestV,RestI,Rest).

extend_inst([],[],[]).


% make_forbid(+Vars,+Inst,-Forbid)
%
% Vars and Inst are lists of the same length. A list of forbid-triples
% of the same length is constructed

/*
make_forbid([FirstU|RestU],[FirstI|RestI],[(FirstU,FirstI,_)|Rest]) :-
    	make_forbid(RestU,RestI,Rest).

make_forbid([],[],[]).
*/


% check_forbid(+Forbid)
%
% Checks a list of forbid-triples. Fails if in one of the triples 
% (Var,Inst,Label) the variable Var 
% is instantiated with an element of the list Inst. If Label is non-ground,
% then the triple is ok and does not have to be tested (again).


check_forbid([(Var,Inst,Flag)|Rest]) :-
	(  (ground(Var), var(Flag)) ->
           \+(member(Var,Inst)),
           Flag = true
        ;  true
        ),
        check_forbid(Rest).

check_forbid([]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% modlean_dec_s4.pl
% 
% Sicstus Prolog
% Copyright (C) 1998: Bernhard Beckert (University of Karlsruhe) and 
%                     Rajeev Gore (Australian National University)
%                     Email: beckert@ira.uka.de, rajeev.gore@arp.anu.edu.au
%
% Purpose:            Tableau-based theorem prover for NNF formulae
%                     of the propositional modal logic S4
%                     Version 2.0, Decision procedure
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% :- module(modlean_dec_s4,[decprove_s4/1]).

:- op(400,fy,box), 
   op(400,fy,dia).

% :- use_module(library(lists),[member/2,reverse/2,append/3]).

% -----------------------------------------------------------------
% decprove_s4(+Fml)
%
% Succeeds if there is a closed tableau for Fml and fails otherwise.
%
% Fml is a formula of propositional modal logic, i.e., the 
% logical operators `-' (negation), `,' (conjunction), 
% `;' (disjunction), box (box operator), and dia (diamond operator)
% may be used, where `-' only occurs on the atomic level.

decprove_s4(Fml) :-
	modlean_tap_prove(Fml,[(1,1)],[],[],[],[],[],[],[],[],[],[],[],[]).


% modlean_tap_prove(Fml,Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid)
%
% Fml: The current formula.
% Label: The label of the current formula
% Univ: The list of universal variables in the curren formula
% Inst: Instantiations of previous copies of the formula
% Lits: The set of literals that have not been used yet to be unified
%       with the current formula to close the current branch
% UnExp: The set of formulae that have not been considered yet
% Sleep: Formulae that have been put asleep
% Free: A list of the variables introduced in labels (can be instantiated)
% Forbid: A list of triples; the first element is a variable, the second is a 
%         list of terms; the third is a flag used in the evaluation. 
%         These are constraints: the first element must not be instaniated 
%         with an element of the list.
%
% In Lits literals are stored in the form
% (Label:Negation), where Negation is the complement of the literal
%
% Inst is a list of lists. It contains for each of the variables in 
% Univ a list of previous instantiations the variable.
%
% In UnExp formulae are stored in the form (Univ:Inst:Label:Fml)
%
% In Sleep formulae are stored in the form (Univ:Inst:Label:Fml)-Vars, where
% at least one of the variables in Vars must be instaniated (i.e., Vars is 
% a list that has to contain at least one ground atom for the 
% formula to be waken up.


% The conjunctive rule

modlean_tap_prove((A,B),Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid,AllLabels,
      UsedD,BlockedD,NewB,UnjustB) :- !,
        copy_term((Label,Univ,Free),(LabelB,UnivB,Free)),
        modlean_tap_prove(A,Label,Univ,Inst,Lits,
              [(UnivB:Inst:LabelB:B)|UnExp],Sleep,Free,Forbid,AllLabels,
              UsedD,BlockedD,NewB,UnjustB).


% The disjunctive rule

modlean_tap_prove((A;B),Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid,AllLabels,
      UsedD,BlockedD,NewB,UnjustB) :- !,
        copy_term((Label,Univ,Free),(LabelA,UnivA,Free)),
        copy_term((Label,Univ,Free),(LabelB,UnivB,Free)),
        extend_inst(Univ,Inst,NewInst),
        make_forbid(Univ,Inst,AddForbid),
        append(AddForbid,Forbid,NewForbid),
        append(Sleep,[(UnivA:NewInst:LabelA:(A;B))-Univ],SleepA),
        append(Sleep,[(UnivB:NewInst:LabelB:(A;B))-Univ],SleepB),
        modlean_tap_prove(A,Label,[],[],Lits,UnExp,SleepA,(Univ+Free),NewForbid,
              AllLabels,UsedD,BlockedD,NewB,UnjustB),
        modlean_tap_prove(B,Label,[],[],Lits,UnExp,SleepB,(Univ+Free),NewForbid,
              AllLabels,UsedD,BlockedD,NewB,UnjustB).


% all the box formulas are assumed to be unjustified first

modlean_tap_prove(box Fml,Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid,AllLabels,
      UsedD,BlockedD,NewB,UnjustB) :- !,
   modlean_tap_prove(expand,_,_,_,Lits,UnExp,Sleep,Free,Forbid,AllLabels,
      UsedD,BlockedD,NewB,[(Univ:Inst:Label:(box Fml))|UnjustB]).


% all dia formulas are blocked at first.

modlean_tap_prove(dia Fml,Label,Univ,Inst,Lits,UnExp,Sleep,Free,Forbid,AllLabels,
      UsedD,BlockedD,NewB,UnjustB) :- !,
   modlean_tap_prove(expand,_,_,_,Lits,UnExp,Sleep,Free,Forbid,AllLabels,
      UsedD,[(Univ:Inst:Label:(dia Fml))|BlockedD],NewB,UnjustB).



% Use a new formula from UnExp

modlean_tap_prove(expand,_,_,_,Lits,[(Univ:Inst:Label:Fml)|UnExpR],Sleep,Free,Forbid,
      AllLabels,UsedD,BlockedD,NewB,UnjustB) :- !,
	modlean_tap_prove(Fml,Label,Univ,Inst,Lits,UnExpR,Sleep,Free,Forbid,
              AllLabels,UsedD,BlockedD,NewB,UnjustB).


% Look for a dia formula in BlockedD that is not blocked and apply dia rule

modlean_tap_prove(expand,_,_,_,Lits,[],Sleep,Free,Forbid,
      AllLabels,UsedD,BlockedD,NewB,UnjustB) :-
        dia_unblocked(UsedD,BlockedD,NewB,
                      (Univ:Inst:Label:dia Fml),RestBlockedD),
    	!,
	reverse([(Fml,Fml)|Label],TmpNewLabel),
        append(TmpNewLabel,_,NewLabel),
        modlean_tap_prove(Fml,[(Fml,Fml)|Label],Univ,Inst,Lits,[],Sleep,Free,Forbid,
              [NewLabel|AllLabels],              
              [(Fml:Label:Univ)|UsedD],RestBlockedD,NewB,UnjustB).


% Wake up (if possible) a formula from Sleep

modlean_tap_prove(expand,_,_,_,Lits,[],Sleep,Free,Forbid,AllLabels,
      UsedD,BlockedD,NewB,UnjustB) :-
	wake_up(Sleep,(Univ:Inst:Label:Fml),SleepR),
	!,
	modlean_tap_prove(Fml,Label,Univ,Inst,Lits,[],SleepR,Free,Forbid,AllLabels,
	      UsedD,BlockedD,NewB,UnjustB).


% Look for a box formula in UnjustB that is justified and apply the box rule

modlean_tap_prove(expand,_,_,_,Lits,[],Sleep,Free,Forbid,AllLabels,
      UsedD,BlockedD,NewB,UnjustB) :-
	justified_box(AllLabels,UnjustB,(Univ:Inst:Label:box Fml),RestUnjustB),
	!,
	copy_term((Label,Univ,Free),(LabelT,UnivT,Free)),
	copy_term((Label,Univ,Free),(Label4,Univ4,Free)),
        (  new_box(Fml,Label,Univ,NewB) ->
           NewNewB = [(Fml:Label:Univ)|NewB]
        ;  NewNewB = NewB
        ),
        modlean_tap_prove(Fml,[(X,_)|Label],[X|Univ],[[]|Inst],Lits,
              [(UnivT:Inst:LabelT:Fml)],Sleep,Free,Forbid,AllLabels,
              UsedD,BlockedD,NewNewB,
              [([Y|Univ4]:[[]|Inst]:[(Y,_)|Label4]:(box Fml))|RestUnjustB]).


% Try to use the current literal for closure

modlean_tap_prove(Lit,Label,_,_,[L|Lits],_,_,Free,Forbid,_,_,_,_,_) :-
	Lit \== expand,
        copy_term(((Label:Lit),Free),(New,Free)),
        (    copy_term((L,Free),(New,Free)),
             check_forbid(Forbid)
        ;    modlean_tap_prove(Lit,Label,_,_,Lits,do_not_expand,[],Free,Forbid,_,_,_,_,_)
        ).


% Do not use the current literal for closure

modlean_tap_prove(Lit,LitLabel,_,_,Lits,UnExp,Sleep,Free,Forbid,AllLabels,
      UsedD,BlockedD,NewB,UnjustB) :-
	Lit \== expand,
        UnExp \== do_not_expand,
	!,
    	( (Lit = -Neg; -Lit = Neg) ->
  	  modlean_tap_prove(expand,_,_,_,[(LitLabel:Neg)|Lits],UnExp,Sleep,Free,Forbid,
                AllLabels,UsedD,BlockedD,NewB,UnjustB)
        ).

%modlean_tap_prove(Fml,Label,Univ,Inst,Lits,[],SleepR,Free,Forbid,AllLabels,
%	      UsedD,BlockedD,NewB,UnjustB) :-
%write(modlean_tap_prove(Fml,Label,Univ,Inst,Lits,[],SleepR,Free,Forbid,AllLabels,
%	      UsedD,BlockedD,NewB,UnjustB)),fail.


modlean_tap_prove(Fml,Label,Univ,Inst,Lits,[],SleepR,Free,Forbid,AllLabels,
	      UsedD,BlockedD,NewB,UnjustB) :-
write(backtrack),fail.


% ADDITIONAL PREDICATES

% wake_up(+Sleep,-Fml,-RestSleep)
%
% Looks for the first formula in Sleep than can be waken up. 
% It is returned in Fml, all other formulae in Sleep are returned in 
% the list RestSleep.

wake_up([(Fml-Vars)|Sleep],Fml,Sleep) :- 
	contains_ground(Vars),
	!.

wake_up([First|Sleep],Fml,[First|RestSleep]) :- 
	wake_up(Sleep,Fml,RestSleep).

contains_ground([H|_]) :- \+(var(H)),!.
contains_ground([_|T]) :- contains_ground(T).



% extend_inst(+Vars,+Inst,-NewInst)
%
% Vars and Inst are lists of the same length. Each element of 
% Vars are added at front to the corresponding element of Inst.

/*
extend_inst([FirstV|RestV],[FirstI|RestI],[[FirstV|FirstI]|Rest]) :-
    	extend_inst(RestV,RestI,Rest).

extend_inst([],[],[]).
*/

% make_forbid(+Vars,+Inst,-Forbid)
%
% Vars and Inst are lists of the same length. A list of forbid-triples
% of the same length is constructed

make_forbid([FirstU|RestU],[FirstI|RestI],[(FirstU,FirstI,_)|Rest]) :-
    	make_forbid(RestU,RestI,Rest).

make_forbid([],[],[]).



% check_forbid(+Forbid)
%
% Checks a list of forbid-triples. Fails if in one of the triples 
% (Var,Inst,Label) the variable Var 
% is instantiated with an element of the list Inst. If Label is non-ground,
% then the triple is ok and does not have to be tested (again).


/*
check_forbid([(Var,Inst,Flag)|Rest]) :-
	(  (ground(Var), var(Flag)) ->
           \+(member(Var,Inst)),
           Flag = true
        ;  true
        ),
        check_forbid(Rest).

check_forbid([]).
*/


new_box(Fml,Label,Univ,NewB) :-
	reverse(Label,RevLabel),
   	\+( ( member((Fml:OldLabel:OldUniv),NewB),
	      reverse(OldLabel,RevOldLabel),
              ipr_ident(RevOldLabel,OldUniv,RevLabel,Univ)
          ) ).

ipr_ident([(TauF,_)|TauR],UnivTau,[(SigmaF,_)|SigmaR],UnivSigma) :-
 	(  TauF == SigmaF
        ;  occurs_in(TauF,UnivTau)
        ;  occurs_in(SigmaF,UnivSigma)
        ),
        !,
        ipr_ident(TauR,UnivTau,SigmaR,UnivSigma).

ipr_ident([],_,_,_).

ipr_unif([First|TauR],[First|SigmaR]) :-
        ipr_unif(TauR,SigmaR).

ipr_unif([],_).


occurs_in(X,[H|_]) :-
	X == H,
        !.

occurs_in(X,[_|T]) :-
	occurs_in(X,T).


justified_box(AllLabels,[(Univ:Inst:Label:Fml)|Rest],
              (Univ:Inst:Label:Fml),Rest) :-
	reverse(Label,RevLabel),
	\+(\+(justified(RevLabel,AllLabels))),
 	!.

justified_box(AllLabels,[H|T],Fml,[H|RestT]) :-
	justified_box(AllLabels,T,Fml,RestT).
	


% justified(+Label,+AllLab)
%
% Checks whether Label is justified by the list AllLab of labels.

%justified(Label,_) :- ground(Label).

%justified(Label,[Label|T]) :- justified(Label,T),!.
	
%justified(Label,[_|T]) :- justified(Label,T),!.


dia_unblocked(UsedD,[(Univ:Inst:Label:dia Fml)|RestBlockedD],NewB,
              (Univ:Inst:Label:dia Fml),RestBlockedD) :-
	reverse(Label,RevLabel),
	\+( ( member((Fml:TauLabel:TauUniv),UsedD),
	      reverse(TauLabel,RevTauLabel),
	      ipr_ident(RevTauLabel,TauUniv,RevLabel,Univ),
	      \+( ( member((_:TauPrimeLabel:_),NewB),
		    reverse(TauPrimeLabel,RevTauPrimeLabel),
 		    ipr_unif(RevTauLabel,RevTauPrimeLabel),
                    ipr_unif(RevTauPrimeLabel,RevLabel)
                ) )
          ) ),
        !.

dia_unblocked(UsedD,[H|T],NewB,Fml,[H|RestT]) :-
	dia_unblocked(UsedD,T,NewB,Fml,RestT).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% modleantest.pl
% Sicstus Prolog
% Copyright (C) 1998: Bernhard Beckert (University of Karlsruhe) and 
%                     Rajeev Gore (Australian National University)
%                     Email: beckert@ira.uka.de, rajeev.gore@arp.anu.edu.au
%
% Purpose:            Shell and test data for ModLeanTAP
%                     Provides an interface to both Version 1 and Version 2
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:- op(400,fy,-), 
   op(500,xfy,&), 
   op(600,xfy,v),
   op(650,xfy,=>), 
   op(700,xfy,<=>),
   op(400,fy,box), 
   op(400,fy,dia).

% :- 	use_module(modlean_kt,[prove_kt/2]).
% :- 	use_module(modlean_k,[prove_k/2]).
% :- 	use_module(modlean_s4,[prove_s4/2]).
% :- 	use_module(modlean_kd,[prove_kd/2]).

% :- 	use_module(modlean_dec_kt,[decprove_kt/1]).
% :- 	use_module(modlean_dec_k,[decprove_k/1]).
% :- 	use_module(modlean_dec_s4,[decprove_s4/1]).
% :- 	use_module(modlean_dec_kd,[decprove_kd/1]).

% :- 	use_module(nnf,[nnf/3]).

% :- 	use_module(library(lists),[member/2]).

% ------------------------------------------------------------
%
% provefml(+Name)
% incprovefml(+Name)
% decprovefml(+Name)
%
% provefml succeeds if there is a fact fml(Name,Limit,Formula,Logic) in 
% the data base.
% Formula is transformed into negation normal form, and the  appropriate 
% version of ModLeanTAP is used to show is to be unsatisfiable using the 
% limit Limit.
% Some statistics are printed.
%
% incprovefml is the same as provefml, but incrementally increases the 
% limit instead of using Limit.
%
% decprovefml is similar but uses Version 2.0 of ModLeanTAP, i.e., a
% decision procedure.
% It succeeds if there is a fact fml(Name,Limit,Formula,Logic) in 
% the data base.
% Limit is ignored as the decision procedure does not use any limit.

provefml(Name) :-
	fml(Name,Limit,Formula,Logic),
	write(Name),
	nnf(Formula,NNF,_),
	statistics(runtime,[_,_]),
        ( Logic == kt -> ( prove_kt(NNF,Limit) -> Proof = yes ; Proof = no)
        ; Logic == k  -> ( prove_k(NNF,Limit)  -> Proof = yes ; Proof = no)
        ; Logic == kd -> ( prove_kd(NNF,Limit) -> Proof = yes ; Proof = no)
        ; Logic == s4 -> ( prove_s4(NNF,Limit) -> Proof = yes ; Proof = no)
        ; write('No such logic.'), fail
        ),
	!,
        ( Proof = yes ->
          statistics(runtime,[_,Time]),
	  format(' proved in ~w msec, VarLim = ~w ~n',[Time,Limit])
        ; statistics(runtime,[_,Time]),
          format(' no proof after ~w msec. ~n',[Time])
        ).

incprovefml(Name) :-
	fml(Name,_,Formula,Logic),
	write(Name),
	nnf(Formula,NNF,_),
	statistics(runtime,[_,_]),
        ( Logic == kt -> ( prove_kt(NNF,Limit) -> Proof = yes ; Proof = no)
        ; Logic == k  -> ( prove_k(NNF,Limit)  -> Proof = yes ; Proof = no)
        ; Logic == kd -> ( prove_kd(NNF,Limit) -> Proof = yes ; Proof = no)
        ; Logic == s4 -> ( prove_s4(NNF,Limit) -> Proof = yes ; Proof = no)
        ; write('No such logic.'), fail
        ),
        !,
	( Proof = yes ->
          statistics(runtime,[_,Time]),
	  format(' proved in ~w msec, VarLim = ~w ~n',[Time,Limit])
        ; statistics(runtime,[_,Time]),
          format(' no proof after ~w msec. ~n',[Time])
        ).

decprovefml(Name) :-
	fml(Name,_,Formula,Logic),
	write(Name),
	nnf(Formula,NNF,_),
	statistics(runtime,[_,_]),
        ( Logic == kt -> ( decprove_kt(NNF) -> Proof = yes ; Proof = no)
        ; Logic == k  -> ( decprove_k(NNF)  -> Proof = yes ; Proof = no)
        ; Logic == kd -> ( decprove_kd(NNF) -> Proof = yes ; Proof = no)
        ; Logic == s4 -> ( decprove_s4(NNF) -> Proof = yes ; Proof = no)
        ; write('No such logic.'), fail
        ),
        !,
	( Proof = yes ->
          statistics(runtime,[_,Time]),
	  format(' proved to be UNSATISFIABLE in ~w msec. ~n',[Time])
        ; statistics(runtime,[_,Time]),
          format(' shown to be SATISFIABLE in ~w msec. ~n',[Time])
        ).


% fml(+Name,-Limit,-Formula,Logic)
% fml(+Name,-Limit,-Formula)
%
% fml is used to specify a test problem: the formula Formula with name Name 
% is to be proven to be *unsatisfiable* using the version of ModLeanTAP for
% the logic Logic.
%
% The logical operators  - (negation), & (conjunction), v (disjunction),
% => (implication), <=> (equivalence), box (box operator), 
% dia (diamond operator), true, and false may be used.
%
% If no logic is specified (i.e., fml/3 is used), then the logic K is
% used by default.

fml(Name,Limit,Formula,k) :- fml(Name,Limit,Formula).


fml(t1,2,-((box(p => q)) => ((box p) => (box q))),k).

fml(t2,2,-(((box(p & q))) => ((box p) & (box q))),k).

fml(t3,2,-(((box p) & (box (p => q))) => (box q)),k).

fml(t4,2,-((((box p) & (dia (-q))) => (dia (p & (-q))))),k).

fml(t5,2,-((((dia p) v (dia q)) => (dia(p v q)))),k).

fml(t6,2,(-(((box(p => q)) => ((box p) => (box q))) &
           (((box(p & q))) => ((box p) & (box q))) & 
           (((box p) & (box (p => q))) => (box q)) &
           ((((box p) & (dia (-q))) => (dia (p & (-q))))) &
           ((((dia p) v (dia q)) => (dia(p v q)))))),k).



fml(tst1,10,-(box p => p), kt).

fml(tst2,10,-(p => dia p), kt).
      
fml(tst3,10,-(dia dia p => dia p), kt4).

fml(tst4,10,-(box p => box box p), kt4).

fml(tst5,10,-(box p => dia p), kd).

fml(tst6,10,-(box p => dia p), kd45).

fml(tst8,23,-(box (box (p => box p) => p) => (box (p => box p) => p)),s4).

fml(tst9,10,-((box box p) => box p), s4).

fml(tst10,20,-((box box (p => box p)) => box (p => box p)), s4).

fml(tst11,50,-((dia dia p & box dia q & dia q) => dia p), s4).


fml(kt1,10, -(box p => p), kt).

fml(kt2,10, -(p => dia p), kt).

fml(k45a,10,-(dia box p => box p),k45).

fml(k5a,10,-(dia box p => box p), k5).

fml(k5b,10,-(box (box p => box box p)), k5).

fml(kb1,10,-(dia box p => p), kb).

fml(ktb1,10,-(box p => p), ktb).

fml(s5a,10,-(dia box p => p), s5).

fml(s5b,10,-(dia box p => box p), s5).

fml(s5c,10,-(box dia p => dia p), s5).

fml(s4a,10,-(box dia (p => box dia p)), s4).

fml(s4b,10,-(box dia (dia box p => p)), s4).

fml(s4c,10,-((box box (p => box p)) => box (p => box p)), s4).

fml(kt3,20, -(box (p => q) & box ((p => q) => box (p => q)) &
              box (box (p => q) => p) => q), kt).

fml(bt,2,((dia (-q & -p) & dia (r & -p)) & box (p v q)),kd).

fml(raj6c,2, ((box (-(p) v dia-(p))) & (box (p v q) & box (box p v q))
               & dia -(p) & dia(-q))).

fml(raj6c11, 4, ((box (-(p) v dia-(p))) & (box (p v q) & box (box p v
                   q) & box (p v box q)) & dia -(q))).

fml(raj6c1, 4, ((box (-(p) v dia-(p))) & (box (p v q & r) & box (box p
                v q) & box (p v box q)) & dia -(p) & dia -(q))).

fml(raj6e1a1, 3, ((box (q v -(p))) & (box (p) & box (box q) & box (box
                    q)) & dia -(p) & dia -(q))).  

fml(raj6e1a2, 3, ((box (q v -(p))) & (box (p) & box (box q) & box (p))
                    & dia -(p) & dia -(q))).

fml(raj6e1b, 3, ((box (q v -(p))) & (box (p) & box (box p) & box (p v
                    box q)) & dia -(p) & dia -(q))).

fml(lwb1, 1, -(box p1 v box p2 v box p3 v box p5 v false v (dia -p2 v
               dia -p4 v dia -p2 v dia -p6))).

fml(lwb2, 13, -(box p1 v box p2 v box p3 v box p5 v (dia (-p1 & box
                p1) v dia (-p1 & box p3) v dia (-p2 & box p5) v (dia
                (-p3 & box p1) v dia (-p3 & box p3)) v (dia (-p5 & box
                p1) v dia (-p5 & box p3)) v (dia (-p4 & box p2) v dia
                (-p6 & box p2)) v (dia (-p4 & box p4) v dia (-p6 & box
                p4)) v (dia (-p4 & box p6) v dia (-p6 & box p6))) v
                (dia dia -p2 v dia dia -p4 v dia dia -p5 v dia dia
                -p6))).

fml(lwb3, 53, -(box p1 v box p2 v box p3 v box p5 v (dia (-p1 & box
                p1) v dia (-p1 & box p3) v dia (-p2 & box p5) v (dia
                (-p3 & box p1) v dia (-p3 & box p3)) v (dia (-p5 & box
                p1) v dia (-p5 & box p3)) v (dia dia (-p1 & box p1) v
                dia dia (-p1 & box p3) v dia dia (-p1 & box p5)) v
                (dia (-p4 & box p2) v dia (-p6 & box p2)) v (dia dia
                (-p3 & box p1) v dia dia (-p3 & box p3) v dia dia (-p3
                & box p5)) v (dia (-p4 & box p4) v dia (-p6 & box p4))
                v (dia dia (-p5 & box p1) v dia dia (-p5 & box p3) v
                dia dia (-p5 & box p2) v dia dia (-p5 & box p5)) v
                (dia (-p4 & box p6) v dia (-p6 & box p6)) v (dia dia
                (-p2 & box p2) v dia dia (-p4 & box p2) v dia dia (-p6
                & box p2)) v (dia dia (-p2 & box p4) v dia dia (-p4 &
                box p4) v dia dia (-p6 & box p4)) v (dia dia (-p2 &
                box p6) v dia dia (-p4 & box p6) v dia dia (-p6 & box
                p6))) v (dia dia dia -p2 v dia dia dia -p4 v dia dia
                dia -p2 v dia dia dia -p6))).

fml(lwb4, 40, -(box p1 v box p2 v box p3 v box p5 v (dia (-p1 & box
                p3) v dia (-p1 & box p5) v dia (-p2 & box p1) v (dia
                (-p3 & box p3) v dia (-p3 & box p5)) v (dia (-p5 & box
                p3) v dia (-p5 & box p5)) v (dia dia (-p1 & box p1) v
                dia dia (-p1 & box p3) v dia dia (-p1 & box p4) v dia
                dia (-p1 & box p5)) v (dia (-p4 & box p2) v dia (-p6 &
                box p2)) v (dia dia (-p3 & box p1) v dia dia (-p3 &
                box p3) v dia dia (-p3 & box p5)) v (dia (-p4 & box
                p4) v dia (-p6 & box p4)) v (dia dia (-p5 & box p1) v
                dia dia (-p5 & box p3) v dia dia (-p5 & box p5)) v
                (dia (-p4 & box p6) v dia (-p6 & box p6)) v (dia dia
                dia (-p1 & box p3) v dia dia dia (-p1 & box p5)) v
                (dia dia (-p2 & box p2) v dia dia (-p4 & box p2) v dia
                dia (-p6 & box p2)) v (dia dia dia (-p3 & box p3) v
                dia dia dia (-p3 & box p5)) v (dia dia dia (-p4 & box
                p1) v (dia dia (-p2 & box p4) v dia dia (-p4 & box p4)
                v dia dia (-p6 & box p4))) v (dia dia dia (-p5 & box
                p3) v dia dia dia (-p5 & box p5)) v (dia dia (-p2 &
                box p6) v dia dia (-p4 & box p6) v dia dia (-p6 & box
                p6)) v (dia dia dia (-p2 & box p2) v dia dia dia (-p6
                & box p2)) v (dia dia dia (-p2 & box p4) v dia dia dia
                (-p6 & box p4)) v (dia dia dia (-p2 & box p6) v dia
                dia dia (-p6 & box p6))) v (dia dia dia dia -p2 v dia
                dia dia dia -p4 v dia dia dia dia -p1 v dia dia dia
                dia -p6))).


fml(tx1,10,-((box p => dia p) => dia true)).

fml(tx2,10,-((dia true => box p => dia p))).

fml(tx3,10,-((box p => box box p) => box p & dia q => dia (box p & q))).

fml(tx4,10,-((box p & dia -box p => dia (box p & -box p)) => box p =>
              box box p)).

fml(tx5,10,-((dia p => box dia p) => dia p & dia q => dia (dia p & q))).

fml(tx6,10,-((dia p & dia -dia p => dia (dia p & -dia p)) => dia p =>
              box dia p)).

fml(tx7,10,-((p => box dia p) => p & dia q => dia (dia p & q))).

fml(tx8,10,-((p & dia -dia p => dia (dia p & -dia p)) => p => box dia p)).

fml(tx9,10,-((box false => false) => box p => dia p)).

fml(tx10,10,-((box dia -p => dia -p) & (dia box p => box dia box p) &
               box (dia -p => box dia -p) => box p => box box p)).

fml(tx11,10,-((box -p => -p) & (dia p => box dia p) => p => box dia p)).

fml(tx12,10,-((box (box -p => box box -p) & (dia p => box dia dia p)
               => dia p => box dia p))).

fml(tx13,10,-((box (dia -p => box dia -p) & (box p => box dia box p)
               => box p => box box p))).

fml(tx14,10,-((box p => dia p) & (box p => box box p) & (-p => box dia
               -p) => box p => p)).

fml(tx15,10,-((dia box q => box dia q) => dia (p & box q) => box (p v
               dia q))).

fml(tx16,10,-((box (box p => dia p) & (dia (box p & box p) => box (box
               p v dia p)) => dia box p => box dia p))).

fml(tx17,3,-((box (p & box p => q) v box (q & box q => p) => box (p v
               q) & box (box p v q) & box (p v box q) => box p v box q))).

fml(tx18,10,-((box ((p & box p => q) v (q & box q => p)) & box (box (p
               & box p => q) v (q & box q => p)) & box ((p & box p =>
               q) v box (q & box q => p)) => box (p & box p => q) v
               box (q & box q => p)) => box (p & box p => q) v box (q
               & box q => p))).

fml(tx19,10,-((box (box p => q) v box (box q => p) => box (box p v q)
               & box (p v box q) => box p v box q))).

fml(tx20,10,-((box (box (box p => q) v (box q => p)) & box ((box p =>
               q) v box (box q => p)) => box (box p => q) v box (box q
               => p)) => box (box p => q) v box (box q => p))).

fml(tx21,10,-((box (box p => q) v box (box q => p) => box (p & box p
               => q) v box (q & box q => p)))).

fml(tx22,10,-((box (box p => p) & box (box q => q) & (box (p & box p
               => q) v box (q & box q => p)) => box (box p => q) v box
               (box q => p)))).

fml(tx23,10,-((box (box p => p) & box (box q => q) & (box (box p =>
               box q) v box (box q => box p)) => box (box p => q) v
               box (box q => p)))).

fml(tx24,10,-((box (p & box p => q) => box box (p & box p => q)) &
               (box (q & box q => p) => box box (q & box q => p)) &
               box (box p => box box p) & box (box q => box box q) &
               (box (p & box p => q) v box (q & box q => p)) => box
               (box p => box q) v box (box q => box p))).

fml(tx25,10,-((dia box p => box dia box p) & box (dia -p => box dia
               -p) => box (box p => box q) v box (box q => box p))).

fml(tx26,10,-((box (box -p => dia -p) & (box (box p => box -p) v box
               (box -p => box p)) => dia box p => box dia p))).

fml(tx27,5,-((dia p => box dia p) & (dia -p => box dia -p) => dia box
               p => box dia p)).

fml(tx28,10,-((box (box dia p => dia box p) => box dia p => dia box p)
               & box (box p => box box p) & box (box dia p => dia box
               p) => dia box (p => box p))).

fml(tx29,10,-((box dia box (p => box p) => dia box (p => box p)) &
               (box dia -p => box box dia -p) & dia box (p => box p)
               => box dia p => dia box p)).

fml(tx30,20,-((box (dia q & (-p v -q)) => box box (dia q & (-p v -q)))
               & (box dia p => dia box p) => box dia p & box dia q =>
               dia (p & q))).

fml(tx31,10,-((box dia p & box dia -p => dia (p & -p)) => box dia p =>
               dia box p)).

fml(tx32,10,-((box dia p => dia box p) => box p => dia p)).

fml(tx33,10,-((box ((p v dia p) & (-p v dia -p)) => box box ((p v dia
               p) & (-p v dia -p))) & box (box (p & (p v dia p) & (-p
               v dia -p)) => box box (p & (p v dia p) & (-p v dia
               -p))) & box (box (-p & (p v dia p) & (-p v dia -p)) =>
               box box (-p & (p v dia p) & (-p v dia -p))) & (box dia
               p => dia box p) => box (p v dia p) => dia (p & box
               p))).

fml(tx34,10,-((box (p v dia p) => dia (p & box p)) => box dia p => dia
               box p)).

fml(tx35,10,-((box (box (p & box p) => p & box p) => box (p & box p))
               => box p => box box p)).

fml(tx36,10,-((box (box false => false) => box false) => box dia true
               => box false)).

fml(tx37,10,-((box (box p => p) => box p) => box (box p => p) => dia
               box p => box p)).

fml(tx38,10,-((box dia true => box false) & (box (box p => p) => dia
               box p => box p) => box (box p => p) => box p)).

fml(tx39,10,-((box (box (p & box (box p => p)) => p & box (box p =>
               p)) => box (p & box (box p => p))) & box box (box (box
               p => p) => box p) => box (box (p => box p) => p) => box
               p)).

fml(tx40,10,-((box (box p => box box p) & (box (box p => p) => dia box
               p => box p) => box (box (p => box p) => p) => dia box p
               => box p))).

fml(tx41,10,-((box (box (p => box p) => p) => box box (box (p => box
               p) => p)) & box (box p => box box p) & (box (box (p =>
               box p) => p) => dia box p => p) & (box (box ((p => box
               p) => box (p => box p)) => p => box p) => dia box (p =>
               box p) => p => box p) => box (box (p => box p) => p) =>
               dia box p => box p)).

fml(tx42,10,-((box p => p) & (box (box (p => box p) => p) => dia box p
               => box p) => box (box (p => box p) => p) => dia box p
               => p)).

fml(tx43,10,-((box (box p => p) & (box (box (p => box p) => p) => dia
               box p => p) => box (box (p => box p) => box p) => dia
               box p => p))).

fml(tx44,10,-((box (box (box (p => box p) => box p) => dia box p => p)
               => box (box (p => box p) => box p) => dia box p => p) &
               box (box (p => box p) => p => box p) & box (box (box (p
               => box p) => box p) => dia box p => p) => box (box (p
               => box p) => p) => dia box p => p)).

fml(tx45,20,-((box (box p => p) & (box (box (p => box p) => p) => box
               box (box (p => box p) => p)) & box (box p => box box p)
               & (box (box (p => box p) => p) => dia box p => p) &
               (box (box ((p => box p) => box (p => box p)) => p =>
               box p) => dia box (p => box p) => p => box p) => box
               (box (p => box p) => box p) => dia box p => box p))).

fml(tx46,10,-((box p => p) & (box (box (box (p => box p) => box p) =>
               dia box p => box p) => box (box (p => box p) => box p)
               => dia box p => box p) & box (box (p => box p) => p =>
               box p) & box (box (box (p => box p) => box p) => dia
               box p => box p) => box (box (p => box p) => p) => dia
               box p => p)).

fml(tx47,10,-((box (box (p => box p) => p) => dia box p => p) => box
               (box (p => box p) => p) => dia box p => p v box p)).

fml(tx48,10,-((box p => p) & (box (box (p => box p) => p) => dia box p
               => p v box p) => box (box (p => box p) => p) => dia box
               p => p)).

fml(tx49,10,-((box (box ((box (p => box p) => p) & (box (box (p => box
               p) => p) => box box (box (p => box p) => p)) => box
               ((box (p => box p) => p) & (box (box (p => box p) => p)
               => box box (box (p => box p) => p)))) => (box (p => box
               p) => p) & (box (box (p => box p) => p) => box box (box
               (p => box p) => p))) => (box (p => box p) => p) & (box
               (box (p => box p) => p) => box box (box (p => box p) =>
               p))) & box (box (box (p => box p) => p) => p) => box
               (box (p => box p) => p) => box p)).

fml(tx50,10,-((box p => p) & (box (box (box (p => box p) => p) => box
               p) => box (box (p => box p) => p) => box p) & box (box
               (box (p => box p) => p) => box p) => box (box (p => box
               p) => p) => p)).

fml(tx51,10,-((box (box (p => box p) => p) => p) & box (box (box (p =>
               box p) => p) => p) => box (box (p => box p) => box p)
               => p)).

fml(tx52,10,-((box (box (p => box p) => p => box p) & (box -box (box
               (p => box p) => p) => -box (box (p => box p) => p)) &
               (box (box (p => box p) => box p) => p) => box (box (p
               => box p) => p) => p))).

fml(tx53,10,-((box (box ((box (p => box p) => box p) & (box (box (p =>
               box p) => p) => p) & (box ((box (p => box p) => box p)
               & (box (box (p => box p) => p) => p)) => box box ((box
               (p => box p) => box p) & (box (box (p => box p) => p)
               => p))) => box ((box (p => box p) => box p) & (box (box
               (p => box p) => p) => p) & (box ((box (p => box p) =>
               box p) & (box (box (p => box p) => p) => p)) => box box
               ((box (p => box p) => box p) & (box (box (p => box p)
               => p) => p))))) => (box (p => box p) => box p) & (box
               (box (p => box p) => p) => p) & (box ((box (p => box p)
               => box p) & (box (box (p => box p) => p) => p)) => box
               box ((box (p => box p) => box p) & (box (box (p => box
               p) => p) => p)))) => (box (p => box p) => box p) & (box
               (box (p => box p) => p) => p) & (box ((box (p => box p)
               => box p) & (box (box (p => box p) => p) => p)) => box
               box ((box (p => box p) => box p) & (box (box (p => box
               p) => p) => p)))) & box (box (box (p => box p) => p) =>
               p) => box (box (p => box p) => box p) => box p)).

fml(tx54,10,-((box p => p) & box (box (p => box p) => p => box p) &
               (box (box (p => box p) => box p) => box p) => box (box
               (p => box p) => p) => p)).

fml(tx55,10,-((box (box (p => box p) => p) => p) & (box (box ((box (p
               => box p) => p) & (box (box (p => box p) => p) => box
               box (box (p => box p) => p)) => box ((box (p => box p)
               => p) & (box (box (p => box p) => p) => box box (box (p
               => box p) => p)))) => (box (p => box p) => p) & (box
               (box (p => box p) => p) => box box (box (p => box p) =>
               p))) => (box (p => box p) => p) & (box (box (p => box
               p) => p) => box box (box (p => box p) => p))) => box
               (box (p => box p) => p) => p v box p)).

fml(tx56,10,-((box p => p) & (box (box (box (p => box p) => p) => p v
               box p) => box (box (p => box p) => p) => p v box p) &
               box (box (box (p => box p) => p) => p v box p) => box
               (box (p => box p) => p) => p)).

fml(tx57,30,-((box (box ((box (p => box q) => box q) & (box (box (p =>
               box q) => box q) => box box (box (p => box q) => box
               q)) => box ((box (p => box q) => box q) & (box (box (p
               => box q) => box q) => box box (box (p => box q) => box
               q)))) => (box (p => box q) => box q) & (box (box (p =>
               box q) => box q) => box box (box (p => box q) => box
               q))) => (box (p => box q) => box q) & (box (box (p =>
               box q) => box q) => box box (box (p => box q) => box
               q))) & (box (box ((box ((p => box q) => box (p => box
               q)) => p => box q) & (box (box ((p => box q) => box (p
               => box q)) => p => box q) => box box (box ((p => box q)
               => box (p => box q)) => p => box q)) => box ((box ((p
               => box q) => box (p => box q)) => p => box q) & (box
               (box ((p => box q) => box (p => box q)) => p => box q)
               => box box (box ((p => box q) => box (p => box q)) => p
               => box q)))) => (box ((p => box q) => box (p => box q))
               => p => box q) & (box (box ((p => box q) => box (p =>
               box q)) => p => box q) => box box (box ((p => box q) =>
               box (p => box q)) => p => box q))) => (box ((p => box
               q) => box (p => box q)) => p => box q) & (box (box ((p
               => box q) => box (p => box q)) => p => box q) => box
               box (box ((p => box q) => box (p => box q)) => p => box
               q))) & box (box (box ((p => box q) => box (p => box q))
               => p => box q) => p => box q) => box (box (p => box q)
               => box q) & box (box (-p => box q) => box q) => box
               q)).

fml(tx58,10,-((box (box (p => box p) => p) => box (p => box p) => p) &
               box box (box (p => box p) => p => box p) & (box (box (p
               => box p) => p) => box box (box (p => box p) => p)) &
               box (box p => box box p) & (box (box (p => box (p =>
               box p)) => box (p => box p)) & box (box (-p => box (p
               => box p)) => box (p => box p)) => box (p => box p)) =>
               box (box (p => box p) => p) => p)).

fml(tx59,10,-((box (box (p => box p) => p) => p) => box p => p)).

fml(tx60,10,-((box (box (p & (box p => box box p) => box (p & (box p
               => box box p))) => p & (box p => box box p)) => p &
               (box p => box box p)) => box p => box box p)).

/*
fml(tx61,10,-((box (box (dia -p => box dia -p) => dia -p) => dia -p) &
               (box (box (dia -p & (box dia -p => box box dia -p) =>
               box (dia -p & (box dia -p => box box dia -p))) => dia
               -p & (box dia -p => box box dia -p)) => dia -p & (box
               dia -p => box box dia -p)) & (box (box (-box (p => box
               p) & (box -box (p => box p) => box box -box (p => box
               p)) => box (-box (p => box p) & (box -box (p => box p)
               => box box -box (p => box p)))) => -box (p => box p) &
               (box -box (p => box p) => box box -box (p => box p)))
               => -box (p => box p) & (box -box (p => box p) => box
               box -box (p => box p))) & box (box (box (p => box p) =>
               p) => p) => box dia p => dia box p)).
*/

fml(tx62,10,-((box (box (p => box p) => p) => p) => box (box (p => box
               p) => p) => dia box p => p)).

fml(tx63,10,-((box (box (p => box p) => p) => box p) => box (box (p =>
               box p) => p) => dia box p => box p)).

fml(tx64,10,-((box (box -p => -p) & (box dia p => dia box p) & (box
               (box (p => box p) => p) => dia box p => p) => box (box
               (p => box p) => p) => p))).

fml(tx65,10,-((dia box (box p => q) => (box p => q) => box (box p =>
               q)) => box (box p => q) v box (box q => p))).

fml(tx66,10,-((box dia box p => dia box p) & (dia box p => p => box p)
               => box dia box p => p => box p)).

fml(tx67,10,-((box box (box (box p => box box p) => box p => box box
               p) & box box (box -(box dia box p & box p) => -(box dia
               box p & box p)) & box (box p => box box p) & (box (box
               dia box p => -box p) v box (box -box p => dia box p)) &
               (box dia box p => p => box p) => dia box p => p => box
               p))).

fml(tx68,10,-((box dia p => dia p) & box (box -box dia p => -box dia
               p) & (box dia p => box box dia p) & box (dia box dia p
               => p => box p) => box dia p => dia box p)).

fml(tx69,10,-((box box (box -p => -p) & (dia box dia p => p => box p)
               => dia box p => p => box p))).

fml(tx70,10,-((box -box p => box box -box p) & box (box dia p => dia
               box p) & (dia box p => p => box p) => dia box dia p =>
               p => box p)).

fml(tx71,10,-((box box box (box p => p) & (dia box dia p => p => box
               p) & box (dia box dia -p => -p => box -p) => p => box
               (dia p => p)))).

fml(tx72,10,-((box (box (p => box p) => p) => box (p => box p) => p) &
               box (box (p & (box (p => box p) => p)) => box box (p &
               (box (p => box p) => p))) & (dia box (p => box p) => (p
               => box p) => box (p => box p)) => box (box (p => box p)
               => p) => dia box p => p)).

