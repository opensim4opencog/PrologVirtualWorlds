%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%	SWIACTX.PL:		ActiveX interface module
%
%		David Hovel
%		Microsoft Research
%		March 1, 2000
%
%	
%	For complete and current documentation, refer to:
%
%			\\davidhov0\public\swiactx\doc\swiproex.doc
%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%	NOTE: If the underlying predicates in this file change, you must
%			rebuild the index file "index.pl" by using make_library_index/1,
%			with the directory name as the single argument; e.g.:
%
%					make_library_index(library).
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%		Interface implementation
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%	This module and its companion DLL provide dynamic access to ActiveX objects
%	through their scriptable IDispatch interfaces.
%
%	All functions use an "ActiveX functor", which is formatted like
%
%				activeXObject(n)
%
%		where n is an integer.   This integer is a registration index that generally
%		remains valid until the object is released or the ActiveX context is popped.		
%
%
%	Function arguments and return values are generally formatted according to the
%	specifications given in the type library for the object in question.  Certain
%	types of conversion are coerced.  For example, the conversion of integers to
%	strings.  However, users must be familiar with the type library's requirements
%	for correct behavior.  This is particularly true when property methods
%	(get/put/propput) are invoked, since they require additional information to
%	be correctly selected.
%
%
%	Blow-by-blow info for variant type conversion:
%
%		VT_BSTR:		[in]	Strings, atoms and numbers can all be sent as BSTRs.
%						[out]	Returned strings are converted to Prolog strings
%
%		VT_I4,VT_I2,VT_U4,VT_INT:
%						[in]	Integers are converted to the appropriate precision.
%						[out]	Returned integers are converted to integers.
%
%		VT_R4, VT_R8:	[in]	Prolog floats become the appropriate type.
%						[out]	Returned value becomes Prolog float.
%
%		VT_DATE:		[in]	NOT SUPPORTED YET
%						[out]	Converted to string value
%
%		VT_USERDEFINED:
%						[in]	Only a type whose base is an ENUM can be passed;
%								value is convert to integer (VT_I4).
%
%		VT_BOOL:		[in/out]	Conversion is between the atoms 'true' (for
%								VARIANT_TRUE) and 'false' (for VARIANT_FALSE).
%		
%		VT_DISPATCH:	[in]	Must be functor "activeXObject(n)" as above;
%								converted to IDispatch.
%						[out]	Returned pointers are registered and unified
%								with the standard functor.
%
%		VT_EMPTY, VT_NULL:
%						[in]	Not allowed.
%						[out]	Empty return values are unified with the empty list,
%								'[]'.
%
%	Interface invocation failures are reported as Prolog failures; that is, the
%	invocation predicate fails.  Formatting failures, such as invalid method names
%	or the inability to correctly convert arguments cause Prolog exceptions.
%
%	Interface methods that return or require interface pointers can be directly
%	invoked, since new ActiveX objects will be automatically converted or created
%	as required.
%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%		Predicates
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%	
%		actx_create_object( StringProgIdOrGuid+, FunctorObject-)
%
%				Given a Prog ID or GUID, create the ActiveX object and register its
%				IDispatch interface.
%
%		actx_release_object( FunctorObject+ )
%
%				Release a previously registered ActiveX object interface.
%
%		actx_invoke_object( FunctorObject+, InvocationAtomOrList+, ArgumentList+, Result- )
%
%				Call a function through the object's IDispatch interace. The arguments are:
%
%					FunctorObject: a functor of the form "activeXObject(n)", created by
%									"actx_create_object" or similar functions.
%
%					InvocationAtomOrList:	Usually an atom (not a string) representing the
%									method name.  If the function is a property then this
%									argument must be a list where the method name atom is the
%									first element and a special atom representing the method
%									type is the second element.  For example:
%
%										actxInvokeObject(IPField,['Value','propget'],[],Field),
%
%									This example calls the "Value" method as a DISPATCH_PROPERTYGET
%									function.  The method invocation atoms are:
%
%											'func'			DISPATCH_METHOD
%											'propget'		DISPATCH_PROPERTYGET
%											'propput'		DISPATCH_PROPERTYPUT
%											'propputref'	DISPATCH_PROPERTYPUTREF
%								
%					ArgumentList:	This argument must be a (possibly empty) list of arguments.
%									The list elements are converted or coerced based upon the
%									type information in the typelib of the target IDispatch interface.
%
%					Result:			This argument is generally a variable (i.e., unbound), but
%									unification will work correctly if it's already bound.
%
%		actx_query_interface( FunctorObject+, StringGuid+, NewFunctorObject- )
%
%				Create a new functor object by querying an existing interface for a new interface.
%				If the QI succeeds, the new interface is then queried for its IDispatch interface,
%				and, if successful, that interface is registered and returned.
%
%		actx_enum_object( FunctorObject+, EnumerationResult- )  (non-determinisitic)
%
%				This predicate performs standard OLE IEnumVARIANT enumeration (i.e., DISPID_NEWENUM).
%				This predicate succeeds once for each element of the collection.
%
%		actx_collection_list( FunctorObject+, CollectionAsList- )
%			
%				This predicate also performs standard OLE enumeration, but the results are returned
%				as a (possibly empty) list.
%
%		actx_clone_object( FunctorObject+, NewFunctorObject- )
%
%				This predicate creates a new, registered ActiveX functor object.
%
%		actx_release_all
%				
%				This predicate releases all registered ActiveX objects.
%
%		actx_context				(non-determinisitic)
%
%				Every ActiveX object is assigned a numeric context indicator when registered.
%				This predicate increments that context number and succeeds only once, although
%				it is non-deterministic.  It will always fail the backtrack (REDO) attempt.
%				During the backtrack attempt or during any cut, all ActiveX objects registered
%				in its context or deeper are deregistered and discarded.
%
%		actx_break
%
%				This predicate is for debugging only.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


:- module(swiactx,
          [
			actx_break/0,					%	actx_break
			actx_clone_object/2,			%	actx_clone_object( FunctorObject+, NewFunctorObject- )
			actx_collection_list/2,			%	actx_collection_list( FunctorObject+, CollectionAsList- )
			actx_context/0,					%	actx_context
			actx_context_global/2,			%	actx_context_global( FunctorObject+, BooleanAtomOrUnboundVar- )
			actx_create_object/2,			%	actx_create_object( StringProgId+, FunctorObject-)
			actx_enum_object/2,				%	actx_enum_object( FunctorObject+, EnumerationResult- )  (non-determinisitic)
			actx_errors_as_exceptions/1,	%	actx_errors_as_exceptions( BooleanAtomOrUnbound? )
			actx_invoke_object/4,			%	actx_invoke_object( FunctorObject+, InvocationAtomOrList+, ArgumentList+, Result- )
			actx_list_to_date/2,			%	actx_list_to_date( DateAsListOfInts?, DateAsFloat? )
			actx_query_interface/3,			%	actx_query_interface( FunctorObject+, StringGuid+, NewFunctorObject- )
			actx_release_all/0,				%	actx_release_all
			actx_release_object/1			%	actx_release_object( FunctorObject+ )
		  ]).

%
%	This module relies upon the special "win32debugmode" predicate to determine which DLL to load.
%
%	SWI-Prolog will crash (i.e., many predicates will fail) if an attempt is made to use a _DEBUG build
%	version of an extension DLL with a non-debug build of LIBPL and vice-versa.
%
%	The following predicates are used to bind to the correct DLL based upon the build-in build flavor
%	of the LIBPL linked into the current executable.
%

:- load_foreign_library('swiactx.dll').	

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%	End of SWIACTX.PL
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
