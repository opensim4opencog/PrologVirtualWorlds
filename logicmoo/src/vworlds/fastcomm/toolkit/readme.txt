This is a collection of high performance threadsafe data structures, authored by myself and other people. - CH 8/12/99

Usage: 
- include tools.h in any header files you want.
- include tools.cpp in your project somewhere.

File breakdown:
Overall:
	tools.cpp
	tools.h
	
Various useful locks:
	locks.h

Lockfree queue (templatized):
	lockfree.h

Work Queue using completion ports:
	WorkQ.CPP
	WorkQ.h
