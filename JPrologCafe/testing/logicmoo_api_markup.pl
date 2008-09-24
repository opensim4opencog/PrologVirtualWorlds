:-include('logicmoo_utils_header.pl'). %<?
% ===================================================================
% File 'moo_markup.pl'
% Maintainers: Douglas Miles
% Contact: dmiles@users.sourceforge.net ;
% Version: 'moo_markup.pl' 1.0.0
% Revised At:  $Date: 2002/12/03 08:34:05 $
% ===================================================================
% Major functions:
% This file meets the needs of an external agent working for the needs of eigther an automated or human user
% Interface with Java with XML to display explaination trees and variable bindings
% ===================================================================
:-index(toMarkup(1,1,1,1)).
:-index(toMarkupFormula(1,1,1,1)).

% ===================================================================
% toMarkup(-Markup,-Prolog,-PrologVarableList, +Output)
% example: toMarkup(html, explaination(('Military':996:subclass('IntransitiveRelation', 'BinaryRelation')^B)* ('Military':836:subclass('BinaryRelation', 'Relation')^C)*forall('IntransitiveRelation', forall(D, forall('Relation', holds(subclass, 'IntransitiveRelation', D)and holds(subclass, D, 'Relation')=>holds(subclass, 'IntransitiveRelation', 'Relation'))))*sfind(instance(subclass, 'PartialOrderingRelation'))*sfind(subclass('PartialOrderingRelation', 'TransitiveRelation'))* ('Military':2756:instance(on, 'IntransitiveRelation')^E)), ['X'=on|A],O),write_ln(O).
%
% Markup := [html,kif,pml,leml] (Expandable)
% Prolog := any prolog term
% PrologVaraibles list is the equal list as produced by read/3  [=(Name,Val)|...]
% Output is an CharicterAtom (the difference is this atom is not added the the symbol table)
% ===================================================================
% ===================================================================
% term_to_leml(-Prolog, +Output)
%
% arity 2 version (note html) is sufficient for printing values
% ===================================================================


toMarkup(chat,Var,VS,Chars):-!,catch(toMarkup(bach,Var,VS,Chars),_,true),!.
toMarkup(java,Var,VS,Chars):-!,catch(toMarkup(html,Var,VS,Chars),_,true),!.

% VARIABLES
toMarkup(L,C,Vars,Chars):-isSlot(C),!,toMarkupSlot(L,C,Vars,Chars).

% COMPOUND
toMarkup(L,C,Vars,Chars):-compound(C),!,toMarkupFormula(L,C,Vars,Chars).


toMarkup(html,'$spacer',Vars,'\n<hr>\n').
toMarkup(_,'$spacer',Vars,'\n;; ------------------------------------------------------------------------------\n\n').

toMarkup(L,formula(C),Vars,Chars):-!,toMarkupFormula(L,C,Vars,Chars).

%Terminal Control
toMarkup(html,lparen,Vars,'('):-!.
toMarkup(html,rparen,Vars,')'):-!.
toMarkup(kif,lparen,Vars,'('):-!.
toMarkup(kif,rparen,Vars,')'):-!.
toMarkup(html,nl,Vars,'<br>'):-!.
toMarkup(html,tab,Vars,'<li>'):-!.
toMarkup(kif,nl,Vars,'\n'):-!.
toMarkup(kif,tab,Vars,'\t'):-!.

toMarkup(_,surf,Vars,''):-!.
toMarkup(_,end_of_file,Vars,''):-!.

toMarkup(_,',',Vars,'and'):-!.
toMarkup(_,';',Vars,'or'):-!.
toMarkup(_,'=',Vars,'equal'):-!.
toMarkup(_,'deduced',Vars,' ').


%LISTS
%toMarkup(LANG,[COMP],Vars,Atom)

toMarkup(L,[],Vars,Atom):-toMarkup(L,'NullSet',Vars,Atom).
%toMarkup(html,[Su|Bj],Vars,Chars):-toMarkupList(html,[Su|Bj],Vars,Chars1),sformat(Chars,'<div>(<ul>~w </ul>)</div>',[Chars1]).

close_varlist([]):-!.
close_varlist('$VAR'(_)):-!.
close_varlist([V|VV]):-close_varlist(VV),!.


toMarkup(html,option(Option),Vars,Chars):-sformat(Chars,'<option value="~w">~w</option>',[Option,Option]).

% Numbers
toMarkup(_,Atom,_VS,Chars):-float(Atom),!,sformat(Chars,'~f',[Atom]).
toMarkup(_,Atom,_VS,Chars):-number(Atom),!,sformat(Chars,'~w',[Atom]).

toMarkup(L,Value,Vars,Chars):-
        mooCache(PredR, skolem, Value = x(Name,Expression),SKVARS,Context, Ctx, TN, Auth, State),!,
            toMarkup(kif,Name,Vars,NameQ),  prependQuestionMark(NameQ,NameQM),
            subst(x(Sk,Expression),Sk,NameQM,x(NSk,NExpression)),!,
            toMarkup(L,exists([NSk],NExpression),SKVARS,Chars).

% PRETTYNESS
toMarkup(_,';',Vars,'or ').
toMarkup(_,',',Vars,'and ').
toMarkup(_,'neg',Vars,'neg ').
%toMarkup(_,entails,Vars,'modus-ponens ').
%toMarkup(_,entails,Vars,'modus-tollens ').

%toMarkup(L,Char,Vars,Char):-L==kif.

toMarkup(html,Atom,Vars,Char):-!,
      my2_www_form_encode(Atom,E),
      sformat(Char,'<a href="browse.moo?find=~w">~w<a>',[E,Atom]).

toMarkup(bach,Atom,Vars,Chars):-atom(Atom),atom_codes(Atom,Codes),member(32,Codes),sformat(Chars,'~w',[Atom]).
%TODO toMarkup(bach,Atom,Vars,Chars):-atom(Atom),sformat(Chars,'"~w"',[Atom]).
toMarkup(bach,Atom,Vars,Chars):-atom(Atom),sformat(Chars,'~w',[Atom]).
      

toMarkup(_,Atom,Vars,Atom):-!.

my2_www_form_encode(H,E):-
	 sformat(S,'~q',[H]),
	 string_to_atom(S,A),
	 www_form_encode(A,E),!.


%TODO Number?

% ================================================
%      toMarkupList
% ================================================

toMarkupList(L,Var,VS,Chars):-isSlot(Var),!,toMarkupSlot(L,Var,VS,Chars).
toMarkupList(_,[],VS,''):-!.
toMarkupList(LANG,[H],VS,Chars):-!,
        toMarkup(LANG,H,VS,Chars).
toMarkupList(LANG,[H|T],VS,Chars):-!,
        toMarkup(LANG,H,VS,Chars1),
        toMarkupList(LANG,T,VS,Chars2),
        sformat(Chars,'~w ~w',[Chars1,Chars2]).

markUpVARLIST(L,[],Vars,''):-!.
markUpVARLIST(L,'$VAR'(_),Vars,''):-!.

markUpVARLIST(L,[VV|Varnames],Vars,Chars):-
                  VV=..[_,Name,Value],!,
                  toMarkupVarEquals(L,'$VAR'(Name),Value,Vars,Chars1),
                  markUpVARLIST(L,Varnames,Vars,Chars2),
                  sformat(Chars,'~w\n~w',[Chars1,Chars2]).

toMarkupVarEquals(_,Name,Value,Vars,Chars):-
            toMarkup(kif,Name,Vars,NameQ),
            toMarkup(L,Value,Vars,ValChars),
            sformat(Chars,'~w = ~w',[NameQ,ValChars]).

% Real Prolog Var
toMarkupSlot(L,Slot,VarList,Chars):- isVarProlog(Slot),!,
        toMarkup_makeNamePrologVar(L,VarList,Slot,Name),
        atom_concat('?',Name,Chars),!.
% Slot 'Typed'
toMarkupSlot(L,Slot,VarList,Chars):-isQualifiedAs(Slot,BaseType,Value,Subtype), !,
        toMarkup_makeName(L,VarList,Slot,Subtype,Value,Name),
        close_freeVars(VarList,NVarList),
        append(NVarList,[Name=Value],NV),
        toMarkup(L,Value,NV,VChars),
        sformat(Chars,'<div title="~w">~w</div>',[Subtype,VChars]).

toMarkup_makeNamePrologVar(L,VarList,Value,Name):-member(Name=Var,VarList),Var==Value,!.
toMarkup_makeNamePrologVar(L,VarList,Value,Name):-getVarAtom(Value,NUame),atom_concat('',NUame,Name).

getVarAtom(Value,Name):-var(Value),!,term_to_atom(Value,Vname),atom_codes(Vname,[95,_|CODES]),atom_codes(Name,CODES),!.
getVarAtom('$VAR'(VNUM),Name):-concat_atom([VNUM],Name),!.



toMarkup_makeName(L,VarList,Slot,BaseType,Value,Name):-
        member(Name=Var,VarList),Var==Slot,!.
toMarkup_makeName(L,VarList,Slot,BaseType,Value,Name):-
        member(Name=Var,VarList),Var==Value,!.
toMarkup_makeName(L,VarList,Slot,BaseType,Value,Name):-atom_concat('?',BaseType,Name).



close_freeVars(V,V):-proper_list(V),!.
close_freeVars(V,[]):-isSlot(V),!. %Closing List if there are no free getPrologVars
close_freeVars([X|XX],[X|More]):- close_freeVars(XX,More).





toMarkup_varProlog(kif,Var,_VS,NameQ):- _VS=[VV|_],nonvar(VV),VV=..[_,Name,VarRef],number(Name),Var==VarRef,!,sformat(NameQ,'?~d',[Name]).
toMarkup_varProlog(kif,Var,_VS,NameQ):- _VS=[VV|_],nonvar(VV),VV=..[_,Name,VarRef],Var==VarRef,!,sformat(NameQ,'?~w',[Name]).

toMarkup_varProlog(html,Var,_VS,NameQ):- _VS=[VV|_],nonvar(VV),VV=..[_,Name,VarRef],number(Name),Var==VarRef,!,sformat(NameQ,'?~d',[Name]).
toMarkup_varProlog(html,Var,_VS,NameQ):- _VS=[VV|_],nonvar(VV),VV=..[_,Name,VarRef],Var==VarRef,!,sformat(NameQ,'?~w',[Name]).

toMarkup_varProlog(T,Var,[_|Rest],Name):-nonvar(Rest),toMarkup_varProlog(T,Var,Rest,Name).
toMarkup_varProlog(kif,VAR,_,VarName):-term_to_atom(VAR,AVAR),atom_codes(AVAR,[95|CODES]),!,catch(sformat(VarName,'?HYP-~s',[CODES]),_,VarName='?HYP-AVAR').
toMarkup_varProlog(kif,VAR,_,VarName):-term_to_atom(VAR,AVAR),atom_codes(AVAR,CODES),!,catch(sformat(VarName,'?HYP-~s',[CODES]),_,VarName='?HYP-AVAR').
toMarkup_varProlog(html,VAR,VS,VarName):-toMarkup_varProlog(kif,VAR,VS,VarName).

prependQuestionMark(Name,NameQ):-atom_concat('?',Name,NameQ).


% ===================================================
% Special Objects
% ===================================================
%toMarkupFormula(L,C,Vars,Chars):-       writeq( C=Vars),nl,fail.


toMarkupFormula(L,C,Vars,Chars):-isSlot(C),!,toMarkupSlot(L,C,Vars,Chars).
toMarkupFormula(L,C,Vars,Chars):-not(compound(C)),!,toMarkup(L,C,Vars,Chars).


% QUOTED STRING FORMAT
toMarkupFormula(L,string([]),Vars,'""'):-!.
toMarkupFormula(L,string(''),Vars,'""'):-!.
toMarkupFormula(L,string(Atom),Vars,Chars):-!,sformat(Chars,'"~w"',[Atom]).
toMarkupFormula(L,'$stringCodes'(Atom),Vars,Chars):-!,sformat(Chars,'"~w"',[Atom]).
toMarkupFormula(L,'$stringSplit'(List),Vars,Chars):-
      toMarkupList(L,List,Vars,Chars1),sformat(Chars,'"~w"',[Chars1]).
toMarkupFormula(L,Atom,_VS,Chars):-((isCharCodelist(Atom);string(Atom);is_string(Atom))),!,
%        catch(sformat(Chars,'"~s"',[Atom]),_,sformat(Chars,'"~w"',[Atom])).
        catch(sformat(Chars,'~s',[Atom]),_,sformat(Chars,'~w',[Atom])).


toMarkupFormula(bach,float(Atom),_VS,Chars):-float(Atom) -> sformat(Chars,'~f',[Atom]);sformat(Chars,'~w',[Atom]).
toMarkupFormula(bach,int(Atom),_VS,Chars):-sformat(Chars,'~w',[Atom]).



toMarkupFormula(kif,'.'(Su,Bj),Vars,Chars):- 
      Bj \==[],
      atom(Su),not(is_list(Bj)),!,
      toMarkupFormula(kif,Su,Vars,Chars1),
      toMarkupFormula(kif,Bj,Vars,Chars2),!,
      sformat(Chars,'(~w . ~w)',[Chars1,Chars2]).


% SPECIAL FORMATS


toMarkupFormula(Form,saved(Data,Mt,Vars,Lits),_VS,Chars):-
      clause(saved(Data,Mt,Vars,Lits),true,ID),!,
      toMarkupFormula(Form,Data,Vars,Chars1),
      sformat(Chars,'<a href="editform.moo?id=~w" title="Edit EL ~w"><img border=0 src="http://www.cyc.com/cycdoc/img/cb/white.gif"></a>~w in ~w',[ID,ID,Chars1,Mt]),!.

toMarkupFormula(L,saved(Data,Mt,Vars,Lits),_,O):-!,toMarkupFormula(L,Data,_,O).

toMarkupFormula(L,':-'(C,true),Vars,Chars):-toMarkup(L,C,Vars,Chars).
toMarkupFormula(L,(T^V),Vars,Chars):-var_merge(Vars,V,TVars),!,toMarkup(L,T,TVars,Chars).

% No parens (nv = no vector)
toMarkupFormula(L,nv(Subj),Vars,Chars):-is_list(Subj),!,toMarkupList(L,Subj,Vars,Chars).
toMarkupFormula(L,nv(Subj),Vars,Chars):-!,toMarkup(L,Subj,Vars,Chars).

toMarkupFormula(_,writeFmt(F,A),Vars,Chars):-sformat(Chars,F,A),!.

toMarkupFormula(kif,[Su|Bj],Vars,Chars):-toMarkupList(kif,[Su|Bj],Vars,Chars1),sformat(Chars,'(~w)',[Chars1]).

toMarkupFormula(_,writeq(Term),Vars,Atom):-!,sformat(Atom,'~q',[Term]).
toMarkupFormula(kif,maillink(Title,Address,Subject),Vars,Address):-!.
toMarkupFormula(kif,weblink(Title,URL),Vars,Title):-!.
toMarkupFormula(kif,helplink(Title,URL),Vars,Title):-!.
toMarkupFormula(L,explaination(PB),Vars,Atom):-
        flag(explaination_linenumber,_,1),
        toMarkupFormula(L,PB,Vars,AtomS),!,
        sformat(Atom,'\nExplaination:\n~w\n',[AtomS]).

toMarkupFormula(LANG,krlog(COMP),Vars,Atom):-!,prolog_to_krlog(COMP,KR),toMarkup(LANG,KR,Vars,Atom).

toMarkupFormula(LANG,kif(COMP),Vars,Atom):-!,toMarkup(kif,COMP,Vars,Atom).
toMarkupFormula(LANG,html(COMP),Vars,Atom):-!,toMarkup(html,COMP,Vars,Atom).

toMarkupFormula(html,select(Name,OptionList),Vars,Chars):-toMarkup(html,options(OptionList),Vars,Options),sformat(Chars,'<select sort name="~w" id="~w" size="1">~w</select>',[Name,Name,Options]).
toMarkupFormula(html,chectheoryox(Name,on),Vars,Chars):-
                sformat(Chars,'<input type=chectheoryox name="~w" id="~w" checked>',[Name,Name]),!.
toMarkupFormula(html,chectheoryox(Name,_),Vars,Chars):-
                sformat(Chars,'<input type=chectheoryox name="~w" id="~w">',[Name,Name]),!.
toMarkupFormula(html,options([]),Vars,'').

toMarkupFormula(L,getPrologVars(Form),Vars,Chars):-markUpVARLIST(L,Form,Vars,SChars),sformat(Chars,'~w',[SChars]),!.

toMarkupFormula(L,getPrologVars(Form),Vars,Chars):-!,sformat(Chars,'; var_post_err (~q). ',[Form]).


toMarkupFormula(html,qresult(Res),Vars,Chars):-!,sformat(Chars,'Result ',[Res]).

toMarkupFormula(kif,qresult(Res),Vars,''):-!. %,sformat(Chars,'res="~w"\n',[Res]).

% Back into Standard Terms

format_o(Format,Stuff):-
        toMarkup(html,Stuff,_,Chars),writeFmt(Format,[Chars]).


toMarkupFormula(html,options([Option|List]),Vars,Chars):-
               toMarkup(html,option(Option),Vars,Chars2),
               toMarkup(html,options(List),Vars,Out3),
               atom_concat(Chars2,Out3,Chars).

toMarkupFormula(Form,entails(A,C),Vars,Chars):-!,
      toMarkupFormula(Agent,'#$codeEntailment'(C,A),Vars,Chars1),
      sformat(Chars,'<img src="http://www.cyc.com/cycdoc/img/cb/purple.gif">~w in CODE',[Chars1]),!.


toMarkupFormula(L,hidden(F,Args),Vars,''):-!.

toMarkupFormula(L,'$nartFixed'(C),Vars,O):-!,toMarkupFormula(L,(C),Vars,O),!.

toMarkupFormula(html,colourize(Color,Thing),Vars,Chars):-!,
        toMarkupFormula(html,Thing,Vars,Chars1),!,
        sformat(Chars,'<font color="~w">~w</font>\n',[Color,Chars1]).

toMarkupFormula(L,colourize(Color,Thing),Vars,Chars):-!,
        toMarkupFormula(L,Thing,Vars,Chars),!.

/*
toMarkupFormula(L,','(A,B),Vars,Chars):-!,
        prolog_to_krlog(','(A,B),KR),
        toMarkupFormula(L,KR,Vars,Chars),!.
*/


toMarkupFormula(L,write_dollar('$v',[A|Args]),Vars,Chars):-!,
                Flag=..[getPrologVars,A|Args],!,
                toMarkupFormula(L,Flag,Vars,Chars).

toMarkupFormula(L,table_(Goal,Lits),Vars,Chars):-!,
                toMarkupFormula(L,table_p(Lits,Goal),Vars,Chars).


toMarkupFormula(L,write_dollar(F,[A|Args]),Vars,Chars):-!,
        toMarkupFormula(L,A,Vars,Chars1),
        toMarkupFormula(L,hidden(F,Args),Vars,Chars2),!,
        sformat(Chars,'~w~w',[Chars1,Chars2]).

toMarkupFormula(L,'$existential'(VarName,Name,Literal),Vars,O):-!,
        toMarkupFormula(L,'existential'(VarName),Vars,O).

toMarkupFormula(L,'$eval'(Function),Vars,O):-!,
        toMarkupFormula(L,' eval'(Function),Vars,O).


toMarkupFormula(L,functional(VarName,Domains,Literal),Vars,O):-
        toMarkup(L,Literal,Vars,O),!.

close_list_var(M,[]):-isSlot(M),!.
close_list_var([[M]|Ms],[M|Ls]):-!,
        close_list_var(Ms,Ls).
close_list_var([M|Ms],[M|Ls]):-!,
        close_list_var(Ms,Ls).


      

toMarkupFormula(L,unused(C,P),Vars,O):-!,
        toMarkupFormula(L,notused(C,writeq(P)),Vars,O).

toMarkupFormula(L,ff([]),Vars,'[]'):-!.

toMarkupFormula(L,ff([Flag|Flags]),Vars,Chars):-!,
        toMarkupFormula(L,flag(Flag),Vars,Chars1),
        toMarkupFormula(L,ff(Flags),Vars,Chars2),
        sformat(Chars,'~w, ~w',[Chars1, Chars2]).

toMarkupFormula(L,domargs([]),Vars,''):-!.

toMarkupFormula(L,domargs([(P:N)]),Vars,Chars):-!,
        toMarkupFormula(L,P,Vars,Chars1),
        sformat(Chars,'~w:~w',[Chars1,N]).

toMarkupFormula(L,domargs([(P:N)|Flags]),Vars,Chars):-!,
        toMarkupFormula(L,P,Vars,Chars1),
        toMarkupFormula(L,domargs(Flags),Vars,Chars2),
        sformat(Chars,'~s:~w,~w',[Chars1,N,Chars2]).

toMarkupFormula(L,flag(Flag),Vars,Chars):-
        Flag=..[domainV,Var,DomArgs],!,
        toMarkupFormula(L,Var,Vars,VarChars),
        toMarkupFormula(L,domargs(DomArgs),Vars,ArgChars),
        sformat(Chars,'~w(~w,[~w])',[domainV,VarChars,ArgChars]).

toMarkupFormula(L,flag(Flag),Vars,Chars):-
        Flag=..[Name,Var,Args],!,
        toMarkupFormula(L,Var,Vars,VarChars),
        sformat(Chars,'~w(~w, ~q)',[Name,VarChars,Args]).
toMarkupFormula(L,flag(Flag),Vars,Chars):-!,
        toMarkupFormula(L,writeq(Flag),Vars,Chars).


toMarkupFormula(L,writeq(Atom),_VS,Chars):-!,sformat(Chars,'~q',[Atom]).

toMarkupFormula(L,[],Vars,''):-!.

toMarkupFormula(bach,':'(Bj,list([])),Vars,Chars):-!,sformat(Chars,'~w:[]',[Bj]).
toMarkupFormula(bach,':'(Bj,map([])),Vars,Chars):-!,sformat(Chars,'~w:{}',[Bj]).
toMarkupFormula(bach,':'(Bj,More),Vars,Chars):-!,
	 toMarkupFormula(bach,More,Vars,Chars2),
        sformat(Chars,'~w:~w',[Bj,Chars2]).

toMarkupFormula(bach,[Bj],Vars,Chars):-!,toMarkupFormula(bach,Bj,Vars,Chars).
toMarkupFormula(bach,[Bj|More],Vars,Chars):-!,
	 toMarkupFormula(bach,Bj,Vars,Chars1),
	 toMarkupFormula(bach,More,Vars,Chars2),
        sformat(Chars,'~w,~w',[Chars1,Chars2]).

%toMarkupFormula(L,[A | B],Vars,Chars):-proper_list([A | B]),append(['('|[A | B],[')'],TRY),toMarkupList(L,[Su|Bj],Vars,Chars).
%toMarkupFormula(L,[A | B],Vars,Chars):-catch(TRY=..['',A | B],_,fail),toMarkupFormula(L,TRY,Varsr,Chars),!.
%toMarkupFormula(L,[A | B],Vars,Chars):-catch(TRY=..[A | B],_,fail),toMarkupFormula(L,TRY,Vars,Chars),!.
%toMarkupFormula(L,[A | B],Vars,Chars):-catch(TRY=..[A | B],_,fail),toMarkupFormula(L,TRY,Vars,Chars),!.
toMarkupFormula(L,[Su|Bj],Vars,Chars):-
        toMarkupList(L,[Su|Bj],Vars,Chars1),
        sformat(Chars,'(~w)',[Chars1]).



/*
toMarkupFormula(L,Term,Vars,O):-
        Term=..[holds,F|Args],isNonVar(F),not_a_function(F),!,
        NTerm=..[F|Args],
        toMarkupFormula(L,NTerm,Vars,O).
*/
toMarkupFormula(L,'$VAR'(_)* X ,Vars,Chars):-!,toMarkupFormula(L, X ,Vars,Chars).
toMarkupFormula(L, X * '$VAR'(_) ,Vars,Chars):-!,toMarkupFormula(L, X ,Vars,Chars).
toMarkupFormula(L,(A * []),Vars,Chars):-!,toMarkupFormula(L,A ,Vars,Chars).
toMarkupFormula(L,([] * A),Vars,Chars):-!,toMarkupFormula(L,A ,Vars,Chars).
toMarkupFormula(L,deduced* X ,Vars,Chars):-!,toMarkupFormula(L, X ,Vars,Chars).
toMarkupFormula(L, X * deduced ,Vars,Chars):-!,toMarkupFormula(L, X ,Vars,Chars).


toMarkupFormula(L,domainV(Var,ReqsL),Vars,Chars):-
        toMarkupFormula(L,' domainV'(Var,writeq(ReqsL)),Vars,Chars).
toMarkupFormula(L,domainC(Var,ReqsL),Vars,Chars):-
        toMarkupFormula(L,' domainC'(Var,writeq(ReqsL)),Vars,Chars).
toMarkupFormula(L,domainA(Var,ReqsL),Vars,Chars):-
        toMarkupFormula(L,' domainA'(Var,writeq(ReqsL)),Vars,Chars).
toMarkupFormula(L,existsC(Var,ReqsL),Vars,Chars):-
        toMarkupFormula(L,' existsC'(Var,writeq(ReqsL)),Vars,Chars).
toMarkupFormula(L,existsA(Var,ReqsL),Vars,Chars):-
        toMarkupFormula(L,' existsA'(Var,writeq(ReqsL)),Vars,Chars).

toMarkupFormula(L,(A * B),Vars,Chars):-!,
        toMarkupFormula(L,B,Vars,Chars2),
        toMarkupFormula(L,A,Vars,Chars1),
        sformat(Chars,'~w\n~w',[Chars2, Chars1]).

toMarkupFormula(L,formula(C),Vars,Chars):-!,
        toMarkupFormula(L,C,Vars,Chars).


toMarkupFormula(html,undefined_constants(UnDefinedList),_,O):-
        toMarkupFormula(kif,nv(UnDefinedList),_,I),
        sformat(O,'\n<font color=red>Warning Undefined constants: <font color=black size=+1>~w</font></font>',[I]).

toMarkupFormula(kif,undefined_constants(UnDefinedList),_,O):-
        toMarkupFormula(kif,(UnDefinedList),_,I),
        sformat(O,'\nWarning Undefined constants ~w',[I]).



toMarkupFormula(L,C,Vars,Chars):-is_list(C),!,make_args_out(L,C,Vars,Chars1),sformat(Chars,'(~w)',[Chars1]).

% ==================================================
% Unest And/Or
% ==================================================

toMarkupFormula(L,and(and(and(and(and(F,E),D),C),B),A),VS,Chars):-!, toMarkupFormula(L,and(F,E,D,C,B,A),VS,Chars).
toMarkupFormula(L,and(and(and(and(E,D),C),B),A),VS,Chars):-!, toMarkupFormula(L,and(E,D,C,B,A),VS,Chars).
toMarkupFormula(L,and(and(and(D,C),B),A),VS,Chars):-!, toMarkupFormula(L,and(D,C,B,A),VS,Chars).
toMarkupFormula(L,and(and(B,C),A),VS,Chars):-!, toMarkupFormula(L,and(C,B,A),VS,Chars).
toMarkupFormula(L,and(A,and(B,and(C,and(D,and(E,F))))),VS,Chars):-!, toMarkupFormula(L,'and'(A,B,C,D,E,F),VS,Chars).
toMarkupFormula(L,and(A,and(B,and(C,and(D,E)))),VS,Chars):-!, toMarkupFormula(L,'and'(A,B,C,D,E),VS,Chars).
toMarkupFormula(L,and(A,and(B,and(C,D))),VS,Chars):-!, toMarkupFormula(L,'and'(A,B,C,D),VS,Chars).
toMarkupFormula(L,and(A,and(B,C)),VS,Chars):-!, toMarkupFormula(L,'and'(A,B,C),VS,Chars).
toMarkupFormula(L,or(or(or(or(D,E),D),B),A),VS,Chars):-!, toMarkupFormula(L,or(E,D,C,B,A),VS,Chars).
toMarkupFormula(L,or(or(or(C,D),B),A),VS,Chars):-!, toMarkupFormula(L,or(D,C,B,A),VS,Chars).
toMarkupFormula(L,or(or(B,C),A),VS,Chars):-!, toMarkupFormula(L,or(C,B,A),VS,Chars).
toMarkupFormula(L,or(A,or(B,or(C,or(D,E)))),VS,Chars):-!, toMarkupFormula(L,'or'(A,B,C,D,E),VS,Chars).
toMarkupFormula(L,or(A,or(B,or(C,D))),VS,Chars):-!, toMarkupFormula(L,'or'(A,B,C,D),VS,Chars).
toMarkupFormula(L,or(A,or(B,C)),VS,Chars):-!, toMarkupFormula(L,'or'(A,B,C),VS,Chars).

% ==================================================
% Mark terms as implemented in code
% ==================================================

toMarkupFormula(html,incode(X),Vars,HAtom):-!,
        toMarkupFormula(L,bullet(X),Vars,Atom),
        sformat(HAtom,'<table border=0><tr><td><pre>~w</pre></td><td><pre>Implemented in code.</pre></td></tr></table>',[Atom]).

toMarkupFormula(kif,incode(X),Vars,HAtom):-!,
        toMarkupFormula(L,bullet(X),Vars,Atom),
        sformat(HAtom,'~w\nImplemented in code.\n',[Atom]).


toMarkupFormula(html,incode(X,M),Vars,HAtom):-!,
        toMarkupFormula(L,bullet(X),Vars,Atom),
        sformat(HAtom,'<table border=0><tr><td><pre>~w</pre></td><td><pre>Implemented in code.\n~w</pre></td></tr></table>',[Atom,M]).

toMarkupFormula(kif,incode(X,M),Vars,HAtom):-!,
        toMarkupFormula(L,bullet(X),Vars,Atom),
        sformat(HAtom,'~w\nImplemented in code.\n (~w)\n',[Atom,M]).




toMarkupFormula(L,cfind(entails(Pre,Post)),Vars,Chars):-
        mooCache(PredR,Post,Pre,T,true,Context,Explaination),
        toMarkupFormula(L,Explaination,Vars,Chars),!.

% ==================================================
% Show explaination of cross reference optimization
% ==================================================
toMarkupFormula(L,g_h(_),Vars,''):-!.
toMarkupFormula(L,tid(_),Vars,''):-!.

toMarkupFormula(L,crossref(X,Y),Vars,Atom):-!,
        crossref_to_explaination(crossref(X,Y),P),
        toMarkupFormula(L,P,Vars,Atom).

toMarkupFormula(L,crossref(X),Vars,Atom):-!,
        crossref_to_explaination(crossref(X),P),
        toMarkupFormula(L,P,Vars,Atom).


% ==========================
% Slolem  rewriting
% ==========================

toMarkupFormula(L,(X),Vars,Chars):- nonvar(X),X=..['E',Sk|ArgS],!,
        Y=..[Sk|ArgS],!,
        toMarkupFormula(L,Y,Vars,Chars).

% =====================
% remove_nonvars
% =====================

remove_nonvars(V,V):-isSlot(V),!.
remove_nonvars([],[]):-!.
remove_nonvars([V|L],LL):-isNonVar(V),!,remove_nonvars(L,LL).
remove_nonvars([V|L],[V|LL]):-remove_nonvars(L,LL).



% =====================
% Forall
% =====================

toMarkupFormula(L,forall(V,F),Vars,Chars):-not(is_list(V)),!,
        group_forall(forall(V,F),Next),!,
        cleanQuantifierConversionForWrite_forall(Next,O),
        toMarkupFormula(L,O,Vars,Chars).

cleanQuantifierConversionForWrite_forall(forall(VL,F),O):-
        remove_nonvars(VL,NL),
        ((NL=[],!,O=F);(!,O=forall(NL,F))).

toMarkupFormula(L,forall(V,F),Vars,Chars):- not(is_list(V)),!,
        toMarkupFormula(L,forall([V],F),Vars,Chars).

group_forall(forall(V1,forall(V2,forall(V3,forall(V4,forall(V5,F))))),forall([V1,V2,V3,V4,V5],F)):-!.
group_forall(forall(V1,forall(V2,forall(V3,forall(V4,F)))),forall([V1,V2,V3,V4],F)):-!.
group_forall(forall(V1,forall(V2,forall(V3,F))),forall([V1,V2,V3],F)):-!.
group_forall(forall(V1,forall(V2,F)),forall([V1,V2],F)):-!.
group_forall(forall(V1,F),forall([V1],F)):-!.

% =====================
% Exists
% =====================



toMarkupFormula(L,exists(V,F),Vars,Chars):-not(is_list(V)),!,
        group_exists(exists(V,F),Next),!,
        cleanQuantifierConversionForWrite_exists(Next,O),
        toMarkupFormula(L,O,Vars,Chars).

cleanQuantifierConversionForWrite_exists(exists(VL,F),O):-
        remove_nonvars(VL,NL),
        ((NL=[],!,O=F);(!,O=exists(NL,F))).

toMarkupFormula(L,exists(V,F),Vars,Chars):- not(is_list(V)),!,
        toMarkupFormula(L,exists([V],F),Vars,Chars).

group_exists(exists(V1,exists(V2,exists(V3,exists(V4,exists(V5,F))))),exists([V1,V2,V3,V4,V5],F)):-!.
group_exists(exists(V1,exists(V2,exists(V3,exists(V4,F)))),exists([V1,V2,V3,V4],F)):-!.
group_exists(exists(V1,exists(V2,exists(V3,F))),exists([V1,V2,V3],F)):-!.
group_exists(exists(V1,exists(V2,F)),exists([V1,V2],F)):-!.
group_exists(exists(V1,F),exists([V1],F)):-!.
% =====================
% Exists
% =====================

toMarkupFormula(L,exists(V,F),Vars,Chars):-not(is_list(V)),!,
        group_exists(exists(V,F),Next),!,
        cleanQuantifierConversionForWrite_exists(Next,O),
        toMarkupFormula(L,O,Vars,Chars).

cleanQuantifierConversionForWrite_exists(exists(VL,F),O):-
        remove_nonvars(VL,NL),
        ((NL=[],!,O=F);(!,O=exists(NL,F))).

toMarkupFormula(L,exists(V,F),Vars,Chars):- not(is_list(V)),!,
        toMarkupFormula(L,exists([V],F),Vars,Chars).

group_exists(exists(V1,exists(V2,exists(V3,exists(V4,exists(V5,F))))),exists([V1,V2,V3,V4,V5],F)):-!.
group_exists(exists(V1,exists(V2,exists(V3,exists(V4,F)))),exists([V1,V2,V3,V4],F)):-!.
group_exists(exists(V1,exists(V2,exists(V3,F))),exists([V1,V2,V3],F)):-!.
group_exists(exists(V1,exists(V2,F)),exists([V1,V2],F)):-!.
group_exists(exists(V1,F),exists([V1],F)):-!.

% ==================================================
% Finds the clausification then displays the explaination
% ==================================================
toMarkupFormula(L,map(ARGS),Vars,Chars):-
                flag(indent,X,X+1),
                indent_it_x(X,PreOut),!,
                make_args_out(L,ARGS,Vars,ArgsOut),!,
                sformat(Chars,'~w{~w}',[PreOut,ArgsOut]), !,
                flag(indent,NX,NX-1).

toMarkupFormula(L,list(ARGS),Vars,Chars):-
                flag(indent,X,X+1),
                indent_it_x(X,PreOut),!,
                make_args_out(L,ARGS,Vars,ArgsOut),!,
                sformat(Chars,'~w[~w]',[PreOut,ArgsOut]), !,
                flag(indent,NX,NX-1).


% ===================================================
% Pretty Print Formula
% ===================================================

toMarkupFormula(L,C,Vars,Chars):-
                C=..[Pred|ARGS],!,
                flag(indent,X,X+1),
                indent_it_x(X,PreOut),!,
                toMarkup(L,Pred,Vars,PredOut),!,
                make_args_out(L,ARGS,Vars,ArgsOut),!,
                sformat(Chars,'~w(~w ~w)',[PreOut,PredOut,ArgsOut]), !,
                flag(indent,NX,NX-1).

% ===================================================
% Make Args
% ===================================================

make_args_out(bach,[],Vars,''):-!.
make_args_out(bach,[C],Vars,ArgsOut):-
	 flag(indent,X,X),indent_it_x(X,PreOut),!,
	 toMarkupFormula(bach,C,Vars,Chars1),
	 sformat(ArgsOut,'~w~w',[PreOut,Chars1]).

make_args_out(bach,[C|GS],Vars,ArgsOut):-
	 toMarkupFormula(bach,C,Vars,Chars1),
	 make_args_out(bach,GS,Vars,Chars2),!,
	 flag(indent,X,X),indent_it_x(X,PreOut),!,
	 sformat(ArgsOut,'~w~w,~w',[PreOut,Chars1,Chars2]).


make_args_out(L,[],Vars,''):-!.
make_args_out(L,[C],Vars,ArgsOut):-
                toMarkupFormula(L,C,Vars,ArgsOut).
make_args_out(L,[C|GS],Vars,ArgsOut):-
                toMarkupFormula(L,C,Vars,Chars1),
                make_args_out(L,GS,Vars,Chars2),!,
                sformat(ArgsOut,'~w ~w',[Chars1,Chars2]).

indent_it_x(0,''):-!.
indent_it_x(1,'\n         '):-!.
indent_it_x(X,Chars):-XX is X -1,!, indent_it_x(XX,OutP),!,sformat(Chars,'~w   ',[OutP]),!.


