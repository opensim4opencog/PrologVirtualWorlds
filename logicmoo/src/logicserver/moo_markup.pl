% ===================================================================
% File 'moo_markup.pl'
% Maintainers: Douglas Miles
% Contact: dmiles@users.sourceforge.net ;
% Version: 'moo_markup.pl' 1.0.0
% Revised At:  $Date: 2002-03-14 12:46:24 $

% ===================================================================
% Major functions:
% This file meets the needs of an external agent working for the needs of eigther an automated or human user
% Interface with Java with XML to display explaination trees and variable bindings
% ===================================================================

:-include('moo_header.pl').
:-index(getMarkupFormula(1,1,1,1)).
:-index(toMarkUp_lang(1,1,1,1)).

%Tests

%stest3 :- toMarkUp(html, explaination(('Military':996:subclass('IntransitiveRelation', 'BinaryRelation')^B)* ('Military':836:subclass('BinaryRelation', 'Relation')^C)*forall('IntransitiveRelation', forall(D, forall('Relation', holds(subclass, 'IntransitiveRelation', D)and holds(subclass, D, 'Relation')=>holds(subclass, 'IntransitiveRelation', 'Relation'))))*sfind(instance(subclass, 'PartialOrderingRelation'))*sfind(subclass('PartialOrderingRelation', 'TransitiveRelation'))* ('Military':2756:instance(on, 'IntransitiveRelation')^E)), ['X'=on|A],O),write_ln(O).

% ================================================================
%   Transform Signals to Objects
% ================================================================

% ===================================================================
% writeMarkup(-Prolog)
%
% Replaces writeq in some cases
% ===================================================================
writeMarkup(Term):-term_to_leml(Term,Chars),write(Chars).


% ===================================================================
% toMarkUp_lang(-Markup,-Prolog,-PrologVarableList, +Output)
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
term_to_leml(Term,Chars):-toMarkUp(html,Term,_,Chars),!.


toMarkUp(chat,Var,VS,Chars):-!,catch(toMarkUp(kif,Var,VS,Chars),_,true),!.
toMarkUp(java,Var,VS,Chars):-!,catch(toMarkUp(html,Var,VS,Chars),_,true),!.

toMarkUp(L,T,V,Chars):-!,
        ignore(catch(/**/((
        copy_term((T,V),(CT,CV)),
        numbervars((CT,CV),'$VAR',0,_),%true,
        toMarkUp_lang(L,CT,CV,Chars))),_,true)),!.

% VARIABLES
toMarkUp_lang(L,C,Vars,Out):-isSlot(C),!,toMarkUp_slotValue(L,C,Vars,Out).

toMarkUp_lang(html,'$spacer',Vars,'\n<hr>\n').
toMarkUp_lang(_,'$spacer',Vars,'\n;; ------------------------------------------------------------------------------\n\n').

tml(Form):-toMarkUp_lang(html,formula(Form),Vars,Out),write(Out),nl.

toMarkUp_lang(L,formula(C),Vars,Out):-!,
        getMarkupFormula(L,C,Vars,Out).

% ===================================================
% Pretty Print Formula
% ===================================================
%getMarkupFormula(L,C,Vars,Out):-       writeq( C=Vars),nl,fail.


getMarkupFormula(L,C,Vars,Out):-isSlot(C),!,toMarkUp_lang(L,C,Vars,Out).
getMarkupFormula(L,C,Vars,Out):-not(compound(C)),!,toMarkUp_lang(L,C,Vars,Out).

% QUOTED STRING FORMAT
getMarkupFormula(L,Atom,_VS,Chars):-((isCharCodelist(Atom);string(Atom))),!,
        catch(sformat(Chars,'"~s"',[Atom]),_,sformat(Chars,'"~w"',[Atom])).

getMarkupFormula(L,string(C),Vars,C):-!.

getMarkupFormula(L,hidden(F,Args),Vars,''):-!.

getMarkupFormula(html,colourize(Color,Thing),Vars,Chars):-!,
        getMarkupFormula(html,Thing,Vars,Chars1),!,
        sformat(Chars,'<font color="~w">~w</font>\n',[Color,Chars1]).

getMarkupFormula(L,colourize(Color,Thing),Vars,Chars):-!,
        getMarkupFormula(L,Thing,Vars,Chars),!.

/*
getMarkupFormula(L,','(A,B),Vars,Chars):-!,
        prolog_to_krlog(','(A,B),KR),
        getMarkupFormula(L,KR,Vars,Chars),!.
*/


getMarkupFormula(L,write_dollar('$v',[A|Args]),Vars,Chars):-!,
                Flag=..[getPrologVars,A|Args],!,
                getMarkupFormula(L,Flag,Vars,Chars).

getMarkupFormula(L,table_(Goal,Lits),Vars,Chars):-!,
                getMarkupFormula(L,table_p(Lits,Goal),Vars,Chars).


getMarkupFormula(L,write_dollar(F,[A|Args]),Vars,Chars):-!,
        getMarkupFormula(L,A,Vars,Chars1),
        getMarkupFormula(L,hidden(F,Args),Vars,Chars2),!,
        sformat(Chars,'~w~w',[Chars1,Chars2]).

getMarkupFormula(L,'$existential'(VarName,Name,Literal),Vars,O):-!,
        getMarkupFormula(L,'existential'(VarName),Vars,O).

getMarkupFormula(L,'$eval'(Function),Vars,O):-!,
        getMarkupFormula(L,' eval'(Function),Vars,O).


getMarkupFormula(L,functional(VarName,Domains,Literal),Vars,O):-
        toMarkUp_lang(L,Literal,Vars,O),!.

close_list_var(M,[]):-isSlot(M),!.
close_list_var([[M]|Ms],[M|Ls]):-!,
        close_list_var(Ms,Ls).
close_list_var([M|Ms],[M|Ls]):-!,
        close_list_var(Ms,Ls).

getMarkupFormula(L,Term,Vars,Chars):-
        Term=..[F,A|Args],
        atom_concat('$',_,F), !,
        getMarkupFormula(L,write_dollar(F,[A|Args]),Vars,Chars).


getMarkupFormula(L,unused(C,P),Vars,O):-!,
        getMarkupFormula(L,notused(C,writeq(P)),Vars,O).

getMarkupFormula(L,ff([]),Vars,'[]'):-!.

getMarkupFormula(L,ff([Flag|Flags]),Vars,Chars):-!,
        getMarkupFormula(L,flag(Flag),Vars,Chars1),
        getMarkupFormula(L,ff(Flags),Vars,Chars2),
        sformat(Chars,'~w, ~w',[Chars1, Chars2]).

getMarkupFormula(L,domargs([]),Vars,''):-!.

getMarkupFormula(L,domargs([(P:N)]),Vars,Chars):-!,
        getMarkupFormula(L,P,Vars,Chars1),
        sformat(Chars,'~w:~w',[Chars1,N]).

getMarkupFormula(L,domargs([(P:N)|Flags]),Vars,Chars):-!,
        getMarkupFormula(L,P,Vars,Chars1),
        getMarkupFormula(L,domargs(Flags),Vars,Chars2),
        sformat(Chars,'~s:~w,~w',[Chars1,N,Chars2]).

getMarkupFormula(L,flag(Flag),Vars,Chars):-
        Flag=..[domainV,Var,DomArgs],!,
        getMarkupFormula(L,Var,Vars,VarChars),
        getMarkupFormula(L,domargs(DomArgs),Vars,ArgChars),
        sformat(Chars,'~w(~w,[~w])',[domainV,VarChars,ArgChars]).

getMarkupFormula(L,flag(Flag),Vars,Chars):-
        Flag=..[Name,Var,Args],!,
        getMarkupFormula(L,Var,Vars,VarChars),
        sformat(Chars,'~w(~w, ~q)',[Name,VarChars,Args]).
getMarkupFormula(L,flag(Flag),Vars,Chars):-!,
        getMarkupFormula(L,writeq(Flag),Vars,Chars).







getMarkupFormula(L,writeq(Atom),_VS,Chars):-!,sformat(Chars,'~q',[Atom]).

getMarkupFormula(L,[],Vars,''):-!.
%getMarkupFormula(L,[A | B],Vars,Chars):-proper_list([A | B]),append(['('|[A | B],[')'],TRY),toMarkUp_list(L,[Su|Bj],Vars,Chars).
%getMarkupFormula(L,[A | B],Vars,Chars):-catch(TRY=..['',A | B],_,fail),getMarkupFormula(L,TRY,Varsr,Chars),!.
%getMarkupFormula(L,[A | B],Vars,Chars):-catch(TRY=..[A | B],_,fail),getMarkupFormula(L,TRY,Vars,Chars),!.
%getMarkupFormula(L,[A | B],Vars,Chars):-catch(TRY=..[A | B],_,fail),getMarkupFormula(L,TRY,Vars,Chars),!.
getMarkupFormula(L,[Su|Bj],Vars,Chars):-
        toMarkUp_list(L,[Su|Bj],Vars,Chars1),
        sformat(Chars,'(~w)',[Chars1]).

/*
getMarkupFormula(L,Term,Vars,O):-
        Term=..[holds,F|Args],isNonVar(F),not_a_function(F),!,
        NTerm=..[F|Args],
        getMarkupFormula(L,NTerm,Vars,O).
*/
getMarkupFormula(L,'$VAR'(_)* X ,Vars,Out):-!,getMarkupFormula(L, X ,Vars,Out).
getMarkupFormula(L, X * '$VAR'(_) ,Vars,Out):-!,getMarkupFormula(L, X ,Vars,Out).
getMarkupFormula(L,(A * []),Vars,Out):-!,getMarkupFormula(L,A ,Vars,Out).
getMarkupFormula(L,([] * A),Vars,Out):-!,getMarkupFormula(L,A ,Vars,Out).
getMarkupFormula(L,deduced* X ,Vars,Out):-!,getMarkupFormula(L, X ,Vars,Out).
getMarkupFormula(L, X * deduced ,Vars,Out):-!,getMarkupFormula(L, X ,Vars,Out).


getMarkupFormula(L,domainV(Var,ReqsL),Vars,Chars):-
        getMarkupFormula(L,' domainV'(Var,writeq(ReqsL)),Vars,Chars).
getMarkupFormula(L,domainC(Var,ReqsL),Vars,Chars):-
        getMarkupFormula(L,' domainC'(Var,writeq(ReqsL)),Vars,Chars).
getMarkupFormula(L,domainA(Var,ReqsL),Vars,Chars):-
        getMarkupFormula(L,' domainA'(Var,writeq(ReqsL)),Vars,Chars).
getMarkupFormula(L,existsC(Var,ReqsL),Vars,Chars):-
        getMarkupFormula(L,' existsC'(Var,writeq(ReqsL)),Vars,Chars).
getMarkupFormula(L,existsA(Var,ReqsL),Vars,Chars):-
        getMarkupFormula(L,' existsA'(Var,writeq(ReqsL)),Vars,Chars).

getMarkupFormula(L,(A * B),Vars,Chars):-!,
        getMarkupFormula(L,B,Vars,Chars2),
        getMarkupFormula(L,A,Vars,Chars1),
        sformat(Chars,'~w\n~w',[Chars2, Chars1]).

getMarkupFormula(L,formula(C),Vars,Out):-!,
        getMarkupFormula(L,C,Vars,Out).


getMarkupFormula(html,undefined_constants(UnDefinedList),_,O):-
        getMarkupFormula(kif,nv(UnDefinedList),_,I),
        sformat(O,'\n<font color=red>Warning Undefined constants: <font color=black size=+1>~w</font></font>',[I]).

getMarkupFormula(kif,undefined_constants(UnDefinedList),_,O):-
        getMarkupFormula(kif,(UnDefinedList),_,I),
        sformat(O,'\Warning Undefined constants ~w',[I]).



getMarkupFormula(L,C,Vars,Out):-is_list(C),!,make_args_out(L,C,Vars,Out1),sformat(Out,'(~w)',[Out1]).
%getMarkupFormula(L,C,Vars,Out):-not(compound(C)),!,toMarkUp_lang(L,C,Vars,Out).

/*
getMarkupFormula(L,and(A,B),VS,Chars):-
        collect_op(and(A,B),O),!,
        getMarkupFormula(L,O,VS,Chars).

collect_op(and(A,B),and(A,B)):-not(A=and(_,_)),not(B=and(_,_)).
collect_op(and(A,B
*/

% ==================================================
% Unest And/Or
% ==================================================

getMarkupFormula(L,and(and(and(and(and(F,E),D),C),B),A),VS,Chars):-!, getMarkupFormula(L,and(F,E,D,C,B,A),VS,Chars).
getMarkupFormula(L,and(and(and(and(E,D),C),B),A),VS,Chars):-!, getMarkupFormula(L,and(E,D,C,B,A),VS,Chars).
getMarkupFormula(L,and(and(and(D,C),B),A),VS,Chars):-!, getMarkupFormula(L,and(D,C,B,A),VS,Chars).
getMarkupFormula(L,and(and(B,C),A),VS,Chars):-!, getMarkupFormula(L,and(C,B,A),VS,Chars).
getMarkupFormula(L,and(A,and(B,and(C,and(D,and(E,F))))),VS,Chars):-!, getMarkupFormula(L,'and'(A,B,C,D,E,F),VS,Chars).
getMarkupFormula(L,and(A,and(B,and(C,and(D,E)))),VS,Chars):-!, getMarkupFormula(L,'and'(A,B,C,D,E),VS,Chars).
getMarkupFormula(L,and(A,and(B,and(C,D))),VS,Chars):-!, getMarkupFormula(L,'and'(A,B,C,D),VS,Chars).
getMarkupFormula(L,and(A,and(B,C)),VS,Chars):-!, getMarkupFormula(L,'and'(A,B,C),VS,Chars).
getMarkupFormula(L,or(or(or(or(D,E),D),B),A),VS,Chars):-!, getMarkupFormula(L,or(E,D,C,B,A),VS,Chars).
getMarkupFormula(L,or(or(or(C,D),B),A),VS,Chars):-!, getMarkupFormula(L,or(D,C,B,A),VS,Chars).
getMarkupFormula(L,or(or(B,C),A),VS,Chars):-!, getMarkupFormula(L,or(C,B,A),VS,Chars).
getMarkupFormula(L,or(A,or(B,or(C,or(D,E)))),VS,Chars):-!, getMarkupFormula(L,'or'(A,B,C,D,E),VS,Chars).
getMarkupFormula(L,or(A,or(B,or(C,D))),VS,Chars):-!, getMarkupFormula(L,'or'(A,B,C,D),VS,Chars).
getMarkupFormula(L,or(A,or(B,C)),VS,Chars):-!, getMarkupFormula(L,'or'(A,B,C),VS,Chars).

% ==================================================
% Mark terms as implemented in code
% ==================================================

getMarkupFormula(html,incode(X),Vars,HAtom):-!,
        getMarkupFormula(L,bullet(X),Vars,Atom),
        sformat(HAtom,'<table border=0><tr><td><pre>~w</pre></td><td><pre>Implemented in code.</pre></td></tr></table>',[Atom]).

getMarkupFormula(kif,incode(X),Vars,HAtom):-!,
        getMarkupFormula(L,bullet(X),Vars,Atom),
        sformat(HAtom,'~w\nImplemented in code.\n',[Atom]).


getMarkupFormula(html,incode(X,M),Vars,HAtom):-!,
        getMarkupFormula(L,bullet(X),Vars,Atom),
        sformat(HAtom,'<table border=0><tr><td><pre>~w</pre></td><td><pre>Implemented in code.\n~w</pre></td></tr></table>',[Atom,M]).

getMarkupFormula(kif,incode(X,M),Vars,HAtom):-!,
        getMarkupFormula(L,bullet(X),Vars,Atom),
        sformat(HAtom,'~w\nImplemented in code.\n (~w)\n',[Atom,M]).

% ==================================================
% Finds the clausification then displays the explaination
% ==================================================

getMarkupFormula(L,cfind(entails(Pre,Post)),Vars,Out):-
        mooCache(PredR,Post,Pre,T,true,Context,Explaination),
        getMarkupFormula(L,Explaination,Vars,Out),!.

% ==================================================
% Show explaination of cross reference optimization
% ==================================================
getMarkupFormula(L,g_h(_),Vars,''):-!.
getMarkupFormula(L,tid(_),Vars,''):-!.

getMarkupFormula(L,crossref(X,Y),Vars,Atom):-!,
        crossref_to_explaination(crossref(X,Y),P),
        getMarkupFormula(L,P,Vars,Atom).

getMarkupFormula(L,crossref(X),Vars,Atom):-!,
        crossref_to_explaination(crossref(X),P),
        getMarkupFormula(L,P,Vars,Atom).


% ==================================================
% Surface Find
% ==================================================

getMarkupFormula(L,sfind(X),Vars,Out):- nonvar(X),
        mooCache(PredR, surface, X,V,Context, Ctx, TN, Auth, State),!,
        var_merge(Vars,V,TVars),!,
        getMarkupFormula(L,surf(Context,TN),TVars,Out).

% ==================================================
% Find a surface form, Display its explaination, show instanced version
% ==================================================

getMarkupFormula(L,sfindi(X),Vars,Out):- nonvar(X),
        mooCache(PredR, surface, X,V,Context, Ctx, TN, Auth, State),!,
        var_merge(Vars,V,TVars),!,
        getMarkupFormula(L,surf(Context,TN) * bullet_a(X),TVars,Out).

getMarkupFormula(L,sfindi(X),Vars,Out):- nonvar(X),
        getMarkupFormula(L,bullet_a(X),Vars,Out).


% ==================================================
% VIA
% ==================================================

getMarkupFormula(L,via(Form,V),Vars,Out):-
        (var_merge(Vars,V,TVars)),
        getMarkupFormula(L,via(Form),TVars,Out),!.


getMarkupFormula(L,via(entails(Pre,(Post))),Vars,Out):-not(isMooOption(show_entails,true)),!,
        getMarkupFormula(L,(  via('=>'(Pre,Post)) ),Vars,Out).


getMarkupFormula(L,'-'(Form),Vars,Out):-
        getMarkupFormula(L,not(Form),Vars,Out).

getMarkupFormula(L,'+'(Form),Vars,Out):-
        getMarkupFormula(L,(Form),Vars,Out).

getMarkupFormula(L,via(Form),Vars,Out):-
        getMarkupFormula(L,bullet_a(Form),Vars,Out).


getMarkupFormula(L,(entails(CList,UConsq,false)),Vars,Out):-!,
        getMarkupFormula(L,entails(CList,not(UConsq)),Vars,Out).

getMarkupFormula(L,(entails(CList,UConsq,true)),Vars,Out):-!,
        getMarkupFormula(L,entails(CList,(UConsq)),Vars,Out).

getMarkupFormula(L,(entails(true,(Post))),Vars,Out):-!,
        getMarkupFormula(L,(Post),Vars,Out).

% ==================================================
% nv(_) Print list as non-vecorted
% ==================================================

getMarkupFormula(L,nv(Subj),Vars,Chars):-!,toMarkUp_list(L,Subj,Vars,Chars).

% ==========================
% Maintainerial writing
% ==========================

getMarkupFormula(L,surf(Context,TN),Vars,Atom):-
        mooCache(PredR,surface, OForm, OVars,Context,TN,_, _),!,
        getMarkupFormula(L,OForm,OVars,Orig),
        flag(explaination_linenumber,LN,LN+1),
        getMarkupFormula(L,bullet(Context,TN,LN,Orig),Vars,Atom).
getMarkupFormula(L,surf(Context,TN),Vars,Atom):-!,
        getMarkupFormula(L,bullet('assertion lookup failure'(Context,TN)),Vars,Atom).

% ==========================
% Bullet writing
% ==========================

getMarkupFormula(L,bullet_a(X),Vars,S):-
        flag(indent,_,0),
        getMarkupFormula(L,X,Vars,SStatement),
        flag(explaination_linenumber,LN,LN),
        LNB is LN-1,
        sformat(S,'~wa. ~w\n',[LNB,SStatement]).

getMarkupFormula(L,bullet(X),Vars,Atom):-!,
        flag(explaination_linenumber,LN,LN+1),
        getMarkupFormula(L,X,Vars,Orig),
        getMarkupFormula(L,bullet('Kernel','GlobalContext',9100000,LN,Orig),Vars,Atom).

getMarkupFormula(html,bullet(Context,TN,LN,Orig),Vars,Atom):-!,%true,
        flag(indent,_,0),
        (catch((TN < 100000),_,fail) ->
                sformat(Atom,'~w <A href="stheory.jsp?req=SA&stheory=~w&id=~w" title="~w ~w ~w" ><img border=0 src="bullet.gif"/></A> ~w',[LN,Context,TN,TN,Context,Orig]);
                sformat(Atom,'~w <img border=0 src="bullet.gif" title="Not added to browser ~w (~w)"> ~w',[LN,Context,Orig])),!.

getMarkupFormula(kif,bullet(Context,TN,LN,Orig),Vars,Atom):-!,
        flag(indent,_,0),
%       getMarkupFormula(kif,asserted(Ctx,Orig),Vars,F),
        getMarkupFormula(kif,Orig,Vars,F),
        sformat(Atom,'~w. ~w',[LN,F]).

% ==========================
% Slolem  rewriting
% ==========================

getMarkupFormula(L,(X),Vars,Out):- nonvar(X),X=..['E',Sk|ArgS],!,
        Y=..[Sk|ArgS],!,
        getMarkupFormula(L,Y,Vars,Out).

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

getMarkupFormula(L,forall(V,F),Vars,Chars):-not(is_list(V)),!,
        group_forall(forall(V,F),Next),!,
        cleanQuantifierConversionForWrite_forall(Next,O),
        getMarkupFormula(L,O,Vars,Chars).

cleanQuantifierConversionForWrite_forall(forall(VL,F),O):-
        remove_nonvars(VL,NL),
        ((NL=[],!,O=F);(!,O=forall(NL,F))).

getMarkupFormula(L,forall(V,F),Vars,Chars):- not(is_list(V)),!,
        getMarkupFormula(L,forall([V],F),Vars,Chars).

group_forall(forall(V1,forall(V2,forall(V3,forall(V4,forall(V5,F))))),forall([V1,V2,V3,V4,V5],F)):-!.
group_forall(forall(V1,forall(V2,forall(V3,forall(V4,F)))),forall([V1,V2,V3,V4],F)):-!.
group_forall(forall(V1,forall(V2,forall(V3,F))),forall([V1,V2,V3],F)):-!.
group_forall(forall(V1,forall(V2,F)),forall([V1,V2],F)):-!.
group_forall(forall(V1,F),forall([V1],F)):-!.

% =====================
% Exists
% =====================



getMarkupFormula(L,exists(V,F),Vars,Chars):-not(is_list(V)),!,
        group_exists(exists(V,F),Next),!,
        cleanQuantifierConversionForWrite_exists(Next,O),
        getMarkupFormula(L,O,Vars,Chars).

cleanQuantifierConversionForWrite_exists(exists(VL,F),O):-
        remove_nonvars(VL,NL),
        ((NL=[],!,O=F);(!,O=exists(NL,F))).

getMarkupFormula(L,exists(V,F),Vars,Chars):- not(is_list(V)),!,
        getMarkupFormula(L,exists([V],F),Vars,Chars).

group_exists(exists(V1,exists(V2,exists(V3,exists(V4,exists(V5,F))))),exists([V1,V2,V3,V4,V5],F)):-!.
group_exists(exists(V1,exists(V2,exists(V3,exists(V4,F)))),exists([V1,V2,V3,V4],F)):-!.
group_exists(exists(V1,exists(V2,exists(V3,F))),exists([V1,V2,V3],F)):-!.
group_exists(exists(V1,exists(V2,F)),exists([V1,V2],F)):-!.
group_exists(exists(V1,F),exists([V1],F)):-!.
% =====================
% Exists
% =====================

getMarkupFormula(L,exists(V,F),Vars,Chars):-not(is_list(V)),!,
        group_exists(exists(V,F),Next),!,
        cleanQuantifierConversionForWrite_exists(Next,O),
        getMarkupFormula(L,O,Vars,Chars).

cleanQuantifierConversionForWrite_exists(exists(VL,F),O):-
        remove_nonvars(VL,NL),
        ((NL=[],!,O=F);(!,O=exists(NL,F))).

getMarkupFormula(L,exists(V,F),Vars,Chars):- not(is_list(V)),!,
        getMarkupFormula(L,exists([V],F),Vars,Chars).

group_exists(exists(V1,exists(V2,exists(V3,exists(V4,exists(V5,F))))),exists([V1,V2,V3,V4,V5],F)):-!.
group_exists(exists(V1,exists(V2,exists(V3,exists(V4,F)))),exists([V1,V2,V3,V4],F)):-!.
group_exists(exists(V1,exists(V2,exists(V3,F))),exists([V1,V2,V3],F)):-!.
group_exists(exists(V1,exists(V2,F)),exists([V1,V2],F)):-!.
group_exists(exists(V1,F),exists([V1],F)):-!.

% =====================
% Findall
% =====================
        /*
getMarkupFormula(L,findall(V,F),Vars,Chars):-not(is_list(V)),!,
        group_findall(findall(V,F),Next),!,
        cleanQuantifierConversionForWrite_findall(Next,O),
        getMarkupFormula(L,O,Vars,Chars).

cleanQuantifierConversionForWrite_findall(findall(VL,F),O):-
        remove_nonvars(VL,NL),
        ((NL=[],!,O=F);(!,O=findall(NL,F))).

getMarkupFormula(L,findall(V,F),Vars,Chars):- not(is_list(V)),!,
        getMarkupFormula(L,findall([V],F),Vars,Chars).

group_findall(findall(V1,findall(V2,findall(V3,findall(V4,findall(V5,F))))),findall([V1,V2,V3,V4,V5],F)):-!.
group_findall(findall(V1,findall(V2,findall(V3,findall(V4,F)))),findall([V1,V2,V3,V4],F)):-!.
group_findall(findall(V1,findall(V2,findall(V3,F))),findall([V1,V2,V3],F)):-!.
group_findall(findall(V1,findall(V2,F)),findall([V1,V2],F)):-!.
group_findall(findall(V1,F),findall([V1],F)):-!.
                                 */
% =====================
% Indentation
% =====================

getMarkupFormula(L,C,Vars,Out):-
                C=..[Pred|ARGS],!,
                flag(indent,X,X+1),
                indent_it_x(X,PreOut),!,
                toMarkUp_lang(L,Pred,Vars,PredOut),!,
                make_args_out(L,ARGS,Vars,ArgsOut),!,
                sformat(Out,'~w(~w ~w)',[PreOut,PredOut,ArgsOut]), !,
                flag(indent,NX,NX-1).

make_args_out(L,[],Vars,''):-!.
make_args_out(L,[C],Vars,ArgsOut):-
                getMarkupFormula(L,C,Vars,ArgsOut).
make_args_out(L,[C|GS],Vars,ArgsOut):-
                getMarkupFormula(L,C,Vars,Out1),
                make_args_out(L,GS,Vars,Out2),!,
                sformat(ArgsOut,'~w ~w',[Out1,Out2]).

indent_it_x(0,''):-!.
indent_it_x(1,'\n         '):-!.
indent_it_x(X,Out):-XX is X -1,!, indent_it_x(XX,OutP),!,sformat(Out,'~w   ',[OutP]),!.

% =====================
% Prolog Tr./ansformation
% =====================

toMarkUp_lang(L,':-'(C,true),Vars,Out):-prolog_to_krlog(C,KR),!,toMarkUp_lang(L,KR,Vars,Out).
toMarkUp_lang(L,':-'(C,A),Vars,Out):-prolog_to_krlog(C,KRC),prolog_to_krlog(A,KRA),!,toMarkUp_lang(L,'=>'(KRA,KRC),Vars,Out).

toMarkUp_lang(L,(T^V),Vars,Out):-var_merge(Vars,V,TVars),!,toMarkUp_lang(L,T,TVars,Out).

%Terminal Control
toMarkUp_lang(html,lparen,Vars,'('):-!.
toMarkUp_lang(html,rparen,Vars,')'):-!.
toMarkUp_lang(kif,lparen,Vars,'('):-!.
toMarkUp_lang(kif,rparen,Vars,')'):-!.
toMarkUp_lang(html,nl,Vars,'<br>'):-!.
toMarkUp_lang(html,tab,Vars,'<li>'):-!.
toMarkUp_lang(kif,nl,Vars,'\n'):-!.
toMarkUp_lang(kif,tab,Vars,'\t'):-!.

% No parens (nv = no vector)
toMarkUp_lang(L,nv(Subj),Vars,Chars):-is_list(Subj),!,toMarkUp_list(L,Subj,Vars,Chars).
toMarkUp_lang(L,nv(Subj),Vars,Chars):-!,toMarkUp_lang(L,Subj,Vars,Chars).

toMarkUp_lang(_,writeFmt(F,A),Vars,Out):-sformat(Out,F,A),!.
toMarkUp_lang(_,surf,Vars,''):-!.
toMarkUp_lang(_,end_of_file,Vars,''):-!.

toMarkUp_lang(_,',',Vars,'and'):-!.
toMarkUp_lang(_,';',Vars,'or'):-!.
toMarkUp_lang(_,'=',Vars,'equal'):-!.
toMarkUp_lang(_,'deduced',Vars,' ').

% QUOTED STRING FORMAT
toMarkUp_lang(kif,Atom,_VS,Chars):-isCharCodelist(Atom),!,
        catch(sformat(Chars,'"~s"',[Atom]),_,sformat(Chars,'"~w"',[Atom])).
toMarkUp_lang(kif,Atom,_VS,Chars):-string(Atom),!,
        catch(sformat(Chars,'"~s"',[Atom]),_,sformat(Chars,'"~w"',[Atom])).


%LISTS
%toMarkUp_lang(LANG,[COMP],Vars,Atom)

toMarkUp_lang(L,[],Vars,Atom):-toMarkUp_lang(L,'NullSet',Vars,Atom).
%toMarkUp_lang(html,[Su|Bj],Vars,Chars):-toMarkUp_list(html,[Su|Bj],Vars,Chars1),sformat(Chars,'<div>(<ul>~w </ul>)</div>',[Chars1]).
toMarkUp_lang(kif,[Su|Bj],Vars,Chars):-toMarkUp_list(kif,[Su|Bj],Vars,Chars1),sformat(Chars,'(~w)',[Chars1]).


close_varlist([]):-!.
close_varlist('$VAR'(_)):-!.
close_varlist([V|VV]):-close_varlist(VV),!.

% SPECIAL FORMATS

toMarkUp_lang(_,writeq(Term),Vars,Atom):-!,sformat(Atom,'~q',[Term]).
toMarkUp_lang(kif,maillink(Title,Address,Subject),Vars,Address):-!.
toMarkUp_lang(kif,weblink(Title,URL),Vars,Title):-!.
toMarkUp_lang(kif,helplink(Title,URL),Vars,Title):-!.
toMarkUp_lang(L,explaination(PB),Vars,Atom):-
        flag(explaination_linenumber,_,1),
        getMarkupFormula(L,PB,Vars,AtomS),!,
        sformat(Atom,'\nExplaination:\n~w\n',[AtomS]).

toMarkUp_lang(LANG,krlog(COMP),Vars,Atom):-!,prolog_to_krlog(COMP,KR),toMarkUp_lang(LANG,KR,Vars,Atom).

toMarkUp_lang(LANG,kif(COMP),Vars,Atom):-!,toMarkUp_lang(kif,COMP,Vars,Atom).
toMarkUp_lang(LANG,html(COMP),Vars,Atom):-!,toMarkUp_lang(html,COMP,Vars,Atom).

toMarkUp_lang(html,select(Name,OptionList),Vars,Out):-toMarkUp_lang(html,options(OptionList),Vars,Options),sformat(Out,'<select sort name="~w" id="~w" size="1">~w</select>',[Name,Name,Options]).
toMarkUp_lang(html,chectheoryox(Name,on),Vars,Out):-
                sformat(Out,'<input type=chectheoryox name="~w" id="~w" checked>',[Name,Name]),!.
toMarkUp_lang(html,chectheoryox(Name,_),Vars,Out):-
                sformat(Out,'<input type=chectheoryox name="~w" id="~w">',[Name,Name]),!.
toMarkUp_lang(html,options([]),Vars,'').

toMarkUp_lang(L,getPrologVars(Form),Vars,Chars):-markUpVARLIST(L,Form,Vars,SChars),sformat(Chars,'~w',[SChars]),!.

toMarkUp_lang(L,getPrologVars(Form),Vars,Chars):-!,sformat(Chars,'; var_post_err (~q). ',[Form]).


toMarkUp_lang(html,qresult(Res),Vars,Chars):-!,sformat(Chars,'Result ',[Res]).

toMarkUp_lang(kif,qresult(Res),Vars,''):-!. %,sformat(Chars,'res="~w"\n',[Res]).

% Back into Standard Terms

format_o(Format,Stuff):-
        toMarkUp_lang(html,Stuff,_,Out),writeFmt(Format,[Out]).


toMarkUp_lang(html,options([Option|List]),Vars,Out):-
               toMarkUp_lang(html,option(Option),Vars,Out2),
               toMarkUp_lang(html,options(List),Vars,Out3),
               atom_concat(Out2,Out3,Out).

toMarkUp_lang(html,option(Option),Vars,Out):-sformat(Out,'<option value="~w">~w</option>',[Option,Option]).

% Numbers
toMarkUp_lang(_,Atom,_VS,Chars):-number(Atom),!,sformat(Chars,'~w',[Atom]).

toMarkUp_lang(L,Value,Vars,Chars):-
        mooCache(PredR, skolem, Value = x(Name,Expression),SKVARS,Context, Ctx, TN, Auth, State),!,
            toMarkUp_lang(kif,Name,Vars,NameQ),  prependQuestionMark(NameQ,NameQM),
            subst(x(Sk,Expression),Sk,NameQM,x(NSk,NExpression)),!,
            toMarkUp_lang(L,exists([NSk],NExpression),SKVARS,Chars).

% all other Formulas get intercepted here
toMarkUp_lang(L,Term,Vars,Chars):-compound(Term),!,
        getMarkupFormula(L,Term,Vars,Chars),!.

% PRETTYNESS
toMarkUp_lang(_,';',Vars,'or ').
toMarkUp_lang(_,',',Vars,'and ').
toMarkUp_lang(_,'neg',Vars,'neg ').
%toMarkUp_lang(_,entails,Vars,'modus-ponens ').
%toMarkUp_lang(_,entails,Vars,'modus-tollens ').



% Not compound - TEXT
toMarkUp_lang(html,Atom,Vars,Chars):-
        atom_codes(Atom,[115,107|_]),!,
                atom_lookup_theory_ctx(html,Atom,Context,Result,ID,Color,Page),!,
                (Result=ml(This) -> toMarkUp_lang(html,This,Vars,SResult) ; SResult=Result),
                (Context=none ->
                        sformat(Chars,'<font color=~w>~w</font>',[Color,SResult]);
                        sformat(Chars,'<A HREF="~w.jsp?logicforms=logicforms&submit=All%20Forms&data=~w&theory=~w">~w</A>',[Page,ID,Context,SResult])
                ).

toMarkUp_lang(html,Atom,Vars,Chars):-
                atom_lookup_theory_ctx(html,Atom,Context,Result,ID,Color,Page),!,
                (Result=ml(This) -> toMarkUp_lang(html,This,Vars,SResult) ; SResult=Result),
                (Context=none ->
                        sformat(Chars,'<font color=~w>~w</font>',[Color,SResult]);
                        sformat(Chars,'<A HREF="~w.jsp?req=SC&term=~w&stheory=~w">~w</A>',[Page,ID,Context,SResult])
                ).

toMarkUp_lang(kif,Atom,Vars,Chars):-
                atom_lookup_theory_ctx(kif,Atom,Context,Result,ID,Color,Page),!,
                (Result=ml(This) -> toMarkUp_lang(html,This,Vars,SResult) ; SResult=Result),
                        sformat(Chars,'~w',[SResult]).

% Lookup Proc
atom_lookup_theory_ctx(kif,Atom,none,none,Atom,Atom,black,stheory):-!.

atom_lookup_theory_ctx(_,Atom,Context,'GlobalContext',Atom,Atom,purple,skolems):-
        hlPredicateAttribute(Atom,'SkolemFunction'),!,isMooOption(opt_theory=Context),!.

atom_lookup_theory_ctx(Lang,Atom,Context,Atom,B,C,stheory):-
        atom_lookup_theory_ctx(Lang,Atom,Context,Atom,B,C).

atom_lookup_theory_ctx(kif,Atom,none,none,Atom,Atom,black):-!.
atom_lookup_theory_ctx(L,Atom,none,none,Atom,Atom,black):-once(atom_codes(Atom,Codes)),
        once((memberchk(34,Codes);memberchk(63,Codes);memberchk(32,Codes);memberchk(37,Codes))),!. % String
atom_lookup_theory_ctx(_,Atom,Context,'GlobalContext',Atom,Atom,blue):-!,isMooOption(opt_theory=Context),!. % Leftover must be PrologMOO (TODO)
atom_lookup_theory_ctx(_,Atom,'PrologMOO','GlobalContext',Atom,Atom,blue):-!.

codes_to_links(Codes,PrettyAtom):-
        getUnquotedCodes(Codes,UCodes),
        getKIFTokens(UCodes,WordList),
        concat_atom(WordList,'-',PrettyAtom),!.

getUnquotedCodes([34|Codes],UCodes):-
        (reverse(Codes,RCodes)),
        (ltrim(RCodes,[34|RUCodes])),
        reverse(RUCodes,UCodes).

getUnquotedCodes(UCodes,UCodes):-!.


%TODO Number?

% ================================================
%      toMarkUp_list
% ================================================

toMarkUp_list(L,Var,VS,Chars):-isSlot(Var),!,toMarkUp_slotValue(L,Var,VS,Chars).
toMarkUp_list(_,[],VS,''):-!.
toMarkUp_list(LANG,[H],VS,Chars):-!,
        toMarkUp_lang(LANG,H,VS,Chars).
toMarkUp_list(LANG,[H|T],VS,Chars):-!,
        toMarkUp_lang(LANG,H,VS,Chars1),
        toMarkUp_list(LANG,T,VS,Chars2),
        sformat(Chars,'~w ~w',[Chars1,Chars2]).

markUpVARLIST(L,[],Vars,''):-!.
markUpVARLIST(L,'$VAR'(_),Vars,''):-!.

markUpVARLIST(L,[VV|Varnames],Vars,Chars):-
                  VV=..[_,Name,Value],!,
                  toMarkupVarEquals(L,Name,Value,Vars,Chars1),
                  markUpVARLIST(L,Varnames,Vars,Chars2),
                  sformat(Chars,'~w\n~w',[Chars1,Chars2]).

toMarkupVarEquals(_,Name,Value,Vars,Chars):-
            toMarkUp_lang(kif,Name,Vars,NameQ),
            toMarkUp_slotValue(L,Value,Vars,ValChars),
            sformat(Chars,'~w = ~w',[NameQ,ValChars]).


% Real Prolog Var
toMarkUp_slotValue(L,Slot,VarList,Chars):- isVarProlog(Slot),!,
        toMarkUp_makeNamePrologVar(L,VarList,Slot,Name),
        atom_concat('?',Name,Chars),!.
% Slot 'Typed'
toMarkUp_slotValue(L,Slot,VarList,Chars):-isQualifiedAs(Slot,BaseType,Value,Subtype), !,
        toMarkUp_makeName(L,VarList,Slot,Subtype,Value,Name),
        close_freeVars(VarList,NVarList),
        append(NVarList,[Name=Value],NV),
        toMarkUp_lang(L,Value,NV,VChars),
        sformat(Chars,'<div title="~w">~w</div>',[Subtype,VChars]).

toMarkUp_makeNamePrologVar(L,VarList,Value,Name):-member(Name=Var,VarList),Var==Value,!.
toMarkUp_makeNamePrologVar(L,VarList,Value,Name):-getVarAtom(Value,NUame),atom_concat('?',NUame,Name).

getVarAtom(Value,Name):-var(Value),!,term_to_atom(Value,Vname),atom_codes(AVAR,[95,_|CODES]),atom_codes(Name,CODES),!.
getVarAtom('$VAR'(VNUM),Name):-concat_atom([VNUM],Name),!.



toMarkUp_makeName(L,VarList,Slot,BaseType,Value,Name):-
        member(Name=Var,VarList),Var==Slot,!.
toMarkUp_makeName(L,VarList,Slot,BaseType,Value,Name):-
        member(Name=Var,VarList),Var==Value,!.
toMarkUp_makeName(L,VarList,Slot,BaseType,Value,Name):-atom_concat('?',BaseType,Name).



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


