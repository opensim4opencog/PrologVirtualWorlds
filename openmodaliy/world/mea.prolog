% ===============================================================================================
%    by Mea_culpa
% ===============================================================================================

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

% ==================================================
% Mud Dictionary
% ==================================================

:-dynamic(mud_dictionary/2).

mud_dictionary([dinning,room],['dinning room']).
mud_dictionary([washing,machine],['washing machine']).
mud_dictionary([my,body],['self']).
mud_dictionary([back,yard],['back yard']).
mud_dictionary([stand,table],['stand table']).
mud_dictionary([light,switch],[light_switch]).
mud_dictionary([living,room],['living room']).
mud_dictionary([easy,chair],[easy_chair]).
mud_dictionary([coffee,table],[coffee_table]).
mud_dictionary([coffee,cup],[coffee_cup]).
mud_dictionary([top,drawer],[top_drawer]).
mud_dictionary([middle,drawer],[middle_drawer]).
mud_dictionary([bottom,drawer],[bottom_drawer]).
mud_dictionary([anthurium,crystallinum],[anthurium_crystallinum]).


% ==================================================
% Rooms
% ==================================================

:-multifile(room/2).
:-dynamic(room/1).

room(kitchen).
room(office).
room(hall).
room('living room').
room('dining room').
room(cellar).
room('bed room').
room(closet).
room('back yard').


% ==================================================
% Doors
% ==================================================

:-multifile(door/2).
:-dynamic(door/2).

door(office, hall).
door(kitchen, office).
door(hall, 'dining room').
door('living room', hall).
door(kitchen, cellar).
door('dining room', kitchen).
door(hall,'bed room').
door('bed room', closet).
door(kitchen, 'back yard').

connect(Thing,Noun2) :- door(Thing,Noun2); door(Noun2,Thing).

% ==================================================
% Where Things are Located
% ==================================================
:-assertOnLoad(corpius('me')).

:-dynamic(loc/2).
:-multifile(loc/2).

:-op(600,fx,onSpawn).

onSpawn(_).
onSpawn(ClassFact):- ClassFact=..[Funct,EntityClassA,EntityClassB],onSpawn(Funct,EntityClassA,EntityClassB).

onSpawn(Funct,EntityClassA,EntityClassB):-
		createByNameMangle(EntityClassA,IDA),
		createByNameMangle(EntityClassB,IDB),
		wfAssert(valueOf(IDA,Funct,EntityClassB)).
		
createByNameMangle(EntityClassA,EntityClassA):-compound(EntityClassA).
createByNameMangle(EntityClassA,IDA):-
	gensym(EntityClassA,IDA),
	englishServerInterface([create,EntityClassA,IDA]).
	

:-onSpawn loc(desk, office).
:-onSpawn loc(apple(color([red])), kitchen).
:-onSpawn loc(apple(color([green])), kitchen).
:-onSpawn loc(flashlight, desk).
:-onSpawn loc('washing machine', cellar).
:-onSpawn loc(nani, 'washing machine').
:-onSpawn loc(broccoli, kitchen).
:-onSpawn loc(crackers, kitchen).
:-onSpawn loc(computer, office).
:-onSpawn loc(envelope, desk).
:-onSpawn loc(stamp, envelope).
:-onSpawn loc(key, envelope).
:-onSpawn loc(bed, 'bed room').
:-onSpawn loc('stand table', 'bed room').
:-onSpawn loc(pajamas, 'stand table').
:-onSpawn loc(shirt, closet).
:-onSpawn loc(pillow, bed).
:-onSpawn loc(blanket, bed).                                                              
:-onSpawn loc(table, kitchen).
:-onSpawn loc(coat, closet).
:-onSpawn loc(pants, closet).
:-onSpawn loc(light_switch, 'bed room').
:-onSpawn loc(ball, 'back yard').
:-onSpawn loc(sofa, 'living room').
:-onSpawn loc(easy_chair, 'living room').
:-onSpawn loc(coffee_table, 'living room').
:-onSpawn loc(coffee_cup, kitchen).
:-onSpawn loc(coffee, coffee_cup).
:-onSpawn loc(book(color([green])), coffee_table).
:-onSpawn loc(book(color([pink])), coffee_table).
:-onSpawn loc(bookcase, 'living room').
:-onSpawn loc(dresser, 'bed room').
:-onSpawn loc(top_drawer, dresser).
:-onSpawn loc(middle_drawer, dresser).
:-onSpawn loc(bottom_drawer, dresser). 
:-onSpawn loc(houseplant, planter).
:-onSpawn loc(planter, gueridon). 
:-onSpawn loc(gueridon, 'living room').
:-onSpawn loc(garden_plant, 'back yard').
:-onSpawn loc(me, kitchen).
:-onSpawn loc(candle,kitchen).


:-dynamic(loc_s/2).


locaton_s(class_template(RealObj,D1,D2,Weight),Place):-
	loc(RealObj,Place),class_template(RealObj,D1,D2),
	member(weight(Weight),D2),!.
locaton_s(class_template(RealObj,D1,D2,0),Place):-
	loc(RealObj,Place), class_template(RealObj,D1,D2).	







% ==================================================
% class_template Descriptions
% ==================================================

:-dynamic(class_template/4).
:-op(600,fx,setTemplate).

setTemplate(_).

:-setTemplate class_template(candle, [color(blue),size(small)],[shape(triangular),material(wax),texture(sticky)]).
:-setTemplate class_template(apple, [color(red),size(small)],[shape(round),is_a(fruit),texture(smooth)]).
:-setTemplate class_template(apple, [color([green,pastel]),size(small)],[shape(round),is_a(fruit),texture(smooth)]).
:-setTemplate class_template(table, [color([blue,white]),size(large)],[shape(oval),material(wood),texture([bumpy,irregular]),weight(50)]).
:-setTemplate class_template(pillow, [color(white),size(medium)],[shape(rectangular),material([cotton,flannel]),texture([fluffy,soft])]).
:-setTemplate class_template(blanket, [color([green,dark]),size([large,bulky])],[shape(rectangular),material(rayon),texture([plush,soft])]).                                                              
:-setTemplate class_template('stand table', [color([black,shiney]),size(medium)],[shape([cubular,rectangular]),material(wood),texture(smooth),weight(25)]).
:-setTemplate class_template(pajams, [color([blue,pastel]),size([medium,fits])],[shape(unique),material([cotton,rayon ]),texture(soft)]).
:-setTemplate class_template(shirt, [color(black),size([medium,fits])],[shape(like(torso)),material(cotton),texture([soft,threadbare])]).
:-setTemplate class_template(flashlight, [color(yellow),size([medium,small])],[shape(cylindrical),material(plastic),texture(ribbed)]).
:-setTemplate class_template(key, [color(silver),size([small,tiny])],[shape(unique),material(metal)],[texture([ridged,bumpy])]).
:-setTemplate class_template(envelope, [color(white),size(small)],[shape(rectangular),material(paper),texture(smooth)]).
:-setTemplate class_template(desk, [color([brown,shiney]),size(large)],[shape([cubular,rectangular]),texture([smooth,ridged]),weight(175)]).
:-setTemplate class_template('washing machine', [color(white),size(large)],[shape([cubular,square]),material(metal),texture(smooth),weight(125)]).
:-setTemplate class_template(broccoli, [color(green),size(small)],[shape(like(tree)),is_a(vegetable),texture([porous,bumpy])]).                                                                    
:-setTemplate class_template(crackers, [color(tan),is_a(bread)],[shape([circular,flat]),size(small),texture([dry,coarse])]).
:-setTemplate class_template(computer, [color(grey),material(plastic)],[shape([cubular,rectangular]),size(medium),texture(smooth)]).
:-setTemplate class_template(stamp, [color('Polychromatic'),size(tiny,thin)],[shape(square),material(paper),texture([smooth,sticky])]).
:-setTemplate class_template(coat, [color(black),size([medium,fits])],[shape(like(torso)),material([wool,cotton,rayon ]),texture(coarse)]).
:-setTemplate class_template(pants, [color(black),size([medium,fits])],[shape(like(ass)),material([denim]),texture(coarse)]).
:-setTemplate class_template(nani, [color(brown),color(white),size(medium)],[shape(like(teddy_bear)),material(plush),texture([fuzzy,soft])]).
:-setTemplate class_template(light_switch, [color(cream),size(small)],[shape(rectangular),material(plastic),texture(smooth)]).
:-setTemplate class_template(ball, [color(red),size(medium)],[shape(spherical),material(rubber),texture(bumpy)]).
:-setTemplate class_template(coffee, [color(black,brown),size(small)],[shape(fluid),material(liquid),texture(wet)]).
:-setTemplate class_template(coffee_cup, [color(black,brown),size(small)],[shape(fluid),material(liquid),texture(wet)]).
:-setTemplate class_template(sofa, [color([blue,dark]),size([large,wide,long])],[shape(polyhedral),material([poplin,wood]),texture(ribbed),weight(125)]).
:-setTemplate class_template(easy_chair, [color(blue),size([large,narrow])],[shape(polyhedral),material([poplin,wood]),texture(ribbed),weight(75)]).
:-setTemplate class_template(coffee_table, [color([brown,shiney]),size(medium)],[shape(circular),material(wood),texture(smooth),weight(35)]).
:-setTemplate class_template(book, [color(green),color(white),color(black),size(small)],[shape(cubular,rectangular),material(paper),texture(smooth)]).
:-setTemplate class_template(book, [color(pink),color(white),color(black),size(small)],[shape(cubular,rectangular),material(paper),texture(smooth)]).
:-setTemplate class_template(bookcase, [color([brown,dark,shiney]),size(large)],[shape([square,concave_polyhedron]),material(wood),texture(smooth),weight(60)]).
:-setTemplate class_template(dresser, [color([brown,shiney]),size(large)],[shape([rectangular,polyhedral]),material(wood),texture(smooth),weight(45)]).
:-setTemplate class_template(top_drawer, [color([brown,shiney]),size(medium)],[shape([rectangular,concave_polyhedron]),material(wood),texture(smooth)]). 
:-setTemplate class_template(middle_drawer, [color([brown,shiney]),size(medium)],[shape([rectangular,concave_polyhedron]),material(wood),texture(smooth)]).
:-setTemplate class_template(bottom_drawer, [color([brown,shiney]),size(medium)],[shape([rectangular,concave_polyhedron]),material(wood),texture(smooth)]).
:-setTemplate class_template(planter, [color([pastel,blue]),size(medium)],[shape([cylindrical,trapezohedron]),material(clay),texture(abrasive),weight(15)]).
:-setTemplate class_template(houseplant, [color(green),size(medium)],[shape(anthurium_crystallinum),material(plant_tissue),texture(smooth)]).
:-setTemplate class_template(gueridon, [color([brown,shiney]),size(medium)],[shape(circular),material(wood),texture(smooth),weight(25)]).		 



:-setTemplate class_template(teddy_bear, [color(natural)],[shape(unique)]).
:-setTemplate class_template(torso, [color(like(skin))],[shape(unique)]).
:-setTemplate class_template(skin, [color(unique)],[shape(unique)]).

%:-setTemplate class_template(, [color(),size()],[shape(),material(),texture()]),weight()]).

:-dynamic(property/2).
:-dynamic(subpart/2).  

property(Obj,Words):-
	((Obj =..[Type,Spec] ; (Obj=Type,Spec=[]))),!,
	get_property(Type,Spec,Pro),
	transform_property(Pro,Property),
	Property=..Words.


transform_property(Pro,Property):-
	  Pro=..[Type,like(Other)],
	  Property=..[Type,_],!,
	  property(Other,Property).
transform_property(Property,Property).

	  
get_property(Type,Specification,Property):-
	member(Property,Specification).
get_property(Type,Specification,Property):-
	object(Type,D1,D2),
	append(D1,D2,DD),
	member(Property,DD),
	not(functor_member(Property,Specification)).



