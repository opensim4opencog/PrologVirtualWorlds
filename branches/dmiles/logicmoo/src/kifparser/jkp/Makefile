# This makefile follows that of jb

E=jkp
JBFPATH=../jb

##################################################
# Wrap java and javac execution so we can set the
# CLASSPATH environment variable
#JEXEC=jexec
JEXEC=

JAVA=$(JEXEC) java
JAVAC=$(JEXEC) javac -g
##################################################

# Turn on to see parser actions and lex output
#DEBUG=-debug

# Choose flex lexer optimization level
# default: smallest and slowest
FLEXOPT=Cem
# -Cfe: larger and medium speed
#FLEXOPT=Cfe
# -Cfa: largest and fastest
# This option may cause failure in the java compiler and/or virtual machine
# because the .class files are too large
#FLEXOPT=Cfa

#BISONFLAGS=-v
BISONFLAGS=
FLEXFLAGS=-$(FLEXOPT)

BISON=bison
FLEX=flex

JBFLAGS=-yyerror yyverror
JFFLAGS=--$(FLEXOPT)

JF=jf.class
JB=jb.class

LEXTEMPLATE=yylex.$(FLEXOPT).template

##################################################

all: Main.class

run: Main.class
	$(JAVA) Main $(DEBUG) <$(E).cmds

clean:
	rm -f *.class
	rm -f YYparse.java YYlex.java YYtokentypes.java
	rm -f *.tab.c *.lex.java *.yy.c

Main.class YYparse.class YYlex.class YYtokentypes.class: \
		Main.java YYparse.java YYlex.java YYtokentypes.java
	$(JAVAC) Main.java YYparse.java YYlex.java YYtokentypes.java

YYparse.java YYlex.java YYtokentypes.java: \
		$(E).tab.c \
		$(E).lex.java \
		$(JBFPATH)/jbf/yyparse.template \
		$(JBFPATH)/jbf/yytokentypes.template $(JBFPATH)/jbf/$(JB)
	$(JAVA) jbf.jb \
		$(JBFLAGS) $(E).tab.c \
		$(JBFPATH)/jbf/yyparse.template YYparse.java \
		$(JBFPATH)/jbf/yytokentypes.template YYtokentypes.java \
		$(E).lex.java YYlex.java

##################################################
# KIF example using bison and flex

jkp.tab.c jkp.output: jkp.y
	$(BISON) $(BISONFLAGS) jkp.y
	
jkp.yy.c: jkp.lex
	$(FLEX) $(FLEXFLAGS) -ojkp.yy.c jkp.lex

jkp.lex.java: jkp.yy.c $(JBFPATH)/jbf/$(LEXTEMPLATE) $(JBFPATH)/jbf/$(JF)
	$(JAVA) jbf.jf $(JFFLAGS) jkp.yy.c $(JBFPATH)/jbf/$(LEXTEMPLATE) jkp.lex.java

#$(JBFPATH)/jbf/$(JF) $(JFFLAGS) jkp.yy.c $(JBFPATH)/jbf/$(LEXTEMPLATE) jkp.lex.java

