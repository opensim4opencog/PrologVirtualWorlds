Attribute VB_Name = "modWinVer"
Option Explicit

Public Const mlngWindows95 = 0
Public Const mlngWindowsNT = 1
Public Const mlngWindows2000 = 2
Public Declare Function GetVersion Lib "kernel32" () As Long
Public glngWhichWindows32 As Long

