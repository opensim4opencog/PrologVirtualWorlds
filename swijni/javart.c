/***********************************************************************
 * 
 * Now prolog can call java!!!
 * 
 *  File:    $Id: javart.c,v 1.2 2002-11-16 18:49:51 dmiles Exp $
 *  Date:    $Date: 2002-11-16 18:49:51 $
 *  Author:  Douglas Miles
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *  
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library Public License for more details.
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <SWI-Prolog.h>	 

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

static char* cp;

static JavaVMOption options[4];

static jint result_of_JNI_CreateJavaVM=-1; /* Means not created */

static jclass class_pointer_JavaRt = 0;	/*  Uninitialized */

static jclass class_jstring;

static jclass class_jobject;

static atom_t JAVA_OBJECT;
static atom_t JAVA_INSTANCE;
static atom_t JAVA_PARSE;

/*
static jclass class_jchar;
static jclass class_jbyte;
static jclass class_jboolean;
static jclass class_jfloat;
static jclass class_jdouble;
static jclass class_jint;
static jclass class_jlong;
static jclass class_jshort;
static jclass class_jclass;
*/

static jmethodID invoke_object_method;
static long flags;

/* Defines */
#define STRING char *

#define JNI_ENV (*jni_env)
#define JAVA_VM (*java_vm)

#define javart_FindClass(classname) \
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
foreign_t pl_prep_vm();
foreign_t java_prep_vm();
foreign_t destroy_vm();
foreign_t pl_java_destroy_vm();
foreign_t prep_vm_phase2();
install_t install();
static void init_prolog(int argc, STRING *argv);
static jobject term_to_jobject(term_t temp_term);


foreign_t destroy_vm() {
	result_of_JNI_CreateJavaVM=-1;		/* Means not created */
	class_pointer_JavaRt = 0;	/* Uninitialized */
	if (java_vm) {
		JAVA_VM->DestroyJavaVM(java_vm);
		jni_env = 0;
	}
	PL_succeed;
}

foreign_t java_prep_vm() {
	/* Allows it to be called more then once */
	if (class_pointer_JavaRt) PL_succeed;

	{

		cp = malloc( 8192 ); 

		sprintf(cp, "-Djava.class.path=%s", getenv( "CLASSPATH" ) );


		options[0].optionString = "-Djava.compiler=NONE";	/* disable JIT */
		options[1].optionString = cp;	/* user classes */
		options[2].optionString = "-Djava.library.path=.";	/* set native library path */
		options[3].optionString = "";						/* print JNI-related messages */

		// options[3].optionString = "-verbose:jni";   	/* print JNI-related messages */

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

		if (result_of_JNI_CreateJavaVM < 0) {
			fprintf(stderr, "Can't create Java VM\n%i\n",result_of_JNI_CreateJavaVM);
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
		class_jclass = javart_FindClass("java/lang/Class");
		
		class_jboolean = javart_FindClass("java/lang/Boolean");
		class_jbyte = javart_FindClass("java/lang/Byte");
		class_jchar = javart_FindClass("java/lang/Char");
		class_jshort = javart_FindClass("java/lang/Short");
		class_jint = javart_FindClass("java/lang/Integer");
		class_jlong = javart_FindClass("java/lang/Long");
		class_jfloat = javart_FindClass("java/lang/Float");
		*/
		class_pointer_JavaRt = javart_FindClass("swijni/JavaRt");

		class_jobject = javart_FindClass("java/lang/Object");

		class_jstring = javart_FindClass("java/lang/String");

		if (!class_pointer_JavaRt || !class_jstring) {
			fprintf(stderr, "Can't find \"swijni.JavaRt\" (Set Your Classpath)\n");
			PL_fail;
		}

		invoke_object_method = JNI_ENV->GetStaticMethodID(jni_env, 
														  class_pointer_JavaRt, 
														  "invokeObject",
														  "(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;");
		if (!invoke_object_method) {
			fprintf(stderr, "Cant GetStaticMethodID (invokeObject)\n");
			PL_fail;
		}
		PL_succeed;
	}

}

jstring intToObjRef(term_t temp_term) {
	char temp_parse[64];
	int temp_int;
	PL_get_integer(temp_term,&temp_int);
	sprintf(temp_parse,"o%i",temp_int);
	return chars_to_jstring(temp_parse);
}


static jobjectArray list2MethodArgs(term_t arg_list) {

	int countup=0;
	char temp_parse[64];

	term_t temp_term = PL_new_term_ref();	   /* variable for the elements */

	term_t prolog_list = PL_copy_term_ref(arg_list);	/* copy as we need to write */

	jobjectArray method_args;

	while ( PL_get_list(prolog_list, temp_term, prolog_list) ) countup++;
	
	method_args=(jobjectArray) JNI_ENV->NewObjectArray(jni_env,
													   countup,
													   (jclass) class_jobject,
													   NULL);  
	countup=0;

	prolog_list = PL_copy_term_ref(arg_list);	/* copy as we need to write */

	while ( PL_get_list(prolog_list, temp_term, prolog_list) ) {
		JNI_ENV->SetObjectArrayElement(jni_env,
									   method_args,
									   countup,
									   (jobject) term_to_jobject(temp_term));
		countup++;
	}

	return method_args;

}


static jobject term_to_jobject(term_t temp_term) {

	functor_t functor;
	int arity, len, n;
	STRING temp_string;
	char temp_parse[4096];
	double temp_float;
	int temp_int;

	switch ( PL_term_type(temp_term) ) {
	case PL_VARIABLE:
		sprintf(temp_parse,"_%p",temp_term);
		return chars_to_jstring(temp_parse);
	case PL_ATOM:
		if (PL_get_nil(temp_term)) return(jobject) list2MethodArgs(temp_term);

		PL_get_atom_nchars(temp_term,&len,&temp_string);

		// Special Atoms
		switch (len) {
		case 5:
			if (PL_unify_atom_chars(temp_term,"false"))	return chars_to_jstring("bf");
		case 0:
			return chars_to_jstring("s");
		case 4:
			if (PL_unify_atom_chars(temp_term,"true")) return chars_to_jstring("bt");
			if (PL_unify_atom_chars(temp_term,"null")) return chars_to_jstring("n");
		case 6:
			if (PL_unify_atom_chars(temp_term,"javart")) return chars_to_jstring("oJavaRt");
		}

		sprintf(temp_parse,"s%s",temp_string); return chars_to_jstring(temp_parse);

	case PL_STRING:
		PL_get_chars(temp_term,&temp_string,CVT_ALL);
		sprintf(temp_parse,"s%s",temp_string);
		return chars_to_jstring(temp_parse);
	case PL_INTEGER: 
		PL_get_integer(temp_term,&temp_int);
		sprintf(temp_parse,"i%i",temp_int);
		return chars_to_jstring(temp_parse);
	case PL_FLOAT: //todo return a jobject float 
		PL_get_float(temp_term,&temp_float);
		sprintf(temp_parse,"l%f",temp_float);
		return chars_to_jstring(temp_parse);
	case PL_TERM:

		if (PL_is_list(temp_term))	return(jobject) list2MethodArgs(temp_term);

		{
			atom_t name = PL_new_term_ref();

			PL_get_name_arity(temp_term, &name, &arity);

			if (name == JAVA_OBJECT || name == JAVA_INSTANCE) {
				term_t arg1 = PL_new_term_ref();
				PL_get_arg(1, temp_term, arg1);
				return(jstring) intToObjRef(arg1);
			}
			if (name == JAVA_PARSE) {
				term_t arg1 = PL_new_term_ref();
				PL_get_arg(1, temp_term, arg1);
				PL_get_atom_nchars(arg1,&len,&temp_string);
				sprintf(temp_parse,"%s",temp_string);
				return chars_to_jstring(temp_parse);
			}
			PL_get_chars(temp_term,&temp_string,CVT_ALL);
			sprintf(temp_parse,"t%s",temp_string);
			return chars_to_jstring(temp_parse);
		}
	}
	PL_get_chars(temp_term,&temp_string,CVT_ALL);
	sprintf(temp_parse,"u%s",temp_string);
	return chars_to_jstring(temp_parse);

}

foreign_t pl_java_invoke_object(term_t object_term,term_t method_term,term_t arg_list,term_t result_term) {
	jstring result_string_object;
	jobjectArray method_args;
	foreign_t prolog_result;

	STRING method_result_chars;
	term_t temp_term = PL_new_term_ref();	   /* variable for the elements */  

	result_string_object = JNI_ENV->CallStaticObjectMethod(jni_env,
														   class_pointer_JavaRt,
														   invoke_object_method,
														   term_to_jobject(object_term),
														   term_to_jobject(method_term),
														   list2MethodArgs(arg_list));

	if (!result_string_object)
		method_result_chars = "null";
	else
		method_result_chars=jstring_to_chars(result_string_object);

	if (PL_chars_to_term(method_result_chars,temp_term)) {
		prolog_result =(foreign_t)PL_unify(result_term,temp_term); 
	} else {
		prolog_result =(foreign_t)PL_unify_string_chars(result_term,method_result_chars); 
	}

	//  JNI_ENV->DeleteLocalRef(jni_env,method_args);
	//	JNI_ENV->ReleaseStringUTFChars(jni_env, result_string_object, method_result_chars);

	return prolog_result;

}

int main(int argc, char**argv) {
 PL_initialise(argc,argv);
 PL_install_readline();
 install();
 PL_toplevel();
}

install_t install() {
	PL_register_foreign("java_create_vm", 0, java_prep_vm, 0);
	PL_register_foreign("java_destroy_vm", 0, destroy_vm, 0);
	PL_register_foreign("java_invoke_object_protected", 4, pl_java_invoke_object, 0);

	JAVA_OBJECT= PL_new_atom("$java_object");
	JAVA_INSTANCE = PL_new_atom("$java_instance");
	JAVA_PARSE = PL_new_atom("$java_parse");

	PL_register_atom(JAVA_OBJECT);
	PL_register_atom(JAVA_INSTANCE);
	PL_register_atom(JAVA_PARSE);

}

