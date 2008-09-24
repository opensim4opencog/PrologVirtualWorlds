end_of_file.
if_but_dont_remember(now, true, thing_at(Thing, Thing2)):-
        ground((Thing, Thing2)),invoke(obj_near_obj(Thing, Thing2),Dist),
        getpropstr(Thing2,'ApproachDistance',Range),Dist < Range .
if_but_dont_remember(now, true, thing_has_prop(Thing, Property)):-
        ground((Thing, Property)),invoke(obj_has_prop(Thing, Property),X),trueness(X).
if_but_dont_remember(now, true, thing_contains(Thing, Contains)):-
        ground((Thing, Contains)),invoke(vb_thing_contains(Thing, Contains),X),trueness(X).
if_but_dont_remember(now, true, thing_prop_value(Thing, Property, Value)):-
        getpropstr(Thing, Property, Value).
if_but_dont_remember(now, true, thing_is_type(Thing, Exemplar)):-
        getpropstr(Thing, exemplar, Exemplar).
if_but_dont_remember(now, true, thing_in_state(Thing, State)):-
        getpropstr(Thing, state, State)
        ;
        (( getpropstr(Thing,states,StateList),member(State,StateList) )).
if_but_dont_remember(now, true, thing_notice(Thing, Thing2)):-
        ground((Thing, Thing2)),invoke(obj_near_obj(Thing, Thing2),Dist), Dist < (8) .
if_but_dont_remember(now, true, thing_has_goal(Thing, Goal)):-
        (( getpropstr(Thing,goals,Goals),member(Goal,Goals) )).
if_but_dont_remember(now, true, thing_knows_method(Thing, Invokation)):-
        invoke(vb_thing_has_method(Thing,Invokation),X),trueness(X).
if_but_dont_remember(now, true, thing_invokation(Thing, Invokation)):-
        getpropstr(Thing,doing,Invokation).
        
if_but_dont_remember(now, false, thing_at(Thing, Thing2)):-
        ground((Thing, Thing2)),invoke(obj_near_obj(Thing, Thing2),Dist),
        getpropstr(Thing2,'ApproachDistance',Range),Dist > (Range) .
if_but_dont_remember(now, false, thing_has_prop(Thing, Property)):-
        test_not(if_but_dont_remember(now, true, thing_has_prop(Thing, Property))).
if_but_dont_remember(now, false, thing_contains(Thing, Contains)):-
        test_not(if_but_dont_remember(now, true, thing_contains(Thing, Contains))).
if_but_dont_remember(now, false, thing_prop_value(Thing, Property, Value)):-
        test_not(if_but_dont_remember(now, true, thing_prop_value(Thing, Property, Value))).
if_but_dont_remember(now, false, thing_is_type(Thing, Exemplar)):-
        test_not(if_but_dont_remember(now, true, thing_is_type(Thing, Exemplar))).
if_but_dont_remember(now, false, thing_in_state(Thing, State)):-
        test_not(if_but_dont_remember(now, true, thing_in_state(Thing, State))).
if_but_dont_remember(now, false, thing_has_goal(Thing, Goal)):-
        test_not(if_but_dont_remember(now, true, thing_has_goal(Thing, Goal))).
if_but_dont_remember(now, false, thing_notice(Thing, Thing2)):-
        test_not(if_but_dont_remember(now, true, thing_notice(Thing, Thing2))).        
if_but_dont_remember(now, false, thing_invokation(Thing, Invokation)):-
        test_not(if_but_dont_remember(now, true, thing_invokation(Thing, Invokation))).
if_but_dont_remember(now, false, thing_knows_method(Thing, Invokation)):-
        test_not(if_but_dont_remember(now, true, thing_knows_method(Thing, Invokation))).

if(now, Trueness, Fact):-
        if_but_dont_remember(now, Trueness, Fact),
        assert_new(recalled_fact(now, Trueness, Fact)).

if(was, Trueness, Fact):-recalled_fact(now, Trueness, Fact).

if(can_be, Trueness, Fact):-
        if_but_dont_remember(now, Trueness, Fact)
        ;
        recalled_fact(now, Trueness, Fact).

if(soon, true, Pred):-
        Pred=..[_FactNodePredicate,Thing|_Args],
        if(now,true,thing_has_goal(Thing,Pred)).

if(soon, false, Pred):-
        Pred=..[_FactNodePredicate,Thing|_Args],
        if(now,false,thing_has_goal(Thing,Pred)).
        
%  Instanciate Goal State
% -------------
make(now, Truth, thing_at(Thing, Thing2)):-if(now, Truth, thing_at(Thing, Thing2)).
make(now, Truth, thing_at(Agent, Thing2)):-!,invoke(myname,Agent),invoke(approach(Thing2),Truth).

make(now, true, thing_has_prop(Thing, Property)):-if(now,true,thing_has_prop(Thing, Property)).
make(now, true, thing_contains(_Thing, _Contains)):-!.
make(now, true, thing_prop_value(thing, property, value)).
make(now, true, thing_is_type(thing, exemplar)).
make(now, true, thing_in_state(thing, state)).
make(now, true, thing_has_goal(thing, goal)).
make(now, true, thing_notice(thing, thing2)).
make(now, true, thing_knows_method(thing, invokation)).
%  Action
%-------------
make(now, true, thing_invokation(thing, invokation)).

%  Instanciate Goal State
make(now, false, thing_at(thing, thing2)).
make(now, false, thing_has_prop(thing, property)).
make(now, false, thing_contains(thing, contains)).
make(now, false, thing_prop_value(thing, property, value)).
make(now, false, thing_invokation(thing, invokation)).
make(now, false, thing_is_type(thing, exemplar)).
make(now, false, thing_in_state(thing, state)).
make(now, false, thing_has_goal(thing, goal)).
make(now, false, thing_notice(thing, thing2)).
make(now, false, thing_knows_method(thing, invokation)).
make(was, true, thing_at(thing, thing2)).
make(was, true, thing_has_prop(thing, property)).
make(was, true, thing_contains(thing, contains)).
make(was, true, thing_prop_value(thing, property, value)).
make(was, true, thing_invokation(thing, invokation)).
make(was, true, thing_is_type(thing, exemplar)).
make(was, true, thing_in_state(thing, state)).
make(was, true, thing_has_goal(thing, goal)).
make(was, true, thing_notice(thing, thing2)).
make(was, true, thing_knows_method(thing, invokation)).
make(was, false, thing_at(thing, thing2)).
make(was, false, thing_has_prop(thing, property)).
make(was, false, thing_contains(thing, contains)).
make(was, false, thing_prop_value(thing, property, value)).
make(was, false, thing_invokation(thing, invokation)).
make(was, false, thing_is_type(thing, exemplar)).
make(was, false, thing_in_state(thing, state)).
make(was, false, thing_has_goal(thing, goal)).
make(was, false, thing_notice(thing, thing2)).
make(was, false, thing_knows_method(thing, invokation)).
make(can_be, true, thing_at(thing, thing2)).
make(can_be, true, thing_has_prop(thing, property)).
make(can_be, true, thing_contains(thing, contains)).
make(can_be, true, thing_prop_value(thing, property, value)).
make(can_be, true, thing_invokation(thing, invokation)).
make(can_be, true, thing_is_type(thing, exemplar)).
make(can_be, true, thing_in_state(thing, state)).
make(can_be, true, thing_has_goal(thing, goal)).
make(can_be, true, thing_notice(thing, thing2)).
make(can_be, true, thing_knows_method(thing, invokation)).
make(can_be, false, thing_at(thing, thing2)).
make(can_be, false, thing_has_prop(thing, property)).
make(can_be, false, thing_contains(thing, contains)).
make(can_be, false, thing_prop_value(thing, property, value)).
make(can_be, false, thing_invokation(thing, invokation)).
make(can_be, false, thing_is_type(thing, exemplar)).
make(can_be, false, thing_in_state(thing, state)).
make(can_be, false, thing_has_goal(thing, goal)).
make(can_be, false, thing_notice(thing, thing2)).
make(can_be, false, thing_knows_method(thing, invokation)).
make(soon, true, thing_at(thing, thing2)).
make(soon, true, thing_has_prop(thing, property)).
make(soon, true, thing_contains(thing, contains)).
make(soon, true, thing_prop_value(thing, property, value)).
make(soon, true, thing_invokation(thing, invokation)).
make(soon, true, thing_is_type(thing, exemplar)).
make(soon, true, thing_in_state(thing, state)).
make(soon, true, thing_has_goal(thing, goal)).
make(soon, true, thing_notice(thing, thing2)).
make(soon, true, thing_knows_method(thing, invokation)).
make(soon, false, thing_at(thing, thing2)).
make(soon, false, thing_has_prop(thing, property)).
make(soon, false, thing_contains(thing, contains)).
make(soon, false, thing_prop_value(thing, property, value)).
make(soon, false, thing_invokation(thing, invokation)).
make(soon, false, thing_is_type(thing, exemplar)).
make(soon, false, thing_in_state(thing, state)).
make(soon, false, thing_has_goal(thing, goal)).
make(soon, false, thing_notice(thing, thing2)).
make(soon, false, thing_knows_method(thing, invokation)).
find(now, true, thing_at(thing, thing2)).
find(now, true, thing_has_prop(thing, property)).
find(now, true, thing_contains(thing, contains)).
find(now, true, thing_prop_value(thing, property, value)).
find(now, true, thing_invokation(thing, invokation)).
find(now, true, thing_is_type(thing, exemplar)).
find(now, true, thing_in_state(thing, state)).
find(now, true, thing_has_goal(thing, goal)).
find(now, true, thing_notice(thing, thing2)).
find(now, true, thing_knows_method(thing, invokation)).
find(now, false, thing_at(thing, thing2)).
find(now, false, thing_has_prop(thing, property)).
find(now, false, thing_contains(thing, contains)).
find(now, false, thing_prop_value(thing, property, value)).
find(now, false, thing_invokation(thing, invokation)).
find(now, false, thing_is_type(thing, exemplar)).
find(now, false, thing_in_state(thing, state)).
find(now, false, thing_has_goal(thing, goal)).
find(now, false, thing_notice(thing, thing2)).
find(now, false, thing_knows_method(thing, invokation)).
find(was, true, thing_at(thing, thing2)).
find(was, true, thing_has_prop(thing, property)).
find(was, true, thing_contains(thing, contains)).
find(was, true, thing_prop_value(thing, property, value)).
find(was, true, thing_invokation(thing, invokation)).
find(was, true, thing_is_type(thing, exemplar)).
find(was, true, thing_in_state(thing, state)).
find(was, true, thing_has_goal(thing, goal)).
find(was, true, thing_notice(thing, thing2)).
find(was, true, thing_knows_method(thing, invokation)).
find(was, false, thing_at(thing, thing2)).
find(was, false, thing_has_prop(thing, property)).
find(was, false, thing_contains(thing, contains)).
find(was, false, thing_prop_value(thing, property, value)).
find(was, false, thing_invokation(thing, invokation)).
find(was, false, thing_is_type(thing, exemplar)).
find(was, false, thing_in_state(thing, state)).
find(was, false, thing_has_goal(thing, goal)).
find(was, false, thing_notice(thing, thing2)).
find(was, false, thing_knows_method(thing, invokation)).
find(can_be, true, thing_at(thing, thing2)).
find(can_be, true, thing_has_prop(thing, property)).
find(can_be, true, thing_contains(thing, contains)).
find(can_be, true, thing_prop_value(thing, property, value)).
find(can_be, true, thing_invokation(thing, invokation)).
find(can_be, true, thing_is_type(thing, exemplar)).
find(can_be, true, thing_in_state(thing, state)).
find(can_be, true, thing_has_goal(thing, goal)).
find(can_be, true, thing_notice(thing, thing2)).
find(can_be, true, thing_knows_method(thing, invokation)).
find(can_be, false, thing_at(thing, thing2)).
find(can_be, false, thing_has_prop(thing, property)).
find(can_be, false, thing_contains(thing, contains)).
find(can_be, false, thing_prop_value(thing, property, value)).
find(can_be, false, thing_invokation(thing, invokation)).
find(can_be, false, thing_is_type(thing, exemplar)).
find(can_be, false, thing_in_state(thing, state)).
find(can_be, false, thing_has_goal(thing, goal)).
find(can_be, false, thing_notice(thing, thing2)).
find(can_be, false, thing_knows_method(thing, invokation)).
find(soon, true, thing_at(thing, thing2)).
find(soon, true, thing_has_prop(thing, property)).
find(soon, true, thing_contains(thing, contains)).
find(soon, true, thing_prop_value(thing, property, value)).
find(soon, true, thing_invokation(thing, invokation)).
find(soon, true, thing_is_type(thing, exemplar)).
find(soon, true, thing_in_state(thing, state)).
find(soon, true, thing_has_goal(thing, goal)).
find(soon, true, thing_notice(thing, thing2)).
find(soon, true, thing_knows_method(thing, invokation)).
find(soon, false, thing_at(thing, thing2)).
find(soon, false, thing_has_prop(thing, property)).
find(soon, false, thing_contains(thing, contains)).
find(soon, false, thing_prop_value(thing, property, value)).
find(soon, false, thing_invokation(thing, invokation)).
find(soon, false, thing_is_type(thing, exemplar)).
find(soon, false, thing_in_state(thing, state)).
find(soon, false, thing_has_goal(thing, goal)).
find(soon, false, thing_notice(thing, thing2)).
find(soon, false, thing_knows_method(thing, invokation)).


/*
fact_predicate(X):-fact_predicates(List),member(X,List).

fact_predicates([
thing_at(thing,thing2),
thing_has_prop(thing,property),
thing_contains(thing,contains),
thing_prop_value(thing,property,value),
thing_invokation(thing,invokation),
thing_is_type(thing,exemplar),
thing_in_state(thing,state),
thing_has_goal(thing,goal),
thing_notice(thing,thing2),
thing_knows_method(thing,invokation)]).


%dirrection_Time_Truth_Fact
%----------------
if(now,true,Fact):-fact_predicate(Fact).
if(now,false,Fact):-fact_predicate(Fact).
if(was,true,Fact):-fact_predicate(Fact).
if(was,false,Fact):-fact_predicate(Fact).
if(can_be,true,Fact):-fact_predicate(Fact).
if(can_be,false,Fact):-fact_predicate(Fact).
if(soon,true,Fact):-fact_predicate(Fact).
if(soon,false,Fact):-fact_predicate(Fact).

make(now,true,Fact):-fact_predicate(Fact).
make(now,false,Fact):-fact_predicate(Fact).
make(was,true,Fact):-fact_predicate(Fact).
make(was,false,Fact):-fact_predicate(Fact).
make(can_be,true,Fact):-fact_predicate(Fact).
make(can_be,false,Fact):-fact_predicate(Fact).
make(soon,true,Fact):-fact_predicate(Fact).
make(soon,false,Fact):-fact_predicate(Fact).

find(now,true,Fact):-fact_predicate(Fact).
find(now,false,Fact):-fact_predicate(Fact).
find(was,true,Fact):-fact_predicate(Fact).
find(was,false,Fact):-fact_predicate(Fact).
find(can_be,true,Fact):-fact_predicate(Fact).
find(can_be,false,Fact):-fact_predicate(Fact).
find(soon,true,Fact):-fact_predicate(Fact).
find(soon,false,Fact):-fact_predicate(Fact).
*/


nd_getprop(Object,Property,Result):-var(Result),!,getpropstr(Object,Property,Result).
nd_getprop(Object,Property,Result):-ground(Result),!,
        getpropstr(Object,Property,Result1),
        Result=Result1.
        
nd_getprop(Object,Property,Unifiable):-!,
        invoke(getproptype(Object,Property),Type),
        invoke(getpropstr(Object,Property),Result),
        Possibility1=..[Type,Result],
        Possibility2=Result,
        (Unifiable=Possibility1 ; Unifiable=Possibility2).        





test_not(X):-X,!,fail.
test_not(_).

