:- op(800,xfy,&).
:- op(900,yfx,:).


plans(CurrentThread,_) :- unless(consistent(CurrentThread,true)), !, nl, write('impossible'), nl.
plans(CurrentThread,Plan) :- plan(CurrentThread,true,Plan,Planned), nl, write_out(Planned), nl.


write_out(Plan:Proposed_Function) :- !, write_out1(Plan), write(Proposed_Function), write('.'), nl.
write_out(Plan) :- write(Plan), write('.'), nl.

write_out1(Plan:Proposed_Function) :- !, write_out1(Plan), write(Proposed_Function), write(';'), nl.
write_out1(Plan) :- write(Plan), write(';'), nl.

plan(Event & CurrentThread,NextEvents,Plan,Plan2) :- !, solve(Event,NextEvents,Plan,EventFuture,Planned), plan(CurrentThread,EventFuture,Planned,Plan2).
plan(Event,NextEvents,Plan,Planned) :- solve(Event,NextEvents,Plan,_,Planned).

solve(Event,NextEvents,Plan,NextEvents,Plan) :- unchangable_events(Event).
solve(Event,NextEvents,Plan,EventFuture,Plan) :- holds(Event,Plan), and(Event,NextEvents,EventFuture).
solve(Event,NextEvents,Plan,Event & NextEvents,Planned) :- enable_the_event_with_this_method(Event,Proposed_Function), achieve(Event,Proposed_Function,NextEvents,Plan,Planned).

achieve(_,Proposed_Function,NextEvents,Plan,Planned:Proposed_Function ) :- 
   preserves(Proposed_Function,NextEvents),
   enabled_method_from_event(Proposed_Function,CurrentThread),
   consistent(CurrentThread,NextEvents),
   plan(CurrentThread,NextEvents,Plan,Planned),
   preserves(Proposed_Function,NextEvents).
achieve(Event,Proposed_Function,NextEvents,Plan:Function,Planned:Function) :- 
   are_preserved_durring(Event,Function),
  unwind(NextEvents,Function,EventFuture),
   achieve(Event,Proposed_Function,EventFuture,Plan,Planned),
   are_preserved_durring(Event,Function).

holds(Event,_:Function) :- enable_the_event_with_this_method(Event,Function).
holds(Event,Plan:Function) :- !, 
   are_preserved_durring(Event,Function),
   holds(Event,Plan),
   are_preserved_durring(Event,Function).
holds(Event,Plan) :- initial_given_events(Plan,Event).

are_preserved_durring(Event,Function) :- instanciate(Event & Function,0,_), remove_the_event_with_this_method(Event,Function), !, fail.
are_preserved_durring(_,_).

preserves(Proposed_Function,Event & CurrentThread) :- are_preserved_durring(Event,Proposed_Function), preserves(Proposed_Function,CurrentThread).
preserves(_,true).

unwind(NextEvents,Function,Event) :- 
   enabled_method_from_event(Function,CurrentThread),
   unwind1(NextEvents,Function,CurrentThread,EventFuture),
   conjoin(CurrentThread,EventFuture,Event).

unwind1(Event & NextEvents,Function,CurrentThread,EventFuture) :- enable_the_event_with_this_method(Task,Function), public(Event,Task), !, unwind1(NextEvents,Function,CurrentThread,EventFuture).
unwind1(Event & NextEvents,Function,CurrentThread,EventFuture) :- element_of(Task,CurrentThread), public(Event,Task), !, unwind1(NextEvents,Function,CurrentThread,EventFuture).
unwind1(Event & NextEvents,Function,CurrentThread,Event & EventFuture) :- unwind1(NextEvents,Function,CurrentThread,EventFuture).
unwind1(true,_,_,true).

consistent(CurrentThread,NextEvents) :- 
   instanciate(CurrentThread & NextEvents,0,_),
   impossible(Event),
   unless(unless(intersection_of(CurrentThread,Event))),
   implied(Event,CurrentThread & NextEvents), 
   !, fail.
consistent(_,_).

and(Event,NextEvents,NextEvents) :- element_of(Task,NextEvents), public(Event,Task), !.
and(Event,NextEvents,Event & NextEvents).

conjoin(Event & CurrentThread,NextEvents,Event & EventFuture) :- !, conjoin(CurrentThread,NextEvents,EventFuture).
conjoin(Event,NextEvents,Event & NextEvents).

element_of(Event,Task & _) :- element_of(Event,Task).
element_of(Event,_ & CurrentThread) :- !, element_of(Event,CurrentThread).
element_of(Event,Event).

intersection_of(Initialization,Functions) :- element_of(Event,Initialization), element_of(Event,Functions).

implied(Initialization & Functions,CurrentThread) :- !, implied(Initialization,CurrentThread), implied(Functions,CurrentThread).
implied(Event,CurrentThread) :- element_of(Event,CurrentThread).
implied(Event,_) :- Event.

own_task(Event,Task) :- 
   unless(Event=Task),
   unless(Event=qqq(_)),
   unless(Task=qqq(_)).

public(Event,Task) :- unless(private(Event,Task)).

private(Event,Task) :- instanciate(Event & Task,0,_), Event=Task, !, fail.
private(_,_).

instanciate(qqq(PrimaryInstance),PrimaryInstance,SecondaryInstance) :- !, SecondaryInstance is PrimaryInstance+1.
instanciate(qqq(_),PrimaryInstance,PrimaryInstance) :- !.
instanciate(Event,PrimaryInstance,SecondaryInstance) :- Event =.. [_|EventList], instanciate_tasklist(EventList,PrimaryInstance,SecondaryInstance).

instanciate_tasklist([Event|EventList],PrimaryInstance,Semiphore) :- instanciate(Event,PrimaryInstance,SecondaryInstance), instanciate_tasklist(EventList,SecondaryInstance,Semiphore).
instanciate_tasklist([],PrimaryInstance,PrimaryInstance).

unless(Event) :-  Event, !, fail.
unless(_).





%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

test1 :- plans( status(co_create(1),on), start).
test2 :- plans( thread_process_started(program(1),program(2)) & thread_process_started(program(2),program(3)), start).
test3 :- plans( dynamicly_linked(prolog,module(6)), start).
test4 :- plans( thread_process_started(program(2),program(3)) & thread_process_started(program(3),interface(1)) &
		status(co_create(1),on) & thread_process_started(program(1),program(2)) &
		compiled(prolog,function(2)), start).

impossible(_) :- fail.  % To stop Quintus complaining.

enable_the_event_with_this_method( dynamicly_linked(prolog,P), 	localmethod(P,_)).
enable_the_event_with_this_method( thread_process_started(prolog,X),	recomplie(X,_)).
enable_the_event_with_this_method( thread_process_started(X,Y), 	pushto(X,Y,_)).
enable_the_event_with_this_method( thread_process_started(Y,X),	pushto(X,Y,_)).
enable_the_event_with_this_method( status(S,on),	user_invoke(S)).
enable_the_event_with_this_method( on(prolog,B),	load_dll(B)).
enable_the_event_with_this_method( register_dll,		unload_dll(_)).
enable_the_event_with_this_method( compiled(prolog,R2), 	dynamicly_linked(_,_,R2)).

%%%enable_the_event_with_this_method( 

remove_the_event_with_this_method( dynamicly_linked(X,_),U) :- moved(X,U).
remove_the_event_with_this_method( thread_process_started(Z,prolog),U) :- !, remove_the_event_with_this_method(thread_process_started(prolog,Z),U).
remove_the_event_with_this_method( thread_process_started(prolog,X), pushto(X,_,_)) :- !, fail.
remove_the_event_with_this_method( thread_process_started(prolog,B), load_dll(B)) :- !, fail.
remove_the_event_with_this_method( thread_process_started(prolog,B), unload_dll(B)) :- !, fail.
remove_the_event_with_this_method( thread_process_started(X,_),U) :- moved(X,U).
remove_the_event_with_this_method( thread_process_started(_,X),U) :- moved(X,U).
remove_the_event_with_this_method( on(X,_),U) :- moved(X,U).
remove_the_event_with_this_method( register_dll,load_dll(_)).
remove_the_event_with_this_method( compiled(prolog,_), dynamicly_linked(_,_,_)).
remove_the_event_with_this_method( status(S,_), user_invoke(S)).

moved( prolog, localmethod(_,_)).
moved( prolog, recomplie(_,_)).
moved( prolog, pushto(_,_,_)).
moved( X, pushto(X,_,_)).
moved( prolog, load_dll(_)).
moved( prolog, unload_dll(_)).
moved( prolog, dynamicly_linked(_,_,_)).


enabled_method_from_event( localmethod(P,R), loccompiled(P,R) & compiled(prolog,R) & register_dll).
enabled_method_from_event( recomplie(X,R), compiled(X,R) & compiled(prolog,R) & register_dll).
enabled_method_from_event( pushto(X,Y,R),
	interfacing(X) & compiled(Y,R) & compiled(X,R) & thread_process_started(prolog,X) & register_dll).
enabled_method_from_event( user_invoke(co_create(S)),
	on(prolog,program(1)) & thread_process_started(program(1), co_create(S))).
enabled_method_from_event( load_dll(program(B)), thread_process_started(prolog,program(B)) & register_dll).
enabled_method_from_event( unload_dll(program(B)), on(prolog,program(B))).
enabled_method_from_event( dynamicly_linked(D,R1,R2),
	static_link(D,R1,R2) & compiled(prolog,R1) & thread_process_started(prolog,D) & register_dll).

unchangable_events( static_link(D,R1,R2)) :- static_link1(D,R1,R2).
unchangable_events( static_link(D,R2,R1)) :- static_link1(D,R1,R2).
unchangable_events( compiled(D,R1)) :- unchangable_events(static_link(D,_,R1)).
unchangable_events( interfacing(program(_))).
unchangable_events( loccompiled(module(6),function(4))).
unchangable_events( compiled(co_create(1),function(1))).
unchangable_events( dynamicly_linked(co_create(1),module(4))).

static_link1(interface(N),function(N),function(5)) :- range(N,1,4).

range(M,M,_).
range(M,L,N) :- L < N, L1 is L+1, range(M,L1,N).

initial_given_events( start, dynamicly_linked(program(N), module(N))) :- range(N,1,3).
initial_given_events( start, dynamicly_linked(prolog,module(5))).
initial_given_events( start, compiled(program(N),function(1))) :- range(N,1,3).
initial_given_events( start, compiled(prolog,function(1))).
initial_given_events( start, register_dll).
initial_given_events( start, status(co_create(1),off)).

