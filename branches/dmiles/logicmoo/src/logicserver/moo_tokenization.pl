/*





  Sicstus and SWI-Prolog Compatible Code





     - by Douglas Miles 1991-1999


     - admin@vast1.com





*/


:-module(tokenization,[


	periodize/2,


%	term_to_atom/2,


	chars_to_atom/2,


	chars_to_term/2,


	term_to_chars/2,


	term_to_string/2,


	atom_to_term/2,


	atom_to_chars/2,


	free/1,


	any_to_words/2,


	is_whitespace/1,


	is_string/1,


	is_digit/1,


	is_letter/1,


	is_alphanum/1,


	string_form/2,


	string_to_words/2,


	tolower/2,


	text/1,


        wt/2,


        tw/2,


        wn/2,


        any_to_tokens/2,


        any_to_string/2,


        words_to_tokens/2,


        tokens_to_words/2,


        prevail/2,


  %      best_words/2,


	toupper/2,


        precharged_wn/2


]).


	


toupper([],[]):-!.


toupper([X|Xs],[Y|Ys]):-!,


        ((X>96, X<123, Y is X - 32);(Y =X)),


        toupper(Xs,Ys).





%:-use_module(library(lists)).


%:-use_module(library(charsio)).





atom_to_term(Atom,Term):-catch(atom_term(Atom,Term),_,fail).





periodize(Chars,Chars):-last(Chars,46),!.


periodize(Chars,Period):-!,append(Chars,[46],Period).


				


%term_to_atom(Term,Atom):-term_atom(Term,Atom).


	 %  format_to_chars("~p",[Term],Chars),name(Atom,Chars).





chars_to_atom(Chars,Atom):-name(Atom,Chars).





atom_to_chars(Atom,Chars):- %swritef(Chars,"%t",[ATOM]).


	name(Atom,Chars). % /2 built-in





chars_to_term(Chars,Term):-


	chars_to_atom(Chars,Atom),


	atom_to_term(Atom,Term).





read_from_chars(Chars,Term):-chars_to_term(Chars,Term).





term_to_chars(Term,Chars):- %swritef(Chars,"%t",[Term]).


	term_to_atom(Term,Atom),


	atom_to_chars(Atom,Chars).


	%format_to_chars("~p",[Term],Chars).





term_to_string(Term,Chars):-term_to_chars(Term,Chars).








free(X):-not(ground(X)).





% :-[replace].





substr(A,B):-string_to_list(A,L),subset(B,L).





is_string([A,B]):-!,integer(A),integer(B),


        A<256,B<256,A>(-1),B>(-1),C is A + B, C>13.


is_string([A|L]):-!,integer(A),A<256,A>(-1),is_string(L).





is_whitespace(WS):-!,member(WS,[32|"_,.-"]);WS<32;WS>127.





is_letter(LC):-!,


  member(LC,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ#").





is_digit(DC):-!,member(DC,"0123456789").





is_alphanum(AN):-is_letter(AN);is_digit(AN).





string_form(X,Y):-str_form(X,Y).





str_form([],[]):-!.


str_form([White1,White2|Xs],Ys):-


        is_whitespace(White1),is_whitespace(White2),!,


        str_form([32|Xs],Ys).


str_form([White|Xs],[32|Ys]):-is_whitespace(White),!,str_form(Xs,Ys).


str_form([40,X,41,46|Xs],Ys):-is_letter(X),!,str_form(Xs,Ys).


str_form([X|Xs],[32,X,32|Ys]):-


        not(is_alphanum(X)),!,


        str_form(Xs,Ys).


str_form([N,A|Xs],[N,32,A|Ys]):-


        ((is_letter(A),is_digit(N));(is_letter(N),is_digit(A))),!,


        str_form(Xs,Ys).


str_form([X|Xs],[X|Ys]):-!,str_form(Xs,Ys).

any_to_words(ANY,WORDS):-any_to_string(ANY,STR),!,


        str_form(STR,STR2),!,tolower( STR2,STR3 ),!,


        string_to_words(STR3,WORDS).





string_to_words([],[]):-!.


string_to_words([32|S],W):-!,string_to_words(S,W).


string_to_words(STR,[WORD]):-not(member(32,STR)),!,name(WORD,STR).


string_to_words(STR,[W|Ws]):-append(S,[32|Ss],STR),!,


        name(W,S),string_to_words(Ss,Ws).





tolower([],[]):-!.


tolower([A|M],[B|L]):- A <91, A>64,!, B is A + 32, tolower(M,L).


tolower([A|M],[A|L]):- !,tolower(M,L).





text([A]):-!,integer(A).


text([A|L]):-!,integer(A),text(L).








/*





     - by Douglas Miles 1991-1999


     - admin@vast1.com





   This is a going to become a separate module with its own PID


	and stack..





	As the Entire semantic Network is in SynsetId form,


	we need to be sure not give "text" to any learning


	centers





   FYI: priv_cmpd,priv_cmpv will be seen throughout code perposely


        as it will be an extern call to a C module





*/


/*


:-module(tokenization,[


	wt/2,


	tw/2,


	wn/2,


	any_to_tokens/2,


	any_to_string/2,


	words_to_tokens/2,


	tokens_to_words/2,


	prevail/2,


	best_words/2,


	precharged_wn/2


	]).


*/








tw([Token],WordL):-Table is Token // 100000000, tw([Token],WordL,Table).





tw(Token,WordL,1):-!,w_n(WordL,Token).


tw(Token,WordL,2):-!,w_v(WordL,Token).


tw(Token,WordL,3):-w_a(WordL,Token),!.


tw(Token,WordL,3):-!,w_as(WordL,Token).


tw(Token,WordL,4):-!,w_av(WordL,Token).





wn(Word,Token):-precharged_wn(Word,Token).


wn(Word,Token):-wt([Word],[Token]).





wt(Word,Token):-w_v(Word,Token).


wt(Word,Token):-w_n(Word,Token).


wt(Word,Token):-w_a(Word,Token).


wt(Word,Token):-w_as(Word,Token).


wt(Word,Token):-w_av(Word,Token).





any_to_tokens(Variant,Tokens):-


	any_to_words(Variant,Words),


	words_to_tokens(Words,Tokens).





any_to_string([],""):-!.


any_to_string(Str,Str):-is_string(Str),!.


any_to_string(N,Str):-integer(N),N>9999999,tw([N],[Atom|_]),name(Atom,Str),!.


any_to_string([D],Str):-!,any_to_string(D,Str).


any_to_string([D|M],Str):-!,any_to_string(D,DS),any_to_string(M,MS),


        append(DS,[32|MS],Str).


any_to_string(Atom,Str):-atomic(Atom),!,name(Atom,Str).


any_to_string(Term,Str):-term_to_chars(Term,Str).





words_to_tokens(Words,Tokens):-priv_cmpd(Words,Tokens,[tokenize]).





tokens_to_words(Tokens,Words):-priv_cmpd(Tokens,Words,[detoken]).





priv_cmpd([],[],_):-!.


priv_cmpd([A,B,C,D|L],[X|R],Set):-priv_cmpv([A,B,C,D],[X],Set),!,priv_cmpd(L,R,Set).


priv_cmpd([A,B,C|L],[X|R],Set):-priv_cmpv([A,B,C],[X],Set),!,priv_cmpd(L,R,Set).


priv_cmpd([A,B|L],[X|R],Set):-priv_cmpv([A,B],[X],Set),!,priv_cmpd(L,R,Set).


priv_cmpd([A|L],[X|R],Set):-priv_cmpv([A],[X],Set),!,priv_cmpd(L,R,Set).


priv_cmpd([A|L],[A|R],Set):-!,priv_cmpd(L,R,Set).





priv_cmpv( W ,T,[tokenize]):-wn(W,T).


priv_cmpv( T ,W,[detoken]) :-wn(W,T).





prevail(W1,W2):- 


	wn([W1],[T1]),wn([W2],[T2]),


	traceroute(T1,T2,[P|L],_),!,dmesg([P,L]),


	asserta(precharged_wn([W1],[P])).





best_word(Word,Token):-


        asserta(precharged_wn(Word,Token)).





:-dynamic(precharged_wn/2).


%:-[wn_precharge].


