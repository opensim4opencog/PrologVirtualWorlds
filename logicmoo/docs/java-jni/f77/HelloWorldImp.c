#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jni.h>
#include "HelloWorld.h"

/* f77 compilers tend to append the underscore. If the f77 name already
   has one underscore, g77 will add *two* underscores at the end. Define
   F77_ADD_UNDERSCORE in the Makefile. */

#ifndef F77_ADD_UNDERSCORE
# define F77_ADD_UNDERSCORE 1
#endif

#if F77_ADD_UNDERSCORE
# define F77_FUNCTION(f) f##_
# define F77_FUNCTION2(f) f##__
#else
# define F77_FUNCTION(f) f
# define F77_FUNCTION2(f) f
#endif

extern int F77_FUNCTION(func1) ();
extern int F77_FUNCTION(func2) (const char *, int *, float *, unsigned long);

JNIEXPORT void JNICALL 
Java_HelloWorld_displayHelloWorld (JNIEnv *env, jobject obj) 
{
  printf ("Java JNI: Calling f77 function func1\n");
  printf ("func1 returned: %d\n", F77_FUNCTION(func1) ());
  return;
}

JNIEXPORT void JNICALL 
Java_HelloWorld_displayHelloWorld2 (JNIEnv *env, jobject obj) 
{
  int i = -99;
  float f = 3.14159;
  const char *str = "hello world!";

  printf ("Java JNI: Calling f77 function func2 (%s, %d, %f)\n",
          str, i, f);
  /* Note how the string length parameters are passed to g77. */
  printf ("func1 returned: %d\n", 
          F77_FUNCTION(func2) (str, &i, &f, strlen (str)));
  return;
}

