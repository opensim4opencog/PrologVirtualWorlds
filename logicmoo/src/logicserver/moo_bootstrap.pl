
% utility modules

:-ensure_loaded('moo_utility.pl').
:-ensure_loaded('moo_fileutils.pl').
:-ensure_loaded('moo_uwdgraphs.pl').
:-ensure_loaded('moo_slg_tabling.pl').
:-ensure_loaded('moo_reader.pl').
:-ensure_loaded('moo_database.pl').
:-ensure_loaded('moo_threads.pl').
:-ensure_loaded('moo_globalisms.pl').
:-ensure_loaded('moo_java.pl').


% server modules
:-ensure_loaded('moo_httpd.pl').
:-ensure_loaded('moo_soap.pl').
:-ensure_loaded('moo_browser.pl').
:-ensure_loaded('moo_markup.pl').
:-ensure_loaded('moo_generation.pl').
:-ensure_loaded('moo_cmdline.pl').
:-ensure_loaded('moo_api.pl').

% inference type modules
:-ensure_loaded('moo_transitiveclosure.pl').
:-ensure_loaded('moo_oprove.pl').
:-ensure_loaded('moo_icl.pl').
:-ensure_loaded('moo_tap.pl').
:-ensure_loaded('moo_simple.pl').
:-ensure_loaded('moo_pttp_xray.pl').
:-ensure_loaded('moo_belief.pl').

% other
:-ensure_loaded('moo_sentence.pl').
:-ensure_loaded('moo_cycl.pl').
:-ensure_loaded('moo_nfs.pl').
:-ensure_loaded('moo_english.pl').
:-ensure_loaded('moo_chat.pl').
:-ensure_loaded('moo_compiler.pl').
:-ensure_loaded('moo_functionals.pl').
:-ensure_loaded('moo_insert.pl').
:-ensure_loaded('moo_testing.pl').

:-ensure_loaded('moo_image.data').

moduleFile(image('PrologMOO'),'moo_image.data').


