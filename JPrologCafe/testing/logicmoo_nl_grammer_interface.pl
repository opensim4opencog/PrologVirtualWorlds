
:-include('logicmoo_utils_header.pl'). %<?

tgram1:- tgram('How dynamic is Major Tom when he floats above the eastern sky?').
tgram2:- tgram('How dynamic is Major Tom?').
tgram3:- tgram('The cat sat on the mat.').   
tgram4:- tgram('The cat sat.').
   


%['EVENT'('PERSON'(['I']), 'ACTION'([feel], 'PROPERTY'([good])))]


tgram(Text):-
   tgram(Text,Structure),
   writeq(Structure),!.

tgramS(Text,Semantic1,Assertions):-
   tgramS1(Text,Semantic,Assertions),
   tgramS2(Semantic,Semantic1),!.


tgramS1(Text,Semantic,Assertions):-
   tgram(Text,linkgram(Structure)),
   member(interpertation(I),Structure),
   member('Semantic'(Semantic),I),
   (member(Assertions,I),is_list(Assertions);Assertions=[]),!.

   
tgramS2([X],Word):-!,tgramS2(X,Word).
tgramS2(Semantic,Semantic).

tgram(Text,Nf):-
   http_get_text_hardcoded(Text,Stream,Socket),
   %writeq(Stream),
   load_xml_file(Stream,XMLStructure),
   catch(ignore(tcp_close_socket(Socket)),_,true),
   xml_grammer_to_semantics(XMLStructure,Nf).



xml_interpretation(Atom,Atom):-atom(Atom),!.
xml_interpretation(linkgram(Atom),O):-!,xml_interpretation((Atom),O).
xml_interpretation([H|T],TT):-atom(H),!,xml_interpretation(T,TT).
xml_interpretation([element(H,_,_)|T],TT):-
	 member(H,[wordset,linkcount]),!,xml_interpretation(T,TT).
xml_interpretation([H|T],[HH|TT]):-
      xml_interpretation_each(H,HH),xml_interpretation(T,TT).

%xml_interpretation([_|T],TT):-xml_interpretation(T,TT).

xml_interpretation_each(element(xdata,[],StructureIn),StructureOut):-!,xml_interpretation_list(StructureIn,StructureOut).
xml_interpretation_each(element(synparse,[],StructureIn),'Syntax'(StructureOut)):-!,xml_interpretation_list(StructureIn,StructureOut).
xml_interpretation_each(element(semparse,[],StructureIn),'Semantic'(StructureOut)):-!,xml_interpretation_list(StructureIn,StructureOut).


xml_interpretation_list([],[]).
xml_interpretation_list([H|T],TT):-noH(H),!,xml_interpretation_list(T,TT).
xml_interpretation_list([H|T],[HH|TT]):-xml_interpretation_list(H,HH),xml_interpretation_list(T,TT).
xml_interpretation_list(element(xrec,[],StructureIn),meaning(Word,POS,DictListO)):-
	 member(element(lex,[],Word),StructureIn),
	 (member(element(pos,[],POS),StructureIn);POS=asIs),
	 member(element(dic,[],DictList),StructureIn),!,
	 xml_each_dictlist(DictList,DictListO).      
xml_interpretation_list(element(Atom,[],O),H):-xml_interpretation_list(O,Out),
	 joinH(Atom,Out,H).
xml_interpretation_list(A,H):-wnSplit(A,H).

joinH(Atom,[Out],H):- H=..[Atom,Out].
joinH(Atom,Out,H):- H=..[Atom|Out].


wnSplit(Atom,List):-
      atom_codes(Atom,X),
      subst(X,95,32,Y),
      getWordTokens(Y,List),!.

readWebpage(URL):-readWebpage(URL,After),writeq(After).

readWebpage(URL,After):-
	readWebpageML(URL,XMLStructure),!,
	xml_grammer_to_semantics(XMLStructure,After).

readWebpageML(URL,XMLStructure):-
      http_get_stream(URL,Stream),
         load_html_file(Stream,XMLStructure),
      catch(ignore(tcp_close_socket(Socket)),_,true),!.

      


   
http_get_text_hardcoded(Text,ReadFd,Socket):-
   tcp_socket(Socket),
   tcp_connect(Socket,'4.33.94.173':2002),
   tcp_open_socket(Socket, ReadFd, WriteFd),
   format(WriteFd,'GET/XML?pos="~w"~n~n',[Text]),
   flush_output(WriteFd),!.

http_get_text(Text,Stream):-
   www_form_encode(Text,TextCoded),
   sformat(S,'http://4.33.94.173:2002/XML?pos="~w"',[TextCoded]),
   http_get_stream(S,Stream).

http_get_stream(URL,ReadFd):-
   parse_url(URL,Structure),
   member(host(Host),Structure),
   (member(port(Port),Structure);Port=80),
   http_location(Structure, LocationCodes),
   tcp_socket(Socket),
   tcp_connect(Socket,Host:Port),
   tcp_open_socket(Socket, ReadFd, WriteFd),
   format(WriteFd,'GET ~w~n~n',[LocationCodes]),
   flush_output(WriteFd),!.


xml_grammer_to_semantics_next([(I)],O):-
      xml_grammer_to_semantics_next(I,O),!.
xml_grammer_to_semantics_next(element(html,X,I),O):-
      xml_grammer_to_semantics_next(I,O),!.
xml_grammer_to_semantics_next(html(I),O):-
      xml_grammer_to_semantics_next(I,O),!.
xml_grammer_to_semantics_next(linkgram(I),O):-
      xml_grammer_to_semantics_next(I,O),!.
xml_grammer_to_semantics_next([H|T],O):-
      member(interpertation(I),[H|T]),!,
      xml_grammer_to_semantics_next(I,O),!.
xml_grammer_to_semantics_next([H|T],O):-
      member(element(Get,_,I),[H|T]),member(Get,[body,center]),
      xml_grammer_to_semantics_next(I,O),!.
xml_grammer_to_semantics_next([H|T],O):-
      member('Semantic'(I),[H|T]),!,
      xml_grammer_to_semantics_next(I,O),!.
xml_grammer_to_semantics_next([H|T],O):-
      member('body'(I),[H|T]),!,
      xml_grammer_to_semantics_next(I,O),!.
xml_grammer_to_semantics_next([H|T],O):-
      member('Body'(I),[H|T]),!,
      xml_grammer_to_semantics_next(I,O),!.
xml_grammer_to_semantics_next(O,O).


xml_grammer_to_semantics(I,O):-
      xml_grammer_to_semantics_first(I,M),!,
      xml_grammer_to_semantics_next(M,O).



xml_grammer_to_semantics_first(linkgram(I),O):-
      xml_grammer_to_semantics_first(I,O),!.
xml_grammer_to_semantics_first([],[]).
xml_grammer_to_semantics_first([element(linkgram,_,StructureIn)|_],linkgram(StructureOut)):-!,
      xml_grammer_to_semantics_first(StructureIn,StructureOut).

xml_grammer_to_semantics_first([element(interpertation,_,H)|T],[interpertation(HH)|TT]):-
	 xml_interpretation(H,HH),
	 xml_grammer_to_semantics_first(T,TT).	 
%xml_grammer_to_semantics_first([_|T],TT):-!,xml_grammer_to_semantics_first(T,TT).
xml_grammer_to_semantics_first(Structure,Structure).



xml_each_dictlist([],[]).
xml_each_dictlist([H|T],TT):-noH(H),!,xml_each_dictlist(T,TT).
xml_each_dictlist([H|T],[HH|TT]):-xml_each_dictlist(H,HH),xml_each_dictlist(T,TT).
xml_each_dictlist(element(_,_,[DictListO]),DictListO).
xml_each_dictlist(DictListO,DictListO).

noH('\n').
noH('\n\n').
noH(' \n').
noH(' \n\n').
noH('\n ').
noH('\n  ').
noH('\n\n ').
noH(' ').
noH('  ').

atom_trim(Atom,Trimed):-atom_codes(Atom,Codes),
      codes_trim(Codes,Trim),
      reverse(Trim,R),
      codes_trim(R,RTrimR),
      reverse(RTrimR,RTrim),
      atom_codes(Trimed,RTrim),!.

codes_trim([],[]).
codes_trim([C|Cs],CCs):-char_type(C,white),!,codes_trim(Cs,CCs).
codes_trim(CCs,CCs).

%codes_trim([C|Cs],[C|CCs]):-!,codes_trim(Cs,CCs).
   
      

