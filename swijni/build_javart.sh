#!/bin/bash

source ../../bin/setpaths

	
javac JavaRt.java

#export LD_LIBRARY_PATH=".:/opt/jdk/jre/lib/i386:/usr/lib/pl-5.0.7/runtime/i686-linux-gnu:/opt/jdk/jre/lib/i386/client"

plld -I$JAVA_HOME/include/linux/ -I$JAVA_HOME/include/ -L$JAVA_HOME/jre/lib/i386/ -L$JAVA_HOME/jre/lib/i386/server/ -o swijnit swijni.c  -ljava -lreadline

plld   swijni.c  --shared -o javart.so -I$JAVA_HOME/include/linux/ -I$JAVA_HOME/include/ -L$JAVA_HOME/jre/lib/i386/  -ljava -lreadline



