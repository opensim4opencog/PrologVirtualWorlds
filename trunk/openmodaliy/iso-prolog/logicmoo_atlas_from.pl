:-include('logicmoo_utils_header.pl').


getActiveForCC(Channel,Agent,Id):-(isActiveForCC(Channel,Agent,Id)),!.      
getActiveForCC(Channel,Agent,Id):-(isPlayerForCC(Channel,Agent,Id)),!.      
getActiveForCC(Channel,Agent,CC).


isActiveForCC(Channel,Agent,Active):-isPlayerForCC(Channel,Agent,Player),wfQuery(controls(Player,Active)).
isActiveForCC(Channel,Agent,Active):-wfQuery(controls(Channel,Agent,Active)).

isPlayerForCC(Channel,Agent,Active):-wfQuery(controls(Channel,Agent,Active)).
