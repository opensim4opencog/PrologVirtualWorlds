%//------------------------------------------------------------------------------
:-	set_feature(debug_on_error, false),
	set_feature(report_error, false), % override internal system error handling
	set_feature(syntax_errors, false).


:-style_check(+atom).
:-style_check(+string).
:-[tokenization].
:-['moo_metabuiltins.pl'].
:-['moo_apldriver.pl'].
:-['moo_vb6support.pl'].
:-['moo_vwevents.pl'].
:-['moo_vwmethods.pl'].
:-['moo_vwevent_to_method.pl'].
:-['moo_event_to_property.pl'].
:-['moo_planning.pl'].
:-['moo_planning_datahooks.pl'].
:-['moo_vwtriggers.pl'].
:-['moo_goal_threads.pl'].
:-['moo_type_template.pl'].

:-apl_load_macrofile('moo_espg.pl').

:-['moo_actx_invokes_win32.pl'].



