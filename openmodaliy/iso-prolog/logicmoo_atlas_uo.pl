:-include('logicmoo_utils_header.pl').
end_of_file.
%<?
:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).

:-ensure_loaded('logicmoo_utils.pl').
% ================================================================
%  Ultima Online Server
% ================================================================
% 5000-5010 Game
% 7775-7777 Login 
% 8888 Patch 
% 8800-8900 UO Messenger 
% 9999 Patch
% 7875 UOMonitor


% =================================================================================
% =================================================================================
% Client Handler Thread
% =================================================================================
% =================================================================================

uoWriteServerlist(Stream):-
   uoWriteObject(Stream,packet('Britannia List',loop([server(w(1),slist("LogicMoo"),percent(25),timezone(8),ip(12,225,240,66))]))).


packetLog(In):-
      flag(packetLog,X,X+1),
      get0(In,C),writeq(user_error,X),writeSTDERRHex([C]),!.

clientEvent(Channel,Agent,CMD):-uoClientOp(Channel,Agent,CMD),!.

% ================================================================
% ================================================================
% Service Client/Server OP
% ================================================================
% ================================================================

uoClientOp(Channel,Agent,packet('Acct Login Req', ['Name':c(NameString), 'Password':c(PasswordString),_],Comment)):-!,
      %nameFromChars(NameString,Name),
      %nameFromChars(PasswordString,Password),
      uoWriteObject(Channel,Agent,packet('Acct Login Ok',["Welcome!"])),
      uoWriteServerlist(CC),!.
      
% ================================================================
% ================================================================
% Writing to Ultima Online Client/Server (Object Mode)
% ================================================================
% ================================================================
uoWriteObject('$stream'(Stream),Object):-!,
      uoObjectToPacket(packet,Object,Chars),
      writeSTDERRHex(Chars),
      format('$stream'(Stream),'~s',[Chars]),flush_output('$stream'(Stream)).

uoWriteObject(Channel,Agent,Object):-
      wantsEvents(Channel,Agent,In,Out,'UOX'),!,uoWriteObject(Out,Object).



writeSTDERRHex([]):-nl(user_error),flush_output(user_error).
writeSTDERRHex([H|T]):-format(user_error,' 0x~16R',[H]),writeSTDERRHex(T).

% ================================================================
% ================================================================
% Writing to Ultima Online Client/Server (Binary Mode)
% ================================================================
% ================================================================
uo_map_fit(Object,Results,ToFit):-uo_map(Object,ToFit),Object=..[_|Results].

uo_map(server(Index,Name,Fullness,TimeZone,IP),
   [word:'Server Index',char-32:'Server Name',byte:'Percent Full',byte:'Timezone',dword:'Server IP']).

%[byte:'ID(81)',word:'Packet Size',byte - * :'Unknown'])

uoObjectToPacket(N,packet(Name,Params,_),Out):-!,uoObjectToPacket(N,packet(Name,Params),Out).
uoObjectToPacket(_,packet(Name,Params),[Code|Out]):-!,
      uo_packet(Code,Name,Comment,Bytes,[_|Map]),
      (number(Bytes) -> uoObjectToPacket(Map,Params,Out);
      (uoObjectToPacket(Map,Params,End),length(End,N),Word is N+3,
      MSB0 is Word /\ 65280,LSB0 is Word /\ 255,
      Out=[MSB0,LSB0|End])).

uoObjectToPacket(Fit,loop(List),[MSB0,LSB0|Out]):-!,
      length(List,Word),MSB0 is Word /\ 65280,LSB0 is Word /\ 255,
      uoObjectToPacket(Fit,List,Out).

uoObjectToPacket(_,Object,Packet):-
      uo_map_fit(Object,Results,ToFit),!,
      uoObjectToPacket(ToFit,Results,Packet).


%uoObjectToPacket(_,Fit:Object,Packet):-!,uoObjectToPacket(Fit,Object,Packet).
uoObjectToPacket(Fit:_,Object,Packet):-!,uoObjectToPacket(Fit,Object,Packet).
uoObjectToPacket(byte,Symbol,[Num]):-uoConstantTable(Symbol,Num),!.
uoObjectToPacket(char,Symbol,[Num]):-uoConstantTable(Symbol,Num),!.
uoObjectToPacket(word,Symbol,[LSB0,MSB0]):-uoConstantTable(Symbol,Word),!,
      MSB0 is Word /\ 65280,LSB0 is Word /\ 255.
uoObjectToPacket(dword,ip(A,B,C,D),[A,B,C,D]).
uoObjectToPacket(char-Num,Chars,Out):-uoCharsToPacketLength(Num,Chars,Out).
uoObjectToPacket(byte-Num,Chars,Out):-uoCharsToPacketLength(Num,Chars,Out).

uoObjectToPacket(dword,Symbol,[LSB0,MSB0,LSB1,MSB1]):-uoConstantTable(Symbol,Word),!,
      MSB1 is Word /\ -16777216,LSB1 is Word /\ 16711680,MSB0 is Word /\ 65280,LSB0 is Word /\ 255.

uoObjectToPacket(Fit,[L|LL],OOO):- uoObjectToPacketS(Fit,[L|LL],OOO).
uoObjectToPacket(Fit,Compound,T):-compound(Compound),!,arg(1,Compound,TT),uoObjectToPacket(Fit,TT,T).
uoObjectToPacket(_,X,[X]).

uoConstantTable(N,N):-number(N),!.

tp:-uoObjectToPacket(_,packet('Britannia List',loop([server(w(1),slist("LogicMoo"),percent(25),timezone(8),ip(12,225,240,66))])),O),
   writeq(O),nl.


uoObjectToPacketS([H],[L],O):-uoObjectToPacket(H,L,O).
uoObjectToPacketS([H|T],[L|LL],OOO):-uoObjectToPacket(H,L,O),uoObjectToPacketS(T,LL,OO),append(O,OO,OOO).


% ================================================================
% =================================================================================
% Make a CharList a certain length
% =================================================================================
% ================================================================
uoCharsToPacketLength(*,Chars,Chars).
uoCharsToPacketLength(Num,Chars,Out):-length(Chars,N),X is N - Num,
      uoCharsToPacket(X,Chars,Out).

% Juest right
uoCharsToPacket(0,Chars,Chars).
% too long
uoCharsToPacket(Less,Chars,Out):-Less>0,!,length(Out,Less),append(Out,_,Chars).
% too short
uoCharsToPacket(More,Chars,Out):-Less is abs(More),length_init(Added,Less),!,append(Chars,Added,Out).

length_init([],0).
length_init([0],1).
length_init([0,0],2).
length_init([0,0,0],3).
length_init([0,0,0,0],4).
length_init([0,0,0,0|More],N):-NN is N-4,length_init(More,NN).

% ================================================================
% Reading from Ultima Online Client/Server (Binary Mode)
% ================================================================
uoReadPacket(Stream,end_of_file):-at_end_of_stream(Stream),sleep(0.5).
uoReadPacket(Stream,packet(Name,Info,Comment)):-
      get0(Stream,Code),
      writeSTDERRHex([Code]),
      uo_packet(Code,Name,Comment,Bytes,[_|List]),!,
      writeSTDERR(uo_packet(Code,Name,Comment,Bytes,List)),!,
      uoReadPacket2(Stream,Bytes,List,Info).

uoReadPacket2(Stream,packetsize(2),[_|List],Packet):-!,
      get0(Stream,MSB),get0(Stream,LSB),
      Bytes is MSB*256+LSB-3,
      uoReadPacket_list(Stream,Bytes,List,Packet,_).

uoReadPacket2(Stream,Bytes,List,Packet):-
      BytesLeft is Bytes-1,%trace,
      uoReadPacket_list(Stream,BytesLeft,List,Packet,_).


uoReadPacket_list(Stream,0,[],[],0).
uoReadPacket_list(Stream,0,NotDone,[notDone(NotDone)],0).
uoReadPacket_list(Stream,BytesLeft,[],[leftOver(Data)],0):-get_n_codes(Stream,BytesLeft,Data).
uoReadPacket_list(Stream,Bytes,[T:Name,loop:Label|List],[loop(Name,Result,Label)|Rest],BytesDone):-!,
      uoReadPacket_value(Stream,T,Bytes,Name,_:Number,BytesLeft),
      (append(Content,[endloop:Label|Finish],List);append(Content,[endloop:_|Finish],List)),!,
      uoReadPacket_loop(Stream,BytesLeft,Name,Number,Content,Label,Result,BytesLeftOver),
      uoReadPacket_list(Stream,BytesLeftOver,Finish,Rest,BytesDone).
uoReadPacket_list(Stream,Bytes,[T:Name|List],[Result|Rest],BytesDone):-
      uoReadPacket_value(Stream,T,Bytes,Name,Result,BytesLeft),
      uoReadPacket_list(Stream,BytesLeft,List,Rest,BytesDone).

uoReadPacket_loop(Stream,BytesLeft,Name,Label,0,Content,[],BytesLeft).
uoReadPacket_loop(Stream,BytesLeft,Name,Label,N,Content,[Result|More],BytesLeftOver):-
      uoReadPacket_list(Stream,BytesLeft,Content,Result,BytesLeftList),
      NN is N-1,
      uoReadPacket_loop(Stream,BytesLeftList,Name,Label,NN,Content,More,BytesLeftOver).
   
      

% Read rest
uoReadPacket_value(Stream,char- *,Bytes,Name,Name:c(Result),0):-!,get_n_codes(Stream,Bytes,Result).
uoReadPacket_value(Stream,byte- *,Bytes,Name,Name:b(Result),0):-!,get_n_codes(Stream,Bytes,Result).
uoReadPacket_value(Stream,word- *,Bytes,Name,Name:w(Result),0):-!,get_n_codes(Stream,Bytes,Result).
uoReadPacket_value(Stream,dword- *,Bytes,Name,Name:dw(Result),0):-!,get_n_codes(Stream,Bytes,Result).

% Read N
uoReadPacket_value(Stream,char-N,Bytes,Name,Name:c(Result),BytesLeft):-!,get_n_codes(Stream,N,Result),BytesLeft is Bytes-N.
uoReadPacket_value(Stream,byte-N,Bytes,Name,Name:b(Result),BytesLeft):-!,get_n_codes(Stream,N,Result),BytesLeft is Bytes-N.
uoReadPacket_value(Stream,word-N,Bytes,Name,Name:c(Result),BytesLeft):-!,get_w_codes(Stream,N,Result),BytesLeft is Bytes-N*2.
uoReadPacket_value(Stream,dword-N,Bytes,Name,Name:b(Result),BytesLeft):-!,get_d_codes(Stream,N,Result),BytesLeft is Bytes-N*4.

% Read 1
uoReadPacket_value(Stream,char,Bytes,Name,Name:Result,BytesLeft):-!,get0(Stream,Result),BytesLeft is Bytes-1.
uoReadPacket_value(Stream,byte,Bytes,Name,Name:Result,BytesLeft):-!,get0(Stream,Result),BytesLeft is Bytes-1.
uoReadPacket_value(Stream,word,Bytes,Name,Name:Word,BytesLeft):-!,read_word(Stream,Word),BytesLeft is Bytes-2.
uoReadPacket_value(Stream,dword,Bytes,Name,Name:Dword,BytesLeft):-!,read_dword(Stream,Dword),BytesLeft is Bytes-4.


% Readers
read_word(Stream,Word):-!,get0(Stream,MSB),get0(Stream,LSB),Word is MSB*256+LSB.
read_dword(Stream,Dword):-get0(Stream,MSB2),get0(Stream,LSB2),get0(Stream,MSB1),get0(Stream,LSB1),Dword is MSB1*256+LSB1+MSB2*16777216+LSB2*65536.

get_n_codes(Stream,0,[]).
get_n_codes(Stream,1,[Code]):-!,get0(Stream,Code).
get_n_codes(Stream,N,[Code|Ns]):-get0(Stream,Code),NN is N-1,get_n_codes(Stream,NN,Ns).

get_w_codes(Stream,0,[]).
get_w_codes(Stream,1,[Word]):-!,read_word(Stream,Word).
get_w_codes(Stream,N,[Word|Ns]):-read_word(Stream,Word),get_w_codes(Stream,NN,Ns).

get_d_codes(Stream,0,[]).
get_d_codes(Stream,1,[Word]):-!,read_dword(Stream,Word).
get_d_codes(Stream,N,[Word|Ns]):-read_dword(Stream,Word),get_d_codes(Stream,NN,Ns).
   
% ================================================================
% Client/Server Protocol definations
% ================================================================
uo_packet(0x00,'Login','Create a new character',0x68,[byte:'ID(00)',dword:'0xedededed',dword:'0xffffffff',byte:'0',char-60:'Character Name',byte:'Gender',byte:'Strength',byte:'Dexterity',byte:'Intelligence',byte:'Skill 1',byte:'Skill 1 Amount',byte:'Skill 2',byte:'Skill 2 Amount',byte:'Skill 3',byte:'Skill 3 Amount',word:'Skin Color',word:'Hair Style',word:'Hair Color',word:'Beard Style',word:'Beard Color',word:'Starting City',word:'0',word:'Slot',dword:'Client IP',word:'Shirt color',word:'Pants Color']).
uo_packet(0x01,'Logout','Character returns to main menu from character select menu.',5,[byte:'ID(01)',dword:'0xffffffff']). 
uo_packet(0x02,'Req Move','Ask the server if we can walk.',7,[byte:'ID (02)',byte:'Direction',byte:'Sequence Number',dword:'Fastwalk Prevention Key']).
uo_packet(0x03,'Speech','Send speech to the server',packetsize(2),[byte:'ID (03)',word:'Size of Packet',byte:'Mode (0=say 2=emote 8=whipser 9=yell)',word:'Text Color',word:'Font',byte - * :'Text']).  
uo_packet(0x04,'God Mode','Toggles god mode on/off',2,[byte:'ID(04)',byte:'God mode on/off']).  
uo_packet(0x05,'Attack','Send attack to server',5,[byte:'ID(05)',dword:'Serial of character to attack']).
uo_packet(0x06,'Req Obj Use','Ask the server if we can use an object (double click)',5,[byte:'ID(06)',dword:'Serial of item double clicked']).
uo_packet(0x07,'Req Get Obj','Ask the server for item',7,[byte:'ID(07)',dword:'Serial of item',word:'Amount of item']).
uo_packet(0x08,'Req Drop Obj','Ask the server to drop an item',0x0E,[byte:'ID(08)',dword:'Item Serial',word:'X',word:'Y',char:'Z',dword:'Container Serial (-1 = ground)']).
uo_packet(0x09,'Req Look','Ask the server to look at an item (single click)',5,[byte:'ID(09)',dword:'Serial of item single clicked']).
uo_packet(0x0A,'Edit','Edit dynamics and statics Types are: 04 = Dynamic item  07 = NPC  0A = Static Item',0x0B,[byte:'ID (0A)',byte:'Type',word:'X',word:'Y',word:'ID',byte:'Z',word:'Unknown']).
uo_packet(0x0B,'Edit Area','Edit Area',0x0A,[byte:'ID(0B)',byte-9:'Unknown']).
uo_packet(0x0C,'Tile Data','Alter tiledata (send new tiledata to the server)',packetsize(2),[byte:'ID(0C)',word:'Packet Size',word:'Tile ID',byte-37:'New Tile data (see tiledata.mul)',word:'F777']).
uo_packet(0x0D,'NPC Data','Send new NPC data to the server',3,[byte:'ID(0D)',byte-2:'Unknown']).
uo_packet(0x0E,'Template Data','Edit template data',packetsize(2),[byte:'ID(0E)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x0F,'Paperdoll','Paperdoll',0x3D,[byte:'ID(0F)',byte-0x3C:'Unknown']).
uo_packet(0x10,'Hue Data','Modify Hue Data',0xD7,[byte:'ID(10)',byte-0xD6:'Unknown']).
uo_packet(0x11,'Mobile Stat','The status of a character',packetsize(2),[byte:'ID(11)',word:'Packet Size',dword:'Serial',char-30:'Character Name',word:'Hit Points',word:'Max Hit Points',byte:'Allow Name change (FFh = yes)',byte:'Valid stats (1 = yes)',byte:'Gender',word:'Strength',word:'Dexterity',word:'Intelligence',word:'Stamina',word:'Max Stamina',word:'Mana',word:'Max Mana',dword:'Gold',word:'Armor Rating',word:'Weight']).
uo_packet(0x12,'God Command','External command use skill magic etc CommandTypes 0x24 = Use Skill  0x56 = Macro Spell 0x58 = Open Door 0x6b = God Mode Command 0xc7 = Action',packetsize(2),[byte:'ID (12)',word:'Packet Size',byte:'CommandType',byte - * :'Command']).
uo_packet(0x13,'Req Obj Equip','Equip an item',0x0A,[byte:'ID(13)',dword:'Item Serial',byte:'Layer',dword:'Container Serial']).
uo_packet(0x14,'Elev Change','Change Z value of item',6,[byte:'ID(14)',byte-5:'Unknown']).
uo_packet(0x15,'Follow','Follow character',9,[byte:'ID(15)',byte-8:'Unknown']).
uo_packet(0x16,'Req Script Names','Get a list of script names',1,[byte:'ID(16)']).
uo_packet(0x17,'Script Tree Cmd','Script Modification commands',packetsize(2),[byte:'ID(17)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x18,'Script Attach','Add new script to server',packetsize(2),[byte:'ID(18)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x19,'NPC Convo Data','Modify NPC Speech data',packetsize(2),[byte:'ID(19)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x1A,'Move','Move or place an item on the ground',packetsize(2),[byte:'ID(1A)',word:'Packet Size',dword:'Item Serial',word:'Item ID',word:'Item Amount (if Serial&0x80000000)',byte:'Stack ID (if Item ID&0x8000) add to Item ID',word:'X',word:'Y',byte:'Direction (if X&0x8000)',char:'Z',word:'Hue (if Y&0x8000)',byte:'Status if (y&0x4000)']).
uo_packet(0x1B,'Login Confirm','Player initialization',0x25,[byte:'ID(1B)',dword:'Serial',dword:'0',word:'ID',word:'X',word:'Y',byte:'0',char:'Z',byte:'Direction',dword:'7Fh',dword:'0',word:'7',byte:'Status',word:'Highlight Color',word:'0',dword:'0']).
uo_packet(0x1C,'Text','Someone is speaking or the server is sending us info',packetsize(2),[byte:'ID(1C)',word:'Packet Size',dword:'Character Serial',word:'Character ID',byte:'Type',word:'Text Color',word:'Font',char-30:'Name',char - *  :'Text']).
uo_packet(0x1D,'Destroy Object','Remove Item from scene',5,[byte:'ID(1D)',dword:'Item Serial']).
uo_packet(0x1E,'Animate','Control Animation',4,[byte:'ID(1E)',byte-3:'Unknown']).
uo_packet(0x1F,'Explode','Cause explosion',8,[byte:'ID(1F)',byte-7:'Unknown']).
uo_packet(0x20,'Z Move','Character is being moved by the server',0x13,[byte:'ID(20)',dword:'Serial',word:'ID',byte:'0',word:'Skin Color',byte:'Status',word:'X',word:'Y',word:'0',byte:'Direction',char:'Z']).
uo_packet(0x21,'Blocked Move','Server rejected our walk request',8,[byte:'ID(21)',byte:'Sequence Rejected',word:'X location to jump back to',word:'Y',byte:'Direction',char:'Z']).
uo_packet(0x22,'OK Move','Server accepted our walk request',3,[byte:'ID(22)',byte:'Sequence accepted',byte:'Status']).
uo_packet(0x23,'Obj Move','Drag Item',0x1A,[byte:'ID(23)',word:'Item ID',byte:'0',word:'0',word:'Item Amount',dword:'Source Serial',word:'Source X',word:'Source Y',char:'Source Z',dword:'Target Serial',word:'Target X',word:'Target Y',char:'Target Z']).
uo_packet(0x24,'Open Gump','Open a gump',7,[byte:'ID(24)',dword:'Serial',word:'Gump',word:'Packet Size',word:'Number of Items',loop:'Item',dword:'Item Serial',word:'Item ID',byte:'0',word:'Item Amount',word:'Item X',word:'Item Y',dword:'Container Serial',word:'Item Hue',endloop:'Item']).
uo_packet(0x25,'Obj to Obj','Add Object to an Object',0x14,[byte:'ID(25)',dword:'Item Serial',word:'Item ID',byte:'0',word:'Item Amount',word:'Item X',word:'Item Y',dword:'Container Serial',word:'Item Color']).
uo_packet(0x26,'Old Client','Old Client kick',5,[byte:'ID(26)',dword:'Serial of GM who kicked']).
uo_packet(0x27,'Get Obj Failed','Unable to pick up object',2,[byte:'ID(27)',byte:'Where']).
uo_packet(0x28,'Drop Obj Failed','Unable to drop object',5,[byte:'ID(28)',byte-4:'Unknown']).
uo_packet(0x29,'Drop Obj OK','Object dropped ok',1,[byte:'ID(29)']).
uo_packet(0x2A,'Blood','Blood mode',5,[byte:'ID(2A)',byte-4:'Unknown']).
uo_packet(0x2B,'God Mode','God Mode is on/off (server response to packet 04)',2,[byte:'ID(2B)',byte:'God mode on/off']).
uo_packet(0x2C,'Death','Choose resurrection Choice: 0=server ask 1=resurrect 2=ghost',2,[byte:'ID(2C)',byte:'Choice']).
uo_packet(0x2D,'Health','Health',0x11,[byte:'ID(2D)',byte-0x10:'Unknown']).
uo_packet(0x2E,'Equip Item','Character is wearing an item',0x0F,[byte:'ID(2E)',dword:'Item Serial',word:'Item ID',byte:'0',byte:'Layer',dword:'Container Serial',word:'Item Color']).
uo_packet(0x2F,'Swing','Fight swing',0x0A,[byte:'ID(2F)',byte:'0',dword:'Attacker Serial',dword:'Defender Serial']).
uo_packet(0x30,'Attack OK','Attack granted',5,[byte:'ID(30)',byte-4:'Unknown']).
uo_packet(0x31,'Attack End','Attack ended',1,[byte:'ID(31)']).
uo_packet(0x32,'Hack Mover','God mode admin command',2,[byte:'ID(32)',byte:'Unknown']).
uo_packet(0x33,'Group','Group command',2,[byte:'ID(33)',byte:'Command']).
uo_packet(0x34,'Client Query','Get Status  Types: 0x04=Basic Status (Packet 0x11) 0x05=Request Skills (Packet 0x3A)',0x0A,[byte:'ID (34)',dword:'0xedededed',byte:'Type',dword:'Serial']).
uo_packet(0x35,'Resource Type','Get resource type',0x8D,[byte:'ID(35)',byte-0x8C:'Unknown']).
uo_packet(0x36,'Resource Tile Data','Resource tile data',packetsize(2),[byte:'ID(36)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x37,'Move Object','Move an object',8,[byte:'ID(37)',byte-7:'Unknown']).
uo_packet(0x38,'Follow Move','Follow move',7,[byte:'ID(38)',byte-6:'Unknown']).
uo_packet(0x39,'Groups','Groups',9,[byte:'ID(39)',byte-8:'Unknown']).
uo_packet(0x3A,'Skills','Update skills Client version is only ID Packet Size Skill ID and Lock Status',packetsize(2),[byte:'ID(3A)',word:'Packet Size',byte:'List Type (ff=no loop','else loop till ID=0)',loop:'Skill',word:'Skill ID',word:'Skill Value',word:'Base Value',byte:'Lock Status',endloop:'Skill']).
uo_packet(0x3B,'Offer Accept','Accept Offer',packetsize(2),[byte:'ID(3B)',word:'Packet Size',dword:'Vendor Serial',byte:'Num of Items (0=clear message)',loop:'Item',byte:'Layer',dword:'Item Serial',word:'Item Amount',endloop:'Item']).
uo_packet(0x3C,'Multi Obj to Obj','Add objects to object',packetsize(2),[byte:'ID(3C)',word:'Packet Size',word:'Number of Items',loop:'Item',dword:'Item Serial',word:'Item ID',byte:'0',word:'Item Amount',word:'Item X',word:'Item Y',dword:'Container Serial',word:'Item Color',endloop:'Item']).
uo_packet(0x3D,'Ship','Ship',2,[byte:'ID(3D)',byte:'Unknown']).
uo_packet(0x3E,'Versions','Version retrieval',0x25,[byte:'ID(3E)',byte-0x24:'Unknown']).
uo_packet(0x3F,'Upd Obj Chunk','Update object chunk',packetsize(2),[byte:'ID(3F)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x40,'Upd Terr Chunk','Update terrain chunk',0xC9,[byte:'ID(40)',byte-0xC8:'Unknown']).
uo_packet(0x41,'Update Tile Data','Send an updated tiledata.mul entry to the client',packetsize(2),[byte:'ID(41)',word:'Packet Size',word:'Tile ID',byte-37:'Tile Data (see tiledata.mul)',word:'F777']).
uo_packet(0x42,'Update Art','Send updated art to client',packetsize(2),[byte:'ID(42)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x43,'Update Anim','Send new animation data to client',0x29,[byte:'ID(43)',byte-0x28:'Unknown']).
uo_packet(0x44,'Update Hues','Send new hue information to the client',0xC9,[byte:'ID(44)',byte-0xC8:'Unknown']).
uo_packet(0x45,'Ver Ok','Ver Ok',5,[byte:'ID(45)',byte-4:'Unknown']).
uo_packet(0x46,'New Art','Send new artwork',packetsize(2),[byte:'ID(46)',word:'Packet Size',dword:'Tile ID',byte - * :'Art Data (see art.mul)']).
uo_packet(0x47,'New Terr','Send new terrain to the server',0x0B,[byte:'ID(47)',word:'X',word:'Y',word:'Art ID',dword:'Unknown']).
uo_packet(0x48,'New Anim','Send new animation data (static tile animation)',0x49,[byte:'ID(48)',dword:'Tile ID',byte-64:'Frames',byte:'Unknown',byte:'Numer of Frames Used',byte:'Frame Interval',byte:'Start Interval']).
uo_packet(0x49,'New Hues','Send new hues',0x5D,[byte:'ID(49)',dword:'Hue ID',word - 32 : 'Hue Values',word:'Start',word:'End',char - 20:'Hue Name']).
uo_packet(0x4A,'Destroy Art','Destroy artwork',5,[byte:'ID(4A)',dword:'Art ID']).
uo_packet(0x4B,'Check Ver','Check client version',9,[byte:'ID(4B)',byte-8:'Unknown']).
uo_packet(0x4C,'Script Names','Modify script names',packetsize(2),[byte:'ID(4C)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x4D,'Script File','Edit script file',packetsize(2),[byte:'ID(4D)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x4E,'Light Change','Set the personal light level',6,[byte:'ID(4E)',dword:'Serial',byte:'Level']).
uo_packet(0x4F,'Sunlight','Set light level 2 bytes 0 = Bright  1F = Black',2,[byte:'ID(4F)',byte:'Level']).
uo_packet(0x50,'Board Header','Bulletin Board Header',packetsize(2),[byte:'ID(50)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x51,'Board Msg','Bulletin Board Message',packetsize(2),[byte:'ID(51)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x52,'Post Msg','Post Bulletin Board Message',packetsize(2),[byte:'ID(52)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x53,'Login Reject','Login Rejected (1=no character  2=char exists  3-4=Cant connect  5=character already in world  6=login problem  7=idle  8=cant connect)',2,[byte:'ID(53)',byte:'Message']).
uo_packet(0x54,'Sound','Play a sound effect Flags = 0 repeating 1 = single play',0x0C,[byte:'ID(54)',byte:'Flags',word:'Effect',word:'Volume',word:'X',word:'Y',word:'Z']).
uo_packet(0x55,'Login Complete','Begin Game',1,[byte:'ID(55)']).
uo_packet(0x56,'Map Command','Plot course for ships',0x0B,[byte:'ID(56)',dword:'Serial',byte:'Action (1=add 5=delete 6=toggle edit)',byte:'Pin Number',word:'Pin X',word:'Pin Y']).
uo_packet(0x57,'Upd Regions','Update regions',0x6E,[byte:'ID(57)',byte-0x6D:'Unknown']).
uo_packet(0x58,'New Region','Create a new region',0x6A,[byte:'ID(58)',char-40:'Area Name',dword:'0',word:'X',word:'Y',word:'Width',word:'Height',word:'Z1',word:'Z2',char-40:'Description',word:'Sound FX',word:'Music',word:'Night Sound FX',byte:'Dungeon',word:'Light']).
uo_packet(0x59,'New Context FX','Create a new effect',packetsize(2),[byte:'ID(59)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x5A,'Upd Context FX','Update effect',packetsize(2),[byte:'ID(5A)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x5B,'Game Time','Set the time of day',4,[byte:'ID(5B)',byte:'Hours',byte:'Minutes',byte:'Seconds']).
uo_packet(0x5C,'Restart Ver','Resart Ver',2,[byte:'ID(5C)',byte:'Unknown']).
uo_packet(0x5D,'Pre Login','Select the character to play',0x49,[byte:'ID(5D)',dword:'0xEDEDEDED',char-32:'Character Name',dword-7:'Unknown',dword:'Character Index',dword:'Client IP']).
uo_packet(0x5E,'Server List','Server List',packetsize(2),[byte:'ID(5E)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x5F,'Server Add','Add server',0x31,[byte:'ID(5F)',byte-0x30:'Unknown']).
uo_packet(0x60,'Server Remove','Delete a server',5,[byte:'ID(60)',byte-4:'Unknown']).
uo_packet(0x61,'Destroy Static','Delete a static',9,[byte:'ID(61)',word:'X',word:'Y',byte:'Unknown',char:'Z',word:'ID']).
uo_packet(0x62,'Move Static','Move a static',0x0F,[byte:'ID(62)',byte-0x0E:'Unknown']).
uo_packet(0x63,'Area Load','Load an area',0x0D,[byte:'ID(63)',byte-0x0C:'Unknown']).
uo_packet(0x64,'Area Load Req','Attempt to load area',1,[byte:'ID(64)']).
uo_packet(0x65,'Weather Change','Set current weather  4 bytes Type: 0x00 It starts to rain  0x01 = A fierce storm approaches  0x02 = It begins to snow  0x03 = A storm is brewing.  0xff = Stop all weather',4,[byte:'ID(65)',byte:'Weather Type',byte:'Number of weather effects',byte:'Temperature']).
uo_packet(0x66,'Book Page','Show book  As a client message  this writes to the book',packetsize(2),[byte:'ID(66)',word:'Packet Size',dword:'Item Serial',word:'Number of Pages',loop:'Page',word:'Page Index',word:'Number of Lines',loop:'Line',byte - * :'Text',endloop:'Line',endloop:'Page']).
uo_packet(0x67,'Simped','Simped',0x15,[byte:'ID(67)',byte-0x14:'Unknown']).
uo_packet(0x68,'Script LS Attach','Add LS script',packetsize(2),[byte:'ID(68)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x69,'Friends','Unknown',packetsize(2),[byte:'ID(69)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x6A,'Friend Notify','Notify Friend',3,[byte:'ID(6A)',byte-2:'Unknown']).
uo_packet(0x6B,'Key Use','Use Key',9,[byte:'ID(6B)',byte-8:'Unknown']).
uo_packet(0x6C,'Target','Bring up targeting cursor',0x13,[byte:'ID(6C)',byte:'Target Type',dword:'Character Serial',byte:'Check Crime',dword:'Item Serial',word:'X (ffff=cancel)',word:'Y (ffff=cancel)',byte:'Unknown',byte:'Z']).
uo_packet(0x6D,'Music','Play a midi music file',3,[byte:'ID(6D)',word:'Midi Song']).
uo_packet(0x6E,'Anim','Show an animation',0x0E,[byte:'ID(6E)',dword:'Serial',word:'Action',word:'5',word:'Direction',word:'Repeat Times',byte:'Forward (0=forward 1= backward)',byte:'Repeat Flag',byte:'Delay (0=fast','0xff=slow)']).
uo_packet(0x6F,'Trade','Open trade window  Type: 0 = Start Trading  1 = Cancel Trade  2 = Reset Checkmarks',packetsize(2),[byte:'ID (6F)',word:'Packet Size',byte:'Type (0=start','1=cancel','2=change checkmarks)',dword:'Character Serial',dword:'Item 1 Serial (type0 only)',dword:'Item 2 Serial (type0 only)',byte:'Name Included (type 0 only)',char-30:'Name (type 0 only','only if Name Included)']).
uo_packet(0x70,'Effect','Play a special effect (like magic) Type: 00 = Go from source to dest 01 = Lightning strike  02 = Stay at location 03 = Stay with source',0x1C,[byte:'ID (70)',byte:'Type',dword:'Character Serial',dword:'Target Serial',word:'Object ID',word:'X',word:'Y',char:'Z',word:'Target X',word:'Target Y',char:'Target Z',word:'Speed',word:'0',word:'Explode']).
uo_packet(0x71,'BBoard','Display a message board   Type 0=draw board  1=get posts  2-3=get body  4=ack dl  5=post  6=delete',packetsize(2),[byte:'ID (71)',word:'Packet Size',byte:'Type']).
uo_packet(v([0x71,_,_,0]),'Draw Board','Draw the Bulletin board',packetsize(2),[byte:'ID(71)',word:'Packet Size',byte:'Type (0)',dword:'Board Serial',char-22:'Board Name',dword:'ID',dword:'0']).
uo_packet(v([0x71,_,_,1]),'Message List','List of bulletin board messages',packetsize(2),[byte:'ID(71)',word:'Packet Size',byte:'Type (1)',dword:'Board Serial',dword:'Message Serial',dword:'Container Serial (0 = top level)',byte:'Name Length',char - *  :'Name',byte:'Subject Length',char - *  :'Subject',byte:'Time Length',char - *  :'Time']).
uo_packet(v([0x71,_,_,2]),'Message','A single message on a bulletin board',packetsize(2),[byte:'ID(71)',word:'Packet Length',byte:'Type (2)',dword:'Board Serial',dword:'Message Serial',byte:'Name Length',char - *  :'Name',byte:'Subject Length',char - *  :'Subject',byte:'Time Length',char - *  :'Time',byte-29:'Unknown',byte:'Num Lines',loop:'Line',byte:'Line Length',char - *  :'Line Text',endloop:'Line']).
uo_packet(v([0x71,_,_,3]),'Get Message','Ask the server for a message',packetsize(2),[byte:'ID(71)',word:'Packet Size',byte:'Type (3)',dword:'Board Serial',dword:'Message Serial']).
uo_packet(v([0x71,_,_,4]),'Get Summary','Get a bulletin boards message summary',packetsize(2),[byte:'ID(71)',word:'Packet Size',byte:'Type (4)',dword:'Board Serial',dword:'Message Serial']).
uo_packet(v([0x71,_,_,5]),'Post Message','Post a message to a bulletin board',packetsize(2),[byte:'ID(71)',word:'Packet Size',byte:'Type (5)',dword:'Board Serial',dword:'Reply Message Serial (0 if no reply)',byte:'Subject Length',char - *  :'Subject',byte:'Num Lines',loop:'Line',byte:'Line Length',char - *  :'Line Text',endloop:'Line']).
uo_packet(v([0x71,_,_,6]),'Delete Message','Delete a posted message',packetsize(2),[byte:'ID(71)',word:'Packet Size',byte:'Type (6)',dword:'Board Serial',dword:'Message Serial']).
uo_packet(0x72,'Combat','Set war mode and stuff',5,[byte:'ID(72)',byte:'War Mode',word:'Unknown',byte:'Unknown']).
uo_packet(0x73,'Ping','Ping and Pong',2,[byte:'ID(73)',byte:'Value']).
uo_packet(0x74,'Shop Data','Purchase details',packetsize(2),[byte:'ID(74)',word:'Packet Size',dword:'Vendor Serial',byte:'Number of Items',loop:'Item',dword:'Price',byte:'Name Length',char - *  :'Name',endloop:'Item']).
uo_packet(0x75,'Rename MOB','Rename character',0x23,[byte:'ID(75)',dword:'Serial',char-30:'Name']).
uo_packet(0x76,'Server Change','Server change',0x10,[byte:'ID(76)',byte-0x0F:'Unknown']).
uo_packet(0x77,'Naked MOB','Another character or monster is walking (why is this called Naked MOB?)',0x11,[byte:'ID(77)',dword:'Serial',word:'ID',word:'X',word:'Y',char:'Z',byte:'Direction',word:'Skin Color',byte:'Status',byte:'Notoriety (Murderer','Friend etc)']).
uo_packet(0x78,'Equipped MOB','Add a character to the scene',packetsize(2),[byte:'ID(78)',word:'Packet Size',dword:'Serial',word:'ID',word:'Amount/Corpse (if Serial&0x80000000)',word:'X',word:'Y',char:'Z',byte:'Direction',word:'Skin Color',byte:'Status',byte:'Notoriety',loop:'Item (while Serial!=0)',dword:'Item Serial',word:'Item ID',byte:'Item Layer',word:'Item Color (if Item ID&0x8000)',endloop:'Item']).
uo_packet(0x79,'Resource Query','Get Resource',9,[byte:'ID(79)',dword:'Unknown',dword:'Unknown']).
uo_packet(0x7A,'Resource Data','Resource Data',packetsize(2),[byte:'ID(7A)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x7B,'Sequence','Sequence',2,[byte:'ID(7B)',byte:'Unknown']).
uo_packet(0x7C,'Obj Picker','Display a Gump with options (like the Add menu)',packetsize(2),[byte:'ID(7C)',word:'Packet Size',dword:'Gump Serial',word:'Gump ID',byte:'Title Length',byte - * :'Title',byte:'Number of Lines',loop:'Line',word:'Choice ID',word:'Checked',byte:'Line Length',char - *  :'Text',endloop:'Line']).
uo_packet(0x7D,'Picked Obj','Choose an option from 7C',0x0D,[byte:'ID(7D)',dword:'Gump Serial',word:'Gump ID',word:'Item ID',dword:'Unknown']).
uo_packet(0x7E,'God View Query','Get god view data',2,[byte:'ID(7E)',byte:'God view stat id']).
uo_packet(0x7F,'God View Data','God view data',packetsize(2),[byte:'ID(7F)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x80,'Acct Login Req','Login to the login server.',0x3E,[byte:'ID(80)',char-30:'Name',char-30:'Password',byte:'Unknown']).
uo_packet(0x81,'Acct Login Ok','Login ok',packetsize(2),[byte:'ID(81)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x82,'Acct Login Fail','There was an error logging in 0=No Account  1=Account in Use  2=Account Blocked   3=No Password',2,[byte:'ID(82)',byte:'Error']).
uo_packet(0x83,'Acct Del Char','Delete a player',0x27,[byte:'ID(83)',byte-30:'Password',dword:'Character Index',dword:'Client IP']).
uo_packet(0x84,'Chg Char PW','Change password',0x45,[byte:'ID(84)',byte-0x44:'Unknown']).
uo_packet(0x85,'Chg Char Result','Result of Change character request',2,[byte:'ID(85)',byte:'Unknown']).
uo_packet(0x86,'All Characters','Resend Characters after delete',packetsize(2),[byte:'ID(86)',word:'Packet Size',byte:'Number of Characters',loop:'Character',char-30:'Name',char-30:'Password',endloop:'Character']).
uo_packet(0x87,'Send Resources','Send resources',packetsize(2),[byte:'ID(87)',word:'Packet Size',word:'0x101',word:'0',byte:'Unknown',dword:'X',dword:'Y',dword:'X2',dword:'Y2',byte:'Name Length',char - *  :'Name']).
uo_packet(0x88,'Open Paper Doll','Display paper doll',0x42,[byte:'ID(88)',dword:'Character Serial',char-60:'Name',byte:'Status']).
uo_packet(0x89,'Corpse EQ','Corpses backpack',packetsize(2),[byte:'ID(89)',word:'Packet Size',dword:'Container Serial',loop:'Item (while Layer!=0)',byte:'Layer',dword:'Item Serial',endloop:'Item']).
uo_packet(0x8A,'Trigger Edit','Edit Trigger',packetsize(2),[byte:'ID(8A)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x8B,'Display Sign','Show Sign',packetsize(2),[byte:'ID(8B)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x8C,'Agent Server','Relay to the game server.',0x0B,[byte:'ID(8C)',dword:'Server IP',word:'Server Port',dword:'Auth ID']).
uo_packet(0x8D,'Unused3','Unsued',packetsize(2),[byte:'ID(8D)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x8E,'Move Character','Move Character',packetsize(2),[byte:'ID(8E)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x8F,'Unused4','Unused',packetsize(2),[byte:'ID(8F)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x90,'Open Course Gump','Show map for plotting etc.',0x13,[byte:'ID(90)',dword:'Serial',word:'Corner Image',word:'X1',word:'Y1',word:'X2',word:'Y2',word:'Width',word:'Height']).
uo_packet(0x91,'Post Login','Login to the game server',0x41,[byte:'ID(91)',dword:'Auth ID',char - 30:'Name',char - 30:'Password']).
uo_packet(0x92,'Upd Multi','Update Multi',packetsize(2),[byte:'ID(92)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x93,'Book Hdr','Open a book',0x63,[byte:'ID(93)',dword:'Item Serial',byte:'Editable',byte:'1',word:'Number of Pages',char-60:'Title',char-30:'Author']).
uo_packet(0x94,'Upd Skill','Alter skill',packetsize(2),[byte:'ID(94)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x95,'Hue Picker','Dye',9,[byte:'ID(95)',dword:'Item Serial',word:'Item ID',word:'Color']).
uo_packet(0x96,'Game Cent Mon','Monitor game mostly through godview',packetsize(2),[byte:'ID(96)',word:'Packet Size',byte:'Command',byte - * :'Unknown']).
uo_packet(0x97,'Player Move','Player move',2,[byte:'ID(97)',byte:'Unknown']).
uo_packet(0x98,'MOB Name','Alter name',packetsize(2),[byte:'ID(98)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x99,'Target Multi','Targeting cursor for multis',0x1A,[byte:'ID(99)',byte:'Allow Ground',dword:'Deed Serial',dword:'0',dword:'0',dword:'0',word:'Multi ID',dword:'0',word:'0']).
uo_packet(0x9A,'Text Entry','Text Entry',packetsize(2),[byte:'ID(9A)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0x9B,'Request Assist','Page a GM',2,[byte:'ID(9B)',byte:'Unknown']).
uo_packet(0x9C,'Assist Request','Assistance response',0x35,[byte:'ID(9C)',byte-0x34:'Unknown']).
uo_packet(0x9D,'GM Single','GM Single',0x33,[byte:'ID(9D)',byte-0x32:'Unknown']).
uo_packet(0x9E,'Shop Sell','Details for sales',packetsize(2),[byte:'ID(9E)',word:'Packet Size',dword:'Vendor Serial',word:'Number of Items',loop:'Item',dword:'Item Serial',word:'Item ID',word:'Item Color',word:'Item Amount',word:'Value',word:'Name Length',char - *  :'Name',endloop:'Item']).
uo_packet(0x9F,'Shop Offer','Sell window',packetsize(2),[byte:'ID(9F)',word:'Packet Size',dword:'Vendor Serial',byte:'Unknown',byte:'Number of Items',loop:'Item',dword:'Item Serial',word:'Item Amount',endloop:'Item']).
uo_packet(0xA0,'Britannia Select','Select a shard to play on',3,[byte:'ID(A0)',word:'Server Index']).
uo_packet(0xA1,'HP Health','Adjust strength and Hit Points',9,[byte:'ID(A1)',dword:'Serial',word:'Strength',word:'Hit Points']).
uo_packet(0xA2,'Mana Health','Update Intelligence and Mana',9,[byte:'ID(A2)',dword:'Serial',word:'Intelligence',word:'Mana']).
uo_packet(0xA3,'Fat Health','Update Dexterity and Stamina',9,[byte:'ID(A3)',dword:'Serial',word:'Dexterity',word:'Stamina']).
uo_packet(0xA4,'Hardware Info','Send system info to the server',0x95,[byte:'ID(A4)',byte-0x94:'Unknown']).
uo_packet(0xA5,'Web Browse','Launch a browser and to to URL',packetsize(2),[byte:'ID(A5)',word:'Packet Size',char - *  :'URL']).
uo_packet(0xA6,'Message','Show a scroll on the screen',packetsize(2),[byte:'ID(A6)',word:'Packet Size',byte:'Font',word:'0',word:'Tip Number',word:'Text Length',char - *  :'Text']).
uo_packet(0xA7,'Req Tip','Get a tooltip',4,[byte:'ID(A7)',word:'Tip',byte:'0 = tip 1 = notice']).
uo_packet(0xA8,'Britannia List','List the available shards',packetsize(2),[byte:'ID(A8)',word:'Packet Size',byte:'FFh',word:'Number of Servers',loop:'Server',word:'Server Index',char-32:'Server Name',byte:'Percent Full',byte:'Timezone',dword:'Server IP',endloop:'Server']).
uo_packet(0xA9,'Cities and Chars','List of characters as well as starting cities  Flags: 0x8 = enable popups  0x4 = server limits number of characters per account (flags>>4 = number of characters)',packetsize(2),[byte:'ID(A9)',word:'Packet Size',byte:'Number of Characters',loop:'Character',char-60:'Character Name',endloop:'Character',byte:'Number of Cities',loop:'City',byte:'Index',char-31:'City Name',char-31:'Area of Town',endloop:'City',dword:'Flags (May not be present)']).
uo_packet(0xAA,'Current Target','Current attack target',5,[byte:'ID(AA)',dword:'Serial (0 = attack invalid)']).
uo_packet(0xAB,'String Query','Text Entry',packetsize(2),[byte:'ID(AB)',word:'Packet Size',dword:'Serial',byte:'Parent ID',byte:'Buttom ID',word:'Text Length',char - *  :'Text',byte:'Style (0=none','1=normal','2=numerical)',dword:'Max Length',word:'Label Length',char - *  :'Label']).
uo_packet(0xAC,'String Response','Text Entry Response',packetsize(2),[byte:'ID(AC)',word:'Packet Size',dword:'Serial',byte:'Type',byte:'Index',byte:'Mode (0=cancel','1=ok)',word:'Text Length',char - *  :'Text']).
uo_packet(0xAD,'Speech Unicode','Speak in unicode -  If Mode&0xc0 then there are keywords (from speech.mul) present. Keywords: The first 12 bits = the number of keywords present. The keywords are included right after this each one is 12 bits also. The keywords are padded to the closest byte.  For example if there are 2 keywords it will take up 5 bytes.  12bits for the number  and 12 bits for each keyword.  12+12+12=36.  Which will be padded 4 bits to 40 bits or 5 bytes.',packetsize(2),[byte:'ID(AD)',word:'Packet Length',byte:'Mode (0=say','2=emote','8=whisper','9=yell) *',word:'Text Color',word:'Font',char-4:'Language',byte - * :'Keywords (Only present if mode&0xc0)',char - *  :'Text']).
uo_packet(0xAE,'Text Unicode','Someone is speaking in Unicode',packetsize(2),[byte:'ID(AE)',word:'Packet Size',dword:'Serial',word:'ID',byte:'Type',word:'Text Color',word:'Font',char-4:'Language',char-30:'Name',char - *  :'Text']).
uo_packet(0xAF,'Death Anim','Death Anim',0x0D,[byte:'ID(AF)',dword:'Character Serial',dword:'Corpse Serial',dword:'0']).
uo_packet(0xB0,'Generic Gump','Show Gump',packetsize(2),[byte:'ID(B0)',word:'Packet Size',dword:'Serial',dword:'Gump ID',dword:'6Eh',dword:'46h',word:'Layout Length',char - *  :'Layout',word:'Number of Lines',loop:'Line',word:'Line Length',word - *  :'Text',endloop:'Line']).
uo_packet(0xB1,'Gen Gump Trig','Generic Gump Choice',packetsize(2),[byte:'ID(B1)',word:'Packet Size',byte - * :'Button Data']).
uo_packet(0xB2,'Chat Msg','Chat Message',packetsize(2),[byte:'ID(B2)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0xB3,'Chat Text','Chat Text',packetsize(2),[byte:'ID(B3)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0xB4,'Target Obj List','Target object list',packetsize(2),[byte:'ID(B4)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0xB5,'Chat Open','Chat open',0x40,[byte:'ID(B5)',byte-0x39:'Unknown']).
uo_packet(0xB6,'Help Request','Show Popup Help',9,[byte:'ID(B6)',dword:'Help Serial',byte:'Langauge Number',char-3:'Language']).
uo_packet(0xB7,'Help Unicode Text','Display Popup help',packetsize(2),[byte:'ID(B7)',word:'Packet Size',dword:'Help Serial',word - *  :'Unicode Text']).
uo_packet(0xB8,'Char Profile','Profile',packetsize(2),[byte:'ID(B8)',word:'Packet Size',byte:'Mode',dword:'Serial']).
uo_packet(0xB9,'Features','Feature Enable  Bit 1 = Enable T2A Features (chat)  Bit 2 = Enable LBR Features (animations)',3,[byte:'ID (B9)',word:'Enable']).
uo_packet(0xBA,'Pointer','Display Quest Pointer',6,[byte:'ID(BA)',byte:'Active',word:'X',word:'Y']).
uo_packet(0xBB,'Acct ID','Account ID',9,[byte:'ID(BB)',byte-8:'Unknown']).
uo_packet(0xBC,'Game Season','Season',3,[byte:'ID(BC)',byte:'Season',byte:'Cursor']).
uo_packet(0xBD,'Client Version','Send client version to the server',packetsize(2),[byte:'ID(BD)',word:'Packet Size',char - *  :'Version String']).
uo_packet(0xBE,'Assist Version','Assist Version',packetsize(2),[byte:'ID(BE)',word:'Packet Size',byte - * :'Unknown']).
uo_packet(0xBF,'Generic Command','Miscellaneous Commands',packetsize(2),[byte:'ID(BF)',word:'Packet Size',word:'Command']).
uo_packet(v([0xBF,_,_,1]),'Fast Walk','Cycles through the keys in the stack when walking.',packetsize(2),[byte:'ID(BF)',word:'Packet Size',word:'Command (1)',dword-6:'Key Stack']).
uo_packet(v([0xBF,_,_,13]),'Unknown','Unknown packet The client sends this packet just after it sends packet 09  for the same serial.',packetsize(2),[byte:'ID(BF)',word:'Size',word:'Command (13)',dword:'Serial']).
uo_packet(v([0xBF,_,_,18]),'Enable Map Diffs','This packet is sent by the server to the client telling the client to use the mapdif* and stadif* files to patch the map and statics. Usually there are 3 maps in this packet 0 = fellucca 1 = trammel and 2 = ilshenar.',packetsize(2),[byte:'ID(BF)',word:'Size',word:'Subcommand (18)',dword:'Number of Maps',loop:map,dword:'Number of Map patches in this map',dword:'Number of Static patches in this map',endloop:map]).
uo_packet(0xCB,'GQ Count','GQ Count',7,[byte:'ID(CB)',byte-6:'Unknown']).
uo_packet(0xChannel,Agent,'Text ID Plus String','Text ID and String',packetsize(2),[byte:'ID(CC)',word:'Packet Size',byte - * :'Unknown']).

