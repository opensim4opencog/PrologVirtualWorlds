#include <jni.h>

#ifdef _WIN32
#define PATH_SEPARATOR ';'
#else /* UNIX */
#define PATH_SEPARATOR ':'
#endif

#define USER_CLASSPATH "." /* where Prog.class is */

main() {
    JNIEnv *env;
    JavaVM *jvm;
    JDK1_1InitArgs vm_args;
    jint res;
    jclass cls;
    jmethodID mid;
    jstring jstr;
    jobjectArray args;
    char classpath[1024];


    /* IMPORTANT: specify vm_args version # if you use JDK1.1.2 and beyond */
    vm_args.version = 0x00010001;

    JNI_GetDefaultJavaVMInitArgs(&vm_args);

    /* Append USER_CLASSPATH to the end of default system class path */
    sprintf(classpath, "%s%c%s",
            vm_args.classpath, PATH_SEPARATOR, USER_CLASSPATH);
    vm_args.classpath = classpath;

    /* Create the Java VM */
    res = JNI_CreateJavaVM(&jvm,&env,&vm_args);
    if (res < 0) {
        fprintf(stderr, "Can't create Java VM\n");
        exit(1);
    }

    cls = (*env)->FindClass(env, "Prog");

    if (cls == 0) {
        fprintf(stderr, "Can't find Prog class\n");
        exit(1);
    }
 
    mid = (*env)->GetStaticMethodID(env, cls, "main", "([Ljava/lang/String;)V");
    if (mid == 0) {
        fprintf(stderr, "Can't find Prog.main\n");
        exit(1);
    }

    jstr = (*env)->NewStringUTF(env, " from C!");
    if (jstr == 0) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    args = (*env)->NewObjectArray(env, 1, 
                        (*env)->FindClass(env, "java/lang/String"), jstr);

    if (args == 0) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    (*env)->CallStaticVoidMethod(env, cls, mid, args);

    (*jvm)->DestroyJavaVM(jvm);
}
