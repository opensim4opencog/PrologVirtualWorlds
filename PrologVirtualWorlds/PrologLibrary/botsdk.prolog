%//------------------------------------------------------------------------------
:-	set_feature(debug_on_error, false),
	set_feature(report_error, false), % override internal system error handling
	set_feature(syntax_errors, false).


:-style_check(+atom).
:-style_check(+string).
:-[tokenization].
:-['metabuiltins.prolog'].
:-['apldriver.prolog'].
:-['vb6support.prolog'].
:-['vwevents.prolog'].
:-['vwmethods.prolog'].
:-['vwevent_to_method.prolog'].
:-['event_to_property.prolog'].
:-['planning.prolog'].
:-['planning_datahooks.prolog'].
:-['vwtriggers.prolog'].
:-['eliza.prolog'].
:-['goal_threads.prolog'].
:-['type_template.prolog'].

:-apl_load_macrofile('espg.prolog').
:-apl_load_datafile('data.prolog').
:-eliza.

:-['actx_invokes.prolog'].


:-post(cosulted).


