Attribute VB_Name = "modTransparency"
Option Explicit

Declare Function GetWindowLong Lib "user32" Alias "GetWindowLongA" (ByVal hWnd As Long, ByVal nIndex As Long) As Long
Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hWnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
Declare Function SetLayeredWindowAttributes Lib "user32" (ByVal hWnd As Long, ByVal crKey As Long, ByVal bAlpha As Byte, ByVal dwFlags As Long) As Long
Declare Function UpdateLayeredWindow Lib "user32" (ByVal hWnd As Long, ByVal hdcDst As Long, pptDst As Any, psize As Any, ByVal hdcSrc As Long, pptSrc As Any, crKey As Long, ByVal pblend As Long, ByVal dwFlags As Long) As Long
Declare Function FindWindow Lib "user32.dll" Alias "FindWindowA" (ByVal lpClassName As Any, ByVal lpWindowName As Any) As Long

Public Type POINTAPI
    x As Long
    y As Long
    End Type

Public Type SIZE
    cx As Long
    cy As Long
    End Type

Public Type BLENDFUNCTION
    BlendOp As Byte
    BlendFlags As Byte
    SourceConstantAlpha As Byte
    AlphaFormat As Byte
    End Type
    Public Const WS_EX_LAYERED = &H80000
    Public Const GWL_STYLE = (-16)
    Public Const GWL_EXSTYLE = (-20)
    Public Const AC_SRC_OVER = &H0
    Public Const AC_SRC_ALPHA = &H1
    Public Const AC_SRC_NO_PREMULT_ALPHA = &H1
    Public Const AC_SRC_NO_ALPHA = &H2
    Public Const AC_DST_NO_PREMULT_ALPHA = &H10
    Public Const AC_DST_NO_ALPHA = &H20
    Public Const LWA_COLORKEY = &H1
    Public Const LWA_ALPHA = &H2
    Public Const ULW_COLORKEY = &H1
    Public Const ULW_ALPHA = &H2
    Public Const ULW_OPAQUE = &H4
    Public lret As Long

Function CheckLayered(ByVal hWnd As Long) As Boolean
    lret = GetWindowLong(hWnd, GWL_EXSTYLE)
    If (lret And WS_EX_LAYERED) = WS_EX_LAYERED Then
        CheckLayered = True
    Else
        CheckLayered = False
    End If
End Function

Function SetLayered(ByVal hWnd As Long, SetAs As Boolean, bAlpha As Byte)
    lret = GetWindowLong(hWnd, GWL_EXSTYLE)
    If SetAs = True Then
        lret = lret Or WS_EX_LAYERED
    Else
        lret = lret And Not WS_EX_LAYERED
    End If
    SetWindowLong hWnd, GWL_EXSTYLE, lret
    SetLayeredWindowAttributes hWnd, 0, bAlpha, LWA_ALPHA
End Function

