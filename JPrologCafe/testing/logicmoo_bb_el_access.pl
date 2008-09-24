:-include('logicmoo_utils_header.pl'). %<?
mooCache(Head,Body,Flags, KB,CTX,Proof):-saved(Head,Ctx,Proof,KB).
% ===========================================================
% Ask
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(submit=ask,Options),!, make,
        ensureMooOption(Agent,opt_ctx_request='GlobalContext',Ctx),
        ensureMooOption(Agent,opt_theory='LogicMOO',Context),
        ensureMooOption(Agent,sf=surf,Askion),
        atom_codes(Askion,Askion_Chars),
        ensureMooOption(Agent,user='Web',Agent),
        ensureMooOption(Agent,interp='kif',Interp),
        logOnFailure(getCleanCharsWhitespaceProper(Askion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        logOnFailure(invokeOperation(quiet,request(NEWFORM),Ctx,TrackingAtom,Context,Agent,Vars)).

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).

surfaceInference(X):-surfaceInference(X,Mt).

surfaceInference(X,Mt):-saved(X,_,_,Mt).


isArgIsa(Pred,N,Type):-saved('#$argIsa'(Pred,N,Type),_,_,_).
isArgIsa(Pred,1,Type):-saved('#$arg1Isa'(Pred,Type),_,_,_).
isArgIsa(Pred,2,Type):-saved('#$arg2Isa'(Pred,Type),_,_,_).
isArgIsa(Pred,3,Type):-saved('#$arg3Isa'(Pred,Type),_,_,_).
isArgIsa(Pred,4,Type):-saved('#$arg4Isa'(Pred,Type),_,_,_).
isArgIsa(Pred,N,'#$Collection'):-isArgGenl(Pred,N,Type).
%isArgIsa(Pred,N,'#$SubCollectionFn'(Type)):-isArgGenl(Pred,N,Type).
isArgIsa(Pred,_,'#$Thing').

isArgGenl(Pred,N,Type):-saved('#$argGenl'(Pred,N,Type),_,_,_).
isArgGenl(Pred,1,Type):-saved('#$arg1Genl'(Pred,Type),_,_,_).
isArgGenl(Pred,2,Type):-saved('#$arg2Genl'(Pred,Type),_,_,_).
isArgGenl(Pred,3,Type):-saved('#$arg3Genl'(Pred,Type),_,_,_).
isArgGenl(Pred,4,Type):-saved('#$arg4Genl'(Pred,Type),_,_,_).


saveEL:-saveEL('best.sorted').
saveEL(Filename):-
   tell(Filename),
format('      
:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:-index(saved(1,1,0,0)).
:- dynamic saved/4.
'),
   listing(saved),
   told.


