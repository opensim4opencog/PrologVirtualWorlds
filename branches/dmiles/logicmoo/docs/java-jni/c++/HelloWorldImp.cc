#include <jni.h>
#include "HelloWorld.h"

#include <cstdlib>
#include <strstream>

using namespace std;

struct 
LocalFoo 
{
  LocalFoo () 
  { 
    ++instances;
    cerr << "#" << instances << " LocalFoo created." << endl;
  }
  ~LocalFoo () 
  { 
    cerr << "#" << instances << " LocalFoo destroyed." << endl;
    --instances;
  }
  static int instances;
};

int LocalFoo::instances;

static LocalFoo static_foo;

JNIEXPORT void JNICALL 
Java_HelloWorld_displayHelloWorld (JNIEnv *env, jobject) 
{
  {
    char *foo = new char [1024];
    ostrstream ostr1 (foo, 1024);
    ostr1 << "HelloWorld::displayHelloWorld: Entering.";
    cerr << ostr1.str() << endl;
    delete[] foo;
  }
  
  LocalFoo localfoo1;
  LocalFoo* localfoo2 = new LocalFoo;

  delete localfoo2;
  
  {
    char *foo = new char [1024];
    ostrstream ostr1 (foo, 1024);
    ostr1 << "HelloWorld::displayHelloWorld: Leaving.";
    cerr << ostr1.str() << endl;
    delete[] foo;
  }
  return;
}
