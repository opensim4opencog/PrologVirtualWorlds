dlltool -k --input-def javai.def --dll javai.dll --output-lib javai.a
gcc -o invoke -g -O2 -Ic:/jdk1.1.7A/include -Ic:/jdk1.1.7A/include/win32 invoke.c javai.a
