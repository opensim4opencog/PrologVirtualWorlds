Attribute VB_Name = "APICalls"
Option Explicit

        Public Const HKEY_CLASSES_ROOT = &H80000000
        Public Const HKEY_CURRENT_USER = &H80000001
        Public Const HKEY_LOCAL_MACHINE = &H80000002
        Public Const HKEY_USERS = &H80000003
        Public Const KEY_ALL_ACCESS = &H3F
        Public Const REG_OPTION_NON_VOLATILE = 0&
        Public Const REG_CREATED_NEW_KEY = &H1
        Public Const REG_OPENED_EXISTING_KEY = &H2
        Public Const ERROR_SUCCESS = 0&
        Public Const REG_SZ = (1)

Type SECURITY_ATTRIBUTES
    nLength As Long
    lpSecurityDescriptor As Long
    bInheritHandle As Boolean
    End Type


Declare Function RegEnumValue Lib "advapi32.dll" Alias "RegEnumValueA" (ByVal hKey As Long, ByVal dwIndex As Long, ByVal lpValueName As String, lpcbValueName As Long, ByVal lpReserved As Long, lpType As Long, ByVal lpData As String, lpcbData As Long) As Long


Declare Function RegOpenKeyEx Lib "advapi32" Alias "RegOpenKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal ulOptions As Long, ByVal samDesired As Long, phkResult As Long) As Long


Declare Function RegSetValueEx Lib "advapi32" Alias "RegSetValueExA" (ByVal hKey As Long, ByVal lpValueName As String, ByVal Reserved As Long, ByVal dwType As Long, ByVal szData As String, ByVal cbData As Long) As Long


Declare Function RegCloseKey Lib "advapi32" (ByVal hKey As Long) As Long


Declare Function RegCreateKeyEx Lib "advapi32" Alias "RegCreateKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal Reserved As Long, ByVal lpClass As String, ByVal dwOptions As Long, ByVal samDesired As Long, lpSecurityAttributes As SECURITY_ATTRIBUTES, phkResult As Long, lpdwDisposition As Long) As Long

Declare Function VarPtrArray Lib "msvbvm60.dll" Alias "VarPtr" (Ptr() As Any) As Long

Declare Function GetModuleUsage% Lib "KERNEL" (ByVal hModule%)
Public Declare Function LoadResource Lib "kernel32" (ByVal hMorphism As Long, ByVal hResInfo As Long) As Long
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Public Declare Function WaitForSingleObject Lib "kernel32" (ByValhHandle As Long, ByVal dwMilliseconds As Long) As Long

#If Win32 Then
   Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" ( _
      hpvDest As Any, hpvSource As Any, ByVal cbCopy As Long)
#Else
   Declare Sub CopyMemory Lib "KERNEL" Alias "hmemcpy" ( _
      hpvDest As Any, hpvSource As Any, ByVal cbCopy As Long)
#End If

'Public Declare Function StringFromGUID2 Lib "ole32.dll" (rguid As Any, ByVal lpstrClsId As Long, ByVal cbMax As Integer) As Long
Public Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hWnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Any) As Long
Public Declare Function OSWinHelp% Lib "user32" Alias "WinHelpA" (ByVal hWnd&, ByVal HelpFile$, ByVal wCommand%, dwData As Any)

Public Declare Function CreateProcessA Lib "kernel32" (ByVal _
      lpApplicationName As Long, ByVal lpCommandLine As String, ByVal _
      lpProcessAttributes As Long, ByVal lpThreadAttributes As Long, _
      ByVal bInheritHandles As Long, ByVal dwCreationFlags As Long, _
      ByVal lpEnvironment As Long, ByVal lpCurrentDirectory As Long, _
      lpStartupInfo As STARTUPINFO, lpProcessInformation As _
      PROCESS_INFORMATION) As Long

Public Declare Function CloseHandle Lib "kernel32" _
      (ByVal hObject As Long) As Long

Public Declare Function GetExitCodeProcess Lib "kernel32" _
      (ByVal hProcess As Long, lpExitCode As Long) As Long
Declare Function SetWindowPos Lib "user32" (ByVal hWnd As Long, ByVal hWndInsertAfter As Long, ByVal x As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long

Declare Function GetCaretPos Lib "user32" (lpPoint As POINTAPI) As Long


      Private Declare Function CLSIDFromProgID _
         Lib "ole32.dll" (ByVal lpszProgID As Long, _
         pCLSID As GUID) As Long

      Private Declare Function StringFromCLSID _
         Lib "ole32.dll" (pCLSID As GUID, lpszProgID As Long) As Long

      Private Declare Function CLSIDFromString _
         Lib "ole32.dll" (ByVal lpszProgID As Long, _
         pCLSID As GUID) As Long

    


      '====================

      Public Declare Function SetWindowLong _
          Lib "user32" Alias "SetWindowLongA" _
            (ByVal hWnd As Long, _
            ByVal nIndex As Long, _
            ByVal dwNewLong As Long) As Long

      Public Declare Function GetWindowLong Lib "user32" Alias "GetWindowLongA" (ByVal hWnd As Long, ByVal nIndex As Long) As Long

      Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hWnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long

      'Constants for GetWindowLong() and SetWindowLong() APIs.
      Public Const GWL_WNDPROC = (-4)
      Public Const GWL_USERDATA = (-21)



    Private Type STARTUPINFO
        cb As Long
        lpReserved As String

        lpDesktop As String
        lpTitle As String
        dwX As Long
        dwY As Long
        dwXSize As Long
        dwYSize As Long
        dwXCountChars As Long
        dwYCountChars As Long
        dwFillAttribute As Long
        dwFlags As Long
        wShowWindow As Integer
        cbReserved2 As Integer
        lpReserved2 As Long
        hStdInput As Long
        hStdOutput As Long
        hStdError As Long
     End Type

    Private Type PROCESS_INFORMATION
        hProcess As Long

        hThread As Long
        dwProcessID As Long
        dwThreadID As Long
    End Type

    
    Private Const NORMAL_PRIORITY_CLASS = &H20&
    Private Const INFINITE = -1&

    
    
   ' This is the main API used to display the Insert Obj DlgBox
   Private Declare Function OleUIInsertObject Lib "oledlg.dll" _
   Alias "OleUIInsertObjectA" (inParam As Any) As Long

   ' This is used to get the ProgID from Class ID.
   ' Note that this API need us to pass LPOLESTR * from Visual Basic.
   Private Declare Function ProgIDFromCLSID Lib "ole32.dll" _
   (clsid As Any, strAddess As Long) As Long

   ' The memory allocated OLE way need to be released OLE way
   ' with this API.
   Private Declare Sub CoTaskMemFree Lib "ole32.dll" (ByVal pvoid As Long)

   ' Widely used CopyMemory API.


   ' Note that you need to get the strlength of the UNICODE string.
   Private Declare Function lstrlenW Lib "kernel32" _
   (ByVal lpString As Long) As Long

   ' Constants used in the dwFlags of OleUIInsertObjectType.
   Const IOF_SHOWHELP = &H1
   Const IOF_SELECTCREATENEW = &H2
   Const IOF_SELECTCREATEFROMFILE = &H4
   Const IOF_CHECKLINK = &H8
   Const IOF_CHECKDISPLAYASICON = &H10
   Const IOF_CREATENEWOBJECT = &H20
   Const IOF_CREATEFILEOBJECT = &H40
   Const IOF_CREATELINKOBJECT = &H80
   Const IOF_DISABLELINK = &H100
   Const IOF_VERIFYSERVERSEXIST = &H200
   Const IOF_DISABLEDISPLAYASICON = &H400
   Const IOF_HIDECHANGEICON = &H800
   Const IOF_SHOWINSERTCONTROL = &H1000
   Const IOF_SELECTCREATECONTROL = &H2000

   ' Return codes from OleUIInsertObject
   Const OLEUI_FALSE = 0
   Const OLEUI_SUCCESS = 1               ' No error, same as OLEUI_OK.
   Const OLEUI_OK = 1                    ' OK button pressed.
   Const OLEUI_CANCEL = 2                ' Cancel button pressed.

   ' GUID, IID, CLSID, etc
   Private Type GUID
       Data1 As Long
       Data2 As Integer
       Data3 As Integer
       Data4(0 To 7) As Byte
   End Type

   ' Main UDT used in OleUIInsertObject.
   Private Type OleUIInsertObjectType
   ' These IN fields are standard across all OLEUI dialog box functions.
       cbStruct As Long
       dwFlags As Long
       hWndOwner As Long
       lpszCaption  As String    ' LPCSTR
       lpfnHook As Long          ' LPFNOLEUIHOOK
       lCustData As Long         ' LPARAM
       hMorphism  As Long
       lpszTemplate As String    ' LPCSTR
       hResource As Long         ' HRSRC
       clsid As GUID
 
   ' Specifics for OLEUIINSERTOBJECT.
       lpszFile As String        ' LPTSTR
       cchFile As Long
       cClsidExclude As Long
       lpClsidExclude As Long    ' LPCLSID
       IID As GUID
 
   ' Specifics to create objects if flags say so.
       oleRender As Long
       lpFormatEtc As Long       ' LPFORMATETC
       lpIOleClientSite As Long  ' LPOLECLIENTSITE
       lpIStorage As Long        ' LPSTORAGE
       ppvObj As Long            ' LPVOID FAR *
       sc As Long                ' SCODE
       hMetaPict As Long         ' HGLOBAL
   End Type

   

Private Sub button1_click()
    Dim x%, Z%
   x% = Shell("notepad.exe")
      While GetModuleUsage(x%) > 0
' Has program finished?
   Z% = DoEvents()
' If not, yield to Windows.
   Wend
End Sub



Public Sub StringFromPointer(pOLESTR As Long, strOut As String)
   Dim ByteArray(255) As Byte
   Dim intTemp As Integer
   Dim intCount As Integer
   Dim i As Integer

   intTemp = 1

   'Walk the string and retrieve the first byte of each WORD.
   While intTemp <> 0
      CopyMemory intTemp, ByVal pOLESTR + i, 2
      ByteArray(intCount) = intTemp
      intCount = intCount + 1
      i = i + 2
   Wend

   'Copy the byte array to our string.
   CopyMemory ByVal strOut, ByteArray(0), intCount
End Sub

    Public Sub ExecCmd(cmdline$)
        Dim ret&
        Dim proc As PROCESS_INFORMATION
        Dim start As STARTUPINFO

        start.cb = Len(start)

        ' Shell the Application:
        ret& = CreateProcessA(0&, cmdline$, 0&, 0&, 1&, NORMAL_PRIORITY_CLASS, 0&, 0&, start, proc)

        ' Wait for the shelled application to finish:
        ret& = WaitForSingleObject(proc.hProcess, INFINITE)
        ret& = CloseHandle(proc.hProcess)
    End Sub
    






Public Function bSetRegValue(ByVal hKey As Long, ByVal lpszSubKey As String, ByVal sSetValue As String, ByVal sValue As String) As Boolean
    On Error Resume Next
    Dim phkResult As Long
    Dim lResult As Long
    Dim SA As SECURITY_ATTRIBUTES
    Dim lCreate As Long
    RegCreateKeyEx hKey, lpszSubKey, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, SA, phkResult, lCreate
    lResult = RegSetValueEx(phkResult, sSetValue, 0, REG_SZ, sValue, CLng(Len(sValue) + 1))
    RegCloseKey phkResult
    bSetRegValue = (lResult = ERROR_SUCCESS)
End Function


Public Function bGetRegValue(ByVal hKey As Long, ByVal sKey As String, ByVal sSubKey As String) As String
    Dim lResult As Long
    Dim phkResult As Long
    Dim dWReserved As Long
    Dim szBuffer As String
    Dim lBuffSize As Long
    Dim szBuffer2 As String
    Dim lBuffSize2 As Long
    Dim lIndex As Long
    Dim ltype As Long, bfound
    Dim sCompKey As String
    lIndex = 0
    lResult = RegOpenKeyEx(hKey, sKey, 0, 1, phkResult)


    Do While lResult = ERROR_SUCCESS And Not (bfound)
        szBuffer = Space(255)
        lBuffSize = Len(szBuffer)
        szBuffer2 = Space(255)
        lBuffSize2 = Len(szBuffer2)
        lResult = RegEnumValue(phkResult, lIndex, szBuffer, lBuffSize, dWReserved, ltype, szBuffer2, lBuffSize2)


        If (lResult = ERROR_SUCCESS) Then
            sCompKey = Left(szBuffer, lBuffSize)


            If (sCompKey = sSubKey) Then
                bGetRegValue = Left(szBuffer2, lBuffSize2 - 1)
            End If
        End If
        lIndex = lIndex + 1
    Loop
    RegCloseKey phkResult
End Function

