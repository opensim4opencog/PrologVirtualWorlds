gcc -g -O2 -c -Ic:/jdk1.1.7A/include -Ic:/jdk1.1.7A/include/win32 -g HelloWorldImp.c
dllwrap --output-def hello.def --add-stdcall-alias -o hello.dll -s HelloWorldImp.o
