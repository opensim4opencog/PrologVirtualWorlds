Attribute VB_Name = "modSettings"
Public Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpString As Any, ByVal lpFileName As String) As Long
Public Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long
Public Const CB_FINDSTRING = &H14C
Public Const CB_FINDSTRINGEXACT = &H158


Global INI As String
Global winINI As String


'* Server settings
Global strServer    As String
Global strMyNick    As String
Global strOtherNick As String
Global strFullName  As String
Global strMyIdent   As String
Global lngPort      As Long
Global bConOnLoad   As Boolean
Global bReconnect   As Boolean
Global bInvisible   As Boolean
Global bRetry       As Boolean
Global intRetry     As Integer

'* Font/colors...
Global strFont As String
Global strFontSize As Integer
Global lngBackColor As Long
Global lngForeColor As Long
Global lngLeftColor As Long
Global lngRightColor As Long
Global strFontName As String
Global intFontSize As Integer

'* Windows/interface
Global bStretchButtons  As Boolean
Global intButtonWidth   As Integer
Global intIface         As Integer
Global Const IFT_SIMPLE = 1
Global Const IFT_FANCY = 2

'* IRC Settings
Global bWhoisInQuery    As Boolean
Global MAX_TEXT_HISTORY As Integer
Global bRejoinOnKick    As Boolean
Global bHidePing        As Boolean

'* Misc. variables
Global bAcceptDCC As Boolean
Global strDCCNick   As String
Global strDCCFile   As String
Global lngDCCSize   As Long
Global lngDCCStart  As Long
Global lngAccept    As Long
Function GetINI(strINI As String, strSection As String, strSetting As String, strDefault As String)
    Dim lngReturn As Long, strReturn As String, lngSize As Long
    lngSize = 255
    strReturn = String(lngSize, 0)
    lngReturn = GetPrivateProfileString(strSection, strSetting, strDefault, strReturn, lngSize, winINI)
    If strReturn = "" Then
        GetINI = strDefault
        PutINI strINI, strSection, strSetting, strDefault
    Else
        GetINI = LeftOf(strReturn, Chr(0))
    End If

End Function

Sub PutINI(strINI As String, strSection As String, strLValue As String, strRValue As String)
    Dim lngReturn As Long
    lngReturn = WritePrivateProfileString(strSection, strLValue, strRValue, strINI)
    'MsgBox lngReturn & "..ini"
End Sub

Public Function TF(bVal As Boolean) As Integer
    If bVal Then
        TF = 1
    Else
        TF = 0
    End If
End Function

Function FileExists(FullFileName As String) As Boolean
    On Error GoTo MakeF
        'If file does Not exist, there will be an Error
        Open FullFileName For Input As #1
        Close #1
        'no error, file exists
        FileExists = True
    Exit Function
MakeF:
        'error, file does Not exist
        FileExists = False
    Exit Function
End Function
Function ReadINI(strSection As String, strSetting As String, strDefault As String)
    Dim lngReturn As Long, strReturn As String, lngSize As Long
    lngSize = 255
    strReturn = String(lngSize, 0)
    lngReturn = GetPrivateProfileString(strSection, strSetting, strDefault, strReturn, lngSize, path & "settings.ini")
    If strReturn = "" Then
        ReadINI = strDefault
        WriteINI strSection, strSetting, strDefault
    Else
        ReadINI = LeftOf(strReturn, Chr(0))
    End If
End Function



Sub WriteINI(strSection As String, strLValue As String, strRValue As String)
    Dim lngReturn As Long
    lngReturn = WritePrivateProfileString(strSection, strLValue, strRValue, path & "settings.ini")
    'MsgBox lngReturn & "..ini"
End Sub


