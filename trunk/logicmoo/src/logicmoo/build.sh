#!/bin/bash

source ../../bin/setpaths
	
# javac *.java

# plld -I$JAVA_HOME/include/linux/ -I$JAVA_HOME/include/ -L$JAVA_HOME/jre/lib/i386/ -o javart javart.c  -ljava 

plld   javart.c  -shared -o javart.so -I$JAVA_HOME/include/linux/ -I$JAVA_HOME/include/ -L$JAVA_HOME/jre/lib/i386/  -ljava 




