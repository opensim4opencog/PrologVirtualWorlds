#!/bin/sh
#export LD_LIBRARY_PATH=".:/opt/jdk/jre/lib/i386:/usr/lib/pl-5.0.7/runtime/i686-linux-gnu:/opt/jdk/jre/lib/i386/client"
javac JavaRt.java -d ".."

plld -I$JAVA_HOME/include/linux/ -I$JAVA_HOME/include/ -L$JAVA_HOME/jre/lib/i386/ -L$JAVA_HOME/jre/lib/i386/server/ -o pljavart javart.c  -ljava -lreadline

plld -shared javart.c -o javart.so -I$JAVA_HOME/include/linux/ -I$JAVA_HOME/include/ -L$JAVA_HOME/jre/lib/i386/ -ljava

javac SwiJni.java -d ".."

plld -I$JAVA_HOME/include/linux/ -I$JAVA_HOME/include/ -L$JAVA_HOME/jre/lib/i386/ -L$JAVA_HOME/jre/lib/i386/server/ -o plswijni swijni.c  -ljava -lreadline

plld -shared swijni.c -o swijni.so -I$JAVA_HOME/include/linux/ -I$JAVA_HOME/include/ -L$JAVA_HOME/jre/lib/i386/ -ljava
