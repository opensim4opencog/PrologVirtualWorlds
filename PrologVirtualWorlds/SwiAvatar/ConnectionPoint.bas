Attribute VB_Name = "ConnectionPoint"
'*********************************************************************************************
'
' Implementing source interfaces
'
' IConnectionPoint object
'
'*********************************************************************************************
'
' Author: Eduardo A. Morcillo
' E-Mail: e_morcillo@yahoo.com
' Web Page: http://www.domaindlx.com/e_morcillo
'
' Distribution: You can freely use this code in your own applications but you
'               can't publish this code in a web site, online service, or any
'               other media, without my express permission.
'
' Usage: at your own risk.
'
' Tested on: Windows 98 + VB5
'
' History:
'           06/03/2000 - This code was released
'
'*********************************************************************************************
Option Explicit

' ==== Private variables ====
Private IID_IUnknown As UUID
Private IID_IConnectionPoint As UUID

' ==== IConnectionPoint object ====

Private Type ConnectionPoint
   vtable As Long                      ' Pointer to v-table
   RefCount As Long                    ' Reference count
   Connections As Collection           ' Connections collection
   IID As UUID                         ' IID of source interface
   Parent As IConnectionPointContainer ' Parent object
End Type

' ==== API declarations ====

' Global Memory Flags
Const GMEM_FIXED = &H0
Const GMEM_ZEROINIT = &H40
Const GPTR = (GMEM_FIXED Or GMEM_ZEROINIT)

Private Declare Function GlobalAlloc Lib "kernel32" (ByVal wFlags As Long, ByVal dwBytes As Long) As Long
Private Declare Function GlobalFree Lib "kernel32" (ByVal hMem As Long) As Long
Private Declare Function GlobalHandle Lib "kernel32" (wMem As Any) As Long
Private Declare Function GlobalLock Lib "kernel32" (ByVal hMem As Long) As Long
Private Declare Function GlobalReAlloc Lib "kernel32" (ByVal hMem As Long, ByVal dwBytes As Long, ByVal wFlags As Long) As Long

Public Const E_NOTIMPL = &H80004001
Public Const E_NOINTERFACE = &H80004002

Public Declare Function VirtualProtect Lib "kernel32" (ByVal lpAddress As Long, ByVal dwSize As Long, ByVal flNewProtect As Long, lpflOldProtect As Long) As Long
Public Const PAGE_EXECUTE_READWRITE& = &H40&

Public Declare Function IsEqualGUID Lib "ole32" (rguid1 As UUID, rguid2 As UUID) As Boolean

Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Dest As Any, Src As Any, ByVal cb As Long)
Private Declare Sub ZeroMemory Lib "kernel32" Alias "RtlZeroMemory" (Dest As Any, ByVal cb As Long)

'*********************************************************************************************
'
' pvIConnectionPoint_QueryInterface
'
' Returns a pointer to the requested interface
'
'*********************************************************************************************
Private Function pvIConnectionPoint_QueryInterface(This As ConnectionPoint, riid As UUID, lObj As Long) As Long

   If IsEqualGUID(riid, IID_IUnknown) Or _
      IsEqualGUID(riid, IID_IConnectionPoint) Then

      ' Return a pointer to
      ' this object
      lObj = VarPtr(This)

      ' Increment the reference count
      This.RefCount = This.RefCount + 1

   Else

      lObj = 0

      ' Return the error
      pvIConnectionPoint_QueryInterface = E_NOINTERFACE

   End If

End Function

'*********************************************************************************************
'
' pvIConnectionPoint_AddRef
'
' Increments the IConnectionPoint reference count.
'
'*********************************************************************************************
Private Function pvIConnectionPoint_AddRef(This As ConnectionPoint) As Long

   ' Increment the reference count
   This.RefCount = This.RefCount + 1

   ' Return the reference count
   pvIConnectionPoint_AddRef = This.RefCount

End Function

'*********************************************************************************************
'
' pvIConnectionPoint_Release
'
' Decrements the object reference count
'
'*********************************************************************************************
Private Function pvIConnectionPoint_Release(This As ConnectionPoint) As Long

   With This
   
      ' Decrement the reference count
      .RefCount = This.RefCount - 1

      ' Return the reference count
      pvIConnectionPoint_Release = .RefCount

      ' Destroy the object if
      ' the reference count is 0
      If .RefCount = 0 Then
         
         ' Release the objects
         Set .Connections = Nothing
         
         ' Destroy the v-table
         GlobalFree .vtable
         
         ' Destroy the object
         GlobalFree VarPtr(This)
         
      End If
   
   End With
   
End Function

'*********************************************************************************************
'
' pvIConnectionPoint_GetConnectionInterface
'
' Returns the interface ID of the interface managed by this object.
'
'*********************************************************************************************
Private Function pvIConnectionPoint_GetConnectionInterface( _
            This As ConnectionPoint, _
            piid As UUID) As Long
            
   ' Copy the IID
   piid = This.IID
            
End Function

'*********************************************************************************************
'
' pvIConnectionPoint_GetConnectionPointContainer
'
' Returns the parent IConnectionPointContainer
'
'*********************************************************************************************
Private Function pvIConnectionPoint_GetConnectionPointContainer( _
            This As ConnectionPoint, _
            ppCPC As IConnectionPointContainer) As Long
            
   ' Return the parent
   Set ppCPC = This.Parent
            
End Function

'*********************************************************************************************
'
' pvIConnectionPoint_Advise
'
' Connects an object with the connection point object.
'
'*********************************************************************************************
Private Function pvIConnectionPoint_Advise( _
            This As ConnectionPoint, _
            ByVal pObjetPtr As ITLib.IUnknown, _
            pdwCookie As Long) As Long
Dim Sink As IUnknown

   On Error Resume Next
   
   With This
   
      ' Query for the source interface ID
      Set Sink = pObjetPtr.QueryInterface(This.IID)
     
      If Not Sink Is Nothing Then
      
         ' Add the object to the collection,
         ' using the object pointer as its key
         .Connections.Add Sink, CStr(ObjPtr(pObjetPtr))
         
         ' Return the connection key
         pdwCookie = ObjPtr(pObjetPtr)
         
      Else
      
         ' Cannot connect because the
         ' object does not support the
         ' source interface.
         pvIConnectionPoint_Advise = CONNECT_E_CANNOTCONNECT
         
      End If
      
   End With
            
End Function

'*********************************************************************************************
'
' pvIConnectionPoint_Unadvise
'
' Disconnects an object from this connection point object.
'
'*********************************************************************************************
Private Function pvIConnectionPoint_Unadvise( _
            This As ConnectionPoint, _
            ByVal dwCookie As Long) As Long
            
   On Error Resume Next
   
   With This
   
      ' Remove the object from
      ' the collection
      .Connections.Remove CStr(dwCookie)
      
      If Err.Number <> 0 Then
         pvIConnectionPoint_Unadvise = CONNECT_E_NOCONNECTION
      End If
      
   End With
            
End Function

'*********************************************************************************************
'
' pvIConnectionPoint_EnumConnections
'
' Returns a IEnumConnections object
'
'*********************************************************************************************
Private Function pvIConnectionPoint_EnumConnections( _
            This As ConnectionPoint, _
            ppEnum As IEnumConnections) As Long
   
   ' Not implemented because VB
   ' doesn't call this method.
   
   pvIConnectionPoint_EnumConnections = E_NOTIMPL
   
End Function
    
'*********************************************************************************************
'
' AddrOf
'
' Returns the Addr parameters. Used to pass
' function pointers to variables.
'
'*********************************************************************************************
Private Function AddrOf(ByVal Addr As Long) As Long
   AddrOf = Addr
End Function

'*********************************************************************************************
'
' CreateConnectionPoint
'
' Creates a IConnectionPoint object.
'
' Parameters:
'
' Parent - Parent IConnectionPointContainer
' IID    - IID of the source interface
'
'*********************************************************************************************
Public Function CreateConnectionPoint(ByVal Parent As IConnectionPointContainer, IID As UUID) As stdole.IUnknown
Dim vtable(0 To 7) As Long
Dim TmpCP As ConnectionPoint
Dim CPPtr As Long

   ' Initialize IDs
   With IID_IUnknown
      .Data4(0) = &HC0
      .Data4(7) = &H46
   End With

   With IID_IConnectionPoint
      .Data1 = &HB196B286
      .Data2 = &HBAB4
      .Data3 = &H101A
      .Data4(0) = &HB6
      .Data4(1) = &H9C
      .Data4(2) = &H0
      .Data4(3) = &HAA
      .Data4(4) = &H0
      .Data4(5) = &H34
      .Data4(6) = &H1D
      .Data4(7) = &H7
   End With

   ' Initialize the vtable
   vtable(0) = AddrOf(AddressOf pvIConnectionPoint_QueryInterface)
   vtable(1) = AddrOf(AddressOf pvIConnectionPoint_AddRef)
   vtable(2) = AddrOf(AddressOf pvIConnectionPoint_Release)
   vtable(3) = AddrOf(AddressOf pvIConnectionPoint_GetConnectionInterface)
   vtable(4) = AddrOf(AddressOf pvIConnectionPoint_GetConnectionPointContainer)
   vtable(5) = AddrOf(AddressOf pvIConnectionPoint_Advise)
   vtable(6) = AddrOf(AddressOf pvIConnectionPoint_Unadvise)
   vtable(7) = AddrOf(AddressOf pvIConnectionPoint_EnumConnections)
   
   ' Fill a temporary
   ' ConnectionPoint structure
   With TmpCP
      
      ' Create a new collection
      Set .Connections = New Collection
      
      ' Copy the source interface ID
      .IID = IID
      
      ' Set the parent
      Set .Parent = Parent
      
      ' Initialize the reference count
      .RefCount = 1
      
      ' Allocate memory for the v-table
      .vtable = GlobalAlloc(GPTR, 32)
      
      ' Copy the v-table to the pointer
      CopyMemory ByVal .vtable, vtable(0), 32
      
   End With
   
   ' Allocate memory for the object
   CPPtr = GlobalAlloc(GPTR, LenB(TmpCP))
   
   ' Copy the structure to the pointer
   CopyMemory ByVal CPPtr, TmpCP, LenB(TmpCP)
   
   ' Release the collection without
   ' decrementing its reference count
   CopyMemory TmpCP.Connections, 0&, 4
   
   ' Copy the pointer to the return value
   CopyMemory CreateConnectionPoint, CPPtr, 4
   
End Function

'*********************************************************************************************
'
' RaiseEvents
'
' Raises an event from the given connection point.
'
'*********************************************************************************************
Public Sub RaiseEvents(ByVal oCP As stdole.IUnknown, ByVal EventName As Variant, ParamArray Params() As Variant)
Dim CP As ConnectionPoint
Dim vSink As Variant
Dim Idx As Long
Dim Args() As Variant
   
   ' Get the struct from the object
   CopyMemory CP, ByVal ObjPtr(oCP), Len(CP)
   
   If Not IsMissing(Params) Then
      
      ' Copy the arguments to an array
      ReDim Args(0 To UBound(Params))
      
      For Idx = 0 To UBound(Params)
         If IsObject(Params(Idx)) Then
            Set Args(Idx) = Params(Idx)
         Else
            Args(Idx) = Params(Idx)
         End If
      Next
      
   End If
   
   ' Call the event for each
   ' connected object
   For Each vSink In CP.Connections
      pvInvoke vSink, EventName, Args
   Next
   
   ' Clear the structure without
   ' releasing anything on it.
   ZeroMemory CP, LenB(CP)
   
End Sub
'*********************************************************************************************
'
' pvInvoke
'
' Calls the event function.
'
' Parameters:
'
' oDispatch          - Event sink object
' MemberNameOrDISPID - Event name or DISPID
' Args               - Array of arguments
'
'*********************************************************************************************
Private Sub pvInvoke( _
      ByVal oDispatch As IVBDispatch, _
      ByVal MemberNameOrDISPID As Variant, _
      Args As Variant)

Dim IID_NULL As UUID                 ' NULL interface ID
Dim lDISPID As Long                  ' Dispatch ID
Dim DISPPARAMS As ITLib.DISPPARAMS   ' Parameters UDT
Dim EXCEPINFO As ITLib.EXCEPINFO     ' Exception Error info
Dim lArgErr As Long                  ' Argument that produced the error
Dim vResult As Variant               ' vResult value
Dim aParams() As Variant             ' Parameters array
Dim aNamedParams() As Long           ' Named parameters array
Dim lResult As Long                  ' pvInvoke return value
Dim lIdx As Long, lMax As Long       ' Index and number of parameters
   
   On Error Resume Next
   
   ' If MemberNameOrDISPID is numeric
   ' it represents the lDISPID of the
   ' member otherwise get the lDISPID
   ' of the given member name
   If IsNumeric(MemberNameOrDISPID) Then
      lDISPID = MemberNameOrDISPID
   Else
      MsgBox "Event names are not supported.", vbInformation, "ConnectionPoint"
      Exit Sub
   End If
        
   ' Get parameters count
   lMax = UBound(Args)
   
   ' Check if there are arguments
   If Err.Number = 0 Then
      
      ' Redim the parameters array
      ReDim aParams(0 To lMax)
            
      ' Fill parameters arrays. The array has
      ' to be filled in reseversed order
      ' so the first element is the last
      ' parameter.
            
      For lIdx = 0 To lMax
         If IsObject(Args(lIdx)) Then
            Set aParams(lMax - lIdx) = Args(lIdx)
         Else
            aParams(lMax - lIdx) = Args(lIdx)
         End If
      Next
            
      ' Fill the DISPPARAMS structure
      With DISPPARAMS
         .cArgs = UBound(aParams) + 1
         .rgPointerToVariantArray = VarPtr(aParams(0))
      End With
      
   End If
   
   ' pvInvoke method/property
   lResult = oDispatch.Invoke(lDISPID, IID_NULL, 0, INVOKE_FUNC, DISPPARAMS, VarPtr(vResult), EXCEPINFO, lArgErr)
   
   On Error GoTo 0
   
   If lResult <> 0 Then
   
      ' There was an error
        
      If lResult = DISP_E_EXCEPTION Then
                
         Err.Raise EXCEPINFO.wCode
            
      Else
                
         Err.Raise lResult
            
      End If
                
   End If
       
End Sub

'*********************************************************************************************
'
' pvFindConnectionPoint
'
' Returns a connection point for the requested source interface. This function
' replaces the VB implementation.
'
'*********************************************************************************************
Private Function pvFindConnectionPoint(ByVal This As IConnectionPointContainer, riid As UUID, oCP As IConnectionPoint) As Long
Dim oCallback As ICPCCallback

   ' Get the callback interface
   Set oCallback = This
   
   ' Call back to the class
   pvFindConnectionPoint = oCallback.FindConnectionPoint(riid, oCP)
   
   ' Release the callback object
   Set oCallback = Nothing
   
End Function

'*********************************************************************************************
'
' pvEnumConnectionPoints
'
' Returns a new IEnumConnectionPoints object. This function replaces the
' VB implementation.
'
'*********************************************************************************************
Private Function pvEnumConnectionPoints(ByVal This As IConnectionPointContainer, EnumPtr As Long) As Long

   ' This method is not implemented because
   ' VB does not call it.
   
   pvEnumConnectionPoints = E_NOTIMPL
   
End Function

'*********************************************************************************************
'
' ReplaceCPC
'
' Replaces the IConnectionPointContainer methods
'
'*********************************************************************************************
Public Sub ReplaceCPC(ByVal oObj As IConnectionPointContainer)
Dim oCallback As ICPCCallback
Dim oEnm As IEnumConnectionPoints
Dim oCP As IConnectionPoint
Dim lFindPtr As Long
Dim lEnumPtr As Long
   
   ' Get the callback interface
   Set oCallback = oObj
   
   On Error Resume Next
   
   ' Get connection point enumeration
   Set oEnm = oObj.EnumConnectionPoints
   
   If Not oEnm Is Nothing Then
      ' Get the first connection point
      oEnm.Next 1, oCP
   End If
   
   ' Replace the functions of ICPC
   lEnumPtr = pvReplaceVTableEntry(ObjPtr(oObj), 4, AddressOf pvEnumConnectionPoints)
   lFindPtr = pvReplaceVTableEntry(ObjPtr(oObj), 5, AddressOf pvFindConnectionPoint)
      
   ' Save the data
   oCallback.SavePointers oCP, lFindPtr, lEnumPtr
   
End Sub

'*********************************************************************************************
'
' RestoreCPC
'
' Restores the original IConnectionPointContainer methods
'
'*********************************************************************************************
Public Sub RestoreCPC(ByVal oObj As IConnectionPointContainer)
Dim oCallback As ICPCCallback
Dim oCP As IConnectionPoint
Dim lFindPtr As Long
Dim lEnumPtr As Long

   ' Get the callback interface
   Set oCallback = oObj
   
   ' Get the original pointers
   oCallback.GetPointers lFindPtr, lEnumPtr
   
   ' Replace the functions of ICPC
   pvReplaceVTableEntry ObjPtr(oObj), 4, lEnumPtr
   pvReplaceVTableEntry ObjPtr(oObj), 5, lFindPtr
         
End Sub

'*********************************************************************************************
'
' pvReplaceVTableEntry
'
' Replaces an entry in a object v-table. From Hardcore VB book.
'
'*********************************************************************************************
Private Function pvReplaceVTableEntry(ByVal oObject As Long, ByVal nEntry As Integer, ByVal pFunc As Long) As Long
Dim pFuncOld As Long, pVTableHead As Long
Dim pFuncTmp As Long, lOldProtect As Long
     
    ' Object pointer contains a pointer to v-table--copy it to temporary
    ' pVTableHead = *oObject;
    CopyMemory pVTableHead, ByVal oObject, 4
    
    ' Calculate pointer to specified entry
    pFuncTmp = pVTableHead + (nEntry - 1) * 4
    
    ' Save address of previous method for return
    ' pFuncOld = *pFuncTmp;
    CopyMemory pFuncOld, ByVal pFuncTmp, 4
    
    ' Ignore if they're already the same
    If pFuncOld <> pFunc Then
        ' Need to change page protection to write to code
        VirtualProtect pFuncTmp, 4, PAGE_EXECUTE_READWRITE, lOldProtect
        
        ' Write the new function address into the v-table
        CopyMemory ByVal pFuncTmp, pFunc, 4     ' *pFuncTmp = pfunc;
        
        ' Restore the previous page protection
        VirtualProtect pFuncTmp, 4, lOldProtect, lOldProtect 'Optional
        
    End If
    
    'return address of original proc
    pvReplaceVTableEntry = pFuncOld
    
End Function




