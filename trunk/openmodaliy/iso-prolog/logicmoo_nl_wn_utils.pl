:-include('logicmoo_utils_header.pl').
% ===================================================================
% ===================================================================
								     
clientEvent(Channel,Agent,english(phrase([wordnet,ID|_],Codes),_)):-
      findall(String,'#$wnS'(ID,_, String, _, _,_, _, _G190),Out),say((Out)).



% ===================================================================
% Dynamics
% ===================================================================

% ===================================================================
% Load EtoC data
% ===================================================================

%:-include('nlparser.data').
%:-ensure_loaded('nlparser.data').


convertWNSToCycMap:-tell(o),fail.
convertWNSToCycMap:-convertWNSToCycMap0.
convertWNSToCycMap:-convertWNSToCycMap1.
convertWNSToCycMap:-convertWNSToCycMap2.
convertWNSToCycMap:-convertWNSToCycMap3.
convertWNSToCycMap:-convertWNSToCycMap4.
convertWNSToCycMap:-convertWNSToCycMap5.
convertWNSToCycMap:-convertWNSToCycMap6.
convertWNSToCycMap:-convertWNSToCycMap7.
convertWNSToCycMap:-convertWNSToCycMap8.
convertWNSToCycMap:-convertWNSToCycMap9.
convertWNSToCycMap:-convertWNSToCycMapA.
convertWNSToCycMap:-convertWNSToCycMapB.
convertWNSToCycMap:-convertWNSToCycMapC.
convertWNSToCycMap:-convertWNSToCycMapD.
convertWNSToCycMap:-told.

idToPos(WNNUM,CycString):-
   number_codes(WNNUM,[Num|Codes]),
   numFront(Num,Front),
   atom_codes(CycString,[Front|Codes]),!.

numFront(49,78). % N
numFront(50,86). % V
numFront(51,65). % A
numFront(52,82). % R


transferPOS(n,'#$Noun').
transferPOS(v,'#$Verb').
transferPOS(a,'#$Adjective').
transferPOS(s,'#$AdjectiveSatellite').
transferPOS(r,'#$Adverb').


%s(synset_id,w_num,'word',ss_type,sense_number,tag_count). 
convertWNSToCycMap0:-
   ensure_loaded(wn_s),
   s(WNNUM, A,Atom, POS, B, C),
   wnSplit(Atom,MultiwordString),
   idToPos(WNNUM,CycString),
   transferPOS(POS,CycPOS),
   format('~q.~n',[saved('#$wnS'(CycString, A, '$stringSplit'(MultiwordString), CycPOS, B, C),'#$WordnetCompleteMt',_,['#$wnS',CycString, '$stringSplit',CycPOS|MultiwordString])]),fail.
 


convertWNSToCycMap1:-
      ensure_loaded(wn_hyp),
      hyp(NN,AA),
      idToPos(NN,N),idToPos(AA,A),
      format('saved(\'#$wnHypernym\'(\'~w\',\'~w\'),\'#$WordnetCompleteMt\',_,[\'#$wnHypernym\',\'~w\',\'~w\']).~n',[N,A,N,A]),fail.

convertWNSToCycMap2:-
      ensure_loaded(wn_ent),
      ent(NN,AA),
      idToPos(NN,N),idToPos(AA,A),
      format('saved(\'#$wnEntailment\'(\'~w\',\'~w\'),\'#$WordnetCompleteMt\',_,[\'#$wnEntailment\',\'~w\',\'~w\']).~n',[N,A,N,A]),fail.

convertWNSToCycMap3:-
      ensure_loaded(wn_sim),
      sim(NN,AA),
      idToPos(NN,N),idToPos(AA,A),
      format('saved(\'#$wnSimilarInMeaning\'(\'~w\',\'~w\'),\'#$WordnetCompleteMt\',_,[\'#$wnSimilarInMeaning\',\'~w\',\'~w\']).~n',[N,A,N,A]),fail.

convertWNSToCycMap4:-
      ensure_loaded(wn_mm),
      mm(NN,AA),
      idToPos(NN,N),idToPos(AA,A),
      format('saved(\'#$wnMemberMeronym\'(\'~w\',\'~w\'),\'#$WordnetCompleteMt\',_,[\'#$wnMemberMeronym\',\'~w\',\'~w\']).~n',[N,A,N,A]),fail.

convertWNSToCycMap5:-
      ensure_loaded(wn_ms),
      ms(NN,AA),
      idToPos(NN,N),idToPos(AA,A),
      format('saved(\'#$wnSubstanceMeronym\'(\'~w\',\'~w\'),\'#$WordnetCompleteMt\',_,[\'#$wnSubstanceMeronym\',\'~w\',\'~w\']).~n',[N,A,N,A]),fail.

convertWNSToCycMap6:-
      ensure_loaded(wn_mp),
      mp(NN,AA),
      idToPos(NN,N),idToPos(AA,A),
      format('saved(\'#$wnPartMeronym\'(\'~w\',\'~w\'),\'#$WordnetCompleteMt\',_,[\'#$wnPartMeronym\',\'~w\',\'~w\']).~n',[N,A,N,A]),fail.

convertWNSToCycMap7:-
      ensure_loaded(wn_cs),
      cs(NN,AA),
      idToPos(NN,N),idToPos(AA,A),
      format('saved(\'#$wnCauses\'(\'~w\',\'~w\'),\'#$WordnetCompleteMt\',_,[\'#$wnCauses\',\'~w\',\'~w\']).~n',[N,A,N,A]),fail.

convertWNSToCycMap8:-
      ensure_loaded(wn_vgp),
      vgp(NN,AA),
      idToPos(NN,N),idToPos(AA,A),
      format('saved(\'#$wnVerbsSimilarInMeaning\'(\'~w\',\'~w\'),\'#$WordnetCompleteMt\',_,[\'#$wnVerbsSimilarInMeaning\',\'~w\',\'~w\']).~n',[N,A,N,A]),fail.

convertWNSToCycMap9:-
      ensure_loaded(wn_at),
      at(NN,AA),
      idToPos(NN,N),idToPos(AA,A),
      format('saved(\'#$wnAttribute\'(\'~w\',\'~w\'),\'#$WordnetCompleteMt\',_,[\'#$wnAttribute\',\'~w\',\'~w\']).~n',[N,A,N,A]),fail.

convertWNSToCycMapA:-
      ensure_loaded(wn_ant),
      ant(NN,X,AA,Y),
      idToPos(NN,N),idToPos(AA,A),
      format('saved(\'#$wnAntonymous\'(\'~w\',~w,\'~w\',~w),\'#$WordnetCompleteMt\',_,[\'#$wnAntonymous\',\'~w\',\'~w\']).~n',[N,X,A,Y,N,A]),fail.

convertWNSToCycMapB:-
      ensure_loaded(wn_sa),
      sa(NN,X,AA,Y),
      idToPos(NN,N),idToPos(AA,A),
      format('saved(\'#$wnFollowsInMeaning\'(\'~w\',~w,\'~w\',~w),\'#$WordnetCompleteMt\',_,[\'#$wnFollowsInMeaning\',\'~w\',\'~w\']).~n',[N,X,A,Y,N,A]),fail.

convertWNSToCycMapC:-
      ensure_loaded(wn_ppl),
      ppl(NN,X,AA,Y),
      idToPos(NN,N),idToPos(AA,A),
      format('saved(\'#$wnAdjectiveParticiple\'(\'~w\',~w,\'~w\',~w),\'#$WordnetCompleteMt\',_,[\'#$wnAdjectiveParticiple\',\'~w\',\'~w\']).~n',[N,X,A,Y,N,A]),fail.

convertWNSToCycMapD:-
      ensure_loaded(wn_per),
      per(NN,X,AA,Y),
      idToPos(NN,N),idToPos(AA,A),
      format('saved(\'#$wnAdjectivePertains\'(\'~w\',~w,\'~w\',~w),\'#$WordnetCompleteMt\',_,[\'#$wnAdjectivePertains\',\'~w\',\'~w\']).~n',[N,X,A,Y,N,A]),fail.



%saved('#$termOfUnit'(
      %'$nartFixed'('#$MeaningInSystemFn'('#$WordNet-1997Version', 'N03952527')), 
      %'#$MeaningInSystemFn'('#$WordNet-1997Version', '$stringSplit'(['N03952527']))), 
      %'#$BaseKB', A, ['#$MeaningInSystemFn', '#$WordNet-1997Version', '#$termOfUnit', '$nartFixed', '$stringSplit', 'N03952527']).
%(MeaningInSystemFn WordNet-1997Version "A00066275")



