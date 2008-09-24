
:-dynamic(instance_type_objname/2).
:-dynamic(learned/1).

gps(Thing,Property,Value).

object_template(thing(ObjName)):-instance_type_objname(thing,ObjName).
object_template(thing(ObjName)):-ground(ObjName),
        instance_type_objname(InstanceType,ObjName),
        ancestor(InstanceType,thing).

object_template(Pred):-
        catch2(Pred =.. [InstanceType,ObjName]),
        instance_type_objname(InstanceType,ObjName).

object_template(Pred):-
        catch2(Pred =.. [Type,ObjName]),
        instance_type_objname(InstanceType,ObjName),
        ancestor(InstanceType,Type).


instance_type_objname(Type,ObjName):-learned(instance_type_objname(Type,ObjName)).
instance_type_objname(Type,ObjName):-base_class_syntax(Type,ObjName).


base_class_syntax(vector,vector(1,1,1,1)).
base_class_syntax(vector,vector(_,_,_,_)).
base_class_syntax(vector,vector(1,1,1)).
base_class_syntax(vector,vector(_,_,_)).
base_class_syntax(ipropertylist,ipropertylist([])).
base_class_syntax(ipropertylist,ipropertylist(X)):-is_list(X).
base_class_syntax(ipropertymap,ipropertymap([])).
base_class_syntax(ipropertymap,ipropertymap(NameValuePairs)):-is_name_value_pairs(NameValuePairs).
base_class_syntax(array,array([])).
base_class_syntax(array,array(X)):-is_list(X).
base_class_syntax(string,string([])).
base_class_syntax(string,string(X)):-is_list(X).
base_class_syntax(string,string(X)):-atom(X).
%%base_class_syntax(string,X):-base_class_syntax(string,string(X)).



is_name_value_pairs([]):-!.
is_name_value_pairs([Property=Value|NameValuePairs]):-legal_pair(Property=Value),is_name_value_pairs(NameValuePairs).


legal_pair(_Property=_Value):-!.


ancestor(X,X).


ax_create(X):-actx_create_object(X,F),assert(foo(F)).

myguid('{2A24CF73-DFB2-4208-BEF3-2AD1214143E1}').
myt('{27EE3935-66AD-403E-BB2E-299AB01A3FFF}').
av('{19C15475-D70D-42E7-81EA-B732700CD4B4}').



obj_prop_get(X,Y,Z).

obj_prop_get(ithing('Admin'),Y,Z).


obj_prop_put(X,Y,Z).



