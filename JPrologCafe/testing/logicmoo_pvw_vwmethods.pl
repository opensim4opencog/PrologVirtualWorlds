end_of_file.
formed_invokation(Method):-formed_invokation(_Exemplar,Method,_ReturnValue).

/* Inherited Thing */
formed_invokation(thing,approach(thing([1])),none).
formed_invokation(thing,dogesture(gesture([1])),none).
formed_invokation(thing,donamedgesture(mtransable([1])),none).
formed_invokation(thing,drop(innerthing([1])),none).
formed_invokation(thing,emote(gesture([1])),none).
formed_invokation(thing,emote(gesture([1]),mtransable([2])),none).
formed_invokation(thing,getanimationfilekeys(thing([1])),file([_|_])).
formed_invokation(thing,getframe(thing([1])),frame).
formed_invokation(thing,getgeometryframe(thing([1])) ,frame([2])).
formed_invokation(thing,getgesturenames(thing([1])),gesture([2])).
formed_invokation(thing,getnamedmeshcolors(meshname([1])),color([2])). 
formed_invokation(thing,getnamedmeshes(thing([1])),mesh([_|_])). 
formed_invokation(thing,getnamedmeshtexture(meshname([1])),mesh([_|_])).
formed_invokation(thing,moveinto(thing([2])),none).
formed_invokation(thing,report(mtransable([1])),none).
formed_invokation(thing,say(mtransable([1]),thing([_|_])),none).
formed_invokation(thing,say(mtransable([1])),none).
formed_invokation(thing,setanimationindex(propname([1]),index([1]),optionbits([1])),none).
formed_invokation(thing,setnamedmeshcolor(meshname([1]),color([1])),none).
formed_invokation(thing,setnamedmeshtexture(meshname([1]),url([1])),none).
%%%%*%%%formed_invokation(thing,startanimation(propname([1]),pathmap([1]),varcallback,varoptions,vartimescale,varstartrange,varendrange,varindex),none).
%%%%*%%%formed_invokation(thing,stopanimation(propname([1]),varoptions,none).
formed_invokation(thing,whisper(mtransable([1]),thing([_|_])),none).

formed_invokation(thing,addproperty(thing([2]),propname([1])),none).
formed_invokation(thing,removeproperty(thing([2]),propname([1])),none).


/* Inherited Avatar */
formed_invokation(avatar,addtouserprofilelist(avatar([1]),propname([2])),none).
formed_invokation(avatar,removefromuserprofilelist(avatar([1]),propname([2])),none).
formed_invokation(avatar,ingnoreavatar(avatar([2])),none).
formed_invokation(avatar,uningnoreavatar(avatar([2])),none).
formed_invokation(avatar,alert(avatar([1]),title([1]),text([1]),thing([_|_]),varnotifymethod([1]),varnotifyparam([1])),none).
formed_invokation(avatar,confirm(title([1]),text([1]),thing([_|_]),varnotifymethod([1]),varnotifyparam([1])),none).
formed_invokation(avatar,prompt(title([1]),text([1]),default([1]),thing([_|_]),varnotifymethod([1]),varnotifyparam([1])),none).
formed_invokation(avatar,unwear(thing([2])),none).
formed_invokation(avatar,wear(thing([2])),none).
formed_invokation(avatar,updateuserprofilelist(avatar([1]),menupropname([1])),none).

/* Macros */
formed_invokation(thing,page(thing([_|_]),mtransable([1])),none).
formed_invokation(thing,set_property(propname([1]),value([1])),none).
formed_invokation(avatar,userconnect(_world,avatar([1])),none).
formed_invokation(avatar,userdisconnect(avatar([1])),none).
formed_invokation(thing,take_inside(thing([2])),none).
formed_invokation(thing,create_property_with_value(thing([2]),propname([1]),value([1])),none).

/* Inherited World */
formed_invokation(world,createinstance(thingname([2]),exemplar([1])),thing([2])).
formed_invokation(world,destroyobject(thing([2])),none).
formed_invokation(world,createexemplar(exemplarname([1]),exemplar([1])),thing([2])).

formed_invokation(world,createpropertylist(propnamedefualt([1])),properylist([2])).
formed_invokation(world,createpropertymap(propnamedefualt([1])),properymap([1])).
formed_invokation(world,createvector(x([1]),y([1]),z([1])),vector([1])).

formed_invokation(world,destroyobject(thing([2])),none).


formed_VBScript(_).

formed_state( is_at(thing(1),thing(2),yesno(1)) ).
formed_state( is_inside(thing(1),thing(2),yesno(1)) ).
formed_state( property_set(thing(1),propname(1),value(1)) ).
formed_state( can_change_property(thing(1),thing(2),propname(1),yesno(1)) ).
formed_state( is_connected(thing(1),yesno(1)) ).
formed_state( is_takable(thing(1),yesno(1)) ).
formed_state( is_container(thing(1),yesno(1)) ).
formed_state( is_uiaccessable(thing(1),yesno(1)) ).
formed_state( is_host(thing(1),yesno(1)) ).
formed_state( is_of_type(thing(1),exemplarname(1),yesno(1)) ).
formed_state( is_accessory(thing(1),yesno(1)) ).


formed_truism(_).   

formed_implication(is_news(_)).

formed_event(oninvoke(_Thing,_Method,_Args)).
formed_event(onproperty(_Thing,_PropertyName,_Value)).
formed_event(onui(_Thing,say,_Mtransable)).
formed_event(oninvoke(bot(phebe), onpeertell, [avatar(VBScripter), empty(_G11969), [i, am, VBScripter], 0])).
formed_event(oninvoke(avatar(vbscripter), getframe, [])).
formed_event(oninvoke(location('lobby location'), receivewhisper, [[base=whisper:[p, pres, i0, spact], tsub=[admin:[p, pers3, sing]], tind=[VBScripter:[p, pers3, sing, prprn], prpcnjlem=[to]], mod=[hi:[p, f0]], ltopic=[admin:[p]]], [avatar(admin), sampleavatar(VBScripter)]])).
%%%%%formed_event(onreport(vwPage, [base=whisper:[p, pres, i0, spact], tsub=[admin:[p, pers3, sing]], tind=[VBScripter:[p, pers3, sing, prprn], prpcnjlem=[to]], mod=[hi:[p, f0]], ltopic=[admin:[p]]]))
formed_event( oninvoke(avatar([1]), onclientpositionchanged, [101, vector(0.797762, 2.3, 1.13, 2.5)]) ).
formed_event( oninvoke(avatar([1]), onclientpositionchanged, [101, vector(0.436019, 2.3, 1.13, 5.5)]) ).





