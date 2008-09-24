% ================================================================================
% consultation(Request_Compiled(i),Consultation_Type(?),ResultSingle_bindings(o),Truth(o),How(o))
% Request_Compiled = WFS Format
% ResultSingle_bindings = XSB_Compiled_Format
% Consultation_Type = member of [consult_truth,consult_facts,consult_single_bindings]
% Truth = Real Number or tabled_true or not_proved
% How = Explaination Used
% ================================================================================
% :-include('moo_header.pl').

% ==========================================================
%  Consultation
% ==========================================================

:- dynamic consultation_gafs/1.

'moo-consultation'(Context_atom,ConsultationRequestP,ListOfGafsAsserted):-
         agentConsultation(Context_atom,ConsultationRequestP,ListOfGafsAsserted),!,
         member(ConsultationRequest,ListOfGafsAsserted).

'user-believes'(ConsultationRequestP):-
         agentConsultation('GlobalContext',ConsultationRequestP,ListOfGafsAsserted),!,
         member(ConsultationRequest,ListOfGafsAsserted).

agentConsultation(Context_atom,ConsultationRequest,ListOfGafsAsserted):-
                     %member(Method,[consult_single_bindings,consult_facts,consult_truth]),!,
                     retractall(consultation_gafs(_)),
                     write_direct_l(['<Consult>',nl]),
                     ConsultationRequest=.. STERM,
                     agentConsult_want(Context_atom,STERM),
                     write_direct_l(['</Consult>',nl]), 
                     once(agentConsult_get_results(Context_atom,ListOfGafsAsserted)).

                     
agentConsult_want(Context_atom,[ConsultationPred|RequestArgs]):-
                     length(RequestArgs,Arity) ,
                     write_direct_l(['<Want pred="',ConsultationPred,'" arity="',Arity,'" ctxt="',Context_atom,'">',nl]),
                     agentConsult_args(Context_atom,ConsultationPred,1,RequestArgs),                                          
                     write_direct_l(['</Want>',nl]).
                             
agentConsult_args(_Context_atom,_Consultation,_Num,[]):-!.                                          
agentConsult_args(Context_atom,ConsultationPred,_Num,[Head|Tail]):-!,   %true,
                     once(agentConsult_one_arg(ConsultationPred,Context_atom,_Num,Head)),
                     Next_Num is _Num +1,
                     agentConsult_args(Context_atom,ConsultationPred,Next_Num,Tail).

agentConsult_one_arg(Context_atom,ConsultationPred,_Num,Head):- (var(Head);member(Head,['-','?','+','#'])), !,
                  write_direct_l(['<arg argn="',_Num,'"']),
                  %ignore((theory(Context_atom,['surface-domain',ConsultationPred,_Num,ArgDomain]),write_direct_l([' domain="',ArgDomain,'"']))),
                  write_direct_l([' state="need"/>',nl]).
         
agentConsult_one_arg(Context_atom,ConsultationPred,_Num,Head):-!,
                  write_direct_l(['<arg argn="',_Num,'"']),
                  %ignore((theory(Context_atom,['surface-domain',ConsultationPred,_Num,ArgDomain]),write_direct_l([' domain="',ArgDomain,'"']))),
                  write_direct_l([' state="bound">',Head,'</arg>',nl]).

agentConsult_get_results(Context_atom,(PSurface,CSurface,WFS,CFORM,NextTerm)):-  
                        once(((xsbRequestStream(Stream),conv_readS(Stream,Assert_chars,STerm,_Vars));conv_readS(Assert_chars,STerm,_Vars))),

                        once((
                           member(STerm,[[continue],[abort]])
                            ;
                              ((
                               ignore((           
                                          once(tell_retract_parse_chars(Assert_chars,Pterm,Vars)),
                                          source_compile(Pterm,Ctx,TN,Context,CM,Vars,PSurface,CSurface,WFS,CFORM),
                                          do_to_conjuncts(CSurface,assert_prolog_tm),!,
                                          do_to_conjuncts(CFORM,assert_prolog_tm),!
                                       )), 
                              agentConsult_get_results(Context_atom,NextTerm),
                              
                              ignore(NextTerm=end_of_file)
                             ))
                        )).

                        %,!,findall(GafAsserted,consultation_gafs(GafAsserted),ListOfGafsAsserted).



consultation(PredicateI,PProtoArgs,PArgs,_Cxt):- 
       pterm_to_sterm(PProtoArgs,ProtoArgs),
       pterm_to_sterm(PArgs,Args),
               consultation_match_list(Args,ProtoArgs,ConsultTemplate),
            %   writeIfOption(rt,[want,[PredicateI|Args],asking,[PredicateI|ConsultTemplate]]),
               once((
                     ((moo_K_scenario(_Cxt,[PredicateI|ConsultTemplate]),!))
                     ;
                     (( 
                       
                        agentConsultation(_Cxt,[PredicateI|ConsultTemplate],_ListOfGafsAsserted),
                        assert(moo_K_scenario(_Cxt,[PredicateI|ConsultTemplate]))
                     ))
               )).

%'AssignmentFn'(Pred,ARGS,Ctx):-abort('AssignmentFn'(Pred,ARGS,Ctx)).


               
%ignore((_Often=once,fail,retract(_,[consultation_prototype,once,PredicateI,ProtoArgs])))


consultation_match_list(ARGS,ARGSP,ARGSP):- ARGS == ARGSP,!.
consultation_match_list([ARG|ARGS],[PROTO|PROTOS],[Suggestion|Suggestions]):-
      consultation_match(ARG,PROTO,Suggestion),
      consultation_match_list(ARGS,PROTOS,Suggestions).

consultation_match(ARG,PROTO,PROTO):- ARG==PROTO,!.
consultation_match(ARG,must_be_bound_before_call,ARG):- ground(ARG),!.
consultation_match(ARG,(+),ARG):- ground(ARG),!.
consultation_match(ARG,[+],ARG):- ground(ARG),!.
consultation_match(ARG,must_be_unbound_before_call,ARG):- var(ARG),!.
consultation_match(ARG,(-),ARG):- var(ARG),!.
consultation_match(ARG,[-],ARG):- var(ARG),!.
consultation_match(Var,consultation_must_bind_without_suggestion,Var):-var(Var),!.
consultation_match(_,consultation_must_bind_without_suggestion,_).
consultation_match(ARG,consultation_must_bind,ARG).
consultation_match(ARG,(-),ARG).
consultation_match(ARG,(?),ARG).
consultation_match(ARG,(#),ARG).

