#include <jni.h>
#include <SWI-Prolog.h>	 

/*
   NAME
	 swi_jni.c - <Brief description> 

   DESCRIPTION
   <Detailed description>
   
   MODIFIED   (MM/DD/YY)
   <Name    mm/dd/yy>
*/

/*
	TODO

	0. Re code review (see #4).
	1. Add comments.
	2. Sanity check pointers.
	3. Add general error function/macro.
	4. I would re code to avoid all exit()'s (so you can destroy on errors) and have it different... but...
	5. Add debugging flag (should print to a file not &2)
*/


/* Globals */
static JNIEnv *jni_env;
static JavaVM *java_vm;
static JavaVMInitArgs vm_args;


static JavaVMOption options[4];
static jint result_of_JNI_CreateJavaVM=-1; /* Means not created */
static jclass class_pointer_JNIPrologServer = 0; /*  Uninitialized */
static jclass class_jstring;
static jclass class_jobject;
static jclass class_jchar;
static jclass class_jbyte;
static jclass class_jboolean;
static jclass class_jfloat;
static jclass class_jdouble;
static jclass class_jint;
static jclass class_jlong;
static jclass class_jshort;
static jclass class_jclass;

static jmethodID invoke_object_method;
static long flags;

/* Defines */
#define STRING char *

#define JNI_ENV (*jni_env)
#define JAVA_VM (*java_vm)

#define sj_FindClass(classname) \
    (JNI_ENV->FindClass(jni_env,(STRING) classname))

#define chars_to_jstring(source) \
    ((jstring)JNI_ENV->NewStringUTF(jni_env,(STRING )source)) 

#define jstring_to_chars(source) \
    ((STRING )JNI_ENV->GetStringUTFChars(jni_env,(jstring )source,(jboolean )JNI_FALSE)) 

#define create_type(class_java) \
	((jobject)JNI_ENV->NewObject(jni_env, (jclass )class_java, (jmethodID)GetMethodID(jni_env,(jclass )class_java, "<init>", "()V"))

/* Arbitrary! And wrong anyhow */
#define MAX_STRINGARG_LEN 64

/* Initial length of ObjectArray's (May be resized runtime) */
#define MAX_ARGS 32



/* Flags */

/* 
	I'll add these later... along with descent log file.

*/

#define __DEBUG     0x0001                /* Set debugging */

#define IS_DEBUG    (flags & __DEBUG)

#define SET_DEBUG   (flags |= __DEBUG)


/* Prototypes */
static foreign_t pl_prep_vm();
static foreign_t java_prep_vm();
static foreign_t destroy_vm();
static foreign_t pl_java_destroy_vm();
static foreign_t prep_vm_phase2();
static install_t install();
static void init_prolog(int argc, STRING *argv);


foreign_t 
destroy_vm() 
	{
	result_of_JNI_CreateJavaVM=-1;		/* Means not created */
	class_pointer_JNIPrologServer = 0;	/* Uninitialized */
	if (java_vm)
		{
		JAVA_VM->DestroyJavaVM(java_vm);
		jni_env = 0;
		}
	PL_succeed;
	}

foreign_t 
java_prep_vm()
	{
		STRING szClasspath = getenv( "CLASSPATH" );
		STRING cp = malloc( 8192 ); 
	
		/* Allows it to be called more then once */
	if (!result_of_JNI_CreateJavaVM)
		PL_succeed;

	destroy_vm();

	
	sprintf(cp,"-Djava.class.path=.:%s",szClasspath);

	options[0].optionString = "-Djava.compiler=NONE";	/* disable JIT */
	options[1].optionString = cp;	/* user classes */
	options[2].optionString = "-Djava.library.path=.";	/* set native library path */
	options[3].optionString = "";						/* print JNI-related messages */

/*	options[3].optionString = "-verbose:jni"; */		/* print JNI-related messages */

	vm_args.version = JNI_VERSION_1_2;
	vm_args.options = options;
	vm_args.nOptions = 4;
	vm_args.ignoreUnrecognized = 1;     

	/*
		Note that in the Java 2 SDK, there is no longer any need to call 
	*/
	result_of_JNI_CreateJavaVM = JNI_CreateJavaVM(&java_vm, 
												  (void **)&jni_env, 
												  &vm_args);

	if (result_of_JNI_CreateJavaVM < 0)
		{
		fprintf(stderr, "Can't create Java VM\n%i\n",result_of_JNI_CreateJavaVM);
		PL_fail;
		}
	return prep_vm_phase2();
	}

/* This is used to create the JavaVM to setupo your own paths */
foreign_t 
pl_prep_vm(term_t classpath_term, term_t librarypath_term) 
	{
		STRING szClasspath = getenv( "CLASSPATH" );
		STRING cp = malloc( 8192 ); 

	/* This will force a restart of JNI */
	destroy_vm();
	
	sprintf(cp,"-Djava.class.path=.:%s",szClasspath);

	options[0].optionString = "-Djava.compiler=NONE";	/* disable JIT */
	options[1].optionString = cp;	/* user classes */
	options[2].optionString = "-Djava.library.path=.";	/* set native library path */
	options[3].optionString = "";						/* print JNI-related messages */

/*	options[3].optionString = "-verbose:jni"; */		/* print JNI-related messages */

	vm_args.version = JNI_VERSION_1_2;
	vm_args.options = options;
	vm_args.nOptions = 4;
	vm_args.ignoreUnrecognized = 1;     

	/*
		Note that in the Java 2 SDK, there is no longer any need to call 
	*/
	result_of_JNI_CreateJavaVM = JNI_CreateJavaVM(&java_vm, 
												  (void **)&jni_env, 
												  &vm_args);

	if (result_of_JNI_CreateJavaVM < 0)
		{
		fprintf(stderr, "Can't create Java VM\n%i\n",result_of_JNI_CreateJavaVM);
		PL_fail;
		}
	return prep_vm_phase2();
	}


foreign_t  
prep_vm_phase2() 
	{
	if (class_pointer_JNIPrologServer)
		PL_succeed;

	class_pointer_JNIPrologServer = JNI_ENV->FindClass(jni_env, "com.tks.sigma.prolog.JNIPrologServer");

	if (!class_pointer_JNIPrologServer)
		{
		fprintf(stderr, "Can't find JNIPrologServer class\n");
		PL_fail;
		}
/*
Java Type  Native Type  Description  
-----------------------------------
boolean  jboolean  unsigned 8 bits  
byte  jbyte  signed 8 bits  
char  jchar  unsigned 16 bits  
short  jshort  signed 16 bits  
int  jint  signed 32 bits  
long  jlong  signed 64 bits  
float  jfloat  32 bits  
double  jdouble  64 bits  
void  void  N/A  

*/

	/*
	class_jobject = sj_FindClass("java/lang/Object");
	
	class_jclass = sj_FindClass("java/lang/Class");
	
	class_jboolean = sj_FindClass("java/lang/Boolean");
	class_jbyte = sj_FindClass("java/lang/Byte");
	class_jchar = sj_FindClass("java/lang/Char");
	class_jshort = sj_FindClass("java/lang/Short");
	class_jint = sj_FindClass("java/lang/Integer");
	class_jlong = sj_FindClass("java/lang/Long");
	class_jfloat = sj_FindClass("java/lang/Float");
	*/
	class_jstring = sj_FindClass("java/lang/String");


	invoke_object_method = JNI_ENV->GetStaticMethodID(jni_env, 
													  class_pointer_JNIPrologServer, 
													  "invokeObject",
													  "(Ljava/lang/String;Ljava/lang/String;I[Ljava/lang/String;)Ljava/lang/String;");
	if (!invoke_object_method)
		{
		fprintf(stderr, "Cant GetStaticMethodID (invokeObject)\n");
		PL_fail;
		}
	PL_succeed;
	}

static jobject term_to_jstring(term_t temp_term)
	{

	functor_t functor;
	int arity, len, n;
	STRING temp_string;
	char temp_parse[1024];
	double temp_float;
	int temp_int;

	switch ( PL_term_type(temp_term) )
		{
		case PL_VARIABLE:
			return chars_to_jstring("variable");
		case PL_ATOM:
			if (temp_term)
				{
				}
			if (PL_unify_atom_chars(temp_term,"true")) return chars_to_jstring("$xt");
			if (PL_unify_atom_chars(temp_term,"false"))	return chars_to_jstring("$xf");
			PL_get_atom_nchars(temp_term,&len,&temp_string);
			if (len==2 && PL_unify_atom_chars(temp_term,"[]")) return chars_to_jstring("$$n");
			if (len>1 )	return chars_to_jstring(temp_string);
			if (len==0)	return chars_to_jstring("");
			sprintf(temp_parse,"$TChar,%s",temp_string);
			return chars_to_jstring(temp_parse);
		case PL_STRING:
			PL_get_chars(temp_term,&temp_string,CVT_ALL);
			return chars_to_jstring(temp_string);
		case PL_INTEGER: 
			PL_get_integer(temp_term,&temp_int);
			sprintf(temp_parse,"$i%i",temp_int);
			return chars_to_jstring(temp_parse);
		case PL_FLOAT: //todo return a jobject float 
			PL_get_float(temp_term,&temp_float);
			sprintf(temp_parse,"$f%f",temp_float);
			return chars_to_jstring(temp_parse);
		case PL_TERM:
			if (PL_get_nil(temp_term)) return chars_to_jstring("");
			if (PL_is_list(temp_term))
				{
				PL_get_chars(temp_term,&temp_string,CVT_ALL);
				sprintf(temp_parse,"$l%s",temp_string);
				return chars_to_jstring(temp_parse);
				} else
				{
				term_t arg1 = PL_new_term_ref();
				atom_t name = PL_new_term_ref();
				int arity;
				PL_get_name_arity(temp_term,&name, &arity);
				PL_get_arg(1, temp_term, arg1);
				if (PL_is_compound(arg1))
					{
					PL_get_arg(2, arg1, temp_term);
					return term_to_jstring(temp_term);
					}
				PL_get_chars(arg1,&temp_string,CVT_ALL);
				sprintf(temp_parse,"$T%s,%s",PL_atom_chars(name),temp_string);
				return chars_to_jstring(temp_parse);
				}
		default:
			PL_get_chars(temp_term,&temp_string,CVT_ALL);
			sprintf(temp_parse,"$u%s",temp_string);
			return chars_to_jstring(temp_parse);
		}

	}


static int 
call_chars(const char *goal)
	{
	fid_t fid = PL_open_foreign_frame();
	term_t g = PL_new_term_ref();
	int rval=0;

	if ( PL_chars_to_term(goal, g) )
		rval = PL_call(g, NULL);
	else
		rval = FALSE;

	PL_discard_foreign_frame(fid);
	return rval;
	}




static foreign_t
pl_java_invoke_object(term_t object_term,term_t method_term,term_t arg_list,term_t result_term) 
	{
	jstring result_string_object;
	jobjectArray method_args;
	term_t temp_term = PL_new_term_ref();	   /* variable for the elements */
	term_t prolog_list = PL_copy_term_ref(arg_list);	/* copy as we need to write */
	int countup=0;
	foreign_t prolog_result;

	STRING method_result_chars;

	java_prep_vm();	 // This is just sanity in case pl_prep_vm was not called

	/* Build default Object Array */
	printf("method_args");
	method_args=(jobjectArray) JNI_ENV->NewObjectArray(jni_env,
													   MAX_ARGS,
													   class_jstring,
													   chars_to_jstring(""));      

	//printf("=");
	while ( PL_get_list(prolog_list, temp_term, prolog_list) )
		{
		//printf("%s ", arg);
		JNI_ENV->SetObjectArrayElement(jni_env,method_args,
									   countup,
									   term_to_jstring(temp_term));
		countup++;
		}
	//printf("(%d)\n ", countup);

	/* Call the method */

	result_string_object = JNI_ENV->CallStaticObjectMethod(jni_env,
														   class_pointer_JNIPrologServer,
														   invoke_object_method,
														   term_to_jstring(object_term),
														   term_to_jstring(method_term),
														   (jint)(countup),
														   method_args);
	if (!result_string_object)
		{
		fprintf(stderr, "ERROR:  Out Of Memory Error (CallStaticObjectMethod)\n");
		PL_fail;
		}

	if (!(method_result_chars=jstring_to_chars(result_string_object)))
		{
		fprintf(stderr, "ERROR: Could not get string from result (%d)\n",result_string_object);
		PL_fail;
		}

	//printf("method_result_chars=%s\n ",method_result_chars);

	if (PL_chars_to_term(method_result_chars,temp_term))
		{
		prolog_result =(foreign_t)PL_unify(result_term,temp_term); 
		} else
		{
		prolog_result =(foreign_t)PL_unify_string_chars(result_term,method_result_chars); 
		}

	JNI_ENV->ReleaseStringUTFChars(jni_env, result_string_object, method_result_chars);
	JNI_ENV->DeleteLocalRef(jni_env,method_args);

	return prolog_result;

	}


static install_t 
install()
	{
	PL_register_foreign("java_create_vm", 0, java_prep_vm, 0);
	PL_register_foreign("java_create_vm", 2, pl_prep_vm, 0);
	PL_register_foreign("java_destroy_vm", 0, destroy_vm, 0);
	PL_register_foreign("java_invoke_object", 4, pl_java_invoke_object, 0);
	}


static void 
init_prolog(argc, argv) 
int argc;
STRING *argv;
{
	STRING av[10];
	int ac = 0;

	av[ac++] = argv[0];
/*
	av[ac++] = "-x";
	av[ac++] = "mystate";
*/
	av[ac]   = NULL;

	if (!PL_initialise(ac, av))
		PL_halt(1);

	install();
	PL_install_readline(); 
}

/* Main is used when invoking from commandline */
int 
main(argc, argv) 
int argc;
STRING *argv;
{
	init_prolog(argc,argv);
	PL_toplevel();
	return 0;
}

