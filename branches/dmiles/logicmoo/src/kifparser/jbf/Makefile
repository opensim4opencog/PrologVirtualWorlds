# Java files

# The template instantiations
JAVAI= \
Object_Vector.java \
Object_Stack.java \
Int_Vector.java \
Int_Stack.java \
Char_Vector.java

JAVAB= \
ParseException.java \
LexException.java \
DequeueException.java \
Queue.java \
Dequeue.java \
YYtoken.java \
YYlocation.java \
YYlexbuffer.java \
YYnode.java \
YYterminal.java \
YYnonterminal.java \
CharVector.java \
YYhash.java

LEXTEMPLATES= \
yylex.Cem.template \
yylex.Cfe.template \
yylex.Cfa.template

# Java->Class not always 1-1
CLB=$(JAVAB:.java=.class)

CLI=$(JAVAI:.java=.class)

UTILS=subst.java
CLUT=$(UTILS:.java=.class)

TRANSFORMS=jf.java jb.java
CLTR=$(TRANSFORMS:.java=.class)

##################################################
# Wrap java and javac execution so we can set the
# CLASSPATH environment variable
JEXEC=jexec
JDFLAG=-d ..

JAVA=$(JEXEC) java
JAVAC=$(JEXEC) javac -g $(JDFLAG)
##################################################

all: $(CLUT) $(CLI) $(CLB) $(CLTR) $(LEXTEMPLATES)

clean:
	rm -f ./*.class
	rm -f $(JAVAI)
	rm -f $(LEXTEMPLATES)

$(CLB): $(JAVAB)
	$(JAVAC) $(JAVAB)

$(CLI): $(JAVAI)
	$(JAVAC) $(JAVAI)

Char_Vector.java: vector.template
	$(JAVA) jbf.subst <vector.template >$@ \
		"@ELEMTYPE@" "char" "@TYPE@" "Char" "@NULL@" "0"

Int_Vector.java: vector.template
	$(JAVA) jbf.subst <vector.template >$@ \
		"@ELEMTYPE@" "int" "@TYPE@" "Int" "@NULL@" "0"

Int_Stack.java: stack.template
	$(JAVA) jbf.subst <stack.template >$@ \
		"@ELEMTYPE@" "int" "@TYPE@" "Int" "@NULL@" "0"

Object_Vector.java: vector.template
	$(JAVA) jbf.subst <vector.template >$@ \
		"@ELEMTYPE@" "Object" "@TYPE@" "Object" "@NULL@" "null"

Object_Stack.java: stack.template
	$(JAVA) jbf.subst <stack.template >$@ \
		"@ELEMTYPE@" "Object" "@TYPE@" "Object" "@NULL@" "null"

##################################################

yylex.Cem.template: yylex.template subst.class
	$(JAVA) <yylex.template >$@ jbf.subst \
	'^//>Cem>' '""' '^//>Cfe>.*' '""' '^//>Cfa>.*' '""' '^//>Cfae>.*' '""'

yylex.Cfe.template: yylex.template
	$(JAVA) jbf.subst <yylex.template >$@ \
	'^//>Cem>.*' '""' '^//>Cfe>' '""' '^//>Cfa>.*' '""' '^//>Cfae>' '""'

yylex.Cfa.template: yylex.template
	$(JAVA) jbf.subst <yylex.template >$@ \
	'^//>Cem>.*' '""' '^//>Cfe>.*' '""' '^//>Cfa>' '""' '^//>Cfae>' '""'

##################################################

subst.class: subst.java
	$(JAVAC) subst.java

jf.class: jf.java
	$(JAVAC) jf.java

jb.class: jb.java
	$(JAVAC) jb.java

