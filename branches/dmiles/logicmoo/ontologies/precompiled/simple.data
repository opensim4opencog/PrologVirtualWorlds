
avatar(admin).
avatar(vbscripter).
avatar(X):-bot(X).

bot(the_robot).

%// thing(X):-avatar(X). % avoid programatic symetry

thing(X):-pthing(X).
thing(X):-athing(X).


pthing(apple).
pthing(orange).
pthing(X):-avatar(X).

athing(karma).

grasper(left_hand).
grasper(right_hand).

sense_organ(eyes).
sense_organ(nose).
sense_organ(ears).

bodypart(body).
bodypart(leg).
bodypart(X):-sense_organ(X).

text([noise]).

force(kenesis).


location(X):-room(X).
location([X,Y,Z]):-vector(X,Y,Z).
room(main_room).
room(other_room).


avalue(good).
avalue(bad).
pvalue(warm).

propname(Attrib):-thing_part(_Thing,Attrib).

bindable(Thing,Attrib,Value):-
        if(thing_part(Thing,Attrib)),
        if(prop_value(Attrib,Value)).


thing_part(avatar(_),cp(_)).
thing_part(avatar(_),ltm(_)).
thing_part(avatar(_),ltm(_)). %dups test
thing_part(avatar(_),Bpart):-p(bodypart,Bpart).
thing_part(avatar(_),moral).
thing_part(_,numeral).
thing_part(_,moral).

prop_value(cp(_),thought).
prop_value(cp(_),morethought).
prop_value(numeral,1).
prop_value(numeral,2).
prop_value(moral,good).
prop_value(moral,bad).
prop_value(A,V):-adda(prop_value(A,V)).
