#!/bin/bash

source ./bin/setpaths

# Currently ant breaks

cd $LOGICMOOBASE/src/partner/jamud

ant

cd $LOGICMOOBASE

# So we will build the rest manully

cd $LOGICMOOBASE/src/logicmoo

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/com/jamud/commands/olc

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/com/jamud/commands/logicmoo

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/com/jamud/commands/

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/com/jamud/communications/

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/com/jamud/connections/

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/com/jamud/creation/

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/jamud/

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/jamud/

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/jamud/board

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/jamud/channel

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/jamud/command

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/jamud/object

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/jamud/object/event

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/jamud/plugin

javac *.java

cd $LOGICMOOBASE/src/partner/jamud/src/jamud/util

javac *.java
    
cd $LOGICMOOBASE
				  
				  
#    
