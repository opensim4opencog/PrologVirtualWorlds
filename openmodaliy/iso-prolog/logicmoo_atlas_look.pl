:-include('logicmoo_utils_header.pl').
% =================================================================================
% =================================================================================
% Execute Look command
% =================================================================================
% =================================================================================      

% =================================================================================      
% English
% =================================================================================      
clientEvent(Channel,Agent,imperative_text(Agent,[look,Thing],packet(Channel,Serial,Agent,Refno,Time))):-!,
	 clientEvent(Channel,Agent,look(Agent,at,Thing,Refno)).

clientEvent(Channel,Agent,imperative_text(Agent,[look,Prep,Thing],packet(Channel,Serial,Agent,Refno,Time))):-!,
	 clientEvent(Channel,Agent,look(Agent,Prep,Thing,Refno)).

clientEvent(Channel,Agent,imperative_text(Agent,[look],packet(Channel,Serial,Agent,Refno,Time))):-
	 locationOfChar(Channel,Agent,Room),!,
	 clientEvent(Channel,Agent,look(Agent,in,Room,Refno)).

clientEvent(Channel,Agent,imperative_text(Agent,[look],packet(Channel,Serial,Agent,Refno,Time))):-
	 clientEvent(Channel,Agent,look(Agent,in,Agent,Refno)).

% =================================================================================      
% Altas
% =================================================================================      


% Look Blank
clientEvent(Channel,Agent,message(look(OPTokens),packet(Channel,Serial,Agent,Refno,Time))):-
      locationOfChar(Channel,Agent,Room),!,
      clientEvent(Channel,Agent,look(Agent,in,Room,Refno)).

% Look at self 
clientEvent(Channel,Agent,message(look(thing(''),OPTokens),packet(_,_,Agent,Refno,_))):-!,
      clientEvent(Channel,Agent,look(Agent,at,Agent,Refno)).

% Look Something 
clientEvent(Channel,Agent,message(look(Object,OPTokens),packet(_,_,Agent,Refno,_))):-!,
      getIdOut(Object,Id),clientEvent(Channel,Agent,look(Agent,at,Id,Refno)).

%[parents:list([look]), objtype:op]
clientEvent(Channel,Agent,message(look(Opage), packet(Channel,Serial,Agent,Refno,Time))):-!,
      clientEvent(Channel,Agent,look(Agent,at,Agent,Refno)).



% =================================================================================      
% LOOK EVENT
% =================================================================================     

clientEvent(Channel,Agent,look(Agent,_,Target,Refno)):-
      wfGetPropValue(Agent,loc,Target),!,
      (wfQuery(controls(Channel,Agent,Agent));wfAssert(controls(Channel,Agent,Agent))),!,
      canSeeDistance(Agent,Range),!,
      getObjectLookInfo(Agent,30,Target,Map),
      writeMessageOp(Channel,Agent,Agent,Target,Refno,['sight'],Map),!.

clientEvent(Channel,Agent,look(Agent,_,Target,Refno)):-
      getObjectLookInfo(Agent,30,Target,Map),
      writeMessageOp(Channel,Agent,Agent,Target,Refno,['sight'],Map),!.

clientEvent(Channel,Agent,look(Agent,_,Target,Refno)):-
      writeMessageOp(Channel,Agent,Agent,Target,Refno,['sight'],map([])),!.


% =================================================================================      
% LOOK PROC
% =================================================================================     

% getObjectLookInfo 
getObjectLookInfo(Agent,Range,Target,map([contains:list(VisList)|ValuesList])):-
      getAllNamedProps(Target,Props1),
      getOnlyPropsUsedPublic(Props1,Props2),
      fdelete(Props2,[contains],Props),
      getValuesForProps(Target,Props,ValuesList),
      getContentsVisableRange(Agent,Range,Target,VisList).

getContentsVisableRange(Agent,Range,Target,VisList):-
      getPosition(Agent,XYZ),
      getAllInLoc(Target,AllHere),
      canBeSeenFromS(XYZ,Range,AllHere,VisList),!.

getAllPossibleSeers(E,Seers):-
      wfGetPropValue(E,loc,Room),!,
      getAllInLoc(Room,Seers).

getAllInLoc(Room,AllHere):-
      findall(EE,wfGetPropValueInstance(EE,loc,Room),List),
      wfGetPropValue(Room,contains,list(Things)),
      append(List,Things,AllHereU),sort(AllHereU,AllHereD),!,fdelete(AllHereD,[game_entity],AllHere).

canBeSeenFromS(Agent,Range,[],[]):-!.
canBeSeenFromS(Agent,Range,_,[]):-Range<0,!.
canBeSeenFromS(Agent,Range,[Here|All],[Here|VisList]):-
      distanceOf(Agent,Here,Distance),Distance<Range,!,
      canBeSeenFromS(Agent,Range,All,VisList).
canBeSeenFromS(Agent,Range,[Here|All],VisList):-
      canBeSeenFromS(Agent,Range,All,VisList).
 
%atlasClientOp(clientconnection1, message(look([parents:list([look]), objtype:op]), packet('', 0, kid1, 5, 0)))
% S: [@serialnoo=3778@refno=4$from=world_0$to=nephrael_67#seconds=75426.2#future_seconds=0$time_string=(args=[(contains=$giwo_45$Thomas__Achtner_1$gfire1_12$munin_23$Giwo_34$nephrael_56$coin_61$nephrael_67)$id=world_0$objtype=object(parents=$world)])(parents=$sight)$id=$objtype=op$name=]

:-dynamic(look/1).  


look(CC) :-	     % looks at the room
  locationOfChar(Channel,Agent,Place),
  list_things(Place,Things),
  sayIfFeedback(Channel,Agent,(['  I am in',Place,'seeing:'|Things])),
  list_connections(Place,Places),
  sayIfFeedback(Channel,Agent,(['can go to:'|Places])),!.
look(CC) :-	     % looks at the room
  sayIfFeedback(Channel,Agent,(['you are nowhere'])),!.

% ==================================================
% Looking at Thing
% ==================================================
look(Agent,in,Thing,Refno):-!,look_in(Channel,Agent,Thing).
look(Agent,_,Thing,Refno):-look(Channel,Agent,Thing).

look_in(Channel,Agent,Thing):-
	location(Noun2,Thing),
	sayIfFeedback(Channel,Agent,[i,see,Noun2]),fail.

look(Channel,Agent,Thing):-
      subpart(Thing,Noun2),
      look(Channel,Agent,Noun2).
look(Channel,Agent,Thing):-
      property(Thing,Noun2),
      sayIfFeedback(Channel,Agent,Noun2),fail.
look(Channel,Agent,Thing):-
     switch_status(Thing,Noun2),
     sayIfFeedback(Channel,Agent,[Thing,is,Noun2]),fail.
%look(Channel,Agent,Thing):-sayIfFeedback(Channel,Agent,[i,dont,see,anything,special,about,Thing]).


/*
% looking at one of your existing chars and take it
look_command(Channel,Agent,map([id:CharWanted]),OPTokens,Refno):-
	% wfQuery('#$ableToAffect'(Player,CharWanted)),
	 getObjectMap(CharWanted,Charmap),
	 sendSighting(CharWanted,Channel,Agent,Refno,Charmap),!.

% looking at a someone elses existing chars
look_command(Channel,Agent,map([id:CharWanted]),OPTokens,Refno):-member(from:Player,List),
	 not(wfQuery('#$ableToAffect'(Player,CharWanted))),
	 wfQuery('#$ableToAffect'(SomeoneElse,CharWanted)),!,
	 getObjectMap(CharWanted,Charmap),
	 sendSighting(CharWanted,Channel,Agent,Refno,Charmap).

% looking at a non existing Target
look_command(Channel,Agent,map([id:CharWanted]),OPTokens,Refno):-member(from:Player,List),
	 wfQuery('#$ableToAffect'(Player,CharWanted)),
	 getObjectMap(CharWanted,Charmap),
	 sendSighting(CharWanted,Channel,Agent,Refno,Charmap),!.
*/

/*
lientOp(clientconnection1, message(touch(thing('Zephyr1'), [args:list([map([id:'Zephyr1'])])
ouch]), objtype:op]), packet('', 0, 'Zephyr1', 36, 0)))


unhandled(clientconnection1, message(touch(thing('Zephyr1'), [args:list([map([id:'Zephyr1'])]
touch]), objtype:op]), packet('', 0, 'Zephyr1', 36, 0)))


clientEvent(clientconnection2, message(talk(concept([say:'make an emote again so i can capture']
p([say:'make an emote again so i can capture'])]), parents:list([talk]), objtype:op]), packet
18, 0)))

bach: clientconnection1 [@serialno=83@refno=18$from=lilguy1$to=Zephyr1#seconds=1029945535.654
.000000$time_string=Wed Aug 21 11:58:55 2002(args=[$from=lilguy1(args=[$say=make an emote aga
re])(parents=$talk)$objtype=op])(parents=$sound)$id=$objtype=op$name=sever message op]
.
bach: clientconnection2 [@serialno=84@refno=18$from=lilguy1$to=lilguy1#seconds=1029945535.671
.000000$time_string=Wed Aug 21 11:58:55 2002(args=[$from=lilguy1(args=[$say=make an emote aga
re])(parents=$talk)$objtype=op])(parents=$sound)$id=$objtype=op$name=sever message op]
.

clientEvent(clientconnection2, message(talk(concept([say:emotes, style:shout]), [args:list([map(
e:shout])]), parents:list([talk]), objtype:op]), packet('', 0, lilguy1, 19, 0)))
ientOp(clientconnection1, message(talk(concept([say:'here\'s another thing - touch']), [args:list([map([say:'here\'s another thing - touch'])]), parents:list([talk]), objtype:op]), packet('', 0, 'Zephyr1', 39, 0)))

bach: clientconnection1 [@serialno=87@refno=39$from=Zephyr1$to=Zephyr1#seconds=1029945597.094#future_seconds=0.000000$time_string=Wed Aug 21 11:59:57 2002(args=[$from=Zephyr1(args=[$say=here's another thing - touch])(parents=$talk)$objtype=op])(parents=$sound)$id=$objtype=op$name=sever message op]
.
bach: clientconnection2 [@serialno=88@refno=39$from=Zephyr1$to=lilguy1#seconds=1029945597.111#future_seconds=0.000000$time_string=Wed Aug 21 11:59:57 2002(args=[$from=Zephyr1(args=[$say=here's another thing - touch])(parents=$talk)$objtype=op])(parents=$sound)$id=$objtype=op$name=sever message op]
.

clientEvent(clientconnection1, message(touch(thing('Bobby'), [args:list([map([id:'Bobby'])]), parents:list([touch]), objtype:op]), packet('', 0, 'Zephyr1', 40, 0)))


unhandled(clientconnection1, message(touch(thing('Bobby'), [args:list([map([id:'Bobby'])]), parents:list([touch]), objty

bach: clientconnection1 [@serialno=113@refno=51$from=Zephyr1$to=Zephyr1#seconds=1029945765.734#future_seconds=0.000000$time_string=Wed Aug 21 12:02:45 2002(args=[$from=Zephyr1(args=[$say=+2b24%_+2b40%+2b28+2b29+2b23])(parents=$talk)$objtype=op])(parents=$sound)$id=$objtype=op$name=sever message op]
.
bach: clientconnection2 [@serialno=114@refno=51$from=Zephyr1$to=lilguy1#seconds=1029945765.750#future_seconds=0.000000$time_string=Wed Aug 21 12:02:45 2002(args=[$from=Zephyr1(args=[$say=+2b24%_+2b40%+2b28+2b29+2b23])(parents=$talk)$objtype=op])(parents=$sound)$id=$objtype=op$name=sever message op]
.

clientEvent(clientconnection1, message(imaginary(concept([description:emotes, id:emote]), [args:list([map([description:emotes, id:emote])]), parents:list([imaginary]), objtype:op]), packet('', 0, 'Zephyr1', 52, 0)))


unhandled(clientconnection1, message(imaginary(concept([description:emotes, id:emote]), [args:list([map([description:emotes, id:emote])]), parents:list([imaginary]), objtype:op]), packet('', 0, 'Zephyr1', 52, 0)))


clientEvent(clientconnection1, message(touch(thing('Bobby'), [args:list([map([id:'Bobby'])]), parents:list([touch]), objtype:op]), packet('', 0, 'Zephyr1', 53, 0)))


unhandled(clientconnection1, message(touch(thing('Bobby'), [args:list([map([id:'Bobby'])]), parents:list([touch]), objtype:op]), packet('', 0, 'Zephyr1', 53, 0)))

unhandled(clientconnection1, message(imaginary(concept([description:emotes, id:emote]), [args:list([map([description:emotes, id:emote])]), parents:list([imaginary]), objtype:op]), packet('', 0, 'Zephyr1', 52, 0)))


clientEvent(clientconnection1, message(touch(thing('Bobby'), [args:list([map([id:'Bobby'])]), parents:list([touch]), objtype:op]), packet('', 0, 'Zephyr1', 53, 0)))


unhandled(clientconnection1, message(touch(thing('Bobby'), [args:list([map([id:'Bobby'])]), parents:list([touch]), objtype:op]), packet('', 0, 'Zephyr1', 53, 0)))


clientEvent(clientconnection1, message(talk(concept([say:oog, loc:lobby]), [args:list([map([say:oog, loc:lobby])]), parents:list([talk]), objtype:op]), packet(lobby, 0, 'Zephyr', 54, 0)))


unhandled(clientconnection1, message(talk(concept([say:oog, loc:lobby]), [args:list([map([say:oog, loc:lobby])]), parents:list([talk]), objtype:op]), packet(lobby, 0, 'Zephyr', 54, 0)))


*/


