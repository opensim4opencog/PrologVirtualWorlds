#!/bin/bash

source ../../bin/setpaths
	
javac JNIPrologServer.java

plld -I$JAVA_HOME/include/linux/ -I$JAVA_HOME/include/ -L$JAVA_HOME/jre/lib/i386/ -o swijnit swijni.c  -ljava 

plld   swijni.c  -shared -o swijni.so -I$JAVA_HOME/include/linux/ -I$JAVA_HOME/include/ -L$JAVA_HOME/jre/lib/i386/  -ljava 




