#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include "HelloWorld.h"

JNIEXPORT void JNICALL 
Java_HelloWorld_displayHelloWorld (JNIEnv *env, jobject obj) 
{
  char *home;
  printf("Java JNI\n");
  home = getenv ("HOME");
  printf("HOME = %s\n", (home) ? home : "(NULL)");
  return;
}

