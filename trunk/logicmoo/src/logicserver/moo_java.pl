/*
:-module(moo_java,[
   startJava/0,
   startJamud/0,
   createJamud/0,
   loadJamudReferences/0]).
*/


:-dynamic(java_object_known/1).
:-module_transparent(java_object_known/1).



startJava:-
	 java_start("/opt/sourceforge/logicmoo/src:/opt/sourceforge/logicmoo/src/partner/jamud/src:/opt/sourceforge/logicmoo/lib/:cos.jar:/opt/sourceforge/logicmoo/lib/ecs-1.4.1.jar:/opt/sourceforge/logicmoo/lib/jdom.jar:/opt/sourceforge/logicmoo/lib/jaxp.jar:/opt/sourceforge/logicmoo/lib/rdf-api-2001-01-19.jar:/opt/sourceforge/logicmoo/lib/plc.jar:/opt/sourceforge/logicmoo/lib/cpj.jar:/opt/sourceforge/logicmoo/lib/nanoxml.jar:/opt/sourceforge/logicmoo/lib/jpl.jar:/opt/sourceforge/logicmoo/lib/crawler.jar").

startJamud:-
	 jamud_object(JAMUD),!,
	 java_invoke_method(JAMUD,startJamud(_X)).

createJamud:-
	 mooBaseJavaClass(MudClass),
	 java_create_object(MudClass,JAMUD),
	 %java_object(JAMUD)
	 format('\MudClass=~q\n',[JAMUD]),!,
	 assert(jamud_object(JAMUD)),!.
			
createJamud:-
	    format('\nCould not create the LogicMOO object\n',[]).
	 

loadJamudReferences:-
               jamud_object(JAMUD),
	 java_invoke_method(JAMUD,getJamudInstance(Instance)),
	 java_invoke_method(JAMUD,getJamudMudObjectRoot(MudRoot)),!,
	 assert(jamud_instance(Instance)),
	 assert(jamud_root(MudRoot)),
	 showInstanceValue(MudRoot).
	 
showInstanceValue(Object):-
   getJavatypeValueFn(Object,Name,_ActualName,Value),
   format('\n ~q \n',[equal('JavaMemberFn'('JavaObjectFn'(Object),Name),Value)]),
   fail.
showInstanceValue(_Object):-
   inferJavatypeMemberFnTuples(Term),
   format('\n~q\n',[Term]),
   fail.

   java_object_known(_).

showInstanceValue(_Object):-!.

:-dynamic(classToPLStructure/2).

javaClassToPLStructure(Object,Term):-
	 nonvar(Object),classToPLStructure(Object,Term),!.

javaClassToPLStructure(Object,Term):-
      jamud_object(JAMUD),
      java_invoke_method(JAMUD,classToPLStructure(Object,Term)),!,
      assert(classToPLStructure(Object,Term)).

getInstanceValue(Object,ActualName,Value):-
      getInstanceValue(Object,_Name,ActualName,Value),!.
getInstanceValue(Object,Name,Value):-
      getInstanceValue(Object,Name,_ActualName,Value),!.

getInstanceValue(Object,Name,ActualName,Value):-
      getInstanceValue(Object,Name,ActualName,Value,_Type).


inferJavatypeMemberFnTuples(equal('JavaMemberFn'('JavaObjectFn'(Object),Name),Result)):-
      java_object(Object),moo_java:java_object_known(Object),
      getJavatypeValueFn(Object,Name,_ActualName,Result).

getJavatypeValueFn(Object,ActualName,Value):-
      getJavatypeValueFn(Object,_Name,ActualName,Value),!.
getJavatypeValueFn(Object,Name,Value):-
      getJavatypeValueFn(Object,Name,_ActualName,Value),!.

getJavatypeValueFn(Object,Name,ActualName,'JavaValueFn'(Type,Value)):-
      getInstanceValue(Object,Name,ActualName,Value,Type).


getInstanceValue(Object,Name,ActualName,Value,Type):-
      javaClassToPLStructure(Object,Term),
      get_call_term_from_class(Object,Term,Name,Value,Type,ActualName,CallTerm),
      catch(CallTerm,E,format('\nAttempting "~q" resulted in ~q\n',[CallTerm,E])).

get_call_term_from_class(Object,Term,Name,Value,Type,Name,java_get_value(Object,MethodCall)):-
      catch(arg(1,Term,Arg1),_,fail),
      arg(1,Arg1,FieldsList),
      member(Field,FieldsList),
      functor(Field,ActualName,1),
      arg(1,Field,Type),
      MethodCall=..[ActualName,Value].

get_call_term_from_class(Object,Term,Name,Value,Type,ActualName,java_invoke_method(Object,MethodCall)):-
      catch(arg(2,Term,Arg1),_,fail),
      arg(1,Arg1,MethodsList),
      member(Method,MethodsList),
      functor(Method,ActualName,MA),
      arg(MA,Method,Type),
      match_method(MethodsList,Method,ActualName,MA,Name,Value,MethodCall).

match_method(_MethodsList,_Method,ActualName,_MA,Name,_Value,_MethodCall):-
      var(Name),
      % Make sure we don't walk blindly into some side-effect method when the Name is a variable
      member(Lead,['load','save','set','add','wait','equals','toString',
	 'unload','create','destroy','kill','stop','terminate','final','write','print','append',
	 ('['),('_'),('(') ]),atom_concat(Lead,_,ActualName),!,fail.
      
% Not that zero length happens but just incase fail. 
match_method(_MethodsList,_Method,_ActualName,_0,_Name,_Value,_MethodCall):-!,fail.

% Explore Simple No parameter methods
match_method(MethodsList,_Method,ActualName,1,Name,Value,MethodCall):-!,
           % Look for an identical match
      ((Name == ActualName) ;
           % Or no match, guess what was wanted by prepending 'get' or 'to'
      ((atom_concat('get',Name,ActualName);atom_concat('to',Name,ActualName)),
   	   % If we did a prepend, confirm that the Name was not already in list 
	 \+ (member(Term,MethodsList),functor(Term,Name,_)))),!,
      MethodCall=..[ActualName,Value].

% Explore Explicitly called 
match_method(_MethodsList,_Method,ActualName,MA,Name,Value,MethodCall):-
      nonvar(Name),functor(Name,ActualName,TA),
      Name =.. [ActualName|Rest],!,
      ((TA is MA -1 , Left= Rest);append(Left,[_],Rest)),!,
      append(Left,[Value],NewRest),!,MethodCall =.. [ActualName|NewRest].



%:-initialization(startLogicMoo).


doTopLoop:-
    eng_status.




