gcc -g -O2 -c -Ic:/jdk1.1.7A/include -Ic:/jdk1.1.7A/include/win32 HelloWorldImp.c
g77 -g -O2 -c f77func.f
dllwrap --output-def hello.def --add-stdcall-alias -o hello.dll -s HelloWorldImp.o f77func.o -lg2c
