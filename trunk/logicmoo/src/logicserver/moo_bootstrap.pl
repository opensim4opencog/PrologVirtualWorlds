
:-write('% Loading Bootstrap'),nl.
	
% utility modules
:-moduleEnsureLoaded('moo_utility.pl').
:-moduleEnsureLoaded('moo_fileutils.pl').
:-moduleEnsureLoaded('moo_uwdgraphs.pl').
:-moduleEnsureLoaded('moo_slg_tabling.pl').
:-moduleEnsureLoaded('moo_reader.pl').
:-moduleEnsureLoaded('moo_database.pl').

% server modules
:-moduleEnsureLoaded('moo_listener.pl').
:-moduleEnsureLoaded('moo_httpd.pl').
:-moduleEnsureLoaded('moo_soap.pl').
:-moduleEnsureLoaded('moo_browser.pl').
:-moduleEnsureLoaded('moo_markup.pl').
:-moduleEnsureLoaded('moo_generation.pl').
:-moduleEnsureLoaded('moo_cmdline.pl').
:-moduleEnsureLoaded('moo_command.pl').
:-moduleEnsureLoaded('moo_api.pl').

% inference type modules
:-moduleEnsureLoaded('moo_transitiveclosure.pl').
:-moduleEnsureLoaded('moo_oprove.pl').
:-moduleEnsureLoaded('moo_icl.pl').
:-moduleEnsureLoaded('moo_tap.pl').
:-moduleEnsureLoaded('moo_simple.pl').
:-moduleEnsureLoaded('moo_pttp_xray.pl').
:-moduleEnsureLoaded('moo_belief.pl').

% other
:-moduleEnsureLoaded('moo_sentence.pl').
:-moduleEnsureLoaded('moo_cycl.pl').
:-moduleEnsureLoaded('moo_nnf.pl').
:-moduleEnsureLoaded('moo_english.pl').
:-moduleEnsureLoaded('moo_chat.pl').
:-moduleEnsureLoaded('moo_compiler.pl').
:-moduleEnsureLoaded('moo_functionals.pl').
:-moduleEnsureLoaded('moo_insert.pl').
:-moduleEnsureLoaded('moo_testing.pl').

:-write('% Loading Image'),nl.
:-moduleEnsureLoaded('moo_image.data').

