# Make file for directory: demos


JAVA=javac
JFLAGS=-deprecation -g -J-mx300000000


all: AtpListener.class Demo.class Demo2.class Demo3.class Demo3NetworkKb.class DemoGeneral.class DemoTupleKb.class KbLoaderTest.class KbSummary.class RemoteListener.class WalkerTest.class

AtpListener.class: AtpListener.java
	$(JAVA) $(JFLAGS) AtpListener.java

Demo.class: Demo.java
	$(JAVA) $(JFLAGS) Demo.java

Demo2.class: Demo2.java
	$(JAVA) $(JFLAGS) Demo2.java

Demo3.class: Demo3.java
	$(JAVA) $(JFLAGS) Demo3.java

Demo3NetworkKb.class: Demo3NetworkKb.java
	$(JAVA) $(JFLAGS) Demo3NetworkKb.java

DemoGeneral.class: DemoGeneral.java
	$(JAVA) $(JFLAGS) DemoGeneral.java

DemoTupleKb.class: DemoTupleKb.java
	$(JAVA) $(JFLAGS) DemoTupleKb.java

KbLoaderTest.class: KbLoaderTest.java
	$(JAVA) $(JFLAGS) KbLoaderTest.java

KbSummary.class: KbSummary.java
	$(JAVA) $(JFLAGS) KbSummary.java

RemoteListener.class: RemoteListener.java
	$(JAVA) $(JFLAGS) RemoteListener.java

WalkerTest.class: WalkerTest.java
	$(JAVA) $(JFLAGS) WalkerTest.java

clean:
	rm -f *.class core
